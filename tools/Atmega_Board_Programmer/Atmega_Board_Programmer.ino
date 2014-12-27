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

// Atmega chip programmer
// Author: Nick Gammon, modified by NicoHood
// Date: 22nd May 2012
// Version: 1.25

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
// Version 1.23-A: Removed unnecessary Bootloaders, added Hoodloader
// Version 1.23-B: Fixed resetting bug when connected directly
// Version 1.24: Added bootloader for Uno Atmega16U2 chip (the USB interface)
// Version 1.25: Fixed bug re verifying uploaded sketch for the Lilypad
// Version 1.25-A: Fixed uploading problems with a standalone Arduino
// Version 1.26-A: Turn off programming mode when done (so chip can run)
// Version 1.26-B: Updated to HoodLoader 2.0.1
// Version 1.26-C: Updated to HoodLoader 2.0.2
// Version 1.26-D: Corrected Fuses to original Arduino values

#define VERSION "1.26-D"

//================================================================================
// HoodLoader2 definitions
//================================================================================

// depending on the uploading board decide what hex file we should use
#ifdef ARDUINO_AVR_MEGA2560
#define HOODLOADER2_16U2_MEGA
#elif defined ARDUINO_AVR_UNO
#define HOODLOADER2_16U2_UNO
#else
// by default use the file for Arduino Uno. Its no problem to upload an Uno file to a Mega though.
#define HOODLOADER2_16U2_UNO
#endif

#define BUTTON_DFU 8
#define BUTTON_HOODLOADER2 7

char force16u2;

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
 
#include <SPI.h>
#include <avr/pgmspace.h>

#define SERIAL Serial
const unsigned long BAUD_RATE = 115200;

const byte CLOCKOUT = 9;
const byte RESET = 10;  // --> goes to reset on the target board

#if ARDUINO < 100
  const byte SCK = 13;    // SPI clock
#endif

// number of items in an array
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

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
#include "bootloader_atmega16u2.h"
#include "bootloader_atmega16u2_HoodLoader2.h"

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

  { { 0x1E, 0x95, 0x14 }, "ATmega328",  32 * kb,       512 },
        
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
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512 },
  
#ifdef HOODLOADER2_16U2_MEGA
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   512 ,
        Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex,   // loader image
        0x0000,                   // start address (0x0000-0x4000)
        sizeof Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex, 
        128, // page size in bytes (for committing)
        0xEF, // fuse low byte: external clock, m
        0xD9, // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
        0xF4, // fuse extended byte: brown-out detection at 2.6V
        0xCF }, // lock bits
       
#else // HOODLOADER2_16U2_UNO
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   512 ,
        Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex,   // loader image
        0x0000,                   // start address (0x0000-0x4000)
        sizeof Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex, 
        128, // page size in bytes (for committing)
        0xEF, // fuse low byte: external clock, m
        0xD9, // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
        0xF4, // fuse extended byte: brown-out detection at 2.6V
        0xCF }, // lock bits
        
#endif
  
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2",  32 * kb,   512 },

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
    SERIAL.print (F("0x"));
  // try to avoid using sprintf
  char buf [4] = { ((b >> 4) & 0x0F) | '0', (b & 0x0F) | '0', ' ' , 0 };
  if (buf [0] > '9')
    buf [0] += 7;
  if (buf [1] > '9')
    buf [1] += 7;
  SERIAL.print (buf);
  if (newline)
    SERIAL.println ();
  }  // end of showHex 
 
// convert a boolean to Yes/No 
void showYesNo (const boolean b, const boolean newline = false)
  {
  if (b)
    SERIAL.print (F("Yes"));
  else
    SERIAL.print (F("No"));
  if (newline)
    SERIAL.println ();
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
  SERIAL.print (F("Committing page starting at 0x"));
  SERIAL.println (addr, HEX);

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
  SERIAL.print (F("LFuse = "));
  showHex (program (readLowFuseByte, readLowFuseByteArg2), true);
  SERIAL.print (F("HFuse = "));
  showHex (program (readHighFuseByte, readHighFuseByteArg2), true);
  SERIAL.print (F("EFuse = "));
  showHex (program (readExtendedFuseByte, readExtendedFuseByteArg2), true);
  SERIAL.print (F("Lock byte = "));
  showHex (program (readLockByte, readLockByteArg2), true);
  SERIAL.print ("Clock calibration = ");
  showHex (program (readCalibrationByte), true);  
  }  // end of getFuseBytes

