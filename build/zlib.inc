# zlib.inc
#
# Generic gnumake .inc for building zlib
#

# Sources

ZLIB.C :=
ZLIB.C += src/zlib/src/adler32.c
ZLIB.C += src/zlib/src/crc32.c
ZLIB.C += src/zlib/src/compress.c
ZLIB.C += src/zlib/src/deflate.c
ZLIB.C += src/zlib/src/infback.c
ZLIB.C += src/zlib/src/inffast.c
ZLIB.C += src/zlib/src/inflate.c
ZLIB.C += src/zlib/src/inftrees.c
ZLIB.C += src/zlib/src/trees.c
ZLIB.C += src/zlib/src/uncompr.c
ZLIB.C += src/zlib/src/zutil.c
ZLIB.C += src/zlib/src/gzlib.c
ZLIB.C += src/zlib/src/gzread.c
ZLIB.C += src/zlib/src/gzwrite.c
ZLIB.C += src/zlib/src/gzclose.c

#

ZLIB.CFLAGS     := -Isrc/zlib/include

ZLIB.STATIC     := libzlib.a
