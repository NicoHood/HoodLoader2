#define VERSION "1.32-HL205-B"

//================================================================================
// HoodLoader2 Settings
//================================================================================

// Led pin to signalize programming was successfull
#define LED_PIN LED_BUILTIN

// Select one of the files below
#define HEXFILE HEXFILE_HoodLoader2_0_5_Uno_atmega16u2_hex

// Newest HoodLoader2.0.5 hex files
#define HEXFILE_HoodLoader2_0_5_Leonardo_atmega32u4_hex 1
#define HEXFILE_HoodLoader2_0_5_Micro_atmega32u4_hex 2
#define HEXFILE_HoodLoader2_0_5_Uno_atmega16u2_hex 3
#define HEXFILE_HoodLoader2_0_5_Mega_atmega16u2_hex 4
#define HEXFILE_HoodLoader2_0_5_Uno_atmega32u2_hex 5
#define HEXFILE_HoodLoader2_0_5_Mega_atmega32u2_hex 6
#define HEXFILE_HoodLoader2_0_5_Uno_at90usb82_hex 7
#define HEXFILE_HoodLoader2_0_5_Mega_at90usb82_hex 8
#define HEXFILE_HoodLoader2_0_5_Uno_at90usb162_hex 9
#define HEXFILE_HoodLoader2_0_5_Mega_at90usb162_hex 10
#define HEXFILE_HoodLoader2_0_5_Uno_atmega8u2_hex 11
#define HEXFILE_HoodLoader2_0_5_Mega_atmega8u2_hex 12

// Legacy versions
#define HEXFILE_HoodLoader2_0_4_Uno_16u2_hex 101
#define HEXFILE_HoodLoader2_0_4_Mega_16u2_hex 102
#define HEXFILE_HoodLoader2_0_4_Uno_32u2_hex 103
#define HEXFILE_HoodLoader2_0_4_Mega_32u2_hex 104

// Original DFU Bootloader with/without USB-Serial Firmware
#define HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full 201
#define HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex_Full 202
#define HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex 203

// Recomplied Lufa DFU bootloaders (compatible with Arduino Leds)
#define HEXFILE_BootloaderDFU_atmega32u4_leo_hex 301
#define HEXFILE_BootloaderDFU_atmega16u2_hex 302
#define HEXFILE_BootloaderDFU_atmega32u2_hex 303
#define HEXFILE_BootloaderDFU_at90usb82_hex 304
#define HEXFILE_BootloaderDFU_at90usb162_hex 305
#define HEXFILE_BootloaderDFU_atmega8u2_hex 306

// For testing purposes
#define HEXFILE_DEV 1000

//================================================================================
// HoodLoader2 Predefinitions
//================================================================================

// Map number back to array name
#if(HEXFILE == HEXFILE_HoodLoader2_0_5_Leonardo_atmega32u4_hex)
#define HEXARR HoodLoader2_0_5_Leonardo_atmega32u4_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Micro_atmega32u4_hex)
#define HEXARR HoodLoader2_0_5_Micro_atmega32u4_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega16u2_hex)
#define HEXARR HoodLoader2_0_5_Uno_atmega16u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega16u2_hex)
#define HEXARR HoodLoader2_0_5_Mega_atmega16u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega32u2_hex)
#define HEXARR HoodLoader2_0_5_Uno_atmega32u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega32u2_hex)
#define HEXARR HoodLoader2_0_5_Mega_atmega32u2_hex
#elif(HEXFILE == HoodLoader2_0_5_Uno_at90usb162_hex)
#define HEXARR HoodLoader2_0_5_Uno_at90usb162_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_at90usb162_hex)
#define HEXARR HoodLoader2_0_5_Mega_at90usb162_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega8u2_hex)
#define HEXARR HoodLoader2_0_5_Uno_atmega8u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega8u2_hex)
#define HEXARR HoodLoader2_0_5_Mega_atmega8u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_4_Uno_16u2_hex)
#define HEXARR HoodLoader2_0_4_Uno_16u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_4_Mega_16u2_hex)
#define HEXARR HoodLoader2_0_4_Mega_16u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_4_Uno_32u2_hex)
#define HEXARR HoodLoader2_0_4_Uno_32u2_hex
#elif(HEXFILE == HEXFILE_HoodLoader2_0_4_Mega_32u2_hex)
#define HEXARR HoodLoader2_0_4_Mega_32u2_hex
#elif(HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full)
#define HEXARR Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full
#elif(HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex_Full)
#define HEXARR Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega2560_Rev3_hex_Full
#elif(HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex)
#define HEXARR Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex
#elif(HEXFILE == HEXFILE_BootloaderDFU_atmega32u4_leo_hex)
#define HEXARR BootloaderDFU_atmega32u4_leo_hex
#elif(HEXFILE == HEXFILE_BootloaderDFU_atmega16u2_hex)
#define HEXARR BootloaderDFU_atmega16u2_hex
#elif(HEXFILE == HEXFILE_BootloaderDFU_atmega32u2_hex)
#define HEXARR BootloaderDFU_atmega32u2_hex
#elif(HEXFILE == HEXFILE_BootloaderDFU_at90usb162)
#define HEXARR BootloaderDFU_at90usb162
#elif(HEXFILE == HEXFILE_BootloaderDFU_atmega8u2_hex)
#define HEXARR BootloaderDFU_atmega8u2_hex

