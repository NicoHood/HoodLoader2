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
*  \brief Board specific information header for the Arduino Leonardo board.
*  \copydetails Group_BoardInfo_LEONARDO
*
*  \note This file should not be included directly. It is automatically included as needed by the Board driver
*        dispatch header located in LUFA/Drivers/Board/Board.h.
*/

/** \ingroup Group_BoardInfo
*  \defgroup Group_BoardInfo_LEONARDO LEONARDO
*  \brief Board specific information header for the Arduino Leonardo board.
*
*  Board specific information header for the Arduino Leonardo board (http://arduino.cc/en/Main/arduinoBoardLeonardo).
*
*  @{
*/

#ifndef __BOARD_HOODLOADER_H__
#define __BOARD_HOODLOADER_H__

/* Includes: */
//#include "../../../../Common/Common.h"
//#include "../../LEDs.h"

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

// 1200 is the baud to load the Bootloader from an Arduino sketch, 57600 turns out to be the actual baud rate for uploading
//TODO change baud rate to something people wont normally use
//#define BAUDRATE_CDC_BOOTLOADER 1200
#define BAUDRATE_CDC_BOOTLOADER 57600

#define ARDUINO_PORT PORTD
#define ARDUINO_DDR DDRD

#define AVR_RESET_LINE_PORT PORTD
#define AVR_RESET_LINE_DDR DDRD
#define AVR_RESET_LINE_PIN PIND
#define AVR_RESET_LINE_MASK (1 << PD7)

	// hardware configuration
#define AVR_SPI_PIN PINB
#define AVR_SPI_PORT PORTB
#define AVR_SPI_DDR DDRB
#define AVR_MOSI PB2
#define AVR_MISO PB3
#define AVR_SCK PB1
	// The "real", hardware SS pin is not connected on an Arduino R3 so we use another pin as SS.
	// The hardware SS pin still needs to be in output mode to enable SPI master mode.
	// SS is the pin thats close to the TX Led (bottom right)
#define AVR_SS PB4
#define AVR_HARDWARE_SS PB0

#define LOW 0
#define HIGH 1

	/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif

/** @} */

