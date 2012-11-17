#!/bin/bash
# Copyright (c) 2012 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# nacl-regal.sh
#
# usage:  nacl-regal.sh
#
# this script downloads, patches, and builds ffmpeg for Native Client 
#

readonly PACKAGE_NAME=regal
export NACL_GLIBC=1
export NACL_PACKAGES_BITSIZE=64

source ./build/nacl/common.sh

export CC=${NACLCC}
export CXX=${NACLCXX}
export LD=${NACLLD}
export AR=${NACLAR}
export STRIP=${NACLSTRIP}
export RANLIB=${NACLRANLIB}
export PKG_CONFIG_PATH=${NACL_SDK_USR_LIB}/pkgconfig
export PKG_CONFIG_LIBDIR=${NACL_SDK_USR_LIB}
export FREETYPE_CONFIG=${NACL_SDK_USR_BIN}/freetype-config
export PATH=${NACL_BIN_PATH}:${PATH};

exec make -f Makefile SYSTEM=nacl-${NACL_PACKAGES_BITSIZE} $*