// Dev settings
#elif(HEXFILE == HEXFILE_DEV)
#define HEXARR HoodLoader2_hex
#define USE_AT90USB82 true
#define START_ADDRESS 0x1000
#define LOW_FUSE 0xEF
#define HIGH_FUSE 0xD8
#define EXT_FUSE 0xFC
#define LOCK_BITS 0xCF

#endif

#ifndef HEXFILE
#error Please select a (single!) hexfile in the list above
#endif

// Default 16u2 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega16u2_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega16u2_hex) \
|| (HEXFILE == HEXFILE_HoodLoader2_0_4_Uno_16u2_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_4_Mega_16u2_hex) \
|| (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full) \
|| (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega_Rev3_hex_Full) \
|| (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega16u2_hex)

#define USE_ATMEGA16U2 true
#define START_ADDRESS 0x3000
#define LOW_FUSE 0xEF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xFC // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0xCF // lock bits

#endif // 16u2 settings


// Default 32u2 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega32u2_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega32u2_hex) \
|| (HEXFILE == HEXFILE_HoodLoader2_0_4_Uno_32u2_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_4_Mega_32u2_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega32u2_hex)

#define USE_ATMEGA32U2 true
#define START_ADDRESS 0x7000
#define LOW_FUSE 0xEF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xFC // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0xCF // lock bits

#endif // 32u2 settings

// Default 8u2 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_atmega8u2_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_atmega8u2_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega8u2_hex)

#define USE_ATMEGA8U2 true
#define START_ADDRESS 0x1000
#define LOW_FUSE 0xEF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xFC // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0xCF // lock bits

#endif // 8u2 settings


// Default at90usb162 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_at90usb162_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_at90usb162_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_at90usb162_hex)

#define USE_AT90USB162 true
#define START_ADDRESS 0x3000
#define LOW_FUSE 0xEF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xFC // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0xCF // lock bits

#endif // at90usb162 settings

// Default at90usb82 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Uno_at90usb82_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Mega_at90usb82_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_at90usb82_hex)

#define USE_AT90USB82 true
#define START_ADDRESS 0x1000
#define LOW_FUSE 0xEF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xFC // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0xCF // lock bits

#endif // at90usb82 settings

// Default 32u4 settings
#if (HEXFILE == HEXFILE_HoodLoader2_0_5_Leonardo_atmega32u4_hex) || (HEXFILE == HEXFILE_HoodLoader2_0_5_Micro_atmega32u4_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega32u4_hex)

#define USE_ATMEGA32U4 true
#define START_ADDRESS 0x7000
#define LOW_FUSE 0xFF // fuse low byte: external clock, m
#define HIGH_FUSE 0xD8 // fuse high byte: SPI enable, boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xCB // fuse extended byte: brown-out detection at 2.6V
#define LOCK_BITS 0x2F // lock bits

#endif // 32u4 settings

// DFU settings
#if (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex_Full) \
|| (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Mega_Rev3_hex_Full)

#define START_ADDRESS 0x0000 // Bootloader + Firmware
#define HIGH_FUSE 0xD9 // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xF4 // fuse extended byte: brown-out detection at 2.6V

#elif (HEXFILE == HEXFILE_Arduino_COMBINED_dfu_usbserial_atmega16u2_Uno_Rev3_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega16u2_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega16u2_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega32u2_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_at90usb82_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_at90usb162_hex) \
|| (HEXFILE == HEXFILE_BootloaderDFU_atmega8u2_hex)

#define HIGH_FUSE 0xD9 // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xF4 // fuse extended byte: brown-out detection at 2.6V

#elif (HEXFILE == HEXFILE_BootloaderDFU_atmega32u4_hex)

