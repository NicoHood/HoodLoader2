HoodLoader2.0.5
===============

![header](header.jpg)

**HoodLoader2 is a CDC BootLoader with self reprogramming and Fast USB-Serial function.**

An Arduino Uno/Mega board has two Microcontroller of which one(16u2) is normally used for USB-Serial translation.
But we can also use it as standalone AVR Microcontroller with (or without) USB functions as well.

HoodLoader2 gives you the option to **reprogram the 16u2** of a normal Arduino Uno/Mega R3 with **custom sketches**.
This means you can use the 16u2 as a normal USB AVR like a Leonardo.
You have a full compatible USB-HID core, CDC Serial and you can also use the 7 i/o pins of the 16u2.
The extended HID devices of the [HID Project](https://github.com/NicoHood/HID) also apply for the HoodLoader2.

The 16u2 is somehow limited in its functions but still a great addition if you know how to use it. It also compatible with
[FastLED](https://github.com/FastLED/FastLED) and [IRLremote](https://github.com/NicoHood/IRLremote)(with PCINT) for example.

The great thing about this is that you actually have **two fully Arduino compatible Microcontrollers in one Arduino Uno/Mega board**
– the board most of you already own. Your IO MCU (328/2560) is **still reprogrammable** if you enter bootloader mode.
All you need for this is a normal Arduino Uno/Mega R3 and some cables to install the new HoodLoader2.

With version 2.0.5 you can now use HoodLoader2 also on a 32u4 which gives you even more power.

Please __read the wiki carefully__ to avoid any problems. You will find most of the answers to your questions already in the wiki.

<a href="https://www.buymeacoffee.com/Mnwg9NrCK" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: auto !important;width: auto !important;" ></a>

Wiki
====

All documentation moved to the [wiki page](https://github.com/NicoHood/HoodLoader2/wiki) (most up to date documentation).

An offline snapshot (also for older versions) is available in [releases](https://github.com/NicoHood/HoodLoader2/releases)


Contact
=======

Contact information can be found here:

www.nicohood.de


Version History
===============
```
2.0.5 Release (26.09.2015)
* No need for HID-Project anymore, PR3640 added u2 support to IDE 1.6.6
* Added up to 2M baud support (thx to https://github.com/urjaman/fast-usbserial)
* Improved USART reconfiguration
* Changed Magic Key to RAMEND
  (requires new HID-Project IDE patch, but is still backwards compatible)
* Saved a lot of ram by moving descriptors to PROGMEM (8/16u2)
* Added 32u4 support with saver bootkey option and backwards compatibility
* Added at90usb82 support
* Added EEPROM program support for 32u2
* Switch to U2Xn=0 if needed
* Added USB reconnect support
* General improvements to reduce flash size
* Updated and improved Installation sketch
* Added standard Uno/Mega bootloaders
* Removed /tools folder and added information to wiki instead
* Added recompile option to change single/double tab reset
* Added recompile option to change power on behavior
* Added optional old Atmega328 bootloader compatibility(baud 57600),
  only when compiled with a different baud
* Note: Can only be (re)compiled with AVR-GCC 5.1 and LTO (see wiki)

2.0.4 Release (20.03.2015)
* HoodLoader2.0.4
 * Upload verification on USB hubs fix in bootloader
 * Fixed 1 byte buffer overflow
 * Improved installation sketch
 * Moved to Lufa Board definition (with a fix)
* Added HID-Project 2.2 compatibility
 * Added boards.txt for Uno/Mega + HID-Bridge
 * Improved Uno/Mega uploading (no double tab needed any more)
 * Updated USB drivers for better PC side recognition
   (Each USB core has a different PID + Windows driver)
 * Fixed wrong PIDs in boards.txt

2.0.3 Release (28.01.2015)
* Added HID Project 2.1 board definition compatibility
* Fixed usb flag pass via boards.txt (instead of pins_arduino.h)
* Fixed 16u2 4 pin header pinout picture
* DFU burn bootloader fix
* HoodLoader2.0.3 released (minor firmware CDC identifier fix)
* Added 32u2 bootloader to the installation sketch as well
* Improved flash.bat file and included avrdude for windows
* Improved Readme instructions
* Updated USB drivers with 4 different PIDs
* Moved documentation to the wiki

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


License and Copyright
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
