.. image::  https://raw.github.com/p3/regal/master/doc/regal.jpg
   :height: 128 px
   :width:  128 px
   :align: right

=====
Regal
=====

| https://github.com/p3/regal
| `Downloads <https://bitbucket.org/nigels_com/regal/downloads>`_, `Issues <https://github.com/p3/regal/issues>`_, `Commits <https://github.com/p3/regal/commits/master>`_.
| Status: |status|

.. |status| image:: https://travis-ci.org/nigels-com/regal.png?branch=master
   :target: https://travis-ci.org/nigels-com/regal
   :alt: Build Status

Recent News
===========

* November 2013 - Regal Wrangler support added.
* October 2013 - `Emscripten <https://github.com/kripken/emscripten>`_ support added.
* July 2013 - New binaries available for Windows and Mac.
* July 2013 - `OpenGL 4.4 <http://www.opengl.org/registry/>`_ API support added.
* July 2013 - `apitrace <http://apitrace.github.io/>`_ support added.

Summary
=======

Regal is a user-space OpenGL layer for OpenGL 2.x, 3.x, 4.x, Core contexts and ES 2.0.
Regal implements OpenGL loading, emulation for ES and Core contexts and tools for debugging.

Status
======

Regal has been in active development on github since May of 2012 ---
it is in deployment for internal projects, and has received significant
external contributions.

Features such as emulation might not be complete or bug-free enough for
every situation, milage may vary, contributions welcome.

.. contents::

Goals
=====

Regal is a portable OpenGL layer on top of existing OpenGL implementations.
Compile and link an app against Regal and deploy on various OpenGL implementations
including compatibility, core and ES 2.0 OpenGL contexts.

* Portability
    Consistent OpenGL API that runs on all major platforms:
    Windows, Linux, Mac, iOS, and Android

* Compatible
    Immediate mode, fixed function, GL_QUADS work everywhere, emulated as necessary.

* Modern OpenGL API
    Direct State Access (DSA) and Vertex Array Object (VAO), emulated as necessary.

* Open Source
    There is nothing behind the curtain.
    You can see what Regal is doing and change it if needed.

* Ease of Use
    Set breakpoints on OpenGL functions.
    Step into the code.
    Inspect state.

* Efficiency
    If it cannot be implemented efficiently in Regal, it is not universally supported.
    For example, tessellation support requires tessellation hardware.

Deployment
==========

* Most code remains unchanged.
* Optionally **#include <GL/Regal.h>**
* Link with **regal32.dll**, **libRegal.so** or **libRegal.dylib**

From an application developer's perspective, Regal just looks like an OpenGL implementation.
You link with it instead of your platform's OpenGL library or framework, and that's really
all you have to do to use Regal.  The rest of your code can remain unchanged.

Downloads
=========

* `github.com sources <https://github.com/p3/regal>`_, `ZIP snapshot <https://github.com/p3/regal/archive/master.zip>`_.

July 30th 2013

* `Windows x86 Debug <https://bitbucket.org/nigels_com/regal/downloads/Regal_Windows_x86_Debug_20130730.zip>`_ | `Windows x86 Release <https://bitbucket.org/nigels_com/regal/downloads/Regal_Windows_x86_Release_20130730.zip>`_
* `Windows AMD64 Debug <https://bitbucket.org/nigels_com/regal/downloads/Regal_Windows_x64_Debug_20130730.zip>`_ | `Windows AMD64 Release <https://bitbucket.org/nigels_com/regal/downloads/Regal_Windows_x64_Release_20130730.zip>`_
* `Mac Universal Debug <https://bitbucket.org/nigels_com/regal/downloads/Regal_Mac_Debug_20130730.tgz>`_         | `Mac Universal Release <https://bitbucket.org/nigels_com/regal/downloads/Regal_Mac_Release_20130730.tgz>`_

Archives

* `Archived Downloads <https://bitbucket.org/nigels_com/regal/downloads>`_

Features
========

Emulation
---------

- Fixed function texturing, lighting and fog.
- Direct State Access extension

  http://www.opengl.org/registry/specs/EXT/direct_state_access.txt