// burn the bootloader to the target device
void writeBootloader ()
  {

  if (signatures [foundSig].bootloader == 0)
    {
    SERIAL.println (F("No bootloader support for this device."));
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

   
  SERIAL.print (F("Bootloader address = 0x"));
  SERIAL.println (addr, HEX);
  SERIAL.print (F("Bootloader length = "));
  SERIAL.print (len);
  SERIAL.println (F(" bytes."));

  byte subcommand = 'U';
  
  // 16u2: DFU + USB-SERIAL or HoodLoader2
  if (signatures [foundSig].sig [0] == 0x1E &&
      signatures [foundSig].sig [1] == 0x94 &&
      signatures [foundSig].sig [2] == 0x89)
    {
      // HoodLoader 2 save mode
      if(force16u2 == 'H' || force16u2 == 'D')
         subcommand = force16u2;
      else{
        SERIAL.println (F("Type 'H' to use HoodLoader2, or 'D' for the original DFU + USB-SERIAL firmware."));   
        do
          {
          subcommand = toupper (SERIAL.read ());
          
          } while (subcommand != 'H' && subcommand != 'D');
      }
    
    if (subcommand == 'H')
      {
      newlFuse = 0xFF;
      newhFuse = 0xD8;
      newextFuse = 0xFC;
      newlockByte = 0x3F;
      addr = 0x3000;
#ifdef HOODLOADER2_16U2_MEGA
      SERIAL.println (F("Using Hoodloader2 Mega."));
      bootloader = HoodLoader2_0_2_Mega_16u2_hex;
      len = sizeof HoodLoader2_0_2_Mega_16u2_hex;
#else // HOODLOADER2_16U2_UNO
      SERIAL.println (F("Using Hoodloader2 Uno."));
      bootloader = HoodLoader2_0_2_Uno_16u2_hex;
      len = sizeof HoodLoader2_0_2_Uno_16u2_hex;
#endif
      }
      
    else if (subcommand == 'D')
#ifdef HOODLOADER2_16U2_MEGA
      SERIAL.println (F("Using original DFU + USB-SERIAL loader for Mega."));
#else // HOODLOADER2_16U2_UNO
      SERIAL.println (F("Using original DFU + USB-SERIAL loader for Uno."));
#endif
     }

  unsigned long oldPage = addr & pagemask;

  SERIAL.println (F("Type 'Q' to quit, 'V' to verify, or 'G' to program the chip with the bootloader ..."));
  char command;
  do
    {
    command = toupper (SERIAL.read ());
    
    // HoodLoader 2 save mode
    if(force16u2 == 'H' || force16u2 == 'D')
      command = 'G';
      
    } while (command != 'G' && command != 'V' && command != 'Q');
    
  // let them do nothing
  if (command == 'Q')
  return;

  if (command == 'G')
    {
      
    // Automatically fix up fuse to run faster, then write to device
    if (lFuse != newlFuse)
      {
      if ((lFuse & 0x80) == 0)
        SERIAL.println (F("Clearing 'Divide clock by 8' fuse bit."));
      
      SERIAL.println (F("Fixing low fuse setting ..."));
      writeFuse (newlFuse, writeLowFuseByte);
      delay (1000);
      digitalWrite (RESET, HIGH); // latch fuse
      startProgramming ();
      delay (1000);
      }
      
    SERIAL.println (F("Erasing chip ..."));
    program (progamEnable, chipErase);   // erase it
    delay (20);  // for Atmega8
    pollUntilReady (); 
    SERIAL.println (F("Writing bootloader ..."));
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
    SERIAL.println ("Written.");
    }  // end if programming
  
  SERIAL.println (F("Verifying ..."));
  
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
        SERIAL.print (F("Verification error at address "));
        SERIAL.print (addr + i, HEX);
        SERIAL.print (F(". Got: "));
        showHex (found);
        SERIAL.print (F(" Expected: "));
        showHex (expected, true);
        }  // end of haven't shown 100 errors yet
      errors++;
      }  // end if error
    }  // end of for

  if (errors == 0)
    SERIAL.println (F("No errors found."));
  else
    {
    SERIAL.print (errors, DEC);
    SERIAL.println (F(" verification error(s)."));
    if (errors > 100)
      SERIAL.println (F("First 100 shown."));
    return;  // don't change fuses if errors
    }  // end if
    
  if (command == 'G')
    {
    SERIAL.println (F("Writing fuses ..."));
  
    writeFuse (newlFuse, writeLowFuseByte);
    writeFuse (newhFuse, writeHighFuseByte);
    writeFuse (newextFuse, writeExtendedFuseByte);
    writeFuse (newlockByte, writeLockByte);
  
    // confirm them
    getFuseBytes ();
    }  // end if programming
    
  SERIAL.println (F("Done."));

  } // end of writeBootloader
  
  
