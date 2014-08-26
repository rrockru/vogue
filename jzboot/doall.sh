#!/bin/sh

sudo ./mkyaffs2image 1 ../rootfs/ root.fs
sudo ./jzboot -i 0 -C script/flash_rrock_linux.scr
