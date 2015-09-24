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

#ifndef __LEDS_HOODLOADER_H__
#define __LEDS_HOODLOADER_H__

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

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
		
			// Arduino Leonardo/Micro
			#if defined(__AVR_ATmega32U4__)
			
	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			#define LEDS_PORTB_LEDS       (LEDS_LED1)
			#define LEDS_PORTD_LEDS       (LEDS_LED2)
			#define LEDS_PORTC_LEDS       (LEDS_LED3)
	#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** LED mask for the first LED on the board. */
			#define LEDS_LED1        (1 << 0)

			/** LED mask for the second LED on the board. */
			#define LEDS_LED2        (1 << 5)

			/** LED mask for the third LED on the board. */
			#define LEDS_LED3        (1 << 7)

			/** LED mask for all the LEDs on the board. */
			#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2 | LEDS_LED3)

			/** LED mask for none of the board LEDs. */
			#define LEDS_NO_LEDS     0

			/** LED mask for the library LED driver, to indicate TX activity. */
			#define LEDMASK_TX       LEDS_LED2

			/** LED mask for the library LED driver, to indicate RX activity. */
			#define LEDMASK_RX       LEDS_LED1

			/** LED mask for the library LED driver, to indicate other activity. */
			#define LEDMASK_13       LEDS_LED3
			
			// Direct Port access for more speed
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

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void LEDs_Init(void)
			{
				DDRB  |=  LEDS_PORTB_LEDS;
				PORTB |=  LEDS_PORTB_LEDS;
				DDRD  |=  LEDS_PORTD_LEDS;
				PORTD |=  LEDS_PORTD_LEDS;
				DDRC  |=  LEDS_PORTC_LEDS;
				PORTC &= ~LEDS_PORTC_LEDS;
			}

			static inline void LEDs_Disable(void)
			{
				DDRB  &= ~LEDS_PORTB_LEDS;
				PORTB &= ~LEDS_PORTB_LEDS;
				DDRD  &= ~LEDS_PORTD_LEDS;
				PORTD &= ~LEDS_PORTD_LEDS;
				DDRC  &= ~LEDS_PORTC_LEDS;
				PORTC &= ~LEDS_PORTC_LEDS;
			}

			static inline void LEDs_TurnOnLEDs(const uint8_t LEDMask)
			{
				PORTB &= ~(LEDMask & LEDS_PORTB_LEDS);
				PORTD &= ~(LEDMask & LEDS_PORTD_LEDS);
				PORTC |=  (LEDMask & LEDS_PORTC_LEDS);
			}

			static inline void LEDs_TurnOffLEDs(const uint8_t LEDMask)
			{
				PORTB |=  (LEDMask & LEDS_PORTB_LEDS);
				PORTD |=  (LEDMask & LEDS_PORTD_LEDS);
				PORTC &= ~(LEDMask & LEDS_PORTC_LEDS);
			}

			static inline void LEDs_SetAllLEDs(const uint8_t LEDMask)
			{
				PORTB = ((PORTB |  LEDS_PORTB_LEDS) & ~(LEDMask & LEDS_PORTB_LEDS));
				PORTD = ((PORTD |  LEDS_PORTD_LEDS) & ~(LEDMask & LEDS_PORTD_LEDS));
				PORTC = ((PORTC & ~LEDS_PORTC_LEDS) |  (LEDMask & LEDS_PORTC_LEDS));
			}

			static inline void LEDs_ChangeLEDs(const uint8_t LEDMask,
			                                   const uint8_t ActiveMask)
			{
				PORTB = ((PORTB |  (LEDMask & LEDS_PORTB_LEDS)) & ~(ActiveMask & LEDS_PORTB_LEDS));
				PORTD = ((PORTD |  (LEDMask & LEDS_PORTD_LEDS)) & ~(ActiveMask & LEDS_PORTD_LEDS));
				PORTC = ((PORTC & ~(LEDMask & LEDS_PORTC_LEDS)) |  (ActiveMask & LEDS_PORTC_LEDS));
			}

			static inline void LEDs_ToggleLEDs(const uint8_t LEDMask)
			{
				PINB  = (LEDMask & LEDS_PORTB_LEDS);
				PIND  = (LEDMask & LEDS_PORTD_LEDS);
				PINC  = (LEDMask & LEDS_PORTC_LEDS);
			}

			static inline uint8_t LEDs_GetLEDs(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t LEDs_GetLEDs(void)
			{
				return ((~PORTB & LEDS_PORTB_LEDS) | (~PORTD & LEDS_PORTD_LEDS) | (PORTC & LEDS_PORTC_LEDS));
			}
		#endif
			
			// Arduino Uno/Mega/Due 8/16/32u2
			#else
			
	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** LED mask for the first LED on the board. */
			#define LEDS_LED1        (1 << 5)

			/** LED mask for the second LED on the board. */
			#define LEDS_LED2        (1 << 4)

			/** LED mask for all the LEDs on the board. */
			#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2)

			/** LED mask for none of the board LEDs. */
			#define LEDS_NO_LEDS     0
			
			/** LED mask for the library LED driver, to indicate TX activity. */
			#define LEDMASK_TX       LEDS_LED1
			
			/** LED mask for the library LED driver, to indicate RX activity. */
			#define LEDMASK_RX       LEDS_LED2
			
			// Direct Port access for more speed
			#define LEDs_TurnOnTXLED (PORTD &= ~LEDMASK_TX)
			#define LEDs_TurnOnRXLED (PORTD &= ~LEDMASK_RX)
			#define LEDs_TurnOffTXLED (PORTD |= LEDMASK_TX)
			#define LEDs_TurnOffRXLED (PORTD |= LEDMASK_RX)

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void LEDs_Init(void)
			{
				DDRD  |= LEDS_ALL_LEDS;
				PORTD |= LEDS_ALL_LEDS;
			}

			static inline void LEDs_Disable(void)
			{
				DDRD  &= ~LEDS_ALL_LEDS;
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

			static inline void LEDs_ChangeLEDs(const uint8_t LEDMask,
			                                   const uint8_t ActiveMask)
			{
				PORTD = ((PORTD | LEDMask) & ~ActiveMask);
			}

			static inline void LEDs_ToggleLEDs(const uint8_t LEDMask)
			{
				PIND  = LEDMask;
			}

			static inline uint8_t LEDs_GetLEDs(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t LEDs_GetLEDs(void)
			{
				return (~PORTD & LEDS_ALL_LEDS);
			}
		#endif
			
			#endif // Arduino Uno/Mega/Due 8/16/32u2

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

