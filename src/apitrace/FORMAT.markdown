# Trace binary format specification #

This document specifies the binary format of trace streams.

Trace streams are not written verbatim to file, but compressed, nowadays with
snappy (see `common/trace_file_snappy.cpp` for details).  Previously they used
to be compressed with gzip.


## Versions ##

We keep backwards compatability reading old traces, i.e., it should always be
possible to parse and retrace old trace files.

The trace version number refers not only to changes in the binary format
representation, but also semantic changes in the way certain functions should
be retraced.

| `version_no` | Changes |
| ------------ | ------- |
| 0 | initial implementation |
| 1 | support for OpenGL user arrays as blobs (whereas before calls that operated with user memory instead of VBOs should be ignored) |
| 2 | malloc/free memory calls |
| 3 | enums signatures with the whole set of name/value pairs |
| 4 | call enter events include thread no |
| 5 | support for call backtraces |

Writing/editing old traces is not supported however.  An older version of
apitrace should be used in such circunstances.


## Basic types ##

| Type | Description |
| ---- | ----------- |
| `byte` | Raw byte. |
| `uint` | Variable-length unsigned integer, where the most significative bit is zero for last byte or non-zero if more bytes follow; the 7 least significant bits of each byte are used for the integer's bits, in little-endian order. |
| `float` | 32 bits single precision floating point number |
| `double` | 64 bits single precision floating point number |

Strings are length-prefixed.  The trailing zero is implied, not appearing neither in the length prefix nor in the raw bytes.

    string = count byte*

    count = uint


## Grammar ##

The trace consists of a small header with version information, followed by an
arbitrary number of events.

    trace = version_no event*

    version_no = uint

### Calls ###

Calls consist of an enter and leave event pair.  Calls are numbered from zero,
and the call number is implied for the enter event.

    event = 0x00 thread_no call_sig call_detail+  // enter call (version_no >= 4)
          | 0x00 call_sig call_detail+            // enter call (version_no < 4)
          | 0x01 call_no call_detail+             // leave call

    call_sig = id function_name count arg_name*  // first occurrence
             | id                                // follow-on occurrences

    call_detail = 0x00                  // terminator
                | 0x01 arg_no value     // argument value
                | 0x02 value            // return value
                | 0x03 thread_no        // thread number (version_no < 4)
                | 0x04 count frame*     // stack backtrace

    arg_name = string
    function_name = string

    arg_no = uint
    call_no = uint
    thread_no = uint

    id = uint

### Values ###

    value = 0x00                    // null pointer
          | 0x01                    // false value
          | 0x02                    // true
          | 0x03 uint               // negative integer
          | 0x04 uint               // positive integer value
          | 0x05 float              // single-precision floating point value
          | 0x06 double             // double-precision floating point value
          | 0x07 string             // character string value (zero terminator implied)
          | 0x08 string             // binary blob
          | 0x09 enum_sig value     // enumeration (version_no >= 3)
          | 0x09 string value       // enumeration (version_no < 3)
          | 0x0a bitmask_sig value  // integer bitmask
          | 0x0b count value*       // array
          | 0x0c struct_sig value*  // structure
          | 0x0d uint               // opaque pointer
          | 0x0e value value        // human-machine representation
          | 0x0f wstring            // wide character string value (zero terminator implied)

    enum_sig = id count (name value)+  // first occurrence
             | id                      // follow-on occurrences

    bitmask_sig = id count (name value)+  // first occurrence
                | id                      // follow-on occurrences

    struct_sig = id count member_name*  // first occurrence
               | id                     // follow-on occurrences

    name = string
    member_name = string

    wstring = count uint*

### Backtraces ###

    frame = id frame_detail+  // first occurrence
          | id                // follow-on occurrences

    frame_detail = 0x00         // terminator
                 | 0x01 string  // module name
                 | 0x02 string  // function name
                 | 0x03 string  // source file name
                 | 0x04 uint    // source line number
                 | 0x05 uint    // byte offset from module start
