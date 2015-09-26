/*
LUFA Library
Copyright (C) Dean Camera, 2014.

dean [at] fourwalledcubicle [dot] com
www.lufa-lib.org
*/

/*
Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

Permission to use, copy, modify, distribute, and sell this
software and its documentation for any purpose is hereby granted
without fee, provided that the above copyright notice appear in
all copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/

/*
Copyright(c) 2014-2015 NicoHood
See the readme for credit to other people.

This file is part of Hoodloader2.

Hoodloader2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Hoodloader2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Hoodloader2.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file
*
*  Main source file for the CDC class bootloader. This file contains the complete bootloader logic.
*/

#define  INCLUDE_FROM_BOOTLOADERCDC_C
#include "HoodLoader2.h"

/** Contains the current baud rate and other settings of the first virtual serial port. This must be retained as some
*  operating systems will not open the port unless the settings can be set successfully.
*/
static CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 0,
	.CharFormat = CDC_LINEENCODING_OneStopBit,
	.ParityType = CDC_PARITY_None,
	.DataBits = 8
};


/* NOTE: Using Linker Magic,
* - Reserved 256 bytes from start of RAM at 0x100 for UART RX Buffer
* so we can use 256-byte aligned addresssing.
* - Also 128 bytes from 0x200 for UART TX buffer, same addressing.
* normal RAM data starts at 0x280, see offset in makefile*/

#define USART2USB_BUFLEN 256 // 0xFF - 8bit
#define USB2USART_BUFLEN 128 // 0x7F - 7bit

// USB-Serial buffer pointers are saved in GPIORn
// for better access (e.g. cbi) in ISRs
// This has nothing to do with r0 and r1!
// GPIORn – General Purpose I/O Register are located in RAM.
// Make sure to set DEVICE_STATE_AS_GPIOR to 2 in the Lufa config.
// They are initialied in the CDC LineEncoding Event
#define USBtoUSART_ReadPtr GPIOR0 // to use cbi()
#define USARTtoUSB_WritePtr GPIOR1

/* USBtoUSART_WritePtr needs to be visible to ISR. */
/* USARTtoUSB_ReadPtr needs to be visible to CDC LineEncoding Event. */
static volatile uint8_t USBtoUSART_WritePtr = 0;
static volatile uint8_t USARTtoUSB_ReadPtr = 0;

// Variable to save how many bytes are laying in the USB TX bank if in bootloader mode
register uint8_t bankTX asm("r6");
//static uint8_t bankTX = 0;

/** Current address counter. This stores the current address of the FLASH or EEPROM as set by the host,
*  and is used when reading or writing to the AVRs memory (either FLASH or EEPROM depending on the issued
*  command.)
*/
static uint32_t CurrAddress;

/** Flag to indicate if the bootloader should be running, or should exit and allow the application code to run
*  via a watchdog reset. When cleared the bootloader will exit, starting the watchdog and entering an infinite
*  loop until the AVR restarts and the application runs.
*/
static bool RunBootloader = true;

// Variable to determine if CDC baudrate is for the bootloader mode, USB-Serial or if CDC is turned off
#define MODE_OFF 0
#define MODE_BOOTLOADER 1
#define MODE_USBSERIAL 2
static uint8_t mode = MODE_OFF;

// MAH 8/15/12- let's make this an 8-bit value instead of 16- that saves on memory because 16-bit addition and
//  comparison compiles to bulkier code. Note that this does *not* require a change to the Arduino core- we're
//  just sort of ignoring the extra byte that the Arduino core puts at the next location.
// ensure the address isnt used anywhere else by adding a compiler flag in the makefile ld flag
// 0x280 is reserved by the makefile (right after out buffers) and 0x8000 should not be used anyways
// To avoid problems inside the sketch a newer methode with RAMEND was introduced with HL2.0.5
// for the u2 series only (to save flash with the 32u4)
volatile uint8_t *const MagicBootKeyPtr = (volatile uint8_t *)BOOTKEY;

// Backwardscompatibility with old Bootkey (adds 20 bytes of flash)
// Undef to save flash for 32u4 devices
// 2nd Bootkey backwards compatibility is normally not needed, wanted and useful
// If you also change the programming speed.
// You may only want to use this if you want to stick to the old HID Project 2.2 software with baud 57600.
#if (BAUDRATE_CDC_BOOTLOADER != 57600)
#undef SECOND_BOOTKEY
#elif defined(__AVR_ATmega32U4__)
volatile uint8_t *const SecondMagicBootKeyPtr = (volatile uint8_t *)0x8000;
#else
volatile uint8_t *const SecondMagicBootKeyPtr = (volatile uint8_t *)SECOND_BOOTKEY;
#endif

/** Magic bootloader key to unlock forced application start mode. */
// Arduino uses a 16bit value but we use a 8 bit value to keep the size low, see above
#define MAGIC_BOOT_KEY               (uint8_t)0x7777

