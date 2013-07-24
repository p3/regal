/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.2 --api wgl 4.0 --api glx 4.0 --api cgl 1.4 --api egl 1.0 --outdir .
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

#if REGAL_SYS_EGL && REGAL_DRIVER && REGAL_STATIC_EGL

REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalDispatch.h"

#ifndef REGAL_NAMESPACE
#error REGAL_STATIC_EGL requires REGAL_NAMESPACE
#endif

#undef eglQuerySurfacePointerANGLE
#undef eglClientWaitSyncKHR
#undef eglCreateSyncKHR
#undef eglDestroySyncKHR
#undef eglGetSyncAttribKHR
#undef eglCreateImageKHR
#undef eglDestroyImageKHR
#undef eglLockSurfaceKHR
#undef eglUnlockSurfaceKHR
#undef eglStreamConsumerAcquireKHR
#undef eglStreamConsumerGLTextureExternalKHR
#undef eglStreamConsumerReleaseKHR
#undef eglCreateStreamFromFileDescriptorKHR
#undef eglGetStreamFileDescriptorKHR
#undef eglCreateStreamProducerSurfaceKHR
#undef eglWaitSyncKHR
#undef eglCreateDRMImageMESA
#undef eglExportDRMImageMESA
#undef eglCoverageMaskNV
#undef eglCoverageOperationNV
#undef eglPostSubBufferNV
#undef eglClientWaitSyncNV
#undef eglCreateFenceSyncNV
#undef eglDestroySyncNV
#undef eglFenceNV
#undef eglGetSyncAttribNV
#undef eglSignalSyncNV
#undef eglGetSystemTimeFrequencyNV
#undef eglGetSystemTimeNV
#undef eglChooseConfig
#undef eglCopyBuffers
#undef eglCreateContext
#undef eglCreatePbufferSurface
#undef eglCreatePixmapSurface
#undef eglCreateWindowSurface
#undef eglDestroyContext
#undef eglDestroySurface
#undef eglGetConfigAttrib
#undef eglGetConfigs
#undef eglGetCurrentContext
#undef eglGetCurrentDisplay
#undef eglGetCurrentSurface
#undef eglGetDisplay
#undef eglGetError
#undef eglGetProcAddress
#undef eglInitialize
#undef eglMakeCurrent
#undef eglQueryContext
#undef eglQueryString
#undef eglQuerySurface
#undef eglSwapBuffers
#undef eglTerminate
#undef eglWaitGL
#undef eglWaitNative
#undef eglBindTexImage
#undef eglReleaseTexImage
#undef eglBindAPI
#undef eglCreatePbufferFromClientBuffer
#undef eglQueryAPI
#undef eglReleaseThread
#undef eglSurfaceAttrib
#undef eglSwapInterval
#undef eglWaitClient

