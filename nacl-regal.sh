#!/bin/bash
# Copyright (c) 2012 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# This script add the NaCl compilers to the PATH environment variable,
# based on $NACL_SDK_ROOT and then invokes make 3 times to build Regal
# for the tree different NaCl arhiitectures.
# Any arguments passed to this script will be passed onto make.  It is
# recommend to pass -j<N> to speed up the build.

set -e

if [ -z "${NACL_SDK_ROOT}" ]; then
    echo "error: NACL_SDK_ROOT not set"
    exit 1
fi

NACL_GLIBC=${NACL_GLIBC:=0}

if [ ${NACL_GLIBC} == 1 ]; then
    LIBC=glibc
else
    LIBC=newlib
fi

OSNAME=$(uname -s)
if [ ${OSNAME} == "Linux" ]; then
    OSNAME=linux
elif [ ${OSNAME} == "Darwin" ]; then
    OSNAME=mac
else
    OSNAME=win
fi

X86_BIN_DIR=${NACL_SDK_ROOT}/toolchain/${OSNAME}_x86_${LIBC}/bin
ARM_BIN_DIR=${NACL_SDK_ROOT}/toolchain/${OSNAME}_arm_newlib/bin

export NACL_GLIBC
export NACL_ARCH
export PATH=${PATH}:${X86_BIN_DIR}:${ARM_BIN_DIR}

echo "Building x86_64"
make SYSTEM=nacl-x86_64 $*
echo "Building i686"
make SYSTEM=nacl-i686 $*

unset NACL_GLIBC
echo "Building ARM"
make SYSTEM=nacl-arm $*
