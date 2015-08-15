/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 Serial HID-Bridge IO
 Select Arduino Uno/Mega HID-Bridge as board.

 After upload make sure the 16u2 sketch runs, not the HoodLoader2 bootloader.
 This example will print random chars via serial to the USB MCU.
 If the USB MCU has the other bridge sketch uploaded
 it should type those chars via keyboard.
*/

void setup() {
  // Start the Serial which is connected with the USB MCU.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.
  Serial.begin(115200);
}

void loop() {
  // Send random letters to the USB MCU
  Serial.write(random('a', 'z'));
  delay(1000);
}