- GL_EXT_debug_marker extension

  http://www.khronos.org/registry/gles/extensions/EXT/EXT_debug_marker.txt

- Fixed function and DSA emulation enabled by default, except when compatibility context
  with DSA extension detected.

- Build-time configuration:

  - **REGAL_EMULATION**         -- Enable/disable the use of emulation
  - **REGAL_FORCE_EMULATION**   -- Force the use of emulation, even for compatibility contexts with DSA

- Environment variable configuration:

  - **REGAL_EMULATION**         -- Enable/disable the use of emulation
  - **REGAL_FORCE_EMULATION**   -- Force the use of emulation, even for compatibility contexts with DSA

- Runime configuration via GL_REGAL_enable_:

  - **GL_EMULATION_REGAL**      -- glEnable/glDisable emulation

.. NOTE:: Emulation is not supported in REGAL_WRANGLER=1 mode.

OpenGL error checking
---------------------

- Disabled by default.

- Build-time configuration: **REGAL_ERROR**

- Environment variable configuration: **REGAL_ERROR**

- Runtime configuration via GL_REGAL_enable_: **GL_ERROR_REGAL**

.. NOTE:: OpenGL error checking is not supported in REGAL_WRANGLER=1 mode.

Logging
-------

Regal supports detailed logging for development purposes.

- **Error**       -- Fatal and non-fatal Regal runtime errors.
- **Warning**     -- Non-fatal Regal warnings.
- **Info**        -- Informational messages.
- **App**         -- Application calls into Regal.
- **Driver**      -- Regal calls into OpenGL.
- **Internal**    -- Internal Regal calls.
- **Http**        -- HTTP traffic logging.

Build-time configuration:

- **REGAL_LOG_ERROR**
- **REGAL_LOG_WARNING**
- **REGAL_LOG_INFO**
- **REGAL_LOG_APP**
- **REGAL_LOG_DRIVER**
- **REGAL_LOG_INTERNAL**
- **REGAL_LOG_HTTP**

* Debug mode: All logging supported with error, warning, info and http logging enabled.
* Release mode: Support for application, driver and internal logging disabled by default.

Environment variable configuration:

- **REGAL_LOG_ERROR**
- **REGAL_LOG_WARNING**
- **REGAL_LOG_INFO**
- **REGAL_LOG_APP**
- **REGAL_LOG_DRIVER**
- **REGAL_LOG_INTERNAL**
- **REGAL_LOG_HTTP**
- **REGAL_LOG_API**       --- Application + Driver OpenGL logging
- **REGAL_LOG_NONE**      --- enable or disable all logging

Environment variable lookup is globally disabled by defining **REGAL_NO_GETENV**
at compile-time.

Runtime configuration via GL_REGAL_log_ extension: (**glEnable/glDisable/glIsEnabled**)

- **GL_LOG_ERROR_REGAL**
- **GL_LOG_WARNING_REGAL**
- **GL_LOG_INFO_REGAL**
- **GL_LOG_APP_REGAL**
- **GL_LOG_DRIVER_REGAL**
- **GL_LOG_INTERNAL_REGAL**
- **GL_LOG_HTTP_REGAL**

Other compile-time or runtime configuration of logging:

- **REGAL_LOG**           --- enable or disable logging
- **REGAL_LOG_FILE**      --- log to specified filename
- **REGAL_LOG_MAX_LINES** --- limit the number of lines of multi-line messages (shaders, etc)
- **REGAL_LOG_JSON**      --- log in JSON mode
- **REGAL_LOG_JSON_FILE** --- JSON log to specified filename

.. NOTE:: API logging is not supported in REGAL_WRANGLER=1 mode.

Multi-threading support
-----------------------

  - Locking and per-thread contexts enabled by default.

  - Per-thread context build-time configuration: **REGAL_NO_TLS**

  - Locking build-time configuration: **REGAL_THREAD_LOCKING**

  - Locking environment variable configuration: **REGAL_THREAD_LOCKING**

