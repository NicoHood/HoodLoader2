/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 Serial HID-Bridge USB
 Select HoodLoader16u2 as board.

 After upload make sure the 16u2 sketch runs, not the HoodLoader2 bootloader.
 This example will type any serial data via keyboard.
*/

#include "Keyboard.h"
#include "HID.h"

void setup() {
  // Start the Serial1 which is connected with the IO MCU.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.
  Serial1.begin(115200);

  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();

  // Start the USB Serial for debugging
  Serial.begin(115200);
}

void loop() {
  // Check if any Serial data from the IO MCU was received
  char c = Serial1.read();

  // If it's a character, print it!
  if (c >= 'a' && c <= 'z') {
    Serial.print(F("USB: "));
    Serial.print(c);
    Keyboard.print(c);
  }
}
