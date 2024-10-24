#!/bin/bash

SHELL_FOLDER=$(dirname $(readlink -f "$0"))
BUILDROOT_FOLDER=${SHELL_FOLDER}/../../../../

echo 'build install package...'
cd $SHELL_FOLDER
PACDIR=CyberRT


if [ -d $PACDIR ]; then
    rm -r $PACDIR
fi

echo "---sgx---"
pwd
echo $PACDIR

mkdir -p $PACDIR
cp -p $SHELL_FOLDER/bashrc $PACDIR

mkdir -p $PACDIR/bin
cp -pr $SHELL_FOLDER/bin/* $PACDIR/bin


mkdir -p $PACDIR/lib
cp -p $SHELL_FOLDER/lib/lib* $PACDIR/lib
cp -pr $SHELL_FOLDER/lib/python3.10/ $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libfastcdr* $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libfastrtps* $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libgflags* $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libglog* $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libgtest* $PACDIR/lib
cp -p $BUILDROOT_FOLDER/output/rockchip_rk3588/target/usr/lib/libprotobuf* $PACDIR/lib

mkdir -p $PACDIR/source
cp -pr $SHELL_FOLDER/share $PACDIR/source
cp -p $SHELL_FOLDER/setup.bash $PACDIR/source
cp -p $SHELL_FOLDER/setup.zsh $PACDIR/source
cp -p $SHELL_FOLDER/cyber_tools_auto_complete.bash $PACDIR/source


