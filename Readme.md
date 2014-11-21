HoodLoader2 Beta
================

![header](pictures/header.jpg)

*Caution: This is a Beta, please read everything carefully to not brick your Arduino!
Some things might be added/changed later and you might wonder where to find them.*

An Arduino Uno/Mega board has two Microcontroller of which one(16u2) is normally used for stupid USB-Serial translation.
But we can also use it as standalone AVR Microcontroller with (or without) USB functions as well.
HoodLoader2 enables this function to use the 16u2 (the USB chip) as normal AVR which is normally not possible with an Arduino Uno/Mega.

**HoodLoader2 takes Arduino Uno's and Mega's functions to a whole new level.**
Version 2 is a __real__ bootloader  for the 16u2 (not a firmware like v1) which replaces the DFU Bootloader
with a custom CDC Bootloader and USB-Serial function. You can reprogram the main MCU (328/2560) and the USB MCU (16u2)
at the same time with all its functions and no limitations.

It adds **fully Arduino IDE compatibility** for the 16u2(with 7 i/o pins)  to use it as standalone AVR for custom sketches too
([16u2 specs](https://github.com/NicoHood/HoodLoader2/blob/master/Readme.md#overview-featureslimitations)).
Whether new __USB__ functions (CDCSerial, HID: Keyboard, Mouse, Media Keys, System, Gamepad), sensors or leds(led strips as well!): anything you can imagine of.
The 16u2 is somehow limited in its functions but still a great addition if you know how to use it. It also compatible with
[FastLED](https://github.com/FastLED/FastLED) and [IRLremote](https://github.com/NicoHood/IRLremote)(with PCINT) for example.
**All you need is a normal Arduino Uno/Mega R3 + USB cable + a few wires.**

Please read the readme carefully to not run into any problems.
You should know what you are doing and what HoodLoader2 is used for.
HoodLoader2 is not used for an Arduino Leonardo/Micro but you can use the [HID Project](https://github.com/NicoHood/HID) for this as well.
If anything is unclear, let me know so I can improve it.
At the end of the readme are some notations for interested people/developers.

See [HID Project](https://github.com/NicoHood/HID) for the Arduino IDE core files/examples.

**See http://nicohood.wordpress.com/ for more tutorials, projects and contact.**

HID Project
===========

This project went through a lot of phases and has now reached an HID library with 3 software solutions.
The idea is to enable enhanced USB functions to almost all 'standard' Arduino boards (Uno, Mega, Leonardo, Micro).
To make things more clear the HID Software is in a separate repository than the HoodLoader (1 & 2) sources.
HoodLoader 1&2 is only used for an Uno/Mega to actually enable USB functions. To use HoodLoader1&2 you also need the HID Project.
HoodLoader 1&2 and HID Project installation instructions are in their own repositories.

**The [HID Project](https://github.com/NicoHood/HID) contains 3 software solutions:**
* Arduino Leonardo/(Pro)Micro extended USB HID-report descriptors
* [HoodLoader1](https://github.com/NicoHood/HoodLoader) 328/2560 HID Serial Protocol functions (Uno/Mega)
* [HoodLoader2](https://github.com/NicoHood/HoodLoader2) 16u2 USB cores with HID (Uno/Mega)

The API (syntax/usage) of the HID Project is always the same for each solution, you can port code from one device to another very easy.
The installation is also the same every time. Once installed you can use all 3 software parts automatically.

Make sure to read the upgrade instructions carefully, since it might have changed from one version to the other.
You should know in what way the version differ. For Arduino Leonardo/Micro/16u2(HoodLoader2) it is an enhanced USB core addition.
The HoodLoader1 API is "only" a Serial Protocol conversion of the HID report to the 16u2 with the 'old' HoodLoader1.

Overview: Features/Limitations
==============================

**Normally every Uno/Mega R3 has a 16u2.** Earlier versions had an 8u2
and some online shops even sell the bigger 32u2 (I've got two of them :D).

**HoodLoader2 functions:**
* CDC Bootloader for the 16u2 (to upload .hex files via avr-dude/Arduino IDE)
* USB-Serial programming of the 328/2560
* No drivers needed for the HoodLoader2 itself
* Self flashing sketch to burn the HoodLoader2 to the 16u2 (modified version from Nick Gammon)

**[HID Project](https://github.com/NicoHood/HID) features (for HoodLoader2):**
* Arduino IDE integration + programming with the 16u2
* IDE example USB-Serial demo (equal to original firmware)
* IDE example sketches for 16u2 HID devices (Mouse, Keyboard, Gamepad, Media/System Keys)
* IDE example sketch for 16u2 - 328/2560 communication (HoodLoader1 technique)

**16u2 features on a normal Uno/Mega R3:**
* 16kb flash (minus 4kb for the HoodLoader2) (8 for a 8u2, 32 for a 32u2)
* 500 bytes Ram 8u2 and 16u2 (1000 bytes Ram for a 32u2)
* 500 bytes EEPROM 8u2 and 16u2 (1000 bytes EEPROM for a 32u2)
* 4 usable USB Endpoints: CDC Serial, HID, MIDI (not all at once)
* 176 bytes DPRAM (ram for USB endpoints, double bank possible)
* 8 and 16 bit timer
* 2 status LEDs (PD4 RX + PD5 TX)
* 7 i/o pins with PCINT, SPI and PWM (3 pin SPI header + additional 4 pin header)
* HW Serial connected to Serial0 of the 328/2560
* PD7 connected to RESET of the 328/2560
* Not all pins are broken out (SS, INT, etc)
* No I2C (u2 series doesn't have TWI) but Serial (to 328/2560) and SPI (master only)
* No analogue pins
* No USB-Serial bridge at baud 57600 possible, used for CDC self reprogramming

| 8/16/32u2 Flash | Bootloader (4kb)                            | Program (4/12/28kb)    |
|:----------------|:--------------------------------------------|:-----------------------|
| Before          | DFU Bootloader (via Flip)                   | USB-Serial Firmware    |
| After           | HoodLoader2 (CDC Bootloader + USB-Serial)   | Custom Sketch/Firmware |

HoodLoader2 is a __Bootloader__ not a firmware. It replaces the DFU bootloader with a CDC bootloader.
**You wont be able to flash any hex file with [Flip](http://www.atmel.com/tools/flip.aspx) any more.**
But this gives us the option to use avr-dude to flash the 16u2 with custom firmware.

The main advantage is to **flash the 16u2 with the Arduino IDE**.
HoodLoader2 comes with full Arduino compatible core (see [HID Project](https://github.com/NicoHood/HID))
to create your very own custom programs with the normally unused 16u2 USB chip.
You can either use the CDC, MIDI, HID interfaces or custom USB devices (not all at once, similar to Teensy).

The USB-Serial bridges allows to reprogram the 328/2560.
So in bootloader mode your Arduino acts like its used to (except baud 57600).

See [HID Project](https://github.com/NicoHood/HID) for the Arduino IDE core files/examples.

HoodLoader2 - Installation
==========================

**Caution: You will overwrite the DFU Bootloader. Read the installing instructions carefully, double check connections and __don't panic__!**
It might be possible that you don't have a 16u2 on your Arduino board, if its a cheap clone.

**You wont be able to flash any hex file with [Flip](http://www.atmel.com/tools/flip.aspx)
via DFU any more** (but via avr-dude instead). You can switch back to DFU later.

**If anything goes wrong while burning the bootloader you can 'brick' your Arduino.**
In this case bricking 'only' means that you cannot use your Arduino's 16u2 USB-Serial bridge any more (and programming via USB too).
Then you need a second working Arduino to burn the firmware again.

To install a bootloader in general you would need an ISP (In system programmer).
If you just want to use your own ISP to burn the hex file, see below.
For those of you who only have a standalone Arduino Uno/Mega R3
we use a modified version of Nick Gammon's [Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches).
In this case the 328/2560 acts as ISP with a the HoodLoader .hex preloaded in PROGMEM.

### Installation sketch (standalone Arduino Uno/Mega)

This installation method is designed to work with a single Arduino Uno/Mega.
The main MCU (328/2560) gets a sketch with a preloaded firmware and is able to flash the 16u2 via ISP.
Its a special modification of [Nick Gammon's Atmega bootloader programmer](http://www.gammon.com.au/forum/?id=11635)
to ensure high security to not brick your Arduino.

First remove __all__ hardware from your Arduino to ensure nothing is messing up the upload.
All you need to do is to connect the SPI lines correct together like this.
**The 100nF Capacitor is needed to prevent the 328/2560 from not being reset.**
Only plug it in __after__ the sketch is uploaded to the 16u2.

![Connections](pictures/connections.jpg)

```
100nF Capacitor:
328/2560 RESET - GND (after sketch is uploaded)

Pin connections:
328/2560 - 16u2
MOSI     - MOSI
MISO     - MISO
SCK      - SCK
PIN 10   - 16u2 RESET

Optional Buttons for recovery:
PIN 8    - Force DFU upload
PIN 7    - Force HoodLoader2 upload
```

The installation sketch is located in *tools/Atmega_Board_Programmer/Atmega_Board_Programmer.ino*, open it with your Arduino IDE.

In the first lines of the sketch you can optionally choose to what Arduino you are uploading to otherwise it will automatically detect the right Arduino.
**Upload the installation sketch to your Arduino Uno/Mega, then put in the 100nF capacitor.**
The normal way is to control the uploading via Serial, so open the Serial port monitor at baud 115200.
Follow the instructions (press H + Enter). Then your Arduino should be flashed with the new firmware. Remove all the wires now.
Once you've done this, normally you don't need to do this again, maybe if there is a new HoodLoader2 version.

**Recovery options/how to get back to the original bootloader:**

If anything goes wrong and you can't access the Serial via USB any more you still are able to try different methods to flash the 16u2.
You can use another Arduino and upload the installation sketch there and try the flash again.
Or you can try to access the recovery options of the installation sketch if its still on the 328/2560:

The only way to manually flash the MCU without Serial access is to connect a button to pin 7 (HoodLoader2)
or to pin 8 (DFU + USB-Serial). Therefore make sure all wires are connected properly,
reset the main MCU (328/2560) and short the pin (7 or 8) to ground and wait a few seconds.

### Burning via ISP (advanced)

This is for advanced users who want to burn the bootloader with an external ISP directly.
You can do this with your program if choice (e.g. avr-dude) or use the Arduino IDE and select 16u2 and hit "burn bootloader".

```
Official documentation:
low_fuses=0xFF
high_fuses=0xD9
extended_fuses=0xF4
unlock_bits=0x3F
lock_bits=0x2F

Actual fuse setting on an official R3 board:
0xEF, // fuse low byte: external clock, m
0xD9, // fuse high byte: SPI enable, NOT boot into bootloader, 4096 byte bootloader
0xF4, // fuse extended byte: brown-out detection at 2.6V
0xCF }, // lock bits
```

For your interest, how to set the boards.txt to upload the firmware directly via the IDE + ISP

```
atmega16u2.name=Atmega 16u2
atmega16u2.upload.tool=avrdude
atmega16u2.upload.protocol=avr109
atmega16u2.upload.maximum_size=12288
atmega16u2.upload.maximum_data_size=512
atmega16u2.upload.speed=57600
atmega16u2.upload.disable_flushing=true
atmega16u2.upload.use_1200bps_touch=true
atmega16u2.upload.wait_for_upload_port=true

atmega16u2.bootloader.tool=avrdude
atmega16u2.bootloader.low_fuses=0xFF
atmega16u2.bootloader.high_fuses=0xD9
atmega16u2.bootloader.extended_fuses=0xF4
atmega16u2.bootloader.file=16u2.hex
atmega16u2.bootloader.unlock_bits=0x3F
atmega16u2.bootloader.lock_bits=0x2F

atmega16u2.build.mcu=atmega16u2
atmega16u2.build.f_cpu=16000000L
atmega16u2.build.usb_product="Atmega 16u2"
atmega16u2.build.board=AVR_16U2
atmega16u2.build.core=arduino
atmega16u2.build.variant=atmega16u2
#atmega16u2.build.extra_flags={build.usb_flags}
```

### Updating HoodLoader2 to a newer version

Updating to a newer HoodLoader2 version just works like the first install.
Upload the installation sketch and follow the instructions again carefully.
For the HID Project read the updating instructions carefully since they might change from version to version.

HoodLoader2 - How to use
==========================

![Bootloader Modes](pictures/bootloader.png)

**Short the reset pin once to enter/restart the actual 16u2 program**. Your 16u2 firmware/sketch starts running.
By default the program starts when you plug in the USB-Cable. If no program is presented the bootloader will run.

**Short the reset pin twice to enter bootloader mode manually** to upload code to the 16u2 or the 328/2560.
Then you have full CDC bootloader functions and USB-Serial with auto reset (like on a not modified Arduino except baud 57600).

Once you've uploaded your custom program it is possible that you don't have the CDC Serial any more.
That might happen if you want to use more USB-HID functions instead of a CDC Serial.
To flash the two MCUs (16u2 and 328/2560) you need to enter bootloader mode again by shorting the reset pin twice.

**In bootloader mode the device may enumerate as different USB device than the uploaded sketch.
The HoodLoader2 itself just needs the standard Arduino drivers.**

![16u2 Reset](pictures/reset.jpg)

### How to upload sketches to the 328/2560

Short the 16u2's reset pin twice to enter bootloader mode. You can upload your Arduino sketch like you are used to.
You might need to check the correct Serial port though. To start the 16u2 sketch again short reset once.

### How to upload sketches to the 16u2 via IDE

You can now use the 16u2 as normal Arduino USB board like a Leonardo/Micro/Teensy.
You have to install new drivers since it will conflict with the official due to new USB functions
(it will use a different PID/VID). If you have a sketch with CDC Serial running you can use the normal upload function.
If not you manually have to enter bootloader mode as described above and below.

To get Serial Debug output from the 328/2560 you have to enter bootloader mode again.
**Keep in mind that the Leds have inverted logic. Writing LOW means turn them on. I got stuck there for an hour...**

**To get the 16u2 board definitions for uploading copy this HoodLoader2 folder into your sketchbook like this: *sketchbook/hardware/HoodLoader2/* **

**Go to the [HID Project](https://github.com/NicoHood/HID) page to get the newest Arduino core library for the 16u2.**
### Temporary dev version can be found here, official update coming soon!:
https://github.com/NicoHood/HID/tree/dev

Follow the installation instructions __carefully__ and try out the examples. To upload the code to your Arduino the easiest way is to
click upload until it says "uploading" and the double tab reset. The PC will automatically detect that the new serial showed up,
upload the new code and run it.

**If you don't want to use the USB-Core** you can also choose under *Tools/USB Core* "No USB functions" to get rid of the USB stuff
and save the ram for other stuff if you don't need it. You also don't need the HID Project essentially if you don't want to use the USB functions.
You have to add an ISR into every sketch then. Checkout the '_16u2_NoUSB_Blink' example in */tools/*.

```cpp
// workaround for undefined USBCON has to be placed in every sketch
// otherwise the timings wont work correctly
ISR(USB_GEN_vect)
{
  UDINT = 0;
}
```

For your interest you can also use this (somehow outdated) [older USB-Core](https://www.mattairtech.com/index.php/development-boards/mt-db-u1.html) from Mattair.

### How to flash/erase firmwares (.hex files) to the 16u2 with avr-dude

If you want to upload a self compiled .hex file from a LUFA project for example like the
[HoodLoader1](https://github.com/NicoHood/HoodLoader) for example
you wont be able to use DFU via Flip any more. But you can use avr-dude instead:

Download avr-dude [here](http://download.savannah.gnu.org/releases/avrdude/) (avrdude-6.1-mingw32.zip for windows).
Open a cmd or bash and try the following code. You can also create a .bat file for easier uploading.
The .bat files are also located in */tools*.

This can also be used to erase the program from your 16u2 if you always want to use it as normal USB-Serial bridge again.

```
#select com port/device and .hex file
avrdude.exe -C avrdude.conf -p at90usb162 -c avr109 -b 57600 -P COM1 -U flash:w:HoodLoader1_8.hex
pause

avrdude.exe -C avrdude.conf -p at90usb162 -c avr109 -b 57600 -P COM1 -e
pause
```

Arduino Uno/Mega 16u2 Pinout
============================

![Arduino Uno Pinout](pictures/pinout.png)
(c) [pighixxx](http://forum.arduino.cc/index.php/topic,146315.0.html)

**Note: Some pins are not connected on a standard Arduino Uno R3** but still listed so you can use them if you have a custom board.
You also need to solder the additional 4 pin header to access all 7 PB i/o pins.
The Arduino Uno/Mega pinout is the same for the 16u2 MCU.

**Keep in mind that the Leds have inverted logic. Writing LOW means turn them on. I got stuck there for an hour...**

```
16u2 Pinout
[NC] = Not connected on an Arduino Uno/Mega R3

D0  - [NC] PB0 PCINT0 SS
D1  - PB1 PCINT1 SCLK
D2  - PB2 PCINT2 MOSI
D3  - PB3 PCINT3 MISO
D4  - PB4 PCINT4
D5  - PB5 PCINT5
D6  - PB6 PCINT6
D7  - PB7 PCINT7 TIMER1C

D8  - [NC] PC7 INT4
D9  - [NC] PC6 PCINT8 TIMER1A
D10 - [NC] PC5 PCINT9 TIMER1B
D11 - [NC] PC4 PCINT10
D12 - [NC] PC2 PCINT11

D13 - [NC] PD0 INT0 TIMER0B
D14 - [NC] PD1 INT1 
D15 - PD2 INT2 USART1 RX
D16 - PD3 INT3 USART1 TX
D17 - PD4 INT4    RXLED
D18 - PD5 PCINT12 TXLED
D19 - [NC] PD6 INT5 
D20 - PD7 INT7 328/2560 RESET
```

Useful Links
============

* [HID Project](https://github.com/NicoHood/HID) for the Arduino IDE core files/examples.
* [LUFA from Dean Camera](http://www.fourwalledcubicle.com/LUFA.php)
* [The Original Arduino Sources](https://github.com/arduino/Arduino/tree/master/hardware/arduino/firmwares/atmegaxxu2/arduino-usbserial)
* [Sparkfun 32u4 Bootloader](https://github.com/sparkfun/SF32u4_boards)
* Nick Gammon's [Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches), [Website](http://www.gammon.com.au/forum/?id=11635).
* [How to use AVR-Dude](http://www.ladyada.net/learn/avr/avrdude.html)
* [Installing a bootloader in general](https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader)
* See http://nicohood.wordpress.com/ for more tutorials, projects and contact.

FAQ
===

Questions might be added here. Feel free to contact me for any questions!

See http://nicohood.wordpress.com/ for more tutorials, projects and contact.

Bugs
====

Gamepads and RAW-HID in an HID multi report gives problems under some OS (Gamepads especially Linux).
This is an OS bug and has nothing to do with the HoodLoader.

Also the HoodLoader only ensures that you can upload sketches to the MCUs. If your USB HID devices have any problems
please check the HID Project for known bugs/updates.

TODO
====
(ignore this, its just for me)

pwm pins?
pictures, example with leds spi, ir sensor, usb function
check if fastled/other libs work
windows key?
uploading baud rate
bank, epsize, buffer size
migrate at90usb162 to atmega16u2 (also the bat file)
add 8u2/32u2/32u4 version
_delay_loop_2() instead delay function
increase buffer to 128 or even 256
current fuses in the uploading sketch?
Reset of 328 when leaving Bootloader mode
Someone should look over the watchdog timer thing
change HL2-CDC name
remove buffer and use endpoint banks? store endpoint state in isr!
mattairtech link, paul link
test pwm timer1c
usb names etc, vid
reset bug, flash leds
test single gamepad
update Burning via ISP (advanced)
system wakeup and other (github pull requests)
HL2> special baud check for bootloader enable again> see github pull req.
first upload 158 usb source, then the update to see the diffs.

Version History
===============
```
2.0.0 Release TODO! (xx.11.2014)
* HID Project updated:
 * Added Arduino IDE Integration
 * Added HID examples
* HoodLoader2 declared stable

2.0.0 Beta-6 Release (05.11.2014)
* Added Arduino-IDE integration workaround for non USB usage

2.0.0 Beta-5 Release (05.11.2014)
* Improved Atmega Bootloader (added stop programming feature)

2.0.0 Beta-4 Release (04.11.2014)
* Improved Atmega Bootloader (Self flashing now works properly)

2.0.0 Beta-3 Release (03.11.2014)
* Improved Atmega Bootloader (DFU now possible)

2.0.0 Beta-2 Release (02.11.2014)
* Added Atmega Bootloader code (no DFU back possible)

2.0.0 Beta-1 Release (02.11.2014)
* Added CDC Bootloader
* Added USB-Serial function
* Made Repository Public

2.0.0 Alpha Release (22.10.2014)
* Added private Github repository with readme
* Added basic functions with dev states
```

Additional Information
======================

### Two Microcontrollers are better than one

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
[HoodLoader1](https://github.com/NicoHood/HoodLoader) or any other custom firmware.

So what does this tell us? You can use the 16u2 as a normal microcontroller of course.
The problem was that its not accessible to everybody because its a bit hard to understand
how to flash the 16u2, how to compile a program for the 16u2 (especially with [LUFA](http://www.fourwalledcubicle.com/LUFA.php))
and the biggest problem: How to flash the main MCU (328/2560)? Because if you change the 16u2's firmware
you cannot simply flash the Arduinos main MCU.

The [HoodLoader1](https://github.com/NicoHood/HoodLoader)
solved this problem for the first time and its still a good and usable firmware to use USB functions
and upload sketches to the main MCU at the same time like you are used to.
But there were a few challenges with that. The biggest problem was just that it is hard to explain
how that complex stuff works. I think it wasn't clear all the time, also because I had two repositories
([HID Project](https://github.com/NicoHood/HID) and [HoodLoader1](https://github.com/NicoHood/HoodLoader)).

The HID stuff always relied on a Serial communication between 16u2 and 328/2560 which wasn't perfect.
The 16u2 got HID commands with a special [NHP](https://github.com/NicoHood/NicoHoodProtocol) from the main MCU
and filtered every other signal and sent it to the PC Serial (CDC Serial).
Because when the PC USB-HID side was slow it had no chance to wait for the USB keys to flush.
And the main MCU had no chance to get any response back from the 16u2.
Otherwise I had to modify the normal Arduino core libraries very bad to filter out incoming back data
(a 'flushed, next key press' signal) again and it would also blow the sketch size.
Last but not least there were some problems with the multi report descriptor of the [HoodLoader1](https://github.com/NicoHood/HoodLoader)).
To keep things simple for now: Some OS (linux especially with gamepads) had problems with
too many USB-HID devices in a same report (Keyboard, Mouse, 4 gamepads, Media, etc).

This time I want to keep things more clear but still more flexible.
And I also want to add more documentation for those of you who want to learn this USB stuff
and develop more professional things on their own. I know that its hard to understand the USB protocol.
Its a lot of text in the readme, but hopefully some people are glad about that information.

But I am not perfect neither. I learned everything on my own. Learning by doing.
**I started from scratch at the end of Jan 2014 with Arduino/Electronics!**
So you can do that too, I believe in you :)


### Hoodpedia

**Some information that you might find useful. Ask me for more help**

* **CDC-Serial:** A virtual Serial port via USB to the PC
* **USB-HID:** Human Interface Device for Keyboard, Mouse, Gamepad etc to access and controll the PC
* **USB Endpoint:** AVR (16u2 has 1 control endpoint and 4 endpoints for user functions. CDC takes 3 (Control, TX, RX), each HID 1)
* **USB Double Bank:** a doubled DPRAM buffer to fill a bank of an endpoint while the other is read from the PC
* **DPRAM:** Ram for USB communication. The 16u2 has 176 DPRAM which all endpoints share.
(USB Control Endpoint normally uses 8 bytes; CDC Control 8; CDC TX, RX, HID 8-64; double bank *2)
* **RAW-HID:** A two way data pipe with the size of the endpoint to send data from and to the USB device. Also called Generic-HID
* **HID-Report Descriptor:** Describes what devices the USB-HID should have with what functions. Very complicated and can cause errors if its not correct.
See [this tutorial](http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/) for more information.
* **HID-Multireport:** Each HID device can have multiple devices in one endpoint. This might cause errors with RAW-HID, Gamepads and others.
* **LUFA:** a lightweight USB-API written in C by Dean Camera. Also used by the HoodLoader.
Somehow hard to understand first but very powerful once you've got into it.

### Changes to the Atmega Bootloader Programmer

To upload the HoodLoader2 I used a modification of Nick Gammon's Atmega Bootloader Programmer.
Instructions can be found [here](http://www.gammon.com.au/forum/?id=11635).

One of the change was to add new bootloader files. You can convert your hex file as described with Nick's Lua script.
One thing to add is the address of the 16u2 in the end_addresses array (line 16). The new .lua script is also in the repository now.

You need to download the MUSHclient as described in his instructions.
He provides a simple, [ready to use zip file](http://www.gammon.com.au/downloads/dlmushclient.htm).

### How to compile with a Raspberry Pi (avr-gcc 4.7.2)

This instruction is for compiling the HoodLoader on your own with a Raspberry Pi (Debian).
I cannot tell you how to compile Lufa with any other OS, but Google can.
Believe me: Windows is a struggle to compile.
**On a raspberry you can only use the older avr-gcc 4.7.2 toolchain. Please tell me how to upgrade if you know how.**

For me the easiest thing is to code with my Windows PC only and compile with my Raspberry over ssh.
I recommend creating a Windows share by right clicking your Arduino folder and hit "share".
Then mount this share with your Raspberry like this:
``` bash
$ cd Desktop
$ mkdir Arduino

#Test mounting:
sudo mount -t cifs -o username=yourusername,password=yourpass,nounix,noserverino //YOUR-PC-NAME/Arduino Arduino

#run it automated at startup. If not connected this can cause a long timeout.
sudo nano /etc/fstab
//YOUR-PC-NAME/Arduino /home/pi/Desktop/Arduino cifs username=yourusername,password=yourpass,nounix,noserverino 0 0
```

You need to install the gcc-avr toolchain, avr-libc and compile with:
``` bash
$ sudo apt-get install gcc-avr avr-libc
$ cd Desktop/Arduino/HoodLoader
$ sudo make clean
$ sudo make
```

**HoodLoader2 Beta compiles with exactly 4000 bytes with a Raspberry Pi.**

### How to compile with Ubuntu (avr-gcc 4.8.1)

On Ubuntu you can use the newer toolchain which saves ~50 bytes of memory.

You need to install the gcc-avr toolchain, avr-libc and compile with:
``` bash
$ sudo add-apt-repository ppa:pmjdebruijn/gcc-avr-release
$ sudo apt-get update
$ sudo apt-cache search gcc-avr
$ sudo apt-get install gcc-avr avr-libc
$ cd Desktop/Arduino/HoodLoader
$ sudo make clean
$ sudo make
```

[Package information](https://launchpad.net/~pmjdebruijn/+archive/ubuntu/gcc-avr-release)

**HoodLoader2 Beta compiles with about 3950 bytes with this toolchain.**

Licence and Copyright
=====================
If you use this library for any cool project let me know!

```
Copyright (c) 2014 NicoHood
See the readme for credit to other people.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```
