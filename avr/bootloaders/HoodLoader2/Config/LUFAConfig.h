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
 *  \brief LUFA Library Configuration Header File
 *
 *  This header file is used to configure LUFA's compile time options,
 *  as an alternative to the compile time constants supplied through
 *  a makefile.
 *
 *  For information on what each token does, refer to the LUFA
 *  manual section "Summary of Compile Tokens".
 */

#ifndef _LUFA_CONFIG_H_
#define _LUFA_CONFIG_H_

	#if (ARCH == ARCH_AVR8)

// Pull in RAMEND definitions
#include <avr/io.h>

		/* Non-USB Related Configuration Tokens: */
//		#define DISABLE_TERMINAL_CODES

		/* USB Class Driver Related Tokens: */
//		#define HID_HOST_BOOT_PROTOCOL_ONLY
//		#define HID_STATETABLE_STACK_DEPTH       {Insert Value Here}
//		#define HID_USAGE_STACK_DEPTH            {Insert Value Here}
//		#define HID_MAX_COLLECTIONS              {Insert Value Here}
//		#define HID_MAX_REPORTITEMS              {Insert Value Here}
//		#define HID_MAX_REPORT_IDS               {Insert Value Here}
//		#define NO_CLASS_DRIVER_AUTOFLUSH

		/* General USB Driver Related Tokens: */
		#define ORDERED_EP_CONFIG
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
		#define USB_DEVICE_ONLY
//		#define USB_HOST_ONLY
//		#define USB_STREAM_TIMEOUT_MS            {Insert Value Here}
//		#define NO_LIMITED_CONTROLLER_CONNECT
		#define NO_SOF_EVENTS

		/* USB Device Mode Driver Related Tokens: */
// Only use RAM Descriptors if we have enough ram
#if ((RAMEND - RAMSTART) >= 512)
		#define USE_RAM_DESCRIPTORS
#else
		#define USE_FLASH_DESCRIPTORS
#endif

//		#define USE_EEPROM_DESCRIPTORS
		#define NO_INTERNAL_SERIAL
		#define FIXED_CONTROL_ENDPOINT_SIZE      8
		#define DEVICE_STATE_AS_GPIOR            2 // GPIOR0 is used for the RX ISR with cbi()
		#define FIXED_NUM_CONFIGURATIONS         1
//		#define CONTROL_ONLY_DEVICE
//		#define INTERRUPT_CONTROL_ENDPOINT
		#define NO_DEVICE_REMOTE_WAKEUP
		#define NO_DEVICE_SELF_POWER

		/* USB Host Mode Driver Related Tokens: */
//		#define HOST_STATE_AS_GPIOR              {Insert Value Here}
//		#define USB_HOST_TIMEOUT_MS              {Insert Value Here}
//		#define HOST_DEVICE_SETTLE_DELAY_MS	     {Insert Value Here}
//		#define NO_AUTO_VBUS_MANAGEMENT
//		#define INVERTED_VBUS_ENABLE_LINE

	#else

		#error Unsupported architecture for this LUFA configuration file.

	#endif
#endif
