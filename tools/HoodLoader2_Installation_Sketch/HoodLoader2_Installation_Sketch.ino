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

#define VERSION "1.31-HL204-A"

//================================================================================
// HoodLoader2 Settings
//================================================================================

// make some of these false to reduce compile size (the ones you don't want)
#define USE_ATMEGA8U2 false
#define USE_ATMEGA16U2 true
#define USE_ATMEGA32U2 false
#define USE_AT90USB162 false // todo, not supported

// also use Mega 16u2 bootloader files?
#define USE_MEGA_BOOTLOADER true

// force Mega bootloader for blind pin selection/DFU hex file
#define FORCE_MEGA_BOOTLOADER false

// DFU options (16u2 only)
#ifdef USE_ATMEGA16U2
#define USE_ATMEGA16U2_DFU true
// use the full 16kb DFU Bootloader with USB-Serial
// you have to deactuivate the mega bootloader then
// but you can stull burn a mega DFU file in the followed setting
#define USE_ATMEGA16U2_DFU_FULL false
#endif

#define BUTTON_DFU 8
#define BUTTON_HOODLOADER2 7

enum {
  HL2_UNO,
  HL2_MEGA,
  DFU
} board;

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

// indicates if programming + verification was successfull
bool success;

const int ENTER_PROGRAMMING_ATTEMPTS = 50;

#include <SPI.h>
#include <avr/pgmspace.h>

const unsigned long BAUD_RATE = 115200;

const byte CLOCKOUT = 9;
const byte RESET = 10;  // --> goes to reset on the target board

#if ARDUINO < 100
const byte SCK = 13;    // SPI clock
#endif

// number of items in an array
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

#define xstr(s) str(s)
#define str(s) #s

// programming commands to send via SPI to the chip
enum {
  progamEnable = 0xAC,

  // writes are preceded by progamEnable
  chipErase = 0x80,
  writeLockByte = 0xE0,
  writeLowFuseByte = 0xA0,
  writeHighFuseByte = 0xA8,
  writeExtendedFuseByte = 0xA4,

  pollReady = 0xF0,

  programAcknowledge = 0x53,

  readSignatureByte = 0x30,
  readCalibrationByte = 0x38,

  readLowFuseByte = 0x50,       readLowFuseByteArg2 = 0x00,
  readExtendedFuseByte = 0x50,  readExtendedFuseByteArg2 = 0x08,
  readHighFuseByte = 0x58,      readHighFuseByteArg2 = 0x08,
  readLockByte = 0x58,          readLockByteArg2 = 0x00,

  readProgramMemory = 0x20,
  writeProgramMemory = 0x4C,
  loadExtendedAddressByte = 0x4D,
  loadProgramMemory = 0x40,

};  // end of enum

// structure to hold signature and other relevant data about each chip
typedef struct {
  byte sig [3];
  char * desc;
  unsigned long flashSize;
  unsigned int baseBootSize;
  const byte * bootloader;
  unsigned long loaderStart;  // bytes
  unsigned int loaderLength;  // bytes
  unsigned long pageSize;     // bytes
  byte lowFuse, highFuse, extFuse, lockByte;
  byte timedWrites;    // if pollUntilReady won't work by polling the chip
} signatureType;

const unsigned long kb = 1024;

// hex bootloader data
#if USE_ATMEGA8U2
#include "bootloader_atmega8u2.h"
#endif
#if USE_ATMEGA16U2
#include "bootloader_atmega16u2.h"
#endif
#if USE_ATMEGA32U2
#include "bootloader_atmega32u2.h"
#endif
#if USE_AT90USB162
#include "bootloader_at90usb162.h"
#endif

