#
# Common include file for GNU make build
#

# Include this once only

ifndef COMMON_INC_INCLUDED
COMMON_INC_INCLUDED := 1

include build/version.inc

SHELL ?= /bin/sh

# Detect the system, as necessary

SYSTEM ?= $(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')

# Include the system-specific settings

SYSTEM.SUPPORTED ?= $(shell test -f config/Makefile.$(SYSTEM) && echo 1)

ifeq ($(SYSTEM.SUPPORTED), 1)
include config/Makefile.$(SYSTEM)
else
$(error "Platform '$(SYSTEM)' not supported")
endif

# Default tools

AR      ?= ar
INSTALL ?= install
RANLIB  ?= ranlib
STRIP   ?= strip
RM      ?= rm -f
LN      ?= ln -sf
CXX     ?= $(CC)
CP      ?= cp -f
MKDIR   ?= mkdir -p

# Release mode is the default

ifndef MODE
MODE := release
endif

ifeq ($(MODE),debug)
OPT   ?= $(CFLAGS.DEBUG)
STRIP :=
endif

ifeq ($(MODE),release)
OPT   ?= $(CFLAGS.RELEASE)
endif

ifndef V
LOG_CXX     ?= @echo " [CXX]     $@";
LOG_CC      ?= @echo " [CC]      $@";
LOG_LD      ?= @echo " [LD]      $@";
LOG_AR      ?= @echo " [AR]      $@";
LOG_RANLIB  ?= @echo " [RANLIB]  $@";
LOG_STRIP   ?= @echo " [STRIP]   $@";
LOG_LN      ?= @echo " [LN]      $@";
LOG_CP      ?= @echo " [CP]      $@";
LOG_MKDIR   ?= @echo " [MKDIR]   $@";
LOG_ABIDUMP ?= @echo " [ABIDUMP] $@";
endif

INCLUDE ?= -Iinclude

override CFLAGS := $(OPT) $(CFLAGS) $(WARN) $(INCLUDE) $(CFLAGS.EXTRA)

# Default target is all

all::

endif
