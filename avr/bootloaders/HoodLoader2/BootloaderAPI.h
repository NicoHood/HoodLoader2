/*
             LUFA Library
     Copyright (C) Dean Camera, 2018.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2018  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
Copyright(c) 2014-2018 NicoHood
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
 *  Header file for BootloaderAPI.c.
 */

#ifndef _BOOTLOADER_API_H_
#define _BOOTLOADER_API_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/boot.h>
		#include <util/atomic.h>
		#include <stdbool.h>

		#include <LUFA/Common/Common.h>

		#include "Config/AppConfig.h"

	/* Function Prototypes: */
		void    BootloaderAPI_ErasePage(const uint32_t Address);
		void    BootloaderAPI_WritePage(const uint32_t Address);
		void    BootloaderAPI_FillWord(const uint32_t Address, const uint16_t Word);
		uint8_t BootloaderAPI_ReadSignature(const uint16_t Address);
		uint8_t BootloaderAPI_ReadFuse(const uint16_t Address);
		uint8_t BootloaderAPI_ReadLock(void);
		void    BootloaderAPI_WriteLock(const uint8_t LockBits);

#endif
