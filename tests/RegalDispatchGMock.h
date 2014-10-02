/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.5 --api wgl 4.5 --api glx 4.5 --api cgl 1.4 --api egl 1.0 --outdir .
*/

/*
  Copyright (c) 2011-2014 NVIDIA Corporation
  Copyright (c) 2012-2014 Scott Nations
  Copyright (c) 2012-2014 Nigel Stewart
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012 Mathias Schott
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

#ifndef __REGAL_DISPATCH_GMOCK_H__
#define __REGAL_DISPATCH_GMOCK_H__

#include "gmock/gmock.h"

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DispatchTableGL;

struct RegalGMockInterface
{
  RegalGMockInterface();
  virtual ~RegalGMockInterface();
  static RegalGMockInterface* current;

  MOCK_METHOD2(glAccum, void(GLenum, GLfloat));
  MOCK_METHOD2(glAlphaFunc, void(GLenum, GLclampf));
  MOCK_METHOD1(glBegin, void(GLenum));
  MOCK_METHOD7(glBitmap, void(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *));
  MOCK_METHOD2(glBlendFunc, void(GLenum, GLenum));
  MOCK_METHOD1(glCallList, void(GLuint));
  MOCK_METHOD3(glCallLists, void(GLsizei, GLenum, const GLvoid *));
  MOCK_METHOD1(glClear, void(GLbitfield));
  MOCK_METHOD4(glClearAccum, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD4(glClearColor, void(GLclampf, GLclampf, GLclampf, GLclampf));
  MOCK_METHOD1(glClearDepth, void(GLclampd));
  MOCK_METHOD1(glClearIndex, void(GLfloat));
  MOCK_METHOD1(glClearStencil, void(GLint));
  MOCK_METHOD2(glClipPlane, void(GLenum, const GLdouble *));
  MOCK_METHOD3(glColor3b, void(GLbyte, GLbyte, GLbyte));
  MOCK_METHOD1(glColor3bv, void(const GLbyte *));
  MOCK_METHOD3(glColor3d, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glColor3dv, void(const GLdouble *));
  MOCK_METHOD3(glColor3f, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glColor3fv, void(const GLfloat *));
  MOCK_METHOD3(glColor3i, void(GLint, GLint, GLint));
  MOCK_METHOD1(glColor3iv, void(const GLint *));
  MOCK_METHOD3(glColor3s, void(GLshort, GLshort, GLshort));
  MOCK_METHOD1(glColor3sv, void(const GLshort *));
  MOCK_METHOD3(glColor3ub, void(GLubyte, GLubyte, GLubyte));
  MOCK_METHOD1(glColor3ubv, void(const GLubyte *));
  MOCK_METHOD3(glColor3ui, void(GLuint, GLuint, GLuint));
  MOCK_METHOD1(glColor3uiv, void(const GLuint *));
  MOCK_METHOD3(glColor3us, void(GLushort, GLushort, GLushort));
  MOCK_METHOD1(glColor3usv, void(const GLushort *));
  MOCK_METHOD4(glColor4b, void(GLbyte, GLbyte, GLbyte, GLbyte));
  MOCK_METHOD1(glColor4bv, void(const GLbyte *));
  MOCK_METHOD4(glColor4d, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glColor4dv, void(const GLdouble *));
  MOCK_METHOD4(glColor4f, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glColor4fv, void(const GLfloat *));
  MOCK_METHOD4(glColor4i, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD1(glColor4iv, void(const GLint *));
  MOCK_METHOD4(glColor4s, void(GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD1(glColor4sv, void(const GLshort *));
  MOCK_METHOD4(glColor4ub, void(GLubyte, GLubyte, GLubyte, GLubyte));
  MOCK_METHOD1(glColor4ubv, void(const GLubyte *));
  MOCK_METHOD4(glColor4ui, void(GLuint, GLuint, GLuint, GLuint));
  MOCK_METHOD1(glColor4uiv, void(const GLuint *));
  MOCK_METHOD4(glColor4us, void(GLushort, GLushort, GLushort, GLushort));
  MOCK_METHOD1(glColor4usv, void(const GLushort *));
  MOCK_METHOD4(glColorMask, void(GLboolean, GLboolean, GLboolean, GLboolean));
  MOCK_METHOD2(glColorMaterial, void(GLenum, GLenum));
  MOCK_METHOD5(glCopyPixels, void(GLint, GLint, GLsizei, GLsizei, GLenum));
  MOCK_METHOD1(glCullFace, void(GLenum));
  MOCK_METHOD2(glDeleteLists, void(GLuint, GLsizei));
  MOCK_METHOD1(glDepthFunc, void(GLenum));
  MOCK_METHOD1(glDepthMask, void(GLboolean));
  MOCK_METHOD2(glDepthRange, void(GLclampd, GLclampd));
  MOCK_METHOD1(glDisable, void(GLenum));
  MOCK_METHOD1(glDrawBuffer, void(GLenum));
  MOCK_METHOD5(glDrawPixels, void(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *));
  MOCK_METHOD1(glEdgeFlag, void(GLboolean));
  MOCK_METHOD1(glEdgeFlagv, void(const GLboolean *));
  MOCK_METHOD1(glEnable, void(GLenum));
  MOCK_METHOD0(glEnd, void(void));
  MOCK_METHOD0(glEndList, void(void));
  MOCK_METHOD1(glEvalCoord1d, void(GLdouble));
  MOCK_METHOD1(glEvalCoord1dv, void(const GLdouble *));
  MOCK_METHOD1(glEvalCoord1f, void(GLfloat));
  MOCK_METHOD1(glEvalCoord1fv, void(const GLfloat *));
  MOCK_METHOD2(glEvalCoord2d, void(GLdouble, GLdouble));
  MOCK_METHOD1(glEvalCoord2dv, void(const GLdouble *));
  MOCK_METHOD2(glEvalCoord2f, void(GLfloat, GLfloat));
  MOCK_METHOD1(glEvalCoord2fv, void(const GLfloat *));
  MOCK_METHOD3(glEvalMesh1, void(GLenum, GLint, GLint));
  MOCK_METHOD5(glEvalMesh2, void(GLenum, GLint, GLint, GLint, GLint));
  MOCK_METHOD1(glEvalPoint1, void(GLint));
  MOCK_METHOD2(glEvalPoint2, void(GLint, GLint));
  MOCK_METHOD3(glFeedbackBuffer, void(GLsizei, GLenum, GLfloat *));
  MOCK_METHOD0(glFinish, void(void));
  MOCK_METHOD0(glFlush, void(void));
  MOCK_METHOD2(glFogf, void(GLenum, GLfloat));
  MOCK_METHOD2(glFogfv, void(GLenum, const GLfloat *));
  MOCK_METHOD2(glFogi, void(GLenum, GLint));
  MOCK_METHOD2(glFogiv, void(GLenum, const GLint *));
  MOCK_METHOD1(glFrontFace, void(GLenum));
  MOCK_METHOD6(glFrustum, void(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glGenLists, GLuint(GLsizei));
  MOCK_METHOD2(glGetBooleanv, void(GLenum, GLboolean *));
  MOCK_METHOD2(glGetClipPlane, void(GLenum, GLdouble *));
  MOCK_METHOD2(glGetDoublev, void(GLenum, GLdouble *));
  MOCK_METHOD0(glGetError, GLenum(void));
  MOCK_METHOD2(glGetFloatv, void(GLenum, GLfloat *));
  MOCK_METHOD2(glGetIntegerv, void(GLenum, GLint *));
  MOCK_METHOD3(glGetLightfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetLightiv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD3(glGetMapdv, void(GLenum, GLenum, GLdouble *));
  MOCK_METHOD3(glGetMapfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetMapiv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD3(glGetMaterialfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetMaterialiv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD2(glGetPixelMapfv, void(GLenum, GLfloat *));
  MOCK_METHOD2(glGetPixelMapuiv, void(GLenum, GLuint *));
  MOCK_METHOD2(glGetPixelMapusv, void(GLenum, GLushort *));
  MOCK_METHOD1(glGetPolygonStipple, void(GLubyte *));
  MOCK_METHOD1(glGetString, const GLubyte *(GLenum));
  MOCK_METHOD3(glGetTexEnvfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetTexEnviv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD3(glGetTexGendv, void(GLenum, GLenum, GLdouble *));
  MOCK_METHOD3(glGetTexGenfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetTexGeniv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD5(glGetTexImage, void(GLenum, GLint, GLenum, GLenum, GLvoid *));
  MOCK_METHOD4(glGetTexLevelParameterfv, void(GLenum, GLint, GLenum, GLfloat *));
  MOCK_METHOD4(glGetTexLevelParameteriv, void(GLenum, GLint, GLenum, GLint *));
  MOCK_METHOD3(glGetTexParameterfv, void(GLenum, GLenum, GLfloat *));
  MOCK_METHOD3(glGetTexParameteriv, void(GLenum, GLenum, GLint *));
  MOCK_METHOD2(glHint, void(GLenum, GLenum));
  MOCK_METHOD1(glIndexMask, void(GLuint));
  MOCK_METHOD1(glIndexd, void(GLdouble));
  MOCK_METHOD1(glIndexdv, void(const GLdouble *));
  MOCK_METHOD1(glIndexf, void(GLfloat));
  MOCK_METHOD1(glIndexfv, void(const GLfloat *));
  MOCK_METHOD1(glIndexi, void(GLint));
  MOCK_METHOD1(glIndexiv, void(const GLint *));
  MOCK_METHOD1(glIndexs, void(GLshort));
  MOCK_METHOD1(glIndexsv, void(const GLshort *));
  MOCK_METHOD0(glInitNames, void(void));
  MOCK_METHOD1(glIsEnabled, GLboolean(GLenum));
  MOCK_METHOD1(glIsList, GLboolean(GLuint));
  MOCK_METHOD2(glLightModelf, void(GLenum, GLfloat));
  MOCK_METHOD2(glLightModelfv, void(GLenum, const GLfloat *));
  MOCK_METHOD2(glLightModeli, void(GLenum, GLint));
  MOCK_METHOD2(glLightModeliv, void(GLenum, const GLint *));
  MOCK_METHOD3(glLightf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD3(glLightfv, void(GLenum, GLenum, const GLfloat *));
  MOCK_METHOD3(glLighti, void(GLenum, GLenum, GLint));
  MOCK_METHOD3(glLightiv, void(GLenum, GLenum, const GLint *));
  MOCK_METHOD2(glLineStipple, void(GLint, GLushort));
  MOCK_METHOD1(glLineWidth, void(GLfloat));
  MOCK_METHOD1(glListBase, void(GLuint));
  MOCK_METHOD0(glLoadIdentity, void(void));
  MOCK_METHOD1(glLoadMatrixd, void(const GLdouble *));
  MOCK_METHOD1(glLoadMatrixf, void(const GLfloat *));
  MOCK_METHOD1(glLoadName, void(GLuint));
  MOCK_METHOD1(glLogicOp, void(GLenum));
  MOCK_METHOD6(glMap1d, void(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *));
  MOCK_METHOD6(glMap1f, void(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *));
  MOCK_METHOD10(glMap2d, void(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *));
  MOCK_METHOD10(glMap2f, void(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *));
  MOCK_METHOD3(glMapGrid1d, void(GLint, GLdouble, GLdouble));
  MOCK_METHOD3(glMapGrid1f, void(GLint, GLfloat, GLfloat));
  MOCK_METHOD6(glMapGrid2d, void(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble));
  MOCK_METHOD6(glMapGrid2f, void(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat));
  MOCK_METHOD3(glMaterialf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD3(glMaterialfv, void(GLenum, GLenum, const GLfloat *));
  MOCK_METHOD3(glMateriali, void(GLenum, GLenum, GLint));
  MOCK_METHOD3(glMaterialiv, void(GLenum, GLenum, const GLint *));
  MOCK_METHOD1(glMatrixMode, void(GLenum));
  MOCK_METHOD1(glMultMatrixd, void(const GLdouble *));
  MOCK_METHOD1(glMultMatrixf, void(const GLfloat *));
  MOCK_METHOD2(glNewList, void(GLuint, GLenum));
  MOCK_METHOD3(glNormal3b, void(GLbyte, GLbyte, GLbyte));
  MOCK_METHOD1(glNormal3bv, void(const GLbyte *));
  MOCK_METHOD3(glNormal3d, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glNormal3dv, void(const GLdouble *));
  MOCK_METHOD3(glNormal3f, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glNormal3fv, void(const GLfloat *));
  MOCK_METHOD3(glNormal3i, void(GLint, GLint, GLint));
  MOCK_METHOD1(glNormal3iv, void(const GLint *));
  MOCK_METHOD3(glNormal3s, void(GLshort, GLshort, GLshort));
  MOCK_METHOD1(glNormal3sv, void(const GLshort *));
  MOCK_METHOD6(glOrtho, void(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glPassThrough, void(GLfloat));
  MOCK_METHOD3(glPixelMapfv, void(GLenum, GLsizei, const GLfloat *));
  MOCK_METHOD3(glPixelMapuiv, void(GLenum, GLsizei, const GLuint *));
  MOCK_METHOD3(glPixelMapusv, void(GLenum, GLsizei, const GLushort *));
  MOCK_METHOD2(glPixelStoref, void(GLenum, GLfloat));
  MOCK_METHOD2(glPixelStorei, void(GLenum, GLint));
  MOCK_METHOD2(glPixelTransferf, void(GLenum, GLfloat));
  MOCK_METHOD2(glPixelTransferi, void(GLenum, GLint));
  MOCK_METHOD2(glPixelZoom, void(GLfloat, GLfloat));
  MOCK_METHOD1(glPointSize, void(GLfloat));
  MOCK_METHOD2(glPolygonMode, void(GLenum, GLenum));
  MOCK_METHOD1(glPolygonStipple, void(const GLubyte *));
  MOCK_METHOD0(glPopAttrib, void(void));
  MOCK_METHOD0(glPopMatrix, void(void));
  MOCK_METHOD0(glPopName, void(void));
  MOCK_METHOD1(glPushAttrib, void(GLbitfield));
  MOCK_METHOD0(glPushMatrix, void(void));
  MOCK_METHOD1(glPushName, void(GLuint));
  MOCK_METHOD2(glRasterPos2d, void(GLdouble, GLdouble));
  MOCK_METHOD1(glRasterPos2dv, void(const GLdouble *));
  MOCK_METHOD2(glRasterPos2f, void(GLfloat, GLfloat));
  MOCK_METHOD1(glRasterPos2fv, void(const GLfloat *));
  MOCK_METHOD2(glRasterPos2i, void(GLint, GLint));
  MOCK_METHOD1(glRasterPos2iv, void(const GLint *));
  MOCK_METHOD2(glRasterPos2s, void(GLshort, GLshort));
  MOCK_METHOD1(glRasterPos2sv, void(const GLshort *));
  MOCK_METHOD3(glRasterPos3d, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glRasterPos3dv, void(const GLdouble *));
  MOCK_METHOD3(glRasterPos3f, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glRasterPos3fv, void(const GLfloat *));
  MOCK_METHOD3(glRasterPos3i, void(GLint, GLint, GLint));
  MOCK_METHOD1(glRasterPos3iv, void(const GLint *));
  MOCK_METHOD3(glRasterPos3s, void(GLshort, GLshort, GLshort));
  MOCK_METHOD1(glRasterPos3sv, void(const GLshort *));
  MOCK_METHOD4(glRasterPos4d, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glRasterPos4dv, void(const GLdouble *));
  MOCK_METHOD4(glRasterPos4f, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glRasterPos4fv, void(const GLfloat *));
  MOCK_METHOD4(glRasterPos4i, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD1(glRasterPos4iv, void(const GLint *));
  MOCK_METHOD4(glRasterPos4s, void(GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD1(glRasterPos4sv, void(const GLshort *));
  MOCK_METHOD1(glReadBuffer, void(GLenum));
  MOCK_METHOD7(glReadPixels, void(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *));
  MOCK_METHOD4(glRectd, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD2(glRectdv, void(const GLdouble *, const GLdouble *));
  MOCK_METHOD4(glRectf, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD2(glRectfv, void(const GLfloat *, const GLfloat *));
  MOCK_METHOD4(glRecti, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD2(glRectiv, void(const GLint *, const GLint *));
  MOCK_METHOD4(glRects, void(GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD2(glRectsv, void(const GLshort *, const GLshort *));
  MOCK_METHOD1(glRenderMode, GLint(GLenum));
  MOCK_METHOD4(glRotated, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD4(glRotatef, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD3(glScaled, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD3(glScalef, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD4(glScissor, void(GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD2(glSelectBuffer, void(GLsizei, GLuint *));
  MOCK_METHOD1(glShadeModel, void(GLenum));
  MOCK_METHOD3(glStencilFunc, void(GLenum, GLint, GLuint));
  MOCK_METHOD1(glStencilMask, void(GLuint));
  MOCK_METHOD3(glStencilOp, void(GLenum, GLenum, GLenum));
  MOCK_METHOD1(glTexCoord1d, void(GLdouble));
  MOCK_METHOD1(glTexCoord1dv, void(const GLdouble *));
  MOCK_METHOD1(glTexCoord1f, void(GLfloat));
  MOCK_METHOD1(glTexCoord1fv, void(const GLfloat *));
  MOCK_METHOD1(glTexCoord1i, void(GLint));
  MOCK_METHOD1(glTexCoord1iv, void(const GLint *));
  MOCK_METHOD1(glTexCoord1s, void(GLshort));
  MOCK_METHOD1(glTexCoord1sv, void(const GLshort *));
  MOCK_METHOD2(glTexCoord2d, void(GLdouble, GLdouble));
  MOCK_METHOD1(glTexCoord2dv, void(const GLdouble *));
  MOCK_METHOD2(glTexCoord2f, void(GLfloat, GLfloat));
  MOCK_METHOD1(glTexCoord2fv, void(const GLfloat *));
  MOCK_METHOD2(glTexCoord2i, void(GLint, GLint));
  MOCK_METHOD1(glTexCoord2iv, void(const GLint *));
  MOCK_METHOD2(glTexCoord2s, void(GLshort, GLshort));
  MOCK_METHOD1(glTexCoord2sv, void(const GLshort *));
  MOCK_METHOD3(glTexCoord3d, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glTexCoord3dv, void(const GLdouble *));
  MOCK_METHOD3(glTexCoord3f, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glTexCoord3fv, void(const GLfloat *));
  MOCK_METHOD3(glTexCoord3i, void(GLint, GLint, GLint));
  MOCK_METHOD1(glTexCoord3iv, void(const GLint *));
  MOCK_METHOD3(glTexCoord3s, void(GLshort, GLshort, GLshort));
  MOCK_METHOD1(glTexCoord3sv, void(const GLshort *));
  MOCK_METHOD4(glTexCoord4d, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glTexCoord4dv, void(const GLdouble *));
  MOCK_METHOD4(glTexCoord4f, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glTexCoord4fv, void(const GLfloat *));
  MOCK_METHOD4(glTexCoord4i, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD1(glTexCoord4iv, void(const GLint *));
  MOCK_METHOD4(glTexCoord4s, void(GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD1(glTexCoord4sv, void(const GLshort *));
  MOCK_METHOD3(glTexEnvf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD3(glTexEnvfv, void(GLenum, GLenum, const GLfloat *));
  MOCK_METHOD3(glTexEnvi, void(GLenum, GLenum, GLint));
  MOCK_METHOD3(glTexEnviv, void(GLenum, GLenum, const GLint *));
  MOCK_METHOD3(glTexGend, void(GLenum, GLenum, GLdouble));
  MOCK_METHOD3(glTexGendv, void(GLenum, GLenum, const GLdouble *));
  MOCK_METHOD3(glTexGenf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD3(glTexGenfv, void(GLenum, GLenum, const GLfloat *));
  MOCK_METHOD3(glTexGeni, void(GLenum, GLenum, GLint));
  MOCK_METHOD3(glTexGeniv, void(GLenum, GLenum, const GLint *));
  MOCK_METHOD8(glTexImage1D, void(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *));
  MOCK_METHOD9(glTexImage2D, void(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *));
  MOCK_METHOD3(glTexParameterf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD3(glTexParameterfv, void(GLenum, GLenum, const GLfloat *));
  MOCK_METHOD3(glTexParameteri, void(GLenum, GLenum, GLint));
  MOCK_METHOD3(glTexParameteriv, void(GLenum, GLenum, const GLint *));
  MOCK_METHOD3(glTranslated, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD3(glTranslatef, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD2(glVertex2d, void(GLdouble, GLdouble));
  MOCK_METHOD1(glVertex2dv, void(const GLdouble *));
  MOCK_METHOD2(glVertex2f, void(GLfloat, GLfloat));
  MOCK_METHOD1(glVertex2fv, void(const GLfloat *));
  MOCK_METHOD2(glVertex2i, void(GLint, GLint));
  MOCK_METHOD1(glVertex2iv, void(const GLint *));
  MOCK_METHOD2(glVertex2s, void(GLshort, GLshort));
  MOCK_METHOD1(glVertex2sv, void(const GLshort *));
  MOCK_METHOD3(glVertex3d, void(GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glVertex3dv, void(const GLdouble *));
  MOCK_METHOD3(glVertex3f, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glVertex3fv, void(const GLfloat *));
  MOCK_METHOD3(glVertex3i, void(GLint, GLint, GLint));
  MOCK_METHOD1(glVertex3iv, void(const GLint *));
  MOCK_METHOD3(glVertex3s, void(GLshort, GLshort, GLshort));
  MOCK_METHOD1(glVertex3sv, void(const GLshort *));
  MOCK_METHOD4(glVertex4d, void(GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(glVertex4dv, void(const GLdouble *));
  MOCK_METHOD4(glVertex4f, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD1(glVertex4fv, void(const GLfloat *));
  MOCK_METHOD4(glVertex4i, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD1(glVertex4iv, void(const GLint *));
  MOCK_METHOD4(glVertex4s, void(GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD1(glVertex4sv, void(const GLshort *));
  MOCK_METHOD4(glViewport, void(GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD3(glAreTexturesResident, GLboolean(GLsizei, const GLuint *, GLboolean *));
  MOCK_METHOD1(glArrayElement, void(GLint));
  MOCK_METHOD2(glBindTexture, void(GLenum, GLuint));
  MOCK_METHOD4(glColorPointer, void(GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD7(glCopyTexImage1D, void(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint));
  MOCK_METHOD8(glCopyTexImage2D, void(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint));
  MOCK_METHOD6(glCopyTexSubImage1D, void(GLenum, GLint, GLint, GLint, GLint, GLsizei));
  MOCK_METHOD8(glCopyTexSubImage2D, void(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD2(glDeleteTextures, void(GLsizei, const GLuint *));
  MOCK_METHOD1(glDisableClientState, void(GLenum));
  MOCK_METHOD3(glDrawArrays, void(GLenum, GLint, GLsizei));
  MOCK_METHOD4(glDrawElements, void(GLenum, GLsizei, GLenum, const GLvoid *));
  MOCK_METHOD2(glEdgeFlagPointer, void(GLsizei, const GLvoid *));
  MOCK_METHOD1(glEnableClientState, void(GLenum));
  MOCK_METHOD2(glGenTextures, void(GLsizei, GLuint *));
  MOCK_METHOD2(glGetPointerv, void(GLenum, GLvoid **));
  MOCK_METHOD3(glIndexPointer, void(GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD1(glIndexub, void(GLubyte));
  MOCK_METHOD1(glIndexubv, void(const GLubyte *));
  MOCK_METHOD3(glInterleavedArrays, void(GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD1(glIsTexture, GLboolean(GLuint));
  MOCK_METHOD3(glNormalPointer, void(GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD2(glPolygonOffset, void(GLfloat, GLfloat));
  MOCK_METHOD0(glPopClientAttrib, void(void));
  MOCK_METHOD3(glPrioritizeTextures, void(GLsizei, const GLuint *, const GLclampf *));
  MOCK_METHOD1(glPushClientAttrib, void(GLbitfield));
  MOCK_METHOD4(glTexCoordPointer, void(GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD7(glTexSubImage1D, void(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *));
  MOCK_METHOD9(glTexSubImage2D, void(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *));
  MOCK_METHOD4(glVertexPointer, void(GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD1(glClientActiveTexture, void(GLenum));
  MOCK_METHOD3(glFogCoordPointer, void(GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD4(glSecondaryColorPointer, void(GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD2(glBindBuffer, void(GLenum, GLuint));
  MOCK_METHOD2(glAttachShader, void(GLuint, GLuint));
  MOCK_METHOD3(glBindAttribLocation, void(GLuint, GLuint, const GLchar *));
  MOCK_METHOD2(glBlendEquationSeparate, void(GLenum, GLenum));
  MOCK_METHOD1(glCompileShader, void(GLuint));
  MOCK_METHOD0(glCreateProgram, GLuint(void));
  MOCK_METHOD1(glCreateShader, GLuint(GLenum));
  MOCK_METHOD1(glDeleteProgram, void(GLuint));
  MOCK_METHOD1(glDeleteShader, void(GLuint));
  MOCK_METHOD2(glDetachShader, void(GLuint, GLuint));
  MOCK_METHOD1(glDisableVertexAttribArray, void(GLuint));
  MOCK_METHOD2(glDrawBuffers, void(GLsizei, const GLenum *));
  MOCK_METHOD1(glEnableVertexAttribArray, void(GLuint));
  MOCK_METHOD7(glGetActiveAttrib, void(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *));
  MOCK_METHOD7(glGetActiveUniform, void(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *));
  MOCK_METHOD4(glGetAttachedShaders, void(GLuint, GLsizei, GLsizei *, GLuint *));
  MOCK_METHOD2(glGetAttribLocation, GLint(GLuint, const GLchar *));
  MOCK_METHOD4(glGetProgramInfoLog, void(GLuint, GLsizei, GLsizei *, GLchar *));
  MOCK_METHOD3(glGetProgramiv, void(GLuint, GLenum, GLint *));
  MOCK_METHOD4(glGetShaderInfoLog, void(GLuint, GLsizei, GLsizei *, GLchar *));
  MOCK_METHOD4(glGetShaderSource, void(GLuint, GLsizei, GLsizei *, GLchar *));
  MOCK_METHOD3(glGetShaderiv, void(GLuint, GLenum, GLint *));
  MOCK_METHOD2(glGetUniformLocation, GLint(GLuint, const GLchar *));
  MOCK_METHOD3(glGetUniformfv, void(GLuint, GLint, GLfloat *));
  MOCK_METHOD3(glGetUniformiv, void(GLuint, GLint, GLint *));
  MOCK_METHOD3(glGetVertexAttribPointerv, void(GLuint, GLenum, GLvoid **));
  MOCK_METHOD3(glGetVertexAttribdv, void(GLuint, GLenum, GLdouble *));
  MOCK_METHOD3(glGetVertexAttribfv, void(GLuint, GLenum, GLfloat *));
  MOCK_METHOD3(glGetVertexAttribiv, void(GLuint, GLenum, GLint *));
  MOCK_METHOD1(glIsProgram, GLboolean(GLuint));
  MOCK_METHOD1(glIsShader, GLboolean(GLuint));
  MOCK_METHOD1(glLinkProgram, void(GLuint));
  MOCK_METHOD4(glShaderSource, void(GLuint, GLsizei, const GLchar * const *, const GLint *));
  MOCK_METHOD4(glStencilFuncSeparate, void(GLenum, GLenum, GLint, GLuint));
  MOCK_METHOD2(glStencilMaskSeparate, void(GLenum, GLuint));
  MOCK_METHOD4(glStencilOpSeparate, void(GLenum, GLenum, GLenum, GLenum));
  MOCK_METHOD2(glUniform1f, void(GLint, GLfloat));
  MOCK_METHOD3(glUniform1fv, void(GLint, GLsizei, const GLfloat *));
  MOCK_METHOD2(glUniform1i, void(GLint, GLint));
  MOCK_METHOD3(glUniform1iv, void(GLint, GLsizei, const GLint *));
  MOCK_METHOD3(glUniform2f, void(GLint, GLfloat, GLfloat));
  MOCK_METHOD3(glUniform2fv, void(GLint, GLsizei, const GLfloat *));
  MOCK_METHOD3(glUniform2i, void(GLint, GLint, GLint));
  MOCK_METHOD3(glUniform2iv, void(GLint, GLsizei, const GLint *));
  MOCK_METHOD4(glUniform3f, void(GLint, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD3(glUniform3fv, void(GLint, GLsizei, const GLfloat *));
  MOCK_METHOD4(glUniform3i, void(GLint, GLint, GLint, GLint));
  MOCK_METHOD3(glUniform3iv, void(GLint, GLsizei, const GLint *));
  MOCK_METHOD5(glUniform4f, void(GLint, GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD3(glUniform4fv, void(GLint, GLsizei, const GLfloat *));
  MOCK_METHOD5(glUniform4i, void(GLint, GLint, GLint, GLint, GLint));
  MOCK_METHOD3(glUniform4iv, void(GLint, GLsizei, const GLint *));
  MOCK_METHOD4(glUniformMatrix2fv, void(GLint, GLsizei, GLboolean, const GLfloat *));
  MOCK_METHOD4(glUniformMatrix3fv, void(GLint, GLsizei, GLboolean, const GLfloat *));
  MOCK_METHOD4(glUniformMatrix4fv, void(GLint, GLsizei, GLboolean, const GLfloat *));
  MOCK_METHOD1(glUseProgram, void(GLuint));
  MOCK_METHOD1(glValidateProgram, void(GLuint));
  MOCK_METHOD2(glVertexAttrib1d, void(GLuint, GLdouble));
  MOCK_METHOD2(glVertexAttrib1dv, void(GLuint, const GLdouble *));
  MOCK_METHOD2(glVertexAttrib1f, void(GLuint, GLfloat));
  MOCK_METHOD2(glVertexAttrib1fv, void(GLuint, const GLfloat *));
  MOCK_METHOD2(glVertexAttrib1s, void(GLuint, GLshort));
  MOCK_METHOD2(glVertexAttrib1sv, void(GLuint, const GLshort *));
  MOCK_METHOD3(glVertexAttrib2d, void(GLuint, GLdouble, GLdouble));
  MOCK_METHOD2(glVertexAttrib2dv, void(GLuint, const GLdouble *));
  MOCK_METHOD3(glVertexAttrib2f, void(GLuint, GLfloat, GLfloat));
  MOCK_METHOD2(glVertexAttrib2fv, void(GLuint, const GLfloat *));
  MOCK_METHOD3(glVertexAttrib2s, void(GLuint, GLshort, GLshort));
  MOCK_METHOD2(glVertexAttrib2sv, void(GLuint, const GLshort *));
  MOCK_METHOD4(glVertexAttrib3d, void(GLuint, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD2(glVertexAttrib3dv, void(GLuint, const GLdouble *));
  MOCK_METHOD4(glVertexAttrib3f, void(GLuint, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD2(glVertexAttrib3fv, void(GLuint, const GLfloat *));
  MOCK_METHOD4(glVertexAttrib3s, void(GLuint, GLshort, GLshort, GLshort));
  MOCK_METHOD2(glVertexAttrib3sv, void(GLuint, const GLshort *));
  MOCK_METHOD2(glVertexAttrib4Nbv, void(GLuint, const GLbyte *));
  MOCK_METHOD2(glVertexAttrib4Niv, void(GLuint, const GLint *));
  MOCK_METHOD2(glVertexAttrib4Nsv, void(GLuint, const GLshort *));
  MOCK_METHOD5(glVertexAttrib4Nub, void(GLuint, GLubyte, GLubyte, GLubyte, GLubyte));
  MOCK_METHOD2(glVertexAttrib4Nubv, void(GLuint, const GLubyte *));
  MOCK_METHOD2(glVertexAttrib4Nuiv, void(GLuint, const GLuint *));
  MOCK_METHOD2(glVertexAttrib4Nusv, void(GLuint, const GLushort *));
  MOCK_METHOD2(glVertexAttrib4bv, void(GLuint, const GLbyte *));
  MOCK_METHOD5(glVertexAttrib4d, void(GLuint, GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD2(glVertexAttrib4dv, void(GLuint, const GLdouble *));
  MOCK_METHOD5(glVertexAttrib4f, void(GLuint, GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD2(glVertexAttrib4fv, void(GLuint, const GLfloat *));
  MOCK_METHOD2(glVertexAttrib4iv, void(GLuint, const GLint *));
  MOCK_METHOD5(glVertexAttrib4s, void(GLuint, GLshort, GLshort, GLshort, GLshort));
  MOCK_METHOD2(glVertexAttrib4sv, void(GLuint, const GLshort *));
  MOCK_METHOD2(glVertexAttrib4ubv, void(GLuint, const GLubyte *));
  MOCK_METHOD2(glVertexAttrib4uiv, void(GLuint, const GLuint *));
  MOCK_METHOD2(glVertexAttrib4usv, void(GLuint, const GLushort *));
  MOCK_METHOD6(glVertexAttribPointer, void(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *));
  MOCK_METHOD1(glPrimitiveRestartIndex, void(GLuint));
  MOCK_METHOD1(glBindVertexArray, void(GLuint));
  MOCK_METHOD4(glBindVertexBuffer, void(GLuint, GLuint, GLintptr, GLsizei));
  MOCK_METHOD2(glVertexAttribBinding, void(GLuint, GLuint));
  MOCK_METHOD5(glVertexAttribFormat, void(GLuint, GLint, GLenum, GLboolean, GLuint));
  MOCK_METHOD4(glVertexAttribIFormat, void(GLuint, GLint, GLenum, GLuint));
  MOCK_METHOD4(glVertexAttribLFormat, void(GLuint, GLint, GLenum, GLuint));
  MOCK_METHOD2(glVertexBindingDivisor, void(GLuint, GLuint));
  MOCK_METHOD1(glClientAttribDefaultEXT, void(GLbitfield));
  MOCK_METHOD2(glDisableClientStateiEXT, void(GLenum, GLuint));
  MOCK_METHOD5(glMultiTexCoordPointerEXT, void(GLenum, GLint, GLenum, GLsizei, const GLvoid *));
};

void InitDispatchTableGMock(DispatchTableGL &tbl);

REGAL_NAMESPACE_END

#endif // ! __REGAL_DISPATCH_GMOCK_H__
