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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_DRIVER && REGAL_STATIC_ES2

REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalLog.h"
#include "RegalBreak.h"
#include "RegalPush.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"
#include "RegalContext.h"

#ifndef REGAL_NAMESPACE
#error REGAL_STATIC_ES2 requires REGAL_NAMESPACE
#endif

#undef glBlendFunc
#undef glClear
#undef glClearColor
#undef glClearStencil
#undef glColorMask
#undef glCullFace
#undef glDepthFunc
#undef glDepthMask
#undef glDisable
#undef glEnable
#undef glFinish
#undef glFlush
#undef glFrontFace
#undef glGetBooleanv
#undef glGetError
#undef glGetFloatv
#undef glGetIntegerv
#undef glGetString
#undef glGetTexParameterfv
#undef glGetTexParameteriv
#undef glHint
#undef glIsEnabled
#undef glLineWidth
#undef glPixelStorei
#undef glReadPixels
#undef glScissor
#undef glStencilFunc
#undef glStencilMask
#undef glStencilOp
#undef glTexImage2D
#undef glTexParameterf
#undef glTexParameterfv
#undef glTexParameteri
#undef glTexParameteriv
#undef glViewport
#undef glBindTexture
#undef glCopyTexImage2D
#undef glCopyTexSubImage2D
#undef glDeleteTextures
#undef glDrawArrays
#undef glDrawElements
#undef glGenTextures
#undef glIsTexture
#undef glPolygonOffset
#undef glTexSubImage2D
#undef glBlendColor
#undef glBlendEquation
#undef glActiveTexture
#undef glCompressedTexImage2D
#undef glCompressedTexSubImage2D
#undef glSampleCoverage
#undef glBlendFuncSeparate
#undef glBindBuffer
#undef glBufferData
#undef glBufferSubData
#undef glDeleteBuffers
#undef glGenBuffers
#undef glGetBufferParameteriv
#undef glIsBuffer
#undef glAttachShader
#undef glBindAttribLocation
#undef glBlendEquationSeparate
#undef glCompileShader
#undef glCreateProgram
#undef glCreateShader
#undef glDeleteProgram
#undef glDeleteShader
#undef glDetachShader
#undef glDisableVertexAttribArray
#undef glEnableVertexAttribArray
#undef glGetActiveAttrib
#undef glGetActiveUniform
#undef glGetAttachedShaders
#undef glGetAttribLocation
#undef glGetProgramInfoLog
#undef glGetProgramiv
#undef glGetShaderInfoLog
#undef glGetShaderSource
#undef glGetShaderiv
#undef glGetUniformLocation
#undef glGetUniformfv
#undef glGetUniformiv
#undef glGetVertexAttribPointerv
#undef glGetVertexAttribfv
#undef glGetVertexAttribiv
#undef glIsProgram
#undef glIsShader
#undef glLinkProgram
#undef glShaderSource
#undef glStencilFuncSeparate
#undef glStencilMaskSeparate
#undef glStencilOpSeparate
#undef glUniform1f
#undef glUniform1fv
#undef glUniform1i
#undef glUniform1iv
#undef glUniform2f
#undef glUniform2fv
#undef glUniform2i
#undef glUniform2iv
#undef glUniform3f
#undef glUniform3fv
#undef glUniform3i
#undef glUniform3iv
#undef glUniform4f
#undef glUniform4fv
#undef glUniform4i
#undef glUniform4iv
#undef glUniformMatrix2fv
#undef glUniformMatrix3fv
#undef glUniformMatrix4fv
#undef glUseProgram
#undef glValidateProgram
#undef glVertexAttrib1f
#undef glVertexAttrib1fv
#undef glVertexAttrib2f
#undef glVertexAttrib2fv
#undef glVertexAttrib3f
#undef glVertexAttrib3fv
#undef glVertexAttrib4f
#undef glVertexAttrib4fv
#undef glVertexAttribPointer
#undef glClearDepthf
#undef glDepthRangef
#undef glGetShaderPrecisionFormat
#undef glReleaseShaderCompiler
#undef glShaderBinary
#undef glBindFramebuffer
#undef glBindRenderbuffer
#undef glCheckFramebufferStatus
#undef glDeleteFramebuffers
#undef glDeleteRenderbuffers
#undef glFramebufferRenderbuffer
#undef glFramebufferTexture2D
#undef glGenFramebuffers
#undef glGenRenderbuffers
#undef glGenerateMipmap
#undef glGetFramebufferAttachmentParameteriv
#undef glGetRenderbufferParameteriv
#undef glIsFramebuffer
#undef glIsRenderbuffer
#undef glRenderbufferStorage

