#
# Common command-line options
# ===========================
#
# To build in debug mode:
#   - use MODE=debug
#
# To build using ccache (http://ccache.samba.org/)
#   - use CCACHE=ccache
#
# To disable stripping of binaries:
#   - use MODE=debug
#   - use STRIP=
#
# To disable symlinks:
#   - use LN=
#
# To see verbose output
#   - use V=1 on gmake command-line

include build/common.inc

include Makefile.zlib
include Makefile.libpng
include Makefile.snappy
include Makefile.apitrace
include Makefile.glsloptlib
include Makefile.pcrelib
include Makefile.regal
include Makefile.regalw
include Makefile.glu
include Makefile.glut
include Makefile.glew
include Makefile.glewinfo

# Examples

include Makefile.dreamtorus
include Makefile.dreamtorus_static
include Makefile.alphatorus
include Makefile.tiger
include Makefile.nacl

# Testing

include Makefile.gtest
include Makefile.regaltest

# Misc

# include Makefile.expat

all::

clean::

clobber:
	$(RM) -r tmp
	$(RM) -r lib
	$(RM) -r bin

# Disable the built-in yacc & lex rules

%.c: %.y
%.c: %.l

.PHONY: all clean clobber