// see Atmega328 datasheet page 298
signatureType signatures [] =
{
  //     signature          description   flash size  bootloader size

  // Attiny84 family
  { { 0x1E, 0x91, 0x0B }, "ATtiny24",   2 * kb,         0 },
  { { 0x1E, 0x92, 0x07 }, "ATtiny44",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0C }, "ATtiny84",   8 * kb,         0 },

  // Attiny85 family
  { { 0x1E, 0x91, 0x08 }, "ATtiny25",   2 * kb,         0 },
  { { 0x1E, 0x92, 0x06 }, "ATtiny45",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0B }, "ATtiny85",   8 * kb,         0 },

  // Atmega328 family
  { { 0x1E, 0x92, 0x0A }, "ATmega48PA",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0F }, "ATmega88PA",   8 * kb,       256 },
  { { 0x1E, 0x94, 0x0B }, "ATmega168PA", 16 * kb,       256 },
  { { 0x1E, 0x95, 0x0F }, "ATmega328P",  32 * kb,       512 },
  { { 0x1E, 0x95, 0x14 }, "ATmega328",  32 * kb,        512 },

  // Atmega644 family
  { { 0x1E, 0x94, 0x0A }, "ATmega164P",   16 * kb,      256 },
  { { 0x1E, 0x95, 0x08 }, "ATmega324P",   32 * kb,      512 },
  { { 0x1E, 0x96, 0x0A }, "ATmega644P",   64 * kb,   1 * kb },

  // Atmega2560 family
  { { 0x1E, 0x96, 0x08 }, "ATmega640",    64 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x03 }, "ATmega1280",  128 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x04 }, "ATmega1281",  128 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x01 }, "ATmega2560",  256 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x02 }, "ATmega2561",  256 * kb,   1 * kb },

  // Atmega32U2 family
#if USE_ATMEGA8U2
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512,
    HoodLoader2_0_4_Uno_8u2_hex,// loader image
    0x1000, // start address
    sizeof HoodLoader2_0_4_Uno_8u2_hex,
    128, // page size in bytes (for committing)
    0xEF, // fuse low byte: external clock, m
    0xD8, // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
    0xF4, // fuse extended byte: brown-out detection at 2.6V
    0xCF // lock bits
  },
#else
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512 },
#endif

#if USE_ATMEGA16U2
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2", 16 * kb, 512,
    HoodLoader2_0_4_Uno_16u2_hex,// loader image
    0x3000, // start address
    sizeof HoodLoader2_0_4_Uno_16u2_hex,
    128, // page size in bytes (for committing)
    0xEF, // fuse low byte: external clock, m
    0xD8, // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
    0xF4, // fuse extended byte: brown-out detection at 2.6V
    0xCF // lock bits
  },
#else
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2", 16 * kb, 512 },
#endif

#if USE_ATMEGA32U2
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2", 32 * kb, 512,
    HoodLoader2_0_4_Uno_32u2_hex,// loader image
    0x7000, // start address
    sizeof HoodLoader2_0_4_Uno_32u2_hex,
    128, // page size in bytes (for committing)
    0xEF, // fuse low byte: external clock, m
    0xD8, // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
    0xF4, // fuse extended byte: brown-out detection at 2.6V
    0xCF // lock bits
  },
#else
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2", 32 * kb, 512 },
#endif

  // Atmega32U4 family
  { { 0x1E, 0x94, 0x88 }, "ATmega16U4",  16 * kb,   512 },
  { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  32 * kb,   4 * kb },

  // ATmega1284P family
  { { 0x1E, 0x97, 0x05 }, "ATmega1284P", 128 * kb,   1 * kb },

  // ATtiny4313 family
  { { 0x1E, 0x91, 0x0A }, "ATtiny2313A", 2 * kb,   0 },
  { { 0x1E, 0x92, 0x0D }, "ATtiny4313",  4 * kb,   0 },

  // ATtiny13 family
  { { 0x1E, 0x90, 0x07 }, "ATtiny13A",     1 * kb, 0 },

  // Atmega8A family
  { { 0x1E, 0x93, 0x07 }, "ATmega8A",     8 * kb, 256 },

};  // end of signatures

// if signature found in above table, this is its index
int foundSig = -1;
byte lastAddressMSB = 0;

// execute one programming instruction ... b1 is command, b2, b3, b4 are arguments
//  processor may return a result on the 4th transfer, this is returned.
byte program (const byte b1, const byte b2 = 0, const byte b3 = 0, const byte b4 = 0)
{
  SPI.transfer (b1);
  SPI.transfer (b2);
  SPI.transfer (b3);
  return SPI.transfer (b4);
} // end of program