Spoofing OpenGL vendor, renderer, version and extension strings
---------------------------------------------------------------

  Build-time configuration

  - **REGAL_GL_VENDOR**
  - **REGAL_GL_RENDERER**
  - **REGAL_GL_VERSION**
  - **REGAL_GL_EXTENSIONS**

  Environment variable configuration:

  - **REGAL_GL_VENDOR**
  - **REGAL_GL_RENDERER**
  - **REGAL_GL_VERSION**
  - **REGAL_GL_EXTENSIONS**

  Environment variable lookup is globally disabled by defining **REGAL_NO_GETENV**
  at compile-time.

Browser-based interface
-----------------------

- Enabled by default.

- **REGAL_HTTP**            --- Build-time and environment variable enable/disable.
- **REGAL_HTTP_PORT**       --- Port for HTTP connections (8080 by default)
- **REGAL_HTTP_LOG_LIMIT**  --- Number of lines of logging buffered

- `http://127.0.0.1:8080/log`_                          --- View log
- `http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL`_    --- Enable/disable application logging
- `http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL`_ --- Enable/disable driver logging
- `http://127.0.0.1:8080`_                              --- State information

.. _http://127.0.0.1:8080/log:                          http://127.0.0.1:8080/log
.. _http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL:    http://127.0.0.1:8080/glEnable?GL_LOG_APP_REGAL
.. _http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL: http://127.0.0.1:8080/glEnable?GL_LOG_DRIVER_REGAL
.. _http://127.0.0.1:8080:                              http://127.0.0.1:8080

.. NOTE:: Browser interface is not supported in REGAL_WRANGLER=1 mode.

Regal OpenGL extensions
-----------------------

  - GL_REGAL_error_string_ for GLU-style GLenum to error string conversion.
  - GL_REGAL_extension_query_ for GLEW-style extension checking.
  - GL_REGAL_log_ for Regal logging configuration.
  - GL_REGAL_enable_ for Regal feature configuration.

Regal API
=========

  - **RegalSetErrorCallback**  --- Optionally set the per-context error callback function.
  - **RegalShareContext**      --- OpenGL contexts are shared.
  - **RegalMakeCurrent**       --- Set an OpenGL context as current for the thread.
  - **RegalDestroyContext**    --- Cleanup Regal context resources.

In Progress
===========

* Light-weight loader-only "wrangler" mode along the lines of GLEW.
* Context sharing support
* glPushAttrib emulation

Planned Features
================

* NV_path_rendering emulation.
* GLSL to GLSL translation.
* ES 3.0 support
* Debug label.
* Debug output.
* Display list emulation.
* Web browser-based debugging tools.

Limitations
===========

* Shared OpenGL contexts supported for single-threaded only.
* Limited GLSL language support.
* GL_QUADS only works in immediate mode or with DrawArrays for ES and core profiles.

.. _GL_REGAL_error_string:    https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_error_string.txt
.. _GL_REGAL_extension_query: https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_extension_query.txt
.. _GL_REGAL_log:             https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_log.txt
.. _GL_REGAL_enable:          https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_enable.txt

Contact Information
===================

* Discussion of Regal is primarily via the `Issue Tracker <https://github.com/p3/regal/issues>`_ currently.

Other Information
=================

* Big Android BBQ 2013: `Developing for Dedicated Android Gaming Systems <https://developer.nvidia.com/content/nvidia-shield-and-bbq-big-android-bbq-hurst-tx>`_ `(.pdf slides) <https://developer.nvidia.com/sites/default/files/akamai/mobile/docs/NVIDIA_Dedicated_Android_Gaming_2013_updated.pdf>`_.
* #AltDevBlogADay blog: `Bringing Regal OpenGL to Native Client <http://www.altdevblogaday.com/2012/09/04/bringing-regal-opengl-to-native-client/>`_.
* opengl.org forum thread: `Introducing Regal <http://www.opengl.org/discussion_boards/showthread.php/178678-Introducing-Regal?p=1241199>`_.
* Slides - `Regal for Software Development <http://www.slideshare.net/nigelstewart1/regal-tlf>`_.

.. raw:: html

    <iframe src="http://www.slideshare.net/slideshow/embed_code/13870512" width="597" height="486" frameborder="0" marginwidth="0" marginheight="0" scrolling="no" style="border:1px solid #CCC;border-width:1px 1px 0;margin-bottom:5px" allowfullscreen> </iframe>

