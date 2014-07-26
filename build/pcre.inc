# pcre.inc
#
# Generic gnumake .inc for building pcre
#

# Sources

PCRE.C :=
#PCRE.C += src/pcre/dftables.c
PCRE.C += src/pcre/pcre16_byte_order.c
PCRE.C += src/pcre/pcre16_chartables.c
PCRE.C += src/pcre/pcre16_compile.c
PCRE.C += src/pcre/pcre16_config.c
PCRE.C += src/pcre/pcre16_dfa_exec.c
PCRE.C += src/pcre/pcre16_exec.c
PCRE.C += src/pcre/pcre16_fullinfo.c
PCRE.C += src/pcre/pcre16_get.c
PCRE.C += src/pcre/pcre16_globals.c
PCRE.C += src/pcre/pcre16_jit_compile.c
PCRE.C += src/pcre/pcre16_maketables.c
PCRE.C += src/pcre/pcre16_newline.c
PCRE.C += src/pcre/pcre16_ord2utf16.c
PCRE.C += src/pcre/pcre16_printint.c
PCRE.C += src/pcre/pcre16_refcount.c
PCRE.C += src/pcre/pcre16_string_utils.c
PCRE.C += src/pcre/pcre16_study.c
PCRE.C += src/pcre/pcre16_tables.c
PCRE.C += src/pcre/pcre16_ucd.c
PCRE.C += src/pcre/pcre16_utf16_utils.c
PCRE.C += src/pcre/pcre16_valid_utf16.c
PCRE.C += src/pcre/pcre16_version.c
PCRE.C += src/pcre/pcre16_xclass.c
PCRE.C += src/pcre/pcre32_byte_order.c
PCRE.C += src/pcre/pcre32_chartables.c
PCRE.C += src/pcre/pcre32_compile.c
PCRE.C += src/pcre/pcre32_config.c
PCRE.C += src/pcre/pcre32_dfa_exec.c
PCRE.C += src/pcre/pcre32_exec.c
PCRE.C += src/pcre/pcre32_fullinfo.c
PCRE.C += src/pcre/pcre32_get.c
PCRE.C += src/pcre/pcre32_globals.c
PCRE.C += src/pcre/pcre32_jit_compile.c
PCRE.C += src/pcre/pcre32_maketables.c
PCRE.C += src/pcre/pcre32_newline.c
PCRE.C += src/pcre/pcre32_ord2utf32.c
PCRE.C += src/pcre/pcre32_printint.c
PCRE.C += src/pcre/pcre32_refcount.c
PCRE.C += src/pcre/pcre32_string_utils.c
PCRE.C += src/pcre/pcre32_study.c
PCRE.C += src/pcre/pcre32_tables.c
PCRE.C += src/pcre/pcre32_ucd.c
PCRE.C += src/pcre/pcre32_utf32_utils.c
PCRE.C += src/pcre/pcre32_valid_utf32.c
PCRE.C += src/pcre/pcre32_version.c
PCRE.C += src/pcre/pcre32_xclass.c
PCRE.C += src/pcre/pcre_byte_order.c
PCRE.C += src/pcre/pcre_chartables.c
PCRE.C += src/pcre/pcre_compile.c
PCRE.C += src/pcre/pcre_config.c
#PCRE.C += src/pcre/pcredemo.c
PCRE.C += src/pcre/pcre_dfa_exec.c
PCRE.C += src/pcre/pcre_exec.c
PCRE.C += src/pcre/pcre_fullinfo.c
PCRE.C += src/pcre/pcre_get.c
PCRE.C += src/pcre/pcre_globals.c
#PCRE.C += src/pcre/pcregrep.c
PCRE.C += src/pcre/pcre_jit_compile.c
#PCRE.C += src/pcre/pcre_jit_test.c
PCRE.C += src/pcre/pcre_maketables.c
PCRE.C += src/pcre/pcre_newline.c
PCRE.C += src/pcre/pcre_ord2utf8.c
PCRE.C += src/pcre/pcreposix.c
PCRE.C += src/pcre/pcre_printint.c
PCRE.C += src/pcre/pcre_refcount.c
PCRE.C += src/pcre/pcre_string_utils.c
PCRE.C += src/pcre/pcre_study.c
PCRE.C += src/pcre/pcre_tables.c
#PCRE.C += src/pcre/pcretest.c
PCRE.C += src/pcre/pcre_ucd.c
PCRE.C += src/pcre/pcre_valid_utf8.c
PCRE.C += src/pcre/pcre_version.c
PCRE.C += src/pcre/pcre_xclass.c

# Internal Headers
# seth: is this used for dependencies?
PCRE.H :=

# Other Headers
# seth: do all of these need to be specified?
PCRE.H.OTHER :=

#
# Includes 

PCRE.INCLUDE :=
PCRE.INCLUDE += -Isrc/pcre

PCRE.STATIC ?= libpcre.a
