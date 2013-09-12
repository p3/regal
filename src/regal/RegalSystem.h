/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.4 --api wgl 4.4 --api glx 4.4 --api cgl 1.4 --api egl 1.0 --outdir .
*/

/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012-2013 Google Inc.
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
*/

/*
  Intended formatting conventions:
  $ astyle --style=allman --indent=spaces=2 --indent-switches
*/

#ifndef __REGAL_SYSTEM_H__
#define __REGAL_SYSTEM_H__

#if defined(_WIN32)
# if defined(PPAPI)
#  ifndef REGAL_SYS_PPAPI
#   define REGAL_SYS_PPAPI 1
#  endif
# else
#  ifndef REGAL_SYS_WGL
#   define REGAL_SYS_WGL 1
#  endif
# endif
# ifndef REGAL_SYS_WIN32
#  define REGAL_SYS_WIN32 1
# endif
#elif defined(__APPLE__)
# include <TargetConditionals.h>
# if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#  ifndef REGAL_SYS_IOS
#   define REGAL_SYS_IOS 1
#  endif
# else
#  ifndef REGAL_SYS_OSX
#   define REGAL_SYS_OSX 1
#  endif
#  ifndef REGAL_SYS_GLX
#   define REGAL_SYS_GLX 1
#  endif
# endif
#elif defined(__native_client__)
# ifndef REGAL_SYS_PPAPI
#  define REGAL_SYS_PPAPI 1
# endif
#elif defined(__ANDROID__)
# ifndef REGAL_SYS_ANDROID
#  define REGAL_SYS_ANDROID 1
# endif
# ifndef REGAL_SYS_EGL
#  define REGAL_SYS_EGL 1
# endif
# ifndef REGAL_SYS_GLX
#  define REGAL_SYS_GLX 0
# endif
#elif defined(EMSCRIPTEN)
# ifndef REGAL_SYS_EMSCRIPTEN
#  define REGAL_SYS_EMSCRIPTEN 1
# endif
# ifndef REGAL_SYS_EGL
#  define REGAL_SYS_EGL 1
# endif
# ifndef REGAL_SYS_ES2
#  define REGAL_SYS_ES2 1
# endif
# ifndef REGAL_SYS_EMSCRIPTEN_STATIC
#  define REGAL_SYS_EMSCRIPTEN_STATIC 0
# endif
# if REGAL_SYS_EMSCRIPTEN_STATIC
#  ifndef REGAL_NAMESPACE
#   define REGAL_NAMESPACE 1
#  endif
# endif
#elif !defined(REGAL_SYS_PPAPI) || !REGAL_SYS_PPAPI
# ifndef REGAL_SYS_X11
#  define REGAL_SYS_X11 1
# endif
# ifndef REGAL_SYS_GLX
#  define REGAL_SYS_GLX REGAL_SYS_X11
# endif
#endif

#ifndef REGAL_SYS_WGL
# define REGAL_SYS_WGL 0
#endif

#ifndef REGAL_SYS_IOS
# define REGAL_SYS_IOS 0
#endif

#ifndef REGAL_SYS_OSX
# define REGAL_SYS_OSX 0
#endif

#ifndef REGAL_SYS_PPAPI
# define REGAL_SYS_PPAPI 0
#endif

#ifndef REGAL_SYS_ANDROID
# define REGAL_SYS_ANDROID 0
#endif

#ifndef REGAL_SYS_EGL
# define REGAL_SYS_EGL 0
#endif

#ifndef REGAL_SYS_GLX
# define REGAL_SYS_GLX 0
#endif

#ifndef REGAL_SYS_X11
# define REGAL_SYS_X11 0
#endif

#ifndef REGAL_SYS_WIN32
# define REGAL_SYS_WIN32 0
#endif

#ifndef REGAL_SYS_EMSCRIPTEN
#define REGAL_SYS_EMSCRIPTEN 0
#endif

#ifndef REGAL_SYS_EMSCRIPTEN_STATIC
#define REGAL_SYS_EMSCRIPTEN_STATIC 0
#endif

#ifndef REGAL_SYS_ES1
#define REGAL_SYS_ES1 0
#endif

#ifndef REGAL_SYS_ES2
#define REGAL_SYS_ES2 (REGAL_SYS_PPAPI || REGAL_SYS_IOS || REGAL_SYS_ANDROID || REGAL_SYS_EMSCRIPTEN || REGAL_SYS_EGL)
#endif

#ifndef REGAL_SYS_GL
#define REGAL_SYS_GL (REGAL_SYS_WGL || (!REGAL_SYS_PPAPI && !REGAL_SYS_IOS && !REGAL_SYS_ANDROID && !REGAL_SYS_EMSCRIPTEN))
#endif

#endif // __REGAL_SYSTEM_H__
