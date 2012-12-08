/*
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

enum Library
{
  LIBRARY_GL,
  LIBRARY_GLES,
  LIBRARY_WGL,
  LIBRARY_GLX,
  LIBRARY_CGL,
  LIBRARY_EGL
};

static
const char *libraryLocation(const Library &library)
{
  const char *ret = NULL;

  if (library==LIBRARY_GL || library==LIBRARY_WGL || library==LIBRARY_GLX)
  {
    // First, try ... variable

    // Second, try default installation location

    if (!ret)
    {
#if REGAL_SYS_OSX
      ret = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
#endif

#if REGAL_SYS_WGL
      ret = getEnvironment("windir");

      /* XP32, Cygwin */
      if (!ret)
        ret = getEnvironment("WINDIR");

      if (ret)
      {
        // This string will be leaked, but needs to remain
        // valid until we're completely shut down.

        char *tmp = (char *) calloc(strlen(ret)+23,1);
        assert(tmp);
        if (tmp)
        {
          strcpy(tmp,ret);
          strcat(tmp,"\\system32\\opengl32.dll");
          return tmp;
        }
        else
          return NULL;
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
        "/usr/lib/nvidia-current-updates/libGL.so.1",   // Ubuntu 12.04 32-bit NVIDIA
        "/usr/lib32/nvidia-current/libGL.so.1",         // Ubuntu NVIDIA
        "/usr/lib32/libGL.so.1",                        // Ubuntu
        "/usr/lib/libGL.so.1",                          // RedHat & Solaris
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

  // GLES

  if (library==LIBRARY_GLES)
  {
    // First, try ... variable

    // Second, try default installation location

    if (!ret)
    {
#if REGAL_SYS_GLX
      return "/usr/lib/libGLESv2.so";
#endif
    }
  }

  // EGL

  if (library==LIBRARY_EGL)
  {
    // First, try ... variable

    // Second, try default installation location

    if (!ret)
    {
#if REGAL_SYS_GLX
      return "/usr/lib/libEGL.so";
#endif
    }
  }

  return ret;
}

#if REGAL_SYS_OSX

#include <dlfcn.h>

