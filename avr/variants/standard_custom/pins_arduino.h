/*
Copyright(c) 2014-2015 NicoHood
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

// include the standard Uno board definition file
#include "../standard/pins_arduino.h"

//================================================================================
// HID Settings
//================================================================================

// HID Project needs to be installed https://github.com/NicoHood/HID

/*
You have two options to enable/disbale hid functions:

The nearly full automatic way which enables the specific hid report for you,
enables the specific hid api and also enables the keyboard led function if needed.

The fully customizable variant where you can create your very own HID report.
You still can use the predefined hid reports or add you very own ones.
You have to enable the specific hid apis on your own then, also the keyboard led function.
*/

#define HID_AUTOMATIC
//#define HID_CUSTOM_SETTINGS

//================================================================================
// Notice for HoodLoader2!
//================================================================================

/*
HoodLoader2 sketches are limited to a maximum 15 byte HID report.
Dont use more than that. (for a Leonardo/Micro it is 63 bytes).

Arduino Uno/Mega's main MCU cannot have a specified HID descriptor,
edit the HoodLoader2 file for the 16u2 instead.
It is more important that the HID APIs are the same on both devices.
*/

//================================================================================
// Automatic
//================================================================================

#ifdef HID_AUTOMATIC
// pre selected hid reports with autoinclude of the api
#define HID_MOUSE_ENABLE // normal mouse with buttons + wheel
//#define HID_MOUSE_ABSOLUTE_ENABLE // only works with system and without gamepad
#define HID_KEYBOARD_LEDS_ENABLE // leds OR keys
//#define HID_KEYBOARD_KEYS_ENABLE
//#define HID_RAWHID_ENABLE // currently not working
//#define HID_CONSUMERCONTROL_ENABLE
//#define HID_SYSTEMCONTROL_ENABLE
//#define HID_GAMEPAD_ENABLE // only works without mouse absolute

//================================================================================
// Custom Settings
//================================================================================

#elif defined(HID_CUSTOM_SETTINGS)

// default setting here shows a mouse + keyboard with no led function
// like in IDE 1.0.6/1.5.8 or lower

// use this to enable the Keyboard Led functions
//#define HID_KEYBOARD_LEDS_ENABLED

// flag that you are using a custom hid report descriptor
#define EXTERN_HID_REPORT

// activate your custom HID-APIs here:
#define HID_MOUSE_API_ENABLE
//#define HID_MOUSE_ABSOLUTE_API_ENABLE
#define HID_KEYBOARD_API_ENABLE
//#define HID_RAWHID_API_ENABLE
//#define HID_CONSUMERCONTROL_API_ENABLE
//#define HID_SYSTEMCONTROL_API_ENABLE
//#define HID_GAMEPAD_API_ENABLE
//#define HID_ENABLE_ALL_APIS // enables all of the ones above

#else
#error Please select automatic or custom hid report in the pins_arduino.h!
#endif