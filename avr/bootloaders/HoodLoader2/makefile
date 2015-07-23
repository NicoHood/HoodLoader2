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
BOARD        = UNO
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = HoodLoader2
SRC          = $(TARGET).c Descriptors.c BootloaderAPI.c BootloaderAPITable.S $(LUFA_SRC_USB)
LUFA_PATH    = ../../../tools/lufa-LUFA-140928/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ $(HOODLOADER2_OPTS) -DBOOT_START_ADDR=$(BOOT_START_OFFSET) $(REGS) -flto -fuse-linker-plugin
LD_FLAGS     = -Wl,--section-start=.text=$(BOOT_START_OFFSET) $(BOOT_API_LD_FLAGS),--section-start=.data=$(RAM_OFFSET) $(REGS) -flto -fuse-linker-plugin


#define LUFA_VID					0x03EB
#define LUFA_PID					0x204A

#define ARDUINO_VID					0x2341
#define ARDUINO_UNO_PID				0x0043 // R3 (0001 R1)
#define ARDUINO_MEGA_PID			0x0042 // R3 (0010 R1)
#define ARDUINO_MEGA_ADK_PID		0x0044 // R3 (003F R1)

HOODLOADER2_OPTS  = -DVENDORID=ARDUINO_VID
HOODLOADER2_OPTS += -DPRODUCTID=ARDUINO_UNO_PID


# Flash size and bootloader section sizes of the target, in KB. These must
# match the target's total FLASH size and the bootloader size set in the
# device's fuses.
FLASH_SIZE_KB         = 16
BOOT_SECTION_SIZE_KB  = 4


# Add an offset of 256 + 128 bytes to RAMSTART for the ringbuffers
# u2 and u4 Series (RAMSTART = 0x100):
RAM_OFFSET            = 0x800280

# Reserved registers for faster USB-Serial convertion
REG0  = r2
REG1  = r3
REG2  = r4
REG3  = r5 # needs to be REG2+1!
REGS  = -ffixed-$(REG0) -ffixed-$(REG1) -ffixed-$(REG2) -ffixed-$(REG3)

HOODLOADER2_OPTS += -DTEMP_REG0=$(REG0)
HOODLOADER2_OPTS += -DTEMP_REG1=$(REG1)
HOODLOADER2_OPTS += -DTEMP_REG2=$(REG2)
HOODLOADER2_OPTS += -DTEMP_REG3=$(REG3)


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
