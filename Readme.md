HoodLoader2.0.3
===============

![header](pictures/header.jpg)

**HoodLoader2 is a CDC BootLoader with self reprogramming and USB-Serial function.**

An Arduino Uno/Mega board has two Microcontroller of which one(16u2) is normally used for USB-Serial translation.
But we can also use it as standalone AVR Microcontroller with (or without) USB functions as well.

HoodLoader2 gives you the option to **reprogram the 16u2** of a normal Arduino Uno/Mega R3 with **custom sketches**.
This means you can use the 16u2 as a normal USB AVR like a Leonardo.
You have a full compatible USB-HID core, CDC Serial and you can also use the 7 i/o pins of the 16u2.
The extended HID devices and USB-Core improvements of the [HID Project](https://github.com/NicoHood/HID) also apply for the HoodLoader2.

The 16u2 is somehow limited in its functions but still a great addition if you know how to use it. It also compatible with
[FastLED](https://github.com/FastLED/FastLED) and [IRLremote](https://github.com/NicoHood/IRLremote)(with PCINT) for example.

The great thing about this is that you actually have **two fully Arduino compatible Microcontrollers in one Arduino Uno/Mega board**
– the board most of you already own. Your main MCU (328/2560) is **still reprogrammable** if you enter bootloader mode.
All you need for this is a normal Arduino Uno/Mega R3 and some cables to install the new HoodLoader2.
Please __read the readme carfully__ to avoid any problems. You will find most of the answers to your questions already in the readme.

**See http://nicohood.wordpress.com/ for more tutorials, projects and contact.**

Overview: Features/Limitations
==============================

**Normally every Uno/Mega R3 has a 16u2.** Earlier versions had an 8u2
and some online shops even sell the bigger 32u2 (I've got two of them :D).

**HoodLoader2 functions:**
* CDC Bootloader for the 16u2 (to upload .hex files via avr-dude/Arduino IDE)
* USB-Serial programming of the 328/2560
* No drivers needed for the HoodLoader2 itself (default Arduino Uno/Mega drivers)
* Self flashing sketch to burn the HoodLoader2 to the 16u2 (modified version from Nick Gammon)

**[HID Project](https://github.com/NicoHood/HID) features (for HoodLoader2):**
* Arduino IDE integration + programming with the 16u2 (USB-Core)
* IDE example sketches for HID devices (Mouse, Keyboard, Gamepad, Media/System Keys)
* IDE example sketch for a working Serial + Keyboard demonstration
* IDE example sketch for PWM demonstration
* IDE example USB-Serial demo (equal to original firmware)
* IDE example sketch for 16u2 - 328/2560 communication (HoodLoader1 technique)
* Newer features are listed in the HID Project page itself.

**16u2 features on a normal Uno/Mega R3:**
* 16kb flash (minus 4kb for the HoodLoader2) (8 for a 8u2, 32 for a 32u2)
* 500 bytes Ram 8u2 and 16u2 (1000 bytes Ram for a 32u2)
* 500 bytes EEPROM 8u2 and 16u2 (1000 bytes EEPROM for a 32u2)
* 4 usable USB Endpoints: CDC Serial, HID, MIDI (not all at once)
* 176 bytes DPRAM (ram for USB endpoints, double bank possible)
* 8 and 16 bit timer
* 2 status LEDs (PD4 RX + PD5 TX)
* 7 usable i/o pins with PCINT, SPI and 1PWM (3 pin SPI header + additional 4 pin header)
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
HoodLoader2 comes with full Arduino compatible USB-Core (see [HID Project](https://github.com/NicoHood/HID))
to create your very own custom programs with the normally unused 16u2 USB chip.

**The USB-Serial bridges allows to reprogram the 328/2560.**
So in bootloader mode your Arduino acts like its used to (except baud 57600).

**See [HID Project](https://github.com/NicoHood/HID) for the Arduino IDE core files/examples.**

HoodLoader2 - Hardware Installation
===================================

**Caution: You will overwrite the DFU Bootloader**
You wont be able to flash any hex file with [Flip](http://www.atmel.com/tools/flip.aspx) via DFU any more.
But via avr-dude instead. You can switch back to DFU later.

Read the installing instructions carefully, double check connections and __don't panic__!**
It might be possible that you don't have a 16u2 on your Arduino board, if its a cheap clone.
It might also be possible that your ICSP header of the 16u2 is flipped 180 degrees on some unofficial boards.

**If anything goes wrong while burning the bootloader you can 'brick' your Arduino.**
In this case bricking 'only' means that you cannot use your Arduino's 16u2 USB-Serial bridge any more (and programming via USB too).
Then you need a second working Arduino to burn the bootloader again. Normally this should not happen.

**You have three options to burn the new bootloader:**
* [a) Burn it with the main MCU (328/2560) on a standalone Arduino Uno/Mega](https://github.com/NicoHood/HoodLoader2/tree/dev#a-installation-sketch-standalone-arduino-unomega)
* [b) Burn it from any other Arduino with the installation sketch](https://github.com/NicoHood/HoodLoader2/tree/dev#b-burn-the-bootloader-from-another-arduino)
* [c) Burn with an ISP (advanced users)](https://github.com/NicoHood/HoodLoader2/tree/dev#c-burning-via-isp-advanced)

### a) Installation sketch (standalone Arduino Uno/Mega)

This installation method is designed to work with a single Arduino Uno/Mega.
To install a bootloader in general you would need an ISP (In system programmer).
Most of you (me too) down own a professional ISP so we will use a trick here.

We use a modified version of Nick Gammon's [Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches).
In this case the 328/2560 acts as ISP with a the HoodLoader2.hex preloaded in PROGMEM.

##### 1. Prepare your Arduino.
First remove __all__ hardware/wires/usb-cable from your Arduino to ensure nothing is messing up the upload.
Connect all wires like in the picture below. Double check connections!

Note: It might be possible that your ICSP header of the 16u2 is flipped 180 degrees on some unofficial boards.

The 100nF Capacitor is needed to prevent the 328/2560 from not being reset.
Only plug it in __after__ the sketch is uploaded to the 16u2, you will see a note later in the instruction.

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

##### 2. Upload the installation sketch
Connect your Arduino to your PC, select the Arduino Uno/Mega board like you are used to.
The installation sketch is located in *tools/Atmega_Board_Programmer/Atmega_Board_Programmer.ino*, open it with your Arduino IDE.

In the first lines of the sketch you can optionally choose to what Arduino you are uploading to otherwise it will automatically detect the correct Arduino.
**Upload the installation sketch to your Arduino Uno/Mega.** This may take a while.

![board-programmer](pictures/board-programmer.png)

##### 3. Burn the bootloader
Once the installation sketch is uploaded to your main MCU you are able to burn the bootloader to your 16u2.
To control the burning you have two options:

##### Control via Serial:
Open the Serial port monitor at baud 115200. **Now put in the 100nF capacitor.**
Follow the instructions (press 'H' + Enter) and wait. You may close the Serial monitor now.

![installation-sketch](pictures/installation-sketch.png)

##### (Optional) Control via pin 7:
**First put in the 100nF capacitor.** Press the reset button.
Connect pin 7 with ground to flash the HoodLoader2 if your serial port does not work for some reason.

Your USB device will disconnect and the 328/2560 will flash the new bootloader.

##### 4. Check bootloader installation
Now your Arduino should be flashed with the new bootloader. If its not correctly installed read the instructions carefully and try again.
To check if the installation worked fine you may check your device like that in "Devices and Printers" for Windows:

![version](pictures/version.png)

You may **remove all wires now**, they are not needed anymore once the HoodLoader2 is flashed.
Normally you don't need to do this again, maybe if there is a new HoodLoader2 version.

**Keep on reading** on how to install the board definition files and use the HoodLoader2.

##### 5. (Optional) Switch back to the old DFU bootloader
If you want to switch back to the original DFU bootloader with usbserial just do the same procedure.
Enter 'D' for DFU on the Serial monitor or short pin 8 to ground. You can use avrdude to flash .hex files so DFU is not really needed anymore.

### b) Burn the bootloader from another Arduino

You can also use a 2nd Arduino to flash the bootloader. For example if you can't access the first Arduino for some reason.
Just connect all wires from the 2nd Arduinos main MCU to the 16u2. You also need to connect 5V-5V and GND-GND to power the board.
Unplug the USB cable of the Arduino you are flashing. In this case you do not need the capacitor.

### c) Burning via ISP (advanced)

This is for advanced users who want to burn the bootloader with an external ISP directly.
You can do this with your program if choice (e.g. avr-dude) or use the Arduino IDE and
select the correct MCU, board, bootloader, programmer and hit "burn bootloader".

```
HoodLoader2 Fuses:
low_fuses=0xFF
high_fuses=0xD8
extended_fuses=0xFC
unlock_bits=0x3F
lock_bits=0x2F


Official DFU + USB-Serial Fuses:
low_fuses=0xEF
high_fuses=0xD9
extended_fuses=0xF4
unlock_bits=0x3F
lock_bits=0xCF
```

### Updating HoodLoader2 to a newer version

Updating to a newer HoodLoader2 version just works like the first install.
Upload the installation sketch and follow the instructions again.

For the HID Project read the updating instructions carefully since they might change from version to version.
Also ensure that your HoodLoader2 board definitions (the avr/ folder) is up to date when you use the newest HID-Project.

**Updating to 2.0.3 just burn the new HoodLoader2.0.3, install the new drivers(for 32u2, 8u2, at90usb162 only) and update to the newest HID Project.**


HoodLoader2 - Software Installation
===================================

You can now use the 16u2 as normal Arduino USB board like a Leonardo/Micro/Teensy with the Arduino IDE.
Therefore you still need to get some software and install it properly. Arduino IDE 1.5.8 or higher is recommended.

##### 1. CDC Driver installation (Windows only):
You have to install new drivers since it will conflict with the official due to new USB functions it will use a different PID/VID).
You can install the proper (unsigned) driver from the .inf file or as an alternative you can also install the (signed)Teensy drivers.

The driver file is named *HoodLoader2.inf*. Also see [this tutorial](http://arduino.cc/en/guide/windows) on how to install the drivers (right click the .inf file and hit install).
[How to install drivers for Windows 8/8.1](https://learn.sparkfun.com/tutorials/disabling-driver-signature-on-windows-8/disabling-signed-driver-enforcement-on-windows-8).

If you don't want to install the unsigned driver, you can also install the [Teensy driver](https://www.pjrc.com/teensy/serial_install.exe) which works for every USB Serial device.
It will then show up as Teensy USB-Serial and not as HoodLoader2 in the device manager. If you have both drivers installed it will show up as HoodLoader2.
The Teensy driver recognizes any CDC USB device as Teensy USB Serial if no other driver is installed. This might be usefull if you plan to change the PID/VID for developing.

If you want it to be recognized as Uno/Mega edit the boards.txt definitions' VID and PID. I don't recommend this to a) know what
Bootloader currently is on your board and b) it seems that with the official signed drivers wont work with unofficial HID modification.

##### 2. Installing board definitions
To get the 16u2 board definitions for uploading, copy this HoodLoader2 folder into your sketchbook like this: *sketchbook/hardware/HoodLoader2/*
Remove the '-master' postfix.

![sketchbook installation](pictures/installation.png)

##### 3. Installing HID Project

To actually use the USB functions you need a working USB-Core for the Arduino IDE. At the moment the Arduino IDE does not natively support the u2 series.
Therefore you need to get the new USB-Core with the HID Project. The HID Project also adds new HID Devices to your USB-Core and some more improvements.
(The HID Project can also be used for Arduino Leonardo/Micro and HoodLoader1.)

Go to the [HID Project](https://github.com/NicoHood/HID) page to get the newest Arduino USB-Core for the 16u2.
Follow the installation instructions __carefully__ and **try out the HoodLoader 2 related examples**.
If you update the HID Project files also ensure you have the newest HoodLoader2 board definition files.

For your interest you can also use this (somehow outdated) [older USB-Core](https://www.mattairtech.com/index.php/development-boards/mt-db-u1.html) with Lufa from Mattairtech.

You should now have installed the bootloader itself, drivers, HL2 board definitions and HID Project. Read on how to use the bootloader and upload programs.

HoodLoader2 - How to use
========================

### How to use the reset

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

### Uploading programs

**To actually use the new HoodLoader2 you have a few options**
* a) Upload sketches to the 328/2560 via Arduino IDE
* b) Upload sketches to the 16u2 via Arduino IDE
* c) Upload firmwares(.hex files) to the 16u2 via Avrdude

### How to upload sketches to the 328/2560

Short the 16u2's reset pin twice to enter bootloader mode. You can upload your Arduino sketch like you are used to.
You might need to check the correct Serial port in the IDE though. To start the 16u2 sketch again short reset once.
To get Serial Debug output from the 328/2560 you also have to enter bootloader mode again.


### How to upload sketches to the 16u2 via IDE

You can now use the 16u2 as normal Arduino USB board like a Leonardo/Micro/Teensy.
Therefore you need the HID-Project which you should have installed above.
[Read the how to use section for HoodLoader2 of the HID Project.](https://github.com/NicoHood/HID/tree/Official-Integration#microleonardo--hoodloader2)

Have a look at the examples in the HID Project and check out the [pinout](https://github.com/NicoHood/HoodLoader2#arduino-unomega-16u2-pinout) below.
**Keep in mind that the Leds have inverted logic. Writing LOW means turn them on.**
Once you've uploaded your first HoodLoader2 sketch it should switch to a different Serial comport and show up as HoodLoader2 16u2:

![hoodloader2device](pictures/hoodloader2device.png)

##### Deactivating USB-Core
If you don't want to use the USB-Core you can also choose under *Tools/USB Core* "No USB functions" to get rid of the USB stuff
and save the ram for other stuff if you don't need it. You also don't need the HID Project essentially if you don't want to use the USB functions.

**The workaround is no longer needed with version 2.0.1 or newer.**

### How to flash/erase firmwares (.hex files) with avrdude

If you want to upload a self compiled firmware(.hex file) from for example a LUFA project like the
[HoodLoader1](https://github.com/NicoHood/HoodLoader) you wont be able to use DFU via Flip any more. But you can use avrdude instead:

A copy of avrdude for windows is already included in *tools/avrdude/*. The libusb0.dll was taken from an Arduino IDE installation.
You can download the newest avrdude [here](http://download.savannah.gnu.org/releases/avrdude/).

Open a cmd or terminal and try the following code. You can also use the provided *tools/flash.bat* file (windows only) for easier uploading.

```bash
#select com port, device and .hex file yourself
avrdude.exe -C avrdude.conf -p atmega16u2 -c avr109 -b 57600 -P COM1 -U flash:w:HoodLoader1_8.hex

avrdude.exe -C avrdude.conf -p atmega16u2 -c avr109 -b 57600 -P COM1 -e
```

This can also be used to erase the program from your 16u2 if you always want to use it as normal USB-Serial bridge again (always stay in bootloader mode).
And yes, it's correct that **you can have HoodLoader2 and HoodLoader1 running 'at the same time'** since v2 is a real bootloader and v1 'only' a firmware.


Arduino Uno/Mega 16u2 Pinout
============================

![Arduino Uno Pinout](pictures/pinout.png)
(c) [pighixxx](http://forum.arduino.cc/index.php/topic,146315.0.html)

**Note: Some pins are not connected on a standard Arduino Uno R3** but still listed so you can use them if you had a custom board. It might also be possible that your ICSP header of the 16u2 is flipped 180 degrees on some unofficial boards.
You also need to solder the additional 4 pin header to access all 7 PB i/o pins. D7 has PWM! (other not connected pins with a timer as well).
The 16u2 pinout is the same for Arduino Uno/Mega.

**Keep in mind that the Leds have inverted logic. Writing LOW means turn them on.**

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
~D7 - PB7 PCINT7 TIMER1C PWM

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

Troubleshoot
============

##### Installation sketch fails to upload
Try to check your wires again. If you have an unofficial board your ISP header (those 6 pins) might be flipped 180 degree.
Also ensure to add a 100nF capacitor, nothing less than 100nF!
![installation-sketch-fail](pictures/installation-sketch-fail.png)

##### Driver installation failed
Read the driver installation section in the readme carefully again. There is a link on how to install the drivers for Windows 8.1.
Optionally you can use the Teensy drivers as described there.

Feel free to [open an issue on github](https://github.com/NicoHood/HoodLoader2/issues) if you have any problems
or [contact](https://nicohood.wordpress.com/about/) me for any questions!

Known Bugs
==========

HoodLoader2 seems to have problems to work properly on some USB hubs.

Baud 57600 cannot be used in bootloader mode since its used for CDC programming.

The main MCU will always reset on a 16u2 reprogramming. There is no way to fix this (yet).

The HoodLoader2 only ensures that you can upload sketches to the MCUs.
If your USB HID device causes any problem please check the HID Project for known bugs/updates.

The Magic BootKey is not stored properly in ram and may cause errors. This is a general Arduino
Software problem, we have to do this to keep compatible. Normally there shouldn't be any noticeable error.
This bug has a workaround in the HID Project so it wont matter any ways. Just want to mention it here.

Useful Links
============

**A great thx to all people who helped and supported me with this. Most of them are linked below.**

* [HID Project](https://github.com/NicoHood/HID) for the Arduino IDE core files/examples.
* [LUFA from Dean Camera](http://www.fourwalledcubicle.com/LUFA.php)
* [The Original Arduino Sources](https://github.com/arduino/Arduino/tree/master/hardware/arduino/firmwares/atmegaxxu2/arduino-usbserial)
* [Sparkfun 32u4 Bootloader](https://github.com/sparkfun/SF32u4_boards)
* [Nick Gammon's Atmega Board Programmmer](https://github.com/nickgammon/arduino_sketches), [Website](http://www.gammon.com.au/forum/?id=11635).
* [Mattairtechs 16u2 Lufa USB Core](https://www.mattairtech.com/index.php/development-boards/mt-db-u1.html)
* [Paul Brook's Minimus 32u2 Arduino USB Core](https://github.com/pbrook/minimus-arduino)
* [Paul Stoffregen's Teensy Core](https://github.com/PaulStoffregen/cores)
* [Keyboard Led Out report by hartmut_holgraefe](http://forum.arduino.cc/index.php?topic=173583.0)
* [atmega memory use](http://jeelabs.org/2011/05/22/atmega-memory-use/)
* [wiki entry about ram addresses](http://en.wikipedia.org/wiki/Atmel_AVR_instruction_set)
* [How to use AVR-Dude](http://www.ladyada.net/learn/avr/avrdude.html)
* [Installing a bootloader in general](https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader)
* [IRLremote is also HoodLoader2 compatible](https://github.com/NicoHood/IRLremote)
* [Hackaday post about HoodLoader2](hackaday.com/2014/11/30/using-the-second-microcontroller-on-an-arduino)
* See http://nicohood.wordpress.com/ for more tutorials, projects and contact.

Version History
===============
```
2.0.3 Release (xx.01.2015)
* added HID Project 2.1 board definition compatibility
* fixed usb flag pass via boards.txt (instead of pins_arduino.h)
* fixed 16u2 4 pin header pinout picture
* DFU burn bootloader fix
* HoodLoader2.0.3 released (minor firmware CDC identifier fix)
* Added 32u2 bootloader to the installation sketch as well
* Improved flash.bat file and included avrdude for windows
* Improved Readme instructions
* Updated USB drivers with 4 different PIDs

2.0.2 Release (30.11.2014)
* HID Project 2.0 official released:
 * Added Arduino IDE Integration for HoodLoader2
 * See official changelog for more information.
* HoodLoader2.0.2 released (led off fix)

2.0.1 Pre-Release (29.11.2014)
* HoodLoader2.0.1 Release
 * Better, full reset after bootloader execution with watchdog
 * This fixes the "No-USB" workaround with the USB clock
 * Fixed fuse bug
 * Fixed magic key passed from Arduino core (HID Project)
 * Changed some descriptor names, bugfix above freed some space
 * Special case 57600 baud for compatibility with the ATmega328 bootloader TODO resolved (baud reserved for cdc)
 * Reset of 328 when leaving Bootloader mode resolved (not possible to fix)
 * _delay_loop_2() instead delay function resolved (saves 2 bytes, more confusing + inaccurate than useful)
 * CPU_PRESCALE resolved (not needed for bootloader, watchdog will reset it anyways)
 * Re enabled LOCK_BYTE_WRITE_SUPPORT function (due to flash improvements)
 * Increased TX/RX EP_SIZE to 64 bytes
 * Increased Serial->USB Buffer to 128 bytes
* HID Project dev update related to the magic key bugfix
* Updated Atmega board programmer sketch with the new firmware and fuses

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

### USB PID/VID

The HoodLoader2 itself uses the official Arduinos VID and PID.
The HID Project and the board definition files uses a special PID but this has nothing to do with the bootloader itself.

```
#define LUFA_VID					0x03EB
#define LUFA_PID					0x204A

#define ARDUINO_VID					0x2341
#define ARDUINO_UNO_PID				0x0043 // R3 (0001 R1)
#define ARDUINO_MEGA_PID			0x0042 // R3 (0010 R1)
#define ARDUINO_MEGA_ADK_PID		0x0044 // R3 (003F R1)

HoodLoader2atmega16u2.vid.0=0x2341
HoodLoader2atmega16u2.pid.0=0x484C
```

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

**HoodLoader2.0.3 compiles with 3958(+2 for Mega) bytes with a Raspberry Pi.**

### How to compile with Ubuntu (avr-gcc 4.8.1)

On Ubuntu you can use the newer toolchain which saves ~50 bytes of memory.

You need to install the gcc-avr toolchain, avr-libc and compile with:
``` bash
$ sudo add-apt-repository ppa:pmjdebruijn/gcc-avr-release
$ sudo apt-get update
$ sudo apt-cache search gcc-avr
$ sudo apt-get install gcc-avr avr-libc git
$ avr-gcc -v
$ cd Desktop
$ mkdir Arduino
$ cd Arduino
$ git clone https://github.com/NicoHood/HoodLoader2.git
$ cd HoodLoader2
$ sudo make clean
$ sudo make
```

[Package information](https://launchpad.net/~pmjdebruijn/+archive/ubuntu/gcc-avr-release)

**HoodLoader2.0.3 compiles with 3908(+2 for Mega) bytes with this toolchain.**

Licence and Copyright
=====================
If you use this library for any cool project let me know!

```
Copyright (c) 2014-2015 NicoHood
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
