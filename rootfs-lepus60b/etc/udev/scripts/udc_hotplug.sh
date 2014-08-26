#!/bin/sh

if [ "$1" == "offline" ]; then
	/sbin/rmmod g_file_storage.ko
fi

if [ "$1" == "online" ]; then
	echo "udc hotplug"
	/sbin/insmod g_file_storage.ko file=/dev/ram0
fi

