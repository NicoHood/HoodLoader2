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

/** \file
 *  \brief Digital button board hardware driver.
 *
 *  This file is the master dispatch header file for the board-specific Buttons driver, for boards containing
 *  physical pushbuttons connected to the microcontroller's GPIO pins.
 *
 *  User code should include this file, which will in turn include the correct Button driver header file for the
 *  currently selected board.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/Buttons.h file in the user project
 *  directory.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 */

/** \ingroup Group_BoardDrivers
 *  \defgroup Group_Buttons Buttons Driver - LUFA/Drivers/Board/Buttons.h
 *  \brief Digital button board hardware driver.
 *
 *  \section Sec_Buttons_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - None
 *
 *  \section Sec_Buttons_ModDescription Module Description
 *  Hardware buttons driver. This provides an easy to use driver for the hardware buttons present on many boards.
 *  It provides a way to easily configure and check the status of all the buttons on the board so that appropriate
 *  actions can be taken.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/Buttons.h file in the user project
 *  directory. Otherwise, it will include the appropriate built-in board driver header file.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 *
 *  \section Sec_Buttons_ExampleUsage Example Usage
 *  The following snippet is an example of how this module may be used within a typical
 *  application.
 *
 *  \code
 *      // Initialize the button driver before first use
 *      Buttons_Init();
 *
 *      printf("Waiting for button press...\r\n");
 *
 *      // Loop until a board button has been pressed
 *      uint8_t ButtonPress;
 *      while (!(ButtonPress = Buttons_GetStatus())) {};
 *
 *      // Display which button was pressed (assuming two board buttons)
 *      printf("Button pressed: %s\r\n", (ButtonPress == BUTTONS_BUTTON1) ? "Button 1" : "Button 2");
 *  \endcode
 *
 *  @{
 */

#ifndef __BUTTONS_ARDUINO_H__
#define __BUTTONS_ARDUINO_H__

/* Includes: */
#include <LUFA/Common/Common.h>

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

	/* Preprocessor Checks: */
#if !defined(__INCLUDE_FROM_BUTTONS_H)
#error Do not include this file directly. Include LUFA/Drivers/Board/Buttons.h instead.
#endif

#define BUTTONS_BUTTON1 (1 << PB5)
#define BUTTONS_BUTTON2 (1 << PB6)
#define BUTTONS_BUTTON3 (1 << PB7) // unused
#define BUTTONS_ALL_BUTTONS (BUTTONS_BUTTON1 | BUTTONS_BUTTON2 | BUTTONS_BUTTON3)

	// Hoodloader defined button usage
#define BUTTON_HID BUTTONS_BUTTON1
#define BUTTON_AUTORESET BUTTONS_BUTTON2

	/* Pseudo-Functions for Doxygen: */
#if !defined(__DOXYGEN__)
	/** Initializes the buttons driver, so that the current button position can be read. This sets the appropriate
	 *  I/O pins to an inputs with pull-ups enabled.
	 *
	 *  This must be called before any Button driver functions are used.
	 */
	static inline void Buttons_Init(void){
		DDRB &= ~BUTTONS_ALL_BUTTONS;
		PORTB |= BUTTONS_ALL_BUTTONS;
	}

	/** Disables the buttons driver, releasing the I/O pins back to their default high-impedance input mode. */
	static inline void Buttons_Disable(void){
		PORTB &= ~BUTTONS_ALL_BUTTONS;
	}

	/** Returns a mask indicating which board buttons are currently pressed.
	 *
	 *  \return Mask of \c BUTTONS_BUTTON* constants indicating which board buttons are currently pressed.
	 */
	static inline uint_reg_t Buttons_GetStatus(void) ATTR_WARN_UNUSED_RESULT;
	static inline uint_reg_t Buttons_GetStatus(void){
		return (PINB & (BUTTONS_ALL_BUTTONS));
	}
#endif

	/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif

/** @} */

