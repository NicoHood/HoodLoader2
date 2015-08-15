#!/bin/bash

MCU=Atmega16u2
TTY=/dev/ttyACM0
# TODO input more via parameter
if [ $1 ];
then
	HEXFILE=$1
else
	read -p "Enter Hexfile: " HEXFILE
fi

echo "Enter this command to upload a .HEX firmware to your HoodLoader2 device:"
echo avrdude -p $MCU -c avr109 -b 57600 -P $TTY -U flash:w:$HEXFILE
#echo avrdude -p Atmega16u2 -c avr109 -b 57600 -P /dev/ttyACM0 -U flash:w:Firmware.hex