// Bootloader timeout timer in ms
#define EXT_RESET_TIMEOUT_PERIOD 750

/** Special startup routine to check if the bootloader was started via a watchdog reset, and if the magic application
*  start key has been loaded into \ref MagicBootKey. If the bootloader started via the watchdog and the key is valid,
*  this will force the user application to start via a software jump.
*/
void Application_Jump_Check(void)
{
	// Save the value of the boot key memory before it is overwritten
	uint8_t bootKeyPtrVal = *MagicBootKeyPtr;
	*MagicBootKeyPtr = 0;
#ifdef SECOND_BOOTKEY
	uint8_t secondBootKeyPtrVal = *SecondMagicBootKeyPtr;
	*SecondMagicBootKeyPtr = 0;
#endif

	// Check the reason for the reset so we can act accordingly
	uint8_t  mcusr_state = MCUSR;		// store the initial state of the Status register
	MCUSR = 0;							// clear all reset flags

	/* Disable watchdog if enabled by bootloader/fuses */
	wdt_disable();

	// Turn off leds on every startup
	LEDs_TurnOffTXLED;
	LEDs_TurnOffRXLED;

	// check if a sketch is presented and what to do then
	if (pgm_read_word(0) != 0xFFFF){

		// First case: external reset, bootKey NOT in memory. We'll put the bootKey in memory, then spin
		//  our wheels for about 750ms, then proceed to the sketch, if there is one. If, during that 750ms,
		//  another external reset occurs, on the next pass through this decision tree, execution will fall
		//  through to the bootloader.
		if ((mcusr_state & (1 << EXTRF))) {
			if ((bootKeyPtrVal != MAGIC_BOOT_KEY)
#ifdef SECOND_BOOTKEY
&& (secondBootKeyPtrVal != MAGIC_BOOT_KEY)
#endif
){
				// set the Bootkey and give the user a few ms chance to enter Bootloader mode
				*MagicBootKeyPtr = MAGIC_BOOT_KEY;
#ifdef SECOND_BOOTKEY
				*SecondMagicBootKeyPtr = MAGIC_BOOT_KEY;
#endif

				// wait for a possible double tab (this methode takes less flash than an ISR)
				_delay_ms(EXT_RESET_TIMEOUT_PERIOD);

				// user was too slow/normal reset, start sketch now
				*MagicBootKeyPtr = 0;
#ifdef SECOND_BOOTKEY
				*SecondMagicBootKeyPtr = 0;
#endif
				// Single rab reset, start sketch
				if(DOUBLE_TAB_RESET_TO_BOOTLOADER)
					StartSketch();
			}
			// Double tab reset, start sketch only if you prefer the bootloader as single tab
			else if(!DOUBLE_TAB_RESET_TO_BOOTLOADER){
				StartSketch();
			}
		}

		// On a power-on reset, we ALWAYS want to go to the sketch. If there is one.
		else if ((mcusr_state & (1 << PORF))){
			if(!POWER_ON_TO_BOOTLOADER){
				StartSketch();
			}
		}

		// On a watchdog reset, if the bootKey isn't set, and there's a sketch, we should just
		//  go straight to the sketch.
		else if ((mcusr_state & (1 << WDRF))){
		    if((bootKeyPtrVal != MAGIC_BOOT_KEY)
#ifdef SECOND_BOOTKEY
            && (secondBootKeyPtrVal != MAGIC_BOOT_KEY)
#endif
            ){
				// If it looks like an "accidental" watchdog reset then start the sketch.
				StartSketch();
		    }
		    // Bootkey present
		    else{
		        // Start HoodLoader2
		    }
		}
	}
}

static void StartSketch(void)
{
	// jump to beginning of application space
	// cppcheck-suppress constStatement
	((void(*)(void))0x0000)();
}

static void ResetMCU(void){
	/* Wait a short time to end all USB transactions and then disconnect */
	_delay_us(1000);

	/* Disconnect from the host - USB interface will be reset later along with the AVR */
	USB_Detach();

	/* Enable the watchdog and force a timeout to reset the AVR */
	// this is the simplest solution since it will clear all the hardware setups
	wdt_enable(WDTO_250MS);
	while(true);
}


