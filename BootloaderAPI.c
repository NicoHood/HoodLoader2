/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
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
 *  Bootloader user application API functions.
 */

#include "BootloaderAPI.h"

void BootloaderAPI_ErasePage(const uint32_t Address)
{
	boot_page_erase_safe(Address);
	boot_spm_busy_wait();
	boot_rww_enable();
}

void BootloaderAPI_WritePage(const uint32_t Address)
{
	boot_page_write_safe(Address);
	boot_spm_busy_wait();
	boot_rww_enable();
}

void BootloaderAPI_FillWord(const uint32_t Address, const uint16_t Word)
{
	boot_page_fill_safe(Address, Word);
}

uint8_t BootloaderAPI_ReadSignature(const uint16_t Address)
{
	return boot_signature_byte_get(Address);
}

uint8_t BootloaderAPI_ReadFuse(const uint16_t Address)
{
	return boot_lock_fuse_bits_get(Address);
}

uint8_t BootloaderAPI_ReadLock(void)
{
	return boot_lock_fuse_bits_get(GET_LOCK_BITS);
}

void BootloaderAPI_WriteLock(const uint8_t LockBits)
{
	boot_lock_bits_set_safe(LockBits);
}
