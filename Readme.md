Hoodloader v2
=============

![header](pictures/header.jpg)

**Hoodloader v2 takes Arduino Uno's and Mega's USB functions to a whole new level.**
Version 2 is a __real__ bootloader  for the 16u2 (not a firmware like v1) which replaces the DFU Bootloader
with a custom CDC Bootloader with USB-Serial function.
The main function is to reprogram the main MCU (328/2560) and the USB MCU (16u2) at the same time
with all its functions and no limitations.

**It adds fully Arduino compatibility for the 16u2 to use it as standalone AVR too.**
So you can use the 16u2 for custom Arduino sketches. Whether __USB__, sensors, leds or anything you can imagine of.
Its fully Arduino compatible now (with some i/o pins). **All you need is a normal Arduino Uno/Mega R3 + USB cable.**

Before I go more into detail you need to understand how the Arduino Uno/Mega works,
what the Hoodloader does and what it can be used for, please read the text below.
I know its a lot of text, but you should understand the basics. 
**Caution: read the installing instructions carefully to not brick your Arduino.**
The rest is for interested people.

See http://nicohood.wordpress.com/ for more tutorials, projects and contact.

HID Project
===========

**The HID project contains 3 software solutions:**
* Arduino Leonardo/Micro additional HID USB cores
* [Hoodloader v1](https://github.com/NicoHood/Hoodloader) 328/2560 HID Serial Protocol functions (Uno/Mega)
* [Hoodloader v2](https://github.com/NicoHood/Hoodloader2) 16u2 cores with USB (Uno/Mega)

This project went to a lot of phases.
The idea is to enable enhanced USB functions to almost all 'standard' Arduino boards (Uno, Mega, Leonardo, Micro).
To make things more clear the HID Software is in this repository and the Hoodloader source in its own repositories (v1 & v2).



Features/Limitations
====================

**Normally any R3 Uno/Mega has a 16u2.** Earlier versions have an 8u2 and some online shops even sell the bigger 32u2.

**Hoodloader v2 functions:**
* CDC Bootloader for the 16u2
* Arduino integration + programming with the 16u2
* USB-Serial programming of the 328/2560
* Self flashing sketch to burn the Hoodloader v2 to the 16u2
* IDE example sketches for HID devices
* IDE example sketch for 16u2 - 328/2560 communication

**16u2 features on a normal R3 Uno/Mega:**
* 16kb flash (minus 4kb for the Hoodloader) (8 for a 8u2, 32 for a 32u2)
* 500 bytes ram (1000 bytes for a 32u2)
* 4 usable USB Endpoints (3 for CDC Serial, 1 for an HID device)
* 176 bytes DPRAM
* 2 status LEDs (TX + RX)
* 7 i/o pins with PCINT, SPI and PWM (3 pin SPI header + additional 4 pin header)
* Some i/o pins are connected to the 328/2560 (Serial, RESET)
* Less i/o pins, not all pins are broken out (SS, INT, etc)
* No I2C (u2 series doesn't have TWI) but Serial (to 328/2560) and SPI (master only)

Two Microconttrollers are better than one (Information)
=======================================================

**This section describes how the whole thing works together and gives some background information.
For better understanding I recommend to read this.**

As you might know the Arduino Uno/Mega have two AVR Microcontroller.
The Uno has a 328 and the Mega has a 2560. What most people dont know is that
both Arduinos also have a 2nd Microcontroller, the 16u2
(before it was a 8u2 and even earlier it was a simple FTDI USB-Serial bridge).
Some cheap Arduinos might not have a 8u2 or 16u2 because simple FTDI chips might be cheaper
and normally not needed for its normal use.

Here I refer to the 16u2 which is standard for every R3 model of Uno/Mega.
The 16u2 is normally used to program the main MCU 328/2560. Its a USB-Serial Bridge.
But it can be used for many different things because its a standalone AVR
with USB functions like a 32u4 on a Leonardo/Micro board.
For example: [Hiduino](https://github.com/ddiakopoulos/hiduino), 
[Darrens Arduino Uno Keyboard](http://hunt.net.nz/users/darran/weblog/b3029/Arduino_UNO_Keyboard_HID_version_03.html)
[Hoodloader v1](https://github.com/NicoHood/Hoodloader) or any other custom firmware.

So what does this tell us? We can use the 16u2 as a normal microcontroller of course.
The problem was that its not accessible to everybody because its a bit hard to understand
how to flash the 16u2, how to compile a program for the 16u2 (especially with [LUFA](http://www.fourwalledcubicle.com/LUFA.php))
and the biggest problem: How to flash the main MCU (328/2560)? Because if we change the 16u2's firmware
we cannot simply flash the Arduinos main MCU.

The [Hoodloader v1](https://github.com/NicoHood/Hoodloader)
solved this problem for the first time and its still a good and usable firmware to use USB functions
and upload sketches to the main MCU at the same time like you are used to.
But there were a few challenges with that. The biggest problem was just that it is hard to explain
how that complex stuff works. I think it wasn't clear all the time, also because I had two repositories
([HID Project](https://github.com/NicoHood/HID) and [Hoodloader v1](https://github.com/NicoHood/Hoodloader)).

The HID stuff always relied on a Serial communication between 16u2 and 328/2560 which wasn't perfect.
The 16u2 got HID commands with a special [NHP](https://github.com/NicoHood/NicoHoodProtocol) from the main MCU
and filtered every other signal and sent it to the PC Serial (CDC Serial).
Because when the PC USB-HID side was slow it had no chance to wait for the USB keys to flush.
And the main MCU had no chance to get any response back from the 16u2.
Otherwise I had to modify the normal Arduino core libraries very bad to filter out incoming back data
(a 'flushed, next key press' signal) again and it would also blow the sketch size.
Last but not least there were some problems with the multi report descriptor of the [Hoodloader v1](https://github.com/NicoHood/Hoodloader)).
To keep things simple for now: Some OS (linux especially with gamepads) had problems with
too many USB-HID devices in a same report (Keyboard, Mouse, 4 gamepads, Media, etc).


This time I want to keep things more clear but still more flexible.
And I also want to add more documentation for those of you who want to learn this USB stuff
and develop more professional things on their own. I know that its hard to understand the USB protocol.
Its a lot of text in the readme, but hopefully some people are glad about that information.
But I am not perfect neither. I learned everything on my own. Learning by doing.
**I started from scratch at the end of Jan 2014 with Arduino/Electronics!**
So you can do that too, I believe in you :)


Hoodloader v2 - what's new?
===========================

It has a **USB-Serial function** (except at baud 1200 which is used for CDC uploading) like the normal USB-Serial firmware
that everyone should have pre-installed on its Arduino Uno/Mega. This is used to reprogram the 328/2560.

Hoodloader v2 is a __Bootloader__ not a firmware. It replaces the DFU bootloader with a CDC bootloader.
**You wont be able to flash any hex file with [Flip](http://www.atmel.com/tools/flip.aspx) anymore.**
But this gives us the option to **use avr-dude to flash the 16u2 with custom firmware** (like some of the listed above).

The main advantage is to **flash the 16u2 with the Arduino IDE**.
Hoodloader v2 comes with full Arduino compatible core to create your very own custom programs on the 2nd USB chip.
You can ether use the CDC + HID or more HIDs without CDC or custom USB devices.

**To sum up: You can flash the 16u2 with avr-dude or Arduino IDE and the 328/2560 with the Arduino IDE like you are used to.**

Hoodloader v2 - How can I brick my Arduino?
===========================================

Bricking is a wide defined term. In this case bricking 'only' means that you cannot use you Arduino any more
without a second ISP or an [Arduino that acts as ISP](http://arduino.cc/en/Tutorial/ArduinoISP) /
[16u2 as ISP with Hoodloader v1](https://github.com/NicoHood/Hoodloader)).

**So if you have a standalone Arduino UNO read the instructions carefully and don't panic if anything seems not to work first!**
The only problem is when anything goes wrong when flashing the 16u2's bootloader. **Once it is installed you are save.**
__Normally nothing should go wrong__, if it does, read the instructions below.

**You also wont be able to flash any hex file with [Flip](http://www.atmel.com/tools/flip.aspx) anymore.**
You should be aware of that. Therefore you can use avr-dude instead.
You can flash the old DFU bootloader with an ISP or the installation 328/2560 sketch
as long as you can access the Serial or Softserial (read installing instructions again carefully before panic).

Hoodloader v2 - Installation
============================

To install a bootloader in general we would need an ISP (In system programmer).
You still can use your ISP if you like to but for those of you who only have a standalone Arduino Uno/Mega R3
we use a modified version of Nick Gammon's [Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches).

Connect all lines together like this:
```
16u2  - 328/2560
MOSI  - MOSI
MISO  - MISO
SCK   - SCK
RESET - PIN 10
```

How to get back to the DFU bootloader



Hoodloader v2 - How to use
==========================

![Bootloader Modes](pictures/bootloader.png)

Short the reset pin once to enter the actual 16u2 program. Then your 16u2 firmware/sketch starts running.
By default the program starts. If no program is presented the bootloader will run.

Because the Hoodloader replaces the 16u2's bootloader you can enter it like the Arduino Leonardo bootloader.
Short the reset pin (see pinout picture) twice to enter bootloader mode (manually) to upload code to the 16u2 or the 328/2560.
Then you have full CDC bootloader functions and USB-Serial with auto reset.

Once you've uploaded your custom program it is possible that you don't have/use the CDC Serial any more.
That might happen if you want to use more USB-HID functions which would be limited by the CDC.
To enter bootloader mode to upload to one of the two MCUs (16u2 and 328/2560) short the reset pin twice again.

![16u2 Reset](pictures/ArduinoUno_R3_Front_Reset.jpg)

Arduino Uno Overview/Pinout
===========================

![Arduino Uno Pinout](pictures/pinout.png)
(c) [pighixxx](http://forum.arduino.cc/index.php/topic,146315.0.html)


16u2 Pinout
```
TODO
```

How to upload sketches to the 328/2560
--------------------------------------

Short the 16u2's reset pin twice to enter bootloader mode. You can upload your Arduino sketch like you are used to.
You might need to check the correct Serial port though. To start the 16u2 sketch again short reset again once.

How to upload sketches to the 16u2
----------------------------------

You can now use the 16u2 as normal Arduino USB board like a Leonardo/Micro/Teensy.
You have to install new drivers since it will conflict with the official due to new USB functions.
(it will use a different PID/VID)



How to upload/flash firmwares (hex files) to the 16u2 with avr-dude
-------------------------------------------------------------------

Install avr-dude with 
or download here

avrdude.exe -C avrdude.conf -p at90usb162 -c avr109 -b 1200 -P COM13 -U flash:w:Hoodloader1_8.hex 
avrdude.exe -C avrdude.conf -p at90usb162 -c avr109 -b 1200 -P COM13 -e


Hoodpedia
=========

**Some information that you might find usefull. Ask me for more help**

CDC-Serial: A virtual Serial port via USB to the PC

USB-HID: Human Interface Device for Keyboard, Mouse, Gamepad etc to access and controll the PC

USB Endpoint: AVR (16u2 has 1 control endpoint and 4 endpoints for user functions. CDC takes 3 (Control, TX, RX), each HID 1)

USB Double Bank: a doubled DPRAM buffer to fill a bank of an endpoint while the other is read from the PC

DPRAM: Ram for USB communication. The 16u2 has 176 DPRAM which all endpoints share.
(USB Control Endpoint normally uses 8 bytes; CDC Control 8; CDC TX, RX, HID 8-64; double bank *2)

RAW-HID: A two way data pipe with the size of the endpoint to send data from and to the USB device. Also called Generic-HID

HID-Report Descriptor: Describes what devices the USB-HID should have with what functions. Very complicated and can cause errors if its not correct.
See [this tutorial](http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/) for more information.

HID-Multireport: Each HID device can have multiple devices in one endpoint. This might cause errors with RAW-HID, Gamepads and others.

LUFA: a lightweight USB-API written in C by Dean Camera. Also used by the Hoodloader.
Somehow hard to understand first but very powerful once you've got into it.

Useful Links
============

Nick Gammon's [Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches).

See http://nicohood.wordpress.com/ for more tutorials, projects and contact.



http://www.ladyada.net/learn/avr/avrdude.html
https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader


FAQ
===

Questions might be added to the readme or here.

See http://nicohood.wordpress.com/ for more tutorials, projects and contact.

Bugs
====

Gamepads and RAW-HID in an HID multi report gives problems under some OS (Gamepads especially Linux)

TODO
====
optimization of the code
baud 1200?
pwm pins?
pinout 16u2
connection to flash
usb-16u2-328/2560 skizze
isp flash sketch modification
pictures, example with leds spi, ir sensor, usb function
pinout
2x press bootloader reset
rename to hoodloader2
add copyright information

How to compile (on a Raspberry Pi)
==================================
This instruction is for compiling with a Raspberry Pi (Debian).
I cannot tell you how to compile Lufa with any other device, but Google can.

For me the easiest thing is to just code with my Windows PC and only compile with my Raspberry over ssh.
I recommend creating a Windows share by right clicking your Arduino folder and hit "share".
``` bash
$ cd Desktop
$ mkdir Arduino

#Test mounting:
sudo mount -t cifs -o username=yourusername,password=yourpass,nounix,noserverino //YOUR-PC-NAME/Arduino Arduino

#run it automated at startup. If not connected this will cause a long timeout.
sudo nano /etc/fstab
//YOUR-PC-NAME/Arduino /home/pi/Desktop/Arduino cifs username=yourusername,password=yourpass,nounix,noserverino 0 0
```

You need to install the gcc-avr toolchain, avr-libc and compile with:
``` bash
$ sudo apt-get install gcc-avr avr-libc
$ cd Desktop/Arduino/Hoodloader
$ sudo make clean
$ sudo make
```

Version History
===============
```
2.0.0 Release (xx.11.2014)
* Added Arduino IDE Integration
* Added HID examples

2.0.0 Beta Release (xx.11.2014)
* Added CDC Bootloader
* Added USB-Serial function

2.0.0 Alpha Release (22.10.2014)
* Added private Github repository with readme
* Added basic functions with dev states
```

Licence and Copyright
=====================
If you use this library for any cool project let me know!

```
Copyright (c) 2014 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```
