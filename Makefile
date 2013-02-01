include config/version

SHELL = /bin/sh
SYSTEM ?= $(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')
SYSTEM.SUPPORTED = $(shell test -f config/Makefile.$(SYSTEM) && echo 1)

ifeq ($(SYSTEM.SUPPORTED), 1)
include config/Makefile.$(SYSTEM)
else
$(error "Platform '$(SYSTEM)' not supported")
endif

REGAL_DEST ?= /usr
BINDIR     ?= $(REGAL_DEST)/bin
LIBDIR     ?= $(REGAL_DEST)/lib

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

AR      ?= ar
INSTALL ?= install
STRIP   ?= strip
RM      ?= rm -f
LN      ?= ln -sf

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

INCLUDE = -Iinclude

override CFLAGS := $(OPT) $(CFLAGS) $(WARN) $(INCLUDE) $(CFLAGS.EXTRA)

all: regal.lib glew.lib glu.lib glut.lib regal.bin bin/regaltest

# REGAL shared and static libraries

export:
	scripts/Export.py --api gl 4.2 --api wgl 4.0 --api glx 4.0 --api cgl 1.4 --api egl 1.0 --outdir .

#
# zlib
#

ZLIB.SRCS += src/zlib/src/adler32.c
ZLIB.SRCS += src/zlib/src/crc32.c
ZLIB.SRCS += src/zlib/src/compress.c
ZLIB.SRCS += src/zlib/src/deflate.c
ZLIB.SRCS += src/zlib/src/infback.c
ZLIB.SRCS += src/zlib/src/inffast.c
ZLIB.SRCS += src/zlib/src/inflate.c
ZLIB.SRCS += src/zlib/src/inftrees.c
ZLIB.SRCS += src/zlib/src/trees.c
ZLIB.SRCS += src/zlib/src/uncompr.c
ZLIB.SRCS += src/zlib/src/zutil.c
ZLIB.SRCS += src/zlib/src/gzlib.c
ZLIB.SRCS += src/zlib/src/gzread.c
ZLIB.SRCS += src/zlib/src/gzwrite.c
ZLIB.SRCS += src/zlib/src/gzclose.c

ZLIB.SRCS.NAMES := $(notdir $(ZLIB.SRCS))
ZLIB.OBJS       := $(addprefix tmp/$(SYSTEM)/zlib/static/,$(ZLIB.SRCS.NAMES))
ZLIB.OBJS       := $(ZLIB.OBJS:.c=.o)
ZLIB.CFLAGS     := -Isrc/zlib/include
ZLIB.STATIC     := libz.a

ifneq ($(filter linux%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter darwin%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
ZLIB.CFLAGS     += -DHAVE_UNISTD_H
endif

zlib.lib: lib/$(ZLIB.STATIC)

tmp/$(SYSTEM)/zlib/static/%.o: src/zlib/src/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(ZLIB.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(ZLIB.STATIC): $(ZLIB.OBJS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(AR) cr $@ $(ZLIB.OBJS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

#
# libpng
#

LIBPNG.SRCS += src/libpng/src/png.c
LIBPNG.SRCS += src/libpng/src/pngerror.c
LIBPNG.SRCS += src/libpng/src/pngget.c
LIBPNG.SRCS += src/libpng/src/pngmem.c
LIBPNG.SRCS += src/libpng/src/pngpread.c
LIBPNG.SRCS += src/libpng/src/pngread.c
LIBPNG.SRCS += src/libpng/src/pngrio.c
LIBPNG.SRCS += src/libpng/src/pngrtran.c
LIBPNG.SRCS += src/libpng/src/pngrutil.c
LIBPNG.SRCS += src/libpng/src/pngset.c
LIBPNG.SRCS += src/libpng/src/pngtrans.c
LIBPNG.SRCS += src/libpng/src/pngwio.c
LIBPNG.SRCS += src/libpng/src/pngwrite.c
LIBPNG.SRCS += src/libpng/src/pngwtran.c
LIBPNG.SRCS += src/libpng/src/pngwutil.c

LIBPNG.SRCS.NAMES := $(notdir $(LIBPNG.SRCS))
LIBPNG.OBJS       := $(addprefix tmp/$(SYSTEM)/libpng/static/,$(LIBPNG.SRCS.NAMES))
LIBPNG.OBJS       := $(LIBPNG.OBJS:.c=.o)
LIBPNG.CFLAGS     := -Isrc/zlib/include -Isrc/libpng/include
LIBPNG.STATIC     := libpng.a

ifneq ($(filter linux%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter darwin%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

ifneq ($(filter nacl%,$(SYSTEM)),)
LIBPNG.CFLAGS     += -DHAVE_UNISTD_H
endif

libpng.lib: zlib.lib lib/$(LIBPNG.STATIC)

tmp/$(SYSTEM)/libpng/static/%.o: src/libpng/src/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIBPNG.CFLAGS) $(CFLAGS) $(PICFLAG) -o $@ -c $<

lib/$(LIBPNG.STATIC): $(LIBPNG.OBJS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(AR) cr $@ $(LIBPNG.OBJS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

#

LIB.LDFLAGS        := -lstdc++ -lpthread -ldl -lm
LIB.LIBS           := 

LIB.SRCS           :=
LIB.SRCS           += src/regal/RegalPixelConversions.cpp
LIB.SRCS           += src/regal/RegalTexC.cpp
LIB.SRCS           += src/regal/RegalPpca.cpp
LIB.SRCS           += src/regal/RegalIff.cpp
LIB.SRCS           += src/regal/Regal.cpp
LIB.SRCS           += src/regal/RegalToken.cpp
LIB.SRCS           += src/regal/RegalLog.cpp
LIB.SRCS           += src/regal/RegalInit.cpp
LIB.SRCS           += src/regal/RegalUtil.cpp
LIB.SRCS           += src/regal/RegalConfig.cpp
LIB.SRCS           += src/regal/RegalLookup.cpp
LIB.SRCS           += src/regal/RegalFrame.cpp
LIB.SRCS           += src/regal/RegalHelper.cpp
LIB.SRCS           += src/regal/RegalMarker.cpp
LIB.SRCS           += src/regal/RegalContext.cpp
LIB.SRCS           += src/regal/RegalContextInfo.cpp
LIB.SRCS           += src/regal/RegalDispatchGlobal.cpp
LIB.SRCS           += src/regal/RegalDispatcher.cpp
LIB.SRCS           += src/regal/RegalDispatchEmu.cpp
LIB.SRCS           += src/regal/RegalDispatchLog.cpp
LIB.SRCS           += src/regal/RegalDispatchCode.cpp
LIB.SRCS           += src/regal/RegalDispatchDebug.cpp
LIB.SRCS           += src/regal/RegalDispatchError.cpp
LIB.SRCS           += src/regal/RegalDispatchCache.cpp
LIB.SRCS           += src/regal/RegalDispatchLoader.cpp
LIB.SRCS           += src/regal/RegalDispatchPpapi.cpp
LIB.SRCS           += src/regal/RegalDispatchStaticES2.cpp
LIB.SRCS           += src/regal/RegalDispatchStaticEGL.cpp
LIB.SRCS           += src/regal/RegalDispatchMissing.cpp
LIB.SRCS           += src/regal/RegalShaderCache.cpp
LIB.SRCS           += src/regal/RegalHttp.cpp
LIB.SRCS           += src/regal/RegalFavicon.cpp
LIB.SRCS           += src/regal/RegalMac.cpp
LIB.SRCS           += src/regal/RegalSo.cpp

# Disable mongoose and Regal HTTP for NaCL build

ifeq ($(filter nacl%,$(SYSTEM)),)
LIB.SRCS           += src/mongoose/mongoose.c
endif

ifeq ($(filter -DREGAL_NO_MD5%,$(CFLAGS)),)
LIB.SRCS           += src/md5/src/md5.c
endif

LIB.SRCS           += src/jsonsl/jsonsl.c
LIB.CFLAGS         += -DJSONSL_STATE_GENERIC=1

LIB.INCLUDE        += -Isrc/zlib/include -Isrc/libpng/include
LIB.INCLUDE        += -Isrc/mongoose
LIB.INCLUDE        += -Isrc/md5/include
LIB.INCLUDE        += -Isrc/lookup3

#
# Add debug-specific flags
#

ifeq ($(MODE),debug)
endif

#
# In release mode we're agressive about leaving out functionality
# in order to minimize the footprint of libRegal.so.1
#

ifeq ($(MODE),release)
LIB.CFLAGS         += -DNDEBUG
LIB.CFLAGS         += -DREGAL_DECL_EXPORT=1
LIB.CFLAGS         += -DREGAL_LOG=0
LIB.CFLAGS         += -DREGAL_LOG_ALL=0
LIB.CFLAGS         += -DREGAL_LOG_ONCE=0
LIB.CFLAGS         += -DREGAL_LOG_JSON=0
LIB.CFLAGS         += -DREGAL_NO_HTTP=1
LIB.CFLAGS         += -DREGAL_NO_ASSERT=1
LIB.CFLAGS         += -DREGAL_NO_PNG=1
LIB.CFLAGS         += -fomit-frame-pointer
LIB.CFLAGS         += -DREGAL_ERROR=0
LIB.CFLAGS         += -DREGAL_DEBUG=0
LIB.CFLAGS         += -DREGAL_CACHE=0
LIB.CFLAGS         += -DREGAL_EMULATION=1    # 0 for "loader only"
LIB.CFLAGS         += -DREGAL_NO_TLS=0       # 1 for single threaded
endif

#
# Flags for custom mode
#

ifeq ($(MODE),custom)
endif

LIB.INCLUDE        += -Isrc/boost

LIB.SRCS.NAMES     := $(notdir $(LIB.SRCS))

LIB.DEPS           :=
LIB.DEPS           += include/GL/Regal.h
LIB.DEPS           += src/regal/RegalPrivate.h

LIB.OBJS           := $(addprefix tmp/$(SYSTEM)/regal/static/,$(LIB.SRCS.NAMES))
LIB.OBJS           := $(LIB.OBJS:.c=.o)
LIB.OBJS           := $(LIB.OBJS:.cpp=.o)
LIB.SOBJS          := $(addprefix tmp/$(SYSTEM)/regal/shared/,$(LIB.SRCS.NAMES))
LIB.SOBJS          := $(LIB.SOBJS:.c=.o)
LIB.SOBJS          := $(LIB.SOBJS:.cpp=.o)

LIB.LIBS           += -Llib -lpng -lz

ifneq ($(filter darwin%,$(SYSTEM)),)
LIB.LDFLAGS        += -Wl,-reexport-lGLU -L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
endif

regal.lib: zlib.lib libpng.lib lib/$(LIB.SHARED) lib/$(LIB.STATIC)

lib/$(LIB.STATIC): $(LIB.OBJS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(AR) cr $@ $^
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

lib/$(LIB.SHARED): lib/$(LIBPNG.STATIC) lib/$(ZLIB.STATIC) $(LIB.SOBJS)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.SO) -o $@ $(LIB.SOBJS) $(LIB.LDFLAGS) $(LIB.LIBS)
ifneq ($(LN),)
	$(LN) $(LIB.SHARED) lib/$(LIB.SONAME)
	$(LN) $(LIB.SHARED) lib/$(LIB.DEVLNK)
ifneq ($(LIB.FRAMEWORK),)
	$(LN) $(LIB.SHARED) lib/$(LIB.FRAMEWORK)
endif
endif
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

tmp/$(SYSTEM)/regal/static/%.o: src/regal/%.cpp $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/regal/%.cpp $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/mongoose/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/mongoose/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/md5/src/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/md5/src/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/static/%.o: src/jsonsl/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

tmp/$(SYSTEM)/regal/shared/%.o: src/jsonsl/%.c $(LIB.DEPS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) $(LIB.INCLUDE) -o $@ -c $<

#
# RegalGLEW
#

GLEW.SRCS        += src/glew/src/glew.c
GLEW.SRCS.NAMES := $(notdir $(GLEW.SRCS))
GLEW.OBJS       := $(addprefix tmp/$(SYSTEM)/glew/shared/,$(GLEW.SRCS.NAMES))
GLEW.OBJS       := $(GLEW.OBJS:.c=.o)
GLEW.CFLAGS     := -Isrc/glew/include -Isrc/glu/include -DGLEW_EXPORTS -DGLEW_BUILD -DGLEW_REGAL
GLEW.LIBS       := -Llib -lRegal
GLEW.SHARED     := libRegalGLEW.$(EXT.DYNAMIC)
GLEW.STATIC     := libRegalGLEW.a

glew.lib: lib/$(GLEW.SHARED)

tmp/$(SYSTEM)/glew/shared/%.o: src/glew/src/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLEW.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(GLEW.SHARED): $(GLEW.OBJS) lib/$(LIB.SHARED)
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) -o $@ $(GLEW.OBJS) $(LIB.LDFLAGS) $(GLEW.LIBS)  -lpthread
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

#
# RegalGLU
#

ifneq ($(filter darwin%,$(SYSTEM)),)
glu.lib:

else

GLU.SRCS        += src/glu/libtess/dict.c
GLU.SRCS        += src/glu/libtess/geom.c
GLU.SRCS        += src/glu/libtess/memalloc.c
GLU.SRCS        += src/glu/libtess/mesh.c
GLU.SRCS        += src/glu/libtess/normal.c
#GLU.SRCS        += src/glu/libtess/priorityq-heap.c
GLU.SRCS        += src/glu/libtess/priorityq.c
GLU.SRCS        += src/glu/libtess/render.c
GLU.SRCS        += src/glu/libtess/sweep.c
GLU.SRCS        += src/glu/libtess/tess.c
GLU.SRCS        += src/glu/libtess/tessmono.c
GLU.SRCS        += src/glu/libutil/error.c
GLU.SRCS        += src/glu/libutil/glue.c
GLU.SRCS        += src/glu/libutil/mipmap.c
GLU.SRCS        += src/glu/libutil/project.c
GLU.SRCS        += src/glu/libutil/quad.c
GLU.SRCS        += src/glu/libutil/registry.c

GLU.SRCS        += src/glu/libnurbs/interface/bezierEval.cc
GLU.SRCS        += src/glu/libnurbs/interface/bezierPatch.cc
GLU.SRCS        += src/glu/libnurbs/interface/bezierPatchMesh.cc
GLU.SRCS        += src/glu/libnurbs/interface/glcurveval.cc
GLU.SRCS        += src/glu/libnurbs/interface/glinterface.cc
GLU.SRCS        += src/glu/libnurbs/interface/glrenderer.cc
GLU.SRCS        += src/glu/libnurbs/interface/glsurfeval.cc
GLU.SRCS        += src/glu/libnurbs/interface/incurveeval.cc
GLU.SRCS        += src/glu/libnurbs/interface/insurfeval.cc
GLU.SRCS        += src/glu/libnurbs/internals/arc.cc
GLU.SRCS        += src/glu/libnurbs/internals/arcsorter.cc
GLU.SRCS        += src/glu/libnurbs/internals/arctess.cc
GLU.SRCS        += src/glu/libnurbs/internals/backend.cc
GLU.SRCS        += src/glu/libnurbs/internals/basiccrveval.cc
GLU.SRCS        += src/glu/libnurbs/internals/basicsurfeval.cc
GLU.SRCS        += src/glu/libnurbs/internals/bin.cc
GLU.SRCS        += src/glu/libnurbs/internals/bufpool.cc
GLU.SRCS        += src/glu/libnurbs/internals/cachingeval.cc
GLU.SRCS        += src/glu/libnurbs/internals/ccw.cc
GLU.SRCS        += src/glu/libnurbs/internals/coveandtiler.cc
GLU.SRCS        += src/glu/libnurbs/internals/curve.cc
GLU.SRCS        += src/glu/libnurbs/internals/curvelist.cc
GLU.SRCS        += src/glu/libnurbs/internals/curvesub.cc
GLU.SRCS        += src/glu/libnurbs/internals/dataTransform.cc
GLU.SRCS        += src/glu/libnurbs/internals/displaylist.cc
GLU.SRCS        += src/glu/libnurbs/internals/flist.cc
GLU.SRCS        += src/glu/libnurbs/internals/flistsorter.cc
GLU.SRCS        += src/glu/libnurbs/internals/hull.cc
GLU.SRCS        += src/glu/libnurbs/internals/intersect.cc
GLU.SRCS        += src/glu/libnurbs/internals/knotvector.cc
GLU.SRCS        += src/glu/libnurbs/internals/mapdesc.cc
GLU.SRCS        += src/glu/libnurbs/internals/mapdescv.cc
GLU.SRCS        += src/glu/libnurbs/internals/maplist.cc
GLU.SRCS        += src/glu/libnurbs/internals/mesher.cc
GLU.SRCS        += src/glu/libnurbs/internals/monotonizer.cc
GLU.SRCS        += src/glu/libnurbs/internals/monoTriangulationBackend.cc
GLU.SRCS        += src/glu/libnurbs/internals/mycode.cc
GLU.SRCS        += src/glu/libnurbs/internals/nurbsinterfac.cc
GLU.SRCS        += src/glu/libnurbs/internals/nurbstess.cc
GLU.SRCS        += src/glu/libnurbs/internals/patch.cc
GLU.SRCS        += src/glu/libnurbs/internals/patchlist.cc
GLU.SRCS        += src/glu/libnurbs/internals/quilt.cc
GLU.SRCS        += src/glu/libnurbs/internals/reader.cc
GLU.SRCS        += src/glu/libnurbs/internals/renderhints.cc
GLU.SRCS        += src/glu/libnurbs/internals/slicer.cc
GLU.SRCS        += src/glu/libnurbs/internals/sorter.cc
GLU.SRCS        += src/glu/libnurbs/internals/splitarcs.cc
GLU.SRCS        += src/glu/libnurbs/internals/subdivider.cc
GLU.SRCS        += src/glu/libnurbs/internals/tobezier.cc
GLU.SRCS        += src/glu/libnurbs/internals/trimline.cc
GLU.SRCS        += src/glu/libnurbs/internals/trimregion.cc
GLU.SRCS        += src/glu/libnurbs/internals/trimvertpool.cc
GLU.SRCS        += src/glu/libnurbs/internals/uarray.cc
GLU.SRCS        += src/glu/libnurbs/internals/varray.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/directedLine.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/gridWrap.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/monoChain.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/monoPolyPart.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/monoTriangulation.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/partitionX.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/partitionY.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/polyDBG.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/polyUtil.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/primitiveStream.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/quicksort.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/rectBlock.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampleComp.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampleCompBot.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampleCompRight.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampleCompTop.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampledLine.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/sampleMonoPoly.cc
GLU.SRCS        += src/glu/libnurbs/nurbtess/searchTree.cc

GLU.SRCS.NAMES := $(notdir $(GLU.SRCS))
GLU.OBJS       := $(addprefix tmp/$(SYSTEM)/glu/shared/,$(GLU.SRCS.NAMES))
GLU.OBJS       := $(GLU.OBJS:.c=.o) $(GLU.OBJS:.cc=.o)
GLU.OBJS       := $(filter %.o,$(GLU.OBJS)) 
GLU.CFLAGS     := -Isrc/glu/include -Isrc/glu/libnurbs/interface -Isrc/glu/libnurbs/internals -Isrc/glu/libnurbs/nurbtess
GLU.CFLAGS     += -DLIBRARYBUILD
GLU.LIBS       := -Llib -lRegal
GLU.LIBS       += -lpthread -lm
GLU.SHARED     := libRegalGLU.$(EXT.DYNAMIC)
GLU.STATIC     := libRegalGLU.a

glu.lib: lib/$(GLU.SHARED)

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libtess/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libutil/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/interface/%.cc
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/internals/%.cc
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/glu/shared/%.o: src/glu/libnurbs/nurbtess/%.cc
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(PICFLAG) $(GLU.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(GLU.SHARED): $(GLU.OBJS) lib/$(LIB.SHARED) 
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) $(GLU.LIBS) $(LIB.LDFLAGS) -o $@ $(GLU.OBJS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

endif

#
# RegalGLUT
#

# Don't build GLUT for OS X or NaCL

ifneq ($(filter darwin%,$(SYSTEM)),)
glut.lib:
else
ifneq ($(filter nacl%,$(SYSTEM)),)
glut.lib:
else

# NOT for windows...

GLUT.SRCS        += src/glut/src/capturexfont.c
GLUT.SRCS        += src/glut/src/glut_glxext.c
GLUT.SRCS        += src/glut/src/glut_menu.c
GLUT.SRCS        += src/glut/src/glut_menu2.c
GLUT.SRCS        += src/glut/src/layerutil.c

# Windows and Linux...

GLUT.SRCS        += src/glut/src/glut_8x13.c
GLUT.SRCS        += src/glut/src/glut_9x15.c
GLUT.SRCS        += src/glut/src/glut_bitmap.c
GLUT.SRCS        += src/glut/src/glut_bwidth.c
GLUT.SRCS        += src/glut/src/glut_cindex.c
GLUT.SRCS        += src/glut/src/glut_cmap.c
GLUT.SRCS        += src/glut/src/glut_cursor.c
GLUT.SRCS        += src/glut/src/glut_dials.c
GLUT.SRCS        += src/glut/src/glut_dstr.c
GLUT.SRCS        += src/glut/src/glut_event.c
GLUT.SRCS        += src/glut/src/glut_ext.c
GLUT.SRCS        += src/glut/src/glut_fcb.c
GLUT.SRCS        += src/glut/src/glut_fullscrn.c
GLUT.SRCS        += src/glut/src/glut_gamemode.c
GLUT.SRCS        += src/glut/src/glut_get.c
GLUT.SRCS        += src/glut/src/glut_hel10.c
GLUT.SRCS        += src/glut/src/glut_hel12.c
GLUT.SRCS        += src/glut/src/glut_hel18.c
GLUT.SRCS        += src/glut/src/glut_init.c
GLUT.SRCS        += src/glut/src/glut_input.c
GLUT.SRCS        += src/glut/src/glut_joy.c
GLUT.SRCS        += src/glut/src/glut_key.c
GLUT.SRCS        += src/glut/src/glut_keyctrl.c
GLUT.SRCS        += src/glut/src/glut_keyup.c
GLUT.SRCS        += src/glut/src/glut_mesa.c
GLUT.SRCS        += src/glut/src/glut_modifier.c
GLUT.SRCS        += src/glut/src/glut_mroman.c
GLUT.SRCS        += src/glut/src/glut_overlay.c
GLUT.SRCS        += src/glut/src/glut_roman.c
GLUT.SRCS        += src/glut/src/glut_shapes.c
GLUT.SRCS        += src/glut/src/glut_space.c
GLUT.SRCS        += src/glut/src/glut_stroke.c
GLUT.SRCS        += src/glut/src/glut_swap.c
GLUT.SRCS        += src/glut/src/glut_swidth.c
GLUT.SRCS        += src/glut/src/glut_tablet.c
GLUT.SRCS        += src/glut/src/glut_teapot.c
GLUT.SRCS        += src/glut/src/glut_tr10.c
GLUT.SRCS        += src/glut/src/glut_tr24.c
GLUT.SRCS        += src/glut/src/glut_util.c
GLUT.SRCS        += src/glut/src/glut_vidresize.c
GLUT.SRCS        += src/glut/src/glut_warp.c
GLUT.SRCS        += src/glut/src/glut_win.c
GLUT.SRCS        += src/glut/src/glut_winmisc.c
GLUT.SRCS        += src/glut/src/glut_ppm.c
GLUT.SRCS.NAMES := $(notdir $(GLUT.SRCS))
GLUT.OBJS       := $(addprefix tmp/$(SYSTEM)/glut/shared/,$(GLUT.SRCS.NAMES))
GLUT.OBJS       := $(GLUT.OBJS:.c=.o)
GLUT.CFLAGS     := -Isrc/glut/include -Isrc/glu/include -DBUILD_GLUT32 -DGLUT_BUILDING_LIB -DGLUT_STATIC
#GLUT.CFLAGS     += -DCDECL=
GLUT.LIBS       := -Llib -lRegal -lRegalGLU
GLUT.LIBS       += -lX11 -lXmu -lXi 
GLUT.LIBS       += -lpthread -lm
GLUT.SHARED     := libRegalGLUT.$(EXT.DYNAMIC)
GLUT.STATIC     := libRegalGLUT.a

glut.lib: lib/$(GLUT.SHARED)

tmp/$(SYSTEM)/glut/shared/%.o: src/glut/src/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(GLUT.CFLAGS) $(CFLAGS) $(PICFLAG) $(CFLAGS.SO) -o $@ -c $<

lib/$(GLUT.SHARED): $(GLUT.OBJS) lib/$(GLU.SHARED) lib/$(LIB.SHARED)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) $(LDFLAGS.DYNAMIC) -o $@ $(GLUT.OBJS) $(GLUT.LIBS) 
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

endif
endif

#
# RegalGLEW glewinfo
#

GLEWINFO.SRCS       += src/glew/src/glewinfo.c 
GLEWINFO.SRCS.NAMES := $(notdir $(GLEWINFO.SRCS))
GLEWINFO.OBJS       := $(addprefix tmp/$(SYSTEM)/glewinfo/static/,$(GLEWINFO.SRCS.NAMES))
GLEWINFO.OBJS       := $(GLEWINFO.OBJS:.c=.o)
GLEWINFO.CFLAGS     := -Iinclude -Isrc/glew/include -DGLEW_REGAL -DGLEW_NO_GLU
GLEWINFO.LIBS       += -Llib -lRegal -lRegalGLEW $(LDFLAGS.GLUT) $(LDFLAGS.GLU) $(LDFLAGS.AGL)

ifneq ($(filter linux%,$(SYSTEM)),)
GLEWINFO.LIBS       += -lX11
endif

tmp/$(SYSTEM)/glewinfo/static/%.o: src/glew/src/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(GLEWINFO.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/glewinfo: $(GLEWINFO.OBJS) lib/$(LIB.SHARED) lib/$(GLEW.SHARED) lib/$(GLUT.SHARED) lib/$(GLU.SHARED)
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(GLEWINFO.OBJS) $(LIB.LDFLAGS) $(GLEWINFO.LIBS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

# Examples

ifneq ($(filter nacl%,$(SYSTEM)),)
regal.bin: bin/nacl$(BIN_EXTENSION) examples/nacl/nacl.nmf
else
regal.bin: bin/glewinfo bin/dreamtorus bin/tiger
endif

#
# dreamtorus
#

DREAMTORUS.SRCS       += examples/dreamtorus/src/render.cpp
DREAMTORUS.SRCS       += examples/dreamtorus/glut/code/main.cpp
DREAMTORUS.SRCS.NAMES := $(notdir $(DREAMTORUS.SRCS))
DREAMTORUS.OBJS       := $(addprefix tmp/$(SYSTEM)/dreamtorus/static/,$(DREAMTORUS.SRCS.NAMES))
DREAMTORUS.OBJS       := $(DREAMTORUS.OBJS:.cpp=.o)
DREAMTORUS.CFLAGS     := -Iinclude -Iexamples/dreamtorus/src
DREAMTORUS.LIBS       += -Llib $(LDFLAGS.GLUT) $(LDFLAGS.GLU) -lRegal 
DREAMTORUS.LIBS       += -lm -lpthread

tmp/$(SYSTEM)/dreamtorus/static/%.o: examples/dreamtorus/src/%.cpp
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(DREAMTORUS.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/dreamtorus/static/%.o: examples/dreamtorus/glut/code/%.cpp
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(DREAMTORUS.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/dreamtorus: $(DREAMTORUS.OBJS) lib/$(LIB.SHARED) lib/$(GLUT.SHARED) lib/$(GLU.SHARED)
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(DREAMTORUS.OBJS) $(LIB.LDFLAGS) $(DREAMTORUS.LIBS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

NACL.SRCS       += examples/nacl/main.cpp
NACL.SRCS.NAMES := $(notdir $(NACL.SRCS))
NACL.OBJS       := $(addprefix tmp/$(SYSTEM)/nacl/static/,$(NACL.SRCS.NAMES))
NACL.OBJS       := $(NACL.OBJS:.cpp=.o)
NACL.CFLAGS     := -Iinclude
NACL.LIBS       += -L./lib -Wl,-Bstatic -lRegal -Wl,-Bdynamic
NACL.LIBS       += -lpng -lz -lm -lpthread -lppapi -lppapi_gles2 -lstdc++

tmp/$(SYSTEM)/nacl/static/%.o: examples/nacl/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(NACL.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/nacl$(BIN_EXTENSION): lib/$(LIB.STATIC) $(NACL.OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(NACL.OBJS) $(NACL.LIBS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

TOOLCHAIN=$(NACL_SDK_ROOT)/toolchain/linux_x86_glibc

# Uncomment this to enable automatic regeneration of the nacl nmf file
#examples/nacl/nacl.nmf: bin/nacl$(BIN_EXTENSION)
	#$(NACL_SDK_ROOT)/tools/create_nmf.py -o $@ -D $(TOOLCHAIN)/x86_64-nacl/bin/objdump -t glibc bin/nacl* -L $(TOOLCHAIN)/x86_64-nacl/lib32 -L $(TOOLCHAIN)/x86_64-nacl/lib -s examples/nacl

# GLUT and GLU dependency for non-Mac, non-Nacl builds

ifeq ($(filter darwin%,$(SYSTEM)),)
ifeq ($(filter nacl%,$(SYSTEM)),)
bin/dreamtorus: lib/$(GLUT.SHARED) lib/$(GLU.SHARED)
endif
endif

#
# tiger
#

TIGER.SRCS       += examples/tiger/nvpr_tiger.c
TIGER.SRCS       += examples/tiger/tiger.c
TIGER.SRCS       += examples/tiger/xform.c
TIGER.SRCS.NAMES := $(notdir $(TIGER.SRCS))
TIGER.OBJS       := $(addprefix tmp/$(SYSTEM)/tiger/static/,$(TIGER.SRCS.NAMES))
TIGER.OBJS       := $(TIGER.OBJS:.c=.o)
TIGER.CFLAGS     := -Iinclude -DGLEW_NO_GLU
TIGER.LIBS       += -Llib -lRegalGLEW $(LDFLAGS.GLUT) $(LDFLAGS.GLU) -lRegal
TIGER.LIBS       += -lm -lpthread

tmp/$(SYSTEM)/tiger/static/%.o: examples/tiger/%.c
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(TIGER.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/tiger: $(TIGER.OBJS) lib/$(GLEW.SHARED) lib/$(LIB.SHARED) lib/$(GLEW.SHARED) lib/$(GLUT.SHARED) lib/$(GLU.SHARED)
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(TIGER.OBJS) $(TIGER.LIBS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

# GLUT and GLU dependency for non-Mac, non-Nacl builds

ifeq ($(filter darwin%,$(SYSTEM)),)
ifeq ($(filter nacl%,$(SYSTEM)),)
bin/tiger:      lib/$(GLUT.SHARED) lib/$(GLU.SHARED)
endif
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
GTEST.CFLAGS     := -Isrc/googletest/include -Isrc/googletest -Isrc/googlemock/include -Isrc/googlemock
GTEST.STATIC     := libgtest.a

tmp/$(SYSTEM)/gtest/static/gtest-all.o: src/googletest/src/gtest-all.cc
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(GTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

tmp/$(SYSTEM)/gtest/static/gmock-all.o: src/googlemock/src/gmock-all.cc
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(GTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

lib/$(GTEST.STATIC): $(GTEST.OBJS)
	@mkdir -p $(dir $@)
	$(CCACHE) $(AR) cr $@ $(GTEST.OBJS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

#
# regaltest
#

REGALTEST.SRCS       += tests/test_main.cpp
REGALTEST.SRCS       += tests/RegalDispatchGMock.cpp
REGALTEST.SRCS       += tests/testRegalPpa.cpp
REGALTEST.SRCS       += tests/testRegalPpca.cpp
REGALTEST.SRCS       += tests/testRegalState.cpp
REGALTEST.SRCS       += tests/testRegalTexC.cpp
REGALTEST.SRCS       += tests/testRegalPixelConversions.cpp
REGALTEST.SRCS       += tests/testStringList.cpp
REGALTEST.SRCS.NAMES := $(notdir $(REGALTEST.SRCS))
REGALTEST.OBJS       := $(addprefix tmp/$(SYSTEM)/regal_tests/static/,$(REGALTEST.SRCS.NAMES))
REGALTEST.OBJS       := $(REGALTEST.OBJS:.cpp=.o)
REGALTEST.CFLAGS     := -Isrc/googletest/include -Isrc/googlemock/include -Isrc/regal -Isrc/boost
REGALTEST.LIBS       := -Llib -lgtest lib/$(LIB.STATIC) -lm -ldl

tmp/$(SYSTEM)/regal_tests/static/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CCACHE) $(CC) $(LIB.CFLAGS) $(REGALTEST.CFLAGS) $(CFLAGS) $(CFLAGS.SO) -o $@ -c $<

bin/regaltest: $(REGALTEST.OBJS) lib/$(GTEST.STATIC) lib/$(LIB.STATIC) lib/$(LIBPNG.STATIC) lib/$(ZLIB.STATIC)
	@mkdir -p $(dir $@)
	$(CCACHE) $(LD) $(LDFLAGS.EXTRA) -o $@ $(REGALTEST.OBJS) $(LIB.LDFLAGS) $(REGALTEST.LIBS) $(LIB.LIBS)
ifneq ($(STRIP),)
	$(STRIP) -x $@
endif

test: bin/regaltest
	bin/regaltest

######################################

clean:
	$(RM) -r tmp/
	$(RM) -r lib/
	$(RM) -r bin/
	$(RM) glew.pc glewmx.pc


.PHONY: export test
.PHONY: regal.lib regal.bin all
.PHONY: clean distclean tardist dist-win32 dist-src