/** Main program entry point. This routine configures the hardware required by the bootloader, then continuously
*  runs the bootloader processing routine until instructed to soft-exit, or hard-reset via the watchdog to start
*  the loaded application code.
*/
int main(void)
{
	/* Setup hardware required for the bootloader */
	SetupHardware();

	/* Enable global interrupts so that the USB stack can function */
	GlobalInterruptEnable();

	while(true) {
		// Pulse generation counters to keep track of the number of milliseconds remaining for each pulse type
		#define TX_RX_LED_PULSE_MS 3
		uint8_t TxLEDPulse = 0;
		uint8_t RxLEDPulse = 0;

		// USB-Serial main loop
		while(true) {
			// USB Task
			uint8_t lastState = USB_DeviceState;
			Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
			if (Endpoint_IsSETUPReceived())
			USB_Device_ProcessControlRequest();

			// Compare last with new state
			uint8_t newState = USB_DeviceState;
			if(newState != DEVICE_STATE_Configured){
				// Try to reconnect if communication is still broken
				if(lastState != DEVICE_STATE_Configured)
					continue;
				// Break and disable USART on connection lost
				else
					break;
			}

			// Finished self reprogramming?
			// Little note: on u2 series moving this upwards saves 2 bytes of flash
			if(!RunBootloader){
				ResetMCU();
			}

			/* Check if endpoint has a command in it sent from the host */
			Endpoint_SelectEndpoint(CDC_RX_EPADDR);
			uint8_t countRX = 0;

			if (Endpoint_IsOUTReceived()){
				// Check if we received any new bytes and if we still have space in the buffer
				countRX = Endpoint_BytesInEndpoint();

				// Acknowledge zero length packet and dont call any read functions
				if (!countRX)
					Endpoint_ClearOUT();
			}

			if(mode == MODE_BOOTLOADER){
				if(countRX){
					Bootloader_Task();
				}
			}

			// Skip USB-Serial translation if CDC Serial is not configured/disabled
			else if(mode == MODE_USBSERIAL)
			{
				//================================================================================
				// USBtoUSART
				//================================================================================

				// Check how much free space the USBtoUSART buffer has
				uint8_t USBtoUSART_free = (USB2USART_BUFLEN-1) - ( (USBtoUSART_WritePtr - USBtoUSART_ReadPtr) & (USB2USART_BUFLEN-1) );

				// Read new data from the USB host if we still have space in the buffer
				if(countRX && countRX <= USBtoUSART_free )
				{
					// Prepare temporary pointer
					uint16_t tmp; // = 0x200 | USBtoUSART_WritePtr;
					asm (
						"ldi %B[tmp], 0x02\n\t"			// (1) Force high byte to 0x200
						"lds %A[tmp], %[writePtr]\n\t"	// (1) Load USBtoUSART_WritePtr into low byte
						// Outputs
						: [tmp] "=&e" (tmp)	// Pointer register, output only
						// Inputs
						: [writePtr] "m" (USBtoUSART_WritePtr) // Memory location
					);

					// Save USB bank into our USBtoUSART ringbuffer
					do {
						register uint8_t data;
						data = Endpoint_Read_8();
						asm (
							"st %a[tmp]+, %[data]\n\t" 	// (2) Save byte in buffer and increment
							"andi %A[tmp], 0x7F\n\t" 	// (1) Wrap around pointer, 128 bytes
							// Outputs
							: [tmp] "=e" (tmp) // Input and output
							// Inputs
							: "0" (tmp), [data] "r" (data)
						);
					} while (--countRX);

					// Acknowledge data
					Endpoint_ClearOUT();

					// Save back new pointer position
					// Just save the lower byte of the pointer
					USBtoUSART_WritePtr = tmp & 0xFF;

					// Enable USART again to flush the buffer
					UCSR1B = (_BV(RXCIE1) | _BV(TXEN1) | _BV(RXEN1) | _BV(UDRIE1));
					
					// Force Leds to turn on
					USBtoUSART_free = 0;
				}

				// Light RX led if we still have data in the USBtoUSART buffer
				if (USBtoUSART_free != (USB2USART_BUFLEN-1)) {
					LEDs_TurnOnRXLED;
					RxLEDPulse = TX_RX_LED_PULSE_MS;
				}

				//================================================================================
				// USARTtoUSB
				//================================================================================

				// This requires the USART RX buffer to be 256 bytes.
				uint8_t count = USARTtoUSB_WritePtr - USARTtoUSB_ReadPtr;

				// Check if we have something worth to send
				if (count) {

					// Check if the UART receive buffer flush timer has expired or the buffer is nearly full
					if ((TIFR0 & (1 << TOV0)) || (count >= (CDC_TX_EPSIZE - 1)) )
					{
						// Send data to the USB host
						Endpoint_SelectEndpoint(CDC_TX_EPADDR);

						// CDC device is ready for receiving bytes
						if (Endpoint_IsINReady())
						{
							// Send a maximum of up to one bank minus one.
							// If we fill the whole bank we'd have to send an empty Zero Length Packet (ZLP)
							// afterwards to determine the end of the transfer.
							// Since this is more complicated we only send single packets
							// with one byte less than the maximum.
							uint8_t txcount = CDC_TX_EPSIZE - 1;
							if (txcount > count)
							txcount = count;

							// Prepare temporary pointer
							uint16_t tmp; // = 0x100 | USARTtoUSBReadPtr
							asm (
								// Do not initialize high byte, it will be done in first loop below.
								"lds %A[tmp], %[readPtr]\n\t"	// (1) Copy read pointer into lower byte
								// Outputs
								: [tmp] "=&e" (tmp)	// Pointer register, output only
								// Inputs
								: [readPtr] "m" (USARTtoUSB_ReadPtr) // Memory location
							);

							// Write all bytes from USART to the USB endpoint
							do {
								register uint8_t data;
								asm (
									"ldi %B[tmp] , 0x01\n\t" 		// (1) Force high byte to 0x01
									"ld %[data] , %a[tmp] +\n\t" 	// (2) Load next data byte, wraps around 255
									// Outputs
									: [data] "=&r" (data),	// Output only
									[tmp] "=e" (tmp) 		// Input and output
									// Inputs
									: "1" (tmp)
								);
								Endpoint_Write_8(data);
							} while (--txcount);

							// Send data to USB Host now
							Endpoint_ClearIN();

							// Save new pointer position
							USARTtoUSB_ReadPtr = tmp & 0xFF;
						}
					}

					// Light TX led if there is data to be send
					LEDs_TurnOnTXLED;
					TxLEDPulse = TX_RX_LED_PULSE_MS;
				}
			}

			// LED timer overflow.
			// Check Leds (this methode takes less flash than an ISR)
			if (TIFR0 & (1 << TOV0)){
				// Reset the timer
				// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_intbits
				TIFR0 = (1 << TOV0);

				// Turn off TX LED once the TX pulse period has elapsed
				if (TxLEDPulse && !(--TxLEDPulse))
				LEDs_TurnOffTXLED;

				// Turn off RX LED once the RX pulse period has elapsed
				if (RxLEDPulse && !(--RxLEDPulse))
				LEDs_TurnOffRXLED;
			}
		};

		// Reset CDC Serial settings and disable USART properly
		LineEncoding.BaudRateBPS = 0;
		CDC_Device_LineEncodingChanged();

		// Dont forget LEDs on if suddenly unconfigured.
		LEDs_TurnOffTXLED;
		LEDs_TurnOffRXLED;
	}
}