extern "C"
{
  extern EGLBoolean  REGAL_CALL eglQuerySurfacePointerANGLE(EGLDisplay dpy, EGLSurface surface, EGLint attribute, GLvoid **value);
  extern EGLint  REGAL_CALL eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR GLsync, EGLint flags, EGLTimeKHR timeout);
  extern EGLSyncKHR  REGAL_CALL eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR GLsync);
  extern EGLBoolean  REGAL_CALL eglGetSyncAttribKHR(EGLDisplay dpy, EGLSyncKHR GLsync, EGLint attribute, EGLint *value);
  extern EGLImageKHR  REGAL_CALL eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image);
  extern EGLBoolean  REGAL_CALL eglLockSurfaceKHR(EGLDisplay display, EGLSurface surface, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglUnlockSurfaceKHR(EGLDisplay display, EGLSurface surface);
  extern EGLBoolean  REGAL_CALL eglStreamConsumerAcquireKHR(EGLDisplay dpy, EGLStreamKHR stream);
  extern EGLBoolean  REGAL_CALL eglStreamConsumerGLTextureExternalKHR(EGLDisplay dpy, EGLStreamKHR stream);
  extern EGLBoolean  REGAL_CALL eglStreamConsumerReleaseKHR(EGLDisplay dpy, EGLStreamKHR stream);
  extern EGLStreamKHR  REGAL_CALL eglCreateStreamFromFileDescriptorKHR(EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor);
  extern EGLNativeFileDescriptorKHR  REGAL_CALL eglGetStreamFileDescriptorKHR(EGLDisplay dpy, EGLStreamKHR stream);
  extern EGLSurface  REGAL_CALL eglCreateStreamProducerSurfaceKHR(EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint *attrib_list);
  extern EGLint  REGAL_CALL eglWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR GLsync, EGLint flags);
  extern EGLImageKHR  REGAL_CALL eglCreateDRMImageMESA(EGLDisplay dpy, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglExportDRMImageMESA(EGLDisplay dpy, EGLImageKHR image, EGLint *name, EGLint *handle, EGLint *stride);
  extern void  REGAL_CALL eglCoverageMaskNV(GLboolean mask);
  extern void  REGAL_CALL eglCoverageOperationNV(GLenum operation);
  extern EGLBoolean  REGAL_CALL eglPostSubBufferNV(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height);
  extern EGLint  REGAL_CALL eglClientWaitSyncNV(EGLSyncNV GLsync, EGLint flags, EGLTimeNV timeout);
  extern EGLSyncNV  REGAL_CALL eglCreateFenceSyncNV(EGLDisplay dpy, EGLenum condition, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglDestroySyncNV(EGLSyncNV GLsync);
  extern EGLBoolean  REGAL_CALL eglFenceNV(EGLSyncNV GLsync);
  extern EGLBoolean  REGAL_CALL eglGetSyncAttribNV(EGLSyncNV GLsync, EGLint attribute, EGLint *value);
  extern EGLBoolean  REGAL_CALL eglSignalSyncNV(EGLSyncNV GLsync, EGLenum mode);
  extern EGLuint64NV  REGAL_CALL eglGetSystemTimeFrequencyNV(void);
  extern EGLuint64NV  REGAL_CALL eglGetSystemTimeNV(void);
  extern EGLBoolean  REGAL_CALL eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
  extern EGLBoolean  REGAL_CALL eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
  extern EGLContext  REGAL_CALL eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
  extern EGLSurface  REGAL_CALL eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
  extern EGLSurface  REGAL_CALL eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
  extern EGLSurface  REGAL_CALL eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
  extern EGLBoolean  REGAL_CALL eglDestroyContext(EGLDisplay dpy, EGLContext ctx);
  extern EGLBoolean  REGAL_CALL eglDestroySurface(EGLDisplay dpy, EGLSurface surface);
  extern EGLBoolean  REGAL_CALL eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
  extern EGLBoolean  REGAL_CALL eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
  extern EGLContext  REGAL_CALL eglGetCurrentContext(void);
  extern EGLDisplay  REGAL_CALL eglGetCurrentDisplay(void);
  extern EGLSurface  REGAL_CALL eglGetCurrentSurface(EGLint readdraw);
  extern EGLDisplay  REGAL_CALL eglGetDisplay(EGLNativeDisplayType display_id);
  extern EGLint  REGAL_CALL eglGetError(void);
  extern __eglMustCastToProperFunctionPointerType  REGAL_CALL eglGetProcAddress(const char *procname);
  extern EGLBoolean  REGAL_CALL eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor);
  extern EGLBoolean  REGAL_CALL eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
  extern EGLBoolean  REGAL_CALL eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
  extern const char * REGAL_CALL eglQueryString(EGLDisplay dpy, EGLint name);
  extern EGLBoolean  REGAL_CALL eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
  extern EGLBoolean  REGAL_CALL eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
  extern EGLBoolean  REGAL_CALL eglTerminate(EGLDisplay dpy);
  extern EGLBoolean  REGAL_CALL eglWaitGL(void);
  extern EGLBoolean  REGAL_CALL eglWaitNative(EGLint engine);
  extern EGLBoolean  REGAL_CALL eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
  extern EGLBoolean  REGAL_CALL eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
  extern EGLBoolean  REGAL_CALL eglBindAPI(EGLenum api);
  extern EGLSurface  REGAL_CALL eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
  extern EGLenum  REGAL_CALL eglQueryAPI(void);
  extern EGLBoolean  REGAL_CALL eglReleaseThread(void);
  extern EGLBoolean  REGAL_CALL eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
  extern EGLBoolean  REGAL_CALL eglSwapInterval(EGLDisplay dpy, EGLint interval);
  extern EGLBoolean  REGAL_CALL eglWaitClient(void);
}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

