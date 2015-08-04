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
 *  Header file for BootloaderCDC.c.
 */

#ifndef _CDC_H_
#define _CDC_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/boot.h>
		#include <avr/eeprom.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <util/atomic.h>
		#include <stdbool.h>

		#include "Descriptors.h"
		#include "BootloaderAPI.h"
		#include "Config/AppConfig.h"

		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>

		#include <LUFA/Drivers/Peripheral/Serial.h>
		#include <LUFA/Drivers/Board/Board.h>
		#include <util/delay.h>

	/* Preprocessor Checks: */
		#if !defined(__OPTIMIZE_SIZE__)
			#error This bootloader requires that it be optimized for size, not speed, to fit into the target device. Change optimization settings and try again.
		#endif

	/* Macros: */
		/** Version major of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MAJOR     0x01

		/** Version minor of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MINOR     0x00

		/** Hardware version major of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MAJOR   0x01

		/** Hardware version minor of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MINOR   0x00

		/** Eight character bootloader firmware identifier reported to the host when requested. */
		#define SOFTWARE_IDENTIFIER          "HL2.0.5"

		/** Port of the onboard leds, serial and reset line */
		#define ARDUINO_PORT PORTD
		#define ARDUINO_DDR DDRD

// Avoid to select the wrong BOARD in the makefile
#if !defined(__LEDS_LEONARDO_H__) && defined(__AVR_ATmega32U4__) || defined(__LEDS_LEONARDO_H__) && !defined(__AVR_ATmega32U4__)
#error Please select LEONARDO as BOARD in the makefile
#endif

#ifdef __AVR_ATmega32U4__
		/** Pin that can reset the main MCU. */
		// PORTB would also be possible (D8-11 + SPI)
		// I will not use it since PB contains the only PCINT
		// And the pins on PD are not better or worse
		#define AVR_RESET_LINE_PORT PORTD
		#define AVR_RESET_LINE_DDR DDRD
		#define AVR_RESET_LINE_PIN PIND
		#define AVR_RESET_LINE_MASK (1 << PD4) // PD4 = D4, PD6 = D12, PD7 = D7

		// Leds PORT needs to be switched for Micro
#if (PRODUCTID == ARDUINO_MICRO_PID)
		#define LEDs_TurnOnTXLED (PORTD &= ~LEDMASK_TX)
		#define LEDs_TurnOnRXLED (PORTB &= ~LEDMASK_RX)
		#define LEDs_TurnOffTXLED (PORTD |= LEDMASK_TX)
		#define LEDs_TurnOffRXLED (PORTB |= LEDMASK_RX)
#else
		#define LEDs_TurnOffTXLED (PORTD &= ~LEDMASK_TX)
		#define LEDs_TurnOffRXLED (PORTB &= ~LEDMASK_RX)
		#define LEDs_TurnOnTXLED (PORTD |= LEDMASK_TX)
		#define LEDs_TurnOnRXLED (PORTB |= LEDMASK_RX)
#endif

#else
		#define LEDs_TurnOnTXLED (PORTD &= ~LEDMASK_TX)
		#define LEDs_TurnOnRXLED (PORTD &= ~LEDMASK_RX)
		#define LEDs_TurnOffTXLED (PORTD |= LEDMASK_TX)
		#define LEDs_TurnOffRXLED (PORTD |= LEDMASK_RX)
#endif

	/* Enums: */
		/** Possible memory types that can be addressed via the bootloader. */
		enum AVR109_Memories
		{
			MEMORY_TYPE_FLASH  = 'F',
			MEMORY_TYPE_EEPROM = 'E',
		};

		/** Possible commands that can be issued to the bootloader. */
		enum AVR109_Commands
		{
			AVR109_COMMAND_Sync                     = 27,
			AVR109_COMMAND_ReadEEPROM               = 'd',
			AVR109_COMMAND_WriteEEPROM              = 'D',
			AVR109_COMMAND_ReadFLASHWord            = 'R',
			AVR109_COMMAND_WriteFlashPage           = 'm',
			AVR109_COMMAND_FillFlashPageWordLow     = 'c',
			AVR109_COMMAND_FillFlashPageWordHigh    = 'C',
			AVR109_COMMAND_GetBlockWriteSupport     = 'b',
			AVR109_COMMAND_BlockWrite               = 'B',
			AVR109_COMMAND_BlockRead                = 'g',
			AVR109_COMMAND_ReadExtendedFuses        = 'Q',
			AVR109_COMMAND_ReadHighFuses            = 'N',
			AVR109_COMMAND_ReadLowFuses             = 'F',
			AVR109_COMMAND_ReadLockbits             = 'r',
			AVR109_COMMAND_WriteLockbits            = 'l',
			AVR109_COMMAND_EraseFLASH               = 'e',
			AVR109_COMMAND_ReadSignature            = 's',
			AVR109_COMMAND_ReadBootloaderSWVersion  = 'V',
			AVR109_COMMAND_ReadBootloaderHWVersion  = 'v',
			AVR109_COMMAND_ReadBootloaderIdentifier = 'S',
			AVR109_COMMAND_ReadBootloaderInterface  = 'p',
			AVR109_COMMAND_SetCurrentAddress        = 'A',
			AVR109_COMMAND_ReadAutoAddressIncrement = 'a',
			AVR109_COMMAND_ReadPartCode             = 't',
			AVR109_COMMAND_EnterProgrammingMode     = 'P',
			AVR109_COMMAND_LeaveProgrammingMode     = 'L',
			AVR109_COMMAND_SelectDeviceType         = 'T',
			AVR109_COMMAND_SetLED                   = 'x',
			AVR109_COMMAND_ClearLED                 = 'y',
			AVR109_COMMAND_ExitBootloader           = 'E',
		};

	/* Type Defines: */
		/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
		typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

	/* Function Prototypes: */
		static void Bootloader_Task(void);
		static void CDC_Device_LineEncodingChanged(void);
		static void SetupHardware(void);
		static void StartSketch(void) __attribute__ ((noinline));
		static void ResetMCU(void);

		void Application_Jump_Check(void) ATTR_INIT_SECTION(3);

		void EVENT_USB_Device_ConfigurationChanged(void);

		#if defined(INCLUDE_FROM_BOOTLOADERCDC_C) || defined(__DOXYGEN__)
			#if !defined(NO_BLOCK_SUPPORT)
			static void    ReadWriteMemoryBlock(const uint8_t Command);
			#endif
			static uint8_t FetchNextCommandByte(void);
			static void    WriteNextResponseByte(const uint8_t Response);
		#endif

#endif