/** Configures all hardware required for the bootloader. */
static void SetupHardware(void)
{
	// Disable clock division
	clock_prescale_set(clock_div_1);

	// Relocate the interrupt vector table to the bootloader section
	MCUCR = (1 << IVCE);
	MCUCR = (1 << IVSEL);

	/* Initialize the USB and other board hardware drivers */
	USB_Init();

	/* Start the flush timer for Leds */
	TCCR0B = (1 << CS02); // clk I/O / 256 (From prescaler)
	
	// Inits Serial pins, leds, reset and erase pins
	// No need to turn off Leds, this is done in the bootkey check function
	Board_Init();
}

/** Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
*  to relay data to and from the attached USB host.
*/
void EVENT_USB_Device_ConfigurationChanged(void)
{
	/* Setup CDC Notification, Rx and Tx Endpoints */
	Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT,
		CDC_NOTIFICATION_EPSIZE, 1
	);

	Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TX_EPSIZE, CDC_TX_BANK_SIZE);

	Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK, CDC_RX_EPSIZE, CDC_RX_BANK_SIZE);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
*  the device from the USB host before passing along unhandled control requests to the library for processing
*  internally.
*/
void EVENT_USB_Device_ControlRequest(void)
{
	/* Ignore any requests that aren't directed to the CDC interface */
	if ((USB_ControlRequest.bmRequestType & (CONTROL_REQTYPE_TYPE | CONTROL_REQTYPE_RECIPIENT)) !=
	(REQTYPE_CLASS | REQREC_INTERFACE))
	{
		return;
	}

	/* Process CDC specific control requests */
	uint8_t bRequest = USB_ControlRequest.bRequest;
	if (bRequest == CDC_REQ_GetLineEncoding){
		if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();

			/* Write the line coding data to the control endpoint */
			// this one is not inline because its already used somewhere in the usb core, so it will dupe code
			Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
			Endpoint_ClearOUT();
		}
	}
	else if (bRequest == CDC_REQ_SetLineEncoding){
		if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();

			// Read the line coding data in from the host into the global struct (made inline)
			//Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));

			uint8_t Length = sizeof(CDC_LineEncoding_t);
			uint8_t* DataStream = (uint8_t*)&LineEncoding;

			bool skip = false;
			while (Length)
			{
				uint8_t USB_DeviceState_LCL = USB_DeviceState;

				if ((USB_DeviceState_LCL == DEVICE_STATE_Unattached) || (USB_DeviceState_LCL == DEVICE_STATE_Suspended) || (Endpoint_IsSETUPReceived())){
					skip = true;
					break;
				}

				if (Endpoint_IsOUTReceived())
				{
					while (Length && Endpoint_BytesInEndpoint())
					{
						*DataStream = Endpoint_Read_8();
						DataStream++;
						Length--;
					}

					Endpoint_ClearOUT();
				}
			}

			if (!skip){
				do
				{
					uint8_t USB_DeviceState_LCL = USB_DeviceState;

					if ((USB_DeviceState_LCL == DEVICE_STATE_Unattached) || (USB_DeviceState_LCL == DEVICE_STATE_Suspended))
					break;
				}while (!(Endpoint_IsINReady()));
			}

			// end of inline Endpoint_Read_Control_Stream_LE

			Endpoint_ClearIN();

			CDC_Device_LineEncodingChanged();
		}
	}
	else if (bRequest == CDC_REQ_SetControlLineState){
		if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
		{
			Endpoint_ClearSETUP();
			Endpoint_ClearStatusStage();

			// check DTR state and reset the MCU
			// You could add the OUTPUT declaration here but it wont help since the pc always tries to open the serial port once.
			// At least if the usb is connected this always results in a main MCU reset if the bootloader is executed.
			// From my testings there is no way to avoid this. Its needed as far as I tested, no way.
			// TODO do not reset main MCU (not possible?)
			Board_Reset(USB_ControlRequest.wValue & CDC_CONTROL_LINE_OUT_DTR);
		}
	}
}

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
*  for later transmission to the host.
*/
ISR(USART1_RX_vect, ISR_NAKED)
{
	// This ISR doesnt change SREG. Whoa.
	asm volatile (
		"lds r3, %[UDR1_Reg]\n\t" 		// (1) Load new Serial byte (UDR1) into r3
		"movw r4, r30\n\t" 				// (1) Backup Z pointer (r30 -> r4, r31 -> r5)
		"in r30, %[writePointer]\n\t" 	// (1) Load USARTtoUSB write buffer 8 bit pointer to lower Z pointer
		"ldi r31, 0x01\n\t" 			// (1) Set higher Z pointer to 0x01
		"st Z+, r3\n\t" 				// (2) Save UDR1 in Z pointer (USARTtoUSB write buffer) and increment
		"out %[writePointer], r30\n\t" 	// (1) Save back new USARTtoUSB buffer pointer location
		"movw r30, r4\n\t" 				// (1) Restore backuped Z pointer
		"reti\n\t"						// (4) Exit ISR

		// Inputs:
		:: [UDR1_Reg] "m" (UDR1), 		// Memory location of UDR1
		[writePointer] "I" (_SFR_IO_ADDR(USARTtoUSB_WritePtr)) // 8 bit pointer to USARTtoUSB write buffer
	);
}

