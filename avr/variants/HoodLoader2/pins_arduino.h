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

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

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

#define TX_RX_LED_INIT	DDRD |= LEDS_ALL_LEDS
#define TXLED0			PORTD |= LEDMASK_TX
#define TXLED1			PORTD &= ~LEDMASK_TX
#define RXLED0			PORTD |= LEDMASK_RX
#define RXLED1			PORTD &= ~LEDMASK_RX

#define LED_BUILTIN 18
#define LED_BUILTIN_RX 17
#define LED_BUILTIN_TX 18

// reset line for 328/2560
#define IO_MCU_RESET_PIN 20

static const uint8_t SS   = 0;
static const uint8_t SCK  = 1;
static const uint8_t MOSI = 2;
static const uint8_t MISO = 3;


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

// 8 interrupts!
#define digitalPinToInterrupt(p) ((p) == 13 ? 0 : ((p) == 14 ? 1 : ((p) == 15 ? 2 : ((p) == 16 ? 3 : ((p) == 8 ? 4 : ((p) == 17 ? 5 : ((p) == 19 ? 6 : ((p) == 20 ? 7 : NOT_AN_INTERRUPT))))))))


#ifdef ARDUINO_MAIN

/*
16u2 Pinout
[NC] = Not connected on an Arduino Uno / Mega R3

D0 - PB0 PCINT0
D1 - PB1 PCINT1
D2 - PB2 PCINT2
D3 - PB3 PCINT3
D4 - PB4 PCINT4
D5 - PB5 PCINT5
D6 - PB6 PCINT6
D7 - PB7 PCINT7 TIMER1C

D8 - [NC] PC7 INT4
D9 - [NC] PC6 PCINT8 TIMER1A
D10 - [NC] PC5 PCINT9 TIMER1B
D11 - [NC] PC4 PCINT10
D12 - [NC] PC2 PCINT11

D13 - [NC] PD0 INT0 TIMER0B
D14 - [NC] PD1 INT1
D15 - PD2 INT2 USART1 RX
D16 - PD3 INT3 USART1 TX
D17 - PD4 INT4    RXLED
D18 - PD5 PCINT12 TXLED
D19 - [NC] PD6 INT5
D20 - PD7 INT7 328 / 2560 RESET
*/

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

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR        Serial
#define SERIAL_PORT_USBVIRTUAL     Serial
#define SERIAL_PORT_HARDWARE       Serial1
#define SERIAL_PORT_HARDWARE_OPEN  Serial1

#endif /* Pins_Arduino_h */
