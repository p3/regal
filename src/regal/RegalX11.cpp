/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012-2013 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
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

 X11 emulation for Android, Windows and Mac
 Nigel Stewart

*/

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

/* Only relevant if X11 isn't available: REGAL_X11==0 */

#if REGAL_SYS_GLX && !REGAL_SYS_X11

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <boost/print/interface.hpp>

#include "RegalLog.h"
#include "RegalInit.h"

#define True  1
#define False 0

extern "C" {

  ////////// libX11

  REGAL_DECL int
  DefaultScreen(Display *display)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::DefaultScreen","display=",display);
    return 1;
  }

  REGAL_DECL Window
  RootWindow(Display *display, int screen_number)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::RootWindow","display=",display," screen number=",screen_number);
    return 1;
  }

  //////////

  REGAL_DECL int
  XMapWindow(Display *display, Window w)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XMapWindow","display=",display," w=",w);
    return 1;
  }

  static Window dummyWindow = 0;

  REGAL_DECL Window
  XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int clss, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XCreateWindow","display=",display," width=",width," height=",height);
    return ++dummyWindow;
  }

  static Colormap dummyColormap = 0;

  REGAL_DECL Colormap
  XCreateColormap(Display *display, Window w, Visual *visual, int alloc)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XCreateColormap","display=",display);
    return ++dummyColormap;
  }

  REGAL_DECL Display *
  XOpenDisplay(char *display_name)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XOpenDisplay","display_name=",boost::print::optional(display_name,display_name));
    return reinterpret_cast<Display *>(1);
  }

  REGAL_DECL int
  XCloseDisplay(Display *display)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XCloseDisplay","display=",display);
    return True;
  }

  static Pixmap dummyPixmap = 0;

  REGAL_DECL Pixmap
  XCreatePixmap(Display *display, Drawable d, unsigned int width, unsigned int height, unsigned int depth)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XCreatePixmap","display=",display," width=",width," height=",height);
    return ++dummyPixmap;
  }

  REGAL_DECL int
  XFreePixmap(Display *display, Pixmap pixmap)
  {
    ::REGAL_NAMESPACE_INTERNAL::Init::init();
    Internal("Regal::XFreePixmap","display=",display," pixmap=",pixmap);
    return True;
  }

} // extern "C"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

REGAL_NAMESPACE_END

#endif // REGAL_SYS_GLX && !REGAL_SYS_X11
