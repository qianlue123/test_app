#!/bin/bash 

set -u

# 项目和openwrt编译源码都在Desktop (在同一目录)
fatherdir=$(dirname `pwd`)
toolName="toolchain-mipsel_24kc_gcc-13.1.0_glibc"
pos="$fatherdir/openwrt-2305/"

cd test_app
rm -rf build
mkdir build && cd build 

export ARCH=mips
export STAGING_DIR=$pos

# tools can't use musl, it not exist libc.so
export CROSS_COMPILE=$pos/staging_dir/$toolName/bin/mipsel-openwrt-linux-

export LD_LIBRARY_PATH=$pos/staging_dir/target-mipsel_24kc_glibc/usr/lib
export PATH=$PATH:$pos/staging_dir/$toolName/bin/

qmake ..
make