ISR(USART1_UDRE_vect, ISR_NAKED)
{
	// Another SREG-less ISR.
	asm volatile (
		"movw r4, r30\n\t" 					// (1) Backup Z pointer (r30 -> r4, r31 -> r5)
		"in r30, %[readPointer]\n\t"		// (1) Load USBtoUSART read buffer 8 bit pointer to lower Z pointer
		"ldi r31, 0x02\n\t" 				// (1) Set higher Z pointer to 0x02
		"ld r3, Z+\n\t" 					// (2) Load next byte from USBtoUSART buffer into r3
		"sts %[UDR1_Reg], r3\n\t"			// (2) Save r3 (next byte) in UDR1
		"out %[readPointer], r30\n\t" 		// (1) Save back new USBtoUSART read buffer pointer location
		"cbi %[readPointer], 7\n\t" 		// (2) Wrap around for 128 bytes
		//     smart after-the-fact andi 0x7F without using SREG
		"movw r30, r4\n\t"					// (1) Restore backuped Z pointer
		"in r2, %[readPointer]\n\t"			// (1) Load USBtoUSART read buffer 8 bit pointer to r2
		"lds r3, %[writePointer]\n\t" 		// (1) Load USBtoUSART write buffer to r3
		"cpse r2, r3\n\t"					// (1/2) Check if USBtoUSART read buffer == USBtoUSART write buffer
		"reti\n\t"							// (4) They are not equal, more bytes coming soon!
		"ldi r30, 0x98\n\t"					// (1) Set r30 temporary to new UCSR1B setting ((1<<RXCIE1) | (1 << RXEN1) | (1 << TXEN1))
		//     ldi needs an upper register, restore Z once more afterwards
		"sts %[UCSR1B_Reg], r30\n\t"		// (2) Turn off this interrupt (UDRIE1), all bytes sent
		"movw r30, r4\n\t"					// (1) Restore backuped Z pointer again (was overwritten again above)
		"reti\n\t"							// (4) Exit ISR

		// Inputs:
		:: [UDR1_Reg] "m" (UDR1),
		[readPointer] "I" (_SFR_IO_ADDR(USBtoUSART_ReadPtr)), 	// 7 bit pointer to USBtoUSART read buffer
		[writePointer] "m" (USBtoUSART_WritePtr), 				// 7 bit pointer to USBtoUSART write buffer
		[UCSR1B_Reg] "m" (UCSR1B)			// Memory location of UDR1
	);
}


