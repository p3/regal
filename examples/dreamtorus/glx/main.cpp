/*

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

*/

#include <GL/Regal.h>
#include "render.h"

#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

int main (int argc, char ** argv)
{
  Display *dpy = XOpenDisplay(0);

  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = NULL;

  const char *extensions = glXQueryExtensionsString(dpy, DefaultScreen(dpy));
  printf("%s\n",extensions);

  static int visual_attribs[] =
  {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_DOUBLEBUFFER, true,
    GLX_RED_SIZE, 1,
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1,
    None
  };

  printf("Getting framebuffer config.\n");
  int fbcount;
  GLXFBConfig *fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), visual_attribs, &fbcount);
  if (!fbc)
  {
    printf("Failed to retrieve a framebuffer config.\n");
    return 1;
  }

  printf("Getting XVisualInfo\n");
  XVisualInfo *vi = glXGetVisualFromFBConfig(dpy, fbc[0]);

  XSetWindowAttributes swa;
  printf("Creating colormap\n");
  swa.colormap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  swa.border_pixel = 0;
  swa.event_mask = StructureNotifyMask;

  printf("Creating window\n");
  int width = 500;
  int height = 500;
  Window win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
  if (!win)
  {
    printf("Failed to create window\n");
    return 1;
  }

  printf("Mapping window\n");
  XMapWindow(dpy, win);

  // Create an oldstyle context first, to get the correct function pointer for glXCreateContextAttribsARB
  GLXContext ctx_old = glXCreateContext(dpy, vi, 0, GL_TRUE);
  glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
  printf("glxCreateContextAttribsARB %p\n",glXCreateContextAttribsARB);
  glXMakeCurrent(dpy, 0, 0);
  glXDestroyContext(dpy, ctx_old);

  if (glXCreateContextAttribsARB == NULL)
  {
    printf("glXCreateContextAttribsARB entry point not found. Aborting.\n");
    return false;
  }

  static int context_attribs[] =
  {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
    None
  };

  printf("Creating context\n");
  GLXContext ctx = glXCreateContextAttribsARB(dpy, fbc[0], NULL, true, context_attribs);
  if (!ctx)
  {
    printf("Failed to create GL3 context.\n");
    return 1;
  }

  printf("Making context current.\n");
  glXMakeCurrent(dpy, win, ctx);

  dreamTorusReshape( width, height );
  for(int i = 0; i < 1000000; i++ ) {
     dreamTorusDisplay( true );
     glXSwapBuffers( dpy, win );
     usleep( 16000 );
  }

  ctx = glXGetCurrentContext();
  glXMakeCurrent(dpy, 0, 0);
  glXDestroyContext(dpy, ctx);
}