// read a byte from flash memory
byte readFlash (unsigned long addr)
{
  byte high = (addr & 1) ? 0x08 : 0;  // set if high byte wanted
  addr >>= 1;  // turn into word address

  // set the extended (most significant) address byte if necessary
  byte MSB = (addr >> 16) & 0xFF;
  if (MSB != lastAddressMSB)
  {
    program (loadExtendedAddressByte, 0, MSB);
    lastAddressMSB = MSB;
  }  // end if different MSB

  return program (readProgramMemory | high, highByte (addr), lowByte (addr));
} // end of readFlash

// write a byte to the flash memory buffer (ready for committing)
byte writeFlash (unsigned long addr, const byte data)
{
  byte high = (addr & 1) ? 0x08 : 0;  // set if high byte wanted
  addr >>= 1;  // turn into word address
  program (loadProgramMemory | high, 0, lowByte (addr), data);
} // end of writeFlash

// show a byte in hex with leading zero and optional newline
void showHex (const byte b, const boolean newline = false, const boolean show0x = true)
{
  if (show0x)
    Serial.print (F("0x"));
  // try to avoid using sprintf
  char buf [4] = { ((b >> 4) & 0x0F) | '0', (b & 0x0F) | '0', ' ' , 0 };
  if (buf [0] > '9')
    buf [0] += 7;
  if (buf [1] > '9')
    buf [1] += 7;
  Serial.print (buf);
  if (newline)
    Serial.println ();
}  // end of showHex

// convert a boolean to Yes/No
void showYesNo (const boolean b, const boolean newline = false)
{
  if (b)
    Serial.print (F("Yes"));
  else
    Serial.print (F("No"));
  if (newline)
    Serial.println ();
}  // end of showYesNo

// poll the target device until it is ready to be programmed
void pollUntilReady ()
{
  if (signatures [foundSig].timedWrites)
    delay (10);  // at least 2 x WD_FLASH which is 4.5 mS
  else
  {
    while ((program (pollReady) & 1) == 1)
    {}  // wait till ready
  }  // end of if
}  // end of pollUntilReady

// commit page
void commitPage (unsigned long addr)
{
  Serial.print (F("Committing page starting at 0x"));
  Serial.println (addr, HEX);

  addr >>= 1;  // turn into word address

  // set the extended (most significant) address byte if necessary
  byte MSB = (addr >> 16) & 0xFF;
  if (MSB != lastAddressMSB)
  {
    program (loadExtendedAddressByte, 0, MSB);
    lastAddressMSB = MSB;
  }  // end if different MSB

  program (writeProgramMemory, highByte (addr), lowByte (addr));
  pollUntilReady ();
}  // end of commitPage

// write specified value to specified fuse/lock byte
void writeFuse (const byte newValue, const byte instruction)
{
  if (newValue == 0)
    return;  // ignore

  program (progamEnable, instruction, 0, newValue);
  pollUntilReady ();
}  // end of writeFuse

void getFuseBytes ()
{
  Serial.print (F("LFuse = "));
  showHex (program (readLowFuseByte, readLowFuseByteArg2), true);
  Serial.print (F("HFuse = "));
  showHex (program (readHighFuseByte, readHighFuseByteArg2), true);
  Serial.print (F("EFuse = "));
  showHex (program (readExtendedFuseByte, readExtendedFuseByteArg2), true);
  Serial.print (F("Lock byte = "));
  showHex (program (readLockByte, readLockByteArg2), true);
  Serial.print (F("Clock calibration = "));
  showHex (program (readCalibrationByte), true);
}  // end of getFuseBytes

