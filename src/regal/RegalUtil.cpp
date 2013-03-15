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
#include <boost/print/string_list.hpp>

using boost::print::print_string;
using boost::print::string_list;

#include "RegalConfig.h"

#if REGAL_SYS_X11
#include <X11/Xatom.h>
#endif

// alloca for VC8

#ifdef _MSC_VER
#include <malloc.h>
#define alloca _alloca
#endif

#if REGAL_SYS_WGL

typedef void *HMODULE;

extern "C" {
  HMODULE __stdcall LoadLibraryA(const char *filename);
  void *  __stdcall GetProcAddress(HMODULE hModule, const char *proc);
}
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#if !REGAL_NO_ASSERT
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
  LIBRARY_ES1,
  LIBRARY_ES2,
  LIBRARY_GL,
  LIBRARY_WGL,
  LIBRARY_GLX,
  LIBRARY_CGL,
  LIBRARY_EGL
};

#ifndef __native_client__
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
        "/usr/lib/fglrx/libGL.so.1",                    // Ubuntu 12.04 32-bit ATI Radeon HD 5450
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

  // ES1

#if REGAL_SYS_ES1
  if (library==LIBRARY_ES1)
  {
    // First, try ... variable

    // Second, try default installation location

    if (!ret)
    {
#if REGAL_SYS_ANDROID
      return "/System/lib/libGLESv1_CM.so";
#elif REGAL_SYS_EGL || REGAL_SYS_GLX
      // TODO - ES1 for Linux?
#endif
    }
  }
#endif

  /* Ubuntu 12.04 x86

  $ find  /usr/lib/i386-linux-gnu/ -name "libEGL*" | xargs ls -la
  lrwxrwxrwx 1 root root     18 Oct 12 05:02 /usr/lib/i386-linux-gnu/libEGL.so -> mesa-egl/libEGL.so
  lrwxrwxrwx 1 root root     15 Oct 12 05:02 /usr/lib/i386-linux-gnu/mesa-egl/libEGL.so -> libEGL.so.1.0.0
  lrwxrwxrwx 1 root root     15 Oct 12 05:02 /usr/lib/i386-linux-gnu/mesa-egl/libEGL.so.1 -> libEGL.so.1.0.0
  -rw-r--r-- 1 root root 129660 Oct 12 05:02 /usr/lib/i386-linux-gnu/mesa-egl/libEGL.so.1.0.0

  $ find  /usr/lib/i386-linux-gnu/ -name "libGLES*" | xargs ls -la
  lrwxrwxrwx 1 root root    18 Oct 12 05:02 /usr/lib/i386-linux-gnu/mesa-egl/libGLESv2.so.2 -> libGLESv2.so.2.0.0
  -rw-r--r-- 1 root root 16248 Oct 12 05:02 /usr/lib/i386-linux-gnu/mesa-egl/libGLESv2.so.2.0.0

  */

  // ES2

  if (library==LIBRARY_ES2)
  {
    // First, try ... variable

    // Second, try default installation location

    if (!ret)
    {
#if REGAL_SYS_EGL || REGAL_SYS_GLX
#if defined(__arm__)
      return "/usr/lib/libGLESv2.so";
#elif defined(__x86_64__) || defined(__x86_64)
      return "/usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2";
#else
      return "/usr/lib/i386-linux-gnu/mesa-egl/libGLESv2.so.2";
#endif
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
#if REGAL_SYS_EGL
#if defined(__arm__)
      return "/usr/lib/libEGL.so";
#elif defined(__x86_64__) || defined(__x86_64)
      return "/usr/lib/x86_64-linux-gnu/mesa-egl/libEGL.so";
#else
      return "/usr/lib/i386-linux-gnu/mesa-egl/libEGL.so";
#endif
#endif
    }
  }

  return ret;
}
#endif

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

#elif REGAL_SYS_GLX || REGAL_SYS_EGL

// General purpose GetProcAddress for GLX or EGL,
// GL or ES 2.0 for Linux, so far.

#include <dlfcn.h>

