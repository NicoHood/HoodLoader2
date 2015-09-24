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

#ifndef __BOARD_HOODLOADER_H__
#define __BOARD_HOODLOADER_H__

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

	/* Includes: */
		#include <LUFA/Common/Common.h>
		#include <LUFA/Drivers/Board/LEDs.h>

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_BOARD_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/Board.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Indicates the board has hardware LEDs mounted. */
			#define BOARD_HAS_LEDS
			
			// USB VID/PID settings
			#define LUFA_VID					0x03EB
			#define LUFA_PID					0x204A

			#define ARDUINO_VID					0x2341
			#define ARDUINO_UNO_PID				0x0043 // R3 (0001 R1)
			#define ARDUINO_MEGA_PID			0x0042 // R3 (0010 R1)
			#define ARDUINO_MEGA_ADK_PID		0x0044 // R3 (003F R1)
			#define ARDUINO_LEONARDO_PID		0x0036 // Bootloader, not program!
			#define ARDUINO_MICRO_PID   		0x0037 // Bootloader, not program!
			#define ARDUINO_DUE_PID             0x003D
			
			// USB product string settings
			#if (PRODUCTID == ARDUINO_UNO_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Uno"
			#elif (PRODUCTID == ARDUINO_MEGA_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Mega"
			#elif (PRODUCTID == ARDUINO_ADK_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 ADK"
			#elif (PRODUCTID == ARDUINO_LEONARDO_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Leo"
			#elif (PRODUCTID == ARDUINO_MICRO_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Micro"
			#elif (PRODUCTID == ARDUINO_DUE_PID)
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Due"
			#else
			#define USB_DESCRIPTOR_STRING L"HoodLoader2 Lufa"
			#endif
			
			// Arduino Due 16u2
			#if (PRODUCTID == ARDUINO_DUE_PID)

			#error DUE is currently not supported because of its reset mechanism. See Issue #16 on Github for more information.

			#define AVR_RESET_LINE_PORT PORTC
			#define AVR_RESET_LINE_DDR DDRC
			#define AVR_RESET_LINE_MASK (1 << 7)

			#define AVR_ERASE_LINE_PORT PORTC
			#define AVR_ERASE_LINE_DDR DDRC
			#define AVR_ERASE_LINE_MASK (1 << 6)
			
			/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Board_Init(void)
			{
				// On Due reset and erase pins are on PortC instead of PortD
				// Portmanipulation with cbi and sbi will be used here
				DDRD |= LEDS_ALL_LEDS | (1 << PD3);
				PORTD |= (1 << PD2);
	
				// INACTIVE => set as INPUT (internal pullup on target /RESET keep it at 3.3v)
				AVR_RESET_LINE_DDR  &= ~AVR_RESET_LINE_MASK;
				AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
	
				// Target /ERASE line is active HIGH: there is a mosfet that inverts logic
				AVR_ERASE_LINE_PORT |= AVR_ERASE_LINE_MASK;
				AVR_ERASE_LINE_DDR  |= AVR_ERASE_LINE_MASK;	
			}
			
			static inline void Board_Reset(bool reset)
			{
				/* Target /RESET line  */
				if (reset) {
					/* ACTIVE   => OUTPUT LOW (0v on target /RESET) */
					AVR_RESET_LINE_DDR  |= AVR_RESET_LINE_MASK;
					AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
				} 
				else {
				 	/* INACTIVE => set as INPUT (internal pullup on target /RESET keep it at 3.3v) */
					AVR_RESET_LINE_DDR  &= ~AVR_RESET_LINE_MASK;
					AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
				}
			}
			
			static inline void Board_Erase(bool erase)
			{
				if (erase)
					AVR_ERASE_LINE_PORT &= ~AVR_ERASE_LINE_MASK;
				else
					AVR_ERASE_LINE_PORT |= AVR_ERASE_LINE_MASK;
			}
		#endif

			// Arduino Leonardo/Micro
			#elif defined(__AVR_ATmega32U4__)
			
			/** Pin that can reset the main MCU. */
			// PORTB would also be possible (D8-11 + SPI)
			// I will not use it since PB contains the only PCINT
			// And the pins on PD are not better or worse
			#define AVR_RESET_LINE_PORT PORTD
			#define AVR_RESET_LINE_DDR DDRD
			#define AVR_RESET_LINE_PIN PIND
			#define AVR_RESET_LINE_MASK (1 << PD4) // PD4 = D4, PD6 = D12, PD7 = D7
			
			/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Board_Init(void)
			{
				// We use = here since the pins should be input/low anyways.
				// This saves us some more bytes for flash
				DDRD = LEDMASK_TX | (1 << PD3) | AVR_RESET_LINE_MASK;
			 	// Results in sbi instructions
				DDRB  |= LEDMASK_RX;
				PORTD |= AVR_RESET_LINE_MASK;
				PORTD |= (1 << PD2);
			}
			
			static inline void Board_Reset(bool reset)
			{
				if (reset)
					AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
				else
					AVR_RESET_LINE_PORT |= AVR_RESET_LINE_MASK;
			}
			
			static inline void Board_Erase(bool erase)
			{
				// No erase pin
			}
		#endif
			
			// Arduino Uno/Mega 8/16/32u2
			#else
			
			/** Pin that can reset the main MCU. */
			#define AVR_RESET_LINE_PORT PORTD
			#define AVR_RESET_LINE_DDR DDRD
			#define AVR_RESET_LINE_PIN PIND
			#define AVR_RESET_LINE_MASK (1 << PD7)
			
			/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Board_Init(void)
			{
				DDRD |= LEDS_ALL_LEDS | (1 << PD3) | AVR_RESET_LINE_MASK;
				PORTD |= AVR_RESET_LINE_MASK;
				PORTD |= (1 << PD2);
			}
			
			static inline void Board_Reset(bool reset)
			{
				if (reset)
					AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
				else
					AVR_RESET_LINE_PORT |= AVR_RESET_LINE_MASK;
			}
			
			static inline void Board_Erase(bool erase)
			{
				// No erase pin
			}
		#endif
			
			#endif // Arduino Uno/Mega 8/16/32u2

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