extern "C"
{
  extern void  REGAL_CALL glBlendFunc(GLenum sfactor, GLenum dfactor);
  extern void  REGAL_CALL glClear(GLbitfield mask);
  extern void  REGAL_CALL glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  extern void  REGAL_CALL glClearStencil(GLint s);
  extern void  REGAL_CALL glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  extern void  REGAL_CALL glCullFace(GLenum mode);
  extern void  REGAL_CALL glDepthFunc(GLenum func);
  extern void  REGAL_CALL glDepthMask(GLboolean flag);
  extern void  REGAL_CALL glDisable(GLenum cap);
  extern void  REGAL_CALL glEnable(GLenum cap);
  extern void  REGAL_CALL glFinish(void);
  extern void  REGAL_CALL glFlush(void);
  extern void  REGAL_CALL glFrontFace(GLenum mode);
  extern void  REGAL_CALL glGetBooleanv(GLenum pname, GLboolean *params);
  extern GLenum  REGAL_CALL glGetError(void);
  extern void  REGAL_CALL glGetFloatv(GLenum pname, GLfloat *params);
  extern void  REGAL_CALL glGetIntegerv(GLenum pname, GLint *params);
  extern const GLubyte * REGAL_CALL glGetString(GLenum name);
  extern void  REGAL_CALL glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
  extern void  REGAL_CALL glGetTexParameteriv(GLenum target, GLenum pname, GLint *params);
  extern void  REGAL_CALL glHint(GLenum target, GLenum mode);
  extern GLboolean  REGAL_CALL glIsEnabled(GLenum cap);
  extern void  REGAL_CALL glLineWidth(GLfloat width);
  extern void  REGAL_CALL glPixelStorei(GLenum pname, GLint param);
  extern void  REGAL_CALL glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
  extern void  REGAL_CALL glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
  extern void  REGAL_CALL glStencilFunc(GLenum func, GLint ref, GLuint mask);
  extern void  REGAL_CALL glStencilMask(GLuint mask);
  extern void  REGAL_CALL glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
  extern void  REGAL_CALL glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  extern void  REGAL_CALL glTexParameterf(GLenum target, GLenum pname, GLfloat param);
  extern void  REGAL_CALL glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
  extern void  REGAL_CALL glTexParameteri(GLenum target, GLenum pname, GLint param);
  extern void  REGAL_CALL glTexParameteriv(GLenum target, GLenum pname, const GLint *params);
  extern void  REGAL_CALL glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
  extern void  REGAL_CALL glBindTexture(GLenum target, GLuint texture);
  extern void  REGAL_CALL glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
  extern void  REGAL_CALL glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  extern void  REGAL_CALL glDeleteTextures(GLsizei n, const GLuint *textures);
  extern void  REGAL_CALL glDrawArrays(GLenum mode, GLint first, GLsizei count);
  extern void  REGAL_CALL glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
  extern void  REGAL_CALL glGenTextures(GLsizei n, GLuint *textures);
  extern GLboolean  REGAL_CALL glIsTexture(GLuint texture);
  extern void  REGAL_CALL glPolygonOffset(GLfloat factor, GLfloat units);
  extern void  REGAL_CALL glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
  extern void  REGAL_CALL glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  extern void  REGAL_CALL glBlendEquation(GLenum mode);
  extern void  REGAL_CALL glActiveTexture(GLenum texture);
  extern void  REGAL_CALL glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
  extern void  REGAL_CALL glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
  extern void  REGAL_CALL glSampleCoverage(GLclampf value, GLboolean invert);
  extern void  REGAL_CALL glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
  extern void  REGAL_CALL glBindBuffer(GLenum target, GLuint buffer);
  extern void  REGAL_CALL glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
  extern void  REGAL_CALL glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
  extern void  REGAL_CALL glDeleteBuffers(GLsizei n, const GLuint *buffers);
  extern void  REGAL_CALL glGenBuffers(GLsizei n, GLuint *buffers);
  extern void  REGAL_CALL glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
  extern GLboolean  REGAL_CALL glIsBuffer(GLuint buffer);
  extern void  REGAL_CALL glAttachShader(GLuint program, GLuint shader);
  extern void  REGAL_CALL glBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
  extern void  REGAL_CALL glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
  extern void  REGAL_CALL glCompileShader(GLuint shader);
  extern GLuint  REGAL_CALL glCreateProgram(void);
  extern GLuint  REGAL_CALL glCreateShader(GLenum type);
  extern void  REGAL_CALL glDeleteProgram(GLuint program);
  extern void  REGAL_CALL glDeleteShader(GLuint shader);
  extern void  REGAL_CALL glDetachShader(GLuint program, GLuint shader);
  extern void  REGAL_CALL glDisableVertexAttribArray(GLuint index);
  extern void  REGAL_CALL glEnableVertexAttribArray(GLuint index);
  extern void  REGAL_CALL glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  extern void  REGAL_CALL glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  extern void  REGAL_CALL glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
  extern GLint  REGAL_CALL glGetAttribLocation(GLuint program, const GLchar *name);
  extern void  REGAL_CALL glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
  extern void  REGAL_CALL glGetProgramiv(GLuint program, GLenum pname, GLint *params);
  extern void  REGAL_CALL glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
  extern void  REGAL_CALL glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
  extern void  REGAL_CALL glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
  extern GLint  REGAL_CALL glGetUniformLocation(GLuint program, const GLchar *name);
  extern void  REGAL_CALL glGetUniformfv(GLuint program, GLint location, GLfloat *params);
  extern void  REGAL_CALL glGetUniformiv(GLuint program, GLint location, GLint *params);
  extern void  REGAL_CALL glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer);
  extern void  REGAL_CALL glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
  extern void  REGAL_CALL glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
  extern GLboolean  REGAL_CALL glIsProgram(GLuint program);
  extern GLboolean  REGAL_CALL glIsShader(GLuint shader);
  extern void  REGAL_CALL glLinkProgram(GLuint program);
  extern void  REGAL_CALL glShaderSource(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length);
  extern void  REGAL_CALL glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
  extern void  REGAL_CALL glStencilMaskSeparate(GLenum face, GLuint mask);
  extern void  REGAL_CALL glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
  extern void  REGAL_CALL glUniform1f(GLint location, GLfloat v0);
  extern void  REGAL_CALL glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
  extern void  REGAL_CALL glUniform1i(GLint location, GLint v0);
  extern void  REGAL_CALL glUniform1iv(GLint location, GLsizei count, const GLint *value);
  extern void  REGAL_CALL glUniform2f(GLint location, GLfloat v0, GLfloat v1);
  extern void  REGAL_CALL glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
  extern void  REGAL_CALL glUniform2i(GLint location, GLint v0, GLint v1);
  extern void  REGAL_CALL glUniform2iv(GLint location, GLsizei count, const GLint *value);
  extern void  REGAL_CALL glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
  extern void  REGAL_CALL glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
  extern void  REGAL_CALL glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
  extern void  REGAL_CALL glUniform3iv(GLint location, GLsizei count, const GLint *value);
  extern void  REGAL_CALL glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
  extern void  REGAL_CALL glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
  extern void  REGAL_CALL glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
  extern void  REGAL_CALL glUniform4iv(GLint location, GLsizei count, const GLint *value);
  extern void  REGAL_CALL glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  extern void  REGAL_CALL glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  extern void  REGAL_CALL glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  extern void  REGAL_CALL glUseProgram(GLuint program);
  extern void  REGAL_CALL glValidateProgram(GLuint program);
  extern void  REGAL_CALL glVertexAttrib1f(GLuint index, GLfloat x);
  extern void  REGAL_CALL glVertexAttrib1fv(GLuint index, const GLfloat *v);
  extern void  REGAL_CALL glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
  extern void  REGAL_CALL glVertexAttrib2fv(GLuint index, const GLfloat *v);
  extern void  REGAL_CALL glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
  extern void  REGAL_CALL glVertexAttrib3fv(GLuint index, const GLfloat *v);
  extern void  REGAL_CALL glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  extern void  REGAL_CALL glVertexAttrib4fv(GLuint index, const GLfloat *v);
  extern void  REGAL_CALL glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
  extern void  REGAL_CALL glClearDepthf(GLclampf d);
  extern void  REGAL_CALL glDepthRangef(GLclampf n, GLclampf f);
  extern void  REGAL_CALL glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
  extern void  REGAL_CALL glReleaseShaderCompiler(void);
  extern void  REGAL_CALL glShaderBinary(GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
  extern void  REGAL_CALL glBindFramebuffer(GLenum target, GLuint framebuffer);
  extern void  REGAL_CALL glBindRenderbuffer(GLenum target, GLuint renderbuffer);
  extern GLenum  REGAL_CALL glCheckFramebufferStatus(GLenum target);
  extern void  REGAL_CALL glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
  extern void  REGAL_CALL glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
  extern void  REGAL_CALL glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
  extern void  REGAL_CALL glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
  extern void  REGAL_CALL glGenFramebuffers(GLsizei n, GLuint *framebuffers);
  extern void  REGAL_CALL glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
  extern void  REGAL_CALL glGenerateMipmap(GLenum target);
  extern void  REGAL_CALL glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
  extern void  REGAL_CALL glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);
  extern GLboolean  REGAL_CALL glIsFramebuffer(GLuint framebuffer);
  extern GLboolean  REGAL_CALL glIsRenderbuffer(GLuint renderbuffer);
  extern void  REGAL_CALL glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

