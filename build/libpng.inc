# libpng.inc
#
# Generic gnumake .inc for building libpng
#

# Sources

LIBPNG.C :=
LIBPNG.C += src/libpng/src/png.c
LIBPNG.C += src/libpng/src/pngerror.c
LIBPNG.C += src/libpng/src/pngget.c
LIBPNG.C += src/libpng/src/pngmem.c
LIBPNG.C += src/libpng/src/pngpread.c
LIBPNG.C += src/libpng/src/pngread.c
LIBPNG.C += src/libpng/src/pngrio.c
LIBPNG.C += src/libpng/src/pngrtran.c
LIBPNG.C += src/libpng/src/pngrutil.c
LIBPNG.C += src/libpng/src/pngset.c
LIBPNG.C += src/libpng/src/pngtrans.c
LIBPNG.C += src/libpng/src/pngwio.c
LIBPNG.C += src/libpng/src/pngwrite.c
LIBPNG.C += src/libpng/src/pngwtran.c
LIBPNG.C += src/libpng/src/pngwutil.c

#

LIBPNG.STATIC     := libpnglib.a

LIBPNG.CFLAGS     := -Isrc/zlib/include -Isrc/libpng/include