void *GetProcAddress( const char *entry )
{
  if (!entry)
      return NULL;

  static const char *lib_OpenGL_filename = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
  static void       *lib_OpenGL = NULL;

  static const char *lib_GL_filename = "/System/Library/Frameworks/OpenGL.framework/Libraries/libGL.dylib";
  static void       *lib_GL = NULL;

  if (!lib_OpenGL || !lib_GL)
  {
    // this chdir business is a hacky solution to avoid recursion
    // when using libRegal as libGL via symlink and DYLD_LIBRARY_PATH=.

    char *oldCwd = getcwd(NULL,0);
    chdir("/");

    // CGL entry points are in OpenGL framework

    if (!lib_OpenGL) {
      lib_OpenGL = dlopen(lib_OpenGL_filename , RTLD_LAZY);
      Info("Loading OpenGL from ",lib_OpenGL_filename,lib_OpenGL ? " succeeded." : " failed.");
    }

    // GL entry point are in libGL.dylib

    if (!lib_GL) {
      lib_GL = dlopen(lib_GL_filename, RTLD_LAZY);
      Info("Loading OpenGL from ",lib_GL_filename,lib_GL ? " succeeded." : " failed.");
    }

    chdir(oldCwd);
    free(oldCwd);
  }

  if (lib_OpenGL && lib_GL)
  {
    void *sym = dlsym( lib_OpenGL, entry );
    Internal("Regal::GetProcAddress ",lib_OpenGL_filename," load of ",entry,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
    sym = dlsym( lib_GL, entry );
    Internal("Regal::GetProcAddress ",lib_GL_filename," load of ",entry,sym ? " succeeded." : " failed.");
    return sym;
  }

  return NULL;
}

#elif REGAL_SYS_PPAPI

void *GetProcAddress( const char *entry )
{
  return NULL;
}

#elif REGAL_SYS_IOS

#include <dlfcn.h>

void *GetProcAddress( const char * entry )
{
  if (!entry)
    return NULL;

  static void *lib_OpenGLES = NULL;
  if (!lib_OpenGLES)
    lib_OpenGLES = dlopen( "/System/Library/Frameworks/OpenGLES.framework/OpenGLES", RTLD_LAZY );

  if (lib_OpenGLES) {
      void *sym = dlsym( lib_OpenGLES, entry );
      return sym;
  }

  return NULL;
}

#elif REGAL_SYS_GLX && REGAL_SYS_EGL

// GLX and EGL

#include <dlfcn.h>

void *GetProcAddress(const char *entry)
{
  // Early-out for NULL entry name

  if (!entry)
    return NULL;

  // EGL

  static void       *lib_EGL          = NULL;
  static const char *lib_EGL_filename = NULL;

  // Search for EGL library (libEGL.so usually) as necessary

  if (!lib_EGL_filename)
  {
    lib_EGL_filename = libraryLocation(LIBRARY_EGL);
    if (!lib_EGL_filename)
      Warning("EGL library not found: ",lib_EGL_filename);
  }

  // Load the EGL library as necessary

  if (!lib_EGL && lib_EGL_filename)
  {
    Info("Loading EGL from ",lib_EGL_filename);
    lib_EGL = dlopen( lib_EGL_filename, RTLD_LAZY );
  }

  // GL ES

  static void       *lib_GLES          = NULL;
  static const char *lib_GLES_filename = NULL;

  // Search for OpenGL ES library (libGLESv2.so usually) as necessary

  if (!lib_GLES_filename)
  {
    lib_GLES_filename = libraryLocation(LIBRARY_GLES);
    if (!lib_GLES_filename)
      Warning("OpenGL ES library not found: ",lib_GLES_filename);
  }

  // Load the OpenGL ES library as necessary

  if (!lib_GLES && lib_GLES_filename)
  {
    Info("Loading OpenGL ES from ",lib_GLES_filename);
    lib_GLES = dlopen( lib_GLES_filename, RTLD_LAZY );
  }

  // Load the entry-point by name, if possible

  if (lib_EGL)
  {
    void *sym = dlsym(lib_EGL,entry);
    Internal("Regal::GetProcAddress ","loading ",entry," from ",lib_EGL_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }

  if (lib_GLES)
  {
    void *sym = dlsym(lib_GLES,entry);
    Internal("Regal::GetProcAddress ","loading ",entry," from ",lib_GLES_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }

  static PFNEGLGETPROCADDRESSPROC eglGetProcAddress = NULL;

  if (lib_EGL)
  {
    if (!eglGetProcAddress)
      eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC) dlsym(lib_EGL, "eglGetProcAddress");

    if (eglGetProcAddress)
    {
      void *sym = (void *) eglGetProcAddress(entry);
      Internal("Regal::GetProcAddress ","loading ",entry," from eglGetProcAddress",sym ? " succeeded." : " failed.");
      return sym;
    }
  }

  return NULL;
}

#elif REGAL_SYS_GLX

// Vanilla GLX

#include <dlfcn.h>

void *GetProcAddress(const char *entry)
{
  // Early-out for NULL entry name

  if (!entry)
    return NULL;

  static void       *lib_GL           = NULL;
  static const char *lib_GL_filename  = NULL;

  // Search for OpenGL library (libGL.so.1 usually) as necessary

  if (!lib_GL_filename)
  {
    lib_GL_filename = libraryLocation(LIBRARY_GL);
    if (!lib_GL_filename)
      Warning("OpenGL library not found.",lib_GL_filename);
  }

  // Load the OpenGL library as necessary

  if (!lib_GL && lib_GL_filename)
  {
    Info("Loading OpenGL from ",lib_GL_filename);
    lib_GL = dlopen( lib_GL_filename, RTLD_LAZY );
  }

  // Load the entry-point by name, if possible

  if (lib_GL)
  {
    void *sym = dlsym(lib_GL,entry);
    Internal("Regal::GetProcAddress ","loading ",entry," from ",lib_GL_filename,sym ? " succeeded." : " failed.");
    return sym;
  }

  return NULL;
}

#elif REGAL_SYS_ANDROID

#include <dlfcn.h>

void *GetProcAddress(const char *entry)
{
  if (!entry)
    return NULL;

  static const char *lib_GLESv2_filename = "/system/lib/libGLESv2.so";
  static void       *lib_GLESv2 = NULL;

  if (!lib_GLESv2)
  {
    lib_GLESv2 = dlopen( lib_GLESv2_filename, RTLD_LAZY );
    Info("Loading GLES from ",lib_GLESv2_filename,lib_GLESv2 ? " succeeded." : " failed.");
  }

  static const char *lib_EGL_filename = "/system/lib/libEGL.so";
  static void       *lib_EGL = NULL;

  if (!lib_EGL)
  {
    lib_EGL = dlopen( lib_EGL_filename, RTLD_LAZY );
    Info("Loading EGL from ",lib_EGL_filename,lib_EGL ? " succeeded." : " failed.");
  }

  if (lib_GLESv2 && lib_EGL)
  {
    void *sym = dlsym( lib_GLESv2, entry );
    Internal("Regal::GetProcAddress ",lib_GLESv2_filename," load of ",entry,sym ? " succeeded." : " failed.");
    if (!sym)
    {
      sym = dlsym( lib_EGL, entry );
      Internal("Regal::GetProcAddress ",lib_EGL_filename," load of ",entry,sym ? " succeeded." : " failed.");
    }
    return sym;
  }

  return NULL;
}

#elif REGAL_SYS_WGL

void *GetProcAddress( const char * entry )
{
  if (!entry)
    return NULL;

  static const char *lib_GL_filename = NULL;
  if (!lib_GL_filename)
    lib_GL_filename = libraryLocation(LIBRARY_GL);

  static HMODULE lib_GL = 0;

  if (!lib_GL && lib_GL_filename) {
    Info("Loading OpenGL from ",lib_GL_filename);
    lib_GL = LoadLibraryA(lib_GL_filename);
  }

  static PROC (__stdcall *wglGetProcAddress)(LPCSTR lpszProc);

  if (lib_GL)
  {
    void *sym = ::GetProcAddress( lib_GL, entry );
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

bool fileExists(const char *filename)
{
  FILE *f = fopen(filename,"r");
  if (f)
    fclose(f);
  return f!=NULL;
}

FILE *fileOpen(const char *filename, const char *mode)
{
  if (filename && *filename)
  {
    if (!strcmp(filename,"stdout"))
      return stdout;

    if (!strcmp(filename,"stderr"))
      return stderr;

    return fopen(filename,mode);
  }

  return NULL;
}

void fileClose(FILE **file)
{
  if (!file || !*file)
    return;

  if (*file==stdout)
    return;

  if (*file==stderr)
    return;

  fclose(*file);
  *file = NULL;
}

REGAL_NAMESPACE_END


