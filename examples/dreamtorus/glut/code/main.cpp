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

- Created by Cass Everitt on 2/1/12.

*/

#include <GL/Regal.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/RegalGLUT.h>
#endif

#include <GL/RegalGLU.h>

#include "render.h"

#include <cstdio>
#include <cstdlib>

static void myDisplay()
{
  dreamTorusDisplay(true);
  glutSwapBuffers();
}

static void myTick(int dummy)
{
  glutPostRedisplay();
  glutTimerFunc( 16, myTick, 0 );
}

static void myToggle(GLenum e, const char *message)
{
  if (glIsEnabled(e))
    glDisable(e);
  else
    glEnable(e);

  if (message)
    printf("%s %s.\n",message, glIsEnabled(e) ? "enabled" : "disabled");
}

static void myKeyboard(unsigned char c, int x, int y)
{
  switch (c)
  {
    case 'q':
    case 27:  /* Esc key */
      exit(0);
      break;

    // GL_REGAL_enable
    // https://github.com/p3/regal/blob/master/doc/extensions/GL_REGAL_enable.txt

    case 'e':
    case 'E':
      myToggle(GL_ERROR_REGAL,"Regal error checking is");
      break;

    case 'd':
    case 'D':
      myToggle(GL_DEBUG_REGAL,"Regal debug checking is");
      break;

    case 'l':
    case 'L':
      myToggle(GL_LOG_REGAL,"Regal driver logging is");
      break;

    case 'm':
    case 'M':
      myToggle(GL_EMULATION_REGAL,"Regal emulation is");
      break;

    case 'r':
    case 'R':
      myToggle(GL_DRIVER_REGAL,"Regal driver is");
      break;
  }
}

static void myOutput(GLenum stream, GLsizei length, const GLchar *message, GLvoid *context)
{
  static int line = 0;
  fprintf(stdout,"%6d | %s",++line,message);
  fflush(stdout);
}

static void myError(GLenum error)
{
  printf("dreamtorus error: %s\n",glErrorStringREGAL(error));
}

int main(int argc, const char *argv[])
{
  #ifndef EMSCRIPTEN
  glutInitDisplayString("rgba>=8 depth double");
  glutInitWindowSize(500, 500);
  #endif
  glutInit( &argc, (char **) argv);
  glutCreateWindow("dreamtorus");

  glLogMessageCallbackREGAL(myOutput);

  // Regal workaround for OSX GLUT

  #ifdef __APPLE__
  RegalMakeCurrent(CGLGetCurrentContext());
  #endif

  // Regal workaround for Emscripten GLUT emulation

  #ifdef EMSCRIPTEN
  RegalMakeCurrent((RegalSystemContext)1);
  #endif

  RegalSetErrorCallback(myError);

  // Exercise REGAL_extension_query extension

  if (glIsSupportedREGAL("GL_REGAL_extension_query"))
  {
    printf("GL_REGAL_extension_query is supported.\n");

    if (glIsSupportedREGAL("GL_EXT_debug_marker"))
      printf("GL_EXT_debug_marker is supported.\n");
    else
      printf("GL_EXT_debug_marker is not supported.\n");

    if (glIsSupportedREGAL("GL_EXT_framebuffer_object"))
      printf("GL_EXT_framebuffer_object is supported.\n");
    else
      printf("GL_EXT_framebuffer_object is not supported.\n");

    if (glIsSupportedREGAL("GL_EXT_direct_state_access"))
      printf("GL_EXT_direct_state_access is supported.\n");
    else
      printf("GL_EXT_direct_state_access is not supported.\n");

    if (glIsSupportedREGAL("GL_NV_path_rendering"))
      printf("GL_NV_path_rendering is supported.\n");
    else
      printf("GL_NV_path_rendering is not supported.\n");
  }

  printf("\n");
  printf("E - Toggle GL error checking.\n");
  printf("D - Toggle debug checking.\n");
  printf("L - Toggle driver logging.\n");
  printf("M - Toggle emulation.\n");
  printf("R - Toggle driver calls.\n");
  printf("\n");

  glutTimerFunc(16, myTick, 0);
  glutDisplayFunc(myDisplay);
  glutReshapeFunc(dreamTorusReshape);
  glutKeyboardFunc(myKeyboard);
  glutMainLoop();
  return 0;
}