void *GetProcAddress(const char *entry)
{
  // Early-out for NULL entry name

  if (!entry)
    return NULL;

  // GL ES 1.x

#if REGAL_SYS_ES1

  static void       *lib_ES1          = NULL;
  static const char *lib_ES1_filename = NULL;

  if (Regal::Config::sysES1)
  {
    // Search for OpenGL ES library as necessary

    if (!lib_ES1_filename)
    {
      lib_ES1_filename = libraryLocation(LIBRARY_ES1);
      if (!lib_ES1_filename)
        Warning("OpenGL ES 1.x library not found.");
    }

    // Load the OpenGL ES 1.x library as necessary

    if (!lib_ES1 && lib_ES1_filename)
    {
      Info("Loading OpenGL ES 1.x from ",lib_ES1_filename);
      lib_ES1 = dlopen( lib_ES1_filename, RTLD_LAZY );
    }
  }
#endif

  // GL ES 2.0

#if REGAL_SYS_ES2

  static void       *lib_ES2          = NULL;
  static const char *lib_ES2_filename = NULL;

  if (Regal::Config::sysES2)
  {
    // Search for OpenGL ES library (libGLESv2.so usually) as necessary

    if (!lib_ES2_filename)
    {
      lib_ES2_filename = libraryLocation(LIBRARY_ES2);
      if (!lib_ES2_filename)
        Warning("OpenGL ES 2.0 library not found.");
    }

    // Load the OpenGL ES library as necessary

    if (!lib_ES2 && lib_ES2_filename)
    {
      Info("Loading OpenGL ES 2.0 from ",lib_ES2_filename);
      lib_ES2 = dlopen( lib_ES2_filename, RTLD_LAZY );
    }
  }
#endif

  // Desktop GL

#if REGAL_SYS_GL

  static void       *lib_GL           = NULL;
  static const char *lib_GL_filename  = NULL;

  if (Regal::Config::sysGL)
  {
    // Search for OpenGL library (libGL.so.1 usually) as necessary

    if (!lib_GL_filename)
    {
      lib_GL_filename = libraryLocation(LIBRARY_GL);
      if (!lib_GL_filename)
        Warning("OpenGL library not found.");
    }

    // Load the OpenGL library as necessary

    if (!lib_GL && lib_GL_filename)
    {
      Info("Loading OpenGL from ",lib_GL_filename);
      lib_GL = dlopen( lib_GL_filename, RTLD_LAZY );
    }
  }
#endif

  // EGL

#if REGAL_SYS_EGL

  static void       *lib_EGL          = NULL;
  static const char *lib_EGL_filename = NULL;

  if (Regal::Config::sysEGL)
  {
    // Search for EGL library (libEGL.so usually) as necessary

    if (!lib_EGL_filename)
    {
      lib_EGL_filename = libraryLocation(LIBRARY_EGL);
      if (!lib_EGL_filename)
        Warning("EGL library not found.");
    }

    // Load the EGL library as necessary

    if (!lib_EGL && lib_EGL_filename)
    {
      Info("Loading EGL from ",lib_EGL_filename);
      lib_EGL = dlopen( lib_EGL_filename, RTLD_LAZY );
    }
  }
#endif

  // GLX

#if REGAL_SYS_GLX

  static void       *lib_GLX           = NULL;
  static const char *lib_GLX_filename  = NULL;

  if (Regal::Config::sysGLX)
  {
    // Search for OpenGL library (libGL.so.1 usually) as necessary

    if (!lib_GLX_filename)
    {
      lib_GLX_filename = libraryLocation(LIBRARY_GL);
      if (!lib_GLX_filename)
        Warning("GL/GLX library not found.");
    }

    // Load the OpenGL library as necessary

    if (!lib_GLX && lib_GLX_filename)
    {
      Info("Loading GL/GLX from: ",lib_GLX_filename);
      lib_GLX = dlopen( lib_GLX_filename, RTLD_LAZY );
    }
  }
#endif

  // Load the entry-point by name, if possible

#if REGAL_SYS_ES1
  if (Regal::Config::sysES1 && lib_ES1)
  {
    void *sym = dlsym(lib_ES1,entry);
    Internal("Regal::GetProcAddress","loading ",entry," from ",lib_ES1_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }
#endif

#if REGAL_SYS_ES2
  if (Regal::Config::sysES2 && lib_ES2)
  {
    void *sym = dlsym(lib_ES2,entry);
    Internal("Regal::GetProcAddress","loading ",entry," from ",lib_ES2_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }
#endif

#if REGAL_SYS_GL
  if (Regal::Config::sysGL && lib_GL)
  {
    void *sym = dlsym(lib_GL,entry);
    Internal("Regal::GetProcAddress","loading ",entry," from ",lib_GL_filename,sym ? " succeeded." : " failed.");
    return sym;
  }
#endif

#if REGAL_SYS_EGL
  if (Regal::Config::sysEGL && lib_EGL)
  {
    void *sym = dlsym(lib_EGL,entry);
    Internal("Regal::GetProcAddress","loading ",entry," from ",lib_EGL_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }
#endif

#if REGAL_SYS_GLX
  if (Regal::Config::sysGLX && lib_GLX)
  {
    void *sym = dlsym(lib_GLX,entry);
    Internal("Regal::GetProcAddress","loading ",entry," from ",lib_GLX_filename,sym ? " succeeded." : " failed.");
    if (sym)
      return sym;
  }
#endif

#if REGAL_SYS_EGL
  static PFNEGLGETPROCADDRESSPROC eglGetProcAddress = NULL;

  if (Regal::Config::sysEGL && lib_EGL)
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
#endif

  // Do we need glxGetProcAddress here?

#if REGAL_SYS_GLX
#endif

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

string makePath(const string &dir, const string &filename)
{
  string tmp(dir);
  const size_t len = tmp.length();
  const char   last = len ? tmp[len-1] : '\0';
  if (len && last!='/' && last!='\\')
    tmp += "/";
  tmp += filename;
  return tmp;
}

string fileRead(FILE *file)
{
  const size_t bufferSize = 4*1024;
  char *buffer = static_cast<char *>(alloca(bufferSize+1));
  string_list<string> tmp;

  while (true)
  {
    size_t bytes = fread(buffer,1,bufferSize,file);
    buffer[bytes] = '\0';

    if (feof(file))
    {
      if (tmp.size())
      {
        tmp.push_back(buffer);
        return tmp.str();
      }
      else
        return string(buffer);
    }
    else
        tmp.push_back(buffer);
  }
}

// X11 stuff

#if REGAL_SYS_X11

// See:
//    http://tronche.com/gui/x/xlib/window-information/XGetWindowProperty.html
//    http://standards.freedesktop.org/wm-spec/latest/ar01s05.html#id2760470

std::string
windowManagerStateDescription(Display *display, Window w)
{
  if (!display || !w)
    return std::string();

  const Atom _NET_WM_STATE = XInternAtom(display,"_NET_WM_STATE",True);
  Atom type = 0;
  int  format = 0;
  unsigned long nitems = 0;
  unsigned long bytes = 0;
  Atom *list = NULL;

  if (XGetWindowProperty(display,w,_NET_WM_STATE,0,1024,False,XA_ATOM,&type,&format,&nitems,&bytes,(unsigned char **) &list)==Success)
  {
    const Atom _NET_WM_STATE_MODAL      = XInternAtom(display,"_NET_WM_STATE_MODAL",True);
    const Atom _NET_WM_STATE_HIDDEN     = XInternAtom(display,"_NET_WM_STATE_HIDDEN",True);
    const Atom _NET_WM_STATE_FULLSCREEN = XInternAtom(display,"_NET_WM_STATE_FULLSCREEN",True);
    const Atom _NET_WM_STATE_FOCUSED    = XInternAtom(display,"_NET_WM_STATE_FOCUSED",True);

    string_list<std::string> tmp;

    for (unsigned long i=0; i<nitems; ++i)
    {
      if (_NET_WM_STATE_MODAL     !=None && list[i]==_NET_WM_STATE_MODAL     ) tmp << "modal";
      if (_NET_WM_STATE_HIDDEN    !=None && list[i]==_NET_WM_STATE_HIDDEN    ) tmp << "hidden";
      if (_NET_WM_STATE_FULLSCREEN!=None && list[i]==_NET_WM_STATE_FULLSCREEN) tmp << "fullscreen";
      if (_NET_WM_STATE_FOCUSED   !=None && list[i]==_NET_WM_STATE_FOCUSED   ) tmp << "focused";
    }

    XFree(list);
    return tmp.join(", ");
  }

  return std::string();
}
#endif

REGAL_NAMESPACE_END


