/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  USB-Serial

  Transferes from USB to HW Serial and vice versa.
  It also resets the main MCU on a DTR rise.

  Keep in mind that HoodLoader2 in bootloader mode
  supports faster USART speeds (2M full duplex)
  and that this us just an example how to reproduce
  a simple USB-Serial converter.

  Reset pin u2 Series: 20
  Reset pin 32u4 with HoodLoader2: 4
*/

// Refine the reset pin to reset the destination MCU.
// This definition by default is equal to the HoodLoader2 used reset pin.
// But you still can use another pin for your layout.
const uint8_t resetPin = IO_MCU_RESET_PIN;

void setup()
{
  // Set main MCU by default active
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);

  // Start USB Serial
  Serial.begin(0);
}

void loop()
{
  // Check if the USB virtual serial wants a new baud rate
  static uint32_t baud = 0;
  if (Serial.baud() != baud) {
    baud = Serial.baud();
    Serial1.end();
    if (baud)
      Serial1.begin(baud);
  }

  // reset the main mcu if DTR goes HIGH
  if (Serial.dtr())
    digitalWrite(resetPin, LOW);
  else
    digitalWrite(resetPin, HIGH);

  // Serial -> USB

  // Check if any data has arrived on the USART serial port
  // and USB is ready for the next bytes
  int readAvailable = Serial1.available();
  if (readAvailable > 0) {

    int writeAvailable = Serial.availableForWrite();
    if (writeAvailable > 0) {

      // Write maximum one EP_SIZE to not block with Zero Length packets
      uint8_t buff[USB_EP_SIZE - 1];
      if (readAvailable > sizeof(buff))
        readAvailable = sizeof(buff);

      // Dont write more than USB can take to not block
      if (readAvailable > writeAvailable)
        readAvailable = writeAvailable;

      // Read data from the hardware serial port

      int n = Serial1.readBytes((char *)buff, readAvailable);

      // Write it to the USB port
      Serial.write(buff, n);
    }
  }


  // USB -> Serial

  // Check if any data has arrived on the USB virtual serial port
  // and USART is ready for the next bytes
  readAvailable = Serial.available();
  if (readAvailable > 0) {

    int writeAvailable = Serial1.availableForWrite();
    if (writeAvailable > 0) {

      // Write maximum one EP_SIZE to not block with Zero Length packets
      if (readAvailable > (USB_EP_SIZE - 1))
        readAvailable = (USB_EP_SIZE - 1);

      // Dont write more than USART buffer is free to not block
      if (readAvailable > writeAvailable)
        readAvailable = writeAvailable;

      // Read data from the USB serial port
      uint8_t buff[SERIAL_TX_BUFFER_SIZE];
      int n = Serial.readBytes((char *)buff, readAvailable);

      // Write it to the USART port
      Serial1.write(buff, n);
    }
  }
}