// burn the bootloader to the target device
void writeBootloader ()
{

  if (signatures [foundSig].bootloader == 0)
  {
    Serial.println (F("No bootloader support for this device."));
    return;
  }  // end if

  int i;

  byte lFuse = program (readLowFuseByte, readLowFuseByteArg2);

  byte newlFuse = signatures [foundSig].lowFuse;
  byte newhFuse = signatures [foundSig].highFuse;
  byte newextFuse = signatures [foundSig].extFuse;
  byte newlockByte = signatures [foundSig].lockByte;


  unsigned long addr = signatures [foundSig].loaderStart;
  unsigned int  len = signatures [foundSig].loaderLength;
  unsigned long pagesize = signatures [foundSig].pageSize;
  unsigned long pagemask = ~(pagesize - 1);
  const byte * bootloader = signatures [foundSig].bootloader;


  // Atmega u2 Series with HoodLoader2
  if (board == DFU)
  {
#if USE_ATMEGA16U2
#if USE_ATMEGA16U2_DFU
    // Atmega16u2
    if (signatures [foundSig].sig [0] == 0x1E &&
        signatures [foundSig].sig [1] == 0x95 &&
        signatures [foundSig].sig [2] == 0x89) {

#if USE_ATMEGA16U2_DFU_FULL
#if defined(ARDUINO_AVR_MEGA2560) || FORCE_MEGA_BOOTLOADER
      bootloader = Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full, // loader image
      len = sizeof(Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full);
#else
      bootloader = Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex_Full, // loader image
      len = sizeof(Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex_Full);
#endif
      addr = 0x0000, // start address
#else
#if defined(ARDUINO_AVR_MEGA2560) || FORCE_MEGA_BOOTLOADER
#error no Mega support for smaller DFU firmware yet
#else
      bootloader = Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex, // loader image
      len = sizeof(Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex);
      addr = 0x3000, // start address
#endif
#endif

      newhFuse = 0xD9;  // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
    }
#endif
#endif
  } // end DFU

#if USE_MEGA_BOOTLOADER
  if (board == HL2_MEGA) {
#if USE_ATMEGA8U2
    // Atmega8u2
    if (signatures [foundSig].sig [0] == 0x1E &&
        signatures [foundSig].sig [1] == 0x94 &&
        signatures [foundSig].sig [2] == 0x89) {
      bootloader = HoodLoader2_0_4_Mega_8u2_hex, // loader image
      len = sizeof(HoodLoader2_0_4_Mega_8u2_hex);
    }
#endif

#if USE_ATMEGA16U2
    // Atmega16u2
    if (signatures [foundSig].sig [0] == 0x1E &&
        signatures [foundSig].sig [1] == 0x95 &&
        signatures [foundSig].sig [2] == 0x89) {
      bootloader = HoodLoader2_0_4_Mega_16u2_hex, // loader image
      len = sizeof(HoodLoader2_0_4_Mega_16u2_hex);
    }
#endif

#if USE_ATMEGA32U2
    // Atmega32u2
    if (signatures [foundSig].sig [0] == 0x1E &&
        signatures [foundSig].sig [1] == 0x96 &&
        signatures [foundSig].sig [2] == 0x89) {
      bootloader = HoodLoader2_0_4_Mega_32u2_hex, // loader image
      len = sizeof(HoodLoader2_0_4_Mega_32u2_hex);
    }
#endif
  } // end Mega
#endif


  Serial.print (F("Bootloader address = 0x"));
  Serial.println (addr, HEX);
  Serial.print (F("Bootloader length = "));
  Serial.print (len);
  Serial.println (F(" bytes."));


  unsigned long oldPage = addr & pagemask;

  Serial.println (F("Programming the chip with the bootloader ..."));

  // Automatically fix up fuse to run faster, then write to device
  if (lFuse != newlFuse)
  {
    if ((lFuse & 0x80) == 0)
      Serial.println (F("Clearing 'Divide clock by 8' fuse bit."));

    Serial.println (F("Fixing low fuse setting ..."));
    writeFuse (newlFuse, writeLowFuseByte);
    delay (1000);
    digitalWrite (RESET, HIGH); // latch fuse
    if (!startProgramming ())
      return;
    delay (1000);
  }

  Serial.println (F("Erasing chip ..."));
  program (progamEnable, chipErase);   // erase it
  delay (20);  // for Atmega8
  pollUntilReady ();
  Serial.println (F("Writing bootloader ..."));
  for (i = 0; i < len; i += 2)
  {
    unsigned long thisPage = (addr + i) & pagemask;
    // page changed? commit old one
    if (thisPage != oldPage)
    {
      commitPage (oldPage);
      oldPage = thisPage;
    }
    writeFlash (addr + i, pgm_read_byte(bootloader + i));
    writeFlash (addr + i + 1, pgm_read_byte(bootloader + i + 1));
  }  // end while doing each word

  // commit final page
  commitPage (oldPage);
  Serial.println (F("Written."));
  // end if programming

  Serial.println (F("Verifying ..."));

  // count errors
  unsigned int errors = 0;
  // check each byte
  for (i = 0; i < len; i++)
  {
    byte found = readFlash (addr + i);
    byte expected = pgm_read_byte(bootloader + i);
    if (found != expected)
    {
      if (errors <= 100)
      {
        Serial.print (F("Verification error at address "));
        Serial.print (addr + i, HEX);
        Serial.print (F(". Got: "));
        showHex (found);
        Serial.print (F(" Expected: "));
        showHex (expected, true);
      }  // end of haven't shown 100 errors yet
      errors++;
    }  // end if error
  }  // end of for

  if (errors == 0) {
    Serial.println (F("No errors found."));
    success = true;

    Serial.println (F("Writing fuses ..."));
    writeFuse (newlFuse, writeLowFuseByte);
    writeFuse (newhFuse, writeHighFuseByte);
    writeFuse (newextFuse, writeExtendedFuseByte);
    writeFuse (newlockByte, writeLockByte);

    // confirm them
    getFuseBytes ();
  }
  else
  {
    Serial.print (errors, DEC);
    Serial.println (F(" verification error(s)."));
    if (errors > 100)
      Serial.println (F("First 100 shown."));
    return;  // don't change fuses if errors
  }  // end if

  Serial.println (F("Done."));

} // end of writeBootloader


