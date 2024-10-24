#!/bin/bash

SHELL_FOLDER=$(dirname $(readlink -f "$0"))
BUILDROOT_FOLDER=${SHELL_FOLDER}/../../../../

echo 'install CyberRT to rk3588...'
cd $SHELL_FOLDER
PACDIR=CyberRT

adb shell 'rm -rf /userdata/CyberRT'

adb push CyberRT /userdata/apps/


