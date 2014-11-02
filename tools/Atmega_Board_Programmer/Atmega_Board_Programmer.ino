// Atmega chip programmer
// Author: Nick Gammon
// Date: 22nd May 2012
// Version: 1.23

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

#define VERSION "1.23"

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
#include "bootloader_atmega168.h"
#include "bootloader_atmega328.h"
#include "bootloader_atmega2560_v2.h"
#include "bootloader_atmega1284.h"
#include "bootloader_lilypad328.h"
#include "bootloader_atmega1280.h"
#include "bootloader_atmega8.h"
#include "bootloader_atmega32u4.h"
#include "bootloader_atmega16u2.h"

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
  { { 0x1E, 0x94, 0x0B }, "ATmega168PA", 16 * kb,       256
    #ifdef ATMEGA168_HEX
  ,
        atmega168_optiboot,   // loader image
        0x3E00,               // start address
        sizeof atmega168_optiboot, 
        128,          // page size in bytes (for committing)
        0xC6,         // fuse low byte: external full-swing crystal
        0xDD,         // fuse high byte: SPI enable, brown-out detection at 2.7V
        0x04,         // fuse extended byte: boot into bootloader, 512 byte bootloader
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
                #else
        },
        #endif
  
  { { 0x1E, 0x95, 0x0F }, "ATmega328P",  32 * kb,       512, 
        atmega328_optiboot,   // loader image
        0x7E00,               // start address
        sizeof atmega328_optiboot, 
        128,          // page size in bytes (for committing)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xDE,         // fuse high byte: SPI enable, boot into bootloader, 512 byte bootloader
        0x05,         // fuse extended byte: brown-out detection at 2.7V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.

  { { 0x1E, 0x95, 0x14 }, "ATmega328",  32 * kb,       512, 
        atmega328_optiboot,   // loader image
        0x7E00,               // start address
        sizeof atmega328_optiboot, 
        128,          // page size in bytes (for committing)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xDE,         // fuse high byte: SPI enable, boot into bootloader, 512 byte bootloader
        0x05,         // fuse extended byte: brown-out detection at 2.7V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
        
  // Atmega644 family
  { { 0x1E, 0x94, 0x0A }, "ATmega164P",   16 * kb,      256 },
  { { 0x1E, 0x95, 0x08 }, "ATmega324P",   32 * kb,      512 },
  { { 0x1E, 0x96, 0x0A }, "ATmega644P",   64 * kb,   1 * kb },

  // Atmega2560 family
  { { 0x1E, 0x96, 0x08 }, "ATmega640",    64 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x03 }, "ATmega1280",  128 * kb,   1 * kb
  
  #ifdef ATMEGA1280_HEX
  ,
        optiboot_atmega1280_hex,
        0x1FC00,      // start address
        sizeof optiboot_atmega1280_hex,   
        256,          // page size in bytes (for committing)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xDE,         // fuse high byte: SPI enable, boot into bootloader, 1280 byte bootloader
        0xF5,         // fuse extended byte: brown-out detection at 2.7V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
        #else
        },
        #endif
        
  { { 0x1E, 0x97, 0x04 }, "ATmega1281",  128 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x01 }, "ATmega2560",  256 * kb,   1 * kb
    #ifdef ATMEGA2560_HEX
  , 
        atmega2560_bootloader_hex,// loader image
        0x3E000,      // start address
        sizeof atmega2560_bootloader_hex,   
        256,          // page size in bytes (for committing)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xD8,         // fuse high byte: SPI enable, boot into bootloader, 8192 byte bootloader
        0xFD,         // fuse extended byte: brown-out detection at 2.7V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
              #else
        },
        #endif
  { { 0x1E, 0x98, 0x02 }, "ATmega2561",  256 * kb,   1 * kb },
  
  // Atmega32U2 family
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512 },
  
//  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   16*kb,
//        UNO_dfu_and_usbserial_combined_hex,   // loader image
//        0x0000,                   // start address (0x0000-0x4000)
//        sizeof UNO_dfu_and_usbserial_combined_hex, 
  
