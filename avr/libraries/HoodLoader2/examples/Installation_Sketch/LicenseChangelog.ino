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


/*

 Copyright 2012 Nick Gammon.


 PERMISSION TO DISTRIBUTE

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.


 LIMITATION OF LIABILITY

 The software is provided "as is", without warranty of any kind, express or implied,
 including but not limited to the warranties of merchantability, fitness for a particular
 purpose and noninfringement. In no event shall the authors or copyright holders be liable
 for any claim, damages or other liability, whether in an action of contract,
 tort or otherwise, arising from, out of or in connection with the software
 or the use or other dealings in the software.

*/

// Atmega chip programmer
// Author: Nick Gammon
// Date: 22nd May 2012

// For more information including wiring, see: http://www.gammon.com.au/forum/?id=11635

// Version 1.1: Reset foundSig to -1 each time around the loop.
// Version 1.2: Put hex bootloader data into separate files
// Version 1.3: Added verify, and MD5 sums
// Version 1.4: Added signatures for ATmeag8U2/16U2/32U2 (7 May 2012)
// Version 1.5: Added signature for ATmega1284P (8 May 2012)
// Version 1.6: Allow sketches to read bootloader area (lockbyte: 0x2F)
// Version 1.7: Added choice of bootloaders for the Atmega328P (8 MHz or 16 MHz)
// Version 1.8: Output an 8 MHz clock on pin 9
// Version 1.9: Added support for Atmega1284P, and fixed some bugs
// Version 1.10: Corrected flash size for Atmega1284P.
// Version 1.11: Added support for Atmega1280. Removed MD5SUM stuff to make room.
// Version 1.12: Added signatures for ATtiny2313A, ATtiny4313, ATtiny13
// Version 1.13: Added signature for Atmega8A
// Version 1.14: Added bootloader for Atmega8
// Version 1.15: Removed extraneous 0xFF from some files
// Version 1.16: Added signature for Atmega328
// Version 1.17: Allowed for running on the Leonardo, Micro, etc.
// Version 1.18: Added timed writing for Atmega8
// Version 1.19: Changed Atmega1280 to use the Optiboot loader.
// Version 1.20: Changed bootloader for Atmega2560 to fix problems with watchdog timer.
// Version 1.21: Automatically clear "divide by 8" fuse bit
// Version 1.22: Fixed compiling problems under IDE 1.5.8
// Version 1.23: Added support for Leonardo bootloader
// Version 1.24: Added bootloader for Uno Atmega16U2 chip (the USB interface)
// Version 1.25: Fixed bug re verifying uploaded sketch for the Lilypad
// Version 1.26: Turn off programming mode when done (so chip can run)
// Version 1.27: Made bootloaders conditional, so you can omit some to save space
// Version 1.28: Changed _BV () macro to bit () macro.
// Version 1.29: Display message if cannot enter programming mode.
// Version 1.30: Various tidy-ups
// Version 1.31: Fixed bug in doing second lot of programming under IDE 1.6.0
// Version 1.32: Bug fixes, added support for At90USB82, At90USB162 signatures

// Changes by NicoHood:
// Version 1.23: Added support for Leonardo bootloader
// Version 1.23-A: Removed unnecessary Bootloaders, added Hoodloader
// Version 1.23-B: Fixed resetting bug when connected directly
// Version 1.24: Added bootloader for Uno Atmega16U2 chip (the USB interface)
// Version 1.25: Fixed bug re verifying uploaded sketch for the Lilypad
// Version 1.25-A: Fixed uploading problems with a standalone Arduino
// Version 1.26-A: Turn off programming mode when done (so chip can run)
// Version 1.26-B: Updated to HoodLoader 2.0.1
// Version 1.26-C: Updated to HoodLoader 2.0.2
// Version 1.26-D: Corrected Fuses to original Arduino values
// Version 1.26-E: DFU lock bytes fix
// Version 1.27: Added HoodLoader2.0.3, also for 32u2 (IDE 1.5.8 or higher)
// Version 1.27-B: Fixed Mega uploading, added timeout, added Leonardo compatibity again (flash size problem)
// Version 1.27-C: Outcommented 32u2 due to a 4kb 328 bootloader problem if no optiboot is installed
// Version 1.29-HL203-A: Updated to 1.29, reworked the whole sketch
// Version 1.30-HL204-A: Updated HL to version 2.0.4
// Version 1.31-HL204-A: Added 8u2, 16u2, 32u2 Support
// Version 1.32-HL204-A: Bug fixes, added support for At90USB82, At90USB162 signatures
// Version 1.32-HL204-B: Signature fixes
// Version 1.32-HL205-A: Updated to HL2.0.5, only supports a single hex file now
// Version 1.32-HL205-A: Updated to the latest HL2.0.5 hex files, added 1.6.6 compatibility
