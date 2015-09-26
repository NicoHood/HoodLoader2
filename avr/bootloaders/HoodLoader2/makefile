#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = USER
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = HoodLoader2
SRC          = $(TARGET).c Descriptors.c BootloaderAPI.c BootloaderAPITable.S $(LUFA_SRC_USB)
LUFA_PATH    = ../lufa-LUFA-140928/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ $(HOODLOADER2_OPTS) -DBOOT_START_ADDR=$(BOOT_START_OFFSET) $(REGS) -flto -fuse-linker-plugin
LD_FLAGS     = -Wl,--section-start=.text=$(BOOT_START_OFFSET) $(BOOT_API_LD_FLAGS),--section-start=.data=$(RAM_OFFSET) $(REGS) -flto -fuse-linker-plugin


# Flash size and bootloader section sizes of the target, in KB. These must
# match the target's total FLASH size and the bootloader size set in the
# device's fuses.
FLASH_SIZE_KB         = 16
BOOT_SECTION_SIZE_KB  = 4

#define LUFA_VID					0x03EB
#define LUFA_PID					0x204A

#define ARDUINO_VID					0x2341
#define ARDUINO_UNO_PID				0x0043 // R3 (0001 R1)
#define ARDUINO_MEGA_PID			0x0042 // R3 (0010 R1)
#define ARDUINO_MEGA_ADK_PID		0x0044 // R3 (003F R1)
#define ARDUINO_LEONARDO_PID		0x0036 // Bootloader, not program!
#define ARDUINO_MICRO_PID   		0x0037 // Bootloader, not program!
#define ARDUINO_DUE_PID             0x003D

HOODLOADER2_OPTS  = -DVENDORID=ARDUINO_VID
HOODLOADER2_OPTS += -DPRODUCTID=ARDUINO_UNO_PID

# 1200 is the baud to load the Bootloader from an Arduino sketch,
# 57600 turns out to be the actual baud rate for uploading for default arduino bootloaders (and arduino nano by default!)
# make also sure to change your boards.txt uploading speed!
# make sure to use a standard baud rate (supported by avrdude or it will (sometimes randomly) not work
HOODLOADER2_OPTS += -DBAUDRATE_CDC_BOOTLOADER=57600

# Select how the single/double tab should behave
HOODLOADER2_OPTS += -DDOUBLE_TAB_RESET_TO_BOOTLOADER=true

# Select if you want to start the bootloader when powered on (plugged in the usb cable)
HOODLOADER2_OPTS += -DPOWER_ON_TO_BOOTLOADER=false

# Bootkey locations
# 2nd Bootkey is not used for 32u4 devices (due to flash size)
# 2nd Bootkey option only makes sense if you compile with the old programming speed 57600
# 2nd Bootkey will only be used if it makes sense (see HoodLoader.c)
# Default 32u4 Bootkey was 0x8000 as 16 bit, bit an 8 bit check is enough normally
# 0x280 was the default bootkey for HL2.0.4 and older
HOODLOADER2_OPTS += -DBOOTKEY=RAMEND
HOODLOADER2_OPTS += -DSECOND_BOOTKEY=0x280


# Add an offset of 256 + 128 bytes to RAMSTART for the ringbuffers
# +1 byte offset for old MagicBootKey support (0x280)
# +1 for better data aligning or in case a 2byte key is written
# u2 and u4 Series (RAMSTART = 0x100):
RAM_OFFSET            = 0x800282

# Reserved registers for faster USB-Serial convertion
# The lower, the better (except 0 and 1)
# r2-5 used for faster USART ISRs
# r6 used for 32u4 optimization
REGS  = -ffixed-r2 -ffixed-r3 -ffixed-r4 -ffixed-r5 -ffixed-r6


# Bootloader address calculation formulas
# Do not modify these macros, but rather modify the dependent values above.
CALC_ADDRESS_IN_HEX   = $(shell printf "0x%X" $$(( $(1) )) )
BOOT_START_OFFSET     = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024 )
BOOT_SEC_OFFSET       = $(call CALC_ADDRESS_IN_HEX, ($(FLASH_SIZE_KB) * 1024) - ($(strip $(1))) )

# Bootloader linker section flags for relocating the API table sections to
# known FLASH addresses - these should not normally be user-edited.
BOOT_SECTION_LD_FLAG  = -Wl,--section-start=$(strip $(1))=$(call BOOT_SEC_OFFSET, $(3)) -Wl,--undefined=$(strip $(2))
BOOT_API_LD_FLAGS     = $(call BOOT_SECTION_LD_FLAG, .apitable_trampolines, BootloaderAPI_Trampolines, 96)
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, .apitable_jumptable,   BootloaderAPI_JumpTable,   32)
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, .apitable_signatures,  BootloaderAPI_Signatures,  8)

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