void InitDispatchTableStaticES2(DispatchTableGL &tbl)
{
  // OpenGL ES 2.0 only

  tbl.rglBlendFunc = glBlendFunc;
  tbl.rglClear = glClear;
  tbl.rglClearColor = glClearColor;
  tbl.rglClearStencil = glClearStencil;
  tbl.rglColorMask = glColorMask;
  tbl.rglCullFace = glCullFace;
  tbl.rglDepthFunc = glDepthFunc;
  tbl.rglDepthMask = glDepthMask;
  tbl.rglDisable = glDisable;
  tbl.rglEnable = glEnable;
  tbl.rglFinish = glFinish;
  tbl.rglFlush = glFlush;
  tbl.rglFrontFace = glFrontFace;
  tbl.rglGetBooleanv = glGetBooleanv;
  tbl.rglGetError = glGetError;
  tbl.rglGetFloatv = glGetFloatv;
  tbl.rglGetIntegerv = glGetIntegerv;
  tbl.rglGetString = glGetString;
  tbl.rglGetTexParameterfv = glGetTexParameterfv;
  tbl.rglGetTexParameteriv = glGetTexParameteriv;
  tbl.rglHint = glHint;
  tbl.rglIsEnabled = glIsEnabled;
  tbl.rglLineWidth = glLineWidth;
  tbl.rglPixelStorei = glPixelStorei;
  tbl.rglReadPixels = glReadPixels;
  tbl.rglScissor = glScissor;
  tbl.rglStencilFunc = glStencilFunc;
  tbl.rglStencilMask = glStencilMask;
  tbl.rglStencilOp = glStencilOp;
  tbl.rglTexImage2D = glTexImage2D;
  tbl.rglTexParameterf = glTexParameterf;
  tbl.rglTexParameterfv = glTexParameterfv;
  tbl.rglTexParameteri = glTexParameteri;
  tbl.rglTexParameteriv = glTexParameteriv;
  tbl.rglViewport = glViewport;
  tbl.rglBindTexture = glBindTexture;
  tbl.rglCopyTexImage2D = glCopyTexImage2D;
  tbl.rglCopyTexSubImage2D = glCopyTexSubImage2D;
  tbl.rglDeleteTextures = glDeleteTextures;
  tbl.rglDrawArrays = glDrawArrays;
  tbl.rglDrawElements = glDrawElements;
  tbl.rglGenTextures = glGenTextures;
  tbl.rglIsTexture = glIsTexture;
  tbl.rglPolygonOffset = glPolygonOffset;
  tbl.rglTexSubImage2D = glTexSubImage2D;
  tbl.rglBlendColor = glBlendColor;
  tbl.rglBlendEquation = glBlendEquation;
  tbl.rglActiveTexture = glActiveTexture;
  tbl.rglCompressedTexImage2D = glCompressedTexImage2D;
  tbl.rglCompressedTexSubImage2D = glCompressedTexSubImage2D;
  tbl.rglSampleCoverage = glSampleCoverage;
  tbl.rglBlendFuncSeparate = glBlendFuncSeparate;
  tbl.rglBindBuffer = glBindBuffer;
  tbl.rglBufferData = glBufferData;
  tbl.rglBufferSubData = glBufferSubData;
  tbl.rglDeleteBuffers = glDeleteBuffers;
  tbl.rglGenBuffers = glGenBuffers;
  tbl.rglGetBufferParameteriv = glGetBufferParameteriv;
  tbl.rglIsBuffer = glIsBuffer;
  tbl.rglAttachShader = glAttachShader;
  tbl.rglBindAttribLocation = glBindAttribLocation;
  tbl.rglBlendEquationSeparate = glBlendEquationSeparate;
  tbl.rglCompileShader = glCompileShader;
  tbl.rglCreateProgram = glCreateProgram;
  tbl.rglCreateShader = glCreateShader;
  tbl.rglDeleteProgram = glDeleteProgram;
  tbl.rglDeleteShader = glDeleteShader;
  tbl.rglDetachShader = glDetachShader;
  tbl.rglDisableVertexAttribArray = glDisableVertexAttribArray;
  tbl.rglEnableVertexAttribArray = glEnableVertexAttribArray;
  tbl.rglGetActiveAttrib = glGetActiveAttrib;
  tbl.rglGetActiveUniform = glGetActiveUniform;
  tbl.rglGetAttachedShaders = glGetAttachedShaders;
  tbl.rglGetAttribLocation = glGetAttribLocation;
  tbl.rglGetProgramInfoLog = glGetProgramInfoLog;
  tbl.rglGetProgramiv = glGetProgramiv;
  tbl.rglGetShaderInfoLog = glGetShaderInfoLog;
  tbl.rglGetShaderSource = glGetShaderSource;
  tbl.rglGetShaderiv = glGetShaderiv;
  tbl.rglGetUniformLocation = glGetUniformLocation;
  tbl.rglGetUniformfv = glGetUniformfv;
  tbl.rglGetUniformiv = glGetUniformiv;
  tbl.rglGetVertexAttribPointerv = glGetVertexAttribPointerv;
  tbl.rglGetVertexAttribfv = glGetVertexAttribfv;
  tbl.rglGetVertexAttribiv = glGetVertexAttribiv;
  tbl.rglIsProgram = glIsProgram;
  tbl.rglIsShader = glIsShader;
  tbl.rglLinkProgram = glLinkProgram;
  tbl.rglShaderSource = glShaderSource;
  tbl.rglStencilFuncSeparate = glStencilFuncSeparate;
  tbl.rglStencilMaskSeparate = glStencilMaskSeparate;
  tbl.rglStencilOpSeparate = glStencilOpSeparate;
  tbl.rglUniform1f = glUniform1f;
  tbl.rglUniform1fv = glUniform1fv;
  tbl.rglUniform1i = glUniform1i;
  tbl.rglUniform1iv = glUniform1iv;
  tbl.rglUniform2f = glUniform2f;
  tbl.rglUniform2fv = glUniform2fv;
  tbl.rglUniform2i = glUniform2i;
  tbl.rglUniform2iv = glUniform2iv;
  tbl.rglUniform3f = glUniform3f;
  tbl.rglUniform3fv = glUniform3fv;
  tbl.rglUniform3i = glUniform3i;
  tbl.rglUniform3iv = glUniform3iv;
  tbl.rglUniform4f = glUniform4f;
  tbl.rglUniform4fv = glUniform4fv;
  tbl.rglUniform4i = glUniform4i;
  tbl.rglUniform4iv = glUniform4iv;
  tbl.rglUniformMatrix2fv = glUniformMatrix2fv;
  tbl.rglUniformMatrix3fv = glUniformMatrix3fv;
  tbl.rglUniformMatrix4fv = glUniformMatrix4fv;
  tbl.rglUseProgram = glUseProgram;
  tbl.rglValidateProgram = glValidateProgram;
  tbl.rglVertexAttrib1f = glVertexAttrib1f;
  tbl.rglVertexAttrib1fv = glVertexAttrib1fv;
  tbl.rglVertexAttrib2f = glVertexAttrib2f;
  tbl.rglVertexAttrib2fv = glVertexAttrib2fv;
  tbl.rglVertexAttrib3f = glVertexAttrib3f;
  tbl.rglVertexAttrib3fv = glVertexAttrib3fv;
  tbl.rglVertexAttrib4f = glVertexAttrib4f;
  tbl.rglVertexAttrib4fv = glVertexAttrib4fv;
  tbl.rglVertexAttribPointer = glVertexAttribPointer;
  tbl.rglClearDepthf = glClearDepthf;
  tbl.rglDepthRangef = glDepthRangef;
  tbl.rglGetShaderPrecisionFormat = glGetShaderPrecisionFormat;
  tbl.rglReleaseShaderCompiler = glReleaseShaderCompiler;
  tbl.rglShaderBinary = glShaderBinary;
  tbl.rglBindFramebuffer = glBindFramebuffer;
  tbl.rglBindRenderbuffer = glBindRenderbuffer;
  tbl.rglCheckFramebufferStatus = glCheckFramebufferStatus;
  tbl.rglDeleteFramebuffers = glDeleteFramebuffers;
  tbl.rglDeleteRenderbuffers = glDeleteRenderbuffers;
  tbl.rglFramebufferRenderbuffer = glFramebufferRenderbuffer;
  tbl.rglFramebufferTexture2D = glFramebufferTexture2D;
  tbl.rglGenFramebuffers = glGenFramebuffers;
  tbl.rglGenRenderbuffers = glGenRenderbuffers;
  tbl.rglGenerateMipmap = glGenerateMipmap;
  tbl.rglGetFramebufferAttachmentParameteriv = glGetFramebufferAttachmentParameteriv;
  tbl.rglGetRenderbufferParameteriv = glGetRenderbufferParameteriv;
  tbl.rglIsFramebuffer = glIsFramebuffer;
  tbl.rglIsRenderbuffer = glIsRenderbuffer;
  tbl.rglRenderbufferStorage = glRenderbufferStorage;

}

REGAL_NAMESPACE_END

#endif