#define HIGH_FUSE 0xD9 // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
#define EXT_FUSE 0xC4 // fuse extended byte: brown-out detection at 2.6V

#endif // DFU settings

//================================================================================
// HoodLoader2 Installation program
//================================================================================


// indicates if programming + verification was successfull
bool success = false;

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
#include "bootloaders.h"
#include "DFUBootloaders.h"
#include "legacyBootloaders.h"
#include "originalBootloaders.h"

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
  { { 0x1E, 0x97, 0x03 }, "ATmega1280",  128 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x04 }, "ATmega1281",  128 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x01 }, "ATmega2560",  256 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x02 }, "ATmega2561",  256 * kb,   1 * kb },

  // AT90USB family
  #ifdef USE_AT90USB82
  { { 0x1E, 0x93, 0x82 }, "At90USB82", 8 * kb, 512,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x93, 0x82 }, "At90USB82", 8 * kb, 512 },
#endif
#ifdef USE_AT90USB162
  { { 0x1E, 0x94, 0x82 }, "At90USB162", 16 * kb, 512,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x94, 0x82 }, "At90USB162", 16 * kb, 512 },
#endif

  // Atmega32U2 family
#ifdef USE_ATMEGA8U2
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512 },
#endif

#ifdef USE_ATMEGA16U2
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2", 16 * kb, 512,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2", 16 * kb, 512 },
#endif

#ifdef USE_ATMEGA32U2
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2", 32 * kb, 512,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2", 32 * kb, 512 },
#endif

  // Atmega32U4 family
  { { 0x1E, 0x94, 0x88 }, "ATmega16U4",  16 * kb,   512 },
#ifdef USE_ATMEGA32U4
  { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  32 * kb,   4 * kb,
    HEXARR,// loader image
    START_ADDRESS,
    sizeof HEXARR,
    128, // page size in bytes (for committing)
    LOW_FUSE,
    HIGH_FUSE,
    EXT_FUSE,
    LOCK_BITS
  },
#else
  { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  32 * kb,   4 * kb },
#endif

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
byte program (const byte b1, const byte b2 = 0, const byte b3 = 0, const byte b4 = 0);
byte program (const byte b1, const byte b2, const byte b3, const byte b4)
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
void showHex (const byte b, const boolean newline = false, const boolean show0x = true);
void showHex (const byte b, const boolean newline, const boolean show0x)
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
void showYesNo (const boolean b, const boolean newline = false);
void showYesNo (const boolean b, const boolean newline)
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
        Serial.println (F("Next attempt in 10 seconds."));
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

  digitalWrite (RESET, HIGH);  // ensure SS stays high for now
  pinMode (CLOCKOUT, OUTPUT);

  // set up Timer 1
  TCCR1A = bit (COM1A0);  // toggle OC1A on Compare Match
  TCCR1B = bit (WGM12) | bit (CS10);   // CTC, no prescaling
  OCR1A =  0;       // output every cycle

  Serial.println();
  Serial.println(F("Welcome to the HoodLoader2 installation sketch!"));
  Serial.println(F("This version will install HoodLoader2 on your Arduino"));
  Serial.println();
  Serial.print(F("You selected number "));
  Serial.print(HEXFILE);
  Serial.println(F(" as bootloader."));
  Serial.println(F("If this is not correct upload the program again with different settings"));
  Serial.println();
  Serial.println(F("Make sure all wires are connected as written in the documentation."));
  Serial.println(F("Please now insert the 100nF capacitor between Reset and Ground on a standalone Arduino."));
  Serial.println(F("In some cases the Arduino will reset then and display these messages again."));
  Serial.println();
  Serial.println(F("On a standalone Arduino you will no longer see any messages here."));
  Serial.println(F("If you uploaded this sketch to a 2nd Aruino you will see some debug output."));
  Serial.println();
  Serial.println(F("If the installation was successfull the on board Led will start blinking fast (every 100ms)."));
  Serial.println(F("If the installation failed the on board Led will start blinking slow (every second)."));
  Serial.println();
  Serial.println(F("Starting programming in 10 seconds, please be patient and wait some time."));
  Serial.println();
  Serial.flush();
}  // end of setup

void loop ()
{
  int speed;
  if (success)
    speed = 100;
  else
    speed = 1000;

  // wait 10 sec and retry if not successful
  uint8_t countLed = 11;
  pinMode(LED_PIN, OUTPUT);
  while (countLed != 0 || success == true)
  {
    // blink led
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= speed) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      countLed--;
      if (success == false)
        Serial.println(countLed);
    }
  }
  pinMode(LED_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();

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

  Serial.println();
}  // end of loop