bool startProgramming ()
{
  Serial.print (F("Attempting to enter programming mode ..."));
  digitalWrite (RESET, HIGH);  // ensure SS stays high for now
  SPI.begin ();
  SPI.setClockDivider (SPI_CLOCK_DIV64);

  byte confirm;
  pinMode (RESET, OUTPUT);
  pinMode (SCK, OUTPUT);
  unsigned int timeout = 0;

  // we are in sync if we get back programAcknowledge on the third byte
  do
  {
    delay (100);
    // ensure SCK low
    digitalWrite (SCK, LOW);
    // then pulse reset, see page 309 of datasheet
    digitalWrite (RESET, HIGH);
    delay (1);  // pulse for at least 2 clock cycles
    digitalWrite (RESET, LOW);
    delay (25);  // wait at least 20 mS
    SPI.transfer (progamEnable);
    SPI.transfer (programAcknowledge);
    confirm = SPI.transfer (0);
    SPI.transfer (0);

    if (confirm != programAcknowledge)
    {
      Serial.print (F("."));
      if (timeout++ >= ENTER_PROGRAMMING_ATTEMPTS)
      {
        Serial.println ();
        Serial.println (F("Failed to enter programming mode. Double-check wiring!"));
        stopProgramming();
        return false;
      }  // end of too many attempts
    }  // end of not entered programming mode
  } while (confirm != programAcknowledge);

  Serial.println ();
  Serial.println (F("Entered programming mode OK."));
  return true;
}  // end of startProgramming

void stopProgramming ()
{
  SPI.end ();

  // turn off pull-ups, if any
  digitalWrite (RESET, LOW);
  digitalWrite (SCK,   LOW);
  digitalWrite (MOSI,  LOW);
  digitalWrite (MISO,  LOW);

  // set everything back to inputs
  pinMode (RESET, INPUT);
  pinMode (SCK,   INPUT);
  pinMode (MOSI,  INPUT);
  pinMode (MISO,  INPUT);

  Serial.println (F("Programming mode off."));
} // end of stopProgramming

void getSignature ()
{
  foundSig = -1;
  lastAddressMSB = 0;

  byte sig [3];
  Serial.print (F("Signature = "));
  for (byte i = 0; i < 3; i++)
  {
    sig [i] = program (readSignatureByte, 0, i);
    showHex (sig [i]);
  }  // end for each signature byte
  Serial.println ();

  for (int j = 0; j < NUMITEMS (signatures); j++)
  {
    if (memcmp (sig, signatures [j].sig, sizeof sig) == 0)
    {
      foundSig = j;
      Serial.print (F("Processor = "));
      Serial.println (signatures [j].desc);
      Serial.print (F("Flash memory size = "));
      Serial.print (signatures [j].flashSize, DEC);
      Serial.println (F(" bytes."));
      if (signatures [foundSig].timedWrites)
        Serial.println (F("Writes are timed, not polled."));
      return;
    }  // end of signature found
  }  // end of for each signature

  Serial.println (F("Unrecogized signature."));
}  // end of getSignature

