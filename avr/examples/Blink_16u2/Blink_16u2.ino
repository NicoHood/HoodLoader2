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

// workaround for undefined USBCON
//	General interrupt
ISR(USB_GEN_vect)
{
	uint8_t udint = UDINT;
	UDINT = 0;
	return;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 18 as an output.
  pinMode(18, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(18, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(18, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
