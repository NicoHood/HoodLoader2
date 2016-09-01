#!/usr/bin/env python2

# Import initelhex: https://pythonhosted.org/IntelHex/index.html
import sys
import os
import hashlib
from intelhex import IntelHex

def main():
    # Check if one input argument is provided
    if len(sys.argv) != 2:
        print "Usage:", sys.argv[0], "bootloader.hex"
        sys.exit(1)

    # Input arguments
    hexfile = sys.argv[1]

    # Check if file exists
    if not os.path.isfile(hexfile):
        print "Error: File does not exist."
        sys.exit(2)

    # Read bootloader data
    bootloader = IntelHex(hexfile)
    bootloader_bin = bootloader.tobinarray()
    #bootloader.dump()

    # Start address
    loaderStart = bootloader.minaddr()

    # Given a start address, deduce where the bootloader ends
    end_addresses = {
        0x1000: 0x2000,
        0x1C00: 0x2000,
        0x1D00: 0x2000,
        0x1E00: 0x2000,
        0x3000: 0x4000,
        0x3800: 0x4000,
        0x3E00: 0x4000,
        0x7000: 0x8000,
        0x7800: 0x8000,
        0x7E00: 0x8000,
        0xF800: 0x10000,
        0x1F000: 0x20000,
        0x1FC00: 0x20000,
        0x3E000: 0x40000,
    }
    if not loaderStart in end_addresses:
        print "Error: Unkown bootloader start address."
        sys.exit(3)
    loaderEnd = end_addresses[loaderStart]

    # Len
    loaderLen = loaderEnd - loaderStart
    if loaderLen < len(bootloader_bin):
        print "Error: Invalid bootloader length."
        sys.exit(4)

    # Calculate md5sum from hexfile and add padding bytes with 0xFF
    md5 = hashlib.md5()
    md5.update(bootloader_bin)
    padding = [0xFF] * (loaderLen - len(bootloader_bin))
    md5.update(bytearray(padding))

    # Calculatae md5sum from the original file so we know which disk file it came from
    filemd5 = hashlib.md5()
    fd = open(hexfile, 'rb')
    filemd5.update(fd.read())
    fd.close()

    # Filename without full path and without ".hex"
    filename = os.path.splitext(os.path.basename(hexfile))[0]

    # Print header
    print '// File =', filename + ".hex"
    print '// Loader start:', hex(loaderStart), 'length', loaderLen
    print '// Bootloader MD5 sum =', md5.hexdigest()
    print '// Original file MD5 sum =', filemd5.hexdigest()
    print
    print 'const uint8_t', filename + '_hex [] PROGMEM = {'

    # Print data
    line = ""
    for i in range(len(bootloader_bin)):
        line += hex(bootloader_bin[i]) + ', '
        if i % 16 == 15:
            print line
            line = ''
    if line:
        print line

    # Print footer
    print '}; // end of', filename + '_hex'

if __name__ == "__main__":
    main()