void startProgramming ()
  {
  SERIAL.println (F("Attempting to enter programming mode ..."));
  digitalWrite (RESET, HIGH); // ensure SS stays high for now
  SPI.begin ();
  SPI.setClockDivider (SPI_CLOCK_DIV64);
  
  byte confirm;
  pinMode (RESET, OUTPUT);
  pinMode (SCK, OUTPUT);
  
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
    } while (confirm != programAcknowledge);
    
  SERIAL.println (F("Entered programming mode OK."));
  }  // end of startProgramming
  
void stopProgramming ()
  {
  SPI.end ();
 
  // turn off pull-ups, if any
  digitalWrite (RESET, LOW);
  digitalWrite (SCK, LOW);
  digitalWrite (MOSI, LOW);
  digitalWrite (MISO, LOW);
 
  // set everything back to inputs
  pinMode (RESET, INPUT);
  pinMode (SCK, INPUT);
  pinMode (MOSI, INPUT);
  pinMode (MISO, INPUT);
 
  Serial.println (F("Programming mode off."));
  } // end of startProgramming

 
void getSignature ()
  {
  foundSig = -1;
  lastAddressMSB = 0;
    
  byte sig [3];
  SERIAL.print (F("Signature = "));
  for (byte i = 0; i < 3; i++)
    {
    sig [i] = program (readSignatureByte, 0, i); 
    showHex (sig [i]);
    }  // end for each signature byte
  SERIAL.println ();
  
  for (int j = 0; j < NUMITEMS (signatures); j++)
    {
    if (memcmp (sig, signatures [j].sig, sizeof sig) == 0)
      {
      foundSig = j;
      SERIAL.print (F("Processor = "));
      SERIAL.println (signatures [j].desc);
      SERIAL.print (F("Flash memory size = "));
      SERIAL.print (signatures [j].flashSize, DEC);
      SERIAL.println (F(" bytes."));
      if (signatures [foundSig].timedWrites)
        SERIAL.println (F("Writes are timed, not polled."));
      return;
      }  // end of signature found
    }  // end of for each signature

  SERIAL.println (F("Unrecogized signature."));  
  }  // end of getSignature

void setup ()
  {
  SERIAL.begin (BAUD_RATE);
  while (!SERIAL) ;  // for Leonardo, Micro etc.
  SERIAL.println ();
  SERIAL.println (F("Atmega chip programmer."));
  SERIAL.println (F("Written by Nick Gammon, modified by NicoHood."));
  SERIAL.println (F("Version " VERSION));
  SERIAL.println (F("Compiled on " __DATE__ " at " __TIME__));
  
  // setup backup buttons
  pinMode(BUTTON_DFU, INPUT_PULLUP);
  pinMode(BUTTON_HOODLOADER2, INPUT_PULLUP);

  digitalWrite (RESET, HIGH);  // ensure SS stays high for now
  SPI.begin ();
  
  // slow down SPI for benefit of slower processors like the Attiny
  SPI.setClockDivider (SPI_CLOCK_DIV64);
    
  pinMode (CLOCKOUT, OUTPUT);
  
  // set up Timer 1
  TCCR1A = _BV (COM1A0);  // toggle OC1A on Compare Match
  TCCR1B = _BV(WGM12) | _BV(CS10);   // CTC, no prescaling
  OCR1A =  0;       // output every cycle
   
 }  // end of setup

void loop () 
  {
  SERIAL.println (F("Type 'H' to use HoodLoader2, or 'D' for the original DFU + USB-SERIAL firmware."));   
  SERIAL.println (F("Type '0' to read other chips (advanced)."));   
  force16u2 = 0x00;
    do
      {
      force16u2 = toupper (SERIAL.read ());
      
      // HoodLoader 2 save mode
      if(!digitalRead(BUTTON_DFU))
        force16u2 = 'D';
      if(!digitalRead(BUTTON_HOODLOADER2))
        force16u2 = 'H';
        
      } while (force16u2 != 'H' && force16u2 != 'D'&& force16u2 != '0');
    
  startProgramming ();
  getSignature ();
  getFuseBytes ();

  // if we found a signature try to write a bootloader  
  if (foundSig != -1)
    writeBootloader ();

  stopProgramming();
  
  SERIAL.println (F("Type 'C' when ready to continue with another chip ..."));
  while (toupper (SERIAL.read ()) != 'C') 
    {
      // HoodLoader 2 save mode
      if(!digitalRead(BUTTON_DFU) || !digitalRead(BUTTON_HOODLOADER2))
        break;
    }
  }  // end of loop
  