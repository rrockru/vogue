#!/bin/sh


if [ "$ACTION" == "add" ]
then
    /bin/mount -t vfat -o codepage=936,iocharset=cp936 /dev/$1 /mnt/usb
elif [ "$ACTION" == "remove" ]
then
#    fuser -k -m /mnt/usb
    sleep 3
    /bin/umount /mnt/usb
fi
