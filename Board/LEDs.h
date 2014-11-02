/*
			 LUFA Library
			 Copyright (C) Dean Camera, 2014.

			 dean [at] fourwalledcubicle [dot] com
			 www.fourwalledcubicle.com
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

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
  */

/*
Copyright(c) 2014 NicoHood
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

#ifndef __LEDS_ARDUINOUNO_H__
#define __LEDS_ARDUINOUNO_H__

/* Includes: */
#include <LUFA/Common/Common.h>

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

	/* Preprocessor Checks: */
#if !defined(__INCLUDE_FROM_LEDS_H)
#error Do not include this file directly. Include LUFA/Drivers/Board/LEDS.h instead.
#endif

	// Pulse generation counters to keep track of the number of milliseconds remaining for each pulse type
#define TX_RX_LED_PULSE_MS 3

	/* Public Interface - May be used in end-application: */
	/* Macros: */
	/** LED mask for the first LED on the board. */
#define LEDS_LED1        (1 << 5)

	/** LED mask for the second LED on the board. */
#define LEDS_LED2        (1 << 4)

	/** LED mask for all the LEDs on the board. */
#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2)

	/** LED mask for the none of the board LEDs */
#define LEDS_NO_LEDS     0

	// LED mask for the library LED driver, to indicate TX activity.
#define LEDMASK_TX               LEDS_LED1

	// LED mask for the library LED driver, to indicate RX activity.
#define LEDMASK_RX               LEDS_LED2

	// AVRISP Leds
#define LEDS_PMODE LEDS_LED1
#define LEDS_ERR LEDS_LED2

	/* Inline Functions: */
#if !defined(__DOXYGEN__)
	static inline void LEDs_Init(void)
	{
		DDRD |= LEDS_ALL_LEDS;
		PORTD |= LEDS_ALL_LEDS;
	}

	static inline void LEDs_Disable(void)
	{
		DDRD &= ~LEDS_ALL_LEDS;
		PORTD |= LEDS_ALL_LEDS;
	}

	static inline void LEDs_TurnOnLEDs(const uint8_t LEDMask)
	{
		PORTD &= ~LEDMask;
	}

	static inline void LEDs_TurnOffLEDs(const uint8_t LEDMask)
	{
		PORTD |= LEDMask;
	}

	static inline void LEDs_SetAllLEDs(const uint8_t LEDMask)
	{
		PORTD = ((PORTD | LEDS_ALL_LEDS) & ~LEDMask);
	}

	static inline void LEDs_ChangeLEDs(const uint8_t LEDMask, const uint8_t ActiveMask)
	{
		PORTD = ((PORTD | ActiveMask) & ~LEDMask);
	}

	static inline void LEDs_ToggleLEDs(const uint8_t LEDMask)
	{
		PORTD ^= LEDMask;
	}

	static inline uint8_t LEDs_GetLEDs(void) ATTR_WARN_UNUSED_RESULT;
	static inline uint8_t LEDs_GetLEDs(void)
	{
		return (PORTD & LEDS_ALL_LEDS);
	}
#endif

	/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif
