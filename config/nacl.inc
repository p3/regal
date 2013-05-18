NAME = $(REGAL_NAME)

ifeq ($(NACL_SDK_ROOT),)
$(error NACL_SDK_ROOT needed for NaCl builds)
endif

#
# Specify on the command-line:
#    NACL_LIBC=newlib
# or NACL_LIBC=glibc
#

NACL_LIBC ?= newlib

ifeq ($(NACL_LIBC),glibc)
PICFLAG = -fPIC
endif

#
# Detect the OS - linux, mac or win
#

NACL_OS=$(shell uname -s)

ifeq ($(NACL_OS),Linux)
  NACL_OS=linux
else
  ifeq ($(NACL_OS),Darwin)
    NACL_OS=mac
  else
    NACL_OS=win
  endif
endif

# ccache by default hashes the compiler’s mtime and size, but this
# is insufficient for distinguising between glibc and newlib NaCl
# variants
#
# $ ls -la pepper_25/toolchain/mac_x86_*/bin/i686-nacl-g++
# -rwxr-xr-x  2 user  user  384 Oct  1 09:21 pepper_25/toolchain/mac_x86_glibc/bin/i686-nacl-g++
# -rwxr-xr-x  2 user  user  384 Oct  1 09:21 pepper_25/toolchain/mac_x86_newlib/bin/i686-nacl-g++
#
# We catenate the SYSTEM variable, NACL_LIBC and NACL_SDK_ROOT for keeping
# the glibc and newlib outputs separate, and per-sdk-version separate.

export CCACHE_COMPILERCHECK=echo $(SYSTEM)$(NACL_LIBC)$(NACL_SDK_ROOT)

ifeq ($(NACL_ARCH),i686)
NACL_LIBTYPE=$(NACL_LIBC)_x86_32
else
NACL_LIBTYPE=$(NACL_LIBC)_$(NACL_ARCH)
endif

ifeq ($(MODE),debug)
LDFLAGS.EXTRA += -L$(NACL_SDK_ROOT)/ports/lib/$(NACL_LIBTYPE)/Debug
LDFLAGS.EXTRA += -L$(NACL_SDK_ROOT)/lib/$(NACL_LIBTYPE)/Debug
else
LDFLAGS.EXTRA += -L$(NACL_SDK_ROOT)/ports/lib/$(NACL_LIBTYPE)/Release
LDFLAGS.EXTRA += -L$(NACL_SDK_ROOT)/lib/$(NACL_LIBTYPE)/Release
endif

NMF_FLAGS += -L$(NACL_SDK_ROOT)/ports/lib/$(NACL_LIBTYPE)/Release
NMF_FLAGS += -L$(NACL_SDK_ROOT)/lib/$(NACL_LIBTYPE)/Release

CFLAGS.EXTRA += -I$(NACL_SDK_ROOT)/include -MMD
WARN = -Wall -Wno-unused-parameter -Wno-char-subscripts -Wno-strict-aliasing
# WARN += -Werror
CFLAGS.DEBUG = -g
CFLAGS.RELEASE = -O2
