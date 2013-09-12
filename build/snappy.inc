# snappy.inc
#
# Generic gnumake .inc for building snappy support into Regal
# https://code.google.com/p/snappy/
#

# snappy sources

SNAPPY.CXX :=
SNAPPY.CXX += src/snappy/snappy-c.cc
SNAPPY.CXX += src/snappy/snappy-sinksource.cc
SNAPPY.CXX += src/snappy/snappy.cc

SNAPPY.H :=
SNAPPY.H += src/snappy/snappy-c.h
SNAPPY.H += src/snappy/snappy-internal.h
SNAPPY.H += src/snappy/snappy-sinksource.h
SNAPPY.H += src/snappy/snappy-stubs-internal.h
SNAPPY.H += src/snappy/snappy-stubs-public.h
SNAPPY.H += src/snappy/snappy.h

SNAPPY.H.OTHER :=

#

SNAPPY.CFLAGS     := -Isrc/snappy
SNAPPY.STATIC     := libsnappy.a