//  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   4*kb,
//        Hoodloader2_Uno_Beta_hex,   // loader image
//        0x3000,                   // start address (0x3000-0x4000)
//        sizeof Hoodloader2_Uno_Beta_hex, 

  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   4*kb,
        at90usb162_bl_usb_1_0_5_hex,   // loader image
        0x3000,                   // start address (0x3000-0x4000)
        sizeof at90usb162_bl_usb_1_0_5_hex, 
        
        128,           // page size in bytes (for committing)
        0xFF,         // fuse low byte:
        0xD9,         // fuse high byte: 
        0xF4,         // fuse extended byte:
        0x2F },       // lock bits:

  
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2",  32 * kb,   512 },

  // Atmega32U4 family
  { { 0x1E, 0x94, 0x88 }, "ATmega16U4",  16 * kb,   512 },
  { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  32 * kb,   4 * kb,
        leonardo_hex,// loader image
        0x7000,      // start address
        sizeof leonardo_hex,   
        128,          // page size in bytes (for committing) (datasheet is wrong about it being 128 words)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xD8,         // fuse high byte: SPI enable, boot into bootloader, 1280 byte bootloader
        0xCB,         // fuse extended byte: brown-out detection at 2.6V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
          


  // ATmega1284P family
  { { 0x1E, 0x97, 0x05 }, "ATmega1284P", 128 * kb,   1 * kb,
        optiboot_atmega1284p_hex,
        0x1FC00,      // start address
        sizeof optiboot_atmega1284p_hex,       
        256,          // page size in bytes (for committing)
        0xFF,         // fuse low byte: external clock, max start-up time
        0xDE,         // fuse high byte: SPI enable, boot into bootloader, 1024 byte bootloader
        0xFD,         // fuse extended byte: brown-out detection at 2.7V
        0x2F },       // lock bits: SPM is not allowed to write to the Boot Loader section.
  
  // ATtiny4313 family
  { { 0x1E, 0x91, 0x0A }, "ATtiny2313A", 2 * kb,   0 },
  { { 0x1E, 0x92, 0x0D }, "ATtiny4313",  4 * kb,   0 },
  
  // ATtiny13 family
  { { 0x1E, 0x90, 0x07 }, "ATtiny13A",     1 * kb, 0 },
  
  // Atmega8A family
  { { 0x1E, 0x93, 0x07 }, "ATmega8A",     8 * kb, 256,
        atmega8_hex,
        0x1C00,      // start address
        sizeof atmega8_hex,       
        64,           // page size in bytes (for committing)
        0xE4,         // fuse low byte: external clock, max start-up time
        0xCA,         // fuse high byte: SPI enable, boot into bootloader, 1024 byte bootloader
        0xFD,         // fuse extended byte: brown-out detection at 2.7V
        0x0F,         // lock bits: SPM is not allowed to write to the Boot Loader section.
        true },       // need to do timed writes, not polled ones
  
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
  Serial.print ("Clock calibration = ");
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

   
  Serial.print (F("Bootloader address = 0x"));
  Serial.println (addr, HEX);
  Serial.print (F("Bootloader length = "));
  Serial.print (len);
  Serial.println (F(" bytes."));

  byte subcommand = 'U';
  
  // Atmega328P or Atmega328
  if (signatures [foundSig].sig [0] == 0x1E &&
      signatures [foundSig].sig [1] == 0x95 &&
      (signatures [foundSig].sig [2] == 0x0F || signatures [foundSig].sig [2] == 0x14)
      )
    {
    Serial.println (F("Type 'L' to use Lilypad (8 MHz) loader, or 'U' for Uno (16 MHz) loader ..."));   
    do
      {
      subcommand = toupper (Serial.read ());
      } while (subcommand != 'L' && subcommand != 'U');
    
    if (subcommand == 'L')  // use internal 8 MHz clock
      {
      Serial.println (F("Using Lilypad 8 MHz loader."));
      bootloader = ATmegaBOOT_168_atmega328_pro_8MHz_hex;
      newlFuse = 0xE2;  // internal 8 MHz oscillator
      newhFuse = 0xDA;  //  2048 byte bootloader, SPI enabled
      addr = 0x7800;
      len = sizeof ATmegaBOOT_168_atmega328_pro_8MHz_hex;
      }  // end of using the 8 MHz clock
    else
      Serial.println (F("Using Uno Optiboot 16 MHz loader."));
     }  // end of being Atmega328P

  unsigned long oldPage = addr & pagemask;

  Serial.println (F("Type 'V' to verify, or 'G' to program the chip with the bootloader ..."));
  char command;
  do
    {
    command = toupper (Serial.read ());
    } while (command != 'G' && command != 'V');

  if (command == 'G')
    {
      
    // Automatically fix up fuse to run faster, then write to device
    if (lFuse != newlFuse)
      {
      if ((lFuse & 0x80) == 0)
        Serial.println (F("Clearing 'Divide clock by 8' fuse bit."));
      
      Serial.println (F("Fixing low fuse setting ..."));
      writeFuse (newlFuse, writeLowFuseByte);
      delay (1000);
      digitalWrite (RESET, HIGH); // latch fuse
      startProgramming ();
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
    Serial.println ("Written.");
    }  // end if programming
  
  Serial.println (F("Verifying ..."));
  
  // count errors
  unsigned int errors = 0;
  // check each byte
  for (i = 0; i < signatures [foundSig].loaderLength; i++)
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

  if (errors == 0)
    Serial.println (F("No errors found."));
  else
    {
    Serial.print (errors, DEC);
    Serial.println (F(" verification error(s)."));
    if (errors > 100)
      Serial.println (F("First 100 shown."));
    return;  // don't change fuses if errors
    }  // end if
    
  if (command == 'G')
    {
    Serial.println (F("Writing fuses ..."));
  
    writeFuse (newlFuse, writeLowFuseByte);
    writeFuse (newhFuse, writeHighFuseByte);
    writeFuse (newextFuse, writeExtendedFuseByte);
    writeFuse (newlockByte, writeLockByte);
  
    // confirm them
    getFuseBytes ();
    }  // end if programming
    
  Serial.println (F("Done."));

  } // end of writeBootloader
  
  
void startProgramming ()
  {
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
    
  Serial.println (F("Entered programming mode OK."));
  }  // end of startProgramming
 
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
  Serial.println (F("Written by Nick Gammon."));
  Serial.println (F("Version " VERSION));
  Serial.println (F("Compiled on " __DATE__ " at " __TIME__));
 
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
    
  startProgramming ();
  getSignature ();
  getFuseBytes ();

  // if we found a signature try to write a bootloader  
  if (foundSig != -1)
    writeBootloader ();

  // release reset    
  digitalWrite (RESET, HIGH);
    
  Serial.println (F("Type 'C' when ready to continue with another chip ..."));
  while (toupper (Serial.read ()) != 'C') 
    {}

  }  // end of loop
  
