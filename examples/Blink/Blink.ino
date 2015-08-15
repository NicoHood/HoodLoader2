/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Blink
  Basic example to test that HoodLoader2 works.

  You may also want to use:
  LED_BUILTIN
  LED_BUILTIN_RX
  LED_BUILTIN_TX
*/

const uint32_t blinkDelay = 1000;
const uint8_t pinLed = LED_BUILTIN;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize led as an output.
  pinMode(pinLed, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(pinLed, HIGH);
  delay(blinkDelay);
  digitalWrite(pinLed, LOW);
  delay(blinkDelay);
}
