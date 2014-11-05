/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/


/*
  Modified 10 April 2012 by Justin Mattair
     for MattairTech MT-DB-U1 boards (www.mattairtech.com)
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

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>
#define USBCON_2 USBCON

#undef USBCON
#define USB_PRODUCT "HL2 16u2"
#define USB_MANUFACTURER "NicoHood"
#define USB_VID 0x16D0
#define USB_PID 0x0856

#define TX_RX_LED_INIT	DDRD |= (1<<0)
#define TXLED0			PORTD |= (1<<0)
#define TXLED1			PORTD &= ~(1<<0)
#define RXLED0			PORTD |= (1<<0)
#define RXLED1			PORTD &= ~(1<<0)

static const uint8_t LED_BUILTIN = 13;

static const uint8_t SS   = 0;
static const uint8_t MOSI = 2;
static const uint8_t MISO = 3;
static const uint8_t SCK  = 1;

#define NUM_DIGITAL_PINS            21
#define NUM_ANALOG_INPUTS           0

// Including Timer 0
#define digitalPinHasPWM(p)         ((p) == 7 || (p) == 9 || (p) == 10 || (p) == 13)

// PCINT Pins: 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 18
#define digitalPinToPCICR(p)    ( (((p) >= 0) && ((p) <= 7)) || \
                                  ((p) == 18) || \
                                  (((p) >= 9) && ((p) <= 12)) ? (&PCICR) : ((uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( (((p) >= 9) && ((p) <= 12)) || ((p) == 18) ? 1 : 0 )

#define digitalPinToPCMSK(p)    ( (((p) >= 0) && ((p) <= 7)) ? (&PCMSK0) : \
                                ( (((p) >= 9) && ((p) <= 12)) || ((p) == 18) ? (&PCMSK1) : \
                                ((uint8_t *)0) ) )

#define digitalPinToPCMSKbit(p) ( (((p) >= 0) && ((p) <= 7)) ? (p) : \
                                ( ((p) == 18) ? 4 : \
                                ( (((p) >= 9) && ((p) <= 12)) ? ((p) - 9) : \
                                0 ) ) )


#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[21] = {
        PB, /* 0 */
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PC, /* 8 */
        PC,
        PC,
        PC,
        PC,
        PD, /* 13 */
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[21] = {
        _BV(0), /* 0, port B */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(7), /* 8, port C */
        _BV(6),
        _BV(5),
        _BV(4),
        _BV(2),
        _BV(0), /* 13, port D */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[21] = {
        NOT_ON_TIMER, /* 0 - port B */
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        TIMER1C,
        NOT_ON_TIMER, /* 8 - port C */
        TIMER1A,
        TIMER1B,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        TIMER0B, /* 13 - port D */
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
        NOT_ON_TIMER,
};


#endif /* ARDUINO_MAIN */
#endif /* Pins_Arduino_h */
