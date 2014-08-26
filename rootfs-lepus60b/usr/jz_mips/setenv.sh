#!/bin/sh

export TSLIB_ROOT=/usr/tslib

export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb
export TSLIB_TSDEVICE=/dev/ts
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts

export PATH=$TSLIB_ROOT/bin:$PATH
export LD_LIBRARY_PATH=$TSLIB_ROOT/lib:$LD_LIBRARY_PATH

#run 'ts_calibrate' to calibrate the touch panel.
#run 'ts_test' to test the touch panel.
