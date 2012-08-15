/*
Copyright (c) 2011 NVIDIA Corporation
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
*/

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <string>
using namespace std;

#include <boost/print/print_string.hpp>
using boost::print::print_string;

#if REGAL_SYS_WGL

typedef void *HMODULE;

extern "C" {
  HMODULE __stdcall LoadLibraryA(const char *filename);
  void *  __stdcall GetProcAddress(HMODULE hModule, const char *proc);
}
#endif

#if REGAL_SYS_NACL
extern void _naclPrintf(const char* str, ...);
extern void* _naclGetProcAddress(const char* name);
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#ifndef REGAL_NO_ASSERT
void
AssertFunction(const char *file, const size_t line, const char *expr)
{
  string message = print_string(file, " ", line, ": ", expr);

#ifdef REGAL_ASSERT_FUNCTION
  REGAL_ASSERT_FUNCTION(message);
#else
  Error("Assertion Failed: ", message);
#endif
}
#endif

inline bool fileExists(const char *path)
{
  FILE *f = fopen(path,"r");
  if (f)
    fclose(f);
  return f != NULL;
}

inline const char * getEnvironment(const char * const var)
{
  const char *ret = NULL;

  if (var) {
    ret = GetEnv(var);

    /* Treat empty environment variable the same as non-existant */
    if (!ret || *ret=='\0')
      return NULL;
  }

  return ret;
}

static
string libraryLocation(const char *lib)
{
  const char *ret = NULL;

  if (!strcmp(lib, "GL") || !strcmp(lib, "WGL") || !strcmp(lib, "GLX")) {
    // First, try ... variable

    // Second, try default installation location

    if (!ret) {
#if REGAL_SYS_OSX
      ret = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
#endif

#if REGAL_SYS_WGL
      ret = getEnvironment("windir");

      /* XP32, Cygwin */
      if (!ret)
        ret = getEnvironment("WINDIR");

      if (ret) {
        return string(ret) + "\\system32\\opengl32.dll";
      }
#endif

#if REGAL_SYS_GLX

#if defined(__x86_64__) || defined(__x86_64)
      const char * const candidates[] = {
        "/usr/lib/amd64/libGL.so.1",              // Solaris
        "/usr/lib64/nvidia/libGL.so.1",           // RedHat
        "/usr/lib64/libGL.so.1",                  // RedHat
        "/usr/lib/nvidia-current/libGL.so.1",     // Ubuntu NVIDIA
        "/usr/lib/libGL.so.1",                    // Ubuntu
        NULL
      };
#else
      const char * const candidates[] = {
        "/usr/lib32/nvidia-current/libGL.so.1",   // Ubuntu NVIDIA
        "/usr/lib32/libGL.so.1",                  // Ubuntu
        "/usr/lib/libGL.so.1",                    // RedHat & Solaris
        NULL
      };
#endif
      for (const char * const *i = candidates; *i; ++i) {
        if (fileExists(*i))
          return *i;
      }
#endif
    }
  }
  return ret;
}

#if REGAL_SYS_OSX

#include <dlfcn.h>

void *GetProcAddress( const char * entry )
{
    static const string lib_OpenGL_filename = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
    static void         *lib_OpenGL = NULL;

    static const string lib_GL_filename = "/System/Library/Frameworks/OpenGL.framework/Libraries/libGL.dylib";
    static void         *lib_GL = NULL;

    if( lib_OpenGL == NULL || lib_GL == NULL ) {
        // this chdir business is a hacky solution to avoid recursion
        // when using libRegal as libGL via symlink and DYLD_LIBRARY_PATH=.

        char old_cwd[256];
        getcwd( old_cwd, sizeof( old_cwd ) );
        chdir( "/" );

        // CGL entry points are in OpenGL framework

        if (!lib_OpenGL) {
            lib_OpenGL = dlopen(lib_OpenGL_filename.c_str() , RTLD_LAZY);
            Info("Loading OpenGL from ",lib_OpenGL_filename,lib_OpenGL ? " succeeded." : " failed.");
        }

        // GL entry point are in libGL.dylib

        if (!lib_GL) {
            lib_GL = dlopen(lib_GL_filename.c_str(), RTLD_LAZY);
            Info("Loading OpenGL from ",lib_GL_filename,lib_GL ? " succeeded." : " failed.");
        }
        chdir( old_cwd );
    }

    if (!entry)
        return NULL;

    if (lib_OpenGL && lib_GL) {
        void * sym;
        sym = dlsym( lib_OpenGL, entry );
        ITrace(lib_OpenGL_filename," load of ",entry,sym ? " succeeded." : " failed.");
        if (sym) {
            return sym;
        }
        sym = dlsym( lib_GL, entry );
        ITrace(lib_GL_filename," load of ",entry,sym ? " succeeded." : " failed.");
        return sym;
    }
    return NULL;
}