void InitDispatchTableStaticEGL(DispatchTableGlobal &tbl)
{
  // EGL global dispatch
  tbl.reglChooseConfig = ::eglChooseConfig;
  tbl.reglCopyBuffers = ::eglCopyBuffers;
  tbl.reglCreateContext = ::eglCreateContext;
  tbl.reglCreatePbufferSurface = ::eglCreatePbufferSurface;
  tbl.reglCreatePixmapSurface = ::eglCreatePixmapSurface;
  tbl.reglCreateWindowSurface = ::eglCreateWindowSurface;
  tbl.reglDestroyContext = ::eglDestroyContext;
  tbl.reglDestroySurface = ::eglDestroySurface;
  tbl.reglGetConfigAttrib = ::eglGetConfigAttrib;
  tbl.reglGetConfigs = ::eglGetConfigs;
  tbl.reglGetCurrentContext = ::eglGetCurrentContext;
  tbl.reglGetCurrentDisplay = ::eglGetCurrentDisplay;
  tbl.reglGetCurrentSurface = ::eglGetCurrentSurface;
  tbl.reglGetDisplay = ::eglGetDisplay;
  tbl.reglGetError = ::eglGetError;
  tbl.reglGetProcAddress = ::eglGetProcAddress;
  tbl.reglInitialize = ::eglInitialize;
  tbl.reglMakeCurrent = ::eglMakeCurrent;
  tbl.reglQueryContext = ::eglQueryContext;
  tbl.reglQueryString = ::eglQueryString;
  tbl.reglQuerySurface = ::eglQuerySurface;
  tbl.reglSwapBuffers = ::eglSwapBuffers;
  tbl.reglTerminate = ::eglTerminate;
  tbl.reglWaitGL = ::eglWaitGL;
  tbl.reglWaitNative = ::eglWaitNative;
  tbl.reglBindTexImage = ::eglBindTexImage;
  tbl.reglReleaseTexImage = ::eglReleaseTexImage;
  tbl.reglBindAPI = ::eglBindAPI;
  tbl.reglCreatePbufferFromClientBuffer = ::eglCreatePbufferFromClientBuffer;
  tbl.reglQueryAPI = ::eglQueryAPI;
  tbl.reglReleaseThread = ::eglReleaseThread;
  tbl.reglSurfaceAttrib = ::eglSurfaceAttrib;
  tbl.reglSwapInterval = ::eglSwapInterval;
  tbl.reglWaitClient = ::eglWaitClient;
#if REGAL_SYS_EMSCRIPTEN == 0
  tbl.reglQuerySurfacePointerANGLE = ::eglQuerySurfacePointerANGLE;
  tbl.reglClientWaitSyncKHR = ::eglClientWaitSyncKHR;
  tbl.reglCreateSyncKHR = ::eglCreateSyncKHR;
  tbl.reglDestroySyncKHR = ::eglDestroySyncKHR;
  tbl.reglGetSyncAttribKHR = ::eglGetSyncAttribKHR;
  tbl.reglCreateImageKHR = ::eglCreateImageKHR;
  tbl.reglDestroyImageKHR = ::eglDestroyImageKHR;
  tbl.reglLockSurfaceKHR = ::eglLockSurfaceKHR;
  tbl.reglUnlockSurfaceKHR = ::eglUnlockSurfaceKHR;
  tbl.reglStreamConsumerAcquireKHR = ::eglStreamConsumerAcquireKHR;
  tbl.reglStreamConsumerGLTextureExternalKHR = ::eglStreamConsumerGLTextureExternalKHR;
  tbl.reglStreamConsumerReleaseKHR = ::eglStreamConsumerReleaseKHR;
  tbl.reglCreateStreamFromFileDescriptorKHR = ::eglCreateStreamFromFileDescriptorKHR;
  tbl.reglGetStreamFileDescriptorKHR = ::eglGetStreamFileDescriptorKHR;
  tbl.reglCreateStreamProducerSurfaceKHR = ::eglCreateStreamProducerSurfaceKHR;
  tbl.reglWaitSyncKHR = ::eglWaitSyncKHR;
  tbl.reglCreateDRMImageMESA = ::eglCreateDRMImageMESA;
  tbl.reglExportDRMImageMESA = ::eglExportDRMImageMESA;
  tbl.reglCoverageMaskNV = ::eglCoverageMaskNV;
  tbl.reglCoverageOperationNV = ::eglCoverageOperationNV;
  tbl.reglPostSubBufferNV = ::eglPostSubBufferNV;
  tbl.reglClientWaitSyncNV = ::eglClientWaitSyncNV;
  tbl.reglCreateFenceSyncNV = ::eglCreateFenceSyncNV;
  tbl.reglDestroySyncNV = ::eglDestroySyncNV;
  tbl.reglFenceNV = ::eglFenceNV;
  tbl.reglGetSyncAttribNV = ::eglGetSyncAttribNV;
  tbl.reglSignalSyncNV = ::eglSignalSyncNV;
  tbl.reglGetSystemTimeFrequencyNV = ::eglGetSystemTimeFrequencyNV;
  tbl.reglGetSystemTimeNV = ::eglGetSystemTimeNV;
#endif // REGAL_SYS_EMSCRIPTEN == 0

}

REGAL_NAMESPACE_END

#endif
