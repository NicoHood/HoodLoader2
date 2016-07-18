#!/bin/bash

cat vidpid.txt | while read mcu vid pid name; do
    make clean
    make VID=$vid PID=$pid MCU=$mcu NAME="$name" "$@" all
    # Please tell me where to find thebuild products...
    #mv build-product.ihex hoodloader_$mcu_$vid_$pid.ihex
done