#elif REGAL_SYS_NACL

void *GetProcAddress(const char *lookupName)
{
  return NULL;
}

#elif REGAL_SYS_IOS

#include <dlfcn.h>

void *GetProcAddress( const char * entry )
{
    static void * lib_OpenGLES = NULL;
    if (lib_OpenGLES == NULL) {
        lib_OpenGLES = dlopen( "/System/Library/Frameworks/OpenGLES.framework/OpenGLES", RTLD_LAZY );
    }

    if (!entry)
        return NULL;

    if (lib_OpenGLES) {
        void * sym;
        sym = dlsym( lib_OpenGLES, entry );
        return sym;
    }
    return NULL;
}

#elif REGAL_SYS_GLX

#include <dlfcn.h>

void *GetProcAddress( const char * entry )
{
    static void * lib_GL = NULL;
    static string lib_GL_filename;
    if (!lib_GL_filename.length()) {
        lib_GL_filename = libraryLocation("GL");
    }
    if (!lib_GL && lib_GL_filename.length()) {
        Info("Loading OpenGL from ",lib_GL_filename);
        lib_GL = dlopen( lib_GL_filename.c_str(), RTLD_LAZY );
    }

    if (!entry)
        return NULL;

    if (lib_GL) {
        void * sym;
        sym = dlsym( lib_GL, entry );
        ITrace("Loading ",entry," from ",lib_GL_filename,sym ? " succeeded." : " failed.");
        return sym;
    }
    return NULL;
}

#elif REGAL_SYS_ANDROID

#include <dlfcn.h>

void *GetProcAddress(const char *entry)
{
  static const string lib_EGL_filename = "/system/lib/libEGL.so";
  static void        *lib_EGL = NULL;
  static const string lib_GLESv2_filename = "/system/lib/libGLESv2.so";
  static void        *lib_GLESv2 = NULL;

  if (!lib_GLESv2)
  {
    lib_GLESv2 = dlopen( lib_GLESv2_filename.c_str(), RTLD_LAZY );
    Info("Loading GLES from ",lib_GLESv2_filename,lib_GLESv2 ? " succeeded." : " failed.");
  }

  if (!lib_EGL)
  {
    lib_EGL = dlopen( lib_EGL_filename.c_str(), RTLD_LAZY );
    Info("Loading EGL from ",lib_EGL_filename,lib_EGL ? " succeeded." : " failed.");
  }

  if (!entry)
      return NULL;

  if (lib_GLESv2 && lib_EGL)
  {
      void *sym = dlsym( lib_GLESv2, entry );
      ITrace(lib_GLESv2_filename," load of ",entry,sym ? " succeeded." : " failed.");
      if (!sym)
      {
        sym = dlsym( lib_EGL, entry );
        ITrace(lib_EGL_filename," load of ",entry,sym ? " succeeded." : " failed.");
      }
      return sym;
  }
  return NULL;
}

#elif REGAL_SYS_WGL

void *GetProcAddress( const char * entry )
{
    static HMODULE lib_GL = 0;
    static PROC (__stdcall *wglGetProcAddress)(LPCSTR lpszProc);
    static string lib_GL_filename;
    if (!lib_GL_filename.length()) {
        lib_GL_filename = libraryLocation("GL");
    }
    if (!lib_GL && lib_GL_filename.length()) {
        Info("Loading OpenGL from ",lib_GL_filename);
        lib_GL = LoadLibraryA(lib_GL_filename.c_str());
    }

    if (!entry)
        return NULL;

    if (lib_GL) {
        void * sym;
        sym = ::GetProcAddress( lib_GL, entry );
        if (sym)
          return sym;
        if (!wglGetProcAddress)
          wglGetProcAddress = (PROC (__stdcall *)(LPCSTR)) ::GetProcAddress( lib_GL, "wglGetProcAddress");
        if (wglGetProcAddress)
          return wglGetProcAddress(entry);
    }
    return NULL;
}

#endif

REGAL_NAMESPACE_END