/** Retrieves the next byte from the host in the CDC data OUT endpoint, and clears the endpoint bank if needed
*  to allow reception of the next data packet from the host.
*
*  \return Next received byte from the host in the CDC data OUT endpoint
*/
static uint8_t FetchNextCommandByte(void)
{
	/* Select the OUT endpoint so that the next data byte can be read */
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	/* If OUT endpoint empty, clear it and wait for the next packet from the host */
	while (!(Endpoint_IsReadWriteAllowed()))
	{
		Endpoint_ClearOUT();

		do
		{
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			return 0;
		}while (!(Endpoint_IsOUTReceived()));
	}

	/* Fetch the next byte from the OUT endpoint */
	return Endpoint_Read_8();
}

/** Writes the next response byte to the CDC data IN endpoint, and sends the endpoint back if needed to free up the
*  bank when full ready for the next byte in the packet to the host.
*
*  \param[in] Response  Next response byte to send to the host
*/
static void WriteNextResponseByte(const uint8_t Response)
{
	/* Select the IN endpoint so that the next data byte can be written */
	Endpoint_SelectEndpoint(CDC_TX_EPADDR);

	// Wait untill endpoint is ready to write
	do
	{
		if (USB_DeviceState == DEVICE_STATE_Unattached)
		return;
	}while (!(Endpoint_IsINReady()));

	/* Write the next byte to the IN endpoint */
	Endpoint_Write_8(Response);
	bankTX++;

	// Send a maximum of up to one bank minus one.
	// If we fill the whole bank we'd have to send an empty Zero Length Packet (ZLP)
	// afterwards to determine the end of the transfer.
	// Since this is more complicated we only send single packets
	// with one byte less than the maximum.
	if(bankTX >= (CDC_TX_EPSIZE - 1)){
		bankTX = 0;
		Endpoint_ClearIN();
	}
}