Platform Notes
==============

Windows
-------

Linux
-----

* RedHat/Fedora recommended packages:

 * 32-bit:

  ``$ sudo yum install gcc-c++ zlib-devel libpng-devel libXmu-devel libXi-devel``

 * 64-bit

  ``$ sudo yum install gcc-c++``

  ``$ sudo yum install zlib-devel.i686   libpng-devel.i686``

  ``$ sudo yum install zlib-devel.x86_64 libpng-devel.x86_64``

  ``$ sudo yum install libXmu-devel libXi-devel``

* Ubuntu/Debian/Mint recommended packages:

 ``$ apt-get install libxmu-dev libxi-dev``

* Use GNU make with Makefile

 ``$ make -f Makefile CCACHE=ccache STRIP=``

* Set **LD_LIBRARY_PATH**

 ``$ export LD_LIBRARY_PATH=`pwd`/lib/linux``

* Run the dreamtorus example

 ``$ REGAL_FORCE_EMULATION=1; bin/linux/dreamtorus``

 ``$ REGAL_FORCE_EMULATION=0; bin/linux/dreamtorus``

 ``$ REGAL_LOG_DRIVER=1; bin/linux/dreamtorus``

OS X
----

Android
-------

iOS
---

PPAPI and NaCl
--------------

* NACL_SDK_ROOT needs to be set

* NACL_LIBC=newlib is the default, specify glibc or pnacl as alternatives

Questions and Answers
=====================

STL, Standard C++ Library, Boost
--------------------------------

* `For and against STL <http://gamedev.stackexchange.com/questions/268/stl-for-games-yea-or-nay>`_.

* `For and against Boost <http://gamedev.stackexchange.com/questions/8980/why-dont-c-game-developers-use-the-boost-library>`_.

Alternatives for OpenGL Loading
-------------------------------

* `GLEW <http://glew.sourceforge.net/>`_.

* `OpenGL Loader Generator <https://bitbucket.org/alfonse/glloadgen/wiki/Home>`_.

Related Efforts
---------------

* Mesa3D mail thread: `Mesa as part of OpenGL-on-OpenGL ES 2.0 <http://lists.freedesktop.org/archives/mesa-dev/2012-March/019692.html>`_

* `glshim <https://github.com/lunixbochs/glshim>`_, `Pandora forum thread <http://boards.openpandora.org/index.php/topic/11506-opengl-implementation-tldr-more-games/>`_.

* Runtime OpenGL to OpenGL ES translator: `glesport <http://unrealvoodoo.org/cgi-bin/cgit/glesport.git/log/>`_, `OpenGL ES for Maemo <http://unrealvoodoo.org/hiteck/blog/2007/07/opengl-es-for-maemo/>`_.

License and Credits
===================

Regal code, API database and generators are `BSD <http://opensource.org/licenses/bsd-license.php>`_ licensed.

::

  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.

`Boost <http://www.boost.org/>`_ is licensed under `Boost Software License <http://www.boost.org/users/license.html>`_.

`zlib <http://www.zlib.net/>`_ by Jean-loup Gailly and Mark Adler is licensed under `zLib License <http://www.zlib.net/zlib_license.html>`_.

`libpng <http://www.libpng.org/pub/png/libpng.html>`_ by Glenn Randers-Pehrson et. al. is licensed under `libpng license <http://www.libpng.org/pub/png/src/libpng-LICENSE.txt>`_.

`lookup3 <http://www.burtleburtle.net/bob/hash/doobs.html>`_ by Bob Jenkins is public domain.

`GLEW <http://glew.sourceforge.net/>`_ is licensed under `BSD and MIT License <http://glew.sourceforge.net/credits.html>`_.

GLU is licensed under `SGI FREE SOFTWARE LICENSE B <http://oss.sgi.com/projects/FreeB/>`_.

GLUT license:

::

  /* Copyright (c) Mark J. Kilgard, 1994, 1995, 1996, 1998. */

  /* This program is freely distributable without licensing fees  and is
     provided without guarantee or warrantee expressed or  implied. This
     program is -not- in the public domain. */

Dreamtorus example program is `public domain <http://unlicense.org/>`_.
