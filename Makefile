# To build in debug mode:
#   - use MODE=debug on gmake command-line
#
# To build using ccache (http://ccache.samba.org/)
#   - use CCACHE=ccache on gmake command-line
#
# To disable stripping of binaries either:
#   - use MODE=debug on gmake command-line
#   - use STRIP= on gmake command-line
#   - edit this makefile to set STRIP to the empty string
#
# To disable symlinks:
#   - use LN= on gmake command-line
#
# To specify additional compiler flags:
#   - use CFLAGS= on gmake command-line
#
# To see verbose output
#   - use V=1 on gmake command-line

include config/version

SHELL = /bin/sh
ifndef SYSTEM
SYSTEM := $(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')
endif
SYSTEM.SUPPORTED = $(shell test -f config/Makefile.$(SYSTEM) && echo 1)

ifeq ($(SYSTEM.SUPPORTED), 1)
include config/Makefile.$(SYSTEM)
else
$(error "Platform '$(SYSTEM)' not supported")
endif

include build/regal.inc
include build/regaltest.inc
include build/zlib.inc
include build/libpng.inc
include build/snappy.inc
include build/apitrace.inc
include build/glu.inc
include build/glut.inc

REGAL_DEST ?= /usr
BINDIR     ?= $(REGAL_DEST)/bin
LIBDIR     ?= $(REGAL_DEST)/lib

AR      ?= ar
INSTALL ?= install
RANLIB  ?= ranlib
STRIP   ?= strip
RM      ?= rm -f
LN      ?= ln -sf
CXX     ?= $(CC)

# Release mode is the default

ifeq ($(MODE),)
MODE := release
endif

ifeq ($(MODE),debug)
OPT = $(CFLAGS.DEBUG)
STRIP :=
endif

ifeq ($(MODE),release)
OPT = $(CFLAGS.RELEASE)
endif

ifndef V
LOG_CXX = @echo " [CXX] $@";
LOG_CC = @echo " [CC] $@";
LOG_LD = @echo " [LD] $@";
LOG_AR = @echo " [AR] $@";
LOG_RANLIB = @echo " [RANLIB] $@";
LOG_STRIP = @echo " [STRIP] $@";
endif

INCLUDE = -Iinclude

override CFLAGS := $(OPT) $(CFLAGS) $(WARN) $(INCLUDE) $(CFLAGS.EXTRA)

#
# Default target: all
#

all: regal.lib glew.lib regal.bin

# GLU and GLUT for platforms other than NaCl and Mac

ifneq ($(filter-out nacl% darwin% emscripten%,$(SYSTEM)),)
all: glu.lib glut.lib
endif

#
# Regenerate Regal sources
#

export:
	python scripts/Export.py --api gl 4.2 --api wgl 4.0 --api glx 4.0 --api cgl 1.4 --api egl 1.0 --outdir .

#
# zlib
#

ZLIB.SRCS       := $(ZLIB.C)
ZLIB.SRCS.NAMES := $(notdir $(ZLIB.SRCS))
ZLIB.OBJS       := $(addprefix tmp/$(SYSTEM)/zlib/static/,$(ZLIB.SRCS.NAMES))
ZLIB.OBJS       := $(ZLIB.OBJS:.c=.o)
ZLIB.DEPS       := $(ZLIB.OBJS:.o=.d)
ZLIB.CFLAGS     := -Isrc/zlib/include
ZLIB.STATIC     := libz.a

-include $(ZLIB.DEPS)

ifneq ($(filter linux%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter darwin%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

zlib.lib: lib/$(SYSTEM)/$(ZLIB.STATIC)

tmp/$(SYSTEM)/zlib/static/%.o: src/zlib/src/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(ZLIB.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(SYSTEM)/$(ZLIB.STATIC): $(ZLIB.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(ZLIB.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# libpng
#

LIBPNG.SRCS       := $(LIBPNG.C)
LIBPNG.SRCS.NAMES := $(notdir $(LIBPNG.SRCS))
LIBPNG.OBJS       := $(addprefix tmp/$(SYSTEM)/libpng/static/,$(LIBPNG.SRCS.NAMES))
LIBPNG.OBJS       := $(LIBPNG.OBJS:.c=.o)
LIBPNG.DEPS       := $(LIBPNG.DEPS:.o=.d)
LIBPNG.CFLAGS     := -Isrc/zlib/include -Isrc/libpng/include
LIBPNG.STATIC     := libpng.a

-include $(LIBPNG.DEPS)

ifneq ($(filter linux%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter darwin%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

libpng.lib: zlib.lib lib/$(SYSTEM)/$(LIBPNG.STATIC)

tmp/$(SYSTEM)/libpng/static/%.o: src/libpng/src/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIBPNG.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(SYSTEM)/$(LIBPNG.STATIC): $(LIBPNG.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(LIBPNG.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# snappy
#

SNAPPY.SRCS       := $(SNAPPY.CXX)
SNAPPY.SRCS.NAMES := $(notdir $(SNAPPY.SRCS))
SNAPPY.OBJS       := $(addprefix tmp/$(SYSTEM)/snappy/static/,$(SNAPPY.SRCS.NAMES))
SNAPPY.OBJS       := $(SNAPPY.OBJS:.c=.o)
SNAPPY.OBJS       := $(SNAPPY.OBJS:.cc=.o)
SNAPPY.DEPS       := $(SNAPPY.DEPS:.o=.d)
SNAPPY.CFLAGS     := -Isrc/snappy
SNAPPY.STATIC     := libsnappy.a

-include $(SNAPPY.DEPS)

ifeq ($(MODE),release)
SNAPPY.CFLAGS         += -DNDEBUG
endif

ifneq ($(filter linux%,$(SYSTEM)),)
SNAPPY.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter darwin%,$(SYSTEM)),)
SNAPPY.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
SNAPPY.CFLAGS     += -DHAVE_UNISTD_H
endif

snappy.lib: lib/$(SYSTEM)/$(SNAPPY.STATIC)

tmp/$(SYSTEM)/snappy/static/%.o: src/snappy/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(SNAPPY.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

tmp/$(SYSTEM)/snappy/static/%.o: src/snappy/%.cc
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CXX) $(SNAPPY.CFLAGS) $(CFLAGS) $(CXXFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(SYSTEM)/$(SNAPPY.STATIC): $(SNAPPY.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(SNAPPY.OBJS)

#
# apitrace
#

APITRACE.SRCS       := $(APITRACE.CXX)
APITRACE.SRCS       := $(filter %.c,$(APITRACE.SRCS)) $(filter %.cc,$(APITRACE.SRCS)) $(filter %.cpp,$(APITRACE.SRCS))
APITRACE.SRCS.NAMES := $(notdir $(APITRACE.SRCS))
APITRACE.OBJS       := $(addprefix tmp/$(SYSTEM)/apitrace/static/,$(APITRACE.SRCS.NAMES))
APITRACE.OBJS       := $(APITRACE.OBJS:.c=.o) $(APITRACE.OBJS:.cc=.o) $(APITRACE.OBJS:.cpp=.o)
APITRACE.OBJS       := $(filter %.o,$(APITRACE.OBJS))
APITRACE.DEPS       := $(APITRACE.OBJS:.o=.d)
APITRACE.CFLAGS     := -Isrc/apitrace/common -Isrc/apitrace/dispatch -Isrc/apitrace/helpers -Isrc/apitrace/wrappers -Isrc/apitrace
APITRACE.CFLAGS     += -Isrc/apitrace/thirdparty/khronos
APITRACE.CFLAGS     += -Isrc/regal -Isrc/snappy -Isrc/zlib/include -Isrc/zlib/src -Isrc/boost
APITRACE.STATIC     := libapitrace.a

APITRACE.CFLAGS     += -DTRACE_OS_LOG=0 -DTRACE_BACKTRACE=0

ifneq ($(filter linux%,$(SYSTEM)),)
APITRACE.CFLAGS    += -DHAVE_X11=1
endif

ifeq ($(MODE),release)
APITRACE.CFLAGS         += -DNDEBUG
APITRACE.CFLAGS         += -DREGAL_NO_ASSERT=1
endif

-include $(APITRACE.DEPS)

apitrace.lib: lib/$(SYSTEM)/$(APITRACE.STATIC)

tmp/$(SYSTEM)/apitrace/static/%.o: src/apitrace/common/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(APITRACE.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

tmp/$(SYSTEM)/apitrace/static/%.o: src/apitrace/dispatch/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(APITRACE.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

tmp/$(SYSTEM)/apitrace/static/%.o: src/apitrace/helpers/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(APITRACE.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

tmp/$(SYSTEM)/apitrace/static/%.o: src/apitrace/wrappers/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(APITRACE.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(SYSTEM)/$(APITRACE.STATIC): $(APITRACE.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(APITRACE.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#

LIB.LDFLAGS        := -lstdc++ -pthread -lm
LIB.LIBS           :=

ifeq ($(filter nacl%,$(SYSTEM)),)
LIB.LDFLAGS        += -ldl
endif

LIB.SRCS           := $(REGAL.CXX)

# Disable mongoose and Regal HTTP for NaCl and Emscripten build

ifeq ($(filter nacl% emscripten,$(SYSTEM)),)
LIB.SRCS           += src/mongoose/mongoose.c
endif

ifeq ($(filter -DREGAL_NO_MD5%,$(CFLAGS)),)
LIB.SRCS           += src/md5/src/md5.c
endif

LIB.SRCS           += src/jsonsl/jsonsl.c

LIB.INCLUDE        += -Isrc/zlib/include
LIB.INCLUDE        += -Isrc/libpng/include
LIB.INCLUDE        += -Isrc/mongoose
LIB.INCLUDE        += -Isrc/md5/include
LIB.INCLUDE        += -Isrc/lookup3
LIB.INCLUDE        += -Isrc/jsonsl

# Optional flags

# REGAL_SYS_ES1 for ES1 back-end support, disabled by default
# LIB.CFLAGS     += -DREGAL_SYS_ES1=1

#
# Add debug-specific flags
#

ifeq ($(MODE),debug)
endif

#
# In release mode we're aggressive about leaving out functionality
# in order to minimize the footprint of libRegal.so.1
#

ifeq ($(MODE),release)
LIB.CFLAGS         += -DNDEBUG
LIB.CFLAGS         += -DREGAL_DECL_EXPORT=1
LIB.CFLAGS         += -DREGAL_LOG_ALL=0
LIB.CFLAGS         += -DREGAL_LOG_ONCE=0
LIB.CFLAGS         += -DREGAL_LOG_JSON=0
LIB.CFLAGS         += -DREGAL_NO_HTTP=1
LIB.CFLAGS         += -DREGAL_NO_ASSERT=1
LIB.CFLAGS         += -DREGAL_NO_PNG=1
LIB.CFLAGS         += -fomit-frame-pointer
REGAL_LOG          ?= 0
REGAL_ERROR        ?= 0
REGAL_CACHE        ?= 0
REGAL_DEBUG        ?= 0
REGAL_EMULATION    ?= 1                      # 0 for "loader only"
REGAL_NO_TLS       ?= 0                      # 1 for single threaded
endif

#
# Flags for custom mode
#

ifeq ($(MODE),custom)
endif

#

ifneq ($(REGAL_LOG),)
LIB.CFLAGS         += -DREGAL_LOG=$(REGAL_LOG)
endif

ifneq ($(REGAL_ERROR),)
LIB.CFLAGS         += -DREGAL_ERROR=$(REGAL_ERROR)
endif

ifneq ($(REGAL_CACHE),)
LIB.CFLAGS         += -DREGAL_CACHE=$(REGAL_CACHE)
endif

ifneq ($(REGAL_DEBUG),)
LIB.CFLAGS         += -DREGAL_DEBUG=$(REGAL_DEBUG)
endif

ifneq ($(REGAL_EMULATION),)
LIB.CFLAGS         += -DREGAL_EMULATION=$(REGAL_EMULATION)
endif

ifneq ($(REGAL_NO_TLS),)
LIB.CFLAGS         += -DREGAL_NO_TLS=$(REGAL_NO_TLS)
endif

LIB.CFLAGS         += -fvisibility=hidden

LIB.INCLUDE        += -Isrc/boost

LIB.SRCS.NAMES     := $(notdir $(LIB.SRCS))

LIB.HEADERS        :=
LIB.HEADERS        += include/GL/Regal.h
LIB.HEADERS        += src/regal/RegalPrivate.h

LIB.OBJS           := $(addprefix tmp/$(SYSTEM)/regal/static/,$(LIB.SRCS.NAMES))
LIB.OBJS           := $(LIB.OBJS:.c=.o)
LIB.OBJS           := $(LIB.OBJS:.cpp=.o)
LIB.DEPS           := $(LIB.OBJS:.o=.d)
LIB.SOBJS          := $(addprefix tmp/$(SYSTEM)/regal/shared/,$(LIB.SRCS.NAMES))
LIB.SOBJS          := $(LIB.SOBJS:.c=.o)
LIB.SOBJS          := $(LIB.SOBJS:.cpp=.o)
LIB.SDEPS          := $(LIBS.SOBJS:.o=.d)

-include $(LIB.DEPS) $(LIB.SDEPS)

LIB.LIBS           += -Llib/$(SYSTEM)
ifneq ($(filter-out nacl% emscripten%,$(SYSTEM)),)
LIB.LIBS           += -lapitrace -lsnappy
endif
LIB.LIBS           += -lpng -lz $(LDFLAGS.X11)

ifneq ($(filter darwin%,$(SYSTEM)),)
LIB.LDFLAGS        += -Wl,-reexport-lGLU -L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
endif

ifneq ($(LIB.STATIC),)
regal.lib: lib/$(SYSTEM)/$(LIB.STATIC)

ifneq ($(filter-out nacl% emscripten%,$(SYSTEM)),)
lib/$(SYSTEM)/$(LIB.STATIC): lib/$(SYSTEM)/$(APITRACE.STATIC) lib/$(SYSTEM)/$(SNAPPY.STATIC)
endif
endif # if building static lib

ifneq ($(LIB.SHARED),)
ifeq ($(filter nacl%,$(SYSTEM)),)
regal.lib: lib/$(SYSTEM)/$(LIB.SHARED)
else
 ifeq ($(NACL_LIBC),glibc)
regal.lib: lib/$(SYSTEM)/$(LIB.SHARED)
 endif
endif
endif # if building shared lib

lib/$(SYSTEM)/$(LIB.STATIC): lib/$(SYSTEM)/$(LIBPNG.STATIC) lib/$(SYSTEM)/$(ZLIB.STATIC) $(LIB.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(LIB.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

ifneq ($(filter-out nacl% emscripten%,$(SYSTEM)),)
lib/$(SYSTEM)/$(LIB.SHARED): lib/$(SYSTEM)/$(LIBPNG.STATIC) lib/$(SYSTEM)/$(ZLIB.STATIC) $(LIB.SOBJS) lib/$(SYSTEM)/$(APITRACE.STATIC) lib/$(SYSTEM)/$(SNAPPY.STATIC)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.SO) -o $@ $(LDFLAGS.STARTGROUP) $(LIB.SOBJS) lib/$(SYSTEM)/$(APITRACE.STATIC) $(LDFLAGS.ENDGROUP) $(LIB.LIBS) $(LIB.LDFLAGS)
else
lib/$(SYSTEM)/$(LIB.SHARED): lib/$(SYSTEM)/$(LIBPNG.STATIC) lib/$(SYSTEM)/$(ZLIB.STATIC) $(LIB.SOBJS)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.SO) -o $@ $(LIB.SOBJS) $(LIB.LIBS) $(LIB.LDFLAGS)
endif
ifneq ($(LN),)
	$(LN) $(LIB.SHARED) lib/$(SYSTEM)/$(LIB.SONAME)
	$(LN) $(LIB.SHARED) lib/$(SYSTEM)/$(LIB.DEVLNK)
ifneq ($(LIB.FRAMEWORK),)
	$(LN) $(LIB.SHARED) lib/$(SYSTEM)/$(LIB.FRAMEWORK)
endif
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

tmp/$(SYSTEM)/regal/static/%.o: src/regal/%.cpp $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/regal/%.cpp $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/mongoose/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/mongoose/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/md5/src/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/md5/src/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/jsonsl/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/jsonsl/%.c $(LIB.HEADERS)
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

#
# RegalGLEW
#

GLEW.SRCS        += src/glew/src/glew.c
GLEW.SRCS.NAMES := $(notdir $(GLEW.SRCS))
GLEW.OBJS       := $(addprefix tmp/$(SYSTEM)/glew/shared/,$(GLEW.SRCS.NAMES))
GLEW.OBJS       := $(GLEW.OBJS:.c=.o)
GLEW.DEPS       := $(GLEW.OBJS:.o=.d)
GLEW.CFLAGS     := -Isrc/glew/include -Isrc/glu/include -DGLEW_EXPORTS -DGLEW_BUILD -DGLEW_REGAL
GLEW.LIBS       := -Llib/$(SYSTEM) -lRegal
GLEW.SHARED     := libRegalGLEW.$(EXT.DYNAMIC)
GLEW.STATIC     := libRegalGLEW.a

-include $(GLEW.DEPS)

ifeq ($(filter emscripten%,%(SYSTEM)),)
glew.lib: lib/$(SYSTEM)/$(GLEW.STATIC)
endif

ifeq ($(filter nacl%,$(SYSTEM)),)
glew.lib: lib/$(SYSTEM)/$(GLEW.SHARED)
endif

tmp/$(SYSTEM)/glew/shared/%.o: src/glew/src/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(PICFLAG) $(GLEW.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(SYSTEM)/$(GLEW.STATIC): $(GLEW.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(GLEW.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

lib/$(SYSTEM)/$(GLEW.SHARED): $(GLEW.OBJS) lib/$(SYSTEM)/$(LIB.SHARED)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) -o $@ $(GLEW.OBJS) $(GLEW.LIBS) $(LIB.LDFLAGS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# RegalGLU
#

GLU.SRCS       := $(GLU.C) $(GLU.CXX)
GLU.SRCS.NAMES := $(notdir $(GLU.SRCS))
GLU.OBJS       := $(addprefix tmp/$(SYSTEM)/glu/shared/,$(GLU.SRCS.NAMES))
GLU.OBJS       := $(GLU.OBJS:.c=.o) $(GLU.OBJS:.cc=.o)
GLU.OBJS       := $(filter %.o,$(GLU.OBJS))
GLU.DEPS       := $(GLU.OBJS:.o=.d)
GLU.CFLAGS     := -Isrc/glu/include -Isrc/glu/libnurbs/interface -Isrc/glu/libnurbs/internals -Isrc/glu/libnurbs/nurbtess
GLU.CFLAGS     += -DLIBRARYBUILD
GLU.LIBS       := -Llib/$(SYSTEM) -lRegal
GLU.LIBS       += -pthread -lm
GLU.SHARED     := libRegalGLU.$(EXT.DYNAMIC)
GLU.STATIC     := libRegalGLU.a

-include $(GLU.DEPS)

glu.lib: lib/$(SYSTEM)/$(GLU.STATIC)

ifeq ($(filter nacl%,$(SYSTEM)),)
glu.lib: lib/$(SYSTEM)/$(GLU.SHARED)
endif

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libtess/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libutil/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/interface/%.cc
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/internals/%.cc
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/nurbtess/%.cc
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(SYSTEM)/$(GLU.SHARED): $(GLU.OBJS) lib/$(SYSTEM)/$(LIB.SHARED)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) $(GLU.LIBS) $(LIB.LDFLAGS) -o $@ $(GLU.OBJS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

lib/$(SYSTEM)/$(GLU.STATIC): $(GLU.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(GLU.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# RegalGLUT
#

GLUT.SRCS       := $(GLUT.C)
GLUT.SRCS.NAMES := $(notdir $(GLUT.SRCS))
GLUT.OBJS       := $(addprefix tmp/$(SYSTEM)/glut/shared/,$(GLUT.SRCS.NAMES))
GLUT.OBJS       := $(GLUT.OBJS:.c=.o)
GLUT.DEPS       := $(GLUT.OBJS:.o=.d)
GLUT.CFLAGS     := -Isrc/glut/include -Isrc/glu/include -DBUILD_GLUT32 -DGLUT_BUILDING_LIB -DGLUT_STATIC
#GLUT.CFLAGS     += -DCDECL=
GLUT.LIBS       := -Llib/$(SYSTEM) -lRegal -lRegalGLU
GLUT.LIBS       += -lX11 -lXmu -lXi
GLUT.LIBS       += -pthread -lm
GLUT.SHARED     := libRegalGLUT.$(EXT.DYNAMIC)
GLUT.STATIC     := libRegalGLUT.a

-include $(GLUT.DEPS)

ifeq ($(filter nacl%,$(SYSTEM)),)
glut.lib: lib/$(SYSTEM)/$(GLUT.SHARED)
endif

tmp/$(SYSTEM)/glut/shared/%.o: src/glut/src/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(GLUT.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) -o $@ -c $<

lib/$(SYSTEM)/$(GLUT.SHARED): $(GLUT.OBJS) lib/$(SYSTEM)/$(GLU.SHARED) lib/$(SYSTEM)/$(LIB.SHARED)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) -o $@ $(GLUT.OBJS) $(GLUT.LIBS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# RegalGLEW glewinfo
#

GLEWINFO.SRCS       += src/glew/src/glewinfo.c
GLEWINFO.SRCS.NAMES := $(notdir $(GLEWINFO.SRCS))
GLEWINFO.OBJS       := $(addprefix tmp/$(SYSTEM)/glewinfo/static/,$(GLEWINFO.SRCS.NAMES))
GLEWINFO.OBJS       := $(GLEWINFO.OBJS:.c=.o)
GLEWINFO.DEPS       := $(GLEWINFO.OBJS:.o=.d)
GLEWINFO.CFLAGS     := -Iinclude -Isrc/glew/include -DGLEW_REGAL -DGLEW_NO_GLU
GLEWINFO.LIBS       += -Llib/$(SYSTEM) -lRegal $(LDFLAGS.X11) -lRegalGLEW $(LDFLAGS.GLUT) $(LDFLAGS.GLU) $(LDFLAGS.AGL)

-include $(GLEWINFO.DEPS)

ifneq ($(filter linux%,$(SYSTEM)),)
GLEWINFO.LIBS       += -lX11
endif

tmp/$(SYSTEM)/glewinfo/static/%.o: src/glew/src/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(GLEWINFO.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/$(SYSTEM)/glewinfo$(BIN.SUFFIX): $(GLEWINFO.OBJS) lib/$(SYSTEM)/$(LIB.SHARED) lib/$(SYSTEM)/$(GLEW.SHARED)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(GLEWINFO.OBJS) $(GLEWINFO.LIBS) $(LIB.LDFLAGS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

# GLUT and GLU dependency for non-Nacl, non-Mac

ifneq ($(filter-out nacl% darwin% emscripten%,$(SYSTEM)),)
bin/$(SYSTEM)/glewinfo$(BIN.SUFFIX):      lib/$(SYSTEM)/$(GLUT.SHARED) lib/$(SYSTEM)/$(GLU.SHARED)
endif

# Examples

ifneq ($(filter nacl%,$(SYSTEM)),)
regal.bin: bin/$(SYSTEM)/nacl$(BIN.SUFFIX) examples/nacl/nacl.nmf
else
regal.bin: bin/$(SYSTEM)/glewinfo$(BIN.SUFFIX) bin/$(SYSTEM)/dreamtorus$(BIN.SUFFIX) bin/$(SYSTEM)/tiger$(BIN.SUFFIX)
ifeq ($(filter emscripten%,%(SYSTEM)),)
regal.bin: bin/$(SYSTEM)/regaltest$(BIN.SUFFIX)
endif
endif

# If we're not building a shared lib, then the binaries
# should depend on the static lib
ifneq ($(LIB.SHARED),)
DEPEND_REGAL_LIB = lib/$(SYSTEM)/$(LIB.SHARED)
else
DEPEND_REGAL_LIB = lib/$(SYSTEM)/$(LIB.STATIC)
endif

#
# dreamtorus
#

DREAMTORUS.SRCS       += examples/dreamtorus/src/render.cpp
DREAMTORUS.SRCS       += examples/dreamtorus/glut/code/main.cpp
DREAMTORUS.SRCS.NAMES := $(notdir $(DREAMTORUS.SRCS))
DREAMTORUS.OBJS       := $(addprefix tmp/$(SYSTEM)/dreamtorus/static/,$(DREAMTORUS.SRCS.NAMES))
DREAMTORUS.OBJS       := $(DREAMTORUS.OBJS:.cpp=.o)
DREAMTORUS.DEPS       := $(DREAMTORUS.DEPS:.o=.d)
DREAMTORUS.CFLAGS     := -Iinclude -Iexamples/dreamtorus/src
DREAMTORUS.LIBS       += -Llib/$(SYSTEM) $(LDFLAGS.GLUT) $(LDFLAGS.GLU) -lRegal $(LDFLAGS.X11)
DREAMTORUS.LIBS       += -lm -pthread

-include $(DREAMTORUS.DEPS)

tmp/$(SYSTEM)/dreamtorus/static/%.o: examples/dreamtorus/src/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(DREAMTORUS.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/dreamtorus/static/%.o: examples/dreamtorus/glut/code/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(DREAMTORUS.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/$(SYSTEM)/dreamtorus$(BIN.SUFFIX): $(DREAMTORUS.OBJS) $(DEPEND_REGAL_LIB)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(DREAMTORUS.OBJS) $(DREAMTORUS.LIBS) $(LIB.LDFLAGS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

# dreamtorus GLUT and GLU dependency for non-Mac, non-Nacl builds

ifneq ($(filter-out nacl% darwin% emscripten%,$(SYSTEM)),)
bin/$(SYSTEM)/dreamtorus$(BIN.SUFFIX): lib/$(SYSTEM)/$(GLUT.SHARED) lib/$(SYSTEM)/$(GLU.SHARED)
endif

#
# NaCl example
#

NACL.SRCS       += examples/nacl/main.c
NACL.SRCS.NAMES := $(notdir $(NACL.SRCS))
NACL.OBJS       := $(addprefix tmp/$(SYSTEM)/nacl/static/,$(NACL.SRCS.NAMES))
NACL.OBJS       := $(NACL.OBJS:.c=.o)
NACL.CFLAGS     := -Iinclude
NACL.LIBS       += -Llib/$(SYSTEM) -lRegal
NACL.LIBS       += -lpng -lz -lm -pthread -lppapi -lppapi_gles2 -lstdc++

tmp/$(SYSTEM)/nacl/static/%.o: examples/nacl/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CC) $(CFLAGS) -std=gnu99 $(NACL.CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/$(SYSTEM)/nacl$(BIN.SUFFIX): lib/$(SYSTEM)/$(LIB.STATIC) $(NACL.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(LD) $(LDFLAGS.EXTRA) -o $@ $(NACL.OBJS) $(NACL.LIBS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

examples/nacl/nacl.nmf: bin/$(SYSTEM)/nacl$(BIN.SUFFIX)
	$(NACL_SDK_ROOT)/tools/create_nmf.py -Llib/$(SYSTEM) $(NMF_FLAGS) -o $@ bin/$(SYSTEM)/nacl*.nexe -s examples/nacl


#
# tiger
#

TIGER.SRCS       += examples/tiger/nvpr_tiger.c
TIGER.SRCS       += examples/tiger/tiger.c
TIGER.SRCS       += examples/tiger/xform.c
TIGER.SRCS.NAMES := $(notdir $(TIGER.SRCS))
TIGER.OBJS       := $(addprefix tmp/$(SYSTEM)/tiger/static/,$(TIGER.SRCS.NAMES))
TIGER.OBJS       := $(TIGER.OBJS:.c=.o)
TIGER.DEPS       := $(TIGER.DEPS:.o=.d)
TIGER.CFLAGS     := -Iinclude -DGLEW_NO_GLU
TIGER.LIBS       += -Llib/$(SYSTEM) -lRegalGLEW $(LDFLAGS.GLUT) $(LDFLAGS.GLU) -lRegal $(LDFLAGS.X11)
TIGER.LIBS       += -lm -pthread

-include $(TIGER.DEPS)

tmp/$(SYSTEM)/tiger/static/%.o: examples/tiger/%.c
	@mkdir -p $(dir $@)
	$(LOG_CC)$(CCACHE) $(CC) $(TIGER.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/$(SYSTEM)/tiger$(BIN.SUFFIX): $(TIGER.OBJS) lib/$(SYSTEM)/$(GLEW.SHARED) lib/$(SYSTEM)/$(LIB.SHARED) lib/$(SYSTEM)/$(GLEW.SHARED)
	@mkdir -p $(dir $@)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(TIGER.OBJS) $(TIGER.LIBS)
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

# GLUT and GLU dependency for non-Nacl, non-Mac

ifneq ($(filter-out nacl% darwin% emscripten%,$(SYSTEM)),)
bin/$(SYSTEM)/tiger$(BIN.SUFFIX):      lib/$(SYSTEM)/$(GLUT.SHARED) lib/$(SYSTEM)/$(GLU.SHARED)
endif

######################################

#
# googletest + googlemock
#

GTEST.SRCS       += src/googletest/src/gtest-all.cc
GTEST.SRCS       += src/googlemock/src/gmock-all.cc
GTEST.SRCS.NAMES := $(notdir $(GTEST.SRCS))
GTEST.OBJS       := $(addprefix tmp/$(SYSTEM)/gtest/static/,$(GTEST.SRCS.NAMES))
GTEST.OBJS       := $(GTEST.OBJS:.cc=.o)
GTEST.DEPS       := $(GTEST.OBJS:.o=.d)
GTEST.CFLAGS     := -Isrc/googletest/include -Isrc/googletest -Isrc/googlemock/include -Isrc/googlemock
GTEST.STATIC     := libgtest.a

-include $(GTEST.DEPS)

tmp/$(SYSTEM)/gtest/static/gtest-all.o: src/googletest/src/gtest-all.cc
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CC) $(GTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/gtest/static/gmock-all.o: src/googlemock/src/gmock-all.cc
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CC) $(GTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(SYSTEM)/$(GTEST.STATIC): $(GTEST.OBJS)
	@mkdir -p $(dir $@)
	$(LOG_AR)$(CCACHE) $(AR) cr $@ $(GTEST.OBJS)
ifneq ($(RANLIB),)
	$(LOG_RANLIB)$(RANLIB) $@
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

#
# regaltest
#

REGALTEST.SRCS       += $(REGALTEST.CXX)
REGALTEST.SRCS.NAMES := $(notdir $(REGALTEST.SRCS))
REGALTEST.OBJS       := $(addprefix tmp/$(SYSTEM)/regaltest/static/,$(REGALTEST.SRCS.NAMES))
REGALTEST.OBJS       := $(REGALTEST.OBJS:.cpp=.o)
REGALTEST.DEPS       := $(REGALTEST.DEPS:.o=.d)
REGALTEST.CFLAGS     := -Isrc/googletest/include -Isrc/googlemock/include -Isrc/regal -Isrc/boost
REGALTEST.LIBS       := -Llib/$(SYSTEM) $(LDFLAGS.X11) -lm

ifeq ($(filter nacl%,$(SYSTEM)),)
REGALTEST.LIBS += -ldl
else
REGALTEST.LIBS += -lnacl_io
endif

-include $(REGALTEST.DEPS)

tmp/$(SYSTEM)/regaltest/static/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(LOG_CXX)$(CCACHE) $(CXX) $(LIB.CFLAGS) $(REGALTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

ifneq ($(filter nacl%,$(SYSTEM)),)
bin/$(SYSTEM)/regaltest$(BIN.SUFFIX): lib/$(SYSTEM)/$(APITRACE.STATIC) lib/$(SYSTEM)/$(SNAPPY.STATIC)
endif

bin/$(SYSTEM)/regaltest$(BIN.SUFFIX): $(REGALTEST.OBJS) lib/$(SYSTEM)/$(GTEST.STATIC) lib/$(SYSTEM)/$(LIB.STATIC) lib/$(SYSTEM)/$(LIBPNG.STATIC) lib/$(SYSTEM)/$(ZLIB.STATIC)
	@mkdir -p $(dir $@)
ifeq ($(filter nacl%,$(SYSTEM)),)
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(REGALTEST.OBJS) $(REGALTEST.LIBS) $(LDFLAGS.STARTGROUP) lib/$(SYSTEM)/$(LIB.STATIC) lib/$(SYSTEM)/$(APITRACE.STATIC) $(LDFLAGS.ENDGROUP) $(LIB.LIBS) lib/$(SYSTEM)/$(GTEST.STATIC) $(LIB.LDFLAGS) 
else
	$(LOG_LD)$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(REGALTEST.OBJS) $(REGALTEST.LIBS) lib/$(SYSTEM)/$(LIB.STATIC) $(LIB.LIBS) lib/$(SYSTEM)/$(GTEST.STATIC) $(LIB.LDFLAGS) 
endif
ifneq ($(STRIP),)
	$(LOG_STRIP)$(STRIP) -x $@
endif

ifneq ($(NACL_ARCH),arm)
test: bin/$(SYSTEM)/regaltest$(BIN.SUFFIX)
	@echo Running tests: $^
ifeq ($(filter nacl%,$(SYSTEM)),)
	$^
else
	"$(NACL_SEL_LDR)" -a -B "$(NACL_IRT)" -- $^
endif
endif

######################################

clean:
	$(RM) -r tmp/$(SYSTEM)
	$(RM) -r lib/$(SYSTEM)
	$(RM) -r bin/$(SYSTEM)

clobber:
	$(RM) -r tmp
	$(RM) -r lib
	$(RM) -r bin

.PHONY: export test all
.PHONY: regal.lib regal.bin
.PHONY: apitrace.lib
.PHONY: zlib.lib libpng.lib snappy.lib glew.lib glu.lib glut.lib
.PHONY: clean clobber