/** Task to read in AVR109 commands from the CDC data OUT endpoint, process them, perform the required actions
*  and send the appropriate response back to the host.
*/
static void Bootloader_Task(){
	// Initialize/reset register variables
	bankTX = 0;

	/* Read in the bootloader command (first byte sent from host) */
	uint8_t Command = FetchNextCommandByte();

	if (Command == AVR109_COMMAND_ExitBootloader)
	{
		RunBootloader = false;

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if ((Command == AVR109_COMMAND_SetLED) || (Command == AVR109_COMMAND_ClearLED) ||
	(Command == AVR109_COMMAND_SelectDeviceType))
	{
		FetchNextCommandByte();

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if ((Command == AVR109_COMMAND_EnterProgrammingMode) || (Command == AVR109_COMMAND_LeaveProgrammingMode))
	{
		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadPartCode)
	{
		/* Return ATMEGA128 part code - this is only to allow AVRProg to use the bootloader */
		WriteNextResponseByte(0x44);
		WriteNextResponseByte(0x00);
	}
	else if (Command == AVR109_COMMAND_ReadAutoAddressIncrement)
	{
		/* Indicate auto-address increment is supported */
		WriteNextResponseByte('Y');
	}
	else if (Command == AVR109_COMMAND_SetCurrentAddress)
	{
		/* Set the current address to that given by the host (translate 16-bit word address to byte address) */
		CurrAddress = (FetchNextCommandByte() << 9);
		CurrAddress |= (FetchNextCommandByte() << 1);

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadBootloaderInterface)
	{
		/* Indicate serial programmer back to the host */
		WriteNextResponseByte('S');
	}
	else if (Command == AVR109_COMMAND_ReadBootloaderIdentifier)
	{
		/* Write the 7-byte software identifier to the endpoint */
		for (uint8_t CurrByte = 0; CurrByte < 7; CurrByte++)
		WriteNextResponseByte(SOFTWARE_IDENTIFIER[CurrByte]);
	}
	else if (Command == AVR109_COMMAND_ReadBootloaderSWVersion)
	{
		WriteNextResponseByte('0' + BOOTLOADER_VERSION_MAJOR);
		WriteNextResponseByte('0' + BOOTLOADER_VERSION_MINOR);
	}
	else if (Command == AVR109_COMMAND_ReadSignature)
	{
		WriteNextResponseByte(AVR_SIGNATURE_3);
		WriteNextResponseByte(AVR_SIGNATURE_2);
		WriteNextResponseByte(AVR_SIGNATURE_1);
	}
	else if (Command == AVR109_COMMAND_EraseFLASH)
	{
		/* Clear the application section of flash */
		for (uint32_t CurrFlashAddress = 0; CurrFlashAddress < (uint32_t)BOOT_START_ADDR; CurrFlashAddress += SPM_PAGESIZE)
		{
			boot_page_erase(CurrFlashAddress);
			boot_spm_busy_wait();
			boot_page_write(CurrFlashAddress);
			boot_spm_busy_wait();
		}

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	#if !defined(NO_LOCK_BYTE_WRITE_SUPPORT)
	else if (Command == AVR109_COMMAND_WriteLockbits)
	{
		/* Set the lock bits to those given by the host */
		boot_lock_bits_set(FetchNextCommandByte());

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	#endif
	else if (Command == AVR109_COMMAND_ReadLockbits)
	{
		WriteNextResponseByte(boot_lock_fuse_bits_get(GET_LOCK_BITS));
	}
	else if (Command == AVR109_COMMAND_ReadLowFuses)
	{
		WriteNextResponseByte(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS));
	}
	else if (Command == AVR109_COMMAND_ReadHighFuses)
	{
		WriteNextResponseByte(boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS));
	}
	else if (Command == AVR109_COMMAND_ReadExtendedFuses)
	{
		WriteNextResponseByte(boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS));
	}
	#if !defined(NO_BLOCK_SUPPORT)
	else if (Command == AVR109_COMMAND_GetBlockWriteSupport)
	{
		WriteNextResponseByte('Y');

		/* Send block size to the host */
		WriteNextResponseByte(SPM_PAGESIZE >> 8);
		WriteNextResponseByte(SPM_PAGESIZE & 0xFF);
	}
	else if ((Command == AVR109_COMMAND_BlockWrite) || (Command == AVR109_COMMAND_BlockRead))
	{
		/* Delegate the block write/read to a separate function for clarity */
		ReadWriteMemoryBlock(Command);
	}
	#endif
	#if !defined(NO_FLASH_BYTE_SUPPORT)
	else if (Command == AVR109_COMMAND_FillFlashPageWordHigh)
	{
		/* Write the high byte to the current flash page */
		boot_page_fill(CurrAddress, FetchNextCommandByte());

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_FillFlashPageWordLow)
	{
		/* Write the low byte to the current flash page */
		boot_page_fill(CurrAddress | 0x01, FetchNextCommandByte());

		/* Increment the address */
		CurrAddress += 2;

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_WriteFlashPage)
	{
		/* Commit the flash page to memory */
		boot_page_write(CurrAddress);

		/* Wait until write operation has completed */
		boot_spm_busy_wait();

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadFLASHWord)
	{
		#if (FLASHEND > 0xFFFF)
		uint16_t ProgramWord = pgm_read_word_far(CurrAddress);
		#else
		uint16_t ProgramWord = pgm_read_word(CurrAddress);
		#endif

		WriteNextResponseByte(ProgramWord >> 8);
		WriteNextResponseByte(ProgramWord & 0xFF);
	}
	#endif
	#if !defined(NO_EEPROM_BYTE_SUPPORT)
	else if (Command == AVR109_COMMAND_WriteEEPROM)
	{
		/* Read the byte from the endpoint and write it to the EEPROM */
		eeprom_write_byte((uint8_t*)((intptr_t)(CurrAddress >> 1)), FetchNextCommandByte());

		/* Increment the address after use */
		CurrAddress += 2;

		/* Send confirmation byte back to the host */
		WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadEEPROM)
	{
		/* Read the EEPROM byte and write it to the endpoint */
		WriteNextResponseByte(eeprom_read_byte((uint8_t*)((intptr_t)(CurrAddress >> 1))));

		/* Increment the address after use */
		CurrAddress += 2;
	}
	#endif
	else if (Command != AVR109_COMMAND_Sync)
	{
		/* Unknown (non-sync) command, return fail code */
		WriteNextResponseByte('?');
	}

	// Check if there is still data in the USB bank that needs to be send
	if(bankTX)
	{
		/* Select the IN endpoint so that the next data byte can be written */
		Endpoint_SelectEndpoint(CDC_TX_EPADDR);

		// Wait untill endpoint is ready to write
		do{
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			return;
		}
		while (!(Endpoint_IsINReady()));

		// Send the endpoint data to the host */
		Endpoint_ClearIN();
		// bankTX will be reset in the next loop, no need to do it here
	}

	// In Bootloader mode clear the Out endpoint
	// Select the OUT endpoint
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	// Acknowledge the command from the host
	// All data is read, if not it is discarded here
	Endpoint_ClearOUT();
}

#if !defined(NO_BLOCK_SUPPORT)
/** Reads or writes a block of EEPROM or FLASH memory to or from the appropriate CDC data endpoint, depending
*  on the AVR109 protocol command issued.
*
*  \param[in] Command  Single character AVR109 protocol command indicating what memory operation to perform
*/
static void ReadWriteMemoryBlock(const uint8_t Command)
{
	uint16_t BlockSize;
	char     MemoryType;

	uint8_t  HighByte = 0;
	uint8_t  LowByte = 0;

	BlockSize = (FetchNextCommandByte() << 8);
	BlockSize |= FetchNextCommandByte();

	MemoryType = FetchNextCommandByte();

	if ((MemoryType != MEMORY_TYPE_FLASH) && (MemoryType != MEMORY_TYPE_EEPROM))
	{
		/* Send error byte back to the host */
		WriteNextResponseByte('?');

		return;
	}

	/* Check if command is to read a memory block */
	if (Command == AVR109_COMMAND_BlockRead)
	{
		/* Re-enable RWW section */
		boot_rww_enable();

		while (BlockSize--)
		{
			if (MemoryType == MEMORY_TYPE_FLASH)
			{
				/* Read the next FLASH byte from the current FLASH page */
				#if (FLASHEND > 0xFFFF)
				WriteNextResponseByte(pgm_read_byte_far(CurrAddress | HighByte));
				#else
				WriteNextResponseByte(pgm_read_byte(CurrAddress | HighByte));
				#endif

				/* If both bytes in current word have been read, increment the address counter */
				if (HighByte)
				CurrAddress += 2;

				HighByte = !HighByte;
			}
			else
			{
				/* Read the next EEPROM byte into the endpoint */
				WriteNextResponseByte(eeprom_read_byte((uint8_t*)(intptr_t)(CurrAddress >> 1)));

				/* Increment the address counter after use */
				CurrAddress += 2;
			}
		}
	}
	else
	{
		uint32_t PageStartAddress = CurrAddress;

		if (MemoryType == MEMORY_TYPE_FLASH)
		{
			boot_page_erase(PageStartAddress);
			boot_spm_busy_wait();
		}

		while (BlockSize--)
		{
			if (MemoryType == MEMORY_TYPE_FLASH)
			{
				/* If both bytes in current word have been written, increment the address counter */
				if (HighByte)
				{
					/* Write the next FLASH word to the current FLASH page */
					boot_page_fill(CurrAddress, ((FetchNextCommandByte() << 8) | LowByte));

					/* Increment the address counter after use */
					CurrAddress += 2;
				}
				else
				{
					LowByte = FetchNextCommandByte();
				}

				HighByte = !HighByte;
			}
			else
			{
				/* Write the next EEPROM byte from the endpoint */
				eeprom_write_byte((uint8_t*)((intptr_t)(CurrAddress >> 1)), FetchNextCommandByte());

				/* Increment the address counter after use */
				CurrAddress += 2;
			}
		}

		/* If in FLASH programming mode, commit the page after writing */
		if (MemoryType == MEMORY_TYPE_FLASH)
		{
			/* Commit the flash page to memory */
			boot_page_write(PageStartAddress);

			/* Wait until write operation has completed */
			boot_spm_busy_wait();
		}

		/* Send response byte back to the host */
		WriteNextResponseByte('\r');
	}
}
#endif

/** Event handler for the CDC Class driver Line Encoding Changed event.
*
*  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
*/
static void CDC_Device_LineEncodingChanged(void)
{
	/* Keep the TX line held high (idle) while the USART is reconfigured */
	PORTD |= (1 << 3);

	/* Must turn off USART before reconfiguring it, otherwise incorrect operation may occur */
	UCSR1B = 0;
	UCSR1A = 0;
	UCSR1C = 0;

	/* Flush data that was about to be sent. */
	USBtoUSART_ReadPtr = 0;
	USBtoUSART_WritePtr = 0;
	USARTtoUSB_ReadPtr = 0;
	USARTtoUSB_WritePtr = 0;

	// Only reconfigure USART if we are not in self reprogramming mode
	// and if the CDC Serial is not disabled
	uint32_t BaudRateBPS = LineEncoding.BaudRateBPS;
	if(BaudRateBPS == 0){
		mode = MODE_OFF;
	}
	else if(BaudRateBPS == BAUDRATE_CDC_BOOTLOADER){
		mode = MODE_BOOTLOADER;
	}
	else
	{
		mode = MODE_USBSERIAL;
		uint8_t ConfigMask = 0;

		switch (LineEncoding.ParityType)
		{
			case CDC_PARITY_Odd:
			ConfigMask = ((1 << UPM11) | (1 << UPM10));
			break;
			case CDC_PARITY_Even:
			ConfigMask = (1 << UPM11);
			break;
		}

		if (LineEncoding.CharFormat == CDC_LINEENCODING_TwoStopBits)
		ConfigMask |= (1 << USBS1);

		switch (LineEncoding.DataBits)
		{
			case 6:
			ConfigMask |= (1 << UCSZ10);
			break;
			case 7:
			ConfigMask |= (1 << UCSZ11);
			break;
			case 8:
			ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
			break;
		}

		// Set the new baud rate before configuring the USART
		uint8_t clockSpeed = (1 << U2X1);
		uint16_t brr = SERIAL_2X_UBBRVAL(BaudRateBPS);

		// No need U2X or cant have U2X.
		if ((brr & 1) || (brr > 4095)) {
			brr >>= 1;
			clockSpeed = 0;
		}

		// Or special case 57600 baud for compatibility with the ATmega328 bootloader.
		else if(((brr == SERIAL_2X_UBBRVAL(57600)) && (BAUDRATE_CDC_BOOTLOADER != 57600))){
			brr = SERIAL_UBBRVAL(57600);
			clockSpeed = 0;
		}

		UBRR1 = brr;

		// Reconfigure the USART
		UCSR1C = ConfigMask;
		UCSR1A = clockSpeed;
		UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));
	}

	/* Release the TX line after the USART has been reconfigured */
	PORTD &= ~(1 << 3);
}