void setup ()
{
  Serial.begin (BAUD_RATE);
  while (!Serial) ;  // for Leonardo, Micro etc.
  Serial.println ();
  Serial.println (F("Atmega chip programmer."));
  Serial.println (F("Written by Nick Gammon, modified by NicoHood."));
  Serial.println (F("Version " VERSION));
  Serial.println (F("Compiled on " __DATE__ " at " __TIME__ " with Arduino IDE " xstr(ARDUINO) "."));

  // setup backup buttons
  pinMode(BUTTON_DFU, INPUT_PULLUP);
  pinMode(BUTTON_HOODLOADER2, INPUT_PULLUP);

  digitalWrite (RESET, HIGH);  // ensure SS stays high for now
  pinMode (CLOCKOUT, OUTPUT);

  // set up Timer 1
  TCCR1A = bit (COM1A0);  // toggle OC1A on Compare Match
  TCCR1B = bit (WGM12) | bit (CS10);   // CTC, no prescaling
  OCR1A =  0;       // output every cycle

}  // end of setup

void loop ()
{
  Serial.println();
  Serial.println(F("Welcome to the HoodLoader2 installation sketch!"));
  Serial.println(F("This version will install HoodLoader2 on your Arduino Uno or Mega"));
  Serial.println();
  Serial.println(F("Make sure all wires are connected as written in the documentation."));
  Serial.println(F("Please now insert the 100nF capacitor between Reset and Ground on a standalone Arduino."));
  Serial.println(F("In some cases the Arduino will reset then and display these messages again."));
  Serial.println();
  Serial.println(F("Select the Arduino board you are using."));
#if USE_ATMEGA16U2_DFU
  Serial.println(F("Type 'U' for Uno or 'M' for Mega and press enter. (Advanced: 'D' for DFU)"));
#else
  Serial.println(F("Type 'U' for Uno or 'M' for Mega and press enter."));
#endif
  Serial.println();

  // wait for user input
  while (true)
  {
    char c = toupper (Serial.read ());
    if (c == 'U' ) {
      board = HL2_UNO;
      break;
    }
    if (c == 'M' ) {
      board = HL2_MEGA;
      break;
    }
    if (c == 'A' || !digitalRead(BUTTON_HOODLOADER2)) {
      // depending on the uploading board decide what hex file we should use (for the blind pin selection mode)
      // by default use the file for Arduino Uno. Its no problem to upload an Uno file to a Mega though.
#if defined(ARDUINO_AVR_MEGA2560) || FORCE_MEGA_BOOTLOADER
      board = HL2_MEGA;
#else
      board = HL2_UNO;
#endif
      break;
    }
    if (c == 'D' || !digitalRead(BUTTON_DFU)) {
      board = DFU;
      break;
    }
  }

  Serial.println(F("Starting programming, please be patient and wait some time."));
  Serial.println();
  Serial.println(F("On a standalone Arduino you will no longer see any messages here."));
  Serial.println(F("If you uploaded this sketch to a 2nd Aruino you will see some debug output."));
  Serial.println();
  Serial.println(F("If the installation was successfull the on board Led will start blinking fast (every 100ms)."));
  Serial.println(F("If the installation failed the on board Led will start blinking slow (every second)."));
  Serial.println();
  Serial.flush();

  success = false;
  if (startProgramming ())
  {
    getSignature ();
    getFuseBytes ();

    // if we found a signature try to write a bootloader
    if (foundSig != -1)
      writeBootloader ();
    stopProgramming ();
  }   // end of if entered programming mode OK

  Serial.println (F("Type 'C' when ready to continue with another chip ..."));

  int speed;
  if (success)
    speed = 100;
  else
    speed = 1000;

  pinMode(LED_BUILTIN, OUTPUT);
  while (toupper (Serial.read ()) != 'C')
  {
    // blink led
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= speed) {
      previousMillis = currentMillis;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
  pinMode(LED_BUILTIN, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

}  // end of loop

