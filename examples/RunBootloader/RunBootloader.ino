/*
 Copyright (c) 2015 NicoHood
 See the readme for credit to other people.

 HoodLoader2 Run Bootloader example

 Starts Bootloader mode.
 This is useful if you only want to use
 the Fast USB-Serial bridge again.
 
 You could delete the firmware with avrdude,
 but this sketch is way simpler to upload for beginners.
 
 It forces a watchdog reset,
 meaning the bootloader is started completely independant.
*/

#include <avr/wdt.h>

void setup() {
  // close interrupts
  cli();

  // write magic key to ram
  *(uint16_t*)MAGIC_KEY_POS = 0x7777;

  // watchdog reset
  wdt_enable(WDTO_120MS);
}

void loop() {
  // wait for reset
}
