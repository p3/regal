
#ifndef _GLPROC_HPP_
#define _GLPROC_HPP_

#include "glimports.hpp"

#if defined(_WIN32)
extern HMODULE _libGlHandle;
#else
extern void * _libGlHandle;
#endif

void * _getPublicProcAddress(const char *procName);
void * _getPrivateProcAddress(const char *procName);


typedef EGLint (* PFN_EGLGETERROR)(void);
extern PFN_EGLGETERROR _eglGetError;

typedef EGLDisplay (* PFN_EGLGETDISPLAY)(EGLNativeDisplayType display_id);
extern PFN_EGLGETDISPLAY _eglGetDisplay;

typedef EGLBoolean (* PFN_EGLINITIALIZE)(EGLDisplay dpy, EGLint * major, EGLint * minor);
extern PFN_EGLINITIALIZE _eglInitialize;

typedef EGLBoolean (* PFN_EGLTERMINATE)(EGLDisplay dpy);
extern PFN_EGLTERMINATE _eglTerminate;

typedef const char * (* PFN_EGLQUERYSTRING)(EGLDisplay dpy, EGLint name);
extern PFN_EGLQUERYSTRING _eglQueryString;

typedef EGLBoolean (* PFN_EGLGETCONFIGS)(EGLDisplay dpy, EGLConfig * configs, EGLint config_size, EGLint * num_config);
extern PFN_EGLGETCONFIGS _eglGetConfigs;

typedef EGLBoolean (* PFN_EGLCHOOSECONFIG)(EGLDisplay dpy, const EGLint * attrib_list, EGLConfig * configs, EGLint config_size, EGLint * num_config);
extern PFN_EGLCHOOSECONFIG _eglChooseConfig;

typedef EGLBoolean (* PFN_EGLGETCONFIGATTRIB)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint * value);
extern PFN_EGLGETCONFIGATTRIB _eglGetConfigAttrib;

typedef EGLSurface (* PFN_EGLCREATEWINDOWSURFACE)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint * attrib_list);
extern PFN_EGLCREATEWINDOWSURFACE _eglCreateWindowSurface;

typedef EGLSurface (* PFN_EGLCREATEPBUFFERSURFACE)(EGLDisplay dpy, EGLConfig config, const EGLint * attrib_list);
extern PFN_EGLCREATEPBUFFERSURFACE _eglCreatePbufferSurface;

typedef EGLSurface (* PFN_EGLCREATEPIXMAPSURFACE)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint * attrib_list);
extern PFN_EGLCREATEPIXMAPSURFACE _eglCreatePixmapSurface;

typedef EGLBoolean (* PFN_EGLDESTROYSURFACE)(EGLDisplay dpy, EGLSurface surface);
extern PFN_EGLDESTROYSURFACE _eglDestroySurface;

typedef EGLBoolean (* PFN_EGLQUERYSURFACE)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint * value);
extern PFN_EGLQUERYSURFACE _eglQuerySurface;

typedef EGLBoolean (* PFN_EGLBINDAPI)(EGLenum api);
extern PFN_EGLBINDAPI _eglBindAPI;

typedef EGLenum (* PFN_EGLQUERYAPI)(void);
extern PFN_EGLQUERYAPI _eglQueryAPI;

typedef EGLBoolean (* PFN_EGLWAITCLIENT)(void);
extern PFN_EGLWAITCLIENT _eglWaitClient;

typedef EGLBoolean (* PFN_EGLRELEASETHREAD)(void);
extern PFN_EGLRELEASETHREAD _eglReleaseThread;

typedef EGLSurface (* PFN_EGLCREATEPBUFFERFROMCLIENTBUFFER)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint * attrib_list);
extern PFN_EGLCREATEPBUFFERFROMCLIENTBUFFER _eglCreatePbufferFromClientBuffer;

typedef EGLBoolean (* PFN_EGLSURFACEATTRIB)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
extern PFN_EGLSURFACEATTRIB _eglSurfaceAttrib;

typedef EGLBoolean (* PFN_EGLBINDTEXIMAGE)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
extern PFN_EGLBINDTEXIMAGE _eglBindTexImage;

typedef EGLBoolean (* PFN_EGLRELEASETEXIMAGE)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
extern PFN_EGLRELEASETEXIMAGE _eglReleaseTexImage;

typedef EGLBoolean (* PFN_EGLSWAPINTERVAL)(EGLDisplay dpy, EGLint interval);
extern PFN_EGLSWAPINTERVAL _eglSwapInterval;

typedef EGLContext (* PFN_EGLCREATECONTEXT)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint * attrib_list);
extern PFN_EGLCREATECONTEXT _eglCreateContext;

typedef EGLBoolean (* PFN_EGLDESTROYCONTEXT)(EGLDisplay dpy, EGLContext ctx);
extern PFN_EGLDESTROYCONTEXT _eglDestroyContext;

typedef EGLBoolean (* PFN_EGLMAKECURRENT)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
extern PFN_EGLMAKECURRENT _eglMakeCurrent;

typedef EGLContext (* PFN_EGLGETCURRENTCONTEXT)(void);
extern PFN_EGLGETCURRENTCONTEXT _eglGetCurrentContext;

typedef EGLSurface (* PFN_EGLGETCURRENTSURFACE)(EGLint readdraw);
extern PFN_EGLGETCURRENTSURFACE _eglGetCurrentSurface;

typedef EGLDisplay (* PFN_EGLGETCURRENTDISPLAY)(void);
extern PFN_EGLGETCURRENTDISPLAY _eglGetCurrentDisplay;

typedef EGLBoolean (* PFN_EGLQUERYCONTEXT)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint * value);
extern PFN_EGLQUERYCONTEXT _eglQueryContext;

typedef EGLBoolean (* PFN_EGLWAITGL)(void);
extern PFN_EGLWAITGL _eglWaitGL;

typedef EGLBoolean (* PFN_EGLWAITNATIVE)(EGLint engine);
extern PFN_EGLWAITNATIVE _eglWaitNative;

typedef EGLBoolean (* PFN_EGLSWAPBUFFERS)(EGLDisplay dpy, EGLSurface surface);
extern PFN_EGLSWAPBUFFERS _eglSwapBuffers;

typedef EGLBoolean (* PFN_EGLCOPYBUFFERS)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
extern PFN_EGLCOPYBUFFERS _eglCopyBuffers;

typedef __eglMustCastToProperFunctionPointerType (* PFN_EGLGETPROCADDRESS)(const char * procname);
extern PFN_EGLGETPROCADDRESS _eglGetProcAddress;

typedef EGLBoolean (* PFN_EGLLOCKSURFACEKHR)(EGLDisplay display, EGLSurface surface, const EGLint * attrib_list);
extern PFN_EGLLOCKSURFACEKHR _eglLockSurfaceKHR;

typedef EGLBoolean (* PFN_EGLUNLOCKSURFACEKHR)(EGLDisplay display, EGLSurface surface);
extern PFN_EGLUNLOCKSURFACEKHR _eglUnlockSurfaceKHR;

typedef EGLImageKHR (* PFN_EGLCREATEIMAGEKHR)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint * attrib_list);
extern PFN_EGLCREATEIMAGEKHR _eglCreateImageKHR;

typedef EGLBoolean (* PFN_EGLDESTROYIMAGEKHR)(EGLDisplay dpy, EGLImageKHR image);
extern PFN_EGLDESTROYIMAGEKHR _eglDestroyImageKHR;

typedef EGLSyncKHR (* PFN_EGLCREATESYNCKHR)(EGLDisplay dpy, EGLenum type, const EGLint * attrib_list);
extern PFN_EGLCREATESYNCKHR _eglCreateSyncKHR;

typedef EGLBoolean (* PFN_EGLDESTROYSYNCKHR)(EGLDisplay dpy, EGLSyncKHR sync);
extern PFN_EGLDESTROYSYNCKHR _eglDestroySyncKHR;

typedef EGLint (* PFN_EGLCLIENTWAITSYNCKHR)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout);
extern PFN_EGLCLIENTWAITSYNCKHR _eglClientWaitSyncKHR;

typedef EGLBoolean (* PFN_EGLSIGNALSYNCKHR)(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode);
extern PFN_EGLSIGNALSYNCKHR _eglSignalSyncKHR;

typedef EGLBoolean (* PFN_EGLGETSYNCATTRIBKHR)(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint * value);
extern PFN_EGLGETSYNCATTRIBKHR _eglGetSyncAttribKHR;

typedef EGLSyncNV (* PFN_EGLCREATEFENCESYNCNV)(EGLDisplay dpy, EGLenum condition, const EGLint * attrib_list);
extern PFN_EGLCREATEFENCESYNCNV _eglCreateFenceSyncNV;

typedef EGLBoolean (* PFN_EGLDESTROYSYNCNV)(EGLSyncNV sync);
extern PFN_EGLDESTROYSYNCNV _eglDestroySyncNV;

typedef EGLBoolean (* PFN_EGLFENCENV)(EGLSyncNV sync);
extern PFN_EGLFENCENV _eglFenceNV;

typedef EGLint (* PFN_EGLCLIENTWAITSYNCNV)(EGLSyncNV sync, EGLint flags, EGLTimeNV timeout);
extern PFN_EGLCLIENTWAITSYNCNV _eglClientWaitSyncNV;

typedef EGLBoolean (* PFN_EGLSIGNALSYNCNV)(EGLSyncNV sync, EGLenum mode);
extern PFN_EGLSIGNALSYNCNV _eglSignalSyncNV;

typedef EGLBoolean (* PFN_EGLGETSYNCATTRIBNV)(EGLSyncNV sync, EGLint attribute, EGLint * value);
extern PFN_EGLGETSYNCATTRIBNV _eglGetSyncAttribNV;

typedef EGLSurface (* PFN_EGLCREATEPIXMAPSURFACEHI)(EGLDisplay dpy, EGLConfig config, struct EGLClientPixmapHI * pixmap);
extern PFN_EGLCREATEPIXMAPSURFACEHI _eglCreatePixmapSurfaceHI;

typedef EGLImageKHR (* PFN_EGLCREATEDRMIMAGEMESA)(EGLDisplay dpy, const EGLint * attrib_list);
extern PFN_EGLCREATEDRMIMAGEMESA _eglCreateDRMImageMESA;

typedef EGLBoolean (* PFN_EGLEXPORTDRMIMAGEMESA)(EGLDisplay dpy, EGLImageKHR image, EGLint * name, EGLint * handle, EGLint * stride);
extern PFN_EGLEXPORTDRMIMAGEMESA _eglExportDRMImageMESA;

typedef EGLBoolean (* PFN_EGLPOSTSUBBUFFERNV)(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height);
extern PFN_EGLPOSTSUBBUFFERNV _eglPostSubBufferNV;

typedef EGLBoolean (* PFN_EGLQUERYSURFACEPOINTERANGLE)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, void * * value);
extern PFN_EGLQUERYSURFACEPOINTERANGLE _eglQuerySurfacePointerANGLE;

typedef EGLuint64NV (* PFN_EGLGETSYSTEMTIMEFREQUENCYNV)(void);
extern PFN_EGLGETSYSTEMTIMEFREQUENCYNV _eglGetSystemTimeFrequencyNV;

typedef EGLuint64NV (* PFN_EGLGETSYSTEMTIMENV)(void);
extern PFN_EGLGETSYSTEMTIMENV _eglGetSystemTimeNV;

typedef void (GL_APIENTRY * PFN_GLEGLIMAGETARGETTEXTURE2DOES)(GLenum target, EGLImageKHR image);
extern PFN_GLEGLIMAGETARGETTEXTURE2DOES _glEGLImageTargetTexture2DOES;

typedef void (GL_APIENTRY * PFN_GLEGLIMAGETARGETRENDERBUFFERSTORAGEOES)(GLenum target, EGLImageKHR image);
extern PFN_GLEGLIMAGETARGETRENDERBUFFERSTORAGEOES _glEGLImageTargetRenderbufferStorageOES;

#ifdef RETRACE
#define eglGetError _eglGetError
#define eglGetDisplay _eglGetDisplay
#define eglInitialize _eglInitialize
#define eglTerminate _eglTerminate
#define eglQueryString _eglQueryString
#define eglGetConfigs _eglGetConfigs
#define eglChooseConfig _eglChooseConfig
#define eglGetConfigAttrib _eglGetConfigAttrib
#define eglCreateWindowSurface _eglCreateWindowSurface
#define eglCreatePbufferSurface _eglCreatePbufferSurface
#define eglCreatePixmapSurface _eglCreatePixmapSurface
#define eglDestroySurface _eglDestroySurface
#define eglQuerySurface _eglQuerySurface
#define eglBindAPI _eglBindAPI
#define eglQueryAPI _eglQueryAPI
#define eglWaitClient _eglWaitClient
#define eglReleaseThread _eglReleaseThread
#define eglCreatePbufferFromClientBuffer _eglCreatePbufferFromClientBuffer
#define eglSurfaceAttrib _eglSurfaceAttrib
#define eglBindTexImage _eglBindTexImage
#define eglReleaseTexImage _eglReleaseTexImage
#define eglSwapInterval _eglSwapInterval
#define eglCreateContext _eglCreateContext
#define eglDestroyContext _eglDestroyContext
#define eglMakeCurrent _eglMakeCurrent
#define eglGetCurrentContext _eglGetCurrentContext
#define eglGetCurrentSurface _eglGetCurrentSurface
#define eglGetCurrentDisplay _eglGetCurrentDisplay
#define eglQueryContext _eglQueryContext
#define eglWaitGL _eglWaitGL
#define eglWaitNative _eglWaitNative
#define eglSwapBuffers _eglSwapBuffers
#define eglCopyBuffers _eglCopyBuffers
#define eglGetProcAddress _eglGetProcAddress
#define eglLockSurfaceKHR _eglLockSurfaceKHR
#define eglUnlockSurfaceKHR _eglUnlockSurfaceKHR
#define eglCreateImageKHR _eglCreateImageKHR
#define eglDestroyImageKHR _eglDestroyImageKHR
#define eglCreateSyncKHR _eglCreateSyncKHR
#define eglDestroySyncKHR _eglDestroySyncKHR
#define eglClientWaitSyncKHR _eglClientWaitSyncKHR
#define eglSignalSyncKHR _eglSignalSyncKHR
#define eglGetSyncAttribKHR _eglGetSyncAttribKHR
#define eglCreateFenceSyncNV _eglCreateFenceSyncNV
#define eglDestroySyncNV _eglDestroySyncNV
#define eglFenceNV _eglFenceNV
#define eglClientWaitSyncNV _eglClientWaitSyncNV
#define eglSignalSyncNV _eglSignalSyncNV
#define eglGetSyncAttribNV _eglGetSyncAttribNV
#define eglCreatePixmapSurfaceHI _eglCreatePixmapSurfaceHI
#define eglCreateDRMImageMESA _eglCreateDRMImageMESA
#define eglExportDRMImageMESA _eglExportDRMImageMESA
#define eglPostSubBufferNV _eglPostSubBufferNV
#define eglQuerySurfacePointerANGLE _eglQuerySurfacePointerANGLE
#define eglGetSystemTimeFrequencyNV _eglGetSystemTimeFrequencyNV
#define eglGetSystemTimeNV _eglGetSystemTimeNV
#define glEGLImageTargetTexture2DOES _glEGLImageTargetTexture2DOES
#define glEGLImageTargetRenderbufferStorageOES _glEGLImageTargetRenderbufferStorageOES
#endif /* RETRACE */


#if defined(_WIN32)

typedef HGLRC (__stdcall * PFN_WGLCREATECONTEXT)(HDC hdc);
extern PFN_WGLCREATECONTEXT _wglCreateContext;

typedef BOOL (__stdcall * PFN_WGLDELETECONTEXT)(HGLRC hglrc);
extern PFN_WGLDELETECONTEXT _wglDeleteContext;

typedef HGLRC (__stdcall * PFN_WGLGETCURRENTCONTEXT)(void);
extern PFN_WGLGETCURRENTCONTEXT _wglGetCurrentContext;

typedef BOOL (__stdcall * PFN_WGLMAKECURRENT)(HDC hdc, HGLRC hglrc);
extern PFN_WGLMAKECURRENT _wglMakeCurrent;

typedef BOOL (__stdcall * PFN_WGLCOPYCONTEXT)(HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask);
extern PFN_WGLCOPYCONTEXT _wglCopyContext;

typedef int (__stdcall * PFN_WGLCHOOSEPIXELFORMAT)(HDC hdc, const PIXELFORMATDESCRIPTOR * ppfd);
extern PFN_WGLCHOOSEPIXELFORMAT _wglChoosePixelFormat;

typedef int (__stdcall * PFN_WGLDESCRIBEPIXELFORMAT)(HDC hdc, int iPixelFormat, UINT nBytes, PIXELFORMATDESCRIPTOR * ppfd);
extern PFN_WGLDESCRIBEPIXELFORMAT _wglDescribePixelFormat;

typedef HDC (__stdcall * PFN_WGLGETCURRENTDC)(void);
extern PFN_WGLGETCURRENTDC _wglGetCurrentDC;

typedef PROC (__stdcall * PFN_WGLGETDEFAULTPROCADDRESS)(const char * lpszProc);
extern PFN_WGLGETDEFAULTPROCADDRESS _wglGetDefaultProcAddress;

typedef int (__stdcall * PFN_WGLGETPIXELFORMAT)(HDC hdc);
extern PFN_WGLGETPIXELFORMAT _wglGetPixelFormat;

typedef BOOL (__stdcall * PFN_WGLSETPIXELFORMAT)(HDC hdc, int iPixelFormat, const PIXELFORMATDESCRIPTOR * ppfd);
extern PFN_WGLSETPIXELFORMAT _wglSetPixelFormat;

typedef BOOL (__stdcall * PFN_WGLSWAPBUFFERS)(HDC hdc);
extern PFN_WGLSWAPBUFFERS _wglSwapBuffers;

typedef BOOL (__stdcall * PFN_WGLSHARELISTS)(HGLRC hglrc1, HGLRC hglrc2);
extern PFN_WGLSHARELISTS _wglShareLists;

typedef HGLRC (__stdcall * PFN_WGLCREATELAYERCONTEXT)(HDC hdc, int iLayerPlane);
extern PFN_WGLCREATELAYERCONTEXT _wglCreateLayerContext;

typedef BOOL (__stdcall * PFN_WGLDESCRIBELAYERPLANE)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nBytes, LAYERPLANEDESCRIPTOR * plpd);
extern PFN_WGLDESCRIBELAYERPLANE _wglDescribeLayerPlane;

typedef int (__stdcall * PFN_WGLSETLAYERPALETTEENTRIES)(HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF * pcr);
extern PFN_WGLSETLAYERPALETTEENTRIES _wglSetLayerPaletteEntries;

typedef int (__stdcall * PFN_WGLGETLAYERPALETTEENTRIES)(HDC hdc, int iLayerPlane, int iStart, int cEntries, COLORREF * pcr);
extern PFN_WGLGETLAYERPALETTEENTRIES _wglGetLayerPaletteEntries;

typedef BOOL (__stdcall * PFN_WGLREALIZELAYERPALETTE)(HDC hdc, int iLayerPlane, BOOL bRealize);
extern PFN_WGLREALIZELAYERPALETTE _wglRealizeLayerPalette;

typedef BOOL (__stdcall * PFN_WGLSWAPLAYERBUFFERS)(HDC hdc, UINT fuPlanes);
extern PFN_WGLSWAPLAYERBUFFERS _wglSwapLayerBuffers;

typedef BOOL (__stdcall * PFN_WGLUSEFONTBITMAPSA)(HDC hdc, DWORD first, DWORD count, DWORD listBase);
extern PFN_WGLUSEFONTBITMAPSA _wglUseFontBitmapsA;

typedef BOOL (__stdcall * PFN_WGLUSEFONTBITMAPSW)(HDC hdc, DWORD first, DWORD count, DWORD listBase);
extern PFN_WGLUSEFONTBITMAPSW _wglUseFontBitmapsW;

typedef DWORD (__stdcall * PFN_WGLSWAPMULTIPLEBUFFERS)(UINT n, const WGLSWAP * ps);
extern PFN_WGLSWAPMULTIPLEBUFFERS _wglSwapMultipleBuffers;

typedef BOOL (__stdcall * PFN_WGLUSEFONTOUTLINESA)(HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, GLYPHMETRICSFLOAT * lpgmf);
extern PFN_WGLUSEFONTOUTLINESA _wglUseFontOutlinesA;

typedef BOOL (__stdcall * PFN_WGLUSEFONTOUTLINESW)(HDC hdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, GLYPHMETRICSFLOAT * lpgmf);
extern PFN_WGLUSEFONTOUTLINESW _wglUseFontOutlinesW;

typedef HANDLE (__stdcall * PFN_WGLCREATEBUFFERREGIONARB)(HDC hDC, int iLayerPlane, UINT uType);
extern PFN_WGLCREATEBUFFERREGIONARB _wglCreateBufferRegionARB;

typedef void (__stdcall * PFN_WGLDELETEBUFFERREGIONARB)(HANDLE hRegion);
extern PFN_WGLDELETEBUFFERREGIONARB _wglDeleteBufferRegionARB;

typedef BOOL (__stdcall * PFN_WGLSAVEBUFFERREGIONARB)(HANDLE hRegion, int x, int y, int width, int height);
extern PFN_WGLSAVEBUFFERREGIONARB _wglSaveBufferRegionARB;

typedef BOOL (__stdcall * PFN_WGLRESTOREBUFFERREGIONARB)(HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
extern PFN_WGLRESTOREBUFFERREGIONARB _wglRestoreBufferRegionARB;

typedef const char * (__stdcall * PFN_WGLGETEXTENSIONSSTRINGARB)(HDC hdc);
extern PFN_WGLGETEXTENSIONSSTRINGARB _wglGetExtensionsStringARB;

typedef BOOL (__stdcall * PFN_WGLGETPIXELFORMATATTRIBIVARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, int * piValues);
extern PFN_WGLGETPIXELFORMATATTRIBIVARB _wglGetPixelFormatAttribivARB;

typedef BOOL (__stdcall * PFN_WGLGETPIXELFORMATATTRIBFVARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, FLOAT * pfValues);
extern PFN_WGLGETPIXELFORMATATTRIBFVARB _wglGetPixelFormatAttribfvARB;

typedef BOOL (__stdcall * PFN_WGLCHOOSEPIXELFORMATARB)(HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats);
extern PFN_WGLCHOOSEPIXELFORMATARB _wglChoosePixelFormatARB;

typedef BOOL (__stdcall * PFN_WGLMAKECONTEXTCURRENTARB)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
extern PFN_WGLMAKECONTEXTCURRENTARB _wglMakeContextCurrentARB;

typedef HDC (__stdcall * PFN_WGLGETCURRENTREADDCARB)(void);
extern PFN_WGLGETCURRENTREADDCARB _wglGetCurrentReadDCARB;

typedef HPBUFFERARB (__stdcall * PFN_WGLCREATEPBUFFERARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int * piAttribList);
extern PFN_WGLCREATEPBUFFERARB _wglCreatePbufferARB;

typedef HDC (__stdcall * PFN_WGLGETPBUFFERDCARB)(HPBUFFERARB hPbuffer);
extern PFN_WGLGETPBUFFERDCARB _wglGetPbufferDCARB;

typedef int (__stdcall * PFN_WGLRELEASEPBUFFERDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
extern PFN_WGLRELEASEPBUFFERDCARB _wglReleasePbufferDCARB;

typedef BOOL (__stdcall * PFN_WGLDESTROYPBUFFERARB)(HPBUFFERARB hPbuffer);
extern PFN_WGLDESTROYPBUFFERARB _wglDestroyPbufferARB;

typedef BOOL (__stdcall * PFN_WGLQUERYPBUFFERARB)(HPBUFFERARB hPbuffer, int iAttribute, int * piValue);
extern PFN_WGLQUERYPBUFFERARB _wglQueryPbufferARB;

typedef BOOL (__stdcall * PFN_WGLBINDTEXIMAGEARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern PFN_WGLBINDTEXIMAGEARB _wglBindTexImageARB;

typedef BOOL (__stdcall * PFN_WGLRELEASETEXIMAGEARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern PFN_WGLRELEASETEXIMAGEARB _wglReleaseTexImageARB;

typedef BOOL (__stdcall * PFN_WGLSETPBUFFERATTRIBARB)(HPBUFFERARB hPbuffer, const int * piAttribList);
extern PFN_WGLSETPBUFFERATTRIBARB _wglSetPbufferAttribARB;

typedef HGLRC (__stdcall * PFN_WGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hShareContext, const int * attribList);
extern PFN_WGLCREATECONTEXTATTRIBSARB _wglCreateContextAttribsARB;

typedef GLboolean (__stdcall * PFN_WGLCREATEDISPLAYCOLORTABLEEXT)(GLushort id);
extern PFN_WGLCREATEDISPLAYCOLORTABLEEXT _wglCreateDisplayColorTableEXT;

typedef GLboolean (__stdcall * PFN_WGLLOADDISPLAYCOLORTABLEEXT)(const GLushort * table, GLuint length);
extern PFN_WGLLOADDISPLAYCOLORTABLEEXT _wglLoadDisplayColorTableEXT;

typedef GLboolean (__stdcall * PFN_WGLBINDDISPLAYCOLORTABLEEXT)(GLushort id);
extern PFN_WGLBINDDISPLAYCOLORTABLEEXT _wglBindDisplayColorTableEXT;

typedef void (__stdcall * PFN_WGLDESTROYDISPLAYCOLORTABLEEXT)(GLushort id);
extern PFN_WGLDESTROYDISPLAYCOLORTABLEEXT _wglDestroyDisplayColorTableEXT;

typedef const char * (__stdcall * PFN_WGLGETEXTENSIONSSTRINGEXT)(void);
extern PFN_WGLGETEXTENSIONSSTRINGEXT _wglGetExtensionsStringEXT;

typedef BOOL (__stdcall * PFN_WGLMAKECONTEXTCURRENTEXT)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
extern PFN_WGLMAKECONTEXTCURRENTEXT _wglMakeContextCurrentEXT;

typedef HDC (__stdcall * PFN_WGLGETCURRENTREADDCEXT)(void);
extern PFN_WGLGETCURRENTREADDCEXT _wglGetCurrentReadDCEXT;

typedef HPBUFFEREXT (__stdcall * PFN_WGLCREATEPBUFFEREXT)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int * piAttribList);
extern PFN_WGLCREATEPBUFFEREXT _wglCreatePbufferEXT;

typedef HDC (__stdcall * PFN_WGLGETPBUFFERDCEXT)(HPBUFFEREXT hPbuffer);
extern PFN_WGLGETPBUFFERDCEXT _wglGetPbufferDCEXT;

typedef int (__stdcall * PFN_WGLRELEASEPBUFFERDCEXT)(HPBUFFEREXT hPbuffer, HDC hDC);
extern PFN_WGLRELEASEPBUFFERDCEXT _wglReleasePbufferDCEXT;

typedef BOOL (__stdcall * PFN_WGLDESTROYPBUFFEREXT)(HPBUFFEREXT hPbuffer);
extern PFN_WGLDESTROYPBUFFEREXT _wglDestroyPbufferEXT;

typedef BOOL (__stdcall * PFN_WGLQUERYPBUFFEREXT)(HPBUFFEREXT hPbuffer, int iAttribute, int * piValue);
extern PFN_WGLQUERYPBUFFEREXT _wglQueryPbufferEXT;

typedef BOOL (__stdcall * PFN_WGLGETPIXELFORMATATTRIBIVEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, int * piValues);
extern PFN_WGLGETPIXELFORMATATTRIBIVEXT _wglGetPixelFormatAttribivEXT;

typedef BOOL (__stdcall * PFN_WGLGETPIXELFORMATATTRIBFVEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, FLOAT * pfValues);
extern PFN_WGLGETPIXELFORMATATTRIBFVEXT _wglGetPixelFormatAttribfvEXT;

typedef BOOL (__stdcall * PFN_WGLCHOOSEPIXELFORMATEXT)(HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats);
extern PFN_WGLCHOOSEPIXELFORMATEXT _wglChoosePixelFormatEXT;

typedef BOOL (__stdcall * PFN_WGLSWAPINTERVALEXT)(int interval);
extern PFN_WGLSWAPINTERVALEXT _wglSwapIntervalEXT;

typedef int (__stdcall * PFN_WGLGETSWAPINTERVALEXT)(void);
extern PFN_WGLGETSWAPINTERVALEXT _wglGetSwapIntervalEXT;

typedef void * (__stdcall * PFN_WGLALLOCATEMEMORYNV)(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
extern PFN_WGLALLOCATEMEMORYNV _wglAllocateMemoryNV;

typedef void (__stdcall * PFN_WGLFREEMEMORYNV)(void * pointer);
extern PFN_WGLFREEMEMORYNV _wglFreeMemoryNV;

typedef BOOL (__stdcall * PFN_WGLGETSYNCVALUESOML)(HDC hdc, INT64 * ust, INT64 * msc, INT64 * sbc);
extern PFN_WGLGETSYNCVALUESOML _wglGetSyncValuesOML;

typedef BOOL (__stdcall * PFN_WGLGETMSCRATEOML)(HDC hdc, INT32 * numerator, INT32 * denominator);
extern PFN_WGLGETMSCRATEOML _wglGetMscRateOML;

typedef INT64 (__stdcall * PFN_WGLSWAPBUFFERSMSCOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder);
extern PFN_WGLSWAPBUFFERSMSCOML _wglSwapBuffersMscOML;

typedef INT64 (__stdcall * PFN_WGLSWAPLAYERBUFFERSMSCOML)(HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder);
extern PFN_WGLSWAPLAYERBUFFERSMSCOML _wglSwapLayerBuffersMscOML;

typedef BOOL (__stdcall * PFN_WGLWAITFORMSCOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 * ust, INT64 * msc, INT64 * sbc);
extern PFN_WGLWAITFORMSCOML _wglWaitForMscOML;

typedef BOOL (__stdcall * PFN_WGLWAITFORSBCOML)(HDC hdc, INT64 target_sbc, INT64 * ust, INT64 * msc, INT64 * sbc);
extern PFN_WGLWAITFORSBCOML _wglWaitForSbcOML;

typedef BOOL (__stdcall * PFN_WGLGETDIGITALVIDEOPARAMETERSI3D)(HDC hDC, int iAttribute, int * piValue);
extern PFN_WGLGETDIGITALVIDEOPARAMETERSI3D _wglGetDigitalVideoParametersI3D;

typedef BOOL (__stdcall * PFN_WGLSETDIGITALVIDEOPARAMETERSI3D)(HDC hDC, int iAttribute, const int * piValue);
extern PFN_WGLSETDIGITALVIDEOPARAMETERSI3D _wglSetDigitalVideoParametersI3D;

typedef BOOL (__stdcall * PFN_WGLGETGAMMATABLEPARAMETERSI3D)(HDC hDC, int iAttribute, int * piValue);
extern PFN_WGLGETGAMMATABLEPARAMETERSI3D _wglGetGammaTableParametersI3D;

typedef BOOL (__stdcall * PFN_WGLSETGAMMATABLEPARAMETERSI3D)(HDC hDC, int iAttribute, const int * piValue);
extern PFN_WGLSETGAMMATABLEPARAMETERSI3D _wglSetGammaTableParametersI3D;

typedef BOOL (__stdcall * PFN_WGLGETGAMMATABLEI3D)(HDC hDC, int iEntries, USHORT * puRed, USHORT * puGreen, USHORT * puBlue);
extern PFN_WGLGETGAMMATABLEI3D _wglGetGammaTableI3D;

typedef BOOL (__stdcall * PFN_WGLSETGAMMATABLEI3D)(HDC hDC, int iEntries, const USHORT * puRed, const USHORT * puGreen, const USHORT * puBlue);
extern PFN_WGLSETGAMMATABLEI3D _wglSetGammaTableI3D;

typedef BOOL (__stdcall * PFN_WGLENABLEGENLOCKI3D)(HDC hDC);
extern PFN_WGLENABLEGENLOCKI3D _wglEnableGenlockI3D;

typedef BOOL (__stdcall * PFN_WGLDISABLEGENLOCKI3D)(HDC hDC);
extern PFN_WGLDISABLEGENLOCKI3D _wglDisableGenlockI3D;

typedef BOOL (__stdcall * PFN_WGLISENABLEDGENLOCKI3D)(HDC hDC, BOOL * pFlag);
extern PFN_WGLISENABLEDGENLOCKI3D _wglIsEnabledGenlockI3D;

typedef BOOL (__stdcall * PFN_WGLGENLOCKSOURCEI3D)(HDC hDC, UINT uSource);
extern PFN_WGLGENLOCKSOURCEI3D _wglGenlockSourceI3D;

typedef BOOL (__stdcall * PFN_WGLGETGENLOCKSOURCEI3D)(HDC hDC, UINT * uSource);
extern PFN_WGLGETGENLOCKSOURCEI3D _wglGetGenlockSourceI3D;

typedef BOOL (__stdcall * PFN_WGLGENLOCKSOURCEEDGEI3D)(HDC hDC, UINT uEdge);
extern PFN_WGLGENLOCKSOURCEEDGEI3D _wglGenlockSourceEdgeI3D;

typedef BOOL (__stdcall * PFN_WGLGETGENLOCKSOURCEEDGEI3D)(HDC hDC, UINT * uEdge);
extern PFN_WGLGETGENLOCKSOURCEEDGEI3D _wglGetGenlockSourceEdgeI3D;

typedef BOOL (__stdcall * PFN_WGLGENLOCKSAMPLERATEI3D)(HDC hDC, UINT uRate);
extern PFN_WGLGENLOCKSAMPLERATEI3D _wglGenlockSampleRateI3D;

typedef BOOL (__stdcall * PFN_WGLGETGENLOCKSAMPLERATEI3D)(HDC hDC, UINT * uRate);
extern PFN_WGLGETGENLOCKSAMPLERATEI3D _wglGetGenlockSampleRateI3D;

typedef BOOL (__stdcall * PFN_WGLGENLOCKSOURCEDELAYI3D)(HDC hDC, UINT uDelay);
extern PFN_WGLGENLOCKSOURCEDELAYI3D _wglGenlockSourceDelayI3D;

typedef BOOL (__stdcall * PFN_WGLGETGENLOCKSOURCEDELAYI3D)(HDC hDC, UINT * uDelay);
extern PFN_WGLGETGENLOCKSOURCEDELAYI3D _wglGetGenlockSourceDelayI3D;

typedef BOOL (__stdcall * PFN_WGLQUERYGENLOCKMAXSOURCEDELAYI3D)(HDC hDC, UINT * uMaxLineDelay, UINT * uMaxPixelDelay);
extern PFN_WGLQUERYGENLOCKMAXSOURCEDELAYI3D _wglQueryGenlockMaxSourceDelayI3D;

typedef void * (__stdcall * PFN_WGLCREATEIMAGEBUFFERI3D)(HDC hDC, DWORD dwSize, UINT uFlags);
extern PFN_WGLCREATEIMAGEBUFFERI3D _wglCreateImageBufferI3D;

typedef BOOL (__stdcall * PFN_WGLDESTROYIMAGEBUFFERI3D)(HDC hDC, void * pAddress);
extern PFN_WGLDESTROYIMAGEBUFFERI3D _wglDestroyImageBufferI3D;

typedef BOOL (__stdcall * PFN_WGLASSOCIATEIMAGEBUFFEREVENTSI3D)(HDC hDC, const HANDLE * pEvent, void * const * pAddress, const DWORD * pSize, UINT count);
extern PFN_WGLASSOCIATEIMAGEBUFFEREVENTSI3D _wglAssociateImageBufferEventsI3D;

typedef BOOL (__stdcall * PFN_WGLRELEASEIMAGEBUFFEREVENTSI3D)(HDC hDC, void * const * pAddress, UINT count);
extern PFN_WGLRELEASEIMAGEBUFFEREVENTSI3D _wglReleaseImageBufferEventsI3D;

typedef BOOL (__stdcall * PFN_WGLENABLEFRAMELOCKI3D)(void);
extern PFN_WGLENABLEFRAMELOCKI3D _wglEnableFrameLockI3D;

typedef BOOL (__stdcall * PFN_WGLDISABLEFRAMELOCKI3D)(void);
extern PFN_WGLDISABLEFRAMELOCKI3D _wglDisableFrameLockI3D;

typedef BOOL (__stdcall * PFN_WGLISENABLEDFRAMELOCKI3D)(BOOL * pFlag);
extern PFN_WGLISENABLEDFRAMELOCKI3D _wglIsEnabledFrameLockI3D;

typedef BOOL (__stdcall * PFN_WGLQUERYFRAMELOCKMASTERI3D)(BOOL * pFlag);
extern PFN_WGLQUERYFRAMELOCKMASTERI3D _wglQueryFrameLockMasterI3D;

typedef BOOL (__stdcall * PFN_WGLGETFRAMEUSAGEI3D)(float * pUsage);
extern PFN_WGLGETFRAMEUSAGEI3D _wglGetFrameUsageI3D;

typedef BOOL (__stdcall * PFN_WGLBEGINFRAMETRACKINGI3D)(void);
extern PFN_WGLBEGINFRAMETRACKINGI3D _wglBeginFrameTrackingI3D;

typedef BOOL (__stdcall * PFN_WGLENDFRAMETRACKINGI3D)(void);
extern PFN_WGLENDFRAMETRACKINGI3D _wglEndFrameTrackingI3D;

typedef BOOL (__stdcall * PFN_WGLQUERYFRAMETRACKINGI3D)(DWORD * pFrameCount, DWORD * pMissedFrames, float * pLastMissedUsage);
extern PFN_WGLQUERYFRAMETRACKINGI3D _wglQueryFrameTrackingI3D;

typedef BOOL (__stdcall * PFN_WGLSETSTEREOEMITTERSTATE3DL)(HDC hDC, UINT uState);
extern PFN_WGLSETSTEREOEMITTERSTATE3DL _wglSetStereoEmitterState3DL;

typedef int (__stdcall * PFN_WGLENUMERATEVIDEODEVICESNV)(HDC hDC, HVIDEOOUTPUTDEVICENV * phDeviceList);
extern PFN_WGLENUMERATEVIDEODEVICESNV _wglEnumerateVideoDevicesNV;

typedef BOOL (__stdcall * PFN_WGLBINDVIDEODEVICENV)(HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int * piAttribList);
extern PFN_WGLBINDVIDEODEVICENV _wglBindVideoDeviceNV;

typedef BOOL (__stdcall * PFN_WGLQUERYCURRENTCONTEXTNV)(int iAttribute, int * piValue);
extern PFN_WGLQUERYCURRENTCONTEXTNV _wglQueryCurrentContextNV;

typedef BOOL (__stdcall * PFN_WGLGETVIDEODEVICENV)(HDC hDC, int numDevices, HPVIDEODEV * hVideoDevice);
extern PFN_WGLGETVIDEODEVICENV _wglGetVideoDeviceNV;

typedef BOOL (__stdcall * PFN_WGLRELEASEVIDEODEVICENV)(HPVIDEODEV hVideoDevice);
extern PFN_WGLRELEASEVIDEODEVICENV _wglReleaseVideoDeviceNV;

typedef BOOL (__stdcall * PFN_WGLBINDVIDEOIMAGENV)(HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
extern PFN_WGLBINDVIDEOIMAGENV _wglBindVideoImageNV;

typedef BOOL (__stdcall * PFN_WGLRELEASEVIDEOIMAGENV)(HPBUFFERARB hPbuffer, int iVideoBuffer);
extern PFN_WGLRELEASEVIDEOIMAGENV _wglReleaseVideoImageNV;

typedef BOOL (__stdcall * PFN_WGLSENDPBUFFERTOVIDEONV)(HPBUFFERARB hPbuffer, int iBufferType, unsigned long * pulCounterPbuffer, BOOL bBlock);
extern PFN_WGLSENDPBUFFERTOVIDEONV _wglSendPbufferToVideoNV;

typedef BOOL (__stdcall * PFN_WGLGETVIDEOINFONV)(HPVIDEODEV hpVideoDevice, unsigned long * pulCounterOutputPbuffer, unsigned long * pulCounterOutputVideo);
extern PFN_WGLGETVIDEOINFONV _wglGetVideoInfoNV;

typedef BOOL (__stdcall * PFN_WGLJOINSWAPGROUPNV)(HDC hDC, GLuint group);
extern PFN_WGLJOINSWAPGROUPNV _wglJoinSwapGroupNV;

typedef BOOL (__stdcall * PFN_WGLBINDSWAPBARRIERNV)(GLuint group, GLuint barrier);
extern PFN_WGLBINDSWAPBARRIERNV _wglBindSwapBarrierNV;

typedef BOOL (__stdcall * PFN_WGLQUERYSWAPGROUPNV)(HDC hDC, GLuint * group, GLuint * barrier);
extern PFN_WGLQUERYSWAPGROUPNV _wglQuerySwapGroupNV;

typedef BOOL (__stdcall * PFN_WGLQUERYMAXSWAPGROUPSNV)(HDC hDC, GLuint * maxGroups, GLuint * maxBarriers);
extern PFN_WGLQUERYMAXSWAPGROUPSNV _wglQueryMaxSwapGroupsNV;

typedef BOOL (__stdcall * PFN_WGLQUERYFRAMECOUNTNV)(HDC hDC, GLuint * count);
extern PFN_WGLQUERYFRAMECOUNTNV _wglQueryFrameCountNV;

typedef BOOL (__stdcall * PFN_WGLRESETFRAMECOUNTNV)(HDC hDC);
extern PFN_WGLRESETFRAMECOUNTNV _wglResetFrameCountNV;

typedef BOOL (__stdcall * PFN_WGLENUMGPUSNV)(UINT iGpuIndex, HGPUNV * phGpu);
extern PFN_WGLENUMGPUSNV _wglEnumGpusNV;

typedef BOOL (__stdcall * PFN_WGLENUMGPUDEVICESNV)(HGPUNV hGpu, UINT iDeviceIndex, _GPU_DEVICE * lpGpuDevice);
extern PFN_WGLENUMGPUDEVICESNV _wglEnumGpuDevicesNV;

typedef HDC (__stdcall * PFN_WGLCREATEAFFINITYDCNV)(const HGPUNV * phGpuList);
extern PFN_WGLCREATEAFFINITYDCNV _wglCreateAffinityDCNV;

typedef BOOL (__stdcall * PFN_WGLENUMGPUSFROMAFFINITYDCNV)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV * hGpu);
extern PFN_WGLENUMGPUSFROMAFFINITYDCNV _wglEnumGpusFromAffinityDCNV;

typedef BOOL (__stdcall * PFN_WGLDELETEDCNV)(HDC hdc);
extern PFN_WGLDELETEDCNV _wglDeleteDCNV;

typedef UINT (__stdcall * PFN_WGLGETGPUIDSAMD)(UINT maxCount, UINT * ids);
extern PFN_WGLGETGPUIDSAMD _wglGetGPUIDsAMD;

typedef INT (__stdcall * PFN_WGLGETGPUINFOAMD)(UINT id, int property, GLenum dataType, UINT size, void * data);
extern PFN_WGLGETGPUINFOAMD _wglGetGPUInfoAMD;

typedef UINT (__stdcall * PFN_WGLGETCONTEXTGPUIDAMD)(HGLRC hglrc);
extern PFN_WGLGETCONTEXTGPUIDAMD _wglGetContextGPUIDAMD;

typedef HGLRC (__stdcall * PFN_WGLCREATEASSOCIATEDCONTEXTAMD)(UINT id);
extern PFN_WGLCREATEASSOCIATEDCONTEXTAMD _wglCreateAssociatedContextAMD;

typedef HGLRC (__stdcall * PFN_WGLCREATEASSOCIATEDCONTEXTATTRIBSAMD)(UINT id, HGLRC hShareContext, const int * attribList);
extern PFN_WGLCREATEASSOCIATEDCONTEXTATTRIBSAMD _wglCreateAssociatedContextAttribsAMD;

typedef BOOL (__stdcall * PFN_WGLDELETEASSOCIATEDCONTEXTAMD)(HGLRC hglrc);
extern PFN_WGLDELETEASSOCIATEDCONTEXTAMD _wglDeleteAssociatedContextAMD;

typedef BOOL (__stdcall * PFN_WGLMAKEASSOCIATEDCONTEXTCURRENTAMD)(HGLRC hglrc);
extern PFN_WGLMAKEASSOCIATEDCONTEXTCURRENTAMD _wglMakeAssociatedContextCurrentAMD;

typedef HGLRC (__stdcall * PFN_WGLGETCURRENTASSOCIATEDCONTEXTAMD)(void);
extern PFN_WGLGETCURRENTASSOCIATEDCONTEXTAMD _wglGetCurrentAssociatedContextAMD;

typedef void (__stdcall * PFN_WGLBLITCONTEXTFRAMEBUFFERAMD)(HGLRC dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_WGLBLITCONTEXTFRAMEBUFFERAMD _wglBlitContextFramebufferAMD;

typedef BOOL (__stdcall * PFN_WGLBINDVIDEOCAPTUREDEVICENV)(UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice);
extern PFN_WGLBINDVIDEOCAPTUREDEVICENV _wglBindVideoCaptureDeviceNV;

typedef UINT (__stdcall * PFN_WGLENUMERATEVIDEOCAPTUREDEVICESNV)(HDC hDc, HVIDEOINPUTDEVICENV * phDeviceList);
extern PFN_WGLENUMERATEVIDEOCAPTUREDEVICESNV _wglEnumerateVideoCaptureDevicesNV;

typedef BOOL (__stdcall * PFN_WGLLOCKVIDEOCAPTUREDEVICENV)(HDC hDc, HVIDEOINPUTDEVICENV hDevice);
extern PFN_WGLLOCKVIDEOCAPTUREDEVICENV _wglLockVideoCaptureDeviceNV;

typedef BOOL (__stdcall * PFN_WGLQUERYVIDEOCAPTUREDEVICENV)(HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int * piValue);
extern PFN_WGLQUERYVIDEOCAPTUREDEVICENV _wglQueryVideoCaptureDeviceNV;

typedef BOOL (__stdcall * PFN_WGLRELEASEVIDEOCAPTUREDEVICENV)(HDC hDc, HVIDEOINPUTDEVICENV hDevice);
extern PFN_WGLRELEASEVIDEOCAPTUREDEVICENV _wglReleaseVideoCaptureDeviceNV;

typedef BOOL (__stdcall * PFN_WGLCOPYIMAGESUBDATANV)(HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_WGLCOPYIMAGESUBDATANV _wglCopyImageSubDataNV;

typedef BOOL (__stdcall * PFN_WGLDXSETRESOURCESHAREHANDLENV)(void * dxObject, HANDLE shareHandle);
extern PFN_WGLDXSETRESOURCESHAREHANDLENV _wglDXSetResourceShareHandleNV;

typedef HANDLE (__stdcall * PFN_WGLDXOPENDEVICENV)(void * dxDevice);
extern PFN_WGLDXOPENDEVICENV _wglDXOpenDeviceNV;

typedef BOOL (__stdcall * PFN_WGLDXCLOSEDEVICENV)(HANDLE hDevice);
extern PFN_WGLDXCLOSEDEVICENV _wglDXCloseDeviceNV;

typedef HANDLE (__stdcall * PFN_WGLDXREGISTEROBJECTNV)(HANDLE hDevice, void * dxObject, GLuint name, GLenum type, GLenum access);
extern PFN_WGLDXREGISTEROBJECTNV _wglDXRegisterObjectNV;

typedef BOOL (__stdcall * PFN_WGLDXUNREGISTEROBJECTNV)(HANDLE hDevice, HANDLE hObject);
extern PFN_WGLDXUNREGISTEROBJECTNV _wglDXUnregisterObjectNV;

typedef BOOL (__stdcall * PFN_WGLDXOBJECTACCESSNV)(HANDLE hObject, GLenum access);
extern PFN_WGLDXOBJECTACCESSNV _wglDXObjectAccessNV;

typedef BOOL (__stdcall * PFN_WGLDXLOCKOBJECTSNV)(HANDLE hDevice, GLint count, HANDLE * hObjects);
extern PFN_WGLDXLOCKOBJECTSNV _wglDXLockObjectsNV;

typedef BOOL (__stdcall * PFN_WGLDXUNLOCKOBJECTSNV)(HANDLE hDevice, GLint count, HANDLE * hObjects);
extern PFN_WGLDXUNLOCKOBJECTSNV _wglDXUnlockObjectsNV;

typedef BOOL (__stdcall * PFN_WGLDELAYBEFORESWAPNV)(HDC hDC, GLfloat seconds);
extern PFN_WGLDELAYBEFORESWAPNV _wglDelayBeforeSwapNV;

typedef PROC (__stdcall * PFN_WGLGETPROCADDRESS)(const char * lpszProc);
extern PFN_WGLGETPROCADDRESS _wglGetProcAddress;

#ifdef RETRACE
#define wglCreateContext _wglCreateContext
#define wglDeleteContext _wglDeleteContext
#define wglGetCurrentContext _wglGetCurrentContext
#define wglMakeCurrent _wglMakeCurrent
#define wglCopyContext _wglCopyContext
#define wglChoosePixelFormat _wglChoosePixelFormat
#define wglDescribePixelFormat _wglDescribePixelFormat
#define wglGetCurrentDC _wglGetCurrentDC
#define wglGetDefaultProcAddress _wglGetDefaultProcAddress
#define wglGetPixelFormat _wglGetPixelFormat
#define wglSetPixelFormat _wglSetPixelFormat
#define wglSwapBuffers _wglSwapBuffers
#define wglShareLists _wglShareLists
#define wglCreateLayerContext _wglCreateLayerContext
#define wglDescribeLayerPlane _wglDescribeLayerPlane
#define wglSetLayerPaletteEntries _wglSetLayerPaletteEntries
#define wglGetLayerPaletteEntries _wglGetLayerPaletteEntries
#define wglRealizeLayerPalette _wglRealizeLayerPalette
#define wglSwapLayerBuffers _wglSwapLayerBuffers
#define wglUseFontBitmapsA _wglUseFontBitmapsA
#define wglUseFontBitmapsW _wglUseFontBitmapsW
#define wglSwapMultipleBuffers _wglSwapMultipleBuffers
#define wglUseFontOutlinesA _wglUseFontOutlinesA
#define wglUseFontOutlinesW _wglUseFontOutlinesW
#define wglCreateBufferRegionARB _wglCreateBufferRegionARB
#define wglDeleteBufferRegionARB _wglDeleteBufferRegionARB
#define wglSaveBufferRegionARB _wglSaveBufferRegionARB
#define wglRestoreBufferRegionARB _wglRestoreBufferRegionARB
#define wglGetExtensionsStringARB _wglGetExtensionsStringARB
#define wglGetPixelFormatAttribivARB _wglGetPixelFormatAttribivARB
#define wglGetPixelFormatAttribfvARB _wglGetPixelFormatAttribfvARB
#define wglChoosePixelFormatARB _wglChoosePixelFormatARB
#define wglMakeContextCurrentARB _wglMakeContextCurrentARB
#define wglGetCurrentReadDCARB _wglGetCurrentReadDCARB
#define wglCreatePbufferARB _wglCreatePbufferARB
#define wglGetPbufferDCARB _wglGetPbufferDCARB
#define wglReleasePbufferDCARB _wglReleasePbufferDCARB
#define wglDestroyPbufferARB _wglDestroyPbufferARB
#define wglQueryPbufferARB _wglQueryPbufferARB
#define wglBindTexImageARB _wglBindTexImageARB
#define wglReleaseTexImageARB _wglReleaseTexImageARB
#define wglSetPbufferAttribARB _wglSetPbufferAttribARB
#define wglCreateContextAttribsARB _wglCreateContextAttribsARB
#define wglCreateDisplayColorTableEXT _wglCreateDisplayColorTableEXT
#define wglLoadDisplayColorTableEXT _wglLoadDisplayColorTableEXT
#define wglBindDisplayColorTableEXT _wglBindDisplayColorTableEXT
#define wglDestroyDisplayColorTableEXT _wglDestroyDisplayColorTableEXT
#define wglGetExtensionsStringEXT _wglGetExtensionsStringEXT
#define wglMakeContextCurrentEXT _wglMakeContextCurrentEXT
#define wglGetCurrentReadDCEXT _wglGetCurrentReadDCEXT
#define wglCreatePbufferEXT _wglCreatePbufferEXT
#define wglGetPbufferDCEXT _wglGetPbufferDCEXT
#define wglReleasePbufferDCEXT _wglReleasePbufferDCEXT
#define wglDestroyPbufferEXT _wglDestroyPbufferEXT
#define wglQueryPbufferEXT _wglQueryPbufferEXT
#define wglGetPixelFormatAttribivEXT _wglGetPixelFormatAttribivEXT
#define wglGetPixelFormatAttribfvEXT _wglGetPixelFormatAttribfvEXT
#define wglChoosePixelFormatEXT _wglChoosePixelFormatEXT
#define wglSwapIntervalEXT _wglSwapIntervalEXT
#define wglGetSwapIntervalEXT _wglGetSwapIntervalEXT
#define wglAllocateMemoryNV _wglAllocateMemoryNV
#define wglFreeMemoryNV _wglFreeMemoryNV
#define wglGetSyncValuesOML _wglGetSyncValuesOML
#define wglGetMscRateOML _wglGetMscRateOML
#define wglSwapBuffersMscOML _wglSwapBuffersMscOML
#define wglSwapLayerBuffersMscOML _wglSwapLayerBuffersMscOML
#define wglWaitForMscOML _wglWaitForMscOML
#define wglWaitForSbcOML _wglWaitForSbcOML
#define wglGetDigitalVideoParametersI3D _wglGetDigitalVideoParametersI3D
#define wglSetDigitalVideoParametersI3D _wglSetDigitalVideoParametersI3D
#define wglGetGammaTableParametersI3D _wglGetGammaTableParametersI3D
#define wglSetGammaTableParametersI3D _wglSetGammaTableParametersI3D
#define wglGetGammaTableI3D _wglGetGammaTableI3D
#define wglSetGammaTableI3D _wglSetGammaTableI3D
#define wglEnableGenlockI3D _wglEnableGenlockI3D
#define wglDisableGenlockI3D _wglDisableGenlockI3D
#define wglIsEnabledGenlockI3D _wglIsEnabledGenlockI3D
#define wglGenlockSourceI3D _wglGenlockSourceI3D
#define wglGetGenlockSourceI3D _wglGetGenlockSourceI3D
#define wglGenlockSourceEdgeI3D _wglGenlockSourceEdgeI3D
#define wglGetGenlockSourceEdgeI3D _wglGetGenlockSourceEdgeI3D
#define wglGenlockSampleRateI3D _wglGenlockSampleRateI3D
#define wglGetGenlockSampleRateI3D _wglGetGenlockSampleRateI3D
#define wglGenlockSourceDelayI3D _wglGenlockSourceDelayI3D
#define wglGetGenlockSourceDelayI3D _wglGetGenlockSourceDelayI3D
#define wglQueryGenlockMaxSourceDelayI3D _wglQueryGenlockMaxSourceDelayI3D
#define wglCreateImageBufferI3D _wglCreateImageBufferI3D
#define wglDestroyImageBufferI3D _wglDestroyImageBufferI3D
#define wglAssociateImageBufferEventsI3D _wglAssociateImageBufferEventsI3D
#define wglReleaseImageBufferEventsI3D _wglReleaseImageBufferEventsI3D
#define wglEnableFrameLockI3D _wglEnableFrameLockI3D
#define wglDisableFrameLockI3D _wglDisableFrameLockI3D
#define wglIsEnabledFrameLockI3D _wglIsEnabledFrameLockI3D
#define wglQueryFrameLockMasterI3D _wglQueryFrameLockMasterI3D
#define wglGetFrameUsageI3D _wglGetFrameUsageI3D
#define wglBeginFrameTrackingI3D _wglBeginFrameTrackingI3D
#define wglEndFrameTrackingI3D _wglEndFrameTrackingI3D
#define wglQueryFrameTrackingI3D _wglQueryFrameTrackingI3D
#define wglSetStereoEmitterState3DL _wglSetStereoEmitterState3DL
#define wglEnumerateVideoDevicesNV _wglEnumerateVideoDevicesNV
#define wglBindVideoDeviceNV _wglBindVideoDeviceNV
#define wglQueryCurrentContextNV _wglQueryCurrentContextNV
#define wglGetVideoDeviceNV _wglGetVideoDeviceNV
#define wglReleaseVideoDeviceNV _wglReleaseVideoDeviceNV
#define wglBindVideoImageNV _wglBindVideoImageNV
#define wglReleaseVideoImageNV _wglReleaseVideoImageNV
#define wglSendPbufferToVideoNV _wglSendPbufferToVideoNV
#define wglGetVideoInfoNV _wglGetVideoInfoNV
#define wglJoinSwapGroupNV _wglJoinSwapGroupNV
#define wglBindSwapBarrierNV _wglBindSwapBarrierNV
#define wglQuerySwapGroupNV _wglQuerySwapGroupNV
#define wglQueryMaxSwapGroupsNV _wglQueryMaxSwapGroupsNV
#define wglQueryFrameCountNV _wglQueryFrameCountNV
#define wglResetFrameCountNV _wglResetFrameCountNV
#define wglEnumGpusNV _wglEnumGpusNV
#define wglEnumGpuDevicesNV _wglEnumGpuDevicesNV
#define wglCreateAffinityDCNV _wglCreateAffinityDCNV
#define wglEnumGpusFromAffinityDCNV _wglEnumGpusFromAffinityDCNV
#define wglDeleteDCNV _wglDeleteDCNV
#define wglGetGPUIDsAMD _wglGetGPUIDsAMD
#define wglGetGPUInfoAMD _wglGetGPUInfoAMD
#define wglGetContextGPUIDAMD _wglGetContextGPUIDAMD
#define wglCreateAssociatedContextAMD _wglCreateAssociatedContextAMD
#define wglCreateAssociatedContextAttribsAMD _wglCreateAssociatedContextAttribsAMD
#define wglDeleteAssociatedContextAMD _wglDeleteAssociatedContextAMD
#define wglMakeAssociatedContextCurrentAMD _wglMakeAssociatedContextCurrentAMD
#define wglGetCurrentAssociatedContextAMD _wglGetCurrentAssociatedContextAMD
#define wglBlitContextFramebufferAMD _wglBlitContextFramebufferAMD
#define wglBindVideoCaptureDeviceNV _wglBindVideoCaptureDeviceNV
#define wglEnumerateVideoCaptureDevicesNV _wglEnumerateVideoCaptureDevicesNV
#define wglLockVideoCaptureDeviceNV _wglLockVideoCaptureDeviceNV
#define wglQueryVideoCaptureDeviceNV _wglQueryVideoCaptureDeviceNV
#define wglReleaseVideoCaptureDeviceNV _wglReleaseVideoCaptureDeviceNV
#define wglCopyImageSubDataNV _wglCopyImageSubDataNV
#define wglDXSetResourceShareHandleNV _wglDXSetResourceShareHandleNV
#define wglDXOpenDeviceNV _wglDXOpenDeviceNV
#define wglDXCloseDeviceNV _wglDXCloseDeviceNV
#define wglDXRegisterObjectNV _wglDXRegisterObjectNV
#define wglDXUnregisterObjectNV _wglDXUnregisterObjectNV
#define wglDXObjectAccessNV _wglDXObjectAccessNV
#define wglDXLockObjectsNV _wglDXLockObjectsNV
#define wglDXUnlockObjectsNV _wglDXUnlockObjectsNV
#define wglDelayBeforeSwapNV _wglDelayBeforeSwapNV
#define wglGetProcAddress _wglGetProcAddress
#endif /* RETRACE */


#elif defined(__APPLE__)

typedef CGLError (* PFN_CGLSETCURRENTCONTEXT)(CGLContextObj ctx);
extern PFN_CGLSETCURRENTCONTEXT _CGLSetCurrentContext;

typedef CGLContextObj (* PFN_CGLGETCURRENTCONTEXT)(void);
extern PFN_CGLGETCURRENTCONTEXT _CGLGetCurrentContext;

typedef CGLError (* PFN_CGLCHOOSEPIXELFORMAT)(const CGLPixelFormatAttribute * attribs, CGLPixelFormatObj * pix, GLint * npix);
extern PFN_CGLCHOOSEPIXELFORMAT _CGLChoosePixelFormat;

typedef CGLError (* PFN_CGLDESTROYPIXELFORMAT)(CGLPixelFormatObj pix);
extern PFN_CGLDESTROYPIXELFORMAT _CGLDestroyPixelFormat;

typedef CGLError (* PFN_CGLDESCRIBEPIXELFORMAT)(CGLPixelFormatObj pix, GLint pix_num, CGLPixelFormatAttribute attrib, GLint * value);
extern PFN_CGLDESCRIBEPIXELFORMAT _CGLDescribePixelFormat;

typedef void (* PFN_CGLRELEASEPIXELFORMAT)(CGLPixelFormatObj pix);
extern PFN_CGLRELEASEPIXELFORMAT _CGLReleasePixelFormat;

typedef CGLPixelFormatObj (* PFN_CGLRETAINPIXELFORMAT)(CGLPixelFormatObj pix);
extern PFN_CGLRETAINPIXELFORMAT _CGLRetainPixelFormat;

typedef GLuint (* PFN_CGLGETPIXELFORMATRETAINCOUNT)(CGLPixelFormatObj pix);
extern PFN_CGLGETPIXELFORMATRETAINCOUNT _CGLGetPixelFormatRetainCount;

typedef CGLError (* PFN_CGLQUERYRENDERERINFO)(GLuint display_mask, CGLRendererInfoObj * rend, GLint * nrend);
extern PFN_CGLQUERYRENDERERINFO _CGLQueryRendererInfo;

typedef CGLError (* PFN_CGLDESTROYRENDERERINFO)(CGLRendererInfoObj rend);
extern PFN_CGLDESTROYRENDERERINFO _CGLDestroyRendererInfo;

typedef CGLError (* PFN_CGLDESCRIBERENDERER)(CGLRendererInfoObj rend, GLint rend_num, CGLRendererProperty prop, GLint * value);
extern PFN_CGLDESCRIBERENDERER _CGLDescribeRenderer;

typedef CGLError (* PFN_CGLCREATECONTEXT)(CGLPixelFormatObj pix, CGLContextObj share, CGLContextObj * ctx);
extern PFN_CGLCREATECONTEXT _CGLCreateContext;

typedef CGLError (* PFN_CGLDESTROYCONTEXT)(CGLContextObj ctx);
extern PFN_CGLDESTROYCONTEXT _CGLDestroyContext;

typedef CGLError (* PFN_CGLCOPYCONTEXT)(CGLContextObj src, CGLContextObj dst, GLbitfield mask);
extern PFN_CGLCOPYCONTEXT _CGLCopyContext;

typedef CGLContextObj (* PFN_CGLRETAINCONTEXT)(CGLContextObj ctx);
extern PFN_CGLRETAINCONTEXT _CGLRetainContext;

typedef void (* PFN_CGLRELEASECONTEXT)(CGLContextObj ctx);
extern PFN_CGLRELEASECONTEXT _CGLReleaseContext;

typedef GLuint (* PFN_CGLGETCONTEXTRETAINCOUNT)(CGLContextObj ctx);
extern PFN_CGLGETCONTEXTRETAINCOUNT _CGLGetContextRetainCount;

typedef CGLPixelFormatObj (* PFN_CGLGETPIXELFORMAT)(CGLContextObj ctx);
extern PFN_CGLGETPIXELFORMAT _CGLGetPixelFormat;

typedef CGLError (* PFN_CGLCREATEPBUFFER)(GLsizei width, GLsizei height, GLenum target, GLenum internalFormat, GLint max_level, CGLPBufferObj * pbuffer);
extern PFN_CGLCREATEPBUFFER _CGLCreatePBuffer;

typedef CGLError (* PFN_CGLDESTROYPBUFFER)(CGLPBufferObj pbuffer);
extern PFN_CGLDESTROYPBUFFER _CGLDestroyPBuffer;

typedef CGLError (* PFN_CGLDESCRIBEPBUFFER)(CGLPBufferObj obj, GLsizei * width, GLsizei * height, GLenum * target, GLenum * internalFormat, GLint * mipmap);
extern PFN_CGLDESCRIBEPBUFFER _CGLDescribePBuffer;

typedef CGLError (* PFN_CGLTEXIMAGEPBUFFER)(CGLContextObj ctx, CGLPBufferObj pbuffer, GLenum source);
extern PFN_CGLTEXIMAGEPBUFFER _CGLTexImagePBuffer;

typedef CGLPBufferObj (* PFN_CGLRETAINPBUFFER)(CGLPBufferObj pbuffer);
extern PFN_CGLRETAINPBUFFER _CGLRetainPBuffer;

typedef void (* PFN_CGLRELEASEPBUFFER)(CGLPBufferObj pbuffer);
extern PFN_CGLRELEASEPBUFFER _CGLReleasePBuffer;

typedef GLuint (* PFN_CGLGETPBUFFERRETAINCOUNT)(CGLPBufferObj pbuffer);
extern PFN_CGLGETPBUFFERRETAINCOUNT _CGLGetPBufferRetainCount;

typedef CGLError (* PFN_CGLSETOFFSCREEN)(CGLContextObj ctx, GLsizei width, GLsizei height, GLint rowbytes, void * baseaddr);
extern PFN_CGLSETOFFSCREEN _CGLSetOffScreen;

typedef CGLError (* PFN_CGLGETOFFSCREEN)(CGLContextObj ctx, GLsizei * width, GLsizei * height, GLint * rowbytes, void * * baseaddr);
extern PFN_CGLGETOFFSCREEN _CGLGetOffScreen;

typedef CGLError (* PFN_CGLSETFULLSCREEN)(CGLContextObj ctx);
extern PFN_CGLSETFULLSCREEN _CGLSetFullScreen;

typedef CGLError (* PFN_CGLSETFULLSCREENONDISPLAY)(CGLContextObj ctx, GLuint display_mask);
extern PFN_CGLSETFULLSCREENONDISPLAY _CGLSetFullScreenOnDisplay;

typedef CGLError (* PFN_CGLSETPBUFFER)(CGLContextObj ctx, CGLPBufferObj pbuffer, GLenum face, GLint level, GLint screen);
extern PFN_CGLSETPBUFFER _CGLSetPBuffer;

typedef CGLError (* PFN_CGLGETPBUFFER)(CGLContextObj ctx, CGLPBufferObj * pbuffer, GLenum * face, GLint * level, GLint * screen);
extern PFN_CGLGETPBUFFER _CGLGetPBuffer;

typedef CGLError (* PFN_CGLCLEARDRAWABLE)(CGLContextObj ctx);
extern PFN_CGLCLEARDRAWABLE _CGLClearDrawable;

typedef CGLError (* PFN_CGLFLUSHDRAWABLE)(CGLContextObj ctx);
extern PFN_CGLFLUSHDRAWABLE _CGLFlushDrawable;

typedef CGLError (* PFN_CGLENABLE)(CGLContextObj ctx, CGLContextEnable pname);
extern PFN_CGLENABLE _CGLEnable;

typedef CGLError (* PFN_CGLDISABLE)(CGLContextObj ctx, CGLContextEnable pname);
extern PFN_CGLDISABLE _CGLDisable;

typedef CGLError (* PFN_CGLISENABLED)(CGLContextObj ctx, CGLContextEnable pname, GLint * enable);
extern PFN_CGLISENABLED _CGLIsEnabled;

typedef CGLError (* PFN_CGLSETPARAMETER)(CGLContextObj ctx, CGLContextParameter pname, const GLint * params);
extern PFN_CGLSETPARAMETER _CGLSetParameter;

typedef CGLError (* PFN_CGLGETPARAMETER)(CGLContextObj ctx, CGLContextParameter pname, GLint * params);
extern PFN_CGLGETPARAMETER _CGLGetParameter;

typedef CGLError (* PFN_CGLSETVIRTUALSCREEN)(CGLContextObj ctx, GLint screen);
extern PFN_CGLSETVIRTUALSCREEN _CGLSetVirtualScreen;

typedef CGLError (* PFN_CGLGETVIRTUALSCREEN)(CGLContextObj ctx, GLint * screen);
extern PFN_CGLGETVIRTUALSCREEN _CGLGetVirtualScreen;

typedef CGLError (* PFN_CGLSETGLOBALOPTION)(CGLGlobalOption pname, const GLint * params);
extern PFN_CGLSETGLOBALOPTION _CGLSetGlobalOption;

typedef CGLError (* PFN_CGLGETGLOBALOPTION)(CGLGlobalOption pname, GLint * params);
extern PFN_CGLGETGLOBALOPTION _CGLGetGlobalOption;

typedef CGLError (* PFN_CGLSETOPTION)(CGLGlobalOption pname, GLint param);
extern PFN_CGLSETOPTION _CGLSetOption;

typedef CGLError (* PFN_CGLGETOPTION)(CGLGlobalOption pname, GLint * param);
extern PFN_CGLGETOPTION _CGLGetOption;

typedef CGLError (* PFN_CGLLOCKCONTEXT)(CGLContextObj ctx);
extern PFN_CGLLOCKCONTEXT _CGLLockContext;

typedef CGLError (* PFN_CGLUNLOCKCONTEXT)(CGLContextObj ctx);
extern PFN_CGLUNLOCKCONTEXT _CGLUnlockContext;

typedef void (* PFN_CGLGETVERSION)(GLint * majorvers, GLint * minorvers);
extern PFN_CGLGETVERSION _CGLGetVersion;

typedef const char * (* PFN_CGLERRORSTRING)(CGLError error);
extern PFN_CGLERRORSTRING _CGLErrorString;

typedef CGLError (* PFN_CGLTEXIMAGEIOSURFACE2D)(CGLContextObj ctx, GLenum target, GLenum internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, IOSurfaceRef ioSurface, GLuint plane);
extern PFN_CGLTEXIMAGEIOSURFACE2D _CGLTexImageIOSurface2D;

typedef CGLShareGroupObj (* PFN_CGLGETSHAREGROUP)(CGLContextObj ctx);
extern PFN_CGLGETSHAREGROUP _CGLGetShareGroup;

typedef CGLError (* PFN_CGLSETSURFACE)(CGLContextObj ctx, CGSConnectionID cid, CGSWindowID wid, CGSSurfaceID sid);
extern PFN_CGLSETSURFACE _CGLSetSurface;

typedef CGLError (* PFN_CGLGETSURFACE)(CGLContextObj ctx, CGSConnectionID * cid, CGSWindowID * wid, CGSSurfaceID * sid);
extern PFN_CGLGETSURFACE _CGLGetSurface;

typedef CGLError (* PFN_CGLUPDATECONTEXT)(CGLContextObj ctx);
extern PFN_CGLUPDATECONTEXT _CGLUpdateContext;

typedef CGLError (* PFN_CGLOPENCLMUXLOCKDOWN)(void);
extern PFN_CGLOPENCLMUXLOCKDOWN _CGLOpenCLMuxLockDown;

typedef GLboolean (* PFN_CGLARECONTEXTSSHARED)(CGLContextObj ctx1, CGLContextObj ctx2);
extern PFN_CGLARECONTEXTSSHARED _CGLAreContextsShared;

typedef CGLContextObj (* PFN_CGLGETNEXTCONTEXT)(CGLContextObj ctx);
extern PFN_CGLGETNEXTCONTEXT _CGLGetNextContext;

typedef void * (* PFN_CGLFRONTDISPATCH)(CGLContextObj ctx);
extern PFN_CGLFRONTDISPATCH _CGLFrontDispatch;

typedef void * (* PFN_CGLBACKDISPATCH)(CGLContextObj ctx);
extern PFN_CGLBACKDISPATCH _CGLBackDispatch;

typedef void (* PFN_CGLSELECTDISPATCH)(CGLContextObj ctx, void * dispatch);
extern PFN_CGLSELECTDISPATCH _CGLSelectDispatch;

typedef void (* PFN_CGLSELECTDISPATCHBOUNDED)(CGLContextObj ctx, void * dispatch, GLint size);
extern PFN_CGLSELECTDISPATCHBOUNDED _CGLSelectDispatchBounded;

typedef void (* PFN_CGLSELECTDISPATCHFUNCTION)(CGLContextObj ctx, void * functionPtr, GLint functionId);
extern PFN_CGLSELECTDISPATCHFUNCTION _CGLSelectDispatchFunction;

typedef void (* PFN_CGLRESTOREDISPATCH)(CGLContextObj ctx);
extern PFN_CGLRESTOREDISPATCH _CGLRestoreDispatch;

typedef void (* PFN_CGLRESTOREDISPATCHFUNCTION)(CGLContextObj ctx, GLint functionId);
extern PFN_CGLRESTOREDISPATCHFUNCTION _CGLRestoreDispatchFunction;

typedef CGLError (* PFN_CGLSETPBUFFERVOLATILESTATE)(CGLPBufferObj pbuffer, void * state);
extern PFN_CGLSETPBUFFERVOLATILESTATE _CGLSetPBufferVolatileState;

#ifdef RETRACE
#define CGLSetCurrentContext _CGLSetCurrentContext
#define CGLGetCurrentContext _CGLGetCurrentContext
#define CGLChoosePixelFormat _CGLChoosePixelFormat
#define CGLDestroyPixelFormat _CGLDestroyPixelFormat
#define CGLDescribePixelFormat _CGLDescribePixelFormat
#define CGLReleasePixelFormat _CGLReleasePixelFormat
#define CGLRetainPixelFormat _CGLRetainPixelFormat
#define CGLGetPixelFormatRetainCount _CGLGetPixelFormatRetainCount
#define CGLQueryRendererInfo _CGLQueryRendererInfo
#define CGLDestroyRendererInfo _CGLDestroyRendererInfo
#define CGLDescribeRenderer _CGLDescribeRenderer
#define CGLCreateContext _CGLCreateContext
#define CGLDestroyContext _CGLDestroyContext
#define CGLCopyContext _CGLCopyContext
#define CGLRetainContext _CGLRetainContext
#define CGLReleaseContext _CGLReleaseContext
#define CGLGetContextRetainCount _CGLGetContextRetainCount
#define CGLGetPixelFormat _CGLGetPixelFormat
#define CGLCreatePBuffer _CGLCreatePBuffer
#define CGLDestroyPBuffer _CGLDestroyPBuffer
#define CGLDescribePBuffer _CGLDescribePBuffer
#define CGLTexImagePBuffer _CGLTexImagePBuffer
#define CGLRetainPBuffer _CGLRetainPBuffer
#define CGLReleasePBuffer _CGLReleasePBuffer
#define CGLGetPBufferRetainCount _CGLGetPBufferRetainCount
#define CGLSetOffScreen _CGLSetOffScreen
#define CGLGetOffScreen _CGLGetOffScreen
#define CGLSetFullScreen _CGLSetFullScreen
#define CGLSetFullScreenOnDisplay _CGLSetFullScreenOnDisplay
#define CGLSetPBuffer _CGLSetPBuffer
#define CGLGetPBuffer _CGLGetPBuffer
#define CGLClearDrawable _CGLClearDrawable
#define CGLFlushDrawable _CGLFlushDrawable
#define CGLEnable _CGLEnable
#define CGLDisable _CGLDisable
#define CGLIsEnabled _CGLIsEnabled
#define CGLSetParameter _CGLSetParameter
#define CGLGetParameter _CGLGetParameter
#define CGLSetVirtualScreen _CGLSetVirtualScreen
#define CGLGetVirtualScreen _CGLGetVirtualScreen
#define CGLSetGlobalOption _CGLSetGlobalOption
#define CGLGetGlobalOption _CGLGetGlobalOption
#define CGLSetOption _CGLSetOption
#define CGLGetOption _CGLGetOption
#define CGLLockContext _CGLLockContext
#define CGLUnlockContext _CGLUnlockContext
#define CGLGetVersion _CGLGetVersion
#define CGLErrorString _CGLErrorString
#define CGLTexImageIOSurface2D _CGLTexImageIOSurface2D
#define CGLGetShareGroup _CGLGetShareGroup
#define CGLSetSurface _CGLSetSurface
#define CGLGetSurface _CGLGetSurface
#define CGLUpdateContext _CGLUpdateContext
#define CGLOpenCLMuxLockDown _CGLOpenCLMuxLockDown
#define CGLAreContextsShared _CGLAreContextsShared
#define CGLGetNextContext _CGLGetNextContext
#define CGLFrontDispatch _CGLFrontDispatch
#define CGLBackDispatch _CGLBackDispatch
#define CGLSelectDispatch _CGLSelectDispatch
#define CGLSelectDispatchBounded _CGLSelectDispatchBounded
#define CGLSelectDispatchFunction _CGLSelectDispatchFunction
#define CGLRestoreDispatch _CGLRestoreDispatch
#define CGLRestoreDispatchFunction _CGLRestoreDispatchFunction
#define CGLSetPBufferVolatileState _CGLSetPBufferVolatileState
#endif /* RETRACE */


#elif defined(HAVE_X11)

typedef XVisualInfo * (* PFN_GLXCHOOSEVISUAL)(Display * dpy, int screen, int * attribList);
extern PFN_GLXCHOOSEVISUAL _glXChooseVisual;

typedef GLXContext (* PFN_GLXCREATECONTEXT)(Display * dpy, XVisualInfo * vis, GLXContext shareList, int direct);
extern PFN_GLXCREATECONTEXT _glXCreateContext;

typedef void (* PFN_GLXDESTROYCONTEXT)(Display * dpy, GLXContext ctx);
extern PFN_GLXDESTROYCONTEXT _glXDestroyContext;

typedef int (* PFN_GLXMAKECURRENT)(Display * dpy, GLXDrawable drawable, GLXContext ctx);
extern PFN_GLXMAKECURRENT _glXMakeCurrent;

typedef void (* PFN_GLXCOPYCONTEXT)(Display * dpy, GLXContext src, GLXContext dst, unsigned long mask);
extern PFN_GLXCOPYCONTEXT _glXCopyContext;

typedef void (* PFN_GLXSWAPBUFFERS)(Display * dpy, GLXDrawable drawable);
extern PFN_GLXSWAPBUFFERS _glXSwapBuffers;

typedef GLXPixmap (* PFN_GLXCREATEGLXPIXMAP)(Display * dpy, XVisualInfo * visual, Pixmap pixmap);
extern PFN_GLXCREATEGLXPIXMAP _glXCreateGLXPixmap;

typedef void (* PFN_GLXDESTROYGLXPIXMAP)(Display * dpy, GLXPixmap pixmap);
extern PFN_GLXDESTROYGLXPIXMAP _glXDestroyGLXPixmap;

typedef int (* PFN_GLXQUERYEXTENSION)(Display * dpy, int * errorb, int * event);
extern PFN_GLXQUERYEXTENSION _glXQueryExtension;

typedef int (* PFN_GLXQUERYVERSION)(Display * dpy, int * maj, int * min);
extern PFN_GLXQUERYVERSION _glXQueryVersion;

typedef int (* PFN_GLXISDIRECT)(Display * dpy, GLXContext ctx);
extern PFN_GLXISDIRECT _glXIsDirect;

typedef int (* PFN_GLXGETCONFIG)(Display * dpy, XVisualInfo * visual, int attrib, int * value);
extern PFN_GLXGETCONFIG _glXGetConfig;

typedef GLXContext (* PFN_GLXGETCURRENTCONTEXT)(void);
extern PFN_GLXGETCURRENTCONTEXT _glXGetCurrentContext;

typedef GLXDrawable (* PFN_GLXGETCURRENTDRAWABLE)(void);
extern PFN_GLXGETCURRENTDRAWABLE _glXGetCurrentDrawable;

typedef void (* PFN_GLXWAITGL)(void);
extern PFN_GLXWAITGL _glXWaitGL;

typedef void (* PFN_GLXWAITX)(void);
extern PFN_GLXWAITX _glXWaitX;

typedef void (* PFN_GLXUSEXFONT)(Font font, int first, int count, int list);
extern PFN_GLXUSEXFONT _glXUseXFont;

typedef const char * (* PFN_GLXQUERYEXTENSIONSSTRING)(Display * dpy, int screen);
extern PFN_GLXQUERYEXTENSIONSSTRING _glXQueryExtensionsString;

typedef const char * (* PFN_GLXQUERYSERVERSTRING)(Display * dpy, int screen, int name);
extern PFN_GLXQUERYSERVERSTRING _glXQueryServerString;

typedef const char * (* PFN_GLXGETCLIENTSTRING)(Display * dpy, int name);
extern PFN_GLXGETCLIENTSTRING _glXGetClientString;

typedef Display * (* PFN_GLXGETCURRENTDISPLAY)(void);
extern PFN_GLXGETCURRENTDISPLAY _glXGetCurrentDisplay;

typedef GLXFBConfig * (* PFN_GLXCHOOSEFBCONFIG)(Display * dpy, int screen, const int * attribList, int * nitems);
extern PFN_GLXCHOOSEFBCONFIG _glXChooseFBConfig;

typedef int (* PFN_GLXGETFBCONFIGATTRIB)(Display * dpy, GLXFBConfig config, int attribute, int * value);
extern PFN_GLXGETFBCONFIGATTRIB _glXGetFBConfigAttrib;

typedef GLXFBConfig * (* PFN_GLXGETFBCONFIGS)(Display * dpy, int screen, int * nelements);
extern PFN_GLXGETFBCONFIGS _glXGetFBConfigs;

typedef XVisualInfo * (* PFN_GLXGETVISUALFROMFBCONFIG)(Display * dpy, GLXFBConfig config);
extern PFN_GLXGETVISUALFROMFBCONFIG _glXGetVisualFromFBConfig;

typedef GLXWindow (* PFN_GLXCREATEWINDOW)(Display * dpy, GLXFBConfig config, Window win, const int * attribList);
extern PFN_GLXCREATEWINDOW _glXCreateWindow;

typedef void (* PFN_GLXDESTROYWINDOW)(Display * dpy, GLXWindow window);
extern PFN_GLXDESTROYWINDOW _glXDestroyWindow;

typedef GLXPixmap (* PFN_GLXCREATEPIXMAP)(Display * dpy, GLXFBConfig config, Pixmap pixmap, const int * attribList);
extern PFN_GLXCREATEPIXMAP _glXCreatePixmap;

typedef void (* PFN_GLXDESTROYPIXMAP)(Display * dpy, GLXPixmap pixmap);
extern PFN_GLXDESTROYPIXMAP _glXDestroyPixmap;

typedef GLXPbuffer (* PFN_GLXCREATEPBUFFER)(Display * dpy, GLXFBConfig config, const int * attribList);
extern PFN_GLXCREATEPBUFFER _glXCreatePbuffer;

typedef void (* PFN_GLXDESTROYPBUFFER)(Display * dpy, GLXPbuffer pbuf);
extern PFN_GLXDESTROYPBUFFER _glXDestroyPbuffer;

typedef void (* PFN_GLXQUERYDRAWABLE)(Display * dpy, GLXDrawable draw, int attribute, unsigned int * value);
extern PFN_GLXQUERYDRAWABLE _glXQueryDrawable;

typedef GLXContext (* PFN_GLXCREATENEWCONTEXT)(Display * dpy, GLXFBConfig config, int renderType, GLXContext shareList, int direct);
extern PFN_GLXCREATENEWCONTEXT _glXCreateNewContext;

typedef int (* PFN_GLXMAKECONTEXTCURRENT)(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
extern PFN_GLXMAKECONTEXTCURRENT _glXMakeContextCurrent;

typedef GLXDrawable (* PFN_GLXGETCURRENTREADDRAWABLE)(void);
extern PFN_GLXGETCURRENTREADDRAWABLE _glXGetCurrentReadDrawable;

typedef int (* PFN_GLXQUERYCONTEXT)(Display * dpy, GLXContext ctx, int attribute, int * value);
extern PFN_GLXQUERYCONTEXT _glXQueryContext;

typedef void (* PFN_GLXSELECTEVENT)(Display * dpy, GLXDrawable drawable, unsigned long mask);
extern PFN_GLXSELECTEVENT _glXSelectEvent;

typedef void (* PFN_GLXGETSELECTEDEVENT)(Display * dpy, GLXDrawable drawable, unsigned long * mask);
extern PFN_GLXGETSELECTEDEVENT _glXGetSelectedEvent;

typedef GLXContext (* PFN_GLXCREATECONTEXTATTRIBSARB)(Display * dpy, GLXFBConfig config, GLXContext share_context, int direct, const int * attrib_list);
extern PFN_GLXCREATECONTEXTATTRIBSARB _glXCreateContextAttribsARB;

typedef Display * (* PFN_GLXGETCURRENTDISPLAYEXT)(void);
extern PFN_GLXGETCURRENTDISPLAYEXT _glXGetCurrentDisplayEXT;

typedef int (* PFN_GLXQUERYCONTEXTINFOEXT)(Display * dpy, GLXContext context, int attribute, int * value);
extern PFN_GLXQUERYCONTEXTINFOEXT _glXQueryContextInfoEXT;

typedef GLXContextID (* PFN_GLXGETCONTEXTIDEXT)(const GLXContext context);
extern PFN_GLXGETCONTEXTIDEXT _glXGetContextIDEXT;

typedef GLXContext (* PFN_GLXIMPORTCONTEXTEXT)(Display * dpy, GLXContextID contextID);
extern PFN_GLXIMPORTCONTEXTEXT _glXImportContextEXT;

typedef void (* PFN_GLXFREECONTEXTEXT)(Display * dpy, GLXContext context);
extern PFN_GLXFREECONTEXTEXT _glXFreeContextEXT;

typedef void (* PFN_GLXSWAPINTERVALEXT)(Display * dpy, GLXDrawable drawable, int interval);
extern PFN_GLXSWAPINTERVALEXT _glXSwapIntervalEXT;

typedef void (* PFN_GLXBINDTEXIMAGEEXT)(Display * display, GLXDrawable drawable, int buffer, const int * attrib_list);
extern PFN_GLXBINDTEXIMAGEEXT _glXBindTexImageEXT;

typedef void (* PFN_GLXRELEASETEXIMAGEEXT)(Display * display, GLXDrawable drawable, int buffer);
extern PFN_GLXRELEASETEXIMAGEEXT _glXReleaseTexImageEXT;

typedef unsigned int (* PFN_GLXGETAGPOFFSETMESA)(const void * pointer);
extern PFN_GLXGETAGPOFFSETMESA _glXGetAGPOffsetMESA;

typedef void (* PFN_GLXCOPYSUBBUFFERMESA)(Display * dpy, GLXDrawable drawable, int x, int y, int width, int height);
extern PFN_GLXCOPYSUBBUFFERMESA _glXCopySubBufferMESA;

typedef GLXPixmap (* PFN_GLXCREATEGLXPIXMAPMESA)(Display * dpy, XVisualInfo * visual, Pixmap pixmap, Colormap cmap);
extern PFN_GLXCREATEGLXPIXMAPMESA _glXCreateGLXPixmapMESA;

typedef int (* PFN_GLXRELEASEBUFFERSMESA)(Display * dpy, GLXDrawable drawable);
extern PFN_GLXRELEASEBUFFERSMESA _glXReleaseBuffersMESA;

typedef int (* PFN_GLXSET3DFXMODEMESA)(int mode);
extern PFN_GLXSET3DFXMODEMESA _glXSet3DfxModeMESA;

typedef int (* PFN_GLXSWAPINTERVALMESA)(unsigned int interval);
extern PFN_GLXSWAPINTERVALMESA _glXSwapIntervalMESA;

typedef int (* PFN_GLXGETSWAPINTERVALMESA)(void);
extern PFN_GLXGETSWAPINTERVALMESA _glXGetSwapIntervalMESA;

typedef void (* PFN_GLXCOPYIMAGESUBDATANV)(Display * dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLXCOPYIMAGESUBDATANV _glXCopyImageSubDataNV;

typedef unsigned int * (* PFN_GLXENUMERATEVIDEODEVICESNV)(Display * dpy, int screen, int * nelements);
extern PFN_GLXENUMERATEVIDEODEVICESNV _glXEnumerateVideoDevicesNV;

typedef int (* PFN_GLXBINDVIDEODEVICENV)(Display * dpy, unsigned int video_slot, unsigned int video_device, const int * attrib_list);
extern PFN_GLXBINDVIDEODEVICENV _glXBindVideoDeviceNV;

typedef int (* PFN_GLXJOINSWAPGROUPNV)(Display * dpy, GLXDrawable drawable, GLuint group);
extern PFN_GLXJOINSWAPGROUPNV _glXJoinSwapGroupNV;

typedef int (* PFN_GLXBINDSWAPBARRIERNV)(Display * dpy, GLuint group, GLuint barrier);
extern PFN_GLXBINDSWAPBARRIERNV _glXBindSwapBarrierNV;

typedef int (* PFN_GLXQUERYSWAPGROUPNV)(Display * dpy, GLXDrawable drawable, GLuint * group, GLuint * barrier);
extern PFN_GLXQUERYSWAPGROUPNV _glXQuerySwapGroupNV;

typedef int (* PFN_GLXQUERYMAXSWAPGROUPSNV)(Display * dpy, int screen, GLuint * maxGroups, GLuint * maxBarriers);
extern PFN_GLXQUERYMAXSWAPGROUPSNV _glXQueryMaxSwapGroupsNV;

typedef int (* PFN_GLXQUERYFRAMECOUNTNV)(Display * dpy, int screen, GLuint * count);
extern PFN_GLXQUERYFRAMECOUNTNV _glXQueryFrameCountNV;

typedef int (* PFN_GLXRESETFRAMECOUNTNV)(Display * dpy, int screen);
extern PFN_GLXRESETFRAMECOUNTNV _glXResetFrameCountNV;

typedef void * (* PFN_GLXALLOCATEMEMORYNV)(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
extern PFN_GLXALLOCATEMEMORYNV _glXAllocateMemoryNV;

typedef void (* PFN_GLXFREEMEMORYNV)(void * pointer);
extern PFN_GLXFREEMEMORYNV _glXFreeMemoryNV;

typedef int (* PFN_GLXBINDVIDEOCAPTUREDEVICENV)(Display * dpy, unsigned int video_capture_slot, GLXVideoCaptureDeviceNV device);
extern PFN_GLXBINDVIDEOCAPTUREDEVICENV _glXBindVideoCaptureDeviceNV;

typedef GLXVideoCaptureDeviceNV * (* PFN_GLXENUMERATEVIDEOCAPTUREDEVICESNV)(Display * dpy, int screen, int * nelements);
extern PFN_GLXENUMERATEVIDEOCAPTUREDEVICESNV _glXEnumerateVideoCaptureDevicesNV;

typedef void (* PFN_GLXLOCKVIDEOCAPTUREDEVICENV)(Display * dpy, GLXVideoCaptureDeviceNV device);
extern PFN_GLXLOCKVIDEOCAPTUREDEVICENV _glXLockVideoCaptureDeviceNV;

typedef int (* PFN_GLXQUERYVIDEOCAPTUREDEVICENV)(Display * dpy, GLXVideoCaptureDeviceNV device, int attribute, int * value);
extern PFN_GLXQUERYVIDEOCAPTUREDEVICENV _glXQueryVideoCaptureDeviceNV;

typedef void (* PFN_GLXRELEASEVIDEOCAPTUREDEVICENV)(Display * dpy, GLXVideoCaptureDeviceNV device);
extern PFN_GLXRELEASEVIDEOCAPTUREDEVICENV _glXReleaseVideoCaptureDeviceNV;

typedef int (* PFN_GLXGETVIDEODEVICENV)(Display * dpy, int screen, int numVideoDevices, GLXVideoDeviceNV * pVideoDevice);
extern PFN_GLXGETVIDEODEVICENV _glXGetVideoDeviceNV;

typedef int (* PFN_GLXRELEASEVIDEODEVICENV)(Display * dpy, int screen, GLXVideoDeviceNV VideoDevice);
extern PFN_GLXRELEASEVIDEODEVICENV _glXReleaseVideoDeviceNV;

typedef int (* PFN_GLXBINDVIDEOIMAGENV)(Display * dpy, GLXVideoDeviceNV VideoDevice, GLXPbuffer pbuf, int iVideoBuffer);
extern PFN_GLXBINDVIDEOIMAGENV _glXBindVideoImageNV;

typedef int (* PFN_GLXRELEASEVIDEOIMAGENV)(Display * dpy, GLXPbuffer pbuf);
extern PFN_GLXRELEASEVIDEOIMAGENV _glXReleaseVideoImageNV;

typedef int (* PFN_GLXSENDPBUFFERTOVIDEONV)(Display * dpy, GLXPbuffer pbuf, int iBufferType, unsigned long * pulCounterPbuffer, GLboolean bBlock);
extern PFN_GLXSENDPBUFFERTOVIDEONV _glXSendPbufferToVideoNV;

typedef int (* PFN_GLXGETVIDEOINFONV)(Display * dpy, int screen, GLXVideoDeviceNV VideoDevice, unsigned long * pulCounterOutputPbuffer, unsigned long * pulCounterOutputVideo);
extern PFN_GLXGETVIDEOINFONV _glXGetVideoInfoNV;

typedef int (* PFN_GLXGETSYNCVALUESOML)(Display * dpy, GLXDrawable drawable, int64_t * ust, int64_t * msc, int64_t * sbc);
extern PFN_GLXGETSYNCVALUESOML _glXGetSyncValuesOML;

typedef int (* PFN_GLXGETMSCRATEOML)(Display * dpy, GLXDrawable drawable, int32_t * numerator, int32_t * denominator);
extern PFN_GLXGETMSCRATEOML _glXGetMscRateOML;

typedef int64_t (* PFN_GLXSWAPBUFFERSMSCOML)(Display * dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder);
extern PFN_GLXSWAPBUFFERSMSCOML _glXSwapBuffersMscOML;

typedef int (* PFN_GLXWAITFORMSCOML)(Display * dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder, int64_t * ust, int64_t * msc, int64_t * sbc);
extern PFN_GLXWAITFORMSCOML _glXWaitForMscOML;

typedef int (* PFN_GLXWAITFORSBCOML)(Display * dpy, GLXDrawable drawable, int64_t target_sbc, int64_t * ust, int64_t * msc, int64_t * sbc);
extern PFN_GLXWAITFORSBCOML _glXWaitForSbcOML;

typedef void (* PFN_GLXCUSHIONSGI)(Display * dpy, Window window, float cushion);
extern PFN_GLXCUSHIONSGI _glXCushionSGI;

typedef int (* PFN_GLXMAKECURRENTREADSGI)(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
extern PFN_GLXMAKECURRENTREADSGI _glXMakeCurrentReadSGI;

typedef GLXDrawable (* PFN_GLXGETCURRENTREADDRAWABLESGI)(void);
extern PFN_GLXGETCURRENTREADDRAWABLESGI _glXGetCurrentReadDrawableSGI;

typedef int (* PFN_GLXSWAPINTERVALSGI)(int interval);
extern PFN_GLXSWAPINTERVALSGI _glXSwapIntervalSGI;

typedef int (* PFN_GLXGETVIDEOSYNCSGI)(unsigned int * count);
extern PFN_GLXGETVIDEOSYNCSGI _glXGetVideoSyncSGI;

typedef int (* PFN_GLXWAITVIDEOSYNCSGI)(int divisor, int remainder, unsigned int * count);
extern PFN_GLXWAITVIDEOSYNCSGI _glXWaitVideoSyncSGI;

typedef int (* PFN_GLXGETFBCONFIGATTRIBSGIX)(Display * dpy, GLXFBConfigSGIX config, int attribute, int * value);
extern PFN_GLXGETFBCONFIGATTRIBSGIX _glXGetFBConfigAttribSGIX;

typedef GLXFBConfigSGIX * (* PFN_GLXCHOOSEFBCONFIGSGIX)(Display * dpy, int screen, int * attrib_list, int * nelements);
extern PFN_GLXCHOOSEFBCONFIGSGIX _glXChooseFBConfigSGIX;

typedef GLXPixmap (* PFN_GLXCREATEGLXPIXMAPWITHCONFIGSGIX)(Display * dpy, GLXFBConfigSGIX config, Pixmap pixmap);
extern PFN_GLXCREATEGLXPIXMAPWITHCONFIGSGIX _glXCreateGLXPixmapWithConfigSGIX;

typedef GLXContext (* PFN_GLXCREATECONTEXTWITHCONFIGSGIX)(Display * dpy, GLXFBConfigSGIX config, int render_type, GLXContext share_list, int direct);
extern PFN_GLXCREATECONTEXTWITHCONFIGSGIX _glXCreateContextWithConfigSGIX;

typedef XVisualInfo * (* PFN_GLXGETVISUALFROMFBCONFIGSGIX)(Display * dpy, GLXFBConfigSGIX config);
extern PFN_GLXGETVISUALFROMFBCONFIGSGIX _glXGetVisualFromFBConfigSGIX;

typedef GLXFBConfigSGIX (* PFN_GLXGETFBCONFIGFROMVISUALSGIX)(Display * dpy, XVisualInfo * vis);
extern PFN_GLXGETFBCONFIGFROMVISUALSGIX _glXGetFBConfigFromVisualSGIX;

typedef GLXPbufferSGIX (* PFN_GLXCREATEGLXPBUFFERSGIX)(Display * dpy, GLXFBConfigSGIX config, unsigned int width, unsigned int height, int * attrib_list);
extern PFN_GLXCREATEGLXPBUFFERSGIX _glXCreateGLXPbufferSGIX;

typedef void (* PFN_GLXDESTROYGLXPBUFFERSGIX)(Display * dpy, GLXPbufferSGIX pbuf);
extern PFN_GLXDESTROYGLXPBUFFERSGIX _glXDestroyGLXPbufferSGIX;

typedef int (* PFN_GLXQUERYGLXPBUFFERSGIX)(Display * dpy, GLXPbufferSGIX pbuf, int attribute, unsigned int * value);
extern PFN_GLXQUERYGLXPBUFFERSGIX _glXQueryGLXPbufferSGIX;

typedef void (* PFN_GLXSELECTEVENTSGIX)(Display * dpy, GLXDrawable drawable, unsigned long mask);
extern PFN_GLXSELECTEVENTSGIX _glXSelectEventSGIX;

typedef void (* PFN_GLXGETSELECTEDEVENTSGIX)(Display * dpy, GLXDrawable drawable, unsigned long * mask);
extern PFN_GLXGETSELECTEDEVENTSGIX _glXGetSelectedEventSGIX;

typedef void (* PFN_GLXBINDSWAPBARRIERSGIX)(Display * dpy, GLXDrawable drawable, int barrier);
extern PFN_GLXBINDSWAPBARRIERSGIX _glXBindSwapBarrierSGIX;

typedef int (* PFN_GLXQUERYMAXSWAPBARRIERSSGIX)(Display * dpy, int screen, int * max);
extern PFN_GLXQUERYMAXSWAPBARRIERSSGIX _glXQueryMaxSwapBarriersSGIX;

typedef void (* PFN_GLXJOINSWAPGROUPSGIX)(Display * dpy, GLXDrawable drawable, GLXDrawable member);
extern PFN_GLXJOINSWAPGROUPSGIX _glXJoinSwapGroupSGIX;

typedef int (* PFN_GLXBINDCHANNELTOWINDOWSGIX)(Display * display, int screen, int channel, Window window);
extern PFN_GLXBINDCHANNELTOWINDOWSGIX _glXBindChannelToWindowSGIX;

typedef int (* PFN_GLXCHANNELRECTSGIX)(Display * display, int screen, int channel, int x, int y, int w, int h);
extern PFN_GLXCHANNELRECTSGIX _glXChannelRectSGIX;

typedef int (* PFN_GLXQUERYCHANNELRECTSGIX)(Display * display, int screen, int channel, int * dx, int * dy, int * dw, int * dh);
extern PFN_GLXQUERYCHANNELRECTSGIX _glXQueryChannelRectSGIX;

typedef int (* PFN_GLXQUERYCHANNELDELTASSGIX)(Display * display, int screen, int channel, int * x, int * y, int * w, int * h);
extern PFN_GLXQUERYCHANNELDELTASSGIX _glXQueryChannelDeltasSGIX;

typedef int (* PFN_GLXCHANNELRECTSYNCSGIX)(Display * display, int screen, int channel, GLenum synctype);
extern PFN_GLXCHANNELRECTSYNCSGIX _glXChannelRectSyncSGIX;

typedef __GLXextFuncPtr (* PFN_GLXGETPROCADDRESSARB)(const GLubyte * procName);
extern PFN_GLXGETPROCADDRESSARB _glXGetProcAddressARB;

typedef __GLXextFuncPtr (* PFN_GLXGETPROCADDRESS)(const GLubyte * procName);
extern PFN_GLXGETPROCADDRESS _glXGetProcAddress;

#ifdef RETRACE
#define glXChooseVisual _glXChooseVisual
#define glXCreateContext _glXCreateContext
#define glXDestroyContext _glXDestroyContext
#define glXMakeCurrent _glXMakeCurrent
#define glXCopyContext _glXCopyContext
#define glXSwapBuffers _glXSwapBuffers
#define glXCreateGLXPixmap _glXCreateGLXPixmap
#define glXDestroyGLXPixmap _glXDestroyGLXPixmap
#define glXQueryExtension _glXQueryExtension
#define glXQueryVersion _glXQueryVersion
#define glXIsDirect _glXIsDirect
#define glXGetConfig _glXGetConfig
#define glXGetCurrentContext _glXGetCurrentContext
#define glXGetCurrentDrawable _glXGetCurrentDrawable
#define glXWaitGL _glXWaitGL
#define glXWaitX _glXWaitX
#define glXUseXFont _glXUseXFont
#define glXQueryExtensionsString _glXQueryExtensionsString
#define glXQueryServerString _glXQueryServerString
#define glXGetClientString _glXGetClientString
#define glXGetCurrentDisplay _glXGetCurrentDisplay
#define glXChooseFBConfig _glXChooseFBConfig
#define glXGetFBConfigAttrib _glXGetFBConfigAttrib
#define glXGetFBConfigs _glXGetFBConfigs
#define glXGetVisualFromFBConfig _glXGetVisualFromFBConfig
#define glXCreateWindow _glXCreateWindow
#define glXDestroyWindow _glXDestroyWindow
#define glXCreatePixmap _glXCreatePixmap
#define glXDestroyPixmap _glXDestroyPixmap
#define glXCreatePbuffer _glXCreatePbuffer
#define glXDestroyPbuffer _glXDestroyPbuffer
#define glXQueryDrawable _glXQueryDrawable
#define glXCreateNewContext _glXCreateNewContext
#define glXMakeContextCurrent _glXMakeContextCurrent
#define glXGetCurrentReadDrawable _glXGetCurrentReadDrawable
#define glXQueryContext _glXQueryContext
#define glXSelectEvent _glXSelectEvent
#define glXGetSelectedEvent _glXGetSelectedEvent
#define glXCreateContextAttribsARB _glXCreateContextAttribsARB
#define glXGetCurrentDisplayEXT _glXGetCurrentDisplayEXT
#define glXQueryContextInfoEXT _glXQueryContextInfoEXT
#define glXGetContextIDEXT _glXGetContextIDEXT
#define glXImportContextEXT _glXImportContextEXT
#define glXFreeContextEXT _glXFreeContextEXT
#define glXSwapIntervalEXT _glXSwapIntervalEXT
#define glXBindTexImageEXT _glXBindTexImageEXT
#define glXReleaseTexImageEXT _glXReleaseTexImageEXT
#define glXGetAGPOffsetMESA _glXGetAGPOffsetMESA
#define glXCopySubBufferMESA _glXCopySubBufferMESA
#define glXCreateGLXPixmapMESA _glXCreateGLXPixmapMESA
#define glXReleaseBuffersMESA _glXReleaseBuffersMESA
#define glXSet3DfxModeMESA _glXSet3DfxModeMESA
#define glXSwapIntervalMESA _glXSwapIntervalMESA
#define glXGetSwapIntervalMESA _glXGetSwapIntervalMESA
#define glXCopyImageSubDataNV _glXCopyImageSubDataNV
#define glXEnumerateVideoDevicesNV _glXEnumerateVideoDevicesNV
#define glXBindVideoDeviceNV _glXBindVideoDeviceNV
#define glXJoinSwapGroupNV _glXJoinSwapGroupNV
#define glXBindSwapBarrierNV _glXBindSwapBarrierNV
#define glXQuerySwapGroupNV _glXQuerySwapGroupNV
#define glXQueryMaxSwapGroupsNV _glXQueryMaxSwapGroupsNV
#define glXQueryFrameCountNV _glXQueryFrameCountNV
#define glXResetFrameCountNV _glXResetFrameCountNV
#define glXAllocateMemoryNV _glXAllocateMemoryNV
#define glXFreeMemoryNV _glXFreeMemoryNV
#define glXBindVideoCaptureDeviceNV _glXBindVideoCaptureDeviceNV
#define glXEnumerateVideoCaptureDevicesNV _glXEnumerateVideoCaptureDevicesNV
#define glXLockVideoCaptureDeviceNV _glXLockVideoCaptureDeviceNV
#define glXQueryVideoCaptureDeviceNV _glXQueryVideoCaptureDeviceNV
#define glXReleaseVideoCaptureDeviceNV _glXReleaseVideoCaptureDeviceNV
#define glXGetVideoDeviceNV _glXGetVideoDeviceNV
#define glXReleaseVideoDeviceNV _glXReleaseVideoDeviceNV
#define glXBindVideoImageNV _glXBindVideoImageNV
#define glXReleaseVideoImageNV _glXReleaseVideoImageNV
#define glXSendPbufferToVideoNV _glXSendPbufferToVideoNV
#define glXGetVideoInfoNV _glXGetVideoInfoNV
#define glXGetSyncValuesOML _glXGetSyncValuesOML
#define glXGetMscRateOML _glXGetMscRateOML
#define glXSwapBuffersMscOML _glXSwapBuffersMscOML
#define glXWaitForMscOML _glXWaitForMscOML
#define glXWaitForSbcOML _glXWaitForSbcOML
#define glXCushionSGI _glXCushionSGI
#define glXMakeCurrentReadSGI _glXMakeCurrentReadSGI
#define glXGetCurrentReadDrawableSGI _glXGetCurrentReadDrawableSGI
#define glXSwapIntervalSGI _glXSwapIntervalSGI
#define glXGetVideoSyncSGI _glXGetVideoSyncSGI
#define glXWaitVideoSyncSGI _glXWaitVideoSyncSGI
#define glXGetFBConfigAttribSGIX _glXGetFBConfigAttribSGIX
#define glXChooseFBConfigSGIX _glXChooseFBConfigSGIX
#define glXCreateGLXPixmapWithConfigSGIX _glXCreateGLXPixmapWithConfigSGIX
#define glXCreateContextWithConfigSGIX _glXCreateContextWithConfigSGIX
#define glXGetVisualFromFBConfigSGIX _glXGetVisualFromFBConfigSGIX
#define glXGetFBConfigFromVisualSGIX _glXGetFBConfigFromVisualSGIX
#define glXCreateGLXPbufferSGIX _glXCreateGLXPbufferSGIX
#define glXDestroyGLXPbufferSGIX _glXDestroyGLXPbufferSGIX
#define glXQueryGLXPbufferSGIX _glXQueryGLXPbufferSGIX
#define glXSelectEventSGIX _glXSelectEventSGIX
#define glXGetSelectedEventSGIX _glXGetSelectedEventSGIX
#define glXBindSwapBarrierSGIX _glXBindSwapBarrierSGIX
#define glXQueryMaxSwapBarriersSGIX _glXQueryMaxSwapBarriersSGIX
#define glXJoinSwapGroupSGIX _glXJoinSwapGroupSGIX
#define glXBindChannelToWindowSGIX _glXBindChannelToWindowSGIX
#define glXChannelRectSGIX _glXChannelRectSGIX
#define glXQueryChannelRectSGIX _glXQueryChannelRectSGIX
#define glXQueryChannelDeltasSGIX _glXQueryChannelDeltasSGIX
#define glXChannelRectSyncSGIX _glXChannelRectSyncSGIX
#define glXGetProcAddressARB _glXGetProcAddressARB
#define glXGetProcAddress _glXGetProcAddress
#endif /* RETRACE */


#endif

typedef void (APIENTRY * PFN_GLCULLFACE)(GLenum mode);
extern PFN_GLCULLFACE _glCullFace;

typedef void (APIENTRY * PFN_GLFRONTFACE)(GLenum mode);
extern PFN_GLFRONTFACE _glFrontFace;

typedef void (APIENTRY * PFN_GLHINT)(GLenum target, GLenum mode);
extern PFN_GLHINT _glHint;

typedef void (APIENTRY * PFN_GLLINEWIDTH)(GLfloat width);
extern PFN_GLLINEWIDTH _glLineWidth;

typedef void (APIENTRY * PFN_GLPOINTSIZE)(GLfloat size);
extern PFN_GLPOINTSIZE _glPointSize;

typedef void (APIENTRY * PFN_GLPOLYGONMODE)(GLenum face, GLenum mode);
extern PFN_GLPOLYGONMODE _glPolygonMode;

typedef void (APIENTRY * PFN_GLSCISSOR)(GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLSCISSOR _glScissor;

typedef void (APIENTRY * PFN_GLTEXPARAMETERF)(GLenum target, GLenum pname, GLfloat param);
extern PFN_GLTEXPARAMETERF _glTexParameterf;

typedef void (APIENTRY * PFN_GLTEXPARAMETERFV)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLTEXPARAMETERFV _glTexParameterfv;

typedef void (APIENTRY * PFN_GLTEXPARAMETERI)(GLenum target, GLenum pname, GLint param);
extern PFN_GLTEXPARAMETERI _glTexParameteri;

typedef void (APIENTRY * PFN_GLTEXPARAMETERIV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXPARAMETERIV _glTexParameteriv;

typedef void (APIENTRY * PFN_GLTEXIMAGE1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE1D _glTexImage1D;

typedef void (APIENTRY * PFN_GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE2D _glTexImage2D;

typedef void (APIENTRY * PFN_GLDRAWBUFFER)(GLenum mode);
extern PFN_GLDRAWBUFFER _glDrawBuffer;

typedef void (APIENTRY * PFN_GLCLEAR)(GLbitfield mask);
extern PFN_GLCLEAR _glClear;

typedef void (APIENTRY * PFN_GLCLEARCOLOR)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFN_GLCLEARCOLOR _glClearColor;

typedef void (APIENTRY * PFN_GLCLEARSTENCIL)(GLint s);
extern PFN_GLCLEARSTENCIL _glClearStencil;

typedef void (APIENTRY * PFN_GLCLEARDEPTH)(GLdouble depth);
extern PFN_GLCLEARDEPTH _glClearDepth;

typedef void (APIENTRY * PFN_GLSTENCILMASK)(GLuint mask);
extern PFN_GLSTENCILMASK _glStencilMask;

typedef void (APIENTRY * PFN_GLCOLORMASK)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern PFN_GLCOLORMASK _glColorMask;

typedef void (APIENTRY * PFN_GLDEPTHMASK)(GLboolean flag);
extern PFN_GLDEPTHMASK _glDepthMask;

typedef void (APIENTRY * PFN_GLDISABLE)(GLenum cap);
extern PFN_GLDISABLE _glDisable;

typedef void (APIENTRY * PFN_GLENABLE)(GLenum cap);
extern PFN_GLENABLE _glEnable;

typedef void (APIENTRY * PFN_GLFINISH)(void);
extern PFN_GLFINISH _glFinish;

typedef void (APIENTRY * PFN_GLFLUSH)(void);
extern PFN_GLFLUSH _glFlush;

typedef void (APIENTRY * PFN_GLBLENDFUNC)(GLenum sfactor, GLenum dfactor);
extern PFN_GLBLENDFUNC _glBlendFunc;

typedef void (APIENTRY * PFN_GLLOGICOP)(GLenum opcode);
extern PFN_GLLOGICOP _glLogicOp;

typedef void (APIENTRY * PFN_GLSTENCILFUNC)(GLenum func, GLint ref, GLuint mask);
extern PFN_GLSTENCILFUNC _glStencilFunc;

typedef void (APIENTRY * PFN_GLSTENCILOP)(GLenum fail, GLenum zfail, GLenum zpass);
extern PFN_GLSTENCILOP _glStencilOp;

typedef void (APIENTRY * PFN_GLDEPTHFUNC)(GLenum func);
extern PFN_GLDEPTHFUNC _glDepthFunc;

typedef void (APIENTRY * PFN_GLPIXELSTOREF)(GLenum pname, GLfloat param);
extern PFN_GLPIXELSTOREF _glPixelStoref;

typedef void (APIENTRY * PFN_GLPIXELSTOREI)(GLenum pname, GLint param);
extern PFN_GLPIXELSTOREI _glPixelStorei;

typedef void (APIENTRY * PFN_GLREADBUFFER)(GLenum mode);
extern PFN_GLREADBUFFER _glReadBuffer;

typedef void (APIENTRY * PFN_GLREADPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels);
extern PFN_GLREADPIXELS _glReadPixels;

typedef void (APIENTRY * PFN_GLGETBOOLEANV)(GLenum pname, GLboolean * params);
extern PFN_GLGETBOOLEANV _glGetBooleanv;

typedef void (APIENTRY * PFN_GLGETDOUBLEV)(GLenum pname, GLdouble * params);
extern PFN_GLGETDOUBLEV _glGetDoublev;

typedef GLenum (APIENTRY * PFN_GLGETERROR)(void);
extern PFN_GLGETERROR _glGetError;

typedef void (APIENTRY * PFN_GLGETFLOATV)(GLenum pname, GLfloat * params);
extern PFN_GLGETFLOATV _glGetFloatv;

typedef void (APIENTRY * PFN_GLGETINTEGERV)(GLenum pname, GLint * params);
extern PFN_GLGETINTEGERV _glGetIntegerv;

typedef const GLubyte * (APIENTRY * PFN_GLGETSTRING)(GLenum name);
extern PFN_GLGETSTRING _glGetString;

typedef void (APIENTRY * PFN_GLGETTEXIMAGE)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
extern PFN_GLGETTEXIMAGE _glGetTexImage;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXPARAMETERFV _glGetTexParameterfv;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXPARAMETERIV _glGetTexParameteriv;

typedef void (APIENTRY * PFN_GLGETTEXLEVELPARAMETERFV)(GLenum target, GLint level, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXLEVELPARAMETERFV _glGetTexLevelParameterfv;

typedef void (APIENTRY * PFN_GLGETTEXLEVELPARAMETERIV)(GLenum target, GLint level, GLenum pname, GLint * params);
extern PFN_GLGETTEXLEVELPARAMETERIV _glGetTexLevelParameteriv;

typedef GLboolean (APIENTRY * PFN_GLISENABLED)(GLenum cap);
extern PFN_GLISENABLED _glIsEnabled;

typedef void (APIENTRY * PFN_GLDEPTHRANGE)(GLdouble zNear, GLdouble zFar);
extern PFN_GLDEPTHRANGE _glDepthRange;

typedef void (APIENTRY * PFN_GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLVIEWPORT _glViewport;

typedef void (APIENTRY * PFN_GLNEWLIST)(GLuint list, GLenum mode);
extern PFN_GLNEWLIST _glNewList;

typedef void (APIENTRY * PFN_GLENDLIST)(void);
extern PFN_GLENDLIST _glEndList;

typedef void (APIENTRY * PFN_GLCALLLIST)(GLuint list);
extern PFN_GLCALLLIST _glCallList;

typedef void (APIENTRY * PFN_GLCALLLISTS)(GLsizei n, GLenum type, const GLvoid * lists);
extern PFN_GLCALLLISTS _glCallLists;

typedef void (APIENTRY * PFN_GLDELETELISTS)(GLuint list, GLsizei range);
extern PFN_GLDELETELISTS _glDeleteLists;

typedef GLuint (APIENTRY * PFN_GLGENLISTS)(GLsizei range);
extern PFN_GLGENLISTS _glGenLists;

typedef void (APIENTRY * PFN_GLLISTBASE)(GLuint base);
extern PFN_GLLISTBASE _glListBase;

typedef void (APIENTRY * PFN_GLBEGIN)(GLenum mode);
extern PFN_GLBEGIN _glBegin;

typedef void (APIENTRY * PFN_GLBITMAP)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
extern PFN_GLBITMAP _glBitmap;

typedef void (APIENTRY * PFN_GLCOLOR3B)(GLbyte red, GLbyte green, GLbyte blue);
extern PFN_GLCOLOR3B _glColor3b;

typedef void (APIENTRY * PFN_GLCOLOR3BV)(const GLbyte * v);
extern PFN_GLCOLOR3BV _glColor3bv;

typedef void (APIENTRY * PFN_GLCOLOR3D)(GLdouble red, GLdouble green, GLdouble blue);
extern PFN_GLCOLOR3D _glColor3d;

typedef void (APIENTRY * PFN_GLCOLOR3DV)(const GLdouble * v);
extern PFN_GLCOLOR3DV _glColor3dv;

typedef void (APIENTRY * PFN_GLCOLOR3F)(GLfloat red, GLfloat green, GLfloat blue);
extern PFN_GLCOLOR3F _glColor3f;

typedef void (APIENTRY * PFN_GLCOLOR3FV)(const GLfloat * v);
extern PFN_GLCOLOR3FV _glColor3fv;

typedef void (APIENTRY * PFN_GLCOLOR3I)(GLint red, GLint green, GLint blue);
extern PFN_GLCOLOR3I _glColor3i;

typedef void (APIENTRY * PFN_GLCOLOR3IV)(const GLint * v);
extern PFN_GLCOLOR3IV _glColor3iv;

typedef void (APIENTRY * PFN_GLCOLOR3S)(GLshort red, GLshort green, GLshort blue);
extern PFN_GLCOLOR3S _glColor3s;

typedef void (APIENTRY * PFN_GLCOLOR3SV)(const GLshort * v);
extern PFN_GLCOLOR3SV _glColor3sv;

typedef void (APIENTRY * PFN_GLCOLOR3UB)(GLubyte red, GLubyte green, GLubyte blue);
extern PFN_GLCOLOR3UB _glColor3ub;

typedef void (APIENTRY * PFN_GLCOLOR3UBV)(const GLubyte * v);
extern PFN_GLCOLOR3UBV _glColor3ubv;

typedef void (APIENTRY * PFN_GLCOLOR3UI)(GLuint red, GLuint green, GLuint blue);
extern PFN_GLCOLOR3UI _glColor3ui;

typedef void (APIENTRY * PFN_GLCOLOR3UIV)(const GLuint * v);
extern PFN_GLCOLOR3UIV _glColor3uiv;

typedef void (APIENTRY * PFN_GLCOLOR3US)(GLushort red, GLushort green, GLushort blue);
extern PFN_GLCOLOR3US _glColor3us;

typedef void (APIENTRY * PFN_GLCOLOR3USV)(const GLushort * v);
extern PFN_GLCOLOR3USV _glColor3usv;

typedef void (APIENTRY * PFN_GLCOLOR4B)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern PFN_GLCOLOR4B _glColor4b;

typedef void (APIENTRY * PFN_GLCOLOR4BV)(const GLbyte * v);
extern PFN_GLCOLOR4BV _glColor4bv;

typedef void (APIENTRY * PFN_GLCOLOR4D)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern PFN_GLCOLOR4D _glColor4d;

typedef void (APIENTRY * PFN_GLCOLOR4DV)(const GLdouble * v);
extern PFN_GLCOLOR4DV _glColor4dv;

typedef void (APIENTRY * PFN_GLCOLOR4F)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFN_GLCOLOR4F _glColor4f;

typedef void (APIENTRY * PFN_GLCOLOR4FV)(const GLfloat * v);
extern PFN_GLCOLOR4FV _glColor4fv;

typedef void (APIENTRY * PFN_GLCOLOR4I)(GLint red, GLint green, GLint blue, GLint alpha);
extern PFN_GLCOLOR4I _glColor4i;

typedef void (APIENTRY * PFN_GLCOLOR4IV)(const GLint * v);
extern PFN_GLCOLOR4IV _glColor4iv;

typedef void (APIENTRY * PFN_GLCOLOR4S)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern PFN_GLCOLOR4S _glColor4s;

typedef void (APIENTRY * PFN_GLCOLOR4SV)(const GLshort * v);
extern PFN_GLCOLOR4SV _glColor4sv;

typedef void (APIENTRY * PFN_GLCOLOR4UB)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern PFN_GLCOLOR4UB _glColor4ub;

typedef void (APIENTRY * PFN_GLCOLOR4UBV)(const GLubyte * v);
extern PFN_GLCOLOR4UBV _glColor4ubv;

typedef void (APIENTRY * PFN_GLCOLOR4UI)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern PFN_GLCOLOR4UI _glColor4ui;

typedef void (APIENTRY * PFN_GLCOLOR4UIV)(const GLuint * v);
extern PFN_GLCOLOR4UIV _glColor4uiv;

typedef void (APIENTRY * PFN_GLCOLOR4US)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern PFN_GLCOLOR4US _glColor4us;

typedef void (APIENTRY * PFN_GLCOLOR4USV)(const GLushort * v);
extern PFN_GLCOLOR4USV _glColor4usv;

typedef void (APIENTRY * PFN_GLEDGEFLAG)(GLboolean flag);
extern PFN_GLEDGEFLAG _glEdgeFlag;

typedef void (APIENTRY * PFN_GLEDGEFLAGV)(const GLboolean * flag);
extern PFN_GLEDGEFLAGV _glEdgeFlagv;

typedef void (APIENTRY * PFN_GLEND)(void);
extern PFN_GLEND _glEnd;

typedef void (APIENTRY * PFN_GLINDEXD)(GLdouble c);
extern PFN_GLINDEXD _glIndexd;

typedef void (APIENTRY * PFN_GLINDEXDV)(const GLdouble * c);
extern PFN_GLINDEXDV _glIndexdv;

typedef void (APIENTRY * PFN_GLINDEXF)(GLfloat c);
extern PFN_GLINDEXF _glIndexf;

typedef void (APIENTRY * PFN_GLINDEXFV)(const GLfloat * c);
extern PFN_GLINDEXFV _glIndexfv;

typedef void (APIENTRY * PFN_GLINDEXI)(GLint c);
extern PFN_GLINDEXI _glIndexi;

typedef void (APIENTRY * PFN_GLINDEXIV)(const GLint * c);
extern PFN_GLINDEXIV _glIndexiv;

typedef void (APIENTRY * PFN_GLINDEXS)(GLshort c);
extern PFN_GLINDEXS _glIndexs;

typedef void (APIENTRY * PFN_GLINDEXSV)(const GLshort * c);
extern PFN_GLINDEXSV _glIndexsv;

typedef void (APIENTRY * PFN_GLNORMAL3B)(GLbyte nx, GLbyte ny, GLbyte nz);
extern PFN_GLNORMAL3B _glNormal3b;

typedef void (APIENTRY * PFN_GLNORMAL3BV)(const GLbyte * v);
extern PFN_GLNORMAL3BV _glNormal3bv;

typedef void (APIENTRY * PFN_GLNORMAL3D)(GLdouble nx, GLdouble ny, GLdouble nz);
extern PFN_GLNORMAL3D _glNormal3d;

typedef void (APIENTRY * PFN_GLNORMAL3DV)(const GLdouble * v);
extern PFN_GLNORMAL3DV _glNormal3dv;

typedef void (APIENTRY * PFN_GLNORMAL3F)(GLfloat nx, GLfloat ny, GLfloat nz);
extern PFN_GLNORMAL3F _glNormal3f;

typedef void (APIENTRY * PFN_GLNORMAL3FV)(const GLfloat * v);
extern PFN_GLNORMAL3FV _glNormal3fv;

typedef void (APIENTRY * PFN_GLNORMAL3I)(GLint nx, GLint ny, GLint nz);
extern PFN_GLNORMAL3I _glNormal3i;

typedef void (APIENTRY * PFN_GLNORMAL3IV)(const GLint * v);
extern PFN_GLNORMAL3IV _glNormal3iv;

typedef void (APIENTRY * PFN_GLNORMAL3S)(GLshort nx, GLshort ny, GLshort nz);
extern PFN_GLNORMAL3S _glNormal3s;

typedef void (APIENTRY * PFN_GLNORMAL3SV)(const GLshort * v);
extern PFN_GLNORMAL3SV _glNormal3sv;

typedef void (APIENTRY * PFN_GLRASTERPOS2D)(GLdouble x, GLdouble y);
extern PFN_GLRASTERPOS2D _glRasterPos2d;

typedef void (APIENTRY * PFN_GLRASTERPOS2DV)(const GLdouble * v);
extern PFN_GLRASTERPOS2DV _glRasterPos2dv;

typedef void (APIENTRY * PFN_GLRASTERPOS2F)(GLfloat x, GLfloat y);
extern PFN_GLRASTERPOS2F _glRasterPos2f;

typedef void (APIENTRY * PFN_GLRASTERPOS2FV)(const GLfloat * v);
extern PFN_GLRASTERPOS2FV _glRasterPos2fv;

typedef void (APIENTRY * PFN_GLRASTERPOS2I)(GLint x, GLint y);
extern PFN_GLRASTERPOS2I _glRasterPos2i;

typedef void (APIENTRY * PFN_GLRASTERPOS2IV)(const GLint * v);
extern PFN_GLRASTERPOS2IV _glRasterPos2iv;

typedef void (APIENTRY * PFN_GLRASTERPOS2S)(GLshort x, GLshort y);
extern PFN_GLRASTERPOS2S _glRasterPos2s;

typedef void (APIENTRY * PFN_GLRASTERPOS2SV)(const GLshort * v);
extern PFN_GLRASTERPOS2SV _glRasterPos2sv;

typedef void (APIENTRY * PFN_GLRASTERPOS3D)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLRASTERPOS3D _glRasterPos3d;

typedef void (APIENTRY * PFN_GLRASTERPOS3DV)(const GLdouble * v);
extern PFN_GLRASTERPOS3DV _glRasterPos3dv;

typedef void (APIENTRY * PFN_GLRASTERPOS3F)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLRASTERPOS3F _glRasterPos3f;

typedef void (APIENTRY * PFN_GLRASTERPOS3FV)(const GLfloat * v);
extern PFN_GLRASTERPOS3FV _glRasterPos3fv;

typedef void (APIENTRY * PFN_GLRASTERPOS3I)(GLint x, GLint y, GLint z);
extern PFN_GLRASTERPOS3I _glRasterPos3i;

typedef void (APIENTRY * PFN_GLRASTERPOS3IV)(const GLint * v);
extern PFN_GLRASTERPOS3IV _glRasterPos3iv;

typedef void (APIENTRY * PFN_GLRASTERPOS3S)(GLshort x, GLshort y, GLshort z);
extern PFN_GLRASTERPOS3S _glRasterPos3s;

typedef void (APIENTRY * PFN_GLRASTERPOS3SV)(const GLshort * v);
extern PFN_GLRASTERPOS3SV _glRasterPos3sv;

typedef void (APIENTRY * PFN_GLRASTERPOS4D)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLRASTERPOS4D _glRasterPos4d;

typedef void (APIENTRY * PFN_GLRASTERPOS4DV)(const GLdouble * v);
extern PFN_GLRASTERPOS4DV _glRasterPos4dv;

typedef void (APIENTRY * PFN_GLRASTERPOS4F)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLRASTERPOS4F _glRasterPos4f;

typedef void (APIENTRY * PFN_GLRASTERPOS4FV)(const GLfloat * v);
extern PFN_GLRASTERPOS4FV _glRasterPos4fv;

typedef void (APIENTRY * PFN_GLRASTERPOS4I)(GLint x, GLint y, GLint z, GLint w);
extern PFN_GLRASTERPOS4I _glRasterPos4i;

typedef void (APIENTRY * PFN_GLRASTERPOS4IV)(const GLint * v);
extern PFN_GLRASTERPOS4IV _glRasterPos4iv;

typedef void (APIENTRY * PFN_GLRASTERPOS4S)(GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLRASTERPOS4S _glRasterPos4s;

typedef void (APIENTRY * PFN_GLRASTERPOS4SV)(const GLshort * v);
extern PFN_GLRASTERPOS4SV _glRasterPos4sv;

typedef void (APIENTRY * PFN_GLRECTD)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern PFN_GLRECTD _glRectd;

typedef void (APIENTRY * PFN_GLRECTDV)(const GLdouble * v1, const GLdouble * v2);
extern PFN_GLRECTDV _glRectdv;

typedef void (APIENTRY * PFN_GLRECTF)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern PFN_GLRECTF _glRectf;

typedef void (APIENTRY * PFN_GLRECTFV)(const GLfloat * v1, const GLfloat * v2);
extern PFN_GLRECTFV _glRectfv;

typedef void (APIENTRY * PFN_GLRECTI)(GLint x1, GLint y1, GLint x2, GLint y2);
extern PFN_GLRECTI _glRecti;

typedef void (APIENTRY * PFN_GLRECTIV)(const GLint * v1, const GLint * v2);
extern PFN_GLRECTIV _glRectiv;

typedef void (APIENTRY * PFN_GLRECTS)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern PFN_GLRECTS _glRects;

typedef void (APIENTRY * PFN_GLRECTSV)(const GLshort * v1, const GLshort * v2);
extern PFN_GLRECTSV _glRectsv;

typedef void (APIENTRY * PFN_GLTEXCOORD1D)(GLdouble s);
extern PFN_GLTEXCOORD1D _glTexCoord1d;

typedef void (APIENTRY * PFN_GLTEXCOORD1DV)(const GLdouble * v);
extern PFN_GLTEXCOORD1DV _glTexCoord1dv;

typedef void (APIENTRY * PFN_GLTEXCOORD1F)(GLfloat s);
extern PFN_GLTEXCOORD1F _glTexCoord1f;

typedef void (APIENTRY * PFN_GLTEXCOORD1FV)(const GLfloat * v);
extern PFN_GLTEXCOORD1FV _glTexCoord1fv;

typedef void (APIENTRY * PFN_GLTEXCOORD1I)(GLint s);
extern PFN_GLTEXCOORD1I _glTexCoord1i;

typedef void (APIENTRY * PFN_GLTEXCOORD1IV)(const GLint * v);
extern PFN_GLTEXCOORD1IV _glTexCoord1iv;

typedef void (APIENTRY * PFN_GLTEXCOORD1S)(GLshort s);
extern PFN_GLTEXCOORD1S _glTexCoord1s;

typedef void (APIENTRY * PFN_GLTEXCOORD1SV)(const GLshort * v);
extern PFN_GLTEXCOORD1SV _glTexCoord1sv;

typedef void (APIENTRY * PFN_GLTEXCOORD2D)(GLdouble s, GLdouble t);
extern PFN_GLTEXCOORD2D _glTexCoord2d;

typedef void (APIENTRY * PFN_GLTEXCOORD2DV)(const GLdouble * v);
extern PFN_GLTEXCOORD2DV _glTexCoord2dv;

typedef void (APIENTRY * PFN_GLTEXCOORD2F)(GLfloat s, GLfloat t);
extern PFN_GLTEXCOORD2F _glTexCoord2f;

typedef void (APIENTRY * PFN_GLTEXCOORD2FV)(const GLfloat * v);
extern PFN_GLTEXCOORD2FV _glTexCoord2fv;

typedef void (APIENTRY * PFN_GLTEXCOORD2I)(GLint s, GLint t);
extern PFN_GLTEXCOORD2I _glTexCoord2i;

typedef void (APIENTRY * PFN_GLTEXCOORD2IV)(const GLint * v);
extern PFN_GLTEXCOORD2IV _glTexCoord2iv;

typedef void (APIENTRY * PFN_GLTEXCOORD2S)(GLshort s, GLshort t);
extern PFN_GLTEXCOORD2S _glTexCoord2s;

typedef void (APIENTRY * PFN_GLTEXCOORD2SV)(const GLshort * v);
extern PFN_GLTEXCOORD2SV _glTexCoord2sv;

typedef void (APIENTRY * PFN_GLTEXCOORD3D)(GLdouble s, GLdouble t, GLdouble r);
extern PFN_GLTEXCOORD3D _glTexCoord3d;

typedef void (APIENTRY * PFN_GLTEXCOORD3DV)(const GLdouble * v);
extern PFN_GLTEXCOORD3DV _glTexCoord3dv;

typedef void (APIENTRY * PFN_GLTEXCOORD3F)(GLfloat s, GLfloat t, GLfloat r);
extern PFN_GLTEXCOORD3F _glTexCoord3f;

typedef void (APIENTRY * PFN_GLTEXCOORD3FV)(const GLfloat * v);
extern PFN_GLTEXCOORD3FV _glTexCoord3fv;

typedef void (APIENTRY * PFN_GLTEXCOORD3I)(GLint s, GLint t, GLint r);
extern PFN_GLTEXCOORD3I _glTexCoord3i;

typedef void (APIENTRY * PFN_GLTEXCOORD3IV)(const GLint * v);
extern PFN_GLTEXCOORD3IV _glTexCoord3iv;

typedef void (APIENTRY * PFN_GLTEXCOORD3S)(GLshort s, GLshort t, GLshort r);
extern PFN_GLTEXCOORD3S _glTexCoord3s;

typedef void (APIENTRY * PFN_GLTEXCOORD3SV)(const GLshort * v);
extern PFN_GLTEXCOORD3SV _glTexCoord3sv;

typedef void (APIENTRY * PFN_GLTEXCOORD4D)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern PFN_GLTEXCOORD4D _glTexCoord4d;

typedef void (APIENTRY * PFN_GLTEXCOORD4DV)(const GLdouble * v);
extern PFN_GLTEXCOORD4DV _glTexCoord4dv;

typedef void (APIENTRY * PFN_GLTEXCOORD4F)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern PFN_GLTEXCOORD4F _glTexCoord4f;

typedef void (APIENTRY * PFN_GLTEXCOORD4FV)(const GLfloat * v);
extern PFN_GLTEXCOORD4FV _glTexCoord4fv;

typedef void (APIENTRY * PFN_GLTEXCOORD4I)(GLint s, GLint t, GLint r, GLint q);
extern PFN_GLTEXCOORD4I _glTexCoord4i;

typedef void (APIENTRY * PFN_GLTEXCOORD4IV)(const GLint * v);
extern PFN_GLTEXCOORD4IV _glTexCoord4iv;

typedef void (APIENTRY * PFN_GLTEXCOORD4S)(GLshort s, GLshort t, GLshort r, GLshort q);
extern PFN_GLTEXCOORD4S _glTexCoord4s;

typedef void (APIENTRY * PFN_GLTEXCOORD4SV)(const GLshort * v);
extern PFN_GLTEXCOORD4SV _glTexCoord4sv;

typedef void (APIENTRY * PFN_GLVERTEX2D)(GLdouble x, GLdouble y);
extern PFN_GLVERTEX2D _glVertex2d;

typedef void (APIENTRY * PFN_GLVERTEX2DV)(const GLdouble * v);
extern PFN_GLVERTEX2DV _glVertex2dv;

typedef void (APIENTRY * PFN_GLVERTEX2F)(GLfloat x, GLfloat y);
extern PFN_GLVERTEX2F _glVertex2f;

typedef void (APIENTRY * PFN_GLVERTEX2FV)(const GLfloat * v);
extern PFN_GLVERTEX2FV _glVertex2fv;

typedef void (APIENTRY * PFN_GLVERTEX2I)(GLint x, GLint y);
extern PFN_GLVERTEX2I _glVertex2i;

typedef void (APIENTRY * PFN_GLVERTEX2IV)(const GLint * v);
extern PFN_GLVERTEX2IV _glVertex2iv;

typedef void (APIENTRY * PFN_GLVERTEX2S)(GLshort x, GLshort y);
extern PFN_GLVERTEX2S _glVertex2s;

typedef void (APIENTRY * PFN_GLVERTEX2SV)(const GLshort * v);
extern PFN_GLVERTEX2SV _glVertex2sv;

typedef void (APIENTRY * PFN_GLVERTEX3D)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEX3D _glVertex3d;

typedef void (APIENTRY * PFN_GLVERTEX3DV)(const GLdouble * v);
extern PFN_GLVERTEX3DV _glVertex3dv;

typedef void (APIENTRY * PFN_GLVERTEX3F)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLVERTEX3F _glVertex3f;

typedef void (APIENTRY * PFN_GLVERTEX3FV)(const GLfloat * v);
extern PFN_GLVERTEX3FV _glVertex3fv;

typedef void (APIENTRY * PFN_GLVERTEX3I)(GLint x, GLint y, GLint z);
extern PFN_GLVERTEX3I _glVertex3i;

typedef void (APIENTRY * PFN_GLVERTEX3IV)(const GLint * v);
extern PFN_GLVERTEX3IV _glVertex3iv;

typedef void (APIENTRY * PFN_GLVERTEX3S)(GLshort x, GLshort y, GLshort z);
extern PFN_GLVERTEX3S _glVertex3s;

typedef void (APIENTRY * PFN_GLVERTEX3SV)(const GLshort * v);
extern PFN_GLVERTEX3SV _glVertex3sv;

typedef void (APIENTRY * PFN_GLVERTEX4D)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEX4D _glVertex4d;

typedef void (APIENTRY * PFN_GLVERTEX4DV)(const GLdouble * v);
extern PFN_GLVERTEX4DV _glVertex4dv;

typedef void (APIENTRY * PFN_GLVERTEX4F)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLVERTEX4F _glVertex4f;

typedef void (APIENTRY * PFN_GLVERTEX4FV)(const GLfloat * v);
extern PFN_GLVERTEX4FV _glVertex4fv;

typedef void (APIENTRY * PFN_GLVERTEX4I)(GLint x, GLint y, GLint z, GLint w);
extern PFN_GLVERTEX4I _glVertex4i;

typedef void (APIENTRY * PFN_GLVERTEX4IV)(const GLint * v);
extern PFN_GLVERTEX4IV _glVertex4iv;

typedef void (APIENTRY * PFN_GLVERTEX4S)(GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLVERTEX4S _glVertex4s;

typedef void (APIENTRY * PFN_GLVERTEX4SV)(const GLshort * v);
extern PFN_GLVERTEX4SV _glVertex4sv;

typedef void (APIENTRY * PFN_GLCLIPPLANE)(GLenum plane, const GLdouble * equation);
extern PFN_GLCLIPPLANE _glClipPlane;

typedef void (APIENTRY * PFN_GLCOLORMATERIAL)(GLenum face, GLenum mode);
extern PFN_GLCOLORMATERIAL _glColorMaterial;

typedef void (APIENTRY * PFN_GLFOGF)(GLenum pname, GLfloat param);
extern PFN_GLFOGF _glFogf;

typedef void (APIENTRY * PFN_GLFOGFV)(GLenum pname, const GLfloat * params);
extern PFN_GLFOGFV _glFogfv;

typedef void (APIENTRY * PFN_GLFOGI)(GLenum pname, GLint param);
extern PFN_GLFOGI _glFogi;

typedef void (APIENTRY * PFN_GLFOGIV)(GLenum pname, const GLint * params);
extern PFN_GLFOGIV _glFogiv;

typedef void (APIENTRY * PFN_GLLIGHTF)(GLenum light, GLenum pname, GLfloat param);
extern PFN_GLLIGHTF _glLightf;

typedef void (APIENTRY * PFN_GLLIGHTFV)(GLenum light, GLenum pname, const GLfloat * params);
extern PFN_GLLIGHTFV _glLightfv;

typedef void (APIENTRY * PFN_GLLIGHTI)(GLenum light, GLenum pname, GLint param);
extern PFN_GLLIGHTI _glLighti;

typedef void (APIENTRY * PFN_GLLIGHTIV)(GLenum light, GLenum pname, const GLint * params);
extern PFN_GLLIGHTIV _glLightiv;

typedef void (APIENTRY * PFN_GLLIGHTMODELF)(GLenum pname, GLfloat param);
extern PFN_GLLIGHTMODELF _glLightModelf;

typedef void (APIENTRY * PFN_GLLIGHTMODELFV)(GLenum pname, const GLfloat * params);
extern PFN_GLLIGHTMODELFV _glLightModelfv;

typedef void (APIENTRY * PFN_GLLIGHTMODELI)(GLenum pname, GLint param);
extern PFN_GLLIGHTMODELI _glLightModeli;

typedef void (APIENTRY * PFN_GLLIGHTMODELIV)(GLenum pname, const GLint * params);
extern PFN_GLLIGHTMODELIV _glLightModeliv;

typedef void (APIENTRY * PFN_GLLINESTIPPLE)(GLint factor, GLushort pattern);
extern PFN_GLLINESTIPPLE _glLineStipple;

typedef void (APIENTRY * PFN_GLMATERIALF)(GLenum face, GLenum pname, GLfloat param);
extern PFN_GLMATERIALF _glMaterialf;

typedef void (APIENTRY * PFN_GLMATERIALFV)(GLenum face, GLenum pname, const GLfloat * params);
extern PFN_GLMATERIALFV _glMaterialfv;

typedef void (APIENTRY * PFN_GLMATERIALI)(GLenum face, GLenum pname, GLint param);
extern PFN_GLMATERIALI _glMateriali;

typedef void (APIENTRY * PFN_GLMATERIALIV)(GLenum face, GLenum pname, const GLint * params);
extern PFN_GLMATERIALIV _glMaterialiv;

typedef void (APIENTRY * PFN_GLPOLYGONSTIPPLE)(const GLubyte * mask);
extern PFN_GLPOLYGONSTIPPLE _glPolygonStipple;

typedef void (APIENTRY * PFN_GLSHADEMODEL)(GLenum mode);
extern PFN_GLSHADEMODEL _glShadeModel;

typedef void (APIENTRY * PFN_GLTEXENVF)(GLenum target, GLenum pname, GLfloat param);
extern PFN_GLTEXENVF _glTexEnvf;

typedef void (APIENTRY * PFN_GLTEXENVFV)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLTEXENVFV _glTexEnvfv;

typedef void (APIENTRY * PFN_GLTEXENVI)(GLenum target, GLenum pname, GLint param);
extern PFN_GLTEXENVI _glTexEnvi;

typedef void (APIENTRY * PFN_GLTEXENVIV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXENVIV _glTexEnviv;

typedef void (APIENTRY * PFN_GLTEXGEND)(GLenum coord, GLenum pname, GLdouble param);
extern PFN_GLTEXGEND _glTexGend;

typedef void (APIENTRY * PFN_GLTEXGENDV)(GLenum coord, GLenum pname, const GLdouble * params);
extern PFN_GLTEXGENDV _glTexGendv;

typedef void (APIENTRY * PFN_GLTEXGENF)(GLenum coord, GLenum pname, GLfloat param);
extern PFN_GLTEXGENF _glTexGenf;

typedef void (APIENTRY * PFN_GLTEXGENFV)(GLenum coord, GLenum pname, const GLfloat * params);
extern PFN_GLTEXGENFV _glTexGenfv;

typedef void (APIENTRY * PFN_GLTEXGENI)(GLenum coord, GLenum pname, GLint param);
extern PFN_GLTEXGENI _glTexGeni;

typedef void (APIENTRY * PFN_GLTEXGENIV)(GLenum coord, GLenum pname, const GLint * params);
extern PFN_GLTEXGENIV _glTexGeniv;

typedef void (APIENTRY * PFN_GLFEEDBACKBUFFER)(GLsizei size, GLenum type, GLfloat * buffer);
extern PFN_GLFEEDBACKBUFFER _glFeedbackBuffer;

typedef void (APIENTRY * PFN_GLSELECTBUFFER)(GLsizei size, GLuint * buffer);
extern PFN_GLSELECTBUFFER _glSelectBuffer;

typedef GLint (APIENTRY * PFN_GLRENDERMODE)(GLenum mode);
extern PFN_GLRENDERMODE _glRenderMode;

typedef void (APIENTRY * PFN_GLINITNAMES)(void);
extern PFN_GLINITNAMES _glInitNames;

typedef void (APIENTRY * PFN_GLLOADNAME)(GLuint name);
extern PFN_GLLOADNAME _glLoadName;

typedef void (APIENTRY * PFN_GLPASSTHROUGH)(GLfloat token);
extern PFN_GLPASSTHROUGH _glPassThrough;

typedef void (APIENTRY * PFN_GLPOPNAME)(void);
extern PFN_GLPOPNAME _glPopName;

typedef void (APIENTRY * PFN_GLPUSHNAME)(GLuint name);
extern PFN_GLPUSHNAME _glPushName;

typedef void (APIENTRY * PFN_GLCLEARACCUM)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFN_GLCLEARACCUM _glClearAccum;

typedef void (APIENTRY * PFN_GLCLEARINDEX)(GLfloat c);
extern PFN_GLCLEARINDEX _glClearIndex;

typedef void (APIENTRY * PFN_GLINDEXMASK)(GLuint mask);
extern PFN_GLINDEXMASK _glIndexMask;

typedef void (APIENTRY * PFN_GLACCUM)(GLenum op, GLfloat value);
extern PFN_GLACCUM _glAccum;

typedef void (APIENTRY * PFN_GLPOPATTRIB)(void);
extern PFN_GLPOPATTRIB _glPopAttrib;

typedef void (APIENTRY * PFN_GLPUSHATTRIB)(GLbitfield mask);
extern PFN_GLPUSHATTRIB _glPushAttrib;

typedef void (APIENTRY * PFN_GLMAP1D)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);
extern PFN_GLMAP1D _glMap1d;

typedef void (APIENTRY * PFN_GLMAP1F)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);
extern PFN_GLMAP1F _glMap1f;

typedef void (APIENTRY * PFN_GLMAP2D)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);
extern PFN_GLMAP2D _glMap2d;

typedef void (APIENTRY * PFN_GLMAP2F)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);
extern PFN_GLMAP2F _glMap2f;

typedef void (APIENTRY * PFN_GLMAPGRID1D)(GLint un, GLdouble u1, GLdouble u2);
extern PFN_GLMAPGRID1D _glMapGrid1d;

typedef void (APIENTRY * PFN_GLMAPGRID1F)(GLint un, GLfloat u1, GLfloat u2);
extern PFN_GLMAPGRID1F _glMapGrid1f;

typedef void (APIENTRY * PFN_GLMAPGRID2D)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern PFN_GLMAPGRID2D _glMapGrid2d;

typedef void (APIENTRY * PFN_GLMAPGRID2F)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern PFN_GLMAPGRID2F _glMapGrid2f;

typedef void (APIENTRY * PFN_GLEVALCOORD1D)(GLdouble u);
extern PFN_GLEVALCOORD1D _glEvalCoord1d;

typedef void (APIENTRY * PFN_GLEVALCOORD1DV)(const GLdouble * u);
extern PFN_GLEVALCOORD1DV _glEvalCoord1dv;

typedef void (APIENTRY * PFN_GLEVALCOORD1F)(GLfloat u);
extern PFN_GLEVALCOORD1F _glEvalCoord1f;

typedef void (APIENTRY * PFN_GLEVALCOORD1FV)(const GLfloat * u);
extern PFN_GLEVALCOORD1FV _glEvalCoord1fv;

typedef void (APIENTRY * PFN_GLEVALCOORD2D)(GLdouble u, GLdouble v);
extern PFN_GLEVALCOORD2D _glEvalCoord2d;

typedef void (APIENTRY * PFN_GLEVALCOORD2DV)(const GLdouble * u);
extern PFN_GLEVALCOORD2DV _glEvalCoord2dv;

typedef void (APIENTRY * PFN_GLEVALCOORD2F)(GLfloat u, GLfloat v);
extern PFN_GLEVALCOORD2F _glEvalCoord2f;

typedef void (APIENTRY * PFN_GLEVALCOORD2FV)(const GLfloat * u);
extern PFN_GLEVALCOORD2FV _glEvalCoord2fv;

typedef void (APIENTRY * PFN_GLEVALMESH1)(GLenum mode, GLint i1, GLint i2);
extern PFN_GLEVALMESH1 _glEvalMesh1;

typedef void (APIENTRY * PFN_GLEVALPOINT1)(GLint i);
extern PFN_GLEVALPOINT1 _glEvalPoint1;

typedef void (APIENTRY * PFN_GLEVALMESH2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern PFN_GLEVALMESH2 _glEvalMesh2;

typedef void (APIENTRY * PFN_GLEVALPOINT2)(GLint i, GLint j);
extern PFN_GLEVALPOINT2 _glEvalPoint2;

typedef void (APIENTRY * PFN_GLALPHAFUNC)(GLenum func, GLfloat ref);
extern PFN_GLALPHAFUNC _glAlphaFunc;

typedef void (APIENTRY * PFN_GLPIXELZOOM)(GLfloat xfactor, GLfloat yfactor);
extern PFN_GLPIXELZOOM _glPixelZoom;

typedef void (APIENTRY * PFN_GLPIXELTRANSFERF)(GLenum pname, GLfloat param);
extern PFN_GLPIXELTRANSFERF _glPixelTransferf;

typedef void (APIENTRY * PFN_GLPIXELTRANSFERI)(GLenum pname, GLint param);
extern PFN_GLPIXELTRANSFERI _glPixelTransferi;

typedef void (APIENTRY * PFN_GLPIXELMAPFV)(GLenum map, GLsizei mapsize, const GLfloat * values);
extern PFN_GLPIXELMAPFV _glPixelMapfv;

typedef void (APIENTRY * PFN_GLPIXELMAPUIV)(GLenum map, GLsizei mapsize, const GLuint * values);
extern PFN_GLPIXELMAPUIV _glPixelMapuiv;

typedef void (APIENTRY * PFN_GLPIXELMAPUSV)(GLenum map, GLsizei mapsize, const GLushort * values);
extern PFN_GLPIXELMAPUSV _glPixelMapusv;

typedef void (APIENTRY * PFN_GLCOPYPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern PFN_GLCOPYPIXELS _glCopyPixels;

typedef void (APIENTRY * PFN_GLDRAWPIXELS)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLDRAWPIXELS _glDrawPixels;

typedef void (APIENTRY * PFN_GLGETCLIPPLANE)(GLenum plane, GLdouble * equation);
extern PFN_GLGETCLIPPLANE _glGetClipPlane;

typedef void (APIENTRY * PFN_GLGETLIGHTFV)(GLenum light, GLenum pname, GLfloat * params);
extern PFN_GLGETLIGHTFV _glGetLightfv;

typedef void (APIENTRY * PFN_GLGETLIGHTIV)(GLenum light, GLenum pname, GLint * params);
extern PFN_GLGETLIGHTIV _glGetLightiv;

typedef void (APIENTRY * PFN_GLGETMAPDV)(GLenum target, GLenum query, GLdouble * v);
extern PFN_GLGETMAPDV _glGetMapdv;

typedef void (APIENTRY * PFN_GLGETMAPFV)(GLenum target, GLenum query, GLfloat * v);
extern PFN_GLGETMAPFV _glGetMapfv;

typedef void (APIENTRY * PFN_GLGETMAPIV)(GLenum target, GLenum query, GLint * v);
extern PFN_GLGETMAPIV _glGetMapiv;

typedef void (APIENTRY * PFN_GLGETMATERIALFV)(GLenum face, GLenum pname, GLfloat * params);
extern PFN_GLGETMATERIALFV _glGetMaterialfv;

typedef void (APIENTRY * PFN_GLGETMATERIALIV)(GLenum face, GLenum pname, GLint * params);
extern PFN_GLGETMATERIALIV _glGetMaterialiv;

typedef void (APIENTRY * PFN_GLGETPIXELMAPFV)(GLenum map, GLfloat * values);
extern PFN_GLGETPIXELMAPFV _glGetPixelMapfv;

typedef void (APIENTRY * PFN_GLGETPIXELMAPUIV)(GLenum map, GLuint * values);
extern PFN_GLGETPIXELMAPUIV _glGetPixelMapuiv;

typedef void (APIENTRY * PFN_GLGETPIXELMAPUSV)(GLenum map, GLushort * values);
extern PFN_GLGETPIXELMAPUSV _glGetPixelMapusv;

typedef void (APIENTRY * PFN_GLGETPOLYGONSTIPPLE)(GLubyte * mask);
extern PFN_GLGETPOLYGONSTIPPLE _glGetPolygonStipple;

typedef void (APIENTRY * PFN_GLGETTEXENVFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXENVFV _glGetTexEnvfv;

typedef void (APIENTRY * PFN_GLGETTEXENVIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXENVIV _glGetTexEnviv;

typedef void (APIENTRY * PFN_GLGETTEXGENDV)(GLenum coord, GLenum pname, GLdouble * params);
extern PFN_GLGETTEXGENDV _glGetTexGendv;

typedef void (APIENTRY * PFN_GLGETTEXGENFV)(GLenum coord, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXGENFV _glGetTexGenfv;

typedef void (APIENTRY * PFN_GLGETTEXGENIV)(GLenum coord, GLenum pname, GLint * params);
extern PFN_GLGETTEXGENIV _glGetTexGeniv;

typedef GLboolean (APIENTRY * PFN_GLISLIST)(GLuint list);
extern PFN_GLISLIST _glIsList;

typedef void (APIENTRY * PFN_GLFRUSTUM)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern PFN_GLFRUSTUM _glFrustum;

typedef void (APIENTRY * PFN_GLLOADIDENTITY)(void);
extern PFN_GLLOADIDENTITY _glLoadIdentity;

typedef void (APIENTRY * PFN_GLLOADMATRIXF)(const GLfloat * m);
extern PFN_GLLOADMATRIXF _glLoadMatrixf;

typedef void (APIENTRY * PFN_GLLOADMATRIXD)(const GLdouble * m);
extern PFN_GLLOADMATRIXD _glLoadMatrixd;

typedef void (APIENTRY * PFN_GLMATRIXMODE)(GLenum mode);
extern PFN_GLMATRIXMODE _glMatrixMode;

typedef void (APIENTRY * PFN_GLMULTMATRIXF)(const GLfloat * m);
extern PFN_GLMULTMATRIXF _glMultMatrixf;

typedef void (APIENTRY * PFN_GLMULTMATRIXD)(const GLdouble * m);
extern PFN_GLMULTMATRIXD _glMultMatrixd;

typedef void (APIENTRY * PFN_GLORTHO)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern PFN_GLORTHO _glOrtho;

typedef void (APIENTRY * PFN_GLPOPMATRIX)(void);
extern PFN_GLPOPMATRIX _glPopMatrix;

typedef void (APIENTRY * PFN_GLPUSHMATRIX)(void);
extern PFN_GLPUSHMATRIX _glPushMatrix;

typedef void (APIENTRY * PFN_GLROTATED)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLROTATED _glRotated;

typedef void (APIENTRY * PFN_GLROTATEF)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLROTATEF _glRotatef;

typedef void (APIENTRY * PFN_GLSCALED)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLSCALED _glScaled;

typedef void (APIENTRY * PFN_GLSCALEF)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLSCALEF _glScalef;

typedef void (APIENTRY * PFN_GLTRANSLATED)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLTRANSLATED _glTranslated;

typedef void (APIENTRY * PFN_GLTRANSLATEF)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTRANSLATEF _glTranslatef;

typedef void (APIENTRY * PFN_GLDRAWARRAYS)(GLenum mode, GLint first, GLsizei count);
extern PFN_GLDRAWARRAYS _glDrawArrays;

typedef void (APIENTRY * PFN_GLDRAWELEMENTS)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
extern PFN_GLDRAWELEMENTS _glDrawElements;

typedef void (APIENTRY * PFN_GLGETPOINTERV)(GLenum pname, GLvoid * * params);
extern PFN_GLGETPOINTERV _glGetPointerv;

typedef void (APIENTRY * PFN_GLPOLYGONOFFSET)(GLfloat factor, GLfloat units);
extern PFN_GLPOLYGONOFFSET _glPolygonOffset;

typedef void (APIENTRY * PFN_GLCOPYTEXIMAGE1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern PFN_GLCOPYTEXIMAGE1D _glCopyTexImage1D;

typedef void (APIENTRY * PFN_GLCOPYTEXIMAGE2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern PFN_GLCOPYTEXIMAGE2D _glCopyTexImage2D;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYTEXSUBIMAGE1D _glCopyTexSubImage1D;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXSUBIMAGE2D _glCopyTexSubImage2D;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE1D _glTexSubImage1D;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE2D _glTexSubImage2D;

typedef void (APIENTRY * PFN_GLBINDTEXTURE)(GLenum target, GLuint texture);
extern PFN_GLBINDTEXTURE _glBindTexture;

typedef void (APIENTRY * PFN_GLDELETETEXTURES)(GLsizei n, const GLuint * textures);
extern PFN_GLDELETETEXTURES _glDeleteTextures;

typedef void (APIENTRY * PFN_GLGENTEXTURES)(GLsizei n, GLuint * textures);
extern PFN_GLGENTEXTURES _glGenTextures;

typedef GLboolean (APIENTRY * PFN_GLISTEXTURE)(GLuint texture);
extern PFN_GLISTEXTURE _glIsTexture;

typedef void (APIENTRY * PFN_GLARRAYELEMENT)(GLint i);
extern PFN_GLARRAYELEMENT _glArrayElement;

typedef void (APIENTRY * PFN_GLCOLORPOINTER)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLCOLORPOINTER _glColorPointer;

typedef void (APIENTRY * PFN_GLDISABLECLIENTSTATE)(GLenum array);
extern PFN_GLDISABLECLIENTSTATE _glDisableClientState;

typedef void (APIENTRY * PFN_GLEDGEFLAGPOINTER)(GLsizei stride, const GLvoid * pointer);
extern PFN_GLEDGEFLAGPOINTER _glEdgeFlagPointer;

typedef void (APIENTRY * PFN_GLENABLECLIENTSTATE)(GLenum array);
extern PFN_GLENABLECLIENTSTATE _glEnableClientState;

typedef void (APIENTRY * PFN_GLINDEXPOINTER)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLINDEXPOINTER _glIndexPointer;

typedef void (APIENTRY * PFN_GLINTERLEAVEDARRAYS)(GLenum format, GLsizei stride, const GLvoid * pointer);
extern PFN_GLINTERLEAVEDARRAYS _glInterleavedArrays;

typedef void (APIENTRY * PFN_GLNORMALPOINTER)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLNORMALPOINTER _glNormalPointer;

typedef void (APIENTRY * PFN_GLTEXCOORDPOINTER)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLTEXCOORDPOINTER _glTexCoordPointer;

typedef void (APIENTRY * PFN_GLVERTEXPOINTER)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXPOINTER _glVertexPointer;

typedef GLboolean (APIENTRY * PFN_GLARETEXTURESRESIDENT)(GLsizei n, const GLuint * textures, GLboolean * residences);
extern PFN_GLARETEXTURESRESIDENT _glAreTexturesResident;

typedef void (APIENTRY * PFN_GLPRIORITIZETEXTURES)(GLsizei n, const GLuint * textures, const GLclampf * priorities);
extern PFN_GLPRIORITIZETEXTURES _glPrioritizeTextures;

typedef void (APIENTRY * PFN_GLINDEXUB)(GLubyte c);
extern PFN_GLINDEXUB _glIndexub;

typedef void (APIENTRY * PFN_GLINDEXUBV)(const GLubyte * c);
extern PFN_GLINDEXUBV _glIndexubv;

typedef void (APIENTRY * PFN_GLPOPCLIENTATTRIB)(void);
extern PFN_GLPOPCLIENTATTRIB _glPopClientAttrib;

typedef void (APIENTRY * PFN_GLPUSHCLIENTATTRIB)(GLbitfield mask);
extern PFN_GLPUSHCLIENTATTRIB _glPushClientAttrib;

typedef void (APIENTRY * PFN_GLBLENDCOLOR)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFN_GLBLENDCOLOR _glBlendColor;

typedef void (APIENTRY * PFN_GLBLENDEQUATION)(GLenum mode);
extern PFN_GLBLENDEQUATION _glBlendEquation;

typedef void (APIENTRY * PFN_GLDRAWRANGEELEMENTS)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
extern PFN_GLDRAWRANGEELEMENTS _glDrawRangeElements;

typedef void (APIENTRY * PFN_GLTEXIMAGE3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE3D _glTexImage3D;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE3D _glTexSubImage3D;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXSUBIMAGE3D _glCopyTexSubImage3D;

typedef void (APIENTRY * PFN_GLCOLORTABLE)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);
extern PFN_GLCOLORTABLE _glColorTable;

typedef void (APIENTRY * PFN_GLCOLORTABLEPARAMETERFV)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLCOLORTABLEPARAMETERFV _glColorTableParameterfv;

typedef void (APIENTRY * PFN_GLCOLORTABLEPARAMETERIV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLCOLORTABLEPARAMETERIV _glColorTableParameteriv;

typedef void (APIENTRY * PFN_GLCOPYCOLORTABLE)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCOLORTABLE _glCopyColorTable;

typedef void (APIENTRY * PFN_GLGETCOLORTABLE)(GLenum target, GLenum format, GLenum type, GLvoid * table);
extern PFN_GLGETCOLORTABLE _glGetColorTable;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETCOLORTABLEPARAMETERFV _glGetColorTableParameterfv;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETCOLORTABLEPARAMETERIV _glGetColorTableParameteriv;

typedef void (APIENTRY * PFN_GLCOLORSUBTABLE)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data);
extern PFN_GLCOLORSUBTABLE _glColorSubTable;

typedef void (APIENTRY * PFN_GLCOPYCOLORSUBTABLE)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCOLORSUBTABLE _glCopyColorSubTable;

typedef void (APIENTRY * PFN_GLCONVOLUTIONFILTER1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image);
extern PFN_GLCONVOLUTIONFILTER1D _glConvolutionFilter1D;

typedef void (APIENTRY * PFN_GLCONVOLUTIONFILTER2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image);
extern PFN_GLCONVOLUTIONFILTER2D _glConvolutionFilter2D;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERF)(GLenum target, GLenum pname, GLfloat params);
extern PFN_GLCONVOLUTIONPARAMETERF _glConvolutionParameterf;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERFV)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLCONVOLUTIONPARAMETERFV _glConvolutionParameterfv;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERI)(GLenum target, GLenum pname, GLint params);
extern PFN_GLCONVOLUTIONPARAMETERI _glConvolutionParameteri;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERIV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLCONVOLUTIONPARAMETERIV _glConvolutionParameteriv;

typedef void (APIENTRY * PFN_GLCOPYCONVOLUTIONFILTER1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCONVOLUTIONFILTER1D _glCopyConvolutionFilter1D;

typedef void (APIENTRY * PFN_GLCOPYCONVOLUTIONFILTER2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYCONVOLUTIONFILTER2D _glCopyConvolutionFilter2D;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONFILTER)(GLenum target, GLenum format, GLenum type, GLvoid * image);
extern PFN_GLGETCONVOLUTIONFILTER _glGetConvolutionFilter;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONPARAMETERFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETCONVOLUTIONPARAMETERFV _glGetConvolutionParameterfv;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETCONVOLUTIONPARAMETERIV _glGetConvolutionParameteriv;

typedef void (APIENTRY * PFN_GLGETSEPARABLEFILTER)(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span);
extern PFN_GLGETSEPARABLEFILTER _glGetSeparableFilter;

typedef void (APIENTRY * PFN_GLSEPARABLEFILTER2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column);
extern PFN_GLSEPARABLEFILTER2D _glSeparableFilter2D;

typedef void (APIENTRY * PFN_GLGETHISTOGRAM)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
extern PFN_GLGETHISTOGRAM _glGetHistogram;

typedef void (APIENTRY * PFN_GLGETHISTOGRAMPARAMETERFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETHISTOGRAMPARAMETERFV _glGetHistogramParameterfv;

typedef void (APIENTRY * PFN_GLGETHISTOGRAMPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETHISTOGRAMPARAMETERIV _glGetHistogramParameteriv;

typedef void (APIENTRY * PFN_GLGETMINMAX)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
extern PFN_GLGETMINMAX _glGetMinmax;

typedef void (APIENTRY * PFN_GLGETMINMAXPARAMETERFV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETMINMAXPARAMETERFV _glGetMinmaxParameterfv;

typedef void (APIENTRY * PFN_GLGETMINMAXPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMINMAXPARAMETERIV _glGetMinmaxParameteriv;

typedef void (APIENTRY * PFN_GLHISTOGRAM)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern PFN_GLHISTOGRAM _glHistogram;

typedef void (APIENTRY * PFN_GLMINMAX)(GLenum target, GLenum internalformat, GLboolean sink);
extern PFN_GLMINMAX _glMinmax;

typedef void (APIENTRY * PFN_GLRESETHISTOGRAM)(GLenum target);
extern PFN_GLRESETHISTOGRAM _glResetHistogram;

typedef void (APIENTRY * PFN_GLRESETMINMAX)(GLenum target);
extern PFN_GLRESETMINMAX _glResetMinmax;

typedef void (APIENTRY * PFN_GLACTIVETEXTURE)(GLenum texture);
extern PFN_GLACTIVETEXTURE _glActiveTexture;

typedef void (APIENTRY * PFN_GLSAMPLECOVERAGE)(GLfloat value, GLboolean invert);
extern PFN_GLSAMPLECOVERAGE _glSampleCoverage;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE3D _glCompressedTexImage3D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE2D _glCompressedTexImage2D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE1D _glCompressedTexImage1D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE3D _glCompressedTexSubImage3D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE2D _glCompressedTexSubImage2D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE1D _glCompressedTexSubImage1D;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDTEXIMAGE)(GLenum target, GLint level, GLvoid * img);
extern PFN_GLGETCOMPRESSEDTEXIMAGE _glGetCompressedTexImage;

typedef void (APIENTRY * PFN_GLCLIENTACTIVETEXTURE)(GLenum texture);
extern PFN_GLCLIENTACTIVETEXTURE _glClientActiveTexture;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1D)(GLenum target, GLdouble s);
extern PFN_GLMULTITEXCOORD1D _glMultiTexCoord1d;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1DV)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD1DV _glMultiTexCoord1dv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1F)(GLenum target, GLfloat s);
extern PFN_GLMULTITEXCOORD1F _glMultiTexCoord1f;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1FV)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD1FV _glMultiTexCoord1fv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1I)(GLenum target, GLint s);
extern PFN_GLMULTITEXCOORD1I _glMultiTexCoord1i;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1IV)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD1IV _glMultiTexCoord1iv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1S)(GLenum target, GLshort s);
extern PFN_GLMULTITEXCOORD1S _glMultiTexCoord1s;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1SV)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD1SV _glMultiTexCoord1sv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2D)(GLenum target, GLdouble s, GLdouble t);
extern PFN_GLMULTITEXCOORD2D _glMultiTexCoord2d;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2DV)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD2DV _glMultiTexCoord2dv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2F)(GLenum target, GLfloat s, GLfloat t);
extern PFN_GLMULTITEXCOORD2F _glMultiTexCoord2f;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2FV)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD2FV _glMultiTexCoord2fv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2I)(GLenum target, GLint s, GLint t);
extern PFN_GLMULTITEXCOORD2I _glMultiTexCoord2i;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2IV)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD2IV _glMultiTexCoord2iv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2S)(GLenum target, GLshort s, GLshort t);
extern PFN_GLMULTITEXCOORD2S _glMultiTexCoord2s;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2SV)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD2SV _glMultiTexCoord2sv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3D)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern PFN_GLMULTITEXCOORD3D _glMultiTexCoord3d;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3DV)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD3DV _glMultiTexCoord3dv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3F)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern PFN_GLMULTITEXCOORD3F _glMultiTexCoord3f;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3FV)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD3FV _glMultiTexCoord3fv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3I)(GLenum target, GLint s, GLint t, GLint r);
extern PFN_GLMULTITEXCOORD3I _glMultiTexCoord3i;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3IV)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD3IV _glMultiTexCoord3iv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3S)(GLenum target, GLshort s, GLshort t, GLshort r);
extern PFN_GLMULTITEXCOORD3S _glMultiTexCoord3s;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3SV)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD3SV _glMultiTexCoord3sv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4D)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern PFN_GLMULTITEXCOORD4D _glMultiTexCoord4d;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4DV)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD4DV _glMultiTexCoord4dv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4F)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern PFN_GLMULTITEXCOORD4F _glMultiTexCoord4f;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4FV)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD4FV _glMultiTexCoord4fv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4I)(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern PFN_GLMULTITEXCOORD4I _glMultiTexCoord4i;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4IV)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD4IV _glMultiTexCoord4iv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4S)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern PFN_GLMULTITEXCOORD4S _glMultiTexCoord4s;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4SV)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD4SV _glMultiTexCoord4sv;

typedef void (APIENTRY * PFN_GLLOADTRANSPOSEMATRIXF)(const GLfloat * m);
extern PFN_GLLOADTRANSPOSEMATRIXF _glLoadTransposeMatrixf;

typedef void (APIENTRY * PFN_GLLOADTRANSPOSEMATRIXD)(const GLdouble * m);
extern PFN_GLLOADTRANSPOSEMATRIXD _glLoadTransposeMatrixd;

typedef void (APIENTRY * PFN_GLMULTTRANSPOSEMATRIXF)(const GLfloat * m);
extern PFN_GLMULTTRANSPOSEMATRIXF _glMultTransposeMatrixf;

typedef void (APIENTRY * PFN_GLMULTTRANSPOSEMATRIXD)(const GLdouble * m);
extern PFN_GLMULTTRANSPOSEMATRIXD _glMultTransposeMatrixd;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATE)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern PFN_GLBLENDFUNCSEPARATE _glBlendFuncSeparate;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYS)(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount);
extern PFN_GLMULTIDRAWARRAYS _glMultiDrawArrays;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTS)(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei drawcount);
extern PFN_GLMULTIDRAWELEMENTS _glMultiDrawElements;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERF)(GLenum pname, GLfloat param);
extern PFN_GLPOINTPARAMETERF _glPointParameterf;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFV)(GLenum pname, const GLfloat * params);
extern PFN_GLPOINTPARAMETERFV _glPointParameterfv;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERI)(GLenum pname, GLint param);
extern PFN_GLPOINTPARAMETERI _glPointParameteri;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERIV)(GLenum pname, const GLint * params);
extern PFN_GLPOINTPARAMETERIV _glPointParameteriv;

typedef void (APIENTRY * PFN_GLFOGCOORDF)(GLfloat coord);
extern PFN_GLFOGCOORDF _glFogCoordf;

typedef void (APIENTRY * PFN_GLFOGCOORDFV)(const GLfloat * coord);
extern PFN_GLFOGCOORDFV _glFogCoordfv;

typedef void (APIENTRY * PFN_GLFOGCOORDD)(GLdouble coord);
extern PFN_GLFOGCOORDD _glFogCoordd;

typedef void (APIENTRY * PFN_GLFOGCOORDDV)(const GLdouble * coord);
extern PFN_GLFOGCOORDDV _glFogCoorddv;

typedef void (APIENTRY * PFN_GLFOGCOORDPOINTER)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLFOGCOORDPOINTER _glFogCoordPointer;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3B)(GLbyte red, GLbyte green, GLbyte blue);
extern PFN_GLSECONDARYCOLOR3B _glSecondaryColor3b;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3BV)(const GLbyte * v);
extern PFN_GLSECONDARYCOLOR3BV _glSecondaryColor3bv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3D)(GLdouble red, GLdouble green, GLdouble blue);
extern PFN_GLSECONDARYCOLOR3D _glSecondaryColor3d;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3DV)(const GLdouble * v);
extern PFN_GLSECONDARYCOLOR3DV _glSecondaryColor3dv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3F)(GLfloat red, GLfloat green, GLfloat blue);
extern PFN_GLSECONDARYCOLOR3F _glSecondaryColor3f;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3FV)(const GLfloat * v);
extern PFN_GLSECONDARYCOLOR3FV _glSecondaryColor3fv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3I)(GLint red, GLint green, GLint blue);
extern PFN_GLSECONDARYCOLOR3I _glSecondaryColor3i;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3IV)(const GLint * v);
extern PFN_GLSECONDARYCOLOR3IV _glSecondaryColor3iv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3S)(GLshort red, GLshort green, GLshort blue);
extern PFN_GLSECONDARYCOLOR3S _glSecondaryColor3s;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3SV)(const GLshort * v);
extern PFN_GLSECONDARYCOLOR3SV _glSecondaryColor3sv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UB)(GLubyte red, GLubyte green, GLubyte blue);
extern PFN_GLSECONDARYCOLOR3UB _glSecondaryColor3ub;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UBV)(const GLubyte * v);
extern PFN_GLSECONDARYCOLOR3UBV _glSecondaryColor3ubv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UI)(GLuint red, GLuint green, GLuint blue);
extern PFN_GLSECONDARYCOLOR3UI _glSecondaryColor3ui;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UIV)(const GLuint * v);
extern PFN_GLSECONDARYCOLOR3UIV _glSecondaryColor3uiv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3US)(GLushort red, GLushort green, GLushort blue);
extern PFN_GLSECONDARYCOLOR3US _glSecondaryColor3us;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3USV)(const GLushort * v);
extern PFN_GLSECONDARYCOLOR3USV _glSecondaryColor3usv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORPOINTER)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLSECONDARYCOLORPOINTER _glSecondaryColorPointer;

typedef void (APIENTRY * PFN_GLWINDOWPOS2D)(GLdouble x, GLdouble y);
extern PFN_GLWINDOWPOS2D _glWindowPos2d;

typedef void (APIENTRY * PFN_GLWINDOWPOS2DV)(const GLdouble * v);
extern PFN_GLWINDOWPOS2DV _glWindowPos2dv;

typedef void (APIENTRY * PFN_GLWINDOWPOS2F)(GLfloat x, GLfloat y);
extern PFN_GLWINDOWPOS2F _glWindowPos2f;

typedef void (APIENTRY * PFN_GLWINDOWPOS2FV)(const GLfloat * v);
extern PFN_GLWINDOWPOS2FV _glWindowPos2fv;

typedef void (APIENTRY * PFN_GLWINDOWPOS2I)(GLint x, GLint y);
extern PFN_GLWINDOWPOS2I _glWindowPos2i;

typedef void (APIENTRY * PFN_GLWINDOWPOS2IV)(const GLint * v);
extern PFN_GLWINDOWPOS2IV _glWindowPos2iv;

typedef void (APIENTRY * PFN_GLWINDOWPOS2S)(GLshort x, GLshort y);
extern PFN_GLWINDOWPOS2S _glWindowPos2s;

typedef void (APIENTRY * PFN_GLWINDOWPOS2SV)(const GLshort * v);
extern PFN_GLWINDOWPOS2SV _glWindowPos2sv;

typedef void (APIENTRY * PFN_GLWINDOWPOS3D)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLWINDOWPOS3D _glWindowPos3d;

typedef void (APIENTRY * PFN_GLWINDOWPOS3DV)(const GLdouble * v);
extern PFN_GLWINDOWPOS3DV _glWindowPos3dv;

typedef void (APIENTRY * PFN_GLWINDOWPOS3F)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLWINDOWPOS3F _glWindowPos3f;

typedef void (APIENTRY * PFN_GLWINDOWPOS3FV)(const GLfloat * v);
extern PFN_GLWINDOWPOS3FV _glWindowPos3fv;

typedef void (APIENTRY * PFN_GLWINDOWPOS3I)(GLint x, GLint y, GLint z);
extern PFN_GLWINDOWPOS3I _glWindowPos3i;

typedef void (APIENTRY * PFN_GLWINDOWPOS3IV)(const GLint * v);
extern PFN_GLWINDOWPOS3IV _glWindowPos3iv;

typedef void (APIENTRY * PFN_GLWINDOWPOS3S)(GLshort x, GLshort y, GLshort z);
extern PFN_GLWINDOWPOS3S _glWindowPos3s;

typedef void (APIENTRY * PFN_GLWINDOWPOS3SV)(const GLshort * v);
extern PFN_GLWINDOWPOS3SV _glWindowPos3sv;

typedef void (APIENTRY * PFN_GLGENQUERIES)(GLsizei n, GLuint * ids);
extern PFN_GLGENQUERIES _glGenQueries;

typedef void (APIENTRY * PFN_GLDELETEQUERIES)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETEQUERIES _glDeleteQueries;

typedef GLboolean (APIENTRY * PFN_GLISQUERY)(GLuint id);
extern PFN_GLISQUERY _glIsQuery;

typedef void (APIENTRY * PFN_GLBEGINQUERY)(GLenum target, GLuint id);
extern PFN_GLBEGINQUERY _glBeginQuery;

typedef void (APIENTRY * PFN_GLENDQUERY)(GLenum target);
extern PFN_GLENDQUERY _glEndQuery;

typedef void (APIENTRY * PFN_GLGETQUERYIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETQUERYIV _glGetQueryiv;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTIV)(GLuint id, GLenum pname, GLint * params);
extern PFN_GLGETQUERYOBJECTIV _glGetQueryObjectiv;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTUIV)(GLuint id, GLenum pname, GLuint * params);
extern PFN_GLGETQUERYOBJECTUIV _glGetQueryObjectuiv;

typedef void (APIENTRY * PFN_GLBINDBUFFER)(GLenum target, GLuint buffer);
extern PFN_GLBINDBUFFER _glBindBuffer;

typedef void (APIENTRY * PFN_GLDELETEBUFFERS)(GLsizei n, const GLuint * buffer);
extern PFN_GLDELETEBUFFERS _glDeleteBuffers;

typedef void (APIENTRY * PFN_GLGENBUFFERS)(GLsizei n, GLuint * buffer);
extern PFN_GLGENBUFFERS _glGenBuffers;

typedef GLboolean (APIENTRY * PFN_GLISBUFFER)(GLuint buffer);
extern PFN_GLISBUFFER _glIsBuffer;

typedef void (APIENTRY * PFN_GLBUFFERDATA)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
extern PFN_GLBUFFERDATA _glBufferData;

typedef void (APIENTRY * PFN_GLBUFFERSUBDATA)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
extern PFN_GLBUFFERSUBDATA _glBufferSubData;

typedef void (APIENTRY * PFN_GLGETBUFFERSUBDATA)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);
extern PFN_GLGETBUFFERSUBDATA _glGetBufferSubData;

typedef GLvoid * (APIENTRY * PFN_GLMAPBUFFER)(GLenum target, GLenum access);
extern PFN_GLMAPBUFFER _glMapBuffer;

typedef GLboolean (APIENTRY * PFN_GLUNMAPBUFFER)(GLenum target);
extern PFN_GLUNMAPBUFFER _glUnmapBuffer;

typedef void (APIENTRY * PFN_GLGETBUFFERPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETBUFFERPARAMETERIV _glGetBufferParameteriv;

typedef void (APIENTRY * PFN_GLGETBUFFERPOINTERV)(GLenum target, GLenum pname, GLvoid * * params);
extern PFN_GLGETBUFFERPOINTERV _glGetBufferPointerv;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONSEPARATE)(GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATE _glBlendEquationSeparate;

typedef void (APIENTRY * PFN_GLDRAWBUFFERS)(GLsizei n, const GLenum * bufs);
extern PFN_GLDRAWBUFFERS _glDrawBuffers;

typedef void (APIENTRY * PFN_GLSTENCILOPSEPARATE)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern PFN_GLSTENCILOPSEPARATE _glStencilOpSeparate;

typedef void (APIENTRY * PFN_GLSTENCILFUNCSEPARATE)(GLenum face, GLenum func, GLint ref, GLuint mask);
extern PFN_GLSTENCILFUNCSEPARATE _glStencilFuncSeparate;

typedef void (APIENTRY * PFN_GLSTENCILMASKSEPARATE)(GLenum face, GLuint mask);
extern PFN_GLSTENCILMASKSEPARATE _glStencilMaskSeparate;

typedef void (APIENTRY * PFN_GLATTACHSHADER)(GLuint program, GLuint shader);
extern PFN_GLATTACHSHADER _glAttachShader;

typedef void (APIENTRY * PFN_GLBINDATTRIBLOCATION)(GLuint program, GLuint index, const GLchar * name);
extern PFN_GLBINDATTRIBLOCATION _glBindAttribLocation;

typedef void (APIENTRY * PFN_GLCOMPILESHADER)(GLuint shader);
extern PFN_GLCOMPILESHADER _glCompileShader;

typedef GLuint (APIENTRY * PFN_GLCREATEPROGRAM)(void);
extern PFN_GLCREATEPROGRAM _glCreateProgram;

typedef GLuint (APIENTRY * PFN_GLCREATESHADER)(GLenum type);
extern PFN_GLCREATESHADER _glCreateShader;

typedef void (APIENTRY * PFN_GLDELETEPROGRAM)(GLuint program);
extern PFN_GLDELETEPROGRAM _glDeleteProgram;

typedef void (APIENTRY * PFN_GLDELETESHADER)(GLuint shader);
extern PFN_GLDELETESHADER _glDeleteShader;

typedef void (APIENTRY * PFN_GLDETACHSHADER)(GLuint program, GLuint shader);
extern PFN_GLDETACHSHADER _glDetachShader;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXATTRIBARRAY)(GLuint index);
extern PFN_GLDISABLEVERTEXATTRIBARRAY _glDisableVertexAttribArray;

typedef void (APIENTRY * PFN_GLENABLEVERTEXATTRIBARRAY)(GLuint index);
extern PFN_GLENABLEVERTEXATTRIBARRAY _glEnableVertexAttribArray;

typedef void (APIENTRY * PFN_GLGETACTIVEATTRIB)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
extern PFN_GLGETACTIVEATTRIB _glGetActiveAttrib;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORM)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
extern PFN_GLGETACTIVEUNIFORM _glGetActiveUniform;

typedef void (APIENTRY * PFN_GLGETATTACHEDSHADERS)(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * obj);
extern PFN_GLGETATTACHEDSHADERS _glGetAttachedShaders;

typedef GLint (APIENTRY * PFN_GLGETATTRIBLOCATION)(GLuint program, const GLchar * name);
extern PFN_GLGETATTRIBLOCATION _glGetAttribLocation;

typedef void (APIENTRY * PFN_GLGETPROGRAMIV)(GLuint program, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMIV _glGetProgramiv;

typedef void (APIENTRY * PFN_GLGETPROGRAMINFOLOG)(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
extern PFN_GLGETPROGRAMINFOLOG _glGetProgramInfoLog;

typedef void (APIENTRY * PFN_GLGETSHADERIV)(GLuint shader, GLenum pname, GLint * params);
extern PFN_GLGETSHADERIV _glGetShaderiv;

typedef void (APIENTRY * PFN_GLGETSHADERINFOLOG)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
extern PFN_GLGETSHADERINFOLOG _glGetShaderInfoLog;

typedef void (APIENTRY * PFN_GLGETSHADERSOURCE)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
extern PFN_GLGETSHADERSOURCE _glGetShaderSource;

typedef GLint (APIENTRY * PFN_GLGETUNIFORMLOCATION)(GLuint program, const GLchar * name);
extern PFN_GLGETUNIFORMLOCATION _glGetUniformLocation;

typedef void (APIENTRY * PFN_GLGETUNIFORMFV)(GLuint program, GLint location, GLfloat * params);
extern PFN_GLGETUNIFORMFV _glGetUniformfv;

typedef void (APIENTRY * PFN_GLGETUNIFORMIV)(GLuint program, GLint location, GLint * params);
extern PFN_GLGETUNIFORMIV _glGetUniformiv;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBDV)(GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETVERTEXATTRIBDV _glGetVertexAttribdv;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBFV)(GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETVERTEXATTRIBFV _glGetVertexAttribfv;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIV)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBIV _glGetVertexAttribiv;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBPOINTERV)(GLuint index, GLenum pname, GLvoid * * pointer);
extern PFN_GLGETVERTEXATTRIBPOINTERV _glGetVertexAttribPointerv;

typedef GLboolean (APIENTRY * PFN_GLISPROGRAM)(GLuint program);
extern PFN_GLISPROGRAM _glIsProgram;

typedef GLboolean (APIENTRY * PFN_GLISSHADER)(GLuint shader);
extern PFN_GLISSHADER _glIsShader;

typedef void (APIENTRY * PFN_GLLINKPROGRAM)(GLuint program);
extern PFN_GLLINKPROGRAM _glLinkProgram;

typedef void (APIENTRY * PFN_GLSHADERSOURCE)(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length);
extern PFN_GLSHADERSOURCE _glShaderSource;

typedef void (APIENTRY * PFN_GLUSEPROGRAM)(GLuint program);
extern PFN_GLUSEPROGRAM _glUseProgram;

typedef void (APIENTRY * PFN_GLUNIFORM1F)(GLint location, GLfloat v0);
extern PFN_GLUNIFORM1F _glUniform1f;

typedef void (APIENTRY * PFN_GLUNIFORM2F)(GLint location, GLfloat v0, GLfloat v1);
extern PFN_GLUNIFORM2F _glUniform2f;

typedef void (APIENTRY * PFN_GLUNIFORM3F)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFN_GLUNIFORM3F _glUniform3f;

typedef void (APIENTRY * PFN_GLUNIFORM4F)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFN_GLUNIFORM4F _glUniform4f;

typedef void (APIENTRY * PFN_GLUNIFORM1I)(GLint location, GLint v0);
extern PFN_GLUNIFORM1I _glUniform1i;

typedef void (APIENTRY * PFN_GLUNIFORM2I)(GLint location, GLint v0, GLint v1);
extern PFN_GLUNIFORM2I _glUniform2i;

typedef void (APIENTRY * PFN_GLUNIFORM3I)(GLint location, GLint v0, GLint v1, GLint v2);
extern PFN_GLUNIFORM3I _glUniform3i;

typedef void (APIENTRY * PFN_GLUNIFORM4I)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern PFN_GLUNIFORM4I _glUniform4i;

typedef void (APIENTRY * PFN_GLUNIFORM1FV)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM1FV _glUniform1fv;

typedef void (APIENTRY * PFN_GLUNIFORM2FV)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM2FV _glUniform2fv;

typedef void (APIENTRY * PFN_GLUNIFORM3FV)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM3FV _glUniform3fv;

typedef void (APIENTRY * PFN_GLUNIFORM4FV)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM4FV _glUniform4fv;

typedef void (APIENTRY * PFN_GLUNIFORM1IV)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM1IV _glUniform1iv;

typedef void (APIENTRY * PFN_GLUNIFORM2IV)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM2IV _glUniform2iv;

typedef void (APIENTRY * PFN_GLUNIFORM3IV)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM3IV _glUniform3iv;

typedef void (APIENTRY * PFN_GLUNIFORM4IV)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM4IV _glUniform4iv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX2FV _glUniformMatrix2fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX3FV _glUniformMatrix3fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX4FV _glUniformMatrix4fv;

typedef void (APIENTRY * PFN_GLVALIDATEPROGRAM)(GLuint program);
extern PFN_GLVALIDATEPROGRAM _glValidateProgram;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1D)(GLuint index, GLdouble x);
extern PFN_GLVERTEXATTRIB1D _glVertexAttrib1d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB1DV _glVertexAttrib1dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1F)(GLuint index, GLfloat x);
extern PFN_GLVERTEXATTRIB1F _glVertexAttrib1f;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1FV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB1FV _glVertexAttrib1fv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1S)(GLuint index, GLshort x);
extern PFN_GLVERTEXATTRIB1S _glVertexAttrib1s;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1SV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB1SV _glVertexAttrib1sv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2D)(GLuint index, GLdouble x, GLdouble y);
extern PFN_GLVERTEXATTRIB2D _glVertexAttrib2d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB2DV _glVertexAttrib2dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2F)(GLuint index, GLfloat x, GLfloat y);
extern PFN_GLVERTEXATTRIB2F _glVertexAttrib2f;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2FV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB2FV _glVertexAttrib2fv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2S)(GLuint index, GLshort x, GLshort y);
extern PFN_GLVERTEXATTRIB2S _glVertexAttrib2s;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2SV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB2SV _glVertexAttrib2sv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3D)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXATTRIB3D _glVertexAttrib3d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB3DV _glVertexAttrib3dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3F)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLVERTEXATTRIB3F _glVertexAttrib3f;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3FV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB3FV _glVertexAttrib3fv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3S)(GLuint index, GLshort x, GLshort y, GLshort z);
extern PFN_GLVERTEXATTRIB3S _glVertexAttrib3s;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3SV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB3SV _glVertexAttrib3sv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NBV)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIB4NBV _glVertexAttrib4Nbv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NIV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIB4NIV _glVertexAttrib4Niv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NSV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB4NSV _glVertexAttrib4Nsv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern PFN_GLVERTEXATTRIB4NUB _glVertexAttrib4Nub;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUBV)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIB4NUBV _glVertexAttrib4Nubv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIB4NUIV _glVertexAttrib4Nuiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUSV)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIB4NUSV _glVertexAttrib4Nusv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4BV)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIB4BV _glVertexAttrib4bv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4D)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXATTRIB4D _glVertexAttrib4d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB4DV _glVertexAttrib4dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4F)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLVERTEXATTRIB4F _glVertexAttrib4f;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4FV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB4FV _glVertexAttrib4fv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4IV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIB4IV _glVertexAttrib4iv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4S)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLVERTEXATTRIB4S _glVertexAttrib4s;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4SV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB4SV _glVertexAttrib4sv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UBV)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIB4UBV _glVertexAttrib4ubv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIB4UIV _glVertexAttrib4uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4USV)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIB4USV _glVertexAttrib4usv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBPOINTER)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBPOINTER _glVertexAttribPointer;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2X3FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX2X3FV _glUniformMatrix2x3fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3X2FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX3X2FV _glUniformMatrix3x2fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2X4FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX2X4FV _glUniformMatrix2x4fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4X2FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX4X2FV _glUniformMatrix4x2fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3X4FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX3X4FV _glUniformMatrix3x4fv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4X3FV)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX4X3FV _glUniformMatrix4x3fv;

typedef void (APIENTRY * PFN_GLCOLORMASKI)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern PFN_GLCOLORMASKI _glColorMaski;

typedef void (APIENTRY * PFN_GLGETBOOLEANI_V)(GLenum target, GLuint index, GLboolean * data);
extern PFN_GLGETBOOLEANI_V _glGetBooleani_v;

typedef void (APIENTRY * PFN_GLGETINTEGERI_V)(GLenum target, GLuint index, GLint * data);
extern PFN_GLGETINTEGERI_V _glGetIntegeri_v;

typedef void (APIENTRY * PFN_GLENABLEI)(GLenum target, GLuint index);
extern PFN_GLENABLEI _glEnablei;

typedef void (APIENTRY * PFN_GLDISABLEI)(GLenum target, GLuint index);
extern PFN_GLDISABLEI _glDisablei;

typedef GLboolean (APIENTRY * PFN_GLISENABLEDI)(GLenum target, GLuint index);
extern PFN_GLISENABLEDI _glIsEnabledi;

typedef void (APIENTRY * PFN_GLBEGINTRANSFORMFEEDBACK)(GLenum primitiveMode);
extern PFN_GLBEGINTRANSFORMFEEDBACK _glBeginTransformFeedback;

typedef void (APIENTRY * PFN_GLENDTRANSFORMFEEDBACK)(void);
extern PFN_GLENDTRANSFORMFEEDBACK _glEndTransformFeedback;

typedef void (APIENTRY * PFN_GLBINDBUFFERRANGE)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFN_GLBINDBUFFERRANGE _glBindBufferRange;

typedef void (APIENTRY * PFN_GLBINDBUFFERBASE)(GLenum target, GLuint index, GLuint buffer);
extern PFN_GLBINDBUFFERBASE _glBindBufferBase;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKVARYINGS)(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode);
extern PFN_GLTRANSFORMFEEDBACKVARYINGS _glTransformFeedbackVaryings;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKVARYING)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
extern PFN_GLGETTRANSFORMFEEDBACKVARYING _glGetTransformFeedbackVarying;

typedef void (APIENTRY * PFN_GLCLAMPCOLOR)(GLenum target, GLenum clamp);
extern PFN_GLCLAMPCOLOR _glClampColor;

typedef void (APIENTRY * PFN_GLBEGINCONDITIONALRENDER)(GLuint id, GLenum mode);
extern PFN_GLBEGINCONDITIONALRENDER _glBeginConditionalRender;

typedef void (APIENTRY * PFN_GLENDCONDITIONALRENDER)(void);
extern PFN_GLENDCONDITIONALRENDER _glEndConditionalRender;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBIPOINTER)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBIPOINTER _glVertexAttribIPointer;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIIV)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBIIV _glGetVertexAttribIiv;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIUIV)(GLuint index, GLenum pname, GLuint * params);
extern PFN_GLGETVERTEXATTRIBIUIV _glGetVertexAttribIuiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1I)(GLuint index, GLint x);
extern PFN_GLVERTEXATTRIBI1I _glVertexAttribI1i;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2I)(GLuint index, GLint x, GLint y);
extern PFN_GLVERTEXATTRIBI2I _glVertexAttribI2i;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3I)(GLuint index, GLint x, GLint y, GLint z);
extern PFN_GLVERTEXATTRIBI3I _glVertexAttribI3i;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4I)(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLVERTEXATTRIBI4I _glVertexAttribI4i;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1UI)(GLuint index, GLuint x);
extern PFN_GLVERTEXATTRIBI1UI _glVertexAttribI1ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2UI)(GLuint index, GLuint x, GLuint y);
extern PFN_GLVERTEXATTRIBI2UI _glVertexAttribI2ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3UI)(GLuint index, GLuint x, GLuint y, GLuint z);
extern PFN_GLVERTEXATTRIBI3UI _glVertexAttribI3ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UI)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFN_GLVERTEXATTRIBI4UI _glVertexAttribI4ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1IV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI1IV _glVertexAttribI1iv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2IV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI2IV _glVertexAttribI2iv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3IV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI3IV _glVertexAttribI3iv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4IV)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI4IV _glVertexAttribI4iv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1UIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI1UIV _glVertexAttribI1uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2UIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI2UIV _glVertexAttribI2uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3UIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI3UIV _glVertexAttribI3uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UIV)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI4UIV _glVertexAttribI4uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4BV)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIBI4BV _glVertexAttribI4bv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4SV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIBI4SV _glVertexAttribI4sv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UBV)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIBI4UBV _glVertexAttribI4ubv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4USV)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIBI4USV _glVertexAttribI4usv;

typedef void (APIENTRY * PFN_GLGETUNIFORMUIV)(GLuint program, GLint location, GLuint * params);
extern PFN_GLGETUNIFORMUIV _glGetUniformuiv;

typedef void (APIENTRY * PFN_GLBINDFRAGDATALOCATION)(GLuint program, GLuint color, const GLchar * name);
extern PFN_GLBINDFRAGDATALOCATION _glBindFragDataLocation;

typedef GLint (APIENTRY * PFN_GLGETFRAGDATALOCATION)(GLuint program, const GLchar * name);
extern PFN_GLGETFRAGDATALOCATION _glGetFragDataLocation;

typedef void (APIENTRY * PFN_GLUNIFORM1UI)(GLint location, GLuint v0);
extern PFN_GLUNIFORM1UI _glUniform1ui;

typedef void (APIENTRY * PFN_GLUNIFORM2UI)(GLint location, GLuint v0, GLuint v1);
extern PFN_GLUNIFORM2UI _glUniform2ui;

typedef void (APIENTRY * PFN_GLUNIFORM3UI)(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern PFN_GLUNIFORM3UI _glUniform3ui;

typedef void (APIENTRY * PFN_GLUNIFORM4UI)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern PFN_GLUNIFORM4UI _glUniform4ui;

typedef void (APIENTRY * PFN_GLUNIFORM1UIV)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM1UIV _glUniform1uiv;

typedef void (APIENTRY * PFN_GLUNIFORM2UIV)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM2UIV _glUniform2uiv;

typedef void (APIENTRY * PFN_GLUNIFORM3UIV)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM3UIV _glUniform3uiv;

typedef void (APIENTRY * PFN_GLUNIFORM4UIV)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM4UIV _glUniform4uiv;

typedef void (APIENTRY * PFN_GLTEXPARAMETERIIV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXPARAMETERIIV _glTexParameterIiv;

typedef void (APIENTRY * PFN_GLTEXPARAMETERIUIV)(GLenum target, GLenum pname, const GLuint * params);
extern PFN_GLTEXPARAMETERIUIV _glTexParameterIuiv;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERIIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXPARAMETERIIV _glGetTexParameterIiv;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERIUIV)(GLenum target, GLenum pname, GLuint * params);
extern PFN_GLGETTEXPARAMETERIUIV _glGetTexParameterIuiv;

typedef void (APIENTRY * PFN_GLCLEARBUFFERIV)(GLenum buffer, GLint drawbuffer, const GLint * value);
extern PFN_GLCLEARBUFFERIV _glClearBufferiv;

typedef void (APIENTRY * PFN_GLCLEARBUFFERUIV)(GLenum buffer, GLint drawbuffer, const GLuint * value);
extern PFN_GLCLEARBUFFERUIV _glClearBufferuiv;

typedef void (APIENTRY * PFN_GLCLEARBUFFERFV)(GLenum buffer, GLint drawbuffer, const GLfloat * value);
extern PFN_GLCLEARBUFFERFV _glClearBufferfv;

typedef void (APIENTRY * PFN_GLCLEARBUFFERFI)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
extern PFN_GLCLEARBUFFERFI _glClearBufferfi;

typedef const GLubyte * (APIENTRY * PFN_GLGETSTRINGI)(GLenum name, GLuint index);
extern PFN_GLGETSTRINGI _glGetStringi;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINSTANCED)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern PFN_GLDRAWARRAYSINSTANCED _glDrawArraysInstanced;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCED)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
extern PFN_GLDRAWELEMENTSINSTANCED _glDrawElementsInstanced;

typedef void (APIENTRY * PFN_GLTEXBUFFER)(GLenum target, GLenum internalformat, GLuint buffer);
extern PFN_GLTEXBUFFER _glTexBuffer;

typedef void (APIENTRY * PFN_GLPRIMITIVERESTARTINDEX)(GLuint index);
extern PFN_GLPRIMITIVERESTARTINDEX _glPrimitiveRestartIndex;

typedef void (APIENTRY * PFN_GLGETINTEGER64I_V)(GLenum target, GLuint index, GLint64 * data);
extern PFN_GLGETINTEGER64I_V _glGetInteger64i_v;

typedef void (APIENTRY * PFN_GLGETBUFFERPARAMETERI64V)(GLenum target, GLenum pname, GLint64 * params);
extern PFN_GLGETBUFFERPARAMETERI64V _glGetBufferParameteri64v;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE _glFramebufferTexture;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBDIVISOR)(GLuint index, GLuint divisor);
extern PFN_GLVERTEXATTRIBDIVISOR _glVertexAttribDivisor;

typedef void (APIENTRY * PFN_GLMINSAMPLESHADING)(GLfloat value);
extern PFN_GLMINSAMPLESHADING _glMinSampleShading;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONI)(GLuint buf, GLenum mode);
extern PFN_GLBLENDEQUATIONI _glBlendEquationi;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONSEPARATEI)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATEI _glBlendEquationSeparatei;

typedef void (APIENTRY * PFN_GLBLENDFUNCI)(GLuint buf, GLenum src, GLenum dst);
extern PFN_GLBLENDFUNCI _glBlendFunci;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATEI)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern PFN_GLBLENDFUNCSEPARATEI _glBlendFuncSeparatei;

typedef void (APIENTRY * PFN_GLTBUFFERMASK3DFX)(GLuint mask);
extern PFN_GLTBUFFERMASK3DFX _glTbufferMask3DFX;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGEENABLEAMD)(GLenum category, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
extern PFN_GLDEBUGMESSAGEENABLEAMD _glDebugMessageEnableAMD;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGEINSERTAMD)(GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar * buf);
extern PFN_GLDEBUGMESSAGEINSERTAMD _glDebugMessageInsertAMD;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGECALLBACKAMD)(GLDEBUGPROCAMD callback, GLvoid * userParam);
extern PFN_GLDEBUGMESSAGECALLBACKAMD _glDebugMessageCallbackAMD;

typedef GLuint (APIENTRY * PFN_GLGETDEBUGMESSAGELOGAMD)(GLuint count, GLsizei bufsize, GLenum * categories, GLuint * severities, GLuint * ids, GLsizei * lengths, GLchar * message);
extern PFN_GLGETDEBUGMESSAGELOGAMD _glGetDebugMessageLogAMD;

typedef void (APIENTRY * PFN_GLBLENDFUNCINDEXEDAMD)(GLuint buf, GLenum src, GLenum dst);
extern PFN_GLBLENDFUNCINDEXEDAMD _glBlendFuncIndexedAMD;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATEINDEXEDAMD)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern PFN_GLBLENDFUNCSEPARATEINDEXEDAMD _glBlendFuncSeparateIndexedAMD;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONINDEXEDAMD)(GLuint buf, GLenum mode);
extern PFN_GLBLENDEQUATIONINDEXEDAMD _glBlendEquationIndexedAMD;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONSEPARATEINDEXEDAMD)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATEINDEXEDAMD _glBlendEquationSeparateIndexedAMD;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYSINDIRECTAMD)(GLenum mode, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
extern PFN_GLMULTIDRAWARRAYSINDIRECTAMD _glMultiDrawArraysIndirectAMD;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSINDIRECTAMD)(GLenum mode, GLenum type, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
extern PFN_GLMULTIDRAWELEMENTSINDIRECTAMD _glMultiDrawElementsIndirectAMD;

typedef void (APIENTRY * PFN_GLGENNAMESAMD)(GLenum identifier, GLuint num, GLuint * names);
extern PFN_GLGENNAMESAMD _glGenNamesAMD;

typedef void (APIENTRY * PFN_GLDELETENAMESAMD)(GLenum identifier, GLuint num, const GLuint * names);
extern PFN_GLDELETENAMESAMD _glDeleteNamesAMD;

typedef GLboolean (APIENTRY * PFN_GLISNAMEAMD)(GLenum identifier, GLuint name);
extern PFN_GLISNAMEAMD _glIsNameAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORGROUPSAMD)(GLint * numGroups, GLsizei groupsSize, GLuint * groups);
extern PFN_GLGETPERFMONITORGROUPSAMD _glGetPerfMonitorGroupsAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORCOUNTERSAMD)(GLuint group, GLint * numCounters, GLint * maxActiveCounters, GLsizei counterSize, GLuint * counters);
extern PFN_GLGETPERFMONITORCOUNTERSAMD _glGetPerfMonitorCountersAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORGROUPSTRINGAMD)(GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString);
extern PFN_GLGETPERFMONITORGROUPSTRINGAMD _glGetPerfMonitorGroupStringAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORCOUNTERSTRINGAMD)(GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length, GLchar * counterString);
extern PFN_GLGETPERFMONITORCOUNTERSTRINGAMD _glGetPerfMonitorCounterStringAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORCOUNTERINFOAMD)(GLuint group, GLuint counter, GLenum pname, GLvoid * data);
extern PFN_GLGETPERFMONITORCOUNTERINFOAMD _glGetPerfMonitorCounterInfoAMD;

typedef void (APIENTRY * PFN_GLGENPERFMONITORSAMD)(GLsizei n, GLuint * monitors);
extern PFN_GLGENPERFMONITORSAMD _glGenPerfMonitorsAMD;

typedef void (APIENTRY * PFN_GLDELETEPERFMONITORSAMD)(GLsizei n, GLuint * monitors);
extern PFN_GLDELETEPERFMONITORSAMD _glDeletePerfMonitorsAMD;

typedef void (APIENTRY * PFN_GLSELECTPERFMONITORCOUNTERSAMD)(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint * counterList);
extern PFN_GLSELECTPERFMONITORCOUNTERSAMD _glSelectPerfMonitorCountersAMD;

typedef void (APIENTRY * PFN_GLBEGINPERFMONITORAMD)(GLuint monitor);
extern PFN_GLBEGINPERFMONITORAMD _glBeginPerfMonitorAMD;

typedef void (APIENTRY * PFN_GLENDPERFMONITORAMD)(GLuint monitor);
extern PFN_GLENDPERFMONITORAMD _glEndPerfMonitorAMD;

typedef void (APIENTRY * PFN_GLGETPERFMONITORCOUNTERDATAAMD)(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint * data, GLint * bytesWritten);
extern PFN_GLGETPERFMONITORCOUNTERDATAAMD _glGetPerfMonitorCounterDataAMD;

typedef void (APIENTRY * PFN_GLSETMULTISAMPLEFVAMD)(GLenum pname, GLuint index, const GLfloat * val);
extern PFN_GLSETMULTISAMPLEFVAMD _glSetMultisamplefvAMD;

typedef void (APIENTRY * PFN_GLTEXSTORAGESPARSEAMD)(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
extern PFN_GLTEXSTORAGESPARSEAMD _glTexStorageSparseAMD;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGESPARSEAMD)(GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
extern PFN_GLTEXTURESTORAGESPARSEAMD _glTextureStorageSparseAMD;

typedef void (APIENTRY * PFN_GLSTENCILOPVALUEAMD)(GLenum face, GLuint value);
extern PFN_GLSTENCILOPVALUEAMD _glStencilOpValueAMD;

typedef void (APIENTRY * PFN_GLTESSELLATIONFACTORAMD)(GLfloat factor);
extern PFN_GLTESSELLATIONFACTORAMD _glTessellationFactorAMD;

typedef void (APIENTRY * PFN_GLTESSELLATIONMODEAMD)(GLenum mode);
extern PFN_GLTESSELLATIONMODEAMD _glTessellationModeAMD;

typedef void (APIENTRY * PFN_GLBLITFRAMEBUFFERANGLE)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_GLBLITFRAMEBUFFERANGLE _glBlitFramebufferANGLE;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEANGLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEANGLE _glRenderbufferStorageMultisampleANGLE;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINSTANCEDANGLE)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern PFN_GLDRAWARRAYSINSTANCEDANGLE _glDrawArraysInstancedANGLE;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDANGLE)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
extern PFN_GLDRAWELEMENTSINSTANCEDANGLE _glDrawElementsInstancedANGLE;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBDIVISORANGLE)(GLuint index, GLuint divisor);
extern PFN_GLVERTEXATTRIBDIVISORANGLE _glVertexAttribDivisorANGLE;

typedef void (APIENTRY * PFN_GLGETTRANSLATEDSHADERSOURCEANGLE)(GLuint shader, GLsizei bufsize, GLsizei * length, GLchar * source);
extern PFN_GLGETTRANSLATEDSHADERSOURCEANGLE _glGetTranslatedShaderSourceANGLE;

typedef void (APIENTRY * PFN_GLCOPYTEXTURELEVELSAPPLE)(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);
extern PFN_GLCOPYTEXTURELEVELSAPPLE _glCopyTextureLevelsAPPLE;

typedef void (APIENTRY * PFN_GLELEMENTPOINTERAPPLE)(GLenum type, const GLvoid * pointer);
extern PFN_GLELEMENTPOINTERAPPLE _glElementPointerAPPLE;

typedef void (APIENTRY * PFN_GLDRAWELEMENTARRAYAPPLE)(GLenum mode, GLint first, GLsizei count);
extern PFN_GLDRAWELEMENTARRAYAPPLE _glDrawElementArrayAPPLE;

typedef void (APIENTRY * PFN_GLDRAWRANGEELEMENTARRAYAPPLE)(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
extern PFN_GLDRAWRANGEELEMENTARRAYAPPLE _glDrawRangeElementArrayAPPLE;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTARRAYAPPLE)(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);
extern PFN_GLMULTIDRAWELEMENTARRAYAPPLE _glMultiDrawElementArrayAPPLE;

typedef void (APIENTRY * PFN_GLMULTIDRAWRANGEELEMENTARRAYAPPLE)(GLenum mode, GLuint start, GLuint end, const GLint * first, const GLsizei * count, GLsizei primcount);
extern PFN_GLMULTIDRAWRANGEELEMENTARRAYAPPLE _glMultiDrawRangeElementArrayAPPLE;

typedef void (APIENTRY * PFN_GLGENFENCESAPPLE)(GLsizei n, GLuint * fences);
extern PFN_GLGENFENCESAPPLE _glGenFencesAPPLE;

typedef void (APIENTRY * PFN_GLDELETEFENCESAPPLE)(GLsizei n, const GLuint * fences);
extern PFN_GLDELETEFENCESAPPLE _glDeleteFencesAPPLE;

typedef void (APIENTRY * PFN_GLSETFENCEAPPLE)(GLuint fence);
extern PFN_GLSETFENCEAPPLE _glSetFenceAPPLE;

typedef GLboolean (APIENTRY * PFN_GLISFENCEAPPLE)(GLuint fence);
extern PFN_GLISFENCEAPPLE _glIsFenceAPPLE;

typedef GLboolean (APIENTRY * PFN_GLTESTFENCEAPPLE)(GLuint fence);
extern PFN_GLTESTFENCEAPPLE _glTestFenceAPPLE;

typedef void (APIENTRY * PFN_GLFINISHFENCEAPPLE)(GLuint fence);
extern PFN_GLFINISHFENCEAPPLE _glFinishFenceAPPLE;

typedef GLboolean (APIENTRY * PFN_GLTESTOBJECTAPPLE)(GLenum object, GLuint name);
extern PFN_GLTESTOBJECTAPPLE _glTestObjectAPPLE;

typedef void (APIENTRY * PFN_GLFINISHOBJECTAPPLE)(GLenum object, GLint name);
extern PFN_GLFINISHOBJECTAPPLE _glFinishObjectAPPLE;

typedef void (APIENTRY * PFN_GLBUFFERPARAMETERIAPPLE)(GLenum target, GLenum pname, GLint param);
extern PFN_GLBUFFERPARAMETERIAPPLE _glBufferParameteriAPPLE;

typedef void (APIENTRY * PFN_GLFLUSHMAPPEDBUFFERRANGEAPPLE)(GLenum target, GLintptr offset, GLsizeiptr size);
extern PFN_GLFLUSHMAPPEDBUFFERRANGEAPPLE _glFlushMappedBufferRangeAPPLE;

typedef void (APIENTRY * PFN_GLFLUSHRENDERAPPLE)(void);
extern PFN_GLFLUSHRENDERAPPLE _glFlushRenderAPPLE;

typedef void (APIENTRY * PFN_GLFINISHRENDERAPPLE)(void);
extern PFN_GLFINISHRENDERAPPLE _glFinishRenderAPPLE;

typedef void (APIENTRY * PFN_GLSWAPAPPLE)(void);
extern PFN_GLSWAPAPPLE _glSwapAPPLE;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEAPPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEAPPLE _glRenderbufferStorageMultisampleAPPLE;

typedef void (APIENTRY * PFN_GLRESOLVEMULTISAMPLEFRAMEBUFFERAPPLE)(void);
extern PFN_GLRESOLVEMULTISAMPLEFRAMEBUFFERAPPLE _glResolveMultisampleFramebufferAPPLE;

typedef GLenum (APIENTRY * PFN_GLOBJECTPURGEABLEAPPLE)(GLenum objectType, GLuint name, GLenum option);
extern PFN_GLOBJECTPURGEABLEAPPLE _glObjectPurgeableAPPLE;

typedef GLenum (APIENTRY * PFN_GLOBJECTUNPURGEABLEAPPLE)(GLenum objectType, GLuint name, GLenum option);
extern PFN_GLOBJECTUNPURGEABLEAPPLE _glObjectUnpurgeableAPPLE;

typedef void (APIENTRY * PFN_GLGETOBJECTPARAMETERIVAPPLE)(GLenum objectType, GLuint name, GLenum pname, GLint * params);
extern PFN_GLGETOBJECTPARAMETERIVAPPLE _glGetObjectParameterivAPPLE;

typedef GLsync (APIENTRY * PFN_GLFENCESYNCAPPLE)(GLenum condition, GLbitfield flags);
extern PFN_GLFENCESYNCAPPLE _glFenceSyncAPPLE;

typedef GLboolean (APIENTRY * PFN_GLISSYNCAPPLE)(GLsync sync);
extern PFN_GLISSYNCAPPLE _glIsSyncAPPLE;

typedef void (APIENTRY * PFN_GLDELETESYNCAPPLE)(GLsync sync);
extern PFN_GLDELETESYNCAPPLE _glDeleteSyncAPPLE;

typedef GLenum (APIENTRY * PFN_GLCLIENTWAITSYNCAPPLE)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFN_GLCLIENTWAITSYNCAPPLE _glClientWaitSyncAPPLE;

typedef void (APIENTRY * PFN_GLWAITSYNCAPPLE)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFN_GLWAITSYNCAPPLE _glWaitSyncAPPLE;

typedef void (APIENTRY * PFN_GLGETINTEGER64VAPPLE)(GLenum pname, GLint64 * params);
extern PFN_GLGETINTEGER64VAPPLE _glGetInteger64vAPPLE;

typedef void (APIENTRY * PFN_GLGETSYNCIVAPPLE)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values);
extern PFN_GLGETSYNCIVAPPLE _glGetSyncivAPPLE;

typedef void (APIENTRY * PFN_GLTEXTURERANGEAPPLE)(GLenum target, GLsizei length, const GLvoid * pointer);
extern PFN_GLTEXTURERANGEAPPLE _glTextureRangeAPPLE;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERPOINTERVAPPLE)(GLenum target, GLenum pname, GLvoid * * params);
extern PFN_GLGETTEXPARAMETERPOINTERVAPPLE _glGetTexParameterPointervAPPLE;

typedef void (APIENTRY * PFN_GLBINDVERTEXARRAYAPPLE)(GLuint array);
extern PFN_GLBINDVERTEXARRAYAPPLE _glBindVertexArrayAPPLE;

typedef void (APIENTRY * PFN_GLDELETEVERTEXARRAYSAPPLE)(GLsizei n, const GLuint * arrays);
extern PFN_GLDELETEVERTEXARRAYSAPPLE _glDeleteVertexArraysAPPLE;

typedef void (APIENTRY * PFN_GLGENVERTEXARRAYSAPPLE)(GLsizei n, GLuint * arrays);
extern PFN_GLGENVERTEXARRAYSAPPLE _glGenVertexArraysAPPLE;

typedef GLboolean (APIENTRY * PFN_GLISVERTEXARRAYAPPLE)(GLuint array);
extern PFN_GLISVERTEXARRAYAPPLE _glIsVertexArrayAPPLE;

typedef void (APIENTRY * PFN_GLVERTEXARRAYRANGEAPPLE)(GLsizei length, GLvoid * pointer);
extern PFN_GLVERTEXARRAYRANGEAPPLE _glVertexArrayRangeAPPLE;

typedef void (APIENTRY * PFN_GLFLUSHVERTEXARRAYRANGEAPPLE)(GLsizei length, GLvoid * pointer);
extern PFN_GLFLUSHVERTEXARRAYRANGEAPPLE _glFlushVertexArrayRangeAPPLE;

typedef void (APIENTRY * PFN_GLVERTEXARRAYPARAMETERIAPPLE)(GLenum pname, GLint param);
extern PFN_GLVERTEXARRAYPARAMETERIAPPLE _glVertexArrayParameteriAPPLE;

typedef void (APIENTRY * PFN_GLENABLEVERTEXATTRIBAPPLE)(GLuint index, GLenum pname);
extern PFN_GLENABLEVERTEXATTRIBAPPLE _glEnableVertexAttribAPPLE;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXATTRIBAPPLE)(GLuint index, GLenum pname);
extern PFN_GLDISABLEVERTEXATTRIBAPPLE _glDisableVertexAttribAPPLE;

typedef GLboolean (APIENTRY * PFN_GLISVERTEXATTRIBENABLEDAPPLE)(GLuint index, GLenum pname);
extern PFN_GLISVERTEXATTRIBENABLEDAPPLE _glIsVertexAttribEnabledAPPLE;

typedef void (APIENTRY * PFN_GLMAPVERTEXATTRIB1DAPPLE)(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);
extern PFN_GLMAPVERTEXATTRIB1DAPPLE _glMapVertexAttrib1dAPPLE;

typedef void (APIENTRY * PFN_GLMAPVERTEXATTRIB1FAPPLE)(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);
extern PFN_GLMAPVERTEXATTRIB1FAPPLE _glMapVertexAttrib1fAPPLE;

typedef void (APIENTRY * PFN_GLMAPVERTEXATTRIB2DAPPLE)(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);
extern PFN_GLMAPVERTEXATTRIB2DAPPLE _glMapVertexAttrib2dAPPLE;

typedef void (APIENTRY * PFN_GLMAPVERTEXATTRIB2FAPPLE)(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);
extern PFN_GLMAPVERTEXATTRIB2FAPPLE _glMapVertexAttrib2fAPPLE;

typedef void (APIENTRY * PFN_GLRELEASESHADERCOMPILER)(void);
extern PFN_GLRELEASESHADERCOMPILER _glReleaseShaderCompiler;

typedef void (APIENTRY * PFN_GLSHADERBINARY)(GLsizei count, const GLuint * shaders, GLenum binaryformat, const GLvoid * binary, GLsizei length);
extern PFN_GLSHADERBINARY _glShaderBinary;

typedef void (APIENTRY * PFN_GLGETSHADERPRECISIONFORMAT)(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision);
extern PFN_GLGETSHADERPRECISIONFORMAT _glGetShaderPrecisionFormat;

typedef void (APIENTRY * PFN_GLDEPTHRANGEF)(GLfloat n, GLfloat f);
extern PFN_GLDEPTHRANGEF _glDepthRangef;

typedef void (APIENTRY * PFN_GLCLEARDEPTHF)(GLfloat d);
extern PFN_GLCLEARDEPTHF _glClearDepthf;

typedef void (APIENTRY * PFN_GLMEMORYBARRIERBYREGION)(GLbitfield barriers);
extern PFN_GLMEMORYBARRIERBYREGION _glMemoryBarrierByRegion;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINSTANCEDBASEINSTANCE)(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
extern PFN_GLDRAWARRAYSINSTANCEDBASEINSTANCE _glDrawArraysInstancedBaseInstance;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDBASEINSTANCE)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLuint baseinstance);
extern PFN_GLDRAWELEMENTSINSTANCEDBASEINSTANCE _glDrawElementsInstancedBaseInstance;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
extern PFN_GLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE _glDrawElementsInstancedBaseVertexBaseInstance;

typedef GLuint64 (APIENTRY * PFN_GLGETTEXTUREHANDLEARB)(GLuint texture);
extern PFN_GLGETTEXTUREHANDLEARB _glGetTextureHandleARB;

typedef GLuint64 (APIENTRY * PFN_GLGETTEXTURESAMPLERHANDLEARB)(GLuint texture, GLuint sampler);
extern PFN_GLGETTEXTURESAMPLERHANDLEARB _glGetTextureSamplerHandleARB;

typedef void (APIENTRY * PFN_GLMAKETEXTUREHANDLERESIDENTARB)(GLuint64 handle);
extern PFN_GLMAKETEXTUREHANDLERESIDENTARB _glMakeTextureHandleResidentARB;

typedef void (APIENTRY * PFN_GLMAKETEXTUREHANDLENONRESIDENTARB)(GLuint64 handle);
extern PFN_GLMAKETEXTUREHANDLENONRESIDENTARB _glMakeTextureHandleNonResidentARB;

typedef GLuint64 (APIENTRY * PFN_GLGETIMAGEHANDLEARB)(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
extern PFN_GLGETIMAGEHANDLEARB _glGetImageHandleARB;

typedef void (APIENTRY * PFN_GLMAKEIMAGEHANDLERESIDENTARB)(GLuint64 handle, GLenum access);
extern PFN_GLMAKEIMAGEHANDLERESIDENTARB _glMakeImageHandleResidentARB;

typedef void (APIENTRY * PFN_GLMAKEIMAGEHANDLENONRESIDENTARB)(GLuint64 handle);
extern PFN_GLMAKEIMAGEHANDLENONRESIDENTARB _glMakeImageHandleNonResidentARB;

typedef void (APIENTRY * PFN_GLUNIFORMHANDLEUI64ARB)(GLint location, GLuint64 value);
extern PFN_GLUNIFORMHANDLEUI64ARB _glUniformHandleui64ARB;

typedef void (APIENTRY * PFN_GLUNIFORMHANDLEUI64VARB)(GLint location, GLsizei count, const GLuint64 * value);
extern PFN_GLUNIFORMHANDLEUI64VARB _glUniformHandleui64vARB;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMHANDLEUI64ARB)(GLuint program, GLint location, GLuint64 value);
extern PFN_GLPROGRAMUNIFORMHANDLEUI64ARB _glProgramUniformHandleui64ARB;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMHANDLEUI64VARB)(GLuint program, GLint location, GLsizei count, const GLuint64 * values);
extern PFN_GLPROGRAMUNIFORMHANDLEUI64VARB _glProgramUniformHandleui64vARB;

typedef GLboolean (APIENTRY * PFN_GLISTEXTUREHANDLERESIDENTARB)(GLuint64 handle);
extern PFN_GLISTEXTUREHANDLERESIDENTARB _glIsTextureHandleResidentARB;

typedef GLboolean (APIENTRY * PFN_GLISIMAGEHANDLERESIDENTARB)(GLuint64 handle);
extern PFN_GLISIMAGEHANDLERESIDENTARB _glIsImageHandleResidentARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1UI64ARB)(GLuint index, GLuint64EXT x);
extern PFN_GLVERTEXATTRIBL1UI64ARB _glVertexAttribL1ui64ARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1UI64VARB)(GLuint index, const GLuint64EXT * v);
extern PFN_GLVERTEXATTRIBL1UI64VARB _glVertexAttribL1ui64vARB;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBLUI64VARB)(GLuint index, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETVERTEXATTRIBLUI64VARB _glGetVertexAttribLui64vARB;

typedef void (APIENTRY * PFN_GLBINDFRAGDATALOCATIONINDEXED)(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
extern PFN_GLBINDFRAGDATALOCATIONINDEXED _glBindFragDataLocationIndexed;

typedef GLint (APIENTRY * PFN_GLGETFRAGDATAINDEX)(GLuint program, const GLchar * name);
extern PFN_GLGETFRAGDATAINDEX _glGetFragDataIndex;

typedef void (APIENTRY * PFN_GLBUFFERSTORAGE)(GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags);
extern PFN_GLBUFFERSTORAGE _glBufferStorage;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERSTORAGEEXT)(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags);
extern PFN_GLNAMEDBUFFERSTORAGEEXT _glNamedBufferStorageEXT;

typedef void (APIENTRY * PFN_GLCLEARBUFFERDATA)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARBUFFERDATA _glClearBufferData;

typedef void (APIENTRY * PFN_GLCLEARBUFFERSUBDATA)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARBUFFERSUBDATA _glClearBufferSubData;

typedef void (APIENTRY * PFN_GLCLEARNAMEDBUFFERDATAEXT)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARNAMEDBUFFERDATAEXT _glClearNamedBufferDataEXT;

typedef void (APIENTRY * PFN_GLCLEARNAMEDBUFFERSUBDATAEXT)(GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARNAMEDBUFFERSUBDATAEXT _glClearNamedBufferSubDataEXT;

typedef void (APIENTRY * PFN_GLCLEARTEXIMAGE)(GLuint texture, GLint level, GLenum format, GLenum type, const GLvoid * data);
extern PFN_GLCLEARTEXIMAGE _glClearTexImage;

typedef void (APIENTRY * PFN_GLCLEARTEXSUBIMAGE)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);
extern PFN_GLCLEARTEXSUBIMAGE _glClearTexSubImage;

typedef void (APIENTRY * PFN_GLCLIPCONTROL)(GLenum origin, GLenum depth);
extern PFN_GLCLIPCONTROL _glClipControl;

typedef void (APIENTRY * PFN_GLCLAMPCOLORARB)(GLenum target, GLenum clamp);
extern PFN_GLCLAMPCOLORARB _glClampColorARB;

typedef void (APIENTRY * PFN_GLDISPATCHCOMPUTE)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
extern PFN_GLDISPATCHCOMPUTE _glDispatchCompute;

typedef void (APIENTRY * PFN_GLDISPATCHCOMPUTEINDIRECT)(GLintptr indirect);
extern PFN_GLDISPATCHCOMPUTEINDIRECT _glDispatchComputeIndirect;

typedef void (APIENTRY * PFN_GLDISPATCHCOMPUTEGROUPSIZEARB)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
extern PFN_GLDISPATCHCOMPUTEGROUPSIZEARB _glDispatchComputeGroupSizeARB;

typedef void (APIENTRY * PFN_GLCOPYBUFFERSUBDATA)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern PFN_GLCOPYBUFFERSUBDATA _glCopyBufferSubData;

typedef void (APIENTRY * PFN_GLCOPYIMAGESUBDATA)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
extern PFN_GLCOPYIMAGESUBDATA _glCopyImageSubData;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGECONTROLARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
extern PFN_GLDEBUGMESSAGECONTROLARB _glDebugMessageControlARB;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGEINSERTARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
extern PFN_GLDEBUGMESSAGEINSERTARB _glDebugMessageInsertARB;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGECALLBACKARB)(GLDEBUGPROCARB callback, const GLvoid * userParam);
extern PFN_GLDEBUGMESSAGECALLBACKARB _glDebugMessageCallbackARB;

typedef GLuint (APIENTRY * PFN_GLGETDEBUGMESSAGELOGARB)(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
extern PFN_GLGETDEBUGMESSAGELOGARB _glGetDebugMessageLogARB;

typedef void (APIENTRY * PFN_GLCREATETRANSFORMFEEDBACKS)(GLsizei n, GLuint * ids);
extern PFN_GLCREATETRANSFORMFEEDBACKS _glCreateTransformFeedbacks;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKBUFFERBASE)(GLuint xfb, GLuint index, GLuint buffer);
extern PFN_GLTRANSFORMFEEDBACKBUFFERBASE _glTransformFeedbackBufferBase;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKBUFFERRANGE)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizei size);
extern PFN_GLTRANSFORMFEEDBACKBUFFERRANGE _glTransformFeedbackBufferRange;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKIV)(GLuint xfb, GLenum pname, GLint * param);
extern PFN_GLGETTRANSFORMFEEDBACKIV _glGetTransformFeedbackiv;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKI_V)(GLuint xfb, GLenum pname, GLuint index, GLint * param);
extern PFN_GLGETTRANSFORMFEEDBACKI_V _glGetTransformFeedbacki_v;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKI64_V)(GLuint xfb, GLenum pname, GLuint index, GLint64 * param);
extern PFN_GLGETTRANSFORMFEEDBACKI64_V _glGetTransformFeedbacki64_v;

typedef void (APIENTRY * PFN_GLCREATEBUFFERS)(GLsizei n, GLuint * buffers);
extern PFN_GLCREATEBUFFERS _glCreateBuffers;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERSTORAGE)(GLuint buffer, GLsizei size, const void * data, GLbitfield flags);
extern PFN_GLNAMEDBUFFERSTORAGE _glNamedBufferStorage;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERDATA)(GLuint buffer, GLsizei size, const void * data, GLenum usage);
extern PFN_GLNAMEDBUFFERDATA _glNamedBufferData;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERSUBDATA)(GLuint buffer, GLintptr offset, GLsizei size, const void * data);
extern PFN_GLNAMEDBUFFERSUBDATA _glNamedBufferSubData;

typedef void (APIENTRY * PFN_GLCOPYNAMEDBUFFERSUBDATA)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizei size);
extern PFN_GLCOPYNAMEDBUFFERSUBDATA _glCopyNamedBufferSubData;

typedef void (APIENTRY * PFN_GLCLEARNAMEDBUFFERDATA)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARNAMEDBUFFERDATA _glClearNamedBufferData;

typedef void (APIENTRY * PFN_GLCLEARNAMEDBUFFERSUBDATA)(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizei size, GLenum format, GLenum type, const void * data);
extern PFN_GLCLEARNAMEDBUFFERSUBDATA _glClearNamedBufferSubData;

typedef GLvoid * (APIENTRY * PFN_GLMAPNAMEDBUFFER)(GLuint buffer, GLenum access);
extern PFN_GLMAPNAMEDBUFFER _glMapNamedBuffer;

typedef GLvoid * (APIENTRY * PFN_GLMAPNAMEDBUFFERRANGE)(GLuint buffer, GLintptr offset, GLsizei length, GLbitfield access);
extern PFN_GLMAPNAMEDBUFFERRANGE _glMapNamedBufferRange;

typedef GLboolean (APIENTRY * PFN_GLUNMAPNAMEDBUFFER)(GLuint buffer);
extern PFN_GLUNMAPNAMEDBUFFER _glUnmapNamedBuffer;

typedef void (APIENTRY * PFN_GLFLUSHMAPPEDNAMEDBUFFERRANGE)(GLuint buffer, GLintptr offset, GLsizei length);
extern PFN_GLFLUSHMAPPEDNAMEDBUFFERRANGE _glFlushMappedNamedBufferRange;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPARAMETERIV)(GLuint buffer, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDBUFFERPARAMETERIV _glGetNamedBufferParameteriv;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPARAMETERI64V)(GLuint buffer, GLenum pname, GLint64 * params);
extern PFN_GLGETNAMEDBUFFERPARAMETERI64V _glGetNamedBufferParameteri64v;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPOINTERV)(GLuint buffer, GLenum pname, GLvoid * * params);
extern PFN_GLGETNAMEDBUFFERPOINTERV _glGetNamedBufferPointerv;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERSUBDATA)(GLuint buffer, GLintptr offset, GLsizei size, void * data);
extern PFN_GLGETNAMEDBUFFERSUBDATA _glGetNamedBufferSubData;

typedef void (APIENTRY * PFN_GLCREATEFRAMEBUFFERS)(GLsizei n, GLuint * framebuffers);
extern PFN_GLCREATEFRAMEBUFFERS _glCreateFramebuffers;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERRENDERBUFFER)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFN_GLNAMEDFRAMEBUFFERRENDERBUFFER _glNamedFramebufferRenderbuffer;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERPARAMETERI)(GLuint framebuffer, GLenum pname, GLint param);
extern PFN_GLNAMEDFRAMEBUFFERPARAMETERI _glNamedFramebufferParameteri;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURE)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURE _glNamedFramebufferTexture;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURELAYER)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURELAYER _glNamedFramebufferTextureLayer;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERDRAWBUFFER)(GLuint framebuffer, GLenum buf);
extern PFN_GLNAMEDFRAMEBUFFERDRAWBUFFER _glNamedFramebufferDrawBuffer;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERDRAWBUFFERS)(GLuint framebuffer, GLsizei n, const GLenum * bufs);
extern PFN_GLNAMEDFRAMEBUFFERDRAWBUFFERS _glNamedFramebufferDrawBuffers;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERREADBUFFER)(GLuint framebuffer, GLenum src);
extern PFN_GLNAMEDFRAMEBUFFERREADBUFFER _glNamedFramebufferReadBuffer;

typedef void (APIENTRY * PFN_GLINVALIDATENAMEDFRAMEBUFFERDATA)(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments);
extern PFN_GLINVALIDATENAMEDFRAMEBUFFERDATA _glInvalidateNamedFramebufferData;

typedef void (APIENTRY * PFN_GLINVALIDATENAMEDFRAMEBUFFERSUBDATA)(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLINVALIDATENAMEDFRAMEBUFFERSUBDATA _glInvalidateNamedFramebufferSubData;

typedef void (APIENTRY * PFN_GLCLEARNAMEDFRAMEBUFFERIV)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value);
extern PFN_GLCLEARNAMEDFRAMEBUFFERIV _glClearNamedFramebufferiv;

typedef void (APIENTRY * PFN_GLCLEARNAMEDFRAMEBUFFERUIV)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value);
extern PFN_GLCLEARNAMEDFRAMEBUFFERUIV _glClearNamedFramebufferuiv;

typedef void (APIENTRY * PFN_GLCLEARNAMEDFRAMEBUFFERFV)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value);
extern PFN_GLCLEARNAMEDFRAMEBUFFERFV _glClearNamedFramebufferfv;

typedef void (APIENTRY * PFN_GLCLEARNAMEDFRAMEBUFFERFI)(GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil);
extern PFN_GLCLEARNAMEDFRAMEBUFFERFI _glClearNamedFramebufferfi;

typedef void (APIENTRY * PFN_GLBLITNAMEDFRAMEBUFFER)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_GLBLITNAMEDFRAMEBUFFER _glBlitNamedFramebuffer;

typedef GLenum (APIENTRY * PFN_GLCHECKNAMEDFRAMEBUFFERSTATUS)(GLuint framebuffer, GLenum target);
extern PFN_GLCHECKNAMEDFRAMEBUFFERSTATUS _glCheckNamedFramebufferStatus;

typedef void (APIENTRY * PFN_GLGETNAMEDFRAMEBUFFERPARAMETERIV)(GLuint framebuffer, GLenum pname, GLint * param);
extern PFN_GLGETNAMEDFRAMEBUFFERPARAMETERIV _glGetNamedFramebufferParameteriv;

typedef void (APIENTRY * PFN_GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV _glGetNamedFramebufferAttachmentParameteriv;

typedef void (APIENTRY * PFN_GLCREATERENDERBUFFERS)(GLsizei n, GLuint * renderbuffers);
extern PFN_GLCREATERENDERBUFFERS _glCreateRenderbuffers;

typedef void (APIENTRY * PFN_GLNAMEDRENDERBUFFERSTORAGE)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLNAMEDRENDERBUFFERSTORAGE _glNamedRenderbufferStorage;

typedef void (APIENTRY * PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLE)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLE _glNamedRenderbufferStorageMultisample;

typedef void (APIENTRY * PFN_GLGETNAMEDRENDERBUFFERPARAMETERIV)(GLuint renderbuffer, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDRENDERBUFFERPARAMETERIV _glGetNamedRenderbufferParameteriv;

typedef void (APIENTRY * PFN_GLCREATETEXTURES)(GLenum target, GLsizei n, GLuint * textures);
extern PFN_GLCREATETEXTURES _glCreateTextures;

typedef void (APIENTRY * PFN_GLTEXTUREBUFFER)(GLuint texture, GLenum internalformat, GLuint buffer);
extern PFN_GLTEXTUREBUFFER _glTextureBuffer;

typedef void (APIENTRY * PFN_GLTEXTUREBUFFERRANGE)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizei size);
extern PFN_GLTEXTUREBUFFERRANGE _glTextureBufferRange;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
extern PFN_GLTEXTURESTORAGE1D _glTextureStorage1D;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLTEXTURESTORAGE2D _glTextureStorage2D;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLTEXTURESTORAGE3D _glTextureStorage3D;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE2DMULTISAMPLE)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFN_GLTEXTURESTORAGE2DMULTISAMPLE _glTextureStorage2DMultisample;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE3DMULTISAMPLE)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFN_GLTEXTURESTORAGE3DMULTISAMPLE _glTextureStorage3DMultisample;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
extern PFN_GLTEXTURESUBIMAGE1D _glTextureSubImage1D;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
extern PFN_GLTEXTURESUBIMAGE2D _glTextureSubImage2D;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
extern PFN_GLTEXTURESUBIMAGE3D _glTextureSubImage3D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE1D _glCompressedTextureSubImage1D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE2D _glCompressedTextureSubImage2D;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE3D _glCompressedTextureSubImage3D;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYTEXTURESUBIMAGE1D _glCopyTextureSubImage1D;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXTURESUBIMAGE2D _glCopyTextureSubImage2D;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXTURESUBIMAGE3D _glCopyTextureSubImage3D;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERF)(GLuint texture, GLenum pname, GLfloat param);
extern PFN_GLTEXTUREPARAMETERF _glTextureParameterf;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERFV)(GLuint texture, GLenum pname, const GLfloat * param);
extern PFN_GLTEXTUREPARAMETERFV _glTextureParameterfv;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERI)(GLuint texture, GLenum pname, GLint param);
extern PFN_GLTEXTUREPARAMETERI _glTextureParameteri;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIIV)(GLuint texture, GLenum pname, const GLint * params);
extern PFN_GLTEXTUREPARAMETERIIV _glTextureParameterIiv;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIUIV)(GLuint texture, GLenum pname, const GLuint * params);
extern PFN_GLTEXTUREPARAMETERIUIV _glTextureParameterIuiv;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIV)(GLuint texture, GLenum pname, const GLint * param);
extern PFN_GLTEXTUREPARAMETERIV _glTextureParameteriv;

typedef void (APIENTRY * PFN_GLGENERATETEXTUREMIPMAP)(GLuint texture);
extern PFN_GLGENERATETEXTUREMIPMAP _glGenerateTextureMipmap;

typedef void (APIENTRY * PFN_GLBINDTEXTUREUNIT)(GLuint unit, GLuint texture);
extern PFN_GLBINDTEXTUREUNIT _glBindTextureUnit;

typedef void (APIENTRY * PFN_GLGETTEXTUREIMAGE)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels);
extern PFN_GLGETTEXTUREIMAGE _glGetTextureImage;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDTEXTUREIMAGE)(GLuint texture, GLint level, GLsizei bufSize, GLvoid * pixels);
extern PFN_GLGETCOMPRESSEDTEXTUREIMAGE _glGetCompressedTextureImage;

typedef void (APIENTRY * PFN_GLGETTEXTURELEVELPARAMETERFV)(GLuint texture, GLint level, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXTURELEVELPARAMETERFV _glGetTextureLevelParameterfv;

typedef void (APIENTRY * PFN_GLGETTEXTURELEVELPARAMETERIV)(GLuint texture, GLint level, GLenum pname, GLint * params);
extern PFN_GLGETTEXTURELEVELPARAMETERIV _glGetTextureLevelParameteriv;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERFV)(GLuint texture, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXTUREPARAMETERFV _glGetTextureParameterfv;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIIV)(GLuint texture, GLenum pname, GLint * params);
extern PFN_GLGETTEXTUREPARAMETERIIV _glGetTextureParameterIiv;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIUIV)(GLuint texture, GLenum pname, GLuint * params);
extern PFN_GLGETTEXTUREPARAMETERIUIV _glGetTextureParameterIuiv;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIV)(GLuint texture, GLenum pname, GLint * params);
extern PFN_GLGETTEXTUREPARAMETERIV _glGetTextureParameteriv;

typedef void (APIENTRY * PFN_GLCREATEVERTEXARRAYS)(GLsizei n, GLuint * arrays);
extern PFN_GLCREATEVERTEXARRAYS _glCreateVertexArrays;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXARRAYATTRIB)(GLuint vaobj, GLuint index);
extern PFN_GLDISABLEVERTEXARRAYATTRIB _glDisableVertexArrayAttrib;

typedef void (APIENTRY * PFN_GLENABLEVERTEXARRAYATTRIB)(GLuint vaobj, GLuint index);
extern PFN_GLENABLEVERTEXARRAYATTRIB _glEnableVertexArrayAttrib;

typedef void (APIENTRY * PFN_GLVERTEXARRAYELEMENTBUFFER)(GLuint vaobj, GLuint buffer);
extern PFN_GLVERTEXARRAYELEMENTBUFFER _glVertexArrayElementBuffer;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXBUFFER)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern PFN_GLVERTEXARRAYVERTEXBUFFER _glVertexArrayVertexBuffer;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXBUFFERS)(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
extern PFN_GLVERTEXARRAYVERTEXBUFFERS _glVertexArrayVertexBuffers;

typedef void (APIENTRY * PFN_GLVERTEXARRAYATTRIBBINDING)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
extern PFN_GLVERTEXARRAYATTRIBBINDING _glVertexArrayAttribBinding;

typedef void (APIENTRY * PFN_GLVERTEXARRAYATTRIBFORMAT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYATTRIBFORMAT _glVertexArrayAttribFormat;

typedef void (APIENTRY * PFN_GLVERTEXARRAYATTRIBIFORMAT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYATTRIBIFORMAT _glVertexArrayAttribIFormat;

typedef void (APIENTRY * PFN_GLVERTEXARRAYATTRIBLFORMAT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYATTRIBLFORMAT _glVertexArrayAttribLFormat;

typedef void (APIENTRY * PFN_GLVERTEXARRAYBINDINGDIVISOR)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
extern PFN_GLVERTEXARRAYBINDINGDIVISOR _glVertexArrayBindingDivisor;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYIV)(GLuint vaobj, GLenum pname, GLint * param);
extern PFN_GLGETVERTEXARRAYIV _glGetVertexArrayiv;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYINDEXEDIV)(GLuint vaobj, GLuint index, GLenum pname, GLint * param);
extern PFN_GLGETVERTEXARRAYINDEXEDIV _glGetVertexArrayIndexediv;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYINDEXED64IV)(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param);
extern PFN_GLGETVERTEXARRAYINDEXED64IV _glGetVertexArrayIndexed64iv;

typedef void (APIENTRY * PFN_GLCREATESAMPLERS)(GLsizei n, GLuint * samplers);
extern PFN_GLCREATESAMPLERS _glCreateSamplers;

typedef void (APIENTRY * PFN_GLCREATEPROGRAMPIPELINES)(GLsizei n, GLuint * pipelines);
extern PFN_GLCREATEPROGRAMPIPELINES _glCreateProgramPipelines;

typedef void (APIENTRY * PFN_GLCREATEQUERIES)(GLenum target, GLsizei n, GLuint * ids);
extern PFN_GLCREATEQUERIES _glCreateQueries;

typedef void (APIENTRY * PFN_GLDRAWBUFFERSARB)(GLsizei n, const GLenum * bufs);
extern PFN_GLDRAWBUFFERSARB _glDrawBuffersARB;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONIARB)(GLuint buf, GLenum mode);
extern PFN_GLBLENDEQUATIONIARB _glBlendEquationiARB;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONSEPARATEIARB)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATEIARB _glBlendEquationSeparateiARB;

typedef void (APIENTRY * PFN_GLBLENDFUNCIARB)(GLuint buf, GLenum src, GLenum dst);
extern PFN_GLBLENDFUNCIARB _glBlendFunciARB;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATEIARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern PFN_GLBLENDFUNCSEPARATEIARB _glBlendFuncSeparateiARB;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSBASEVERTEX)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
extern PFN_GLDRAWELEMENTSBASEVERTEX _glDrawElementsBaseVertex;

typedef void (APIENTRY * PFN_GLDRAWRANGEELEMENTSBASEVERTEX)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
extern PFN_GLDRAWRANGEELEMENTSBASEVERTEX _glDrawRangeElementsBaseVertex;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDBASEVERTEX)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex);
extern PFN_GLDRAWELEMENTSINSTANCEDBASEVERTEX _glDrawElementsInstancedBaseVertex;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSBASEVERTEX)(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei drawcount, const GLint * basevertex);
extern PFN_GLMULTIDRAWELEMENTSBASEVERTEX _glMultiDrawElementsBaseVertex;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINDIRECT)(GLenum mode, const GLvoid * indirect);
extern PFN_GLDRAWARRAYSINDIRECT _glDrawArraysIndirect;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINDIRECT)(GLenum mode, GLenum type, const GLvoid * indirect);
extern PFN_GLDRAWELEMENTSINDIRECT _glDrawElementsIndirect;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINSTANCEDARB)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern PFN_GLDRAWARRAYSINSTANCEDARB _glDrawArraysInstancedARB;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDARB)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
extern PFN_GLDRAWELEMENTSINSTANCEDARB _glDrawElementsInstancedARB;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERPARAMETERI)(GLenum target, GLenum pname, GLint param);
extern PFN_GLFRAMEBUFFERPARAMETERI _glFramebufferParameteri;

typedef void (APIENTRY * PFN_GLGETFRAMEBUFFERPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETFRAMEBUFFERPARAMETERIV _glGetFramebufferParameteriv;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERPARAMETERIEXT)(GLuint framebuffer, GLenum pname, GLint param);
extern PFN_GLNAMEDFRAMEBUFFERPARAMETERIEXT _glNamedFramebufferParameteriEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDFRAMEBUFFERPARAMETERIVEXT)(GLuint framebuffer, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDFRAMEBUFFERPARAMETERIVEXT _glGetNamedFramebufferParameterivEXT;

typedef GLboolean (APIENTRY * PFN_GLISRENDERBUFFER)(GLuint renderbuffer);
extern PFN_GLISRENDERBUFFER _glIsRenderbuffer;

typedef void (APIENTRY * PFN_GLBINDRENDERBUFFER)(GLenum target, GLuint renderbuffer);
extern PFN_GLBINDRENDERBUFFER _glBindRenderbuffer;

typedef void (APIENTRY * PFN_GLDELETERENDERBUFFERS)(GLsizei n, const GLuint * renderbuffers);
extern PFN_GLDELETERENDERBUFFERS _glDeleteRenderbuffers;

typedef void (APIENTRY * PFN_GLGENRENDERBUFFERS)(GLsizei n, GLuint * renderbuffers);
extern PFN_GLGENRENDERBUFFERS _glGenRenderbuffers;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGE)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGE _glRenderbufferStorage;

typedef void (APIENTRY * PFN_GLGETRENDERBUFFERPARAMETERIV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETRENDERBUFFERPARAMETERIV _glGetRenderbufferParameteriv;

typedef GLboolean (APIENTRY * PFN_GLISFRAMEBUFFER)(GLuint framebuffer);
extern PFN_GLISFRAMEBUFFER _glIsFramebuffer;

typedef void (APIENTRY * PFN_GLBINDFRAMEBUFFER)(GLenum target, GLuint framebuffer);
extern PFN_GLBINDFRAMEBUFFER _glBindFramebuffer;

typedef void (APIENTRY * PFN_GLDELETEFRAMEBUFFERS)(GLsizei n, const GLuint * framebuffers);
extern PFN_GLDELETEFRAMEBUFFERS _glDeleteFramebuffers;

typedef void (APIENTRY * PFN_GLGENFRAMEBUFFERS)(GLsizei n, GLuint * framebuffers);
extern PFN_GLGENFRAMEBUFFERS _glGenFramebuffers;

typedef GLenum (APIENTRY * PFN_GLCHECKFRAMEBUFFERSTATUS)(GLenum target);
extern PFN_GLCHECKFRAMEBUFFERSTATUS _glCheckFramebufferStatus;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE1D _glFramebufferTexture1D;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE2D _glFramebufferTexture2D;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern PFN_GLFRAMEBUFFERTEXTURE3D _glFramebufferTexture3D;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERRENDERBUFFER)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFN_GLFRAMEBUFFERRENDERBUFFER _glFramebufferRenderbuffer;

typedef void (APIENTRY * PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIV)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
extern PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIV _glGetFramebufferAttachmentParameteriv;

typedef void (APIENTRY * PFN_GLGENERATEMIPMAP)(GLenum target);
extern PFN_GLGENERATEMIPMAP _glGenerateMipmap;

typedef void (APIENTRY * PFN_GLBLITFRAMEBUFFER)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_GLBLITFRAMEBUFFER _glBlitFramebuffer;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLE _glRenderbufferStorageMultisample;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURELAYER)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFN_GLFRAMEBUFFERTEXTURELAYER _glFramebufferTextureLayer;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETERIARB)(GLuint program, GLenum pname, GLint value);
extern PFN_GLPROGRAMPARAMETERIARB _glProgramParameteriARB;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTUREARB)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTUREARB _glFramebufferTextureARB;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURELAYERARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFN_GLFRAMEBUFFERTEXTURELAYERARB _glFramebufferTextureLayerARB;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTUREFACEARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
extern PFN_GLFRAMEBUFFERTEXTUREFACEARB _glFramebufferTextureFaceARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMBINARY)(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, GLvoid * binary);
extern PFN_GLGETPROGRAMBINARY _glGetProgramBinary;

typedef void (APIENTRY * PFN_GLPROGRAMBINARY)(GLuint program, GLenum binaryFormat, const GLvoid * binary, GLsizei length);
extern PFN_GLPROGRAMBINARY _glProgramBinary;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETERI)(GLuint program, GLenum pname, GLint value);
extern PFN_GLPROGRAMPARAMETERI _glProgramParameteri;

typedef void (APIENTRY * PFN_GLGETTEXTURESUBIMAGE)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels);
extern PFN_GLGETTEXTURESUBIMAGE _glGetTextureSubImage;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDTEXTURESUBIMAGE)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLvoid * pixels);
extern PFN_GLGETCOMPRESSEDTEXTURESUBIMAGE _glGetCompressedTextureSubImage;

typedef void (APIENTRY * PFN_GLUNIFORM1D)(GLint location, GLdouble x);
extern PFN_GLUNIFORM1D _glUniform1d;

typedef void (APIENTRY * PFN_GLUNIFORM2D)(GLint location, GLdouble x, GLdouble y);
extern PFN_GLUNIFORM2D _glUniform2d;

typedef void (APIENTRY * PFN_GLUNIFORM3D)(GLint location, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLUNIFORM3D _glUniform3d;

typedef void (APIENTRY * PFN_GLUNIFORM4D)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLUNIFORM4D _glUniform4d;

typedef void (APIENTRY * PFN_GLUNIFORM1DV)(GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLUNIFORM1DV _glUniform1dv;

typedef void (APIENTRY * PFN_GLUNIFORM2DV)(GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLUNIFORM2DV _glUniform2dv;

typedef void (APIENTRY * PFN_GLUNIFORM3DV)(GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLUNIFORM3DV _glUniform3dv;

typedef void (APIENTRY * PFN_GLUNIFORM4DV)(GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLUNIFORM4DV _glUniform4dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX2DV _glUniformMatrix2dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX3DV _glUniformMatrix3dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX4DV _glUniformMatrix4dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2X3DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX2X3DV _glUniformMatrix2x3dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2X4DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX2X4DV _glUniformMatrix2x4dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3X2DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX3X2DV _glUniformMatrix3x2dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3X4DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX3X4DV _glUniformMatrix3x4dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4X2DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX4X2DV _glUniformMatrix4x2dv;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4X3DV)(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLUNIFORMMATRIX4X3DV _glUniformMatrix4x3dv;

typedef void (APIENTRY * PFN_GLGETUNIFORMDV)(GLuint program, GLint location, GLdouble * params);
extern PFN_GLGETUNIFORMDV _glGetUniformdv;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYSINDIRECTCOUNTARB)(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
extern PFN_GLMULTIDRAWARRAYSINDIRECTCOUNTARB _glMultiDrawArraysIndirectCountARB;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSINDIRECTCOUNTARB)(GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
extern PFN_GLMULTIDRAWELEMENTSINDIRECTCOUNTARB _glMultiDrawElementsIndirectCountARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBDIVISORARB)(GLuint index, GLuint divisor);
extern PFN_GLVERTEXATTRIBDIVISORARB _glVertexAttribDivisorARB;

typedef void (APIENTRY * PFN_GLGETINTERNALFORMATIV)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params);
extern PFN_GLGETINTERNALFORMATIV _glGetInternalformativ;

typedef void (APIENTRY * PFN_GLGETINTERNALFORMATI64V)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 * params);
extern PFN_GLGETINTERNALFORMATI64V _glGetInternalformati64v;

typedef void (APIENTRY * PFN_GLINVALIDATETEXSUBIMAGE)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLINVALIDATETEXSUBIMAGE _glInvalidateTexSubImage;

typedef void (APIENTRY * PFN_GLINVALIDATETEXIMAGE)(GLuint texture, GLint level);
extern PFN_GLINVALIDATETEXIMAGE _glInvalidateTexImage;

typedef void (APIENTRY * PFN_GLINVALIDATEBUFFERSUBDATA)(GLuint buffer, GLintptr offset, GLsizeiptr length);
extern PFN_GLINVALIDATEBUFFERSUBDATA _glInvalidateBufferSubData;

typedef void (APIENTRY * PFN_GLINVALIDATEBUFFERDATA)(GLuint buffer);
extern PFN_GLINVALIDATEBUFFERDATA _glInvalidateBufferData;

typedef void (APIENTRY * PFN_GLINVALIDATEFRAMEBUFFER)(GLenum target, GLsizei numAttachments, const GLenum * attachments);
extern PFN_GLINVALIDATEFRAMEBUFFER _glInvalidateFramebuffer;

typedef void (APIENTRY * PFN_GLINVALIDATESUBFRAMEBUFFER)(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLINVALIDATESUBFRAMEBUFFER _glInvalidateSubFramebuffer;

typedef GLvoid * (APIENTRY * PFN_GLMAPBUFFERRANGE)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern PFN_GLMAPBUFFERRANGE _glMapBufferRange;

typedef void (APIENTRY * PFN_GLFLUSHMAPPEDBUFFERRANGE)(GLenum target, GLintptr offset, GLsizeiptr length);
extern PFN_GLFLUSHMAPPEDBUFFERRANGE _glFlushMappedBufferRange;

typedef void (APIENTRY * PFN_GLCURRENTPALETTEMATRIXARB)(GLint index);
extern PFN_GLCURRENTPALETTEMATRIXARB _glCurrentPaletteMatrixARB;

typedef void (APIENTRY * PFN_GLMATRIXINDEXUBVARB)(GLint size, const GLubyte * indices);
extern PFN_GLMATRIXINDEXUBVARB _glMatrixIndexubvARB;

typedef void (APIENTRY * PFN_GLMATRIXINDEXUSVARB)(GLint size, const GLushort * indices);
extern PFN_GLMATRIXINDEXUSVARB _glMatrixIndexusvARB;

typedef void (APIENTRY * PFN_GLMATRIXINDEXUIVARB)(GLint size, const GLuint * indices);
extern PFN_GLMATRIXINDEXUIVARB _glMatrixIndexuivARB;

typedef void (APIENTRY * PFN_GLMATRIXINDEXPOINTERARB)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLMATRIXINDEXPOINTERARB _glMatrixIndexPointerARB;

typedef void (APIENTRY * PFN_GLBINDBUFFERSBASE)(GLenum target, GLuint first, GLsizei count, const GLuint * buffers);
extern PFN_GLBINDBUFFERSBASE _glBindBuffersBase;

typedef void (APIENTRY * PFN_GLBINDBUFFERSRANGE)(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes);
extern PFN_GLBINDBUFFERSRANGE _glBindBuffersRange;

typedef void (APIENTRY * PFN_GLBINDTEXTURES)(GLuint first, GLsizei count, const GLuint * textures);
extern PFN_GLBINDTEXTURES _glBindTextures;

typedef void (APIENTRY * PFN_GLBINDSAMPLERS)(GLuint first, GLsizei count, const GLuint * samplers);
extern PFN_GLBINDSAMPLERS _glBindSamplers;

typedef void (APIENTRY * PFN_GLBINDIMAGETEXTURES)(GLuint first, GLsizei count, const GLuint * textures);
extern PFN_GLBINDIMAGETEXTURES _glBindImageTextures;

typedef void (APIENTRY * PFN_GLBINDVERTEXBUFFERS)(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
extern PFN_GLBINDVERTEXBUFFERS _glBindVertexBuffers;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYSINDIRECT)(GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride);
extern PFN_GLMULTIDRAWARRAYSINDIRECT _glMultiDrawArraysIndirect;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSINDIRECT)(GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride);
extern PFN_GLMULTIDRAWELEMENTSINDIRECT _glMultiDrawElementsIndirect;

typedef void (APIENTRY * PFN_GLSAMPLECOVERAGEARB)(GLfloat value, GLboolean invert);
extern PFN_GLSAMPLECOVERAGEARB _glSampleCoverageARB;

typedef void (APIENTRY * PFN_GLACTIVETEXTUREARB)(GLenum texture);
extern PFN_GLACTIVETEXTUREARB _glActiveTextureARB;

typedef void (APIENTRY * PFN_GLCLIENTACTIVETEXTUREARB)(GLenum texture);
extern PFN_GLCLIENTACTIVETEXTUREARB _glClientActiveTextureARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1DARB)(GLenum target, GLdouble s);
extern PFN_GLMULTITEXCOORD1DARB _glMultiTexCoord1dARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1DVARB)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD1DVARB _glMultiTexCoord1dvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1FARB)(GLenum target, GLfloat s);
extern PFN_GLMULTITEXCOORD1FARB _glMultiTexCoord1fARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1FVARB)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD1FVARB _glMultiTexCoord1fvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1IARB)(GLenum target, GLint s);
extern PFN_GLMULTITEXCOORD1IARB _glMultiTexCoord1iARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1IVARB)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD1IVARB _glMultiTexCoord1ivARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1SARB)(GLenum target, GLshort s);
extern PFN_GLMULTITEXCOORD1SARB _glMultiTexCoord1sARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1SVARB)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD1SVARB _glMultiTexCoord1svARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2DARB)(GLenum target, GLdouble s, GLdouble t);
extern PFN_GLMULTITEXCOORD2DARB _glMultiTexCoord2dARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2DVARB)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD2DVARB _glMultiTexCoord2dvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2FARB)(GLenum target, GLfloat s, GLfloat t);
extern PFN_GLMULTITEXCOORD2FARB _glMultiTexCoord2fARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2FVARB)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD2FVARB _glMultiTexCoord2fvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2IARB)(GLenum target, GLint s, GLint t);
extern PFN_GLMULTITEXCOORD2IARB _glMultiTexCoord2iARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2IVARB)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD2IVARB _glMultiTexCoord2ivARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2SARB)(GLenum target, GLshort s, GLshort t);
extern PFN_GLMULTITEXCOORD2SARB _glMultiTexCoord2sARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2SVARB)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD2SVARB _glMultiTexCoord2svARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3DARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern PFN_GLMULTITEXCOORD3DARB _glMultiTexCoord3dARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3DVARB)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD3DVARB _glMultiTexCoord3dvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3FARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern PFN_GLMULTITEXCOORD3FARB _glMultiTexCoord3fARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3FVARB)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD3FVARB _glMultiTexCoord3fvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3IARB)(GLenum target, GLint s, GLint t, GLint r);
extern PFN_GLMULTITEXCOORD3IARB _glMultiTexCoord3iARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3IVARB)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD3IVARB _glMultiTexCoord3ivARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3SARB)(GLenum target, GLshort s, GLshort t, GLshort r);
extern PFN_GLMULTITEXCOORD3SARB _glMultiTexCoord3sARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3SVARB)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD3SVARB _glMultiTexCoord3svARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4DARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern PFN_GLMULTITEXCOORD4DARB _glMultiTexCoord4dARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4DVARB)(GLenum target, const GLdouble * v);
extern PFN_GLMULTITEXCOORD4DVARB _glMultiTexCoord4dvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4FARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern PFN_GLMULTITEXCOORD4FARB _glMultiTexCoord4fARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4FVARB)(GLenum target, const GLfloat * v);
extern PFN_GLMULTITEXCOORD4FVARB _glMultiTexCoord4fvARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4IARB)(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern PFN_GLMULTITEXCOORD4IARB _glMultiTexCoord4iARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4IVARB)(GLenum target, const GLint * v);
extern PFN_GLMULTITEXCOORD4IVARB _glMultiTexCoord4ivARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4SARB)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern PFN_GLMULTITEXCOORD4SARB _glMultiTexCoord4sARB;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4SVARB)(GLenum target, const GLshort * v);
extern PFN_GLMULTITEXCOORD4SVARB _glMultiTexCoord4svARB;

typedef void (APIENTRY * PFN_GLGENQUERIESARB)(GLsizei n, GLuint * ids);
extern PFN_GLGENQUERIESARB _glGenQueriesARB;

typedef void (APIENTRY * PFN_GLDELETEQUERIESARB)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETEQUERIESARB _glDeleteQueriesARB;

typedef GLboolean (APIENTRY * PFN_GLISQUERYARB)(GLuint id);
extern PFN_GLISQUERYARB _glIsQueryARB;

typedef void (APIENTRY * PFN_GLBEGINQUERYARB)(GLenum target, GLuint id);
extern PFN_GLBEGINQUERYARB _glBeginQueryARB;

typedef void (APIENTRY * PFN_GLENDQUERYARB)(GLenum target);
extern PFN_GLENDQUERYARB _glEndQueryARB;

typedef void (APIENTRY * PFN_GLGETQUERYIVARB)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETQUERYIVARB _glGetQueryivARB;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTIVARB)(GLuint id, GLenum pname, GLint * params);
extern PFN_GLGETQUERYOBJECTIVARB _glGetQueryObjectivARB;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTUIVARB)(GLuint id, GLenum pname, GLuint * params);
extern PFN_GLGETQUERYOBJECTUIVARB _glGetQueryObjectuivARB;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFARB)(GLenum pname, GLfloat param);
extern PFN_GLPOINTPARAMETERFARB _glPointParameterfARB;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFVARB)(GLenum pname, const GLfloat * params);
extern PFN_GLPOINTPARAMETERFVARB _glPointParameterfvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMINTERFACEIV)(GLuint program, GLenum programInterface, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMINTERFACEIV _glGetProgramInterfaceiv;

typedef GLuint (APIENTRY * PFN_GLGETPROGRAMRESOURCEINDEX)(GLuint program, GLenum programInterface, const GLchar * name);
extern PFN_GLGETPROGRAMRESOURCEINDEX _glGetProgramResourceIndex;

typedef void (APIENTRY * PFN_GLGETPROGRAMRESOURCENAME)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name);
extern PFN_GLGETPROGRAMRESOURCENAME _glGetProgramResourceName;

typedef void (APIENTRY * PFN_GLGETPROGRAMRESOURCEIV)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei bufSize, GLsizei * length, GLint * params);
extern PFN_GLGETPROGRAMRESOURCEIV _glGetProgramResourceiv;

typedef GLint (APIENTRY * PFN_GLGETPROGRAMRESOURCELOCATION)(GLuint program, GLenum programInterface, const GLchar * name);
extern PFN_GLGETPROGRAMRESOURCELOCATION _glGetProgramResourceLocation;

typedef GLint (APIENTRY * PFN_GLGETPROGRAMRESOURCELOCATIONINDEX)(GLuint program, GLenum programInterface, const GLchar * name);
extern PFN_GLGETPROGRAMRESOURCELOCATIONINDEX _glGetProgramResourceLocationIndex;

typedef void (APIENTRY * PFN_GLPROVOKINGVERTEX)(GLenum mode);
extern PFN_GLPROVOKINGVERTEX _glProvokingVertex;

typedef GLenum (APIENTRY * PFN_GLGETGRAPHICSRESETSTATUSARB)(void);
extern PFN_GLGETGRAPHICSRESETSTATUSARB _glGetGraphicsResetStatusARB;

typedef void (APIENTRY * PFN_GLGETNMAPDVARB)(GLenum target, GLenum query, GLsizei bufSize, GLdouble * v);
extern PFN_GLGETNMAPDVARB _glGetnMapdvARB;

typedef void (APIENTRY * PFN_GLGETNMAPFVARB)(GLenum target, GLenum query, GLsizei bufSize, GLfloat * v);
extern PFN_GLGETNMAPFVARB _glGetnMapfvARB;

typedef void (APIENTRY * PFN_GLGETNMAPIVARB)(GLenum target, GLenum query, GLsizei bufSize, GLint * v);
extern PFN_GLGETNMAPIVARB _glGetnMapivARB;

typedef void (APIENTRY * PFN_GLGETNPIXELMAPFVARB)(GLenum map, GLsizei bufSize, GLfloat * values);
extern PFN_GLGETNPIXELMAPFVARB _glGetnPixelMapfvARB;

typedef void (APIENTRY * PFN_GLGETNPIXELMAPUIVARB)(GLenum map, GLsizei bufSize, GLuint * values);
extern PFN_GLGETNPIXELMAPUIVARB _glGetnPixelMapuivARB;

typedef void (APIENTRY * PFN_GLGETNPIXELMAPUSVARB)(GLenum map, GLsizei bufSize, GLushort * values);
extern PFN_GLGETNPIXELMAPUSVARB _glGetnPixelMapusvARB;

typedef void (APIENTRY * PFN_GLGETNPOLYGONSTIPPLEARB)(GLsizei bufSize, GLubyte * pattern);
extern PFN_GLGETNPOLYGONSTIPPLEARB _glGetnPolygonStippleARB;

typedef void (APIENTRY * PFN_GLGETNCOLORTABLEARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * table);
extern PFN_GLGETNCOLORTABLEARB _glGetnColorTableARB;

typedef void (APIENTRY * PFN_GLGETNCONVOLUTIONFILTERARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * image);
extern PFN_GLGETNCONVOLUTIONFILTERARB _glGetnConvolutionFilterARB;

typedef void (APIENTRY * PFN_GLGETNSEPARABLEFILTERARB)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid * row, GLsizei columnBufSize, GLvoid * column, GLvoid * span);
extern PFN_GLGETNSEPARABLEFILTERARB _glGetnSeparableFilterARB;

typedef void (APIENTRY * PFN_GLGETNHISTOGRAMARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values);
extern PFN_GLGETNHISTOGRAMARB _glGetnHistogramARB;

typedef void (APIENTRY * PFN_GLGETNMINMAXARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values);
extern PFN_GLGETNMINMAXARB _glGetnMinmaxARB;

typedef void (APIENTRY * PFN_GLGETNTEXIMAGEARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * img);
extern PFN_GLGETNTEXIMAGEARB _glGetnTexImageARB;

typedef void (APIENTRY * PFN_GLREADNPIXELSARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid * data);
extern PFN_GLREADNPIXELSARB _glReadnPixelsARB;

typedef void (APIENTRY * PFN_GLGETNCOMPRESSEDTEXIMAGEARB)(GLenum target, GLint lod, GLsizei bufSize, GLvoid * img);
extern PFN_GLGETNCOMPRESSEDTEXIMAGEARB _glGetnCompressedTexImageARB;

typedef void (APIENTRY * PFN_GLGETNUNIFORMFVARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
extern PFN_GLGETNUNIFORMFVARB _glGetnUniformfvARB;

typedef void (APIENTRY * PFN_GLGETNUNIFORMIVARB)(GLuint program, GLint location, GLsizei bufSize, GLint * params);
extern PFN_GLGETNUNIFORMIVARB _glGetnUniformivARB;

typedef void (APIENTRY * PFN_GLGETNUNIFORMUIVARB)(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
extern PFN_GLGETNUNIFORMUIVARB _glGetnUniformuivARB;

typedef void (APIENTRY * PFN_GLGETNUNIFORMDVARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble * params);
extern PFN_GLGETNUNIFORMDVARB _glGetnUniformdvARB;

typedef void (APIENTRY * PFN_GLMINSAMPLESHADINGARB)(GLfloat value);
extern PFN_GLMINSAMPLESHADINGARB _glMinSampleShadingARB;

typedef void (APIENTRY * PFN_GLGENSAMPLERS)(GLsizei count, GLuint * samplers);
extern PFN_GLGENSAMPLERS _glGenSamplers;

typedef void (APIENTRY * PFN_GLDELETESAMPLERS)(GLsizei count, const GLuint * samplers);
extern PFN_GLDELETESAMPLERS _glDeleteSamplers;

typedef GLboolean (APIENTRY * PFN_GLISSAMPLER)(GLuint sampler);
extern PFN_GLISSAMPLER _glIsSampler;

typedef void (APIENTRY * PFN_GLBINDSAMPLER)(GLuint unit, GLuint sampler);
extern PFN_GLBINDSAMPLER _glBindSampler;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERI)(GLuint sampler, GLenum pname, GLint param);
extern PFN_GLSAMPLERPARAMETERI _glSamplerParameteri;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERIV)(GLuint sampler, GLenum pname, const GLint * param);
extern PFN_GLSAMPLERPARAMETERIV _glSamplerParameteriv;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERF)(GLuint sampler, GLenum pname, GLfloat param);
extern PFN_GLSAMPLERPARAMETERF _glSamplerParameterf;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERFV)(GLuint sampler, GLenum pname, const GLfloat * param);
extern PFN_GLSAMPLERPARAMETERFV _glSamplerParameterfv;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERIIV)(GLuint sampler, GLenum pname, const GLint * param);
extern PFN_GLSAMPLERPARAMETERIIV _glSamplerParameterIiv;

typedef void (APIENTRY * PFN_GLSAMPLERPARAMETERIUIV)(GLuint sampler, GLenum pname, const GLuint * param);
extern PFN_GLSAMPLERPARAMETERIUIV _glSamplerParameterIuiv;

typedef void (APIENTRY * PFN_GLGETSAMPLERPARAMETERIV)(GLuint sampler, GLenum pname, GLint * params);
extern PFN_GLGETSAMPLERPARAMETERIV _glGetSamplerParameteriv;

typedef void (APIENTRY * PFN_GLGETSAMPLERPARAMETERIIV)(GLuint sampler, GLenum pname, GLint * params);
extern PFN_GLGETSAMPLERPARAMETERIIV _glGetSamplerParameterIiv;

typedef void (APIENTRY * PFN_GLGETSAMPLERPARAMETERFV)(GLuint sampler, GLenum pname, GLfloat * params);
extern PFN_GLGETSAMPLERPARAMETERFV _glGetSamplerParameterfv;

typedef void (APIENTRY * PFN_GLGETSAMPLERPARAMETERIUIV)(GLuint sampler, GLenum pname, GLuint * params);
extern PFN_GLGETSAMPLERPARAMETERIUIV _glGetSamplerParameterIuiv;

typedef void (APIENTRY * PFN_GLUSEPROGRAMSTAGES)(GLuint pipeline, GLbitfield stages, GLuint program);
extern PFN_GLUSEPROGRAMSTAGES _glUseProgramStages;

typedef void (APIENTRY * PFN_GLACTIVESHADERPROGRAM)(GLuint pipeline, GLuint program);
extern PFN_GLACTIVESHADERPROGRAM _glActiveShaderProgram;

typedef GLuint (APIENTRY * PFN_GLCREATESHADERPROGRAMV)(GLenum type, GLsizei count, const GLchar * const * strings);
extern PFN_GLCREATESHADERPROGRAMV _glCreateShaderProgramv;

typedef void (APIENTRY * PFN_GLBINDPROGRAMPIPELINE)(GLuint pipeline);
extern PFN_GLBINDPROGRAMPIPELINE _glBindProgramPipeline;

typedef void (APIENTRY * PFN_GLDELETEPROGRAMPIPELINES)(GLsizei n, const GLuint * pipelines);
extern PFN_GLDELETEPROGRAMPIPELINES _glDeleteProgramPipelines;

typedef void (APIENTRY * PFN_GLGENPROGRAMPIPELINES)(GLsizei n, GLuint * pipelines);
extern PFN_GLGENPROGRAMPIPELINES _glGenProgramPipelines;

typedef GLboolean (APIENTRY * PFN_GLISPROGRAMPIPELINE)(GLuint pipeline);
extern PFN_GLISPROGRAMPIPELINE _glIsProgramPipeline;

typedef void (APIENTRY * PFN_GLGETPROGRAMPIPELINEIV)(GLuint pipeline, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMPIPELINEIV _glGetProgramPipelineiv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1I)(GLuint program, GLint location, GLint v0);
extern PFN_GLPROGRAMUNIFORM1I _glProgramUniform1i;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1IV)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM1IV _glProgramUniform1iv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1F)(GLuint program, GLint location, GLfloat v0);
extern PFN_GLPROGRAMUNIFORM1F _glProgramUniform1f;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1FV)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM1FV _glProgramUniform1fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1D)(GLuint program, GLint location, GLdouble v0);
extern PFN_GLPROGRAMUNIFORM1D _glProgramUniform1d;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1DV)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM1DV _glProgramUniform1dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UI)(GLuint program, GLint location, GLuint v0);
extern PFN_GLPROGRAMUNIFORM1UI _glProgramUniform1ui;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UIV)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM1UIV _glProgramUniform1uiv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2I)(GLuint program, GLint location, GLint v0, GLint v1);
extern PFN_GLPROGRAMUNIFORM2I _glProgramUniform2i;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2IV)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM2IV _glProgramUniform2iv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2F)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
extern PFN_GLPROGRAMUNIFORM2F _glProgramUniform2f;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2FV)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM2FV _glProgramUniform2fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2D)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
extern PFN_GLPROGRAMUNIFORM2D _glProgramUniform2d;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2DV)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM2DV _glProgramUniform2dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UI)(GLuint program, GLint location, GLuint v0, GLuint v1);
extern PFN_GLPROGRAMUNIFORM2UI _glProgramUniform2ui;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UIV)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM2UIV _glProgramUniform2uiv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3I)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
extern PFN_GLPROGRAMUNIFORM3I _glProgramUniform3i;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3IV)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM3IV _glProgramUniform3iv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3F)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFN_GLPROGRAMUNIFORM3F _glProgramUniform3f;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3FV)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM3FV _glProgramUniform3fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3D)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
extern PFN_GLPROGRAMUNIFORM3D _glProgramUniform3d;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3DV)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM3DV _glProgramUniform3dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UI)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
extern PFN_GLPROGRAMUNIFORM3UI _glProgramUniform3ui;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UIV)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM3UIV _glProgramUniform3uiv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4I)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern PFN_GLPROGRAMUNIFORM4I _glProgramUniform4i;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4IV)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM4IV _glProgramUniform4iv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4F)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFN_GLPROGRAMUNIFORM4F _glProgramUniform4f;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4FV)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM4FV _glProgramUniform4fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4D)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
extern PFN_GLPROGRAMUNIFORM4D _glProgramUniform4d;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4DV)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM4DV _glProgramUniform4dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UI)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern PFN_GLPROGRAMUNIFORM4UI _glProgramUniform4ui;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UIV)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM4UIV _glProgramUniform4uiv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2FV _glProgramUniformMatrix2fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3FV _glProgramUniformMatrix3fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4FV _glProgramUniformMatrix4fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2DV _glProgramUniformMatrix2dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3DV _glProgramUniformMatrix3dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4DV _glProgramUniformMatrix4dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X3FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X3FV _glProgramUniformMatrix2x3fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X2FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X2FV _glProgramUniformMatrix3x2fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X4FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X4FV _glProgramUniformMatrix2x4fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X2FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X2FV _glProgramUniformMatrix4x2fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X4FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X4FV _glProgramUniformMatrix3x4fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X3FV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X3FV _glProgramUniformMatrix4x3fv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X3DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X3DV _glProgramUniformMatrix2x3dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X2DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X2DV _glProgramUniformMatrix3x2dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X4DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X4DV _glProgramUniformMatrix2x4dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X2DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X2DV _glProgramUniformMatrix4x2dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X4DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X4DV _glProgramUniformMatrix3x4dv;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X3DV)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X3DV _glProgramUniformMatrix4x3dv;

typedef void (APIENTRY * PFN_GLVALIDATEPROGRAMPIPELINE)(GLuint pipeline);
extern PFN_GLVALIDATEPROGRAMPIPELINE _glValidateProgramPipeline;

typedef void (APIENTRY * PFN_GLGETPROGRAMPIPELINEINFOLOG)(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
extern PFN_GLGETPROGRAMPIPELINEINFOLOG _glGetProgramPipelineInfoLog;

typedef void (APIENTRY * PFN_GLGETACTIVEATOMICCOUNTERBUFFERIV)(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params);
extern PFN_GLGETACTIVEATOMICCOUNTERBUFFERIV _glGetActiveAtomicCounterBufferiv;

typedef void (APIENTRY * PFN_GLBINDIMAGETEXTURE)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
extern PFN_GLBINDIMAGETEXTURE _glBindImageTexture;

typedef void (APIENTRY * PFN_GLMEMORYBARRIER)(GLbitfield barriers);
extern PFN_GLMEMORYBARRIER _glMemoryBarrier;

typedef void (APIENTRY * PFN_GLDELETEOBJECTARB)(GLhandleARB obj);
extern PFN_GLDELETEOBJECTARB _glDeleteObjectARB;

typedef GLhandleARB (APIENTRY * PFN_GLGETHANDLEARB)(GLenum pname);
extern PFN_GLGETHANDLEARB _glGetHandleARB;

typedef void (APIENTRY * PFN_GLDETACHOBJECTARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
extern PFN_GLDETACHOBJECTARB _glDetachObjectARB;

typedef GLhandleARB (APIENTRY * PFN_GLCREATESHADEROBJECTARB)(GLenum shaderType);
extern PFN_GLCREATESHADEROBJECTARB _glCreateShaderObjectARB;

typedef void (APIENTRY * PFN_GLSHADERSOURCEARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB * * const string, const GLint * length);
extern PFN_GLSHADERSOURCEARB _glShaderSourceARB;

typedef void (APIENTRY * PFN_GLCOMPILESHADERARB)(GLhandleARB shaderObj);
extern PFN_GLCOMPILESHADERARB _glCompileShaderARB;

typedef GLhandleARB (APIENTRY * PFN_GLCREATEPROGRAMOBJECTARB)(void);
extern PFN_GLCREATEPROGRAMOBJECTARB _glCreateProgramObjectARB;

typedef void (APIENTRY * PFN_GLATTACHOBJECTARB)(GLhandleARB containerObj, GLhandleARB obj);
extern PFN_GLATTACHOBJECTARB _glAttachObjectARB;

typedef void (APIENTRY * PFN_GLLINKPROGRAMARB)(GLhandleARB programObj);
extern PFN_GLLINKPROGRAMARB _glLinkProgramARB;

typedef void (APIENTRY * PFN_GLUSEPROGRAMOBJECTARB)(GLhandleARB programObj);
extern PFN_GLUSEPROGRAMOBJECTARB _glUseProgramObjectARB;

typedef void (APIENTRY * PFN_GLVALIDATEPROGRAMARB)(GLhandleARB programObj);
extern PFN_GLVALIDATEPROGRAMARB _glValidateProgramARB;

typedef void (APIENTRY * PFN_GLUNIFORM1FARB)(GLint location, GLfloat v0);
extern PFN_GLUNIFORM1FARB _glUniform1fARB;

typedef void (APIENTRY * PFN_GLUNIFORM2FARB)(GLint location, GLfloat v0, GLfloat v1);
extern PFN_GLUNIFORM2FARB _glUniform2fARB;

typedef void (APIENTRY * PFN_GLUNIFORM3FARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFN_GLUNIFORM3FARB _glUniform3fARB;

typedef void (APIENTRY * PFN_GLUNIFORM4FARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFN_GLUNIFORM4FARB _glUniform4fARB;

typedef void (APIENTRY * PFN_GLUNIFORM1IARB)(GLint location, GLint v0);
extern PFN_GLUNIFORM1IARB _glUniform1iARB;

typedef void (APIENTRY * PFN_GLUNIFORM2IARB)(GLint location, GLint v0, GLint v1);
extern PFN_GLUNIFORM2IARB _glUniform2iARB;

typedef void (APIENTRY * PFN_GLUNIFORM3IARB)(GLint location, GLint v0, GLint v1, GLint v2);
extern PFN_GLUNIFORM3IARB _glUniform3iARB;

typedef void (APIENTRY * PFN_GLUNIFORM4IARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern PFN_GLUNIFORM4IARB _glUniform4iARB;

typedef void (APIENTRY * PFN_GLUNIFORM1FVARB)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM1FVARB _glUniform1fvARB;

typedef void (APIENTRY * PFN_GLUNIFORM2FVARB)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM2FVARB _glUniform2fvARB;

typedef void (APIENTRY * PFN_GLUNIFORM3FVARB)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM3FVARB _glUniform3fvARB;

typedef void (APIENTRY * PFN_GLUNIFORM4FVARB)(GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLUNIFORM4FVARB _glUniform4fvARB;

typedef void (APIENTRY * PFN_GLUNIFORM1IVARB)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM1IVARB _glUniform1ivARB;

typedef void (APIENTRY * PFN_GLUNIFORM2IVARB)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM2IVARB _glUniform2ivARB;

typedef void (APIENTRY * PFN_GLUNIFORM3IVARB)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM3IVARB _glUniform3ivARB;

typedef void (APIENTRY * PFN_GLUNIFORM4IVARB)(GLint location, GLsizei count, const GLint * value);
extern PFN_GLUNIFORM4IVARB _glUniform4ivARB;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX2FVARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX2FVARB _glUniformMatrix2fvARB;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX3FVARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX3FVARB _glUniformMatrix3fvARB;

typedef void (APIENTRY * PFN_GLUNIFORMMATRIX4FVARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLUNIFORMMATRIX4FVARB _glUniformMatrix4fvARB;

typedef void (APIENTRY * PFN_GLGETOBJECTPARAMETERFVARB)(GLhandleARB obj, GLenum pname, GLfloat * params);
extern PFN_GLGETOBJECTPARAMETERFVARB _glGetObjectParameterfvARB;

typedef void (APIENTRY * PFN_GLGETOBJECTPARAMETERIVARB)(GLhandleARB obj, GLenum pname, GLint * params);
extern PFN_GLGETOBJECTPARAMETERIVARB _glGetObjectParameterivARB;

typedef void (APIENTRY * PFN_GLGETINFOLOGARB)(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog);
extern PFN_GLGETINFOLOGARB _glGetInfoLogARB;

typedef void (APIENTRY * PFN_GLGETATTACHEDOBJECTSARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei * count, GLhandleARB * obj);
extern PFN_GLGETATTACHEDOBJECTSARB _glGetAttachedObjectsARB;

typedef GLint (APIENTRY * PFN_GLGETUNIFORMLOCATIONARB)(GLhandleARB programObj, const GLcharARB * name);
extern PFN_GLGETUNIFORMLOCATIONARB _glGetUniformLocationARB;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORMARB)(GLhandleARB programObj, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
extern PFN_GLGETACTIVEUNIFORMARB _glGetActiveUniformARB;

typedef void (APIENTRY * PFN_GLGETUNIFORMFVARB)(GLhandleARB programObj, GLint location, GLfloat * params);
extern PFN_GLGETUNIFORMFVARB _glGetUniformfvARB;

typedef void (APIENTRY * PFN_GLGETUNIFORMIVARB)(GLhandleARB programObj, GLint location, GLint * params);
extern PFN_GLGETUNIFORMIVARB _glGetUniformivARB;

typedef void (APIENTRY * PFN_GLGETSHADERSOURCEARB)(GLhandleARB obj, GLsizei bufSize, GLsizei * length, GLcharARB * source);
extern PFN_GLGETSHADERSOURCEARB _glGetShaderSourceARB;

typedef void (APIENTRY * PFN_GLSHADERSTORAGEBLOCKBINDING)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
extern PFN_GLSHADERSTORAGEBLOCKBINDING _glShaderStorageBlockBinding;

typedef GLint (APIENTRY * PFN_GLGETSUBROUTINEUNIFORMLOCATION)(GLuint program, GLenum shadertype, const GLchar * name);
extern PFN_GLGETSUBROUTINEUNIFORMLOCATION _glGetSubroutineUniformLocation;

typedef GLuint (APIENTRY * PFN_GLGETSUBROUTINEINDEX)(GLuint program, GLenum shadertype, const GLchar * name);
extern PFN_GLGETSUBROUTINEINDEX _glGetSubroutineIndex;

typedef void (APIENTRY * PFN_GLGETACTIVESUBROUTINEUNIFORMIV)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values);
extern PFN_GLGETACTIVESUBROUTINEUNIFORMIV _glGetActiveSubroutineUniformiv;

typedef void (APIENTRY * PFN_GLGETACTIVESUBROUTINEUNIFORMNAME)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
extern PFN_GLGETACTIVESUBROUTINEUNIFORMNAME _glGetActiveSubroutineUniformName;

typedef void (APIENTRY * PFN_GLGETACTIVESUBROUTINENAME)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
extern PFN_GLGETACTIVESUBROUTINENAME _glGetActiveSubroutineName;

typedef void (APIENTRY * PFN_GLUNIFORMSUBROUTINESUIV)(GLenum shadertype, GLsizei count, const GLuint * indices);
extern PFN_GLUNIFORMSUBROUTINESUIV _glUniformSubroutinesuiv;

typedef void (APIENTRY * PFN_GLGETUNIFORMSUBROUTINEUIV)(GLenum shadertype, GLint location, GLuint * params);
extern PFN_GLGETUNIFORMSUBROUTINEUIV _glGetUniformSubroutineuiv;

typedef void (APIENTRY * PFN_GLGETPROGRAMSTAGEIV)(GLuint program, GLenum shadertype, GLenum pname, GLint * values);
extern PFN_GLGETPROGRAMSTAGEIV _glGetProgramStageiv;

typedef void (APIENTRY * PFN_GLNAMEDSTRINGARB)(GLenum type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string);
extern PFN_GLNAMEDSTRINGARB _glNamedStringARB;

typedef void (APIENTRY * PFN_GLDELETENAMEDSTRINGARB)(GLint namelen, const GLchar * name);
extern PFN_GLDELETENAMEDSTRINGARB _glDeleteNamedStringARB;

typedef void (APIENTRY * PFN_GLCOMPILESHADERINCLUDEARB)(GLuint shader, GLsizei count, const GLchar * const * path, const GLint * length);
extern PFN_GLCOMPILESHADERINCLUDEARB _glCompileShaderIncludeARB;

typedef GLboolean (APIENTRY * PFN_GLISNAMEDSTRINGARB)(GLint namelen, const GLchar * name);
extern PFN_GLISNAMEDSTRINGARB _glIsNamedStringARB;

typedef void (APIENTRY * PFN_GLGETNAMEDSTRINGARB)(GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string);
extern PFN_GLGETNAMEDSTRINGARB _glGetNamedStringARB;

typedef void (APIENTRY * PFN_GLGETNAMEDSTRINGIVARB)(GLint namelen, const GLchar * name, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDSTRINGIVARB _glGetNamedStringivARB;

typedef void (APIENTRY * PFN_GLBUFFERPAGECOMMITMENTARB)(GLenum target, GLintptr offset, GLsizei size, GLboolean commit);
extern PFN_GLBUFFERPAGECOMMITMENTARB _glBufferPageCommitmentARB;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERPAGECOMMITMENTEXT)(GLuint buffer, GLintptr offset, GLsizei size, GLboolean commit);
extern PFN_GLNAMEDBUFFERPAGECOMMITMENTEXT _glNamedBufferPageCommitmentEXT;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERPAGECOMMITMENTARB)(GLuint buffer, GLintptr offset, GLsizei size, GLboolean commit);
extern PFN_GLNAMEDBUFFERPAGECOMMITMENTARB _glNamedBufferPageCommitmentARB;

typedef void (APIENTRY * PFN_GLTEXPAGECOMMITMENTARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident);
extern PFN_GLTEXPAGECOMMITMENTARB _glTexPageCommitmentARB;

typedef void (APIENTRY * PFN_GLTEXTUREPAGECOMMITMENTEXT)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
extern PFN_GLTEXTUREPAGECOMMITMENTEXT _glTexturePageCommitmentEXT;

typedef GLsync (APIENTRY * PFN_GLFENCESYNC)(GLenum condition, GLbitfield flags);
extern PFN_GLFENCESYNC _glFenceSync;

typedef GLboolean (APIENTRY * PFN_GLISSYNC)(GLsync sync);
extern PFN_GLISSYNC _glIsSync;

typedef void (APIENTRY * PFN_GLDELETESYNC)(GLsync sync);
extern PFN_GLDELETESYNC _glDeleteSync;

typedef GLenum (APIENTRY * PFN_GLCLIENTWAITSYNC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFN_GLCLIENTWAITSYNC _glClientWaitSync;

typedef void (APIENTRY * PFN_GLWAITSYNC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFN_GLWAITSYNC _glWaitSync;

typedef void (APIENTRY * PFN_GLGETINTEGER64V)(GLenum pname, GLint64 * params);
extern PFN_GLGETINTEGER64V _glGetInteger64v;

typedef void (APIENTRY * PFN_GLGETSYNCIV)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values);
extern PFN_GLGETSYNCIV _glGetSynciv;

typedef void (APIENTRY * PFN_GLPATCHPARAMETERI)(GLenum pname, GLint value);
extern PFN_GLPATCHPARAMETERI _glPatchParameteri;

typedef void (APIENTRY * PFN_GLPATCHPARAMETERFV)(GLenum pname, const GLfloat * values);
extern PFN_GLPATCHPARAMETERFV _glPatchParameterfv;

typedef void (APIENTRY * PFN_GLTEXTUREBARRIER)(void);
extern PFN_GLTEXTUREBARRIER _glTextureBarrier;

typedef void (APIENTRY * PFN_GLTEXBUFFERARB)(GLenum target, GLenum internalformat, GLuint buffer);
extern PFN_GLTEXBUFFERARB _glTexBufferARB;

typedef void (APIENTRY * PFN_GLTEXBUFFERRANGE)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFN_GLTEXBUFFERRANGE _glTexBufferRange;

typedef void (APIENTRY * PFN_GLTEXTUREBUFFERRANGEEXT)(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFN_GLTEXTUREBUFFERRANGEEXT _glTextureBufferRangeEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE3DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE3DARB _glCompressedTexImage3DARB;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE2DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE2DARB _glCompressedTexImage2DARB;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE1DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE1DARB _glCompressedTexImage1DARB;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE3DARB _glCompressedTexSubImage3DARB;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE2DARB _glCompressedTexSubImage2DARB;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE1DARB _glCompressedTexSubImage1DARB;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDTEXIMAGEARB)(GLenum target, GLint level, GLvoid * img);
extern PFN_GLGETCOMPRESSEDTEXIMAGEARB _glGetCompressedTexImageARB;

typedef void (APIENTRY * PFN_GLTEXIMAGE2DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFN_GLTEXIMAGE2DMULTISAMPLE _glTexImage2DMultisample;

typedef void (APIENTRY * PFN_GLTEXIMAGE3DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFN_GLTEXIMAGE3DMULTISAMPLE _glTexImage3DMultisample;

typedef void (APIENTRY * PFN_GLGETMULTISAMPLEFV)(GLenum pname, GLuint index, GLfloat * val);
extern PFN_GLGETMULTISAMPLEFV _glGetMultisamplefv;

typedef void (APIENTRY * PFN_GLSAMPLEMASKI)(GLuint index, GLbitfield mask);
extern PFN_GLSAMPLEMASKI _glSampleMaski;

typedef void (APIENTRY * PFN_GLTEXSTORAGE1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
extern PFN_GLTEXSTORAGE1D _glTexStorage1D;

typedef void (APIENTRY * PFN_GLTEXSTORAGE2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLTEXSTORAGE2D _glTexStorage2D;

typedef void (APIENTRY * PFN_GLTEXSTORAGE3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLTEXSTORAGE3D _glTexStorage3D;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
extern PFN_GLTEXTURESTORAGE1DEXT _glTextureStorage1DEXT;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLTEXTURESTORAGE2DEXT _glTextureStorage2DEXT;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLTEXTURESTORAGE3DEXT _glTextureStorage3DEXT;

typedef void (APIENTRY * PFN_GLTEXSTORAGE2DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFN_GLTEXSTORAGE2DMULTISAMPLE _glTexStorage2DMultisample;

typedef void (APIENTRY * PFN_GLTEXSTORAGE3DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFN_GLTEXSTORAGE3DMULTISAMPLE _glTexStorage3DMultisample;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE2DMULTISAMPLEEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFN_GLTEXTURESTORAGE2DMULTISAMPLEEXT _glTextureStorage2DMultisampleEXT;

typedef void (APIENTRY * PFN_GLTEXTURESTORAGE3DMULTISAMPLEEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFN_GLTEXTURESTORAGE3DMULTISAMPLEEXT _glTextureStorage3DMultisampleEXT;

typedef void (APIENTRY * PFN_GLTEXTUREVIEW)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
extern PFN_GLTEXTUREVIEW _glTextureView;

typedef void (APIENTRY * PFN_GLQUERYCOUNTER)(GLuint id, GLenum target);
extern PFN_GLQUERYCOUNTER _glQueryCounter;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTI64V)(GLuint id, GLenum pname, GLint64 * params);
extern PFN_GLGETQUERYOBJECTI64V _glGetQueryObjecti64v;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTUI64V)(GLuint id, GLenum pname, GLuint64 * params);
extern PFN_GLGETQUERYOBJECTUI64V _glGetQueryObjectui64v;

typedef void (APIENTRY * PFN_GLBINDTRANSFORMFEEDBACK)(GLenum target, GLuint id);
extern PFN_GLBINDTRANSFORMFEEDBACK _glBindTransformFeedback;

typedef void (APIENTRY * PFN_GLDELETETRANSFORMFEEDBACKS)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETETRANSFORMFEEDBACKS _glDeleteTransformFeedbacks;

typedef void (APIENTRY * PFN_GLGENTRANSFORMFEEDBACKS)(GLsizei n, GLuint * ids);
extern PFN_GLGENTRANSFORMFEEDBACKS _glGenTransformFeedbacks;

typedef GLboolean (APIENTRY * PFN_GLISTRANSFORMFEEDBACK)(GLuint id);
extern PFN_GLISTRANSFORMFEEDBACK _glIsTransformFeedback;

typedef void (APIENTRY * PFN_GLPAUSETRANSFORMFEEDBACK)(void);
extern PFN_GLPAUSETRANSFORMFEEDBACK _glPauseTransformFeedback;

typedef void (APIENTRY * PFN_GLRESUMETRANSFORMFEEDBACK)(void);
extern PFN_GLRESUMETRANSFORMFEEDBACK _glResumeTransformFeedback;

typedef void (APIENTRY * PFN_GLDRAWTRANSFORMFEEDBACK)(GLenum mode, GLuint id);
extern PFN_GLDRAWTRANSFORMFEEDBACK _glDrawTransformFeedback;

typedef void (APIENTRY * PFN_GLDRAWTRANSFORMFEEDBACKSTREAM)(GLenum mode, GLuint id, GLuint stream);
extern PFN_GLDRAWTRANSFORMFEEDBACKSTREAM _glDrawTransformFeedbackStream;

typedef void (APIENTRY * PFN_GLBEGINQUERYINDEXED)(GLenum target, GLuint index, GLuint id);
extern PFN_GLBEGINQUERYINDEXED _glBeginQueryIndexed;

typedef void (APIENTRY * PFN_GLENDQUERYINDEXED)(GLenum target, GLuint index);
extern PFN_GLENDQUERYINDEXED _glEndQueryIndexed;

typedef void (APIENTRY * PFN_GLGETQUERYINDEXEDIV)(GLenum target, GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETQUERYINDEXEDIV _glGetQueryIndexediv;

typedef void (APIENTRY * PFN_GLDRAWTRANSFORMFEEDBACKINSTANCED)(GLenum mode, GLuint id, GLsizei primcount);
extern PFN_GLDRAWTRANSFORMFEEDBACKINSTANCED _glDrawTransformFeedbackInstanced;

typedef void (APIENTRY * PFN_GLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED)(GLenum mode, GLuint id, GLuint stream, GLsizei primcount);
extern PFN_GLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED _glDrawTransformFeedbackStreamInstanced;

typedef void (APIENTRY * PFN_GLLOADTRANSPOSEMATRIXFARB)(const GLfloat * m);
extern PFN_GLLOADTRANSPOSEMATRIXFARB _glLoadTransposeMatrixfARB;

typedef void (APIENTRY * PFN_GLLOADTRANSPOSEMATRIXDARB)(const GLdouble * m);
extern PFN_GLLOADTRANSPOSEMATRIXDARB _glLoadTransposeMatrixdARB;

typedef void (APIENTRY * PFN_GLMULTTRANSPOSEMATRIXFARB)(const GLfloat * m);
extern PFN_GLMULTTRANSPOSEMATRIXFARB _glMultTransposeMatrixfARB;

typedef void (APIENTRY * PFN_GLMULTTRANSPOSEMATRIXDARB)(const GLdouble * m);
extern PFN_GLMULTTRANSPOSEMATRIXDARB _glMultTransposeMatrixdARB;

typedef void (APIENTRY * PFN_GLGETUNIFORMINDICES)(GLuint program, GLsizei uniformCount, const GLchar * const * uniformNames, GLuint * uniformIndices);
extern PFN_GLGETUNIFORMINDICES _glGetUniformIndices;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORMSIV)(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params);
extern PFN_GLGETACTIVEUNIFORMSIV _glGetActiveUniformsiv;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORMNAME)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName);
extern PFN_GLGETACTIVEUNIFORMNAME _glGetActiveUniformName;

typedef GLuint (APIENTRY * PFN_GLGETUNIFORMBLOCKINDEX)(GLuint program, const GLchar * uniformBlockName);
extern PFN_GLGETUNIFORMBLOCKINDEX _glGetUniformBlockIndex;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORMBLOCKIV)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params);
extern PFN_GLGETACTIVEUNIFORMBLOCKIV _glGetActiveUniformBlockiv;

typedef void (APIENTRY * PFN_GLGETACTIVEUNIFORMBLOCKNAME)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName);
extern PFN_GLGETACTIVEUNIFORMBLOCKNAME _glGetActiveUniformBlockName;

typedef void (APIENTRY * PFN_GLUNIFORMBLOCKBINDING)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
extern PFN_GLUNIFORMBLOCKBINDING _glUniformBlockBinding;

typedef void (APIENTRY * PFN_GLBINDVERTEXARRAY)(GLuint array);
extern PFN_GLBINDVERTEXARRAY _glBindVertexArray;

typedef void (APIENTRY * PFN_GLDELETEVERTEXARRAYS)(GLsizei n, const GLuint * arrays);
extern PFN_GLDELETEVERTEXARRAYS _glDeleteVertexArrays;

typedef void (APIENTRY * PFN_GLGENVERTEXARRAYS)(GLsizei n, GLuint * arrays);
extern PFN_GLGENVERTEXARRAYS _glGenVertexArrays;

typedef GLboolean (APIENTRY * PFN_GLISVERTEXARRAY)(GLuint array);
extern PFN_GLISVERTEXARRAY _glIsVertexArray;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1D)(GLuint index, GLdouble x);
extern PFN_GLVERTEXATTRIBL1D _glVertexAttribL1d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2D)(GLuint index, GLdouble x, GLdouble y);
extern PFN_GLVERTEXATTRIBL2D _glVertexAttribL2d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3D)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXATTRIBL3D _glVertexAttribL3d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4D)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXATTRIBL4D _glVertexAttribL4d;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL1DV _glVertexAttribL1dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL2DV _glVertexAttribL2dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL3DV _glVertexAttribL3dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4DV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL4DV _glVertexAttribL4dv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBLPOINTER)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBLPOINTER _glVertexAttribLPointer;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBLDV)(GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETVERTEXATTRIBLDV _glGetVertexAttribLdv;

typedef void (APIENTRY * PFN_GLBINDVERTEXBUFFER)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern PFN_GLBINDVERTEXBUFFER _glBindVertexBuffer;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBFORMAT)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern PFN_GLVERTEXATTRIBFORMAT _glVertexAttribFormat;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBIFORMAT)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXATTRIBIFORMAT _glVertexAttribIFormat;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBLFORMAT)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXATTRIBLFORMAT _glVertexAttribLFormat;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBBINDING)(GLuint attribindex, GLuint bindingindex);
extern PFN_GLVERTEXATTRIBBINDING _glVertexAttribBinding;

typedef void (APIENTRY * PFN_GLVERTEXBINDINGDIVISOR)(GLuint bindingindex, GLuint divisor);
extern PFN_GLVERTEXBINDINGDIVISOR _glVertexBindingDivisor;

typedef void (APIENTRY * PFN_GLVERTEXARRAYBINDVERTEXBUFFEREXT)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern PFN_GLVERTEXARRAYBINDVERTEXBUFFEREXT _glVertexArrayBindVertexBufferEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBFORMATEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBFORMATEXT _glVertexArrayVertexAttribFormatEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBIFORMATEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBIFORMATEXT _glVertexArrayVertexAttribIFormatEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBLFORMATEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBLFORMATEXT _glVertexArrayVertexAttribLFormatEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBBINDINGEXT)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
extern PFN_GLVERTEXARRAYVERTEXATTRIBBINDINGEXT _glVertexArrayVertexAttribBindingEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXBINDINGDIVISOREXT)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
extern PFN_GLVERTEXARRAYVERTEXBINDINGDIVISOREXT _glVertexArrayVertexBindingDivisorEXT;

typedef void (APIENTRY * PFN_GLWEIGHTBVARB)(GLint size, const GLbyte * weights);
extern PFN_GLWEIGHTBVARB _glWeightbvARB;

typedef void (APIENTRY * PFN_GLWEIGHTSVARB)(GLint size, const GLshort * weights);
extern PFN_GLWEIGHTSVARB _glWeightsvARB;

typedef void (APIENTRY * PFN_GLWEIGHTIVARB)(GLint size, const GLint * weights);
extern PFN_GLWEIGHTIVARB _glWeightivARB;

typedef void (APIENTRY * PFN_GLWEIGHTFVARB)(GLint size, const GLfloat * weights);
extern PFN_GLWEIGHTFVARB _glWeightfvARB;

typedef void (APIENTRY * PFN_GLWEIGHTDVARB)(GLint size, const GLdouble * weights);
extern PFN_GLWEIGHTDVARB _glWeightdvARB;

typedef void (APIENTRY * PFN_GLWEIGHTUBVARB)(GLint size, const GLubyte * weights);
extern PFN_GLWEIGHTUBVARB _glWeightubvARB;

typedef void (APIENTRY * PFN_GLWEIGHTUSVARB)(GLint size, const GLushort * weights);
extern PFN_GLWEIGHTUSVARB _glWeightusvARB;

typedef void (APIENTRY * PFN_GLWEIGHTUIVARB)(GLint size, const GLuint * weights);
extern PFN_GLWEIGHTUIVARB _glWeightuivARB;

typedef void (APIENTRY * PFN_GLWEIGHTPOINTERARB)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLWEIGHTPOINTERARB _glWeightPointerARB;

typedef void (APIENTRY * PFN_GLVERTEXBLENDARB)(GLint count);
extern PFN_GLVERTEXBLENDARB _glVertexBlendARB;

typedef void (APIENTRY * PFN_GLBINDBUFFERARB)(GLenum target, GLuint buffer);
extern PFN_GLBINDBUFFERARB _glBindBufferARB;

typedef void (APIENTRY * PFN_GLDELETEBUFFERSARB)(GLsizei n, const GLuint * buffers);
extern PFN_GLDELETEBUFFERSARB _glDeleteBuffersARB;

typedef void (APIENTRY * PFN_GLGENBUFFERSARB)(GLsizei n, GLuint * buffers);
extern PFN_GLGENBUFFERSARB _glGenBuffersARB;

typedef GLboolean (APIENTRY * PFN_GLISBUFFERARB)(GLuint buffer);
extern PFN_GLISBUFFERARB _glIsBufferARB;

typedef void (APIENTRY * PFN_GLBUFFERDATAARB)(GLenum target, GLsizeiptrARB size, const GLvoid * data, GLenum usage);
extern PFN_GLBUFFERDATAARB _glBufferDataARB;

typedef void (APIENTRY * PFN_GLBUFFERSUBDATAARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data);
extern PFN_GLBUFFERSUBDATAARB _glBufferSubDataARB;

typedef void (APIENTRY * PFN_GLGETBUFFERSUBDATAARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid * data);
extern PFN_GLGETBUFFERSUBDATAARB _glGetBufferSubDataARB;

typedef GLvoid * (APIENTRY * PFN_GLMAPBUFFERARB)(GLenum target, GLenum access);
extern PFN_GLMAPBUFFERARB _glMapBufferARB;

typedef GLboolean (APIENTRY * PFN_GLUNMAPBUFFERARB)(GLenum target);
extern PFN_GLUNMAPBUFFERARB _glUnmapBufferARB;

typedef void (APIENTRY * PFN_GLGETBUFFERPARAMETERIVARB)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETBUFFERPARAMETERIVARB _glGetBufferParameterivARB;

typedef void (APIENTRY * PFN_GLGETBUFFERPOINTERVARB)(GLenum target, GLenum pname, GLvoid * * params);
extern PFN_GLGETBUFFERPOINTERVARB _glGetBufferPointervARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1DARB)(GLuint index, GLdouble x);
extern PFN_GLVERTEXATTRIB1DARB _glVertexAttrib1dARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1DVARB)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB1DVARB _glVertexAttrib1dvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1FARB)(GLuint index, GLfloat x);
extern PFN_GLVERTEXATTRIB1FARB _glVertexAttrib1fARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1FVARB)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB1FVARB _glVertexAttrib1fvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1SARB)(GLuint index, GLshort x);
extern PFN_GLVERTEXATTRIB1SARB _glVertexAttrib1sARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1SVARB)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB1SVARB _glVertexAttrib1svARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2DARB)(GLuint index, GLdouble x, GLdouble y);
extern PFN_GLVERTEXATTRIB2DARB _glVertexAttrib2dARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2DVARB)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB2DVARB _glVertexAttrib2dvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2FARB)(GLuint index, GLfloat x, GLfloat y);
extern PFN_GLVERTEXATTRIB2FARB _glVertexAttrib2fARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2FVARB)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB2FVARB _glVertexAttrib2fvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2SARB)(GLuint index, GLshort x, GLshort y);
extern PFN_GLVERTEXATTRIB2SARB _glVertexAttrib2sARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2SVARB)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB2SVARB _glVertexAttrib2svARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3DARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXATTRIB3DARB _glVertexAttrib3dARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3DVARB)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB3DVARB _glVertexAttrib3dvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3FARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLVERTEXATTRIB3FARB _glVertexAttrib3fARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3FVARB)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB3FVARB _glVertexAttrib3fvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3SARB)(GLuint index, GLshort x, GLshort y, GLshort z);
extern PFN_GLVERTEXATTRIB3SARB _glVertexAttrib3sARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3SVARB)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB3SVARB _glVertexAttrib3svARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NBVARB)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIB4NBVARB _glVertexAttrib4NbvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NIVARB)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIB4NIVARB _glVertexAttrib4NivARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NSVARB)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB4NSVARB _glVertexAttrib4NsvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUBARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern PFN_GLVERTEXATTRIB4NUBARB _glVertexAttrib4NubARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUBVARB)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIB4NUBVARB _glVertexAttrib4NubvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUIVARB)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIB4NUIVARB _glVertexAttrib4NuivARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4NUSVARB)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIB4NUSVARB _glVertexAttrib4NusvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4BVARB)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIB4BVARB _glVertexAttrib4bvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4DARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXATTRIB4DARB _glVertexAttrib4dARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4DVARB)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB4DVARB _glVertexAttrib4dvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4FARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLVERTEXATTRIB4FARB _glVertexAttrib4fARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4FVARB)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB4FVARB _glVertexAttrib4fvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4IVARB)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIB4IVARB _glVertexAttrib4ivARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4SARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLVERTEXATTRIB4SARB _glVertexAttrib4sARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4SVARB)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB4SVARB _glVertexAttrib4svARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UBVARB)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIB4UBVARB _glVertexAttrib4ubvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UIVARB)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIB4UIVARB _glVertexAttrib4uivARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4USVARB)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIB4USVARB _glVertexAttrib4usvARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBPOINTERARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBPOINTERARB _glVertexAttribPointerARB;

typedef void (APIENTRY * PFN_GLENABLEVERTEXATTRIBARRAYARB)(GLuint index);
extern PFN_GLENABLEVERTEXATTRIBARRAYARB _glEnableVertexAttribArrayARB;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXATTRIBARRAYARB)(GLuint index);
extern PFN_GLDISABLEVERTEXATTRIBARRAYARB _glDisableVertexAttribArrayARB;

typedef void (APIENTRY * PFN_GLPROGRAMSTRINGARB)(GLenum target, GLenum format, GLsizei len, const void * string);
extern PFN_GLPROGRAMSTRINGARB _glProgramStringARB;

typedef void (APIENTRY * PFN_GLBINDPROGRAMARB)(GLenum target, GLuint program);
extern PFN_GLBINDPROGRAMARB _glBindProgramARB;

typedef void (APIENTRY * PFN_GLDELETEPROGRAMSARB)(GLsizei n, const GLuint * programs);
extern PFN_GLDELETEPROGRAMSARB _glDeleteProgramsARB;

typedef void (APIENTRY * PFN_GLGENPROGRAMSARB)(GLsizei n, GLuint * programs);
extern PFN_GLGENPROGRAMSARB _glGenProgramsARB;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETER4DARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLPROGRAMENVPARAMETER4DARB _glProgramEnvParameter4dARB;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETER4DVARB)(GLenum target, GLuint index, const GLdouble * params);
extern PFN_GLPROGRAMENVPARAMETER4DVARB _glProgramEnvParameter4dvARB;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETER4FARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLPROGRAMENVPARAMETER4FARB _glProgramEnvParameter4fARB;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETER4FVARB)(GLenum target, GLuint index, const GLfloat * params);
extern PFN_GLPROGRAMENVPARAMETER4FVARB _glProgramEnvParameter4fvARB;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETER4DARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLPROGRAMLOCALPARAMETER4DARB _glProgramLocalParameter4dARB;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETER4DVARB)(GLenum target, GLuint index, const GLdouble * params);
extern PFN_GLPROGRAMLOCALPARAMETER4DVARB _glProgramLocalParameter4dvARB;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETER4FARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLPROGRAMLOCALPARAMETER4FARB _glProgramLocalParameter4fARB;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETER4FVARB)(GLenum target, GLuint index, const GLfloat * params);
extern PFN_GLPROGRAMLOCALPARAMETER4FVARB _glProgramLocalParameter4fvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMENVPARAMETERDVARB)(GLenum target, GLuint index, GLdouble * params);
extern PFN_GLGETPROGRAMENVPARAMETERDVARB _glGetProgramEnvParameterdvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMENVPARAMETERFVARB)(GLenum target, GLuint index, GLfloat * params);
extern PFN_GLGETPROGRAMENVPARAMETERFVARB _glGetProgramEnvParameterfvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMLOCALPARAMETERDVARB)(GLenum target, GLuint index, GLdouble * params);
extern PFN_GLGETPROGRAMLOCALPARAMETERDVARB _glGetProgramLocalParameterdvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMLOCALPARAMETERFVARB)(GLenum target, GLuint index, GLfloat * params);
extern PFN_GLGETPROGRAMLOCALPARAMETERFVARB _glGetProgramLocalParameterfvARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMIVARB)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMIVARB _glGetProgramivARB;

typedef void (APIENTRY * PFN_GLGETPROGRAMSTRINGARB)(GLenum target, GLenum pname, GLvoid * string);
extern PFN_GLGETPROGRAMSTRINGARB _glGetProgramStringARB;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBDVARB)(GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETVERTEXATTRIBDVARB _glGetVertexAttribdvARB;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBFVARB)(GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETVERTEXATTRIBFVARB _glGetVertexAttribfvARB;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIVARB)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBIVARB _glGetVertexAttribivARB;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBPOINTERVARB)(GLuint index, GLenum pname, GLvoid * * pointer);
extern PFN_GLGETVERTEXATTRIBPOINTERVARB _glGetVertexAttribPointervARB;

typedef GLboolean (APIENTRY * PFN_GLISPROGRAMARB)(GLuint program);
extern PFN_GLISPROGRAMARB _glIsProgramARB;

typedef void (APIENTRY * PFN_GLBINDATTRIBLOCATIONARB)(GLhandleARB programObj, GLuint index, const GLcharARB * name);
extern PFN_GLBINDATTRIBLOCATIONARB _glBindAttribLocationARB;

typedef void (APIENTRY * PFN_GLGETACTIVEATTRIBARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
extern PFN_GLGETACTIVEATTRIBARB _glGetActiveAttribARB;

typedef GLint (APIENTRY * PFN_GLGETATTRIBLOCATIONARB)(GLhandleARB programObj, const GLcharARB * name);
extern PFN_GLGETATTRIBLOCATIONARB _glGetAttribLocationARB;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP1UI)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFN_GLVERTEXATTRIBP1UI _glVertexAttribP1ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP1UIV)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
extern PFN_GLVERTEXATTRIBP1UIV _glVertexAttribP1uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP2UI)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFN_GLVERTEXATTRIBP2UI _glVertexAttribP2ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP2UIV)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
extern PFN_GLVERTEXATTRIBP2UIV _glVertexAttribP2uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP3UI)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFN_GLVERTEXATTRIBP3UI _glVertexAttribP3ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP3UIV)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
extern PFN_GLVERTEXATTRIBP3UIV _glVertexAttribP3uiv;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP4UI)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFN_GLVERTEXATTRIBP4UI _glVertexAttribP4ui;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBP4UIV)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
extern PFN_GLVERTEXATTRIBP4UIV _glVertexAttribP4uiv;

typedef void (APIENTRY * PFN_GLVERTEXP2UI)(GLenum type, GLuint value);
extern PFN_GLVERTEXP2UI _glVertexP2ui;

typedef void (APIENTRY * PFN_GLVERTEXP2UIV)(GLenum type, const GLuint * value);
extern PFN_GLVERTEXP2UIV _glVertexP2uiv;

typedef void (APIENTRY * PFN_GLVERTEXP3UI)(GLenum type, GLuint value);
extern PFN_GLVERTEXP3UI _glVertexP3ui;

typedef void (APIENTRY * PFN_GLVERTEXP3UIV)(GLenum type, const GLuint * value);
extern PFN_GLVERTEXP3UIV _glVertexP3uiv;

typedef void (APIENTRY * PFN_GLVERTEXP4UI)(GLenum type, GLuint value);
extern PFN_GLVERTEXP4UI _glVertexP4ui;

typedef void (APIENTRY * PFN_GLVERTEXP4UIV)(GLenum type, const GLuint * value);
extern PFN_GLVERTEXP4UIV _glVertexP4uiv;

typedef void (APIENTRY * PFN_GLTEXCOORDP1UI)(GLenum type, GLuint coords);
extern PFN_GLTEXCOORDP1UI _glTexCoordP1ui;

typedef void (APIENTRY * PFN_GLTEXCOORDP1UIV)(GLenum type, const GLuint * coords);
extern PFN_GLTEXCOORDP1UIV _glTexCoordP1uiv;

typedef void (APIENTRY * PFN_GLTEXCOORDP2UI)(GLenum type, GLuint coords);
extern PFN_GLTEXCOORDP2UI _glTexCoordP2ui;

typedef void (APIENTRY * PFN_GLTEXCOORDP2UIV)(GLenum type, const GLuint * coords);
extern PFN_GLTEXCOORDP2UIV _glTexCoordP2uiv;

typedef void (APIENTRY * PFN_GLTEXCOORDP3UI)(GLenum type, GLuint coords);
extern PFN_GLTEXCOORDP3UI _glTexCoordP3ui;

typedef void (APIENTRY * PFN_GLTEXCOORDP3UIV)(GLenum type, const GLuint * coords);
extern PFN_GLTEXCOORDP3UIV _glTexCoordP3uiv;

typedef void (APIENTRY * PFN_GLTEXCOORDP4UI)(GLenum type, GLuint coords);
extern PFN_GLTEXCOORDP4UI _glTexCoordP4ui;

typedef void (APIENTRY * PFN_GLTEXCOORDP4UIV)(GLenum type, const GLuint * coords);
extern PFN_GLTEXCOORDP4UIV _glTexCoordP4uiv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP1UI)(GLenum texture, GLenum type, GLuint coords);
extern PFN_GLMULTITEXCOORDP1UI _glMultiTexCoordP1ui;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP1UIV)(GLenum texture, GLenum type, const GLuint * coords);
extern PFN_GLMULTITEXCOORDP1UIV _glMultiTexCoordP1uiv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP2UI)(GLenum texture, GLenum type, GLuint coords);
extern PFN_GLMULTITEXCOORDP2UI _glMultiTexCoordP2ui;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP2UIV)(GLenum texture, GLenum type, const GLuint * coords);
extern PFN_GLMULTITEXCOORDP2UIV _glMultiTexCoordP2uiv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP3UI)(GLenum texture, GLenum type, GLuint coords);
extern PFN_GLMULTITEXCOORDP3UI _glMultiTexCoordP3ui;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP3UIV)(GLenum texture, GLenum type, const GLuint * coords);
extern PFN_GLMULTITEXCOORDP3UIV _glMultiTexCoordP3uiv;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP4UI)(GLenum texture, GLenum type, GLuint coords);
extern PFN_GLMULTITEXCOORDP4UI _glMultiTexCoordP4ui;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDP4UIV)(GLenum texture, GLenum type, const GLuint * coords);
extern PFN_GLMULTITEXCOORDP4UIV _glMultiTexCoordP4uiv;

typedef void (APIENTRY * PFN_GLNORMALP3UI)(GLenum type, GLuint coords);
extern PFN_GLNORMALP3UI _glNormalP3ui;

typedef void (APIENTRY * PFN_GLNORMALP3UIV)(GLenum type, const GLuint * coords);
extern PFN_GLNORMALP3UIV _glNormalP3uiv;

typedef void (APIENTRY * PFN_GLCOLORP3UI)(GLenum type, GLuint color);
extern PFN_GLCOLORP3UI _glColorP3ui;

typedef void (APIENTRY * PFN_GLCOLORP3UIV)(GLenum type, const GLuint * color);
extern PFN_GLCOLORP3UIV _glColorP3uiv;

typedef void (APIENTRY * PFN_GLCOLORP4UI)(GLenum type, GLuint color);
extern PFN_GLCOLORP4UI _glColorP4ui;

typedef void (APIENTRY * PFN_GLCOLORP4UIV)(GLenum type, const GLuint * color);
extern PFN_GLCOLORP4UIV _glColorP4uiv;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORP3UI)(GLenum type, GLuint color);
extern PFN_GLSECONDARYCOLORP3UI _glSecondaryColorP3ui;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORP3UIV)(GLenum type, const GLuint * color);
extern PFN_GLSECONDARYCOLORP3UIV _glSecondaryColorP3uiv;

typedef void (APIENTRY * PFN_GLVIEWPORTARRAYV)(GLuint first, GLsizei count, const GLfloat * v);
extern PFN_GLVIEWPORTARRAYV _glViewportArrayv;

typedef void (APIENTRY * PFN_GLVIEWPORTINDEXEDF)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
extern PFN_GLVIEWPORTINDEXEDF _glViewportIndexedf;

typedef void (APIENTRY * PFN_GLVIEWPORTINDEXEDFV)(GLuint index, const GLfloat * v);
extern PFN_GLVIEWPORTINDEXEDFV _glViewportIndexedfv;

typedef void (APIENTRY * PFN_GLSCISSORARRAYV)(GLuint first, GLsizei count, const GLint * v);
extern PFN_GLSCISSORARRAYV _glScissorArrayv;

typedef void (APIENTRY * PFN_GLSCISSORINDEXED)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
extern PFN_GLSCISSORINDEXED _glScissorIndexed;

typedef void (APIENTRY * PFN_GLSCISSORINDEXEDV)(GLuint index, const GLint * v);
extern PFN_GLSCISSORINDEXEDV _glScissorIndexedv;

typedef void (APIENTRY * PFN_GLDEPTHRANGEARRAYV)(GLuint first, GLsizei count, const GLdouble * v);
extern PFN_GLDEPTHRANGEARRAYV _glDepthRangeArrayv;

typedef void (APIENTRY * PFN_GLDEPTHRANGEINDEXED)(GLuint index, GLdouble n, GLdouble f);
extern PFN_GLDEPTHRANGEINDEXED _glDepthRangeIndexed;

typedef void (APIENTRY * PFN_GLGETFLOATI_V)(GLenum target, GLuint index, GLfloat * data);
extern PFN_GLGETFLOATI_V _glGetFloati_v;

typedef void (APIENTRY * PFN_GLGETDOUBLEI_V)(GLenum target, GLuint index, GLdouble * data);
extern PFN_GLGETDOUBLEI_V _glGetDoublei_v;

typedef void (APIENTRY * PFN_GLWINDOWPOS2DARB)(GLdouble x, GLdouble y);
extern PFN_GLWINDOWPOS2DARB _glWindowPos2dARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2DVARB)(const GLdouble * v);
extern PFN_GLWINDOWPOS2DVARB _glWindowPos2dvARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2FARB)(GLfloat x, GLfloat y);
extern PFN_GLWINDOWPOS2FARB _glWindowPos2fARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2FVARB)(const GLfloat * v);
extern PFN_GLWINDOWPOS2FVARB _glWindowPos2fvARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2IARB)(GLint x, GLint y);
extern PFN_GLWINDOWPOS2IARB _glWindowPos2iARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2IVARB)(const GLint * v);
extern PFN_GLWINDOWPOS2IVARB _glWindowPos2ivARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2SARB)(GLshort x, GLshort y);
extern PFN_GLWINDOWPOS2SARB _glWindowPos2sARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS2SVARB)(const GLshort * v);
extern PFN_GLWINDOWPOS2SVARB _glWindowPos2svARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3DARB)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLWINDOWPOS3DARB _glWindowPos3dARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3DVARB)(const GLdouble * v);
extern PFN_GLWINDOWPOS3DVARB _glWindowPos3dvARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3FARB)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLWINDOWPOS3FARB _glWindowPos3fARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3FVARB)(const GLfloat * v);
extern PFN_GLWINDOWPOS3FVARB _glWindowPos3fvARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3IARB)(GLint x, GLint y, GLint z);
extern PFN_GLWINDOWPOS3IARB _glWindowPos3iARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3IVARB)(const GLint * v);
extern PFN_GLWINDOWPOS3IVARB _glWindowPos3ivARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3SARB)(GLshort x, GLshort y, GLshort z);
extern PFN_GLWINDOWPOS3SARB _glWindowPos3sARB;

typedef void (APIENTRY * PFN_GLWINDOWPOS3SVARB)(const GLshort * v);
extern PFN_GLWINDOWPOS3SVARB _glWindowPos3svARB;

typedef void (APIENTRY * PFN_GLDRAWBUFFERSATI)(GLsizei n, const GLenum * bufs);
extern PFN_GLDRAWBUFFERSATI _glDrawBuffersATI;

typedef void (APIENTRY * PFN_GLELEMENTPOINTERATI)(GLenum type, const GLvoid * pointer);
extern PFN_GLELEMENTPOINTERATI _glElementPointerATI;

typedef void (APIENTRY * PFN_GLDRAWELEMENTARRAYATI)(GLenum mode, GLsizei count);
extern PFN_GLDRAWELEMENTARRAYATI _glDrawElementArrayATI;

typedef void (APIENTRY * PFN_GLDRAWRANGEELEMENTARRAYATI)(GLenum mode, GLuint start, GLuint end, GLsizei count);
extern PFN_GLDRAWRANGEELEMENTARRAYATI _glDrawRangeElementArrayATI;

typedef void (APIENTRY * PFN_GLTEXBUMPPARAMETERIVATI)(GLenum pname, const GLint * param);
extern PFN_GLTEXBUMPPARAMETERIVATI _glTexBumpParameterivATI;

typedef void (APIENTRY * PFN_GLTEXBUMPPARAMETERFVATI)(GLenum pname, const GLfloat * param);
extern PFN_GLTEXBUMPPARAMETERFVATI _glTexBumpParameterfvATI;

typedef void (APIENTRY * PFN_GLGETTEXBUMPPARAMETERIVATI)(GLenum pname, GLint * param);
extern PFN_GLGETTEXBUMPPARAMETERIVATI _glGetTexBumpParameterivATI;

typedef void (APIENTRY * PFN_GLGETTEXBUMPPARAMETERFVATI)(GLenum pname, GLfloat * param);
extern PFN_GLGETTEXBUMPPARAMETERFVATI _glGetTexBumpParameterfvATI;

typedef GLuint (APIENTRY * PFN_GLGENFRAGMENTSHADERSATI)(GLuint range);
extern PFN_GLGENFRAGMENTSHADERSATI _glGenFragmentShadersATI;

typedef void (APIENTRY * PFN_GLBINDFRAGMENTSHADERATI)(GLuint id);
extern PFN_GLBINDFRAGMENTSHADERATI _glBindFragmentShaderATI;

typedef void (APIENTRY * PFN_GLDELETEFRAGMENTSHADERATI)(GLuint id);
extern PFN_GLDELETEFRAGMENTSHADERATI _glDeleteFragmentShaderATI;

typedef void (APIENTRY * PFN_GLBEGINFRAGMENTSHADERATI)(void);
extern PFN_GLBEGINFRAGMENTSHADERATI _glBeginFragmentShaderATI;

typedef void (APIENTRY * PFN_GLENDFRAGMENTSHADERATI)(void);
extern PFN_GLENDFRAGMENTSHADERATI _glEndFragmentShaderATI;

typedef void (APIENTRY * PFN_GLPASSTEXCOORDATI)(GLuint dst, GLuint coord, GLenum swizzle);
extern PFN_GLPASSTEXCOORDATI _glPassTexCoordATI;

typedef void (APIENTRY * PFN_GLSAMPLEMAPATI)(GLuint dst, GLuint interp, GLenum swizzle);
extern PFN_GLSAMPLEMAPATI _glSampleMapATI;

typedef void (APIENTRY * PFN_GLCOLORFRAGMENTOP1ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern PFN_GLCOLORFRAGMENTOP1ATI _glColorFragmentOp1ATI;

typedef void (APIENTRY * PFN_GLCOLORFRAGMENTOP2ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern PFN_GLCOLORFRAGMENTOP2ATI _glColorFragmentOp2ATI;

typedef void (APIENTRY * PFN_GLCOLORFRAGMENTOP3ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern PFN_GLCOLORFRAGMENTOP3ATI _glColorFragmentOp3ATI;

typedef void (APIENTRY * PFN_GLALPHAFRAGMENTOP1ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern PFN_GLALPHAFRAGMENTOP1ATI _glAlphaFragmentOp1ATI;

typedef void (APIENTRY * PFN_GLALPHAFRAGMENTOP2ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern PFN_GLALPHAFRAGMENTOP2ATI _glAlphaFragmentOp2ATI;

typedef void (APIENTRY * PFN_GLALPHAFRAGMENTOP3ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern PFN_GLALPHAFRAGMENTOP3ATI _glAlphaFragmentOp3ATI;

typedef void (APIENTRY * PFN_GLSETFRAGMENTSHADERCONSTANTATI)(GLuint dst, const GLfloat * value);
extern PFN_GLSETFRAGMENTSHADERCONSTANTATI _glSetFragmentShaderConstantATI;

typedef GLvoid * (APIENTRY * PFN_GLMAPOBJECTBUFFERATI)(GLuint buffer);
extern PFN_GLMAPOBJECTBUFFERATI _glMapObjectBufferATI;

typedef void (APIENTRY * PFN_GLUNMAPOBJECTBUFFERATI)(GLuint buffer);
extern PFN_GLUNMAPOBJECTBUFFERATI _glUnmapObjectBufferATI;

typedef void (APIENTRY * PFN_GLPNTRIANGLESIATI)(GLenum pname, GLint param);
extern PFN_GLPNTRIANGLESIATI _glPNTrianglesiATI;

typedef void (APIENTRY * PFN_GLPNTRIANGLESFATI)(GLenum pname, GLfloat param);
extern PFN_GLPNTRIANGLESFATI _glPNTrianglesfATI;

typedef void (APIENTRY * PFN_GLSTENCILOPSEPARATEATI)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern PFN_GLSTENCILOPSEPARATEATI _glStencilOpSeparateATI;

typedef void (APIENTRY * PFN_GLSTENCILFUNCSEPARATEATI)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern PFN_GLSTENCILFUNCSEPARATEATI _glStencilFuncSeparateATI;

typedef GLuint (APIENTRY * PFN_GLNEWOBJECTBUFFERATI)(GLsizei size, const GLvoid * pointer, GLenum usage);
extern PFN_GLNEWOBJECTBUFFERATI _glNewObjectBufferATI;

typedef GLboolean (APIENTRY * PFN_GLISOBJECTBUFFERATI)(GLuint buffer);
extern PFN_GLISOBJECTBUFFERATI _glIsObjectBufferATI;

typedef void (APIENTRY * PFN_GLUPDATEOBJECTBUFFERATI)(GLuint buffer, GLuint offset, GLsizei size, const GLvoid * pointer, GLenum preserve);
extern PFN_GLUPDATEOBJECTBUFFERATI _glUpdateObjectBufferATI;

typedef void (APIENTRY * PFN_GLGETOBJECTBUFFERFVATI)(GLuint buffer, GLenum pname, GLfloat * params);
extern PFN_GLGETOBJECTBUFFERFVATI _glGetObjectBufferfvATI;

typedef void (APIENTRY * PFN_GLGETOBJECTBUFFERIVATI)(GLuint buffer, GLenum pname, GLint * params);
extern PFN_GLGETOBJECTBUFFERIVATI _glGetObjectBufferivATI;

typedef void (APIENTRY * PFN_GLFREEOBJECTBUFFERATI)(GLuint buffer);
extern PFN_GLFREEOBJECTBUFFERATI _glFreeObjectBufferATI;

typedef void (APIENTRY * PFN_GLARRAYOBJECTATI)(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
extern PFN_GLARRAYOBJECTATI _glArrayObjectATI;

typedef void (APIENTRY * PFN_GLGETARRAYOBJECTFVATI)(GLenum array, GLenum pname, GLfloat * params);
extern PFN_GLGETARRAYOBJECTFVATI _glGetArrayObjectfvATI;

typedef void (APIENTRY * PFN_GLGETARRAYOBJECTIVATI)(GLenum array, GLenum pname, GLint * params);
extern PFN_GLGETARRAYOBJECTIVATI _glGetArrayObjectivATI;

typedef void (APIENTRY * PFN_GLVARIANTARRAYOBJECTATI)(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
extern PFN_GLVARIANTARRAYOBJECTATI _glVariantArrayObjectATI;

typedef void (APIENTRY * PFN_GLGETVARIANTARRAYOBJECTFVATI)(GLuint id, GLenum pname, GLfloat * params);
extern PFN_GLGETVARIANTARRAYOBJECTFVATI _glGetVariantArrayObjectfvATI;

typedef void (APIENTRY * PFN_GLGETVARIANTARRAYOBJECTIVATI)(GLuint id, GLenum pname, GLint * params);
extern PFN_GLGETVARIANTARRAYOBJECTIVATI _glGetVariantArrayObjectivATI;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBARRAYOBJECTATI)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);
extern PFN_GLVERTEXATTRIBARRAYOBJECTATI _glVertexAttribArrayObjectATI;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBARRAYOBJECTFVATI)(GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETVERTEXATTRIBARRAYOBJECTFVATI _glGetVertexAttribArrayObjectfvATI;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBARRAYOBJECTIVATI)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBARRAYOBJECTIVATI _glGetVertexAttribArrayObjectivATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1SATI)(GLenum stream, GLshort x);
extern PFN_GLVERTEXSTREAM1SATI _glVertexStream1sATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1SVATI)(GLenum stream, const GLshort * coords);
extern PFN_GLVERTEXSTREAM1SVATI _glVertexStream1svATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1IATI)(GLenum stream, GLint x);
extern PFN_GLVERTEXSTREAM1IATI _glVertexStream1iATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1IVATI)(GLenum stream, const GLint * coords);
extern PFN_GLVERTEXSTREAM1IVATI _glVertexStream1ivATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1FATI)(GLenum stream, GLfloat x);
extern PFN_GLVERTEXSTREAM1FATI _glVertexStream1fATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1FVATI)(GLenum stream, const GLfloat * coords);
extern PFN_GLVERTEXSTREAM1FVATI _glVertexStream1fvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1DATI)(GLenum stream, GLdouble x);
extern PFN_GLVERTEXSTREAM1DATI _glVertexStream1dATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM1DVATI)(GLenum stream, const GLdouble * coords);
extern PFN_GLVERTEXSTREAM1DVATI _glVertexStream1dvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2SATI)(GLenum stream, GLshort x, GLshort y);
extern PFN_GLVERTEXSTREAM2SATI _glVertexStream2sATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2SVATI)(GLenum stream, const GLshort * coords);
extern PFN_GLVERTEXSTREAM2SVATI _glVertexStream2svATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2IATI)(GLenum stream, GLint x, GLint y);
extern PFN_GLVERTEXSTREAM2IATI _glVertexStream2iATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2IVATI)(GLenum stream, const GLint * coords);
extern PFN_GLVERTEXSTREAM2IVATI _glVertexStream2ivATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2FATI)(GLenum stream, GLfloat x, GLfloat y);
extern PFN_GLVERTEXSTREAM2FATI _glVertexStream2fATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2FVATI)(GLenum stream, const GLfloat * coords);
extern PFN_GLVERTEXSTREAM2FVATI _glVertexStream2fvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2DATI)(GLenum stream, GLdouble x, GLdouble y);
extern PFN_GLVERTEXSTREAM2DATI _glVertexStream2dATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM2DVATI)(GLenum stream, const GLdouble * coords);
extern PFN_GLVERTEXSTREAM2DVATI _glVertexStream2dvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3SATI)(GLenum stream, GLshort x, GLshort y, GLshort z);
extern PFN_GLVERTEXSTREAM3SATI _glVertexStream3sATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3SVATI)(GLenum stream, const GLshort * coords);
extern PFN_GLVERTEXSTREAM3SVATI _glVertexStream3svATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3IATI)(GLenum stream, GLint x, GLint y, GLint z);
extern PFN_GLVERTEXSTREAM3IATI _glVertexStream3iATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3IVATI)(GLenum stream, const GLint * coords);
extern PFN_GLVERTEXSTREAM3IVATI _glVertexStream3ivATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3FATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLVERTEXSTREAM3FATI _glVertexStream3fATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3FVATI)(GLenum stream, const GLfloat * coords);
extern PFN_GLVERTEXSTREAM3FVATI _glVertexStream3fvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3DATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXSTREAM3DATI _glVertexStream3dATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM3DVATI)(GLenum stream, const GLdouble * coords);
extern PFN_GLVERTEXSTREAM3DVATI _glVertexStream3dvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4SATI)(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLVERTEXSTREAM4SATI _glVertexStream4sATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4SVATI)(GLenum stream, const GLshort * coords);
extern PFN_GLVERTEXSTREAM4SVATI _glVertexStream4svATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4IATI)(GLenum stream, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLVERTEXSTREAM4IATI _glVertexStream4iATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4IVATI)(GLenum stream, const GLint * coords);
extern PFN_GLVERTEXSTREAM4IVATI _glVertexStream4ivATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4FATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLVERTEXSTREAM4FATI _glVertexStream4fATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4FVATI)(GLenum stream, const GLfloat * coords);
extern PFN_GLVERTEXSTREAM4FVATI _glVertexStream4fvATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4DATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXSTREAM4DATI _glVertexStream4dATI;

typedef void (APIENTRY * PFN_GLVERTEXSTREAM4DVATI)(GLenum stream, const GLdouble * coords);
extern PFN_GLVERTEXSTREAM4DVATI _glVertexStream4dvATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3BATI)(GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz);
extern PFN_GLNORMALSTREAM3BATI _glNormalStream3bATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3BVATI)(GLenum stream, const GLbyte * coords);
extern PFN_GLNORMALSTREAM3BVATI _glNormalStream3bvATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3SATI)(GLenum stream, GLshort nx, GLshort ny, GLshort nz);
extern PFN_GLNORMALSTREAM3SATI _glNormalStream3sATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3SVATI)(GLenum stream, const GLshort * coords);
extern PFN_GLNORMALSTREAM3SVATI _glNormalStream3svATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3IATI)(GLenum stream, GLint nx, GLint ny, GLint nz);
extern PFN_GLNORMALSTREAM3IATI _glNormalStream3iATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3IVATI)(GLenum stream, const GLint * coords);
extern PFN_GLNORMALSTREAM3IVATI _glNormalStream3ivATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3FATI)(GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz);
extern PFN_GLNORMALSTREAM3FATI _glNormalStream3fATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3FVATI)(GLenum stream, const GLfloat * coords);
extern PFN_GLNORMALSTREAM3FVATI _glNormalStream3fvATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3DATI)(GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz);
extern PFN_GLNORMALSTREAM3DATI _glNormalStream3dATI;

typedef void (APIENTRY * PFN_GLNORMALSTREAM3DVATI)(GLenum stream, const GLdouble * coords);
extern PFN_GLNORMALSTREAM3DVATI _glNormalStream3dvATI;

typedef void (APIENTRY * PFN_GLCLIENTACTIVEVERTEXSTREAMATI)(GLenum stream);
extern PFN_GLCLIENTACTIVEVERTEXSTREAMATI _glClientActiveVertexStreamATI;

typedef void (APIENTRY * PFN_GLVERTEXBLENDENVIATI)(GLenum pname, GLint param);
extern PFN_GLVERTEXBLENDENVIATI _glVertexBlendEnviATI;

typedef void (APIENTRY * PFN_GLVERTEXBLENDENVFATI)(GLenum pname, GLfloat param);
extern PFN_GLVERTEXBLENDENVFATI _glVertexBlendEnvfATI;

typedef void (APIENTRY * PFN_GLUNIFORMBUFFEREXT)(GLuint program, GLint location, GLuint buffer);
extern PFN_GLUNIFORMBUFFEREXT _glUniformBufferEXT;

typedef GLint (APIENTRY * PFN_GLGETUNIFORMBUFFERSIZEEXT)(GLuint program, GLint location);
extern PFN_GLGETUNIFORMBUFFERSIZEEXT _glGetUniformBufferSizeEXT;

typedef GLintptr (APIENTRY * PFN_GLGETUNIFORMOFFSETEXT)(GLuint program, GLint location);
extern PFN_GLGETUNIFORMOFFSETEXT _glGetUniformOffsetEXT;

typedef void (APIENTRY * PFN_GLBLENDCOLOREXT)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFN_GLBLENDCOLOREXT _glBlendColorEXT;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONSEPARATEEXT)(GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATEEXT _glBlendEquationSeparateEXT;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATEEXT)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern PFN_GLBLENDFUNCSEPARATEEXT _glBlendFuncSeparateEXT;

typedef void (APIENTRY * PFN_GLBLENDEQUATIONEXT)(GLenum mode);
extern PFN_GLBLENDEQUATIONEXT _glBlendEquationEXT;

typedef void (APIENTRY * PFN_GLCOLORSUBTABLEEXT)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data);
extern PFN_GLCOLORSUBTABLEEXT _glColorSubTableEXT;

typedef void (APIENTRY * PFN_GLCOPYCOLORSUBTABLEEXT)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCOLORSUBTABLEEXT _glCopyColorSubTableEXT;

typedef void (APIENTRY * PFN_GLLOCKARRAYSEXT)(GLint first, GLsizei count);
extern PFN_GLLOCKARRAYSEXT _glLockArraysEXT;

typedef void (APIENTRY * PFN_GLUNLOCKARRAYSEXT)(void);
extern PFN_GLUNLOCKARRAYSEXT _glUnlockArraysEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONFILTER1DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image);
extern PFN_GLCONVOLUTIONFILTER1DEXT _glConvolutionFilter1DEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONFILTER2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image);
extern PFN_GLCONVOLUTIONFILTER2DEXT _glConvolutionFilter2DEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERFEXT)(GLenum target, GLenum pname, GLfloat params);
extern PFN_GLCONVOLUTIONPARAMETERFEXT _glConvolutionParameterfEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERFVEXT)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLCONVOLUTIONPARAMETERFVEXT _glConvolutionParameterfvEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERIEXT)(GLenum target, GLenum pname, GLint params);
extern PFN_GLCONVOLUTIONPARAMETERIEXT _glConvolutionParameteriEXT;

typedef void (APIENTRY * PFN_GLCONVOLUTIONPARAMETERIVEXT)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLCONVOLUTIONPARAMETERIVEXT _glConvolutionParameterivEXT;

typedef void (APIENTRY * PFN_GLCOPYCONVOLUTIONFILTER1DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCONVOLUTIONFILTER1DEXT _glCopyConvolutionFilter1DEXT;

typedef void (APIENTRY * PFN_GLCOPYCONVOLUTIONFILTER2DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYCONVOLUTIONFILTER2DEXT _glCopyConvolutionFilter2DEXT;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONFILTEREXT)(GLenum target, GLenum format, GLenum type, GLvoid * image);
extern PFN_GLGETCONVOLUTIONFILTEREXT _glGetConvolutionFilterEXT;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONPARAMETERFVEXT)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETCONVOLUTIONPARAMETERFVEXT _glGetConvolutionParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETCONVOLUTIONPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETCONVOLUTIONPARAMETERIVEXT _glGetConvolutionParameterivEXT;

typedef void (APIENTRY * PFN_GLGETSEPARABLEFILTEREXT)(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span);
extern PFN_GLGETSEPARABLEFILTEREXT _glGetSeparableFilterEXT;

typedef void (APIENTRY * PFN_GLSEPARABLEFILTER2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column);
extern PFN_GLSEPARABLEFILTER2DEXT _glSeparableFilter2DEXT;

typedef void (APIENTRY * PFN_GLTANGENT3BEXT)(GLbyte tx, GLbyte ty, GLbyte tz);
extern PFN_GLTANGENT3BEXT _glTangent3bEXT;

typedef void (APIENTRY * PFN_GLTANGENT3BVEXT)(const GLbyte * v);
extern PFN_GLTANGENT3BVEXT _glTangent3bvEXT;

typedef void (APIENTRY * PFN_GLTANGENT3DEXT)(GLdouble tx, GLdouble ty, GLdouble tz);
extern PFN_GLTANGENT3DEXT _glTangent3dEXT;

typedef void (APIENTRY * PFN_GLTANGENT3DVEXT)(const GLdouble * v);
extern PFN_GLTANGENT3DVEXT _glTangent3dvEXT;

typedef void (APIENTRY * PFN_GLTANGENT3FEXT)(GLfloat tx, GLfloat ty, GLfloat tz);
extern PFN_GLTANGENT3FEXT _glTangent3fEXT;

typedef void (APIENTRY * PFN_GLTANGENT3FVEXT)(const GLfloat * v);
extern PFN_GLTANGENT3FVEXT _glTangent3fvEXT;

typedef void (APIENTRY * PFN_GLTANGENT3IEXT)(GLint tx, GLint ty, GLint tz);
extern PFN_GLTANGENT3IEXT _glTangent3iEXT;

typedef void (APIENTRY * PFN_GLTANGENT3IVEXT)(const GLint * v);
extern PFN_GLTANGENT3IVEXT _glTangent3ivEXT;

typedef void (APIENTRY * PFN_GLTANGENT3SEXT)(GLshort tx, GLshort ty, GLshort tz);
extern PFN_GLTANGENT3SEXT _glTangent3sEXT;

typedef void (APIENTRY * PFN_GLTANGENT3SVEXT)(const GLshort * v);
extern PFN_GLTANGENT3SVEXT _glTangent3svEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3BEXT)(GLbyte bx, GLbyte by, GLbyte bz);
extern PFN_GLBINORMAL3BEXT _glBinormal3bEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3BVEXT)(const GLbyte * v);
extern PFN_GLBINORMAL3BVEXT _glBinormal3bvEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3DEXT)(GLdouble bx, GLdouble by, GLdouble bz);
extern PFN_GLBINORMAL3DEXT _glBinormal3dEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3DVEXT)(const GLdouble * v);
extern PFN_GLBINORMAL3DVEXT _glBinormal3dvEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3FEXT)(GLfloat bx, GLfloat by, GLfloat bz);
extern PFN_GLBINORMAL3FEXT _glBinormal3fEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3FVEXT)(const GLfloat * v);
extern PFN_GLBINORMAL3FVEXT _glBinormal3fvEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3IEXT)(GLint bx, GLint by, GLint bz);
extern PFN_GLBINORMAL3IEXT _glBinormal3iEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3IVEXT)(const GLint * v);
extern PFN_GLBINORMAL3IVEXT _glBinormal3ivEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3SEXT)(GLshort bx, GLshort by, GLshort bz);
extern PFN_GLBINORMAL3SEXT _glBinormal3sEXT;

typedef void (APIENTRY * PFN_GLBINORMAL3SVEXT)(const GLshort * v);
extern PFN_GLBINORMAL3SVEXT _glBinormal3svEXT;

typedef void (APIENTRY * PFN_GLTANGENTPOINTEREXT)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLTANGENTPOINTEREXT _glTangentPointerEXT;

typedef void (APIENTRY * PFN_GLBINORMALPOINTEREXT)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLBINORMALPOINTEREXT _glBinormalPointerEXT;

typedef void (APIENTRY * PFN_GLCOPYIMAGESUBDATAEXT)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
extern PFN_GLCOPYIMAGESUBDATAEXT _glCopyImageSubDataEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXIMAGE1DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern PFN_GLCOPYTEXIMAGE1DEXT _glCopyTexImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXIMAGE2DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern PFN_GLCOPYTEXIMAGE2DEXT _glCopyTexImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE1DEXT)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYTEXSUBIMAGE1DEXT _glCopyTexSubImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXSUBIMAGE2DEXT _glCopyTexSubImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXSUBIMAGE3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXSUBIMAGE3DEXT _glCopyTexSubImage3DEXT;

typedef void (APIENTRY * PFN_GLCULLPARAMETERDVEXT)(GLenum pname, GLdouble * params);
extern PFN_GLCULLPARAMETERDVEXT _glCullParameterdvEXT;

typedef void (APIENTRY * PFN_GLCULLPARAMETERFVEXT)(GLenum pname, GLfloat * params);
extern PFN_GLCULLPARAMETERFVEXT _glCullParameterfvEXT;

typedef void (APIENTRY * PFN_GLLABELOBJECTEXT)(GLenum type, GLuint object, GLsizei length, const GLchar * label);
extern PFN_GLLABELOBJECTEXT _glLabelObjectEXT;

typedef void (APIENTRY * PFN_GLGETOBJECTLABELEXT)(GLenum type, GLuint object, GLsizei bufSize, GLsizei * length, GLchar * label);
extern PFN_GLGETOBJECTLABELEXT _glGetObjectLabelEXT;

typedef void (APIENTRY * PFN_GLINSERTEVENTMARKEREXT)(GLsizei length, const GLchar * marker);
extern PFN_GLINSERTEVENTMARKEREXT _glInsertEventMarkerEXT;

typedef void (APIENTRY * PFN_GLPUSHGROUPMARKEREXT)(GLsizei length, const GLchar * marker);
extern PFN_GLPUSHGROUPMARKEREXT _glPushGroupMarkerEXT;

typedef void (APIENTRY * PFN_GLPOPGROUPMARKEREXT)(void);
extern PFN_GLPOPGROUPMARKEREXT _glPopGroupMarkerEXT;

typedef void (APIENTRY * PFN_GLDEPTHBOUNDSEXT)(GLclampd zmin, GLclampd zmax);
extern PFN_GLDEPTHBOUNDSEXT _glDepthBoundsEXT;

typedef void (APIENTRY * PFN_GLCLIENTATTRIBDEFAULTEXT)(GLbitfield mask);
extern PFN_GLCLIENTATTRIBDEFAULTEXT _glClientAttribDefaultEXT;

typedef void (APIENTRY * PFN_GLPUSHCLIENTATTRIBDEFAULTEXT)(GLbitfield mask);
extern PFN_GLPUSHCLIENTATTRIBDEFAULTEXT _glPushClientAttribDefaultEXT;

typedef void (APIENTRY * PFN_GLMATRIXLOADFEXT)(GLenum mode, const GLfloat * m);
extern PFN_GLMATRIXLOADFEXT _glMatrixLoadfEXT;

typedef void (APIENTRY * PFN_GLMATRIXLOADDEXT)(GLenum mode, const GLdouble * m);
extern PFN_GLMATRIXLOADDEXT _glMatrixLoaddEXT;

typedef void (APIENTRY * PFN_GLMATRIXMULTFEXT)(GLenum mode, const GLfloat * m);
extern PFN_GLMATRIXMULTFEXT _glMatrixMultfEXT;

typedef void (APIENTRY * PFN_GLMATRIXMULTDEXT)(GLenum mode, const GLdouble * m);
extern PFN_GLMATRIXMULTDEXT _glMatrixMultdEXT;

typedef void (APIENTRY * PFN_GLMATRIXLOADIDENTITYEXT)(GLenum mode);
extern PFN_GLMATRIXLOADIDENTITYEXT _glMatrixLoadIdentityEXT;

typedef void (APIENTRY * PFN_GLMATRIXROTATEFEXT)(GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLMATRIXROTATEFEXT _glMatrixRotatefEXT;

typedef void (APIENTRY * PFN_GLMATRIXROTATEDEXT)(GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLMATRIXROTATEDEXT _glMatrixRotatedEXT;

typedef void (APIENTRY * PFN_GLMATRIXSCALEFEXT)(GLenum mode, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLMATRIXSCALEFEXT _glMatrixScalefEXT;

typedef void (APIENTRY * PFN_GLMATRIXSCALEDEXT)(GLenum mode, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLMATRIXSCALEDEXT _glMatrixScaledEXT;

typedef void (APIENTRY * PFN_GLMATRIXTRANSLATEFEXT)(GLenum mode, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLMATRIXTRANSLATEFEXT _glMatrixTranslatefEXT;

typedef void (APIENTRY * PFN_GLMATRIXTRANSLATEDEXT)(GLenum mode, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLMATRIXTRANSLATEDEXT _glMatrixTranslatedEXT;

typedef void (APIENTRY * PFN_GLMATRIXFRUSTUMEXT)(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern PFN_GLMATRIXFRUSTUMEXT _glMatrixFrustumEXT;

typedef void (APIENTRY * PFN_GLMATRIXORTHOEXT)(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern PFN_GLMATRIXORTHOEXT _glMatrixOrthoEXT;

typedef void (APIENTRY * PFN_GLMATRIXPOPEXT)(GLenum mode);
extern PFN_GLMATRIXPOPEXT _glMatrixPopEXT;

typedef void (APIENTRY * PFN_GLMATRIXPUSHEXT)(GLenum mode);
extern PFN_GLMATRIXPUSHEXT _glMatrixPushEXT;

typedef void (APIENTRY * PFN_GLMATRIXLOADTRANSPOSEFEXT)(GLenum mode, const GLfloat * m);
extern PFN_GLMATRIXLOADTRANSPOSEFEXT _glMatrixLoadTransposefEXT;

typedef void (APIENTRY * PFN_GLMATRIXLOADTRANSPOSEDEXT)(GLenum mode, const GLdouble * m);
extern PFN_GLMATRIXLOADTRANSPOSEDEXT _glMatrixLoadTransposedEXT;

typedef void (APIENTRY * PFN_GLMATRIXMULTTRANSPOSEFEXT)(GLenum mode, const GLfloat * m);
extern PFN_GLMATRIXMULTTRANSPOSEFEXT _glMatrixMultTransposefEXT;

typedef void (APIENTRY * PFN_GLMATRIXMULTTRANSPOSEDEXT)(GLenum mode, const GLdouble * m);
extern PFN_GLMATRIXMULTTRANSPOSEDEXT _glMatrixMultTransposedEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERFEXT)(GLuint texture, GLenum target, GLenum pname, GLfloat param);
extern PFN_GLTEXTUREPARAMETERFEXT _glTextureParameterfEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERFVEXT)(GLuint texture, GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLTEXTUREPARAMETERFVEXT _glTextureParameterfvEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIEXT)(GLuint texture, GLenum target, GLenum pname, GLint param);
extern PFN_GLTEXTUREPARAMETERIEXT _glTextureParameteriEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIVEXT)(GLuint texture, GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXTUREPARAMETERIVEXT _glTextureParameterivEXT;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTUREIMAGE1DEXT _glTextureImage1DEXT;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTUREIMAGE2DEXT _glTextureImage2DEXT;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTURESUBIMAGE1DEXT _glTextureSubImage1DEXT;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTURESUBIMAGE2DEXT _glTextureSubImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXTUREIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern PFN_GLCOPYTEXTUREIMAGE1DEXT _glCopyTextureImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXTUREIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern PFN_GLCOPYTEXTUREIMAGE2DEXT _glCopyTextureImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYTEXTURESUBIMAGE1DEXT _glCopyTextureSubImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXTURESUBIMAGE2DEXT _glCopyTextureSubImage2DEXT;

typedef void (APIENTRY * PFN_GLGETTEXTUREIMAGEEXT)(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
extern PFN_GLGETTEXTUREIMAGEEXT _glGetTextureImageEXT;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERFVEXT)(GLuint texture, GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXTUREPARAMETERFVEXT _glGetTextureParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIVEXT)(GLuint texture, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXTUREPARAMETERIVEXT _glGetTextureParameterivEXT;

typedef void (APIENTRY * PFN_GLGETTEXTURELEVELPARAMETERFVEXT)(GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXTURELEVELPARAMETERFVEXT _glGetTextureLevelParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETTEXTURELEVELPARAMETERIVEXT)(GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params);
extern PFN_GLGETTEXTURELEVELPARAMETERIVEXT _glGetTextureLevelParameterivEXT;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE3DEXT)(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTUREIMAGE3DEXT _glTextureImage3DEXT;

typedef void (APIENTRY * PFN_GLTEXTURESUBIMAGE3DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXTURESUBIMAGE3DEXT _glTextureSubImage3DEXT;

typedef void (APIENTRY * PFN_GLCOPYTEXTURESUBIMAGE3DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXTURESUBIMAGE3DEXT _glCopyTextureSubImage3DEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERFEXT)(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
extern PFN_GLMULTITEXPARAMETERFEXT _glMultiTexParameterfEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERFVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLMULTITEXPARAMETERFVEXT _glMultiTexParameterfvEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERIEXT)(GLenum texunit, GLenum target, GLenum pname, GLint param);
extern PFN_GLMULTITEXPARAMETERIEXT _glMultiTexParameteriEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERIVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLint * params);
extern PFN_GLMULTITEXPARAMETERIVEXT _glMultiTexParameterivEXT;

typedef void (APIENTRY * PFN_GLMULTITEXIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXIMAGE1DEXT _glMultiTexImage1DEXT;

typedef void (APIENTRY * PFN_GLMULTITEXIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXIMAGE2DEXT _glMultiTexImage2DEXT;

typedef void (APIENTRY * PFN_GLMULTITEXSUBIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXSUBIMAGE1DEXT _glMultiTexSubImage1DEXT;

typedef void (APIENTRY * PFN_GLMULTITEXSUBIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXSUBIMAGE2DEXT _glMultiTexSubImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYMULTITEXIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern PFN_GLCOPYMULTITEXIMAGE1DEXT _glCopyMultiTexImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYMULTITEXIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern PFN_GLCOPYMULTITEXIMAGE2DEXT _glCopyMultiTexImage2DEXT;

typedef void (APIENTRY * PFN_GLCOPYMULTITEXSUBIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYMULTITEXSUBIMAGE1DEXT _glCopyMultiTexSubImage1DEXT;

typedef void (APIENTRY * PFN_GLCOPYMULTITEXSUBIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYMULTITEXSUBIMAGE2DEXT _glCopyMultiTexSubImage2DEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXIMAGEEXT)(GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
extern PFN_GLGETMULTITEXIMAGEEXT _glGetMultiTexImageEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXPARAMETERFVEXT)(GLenum texunit, GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETMULTITEXPARAMETERFVEXT _glGetMultiTexParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXPARAMETERIVEXT)(GLenum texunit, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMULTITEXPARAMETERIVEXT _glGetMultiTexParameterivEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXLEVELPARAMETERFVEXT)(GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat * params);
extern PFN_GLGETMULTITEXLEVELPARAMETERFVEXT _glGetMultiTexLevelParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXLEVELPARAMETERIVEXT)(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint * params);
extern PFN_GLGETMULTITEXLEVELPARAMETERIVEXT _glGetMultiTexLevelParameterivEXT;

typedef void (APIENTRY * PFN_GLMULTITEXIMAGE3DEXT)(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXIMAGE3DEXT _glMultiTexImage3DEXT;

typedef void (APIENTRY * PFN_GLMULTITEXSUBIMAGE3DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLMULTITEXSUBIMAGE3DEXT _glMultiTexSubImage3DEXT;

typedef void (APIENTRY * PFN_GLCOPYMULTITEXSUBIMAGE3DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYMULTITEXSUBIMAGE3DEXT _glCopyMultiTexSubImage3DEXT;

typedef void (APIENTRY * PFN_GLBINDMULTITEXTUREEXT)(GLenum texunit, GLenum target, GLuint texture);
extern PFN_GLBINDMULTITEXTUREEXT _glBindMultiTextureEXT;

typedef void (APIENTRY * PFN_GLENABLECLIENTSTATEINDEXEDEXT)(GLenum array, GLuint index);
extern PFN_GLENABLECLIENTSTATEINDEXEDEXT _glEnableClientStateIndexedEXT;

typedef void (APIENTRY * PFN_GLDISABLECLIENTSTATEINDEXEDEXT)(GLenum array, GLuint index);
extern PFN_GLDISABLECLIENTSTATEINDEXEDEXT _glDisableClientStateIndexedEXT;

typedef void (APIENTRY * PFN_GLENABLECLIENTSTATEIEXT)(GLenum array, GLuint index);
extern PFN_GLENABLECLIENTSTATEIEXT _glEnableClientStateiEXT;

typedef void (APIENTRY * PFN_GLDISABLECLIENTSTATEIEXT)(GLenum array, GLuint index);
extern PFN_GLDISABLECLIENTSTATEIEXT _glDisableClientStateiEXT;

typedef void (APIENTRY * PFN_GLMULTITEXCOORDPOINTEREXT)(GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLMULTITEXCOORDPOINTEREXT _glMultiTexCoordPointerEXT;

typedef void (APIENTRY * PFN_GLMULTITEXENVFEXT)(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
extern PFN_GLMULTITEXENVFEXT _glMultiTexEnvfEXT;

typedef void (APIENTRY * PFN_GLMULTITEXENVFVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLMULTITEXENVFVEXT _glMultiTexEnvfvEXT;

typedef void (APIENTRY * PFN_GLMULTITEXENVIEXT)(GLenum texunit, GLenum target, GLenum pname, GLint param);
extern PFN_GLMULTITEXENVIEXT _glMultiTexEnviEXT;

typedef void (APIENTRY * PFN_GLMULTITEXENVIVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLint * params);
extern PFN_GLMULTITEXENVIVEXT _glMultiTexEnvivEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENDEXT)(GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
extern PFN_GLMULTITEXGENDEXT _glMultiTexGendEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENDVEXT)(GLenum texunit, GLenum coord, GLenum pname, const GLdouble * params);
extern PFN_GLMULTITEXGENDVEXT _glMultiTexGendvEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENFEXT)(GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
extern PFN_GLMULTITEXGENFEXT _glMultiTexGenfEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENFVEXT)(GLenum texunit, GLenum coord, GLenum pname, const GLfloat * params);
extern PFN_GLMULTITEXGENFVEXT _glMultiTexGenfvEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENIEXT)(GLenum texunit, GLenum coord, GLenum pname, GLint param);
extern PFN_GLMULTITEXGENIEXT _glMultiTexGeniEXT;

typedef void (APIENTRY * PFN_GLMULTITEXGENIVEXT)(GLenum texunit, GLenum coord, GLenum pname, const GLint * params);
extern PFN_GLMULTITEXGENIVEXT _glMultiTexGenivEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXENVFVEXT)(GLenum texunit, GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETMULTITEXENVFVEXT _glGetMultiTexEnvfvEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXENVIVEXT)(GLenum texunit, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMULTITEXENVIVEXT _glGetMultiTexEnvivEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXGENDVEXT)(GLenum texunit, GLenum coord, GLenum pname, GLdouble * params);
extern PFN_GLGETMULTITEXGENDVEXT _glGetMultiTexGendvEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXGENFVEXT)(GLenum texunit, GLenum coord, GLenum pname, GLfloat * params);
extern PFN_GLGETMULTITEXGENFVEXT _glGetMultiTexGenfvEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXGENIVEXT)(GLenum texunit, GLenum coord, GLenum pname, GLint * params);
extern PFN_GLGETMULTITEXGENIVEXT _glGetMultiTexGenivEXT;

typedef void (APIENTRY * PFN_GLGETFLOATINDEXEDVEXT)(GLenum target, GLuint index, GLfloat * data);
extern PFN_GLGETFLOATINDEXEDVEXT _glGetFloatIndexedvEXT;

typedef void (APIENTRY * PFN_GLGETDOUBLEINDEXEDVEXT)(GLenum target, GLuint index, GLdouble * data);
extern PFN_GLGETDOUBLEINDEXEDVEXT _glGetDoubleIndexedvEXT;

typedef void (APIENTRY * PFN_GLGETPOINTERINDEXEDVEXT)(GLenum target, GLuint index, GLvoid * * data);
extern PFN_GLGETPOINTERINDEXEDVEXT _glGetPointerIndexedvEXT;

typedef void (APIENTRY * PFN_GLGETFLOATI_VEXT)(GLenum target, GLuint index, GLfloat * data);
extern PFN_GLGETFLOATI_VEXT _glGetFloati_vEXT;

typedef void (APIENTRY * PFN_GLGETDOUBLEI_VEXT)(GLenum target, GLuint index, GLdouble * data);
extern PFN_GLGETDOUBLEI_VEXT _glGetDoublei_vEXT;

typedef void (APIENTRY * PFN_GLGETPOINTERI_VEXT)(GLenum target, GLuint index, GLvoid * * data);
extern PFN_GLGETPOINTERI_VEXT _glGetPointeri_vEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTUREIMAGE3DEXT)(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTUREIMAGE3DEXT _glCompressedTextureImage3DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTUREIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTUREIMAGE2DEXT _glCompressedTextureImage2DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTUREIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTUREIMAGE1DEXT _glCompressedTextureImage1DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE3DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE3DEXT _glCompressedTextureSubImage3DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE2DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE2DEXT _glCompressedTextureSubImage2DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDTEXTURESUBIMAGE1DEXT)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDTEXTURESUBIMAGE1DEXT _glCompressedTextureSubImage1DEXT;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDTEXTUREIMAGEEXT)(GLuint texture, GLenum target, GLint lod, GLvoid * img);
extern PFN_GLGETCOMPRESSEDTEXTUREIMAGEEXT _glGetCompressedTextureImageEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXIMAGE3DEXT)(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXIMAGE3DEXT _glCompressedMultiTexImage3DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXIMAGE2DEXT _glCompressedMultiTexImage2DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXIMAGE1DEXT _glCompressedMultiTexImage1DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXSUBIMAGE3DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXSUBIMAGE3DEXT _glCompressedMultiTexSubImage3DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXSUBIMAGE2DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXSUBIMAGE2DEXT _glCompressedMultiTexSubImage2DEXT;

typedef void (APIENTRY * PFN_GLCOMPRESSEDMULTITEXSUBIMAGE1DEXT)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * bits);
extern PFN_GLCOMPRESSEDMULTITEXSUBIMAGE1DEXT _glCompressedMultiTexSubImage1DEXT;

typedef void (APIENTRY * PFN_GLGETCOMPRESSEDMULTITEXIMAGEEXT)(GLenum texunit, GLenum target, GLint lod, GLvoid * img);
extern PFN_GLGETCOMPRESSEDMULTITEXIMAGEEXT _glGetCompressedMultiTexImageEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMSTRINGEXT)(GLuint program, GLenum target, GLenum format, GLsizei len, const GLvoid * string);
extern PFN_GLNAMEDPROGRAMSTRINGEXT _glNamedProgramStringEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETER4DEXT)(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETER4DEXT _glNamedProgramLocalParameter4dEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETER4DVEXT)(GLuint program, GLenum target, GLuint index, const GLdouble * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETER4DVEXT _glNamedProgramLocalParameter4dvEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETER4FEXT)(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETER4FEXT _glNamedProgramLocalParameter4fEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETER4FVEXT)(GLuint program, GLenum target, GLuint index, const GLfloat * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETER4FVEXT _glNamedProgramLocalParameter4fvEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMLOCALPARAMETERDVEXT)(GLuint program, GLenum target, GLuint index, GLdouble * params);
extern PFN_GLGETNAMEDPROGRAMLOCALPARAMETERDVEXT _glGetNamedProgramLocalParameterdvEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMLOCALPARAMETERFVEXT)(GLuint program, GLenum target, GLuint index, GLfloat * params);
extern PFN_GLGETNAMEDPROGRAMLOCALPARAMETERFVEXT _glGetNamedProgramLocalParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMIVEXT)(GLuint program, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDPROGRAMIVEXT _glGetNamedProgramivEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMSTRINGEXT)(GLuint program, GLenum target, GLenum pname, GLvoid * string);
extern PFN_GLGETNAMEDPROGRAMSTRINGEXT _glGetNamedProgramStringEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERS4FVEXT)(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERS4FVEXT _glNamedProgramLocalParameters4fvEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERI4IEXT)(GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERI4IEXT _glNamedProgramLocalParameterI4iEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERI4IVEXT)(GLuint program, GLenum target, GLuint index, const GLint * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERI4IVEXT _glNamedProgramLocalParameterI4ivEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT)(GLuint program, GLenum target, GLuint index, GLsizei count, const GLint * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT _glNamedProgramLocalParametersI4ivEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERI4UIEXT)(GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERI4UIEXT _glNamedProgramLocalParameterI4uiEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT)(GLuint program, GLenum target, GLuint index, const GLuint * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT _glNamedProgramLocalParameterI4uivEXT;

typedef void (APIENTRY * PFN_GLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT)(GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint * params);
extern PFN_GLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT _glNamedProgramLocalParametersI4uivEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT)(GLuint program, GLenum target, GLuint index, GLint * params);
extern PFN_GLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT _glGetNamedProgramLocalParameterIivEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT)(GLuint program, GLenum target, GLuint index, GLuint * params);
extern PFN_GLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT _glGetNamedProgramLocalParameterIuivEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIIVEXT)(GLuint texture, GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXTUREPARAMETERIIVEXT _glTextureParameterIivEXT;

typedef void (APIENTRY * PFN_GLTEXTUREPARAMETERIUIVEXT)(GLuint texture, GLenum target, GLenum pname, const GLuint * params);
extern PFN_GLTEXTUREPARAMETERIUIVEXT _glTextureParameterIuivEXT;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIIVEXT)(GLuint texture, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXTUREPARAMETERIIVEXT _glGetTextureParameterIivEXT;

typedef void (APIENTRY * PFN_GLGETTEXTUREPARAMETERIUIVEXT)(GLuint texture, GLenum target, GLenum pname, GLuint * params);
extern PFN_GLGETTEXTUREPARAMETERIUIVEXT _glGetTextureParameterIuivEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERIIVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLint * params);
extern PFN_GLMULTITEXPARAMETERIIVEXT _glMultiTexParameterIivEXT;

typedef void (APIENTRY * PFN_GLMULTITEXPARAMETERIUIVEXT)(GLenum texunit, GLenum target, GLenum pname, const GLuint * params);
extern PFN_GLMULTITEXPARAMETERIUIVEXT _glMultiTexParameterIuivEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXPARAMETERIIVEXT)(GLenum texunit, GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMULTITEXPARAMETERIIVEXT _glGetMultiTexParameterIivEXT;

typedef void (APIENTRY * PFN_GLGETMULTITEXPARAMETERIUIVEXT)(GLenum texunit, GLenum target, GLenum pname, GLuint * params);
extern PFN_GLGETMULTITEXPARAMETERIUIVEXT _glGetMultiTexParameterIuivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1FEXT)(GLuint program, GLint location, GLfloat v0);
extern PFN_GLPROGRAMUNIFORM1FEXT _glProgramUniform1fEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2FEXT)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
extern PFN_GLPROGRAMUNIFORM2FEXT _glProgramUniform2fEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3FEXT)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFN_GLPROGRAMUNIFORM3FEXT _glProgramUniform3fEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4FEXT)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFN_GLPROGRAMUNIFORM4FEXT _glProgramUniform4fEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1IEXT)(GLuint program, GLint location, GLint v0);
extern PFN_GLPROGRAMUNIFORM1IEXT _glProgramUniform1iEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2IEXT)(GLuint program, GLint location, GLint v0, GLint v1);
extern PFN_GLPROGRAMUNIFORM2IEXT _glProgramUniform2iEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3IEXT)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
extern PFN_GLPROGRAMUNIFORM3IEXT _glProgramUniform3iEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4IEXT)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern PFN_GLPROGRAMUNIFORM4IEXT _glProgramUniform4iEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1FVEXT)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM1FVEXT _glProgramUniform1fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2FVEXT)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM2FVEXT _glProgramUniform2fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3FVEXT)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM3FVEXT _glProgramUniform3fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4FVEXT)(GLuint program, GLint location, GLsizei count, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORM4FVEXT _glProgramUniform4fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1IVEXT)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM1IVEXT _glProgramUniform1ivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2IVEXT)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM2IVEXT _glProgramUniform2ivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3IVEXT)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM3IVEXT _glProgramUniform3ivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4IVEXT)(GLuint program, GLint location, GLsizei count, const GLint * value);
extern PFN_GLPROGRAMUNIFORM4IVEXT _glProgramUniform4ivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2FVEXT _glProgramUniformMatrix2fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3FVEXT _glProgramUniformMatrix3fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4FVEXT _glProgramUniformMatrix4fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X3FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X3FVEXT _glProgramUniformMatrix2x3fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X2FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X2FVEXT _glProgramUniformMatrix3x2fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X4FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X4FVEXT _glProgramUniformMatrix2x4fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X2FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X2FVEXT _glProgramUniformMatrix4x2fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X4FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X4FVEXT _glProgramUniformMatrix3x4fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X3FVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X3FVEXT _glProgramUniformMatrix4x3fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UIEXT)(GLuint program, GLint location, GLuint v0);
extern PFN_GLPROGRAMUNIFORM1UIEXT _glProgramUniform1uiEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UIEXT)(GLuint program, GLint location, GLuint v0, GLuint v1);
extern PFN_GLPROGRAMUNIFORM2UIEXT _glProgramUniform2uiEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UIEXT)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
extern PFN_GLPROGRAMUNIFORM3UIEXT _glProgramUniform3uiEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UIEXT)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern PFN_GLPROGRAMUNIFORM4UIEXT _glProgramUniform4uiEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UIVEXT)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM1UIVEXT _glProgramUniform1uivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UIVEXT)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM2UIVEXT _glProgramUniform2uivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UIVEXT)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM3UIVEXT _glProgramUniform3uivEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UIVEXT)(GLuint program, GLint location, GLsizei count, const GLuint * value);
extern PFN_GLPROGRAMUNIFORM4UIVEXT _glProgramUniform4uivEXT;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERDATAEXT)(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
extern PFN_GLNAMEDBUFFERDATAEXT _glNamedBufferDataEXT;

typedef void (APIENTRY * PFN_GLNAMEDBUFFERSUBDATAEXT)(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
extern PFN_GLNAMEDBUFFERSUBDATAEXT _glNamedBufferSubDataEXT;

typedef GLvoid * (APIENTRY * PFN_GLMAPNAMEDBUFFEREXT)(GLuint buffer, GLenum access);
extern PFN_GLMAPNAMEDBUFFEREXT _glMapNamedBufferEXT;

typedef GLboolean (APIENTRY * PFN_GLUNMAPNAMEDBUFFEREXT)(GLuint buffer);
extern PFN_GLUNMAPNAMEDBUFFEREXT _glUnmapNamedBufferEXT;

typedef GLvoid * (APIENTRY * PFN_GLMAPNAMEDBUFFERRANGEEXT)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern PFN_GLMAPNAMEDBUFFERRANGEEXT _glMapNamedBufferRangeEXT;

typedef void (APIENTRY * PFN_GLFLUSHMAPPEDNAMEDBUFFERRANGEEXT)(GLuint buffer, GLintptr offset, GLsizeiptr length);
extern PFN_GLFLUSHMAPPEDNAMEDBUFFERRANGEEXT _glFlushMappedNamedBufferRangeEXT;

typedef void (APIENTRY * PFN_GLNAMEDCOPYBUFFERSUBDATAEXT)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern PFN_GLNAMEDCOPYBUFFERSUBDATAEXT _glNamedCopyBufferSubDataEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPARAMETERIVEXT)(GLuint buffer, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDBUFFERPARAMETERIVEXT _glGetNamedBufferParameterivEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPOINTERVEXT)(GLuint buffer, GLenum pname, GLvoid * * params);
extern PFN_GLGETNAMEDBUFFERPOINTERVEXT _glGetNamedBufferPointervEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERSUBDATAEXT)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data);
extern PFN_GLGETNAMEDBUFFERSUBDATAEXT _glGetNamedBufferSubDataEXT;

typedef void (APIENTRY * PFN_GLTEXTUREBUFFEREXT)(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
extern PFN_GLTEXTUREBUFFEREXT _glTextureBufferEXT;

typedef void (APIENTRY * PFN_GLMULTITEXBUFFEREXT)(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
extern PFN_GLMULTITEXBUFFEREXT _glMultiTexBufferEXT;

typedef void (APIENTRY * PFN_GLNAMEDRENDERBUFFERSTORAGEEXT)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLNAMEDRENDERBUFFERSTORAGEEXT _glNamedRenderbufferStorageEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDRENDERBUFFERPARAMETERIVEXT)(GLuint renderbuffer, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDRENDERBUFFERPARAMETERIVEXT _glGetNamedRenderbufferParameterivEXT;

typedef GLenum (APIENTRY * PFN_GLCHECKNAMEDFRAMEBUFFERSTATUSEXT)(GLuint framebuffer, GLenum target);
extern PFN_GLCHECKNAMEDFRAMEBUFFERSTATUSEXT _glCheckNamedFramebufferStatusEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURE1DEXT)(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURE1DEXT _glNamedFramebufferTexture1DEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURE2DEXT)(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURE2DEXT _glNamedFramebufferTexture2DEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURE3DEXT)(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURE3DEXT _glNamedFramebufferTexture3DEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERRENDERBUFFEREXT)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFN_GLNAMEDFRAMEBUFFERRENDERBUFFEREXT _glNamedFramebufferRenderbufferEXT;

typedef void (APIENTRY * PFN_GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
extern PFN_GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT _glGetNamedFramebufferAttachmentParameterivEXT;

typedef void (APIENTRY * PFN_GLGENERATETEXTUREMIPMAPEXT)(GLuint texture, GLenum target);
extern PFN_GLGENERATETEXTUREMIPMAPEXT _glGenerateTextureMipmapEXT;

typedef void (APIENTRY * PFN_GLGENERATEMULTITEXMIPMAPEXT)(GLenum texunit, GLenum target);
extern PFN_GLGENERATEMULTITEXMIPMAPEXT _glGenerateMultiTexMipmapEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERDRAWBUFFEREXT)(GLuint framebuffer, GLenum mode);
extern PFN_GLFRAMEBUFFERDRAWBUFFEREXT _glFramebufferDrawBufferEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERDRAWBUFFERSEXT)(GLuint framebuffer, GLsizei n, const GLenum * bufs);
extern PFN_GLFRAMEBUFFERDRAWBUFFERSEXT _glFramebufferDrawBuffersEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERREADBUFFEREXT)(GLuint framebuffer, GLenum mode);
extern PFN_GLFRAMEBUFFERREADBUFFEREXT _glFramebufferReadBufferEXT;

typedef void (APIENTRY * PFN_GLGETFRAMEBUFFERPARAMETERIVEXT)(GLuint framebuffer, GLenum pname, GLint * params);
extern PFN_GLGETFRAMEBUFFERPARAMETERIVEXT _glGetFramebufferParameterivEXT;

typedef void (APIENTRY * PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT _glNamedRenderbufferStorageMultisampleEXT;

typedef void (APIENTRY * PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT)(GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT _glNamedRenderbufferStorageMultisampleCoverageEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTUREEXT)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
extern PFN_GLNAMEDFRAMEBUFFERTEXTUREEXT _glNamedFramebufferTextureEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTURELAYEREXT)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFN_GLNAMEDFRAMEBUFFERTEXTURELAYEREXT _glNamedFramebufferTextureLayerEXT;

typedef void (APIENTRY * PFN_GLNAMEDFRAMEBUFFERTEXTUREFACEEXT)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
extern PFN_GLNAMEDFRAMEBUFFERTEXTUREFACEEXT _glNamedFramebufferTextureFaceEXT;

typedef void (APIENTRY * PFN_GLTEXTURERENDERBUFFEREXT)(GLuint texture, GLenum target, GLuint renderbuffer);
extern PFN_GLTEXTURERENDERBUFFEREXT _glTextureRenderbufferEXT;

typedef void (APIENTRY * PFN_GLMULTITEXRENDERBUFFEREXT)(GLenum texunit, GLenum target, GLuint renderbuffer);
extern PFN_GLMULTITEXRENDERBUFFEREXT _glMultiTexRenderbufferEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1DEXT)(GLuint program, GLint location, GLdouble x);
extern PFN_GLPROGRAMUNIFORM1DEXT _glProgramUniform1dEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2DEXT)(GLuint program, GLint location, GLdouble x, GLdouble y);
extern PFN_GLPROGRAMUNIFORM2DEXT _glProgramUniform2dEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3DEXT)(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLPROGRAMUNIFORM3DEXT _glProgramUniform3dEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4DEXT)(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLPROGRAMUNIFORM4DEXT _glProgramUniform4dEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1DVEXT)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM1DVEXT _glProgramUniform1dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2DVEXT)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM2DVEXT _glProgramUniform2dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3DVEXT)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM3DVEXT _glProgramUniform3dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4DVEXT)(GLuint program, GLint location, GLsizei count, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORM4DVEXT _glProgramUniform4dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2DVEXT _glProgramUniformMatrix2dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3DVEXT _glProgramUniformMatrix3dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4DVEXT _glProgramUniformMatrix4dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X3DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X3DVEXT _glProgramUniformMatrix2x3dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX2X4DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX2X4DVEXT _glProgramUniformMatrix2x4dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X2DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X2DVEXT _glProgramUniformMatrix3x2dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX3X4DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX3X4DVEXT _glProgramUniformMatrix3x4dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X2DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X2DVEXT _glProgramUniformMatrix4x2dvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMMATRIX4X3DVEXT)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
extern PFN_GLPROGRAMUNIFORMMATRIX4X3DVEXT _glProgramUniformMatrix4x3dvEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXOFFSETEXT)(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYVERTEXOFFSETEXT _glVertexArrayVertexOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYCOLOROFFSETEXT)(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYCOLOROFFSETEXT _glVertexArrayColorOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYEDGEFLAGOFFSETEXT)(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYEDGEFLAGOFFSETEXT _glVertexArrayEdgeFlagOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYINDEXOFFSETEXT)(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYINDEXOFFSETEXT _glVertexArrayIndexOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYNORMALOFFSETEXT)(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYNORMALOFFSETEXT _glVertexArrayNormalOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYTEXCOORDOFFSETEXT)(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYTEXCOORDOFFSETEXT _glVertexArrayTexCoordOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYMULTITEXCOORDOFFSETEXT)(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYMULTITEXCOORDOFFSETEXT _glVertexArrayMultiTexCoordOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYFOGCOORDOFFSETEXT)(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYFOGCOORDOFFSETEXT _glVertexArrayFogCoordOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYSECONDARYCOLOROFFSETEXT)(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYSECONDARYCOLOROFFSETEXT _glVertexArraySecondaryColorOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBOFFSETEXT)(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBOFFSETEXT _glVertexArrayVertexAttribOffsetEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBIOFFSETEXT)(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBIOFFSETEXT _glVertexArrayVertexAttribIOffsetEXT;

typedef void (APIENTRY * PFN_GLENABLEVERTEXARRAYEXT)(GLuint vaobj, GLenum array);
extern PFN_GLENABLEVERTEXARRAYEXT _glEnableVertexArrayEXT;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXARRAYEXT)(GLuint vaobj, GLenum array);
extern PFN_GLDISABLEVERTEXARRAYEXT _glDisableVertexArrayEXT;

typedef void (APIENTRY * PFN_GLENABLEVERTEXARRAYATTRIBEXT)(GLuint vaobj, GLuint index);
extern PFN_GLENABLEVERTEXARRAYATTRIBEXT _glEnableVertexArrayAttribEXT;

typedef void (APIENTRY * PFN_GLDISABLEVERTEXARRAYATTRIBEXT)(GLuint vaobj, GLuint index);
extern PFN_GLDISABLEVERTEXARRAYATTRIBEXT _glDisableVertexArrayAttribEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYINTEGERVEXT)(GLuint vaobj, GLenum pname, GLint * param);
extern PFN_GLGETVERTEXARRAYINTEGERVEXT _glGetVertexArrayIntegervEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYPOINTERVEXT)(GLuint vaobj, GLenum pname, GLvoid * * param);
extern PFN_GLGETVERTEXARRAYPOINTERVEXT _glGetVertexArrayPointervEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYINTEGERI_VEXT)(GLuint vaobj, GLuint index, GLenum pname, GLint * param);
extern PFN_GLGETVERTEXARRAYINTEGERI_VEXT _glGetVertexArrayIntegeri_vEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXARRAYPOINTERI_VEXT)(GLuint vaobj, GLuint index, GLenum pname, GLvoid * * param);
extern PFN_GLGETVERTEXARRAYPOINTERI_VEXT _glGetVertexArrayPointeri_vEXT;

typedef void (APIENTRY * PFN_GLDISCARDFRAMEBUFFEREXT)(GLenum target, GLsizei numAttachments, const GLenum * attachments);
extern PFN_GLDISCARDFRAMEBUFFEREXT _glDiscardFramebufferEXT;

typedef void (APIENTRY * PFN_GLCOLORMASKINDEXEDEXT)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern PFN_GLCOLORMASKINDEXEDEXT _glColorMaskIndexedEXT;

typedef void (APIENTRY * PFN_GLGETBOOLEANINDEXEDVEXT)(GLenum target, GLuint index, GLboolean * data);
extern PFN_GLGETBOOLEANINDEXEDVEXT _glGetBooleanIndexedvEXT;

typedef void (APIENTRY * PFN_GLGETINTEGERINDEXEDVEXT)(GLenum target, GLuint index, GLint * data);
extern PFN_GLGETINTEGERINDEXEDVEXT _glGetIntegerIndexedvEXT;

typedef void (APIENTRY * PFN_GLENABLEINDEXEDEXT)(GLenum target, GLuint index);
extern PFN_GLENABLEINDEXEDEXT _glEnableIndexedEXT;

typedef void (APIENTRY * PFN_GLDISABLEINDEXEDEXT)(GLenum target, GLuint index);
extern PFN_GLDISABLEINDEXEDEXT _glDisableIndexedEXT;

typedef GLboolean (APIENTRY * PFN_GLISENABLEDINDEXEDEXT)(GLenum target, GLuint index);
extern PFN_GLISENABLEDINDEXEDEXT _glIsEnabledIndexedEXT;

typedef void (APIENTRY * PFN_GLDRAWARRAYSINSTANCEDEXT)(GLenum mode, GLint start, GLsizei count, GLsizei primcount);
extern PFN_GLDRAWARRAYSINSTANCEDEXT _glDrawArraysInstancedEXT;

typedef void (APIENTRY * PFN_GLDRAWELEMENTSINSTANCEDEXT)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
extern PFN_GLDRAWELEMENTSINSTANCEDEXT _glDrawElementsInstancedEXT;

typedef void (APIENTRY * PFN_GLDRAWRANGEELEMENTSEXT)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
extern PFN_GLDRAWRANGEELEMENTSEXT _glDrawRangeElementsEXT;

typedef void (APIENTRY * PFN_GLFOGCOORDFEXT)(GLfloat coord);
extern PFN_GLFOGCOORDFEXT _glFogCoordfEXT;

typedef void (APIENTRY * PFN_GLFOGCOORDFVEXT)(const GLfloat * coord);
extern PFN_GLFOGCOORDFVEXT _glFogCoordfvEXT;

typedef void (APIENTRY * PFN_GLFOGCOORDDEXT)(GLdouble coord);
extern PFN_GLFOGCOORDDEXT _glFogCoorddEXT;

typedef void (APIENTRY * PFN_GLFOGCOORDDVEXT)(const GLdouble * coord);
extern PFN_GLFOGCOORDDVEXT _glFogCoorddvEXT;

typedef void (APIENTRY * PFN_GLFOGCOORDPOINTEREXT)(GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLFOGCOORDPOINTEREXT _glFogCoordPointerEXT;

typedef void (APIENTRY * PFN_GLBLITFRAMEBUFFEREXT)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_GLBLITFRAMEBUFFEREXT _glBlitFramebufferEXT;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEEXT)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEEXT _glRenderbufferStorageMultisampleEXT;

typedef GLboolean (APIENTRY * PFN_GLISRENDERBUFFEREXT)(GLuint renderbuffer);
extern PFN_GLISRENDERBUFFEREXT _glIsRenderbufferEXT;

typedef void (APIENTRY * PFN_GLBINDRENDERBUFFEREXT)(GLenum target, GLuint renderbuffer);
extern PFN_GLBINDRENDERBUFFEREXT _glBindRenderbufferEXT;

typedef void (APIENTRY * PFN_GLDELETERENDERBUFFERSEXT)(GLsizei n, const GLuint * renderbuffers);
extern PFN_GLDELETERENDERBUFFERSEXT _glDeleteRenderbuffersEXT;

typedef void (APIENTRY * PFN_GLGENRENDERBUFFERSEXT)(GLsizei n, GLuint * renderbuffers);
extern PFN_GLGENRENDERBUFFERSEXT _glGenRenderbuffersEXT;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEEXT _glRenderbufferStorageEXT;

typedef void (APIENTRY * PFN_GLGETRENDERBUFFERPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETRENDERBUFFERPARAMETERIVEXT _glGetRenderbufferParameterivEXT;

typedef GLboolean (APIENTRY * PFN_GLISFRAMEBUFFEREXT)(GLuint framebuffer);
extern PFN_GLISFRAMEBUFFEREXT _glIsFramebufferEXT;

typedef void (APIENTRY * PFN_GLBINDFRAMEBUFFEREXT)(GLenum target, GLuint framebuffer);
extern PFN_GLBINDFRAMEBUFFEREXT _glBindFramebufferEXT;

typedef void (APIENTRY * PFN_GLDELETEFRAMEBUFFERSEXT)(GLsizei n, const GLuint * framebuffers);
extern PFN_GLDELETEFRAMEBUFFERSEXT _glDeleteFramebuffersEXT;

typedef void (APIENTRY * PFN_GLGENFRAMEBUFFERSEXT)(GLsizei n, GLuint * framebuffers);
extern PFN_GLGENFRAMEBUFFERSEXT _glGenFramebuffersEXT;

typedef GLenum (APIENTRY * PFN_GLCHECKFRAMEBUFFERSTATUSEXT)(GLenum target);
extern PFN_GLCHECKFRAMEBUFFERSTATUSEXT _glCheckFramebufferStatusEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE1DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE1DEXT _glFramebufferTexture1DEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE2DEXT _glFramebufferTexture2DEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURE3DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern PFN_GLFRAMEBUFFERTEXTURE3DEXT _glFramebufferTexture3DEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERRENDERBUFFEREXT)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFN_GLFRAMEBUFFERRENDERBUFFEREXT _glFramebufferRenderbufferEXT;

typedef void (APIENTRY * PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXT)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
extern PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXT _glGetFramebufferAttachmentParameterivEXT;

typedef void (APIENTRY * PFN_GLGENERATEMIPMAPEXT)(GLenum target);
extern PFN_GLGENERATEMIPMAPEXT _glGenerateMipmapEXT;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETERIEXT)(GLuint program, GLenum pname, GLint value);
extern PFN_GLPROGRAMPARAMETERIEXT _glProgramParameteriEXT;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERS4FVEXT)(GLenum target, GLuint index, GLsizei count, const GLfloat * params);
extern PFN_GLPROGRAMENVPARAMETERS4FVEXT _glProgramEnvParameters4fvEXT;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERS4FVEXT)(GLenum target, GLuint index, GLsizei count, const GLfloat * params);
extern PFN_GLPROGRAMLOCALPARAMETERS4FVEXT _glProgramLocalParameters4fvEXT;

typedef void (APIENTRY * PFN_GLGETUNIFORMUIVEXT)(GLuint program, GLint location, GLuint * params);
extern PFN_GLGETUNIFORMUIVEXT _glGetUniformuivEXT;

typedef void (APIENTRY * PFN_GLBINDFRAGDATALOCATIONEXT)(GLuint program, GLuint color, const GLchar * name);
extern PFN_GLBINDFRAGDATALOCATIONEXT _glBindFragDataLocationEXT;

typedef GLint (APIENTRY * PFN_GLGETFRAGDATALOCATIONEXT)(GLuint program, const GLchar * name);
extern PFN_GLGETFRAGDATALOCATIONEXT _glGetFragDataLocationEXT;

typedef void (APIENTRY * PFN_GLUNIFORM1UIEXT)(GLint location, GLuint v0);
extern PFN_GLUNIFORM1UIEXT _glUniform1uiEXT;

typedef void (APIENTRY * PFN_GLUNIFORM2UIEXT)(GLint location, GLuint v0, GLuint v1);
extern PFN_GLUNIFORM2UIEXT _glUniform2uiEXT;

typedef void (APIENTRY * PFN_GLUNIFORM3UIEXT)(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern PFN_GLUNIFORM3UIEXT _glUniform3uiEXT;

typedef void (APIENTRY * PFN_GLUNIFORM4UIEXT)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern PFN_GLUNIFORM4UIEXT _glUniform4uiEXT;

typedef void (APIENTRY * PFN_GLUNIFORM1UIVEXT)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM1UIVEXT _glUniform1uivEXT;

typedef void (APIENTRY * PFN_GLUNIFORM2UIVEXT)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM2UIVEXT _glUniform2uivEXT;

typedef void (APIENTRY * PFN_GLUNIFORM3UIVEXT)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM3UIVEXT _glUniform3uivEXT;

typedef void (APIENTRY * PFN_GLUNIFORM4UIVEXT)(GLint location, GLsizei count, const GLuint * value);
extern PFN_GLUNIFORM4UIVEXT _glUniform4uivEXT;

typedef void (APIENTRY * PFN_GLGETHISTOGRAMEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
extern PFN_GLGETHISTOGRAMEXT _glGetHistogramEXT;

typedef void (APIENTRY * PFN_GLGETHISTOGRAMPARAMETERFVEXT)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETHISTOGRAMPARAMETERFVEXT _glGetHistogramParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETHISTOGRAMPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETHISTOGRAMPARAMETERIVEXT _glGetHistogramParameterivEXT;

typedef void (APIENTRY * PFN_GLGETMINMAXEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
extern PFN_GLGETMINMAXEXT _glGetMinmaxEXT;

typedef void (APIENTRY * PFN_GLGETMINMAXPARAMETERFVEXT)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETMINMAXPARAMETERFVEXT _glGetMinmaxParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETMINMAXPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMINMAXPARAMETERIVEXT _glGetMinmaxParameterivEXT;

typedef void (APIENTRY * PFN_GLHISTOGRAMEXT)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern PFN_GLHISTOGRAMEXT _glHistogramEXT;

typedef void (APIENTRY * PFN_GLMINMAXEXT)(GLenum target, GLenum internalformat, GLboolean sink);
extern PFN_GLMINMAXEXT _glMinmaxEXT;

typedef void (APIENTRY * PFN_GLRESETHISTOGRAMEXT)(GLenum target);
extern PFN_GLRESETHISTOGRAMEXT _glResetHistogramEXT;

typedef void (APIENTRY * PFN_GLRESETMINMAXEXT)(GLenum target);
extern PFN_GLRESETMINMAXEXT _glResetMinmaxEXT;

typedef void (APIENTRY * PFN_GLINDEXFUNCEXT)(GLenum func, GLclampf ref);
extern PFN_GLINDEXFUNCEXT _glIndexFuncEXT;

typedef void (APIENTRY * PFN_GLINDEXMATERIALEXT)(GLenum face, GLenum mode);
extern PFN_GLINDEXMATERIALEXT _glIndexMaterialEXT;

typedef void (APIENTRY * PFN_GLAPPLYTEXTUREEXT)(GLenum mode);
extern PFN_GLAPPLYTEXTUREEXT _glApplyTextureEXT;

typedef void (APIENTRY * PFN_GLTEXTURELIGHTEXT)(GLenum pname);
extern PFN_GLTEXTURELIGHTEXT _glTextureLightEXT;

typedef void (APIENTRY * PFN_GLTEXTUREMATERIALEXT)(GLenum face, GLenum mode);
extern PFN_GLTEXTUREMATERIALEXT _glTextureMaterialEXT;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYSEXT)(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);
extern PFN_GLMULTIDRAWARRAYSEXT _glMultiDrawArraysEXT;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSEXT)(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount);
extern PFN_GLMULTIDRAWELEMENTSEXT _glMultiDrawElementsEXT;

typedef void (APIENTRY * PFN_GLSAMPLEMASKEXT)(GLclampf value, GLboolean invert);
extern PFN_GLSAMPLEMASKEXT _glSampleMaskEXT;

typedef void (APIENTRY * PFN_GLSAMPLEPATTERNEXT)(GLenum pattern);
extern PFN_GLSAMPLEPATTERNEXT _glSamplePatternEXT;

typedef void (APIENTRY * PFN_GLCOLORTABLEEXT)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);
extern PFN_GLCOLORTABLEEXT _glColorTableEXT;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEEXT)(GLenum target, GLenum format, GLenum type, GLvoid * data);
extern PFN_GLGETCOLORTABLEEXT _glGetColorTableEXT;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETCOLORTABLEPARAMETERIVEXT _glGetColorTableParameterivEXT;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERFVEXT)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETCOLORTABLEPARAMETERFVEXT _glGetColorTableParameterfvEXT;

typedef void (APIENTRY * PFN_GLPIXELTRANSFORMPARAMETERIEXT)(GLenum target, GLenum pname, GLint param);
extern PFN_GLPIXELTRANSFORMPARAMETERIEXT _glPixelTransformParameteriEXT;

typedef void (APIENTRY * PFN_GLPIXELTRANSFORMPARAMETERFEXT)(GLenum target, GLenum pname, GLfloat param);
extern PFN_GLPIXELTRANSFORMPARAMETERFEXT _glPixelTransformParameterfEXT;

typedef void (APIENTRY * PFN_GLPIXELTRANSFORMPARAMETERIVEXT)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLPIXELTRANSFORMPARAMETERIVEXT _glPixelTransformParameterivEXT;

typedef void (APIENTRY * PFN_GLPIXELTRANSFORMPARAMETERFVEXT)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLPIXELTRANSFORMPARAMETERFVEXT _glPixelTransformParameterfvEXT;

typedef void (APIENTRY * PFN_GLGETPIXELTRANSFORMPARAMETERIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETPIXELTRANSFORMPARAMETERIVEXT _glGetPixelTransformParameterivEXT;

typedef void (APIENTRY * PFN_GLGETPIXELTRANSFORMPARAMETERFVEXT)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETPIXELTRANSFORMPARAMETERFVEXT _glGetPixelTransformParameterfvEXT;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFEXT)(GLenum pname, GLfloat param);
extern PFN_GLPOINTPARAMETERFEXT _glPointParameterfEXT;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFVEXT)(GLenum pname, const GLfloat * params);
extern PFN_GLPOINTPARAMETERFVEXT _glPointParameterfvEXT;

typedef void (APIENTRY * PFN_GLPOLYGONOFFSETEXT)(GLfloat factor, GLfloat bias);
extern PFN_GLPOLYGONOFFSETEXT _glPolygonOffsetEXT;

typedef void (APIENTRY * PFN_GLPROVOKINGVERTEXEXT)(GLenum mode);
extern PFN_GLPROVOKINGVERTEXEXT _glProvokingVertexEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3BEXT)(GLbyte red, GLbyte green, GLbyte blue);
extern PFN_GLSECONDARYCOLOR3BEXT _glSecondaryColor3bEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3BVEXT)(const GLbyte * v);
extern PFN_GLSECONDARYCOLOR3BVEXT _glSecondaryColor3bvEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3DEXT)(GLdouble red, GLdouble green, GLdouble blue);
extern PFN_GLSECONDARYCOLOR3DEXT _glSecondaryColor3dEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3DVEXT)(const GLdouble * v);
extern PFN_GLSECONDARYCOLOR3DVEXT _glSecondaryColor3dvEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3FEXT)(GLfloat red, GLfloat green, GLfloat blue);
extern PFN_GLSECONDARYCOLOR3FEXT _glSecondaryColor3fEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3FVEXT)(const GLfloat * v);
extern PFN_GLSECONDARYCOLOR3FVEXT _glSecondaryColor3fvEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3IEXT)(GLint red, GLint green, GLint blue);
extern PFN_GLSECONDARYCOLOR3IEXT _glSecondaryColor3iEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3IVEXT)(const GLint * v);
extern PFN_GLSECONDARYCOLOR3IVEXT _glSecondaryColor3ivEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3SEXT)(GLshort red, GLshort green, GLshort blue);
extern PFN_GLSECONDARYCOLOR3SEXT _glSecondaryColor3sEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3SVEXT)(const GLshort * v);
extern PFN_GLSECONDARYCOLOR3SVEXT _glSecondaryColor3svEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UBEXT)(GLubyte red, GLubyte green, GLubyte blue);
extern PFN_GLSECONDARYCOLOR3UBEXT _glSecondaryColor3ubEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UBVEXT)(const GLubyte * v);
extern PFN_GLSECONDARYCOLOR3UBVEXT _glSecondaryColor3ubvEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UIEXT)(GLuint red, GLuint green, GLuint blue);
extern PFN_GLSECONDARYCOLOR3UIEXT _glSecondaryColor3uiEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3UIVEXT)(const GLuint * v);
extern PFN_GLSECONDARYCOLOR3UIVEXT _glSecondaryColor3uivEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3USEXT)(GLushort red, GLushort green, GLushort blue);
extern PFN_GLSECONDARYCOLOR3USEXT _glSecondaryColor3usEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3USVEXT)(const GLushort * v);
extern PFN_GLSECONDARYCOLOR3USVEXT _glSecondaryColor3usvEXT;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORPOINTEREXT)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLSECONDARYCOLORPOINTEREXT _glSecondaryColorPointerEXT;

typedef void (APIENTRY * PFN_GLUSESHADERPROGRAMEXT)(GLenum type, GLuint program);
extern PFN_GLUSESHADERPROGRAMEXT _glUseShaderProgramEXT;

typedef void (APIENTRY * PFN_GLACTIVEPROGRAMEXT)(GLuint program);
extern PFN_GLACTIVEPROGRAMEXT _glActiveProgramEXT;

typedef GLuint (APIENTRY * PFN_GLCREATESHADERPROGRAMEXT)(GLenum type, const GLchar * string);
extern PFN_GLCREATESHADERPROGRAMEXT _glCreateShaderProgramEXT;

typedef void (APIENTRY * PFN_GLBINDIMAGETEXTUREEXT)(GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
extern PFN_GLBINDIMAGETEXTUREEXT _glBindImageTextureEXT;

typedef void (APIENTRY * PFN_GLMEMORYBARRIEREXT)(GLbitfield barriers);
extern PFN_GLMEMORYBARRIEREXT _glMemoryBarrierEXT;

typedef void (APIENTRY * PFN_GLSTENCILCLEARTAGEXT)(GLsizei stencilTagBits, GLuint stencilClearTag);
extern PFN_GLSTENCILCLEARTAGEXT _glStencilClearTagEXT;

typedef void (APIENTRY * PFN_GLACTIVESTENCILFACEEXT)(GLenum face);
extern PFN_GLACTIVESTENCILFACEEXT _glActiveStencilFaceEXT;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE1DEXT)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE1DEXT _glTexSubImage1DEXT;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE2DEXT _glTexSubImage2DEXT;

typedef void (APIENTRY * PFN_GLTEXIMAGE3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE3DEXT _glTexImage3DEXT;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE3DEXT _glTexSubImage3DEXT;

typedef void (APIENTRY * PFN_GLTEXBUFFEREXT)(GLenum target, GLenum internalformat, GLuint buffer);
extern PFN_GLTEXBUFFEREXT _glTexBufferEXT;

typedef void (APIENTRY * PFN_GLTEXPARAMETERIIVEXT)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLTEXPARAMETERIIVEXT _glTexParameterIivEXT;

typedef void (APIENTRY * PFN_GLTEXPARAMETERIUIVEXT)(GLenum target, GLenum pname, const GLuint * params);
extern PFN_GLTEXPARAMETERIUIVEXT _glTexParameterIuivEXT;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERIIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETTEXPARAMETERIIVEXT _glGetTexParameterIivEXT;

typedef void (APIENTRY * PFN_GLGETTEXPARAMETERIUIVEXT)(GLenum target, GLenum pname, GLuint * params);
extern PFN_GLGETTEXPARAMETERIUIVEXT _glGetTexParameterIuivEXT;

typedef void (APIENTRY * PFN_GLCLEARCOLORIIEXT)(GLint red, GLint green, GLint blue, GLint alpha);
extern PFN_GLCLEARCOLORIIEXT _glClearColorIiEXT;

typedef void (APIENTRY * PFN_GLCLEARCOLORIUIEXT)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern PFN_GLCLEARCOLORIUIEXT _glClearColorIuiEXT;

typedef GLboolean (APIENTRY * PFN_GLARETEXTURESRESIDENTEXT)(GLsizei n, const GLuint * textures, GLboolean * residences);
extern PFN_GLARETEXTURESRESIDENTEXT _glAreTexturesResidentEXT;

typedef void (APIENTRY * PFN_GLBINDTEXTUREEXT)(GLenum target, GLuint texture);
extern PFN_GLBINDTEXTUREEXT _glBindTextureEXT;

typedef void (APIENTRY * PFN_GLDELETETEXTURESEXT)(GLsizei n, const GLuint * textures);
extern PFN_GLDELETETEXTURESEXT _glDeleteTexturesEXT;

typedef void (APIENTRY * PFN_GLGENTEXTURESEXT)(GLsizei n, GLuint * textures);
extern PFN_GLGENTEXTURESEXT _glGenTexturesEXT;

typedef GLboolean (APIENTRY * PFN_GLISTEXTUREEXT)(GLuint texture);
extern PFN_GLISTEXTUREEXT _glIsTextureEXT;

typedef void (APIENTRY * PFN_GLPRIORITIZETEXTURESEXT)(GLsizei n, const GLuint * textures, const GLclampf * priorities);
extern PFN_GLPRIORITIZETEXTURESEXT _glPrioritizeTexturesEXT;

typedef void (APIENTRY * PFN_GLTEXTURENORMALEXT)(GLenum mode);
extern PFN_GLTEXTURENORMALEXT _glTextureNormalEXT;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTI64VEXT)(GLuint id, GLenum pname, GLint64EXT * params);
extern PFN_GLGETQUERYOBJECTI64VEXT _glGetQueryObjecti64vEXT;

typedef void (APIENTRY * PFN_GLGETQUERYOBJECTUI64VEXT)(GLuint id, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETQUERYOBJECTUI64VEXT _glGetQueryObjectui64vEXT;

typedef void (APIENTRY * PFN_GLBEGINTRANSFORMFEEDBACKEXT)(GLenum primitiveMode);
extern PFN_GLBEGINTRANSFORMFEEDBACKEXT _glBeginTransformFeedbackEXT;

typedef void (APIENTRY * PFN_GLENDTRANSFORMFEEDBACKEXT)(void);
extern PFN_GLENDTRANSFORMFEEDBACKEXT _glEndTransformFeedbackEXT;

typedef void (APIENTRY * PFN_GLBINDBUFFERRANGEEXT)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFN_GLBINDBUFFERRANGEEXT _glBindBufferRangeEXT;

typedef void (APIENTRY * PFN_GLBINDBUFFEROFFSETEXT)(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
extern PFN_GLBINDBUFFEROFFSETEXT _glBindBufferOffsetEXT;

typedef void (APIENTRY * PFN_GLBINDBUFFERBASEEXT)(GLenum target, GLuint index, GLuint buffer);
extern PFN_GLBINDBUFFERBASEEXT _glBindBufferBaseEXT;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKVARYINGSEXT)(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode);
extern PFN_GLTRANSFORMFEEDBACKVARYINGSEXT _glTransformFeedbackVaryingsEXT;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKVARYINGEXT)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
extern PFN_GLGETTRANSFORMFEEDBACKVARYINGEXT _glGetTransformFeedbackVaryingEXT;

typedef void (APIENTRY * PFN_GLARRAYELEMENTEXT)(GLint i);
extern PFN_GLARRAYELEMENTEXT _glArrayElementEXT;

typedef void (APIENTRY * PFN_GLCOLORPOINTEREXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
extern PFN_GLCOLORPOINTEREXT _glColorPointerEXT;

typedef void (APIENTRY * PFN_GLDRAWARRAYSEXT)(GLenum mode, GLint first, GLsizei count);
extern PFN_GLDRAWARRAYSEXT _glDrawArraysEXT;

typedef void (APIENTRY * PFN_GLEDGEFLAGPOINTEREXT)(GLsizei stride, GLsizei count, const GLboolean * pointer);
extern PFN_GLEDGEFLAGPOINTEREXT _glEdgeFlagPointerEXT;

typedef void (APIENTRY * PFN_GLGETPOINTERVEXT)(GLenum pname, GLvoid * * params);
extern PFN_GLGETPOINTERVEXT _glGetPointervEXT;

typedef void (APIENTRY * PFN_GLINDEXPOINTEREXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
extern PFN_GLINDEXPOINTEREXT _glIndexPointerEXT;

typedef void (APIENTRY * PFN_GLNORMALPOINTEREXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
extern PFN_GLNORMALPOINTEREXT _glNormalPointerEXT;

typedef void (APIENTRY * PFN_GLTEXCOORDPOINTEREXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
extern PFN_GLTEXCOORDPOINTEREXT _glTexCoordPointerEXT;

typedef void (APIENTRY * PFN_GLVERTEXPOINTEREXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
extern PFN_GLVERTEXPOINTEREXT _glVertexPointerEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1DEXT)(GLuint index, GLdouble x);
extern PFN_GLVERTEXATTRIBL1DEXT _glVertexAttribL1dEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2DEXT)(GLuint index, GLdouble x, GLdouble y);
extern PFN_GLVERTEXATTRIBL2DEXT _glVertexAttribL2dEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3DEXT)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXATTRIBL3DEXT _glVertexAttribL3dEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4DEXT)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXATTRIBL4DEXT _glVertexAttribL4dEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1DVEXT)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL1DVEXT _glVertexAttribL1dvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2DVEXT)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL2DVEXT _glVertexAttribL2dvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3DVEXT)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL3DVEXT _glVertexAttribL3dvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4DVEXT)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIBL4DVEXT _glVertexAttribL4dvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBLPOINTEREXT)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBLPOINTEREXT _glVertexAttribLPointerEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBLDVEXT)(GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETVERTEXATTRIBLDVEXT _glGetVertexAttribLdvEXT;

typedef void (APIENTRY * PFN_GLVERTEXARRAYVERTEXATTRIBLOFFSETEXT)(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFN_GLVERTEXARRAYVERTEXATTRIBLOFFSETEXT _glVertexArrayVertexAttribLOffsetEXT;

typedef void (APIENTRY * PFN_GLBEGINVERTEXSHADEREXT)(void);
extern PFN_GLBEGINVERTEXSHADEREXT _glBeginVertexShaderEXT;

typedef void (APIENTRY * PFN_GLENDVERTEXSHADEREXT)(void);
extern PFN_GLENDVERTEXSHADEREXT _glEndVertexShaderEXT;

typedef void (APIENTRY * PFN_GLBINDVERTEXSHADEREXT)(GLuint id);
extern PFN_GLBINDVERTEXSHADEREXT _glBindVertexShaderEXT;

typedef GLuint (APIENTRY * PFN_GLGENVERTEXSHADERSEXT)(GLuint range);
extern PFN_GLGENVERTEXSHADERSEXT _glGenVertexShadersEXT;

typedef void (APIENTRY * PFN_GLDELETEVERTEXSHADEREXT)(GLuint id);
extern PFN_GLDELETEVERTEXSHADEREXT _glDeleteVertexShaderEXT;

typedef void (APIENTRY * PFN_GLSHADEROP1EXT)(GLenum op, GLuint res, GLuint arg1);
extern PFN_GLSHADEROP1EXT _glShaderOp1EXT;

typedef void (APIENTRY * PFN_GLSHADEROP2EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2);
extern PFN_GLSHADEROP2EXT _glShaderOp2EXT;

typedef void (APIENTRY * PFN_GLSHADEROP3EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
extern PFN_GLSHADEROP3EXT _glShaderOp3EXT;

typedef void (APIENTRY * PFN_GLSWIZZLEEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern PFN_GLSWIZZLEEXT _glSwizzleEXT;

typedef void (APIENTRY * PFN_GLWRITEMASKEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern PFN_GLWRITEMASKEXT _glWriteMaskEXT;

typedef void (APIENTRY * PFN_GLINSERTCOMPONENTEXT)(GLuint res, GLuint src, GLuint num);
extern PFN_GLINSERTCOMPONENTEXT _glInsertComponentEXT;

typedef void (APIENTRY * PFN_GLEXTRACTCOMPONENTEXT)(GLuint res, GLuint src, GLuint num);
extern PFN_GLEXTRACTCOMPONENTEXT _glExtractComponentEXT;

typedef GLuint (APIENTRY * PFN_GLGENSYMBOLSEXT)(GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
extern PFN_GLGENSYMBOLSEXT _glGenSymbolsEXT;

typedef void (APIENTRY * PFN_GLSETINVARIANTEXT)(GLuint id, GLenum type, const GLvoid * addr);
extern PFN_GLSETINVARIANTEXT _glSetInvariantEXT;

typedef void (APIENTRY * PFN_GLSETLOCALCONSTANTEXT)(GLuint id, GLenum type, const GLvoid * addr);
extern PFN_GLSETLOCALCONSTANTEXT _glSetLocalConstantEXT;

typedef void (APIENTRY * PFN_GLVARIANTBVEXT)(GLuint id, const GLbyte * addr);
extern PFN_GLVARIANTBVEXT _glVariantbvEXT;

typedef void (APIENTRY * PFN_GLVARIANTSVEXT)(GLuint id, const GLshort * addr);
extern PFN_GLVARIANTSVEXT _glVariantsvEXT;

typedef void (APIENTRY * PFN_GLVARIANTIVEXT)(GLuint id, const GLint * addr);
extern PFN_GLVARIANTIVEXT _glVariantivEXT;

typedef void (APIENTRY * PFN_GLVARIANTFVEXT)(GLuint id, const GLfloat * addr);
extern PFN_GLVARIANTFVEXT _glVariantfvEXT;

typedef void (APIENTRY * PFN_GLVARIANTDVEXT)(GLuint id, const GLdouble * addr);
extern PFN_GLVARIANTDVEXT _glVariantdvEXT;

typedef void (APIENTRY * PFN_GLVARIANTUBVEXT)(GLuint id, const GLubyte * addr);
extern PFN_GLVARIANTUBVEXT _glVariantubvEXT;

typedef void (APIENTRY * PFN_GLVARIANTUSVEXT)(GLuint id, const GLushort * addr);
extern PFN_GLVARIANTUSVEXT _glVariantusvEXT;

typedef void (APIENTRY * PFN_GLVARIANTUIVEXT)(GLuint id, const GLuint * addr);
extern PFN_GLVARIANTUIVEXT _glVariantuivEXT;

typedef void (APIENTRY * PFN_GLVARIANTPOINTEREXT)(GLuint id, GLenum type, GLuint stride, const GLvoid * addr);
extern PFN_GLVARIANTPOINTEREXT _glVariantPointerEXT;

typedef void (APIENTRY * PFN_GLENABLEVARIANTCLIENTSTATEEXT)(GLuint id);
extern PFN_GLENABLEVARIANTCLIENTSTATEEXT _glEnableVariantClientStateEXT;

typedef void (APIENTRY * PFN_GLDISABLEVARIANTCLIENTSTATEEXT)(GLuint id);
extern PFN_GLDISABLEVARIANTCLIENTSTATEEXT _glDisableVariantClientStateEXT;

typedef GLuint (APIENTRY * PFN_GLBINDLIGHTPARAMETEREXT)(GLenum light, GLenum value);
extern PFN_GLBINDLIGHTPARAMETEREXT _glBindLightParameterEXT;

typedef GLuint (APIENTRY * PFN_GLBINDMATERIALPARAMETEREXT)(GLenum face, GLenum value);
extern PFN_GLBINDMATERIALPARAMETEREXT _glBindMaterialParameterEXT;

typedef GLuint (APIENTRY * PFN_GLBINDTEXGENPARAMETEREXT)(GLenum unit, GLenum coord, GLenum value);
extern PFN_GLBINDTEXGENPARAMETEREXT _glBindTexGenParameterEXT;

typedef GLuint (APIENTRY * PFN_GLBINDTEXTUREUNITPARAMETEREXT)(GLenum unit, GLenum value);
extern PFN_GLBINDTEXTUREUNITPARAMETEREXT _glBindTextureUnitParameterEXT;

typedef GLuint (APIENTRY * PFN_GLBINDPARAMETEREXT)(GLenum value);
extern PFN_GLBINDPARAMETEREXT _glBindParameterEXT;

typedef GLboolean (APIENTRY * PFN_GLISVARIANTENABLEDEXT)(GLuint id, GLenum cap);
extern PFN_GLISVARIANTENABLEDEXT _glIsVariantEnabledEXT;

typedef void (APIENTRY * PFN_GLGETVARIANTBOOLEANVEXT)(GLuint id, GLenum value, GLboolean * data);
extern PFN_GLGETVARIANTBOOLEANVEXT _glGetVariantBooleanvEXT;

typedef void (APIENTRY * PFN_GLGETVARIANTINTEGERVEXT)(GLuint id, GLenum value, GLint * data);
extern PFN_GLGETVARIANTINTEGERVEXT _glGetVariantIntegervEXT;

typedef void (APIENTRY * PFN_GLGETVARIANTFLOATVEXT)(GLuint id, GLenum value, GLfloat * data);
extern PFN_GLGETVARIANTFLOATVEXT _glGetVariantFloatvEXT;

typedef void (APIENTRY * PFN_GLGETVARIANTPOINTERVEXT)(GLuint id, GLenum value, GLvoid * * data);
extern PFN_GLGETVARIANTPOINTERVEXT _glGetVariantPointervEXT;

typedef void (APIENTRY * PFN_GLGETINVARIANTBOOLEANVEXT)(GLuint id, GLenum value, GLboolean * data);
extern PFN_GLGETINVARIANTBOOLEANVEXT _glGetInvariantBooleanvEXT;

typedef void (APIENTRY * PFN_GLGETINVARIANTINTEGERVEXT)(GLuint id, GLenum value, GLint * data);
extern PFN_GLGETINVARIANTINTEGERVEXT _glGetInvariantIntegervEXT;

typedef void (APIENTRY * PFN_GLGETINVARIANTFLOATVEXT)(GLuint id, GLenum value, GLfloat * data);
extern PFN_GLGETINVARIANTFLOATVEXT _glGetInvariantFloatvEXT;

typedef void (APIENTRY * PFN_GLGETLOCALCONSTANTBOOLEANVEXT)(GLuint id, GLenum value, GLboolean * data);
extern PFN_GLGETLOCALCONSTANTBOOLEANVEXT _glGetLocalConstantBooleanvEXT;

typedef void (APIENTRY * PFN_GLGETLOCALCONSTANTINTEGERVEXT)(GLuint id, GLenum value, GLint * data);
extern PFN_GLGETLOCALCONSTANTINTEGERVEXT _glGetLocalConstantIntegervEXT;

typedef void (APIENTRY * PFN_GLGETLOCALCONSTANTFLOATVEXT)(GLuint id, GLenum value, GLfloat * data);
extern PFN_GLGETLOCALCONSTANTFLOATVEXT _glGetLocalConstantFloatvEXT;

typedef void (APIENTRY * PFN_GLVERTEXWEIGHTFEXT)(GLfloat weight);
extern PFN_GLVERTEXWEIGHTFEXT _glVertexWeightfEXT;

typedef void (APIENTRY * PFN_GLVERTEXWEIGHTFVEXT)(const GLfloat * weight);
extern PFN_GLVERTEXWEIGHTFVEXT _glVertexWeightfvEXT;

typedef void (APIENTRY * PFN_GLVERTEXWEIGHTPOINTEREXT)(GLsizei size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXWEIGHTPOINTEREXT _glVertexWeightPointerEXT;

typedef GLsync (APIENTRY * PFN_GLIMPORTSYNCEXT)(GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);
extern PFN_GLIMPORTSYNCEXT _glImportSyncEXT;

typedef void (APIENTRY * PFN_GLFRAMETERMINATORGREMEDY)(void);
extern PFN_GLFRAMETERMINATORGREMEDY _glFrameTerminatorGREMEDY;

typedef void (APIENTRY * PFN_GLSTRINGMARKERGREMEDY)(GLsizei len, const GLvoid * string);
extern PFN_GLSTRINGMARKERGREMEDY _glStringMarkerGREMEDY;

typedef void (APIENTRY * PFN_GLIMAGETRANSFORMPARAMETERIHP)(GLenum target, GLenum pname, GLint param);
extern PFN_GLIMAGETRANSFORMPARAMETERIHP _glImageTransformParameteriHP;

typedef void (APIENTRY * PFN_GLIMAGETRANSFORMPARAMETERFHP)(GLenum target, GLenum pname, GLfloat param);
extern PFN_GLIMAGETRANSFORMPARAMETERFHP _glImageTransformParameterfHP;

typedef void (APIENTRY * PFN_GLIMAGETRANSFORMPARAMETERIVHP)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLIMAGETRANSFORMPARAMETERIVHP _glImageTransformParameterivHP;

typedef void (APIENTRY * PFN_GLIMAGETRANSFORMPARAMETERFVHP)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLIMAGETRANSFORMPARAMETERFVHP _glImageTransformParameterfvHP;

typedef void (APIENTRY * PFN_GLGETIMAGETRANSFORMPARAMETERIVHP)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETIMAGETRANSFORMPARAMETERIVHP _glGetImageTransformParameterivHP;

typedef void (APIENTRY * PFN_GLGETIMAGETRANSFORMPARAMETERFVHP)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETIMAGETRANSFORMPARAMETERFVHP _glGetImageTransformParameterfvHP;

typedef void (APIENTRY * PFN_GLMULTIMODEDRAWARRAYSIBM)(const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride);
extern PFN_GLMULTIMODEDRAWARRAYSIBM _glMultiModeDrawArraysIBM;

typedef void (APIENTRY * PFN_GLMULTIMODEDRAWELEMENTSIBM)(const GLenum * mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, GLint modestride);
extern PFN_GLMULTIMODEDRAWELEMENTSIBM _glMultiModeDrawElementsIBM;

typedef void (APIENTRY * PFN_GLCOLORPOINTERLISTIBM)(GLint size, GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLCOLORPOINTERLISTIBM _glColorPointerListIBM;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORPOINTERLISTIBM)(GLint size, GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLSECONDARYCOLORPOINTERLISTIBM _glSecondaryColorPointerListIBM;

typedef void (APIENTRY * PFN_GLEDGEFLAGPOINTERLISTIBM)(GLint stride, const GLboolean * * pointer, GLint ptrstride);
extern PFN_GLEDGEFLAGPOINTERLISTIBM _glEdgeFlagPointerListIBM;

typedef void (APIENTRY * PFN_GLFOGCOORDPOINTERLISTIBM)(GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLFOGCOORDPOINTERLISTIBM _glFogCoordPointerListIBM;

typedef void (APIENTRY * PFN_GLINDEXPOINTERLISTIBM)(GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLINDEXPOINTERLISTIBM _glIndexPointerListIBM;

typedef void (APIENTRY * PFN_GLNORMALPOINTERLISTIBM)(GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLNORMALPOINTERLISTIBM _glNormalPointerListIBM;

typedef void (APIENTRY * PFN_GLTEXCOORDPOINTERLISTIBM)(GLint size, GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLTEXCOORDPOINTERLISTIBM _glTexCoordPointerListIBM;

typedef void (APIENTRY * PFN_GLVERTEXPOINTERLISTIBM)(GLint size, GLenum type, GLint stride, const GLvoid * * pointer, GLint ptrstride);
extern PFN_GLVERTEXPOINTERLISTIBM _glVertexPointerListIBM;

typedef void (APIENTRY * PFN_GLBLENDFUNCSEPARATEINGR)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern PFN_GLBLENDFUNCSEPARATEINGR _glBlendFuncSeparateINGR;

typedef void (APIENTRY * PFN_GLVERTEXPOINTERVINTEL)(GLint size, GLenum type, const GLvoid * * pointer);
extern PFN_GLVERTEXPOINTERVINTEL _glVertexPointervINTEL;

typedef void (APIENTRY * PFN_GLNORMALPOINTERVINTEL)(GLenum type, const GLvoid * * pointer);
extern PFN_GLNORMALPOINTERVINTEL _glNormalPointervINTEL;

typedef void (APIENTRY * PFN_GLCOLORPOINTERVINTEL)(GLint size, GLenum type, const GLvoid * * pointer);
extern PFN_GLCOLORPOINTERVINTEL _glColorPointervINTEL;

typedef void (APIENTRY * PFN_GLTEXCOORDPOINTERVINTEL)(GLint size, GLenum type, const GLvoid * * pointer);
extern PFN_GLTEXCOORDPOINTERVINTEL _glTexCoordPointervINTEL;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGECONTROL)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
extern PFN_GLDEBUGMESSAGECONTROL _glDebugMessageControl;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGEINSERT)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
extern PFN_GLDEBUGMESSAGEINSERT _glDebugMessageInsert;

typedef void (APIENTRY * PFN_GLDEBUGMESSAGECALLBACK)(GLDEBUGPROC callback, const void * userParam);
extern PFN_GLDEBUGMESSAGECALLBACK _glDebugMessageCallback;

typedef GLuint (APIENTRY * PFN_GLGETDEBUGMESSAGELOG)(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
extern PFN_GLGETDEBUGMESSAGELOG _glGetDebugMessageLog;

typedef void (APIENTRY * PFN_GLPUSHDEBUGGROUP)(GLenum source, GLuint id, GLsizei length, const GLchar * message);
extern PFN_GLPUSHDEBUGGROUP _glPushDebugGroup;

typedef void (APIENTRY * PFN_GLPOPDEBUGGROUP)(void);
extern PFN_GLPOPDEBUGGROUP _glPopDebugGroup;

typedef void (APIENTRY * PFN_GLOBJECTLABEL)(GLenum identifier, GLuint name, GLsizei length, const GLchar * label);
extern PFN_GLOBJECTLABEL _glObjectLabel;

typedef void (APIENTRY * PFN_GLGETOBJECTLABEL)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label);
extern PFN_GLGETOBJECTLABEL _glGetObjectLabel;

typedef void (APIENTRY * PFN_GLOBJECTPTRLABEL)(const void * ptr, GLsizei length, const GLchar * label);
extern PFN_GLOBJECTPTRLABEL _glObjectPtrLabel;

typedef void (APIENTRY * PFN_GLGETOBJECTPTRLABEL)(const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label);
extern PFN_GLGETOBJECTPTRLABEL _glGetObjectPtrLabel;

typedef GLenum (APIENTRY * PFN_GLGETGRAPHICSRESETSTATUS)(void);
extern PFN_GLGETGRAPHICSRESETSTATUS _glGetGraphicsResetStatus;

typedef void (APIENTRY * PFN_GLREADNPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid * data);
extern PFN_GLREADNPIXELS _glReadnPixels;

typedef void (APIENTRY * PFN_GLGETNUNIFORMFV)(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
extern PFN_GLGETNUNIFORMFV _glGetnUniformfv;

typedef void (APIENTRY * PFN_GLGETNUNIFORMIV)(GLuint program, GLint location, GLsizei bufSize, GLint * params);
extern PFN_GLGETNUNIFORMIV _glGetnUniformiv;

typedef void (APIENTRY * PFN_GLGETNUNIFORMUIV)(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
extern PFN_GLGETNUNIFORMUIV _glGetnUniformuiv;

typedef GLuint (APIENTRY * PFN_GLNEWBUFFERREGION)(GLenum type);
extern PFN_GLNEWBUFFERREGION _glNewBufferRegion;

typedef void (APIENTRY * PFN_GLDELETEBUFFERREGION)(GLuint region);
extern PFN_GLDELETEBUFFERREGION _glDeleteBufferRegion;

typedef void (APIENTRY * PFN_GLREADBUFFERREGION)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLREADBUFFERREGION _glReadBufferRegion;

typedef void (APIENTRY * PFN_GLDRAWBUFFERREGION)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
extern PFN_GLDRAWBUFFERREGION _glDrawBufferRegion;

typedef GLuint (APIENTRY * PFN_GLBUFFERREGIONENABLED)(void);
extern PFN_GLBUFFERREGIONENABLED _glBufferRegionEnabled;

typedef void (APIENTRY * PFN_GLRESIZEBUFFERSMESA)(void);
extern PFN_GLRESIZEBUFFERSMESA _glResizeBuffersMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2DMESA)(GLdouble x, GLdouble y);
extern PFN_GLWINDOWPOS2DMESA _glWindowPos2dMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2DVMESA)(const GLdouble * v);
extern PFN_GLWINDOWPOS2DVMESA _glWindowPos2dvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2FMESA)(GLfloat x, GLfloat y);
extern PFN_GLWINDOWPOS2FMESA _glWindowPos2fMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2FVMESA)(const GLfloat * v);
extern PFN_GLWINDOWPOS2FVMESA _glWindowPos2fvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2IMESA)(GLint x, GLint y);
extern PFN_GLWINDOWPOS2IMESA _glWindowPos2iMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2IVMESA)(const GLint * v);
extern PFN_GLWINDOWPOS2IVMESA _glWindowPos2ivMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2SMESA)(GLshort x, GLshort y);
extern PFN_GLWINDOWPOS2SMESA _glWindowPos2sMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS2SVMESA)(const GLshort * v);
extern PFN_GLWINDOWPOS2SVMESA _glWindowPos2svMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3DMESA)(GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLWINDOWPOS3DMESA _glWindowPos3dMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3DVMESA)(const GLdouble * v);
extern PFN_GLWINDOWPOS3DVMESA _glWindowPos3dvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3FMESA)(GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLWINDOWPOS3FMESA _glWindowPos3fMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3FVMESA)(const GLfloat * v);
extern PFN_GLWINDOWPOS3FVMESA _glWindowPos3fvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3IMESA)(GLint x, GLint y, GLint z);
extern PFN_GLWINDOWPOS3IMESA _glWindowPos3iMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3IVMESA)(const GLint * v);
extern PFN_GLWINDOWPOS3IVMESA _glWindowPos3ivMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3SMESA)(GLshort x, GLshort y, GLshort z);
extern PFN_GLWINDOWPOS3SMESA _glWindowPos3sMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS3SVMESA)(const GLshort * v);
extern PFN_GLWINDOWPOS3SVMESA _glWindowPos3svMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4DMESA)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLWINDOWPOS4DMESA _glWindowPos4dMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4DVMESA)(const GLdouble * v);
extern PFN_GLWINDOWPOS4DVMESA _glWindowPos4dvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4FMESA)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLWINDOWPOS4FMESA _glWindowPos4fMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4FVMESA)(const GLfloat * v);
extern PFN_GLWINDOWPOS4FVMESA _glWindowPos4fvMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4IMESA)(GLint x, GLint y, GLint z, GLint w);
extern PFN_GLWINDOWPOS4IMESA _glWindowPos4iMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4IVMESA)(const GLint * v);
extern PFN_GLWINDOWPOS4IVMESA _glWindowPos4ivMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4SMESA)(GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLWINDOWPOS4SMESA _glWindowPos4sMESA;

typedef void (APIENTRY * PFN_GLWINDOWPOS4SVMESA)(const GLshort * v);
extern PFN_GLWINDOWPOS4SVMESA _glWindowPos4svMESA;

typedef void (APIENTRY * PFN_GLBEGINCONDITIONALRENDERNVX)(GLuint id);
extern PFN_GLBEGINCONDITIONALRENDERNVX _glBeginConditionalRenderNVX;

typedef void (APIENTRY * PFN_GLENDCONDITIONALRENDERNVX)(void);
extern PFN_GLENDCONDITIONALRENDERNVX _glEndConditionalRenderNVX;

typedef void (APIENTRY * PFN_GLMULTIDRAWARRAYSINDIRECTBINDLESSNV)(GLenum mode, const GLvoid * indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
extern PFN_GLMULTIDRAWARRAYSINDIRECTBINDLESSNV _glMultiDrawArraysIndirectBindlessNV;

typedef void (APIENTRY * PFN_GLMULTIDRAWELEMENTSINDIRECTBINDLESSNV)(GLenum mode, GLenum type, const GLvoid * indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
extern PFN_GLMULTIDRAWELEMENTSINDIRECTBINDLESSNV _glMultiDrawElementsIndirectBindlessNV;

typedef GLuint64 (APIENTRY * PFN_GLGETTEXTUREHANDLENV)(GLuint texture);
extern PFN_GLGETTEXTUREHANDLENV _glGetTextureHandleNV;

typedef GLuint64 (APIENTRY * PFN_GLGETTEXTURESAMPLERHANDLENV)(GLuint texture, GLuint sampler);
extern PFN_GLGETTEXTURESAMPLERHANDLENV _glGetTextureSamplerHandleNV;

typedef void (APIENTRY * PFN_GLMAKETEXTUREHANDLERESIDENTNV)(GLuint64 handle);
extern PFN_GLMAKETEXTUREHANDLERESIDENTNV _glMakeTextureHandleResidentNV;

typedef void (APIENTRY * PFN_GLMAKETEXTUREHANDLENONRESIDENTNV)(GLuint64 handle);
extern PFN_GLMAKETEXTUREHANDLENONRESIDENTNV _glMakeTextureHandleNonResidentNV;

typedef GLuint64 (APIENTRY * PFN_GLGETIMAGEHANDLENV)(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
extern PFN_GLGETIMAGEHANDLENV _glGetImageHandleNV;

typedef void (APIENTRY * PFN_GLMAKEIMAGEHANDLERESIDENTNV)(GLuint64 handle, GLenum access);
extern PFN_GLMAKEIMAGEHANDLERESIDENTNV _glMakeImageHandleResidentNV;

typedef void (APIENTRY * PFN_GLMAKEIMAGEHANDLENONRESIDENTNV)(GLuint64 handle);
extern PFN_GLMAKEIMAGEHANDLENONRESIDENTNV _glMakeImageHandleNonResidentNV;

typedef void (APIENTRY * PFN_GLUNIFORMHANDLEUI64NV)(GLint location, GLuint64 value);
extern PFN_GLUNIFORMHANDLEUI64NV _glUniformHandleui64NV;

typedef void (APIENTRY * PFN_GLUNIFORMHANDLEUI64VNV)(GLint location, GLsizei count, const GLuint64 * value);
extern PFN_GLUNIFORMHANDLEUI64VNV _glUniformHandleui64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMHANDLEUI64NV)(GLuint program, GLint location, GLuint64 value);
extern PFN_GLPROGRAMUNIFORMHANDLEUI64NV _glProgramUniformHandleui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMHANDLEUI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64 * values);
extern PFN_GLPROGRAMUNIFORMHANDLEUI64VNV _glProgramUniformHandleui64vNV;

typedef GLboolean (APIENTRY * PFN_GLISTEXTUREHANDLERESIDENTNV)(GLuint64 handle);
extern PFN_GLISTEXTUREHANDLERESIDENTNV _glIsTextureHandleResidentNV;

typedef GLboolean (APIENTRY * PFN_GLISIMAGEHANDLERESIDENTNV)(GLuint64 handle);
extern PFN_GLISIMAGEHANDLERESIDENTNV _glIsImageHandleResidentNV;

typedef void (APIENTRY * PFN_GLBLENDPARAMETERINV)(GLenum pname, GLint value);
extern PFN_GLBLENDPARAMETERINV _glBlendParameteriNV;

typedef void (APIENTRY * PFN_GLBLENDBARRIERNV)(void);
extern PFN_GLBLENDBARRIERNV _glBlendBarrierNV;

typedef void (APIENTRY * PFN_GLBEGINCONDITIONALRENDERNV)(GLuint id, GLenum mode);
extern PFN_GLBEGINCONDITIONALRENDERNV _glBeginConditionalRenderNV;

typedef void (APIENTRY * PFN_GLENDCONDITIONALRENDERNV)(void);
extern PFN_GLENDCONDITIONALRENDERNV _glEndConditionalRenderNV;

typedef void (APIENTRY * PFN_GLCOPYIMAGESUBDATANV)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLCOPYIMAGESUBDATANV _glCopyImageSubDataNV;

typedef void (APIENTRY * PFN_GLDEPTHRANGEDNV)(GLdouble zNear, GLdouble zFar);
extern PFN_GLDEPTHRANGEDNV _glDepthRangedNV;

typedef void (APIENTRY * PFN_GLCLEARDEPTHDNV)(GLdouble depth);
extern PFN_GLCLEARDEPTHDNV _glClearDepthdNV;

typedef void (APIENTRY * PFN_GLDEPTHBOUNDSDNV)(GLdouble zmin, GLdouble zmax);
extern PFN_GLDEPTHBOUNDSDNV _glDepthBoundsdNV;

typedef void (APIENTRY * PFN_GLDRAWTEXTURENV)(GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1);
extern PFN_GLDRAWTEXTURENV _glDrawTextureNV;

typedef void (APIENTRY * PFN_GLMAPCONTROLPOINTSNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid * points);
extern PFN_GLMAPCONTROLPOINTSNV _glMapControlPointsNV;

typedef void (APIENTRY * PFN_GLMAPPARAMETERIVNV)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLMAPPARAMETERIVNV _glMapParameterivNV;

typedef void (APIENTRY * PFN_GLMAPPARAMETERFVNV)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLMAPPARAMETERFVNV _glMapParameterfvNV;

typedef void (APIENTRY * PFN_GLGETMAPCONTROLPOINTSNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid * points);
extern PFN_GLGETMAPCONTROLPOINTSNV _glGetMapControlPointsNV;

typedef void (APIENTRY * PFN_GLGETMAPPARAMETERIVNV)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETMAPPARAMETERIVNV _glGetMapParameterivNV;

typedef void (APIENTRY * PFN_GLGETMAPPARAMETERFVNV)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETMAPPARAMETERFVNV _glGetMapParameterfvNV;

typedef void (APIENTRY * PFN_GLGETMAPATTRIBPARAMETERIVNV)(GLenum target, GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETMAPATTRIBPARAMETERIVNV _glGetMapAttribParameterivNV;

typedef void (APIENTRY * PFN_GLGETMAPATTRIBPARAMETERFVNV)(GLenum target, GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETMAPATTRIBPARAMETERFVNV _glGetMapAttribParameterfvNV;

typedef void (APIENTRY * PFN_GLEVALMAPSNV)(GLenum target, GLenum mode);
extern PFN_GLEVALMAPSNV _glEvalMapsNV;

typedef void (APIENTRY * PFN_GLGETMULTISAMPLEFVNV)(GLenum pname, GLuint index, GLfloat * val);
extern PFN_GLGETMULTISAMPLEFVNV _glGetMultisamplefvNV;

typedef void (APIENTRY * PFN_GLSAMPLEMASKINDEXEDNV)(GLuint index, GLbitfield mask);
extern PFN_GLSAMPLEMASKINDEXEDNV _glSampleMaskIndexedNV;

typedef void (APIENTRY * PFN_GLTEXRENDERBUFFERNV)(GLenum target, GLuint renderbuffer);
extern PFN_GLTEXRENDERBUFFERNV _glTexRenderbufferNV;

typedef void (APIENTRY * PFN_GLDELETEFENCESNV)(GLsizei n, const GLuint * fences);
extern PFN_GLDELETEFENCESNV _glDeleteFencesNV;

typedef void (APIENTRY * PFN_GLGENFENCESNV)(GLsizei n, GLuint * fences);
extern PFN_GLGENFENCESNV _glGenFencesNV;

typedef GLboolean (APIENTRY * PFN_GLISFENCENV)(GLuint fence);
extern PFN_GLISFENCENV _glIsFenceNV;

typedef GLboolean (APIENTRY * PFN_GLTESTFENCENV)(GLuint fence);
extern PFN_GLTESTFENCENV _glTestFenceNV;

typedef void (APIENTRY * PFN_GLGETFENCEIVNV)(GLuint fence, GLenum pname, GLint * params);
extern PFN_GLGETFENCEIVNV _glGetFenceivNV;

typedef void (APIENTRY * PFN_GLFINISHFENCENV)(GLuint fence);
extern PFN_GLFINISHFENCENV _glFinishFenceNV;

typedef void (APIENTRY * PFN_GLSETFENCENV)(GLuint fence, GLenum condition);
extern PFN_GLSETFENCENV _glSetFenceNV;

typedef void (APIENTRY * PFN_GLPROGRAMNAMEDPARAMETER4FNV)(GLuint id, GLsizei len, const GLubyte * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLPROGRAMNAMEDPARAMETER4FNV _glProgramNamedParameter4fNV;

typedef void (APIENTRY * PFN_GLPROGRAMNAMEDPARAMETER4DNV)(GLuint id, GLsizei len, const GLubyte * name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLPROGRAMNAMEDPARAMETER4DNV _glProgramNamedParameter4dNV;

typedef void (APIENTRY * PFN_GLPROGRAMNAMEDPARAMETER4FVNV)(GLuint id, GLsizei len, const GLubyte * name, const GLfloat * v);
extern PFN_GLPROGRAMNAMEDPARAMETER4FVNV _glProgramNamedParameter4fvNV;

typedef void (APIENTRY * PFN_GLPROGRAMNAMEDPARAMETER4DVNV)(GLuint id, GLsizei len, const GLubyte * name, const GLdouble * v);
extern PFN_GLPROGRAMNAMEDPARAMETER4DVNV _glProgramNamedParameter4dvNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMNAMEDPARAMETERDVNV)(GLuint id, GLsizei len, const GLubyte * name, GLdouble * params);
extern PFN_GLGETPROGRAMNAMEDPARAMETERDVNV _glGetProgramNamedParameterdvNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMNAMEDPARAMETERFVNV)(GLuint id, GLsizei len, const GLubyte * name, GLfloat * params);
extern PFN_GLGETPROGRAMNAMEDPARAMETERFVNV _glGetProgramNamedParameterfvNV;

typedef void (APIENTRY * PFN_GLBLITFRAMEBUFFERNV)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFN_GLBLITFRAMEBUFFERNV _glBlitFramebufferNV;

typedef void (APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENV)(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENV _glRenderbufferStorageMultisampleCoverageNV;

typedef void (APIENTRY * PFN_GLPROGRAMVERTEXLIMITNV)(GLenum target, GLint limit);
extern PFN_GLPROGRAMVERTEXLIMITNV _glProgramVertexLimitNV;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTUREEXT)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTUREEXT _glFramebufferTextureEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTURELAYEREXT)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFN_GLFRAMEBUFFERTEXTURELAYEREXT _glFramebufferTextureLayerEXT;

typedef void (APIENTRY * PFN_GLFRAMEBUFFERTEXTUREFACEEXT)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
extern PFN_GLFRAMEBUFFERTEXTUREFACEEXT _glFramebufferTextureFaceEXT;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERI4INV)(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLPROGRAMLOCALPARAMETERI4INV _glProgramLocalParameterI4iNV;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERI4IVNV)(GLenum target, GLuint index, const GLint * params);
extern PFN_GLPROGRAMLOCALPARAMETERI4IVNV _glProgramLocalParameterI4ivNV;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERSI4IVNV)(GLenum target, GLuint index, GLsizei count, const GLint * params);
extern PFN_GLPROGRAMLOCALPARAMETERSI4IVNV _glProgramLocalParametersI4ivNV;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERI4UINV)(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFN_GLPROGRAMLOCALPARAMETERI4UINV _glProgramLocalParameterI4uiNV;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERI4UIVNV)(GLenum target, GLuint index, const GLuint * params);
extern PFN_GLPROGRAMLOCALPARAMETERI4UIVNV _glProgramLocalParameterI4uivNV;

typedef void (APIENTRY * PFN_GLPROGRAMLOCALPARAMETERSI4UIVNV)(GLenum target, GLuint index, GLsizei count, const GLuint * params);
extern PFN_GLPROGRAMLOCALPARAMETERSI4UIVNV _glProgramLocalParametersI4uivNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERI4INV)(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLPROGRAMENVPARAMETERI4INV _glProgramEnvParameterI4iNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERI4IVNV)(GLenum target, GLuint index, const GLint * params);
extern PFN_GLPROGRAMENVPARAMETERI4IVNV _glProgramEnvParameterI4ivNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERSI4IVNV)(GLenum target, GLuint index, GLsizei count, const GLint * params);
extern PFN_GLPROGRAMENVPARAMETERSI4IVNV _glProgramEnvParametersI4ivNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERI4UINV)(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFN_GLPROGRAMENVPARAMETERI4UINV _glProgramEnvParameterI4uiNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERI4UIVNV)(GLenum target, GLuint index, const GLuint * params);
extern PFN_GLPROGRAMENVPARAMETERI4UIVNV _glProgramEnvParameterI4uivNV;

typedef void (APIENTRY * PFN_GLPROGRAMENVPARAMETERSI4UIVNV)(GLenum target, GLuint index, GLsizei count, const GLuint * params);
extern PFN_GLPROGRAMENVPARAMETERSI4UIVNV _glProgramEnvParametersI4uivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMLOCALPARAMETERIIVNV)(GLenum target, GLuint index, GLint * params);
extern PFN_GLGETPROGRAMLOCALPARAMETERIIVNV _glGetProgramLocalParameterIivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMLOCALPARAMETERIUIVNV)(GLenum target, GLuint index, GLuint * params);
extern PFN_GLGETPROGRAMLOCALPARAMETERIUIVNV _glGetProgramLocalParameterIuivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMENVPARAMETERIIVNV)(GLenum target, GLuint index, GLint * params);
extern PFN_GLGETPROGRAMENVPARAMETERIIVNV _glGetProgramEnvParameterIivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMENVPARAMETERIUIVNV)(GLenum target, GLuint index, GLuint * params);
extern PFN_GLGETPROGRAMENVPARAMETERIUIVNV _glGetProgramEnvParameterIuivNV;

typedef void (APIENTRY * PFN_GLPROGRAMSUBROUTINEPARAMETERSUIVNV)(GLenum target, GLsizei count, const GLuint * params);
extern PFN_GLPROGRAMSUBROUTINEPARAMETERSUIVNV _glProgramSubroutineParametersuivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMSUBROUTINEPARAMETERUIVNV)(GLenum target, GLuint index, GLuint * param);
extern PFN_GLGETPROGRAMSUBROUTINEPARAMETERUIVNV _glGetProgramSubroutineParameteruivNV;

typedef void (APIENTRY * PFN_GLUNIFORM1I64NV)(GLint location, GLint64EXT x);
extern PFN_GLUNIFORM1I64NV _glUniform1i64NV;

typedef void (APIENTRY * PFN_GLUNIFORM2I64NV)(GLint location, GLint64EXT x, GLint64EXT y);
extern PFN_GLUNIFORM2I64NV _glUniform2i64NV;

typedef void (APIENTRY * PFN_GLUNIFORM3I64NV)(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern PFN_GLUNIFORM3I64NV _glUniform3i64NV;

typedef void (APIENTRY * PFN_GLUNIFORM4I64NV)(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern PFN_GLUNIFORM4I64NV _glUniform4i64NV;

typedef void (APIENTRY * PFN_GLUNIFORM1I64VNV)(GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLUNIFORM1I64VNV _glUniform1i64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM2I64VNV)(GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLUNIFORM2I64VNV _glUniform2i64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM3I64VNV)(GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLUNIFORM3I64VNV _glUniform3i64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM4I64VNV)(GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLUNIFORM4I64VNV _glUniform4i64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM1UI64NV)(GLint location, GLuint64EXT x);
extern PFN_GLUNIFORM1UI64NV _glUniform1ui64NV;

typedef void (APIENTRY * PFN_GLUNIFORM2UI64NV)(GLint location, GLuint64EXT x, GLuint64EXT y);
extern PFN_GLUNIFORM2UI64NV _glUniform2ui64NV;

typedef void (APIENTRY * PFN_GLUNIFORM3UI64NV)(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern PFN_GLUNIFORM3UI64NV _glUniform3ui64NV;

typedef void (APIENTRY * PFN_GLUNIFORM4UI64NV)(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern PFN_GLUNIFORM4UI64NV _glUniform4ui64NV;

typedef void (APIENTRY * PFN_GLUNIFORM1UI64VNV)(GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLUNIFORM1UI64VNV _glUniform1ui64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM2UI64VNV)(GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLUNIFORM2UI64VNV _glUniform2ui64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM3UI64VNV)(GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLUNIFORM3UI64VNV _glUniform3ui64vNV;

typedef void (APIENTRY * PFN_GLUNIFORM4UI64VNV)(GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLUNIFORM4UI64VNV _glUniform4ui64vNV;

typedef void (APIENTRY * PFN_GLGETUNIFORMI64VNV)(GLuint program, GLint location, GLint64EXT * params);
extern PFN_GLGETUNIFORMI64VNV _glGetUniformi64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1I64NV)(GLuint program, GLint location, GLint64EXT x);
extern PFN_GLPROGRAMUNIFORM1I64NV _glProgramUniform1i64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2I64NV)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
extern PFN_GLPROGRAMUNIFORM2I64NV _glProgramUniform2i64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3I64NV)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern PFN_GLPROGRAMUNIFORM3I64NV _glProgramUniform3i64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4I64NV)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern PFN_GLPROGRAMUNIFORM4I64NV _glProgramUniform4i64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1I64VNV)(GLuint program, GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLPROGRAMUNIFORM1I64VNV _glProgramUniform1i64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2I64VNV)(GLuint program, GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLPROGRAMUNIFORM2I64VNV _glProgramUniform2i64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3I64VNV)(GLuint program, GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLPROGRAMUNIFORM3I64VNV _glProgramUniform3i64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4I64VNV)(GLuint program, GLint location, GLsizei count, const GLint64EXT * value);
extern PFN_GLPROGRAMUNIFORM4I64VNV _glProgramUniform4i64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UI64NV)(GLuint program, GLint location, GLuint64EXT x);
extern PFN_GLPROGRAMUNIFORM1UI64NV _glProgramUniform1ui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UI64NV)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
extern PFN_GLPROGRAMUNIFORM2UI64NV _glProgramUniform2ui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UI64NV)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern PFN_GLPROGRAMUNIFORM3UI64NV _glProgramUniform3ui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UI64NV)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern PFN_GLPROGRAMUNIFORM4UI64NV _glProgramUniform4ui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM1UI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLPROGRAMUNIFORM1UI64VNV _glProgramUniform1ui64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM2UI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLPROGRAMUNIFORM2UI64VNV _glProgramUniform2ui64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM3UI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLPROGRAMUNIFORM3UI64VNV _glProgramUniform3ui64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORM4UI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLPROGRAMUNIFORM4UI64VNV _glProgramUniform4ui64vNV;

typedef void (APIENTRY * PFN_GLVERTEX2HNV)(GLhalfNV x, GLhalfNV y);
extern PFN_GLVERTEX2HNV _glVertex2hNV;

typedef void (APIENTRY * PFN_GLVERTEX2HVNV)(const GLhalfNV * v);
extern PFN_GLVERTEX2HVNV _glVertex2hvNV;

typedef void (APIENTRY * PFN_GLVERTEX3HNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z);
extern PFN_GLVERTEX3HNV _glVertex3hNV;

typedef void (APIENTRY * PFN_GLVERTEX3HVNV)(const GLhalfNV * v);
extern PFN_GLVERTEX3HVNV _glVertex3hvNV;

typedef void (APIENTRY * PFN_GLVERTEX4HNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
extern PFN_GLVERTEX4HNV _glVertex4hNV;

typedef void (APIENTRY * PFN_GLVERTEX4HVNV)(const GLhalfNV * v);
extern PFN_GLVERTEX4HVNV _glVertex4hvNV;

typedef void (APIENTRY * PFN_GLNORMAL3HNV)(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
extern PFN_GLNORMAL3HNV _glNormal3hNV;

typedef void (APIENTRY * PFN_GLNORMAL3HVNV)(const GLhalfNV * v);
extern PFN_GLNORMAL3HVNV _glNormal3hvNV;

typedef void (APIENTRY * PFN_GLCOLOR3HNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
extern PFN_GLCOLOR3HNV _glColor3hNV;

typedef void (APIENTRY * PFN_GLCOLOR3HVNV)(const GLhalfNV * v);
extern PFN_GLCOLOR3HVNV _glColor3hvNV;

typedef void (APIENTRY * PFN_GLCOLOR4HNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
extern PFN_GLCOLOR4HNV _glColor4hNV;

typedef void (APIENTRY * PFN_GLCOLOR4HVNV)(const GLhalfNV * v);
extern PFN_GLCOLOR4HVNV _glColor4hvNV;

typedef void (APIENTRY * PFN_GLTEXCOORD1HNV)(GLhalfNV s);
extern PFN_GLTEXCOORD1HNV _glTexCoord1hNV;

typedef void (APIENTRY * PFN_GLTEXCOORD1HVNV)(const GLhalfNV * v);
extern PFN_GLTEXCOORD1HVNV _glTexCoord1hvNV;

typedef void (APIENTRY * PFN_GLTEXCOORD2HNV)(GLhalfNV s, GLhalfNV t);
extern PFN_GLTEXCOORD2HNV _glTexCoord2hNV;

typedef void (APIENTRY * PFN_GLTEXCOORD2HVNV)(const GLhalfNV * v);
extern PFN_GLTEXCOORD2HVNV _glTexCoord2hvNV;

typedef void (APIENTRY * PFN_GLTEXCOORD3HNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r);
extern PFN_GLTEXCOORD3HNV _glTexCoord3hNV;

typedef void (APIENTRY * PFN_GLTEXCOORD3HVNV)(const GLhalfNV * v);
extern PFN_GLTEXCOORD3HVNV _glTexCoord3hvNV;

typedef void (APIENTRY * PFN_GLTEXCOORD4HNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
extern PFN_GLTEXCOORD4HNV _glTexCoord4hNV;

typedef void (APIENTRY * PFN_GLTEXCOORD4HVNV)(const GLhalfNV * v);
extern PFN_GLTEXCOORD4HVNV _glTexCoord4hvNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1HNV)(GLenum target, GLhalfNV s);
extern PFN_GLMULTITEXCOORD1HNV _glMultiTexCoord1hNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD1HVNV)(GLenum target, const GLhalfNV * v);
extern PFN_GLMULTITEXCOORD1HVNV _glMultiTexCoord1hvNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2HNV)(GLenum target, GLhalfNV s, GLhalfNV t);
extern PFN_GLMULTITEXCOORD2HNV _glMultiTexCoord2hNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD2HVNV)(GLenum target, const GLhalfNV * v);
extern PFN_GLMULTITEXCOORD2HVNV _glMultiTexCoord2hvNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3HNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
extern PFN_GLMULTITEXCOORD3HNV _glMultiTexCoord3hNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD3HVNV)(GLenum target, const GLhalfNV * v);
extern PFN_GLMULTITEXCOORD3HVNV _glMultiTexCoord3hvNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4HNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
extern PFN_GLMULTITEXCOORD4HNV _glMultiTexCoord4hNV;

typedef void (APIENTRY * PFN_GLMULTITEXCOORD4HVNV)(GLenum target, const GLhalfNV * v);
extern PFN_GLMULTITEXCOORD4HVNV _glMultiTexCoord4hvNV;

typedef void (APIENTRY * PFN_GLFOGCOORDHNV)(GLhalfNV fog);
extern PFN_GLFOGCOORDHNV _glFogCoordhNV;

typedef void (APIENTRY * PFN_GLFOGCOORDHVNV)(const GLhalfNV * fog);
extern PFN_GLFOGCOORDHVNV _glFogCoordhvNV;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3HNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
extern PFN_GLSECONDARYCOLOR3HNV _glSecondaryColor3hNV;

typedef void (APIENTRY * PFN_GLSECONDARYCOLOR3HVNV)(const GLhalfNV * v);
extern PFN_GLSECONDARYCOLOR3HVNV _glSecondaryColor3hvNV;

typedef void (APIENTRY * PFN_GLVERTEXWEIGHTHNV)(GLhalfNV weight);
extern PFN_GLVERTEXWEIGHTHNV _glVertexWeighthNV;

typedef void (APIENTRY * PFN_GLVERTEXWEIGHTHVNV)(const GLhalfNV * weight);
extern PFN_GLVERTEXWEIGHTHVNV _glVertexWeighthvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1HNV)(GLuint index, GLhalfNV x);
extern PFN_GLVERTEXATTRIB1HNV _glVertexAttrib1hNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1HVNV)(GLuint index, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIB1HVNV _glVertexAttrib1hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2HNV)(GLuint index, GLhalfNV x, GLhalfNV y);
extern PFN_GLVERTEXATTRIB2HNV _glVertexAttrib2hNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2HVNV)(GLuint index, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIB2HVNV _glVertexAttrib2hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3HNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
extern PFN_GLVERTEXATTRIB3HNV _glVertexAttrib3hNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3HVNV)(GLuint index, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIB3HVNV _glVertexAttrib3hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4HNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
extern PFN_GLVERTEXATTRIB4HNV _glVertexAttrib4hNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4HVNV)(GLuint index, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIB4HVNV _glVertexAttrib4hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS1HVNV)(GLuint index, GLsizei n, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIBS1HVNV _glVertexAttribs1hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS2HVNV)(GLuint index, GLsizei n, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIBS2HVNV _glVertexAttribs2hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS3HVNV)(GLuint index, GLsizei n, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIBS3HVNV _glVertexAttribs3hvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS4HVNV)(GLuint index, GLsizei n, const GLhalfNV * v);
extern PFN_GLVERTEXATTRIBS4HVNV _glVertexAttribs4hvNV;

typedef void (APIENTRY * PFN_GLGENOCCLUSIONQUERIESNV)(GLsizei n, GLuint * ids);
extern PFN_GLGENOCCLUSIONQUERIESNV _glGenOcclusionQueriesNV;

typedef void (APIENTRY * PFN_GLDELETEOCCLUSIONQUERIESNV)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETEOCCLUSIONQUERIESNV _glDeleteOcclusionQueriesNV;

typedef GLboolean (APIENTRY * PFN_GLISOCCLUSIONQUERYNV)(GLuint id);
extern PFN_GLISOCCLUSIONQUERYNV _glIsOcclusionQueryNV;

typedef void (APIENTRY * PFN_GLBEGINOCCLUSIONQUERYNV)(GLuint id);
extern PFN_GLBEGINOCCLUSIONQUERYNV _glBeginOcclusionQueryNV;

typedef void (APIENTRY * PFN_GLENDOCCLUSIONQUERYNV)(void);
extern PFN_GLENDOCCLUSIONQUERYNV _glEndOcclusionQueryNV;

typedef void (APIENTRY * PFN_GLGETOCCLUSIONQUERYIVNV)(GLuint id, GLenum pname, GLint * params);
extern PFN_GLGETOCCLUSIONQUERYIVNV _glGetOcclusionQueryivNV;

typedef void (APIENTRY * PFN_GLGETOCCLUSIONQUERYUIVNV)(GLuint id, GLenum pname, GLuint * params);
extern PFN_GLGETOCCLUSIONQUERYUIVNV _glGetOcclusionQueryuivNV;

typedef void (APIENTRY * PFN_GLPROGRAMBUFFERPARAMETERSFVNV)(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat * params);
extern PFN_GLPROGRAMBUFFERPARAMETERSFVNV _glProgramBufferParametersfvNV;

typedef void (APIENTRY * PFN_GLPROGRAMBUFFERPARAMETERSIIVNV)(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint * params);
extern PFN_GLPROGRAMBUFFERPARAMETERSIIVNV _glProgramBufferParametersIivNV;

typedef void (APIENTRY * PFN_GLPROGRAMBUFFERPARAMETERSIUIVNV)(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint * params);
extern PFN_GLPROGRAMBUFFERPARAMETERSIUIVNV _glProgramBufferParametersIuivNV;

typedef GLuint (APIENTRY * PFN_GLGENPATHSNV)(GLsizei range);
extern PFN_GLGENPATHSNV _glGenPathsNV;

typedef void (APIENTRY * PFN_GLDELETEPATHSNV)(GLuint path, GLsizei range);
extern PFN_GLDELETEPATHSNV _glDeletePathsNV;

typedef GLboolean (APIENTRY * PFN_GLISPATHNV)(GLuint path);
extern PFN_GLISPATHNV _glIsPathNV;

typedef void (APIENTRY * PFN_GLPATHCOMMANDSNV)(GLuint path, GLsizei numCommands, const GLubyte * commands, GLsizei numCoords, GLenum coordType, const GLvoid * coords);
extern PFN_GLPATHCOMMANDSNV _glPathCommandsNV;

typedef void (APIENTRY * PFN_GLPATHCOORDSNV)(GLuint path, GLsizei numCoords, GLenum coordType, const GLvoid * coords);
extern PFN_GLPATHCOORDSNV _glPathCoordsNV;

typedef void (APIENTRY * PFN_GLPATHSUBCOMMANDSNV)(GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte * commands, GLsizei numCoords, GLenum coordType, const GLvoid * coords);
extern PFN_GLPATHSUBCOMMANDSNV _glPathSubCommandsNV;

typedef void (APIENTRY * PFN_GLPATHSUBCOORDSNV)(GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const GLvoid * coords);
extern PFN_GLPATHSUBCOORDSNV _glPathSubCoordsNV;

typedef void (APIENTRY * PFN_GLPATHSTRINGNV)(GLuint path, GLenum format, GLsizei length, const GLvoid * pathString);
extern PFN_GLPATHSTRINGNV _glPathStringNV;

typedef void (APIENTRY * PFN_GLPATHGLYPHSNV)(GLuint firstPathName, GLenum fontTarget, const GLvoid * fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid * charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
extern PFN_GLPATHGLYPHSNV _glPathGlyphsNV;

typedef void (APIENTRY * PFN_GLPATHGLYPHRANGENV)(GLuint firstPathName, GLenum fontTarget, const GLvoid * fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
extern PFN_GLPATHGLYPHRANGENV _glPathGlyphRangeNV;

typedef void (APIENTRY * PFN_GLWEIGHTPATHSNV)(GLuint resultPath, GLsizei numPaths, const GLuint * paths, const GLfloat * weights);
extern PFN_GLWEIGHTPATHSNV _glWeightPathsNV;

typedef void (APIENTRY * PFN_GLCOPYPATHNV)(GLuint resultPath, GLuint srcPath);
extern PFN_GLCOPYPATHNV _glCopyPathNV;

typedef void (APIENTRY * PFN_GLINTERPOLATEPATHSNV)(GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
extern PFN_GLINTERPOLATEPATHSNV _glInterpolatePathsNV;

typedef void (APIENTRY * PFN_GLTRANSFORMPATHNV)(GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat * transformValues);
extern PFN_GLTRANSFORMPATHNV _glTransformPathNV;

typedef void (APIENTRY * PFN_GLPATHPARAMETERIVNV)(GLuint path, GLenum pname, const GLint * value);
extern PFN_GLPATHPARAMETERIVNV _glPathParameterivNV;

typedef void (APIENTRY * PFN_GLPATHPARAMETERINV)(GLuint path, GLenum pname, GLint value);
extern PFN_GLPATHPARAMETERINV _glPathParameteriNV;

typedef void (APIENTRY * PFN_GLPATHPARAMETERFVNV)(GLuint path, GLenum pname, const GLfloat * value);
extern PFN_GLPATHPARAMETERFVNV _glPathParameterfvNV;

typedef void (APIENTRY * PFN_GLPATHPARAMETERFNV)(GLuint path, GLenum pname, GLfloat value);
extern PFN_GLPATHPARAMETERFNV _glPathParameterfNV;

typedef void (APIENTRY * PFN_GLPATHDASHARRAYNV)(GLuint path, GLsizei dashCount, const GLfloat * dashArray);
extern PFN_GLPATHDASHARRAYNV _glPathDashArrayNV;

typedef void (APIENTRY * PFN_GLPATHSTENCILFUNCNV)(GLenum func, GLint ref, GLuint mask);
extern PFN_GLPATHSTENCILFUNCNV _glPathStencilFuncNV;

typedef void (APIENTRY * PFN_GLPATHSTENCILDEPTHOFFSETNV)(GLfloat factor, GLfloat units);
extern PFN_GLPATHSTENCILDEPTHOFFSETNV _glPathStencilDepthOffsetNV;

typedef void (APIENTRY * PFN_GLSTENCILFILLPATHNV)(GLuint path, GLenum fillMode, GLuint mask);
extern PFN_GLSTENCILFILLPATHNV _glStencilFillPathNV;

typedef void (APIENTRY * PFN_GLSTENCILSTROKEPATHNV)(GLuint path, GLint reference, GLuint mask);
extern PFN_GLSTENCILSTROKEPATHNV _glStencilStrokePathNV;

typedef void (APIENTRY * PFN_GLSTENCILFILLPATHINSTANCEDNV)(GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat * transformValues);
extern PFN_GLSTENCILFILLPATHINSTANCEDNV _glStencilFillPathInstancedNV;

typedef void (APIENTRY * PFN_GLSTENCILSTROKEPATHINSTANCEDNV)(GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat * transformValues);
extern PFN_GLSTENCILSTROKEPATHINSTANCEDNV _glStencilStrokePathInstancedNV;

typedef void (APIENTRY * PFN_GLPATHCOVERDEPTHFUNCNV)(GLenum zfunc);
extern PFN_GLPATHCOVERDEPTHFUNCNV _glPathCoverDepthFuncNV;

typedef void (APIENTRY * PFN_GLPATHCOLORGENNV)(GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat * coeffs);
extern PFN_GLPATHCOLORGENNV _glPathColorGenNV;

typedef void (APIENTRY * PFN_GLPATHTEXGENNV)(GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat * coeffs);
extern PFN_GLPATHTEXGENNV _glPathTexGenNV;

typedef void (APIENTRY * PFN_GLPATHFOGGENNV)(GLenum genMode);
extern PFN_GLPATHFOGGENNV _glPathFogGenNV;

typedef void (APIENTRY * PFN_GLCOVERFILLPATHNV)(GLuint path, GLenum coverMode);
extern PFN_GLCOVERFILLPATHNV _glCoverFillPathNV;

typedef void (APIENTRY * PFN_GLCOVERSTROKEPATHNV)(GLuint path, GLenum coverMode);
extern PFN_GLCOVERSTROKEPATHNV _glCoverStrokePathNV;

typedef void (APIENTRY * PFN_GLCOVERFILLPATHINSTANCEDNV)(GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat * transformValues);
extern PFN_GLCOVERFILLPATHINSTANCEDNV _glCoverFillPathInstancedNV;

typedef void (APIENTRY * PFN_GLCOVERSTROKEPATHINSTANCEDNV)(GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat * transformValues);
extern PFN_GLCOVERSTROKEPATHINSTANCEDNV _glCoverStrokePathInstancedNV;

typedef void (APIENTRY * PFN_GLGETPATHPARAMETERIVNV)(GLuint path, GLenum pname, GLint * value);
extern PFN_GLGETPATHPARAMETERIVNV _glGetPathParameterivNV;

typedef void (APIENTRY * PFN_GLGETPATHPARAMETERFVNV)(GLuint path, GLenum pname, GLfloat * value);
extern PFN_GLGETPATHPARAMETERFVNV _glGetPathParameterfvNV;

typedef void (APIENTRY * PFN_GLGETPATHCOMMANDSNV)(GLuint path, GLubyte * commands);
extern PFN_GLGETPATHCOMMANDSNV _glGetPathCommandsNV;

typedef void (APIENTRY * PFN_GLGETPATHCOORDSNV)(GLuint path, GLfloat * coords);
extern PFN_GLGETPATHCOORDSNV _glGetPathCoordsNV;

typedef void (APIENTRY * PFN_GLGETPATHDASHARRAYNV)(GLuint path, GLfloat * dashArray);
extern PFN_GLGETPATHDASHARRAYNV _glGetPathDashArrayNV;

typedef void (APIENTRY * PFN_GLGETPATHMETRICSNV)(GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLsizei stride, GLfloat * metrics);
extern PFN_GLGETPATHMETRICSNV _glGetPathMetricsNV;

typedef void (APIENTRY * PFN_GLGETPATHMETRICRANGENV)(GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat * metrics);
extern PFN_GLGETPATHMETRICRANGENV _glGetPathMetricRangeNV;

typedef void (APIENTRY * PFN_GLGETPATHSPACINGNV)(GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const GLvoid * paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat * returnedSpacing);
extern PFN_GLGETPATHSPACINGNV _glGetPathSpacingNV;

typedef void (APIENTRY * PFN_GLGETPATHCOLORGENIVNV)(GLenum color, GLenum pname, GLint * value);
extern PFN_GLGETPATHCOLORGENIVNV _glGetPathColorGenivNV;

typedef void (APIENTRY * PFN_GLGETPATHCOLORGENFVNV)(GLenum color, GLenum pname, GLfloat * value);
extern PFN_GLGETPATHCOLORGENFVNV _glGetPathColorGenfvNV;

typedef void (APIENTRY * PFN_GLGETPATHTEXGENIVNV)(GLenum texCoordSet, GLenum pname, GLint * value);
extern PFN_GLGETPATHTEXGENIVNV _glGetPathTexGenivNV;

typedef void (APIENTRY * PFN_GLGETPATHTEXGENFVNV)(GLenum texCoordSet, GLenum pname, GLfloat * value);
extern PFN_GLGETPATHTEXGENFVNV _glGetPathTexGenfvNV;

typedef GLboolean (APIENTRY * PFN_GLISPOINTINFILLPATHNV)(GLuint path, GLuint mask, GLfloat x, GLfloat y);
extern PFN_GLISPOINTINFILLPATHNV _glIsPointInFillPathNV;

typedef GLboolean (APIENTRY * PFN_GLISPOINTINSTROKEPATHNV)(GLuint path, GLfloat x, GLfloat y);
extern PFN_GLISPOINTINSTROKEPATHNV _glIsPointInStrokePathNV;

typedef GLfloat (APIENTRY * PFN_GLGETPATHLENGTHNV)(GLuint path, GLsizei startSegment, GLsizei numSegments);
extern PFN_GLGETPATHLENGTHNV _glGetPathLengthNV;

typedef GLboolean (APIENTRY * PFN_GLPOINTALONGPATHNV)(GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat * x, GLfloat * y, GLfloat * tangentX, GLfloat * tangentY);
extern PFN_GLPOINTALONGPATHNV _glPointAlongPathNV;

typedef void (APIENTRY * PFN_GLPIXELDATARANGENV)(GLenum target, GLsizei length, const GLvoid * pointer);
extern PFN_GLPIXELDATARANGENV _glPixelDataRangeNV;

typedef void (APIENTRY * PFN_GLFLUSHPIXELDATARANGENV)(GLenum target);
extern PFN_GLFLUSHPIXELDATARANGENV _glFlushPixelDataRangeNV;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERINV)(GLenum pname, GLint param);
extern PFN_GLPOINTPARAMETERINV _glPointParameteriNV;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERIVNV)(GLenum pname, const GLint * params);
extern PFN_GLPOINTPARAMETERIVNV _glPointParameterivNV;

typedef void (APIENTRY * PFN_GLPRESENTFRAMEKEYEDNV)(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
extern PFN_GLPRESENTFRAMEKEYEDNV _glPresentFrameKeyedNV;

typedef void (APIENTRY * PFN_GLPRESENTFRAMEDUALFILLNV)(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
extern PFN_GLPRESENTFRAMEDUALFILLNV _glPresentFrameDualFillNV;

typedef void (APIENTRY * PFN_GLGETVIDEOIVNV)(GLuint video_slot, GLenum pname, GLint * params);
extern PFN_GLGETVIDEOIVNV _glGetVideoivNV;

typedef void (APIENTRY * PFN_GLGETVIDEOUIVNV)(GLuint video_slot, GLenum pname, GLuint * params);
extern PFN_GLGETVIDEOUIVNV _glGetVideouivNV;

typedef void (APIENTRY * PFN_GLGETVIDEOI64VNV)(GLuint video_slot, GLenum pname, GLint64EXT * params);
extern PFN_GLGETVIDEOI64VNV _glGetVideoi64vNV;

typedef void (APIENTRY * PFN_GLGETVIDEOUI64VNV)(GLuint video_slot, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETVIDEOUI64VNV _glGetVideoui64vNV;

typedef void (APIENTRY * PFN_GLPRIMITIVERESTARTNV)(void);
extern PFN_GLPRIMITIVERESTARTNV _glPrimitiveRestartNV;

typedef void (APIENTRY * PFN_GLPRIMITIVERESTARTINDEXNV)(GLuint index);
extern PFN_GLPRIMITIVERESTARTINDEXNV _glPrimitiveRestartIndexNV;

typedef void (APIENTRY * PFN_GLCOMBINERPARAMETERFVNV)(GLenum pname, const GLfloat * params);
extern PFN_GLCOMBINERPARAMETERFVNV _glCombinerParameterfvNV;

typedef void (APIENTRY * PFN_GLCOMBINERPARAMETERFNV)(GLenum pname, GLfloat param);
extern PFN_GLCOMBINERPARAMETERFNV _glCombinerParameterfNV;

typedef void (APIENTRY * PFN_GLCOMBINERPARAMETERIVNV)(GLenum pname, const GLint * params);
extern PFN_GLCOMBINERPARAMETERIVNV _glCombinerParameterivNV;

typedef void (APIENTRY * PFN_GLCOMBINERPARAMETERINV)(GLenum pname, GLint param);
extern PFN_GLCOMBINERPARAMETERINV _glCombinerParameteriNV;

typedef void (APIENTRY * PFN_GLCOMBINERINPUTNV)(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern PFN_GLCOMBINERINPUTNV _glCombinerInputNV;

typedef void (APIENTRY * PFN_GLCOMBINEROUTPUTNV)(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
extern PFN_GLCOMBINEROUTPUTNV _glCombinerOutputNV;

typedef void (APIENTRY * PFN_GLFINALCOMBINERINPUTNV)(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern PFN_GLFINALCOMBINERINPUTNV _glFinalCombinerInputNV;

typedef void (APIENTRY * PFN_GLGETCOMBINERINPUTPARAMETERFVNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat * params);
extern PFN_GLGETCOMBINERINPUTPARAMETERFVNV _glGetCombinerInputParameterfvNV;

typedef void (APIENTRY * PFN_GLGETCOMBINERINPUTPARAMETERIVNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint * params);
extern PFN_GLGETCOMBINERINPUTPARAMETERIVNV _glGetCombinerInputParameterivNV;

typedef void (APIENTRY * PFN_GLGETCOMBINEROUTPUTPARAMETERFVNV)(GLenum stage, GLenum portion, GLenum pname, GLfloat * params);
extern PFN_GLGETCOMBINEROUTPUTPARAMETERFVNV _glGetCombinerOutputParameterfvNV;

typedef void (APIENTRY * PFN_GLGETCOMBINEROUTPUTPARAMETERIVNV)(GLenum stage, GLenum portion, GLenum pname, GLint * params);
extern PFN_GLGETCOMBINEROUTPUTPARAMETERIVNV _glGetCombinerOutputParameterivNV;

typedef void (APIENTRY * PFN_GLGETFINALCOMBINERINPUTPARAMETERFVNV)(GLenum variable, GLenum pname, GLfloat * params);
extern PFN_GLGETFINALCOMBINERINPUTPARAMETERFVNV _glGetFinalCombinerInputParameterfvNV;

typedef void (APIENTRY * PFN_GLGETFINALCOMBINERINPUTPARAMETERIVNV)(GLenum variable, GLenum pname, GLint * params);
extern PFN_GLGETFINALCOMBINERINPUTPARAMETERIVNV _glGetFinalCombinerInputParameterivNV;

typedef void (APIENTRY * PFN_GLCOMBINERSTAGEPARAMETERFVNV)(GLenum stage, GLenum pname, const GLfloat * params);
extern PFN_GLCOMBINERSTAGEPARAMETERFVNV _glCombinerStageParameterfvNV;

typedef void (APIENTRY * PFN_GLGETCOMBINERSTAGEPARAMETERFVNV)(GLenum stage, GLenum pname, GLfloat * params);
extern PFN_GLGETCOMBINERSTAGEPARAMETERFVNV _glGetCombinerStageParameterfvNV;

typedef void (APIENTRY * PFN_GLMAKEBUFFERRESIDENTNV)(GLenum target, GLenum access);
extern PFN_GLMAKEBUFFERRESIDENTNV _glMakeBufferResidentNV;

typedef void (APIENTRY * PFN_GLMAKEBUFFERNONRESIDENTNV)(GLenum target);
extern PFN_GLMAKEBUFFERNONRESIDENTNV _glMakeBufferNonResidentNV;

typedef GLboolean (APIENTRY * PFN_GLISBUFFERRESIDENTNV)(GLenum target);
extern PFN_GLISBUFFERRESIDENTNV _glIsBufferResidentNV;

typedef void (APIENTRY * PFN_GLMAKENAMEDBUFFERRESIDENTNV)(GLuint buffer, GLenum access);
extern PFN_GLMAKENAMEDBUFFERRESIDENTNV _glMakeNamedBufferResidentNV;

typedef void (APIENTRY * PFN_GLMAKENAMEDBUFFERNONRESIDENTNV)(GLuint buffer);
extern PFN_GLMAKENAMEDBUFFERNONRESIDENTNV _glMakeNamedBufferNonResidentNV;

typedef GLboolean (APIENTRY * PFN_GLISNAMEDBUFFERRESIDENTNV)(GLuint buffer);
extern PFN_GLISNAMEDBUFFERRESIDENTNV _glIsNamedBufferResidentNV;

typedef void (APIENTRY * PFN_GLGETBUFFERPARAMETERUI64VNV)(GLenum target, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETBUFFERPARAMETERUI64VNV _glGetBufferParameterui64vNV;

typedef void (APIENTRY * PFN_GLGETNAMEDBUFFERPARAMETERUI64VNV)(GLuint buffer, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETNAMEDBUFFERPARAMETERUI64VNV _glGetNamedBufferParameterui64vNV;

typedef void (APIENTRY * PFN_GLGETINTEGERUI64VNV)(GLenum value, GLuint64EXT * result);
extern PFN_GLGETINTEGERUI64VNV _glGetIntegerui64vNV;

typedef void (APIENTRY * PFN_GLUNIFORMUI64NV)(GLint location, GLuint64EXT value);
extern PFN_GLUNIFORMUI64NV _glUniformui64NV;

typedef void (APIENTRY * PFN_GLUNIFORMUI64VNV)(GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLUNIFORMUI64VNV _glUniformui64vNV;

typedef void (APIENTRY * PFN_GLGETUNIFORMUI64VNV)(GLuint program, GLint location, GLuint64EXT * params);
extern PFN_GLGETUNIFORMUI64VNV _glGetUniformui64vNV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMUI64NV)(GLuint program, GLint location, GLuint64EXT value);
extern PFN_GLPROGRAMUNIFORMUI64NV _glProgramUniformui64NV;

typedef void (APIENTRY * PFN_GLPROGRAMUNIFORMUI64VNV)(GLuint program, GLint location, GLsizei count, const GLuint64EXT * value);
extern PFN_GLPROGRAMUNIFORMUI64VNV _glProgramUniformui64vNV;

typedef void (APIENTRY * PFN_GLTEXTUREBARRIERNV)(void);
extern PFN_GLTEXTUREBARRIERNV _glTextureBarrierNV;

typedef void (APIENTRY * PFN_GLTEXIMAGE2DMULTISAMPLECOVERAGENV)(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
extern PFN_GLTEXIMAGE2DMULTISAMPLECOVERAGENV _glTexImage2DMultisampleCoverageNV;

typedef void (APIENTRY * PFN_GLTEXIMAGE3DMULTISAMPLECOVERAGENV)(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
extern PFN_GLTEXIMAGE3DMULTISAMPLECOVERAGENV _glTexImage3DMultisampleCoverageNV;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE2DMULTISAMPLENV)(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
extern PFN_GLTEXTUREIMAGE2DMULTISAMPLENV _glTextureImage2DMultisampleNV;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE3DMULTISAMPLENV)(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
extern PFN_GLTEXTUREIMAGE3DMULTISAMPLENV _glTextureImage3DMultisampleNV;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE2DMULTISAMPLECOVERAGENV)(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
extern PFN_GLTEXTUREIMAGE2DMULTISAMPLECOVERAGENV _glTextureImage2DMultisampleCoverageNV;

typedef void (APIENTRY * PFN_GLTEXTUREIMAGE3DMULTISAMPLECOVERAGENV)(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
extern PFN_GLTEXTUREIMAGE3DMULTISAMPLECOVERAGENV _glTextureImage3DMultisampleCoverageNV;

typedef void (APIENTRY * PFN_GLBEGINTRANSFORMFEEDBACKNV)(GLenum primitiveMode);
extern PFN_GLBEGINTRANSFORMFEEDBACKNV _glBeginTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLENDTRANSFORMFEEDBACKNV)(void);
extern PFN_GLENDTRANSFORMFEEDBACKNV _glEndTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKATTRIBSNV)(GLsizei count, const GLint * attribs, GLenum bufferMode);
extern PFN_GLTRANSFORMFEEDBACKATTRIBSNV _glTransformFeedbackAttribsNV;

typedef void (APIENTRY * PFN_GLBINDBUFFERRANGENV)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFN_GLBINDBUFFERRANGENV _glBindBufferRangeNV;

typedef void (APIENTRY * PFN_GLBINDBUFFEROFFSETNV)(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
extern PFN_GLBINDBUFFEROFFSETNV _glBindBufferOffsetNV;

typedef void (APIENTRY * PFN_GLBINDBUFFERBASENV)(GLenum target, GLuint index, GLuint buffer);
extern PFN_GLBINDBUFFERBASENV _glBindBufferBaseNV;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKVARYINGSNV)(GLuint program, GLsizei count, const GLint * locations, GLenum bufferMode);
extern PFN_GLTRANSFORMFEEDBACKVARYINGSNV _glTransformFeedbackVaryingsNV;

typedef void (APIENTRY * PFN_GLACTIVEVARYINGNV)(GLuint program, const GLchar * name);
extern PFN_GLACTIVEVARYINGNV _glActiveVaryingNV;

typedef GLint (APIENTRY * PFN_GLGETVARYINGLOCATIONNV)(GLuint program, const GLchar * name);
extern PFN_GLGETVARYINGLOCATIONNV _glGetVaryingLocationNV;

typedef void (APIENTRY * PFN_GLGETACTIVEVARYINGNV)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
extern PFN_GLGETACTIVEVARYINGNV _glGetActiveVaryingNV;

typedef void (APIENTRY * PFN_GLGETTRANSFORMFEEDBACKVARYINGNV)(GLuint program, GLuint index, GLint * location);
extern PFN_GLGETTRANSFORMFEEDBACKVARYINGNV _glGetTransformFeedbackVaryingNV;

typedef void (APIENTRY * PFN_GLTRANSFORMFEEDBACKSTREAMATTRIBSNV)(GLsizei count, const GLint * attribs, GLsizei nbuffers, const GLint * bufstreams, GLenum bufferMode);
extern PFN_GLTRANSFORMFEEDBACKSTREAMATTRIBSNV _glTransformFeedbackStreamAttribsNV;

typedef void (APIENTRY * PFN_GLBINDTRANSFORMFEEDBACKNV)(GLenum target, GLuint id);
extern PFN_GLBINDTRANSFORMFEEDBACKNV _glBindTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLDELETETRANSFORMFEEDBACKSNV)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETETRANSFORMFEEDBACKSNV _glDeleteTransformFeedbacksNV;

typedef void (APIENTRY * PFN_GLGENTRANSFORMFEEDBACKSNV)(GLsizei n, GLuint * ids);
extern PFN_GLGENTRANSFORMFEEDBACKSNV _glGenTransformFeedbacksNV;

typedef GLboolean (APIENTRY * PFN_GLISTRANSFORMFEEDBACKNV)(GLuint id);
extern PFN_GLISTRANSFORMFEEDBACKNV _glIsTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLPAUSETRANSFORMFEEDBACKNV)(void);
extern PFN_GLPAUSETRANSFORMFEEDBACKNV _glPauseTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLRESUMETRANSFORMFEEDBACKNV)(void);
extern PFN_GLRESUMETRANSFORMFEEDBACKNV _glResumeTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLDRAWTRANSFORMFEEDBACKNV)(GLenum mode, GLuint id);
extern PFN_GLDRAWTRANSFORMFEEDBACKNV _glDrawTransformFeedbackNV;

typedef void (APIENTRY * PFN_GLVDPAUINITNV)(const GLvoid * vdpDevice, const GLvoid * getProcAddress);
extern PFN_GLVDPAUINITNV _glVDPAUInitNV;

typedef void (APIENTRY * PFN_GLVDPAUFININV)(void);
extern PFN_GLVDPAUFININV _glVDPAUFiniNV;

typedef GLvdpauSurfaceNV (APIENTRY * PFN_GLVDPAUREGISTERVIDEOSURFACENV)(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames);
extern PFN_GLVDPAUREGISTERVIDEOSURFACENV _glVDPAURegisterVideoSurfaceNV;

typedef GLvdpauSurfaceNV (APIENTRY * PFN_GLVDPAUREGISTEROUTPUTSURFACENV)(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames);
extern PFN_GLVDPAUREGISTEROUTPUTSURFACENV _glVDPAURegisterOutputSurfaceNV;

typedef GLboolean (APIENTRY * PFN_GLVDPAUISSURFACENV)(GLvdpauSurfaceNV surface);
extern PFN_GLVDPAUISSURFACENV _glVDPAUIsSurfaceNV;

typedef void (APIENTRY * PFN_GLVDPAUUNREGISTERSURFACENV)(GLvdpauSurfaceNV surface);
extern PFN_GLVDPAUUNREGISTERSURFACENV _glVDPAUUnregisterSurfaceNV;

typedef void (APIENTRY * PFN_GLVDPAUGETSURFACEIVNV)(GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values);
extern PFN_GLVDPAUGETSURFACEIVNV _glVDPAUGetSurfaceivNV;

typedef void (APIENTRY * PFN_GLVDPAUSURFACEACCESSNV)(GLvdpauSurfaceNV surface, GLenum access);
extern PFN_GLVDPAUSURFACEACCESSNV _glVDPAUSurfaceAccessNV;

typedef void (APIENTRY * PFN_GLVDPAUMAPSURFACESNV)(GLsizei numSurfaces, const GLvdpauSurfaceNV * surfaces);
extern PFN_GLVDPAUMAPSURFACESNV _glVDPAUMapSurfacesNV;

typedef void (APIENTRY * PFN_GLVDPAUUNMAPSURFACESNV)(GLsizei numSurface, const GLvdpauSurfaceNV * surfaces);
extern PFN_GLVDPAUUNMAPSURFACESNV _glVDPAUUnmapSurfacesNV;

typedef void (APIENTRY * PFN_GLFLUSHVERTEXARRAYRANGENV)(void);
extern PFN_GLFLUSHVERTEXARRAYRANGENV _glFlushVertexArrayRangeNV;

typedef void (APIENTRY * PFN_GLVERTEXARRAYRANGENV)(GLsizei length, const GLvoid * pointer);
extern PFN_GLVERTEXARRAYRANGENV _glVertexArrayRangeNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1I64NV)(GLuint index, GLint64EXT x);
extern PFN_GLVERTEXATTRIBL1I64NV _glVertexAttribL1i64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2I64NV)(GLuint index, GLint64EXT x, GLint64EXT y);
extern PFN_GLVERTEXATTRIBL2I64NV _glVertexAttribL2i64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3I64NV)(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern PFN_GLVERTEXATTRIBL3I64NV _glVertexAttribL3i64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4I64NV)(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern PFN_GLVERTEXATTRIBL4I64NV _glVertexAttribL4i64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1I64VNV)(GLuint index, const GLint64EXT * v);
extern PFN_GLVERTEXATTRIBL1I64VNV _glVertexAttribL1i64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2I64VNV)(GLuint index, const GLint64EXT * v);
extern PFN_GLVERTEXATTRIBL2I64VNV _glVertexAttribL2i64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3I64VNV)(GLuint index, const GLint64EXT * v);
extern PFN_GLVERTEXATTRIBL3I64VNV _glVertexAttribL3i64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4I64VNV)(GLuint index, const GLint64EXT * v);
extern PFN_GLVERTEXATTRIBL4I64VNV _glVertexAttribL4i64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1UI64NV)(GLuint index, GLuint64EXT x);
extern PFN_GLVERTEXATTRIBL1UI64NV _glVertexAttribL1ui64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2UI64NV)(GLuint index, GLuint64EXT x, GLuint64EXT y);
extern PFN_GLVERTEXATTRIBL2UI64NV _glVertexAttribL2ui64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3UI64NV)(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern PFN_GLVERTEXATTRIBL3UI64NV _glVertexAttribL3ui64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4UI64NV)(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern PFN_GLVERTEXATTRIBL4UI64NV _glVertexAttribL4ui64NV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL1UI64VNV)(GLuint index, const GLuint64EXT * v);
extern PFN_GLVERTEXATTRIBL1UI64VNV _glVertexAttribL1ui64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL2UI64VNV)(GLuint index, const GLuint64EXT * v);
extern PFN_GLVERTEXATTRIBL2UI64VNV _glVertexAttribL2ui64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL3UI64VNV)(GLuint index, const GLuint64EXT * v);
extern PFN_GLVERTEXATTRIBL3UI64VNV _glVertexAttribL3ui64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBL4UI64VNV)(GLuint index, const GLuint64EXT * v);
extern PFN_GLVERTEXATTRIBL4UI64VNV _glVertexAttribL4ui64vNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBLI64VNV)(GLuint index, GLenum pname, GLint64EXT * params);
extern PFN_GLGETVERTEXATTRIBLI64VNV _glGetVertexAttribLi64vNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBLUI64VNV)(GLuint index, GLenum pname, GLuint64EXT * params);
extern PFN_GLGETVERTEXATTRIBLUI64VNV _glGetVertexAttribLui64vNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBLFORMATNV)(GLuint index, GLint size, GLenum type, GLsizei stride);
extern PFN_GLVERTEXATTRIBLFORMATNV _glVertexAttribLFormatNV;

typedef void (APIENTRY * PFN_GLBUFFERADDRESSRANGENV)(GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
extern PFN_GLBUFFERADDRESSRANGENV _glBufferAddressRangeNV;

typedef void (APIENTRY * PFN_GLVERTEXFORMATNV)(GLint size, GLenum type, GLsizei stride);
extern PFN_GLVERTEXFORMATNV _glVertexFormatNV;

typedef void (APIENTRY * PFN_GLNORMALFORMATNV)(GLenum type, GLsizei stride);
extern PFN_GLNORMALFORMATNV _glNormalFormatNV;

typedef void (APIENTRY * PFN_GLCOLORFORMATNV)(GLint size, GLenum type, GLsizei stride);
extern PFN_GLCOLORFORMATNV _glColorFormatNV;

typedef void (APIENTRY * PFN_GLINDEXFORMATNV)(GLenum type, GLsizei stride);
extern PFN_GLINDEXFORMATNV _glIndexFormatNV;

typedef void (APIENTRY * PFN_GLTEXCOORDFORMATNV)(GLint size, GLenum type, GLsizei stride);
extern PFN_GLTEXCOORDFORMATNV _glTexCoordFormatNV;

typedef void (APIENTRY * PFN_GLEDGEFLAGFORMATNV)(GLsizei stride);
extern PFN_GLEDGEFLAGFORMATNV _glEdgeFlagFormatNV;

typedef void (APIENTRY * PFN_GLSECONDARYCOLORFORMATNV)(GLint size, GLenum type, GLsizei stride);
extern PFN_GLSECONDARYCOLORFORMATNV _glSecondaryColorFormatNV;

typedef void (APIENTRY * PFN_GLFOGCOORDFORMATNV)(GLenum type, GLsizei stride);
extern PFN_GLFOGCOORDFORMATNV _glFogCoordFormatNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBFORMATNV)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
extern PFN_GLVERTEXATTRIBFORMATNV _glVertexAttribFormatNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBIFORMATNV)(GLuint index, GLint size, GLenum type, GLsizei stride);
extern PFN_GLVERTEXATTRIBIFORMATNV _glVertexAttribIFormatNV;

typedef void (APIENTRY * PFN_GLGETINTEGERUI64I_VNV)(GLenum value, GLuint index, GLuint64EXT * result);
extern PFN_GLGETINTEGERUI64I_VNV _glGetIntegerui64i_vNV;

typedef GLboolean (APIENTRY * PFN_GLAREPROGRAMSRESIDENTNV)(GLsizei n, const GLuint * ids, GLboolean * residences);
extern PFN_GLAREPROGRAMSRESIDENTNV _glAreProgramsResidentNV;

typedef void (APIENTRY * PFN_GLBINDPROGRAMNV)(GLenum target, GLuint program);
extern PFN_GLBINDPROGRAMNV _glBindProgramNV;

typedef void (APIENTRY * PFN_GLDELETEPROGRAMSNV)(GLsizei n, const GLuint * programs);
extern PFN_GLDELETEPROGRAMSNV _glDeleteProgramsNV;

typedef void (APIENTRY * PFN_GLEXECUTEPROGRAMNV)(GLenum target, GLuint id, const GLfloat * params);
extern PFN_GLEXECUTEPROGRAMNV _glExecuteProgramNV;

typedef void (APIENTRY * PFN_GLGENPROGRAMSNV)(GLsizei n, GLuint * programs);
extern PFN_GLGENPROGRAMSNV _glGenProgramsNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMPARAMETERDVNV)(GLenum target, GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETPROGRAMPARAMETERDVNV _glGetProgramParameterdvNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMPARAMETERFVNV)(GLenum target, GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETPROGRAMPARAMETERFVNV _glGetProgramParameterfvNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMIVNV)(GLuint id, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMIVNV _glGetProgramivNV;

typedef void (APIENTRY * PFN_GLGETPROGRAMSTRINGNV)(GLuint id, GLenum pname, GLubyte * program);
extern PFN_GLGETPROGRAMSTRINGNV _glGetProgramStringNV;

typedef void (APIENTRY * PFN_GLGETTRACKMATRIXIVNV)(GLenum target, GLuint address, GLenum pname, GLint * params);
extern PFN_GLGETTRACKMATRIXIVNV _glGetTrackMatrixivNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBDVNV)(GLuint index, GLenum pname, GLdouble * params);
extern PFN_GLGETVERTEXATTRIBDVNV _glGetVertexAttribdvNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBFVNV)(GLuint index, GLenum pname, GLfloat * params);
extern PFN_GLGETVERTEXATTRIBFVNV _glGetVertexAttribfvNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIVNV)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBIVNV _glGetVertexAttribivNV;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBPOINTERVNV)(GLuint index, GLenum pname, GLvoid * * pointer);
extern PFN_GLGETVERTEXATTRIBPOINTERVNV _glGetVertexAttribPointervNV;

typedef GLboolean (APIENTRY * PFN_GLISPROGRAMNV)(GLuint program);
extern PFN_GLISPROGRAMNV _glIsProgramNV;

typedef void (APIENTRY * PFN_GLLOADPROGRAMNV)(GLenum target, GLuint id, GLsizei len, const GLubyte * program);
extern PFN_GLLOADPROGRAMNV _glLoadProgramNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETER4DNV)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLPROGRAMPARAMETER4DNV _glProgramParameter4dNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETER4DVNV)(GLenum target, GLuint index, const GLdouble * v);
extern PFN_GLPROGRAMPARAMETER4DVNV _glProgramParameter4dvNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETER4FNV)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLPROGRAMPARAMETER4FNV _glProgramParameter4fNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETER4FVNV)(GLenum target, GLuint index, const GLfloat * v);
extern PFN_GLPROGRAMPARAMETER4FVNV _glProgramParameter4fvNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETERS4DVNV)(GLenum target, GLuint index, GLsizei count, const GLdouble * v);
extern PFN_GLPROGRAMPARAMETERS4DVNV _glProgramParameters4dvNV;

typedef void (APIENTRY * PFN_GLPROGRAMPARAMETERS4FVNV)(GLenum target, GLuint index, GLsizei count, const GLfloat * v);
extern PFN_GLPROGRAMPARAMETERS4FVNV _glProgramParameters4fvNV;

typedef void (APIENTRY * PFN_GLREQUESTRESIDENTPROGRAMSNV)(GLsizei n, const GLuint * programs);
extern PFN_GLREQUESTRESIDENTPROGRAMSNV _glRequestResidentProgramsNV;

typedef void (APIENTRY * PFN_GLTRACKMATRIXNV)(GLenum target, GLuint address, GLenum matrix, GLenum transform);
extern PFN_GLTRACKMATRIXNV _glTrackMatrixNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBPOINTERNV)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBPOINTERNV _glVertexAttribPointerNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1DNV)(GLuint index, GLdouble x);
extern PFN_GLVERTEXATTRIB1DNV _glVertexAttrib1dNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1DVNV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB1DVNV _glVertexAttrib1dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1FNV)(GLuint index, GLfloat x);
extern PFN_GLVERTEXATTRIB1FNV _glVertexAttrib1fNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1FVNV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB1FVNV _glVertexAttrib1fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1SNV)(GLuint index, GLshort x);
extern PFN_GLVERTEXATTRIB1SNV _glVertexAttrib1sNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB1SVNV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB1SVNV _glVertexAttrib1svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2DNV)(GLuint index, GLdouble x, GLdouble y);
extern PFN_GLVERTEXATTRIB2DNV _glVertexAttrib2dNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2DVNV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB2DVNV _glVertexAttrib2dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2FNV)(GLuint index, GLfloat x, GLfloat y);
extern PFN_GLVERTEXATTRIB2FNV _glVertexAttrib2fNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2FVNV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB2FVNV _glVertexAttrib2fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2SNV)(GLuint index, GLshort x, GLshort y);
extern PFN_GLVERTEXATTRIB2SNV _glVertexAttrib2sNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB2SVNV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB2SVNV _glVertexAttrib2svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3DNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFN_GLVERTEXATTRIB3DNV _glVertexAttrib3dNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3DVNV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB3DVNV _glVertexAttrib3dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3FNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLVERTEXATTRIB3FNV _glVertexAttrib3fNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3FVNV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB3FVNV _glVertexAttrib3fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3SNV)(GLuint index, GLshort x, GLshort y, GLshort z);
extern PFN_GLVERTEXATTRIB3SNV _glVertexAttrib3sNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB3SVNV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB3SVNV _glVertexAttrib3svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4DNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFN_GLVERTEXATTRIB4DNV _glVertexAttrib4dNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4DVNV)(GLuint index, const GLdouble * v);
extern PFN_GLVERTEXATTRIB4DVNV _glVertexAttrib4dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4FNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLVERTEXATTRIB4FNV _glVertexAttrib4fNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4FVNV)(GLuint index, const GLfloat * v);
extern PFN_GLVERTEXATTRIB4FVNV _glVertexAttrib4fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4SNV)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern PFN_GLVERTEXATTRIB4SNV _glVertexAttrib4sNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4SVNV)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIB4SVNV _glVertexAttrib4svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UBNV)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern PFN_GLVERTEXATTRIB4UBNV _glVertexAttrib4ubNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIB4UBVNV)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIB4UBVNV _glVertexAttrib4ubvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS1DVNV)(GLuint index, GLsizei count, const GLdouble * v);
extern PFN_GLVERTEXATTRIBS1DVNV _glVertexAttribs1dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS1FVNV)(GLuint index, GLsizei count, const GLfloat * v);
extern PFN_GLVERTEXATTRIBS1FVNV _glVertexAttribs1fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS1SVNV)(GLuint index, GLsizei count, const GLshort * v);
extern PFN_GLVERTEXATTRIBS1SVNV _glVertexAttribs1svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS2DVNV)(GLuint index, GLsizei count, const GLdouble * v);
extern PFN_GLVERTEXATTRIBS2DVNV _glVertexAttribs2dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS2FVNV)(GLuint index, GLsizei count, const GLfloat * v);
extern PFN_GLVERTEXATTRIBS2FVNV _glVertexAttribs2fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS2SVNV)(GLuint index, GLsizei count, const GLshort * v);
extern PFN_GLVERTEXATTRIBS2SVNV _glVertexAttribs2svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS3DVNV)(GLuint index, GLsizei count, const GLdouble * v);
extern PFN_GLVERTEXATTRIBS3DVNV _glVertexAttribs3dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS3FVNV)(GLuint index, GLsizei count, const GLfloat * v);
extern PFN_GLVERTEXATTRIBS3FVNV _glVertexAttribs3fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS3SVNV)(GLuint index, GLsizei count, const GLshort * v);
extern PFN_GLVERTEXATTRIBS3SVNV _glVertexAttribs3svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS4DVNV)(GLuint index, GLsizei count, const GLdouble * v);
extern PFN_GLVERTEXATTRIBS4DVNV _glVertexAttribs4dvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS4FVNV)(GLuint index, GLsizei count, const GLfloat * v);
extern PFN_GLVERTEXATTRIBS4FVNV _glVertexAttribs4fvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS4SVNV)(GLuint index, GLsizei count, const GLshort * v);
extern PFN_GLVERTEXATTRIBS4SVNV _glVertexAttribs4svNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBS4UBVNV)(GLuint index, GLsizei count, const GLubyte * v);
extern PFN_GLVERTEXATTRIBS4UBVNV _glVertexAttribs4ubvNV;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1IEXT)(GLuint index, GLint x);
extern PFN_GLVERTEXATTRIBI1IEXT _glVertexAttribI1iEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2IEXT)(GLuint index, GLint x, GLint y);
extern PFN_GLVERTEXATTRIBI2IEXT _glVertexAttribI2iEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3IEXT)(GLuint index, GLint x, GLint y, GLint z);
extern PFN_GLVERTEXATTRIBI3IEXT _glVertexAttribI3iEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4IEXT)(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFN_GLVERTEXATTRIBI4IEXT _glVertexAttribI4iEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1UIEXT)(GLuint index, GLuint x);
extern PFN_GLVERTEXATTRIBI1UIEXT _glVertexAttribI1uiEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2UIEXT)(GLuint index, GLuint x, GLuint y);
extern PFN_GLVERTEXATTRIBI2UIEXT _glVertexAttribI2uiEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3UIEXT)(GLuint index, GLuint x, GLuint y, GLuint z);
extern PFN_GLVERTEXATTRIBI3UIEXT _glVertexAttribI3uiEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UIEXT)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFN_GLVERTEXATTRIBI4UIEXT _glVertexAttribI4uiEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1IVEXT)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI1IVEXT _glVertexAttribI1ivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2IVEXT)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI2IVEXT _glVertexAttribI2ivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3IVEXT)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI3IVEXT _glVertexAttribI3ivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4IVEXT)(GLuint index, const GLint * v);
extern PFN_GLVERTEXATTRIBI4IVEXT _glVertexAttribI4ivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI1UIVEXT)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI1UIVEXT _glVertexAttribI1uivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI2UIVEXT)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI2UIVEXT _glVertexAttribI2uivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI3UIVEXT)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI3UIVEXT _glVertexAttribI3uivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UIVEXT)(GLuint index, const GLuint * v);
extern PFN_GLVERTEXATTRIBI4UIVEXT _glVertexAttribI4uivEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4BVEXT)(GLuint index, const GLbyte * v);
extern PFN_GLVERTEXATTRIBI4BVEXT _glVertexAttribI4bvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4SVEXT)(GLuint index, const GLshort * v);
extern PFN_GLVERTEXATTRIBI4SVEXT _glVertexAttribI4svEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4UBVEXT)(GLuint index, const GLubyte * v);
extern PFN_GLVERTEXATTRIBI4UBVEXT _glVertexAttribI4ubvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBI4USVEXT)(GLuint index, const GLushort * v);
extern PFN_GLVERTEXATTRIBI4USVEXT _glVertexAttribI4usvEXT;

typedef void (APIENTRY * PFN_GLVERTEXATTRIBIPOINTEREXT)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLVERTEXATTRIBIPOINTEREXT _glVertexAttribIPointerEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIIVEXT)(GLuint index, GLenum pname, GLint * params);
extern PFN_GLGETVERTEXATTRIBIIVEXT _glGetVertexAttribIivEXT;

typedef void (APIENTRY * PFN_GLGETVERTEXATTRIBIUIVEXT)(GLuint index, GLenum pname, GLuint * params);
extern PFN_GLGETVERTEXATTRIBIUIVEXT _glGetVertexAttribIuivEXT;

typedef void (APIENTRY * PFN_GLBEGINVIDEOCAPTURENV)(GLuint video_capture_slot);
extern PFN_GLBEGINVIDEOCAPTURENV _glBeginVideoCaptureNV;

typedef void (APIENTRY * PFN_GLBINDVIDEOCAPTURESTREAMBUFFERNV)(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
extern PFN_GLBINDVIDEOCAPTURESTREAMBUFFERNV _glBindVideoCaptureStreamBufferNV;

typedef void (APIENTRY * PFN_GLBINDVIDEOCAPTURESTREAMTEXTURENV)(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
extern PFN_GLBINDVIDEOCAPTURESTREAMTEXTURENV _glBindVideoCaptureStreamTextureNV;

typedef void (APIENTRY * PFN_GLENDVIDEOCAPTURENV)(GLuint video_capture_slot);
extern PFN_GLENDVIDEOCAPTURENV _glEndVideoCaptureNV;

typedef void (APIENTRY * PFN_GLGETVIDEOCAPTUREIVNV)(GLuint video_capture_slot, GLenum pname, GLint * params);
extern PFN_GLGETVIDEOCAPTUREIVNV _glGetVideoCaptureivNV;

typedef void (APIENTRY * PFN_GLGETVIDEOCAPTURESTREAMIVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, GLint * params);
extern PFN_GLGETVIDEOCAPTURESTREAMIVNV _glGetVideoCaptureStreamivNV;

typedef void (APIENTRY * PFN_GLGETVIDEOCAPTURESTREAMFVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat * params);
extern PFN_GLGETVIDEOCAPTURESTREAMFVNV _glGetVideoCaptureStreamfvNV;

typedef void (APIENTRY * PFN_GLGETVIDEOCAPTURESTREAMDVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble * params);
extern PFN_GLGETVIDEOCAPTURESTREAMDVNV _glGetVideoCaptureStreamdvNV;

typedef GLenum (APIENTRY * PFN_GLVIDEOCAPTURENV)(GLuint video_capture_slot, GLuint * sequence_num, GLuint64EXT * capture_time);
extern PFN_GLVIDEOCAPTURENV _glVideoCaptureNV;

typedef void (APIENTRY * PFN_GLVIDEOCAPTURESTREAMPARAMETERIVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint * params);
extern PFN_GLVIDEOCAPTURESTREAMPARAMETERIVNV _glVideoCaptureStreamParameterivNV;

typedef void (APIENTRY * PFN_GLVIDEOCAPTURESTREAMPARAMETERFVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat * params);
extern PFN_GLVIDEOCAPTURESTREAMPARAMETERFVNV _glVideoCaptureStreamParameterfvNV;

typedef void (APIENTRY * PFN_GLVIDEOCAPTURESTREAMPARAMETERDVNV)(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble * params);
extern PFN_GLVIDEOCAPTURESTREAMPARAMETERDVNV _glVideoCaptureStreamParameterdvNV;

typedef void (APIENTRY * PFN_GLHINTPGI)(GLenum target, GLint mode);
extern PFN_GLHINTPGI _glHintPGI;

typedef void (APIENTRY * PFN_GLDETAILTEXFUNCSGIS)(GLenum target, GLsizei n, const GLfloat * points);
extern PFN_GLDETAILTEXFUNCSGIS _glDetailTexFuncSGIS;

typedef void (APIENTRY * PFN_GLGETDETAILTEXFUNCSGIS)(GLenum target, GLfloat * points);
extern PFN_GLGETDETAILTEXFUNCSGIS _glGetDetailTexFuncSGIS;

typedef void (APIENTRY * PFN_GLFOGFUNCSGIS)(GLsizei n, const GLfloat * points);
extern PFN_GLFOGFUNCSGIS _glFogFuncSGIS;

typedef void (APIENTRY * PFN_GLGETFOGFUNCSGIS)(GLfloat * points);
extern PFN_GLGETFOGFUNCSGIS _glGetFogFuncSGIS;

typedef void (APIENTRY * PFN_GLSAMPLEMASKSGIS)(GLclampf value, GLboolean invert);
extern PFN_GLSAMPLEMASKSGIS _glSampleMaskSGIS;

typedef void (APIENTRY * PFN_GLSAMPLEPATTERNSGIS)(GLenum pattern);
extern PFN_GLSAMPLEPATTERNSGIS _glSamplePatternSGIS;

typedef void (APIENTRY * PFN_GLPIXELTEXGENPARAMETERISGIS)(GLenum pname, GLint param);
extern PFN_GLPIXELTEXGENPARAMETERISGIS _glPixelTexGenParameteriSGIS;

typedef void (APIENTRY * PFN_GLPIXELTEXGENPARAMETERIVSGIS)(GLenum pname, const GLint * params);
extern PFN_GLPIXELTEXGENPARAMETERIVSGIS _glPixelTexGenParameterivSGIS;

typedef void (APIENTRY * PFN_GLPIXELTEXGENPARAMETERFSGIS)(GLenum pname, GLfloat param);
extern PFN_GLPIXELTEXGENPARAMETERFSGIS _glPixelTexGenParameterfSGIS;

typedef void (APIENTRY * PFN_GLPIXELTEXGENPARAMETERFVSGIS)(GLenum pname, const GLfloat * params);
extern PFN_GLPIXELTEXGENPARAMETERFVSGIS _glPixelTexGenParameterfvSGIS;

typedef void (APIENTRY * PFN_GLGETPIXELTEXGENPARAMETERIVSGIS)(GLenum pname, GLint * params);
extern PFN_GLGETPIXELTEXGENPARAMETERIVSGIS _glGetPixelTexGenParameterivSGIS;

typedef void (APIENTRY * PFN_GLGETPIXELTEXGENPARAMETERFVSGIS)(GLenum pname, GLfloat * params);
extern PFN_GLGETPIXELTEXGENPARAMETERFVSGIS _glGetPixelTexGenParameterfvSGIS;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFSGIS)(GLenum pname, GLfloat param);
extern PFN_GLPOINTPARAMETERFSGIS _glPointParameterfSGIS;

typedef void (APIENTRY * PFN_GLPOINTPARAMETERFVSGIS)(GLenum pname, const GLfloat * params);
extern PFN_GLPOINTPARAMETERFVSGIS _glPointParameterfvSGIS;

typedef void (APIENTRY * PFN_GLSHARPENTEXFUNCSGIS)(GLenum target, GLsizei n, const GLfloat * points);
extern PFN_GLSHARPENTEXFUNCSGIS _glSharpenTexFuncSGIS;

typedef void (APIENTRY * PFN_GLGETSHARPENTEXFUNCSGIS)(GLenum target, GLfloat * points);
extern PFN_GLGETSHARPENTEXFUNCSGIS _glGetSharpenTexFuncSGIS;

typedef void (APIENTRY * PFN_GLTEXIMAGE4DSGIS)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE4DSGIS _glTexImage4DSGIS;

typedef void (APIENTRY * PFN_GLTEXSUBIMAGE4DSGIS)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE4DSGIS _glTexSubImage4DSGIS;

typedef void (APIENTRY * PFN_GLTEXTURECOLORMASKSGIS)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern PFN_GLTEXTURECOLORMASKSGIS _glTextureColorMaskSGIS;

typedef void (APIENTRY * PFN_GLGETTEXFILTERFUNCSGIS)(GLenum target, GLenum filter, GLfloat * weights);
extern PFN_GLGETTEXFILTERFUNCSGIS _glGetTexFilterFuncSGIS;

typedef void (APIENTRY * PFN_GLTEXFILTERFUNCSGIS)(GLenum target, GLenum filter, GLsizei n, const GLfloat * weights);
extern PFN_GLTEXFILTERFUNCSGIS _glTexFilterFuncSGIS;

typedef void (APIENTRY * PFN_GLASYNCMARKERSGIX)(GLuint marker);
extern PFN_GLASYNCMARKERSGIX _glAsyncMarkerSGIX;

typedef GLint (APIENTRY * PFN_GLFINISHASYNCSGIX)(GLuint * markerp);
extern PFN_GLFINISHASYNCSGIX _glFinishAsyncSGIX;

typedef GLint (APIENTRY * PFN_GLPOLLASYNCSGIX)(GLuint * markerp);
extern PFN_GLPOLLASYNCSGIX _glPollAsyncSGIX;

typedef GLuint (APIENTRY * PFN_GLGENASYNCMARKERSSGIX)(GLsizei range);
extern PFN_GLGENASYNCMARKERSSGIX _glGenAsyncMarkersSGIX;

typedef void (APIENTRY * PFN_GLDELETEASYNCMARKERSSGIX)(GLuint marker, GLsizei range);
extern PFN_GLDELETEASYNCMARKERSSGIX _glDeleteAsyncMarkersSGIX;

typedef GLboolean (APIENTRY * PFN_GLISASYNCMARKERSGIX)(GLuint marker);
extern PFN_GLISASYNCMARKERSGIX _glIsAsyncMarkerSGIX;

typedef void (APIENTRY * PFN_GLFLUSHRASTERSGIX)(void);
extern PFN_GLFLUSHRASTERSGIX _glFlushRasterSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTCOLORMATERIALSGIX)(GLenum face, GLenum mode);
extern PFN_GLFRAGMENTCOLORMATERIALSGIX _glFragmentColorMaterialSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTFSGIX)(GLenum light, GLenum pname, GLfloat param);
extern PFN_GLFRAGMENTLIGHTFSGIX _glFragmentLightfSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTFVSGIX)(GLenum light, GLenum pname, const GLfloat * params);
extern PFN_GLFRAGMENTLIGHTFVSGIX _glFragmentLightfvSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTISGIX)(GLenum light, GLenum pname, GLint param);
extern PFN_GLFRAGMENTLIGHTISGIX _glFragmentLightiSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTIVSGIX)(GLenum light, GLenum pname, const GLint * params);
extern PFN_GLFRAGMENTLIGHTIVSGIX _glFragmentLightivSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTMODELFSGIX)(GLenum pname, GLfloat param);
extern PFN_GLFRAGMENTLIGHTMODELFSGIX _glFragmentLightModelfSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTMODELFVSGIX)(GLenum pname, const GLfloat * params);
extern PFN_GLFRAGMENTLIGHTMODELFVSGIX _glFragmentLightModelfvSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTMODELISGIX)(GLenum pname, GLint param);
extern PFN_GLFRAGMENTLIGHTMODELISGIX _glFragmentLightModeliSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTLIGHTMODELIVSGIX)(GLenum pname, const GLint * params);
extern PFN_GLFRAGMENTLIGHTMODELIVSGIX _glFragmentLightModelivSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTMATERIALFSGIX)(GLenum face, GLenum pname, GLfloat param);
extern PFN_GLFRAGMENTMATERIALFSGIX _glFragmentMaterialfSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTMATERIALFVSGIX)(GLenum face, GLenum pname, const GLfloat * params);
extern PFN_GLFRAGMENTMATERIALFVSGIX _glFragmentMaterialfvSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTMATERIALISGIX)(GLenum face, GLenum pname, GLint param);
extern PFN_GLFRAGMENTMATERIALISGIX _glFragmentMaterialiSGIX;

typedef void (APIENTRY * PFN_GLFRAGMENTMATERIALIVSGIX)(GLenum face, GLenum pname, const GLint * params);
extern PFN_GLFRAGMENTMATERIALIVSGIX _glFragmentMaterialivSGIX;

typedef void (APIENTRY * PFN_GLGETFRAGMENTLIGHTFVSGIX)(GLenum light, GLenum pname, GLfloat * params);
extern PFN_GLGETFRAGMENTLIGHTFVSGIX _glGetFragmentLightfvSGIX;

typedef void (APIENTRY * PFN_GLGETFRAGMENTLIGHTIVSGIX)(GLenum light, GLenum pname, GLint * params);
extern PFN_GLGETFRAGMENTLIGHTIVSGIX _glGetFragmentLightivSGIX;

typedef void (APIENTRY * PFN_GLGETFRAGMENTMATERIALFVSGIX)(GLenum face, GLenum pname, GLfloat * params);
extern PFN_GLGETFRAGMENTMATERIALFVSGIX _glGetFragmentMaterialfvSGIX;

typedef void (APIENTRY * PFN_GLGETFRAGMENTMATERIALIVSGIX)(GLenum face, GLenum pname, GLint * params);
extern PFN_GLGETFRAGMENTMATERIALIVSGIX _glGetFragmentMaterialivSGIX;

typedef void (APIENTRY * PFN_GLLIGHTENVISGIX)(GLenum pname, GLint param);
extern PFN_GLLIGHTENVISGIX _glLightEnviSGIX;

typedef void (APIENTRY * PFN_GLFRAMEZOOMSGIX)(GLint factor);
extern PFN_GLFRAMEZOOMSGIX _glFrameZoomSGIX;

typedef void (APIENTRY * PFN_GLIGLOOINTERFACESGIX)(GLenum pname, const GLvoid * params);
extern PFN_GLIGLOOINTERFACESGIX _glIglooInterfaceSGIX;

typedef GLint (APIENTRY * PFN_GLGETINSTRUMENTSSGIX)(void);
extern PFN_GLGETINSTRUMENTSSGIX _glGetInstrumentsSGIX;

typedef void (APIENTRY * PFN_GLINSTRUMENTSBUFFERSGIX)(GLsizei size, GLint * buffer);
extern PFN_GLINSTRUMENTSBUFFERSGIX _glInstrumentsBufferSGIX;

typedef GLint (APIENTRY * PFN_GLPOLLINSTRUMENTSSGIX)(GLint * marker_p);
extern PFN_GLPOLLINSTRUMENTSSGIX _glPollInstrumentsSGIX;

typedef void (APIENTRY * PFN_GLREADINSTRUMENTSSGIX)(GLint marker);
extern PFN_GLREADINSTRUMENTSSGIX _glReadInstrumentsSGIX;

typedef void (APIENTRY * PFN_GLSTARTINSTRUMENTSSGIX)(void);
extern PFN_GLSTARTINSTRUMENTSSGIX _glStartInstrumentsSGIX;

typedef void (APIENTRY * PFN_GLSTOPINSTRUMENTSSGIX)(GLint marker);
extern PFN_GLSTOPINSTRUMENTSSGIX _glStopInstrumentsSGIX;

typedef void (APIENTRY * PFN_GLGETLISTPARAMETERFVSGIX)(GLuint list, GLenum pname, GLfloat * params);
extern PFN_GLGETLISTPARAMETERFVSGIX _glGetListParameterfvSGIX;

typedef void (APIENTRY * PFN_GLGETLISTPARAMETERIVSGIX)(GLuint list, GLenum pname, GLint * params);
extern PFN_GLGETLISTPARAMETERIVSGIX _glGetListParameterivSGIX;

typedef void (APIENTRY * PFN_GLLISTPARAMETERFSGIX)(GLuint list, GLenum pname, GLfloat param);
extern PFN_GLLISTPARAMETERFSGIX _glListParameterfSGIX;

typedef void (APIENTRY * PFN_GLLISTPARAMETERFVSGIX)(GLuint list, GLenum pname, const GLfloat * params);
extern PFN_GLLISTPARAMETERFVSGIX _glListParameterfvSGIX;

typedef void (APIENTRY * PFN_GLLISTPARAMETERISGIX)(GLuint list, GLenum pname, GLint param);
extern PFN_GLLISTPARAMETERISGIX _glListParameteriSGIX;

typedef void (APIENTRY * PFN_GLLISTPARAMETERIVSGIX)(GLuint list, GLenum pname, const GLint * params);
extern PFN_GLLISTPARAMETERIVSGIX _glListParameterivSGIX;

typedef void (APIENTRY * PFN_GLPIXELTEXGENSGIX)(GLenum mode);
extern PFN_GLPIXELTEXGENSGIX _glPixelTexGenSGIX;

typedef void (APIENTRY * PFN_GLDEFORMATIONMAP3DSGIX)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble * points);
extern PFN_GLDEFORMATIONMAP3DSGIX _glDeformationMap3dSGIX;

typedef void (APIENTRY * PFN_GLDEFORMATIONMAP3FSGIX)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat * points);
extern PFN_GLDEFORMATIONMAP3FSGIX _glDeformationMap3fSGIX;

typedef void (APIENTRY * PFN_GLDEFORMSGIX)(GLbitfield mask);
extern PFN_GLDEFORMSGIX _glDeformSGIX;

typedef void (APIENTRY * PFN_GLLOADIDENTITYDEFORMATIONMAPSGIX)(GLbitfield mask);
extern PFN_GLLOADIDENTITYDEFORMATIONMAPSGIX _glLoadIdentityDeformationMapSGIX;

typedef void (APIENTRY * PFN_GLREFERENCEPLANESGIX)(const GLdouble * equation);
extern PFN_GLREFERENCEPLANESGIX _glReferencePlaneSGIX;

typedef void (APIENTRY * PFN_GLSPRITEPARAMETERFSGIX)(GLenum pname, GLfloat param);
extern PFN_GLSPRITEPARAMETERFSGIX _glSpriteParameterfSGIX;

typedef void (APIENTRY * PFN_GLSPRITEPARAMETERFVSGIX)(GLenum pname, const GLfloat * params);
extern PFN_GLSPRITEPARAMETERFVSGIX _glSpriteParameterfvSGIX;

typedef void (APIENTRY * PFN_GLSPRITEPARAMETERISGIX)(GLenum pname, GLint param);
extern PFN_GLSPRITEPARAMETERISGIX _glSpriteParameteriSGIX;

typedef void (APIENTRY * PFN_GLSPRITEPARAMETERIVSGIX)(GLenum pname, const GLint * params);
extern PFN_GLSPRITEPARAMETERIVSGIX _glSpriteParameterivSGIX;

typedef void (APIENTRY * PFN_GLTAGSAMPLEBUFFERSGIX)(void);
extern PFN_GLTAGSAMPLEBUFFERSGIX _glTagSampleBufferSGIX;

typedef void (APIENTRY * PFN_GLCOLORTABLESGI)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);
extern PFN_GLCOLORTABLESGI _glColorTableSGI;

typedef void (APIENTRY * PFN_GLCOLORTABLEPARAMETERFVSGI)(GLenum target, GLenum pname, const GLfloat * params);
extern PFN_GLCOLORTABLEPARAMETERFVSGI _glColorTableParameterfvSGI;

typedef void (APIENTRY * PFN_GLCOLORTABLEPARAMETERIVSGI)(GLenum target, GLenum pname, const GLint * params);
extern PFN_GLCOLORTABLEPARAMETERIVSGI _glColorTableParameterivSGI;

typedef void (APIENTRY * PFN_GLCOPYCOLORTABLESGI)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern PFN_GLCOPYCOLORTABLESGI _glCopyColorTableSGI;

typedef void (APIENTRY * PFN_GLGETCOLORTABLESGI)(GLenum target, GLenum format, GLenum type, GLvoid * table);
extern PFN_GLGETCOLORTABLESGI _glGetColorTableSGI;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERFVSGI)(GLenum target, GLenum pname, GLfloat * params);
extern PFN_GLGETCOLORTABLEPARAMETERFVSGI _glGetColorTableParameterfvSGI;

typedef void (APIENTRY * PFN_GLGETCOLORTABLEPARAMETERIVSGI)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETCOLORTABLEPARAMETERIVSGI _glGetColorTableParameterivSGI;

typedef void (APIENTRY * PFN_GLFINISHTEXTURESUNX)(void);
extern PFN_GLFINISHTEXTURESUNX _glFinishTextureSUNX;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORBSUN)(GLbyte factor);
extern PFN_GLGLOBALALPHAFACTORBSUN _glGlobalAlphaFactorbSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORSSUN)(GLshort factor);
extern PFN_GLGLOBALALPHAFACTORSSUN _glGlobalAlphaFactorsSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORISUN)(GLint factor);
extern PFN_GLGLOBALALPHAFACTORISUN _glGlobalAlphaFactoriSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORFSUN)(GLfloat factor);
extern PFN_GLGLOBALALPHAFACTORFSUN _glGlobalAlphaFactorfSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORDSUN)(GLdouble factor);
extern PFN_GLGLOBALALPHAFACTORDSUN _glGlobalAlphaFactordSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORUBSUN)(GLubyte factor);
extern PFN_GLGLOBALALPHAFACTORUBSUN _glGlobalAlphaFactorubSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORUSSUN)(GLushort factor);
extern PFN_GLGLOBALALPHAFACTORUSSUN _glGlobalAlphaFactorusSUN;

typedef void (APIENTRY * PFN_GLGLOBALALPHAFACTORUISUN)(GLuint factor);
extern PFN_GLGLOBALALPHAFACTORUISUN _glGlobalAlphaFactoruiSUN;

typedef void (APIENTRY * PFN_GLDRAWMESHARRAYSSUN)(GLenum mode, GLint first, GLsizei count, GLsizei width);
extern PFN_GLDRAWMESHARRAYSSUN _glDrawMeshArraysSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUISUN)(GLuint code);
extern PFN_GLREPLACEMENTCODEUISUN _glReplacementCodeuiSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUSSUN)(GLushort code);
extern PFN_GLREPLACEMENTCODEUSSUN _glReplacementCodeusSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUBSUN)(GLubyte code);
extern PFN_GLREPLACEMENTCODEUBSUN _glReplacementCodeubSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUIVSUN)(const GLuint * code);
extern PFN_GLREPLACEMENTCODEUIVSUN _glReplacementCodeuivSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUSVSUN)(const GLushort * code);
extern PFN_GLREPLACEMENTCODEUSVSUN _glReplacementCodeusvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUBVSUN)(const GLubyte * code);
extern PFN_GLREPLACEMENTCODEUBVSUN _glReplacementCodeubvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEPOINTERSUN)(GLenum type, GLsizei stride, const GLvoid * * pointer);
extern PFN_GLREPLACEMENTCODEPOINTERSUN _glReplacementCodePointerSUN;

typedef void (APIENTRY * PFN_GLCOLOR4UBVERTEX2FSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
extern PFN_GLCOLOR4UBVERTEX2FSUN _glColor4ubVertex2fSUN;

typedef void (APIENTRY * PFN_GLCOLOR4UBVERTEX2FVSUN)(const GLubyte * c, const GLfloat * v);
extern PFN_GLCOLOR4UBVERTEX2FVSUN _glColor4ubVertex2fvSUN;

typedef void (APIENTRY * PFN_GLCOLOR4UBVERTEX3FSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLCOLOR4UBVERTEX3FSUN _glColor4ubVertex3fSUN;

typedef void (APIENTRY * PFN_GLCOLOR4UBVERTEX3FVSUN)(const GLubyte * c, const GLfloat * v);
extern PFN_GLCOLOR4UBVERTEX3FVSUN _glColor4ubVertex3fvSUN;

typedef void (APIENTRY * PFN_GLCOLOR3FVERTEX3FSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLCOLOR3FVERTEX3FSUN _glColor3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLCOLOR3FVERTEX3FVSUN)(const GLfloat * c, const GLfloat * v);
extern PFN_GLCOLOR3FVERTEX3FVSUN _glColor3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLNORMAL3FVERTEX3FSUN)(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLNORMAL3FVERTEX3FSUN _glNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLNORMAL3FVERTEX3FVSUN)(const GLfloat * n, const GLfloat * v);
extern PFN_GLNORMAL3FVERTEX3FVSUN _glNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLCOLOR4FNORMAL3FVERTEX3FSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLCOLOR4FNORMAL3FVERTEX3FSUN _glColor4fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLCOLOR4FNORMAL3FVERTEX3FVSUN)(const GLfloat * c, const GLfloat * n, const GLfloat * v);
extern PFN_GLCOLOR4FNORMAL3FVERTEX3FVSUN _glColor4fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FVERTEX3FSUN)(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTEXCOORD2FVERTEX3FSUN _glTexCoord2fVertex3fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FVERTEX3FVSUN)(const GLfloat * tc, const GLfloat * v);
extern PFN_GLTEXCOORD2FVERTEX3FVSUN _glTexCoord2fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD4FVERTEX4FSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLTEXCOORD4FVERTEX4FSUN _glTexCoord4fVertex4fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD4FVERTEX4FVSUN)(const GLfloat * tc, const GLfloat * v);
extern PFN_GLTEXCOORD4FVERTEX4FVSUN _glTexCoord4fVertex4fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR4UBVERTEX3FSUN)(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTEXCOORD2FCOLOR4UBVERTEX3FSUN _glTexCoord2fColor4ubVertex3fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR4UBVERTEX3FVSUN)(const GLfloat * tc, const GLubyte * c, const GLfloat * v);
extern PFN_GLTEXCOORD2FCOLOR4UBVERTEX3FVSUN _glTexCoord2fColor4ubVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR3FVERTEX3FSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTEXCOORD2FCOLOR3FVERTEX3FSUN _glTexCoord2fColor3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR3FVERTEX3FVSUN)(const GLfloat * tc, const GLfloat * c, const GLfloat * v);
extern PFN_GLTEXCOORD2FCOLOR3FVERTEX3FVSUN _glTexCoord2fColor3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FNORMAL3FVERTEX3FSUN)(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTEXCOORD2FNORMAL3FVERTEX3FSUN _glTexCoord2fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FNORMAL3FVERTEX3FVSUN)(const GLfloat * tc, const GLfloat * n, const GLfloat * v);
extern PFN_GLTEXCOORD2FNORMAL3FVERTEX3FVSUN _glTexCoord2fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUN _glTexCoord2fColor4fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUN)(const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);
extern PFN_GLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUN _glTexCoord2fColor4fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFN_GLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUN _glTexCoord4fColor4fNormal3fVertex4fSUN;

typedef void (APIENTRY * PFN_GLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUN)(const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);
extern PFN_GLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUN _glTexCoord4fColor4fNormal3fVertex4fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUIVERTEX3FSUN)(GLuint rc, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUIVERTEX3FSUN _glReplacementCodeuiVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUIVERTEX3FVSUN)(const GLuint * rc, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUIVERTEX3FVSUN _glReplacementCodeuiVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUN)(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUN _glReplacementCodeuiColor4ubVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUN)(const GLuint * rc, const GLubyte * c, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUN _glReplacementCodeuiColor4ubVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR3FVERTEX3FSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUICOLOR3FVERTEX3FSUN _glReplacementCodeuiColor3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * c, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUN _glReplacementCodeuiColor3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUINORMAL3FVERTEX3FSUN)(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUINORMAL3FVERTEX3FSUN _glReplacementCodeuiNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * n, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUN _glReplacementCodeuiNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUN _glReplacementCodeuiColor4fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * c, const GLfloat * n, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUN _glReplacementCodeuiColor4fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUN _glReplacementCodeuiTexCoord2fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * tc, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUN _glReplacementCodeuiTexCoord2fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUN _glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * tc, const GLfloat * n, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUN _glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUN _glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;

typedef void (APIENTRY * PFN_GLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUN)(const GLuint * rc, const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);
extern PFN_GLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUN _glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;

typedef void (APIENTRY * PFN_GLADDSWAPHINTRECTWIN)(GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLADDSWAPHINTRECTWIN _glAddSwapHintRectWIN;

#ifdef RETRACE
#define glCullFace _glCullFace
#define glFrontFace _glFrontFace
#define glHint _glHint
#define glLineWidth _glLineWidth
#define glPointSize _glPointSize
#define glPolygonMode _glPolygonMode
#define glScissor _glScissor
#define glTexParameterf _glTexParameterf
#define glTexParameterfv _glTexParameterfv
#define glTexParameteri _glTexParameteri
#define glTexParameteriv _glTexParameteriv
#define glTexImage1D _glTexImage1D
#define glTexImage2D _glTexImage2D
#define glDrawBuffer _glDrawBuffer
#define glClear _glClear
#define glClearColor _glClearColor
#define glClearStencil _glClearStencil
#define glClearDepth _glClearDepth
#define glStencilMask _glStencilMask
#define glColorMask _glColorMask
#define glDepthMask _glDepthMask
#define glDisable _glDisable
#define glEnable _glEnable
#define glFinish _glFinish
#define glFlush _glFlush
#define glBlendFunc _glBlendFunc
#define glLogicOp _glLogicOp
#define glStencilFunc _glStencilFunc
#define glStencilOp _glStencilOp
#define glDepthFunc _glDepthFunc
#define glPixelStoref _glPixelStoref
#define glPixelStorei _glPixelStorei
#define glReadBuffer _glReadBuffer
#define glReadPixels _glReadPixels
#define glGetBooleanv _glGetBooleanv
#define glGetDoublev _glGetDoublev
#define glGetError _glGetError
#define glGetFloatv _glGetFloatv
#define glGetIntegerv _glGetIntegerv
#define glGetString _glGetString
#define glGetTexImage _glGetTexImage
#define glGetTexParameterfv _glGetTexParameterfv
#define glGetTexParameteriv _glGetTexParameteriv
#define glGetTexLevelParameterfv _glGetTexLevelParameterfv
#define glGetTexLevelParameteriv _glGetTexLevelParameteriv
#define glIsEnabled _glIsEnabled
#define glDepthRange _glDepthRange
#define glViewport _glViewport
#define glNewList _glNewList
#define glEndList _glEndList
#define glCallList _glCallList
#define glCallLists _glCallLists
#define glDeleteLists _glDeleteLists
#define glGenLists _glGenLists
#define glListBase _glListBase
#define glBegin _glBegin
#define glBitmap _glBitmap
#define glColor3b _glColor3b
#define glColor3bv _glColor3bv
#define glColor3d _glColor3d
#define glColor3dv _glColor3dv
#define glColor3f _glColor3f
#define glColor3fv _glColor3fv
#define glColor3i _glColor3i
#define glColor3iv _glColor3iv
#define glColor3s _glColor3s
#define glColor3sv _glColor3sv
#define glColor3ub _glColor3ub
#define glColor3ubv _glColor3ubv
#define glColor3ui _glColor3ui
#define glColor3uiv _glColor3uiv
#define glColor3us _glColor3us
#define glColor3usv _glColor3usv
#define glColor4b _glColor4b
#define glColor4bv _glColor4bv
#define glColor4d _glColor4d
#define glColor4dv _glColor4dv
#define glColor4f _glColor4f
#define glColor4fv _glColor4fv
#define glColor4i _glColor4i
#define glColor4iv _glColor4iv
#define glColor4s _glColor4s
#define glColor4sv _glColor4sv
#define glColor4ub _glColor4ub
#define glColor4ubv _glColor4ubv
#define glColor4ui _glColor4ui
#define glColor4uiv _glColor4uiv
#define glColor4us _glColor4us
#define glColor4usv _glColor4usv
#define glEdgeFlag _glEdgeFlag
#define glEdgeFlagv _glEdgeFlagv
#define glEnd _glEnd
#define glIndexd _glIndexd
#define glIndexdv _glIndexdv
#define glIndexf _glIndexf
#define glIndexfv _glIndexfv
#define glIndexi _glIndexi
#define glIndexiv _glIndexiv
#define glIndexs _glIndexs
#define glIndexsv _glIndexsv
#define glNormal3b _glNormal3b
#define glNormal3bv _glNormal3bv
#define glNormal3d _glNormal3d
#define glNormal3dv _glNormal3dv
#define glNormal3f _glNormal3f
#define glNormal3fv _glNormal3fv
#define glNormal3i _glNormal3i
#define glNormal3iv _glNormal3iv
#define glNormal3s _glNormal3s
#define glNormal3sv _glNormal3sv
#define glRasterPos2d _glRasterPos2d
#define glRasterPos2dv _glRasterPos2dv
#define glRasterPos2f _glRasterPos2f
#define glRasterPos2fv _glRasterPos2fv
#define glRasterPos2i _glRasterPos2i
#define glRasterPos2iv _glRasterPos2iv
#define glRasterPos2s _glRasterPos2s
#define glRasterPos2sv _glRasterPos2sv
#define glRasterPos3d _glRasterPos3d
#define glRasterPos3dv _glRasterPos3dv
#define glRasterPos3f _glRasterPos3f
#define glRasterPos3fv _glRasterPos3fv
#define glRasterPos3i _glRasterPos3i
#define glRasterPos3iv _glRasterPos3iv
#define glRasterPos3s _glRasterPos3s
#define glRasterPos3sv _glRasterPos3sv
#define glRasterPos4d _glRasterPos4d
#define glRasterPos4dv _glRasterPos4dv
#define glRasterPos4f _glRasterPos4f
#define glRasterPos4fv _glRasterPos4fv
#define glRasterPos4i _glRasterPos4i
#define glRasterPos4iv _glRasterPos4iv
#define glRasterPos4s _glRasterPos4s
#define glRasterPos4sv _glRasterPos4sv
#define glRectd _glRectd
#define glRectdv _glRectdv
#define glRectf _glRectf
#define glRectfv _glRectfv
#define glRecti _glRecti
#define glRectiv _glRectiv
#define glRects _glRects
#define glRectsv _glRectsv
#define glTexCoord1d _glTexCoord1d
#define glTexCoord1dv _glTexCoord1dv
#define glTexCoord1f _glTexCoord1f
#define glTexCoord1fv _glTexCoord1fv
#define glTexCoord1i _glTexCoord1i
#define glTexCoord1iv _glTexCoord1iv
#define glTexCoord1s _glTexCoord1s
#define glTexCoord1sv _glTexCoord1sv
#define glTexCoord2d _glTexCoord2d
#define glTexCoord2dv _glTexCoord2dv
#define glTexCoord2f _glTexCoord2f
#define glTexCoord2fv _glTexCoord2fv
#define glTexCoord2i _glTexCoord2i
#define glTexCoord2iv _glTexCoord2iv
#define glTexCoord2s _glTexCoord2s
#define glTexCoord2sv _glTexCoord2sv
#define glTexCoord3d _glTexCoord3d
#define glTexCoord3dv _glTexCoord3dv
#define glTexCoord3f _glTexCoord3f
#define glTexCoord3fv _glTexCoord3fv
#define glTexCoord3i _glTexCoord3i
#define glTexCoord3iv _glTexCoord3iv
#define glTexCoord3s _glTexCoord3s
#define glTexCoord3sv _glTexCoord3sv
#define glTexCoord4d _glTexCoord4d
#define glTexCoord4dv _glTexCoord4dv
#define glTexCoord4f _glTexCoord4f
#define glTexCoord4fv _glTexCoord4fv
#define glTexCoord4i _glTexCoord4i
#define glTexCoord4iv _glTexCoord4iv
#define glTexCoord4s _glTexCoord4s
#define glTexCoord4sv _glTexCoord4sv
#define glVertex2d _glVertex2d
#define glVertex2dv _glVertex2dv
#define glVertex2f _glVertex2f
#define glVertex2fv _glVertex2fv
#define glVertex2i _glVertex2i
#define glVertex2iv _glVertex2iv
#define glVertex2s _glVertex2s
#define glVertex2sv _glVertex2sv
#define glVertex3d _glVertex3d
#define glVertex3dv _glVertex3dv
#define glVertex3f _glVertex3f
#define glVertex3fv _glVertex3fv
#define glVertex3i _glVertex3i
#define glVertex3iv _glVertex3iv
#define glVertex3s _glVertex3s
#define glVertex3sv _glVertex3sv
#define glVertex4d _glVertex4d
#define glVertex4dv _glVertex4dv
#define glVertex4f _glVertex4f
#define glVertex4fv _glVertex4fv
#define glVertex4i _glVertex4i
#define glVertex4iv _glVertex4iv
#define glVertex4s _glVertex4s
#define glVertex4sv _glVertex4sv
#define glClipPlane _glClipPlane
#define glColorMaterial _glColorMaterial
#define glFogf _glFogf
#define glFogfv _glFogfv
#define glFogi _glFogi
#define glFogiv _glFogiv
#define glLightf _glLightf
#define glLightfv _glLightfv
#define glLighti _glLighti
#define glLightiv _glLightiv
#define glLightModelf _glLightModelf
#define glLightModelfv _glLightModelfv
#define glLightModeli _glLightModeli
#define glLightModeliv _glLightModeliv
#define glLineStipple _glLineStipple
#define glMaterialf _glMaterialf
#define glMaterialfv _glMaterialfv
#define glMateriali _glMateriali
#define glMaterialiv _glMaterialiv
#define glPolygonStipple _glPolygonStipple
#define glShadeModel _glShadeModel
#define glTexEnvf _glTexEnvf
#define glTexEnvfv _glTexEnvfv
#define glTexEnvi _glTexEnvi
#define glTexEnviv _glTexEnviv
#define glTexGend _glTexGend
#define glTexGendv _glTexGendv
#define glTexGenf _glTexGenf
#define glTexGenfv _glTexGenfv
#define glTexGeni _glTexGeni
#define glTexGeniv _glTexGeniv
#define glFeedbackBuffer _glFeedbackBuffer
#define glSelectBuffer _glSelectBuffer
#define glRenderMode _glRenderMode
#define glInitNames _glInitNames
#define glLoadName _glLoadName
#define glPassThrough _glPassThrough
#define glPopName _glPopName
#define glPushName _glPushName
#define glClearAccum _glClearAccum
#define glClearIndex _glClearIndex
#define glIndexMask _glIndexMask
#define glAccum _glAccum
#define glPopAttrib _glPopAttrib
#define glPushAttrib _glPushAttrib
#define glMap1d _glMap1d
#define glMap1f _glMap1f
#define glMap2d _glMap2d
#define glMap2f _glMap2f
#define glMapGrid1d _glMapGrid1d
#define glMapGrid1f _glMapGrid1f
#define glMapGrid2d _glMapGrid2d
#define glMapGrid2f _glMapGrid2f
#define glEvalCoord1d _glEvalCoord1d
#define glEvalCoord1dv _glEvalCoord1dv
#define glEvalCoord1f _glEvalCoord1f
#define glEvalCoord1fv _glEvalCoord1fv
#define glEvalCoord2d _glEvalCoord2d
#define glEvalCoord2dv _glEvalCoord2dv
#define glEvalCoord2f _glEvalCoord2f
#define glEvalCoord2fv _glEvalCoord2fv
#define glEvalMesh1 _glEvalMesh1
#define glEvalPoint1 _glEvalPoint1
#define glEvalMesh2 _glEvalMesh2
#define glEvalPoint2 _glEvalPoint2
#define glAlphaFunc _glAlphaFunc
#define glPixelZoom _glPixelZoom
#define glPixelTransferf _glPixelTransferf
#define glPixelTransferi _glPixelTransferi
#define glPixelMapfv _glPixelMapfv
#define glPixelMapuiv _glPixelMapuiv
#define glPixelMapusv _glPixelMapusv
#define glCopyPixels _glCopyPixels
#define glDrawPixels _glDrawPixels
#define glGetClipPlane _glGetClipPlane
#define glGetLightfv _glGetLightfv
#define glGetLightiv _glGetLightiv
#define glGetMapdv _glGetMapdv
#define glGetMapfv _glGetMapfv
#define glGetMapiv _glGetMapiv
#define glGetMaterialfv _glGetMaterialfv
#define glGetMaterialiv _glGetMaterialiv
#define glGetPixelMapfv _glGetPixelMapfv
#define glGetPixelMapuiv _glGetPixelMapuiv
#define glGetPixelMapusv _glGetPixelMapusv
#define glGetPolygonStipple _glGetPolygonStipple
#define glGetTexEnvfv _glGetTexEnvfv
#define glGetTexEnviv _glGetTexEnviv
#define glGetTexGendv _glGetTexGendv
#define glGetTexGenfv _glGetTexGenfv
#define glGetTexGeniv _glGetTexGeniv
#define glIsList _glIsList
#define glFrustum _glFrustum
#define glLoadIdentity _glLoadIdentity
#define glLoadMatrixf _glLoadMatrixf
#define glLoadMatrixd _glLoadMatrixd
#define glMatrixMode _glMatrixMode
#define glMultMatrixf _glMultMatrixf
#define glMultMatrixd _glMultMatrixd
#define glOrtho _glOrtho
#define glPopMatrix _glPopMatrix
#define glPushMatrix _glPushMatrix
#define glRotated _glRotated
#define glRotatef _glRotatef
#define glScaled _glScaled
#define glScalef _glScalef
#define glTranslated _glTranslated
#define glTranslatef _glTranslatef
#define glDrawArrays _glDrawArrays
#define glDrawElements _glDrawElements
#define glGetPointerv _glGetPointerv
#define glPolygonOffset _glPolygonOffset
#define glCopyTexImage1D _glCopyTexImage1D
#define glCopyTexImage2D _glCopyTexImage2D
#define glCopyTexSubImage1D _glCopyTexSubImage1D
#define glCopyTexSubImage2D _glCopyTexSubImage2D
#define glTexSubImage1D _glTexSubImage1D
#define glTexSubImage2D _glTexSubImage2D
#define glBindTexture _glBindTexture
#define glDeleteTextures _glDeleteTextures
#define glGenTextures _glGenTextures
#define glIsTexture _glIsTexture
#define glArrayElement _glArrayElement
#define glColorPointer _glColorPointer
#define glDisableClientState _glDisableClientState
#define glEdgeFlagPointer _glEdgeFlagPointer
#define glEnableClientState _glEnableClientState
#define glIndexPointer _glIndexPointer
#define glInterleavedArrays _glInterleavedArrays
#define glNormalPointer _glNormalPointer
#define glTexCoordPointer _glTexCoordPointer
#define glVertexPointer _glVertexPointer
#define glAreTexturesResident _glAreTexturesResident
#define glPrioritizeTextures _glPrioritizeTextures
#define glIndexub _glIndexub
#define glIndexubv _glIndexubv
#define glPopClientAttrib _glPopClientAttrib
#define glPushClientAttrib _glPushClientAttrib
#define glBlendColor _glBlendColor
#define glBlendEquation _glBlendEquation
#define glDrawRangeElements _glDrawRangeElements
#define glTexImage3D _glTexImage3D
#define glTexSubImage3D _glTexSubImage3D
#define glCopyTexSubImage3D _glCopyTexSubImage3D
#define glColorTable _glColorTable
#define glColorTableParameterfv _glColorTableParameterfv
#define glColorTableParameteriv _glColorTableParameteriv
#define glCopyColorTable _glCopyColorTable
#define glGetColorTable _glGetColorTable
#define glGetColorTableParameterfv _glGetColorTableParameterfv
#define glGetColorTableParameteriv _glGetColorTableParameteriv
#define glColorSubTable _glColorSubTable
#define glCopyColorSubTable _glCopyColorSubTable
#define glConvolutionFilter1D _glConvolutionFilter1D
#define glConvolutionFilter2D _glConvolutionFilter2D
#define glConvolutionParameterf _glConvolutionParameterf
#define glConvolutionParameterfv _glConvolutionParameterfv
#define glConvolutionParameteri _glConvolutionParameteri
#define glConvolutionParameteriv _glConvolutionParameteriv
#define glCopyConvolutionFilter1D _glCopyConvolutionFilter1D
#define glCopyConvolutionFilter2D _glCopyConvolutionFilter2D
#define glGetConvolutionFilter _glGetConvolutionFilter
#define glGetConvolutionParameterfv _glGetConvolutionParameterfv
#define glGetConvolutionParameteriv _glGetConvolutionParameteriv
#define glGetSeparableFilter _glGetSeparableFilter
#define glSeparableFilter2D _glSeparableFilter2D
#define glGetHistogram _glGetHistogram
#define glGetHistogramParameterfv _glGetHistogramParameterfv
#define glGetHistogramParameteriv _glGetHistogramParameteriv
#define glGetMinmax _glGetMinmax
#define glGetMinmaxParameterfv _glGetMinmaxParameterfv
#define glGetMinmaxParameteriv _glGetMinmaxParameteriv
#define glHistogram _glHistogram
#define glMinmax _glMinmax
#define glResetHistogram _glResetHistogram
#define glResetMinmax _glResetMinmax
#define glActiveTexture _glActiveTexture
#define glSampleCoverage _glSampleCoverage
#define glCompressedTexImage3D _glCompressedTexImage3D
#define glCompressedTexImage2D _glCompressedTexImage2D
#define glCompressedTexImage1D _glCompressedTexImage1D
#define glCompressedTexSubImage3D _glCompressedTexSubImage3D
#define glCompressedTexSubImage2D _glCompressedTexSubImage2D
#define glCompressedTexSubImage1D _glCompressedTexSubImage1D
#define glGetCompressedTexImage _glGetCompressedTexImage
#define glClientActiveTexture _glClientActiveTexture
#define glMultiTexCoord1d _glMultiTexCoord1d
#define glMultiTexCoord1dv _glMultiTexCoord1dv
#define glMultiTexCoord1f _glMultiTexCoord1f
#define glMultiTexCoord1fv _glMultiTexCoord1fv
#define glMultiTexCoord1i _glMultiTexCoord1i
#define glMultiTexCoord1iv _glMultiTexCoord1iv
#define glMultiTexCoord1s _glMultiTexCoord1s
#define glMultiTexCoord1sv _glMultiTexCoord1sv
#define glMultiTexCoord2d _glMultiTexCoord2d
#define glMultiTexCoord2dv _glMultiTexCoord2dv
#define glMultiTexCoord2f _glMultiTexCoord2f
#define glMultiTexCoord2fv _glMultiTexCoord2fv
#define glMultiTexCoord2i _glMultiTexCoord2i
#define glMultiTexCoord2iv _glMultiTexCoord2iv
#define glMultiTexCoord2s _glMultiTexCoord2s
#define glMultiTexCoord2sv _glMultiTexCoord2sv
#define glMultiTexCoord3d _glMultiTexCoord3d
#define glMultiTexCoord3dv _glMultiTexCoord3dv
#define glMultiTexCoord3f _glMultiTexCoord3f
#define glMultiTexCoord3fv _glMultiTexCoord3fv
#define glMultiTexCoord3i _glMultiTexCoord3i
#define glMultiTexCoord3iv _glMultiTexCoord3iv
#define glMultiTexCoord3s _glMultiTexCoord3s
#define glMultiTexCoord3sv _glMultiTexCoord3sv
#define glMultiTexCoord4d _glMultiTexCoord4d
#define glMultiTexCoord4dv _glMultiTexCoord4dv
#define glMultiTexCoord4f _glMultiTexCoord4f
#define glMultiTexCoord4fv _glMultiTexCoord4fv
#define glMultiTexCoord4i _glMultiTexCoord4i
#define glMultiTexCoord4iv _glMultiTexCoord4iv
#define glMultiTexCoord4s _glMultiTexCoord4s
#define glMultiTexCoord4sv _glMultiTexCoord4sv
#define glLoadTransposeMatrixf _glLoadTransposeMatrixf
#define glLoadTransposeMatrixd _glLoadTransposeMatrixd
#define glMultTransposeMatrixf _glMultTransposeMatrixf
#define glMultTransposeMatrixd _glMultTransposeMatrixd
#define glBlendFuncSeparate _glBlendFuncSeparate
#define glMultiDrawArrays _glMultiDrawArrays
#define glMultiDrawElements _glMultiDrawElements
#define glPointParameterf _glPointParameterf
#define glPointParameterfv _glPointParameterfv
#define glPointParameteri _glPointParameteri
#define glPointParameteriv _glPointParameteriv
#define glFogCoordf _glFogCoordf
#define glFogCoordfv _glFogCoordfv
#define glFogCoordd _glFogCoordd
#define glFogCoorddv _glFogCoorddv
#define glFogCoordPointer _glFogCoordPointer
#define glSecondaryColor3b _glSecondaryColor3b
#define glSecondaryColor3bv _glSecondaryColor3bv
#define glSecondaryColor3d _glSecondaryColor3d
#define glSecondaryColor3dv _glSecondaryColor3dv
#define glSecondaryColor3f _glSecondaryColor3f
#define glSecondaryColor3fv _glSecondaryColor3fv
#define glSecondaryColor3i _glSecondaryColor3i
#define glSecondaryColor3iv _glSecondaryColor3iv
#define glSecondaryColor3s _glSecondaryColor3s
#define glSecondaryColor3sv _glSecondaryColor3sv
#define glSecondaryColor3ub _glSecondaryColor3ub
#define glSecondaryColor3ubv _glSecondaryColor3ubv
#define glSecondaryColor3ui _glSecondaryColor3ui
#define glSecondaryColor3uiv _glSecondaryColor3uiv
#define glSecondaryColor3us _glSecondaryColor3us
#define glSecondaryColor3usv _glSecondaryColor3usv
#define glSecondaryColorPointer _glSecondaryColorPointer
#define glWindowPos2d _glWindowPos2d
#define glWindowPos2dv _glWindowPos2dv
#define glWindowPos2f _glWindowPos2f
#define glWindowPos2fv _glWindowPos2fv
#define glWindowPos2i _glWindowPos2i
#define glWindowPos2iv _glWindowPos2iv
#define glWindowPos2s _glWindowPos2s
#define glWindowPos2sv _glWindowPos2sv
#define glWindowPos3d _glWindowPos3d
#define glWindowPos3dv _glWindowPos3dv
#define glWindowPos3f _glWindowPos3f
#define glWindowPos3fv _glWindowPos3fv
#define glWindowPos3i _glWindowPos3i
#define glWindowPos3iv _glWindowPos3iv
#define glWindowPos3s _glWindowPos3s
#define glWindowPos3sv _glWindowPos3sv
#define glGenQueries _glGenQueries
#define glDeleteQueries _glDeleteQueries
#define glIsQuery _glIsQuery
#define glBeginQuery _glBeginQuery
#define glEndQuery _glEndQuery
#define glGetQueryiv _glGetQueryiv
#define glGetQueryObjectiv _glGetQueryObjectiv
#define glGetQueryObjectuiv _glGetQueryObjectuiv
#define glBindBuffer _glBindBuffer
#define glDeleteBuffers _glDeleteBuffers
#define glGenBuffers _glGenBuffers
#define glIsBuffer _glIsBuffer
#define glBufferData _glBufferData
#define glBufferSubData _glBufferSubData
#define glGetBufferSubData _glGetBufferSubData
#define glMapBuffer _glMapBuffer
#define glUnmapBuffer _glUnmapBuffer
#define glGetBufferParameteriv _glGetBufferParameteriv
#define glGetBufferPointerv _glGetBufferPointerv
#define glBlendEquationSeparate _glBlendEquationSeparate
#define glDrawBuffers _glDrawBuffers
#define glStencilOpSeparate _glStencilOpSeparate
#define glStencilFuncSeparate _glStencilFuncSeparate
#define glStencilMaskSeparate _glStencilMaskSeparate
#define glAttachShader _glAttachShader
#define glBindAttribLocation _glBindAttribLocation
#define glCompileShader _glCompileShader
#define glCreateProgram _glCreateProgram
#define glCreateShader _glCreateShader
#define glDeleteProgram _glDeleteProgram
#define glDeleteShader _glDeleteShader
#define glDetachShader _glDetachShader
#define glDisableVertexAttribArray _glDisableVertexAttribArray
#define glEnableVertexAttribArray _glEnableVertexAttribArray
#define glGetActiveAttrib _glGetActiveAttrib
#define glGetActiveUniform _glGetActiveUniform
#define glGetAttachedShaders _glGetAttachedShaders
#define glGetAttribLocation _glGetAttribLocation
#define glGetProgramiv _glGetProgramiv
#define glGetProgramInfoLog _glGetProgramInfoLog
#define glGetShaderiv _glGetShaderiv
#define glGetShaderInfoLog _glGetShaderInfoLog
#define glGetShaderSource _glGetShaderSource
#define glGetUniformLocation _glGetUniformLocation
#define glGetUniformfv _glGetUniformfv
#define glGetUniformiv _glGetUniformiv
#define glGetVertexAttribdv _glGetVertexAttribdv
#define glGetVertexAttribfv _glGetVertexAttribfv
#define glGetVertexAttribiv _glGetVertexAttribiv
#define glGetVertexAttribPointerv _glGetVertexAttribPointerv
#define glIsProgram _glIsProgram
#define glIsShader _glIsShader
#define glLinkProgram _glLinkProgram
#define glShaderSource _glShaderSource
#define glUseProgram _glUseProgram
#define glUniform1f _glUniform1f
#define glUniform2f _glUniform2f
#define glUniform3f _glUniform3f
#define glUniform4f _glUniform4f
#define glUniform1i _glUniform1i
#define glUniform2i _glUniform2i
#define glUniform3i _glUniform3i
#define glUniform4i _glUniform4i
#define glUniform1fv _glUniform1fv
#define glUniform2fv _glUniform2fv
#define glUniform3fv _glUniform3fv
#define glUniform4fv _glUniform4fv
#define glUniform1iv _glUniform1iv
#define glUniform2iv _glUniform2iv
#define glUniform3iv _glUniform3iv
#define glUniform4iv _glUniform4iv
#define glUniformMatrix2fv _glUniformMatrix2fv
#define glUniformMatrix3fv _glUniformMatrix3fv
#define glUniformMatrix4fv _glUniformMatrix4fv
#define glValidateProgram _glValidateProgram
#define glVertexAttrib1d _glVertexAttrib1d
#define glVertexAttrib1dv _glVertexAttrib1dv
#define glVertexAttrib1f _glVertexAttrib1f
#define glVertexAttrib1fv _glVertexAttrib1fv
#define glVertexAttrib1s _glVertexAttrib1s
#define glVertexAttrib1sv _glVertexAttrib1sv
#define glVertexAttrib2d _glVertexAttrib2d
#define glVertexAttrib2dv _glVertexAttrib2dv
#define glVertexAttrib2f _glVertexAttrib2f
#define glVertexAttrib2fv _glVertexAttrib2fv
#define glVertexAttrib2s _glVertexAttrib2s
#define glVertexAttrib2sv _glVertexAttrib2sv
#define glVertexAttrib3d _glVertexAttrib3d
#define glVertexAttrib3dv _glVertexAttrib3dv
#define glVertexAttrib3f _glVertexAttrib3f
#define glVertexAttrib3fv _glVertexAttrib3fv
#define glVertexAttrib3s _glVertexAttrib3s
#define glVertexAttrib3sv _glVertexAttrib3sv
#define glVertexAttrib4Nbv _glVertexAttrib4Nbv
#define glVertexAttrib4Niv _glVertexAttrib4Niv
#define glVertexAttrib4Nsv _glVertexAttrib4Nsv
#define glVertexAttrib4Nub _glVertexAttrib4Nub
#define glVertexAttrib4Nubv _glVertexAttrib4Nubv
#define glVertexAttrib4Nuiv _glVertexAttrib4Nuiv
#define glVertexAttrib4Nusv _glVertexAttrib4Nusv
#define glVertexAttrib4bv _glVertexAttrib4bv
#define glVertexAttrib4d _glVertexAttrib4d
#define glVertexAttrib4dv _glVertexAttrib4dv
#define glVertexAttrib4f _glVertexAttrib4f
#define glVertexAttrib4fv _glVertexAttrib4fv
#define glVertexAttrib4iv _glVertexAttrib4iv
#define glVertexAttrib4s _glVertexAttrib4s
#define glVertexAttrib4sv _glVertexAttrib4sv
#define glVertexAttrib4ubv _glVertexAttrib4ubv
#define glVertexAttrib4uiv _glVertexAttrib4uiv
#define glVertexAttrib4usv _glVertexAttrib4usv
#define glVertexAttribPointer _glVertexAttribPointer
#define glUniformMatrix2x3fv _glUniformMatrix2x3fv
#define glUniformMatrix3x2fv _glUniformMatrix3x2fv
#define glUniformMatrix2x4fv _glUniformMatrix2x4fv
#define glUniformMatrix4x2fv _glUniformMatrix4x2fv
#define glUniformMatrix3x4fv _glUniformMatrix3x4fv
#define glUniformMatrix4x3fv _glUniformMatrix4x3fv
#define glColorMaski _glColorMaski
#define glGetBooleani_v _glGetBooleani_v
#define glGetIntegeri_v _glGetIntegeri_v
#define glEnablei _glEnablei
#define glDisablei _glDisablei
#define glIsEnabledi _glIsEnabledi
#define glBeginTransformFeedback _glBeginTransformFeedback
#define glEndTransformFeedback _glEndTransformFeedback
#define glBindBufferRange _glBindBufferRange
#define glBindBufferBase _glBindBufferBase
#define glTransformFeedbackVaryings _glTransformFeedbackVaryings
#define glGetTransformFeedbackVarying _glGetTransformFeedbackVarying
#define glClampColor _glClampColor
#define glBeginConditionalRender _glBeginConditionalRender
#define glEndConditionalRender _glEndConditionalRender
#define glVertexAttribIPointer _glVertexAttribIPointer
#define glGetVertexAttribIiv _glGetVertexAttribIiv
#define glGetVertexAttribIuiv _glGetVertexAttribIuiv
#define glVertexAttribI1i _glVertexAttribI1i
#define glVertexAttribI2i _glVertexAttribI2i
#define glVertexAttribI3i _glVertexAttribI3i
#define glVertexAttribI4i _glVertexAttribI4i
#define glVertexAttribI1ui _glVertexAttribI1ui
#define glVertexAttribI2ui _glVertexAttribI2ui
#define glVertexAttribI3ui _glVertexAttribI3ui
#define glVertexAttribI4ui _glVertexAttribI4ui
#define glVertexAttribI1iv _glVertexAttribI1iv
#define glVertexAttribI2iv _glVertexAttribI2iv
#define glVertexAttribI3iv _glVertexAttribI3iv
#define glVertexAttribI4iv _glVertexAttribI4iv
#define glVertexAttribI1uiv _glVertexAttribI1uiv
#define glVertexAttribI2uiv _glVertexAttribI2uiv
#define glVertexAttribI3uiv _glVertexAttribI3uiv
#define glVertexAttribI4uiv _glVertexAttribI4uiv
#define glVertexAttribI4bv _glVertexAttribI4bv
#define glVertexAttribI4sv _glVertexAttribI4sv
#define glVertexAttribI4ubv _glVertexAttribI4ubv
#define glVertexAttribI4usv _glVertexAttribI4usv
#define glGetUniformuiv _glGetUniformuiv
#define glBindFragDataLocation _glBindFragDataLocation
#define glGetFragDataLocation _glGetFragDataLocation
#define glUniform1ui _glUniform1ui
#define glUniform2ui _glUniform2ui
#define glUniform3ui _glUniform3ui
#define glUniform4ui _glUniform4ui
#define glUniform1uiv _glUniform1uiv
#define glUniform2uiv _glUniform2uiv
#define glUniform3uiv _glUniform3uiv
#define glUniform4uiv _glUniform4uiv
#define glTexParameterIiv _glTexParameterIiv
#define glTexParameterIuiv _glTexParameterIuiv
#define glGetTexParameterIiv _glGetTexParameterIiv
#define glGetTexParameterIuiv _glGetTexParameterIuiv
#define glClearBufferiv _glClearBufferiv
#define glClearBufferuiv _glClearBufferuiv
#define glClearBufferfv _glClearBufferfv
#define glClearBufferfi _glClearBufferfi
#define glGetStringi _glGetStringi
#define glDrawArraysInstanced _glDrawArraysInstanced
#define glDrawElementsInstanced _glDrawElementsInstanced
#define glTexBuffer _glTexBuffer
#define glPrimitiveRestartIndex _glPrimitiveRestartIndex
#define glGetInteger64i_v _glGetInteger64i_v
#define glGetBufferParameteri64v _glGetBufferParameteri64v
#define glFramebufferTexture _glFramebufferTexture
#define glVertexAttribDivisor _glVertexAttribDivisor
#define glMinSampleShading _glMinSampleShading
#define glBlendEquationi _glBlendEquationi
#define glBlendEquationSeparatei _glBlendEquationSeparatei
#define glBlendFunci _glBlendFunci
#define glBlendFuncSeparatei _glBlendFuncSeparatei
#define glTbufferMask3DFX _glTbufferMask3DFX
#define glDebugMessageEnableAMD _glDebugMessageEnableAMD
#define glDebugMessageInsertAMD _glDebugMessageInsertAMD
#define glDebugMessageCallbackAMD _glDebugMessageCallbackAMD
#define glGetDebugMessageLogAMD _glGetDebugMessageLogAMD
#define glBlendFuncIndexedAMD _glBlendFuncIndexedAMD
#define glBlendFuncSeparateIndexedAMD _glBlendFuncSeparateIndexedAMD
#define glBlendEquationIndexedAMD _glBlendEquationIndexedAMD
#define glBlendEquationSeparateIndexedAMD _glBlendEquationSeparateIndexedAMD
#define glMultiDrawArraysIndirectAMD _glMultiDrawArraysIndirectAMD
#define glMultiDrawElementsIndirectAMD _glMultiDrawElementsIndirectAMD
#define glGenNamesAMD _glGenNamesAMD
#define glDeleteNamesAMD _glDeleteNamesAMD
#define glIsNameAMD _glIsNameAMD
#define glGetPerfMonitorGroupsAMD _glGetPerfMonitorGroupsAMD
#define glGetPerfMonitorCountersAMD _glGetPerfMonitorCountersAMD
#define glGetPerfMonitorGroupStringAMD _glGetPerfMonitorGroupStringAMD
#define glGetPerfMonitorCounterStringAMD _glGetPerfMonitorCounterStringAMD
#define glGetPerfMonitorCounterInfoAMD _glGetPerfMonitorCounterInfoAMD
#define glGenPerfMonitorsAMD _glGenPerfMonitorsAMD
#define glDeletePerfMonitorsAMD _glDeletePerfMonitorsAMD
#define glSelectPerfMonitorCountersAMD _glSelectPerfMonitorCountersAMD
#define glBeginPerfMonitorAMD _glBeginPerfMonitorAMD
#define glEndPerfMonitorAMD _glEndPerfMonitorAMD
#define glGetPerfMonitorCounterDataAMD _glGetPerfMonitorCounterDataAMD
#define glSetMultisamplefvAMD _glSetMultisamplefvAMD
#define glTexStorageSparseAMD _glTexStorageSparseAMD
#define glTextureStorageSparseAMD _glTextureStorageSparseAMD
#define glStencilOpValueAMD _glStencilOpValueAMD
#define glTessellationFactorAMD _glTessellationFactorAMD
#define glTessellationModeAMD _glTessellationModeAMD
#define glBlitFramebufferANGLE _glBlitFramebufferANGLE
#define glRenderbufferStorageMultisampleANGLE _glRenderbufferStorageMultisampleANGLE
#define glDrawArraysInstancedANGLE _glDrawArraysInstancedANGLE
#define glDrawElementsInstancedANGLE _glDrawElementsInstancedANGLE
#define glVertexAttribDivisorANGLE _glVertexAttribDivisorANGLE
#define glGetTranslatedShaderSourceANGLE _glGetTranslatedShaderSourceANGLE
#define glCopyTextureLevelsAPPLE _glCopyTextureLevelsAPPLE
#define glElementPointerAPPLE _glElementPointerAPPLE
#define glDrawElementArrayAPPLE _glDrawElementArrayAPPLE
#define glDrawRangeElementArrayAPPLE _glDrawRangeElementArrayAPPLE
#define glMultiDrawElementArrayAPPLE _glMultiDrawElementArrayAPPLE
#define glMultiDrawRangeElementArrayAPPLE _glMultiDrawRangeElementArrayAPPLE
#define glGenFencesAPPLE _glGenFencesAPPLE
#define glDeleteFencesAPPLE _glDeleteFencesAPPLE
#define glSetFenceAPPLE _glSetFenceAPPLE
#define glIsFenceAPPLE _glIsFenceAPPLE
#define glTestFenceAPPLE _glTestFenceAPPLE
#define glFinishFenceAPPLE _glFinishFenceAPPLE
#define glTestObjectAPPLE _glTestObjectAPPLE
#define glFinishObjectAPPLE _glFinishObjectAPPLE
#define glBufferParameteriAPPLE _glBufferParameteriAPPLE
#define glFlushMappedBufferRangeAPPLE _glFlushMappedBufferRangeAPPLE
#define glFlushRenderAPPLE _glFlushRenderAPPLE
#define glFinishRenderAPPLE _glFinishRenderAPPLE
#define glSwapAPPLE _glSwapAPPLE
#define glRenderbufferStorageMultisampleAPPLE _glRenderbufferStorageMultisampleAPPLE
#define glResolveMultisampleFramebufferAPPLE _glResolveMultisampleFramebufferAPPLE
#define glObjectPurgeableAPPLE _glObjectPurgeableAPPLE
#define glObjectUnpurgeableAPPLE _glObjectUnpurgeableAPPLE
#define glGetObjectParameterivAPPLE _glGetObjectParameterivAPPLE
#define glFenceSyncAPPLE _glFenceSyncAPPLE
#define glIsSyncAPPLE _glIsSyncAPPLE
#define glDeleteSyncAPPLE _glDeleteSyncAPPLE
#define glClientWaitSyncAPPLE _glClientWaitSyncAPPLE
#define glWaitSyncAPPLE _glWaitSyncAPPLE
#define glGetInteger64vAPPLE _glGetInteger64vAPPLE
#define glGetSyncivAPPLE _glGetSyncivAPPLE
#define glTextureRangeAPPLE _glTextureRangeAPPLE
#define glGetTexParameterPointervAPPLE _glGetTexParameterPointervAPPLE
#define glBindVertexArrayAPPLE _glBindVertexArrayAPPLE
#define glDeleteVertexArraysAPPLE _glDeleteVertexArraysAPPLE
#define glGenVertexArraysAPPLE _glGenVertexArraysAPPLE
#define glIsVertexArrayAPPLE _glIsVertexArrayAPPLE
#define glVertexArrayRangeAPPLE _glVertexArrayRangeAPPLE
#define glFlushVertexArrayRangeAPPLE _glFlushVertexArrayRangeAPPLE
#define glVertexArrayParameteriAPPLE _glVertexArrayParameteriAPPLE
#define glEnableVertexAttribAPPLE _glEnableVertexAttribAPPLE
#define glDisableVertexAttribAPPLE _glDisableVertexAttribAPPLE
#define glIsVertexAttribEnabledAPPLE _glIsVertexAttribEnabledAPPLE
#define glMapVertexAttrib1dAPPLE _glMapVertexAttrib1dAPPLE
#define glMapVertexAttrib1fAPPLE _glMapVertexAttrib1fAPPLE
#define glMapVertexAttrib2dAPPLE _glMapVertexAttrib2dAPPLE
#define glMapVertexAttrib2fAPPLE _glMapVertexAttrib2fAPPLE
#define glReleaseShaderCompiler _glReleaseShaderCompiler
#define glShaderBinary _glShaderBinary
#define glGetShaderPrecisionFormat _glGetShaderPrecisionFormat
#define glDepthRangef _glDepthRangef
#define glClearDepthf _glClearDepthf
#define glMemoryBarrierByRegion _glMemoryBarrierByRegion
#define glDrawArraysInstancedBaseInstance _glDrawArraysInstancedBaseInstance
#define glDrawElementsInstancedBaseInstance _glDrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertexBaseInstance _glDrawElementsInstancedBaseVertexBaseInstance
#define glGetTextureHandleARB _glGetTextureHandleARB
#define glGetTextureSamplerHandleARB _glGetTextureSamplerHandleARB
#define glMakeTextureHandleResidentARB _glMakeTextureHandleResidentARB
#define glMakeTextureHandleNonResidentARB _glMakeTextureHandleNonResidentARB
#define glGetImageHandleARB _glGetImageHandleARB
#define glMakeImageHandleResidentARB _glMakeImageHandleResidentARB
#define glMakeImageHandleNonResidentARB _glMakeImageHandleNonResidentARB
#define glUniformHandleui64ARB _glUniformHandleui64ARB
#define glUniformHandleui64vARB _glUniformHandleui64vARB
#define glProgramUniformHandleui64ARB _glProgramUniformHandleui64ARB
#define glProgramUniformHandleui64vARB _glProgramUniformHandleui64vARB
#define glIsTextureHandleResidentARB _glIsTextureHandleResidentARB
#define glIsImageHandleResidentARB _glIsImageHandleResidentARB
#define glVertexAttribL1ui64ARB _glVertexAttribL1ui64ARB
#define glVertexAttribL1ui64vARB _glVertexAttribL1ui64vARB
#define glGetVertexAttribLui64vARB _glGetVertexAttribLui64vARB
#define glBindFragDataLocationIndexed _glBindFragDataLocationIndexed
#define glGetFragDataIndex _glGetFragDataIndex
#define glBufferStorage _glBufferStorage
#define glNamedBufferStorageEXT _glNamedBufferStorageEXT
#define glClearBufferData _glClearBufferData
#define glClearBufferSubData _glClearBufferSubData
#define glClearNamedBufferDataEXT _glClearNamedBufferDataEXT
#define glClearNamedBufferSubDataEXT _glClearNamedBufferSubDataEXT
#define glClearTexImage _glClearTexImage
#define glClearTexSubImage _glClearTexSubImage
#define glClipControl _glClipControl
#define glClampColorARB _glClampColorARB
#define glDispatchCompute _glDispatchCompute
#define glDispatchComputeIndirect _glDispatchComputeIndirect
#define glDispatchComputeGroupSizeARB _glDispatchComputeGroupSizeARB
#define glCopyBufferSubData _glCopyBufferSubData
#define glCopyImageSubData _glCopyImageSubData
#define glDebugMessageControlARB _glDebugMessageControlARB
#define glDebugMessageInsertARB _glDebugMessageInsertARB
#define glDebugMessageCallbackARB _glDebugMessageCallbackARB
#define glGetDebugMessageLogARB _glGetDebugMessageLogARB
#define glCreateTransformFeedbacks _glCreateTransformFeedbacks
#define glTransformFeedbackBufferBase _glTransformFeedbackBufferBase
#define glTransformFeedbackBufferRange _glTransformFeedbackBufferRange
#define glGetTransformFeedbackiv _glGetTransformFeedbackiv
#define glGetTransformFeedbacki_v _glGetTransformFeedbacki_v
#define glGetTransformFeedbacki64_v _glGetTransformFeedbacki64_v
#define glCreateBuffers _glCreateBuffers
#define glNamedBufferStorage _glNamedBufferStorage
#define glNamedBufferData _glNamedBufferData
#define glNamedBufferSubData _glNamedBufferSubData
#define glCopyNamedBufferSubData _glCopyNamedBufferSubData
#define glClearNamedBufferData _glClearNamedBufferData
#define glClearNamedBufferSubData _glClearNamedBufferSubData
#define glMapNamedBuffer _glMapNamedBuffer
#define glMapNamedBufferRange _glMapNamedBufferRange
#define glUnmapNamedBuffer _glUnmapNamedBuffer
#define glFlushMappedNamedBufferRange _glFlushMappedNamedBufferRange
#define glGetNamedBufferParameteriv _glGetNamedBufferParameteriv
#define glGetNamedBufferParameteri64v _glGetNamedBufferParameteri64v
#define glGetNamedBufferPointerv _glGetNamedBufferPointerv
#define glGetNamedBufferSubData _glGetNamedBufferSubData
#define glCreateFramebuffers _glCreateFramebuffers
#define glNamedFramebufferRenderbuffer _glNamedFramebufferRenderbuffer
#define glNamedFramebufferParameteri _glNamedFramebufferParameteri
#define glNamedFramebufferTexture _glNamedFramebufferTexture
#define glNamedFramebufferTextureLayer _glNamedFramebufferTextureLayer
#define glNamedFramebufferDrawBuffer _glNamedFramebufferDrawBuffer
#define glNamedFramebufferDrawBuffers _glNamedFramebufferDrawBuffers
#define glNamedFramebufferReadBuffer _glNamedFramebufferReadBuffer
#define glInvalidateNamedFramebufferData _glInvalidateNamedFramebufferData
#define glInvalidateNamedFramebufferSubData _glInvalidateNamedFramebufferSubData
#define glClearNamedFramebufferiv _glClearNamedFramebufferiv
#define glClearNamedFramebufferuiv _glClearNamedFramebufferuiv
#define glClearNamedFramebufferfv _glClearNamedFramebufferfv
#define glClearNamedFramebufferfi _glClearNamedFramebufferfi
#define glBlitNamedFramebuffer _glBlitNamedFramebuffer
#define glCheckNamedFramebufferStatus _glCheckNamedFramebufferStatus
#define glGetNamedFramebufferParameteriv _glGetNamedFramebufferParameteriv
#define glGetNamedFramebufferAttachmentParameteriv _glGetNamedFramebufferAttachmentParameteriv
#define glCreateRenderbuffers _glCreateRenderbuffers
#define glNamedRenderbufferStorage _glNamedRenderbufferStorage
#define glNamedRenderbufferStorageMultisample _glNamedRenderbufferStorageMultisample
#define glGetNamedRenderbufferParameteriv _glGetNamedRenderbufferParameteriv
#define glCreateTextures _glCreateTextures
#define glTextureBuffer _glTextureBuffer
#define glTextureBufferRange _glTextureBufferRange
#define glTextureStorage1D _glTextureStorage1D
#define glTextureStorage2D _glTextureStorage2D
#define glTextureStorage3D _glTextureStorage3D
#define glTextureStorage2DMultisample _glTextureStorage2DMultisample
#define glTextureStorage3DMultisample _glTextureStorage3DMultisample
#define glTextureSubImage1D _glTextureSubImage1D
#define glTextureSubImage2D _glTextureSubImage2D
#define glTextureSubImage3D _glTextureSubImage3D
#define glCompressedTextureSubImage1D _glCompressedTextureSubImage1D
#define glCompressedTextureSubImage2D _glCompressedTextureSubImage2D
#define glCompressedTextureSubImage3D _glCompressedTextureSubImage3D
#define glCopyTextureSubImage1D _glCopyTextureSubImage1D
#define glCopyTextureSubImage2D _glCopyTextureSubImage2D
#define glCopyTextureSubImage3D _glCopyTextureSubImage3D
#define glTextureParameterf _glTextureParameterf
#define glTextureParameterfv _glTextureParameterfv
#define glTextureParameteri _glTextureParameteri
#define glTextureParameterIiv _glTextureParameterIiv
#define glTextureParameterIuiv _glTextureParameterIuiv
#define glTextureParameteriv _glTextureParameteriv
#define glGenerateTextureMipmap _glGenerateTextureMipmap
#define glBindTextureUnit _glBindTextureUnit
#define glGetTextureImage _glGetTextureImage
#define glGetCompressedTextureImage _glGetCompressedTextureImage
#define glGetTextureLevelParameterfv _glGetTextureLevelParameterfv
#define glGetTextureLevelParameteriv _glGetTextureLevelParameteriv
#define glGetTextureParameterfv _glGetTextureParameterfv
#define glGetTextureParameterIiv _glGetTextureParameterIiv
#define glGetTextureParameterIuiv _glGetTextureParameterIuiv
#define glGetTextureParameteriv _glGetTextureParameteriv
#define glCreateVertexArrays _glCreateVertexArrays
#define glDisableVertexArrayAttrib _glDisableVertexArrayAttrib
#define glEnableVertexArrayAttrib _glEnableVertexArrayAttrib
#define glVertexArrayElementBuffer _glVertexArrayElementBuffer
#define glVertexArrayVertexBuffer _glVertexArrayVertexBuffer
#define glVertexArrayVertexBuffers _glVertexArrayVertexBuffers
#define glVertexArrayAttribBinding _glVertexArrayAttribBinding
#define glVertexArrayAttribFormat _glVertexArrayAttribFormat
#define glVertexArrayAttribIFormat _glVertexArrayAttribIFormat
#define glVertexArrayAttribLFormat _glVertexArrayAttribLFormat
#define glVertexArrayBindingDivisor _glVertexArrayBindingDivisor
#define glGetVertexArrayiv _glGetVertexArrayiv
#define glGetVertexArrayIndexediv _glGetVertexArrayIndexediv
#define glGetVertexArrayIndexed64iv _glGetVertexArrayIndexed64iv
#define glCreateSamplers _glCreateSamplers
#define glCreateProgramPipelines _glCreateProgramPipelines
#define glCreateQueries _glCreateQueries
#define glDrawBuffersARB _glDrawBuffersARB
#define glBlendEquationiARB _glBlendEquationiARB
#define glBlendEquationSeparateiARB _glBlendEquationSeparateiARB
#define glBlendFunciARB _glBlendFunciARB
#define glBlendFuncSeparateiARB _glBlendFuncSeparateiARB
#define glDrawElementsBaseVertex _glDrawElementsBaseVertex
#define glDrawRangeElementsBaseVertex _glDrawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex _glDrawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex _glMultiDrawElementsBaseVertex
#define glDrawArraysIndirect _glDrawArraysIndirect
#define glDrawElementsIndirect _glDrawElementsIndirect
#define glDrawArraysInstancedARB _glDrawArraysInstancedARB
#define glDrawElementsInstancedARB _glDrawElementsInstancedARB
#define glFramebufferParameteri _glFramebufferParameteri
#define glGetFramebufferParameteriv _glGetFramebufferParameteriv
#define glNamedFramebufferParameteriEXT _glNamedFramebufferParameteriEXT
#define glGetNamedFramebufferParameterivEXT _glGetNamedFramebufferParameterivEXT
#define glIsRenderbuffer _glIsRenderbuffer
#define glBindRenderbuffer _glBindRenderbuffer
#define glDeleteRenderbuffers _glDeleteRenderbuffers
#define glGenRenderbuffers _glGenRenderbuffers
#define glRenderbufferStorage _glRenderbufferStorage
#define glGetRenderbufferParameteriv _glGetRenderbufferParameteriv
#define glIsFramebuffer _glIsFramebuffer
#define glBindFramebuffer _glBindFramebuffer
#define glDeleteFramebuffers _glDeleteFramebuffers
#define glGenFramebuffers _glGenFramebuffers
#define glCheckFramebufferStatus _glCheckFramebufferStatus
#define glFramebufferTexture1D _glFramebufferTexture1D
#define glFramebufferTexture2D _glFramebufferTexture2D
#define glFramebufferTexture3D _glFramebufferTexture3D
#define glFramebufferRenderbuffer _glFramebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv _glGetFramebufferAttachmentParameteriv
#define glGenerateMipmap _glGenerateMipmap
#define glBlitFramebuffer _glBlitFramebuffer
#define glRenderbufferStorageMultisample _glRenderbufferStorageMultisample
#define glFramebufferTextureLayer _glFramebufferTextureLayer
#define glProgramParameteriARB _glProgramParameteriARB
#define glFramebufferTextureARB _glFramebufferTextureARB
#define glFramebufferTextureLayerARB _glFramebufferTextureLayerARB
#define glFramebufferTextureFaceARB _glFramebufferTextureFaceARB
#define glGetProgramBinary _glGetProgramBinary
#define glProgramBinary _glProgramBinary
#define glProgramParameteri _glProgramParameteri
#define glGetTextureSubImage _glGetTextureSubImage
#define glGetCompressedTextureSubImage _glGetCompressedTextureSubImage
#define glUniform1d _glUniform1d
#define glUniform2d _glUniform2d
#define glUniform3d _glUniform3d
#define glUniform4d _glUniform4d
#define glUniform1dv _glUniform1dv
#define glUniform2dv _glUniform2dv
#define glUniform3dv _glUniform3dv
#define glUniform4dv _glUniform4dv
#define glUniformMatrix2dv _glUniformMatrix2dv
#define glUniformMatrix3dv _glUniformMatrix3dv
#define glUniformMatrix4dv _glUniformMatrix4dv
#define glUniformMatrix2x3dv _glUniformMatrix2x3dv
#define glUniformMatrix2x4dv _glUniformMatrix2x4dv
#define glUniformMatrix3x2dv _glUniformMatrix3x2dv
#define glUniformMatrix3x4dv _glUniformMatrix3x4dv
#define glUniformMatrix4x2dv _glUniformMatrix4x2dv
#define glUniformMatrix4x3dv _glUniformMatrix4x3dv
#define glGetUniformdv _glGetUniformdv
#define glMultiDrawArraysIndirectCountARB _glMultiDrawArraysIndirectCountARB
#define glMultiDrawElementsIndirectCountARB _glMultiDrawElementsIndirectCountARB
#define glVertexAttribDivisorARB _glVertexAttribDivisorARB
#define glGetInternalformativ _glGetInternalformativ
#define glGetInternalformati64v _glGetInternalformati64v
#define glInvalidateTexSubImage _glInvalidateTexSubImage
#define glInvalidateTexImage _glInvalidateTexImage
#define glInvalidateBufferSubData _glInvalidateBufferSubData
#define glInvalidateBufferData _glInvalidateBufferData
#define glInvalidateFramebuffer _glInvalidateFramebuffer
#define glInvalidateSubFramebuffer _glInvalidateSubFramebuffer
#define glMapBufferRange _glMapBufferRange
#define glFlushMappedBufferRange _glFlushMappedBufferRange
#define glCurrentPaletteMatrixARB _glCurrentPaletteMatrixARB
#define glMatrixIndexubvARB _glMatrixIndexubvARB
#define glMatrixIndexusvARB _glMatrixIndexusvARB
#define glMatrixIndexuivARB _glMatrixIndexuivARB
#define glMatrixIndexPointerARB _glMatrixIndexPointerARB
#define glBindBuffersBase _glBindBuffersBase
#define glBindBuffersRange _glBindBuffersRange
#define glBindTextures _glBindTextures
#define glBindSamplers _glBindSamplers
#define glBindImageTextures _glBindImageTextures
#define glBindVertexBuffers _glBindVertexBuffers
#define glMultiDrawArraysIndirect _glMultiDrawArraysIndirect
#define glMultiDrawElementsIndirect _glMultiDrawElementsIndirect
#define glSampleCoverageARB _glSampleCoverageARB
#define glActiveTextureARB _glActiveTextureARB
#define glClientActiveTextureARB _glClientActiveTextureARB
#define glMultiTexCoord1dARB _glMultiTexCoord1dARB
#define glMultiTexCoord1dvARB _glMultiTexCoord1dvARB
#define glMultiTexCoord1fARB _glMultiTexCoord1fARB
#define glMultiTexCoord1fvARB _glMultiTexCoord1fvARB
#define glMultiTexCoord1iARB _glMultiTexCoord1iARB
#define glMultiTexCoord1ivARB _glMultiTexCoord1ivARB
#define glMultiTexCoord1sARB _glMultiTexCoord1sARB
#define glMultiTexCoord1svARB _glMultiTexCoord1svARB
#define glMultiTexCoord2dARB _glMultiTexCoord2dARB
#define glMultiTexCoord2dvARB _glMultiTexCoord2dvARB
#define glMultiTexCoord2fARB _glMultiTexCoord2fARB
#define glMultiTexCoord2fvARB _glMultiTexCoord2fvARB
#define glMultiTexCoord2iARB _glMultiTexCoord2iARB
#define glMultiTexCoord2ivARB _glMultiTexCoord2ivARB
#define glMultiTexCoord2sARB _glMultiTexCoord2sARB
#define glMultiTexCoord2svARB _glMultiTexCoord2svARB
#define glMultiTexCoord3dARB _glMultiTexCoord3dARB
#define glMultiTexCoord3dvARB _glMultiTexCoord3dvARB
#define glMultiTexCoord3fARB _glMultiTexCoord3fARB
#define glMultiTexCoord3fvARB _glMultiTexCoord3fvARB
#define glMultiTexCoord3iARB _glMultiTexCoord3iARB
#define glMultiTexCoord3ivARB _glMultiTexCoord3ivARB
#define glMultiTexCoord3sARB _glMultiTexCoord3sARB
#define glMultiTexCoord3svARB _glMultiTexCoord3svARB
#define glMultiTexCoord4dARB _glMultiTexCoord4dARB
#define glMultiTexCoord4dvARB _glMultiTexCoord4dvARB
#define glMultiTexCoord4fARB _glMultiTexCoord4fARB
#define glMultiTexCoord4fvARB _glMultiTexCoord4fvARB
#define glMultiTexCoord4iARB _glMultiTexCoord4iARB
#define glMultiTexCoord4ivARB _glMultiTexCoord4ivARB
#define glMultiTexCoord4sARB _glMultiTexCoord4sARB
#define glMultiTexCoord4svARB _glMultiTexCoord4svARB
#define glGenQueriesARB _glGenQueriesARB
#define glDeleteQueriesARB _glDeleteQueriesARB
#define glIsQueryARB _glIsQueryARB
#define glBeginQueryARB _glBeginQueryARB
#define glEndQueryARB _glEndQueryARB
#define glGetQueryivARB _glGetQueryivARB
#define glGetQueryObjectivARB _glGetQueryObjectivARB
#define glGetQueryObjectuivARB _glGetQueryObjectuivARB
#define glPointParameterfARB _glPointParameterfARB
#define glPointParameterfvARB _glPointParameterfvARB
#define glGetProgramInterfaceiv _glGetProgramInterfaceiv
#define glGetProgramResourceIndex _glGetProgramResourceIndex
#define glGetProgramResourceName _glGetProgramResourceName
#define glGetProgramResourceiv _glGetProgramResourceiv
#define glGetProgramResourceLocation _glGetProgramResourceLocation
#define glGetProgramResourceLocationIndex _glGetProgramResourceLocationIndex
#define glProvokingVertex _glProvokingVertex
#define glGetGraphicsResetStatusARB _glGetGraphicsResetStatusARB
#define glGetnMapdvARB _glGetnMapdvARB
#define glGetnMapfvARB _glGetnMapfvARB
#define glGetnMapivARB _glGetnMapivARB
#define glGetnPixelMapfvARB _glGetnPixelMapfvARB
#define glGetnPixelMapuivARB _glGetnPixelMapuivARB
#define glGetnPixelMapusvARB _glGetnPixelMapusvARB
#define glGetnPolygonStippleARB _glGetnPolygonStippleARB
#define glGetnColorTableARB _glGetnColorTableARB
#define glGetnConvolutionFilterARB _glGetnConvolutionFilterARB
#define glGetnSeparableFilterARB _glGetnSeparableFilterARB
#define glGetnHistogramARB _glGetnHistogramARB
#define glGetnMinmaxARB _glGetnMinmaxARB
#define glGetnTexImageARB _glGetnTexImageARB
#define glReadnPixelsARB _glReadnPixelsARB
#define glGetnCompressedTexImageARB _glGetnCompressedTexImageARB
#define glGetnUniformfvARB _glGetnUniformfvARB
#define glGetnUniformivARB _glGetnUniformivARB
#define glGetnUniformuivARB _glGetnUniformuivARB
#define glGetnUniformdvARB _glGetnUniformdvARB
#define glMinSampleShadingARB _glMinSampleShadingARB
#define glGenSamplers _glGenSamplers
#define glDeleteSamplers _glDeleteSamplers
#define glIsSampler _glIsSampler
#define glBindSampler _glBindSampler
#define glSamplerParameteri _glSamplerParameteri
#define glSamplerParameteriv _glSamplerParameteriv
#define glSamplerParameterf _glSamplerParameterf
#define glSamplerParameterfv _glSamplerParameterfv
#define glSamplerParameterIiv _glSamplerParameterIiv
#define glSamplerParameterIuiv _glSamplerParameterIuiv
#define glGetSamplerParameteriv _glGetSamplerParameteriv
#define glGetSamplerParameterIiv _glGetSamplerParameterIiv
#define glGetSamplerParameterfv _glGetSamplerParameterfv
#define glGetSamplerParameterIuiv _glGetSamplerParameterIuiv
#define glUseProgramStages _glUseProgramStages
#define glActiveShaderProgram _glActiveShaderProgram
#define glCreateShaderProgramv _glCreateShaderProgramv
#define glBindProgramPipeline _glBindProgramPipeline
#define glDeleteProgramPipelines _glDeleteProgramPipelines
#define glGenProgramPipelines _glGenProgramPipelines
#define glIsProgramPipeline _glIsProgramPipeline
#define glGetProgramPipelineiv _glGetProgramPipelineiv
#define glProgramUniform1i _glProgramUniform1i
#define glProgramUniform1iv _glProgramUniform1iv
#define glProgramUniform1f _glProgramUniform1f
#define glProgramUniform1fv _glProgramUniform1fv
#define glProgramUniform1d _glProgramUniform1d
#define glProgramUniform1dv _glProgramUniform1dv
#define glProgramUniform1ui _glProgramUniform1ui
#define glProgramUniform1uiv _glProgramUniform1uiv
#define glProgramUniform2i _glProgramUniform2i
#define glProgramUniform2iv _glProgramUniform2iv
#define glProgramUniform2f _glProgramUniform2f
#define glProgramUniform2fv _glProgramUniform2fv
#define glProgramUniform2d _glProgramUniform2d
#define glProgramUniform2dv _glProgramUniform2dv
#define glProgramUniform2ui _glProgramUniform2ui
#define glProgramUniform2uiv _glProgramUniform2uiv
#define glProgramUniform3i _glProgramUniform3i
#define glProgramUniform3iv _glProgramUniform3iv
#define glProgramUniform3f _glProgramUniform3f
#define glProgramUniform3fv _glProgramUniform3fv
#define glProgramUniform3d _glProgramUniform3d
#define glProgramUniform3dv _glProgramUniform3dv
#define glProgramUniform3ui _glProgramUniform3ui
#define glProgramUniform3uiv _glProgramUniform3uiv
#define glProgramUniform4i _glProgramUniform4i
#define glProgramUniform4iv _glProgramUniform4iv
#define glProgramUniform4f _glProgramUniform4f
#define glProgramUniform4fv _glProgramUniform4fv
#define glProgramUniform4d _glProgramUniform4d
#define glProgramUniform4dv _glProgramUniform4dv
#define glProgramUniform4ui _glProgramUniform4ui
#define glProgramUniform4uiv _glProgramUniform4uiv
#define glProgramUniformMatrix2fv _glProgramUniformMatrix2fv
#define glProgramUniformMatrix3fv _glProgramUniformMatrix3fv
#define glProgramUniformMatrix4fv _glProgramUniformMatrix4fv
#define glProgramUniformMatrix2dv _glProgramUniformMatrix2dv
#define glProgramUniformMatrix3dv _glProgramUniformMatrix3dv
#define glProgramUniformMatrix4dv _glProgramUniformMatrix4dv
#define glProgramUniformMatrix2x3fv _glProgramUniformMatrix2x3fv
#define glProgramUniformMatrix3x2fv _glProgramUniformMatrix3x2fv
#define glProgramUniformMatrix2x4fv _glProgramUniformMatrix2x4fv
#define glProgramUniformMatrix4x2fv _glProgramUniformMatrix4x2fv
#define glProgramUniformMatrix3x4fv _glProgramUniformMatrix3x4fv
#define glProgramUniformMatrix4x3fv _glProgramUniformMatrix4x3fv
#define glProgramUniformMatrix2x3dv _glProgramUniformMatrix2x3dv
#define glProgramUniformMatrix3x2dv _glProgramUniformMatrix3x2dv
#define glProgramUniformMatrix2x4dv _glProgramUniformMatrix2x4dv
#define glProgramUniformMatrix4x2dv _glProgramUniformMatrix4x2dv
#define glProgramUniformMatrix3x4dv _glProgramUniformMatrix3x4dv
#define glProgramUniformMatrix4x3dv _glProgramUniformMatrix4x3dv
#define glValidateProgramPipeline _glValidateProgramPipeline
#define glGetProgramPipelineInfoLog _glGetProgramPipelineInfoLog
#define glGetActiveAtomicCounterBufferiv _glGetActiveAtomicCounterBufferiv
#define glBindImageTexture _glBindImageTexture
#define glMemoryBarrier _glMemoryBarrier
#define glDeleteObjectARB _glDeleteObjectARB
#define glGetHandleARB _glGetHandleARB
#define glDetachObjectARB _glDetachObjectARB
#define glCreateShaderObjectARB _glCreateShaderObjectARB
#define glShaderSourceARB _glShaderSourceARB
#define glCompileShaderARB _glCompileShaderARB
#define glCreateProgramObjectARB _glCreateProgramObjectARB
#define glAttachObjectARB _glAttachObjectARB
#define glLinkProgramARB _glLinkProgramARB
#define glUseProgramObjectARB _glUseProgramObjectARB
#define glValidateProgramARB _glValidateProgramARB
#define glUniform1fARB _glUniform1fARB
#define glUniform2fARB _glUniform2fARB
#define glUniform3fARB _glUniform3fARB
#define glUniform4fARB _glUniform4fARB
#define glUniform1iARB _glUniform1iARB
#define glUniform2iARB _glUniform2iARB
#define glUniform3iARB _glUniform3iARB
#define glUniform4iARB _glUniform4iARB
#define glUniform1fvARB _glUniform1fvARB
#define glUniform2fvARB _glUniform2fvARB
#define glUniform3fvARB _glUniform3fvARB
#define glUniform4fvARB _glUniform4fvARB
#define glUniform1ivARB _glUniform1ivARB
#define glUniform2ivARB _glUniform2ivARB
#define glUniform3ivARB _glUniform3ivARB
#define glUniform4ivARB _glUniform4ivARB
#define glUniformMatrix2fvARB _glUniformMatrix2fvARB
#define glUniformMatrix3fvARB _glUniformMatrix3fvARB
#define glUniformMatrix4fvARB _glUniformMatrix4fvARB
#define glGetObjectParameterfvARB _glGetObjectParameterfvARB
#define glGetObjectParameterivARB _glGetObjectParameterivARB
#define glGetInfoLogARB _glGetInfoLogARB
#define glGetAttachedObjectsARB _glGetAttachedObjectsARB
#define glGetUniformLocationARB _glGetUniformLocationARB
#define glGetActiveUniformARB _glGetActiveUniformARB
#define glGetUniformfvARB _glGetUniformfvARB
#define glGetUniformivARB _glGetUniformivARB
#define glGetShaderSourceARB _glGetShaderSourceARB
#define glShaderStorageBlockBinding _glShaderStorageBlockBinding
#define glGetSubroutineUniformLocation _glGetSubroutineUniformLocation
#define glGetSubroutineIndex _glGetSubroutineIndex
#define glGetActiveSubroutineUniformiv _glGetActiveSubroutineUniformiv
#define glGetActiveSubroutineUniformName _glGetActiveSubroutineUniformName
#define glGetActiveSubroutineName _glGetActiveSubroutineName
#define glUniformSubroutinesuiv _glUniformSubroutinesuiv
#define glGetUniformSubroutineuiv _glGetUniformSubroutineuiv
#define glGetProgramStageiv _glGetProgramStageiv
#define glNamedStringARB _glNamedStringARB
#define glDeleteNamedStringARB _glDeleteNamedStringARB
#define glCompileShaderIncludeARB _glCompileShaderIncludeARB
#define glIsNamedStringARB _glIsNamedStringARB
#define glGetNamedStringARB _glGetNamedStringARB
#define glGetNamedStringivARB _glGetNamedStringivARB
#define glBufferPageCommitmentARB _glBufferPageCommitmentARB
#define glNamedBufferPageCommitmentEXT _glNamedBufferPageCommitmentEXT
#define glNamedBufferPageCommitmentARB _glNamedBufferPageCommitmentARB
#define glTexPageCommitmentARB _glTexPageCommitmentARB
#define glTexturePageCommitmentEXT _glTexturePageCommitmentEXT
#define glFenceSync _glFenceSync
#define glIsSync _glIsSync
#define glDeleteSync _glDeleteSync
#define glClientWaitSync _glClientWaitSync
#define glWaitSync _glWaitSync
#define glGetInteger64v _glGetInteger64v
#define glGetSynciv _glGetSynciv
#define glPatchParameteri _glPatchParameteri
#define glPatchParameterfv _glPatchParameterfv
#define glTextureBarrier _glTextureBarrier
#define glTexBufferARB _glTexBufferARB
#define glTexBufferRange _glTexBufferRange
#define glTextureBufferRangeEXT _glTextureBufferRangeEXT
#define glCompressedTexImage3DARB _glCompressedTexImage3DARB
#define glCompressedTexImage2DARB _glCompressedTexImage2DARB
#define glCompressedTexImage1DARB _glCompressedTexImage1DARB
#define glCompressedTexSubImage3DARB _glCompressedTexSubImage3DARB
#define glCompressedTexSubImage2DARB _glCompressedTexSubImage2DARB
#define glCompressedTexSubImage1DARB _glCompressedTexSubImage1DARB
#define glGetCompressedTexImageARB _glGetCompressedTexImageARB
#define glTexImage2DMultisample _glTexImage2DMultisample
#define glTexImage3DMultisample _glTexImage3DMultisample
#define glGetMultisamplefv _glGetMultisamplefv
#define glSampleMaski _glSampleMaski
#define glTexStorage1D _glTexStorage1D
#define glTexStorage2D _glTexStorage2D
#define glTexStorage3D _glTexStorage3D
#define glTextureStorage1DEXT _glTextureStorage1DEXT
#define glTextureStorage2DEXT _glTextureStorage2DEXT
#define glTextureStorage3DEXT _glTextureStorage3DEXT
#define glTexStorage2DMultisample _glTexStorage2DMultisample
#define glTexStorage3DMultisample _glTexStorage3DMultisample
#define glTextureStorage2DMultisampleEXT _glTextureStorage2DMultisampleEXT
#define glTextureStorage3DMultisampleEXT _glTextureStorage3DMultisampleEXT
#define glTextureView _glTextureView
#define glQueryCounter _glQueryCounter
#define glGetQueryObjecti64v _glGetQueryObjecti64v
#define glGetQueryObjectui64v _glGetQueryObjectui64v
#define glBindTransformFeedback _glBindTransformFeedback
#define glDeleteTransformFeedbacks _glDeleteTransformFeedbacks
#define glGenTransformFeedbacks _glGenTransformFeedbacks
#define glIsTransformFeedback _glIsTransformFeedback
#define glPauseTransformFeedback _glPauseTransformFeedback
#define glResumeTransformFeedback _glResumeTransformFeedback
#define glDrawTransformFeedback _glDrawTransformFeedback
#define glDrawTransformFeedbackStream _glDrawTransformFeedbackStream
#define glBeginQueryIndexed _glBeginQueryIndexed
#define glEndQueryIndexed _glEndQueryIndexed
#define glGetQueryIndexediv _glGetQueryIndexediv
#define glDrawTransformFeedbackInstanced _glDrawTransformFeedbackInstanced
#define glDrawTransformFeedbackStreamInstanced _glDrawTransformFeedbackStreamInstanced
#define glLoadTransposeMatrixfARB _glLoadTransposeMatrixfARB
#define glLoadTransposeMatrixdARB _glLoadTransposeMatrixdARB
#define glMultTransposeMatrixfARB _glMultTransposeMatrixfARB
#define glMultTransposeMatrixdARB _glMultTransposeMatrixdARB
#define glGetUniformIndices _glGetUniformIndices
#define glGetActiveUniformsiv _glGetActiveUniformsiv
#define glGetActiveUniformName _glGetActiveUniformName
#define glGetUniformBlockIndex _glGetUniformBlockIndex
#define glGetActiveUniformBlockiv _glGetActiveUniformBlockiv
#define glGetActiveUniformBlockName _glGetActiveUniformBlockName
#define glUniformBlockBinding _glUniformBlockBinding
#define glBindVertexArray _glBindVertexArray
#define glDeleteVertexArrays _glDeleteVertexArrays
#define glGenVertexArrays _glGenVertexArrays
#define glIsVertexArray _glIsVertexArray
#define glVertexAttribL1d _glVertexAttribL1d
#define glVertexAttribL2d _glVertexAttribL2d
#define glVertexAttribL3d _glVertexAttribL3d
#define glVertexAttribL4d _glVertexAttribL4d
#define glVertexAttribL1dv _glVertexAttribL1dv
#define glVertexAttribL2dv _glVertexAttribL2dv
#define glVertexAttribL3dv _glVertexAttribL3dv
#define glVertexAttribL4dv _glVertexAttribL4dv
#define glVertexAttribLPointer _glVertexAttribLPointer
#define glGetVertexAttribLdv _glGetVertexAttribLdv
#define glBindVertexBuffer _glBindVertexBuffer
#define glVertexAttribFormat _glVertexAttribFormat
#define glVertexAttribIFormat _glVertexAttribIFormat
#define glVertexAttribLFormat _glVertexAttribLFormat
#define glVertexAttribBinding _glVertexAttribBinding
#define glVertexBindingDivisor _glVertexBindingDivisor
#define glVertexArrayBindVertexBufferEXT _glVertexArrayBindVertexBufferEXT
#define glVertexArrayVertexAttribFormatEXT _glVertexArrayVertexAttribFormatEXT
#define glVertexArrayVertexAttribIFormatEXT _glVertexArrayVertexAttribIFormatEXT
#define glVertexArrayVertexAttribLFormatEXT _glVertexArrayVertexAttribLFormatEXT
#define glVertexArrayVertexAttribBindingEXT _glVertexArrayVertexAttribBindingEXT
#define glVertexArrayVertexBindingDivisorEXT _glVertexArrayVertexBindingDivisorEXT
#define glWeightbvARB _glWeightbvARB
#define glWeightsvARB _glWeightsvARB
#define glWeightivARB _glWeightivARB
#define glWeightfvARB _glWeightfvARB
#define glWeightdvARB _glWeightdvARB
#define glWeightubvARB _glWeightubvARB
#define glWeightusvARB _glWeightusvARB
#define glWeightuivARB _glWeightuivARB
#define glWeightPointerARB _glWeightPointerARB
#define glVertexBlendARB _glVertexBlendARB
#define glBindBufferARB _glBindBufferARB
#define glDeleteBuffersARB _glDeleteBuffersARB
#define glGenBuffersARB _glGenBuffersARB
#define glIsBufferARB _glIsBufferARB
#define glBufferDataARB _glBufferDataARB
#define glBufferSubDataARB _glBufferSubDataARB
#define glGetBufferSubDataARB _glGetBufferSubDataARB
#define glMapBufferARB _glMapBufferARB
#define glUnmapBufferARB _glUnmapBufferARB
#define glGetBufferParameterivARB _glGetBufferParameterivARB
#define glGetBufferPointervARB _glGetBufferPointervARB
#define glVertexAttrib1dARB _glVertexAttrib1dARB
#define glVertexAttrib1dvARB _glVertexAttrib1dvARB
#define glVertexAttrib1fARB _glVertexAttrib1fARB
#define glVertexAttrib1fvARB _glVertexAttrib1fvARB
#define glVertexAttrib1sARB _glVertexAttrib1sARB
#define glVertexAttrib1svARB _glVertexAttrib1svARB
#define glVertexAttrib2dARB _glVertexAttrib2dARB
#define glVertexAttrib2dvARB _glVertexAttrib2dvARB
#define glVertexAttrib2fARB _glVertexAttrib2fARB
#define glVertexAttrib2fvARB _glVertexAttrib2fvARB
#define glVertexAttrib2sARB _glVertexAttrib2sARB
#define glVertexAttrib2svARB _glVertexAttrib2svARB
#define glVertexAttrib3dARB _glVertexAttrib3dARB
#define glVertexAttrib3dvARB _glVertexAttrib3dvARB
#define glVertexAttrib3fARB _glVertexAttrib3fARB
#define glVertexAttrib3fvARB _glVertexAttrib3fvARB
#define glVertexAttrib3sARB _glVertexAttrib3sARB
#define glVertexAttrib3svARB _glVertexAttrib3svARB
#define glVertexAttrib4NbvARB _glVertexAttrib4NbvARB
#define glVertexAttrib4NivARB _glVertexAttrib4NivARB
#define glVertexAttrib4NsvARB _glVertexAttrib4NsvARB
#define glVertexAttrib4NubARB _glVertexAttrib4NubARB
#define glVertexAttrib4NubvARB _glVertexAttrib4NubvARB
#define glVertexAttrib4NuivARB _glVertexAttrib4NuivARB
#define glVertexAttrib4NusvARB _glVertexAttrib4NusvARB
#define glVertexAttrib4bvARB _glVertexAttrib4bvARB
#define glVertexAttrib4dARB _glVertexAttrib4dARB
#define glVertexAttrib4dvARB _glVertexAttrib4dvARB
#define glVertexAttrib4fARB _glVertexAttrib4fARB
#define glVertexAttrib4fvARB _glVertexAttrib4fvARB
#define glVertexAttrib4ivARB _glVertexAttrib4ivARB
#define glVertexAttrib4sARB _glVertexAttrib4sARB
#define glVertexAttrib4svARB _glVertexAttrib4svARB
#define glVertexAttrib4ubvARB _glVertexAttrib4ubvARB
#define glVertexAttrib4uivARB _glVertexAttrib4uivARB
#define glVertexAttrib4usvARB _glVertexAttrib4usvARB
#define glVertexAttribPointerARB _glVertexAttribPointerARB
#define glEnableVertexAttribArrayARB _glEnableVertexAttribArrayARB
#define glDisableVertexAttribArrayARB _glDisableVertexAttribArrayARB
#define glProgramStringARB _glProgramStringARB
#define glBindProgramARB _glBindProgramARB
#define glDeleteProgramsARB _glDeleteProgramsARB
#define glGenProgramsARB _glGenProgramsARB
#define glProgramEnvParameter4dARB _glProgramEnvParameter4dARB
#define glProgramEnvParameter4dvARB _glProgramEnvParameter4dvARB
#define glProgramEnvParameter4fARB _glProgramEnvParameter4fARB
#define glProgramEnvParameter4fvARB _glProgramEnvParameter4fvARB
#define glProgramLocalParameter4dARB _glProgramLocalParameter4dARB
#define glProgramLocalParameter4dvARB _glProgramLocalParameter4dvARB
#define glProgramLocalParameter4fARB _glProgramLocalParameter4fARB
#define glProgramLocalParameter4fvARB _glProgramLocalParameter4fvARB
#define glGetProgramEnvParameterdvARB _glGetProgramEnvParameterdvARB
#define glGetProgramEnvParameterfvARB _glGetProgramEnvParameterfvARB
#define glGetProgramLocalParameterdvARB _glGetProgramLocalParameterdvARB
#define glGetProgramLocalParameterfvARB _glGetProgramLocalParameterfvARB
#define glGetProgramivARB _glGetProgramivARB
#define glGetProgramStringARB _glGetProgramStringARB
#define glGetVertexAttribdvARB _glGetVertexAttribdvARB
#define glGetVertexAttribfvARB _glGetVertexAttribfvARB
#define glGetVertexAttribivARB _glGetVertexAttribivARB
#define glGetVertexAttribPointervARB _glGetVertexAttribPointervARB
#define glIsProgramARB _glIsProgramARB
#define glBindAttribLocationARB _glBindAttribLocationARB
#define glGetActiveAttribARB _glGetActiveAttribARB
#define glGetAttribLocationARB _glGetAttribLocationARB
#define glVertexAttribP1ui _glVertexAttribP1ui
#define glVertexAttribP1uiv _glVertexAttribP1uiv
#define glVertexAttribP2ui _glVertexAttribP2ui
#define glVertexAttribP2uiv _glVertexAttribP2uiv
#define glVertexAttribP3ui _glVertexAttribP3ui
#define glVertexAttribP3uiv _glVertexAttribP3uiv
#define glVertexAttribP4ui _glVertexAttribP4ui
#define glVertexAttribP4uiv _glVertexAttribP4uiv
#define glVertexP2ui _glVertexP2ui
#define glVertexP2uiv _glVertexP2uiv
#define glVertexP3ui _glVertexP3ui
#define glVertexP3uiv _glVertexP3uiv
#define glVertexP4ui _glVertexP4ui
#define glVertexP4uiv _glVertexP4uiv
#define glTexCoordP1ui _glTexCoordP1ui
#define glTexCoordP1uiv _glTexCoordP1uiv
#define glTexCoordP2ui _glTexCoordP2ui
#define glTexCoordP2uiv _glTexCoordP2uiv
#define glTexCoordP3ui _glTexCoordP3ui
#define glTexCoordP3uiv _glTexCoordP3uiv
#define glTexCoordP4ui _glTexCoordP4ui
#define glTexCoordP4uiv _glTexCoordP4uiv
#define glMultiTexCoordP1ui _glMultiTexCoordP1ui
#define glMultiTexCoordP1uiv _glMultiTexCoordP1uiv
#define glMultiTexCoordP2ui _glMultiTexCoordP2ui
#define glMultiTexCoordP2uiv _glMultiTexCoordP2uiv
#define glMultiTexCoordP3ui _glMultiTexCoordP3ui
#define glMultiTexCoordP3uiv _glMultiTexCoordP3uiv
#define glMultiTexCoordP4ui _glMultiTexCoordP4ui
#define glMultiTexCoordP4uiv _glMultiTexCoordP4uiv
#define glNormalP3ui _glNormalP3ui
#define glNormalP3uiv _glNormalP3uiv
#define glColorP3ui _glColorP3ui
#define glColorP3uiv _glColorP3uiv
#define glColorP4ui _glColorP4ui
#define glColorP4uiv _glColorP4uiv
#define glSecondaryColorP3ui _glSecondaryColorP3ui
#define glSecondaryColorP3uiv _glSecondaryColorP3uiv
#define glViewportArrayv _glViewportArrayv
#define glViewportIndexedf _glViewportIndexedf
#define glViewportIndexedfv _glViewportIndexedfv
#define glScissorArrayv _glScissorArrayv
#define glScissorIndexed _glScissorIndexed
#define glScissorIndexedv _glScissorIndexedv
#define glDepthRangeArrayv _glDepthRangeArrayv
#define glDepthRangeIndexed _glDepthRangeIndexed
#define glGetFloati_v _glGetFloati_v
#define glGetDoublei_v _glGetDoublei_v
#define glWindowPos2dARB _glWindowPos2dARB
#define glWindowPos2dvARB _glWindowPos2dvARB
#define glWindowPos2fARB _glWindowPos2fARB
#define glWindowPos2fvARB _glWindowPos2fvARB
#define glWindowPos2iARB _glWindowPos2iARB
#define glWindowPos2ivARB _glWindowPos2ivARB
#define glWindowPos2sARB _glWindowPos2sARB
#define glWindowPos2svARB _glWindowPos2svARB
#define glWindowPos3dARB _glWindowPos3dARB
#define glWindowPos3dvARB _glWindowPos3dvARB
#define glWindowPos3fARB _glWindowPos3fARB
#define glWindowPos3fvARB _glWindowPos3fvARB
#define glWindowPos3iARB _glWindowPos3iARB
#define glWindowPos3ivARB _glWindowPos3ivARB
#define glWindowPos3sARB _glWindowPos3sARB
#define glWindowPos3svARB _glWindowPos3svARB
#define glDrawBuffersATI _glDrawBuffersATI
#define glElementPointerATI _glElementPointerATI
#define glDrawElementArrayATI _glDrawElementArrayATI
#define glDrawRangeElementArrayATI _glDrawRangeElementArrayATI
#define glTexBumpParameterivATI _glTexBumpParameterivATI
#define glTexBumpParameterfvATI _glTexBumpParameterfvATI
#define glGetTexBumpParameterivATI _glGetTexBumpParameterivATI
#define glGetTexBumpParameterfvATI _glGetTexBumpParameterfvATI
#define glGenFragmentShadersATI _glGenFragmentShadersATI
#define glBindFragmentShaderATI _glBindFragmentShaderATI
#define glDeleteFragmentShaderATI _glDeleteFragmentShaderATI
#define glBeginFragmentShaderATI _glBeginFragmentShaderATI
#define glEndFragmentShaderATI _glEndFragmentShaderATI
#define glPassTexCoordATI _glPassTexCoordATI
#define glSampleMapATI _glSampleMapATI
#define glColorFragmentOp1ATI _glColorFragmentOp1ATI
#define glColorFragmentOp2ATI _glColorFragmentOp2ATI
#define glColorFragmentOp3ATI _glColorFragmentOp3ATI
#define glAlphaFragmentOp1ATI _glAlphaFragmentOp1ATI
#define glAlphaFragmentOp2ATI _glAlphaFragmentOp2ATI
#define glAlphaFragmentOp3ATI _glAlphaFragmentOp3ATI
#define glSetFragmentShaderConstantATI _glSetFragmentShaderConstantATI
#define glMapObjectBufferATI _glMapObjectBufferATI
#define glUnmapObjectBufferATI _glUnmapObjectBufferATI
#define glPNTrianglesiATI _glPNTrianglesiATI
#define glPNTrianglesfATI _glPNTrianglesfATI
#define glStencilOpSeparateATI _glStencilOpSeparateATI
#define glStencilFuncSeparateATI _glStencilFuncSeparateATI
#define glNewObjectBufferATI _glNewObjectBufferATI
#define glIsObjectBufferATI _glIsObjectBufferATI
#define glUpdateObjectBufferATI _glUpdateObjectBufferATI
#define glGetObjectBufferfvATI _glGetObjectBufferfvATI
#define glGetObjectBufferivATI _glGetObjectBufferivATI
#define glFreeObjectBufferATI _glFreeObjectBufferATI
#define glArrayObjectATI _glArrayObjectATI
#define glGetArrayObjectfvATI _glGetArrayObjectfvATI
#define glGetArrayObjectivATI _glGetArrayObjectivATI
#define glVariantArrayObjectATI _glVariantArrayObjectATI
#define glGetVariantArrayObjectfvATI _glGetVariantArrayObjectfvATI
#define glGetVariantArrayObjectivATI _glGetVariantArrayObjectivATI
#define glVertexAttribArrayObjectATI _glVertexAttribArrayObjectATI
#define glGetVertexAttribArrayObjectfvATI _glGetVertexAttribArrayObjectfvATI
#define glGetVertexAttribArrayObjectivATI _glGetVertexAttribArrayObjectivATI
#define glVertexStream1sATI _glVertexStream1sATI
#define glVertexStream1svATI _glVertexStream1svATI
#define glVertexStream1iATI _glVertexStream1iATI
#define glVertexStream1ivATI _glVertexStream1ivATI
#define glVertexStream1fATI _glVertexStream1fATI
#define glVertexStream1fvATI _glVertexStream1fvATI
#define glVertexStream1dATI _glVertexStream1dATI
#define glVertexStream1dvATI _glVertexStream1dvATI
#define glVertexStream2sATI _glVertexStream2sATI
#define glVertexStream2svATI _glVertexStream2svATI
#define glVertexStream2iATI _glVertexStream2iATI
#define glVertexStream2ivATI _glVertexStream2ivATI
#define glVertexStream2fATI _glVertexStream2fATI
#define glVertexStream2fvATI _glVertexStream2fvATI
#define glVertexStream2dATI _glVertexStream2dATI
#define glVertexStream2dvATI _glVertexStream2dvATI
#define glVertexStream3sATI _glVertexStream3sATI
#define glVertexStream3svATI _glVertexStream3svATI
#define glVertexStream3iATI _glVertexStream3iATI
#define glVertexStream3ivATI _glVertexStream3ivATI
#define glVertexStream3fATI _glVertexStream3fATI
#define glVertexStream3fvATI _glVertexStream3fvATI
#define glVertexStream3dATI _glVertexStream3dATI
#define glVertexStream3dvATI _glVertexStream3dvATI
#define glVertexStream4sATI _glVertexStream4sATI
#define glVertexStream4svATI _glVertexStream4svATI
#define glVertexStream4iATI _glVertexStream4iATI
#define glVertexStream4ivATI _glVertexStream4ivATI
#define glVertexStream4fATI _glVertexStream4fATI
#define glVertexStream4fvATI _glVertexStream4fvATI
#define glVertexStream4dATI _glVertexStream4dATI
#define glVertexStream4dvATI _glVertexStream4dvATI
#define glNormalStream3bATI _glNormalStream3bATI
#define glNormalStream3bvATI _glNormalStream3bvATI
#define glNormalStream3sATI _glNormalStream3sATI
#define glNormalStream3svATI _glNormalStream3svATI
#define glNormalStream3iATI _glNormalStream3iATI
#define glNormalStream3ivATI _glNormalStream3ivATI
#define glNormalStream3fATI _glNormalStream3fATI
#define glNormalStream3fvATI _glNormalStream3fvATI
#define glNormalStream3dATI _glNormalStream3dATI
#define glNormalStream3dvATI _glNormalStream3dvATI
#define glClientActiveVertexStreamATI _glClientActiveVertexStreamATI
#define glVertexBlendEnviATI _glVertexBlendEnviATI
#define glVertexBlendEnvfATI _glVertexBlendEnvfATI
#define glUniformBufferEXT _glUniformBufferEXT
#define glGetUniformBufferSizeEXT _glGetUniformBufferSizeEXT
#define glGetUniformOffsetEXT _glGetUniformOffsetEXT
#define glBlendColorEXT _glBlendColorEXT
#define glBlendEquationSeparateEXT _glBlendEquationSeparateEXT
#define glBlendFuncSeparateEXT _glBlendFuncSeparateEXT
#define glBlendEquationEXT _glBlendEquationEXT
#define glColorSubTableEXT _glColorSubTableEXT
#define glCopyColorSubTableEXT _glCopyColorSubTableEXT
#define glLockArraysEXT _glLockArraysEXT
#define glUnlockArraysEXT _glUnlockArraysEXT
#define glConvolutionFilter1DEXT _glConvolutionFilter1DEXT
#define glConvolutionFilter2DEXT _glConvolutionFilter2DEXT
#define glConvolutionParameterfEXT _glConvolutionParameterfEXT
#define glConvolutionParameterfvEXT _glConvolutionParameterfvEXT
#define glConvolutionParameteriEXT _glConvolutionParameteriEXT
#define glConvolutionParameterivEXT _glConvolutionParameterivEXT
#define glCopyConvolutionFilter1DEXT _glCopyConvolutionFilter1DEXT
#define glCopyConvolutionFilter2DEXT _glCopyConvolutionFilter2DEXT
#define glGetConvolutionFilterEXT _glGetConvolutionFilterEXT
#define glGetConvolutionParameterfvEXT _glGetConvolutionParameterfvEXT
#define glGetConvolutionParameterivEXT _glGetConvolutionParameterivEXT
#define glGetSeparableFilterEXT _glGetSeparableFilterEXT
#define glSeparableFilter2DEXT _glSeparableFilter2DEXT
#define glTangent3bEXT _glTangent3bEXT
#define glTangent3bvEXT _glTangent3bvEXT
#define glTangent3dEXT _glTangent3dEXT
#define glTangent3dvEXT _glTangent3dvEXT
#define glTangent3fEXT _glTangent3fEXT
#define glTangent3fvEXT _glTangent3fvEXT
#define glTangent3iEXT _glTangent3iEXT
#define glTangent3ivEXT _glTangent3ivEXT
#define glTangent3sEXT _glTangent3sEXT
#define glTangent3svEXT _glTangent3svEXT
#define glBinormal3bEXT _glBinormal3bEXT
#define glBinormal3bvEXT _glBinormal3bvEXT
#define glBinormal3dEXT _glBinormal3dEXT
#define glBinormal3dvEXT _glBinormal3dvEXT
#define glBinormal3fEXT _glBinormal3fEXT
#define glBinormal3fvEXT _glBinormal3fvEXT
#define glBinormal3iEXT _glBinormal3iEXT
#define glBinormal3ivEXT _glBinormal3ivEXT
#define glBinormal3sEXT _glBinormal3sEXT
#define glBinormal3svEXT _glBinormal3svEXT
#define glTangentPointerEXT _glTangentPointerEXT
#define glBinormalPointerEXT _glBinormalPointerEXT
#define glCopyImageSubDataEXT _glCopyImageSubDataEXT
#define glCopyTexImage1DEXT _glCopyTexImage1DEXT
#define glCopyTexImage2DEXT _glCopyTexImage2DEXT
#define glCopyTexSubImage1DEXT _glCopyTexSubImage1DEXT
#define glCopyTexSubImage2DEXT _glCopyTexSubImage2DEXT
#define glCopyTexSubImage3DEXT _glCopyTexSubImage3DEXT
#define glCullParameterdvEXT _glCullParameterdvEXT
#define glCullParameterfvEXT _glCullParameterfvEXT
#define glLabelObjectEXT _glLabelObjectEXT
#define glGetObjectLabelEXT _glGetObjectLabelEXT
#define glInsertEventMarkerEXT _glInsertEventMarkerEXT
#define glPushGroupMarkerEXT _glPushGroupMarkerEXT
#define glPopGroupMarkerEXT _glPopGroupMarkerEXT
#define glDepthBoundsEXT _glDepthBoundsEXT
#define glClientAttribDefaultEXT _glClientAttribDefaultEXT
#define glPushClientAttribDefaultEXT _glPushClientAttribDefaultEXT
#define glMatrixLoadfEXT _glMatrixLoadfEXT
#define glMatrixLoaddEXT _glMatrixLoaddEXT
#define glMatrixMultfEXT _glMatrixMultfEXT
#define glMatrixMultdEXT _glMatrixMultdEXT
#define glMatrixLoadIdentityEXT _glMatrixLoadIdentityEXT
#define glMatrixRotatefEXT _glMatrixRotatefEXT
#define glMatrixRotatedEXT _glMatrixRotatedEXT
#define glMatrixScalefEXT _glMatrixScalefEXT
#define glMatrixScaledEXT _glMatrixScaledEXT
#define glMatrixTranslatefEXT _glMatrixTranslatefEXT
#define glMatrixTranslatedEXT _glMatrixTranslatedEXT
#define glMatrixFrustumEXT _glMatrixFrustumEXT
#define glMatrixOrthoEXT _glMatrixOrthoEXT
#define glMatrixPopEXT _glMatrixPopEXT
#define glMatrixPushEXT _glMatrixPushEXT
#define glMatrixLoadTransposefEXT _glMatrixLoadTransposefEXT
#define glMatrixLoadTransposedEXT _glMatrixLoadTransposedEXT
#define glMatrixMultTransposefEXT _glMatrixMultTransposefEXT
#define glMatrixMultTransposedEXT _glMatrixMultTransposedEXT
#define glTextureParameterfEXT _glTextureParameterfEXT
#define glTextureParameterfvEXT _glTextureParameterfvEXT
#define glTextureParameteriEXT _glTextureParameteriEXT
#define glTextureParameterivEXT _glTextureParameterivEXT
#define glTextureImage1DEXT _glTextureImage1DEXT
#define glTextureImage2DEXT _glTextureImage2DEXT
#define glTextureSubImage1DEXT _glTextureSubImage1DEXT
#define glTextureSubImage2DEXT _glTextureSubImage2DEXT
#define glCopyTextureImage1DEXT _glCopyTextureImage1DEXT
#define glCopyTextureImage2DEXT _glCopyTextureImage2DEXT
#define glCopyTextureSubImage1DEXT _glCopyTextureSubImage1DEXT
#define glCopyTextureSubImage2DEXT _glCopyTextureSubImage2DEXT
#define glGetTextureImageEXT _glGetTextureImageEXT
#define glGetTextureParameterfvEXT _glGetTextureParameterfvEXT
#define glGetTextureParameterivEXT _glGetTextureParameterivEXT
#define glGetTextureLevelParameterfvEXT _glGetTextureLevelParameterfvEXT
#define glGetTextureLevelParameterivEXT _glGetTextureLevelParameterivEXT
#define glTextureImage3DEXT _glTextureImage3DEXT
#define glTextureSubImage3DEXT _glTextureSubImage3DEXT
#define glCopyTextureSubImage3DEXT _glCopyTextureSubImage3DEXT
#define glMultiTexParameterfEXT _glMultiTexParameterfEXT
#define glMultiTexParameterfvEXT _glMultiTexParameterfvEXT
#define glMultiTexParameteriEXT _glMultiTexParameteriEXT
#define glMultiTexParameterivEXT _glMultiTexParameterivEXT
#define glMultiTexImage1DEXT _glMultiTexImage1DEXT
#define glMultiTexImage2DEXT _glMultiTexImage2DEXT
#define glMultiTexSubImage1DEXT _glMultiTexSubImage1DEXT
#define glMultiTexSubImage2DEXT _glMultiTexSubImage2DEXT
#define glCopyMultiTexImage1DEXT _glCopyMultiTexImage1DEXT
#define glCopyMultiTexImage2DEXT _glCopyMultiTexImage2DEXT
#define glCopyMultiTexSubImage1DEXT _glCopyMultiTexSubImage1DEXT
#define glCopyMultiTexSubImage2DEXT _glCopyMultiTexSubImage2DEXT
#define glGetMultiTexImageEXT _glGetMultiTexImageEXT
#define glGetMultiTexParameterfvEXT _glGetMultiTexParameterfvEXT
#define glGetMultiTexParameterivEXT _glGetMultiTexParameterivEXT
#define glGetMultiTexLevelParameterfvEXT _glGetMultiTexLevelParameterfvEXT
#define glGetMultiTexLevelParameterivEXT _glGetMultiTexLevelParameterivEXT
#define glMultiTexImage3DEXT _glMultiTexImage3DEXT
#define glMultiTexSubImage3DEXT _glMultiTexSubImage3DEXT
#define glCopyMultiTexSubImage3DEXT _glCopyMultiTexSubImage3DEXT
#define glBindMultiTextureEXT _glBindMultiTextureEXT
#define glEnableClientStateIndexedEXT _glEnableClientStateIndexedEXT
#define glDisableClientStateIndexedEXT _glDisableClientStateIndexedEXT
#define glEnableClientStateiEXT _glEnableClientStateiEXT
#define glDisableClientStateiEXT _glDisableClientStateiEXT
#define glMultiTexCoordPointerEXT _glMultiTexCoordPointerEXT
#define glMultiTexEnvfEXT _glMultiTexEnvfEXT
#define glMultiTexEnvfvEXT _glMultiTexEnvfvEXT
#define glMultiTexEnviEXT _glMultiTexEnviEXT
#define glMultiTexEnvivEXT _glMultiTexEnvivEXT
#define glMultiTexGendEXT _glMultiTexGendEXT
#define glMultiTexGendvEXT _glMultiTexGendvEXT
#define glMultiTexGenfEXT _glMultiTexGenfEXT
#define glMultiTexGenfvEXT _glMultiTexGenfvEXT
#define glMultiTexGeniEXT _glMultiTexGeniEXT
#define glMultiTexGenivEXT _glMultiTexGenivEXT
#define glGetMultiTexEnvfvEXT _glGetMultiTexEnvfvEXT
#define glGetMultiTexEnvivEXT _glGetMultiTexEnvivEXT
#define glGetMultiTexGendvEXT _glGetMultiTexGendvEXT
#define glGetMultiTexGenfvEXT _glGetMultiTexGenfvEXT
#define glGetMultiTexGenivEXT _glGetMultiTexGenivEXT
#define glGetFloatIndexedvEXT _glGetFloatIndexedvEXT
#define glGetDoubleIndexedvEXT _glGetDoubleIndexedvEXT
#define glGetPointerIndexedvEXT _glGetPointerIndexedvEXT
#define glGetFloati_vEXT _glGetFloati_vEXT
#define glGetDoublei_vEXT _glGetDoublei_vEXT
#define glGetPointeri_vEXT _glGetPointeri_vEXT
#define glCompressedTextureImage3DEXT _glCompressedTextureImage3DEXT
#define glCompressedTextureImage2DEXT _glCompressedTextureImage2DEXT
#define glCompressedTextureImage1DEXT _glCompressedTextureImage1DEXT
#define glCompressedTextureSubImage3DEXT _glCompressedTextureSubImage3DEXT
#define glCompressedTextureSubImage2DEXT _glCompressedTextureSubImage2DEXT
#define glCompressedTextureSubImage1DEXT _glCompressedTextureSubImage1DEXT
#define glGetCompressedTextureImageEXT _glGetCompressedTextureImageEXT
#define glCompressedMultiTexImage3DEXT _glCompressedMultiTexImage3DEXT
#define glCompressedMultiTexImage2DEXT _glCompressedMultiTexImage2DEXT
#define glCompressedMultiTexImage1DEXT _glCompressedMultiTexImage1DEXT
#define glCompressedMultiTexSubImage3DEXT _glCompressedMultiTexSubImage3DEXT
#define glCompressedMultiTexSubImage2DEXT _glCompressedMultiTexSubImage2DEXT
#define glCompressedMultiTexSubImage1DEXT _glCompressedMultiTexSubImage1DEXT
#define glGetCompressedMultiTexImageEXT _glGetCompressedMultiTexImageEXT
#define glNamedProgramStringEXT _glNamedProgramStringEXT
#define glNamedProgramLocalParameter4dEXT _glNamedProgramLocalParameter4dEXT
#define glNamedProgramLocalParameter4dvEXT _glNamedProgramLocalParameter4dvEXT
#define glNamedProgramLocalParameter4fEXT _glNamedProgramLocalParameter4fEXT
#define glNamedProgramLocalParameter4fvEXT _glNamedProgramLocalParameter4fvEXT
#define glGetNamedProgramLocalParameterdvEXT _glGetNamedProgramLocalParameterdvEXT
#define glGetNamedProgramLocalParameterfvEXT _glGetNamedProgramLocalParameterfvEXT
#define glGetNamedProgramivEXT _glGetNamedProgramivEXT
#define glGetNamedProgramStringEXT _glGetNamedProgramStringEXT
#define glNamedProgramLocalParameters4fvEXT _glNamedProgramLocalParameters4fvEXT
#define glNamedProgramLocalParameterI4iEXT _glNamedProgramLocalParameterI4iEXT
#define glNamedProgramLocalParameterI4ivEXT _glNamedProgramLocalParameterI4ivEXT
#define glNamedProgramLocalParametersI4ivEXT _glNamedProgramLocalParametersI4ivEXT
#define glNamedProgramLocalParameterI4uiEXT _glNamedProgramLocalParameterI4uiEXT
#define glNamedProgramLocalParameterI4uivEXT _glNamedProgramLocalParameterI4uivEXT
#define glNamedProgramLocalParametersI4uivEXT _glNamedProgramLocalParametersI4uivEXT
#define glGetNamedProgramLocalParameterIivEXT _glGetNamedProgramLocalParameterIivEXT
#define glGetNamedProgramLocalParameterIuivEXT _glGetNamedProgramLocalParameterIuivEXT
#define glTextureParameterIivEXT _glTextureParameterIivEXT
#define glTextureParameterIuivEXT _glTextureParameterIuivEXT
#define glGetTextureParameterIivEXT _glGetTextureParameterIivEXT
#define glGetTextureParameterIuivEXT _glGetTextureParameterIuivEXT
#define glMultiTexParameterIivEXT _glMultiTexParameterIivEXT
#define glMultiTexParameterIuivEXT _glMultiTexParameterIuivEXT
#define glGetMultiTexParameterIivEXT _glGetMultiTexParameterIivEXT
#define glGetMultiTexParameterIuivEXT _glGetMultiTexParameterIuivEXT
#define glProgramUniform1fEXT _glProgramUniform1fEXT
#define glProgramUniform2fEXT _glProgramUniform2fEXT
#define glProgramUniform3fEXT _glProgramUniform3fEXT
#define glProgramUniform4fEXT _glProgramUniform4fEXT
#define glProgramUniform1iEXT _glProgramUniform1iEXT
#define glProgramUniform2iEXT _glProgramUniform2iEXT
#define glProgramUniform3iEXT _glProgramUniform3iEXT
#define glProgramUniform4iEXT _glProgramUniform4iEXT
#define glProgramUniform1fvEXT _glProgramUniform1fvEXT
#define glProgramUniform2fvEXT _glProgramUniform2fvEXT
#define glProgramUniform3fvEXT _glProgramUniform3fvEXT
#define glProgramUniform4fvEXT _glProgramUniform4fvEXT
#define glProgramUniform1ivEXT _glProgramUniform1ivEXT
#define glProgramUniform2ivEXT _glProgramUniform2ivEXT
#define glProgramUniform3ivEXT _glProgramUniform3ivEXT
#define glProgramUniform4ivEXT _glProgramUniform4ivEXT
#define glProgramUniformMatrix2fvEXT _glProgramUniformMatrix2fvEXT
#define glProgramUniformMatrix3fvEXT _glProgramUniformMatrix3fvEXT
#define glProgramUniformMatrix4fvEXT _glProgramUniformMatrix4fvEXT
#define glProgramUniformMatrix2x3fvEXT _glProgramUniformMatrix2x3fvEXT
#define glProgramUniformMatrix3x2fvEXT _glProgramUniformMatrix3x2fvEXT
#define glProgramUniformMatrix2x4fvEXT _glProgramUniformMatrix2x4fvEXT
#define glProgramUniformMatrix4x2fvEXT _glProgramUniformMatrix4x2fvEXT
#define glProgramUniformMatrix3x4fvEXT _glProgramUniformMatrix3x4fvEXT
#define glProgramUniformMatrix4x3fvEXT _glProgramUniformMatrix4x3fvEXT
#define glProgramUniform1uiEXT _glProgramUniform1uiEXT
#define glProgramUniform2uiEXT _glProgramUniform2uiEXT
#define glProgramUniform3uiEXT _glProgramUniform3uiEXT
#define glProgramUniform4uiEXT _glProgramUniform4uiEXT
#define glProgramUniform1uivEXT _glProgramUniform1uivEXT
#define glProgramUniform2uivEXT _glProgramUniform2uivEXT
#define glProgramUniform3uivEXT _glProgramUniform3uivEXT
#define glProgramUniform4uivEXT _glProgramUniform4uivEXT
#define glNamedBufferDataEXT _glNamedBufferDataEXT
#define glNamedBufferSubDataEXT _glNamedBufferSubDataEXT
#define glMapNamedBufferEXT _glMapNamedBufferEXT
#define glUnmapNamedBufferEXT _glUnmapNamedBufferEXT
#define glMapNamedBufferRangeEXT _glMapNamedBufferRangeEXT
#define glFlushMappedNamedBufferRangeEXT _glFlushMappedNamedBufferRangeEXT
#define glNamedCopyBufferSubDataEXT _glNamedCopyBufferSubDataEXT
#define glGetNamedBufferParameterivEXT _glGetNamedBufferParameterivEXT
#define glGetNamedBufferPointervEXT _glGetNamedBufferPointervEXT
#define glGetNamedBufferSubDataEXT _glGetNamedBufferSubDataEXT
#define glTextureBufferEXT _glTextureBufferEXT
#define glMultiTexBufferEXT _glMultiTexBufferEXT
#define glNamedRenderbufferStorageEXT _glNamedRenderbufferStorageEXT
#define glGetNamedRenderbufferParameterivEXT _glGetNamedRenderbufferParameterivEXT
#define glCheckNamedFramebufferStatusEXT _glCheckNamedFramebufferStatusEXT
#define glNamedFramebufferTexture1DEXT _glNamedFramebufferTexture1DEXT
#define glNamedFramebufferTexture2DEXT _glNamedFramebufferTexture2DEXT
#define glNamedFramebufferTexture3DEXT _glNamedFramebufferTexture3DEXT
#define glNamedFramebufferRenderbufferEXT _glNamedFramebufferRenderbufferEXT
#define glGetNamedFramebufferAttachmentParameterivEXT _glGetNamedFramebufferAttachmentParameterivEXT
#define glGenerateTextureMipmapEXT _glGenerateTextureMipmapEXT
#define glGenerateMultiTexMipmapEXT _glGenerateMultiTexMipmapEXT
#define glFramebufferDrawBufferEXT _glFramebufferDrawBufferEXT
#define glFramebufferDrawBuffersEXT _glFramebufferDrawBuffersEXT
#define glFramebufferReadBufferEXT _glFramebufferReadBufferEXT
#define glGetFramebufferParameterivEXT _glGetFramebufferParameterivEXT
#define glNamedRenderbufferStorageMultisampleEXT _glNamedRenderbufferStorageMultisampleEXT
#define glNamedRenderbufferStorageMultisampleCoverageEXT _glNamedRenderbufferStorageMultisampleCoverageEXT
#define glNamedFramebufferTextureEXT _glNamedFramebufferTextureEXT
#define glNamedFramebufferTextureLayerEXT _glNamedFramebufferTextureLayerEXT
#define glNamedFramebufferTextureFaceEXT _glNamedFramebufferTextureFaceEXT
#define glTextureRenderbufferEXT _glTextureRenderbufferEXT
#define glMultiTexRenderbufferEXT _glMultiTexRenderbufferEXT
#define glProgramUniform1dEXT _glProgramUniform1dEXT
#define glProgramUniform2dEXT _glProgramUniform2dEXT
#define glProgramUniform3dEXT _glProgramUniform3dEXT
#define glProgramUniform4dEXT _glProgramUniform4dEXT
#define glProgramUniform1dvEXT _glProgramUniform1dvEXT
#define glProgramUniform2dvEXT _glProgramUniform2dvEXT
#define glProgramUniform3dvEXT _glProgramUniform3dvEXT
#define glProgramUniform4dvEXT _glProgramUniform4dvEXT
#define glProgramUniformMatrix2dvEXT _glProgramUniformMatrix2dvEXT
#define glProgramUniformMatrix3dvEXT _glProgramUniformMatrix3dvEXT
#define glProgramUniformMatrix4dvEXT _glProgramUniformMatrix4dvEXT
#define glProgramUniformMatrix2x3dvEXT _glProgramUniformMatrix2x3dvEXT
#define glProgramUniformMatrix2x4dvEXT _glProgramUniformMatrix2x4dvEXT
#define glProgramUniformMatrix3x2dvEXT _glProgramUniformMatrix3x2dvEXT
#define glProgramUniformMatrix3x4dvEXT _glProgramUniformMatrix3x4dvEXT
#define glProgramUniformMatrix4x2dvEXT _glProgramUniformMatrix4x2dvEXT
#define glProgramUniformMatrix4x3dvEXT _glProgramUniformMatrix4x3dvEXT
#define glVertexArrayVertexOffsetEXT _glVertexArrayVertexOffsetEXT
#define glVertexArrayColorOffsetEXT _glVertexArrayColorOffsetEXT
#define glVertexArrayEdgeFlagOffsetEXT _glVertexArrayEdgeFlagOffsetEXT
#define glVertexArrayIndexOffsetEXT _glVertexArrayIndexOffsetEXT
#define glVertexArrayNormalOffsetEXT _glVertexArrayNormalOffsetEXT
#define glVertexArrayTexCoordOffsetEXT _glVertexArrayTexCoordOffsetEXT
#define glVertexArrayMultiTexCoordOffsetEXT _glVertexArrayMultiTexCoordOffsetEXT
#define glVertexArrayFogCoordOffsetEXT _glVertexArrayFogCoordOffsetEXT
#define glVertexArraySecondaryColorOffsetEXT _glVertexArraySecondaryColorOffsetEXT
#define glVertexArrayVertexAttribOffsetEXT _glVertexArrayVertexAttribOffsetEXT
#define glVertexArrayVertexAttribIOffsetEXT _glVertexArrayVertexAttribIOffsetEXT
#define glEnableVertexArrayEXT _glEnableVertexArrayEXT
#define glDisableVertexArrayEXT _glDisableVertexArrayEXT
#define glEnableVertexArrayAttribEXT _glEnableVertexArrayAttribEXT
#define glDisableVertexArrayAttribEXT _glDisableVertexArrayAttribEXT
#define glGetVertexArrayIntegervEXT _glGetVertexArrayIntegervEXT
#define glGetVertexArrayPointervEXT _glGetVertexArrayPointervEXT
#define glGetVertexArrayIntegeri_vEXT _glGetVertexArrayIntegeri_vEXT
#define glGetVertexArrayPointeri_vEXT _glGetVertexArrayPointeri_vEXT
#define glDiscardFramebufferEXT _glDiscardFramebufferEXT
#define glColorMaskIndexedEXT _glColorMaskIndexedEXT
#define glGetBooleanIndexedvEXT _glGetBooleanIndexedvEXT
#define glGetIntegerIndexedvEXT _glGetIntegerIndexedvEXT
#define glEnableIndexedEXT _glEnableIndexedEXT
#define glDisableIndexedEXT _glDisableIndexedEXT
#define glIsEnabledIndexedEXT _glIsEnabledIndexedEXT
#define glDrawArraysInstancedEXT _glDrawArraysInstancedEXT
#define glDrawElementsInstancedEXT _glDrawElementsInstancedEXT
#define glDrawRangeElementsEXT _glDrawRangeElementsEXT
#define glFogCoordfEXT _glFogCoordfEXT
#define glFogCoordfvEXT _glFogCoordfvEXT
#define glFogCoorddEXT _glFogCoorddEXT
#define glFogCoorddvEXT _glFogCoorddvEXT
#define glFogCoordPointerEXT _glFogCoordPointerEXT
#define glBlitFramebufferEXT _glBlitFramebufferEXT
#define glRenderbufferStorageMultisampleEXT _glRenderbufferStorageMultisampleEXT
#define glIsRenderbufferEXT _glIsRenderbufferEXT
#define glBindRenderbufferEXT _glBindRenderbufferEXT
#define glDeleteRenderbuffersEXT _glDeleteRenderbuffersEXT
#define glGenRenderbuffersEXT _glGenRenderbuffersEXT
#define glRenderbufferStorageEXT _glRenderbufferStorageEXT
#define glGetRenderbufferParameterivEXT _glGetRenderbufferParameterivEXT
#define glIsFramebufferEXT _glIsFramebufferEXT
#define glBindFramebufferEXT _glBindFramebufferEXT
#define glDeleteFramebuffersEXT _glDeleteFramebuffersEXT
#define glGenFramebuffersEXT _glGenFramebuffersEXT
#define glCheckFramebufferStatusEXT _glCheckFramebufferStatusEXT
#define glFramebufferTexture1DEXT _glFramebufferTexture1DEXT
#define glFramebufferTexture2DEXT _glFramebufferTexture2DEXT
#define glFramebufferTexture3DEXT _glFramebufferTexture3DEXT
#define glFramebufferRenderbufferEXT _glFramebufferRenderbufferEXT
#define glGetFramebufferAttachmentParameterivEXT _glGetFramebufferAttachmentParameterivEXT
#define glGenerateMipmapEXT _glGenerateMipmapEXT
#define glProgramParameteriEXT _glProgramParameteriEXT
#define glProgramEnvParameters4fvEXT _glProgramEnvParameters4fvEXT
#define glProgramLocalParameters4fvEXT _glProgramLocalParameters4fvEXT
#define glGetUniformuivEXT _glGetUniformuivEXT
#define glBindFragDataLocationEXT _glBindFragDataLocationEXT
#define glGetFragDataLocationEXT _glGetFragDataLocationEXT
#define glUniform1uiEXT _glUniform1uiEXT
#define glUniform2uiEXT _glUniform2uiEXT
#define glUniform3uiEXT _glUniform3uiEXT
#define glUniform4uiEXT _glUniform4uiEXT
#define glUniform1uivEXT _glUniform1uivEXT
#define glUniform2uivEXT _glUniform2uivEXT
#define glUniform3uivEXT _glUniform3uivEXT
#define glUniform4uivEXT _glUniform4uivEXT
#define glGetHistogramEXT _glGetHistogramEXT
#define glGetHistogramParameterfvEXT _glGetHistogramParameterfvEXT
#define glGetHistogramParameterivEXT _glGetHistogramParameterivEXT
#define glGetMinmaxEXT _glGetMinmaxEXT
#define glGetMinmaxParameterfvEXT _glGetMinmaxParameterfvEXT
#define glGetMinmaxParameterivEXT _glGetMinmaxParameterivEXT
#define glHistogramEXT _glHistogramEXT
#define glMinmaxEXT _glMinmaxEXT
#define glResetHistogramEXT _glResetHistogramEXT
#define glResetMinmaxEXT _glResetMinmaxEXT
#define glIndexFuncEXT _glIndexFuncEXT
#define glIndexMaterialEXT _glIndexMaterialEXT
#define glApplyTextureEXT _glApplyTextureEXT
#define glTextureLightEXT _glTextureLightEXT
#define glTextureMaterialEXT _glTextureMaterialEXT
#define glMultiDrawArraysEXT _glMultiDrawArraysEXT
#define glMultiDrawElementsEXT _glMultiDrawElementsEXT
#define glSampleMaskEXT _glSampleMaskEXT
#define glSamplePatternEXT _glSamplePatternEXT
#define glColorTableEXT _glColorTableEXT
#define glGetColorTableEXT _glGetColorTableEXT
#define glGetColorTableParameterivEXT _glGetColorTableParameterivEXT
#define glGetColorTableParameterfvEXT _glGetColorTableParameterfvEXT
#define glPixelTransformParameteriEXT _glPixelTransformParameteriEXT
#define glPixelTransformParameterfEXT _glPixelTransformParameterfEXT
#define glPixelTransformParameterivEXT _glPixelTransformParameterivEXT
#define glPixelTransformParameterfvEXT _glPixelTransformParameterfvEXT
#define glGetPixelTransformParameterivEXT _glGetPixelTransformParameterivEXT
#define glGetPixelTransformParameterfvEXT _glGetPixelTransformParameterfvEXT
#define glPointParameterfEXT _glPointParameterfEXT
#define glPointParameterfvEXT _glPointParameterfvEXT
#define glPolygonOffsetEXT _glPolygonOffsetEXT
#define glProvokingVertexEXT _glProvokingVertexEXT
#define glSecondaryColor3bEXT _glSecondaryColor3bEXT
#define glSecondaryColor3bvEXT _glSecondaryColor3bvEXT
#define glSecondaryColor3dEXT _glSecondaryColor3dEXT
#define glSecondaryColor3dvEXT _glSecondaryColor3dvEXT
#define glSecondaryColor3fEXT _glSecondaryColor3fEXT
#define glSecondaryColor3fvEXT _glSecondaryColor3fvEXT
#define glSecondaryColor3iEXT _glSecondaryColor3iEXT
#define glSecondaryColor3ivEXT _glSecondaryColor3ivEXT
#define glSecondaryColor3sEXT _glSecondaryColor3sEXT
#define glSecondaryColor3svEXT _glSecondaryColor3svEXT
#define glSecondaryColor3ubEXT _glSecondaryColor3ubEXT
#define glSecondaryColor3ubvEXT _glSecondaryColor3ubvEXT
#define glSecondaryColor3uiEXT _glSecondaryColor3uiEXT
#define glSecondaryColor3uivEXT _glSecondaryColor3uivEXT
#define glSecondaryColor3usEXT _glSecondaryColor3usEXT
#define glSecondaryColor3usvEXT _glSecondaryColor3usvEXT
#define glSecondaryColorPointerEXT _glSecondaryColorPointerEXT
#define glUseShaderProgramEXT _glUseShaderProgramEXT
#define glActiveProgramEXT _glActiveProgramEXT
#define glCreateShaderProgramEXT _glCreateShaderProgramEXT
#define glBindImageTextureEXT _glBindImageTextureEXT
#define glMemoryBarrierEXT _glMemoryBarrierEXT
#define glStencilClearTagEXT _glStencilClearTagEXT
#define glActiveStencilFaceEXT _glActiveStencilFaceEXT
#define glTexSubImage1DEXT _glTexSubImage1DEXT
#define glTexSubImage2DEXT _glTexSubImage2DEXT
#define glTexImage3DEXT _glTexImage3DEXT
#define glTexSubImage3DEXT _glTexSubImage3DEXT
#define glTexBufferEXT _glTexBufferEXT
#define glTexParameterIivEXT _glTexParameterIivEXT
#define glTexParameterIuivEXT _glTexParameterIuivEXT
#define glGetTexParameterIivEXT _glGetTexParameterIivEXT
#define glGetTexParameterIuivEXT _glGetTexParameterIuivEXT
#define glClearColorIiEXT _glClearColorIiEXT
#define glClearColorIuiEXT _glClearColorIuiEXT
#define glAreTexturesResidentEXT _glAreTexturesResidentEXT
#define glBindTextureEXT _glBindTextureEXT
#define glDeleteTexturesEXT _glDeleteTexturesEXT
#define glGenTexturesEXT _glGenTexturesEXT
#define glIsTextureEXT _glIsTextureEXT
#define glPrioritizeTexturesEXT _glPrioritizeTexturesEXT
#define glTextureNormalEXT _glTextureNormalEXT
#define glGetQueryObjecti64vEXT _glGetQueryObjecti64vEXT
#define glGetQueryObjectui64vEXT _glGetQueryObjectui64vEXT
#define glBeginTransformFeedbackEXT _glBeginTransformFeedbackEXT
#define glEndTransformFeedbackEXT _glEndTransformFeedbackEXT
#define glBindBufferRangeEXT _glBindBufferRangeEXT
#define glBindBufferOffsetEXT _glBindBufferOffsetEXT
#define glBindBufferBaseEXT _glBindBufferBaseEXT
#define glTransformFeedbackVaryingsEXT _glTransformFeedbackVaryingsEXT
#define glGetTransformFeedbackVaryingEXT _glGetTransformFeedbackVaryingEXT
#define glArrayElementEXT _glArrayElementEXT
#define glColorPointerEXT _glColorPointerEXT
#define glDrawArraysEXT _glDrawArraysEXT
#define glEdgeFlagPointerEXT _glEdgeFlagPointerEXT
#define glGetPointervEXT _glGetPointervEXT
#define glIndexPointerEXT _glIndexPointerEXT
#define glNormalPointerEXT _glNormalPointerEXT
#define glTexCoordPointerEXT _glTexCoordPointerEXT
#define glVertexPointerEXT _glVertexPointerEXT
#define glVertexAttribL1dEXT _glVertexAttribL1dEXT
#define glVertexAttribL2dEXT _glVertexAttribL2dEXT
#define glVertexAttribL3dEXT _glVertexAttribL3dEXT
#define glVertexAttribL4dEXT _glVertexAttribL4dEXT
#define glVertexAttribL1dvEXT _glVertexAttribL1dvEXT
#define glVertexAttribL2dvEXT _glVertexAttribL2dvEXT
#define glVertexAttribL3dvEXT _glVertexAttribL3dvEXT
#define glVertexAttribL4dvEXT _glVertexAttribL4dvEXT
#define glVertexAttribLPointerEXT _glVertexAttribLPointerEXT
#define glGetVertexAttribLdvEXT _glGetVertexAttribLdvEXT
#define glVertexArrayVertexAttribLOffsetEXT _glVertexArrayVertexAttribLOffsetEXT
#define glBeginVertexShaderEXT _glBeginVertexShaderEXT
#define glEndVertexShaderEXT _glEndVertexShaderEXT
#define glBindVertexShaderEXT _glBindVertexShaderEXT
#define glGenVertexShadersEXT _glGenVertexShadersEXT
#define glDeleteVertexShaderEXT _glDeleteVertexShaderEXT
#define glShaderOp1EXT _glShaderOp1EXT
#define glShaderOp2EXT _glShaderOp2EXT
#define glShaderOp3EXT _glShaderOp3EXT
#define glSwizzleEXT _glSwizzleEXT
#define glWriteMaskEXT _glWriteMaskEXT
#define glInsertComponentEXT _glInsertComponentEXT
#define glExtractComponentEXT _glExtractComponentEXT
#define glGenSymbolsEXT _glGenSymbolsEXT
#define glSetInvariantEXT _glSetInvariantEXT
#define glSetLocalConstantEXT _glSetLocalConstantEXT
#define glVariantbvEXT _glVariantbvEXT
#define glVariantsvEXT _glVariantsvEXT
#define glVariantivEXT _glVariantivEXT
#define glVariantfvEXT _glVariantfvEXT
#define glVariantdvEXT _glVariantdvEXT
#define glVariantubvEXT _glVariantubvEXT
#define glVariantusvEXT _glVariantusvEXT
#define glVariantuivEXT _glVariantuivEXT
#define glVariantPointerEXT _glVariantPointerEXT
#define glEnableVariantClientStateEXT _glEnableVariantClientStateEXT
#define glDisableVariantClientStateEXT _glDisableVariantClientStateEXT
#define glBindLightParameterEXT _glBindLightParameterEXT
#define glBindMaterialParameterEXT _glBindMaterialParameterEXT
#define glBindTexGenParameterEXT _glBindTexGenParameterEXT
#define glBindTextureUnitParameterEXT _glBindTextureUnitParameterEXT
#define glBindParameterEXT _glBindParameterEXT
#define glIsVariantEnabledEXT _glIsVariantEnabledEXT
#define glGetVariantBooleanvEXT _glGetVariantBooleanvEXT
#define glGetVariantIntegervEXT _glGetVariantIntegervEXT
#define glGetVariantFloatvEXT _glGetVariantFloatvEXT
#define glGetVariantPointervEXT _glGetVariantPointervEXT
#define glGetInvariantBooleanvEXT _glGetInvariantBooleanvEXT
#define glGetInvariantIntegervEXT _glGetInvariantIntegervEXT
#define glGetInvariantFloatvEXT _glGetInvariantFloatvEXT
#define glGetLocalConstantBooleanvEXT _glGetLocalConstantBooleanvEXT
#define glGetLocalConstantIntegervEXT _glGetLocalConstantIntegervEXT
#define glGetLocalConstantFloatvEXT _glGetLocalConstantFloatvEXT
#define glVertexWeightfEXT _glVertexWeightfEXT
#define glVertexWeightfvEXT _glVertexWeightfvEXT
#define glVertexWeightPointerEXT _glVertexWeightPointerEXT
#define glImportSyncEXT _glImportSyncEXT
#define glFrameTerminatorGREMEDY _glFrameTerminatorGREMEDY
#define glStringMarkerGREMEDY _glStringMarkerGREMEDY
#define glImageTransformParameteriHP _glImageTransformParameteriHP
#define glImageTransformParameterfHP _glImageTransformParameterfHP
#define glImageTransformParameterivHP _glImageTransformParameterivHP
#define glImageTransformParameterfvHP _glImageTransformParameterfvHP
#define glGetImageTransformParameterivHP _glGetImageTransformParameterivHP
#define glGetImageTransformParameterfvHP _glGetImageTransformParameterfvHP
#define glMultiModeDrawArraysIBM _glMultiModeDrawArraysIBM
#define glMultiModeDrawElementsIBM _glMultiModeDrawElementsIBM
#define glColorPointerListIBM _glColorPointerListIBM
#define glSecondaryColorPointerListIBM _glSecondaryColorPointerListIBM
#define glEdgeFlagPointerListIBM _glEdgeFlagPointerListIBM
#define glFogCoordPointerListIBM _glFogCoordPointerListIBM
#define glIndexPointerListIBM _glIndexPointerListIBM
#define glNormalPointerListIBM _glNormalPointerListIBM
#define glTexCoordPointerListIBM _glTexCoordPointerListIBM
#define glVertexPointerListIBM _glVertexPointerListIBM
#define glBlendFuncSeparateINGR _glBlendFuncSeparateINGR
#define glVertexPointervINTEL _glVertexPointervINTEL
#define glNormalPointervINTEL _glNormalPointervINTEL
#define glColorPointervINTEL _glColorPointervINTEL
#define glTexCoordPointervINTEL _glTexCoordPointervINTEL
#define glDebugMessageControl _glDebugMessageControl
#define glDebugMessageInsert _glDebugMessageInsert
#define glDebugMessageCallback _glDebugMessageCallback
#define glGetDebugMessageLog _glGetDebugMessageLog
#define glPushDebugGroup _glPushDebugGroup
#define glPopDebugGroup _glPopDebugGroup
#define glObjectLabel _glObjectLabel
#define glGetObjectLabel _glGetObjectLabel
#define glObjectPtrLabel _glObjectPtrLabel
#define glGetObjectPtrLabel _glGetObjectPtrLabel
#define glGetGraphicsResetStatus _glGetGraphicsResetStatus
#define glReadnPixels _glReadnPixels
#define glGetnUniformfv _glGetnUniformfv
#define glGetnUniformiv _glGetnUniformiv
#define glGetnUniformuiv _glGetnUniformuiv
#define glNewBufferRegion _glNewBufferRegion
#define glDeleteBufferRegion _glDeleteBufferRegion
#define glReadBufferRegion _glReadBufferRegion
#define glDrawBufferRegion _glDrawBufferRegion
#define glBufferRegionEnabled _glBufferRegionEnabled
#define glResizeBuffersMESA _glResizeBuffersMESA
#define glWindowPos2dMESA _glWindowPos2dMESA
#define glWindowPos2dvMESA _glWindowPos2dvMESA
#define glWindowPos2fMESA _glWindowPos2fMESA
#define glWindowPos2fvMESA _glWindowPos2fvMESA
#define glWindowPos2iMESA _glWindowPos2iMESA
#define glWindowPos2ivMESA _glWindowPos2ivMESA
#define glWindowPos2sMESA _glWindowPos2sMESA
#define glWindowPos2svMESA _glWindowPos2svMESA
#define glWindowPos3dMESA _glWindowPos3dMESA
#define glWindowPos3dvMESA _glWindowPos3dvMESA
#define glWindowPos3fMESA _glWindowPos3fMESA
#define glWindowPos3fvMESA _glWindowPos3fvMESA
#define glWindowPos3iMESA _glWindowPos3iMESA
#define glWindowPos3ivMESA _glWindowPos3ivMESA
#define glWindowPos3sMESA _glWindowPos3sMESA
#define glWindowPos3svMESA _glWindowPos3svMESA
#define glWindowPos4dMESA _glWindowPos4dMESA
#define glWindowPos4dvMESA _glWindowPos4dvMESA
#define glWindowPos4fMESA _glWindowPos4fMESA
#define glWindowPos4fvMESA _glWindowPos4fvMESA
#define glWindowPos4iMESA _glWindowPos4iMESA
#define glWindowPos4ivMESA _glWindowPos4ivMESA
#define glWindowPos4sMESA _glWindowPos4sMESA
#define glWindowPos4svMESA _glWindowPos4svMESA
#define glBeginConditionalRenderNVX _glBeginConditionalRenderNVX
#define glEndConditionalRenderNVX _glEndConditionalRenderNVX
#define glMultiDrawArraysIndirectBindlessNV _glMultiDrawArraysIndirectBindlessNV
#define glMultiDrawElementsIndirectBindlessNV _glMultiDrawElementsIndirectBindlessNV
#define glGetTextureHandleNV _glGetTextureHandleNV
#define glGetTextureSamplerHandleNV _glGetTextureSamplerHandleNV
#define glMakeTextureHandleResidentNV _glMakeTextureHandleResidentNV
#define glMakeTextureHandleNonResidentNV _glMakeTextureHandleNonResidentNV
#define glGetImageHandleNV _glGetImageHandleNV
#define glMakeImageHandleResidentNV _glMakeImageHandleResidentNV
#define glMakeImageHandleNonResidentNV _glMakeImageHandleNonResidentNV
#define glUniformHandleui64NV _glUniformHandleui64NV
#define glUniformHandleui64vNV _glUniformHandleui64vNV
#define glProgramUniformHandleui64NV _glProgramUniformHandleui64NV
#define glProgramUniformHandleui64vNV _glProgramUniformHandleui64vNV
#define glIsTextureHandleResidentNV _glIsTextureHandleResidentNV
#define glIsImageHandleResidentNV _glIsImageHandleResidentNV
#define glBlendParameteriNV _glBlendParameteriNV
#define glBlendBarrierNV _glBlendBarrierNV
#define glBeginConditionalRenderNV _glBeginConditionalRenderNV
#define glEndConditionalRenderNV _glEndConditionalRenderNV
#define glCopyImageSubDataNV _glCopyImageSubDataNV
#define glDepthRangedNV _glDepthRangedNV
#define glClearDepthdNV _glClearDepthdNV
#define glDepthBoundsdNV _glDepthBoundsdNV
#define glDrawTextureNV _glDrawTextureNV
#define glMapControlPointsNV _glMapControlPointsNV
#define glMapParameterivNV _glMapParameterivNV
#define glMapParameterfvNV _glMapParameterfvNV
#define glGetMapControlPointsNV _glGetMapControlPointsNV
#define glGetMapParameterivNV _glGetMapParameterivNV
#define glGetMapParameterfvNV _glGetMapParameterfvNV
#define glGetMapAttribParameterivNV _glGetMapAttribParameterivNV
#define glGetMapAttribParameterfvNV _glGetMapAttribParameterfvNV
#define glEvalMapsNV _glEvalMapsNV
#define glGetMultisamplefvNV _glGetMultisamplefvNV
#define glSampleMaskIndexedNV _glSampleMaskIndexedNV
#define glTexRenderbufferNV _glTexRenderbufferNV
#define glDeleteFencesNV _glDeleteFencesNV
#define glGenFencesNV _glGenFencesNV
#define glIsFenceNV _glIsFenceNV
#define glTestFenceNV _glTestFenceNV
#define glGetFenceivNV _glGetFenceivNV
#define glFinishFenceNV _glFinishFenceNV
#define glSetFenceNV _glSetFenceNV
#define glProgramNamedParameter4fNV _glProgramNamedParameter4fNV
#define glProgramNamedParameter4dNV _glProgramNamedParameter4dNV
#define glProgramNamedParameter4fvNV _glProgramNamedParameter4fvNV
#define glProgramNamedParameter4dvNV _glProgramNamedParameter4dvNV
#define glGetProgramNamedParameterdvNV _glGetProgramNamedParameterdvNV
#define glGetProgramNamedParameterfvNV _glGetProgramNamedParameterfvNV
#define glBlitFramebufferNV _glBlitFramebufferNV
#define glRenderbufferStorageMultisampleCoverageNV _glRenderbufferStorageMultisampleCoverageNV
#define glProgramVertexLimitNV _glProgramVertexLimitNV
#define glFramebufferTextureEXT _glFramebufferTextureEXT
#define glFramebufferTextureLayerEXT _glFramebufferTextureLayerEXT
#define glFramebufferTextureFaceEXT _glFramebufferTextureFaceEXT
#define glProgramLocalParameterI4iNV _glProgramLocalParameterI4iNV
#define glProgramLocalParameterI4ivNV _glProgramLocalParameterI4ivNV
#define glProgramLocalParametersI4ivNV _glProgramLocalParametersI4ivNV
#define glProgramLocalParameterI4uiNV _glProgramLocalParameterI4uiNV
#define glProgramLocalParameterI4uivNV _glProgramLocalParameterI4uivNV
#define glProgramLocalParametersI4uivNV _glProgramLocalParametersI4uivNV
#define glProgramEnvParameterI4iNV _glProgramEnvParameterI4iNV
#define glProgramEnvParameterI4ivNV _glProgramEnvParameterI4ivNV
#define glProgramEnvParametersI4ivNV _glProgramEnvParametersI4ivNV
#define glProgramEnvParameterI4uiNV _glProgramEnvParameterI4uiNV
#define glProgramEnvParameterI4uivNV _glProgramEnvParameterI4uivNV
#define glProgramEnvParametersI4uivNV _glProgramEnvParametersI4uivNV
#define glGetProgramLocalParameterIivNV _glGetProgramLocalParameterIivNV
#define glGetProgramLocalParameterIuivNV _glGetProgramLocalParameterIuivNV
#define glGetProgramEnvParameterIivNV _glGetProgramEnvParameterIivNV
#define glGetProgramEnvParameterIuivNV _glGetProgramEnvParameterIuivNV
#define glProgramSubroutineParametersuivNV _glProgramSubroutineParametersuivNV
#define glGetProgramSubroutineParameteruivNV _glGetProgramSubroutineParameteruivNV
#define glUniform1i64NV _glUniform1i64NV
#define glUniform2i64NV _glUniform2i64NV
#define glUniform3i64NV _glUniform3i64NV
#define glUniform4i64NV _glUniform4i64NV
#define glUniform1i64vNV _glUniform1i64vNV
#define glUniform2i64vNV _glUniform2i64vNV
#define glUniform3i64vNV _glUniform3i64vNV
#define glUniform4i64vNV _glUniform4i64vNV
#define glUniform1ui64NV _glUniform1ui64NV
#define glUniform2ui64NV _glUniform2ui64NV
#define glUniform3ui64NV _glUniform3ui64NV
#define glUniform4ui64NV _glUniform4ui64NV
#define glUniform1ui64vNV _glUniform1ui64vNV
#define glUniform2ui64vNV _glUniform2ui64vNV
#define glUniform3ui64vNV _glUniform3ui64vNV
#define glUniform4ui64vNV _glUniform4ui64vNV
#define glGetUniformi64vNV _glGetUniformi64vNV
#define glProgramUniform1i64NV _glProgramUniform1i64NV
#define glProgramUniform2i64NV _glProgramUniform2i64NV
#define glProgramUniform3i64NV _glProgramUniform3i64NV
#define glProgramUniform4i64NV _glProgramUniform4i64NV
#define glProgramUniform1i64vNV _glProgramUniform1i64vNV
#define glProgramUniform2i64vNV _glProgramUniform2i64vNV
#define glProgramUniform3i64vNV _glProgramUniform3i64vNV
#define glProgramUniform4i64vNV _glProgramUniform4i64vNV
#define glProgramUniform1ui64NV _glProgramUniform1ui64NV
#define glProgramUniform2ui64NV _glProgramUniform2ui64NV
#define glProgramUniform3ui64NV _glProgramUniform3ui64NV
#define glProgramUniform4ui64NV _glProgramUniform4ui64NV
#define glProgramUniform1ui64vNV _glProgramUniform1ui64vNV
#define glProgramUniform2ui64vNV _glProgramUniform2ui64vNV
#define glProgramUniform3ui64vNV _glProgramUniform3ui64vNV
#define glProgramUniform4ui64vNV _glProgramUniform4ui64vNV
#define glVertex2hNV _glVertex2hNV
#define glVertex2hvNV _glVertex2hvNV
#define glVertex3hNV _glVertex3hNV
#define glVertex3hvNV _glVertex3hvNV
#define glVertex4hNV _glVertex4hNV
#define glVertex4hvNV _glVertex4hvNV
#define glNormal3hNV _glNormal3hNV
#define glNormal3hvNV _glNormal3hvNV
#define glColor3hNV _glColor3hNV
#define glColor3hvNV _glColor3hvNV
#define glColor4hNV _glColor4hNV
#define glColor4hvNV _glColor4hvNV
#define glTexCoord1hNV _glTexCoord1hNV
#define glTexCoord1hvNV _glTexCoord1hvNV
#define glTexCoord2hNV _glTexCoord2hNV
#define glTexCoord2hvNV _glTexCoord2hvNV
#define glTexCoord3hNV _glTexCoord3hNV
#define glTexCoord3hvNV _glTexCoord3hvNV
#define glTexCoord4hNV _glTexCoord4hNV
#define glTexCoord4hvNV _glTexCoord4hvNV
#define glMultiTexCoord1hNV _glMultiTexCoord1hNV
#define glMultiTexCoord1hvNV _glMultiTexCoord1hvNV
#define glMultiTexCoord2hNV _glMultiTexCoord2hNV
#define glMultiTexCoord2hvNV _glMultiTexCoord2hvNV
#define glMultiTexCoord3hNV _glMultiTexCoord3hNV
#define glMultiTexCoord3hvNV _glMultiTexCoord3hvNV
#define glMultiTexCoord4hNV _glMultiTexCoord4hNV
#define glMultiTexCoord4hvNV _glMultiTexCoord4hvNV
#define glFogCoordhNV _glFogCoordhNV
#define glFogCoordhvNV _glFogCoordhvNV
#define glSecondaryColor3hNV _glSecondaryColor3hNV
#define glSecondaryColor3hvNV _glSecondaryColor3hvNV
#define glVertexWeighthNV _glVertexWeighthNV
#define glVertexWeighthvNV _glVertexWeighthvNV
#define glVertexAttrib1hNV _glVertexAttrib1hNV
#define glVertexAttrib1hvNV _glVertexAttrib1hvNV
#define glVertexAttrib2hNV _glVertexAttrib2hNV
#define glVertexAttrib2hvNV _glVertexAttrib2hvNV
#define glVertexAttrib3hNV _glVertexAttrib3hNV
#define glVertexAttrib3hvNV _glVertexAttrib3hvNV
#define glVertexAttrib4hNV _glVertexAttrib4hNV
#define glVertexAttrib4hvNV _glVertexAttrib4hvNV
#define glVertexAttribs1hvNV _glVertexAttribs1hvNV
#define glVertexAttribs2hvNV _glVertexAttribs2hvNV
#define glVertexAttribs3hvNV _glVertexAttribs3hvNV
#define glVertexAttribs4hvNV _glVertexAttribs4hvNV
#define glGenOcclusionQueriesNV _glGenOcclusionQueriesNV
#define glDeleteOcclusionQueriesNV _glDeleteOcclusionQueriesNV
#define glIsOcclusionQueryNV _glIsOcclusionQueryNV
#define glBeginOcclusionQueryNV _glBeginOcclusionQueryNV
#define glEndOcclusionQueryNV _glEndOcclusionQueryNV
#define glGetOcclusionQueryivNV _glGetOcclusionQueryivNV
#define glGetOcclusionQueryuivNV _glGetOcclusionQueryuivNV
#define glProgramBufferParametersfvNV _glProgramBufferParametersfvNV
#define glProgramBufferParametersIivNV _glProgramBufferParametersIivNV
#define glProgramBufferParametersIuivNV _glProgramBufferParametersIuivNV
#define glGenPathsNV _glGenPathsNV
#define glDeletePathsNV _glDeletePathsNV
#define glIsPathNV _glIsPathNV
#define glPathCommandsNV _glPathCommandsNV
#define glPathCoordsNV _glPathCoordsNV
#define glPathSubCommandsNV _glPathSubCommandsNV
#define glPathSubCoordsNV _glPathSubCoordsNV
#define glPathStringNV _glPathStringNV
#define glPathGlyphsNV _glPathGlyphsNV
#define glPathGlyphRangeNV _glPathGlyphRangeNV
#define glWeightPathsNV _glWeightPathsNV
#define glCopyPathNV _glCopyPathNV
#define glInterpolatePathsNV _glInterpolatePathsNV
#define glTransformPathNV _glTransformPathNV
#define glPathParameterivNV _glPathParameterivNV
#define glPathParameteriNV _glPathParameteriNV
#define glPathParameterfvNV _glPathParameterfvNV
#define glPathParameterfNV _glPathParameterfNV
#define glPathDashArrayNV _glPathDashArrayNV
#define glPathStencilFuncNV _glPathStencilFuncNV
#define glPathStencilDepthOffsetNV _glPathStencilDepthOffsetNV
#define glStencilFillPathNV _glStencilFillPathNV
#define glStencilStrokePathNV _glStencilStrokePathNV
#define glStencilFillPathInstancedNV _glStencilFillPathInstancedNV
#define glStencilStrokePathInstancedNV _glStencilStrokePathInstancedNV
#define glPathCoverDepthFuncNV _glPathCoverDepthFuncNV
#define glPathColorGenNV _glPathColorGenNV
#define glPathTexGenNV _glPathTexGenNV
#define glPathFogGenNV _glPathFogGenNV
#define glCoverFillPathNV _glCoverFillPathNV
#define glCoverStrokePathNV _glCoverStrokePathNV
#define glCoverFillPathInstancedNV _glCoverFillPathInstancedNV
#define glCoverStrokePathInstancedNV _glCoverStrokePathInstancedNV
#define glGetPathParameterivNV _glGetPathParameterivNV
#define glGetPathParameterfvNV _glGetPathParameterfvNV
#define glGetPathCommandsNV _glGetPathCommandsNV
#define glGetPathCoordsNV _glGetPathCoordsNV
#define glGetPathDashArrayNV _glGetPathDashArrayNV
#define glGetPathMetricsNV _glGetPathMetricsNV
#define glGetPathMetricRangeNV _glGetPathMetricRangeNV
#define glGetPathSpacingNV _glGetPathSpacingNV
#define glGetPathColorGenivNV _glGetPathColorGenivNV
#define glGetPathColorGenfvNV _glGetPathColorGenfvNV
#define glGetPathTexGenivNV _glGetPathTexGenivNV
#define glGetPathTexGenfvNV _glGetPathTexGenfvNV
#define glIsPointInFillPathNV _glIsPointInFillPathNV
#define glIsPointInStrokePathNV _glIsPointInStrokePathNV
#define glGetPathLengthNV _glGetPathLengthNV
#define glPointAlongPathNV _glPointAlongPathNV
#define glPixelDataRangeNV _glPixelDataRangeNV
#define glFlushPixelDataRangeNV _glFlushPixelDataRangeNV
#define glPointParameteriNV _glPointParameteriNV
#define glPointParameterivNV _glPointParameterivNV
#define glPresentFrameKeyedNV _glPresentFrameKeyedNV
#define glPresentFrameDualFillNV _glPresentFrameDualFillNV
#define glGetVideoivNV _glGetVideoivNV
#define glGetVideouivNV _glGetVideouivNV
#define glGetVideoi64vNV _glGetVideoi64vNV
#define glGetVideoui64vNV _glGetVideoui64vNV
#define glPrimitiveRestartNV _glPrimitiveRestartNV
#define glPrimitiveRestartIndexNV _glPrimitiveRestartIndexNV
#define glCombinerParameterfvNV _glCombinerParameterfvNV
#define glCombinerParameterfNV _glCombinerParameterfNV
#define glCombinerParameterivNV _glCombinerParameterivNV
#define glCombinerParameteriNV _glCombinerParameteriNV
#define glCombinerInputNV _glCombinerInputNV
#define glCombinerOutputNV _glCombinerOutputNV
#define glFinalCombinerInputNV _glFinalCombinerInputNV
#define glGetCombinerInputParameterfvNV _glGetCombinerInputParameterfvNV
#define glGetCombinerInputParameterivNV _glGetCombinerInputParameterivNV
#define glGetCombinerOutputParameterfvNV _glGetCombinerOutputParameterfvNV
#define glGetCombinerOutputParameterivNV _glGetCombinerOutputParameterivNV
#define glGetFinalCombinerInputParameterfvNV _glGetFinalCombinerInputParameterfvNV
#define glGetFinalCombinerInputParameterivNV _glGetFinalCombinerInputParameterivNV
#define glCombinerStageParameterfvNV _glCombinerStageParameterfvNV
#define glGetCombinerStageParameterfvNV _glGetCombinerStageParameterfvNV
#define glMakeBufferResidentNV _glMakeBufferResidentNV
#define glMakeBufferNonResidentNV _glMakeBufferNonResidentNV
#define glIsBufferResidentNV _glIsBufferResidentNV
#define glMakeNamedBufferResidentNV _glMakeNamedBufferResidentNV
#define glMakeNamedBufferNonResidentNV _glMakeNamedBufferNonResidentNV
#define glIsNamedBufferResidentNV _glIsNamedBufferResidentNV
#define glGetBufferParameterui64vNV _glGetBufferParameterui64vNV
#define glGetNamedBufferParameterui64vNV _glGetNamedBufferParameterui64vNV
#define glGetIntegerui64vNV _glGetIntegerui64vNV
#define glUniformui64NV _glUniformui64NV
#define glUniformui64vNV _glUniformui64vNV
#define glGetUniformui64vNV _glGetUniformui64vNV
#define glProgramUniformui64NV _glProgramUniformui64NV
#define glProgramUniformui64vNV _glProgramUniformui64vNV
#define glTextureBarrierNV _glTextureBarrierNV
#define glTexImage2DMultisampleCoverageNV _glTexImage2DMultisampleCoverageNV
#define glTexImage3DMultisampleCoverageNV _glTexImage3DMultisampleCoverageNV
#define glTextureImage2DMultisampleNV _glTextureImage2DMultisampleNV
#define glTextureImage3DMultisampleNV _glTextureImage3DMultisampleNV
#define glTextureImage2DMultisampleCoverageNV _glTextureImage2DMultisampleCoverageNV
#define glTextureImage3DMultisampleCoverageNV _glTextureImage3DMultisampleCoverageNV
#define glBeginTransformFeedbackNV _glBeginTransformFeedbackNV
#define glEndTransformFeedbackNV _glEndTransformFeedbackNV
#define glTransformFeedbackAttribsNV _glTransformFeedbackAttribsNV
#define glBindBufferRangeNV _glBindBufferRangeNV
#define glBindBufferOffsetNV _glBindBufferOffsetNV
#define glBindBufferBaseNV _glBindBufferBaseNV
#define glTransformFeedbackVaryingsNV _glTransformFeedbackVaryingsNV
#define glActiveVaryingNV _glActiveVaryingNV
#define glGetVaryingLocationNV _glGetVaryingLocationNV
#define glGetActiveVaryingNV _glGetActiveVaryingNV
#define glGetTransformFeedbackVaryingNV _glGetTransformFeedbackVaryingNV
#define glTransformFeedbackStreamAttribsNV _glTransformFeedbackStreamAttribsNV
#define glBindTransformFeedbackNV _glBindTransformFeedbackNV
#define glDeleteTransformFeedbacksNV _glDeleteTransformFeedbacksNV
#define glGenTransformFeedbacksNV _glGenTransformFeedbacksNV
#define glIsTransformFeedbackNV _glIsTransformFeedbackNV
#define glPauseTransformFeedbackNV _glPauseTransformFeedbackNV
#define glResumeTransformFeedbackNV _glResumeTransformFeedbackNV
#define glDrawTransformFeedbackNV _glDrawTransformFeedbackNV
#define glVDPAUInitNV _glVDPAUInitNV
#define glVDPAUFiniNV _glVDPAUFiniNV
#define glVDPAURegisterVideoSurfaceNV _glVDPAURegisterVideoSurfaceNV
#define glVDPAURegisterOutputSurfaceNV _glVDPAURegisterOutputSurfaceNV
#define glVDPAUIsSurfaceNV _glVDPAUIsSurfaceNV
#define glVDPAUUnregisterSurfaceNV _glVDPAUUnregisterSurfaceNV
#define glVDPAUGetSurfaceivNV _glVDPAUGetSurfaceivNV
#define glVDPAUSurfaceAccessNV _glVDPAUSurfaceAccessNV
#define glVDPAUMapSurfacesNV _glVDPAUMapSurfacesNV
#define glVDPAUUnmapSurfacesNV _glVDPAUUnmapSurfacesNV
#define glFlushVertexArrayRangeNV _glFlushVertexArrayRangeNV
#define glVertexArrayRangeNV _glVertexArrayRangeNV
#define glVertexAttribL1i64NV _glVertexAttribL1i64NV
#define glVertexAttribL2i64NV _glVertexAttribL2i64NV
#define glVertexAttribL3i64NV _glVertexAttribL3i64NV
#define glVertexAttribL4i64NV _glVertexAttribL4i64NV
#define glVertexAttribL1i64vNV _glVertexAttribL1i64vNV
#define glVertexAttribL2i64vNV _glVertexAttribL2i64vNV
#define glVertexAttribL3i64vNV _glVertexAttribL3i64vNV
#define glVertexAttribL4i64vNV _glVertexAttribL4i64vNV
#define glVertexAttribL1ui64NV _glVertexAttribL1ui64NV
#define glVertexAttribL2ui64NV _glVertexAttribL2ui64NV
#define glVertexAttribL3ui64NV _glVertexAttribL3ui64NV
#define glVertexAttribL4ui64NV _glVertexAttribL4ui64NV
#define glVertexAttribL1ui64vNV _glVertexAttribL1ui64vNV
#define glVertexAttribL2ui64vNV _glVertexAttribL2ui64vNV
#define glVertexAttribL3ui64vNV _glVertexAttribL3ui64vNV
#define glVertexAttribL4ui64vNV _glVertexAttribL4ui64vNV
#define glGetVertexAttribLi64vNV _glGetVertexAttribLi64vNV
#define glGetVertexAttribLui64vNV _glGetVertexAttribLui64vNV
#define glVertexAttribLFormatNV _glVertexAttribLFormatNV
#define glBufferAddressRangeNV _glBufferAddressRangeNV
#define glVertexFormatNV _glVertexFormatNV
#define glNormalFormatNV _glNormalFormatNV
#define glColorFormatNV _glColorFormatNV
#define glIndexFormatNV _glIndexFormatNV
#define glTexCoordFormatNV _glTexCoordFormatNV
#define glEdgeFlagFormatNV _glEdgeFlagFormatNV
#define glSecondaryColorFormatNV _glSecondaryColorFormatNV
#define glFogCoordFormatNV _glFogCoordFormatNV
#define glVertexAttribFormatNV _glVertexAttribFormatNV
#define glVertexAttribIFormatNV _glVertexAttribIFormatNV
#define glGetIntegerui64i_vNV _glGetIntegerui64i_vNV
#define glAreProgramsResidentNV _glAreProgramsResidentNV
#define glBindProgramNV _glBindProgramNV
#define glDeleteProgramsNV _glDeleteProgramsNV
#define glExecuteProgramNV _glExecuteProgramNV
#define glGenProgramsNV _glGenProgramsNV
#define glGetProgramParameterdvNV _glGetProgramParameterdvNV
#define glGetProgramParameterfvNV _glGetProgramParameterfvNV
#define glGetProgramivNV _glGetProgramivNV
#define glGetProgramStringNV _glGetProgramStringNV
#define glGetTrackMatrixivNV _glGetTrackMatrixivNV
#define glGetVertexAttribdvNV _glGetVertexAttribdvNV
#define glGetVertexAttribfvNV _glGetVertexAttribfvNV
#define glGetVertexAttribivNV _glGetVertexAttribivNV
#define glGetVertexAttribPointervNV _glGetVertexAttribPointervNV
#define glIsProgramNV _glIsProgramNV
#define glLoadProgramNV _glLoadProgramNV
#define glProgramParameter4dNV _glProgramParameter4dNV
#define glProgramParameter4dvNV _glProgramParameter4dvNV
#define glProgramParameter4fNV _glProgramParameter4fNV
#define glProgramParameter4fvNV _glProgramParameter4fvNV
#define glProgramParameters4dvNV _glProgramParameters4dvNV
#define glProgramParameters4fvNV _glProgramParameters4fvNV
#define glRequestResidentProgramsNV _glRequestResidentProgramsNV
#define glTrackMatrixNV _glTrackMatrixNV
#define glVertexAttribPointerNV _glVertexAttribPointerNV
#define glVertexAttrib1dNV _glVertexAttrib1dNV
#define glVertexAttrib1dvNV _glVertexAttrib1dvNV
#define glVertexAttrib1fNV _glVertexAttrib1fNV
#define glVertexAttrib1fvNV _glVertexAttrib1fvNV
#define glVertexAttrib1sNV _glVertexAttrib1sNV
#define glVertexAttrib1svNV _glVertexAttrib1svNV
#define glVertexAttrib2dNV _glVertexAttrib2dNV
#define glVertexAttrib2dvNV _glVertexAttrib2dvNV
#define glVertexAttrib2fNV _glVertexAttrib2fNV
#define glVertexAttrib2fvNV _glVertexAttrib2fvNV
#define glVertexAttrib2sNV _glVertexAttrib2sNV
#define glVertexAttrib2svNV _glVertexAttrib2svNV
#define glVertexAttrib3dNV _glVertexAttrib3dNV
#define glVertexAttrib3dvNV _glVertexAttrib3dvNV
#define glVertexAttrib3fNV _glVertexAttrib3fNV
#define glVertexAttrib3fvNV _glVertexAttrib3fvNV
#define glVertexAttrib3sNV _glVertexAttrib3sNV
#define glVertexAttrib3svNV _glVertexAttrib3svNV
#define glVertexAttrib4dNV _glVertexAttrib4dNV
#define glVertexAttrib4dvNV _glVertexAttrib4dvNV
#define glVertexAttrib4fNV _glVertexAttrib4fNV
#define glVertexAttrib4fvNV _glVertexAttrib4fvNV
#define glVertexAttrib4sNV _glVertexAttrib4sNV
#define glVertexAttrib4svNV _glVertexAttrib4svNV
#define glVertexAttrib4ubNV _glVertexAttrib4ubNV
#define glVertexAttrib4ubvNV _glVertexAttrib4ubvNV
#define glVertexAttribs1dvNV _glVertexAttribs1dvNV
#define glVertexAttribs1fvNV _glVertexAttribs1fvNV
#define glVertexAttribs1svNV _glVertexAttribs1svNV
#define glVertexAttribs2dvNV _glVertexAttribs2dvNV
#define glVertexAttribs2fvNV _glVertexAttribs2fvNV
#define glVertexAttribs2svNV _glVertexAttribs2svNV
#define glVertexAttribs3dvNV _glVertexAttribs3dvNV
#define glVertexAttribs3fvNV _glVertexAttribs3fvNV
#define glVertexAttribs3svNV _glVertexAttribs3svNV
#define glVertexAttribs4dvNV _glVertexAttribs4dvNV
#define glVertexAttribs4fvNV _glVertexAttribs4fvNV
#define glVertexAttribs4svNV _glVertexAttribs4svNV
#define glVertexAttribs4ubvNV _glVertexAttribs4ubvNV
#define glVertexAttribI1iEXT _glVertexAttribI1iEXT
#define glVertexAttribI2iEXT _glVertexAttribI2iEXT
#define glVertexAttribI3iEXT _glVertexAttribI3iEXT
#define glVertexAttribI4iEXT _glVertexAttribI4iEXT
#define glVertexAttribI1uiEXT _glVertexAttribI1uiEXT
#define glVertexAttribI2uiEXT _glVertexAttribI2uiEXT
#define glVertexAttribI3uiEXT _glVertexAttribI3uiEXT
#define glVertexAttribI4uiEXT _glVertexAttribI4uiEXT
#define glVertexAttribI1ivEXT _glVertexAttribI1ivEXT
#define glVertexAttribI2ivEXT _glVertexAttribI2ivEXT
#define glVertexAttribI3ivEXT _glVertexAttribI3ivEXT
#define glVertexAttribI4ivEXT _glVertexAttribI4ivEXT
#define glVertexAttribI1uivEXT _glVertexAttribI1uivEXT
#define glVertexAttribI2uivEXT _glVertexAttribI2uivEXT
#define glVertexAttribI3uivEXT _glVertexAttribI3uivEXT
#define glVertexAttribI4uivEXT _glVertexAttribI4uivEXT
#define glVertexAttribI4bvEXT _glVertexAttribI4bvEXT
#define glVertexAttribI4svEXT _glVertexAttribI4svEXT
#define glVertexAttribI4ubvEXT _glVertexAttribI4ubvEXT
#define glVertexAttribI4usvEXT _glVertexAttribI4usvEXT
#define glVertexAttribIPointerEXT _glVertexAttribIPointerEXT
#define glGetVertexAttribIivEXT _glGetVertexAttribIivEXT
#define glGetVertexAttribIuivEXT _glGetVertexAttribIuivEXT
#define glBeginVideoCaptureNV _glBeginVideoCaptureNV
#define glBindVideoCaptureStreamBufferNV _glBindVideoCaptureStreamBufferNV
#define glBindVideoCaptureStreamTextureNV _glBindVideoCaptureStreamTextureNV
#define glEndVideoCaptureNV _glEndVideoCaptureNV
#define glGetVideoCaptureivNV _glGetVideoCaptureivNV
#define glGetVideoCaptureStreamivNV _glGetVideoCaptureStreamivNV
#define glGetVideoCaptureStreamfvNV _glGetVideoCaptureStreamfvNV
#define glGetVideoCaptureStreamdvNV _glGetVideoCaptureStreamdvNV
#define glVideoCaptureNV _glVideoCaptureNV
#define glVideoCaptureStreamParameterivNV _glVideoCaptureStreamParameterivNV
#define glVideoCaptureStreamParameterfvNV _glVideoCaptureStreamParameterfvNV
#define glVideoCaptureStreamParameterdvNV _glVideoCaptureStreamParameterdvNV
#define glHintPGI _glHintPGI
#define glDetailTexFuncSGIS _glDetailTexFuncSGIS
#define glGetDetailTexFuncSGIS _glGetDetailTexFuncSGIS
#define glFogFuncSGIS _glFogFuncSGIS
#define glGetFogFuncSGIS _glGetFogFuncSGIS
#define glSampleMaskSGIS _glSampleMaskSGIS
#define glSamplePatternSGIS _glSamplePatternSGIS
#define glPixelTexGenParameteriSGIS _glPixelTexGenParameteriSGIS
#define glPixelTexGenParameterivSGIS _glPixelTexGenParameterivSGIS
#define glPixelTexGenParameterfSGIS _glPixelTexGenParameterfSGIS
#define glPixelTexGenParameterfvSGIS _glPixelTexGenParameterfvSGIS
#define glGetPixelTexGenParameterivSGIS _glGetPixelTexGenParameterivSGIS
#define glGetPixelTexGenParameterfvSGIS _glGetPixelTexGenParameterfvSGIS
#define glPointParameterfSGIS _glPointParameterfSGIS
#define glPointParameterfvSGIS _glPointParameterfvSGIS
#define glSharpenTexFuncSGIS _glSharpenTexFuncSGIS
#define glGetSharpenTexFuncSGIS _glGetSharpenTexFuncSGIS
#define glTexImage4DSGIS _glTexImage4DSGIS
#define glTexSubImage4DSGIS _glTexSubImage4DSGIS
#define glTextureColorMaskSGIS _glTextureColorMaskSGIS
#define glGetTexFilterFuncSGIS _glGetTexFilterFuncSGIS
#define glTexFilterFuncSGIS _glTexFilterFuncSGIS
#define glAsyncMarkerSGIX _glAsyncMarkerSGIX
#define glFinishAsyncSGIX _glFinishAsyncSGIX
#define glPollAsyncSGIX _glPollAsyncSGIX
#define glGenAsyncMarkersSGIX _glGenAsyncMarkersSGIX
#define glDeleteAsyncMarkersSGIX _glDeleteAsyncMarkersSGIX
#define glIsAsyncMarkerSGIX _glIsAsyncMarkerSGIX
#define glFlushRasterSGIX _glFlushRasterSGIX
#define glFragmentColorMaterialSGIX _glFragmentColorMaterialSGIX
#define glFragmentLightfSGIX _glFragmentLightfSGIX
#define glFragmentLightfvSGIX _glFragmentLightfvSGIX
#define glFragmentLightiSGIX _glFragmentLightiSGIX
#define glFragmentLightivSGIX _glFragmentLightivSGIX
#define glFragmentLightModelfSGIX _glFragmentLightModelfSGIX
#define glFragmentLightModelfvSGIX _glFragmentLightModelfvSGIX
#define glFragmentLightModeliSGIX _glFragmentLightModeliSGIX
#define glFragmentLightModelivSGIX _glFragmentLightModelivSGIX
#define glFragmentMaterialfSGIX _glFragmentMaterialfSGIX
#define glFragmentMaterialfvSGIX _glFragmentMaterialfvSGIX
#define glFragmentMaterialiSGIX _glFragmentMaterialiSGIX
#define glFragmentMaterialivSGIX _glFragmentMaterialivSGIX
#define glGetFragmentLightfvSGIX _glGetFragmentLightfvSGIX
#define glGetFragmentLightivSGIX _glGetFragmentLightivSGIX
#define glGetFragmentMaterialfvSGIX _glGetFragmentMaterialfvSGIX
#define glGetFragmentMaterialivSGIX _glGetFragmentMaterialivSGIX
#define glLightEnviSGIX _glLightEnviSGIX
#define glFrameZoomSGIX _glFrameZoomSGIX
#define glIglooInterfaceSGIX _glIglooInterfaceSGIX
#define glGetInstrumentsSGIX _glGetInstrumentsSGIX
#define glInstrumentsBufferSGIX _glInstrumentsBufferSGIX
#define glPollInstrumentsSGIX _glPollInstrumentsSGIX
#define glReadInstrumentsSGIX _glReadInstrumentsSGIX
#define glStartInstrumentsSGIX _glStartInstrumentsSGIX
#define glStopInstrumentsSGIX _glStopInstrumentsSGIX
#define glGetListParameterfvSGIX _glGetListParameterfvSGIX
#define glGetListParameterivSGIX _glGetListParameterivSGIX
#define glListParameterfSGIX _glListParameterfSGIX
#define glListParameterfvSGIX _glListParameterfvSGIX
#define glListParameteriSGIX _glListParameteriSGIX
#define glListParameterivSGIX _glListParameterivSGIX
#define glPixelTexGenSGIX _glPixelTexGenSGIX
#define glDeformationMap3dSGIX _glDeformationMap3dSGIX
#define glDeformationMap3fSGIX _glDeformationMap3fSGIX
#define glDeformSGIX _glDeformSGIX
#define glLoadIdentityDeformationMapSGIX _glLoadIdentityDeformationMapSGIX
#define glReferencePlaneSGIX _glReferencePlaneSGIX
#define glSpriteParameterfSGIX _glSpriteParameterfSGIX
#define glSpriteParameterfvSGIX _glSpriteParameterfvSGIX
#define glSpriteParameteriSGIX _glSpriteParameteriSGIX
#define glSpriteParameterivSGIX _glSpriteParameterivSGIX
#define glTagSampleBufferSGIX _glTagSampleBufferSGIX
#define glColorTableSGI _glColorTableSGI
#define glColorTableParameterfvSGI _glColorTableParameterfvSGI
#define glColorTableParameterivSGI _glColorTableParameterivSGI
#define glCopyColorTableSGI _glCopyColorTableSGI
#define glGetColorTableSGI _glGetColorTableSGI
#define glGetColorTableParameterfvSGI _glGetColorTableParameterfvSGI
#define glGetColorTableParameterivSGI _glGetColorTableParameterivSGI
#define glFinishTextureSUNX _glFinishTextureSUNX
#define glGlobalAlphaFactorbSUN _glGlobalAlphaFactorbSUN
#define glGlobalAlphaFactorsSUN _glGlobalAlphaFactorsSUN
#define glGlobalAlphaFactoriSUN _glGlobalAlphaFactoriSUN
#define glGlobalAlphaFactorfSUN _glGlobalAlphaFactorfSUN
#define glGlobalAlphaFactordSUN _glGlobalAlphaFactordSUN
#define glGlobalAlphaFactorubSUN _glGlobalAlphaFactorubSUN
#define glGlobalAlphaFactorusSUN _glGlobalAlphaFactorusSUN
#define glGlobalAlphaFactoruiSUN _glGlobalAlphaFactoruiSUN
#define glDrawMeshArraysSUN _glDrawMeshArraysSUN
#define glReplacementCodeuiSUN _glReplacementCodeuiSUN
#define glReplacementCodeusSUN _glReplacementCodeusSUN
#define glReplacementCodeubSUN _glReplacementCodeubSUN
#define glReplacementCodeuivSUN _glReplacementCodeuivSUN
#define glReplacementCodeusvSUN _glReplacementCodeusvSUN
#define glReplacementCodeubvSUN _glReplacementCodeubvSUN
#define glReplacementCodePointerSUN _glReplacementCodePointerSUN
#define glColor4ubVertex2fSUN _glColor4ubVertex2fSUN
#define glColor4ubVertex2fvSUN _glColor4ubVertex2fvSUN
#define glColor4ubVertex3fSUN _glColor4ubVertex3fSUN
#define glColor4ubVertex3fvSUN _glColor4ubVertex3fvSUN
#define glColor3fVertex3fSUN _glColor3fVertex3fSUN
#define glColor3fVertex3fvSUN _glColor3fVertex3fvSUN
#define glNormal3fVertex3fSUN _glNormal3fVertex3fSUN
#define glNormal3fVertex3fvSUN _glNormal3fVertex3fvSUN
#define glColor4fNormal3fVertex3fSUN _glColor4fNormal3fVertex3fSUN
#define glColor4fNormal3fVertex3fvSUN _glColor4fNormal3fVertex3fvSUN
#define glTexCoord2fVertex3fSUN _glTexCoord2fVertex3fSUN
#define glTexCoord2fVertex3fvSUN _glTexCoord2fVertex3fvSUN
#define glTexCoord4fVertex4fSUN _glTexCoord4fVertex4fSUN
#define glTexCoord4fVertex4fvSUN _glTexCoord4fVertex4fvSUN
#define glTexCoord2fColor4ubVertex3fSUN _glTexCoord2fColor4ubVertex3fSUN
#define glTexCoord2fColor4ubVertex3fvSUN _glTexCoord2fColor4ubVertex3fvSUN
#define glTexCoord2fColor3fVertex3fSUN _glTexCoord2fColor3fVertex3fSUN
#define glTexCoord2fColor3fVertex3fvSUN _glTexCoord2fColor3fVertex3fvSUN
#define glTexCoord2fNormal3fVertex3fSUN _glTexCoord2fNormal3fVertex3fSUN
#define glTexCoord2fNormal3fVertex3fvSUN _glTexCoord2fNormal3fVertex3fvSUN
#define glTexCoord2fColor4fNormal3fVertex3fSUN _glTexCoord2fColor4fNormal3fVertex3fSUN
#define glTexCoord2fColor4fNormal3fVertex3fvSUN _glTexCoord2fColor4fNormal3fVertex3fvSUN
#define glTexCoord4fColor4fNormal3fVertex4fSUN _glTexCoord4fColor4fNormal3fVertex4fSUN
#define glTexCoord4fColor4fNormal3fVertex4fvSUN _glTexCoord4fColor4fNormal3fVertex4fvSUN
#define glReplacementCodeuiVertex3fSUN _glReplacementCodeuiVertex3fSUN
#define glReplacementCodeuiVertex3fvSUN _glReplacementCodeuiVertex3fvSUN
#define glReplacementCodeuiColor4ubVertex3fSUN _glReplacementCodeuiColor4ubVertex3fSUN
#define glReplacementCodeuiColor4ubVertex3fvSUN _glReplacementCodeuiColor4ubVertex3fvSUN
#define glReplacementCodeuiColor3fVertex3fSUN _glReplacementCodeuiColor3fVertex3fSUN
#define glReplacementCodeuiColor3fVertex3fvSUN _glReplacementCodeuiColor3fVertex3fvSUN
#define glReplacementCodeuiNormal3fVertex3fSUN _glReplacementCodeuiNormal3fVertex3fSUN
#define glReplacementCodeuiNormal3fVertex3fvSUN _glReplacementCodeuiNormal3fVertex3fvSUN
#define glReplacementCodeuiColor4fNormal3fVertex3fSUN _glReplacementCodeuiColor4fNormal3fVertex3fSUN
#define glReplacementCodeuiColor4fNormal3fVertex3fvSUN _glReplacementCodeuiColor4fNormal3fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fVertex3fSUN _glReplacementCodeuiTexCoord2fVertex3fSUN
#define glReplacementCodeuiTexCoord2fVertex3fvSUN _glReplacementCodeuiTexCoord2fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN _glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN _glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN _glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN _glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN
#define glAddSwapHintRectWIN _glAddSwapHintRectWIN
#endif /* RETRACE */


typedef void (GL_APIENTRY * PFN_GLFRUSTUMF)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern PFN_GLFRUSTUMF _glFrustumf;

typedef void (GL_APIENTRY * PFN_GLORTHOF)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern PFN_GLORTHOF _glOrthof;

typedef void (GL_APIENTRY * PFN_GLCLIPPLANEF)(GLenum plane, const GLfloat * equation);
extern PFN_GLCLIPPLANEF _glClipPlanef;

typedef void (GL_APIENTRY * PFN_GLGETCLIPPLANEF)(GLenum plane, GLfloat * equation);
extern PFN_GLGETCLIPPLANEF _glGetClipPlanef;

typedef void (GL_APIENTRY * PFN_GLALPHAFUNCX)(GLenum func, GLclampx ref);
extern PFN_GLALPHAFUNCX _glAlphaFuncx;

typedef void (GL_APIENTRY * PFN_GLCLEARCOLORX)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
extern PFN_GLCLEARCOLORX _glClearColorx;

typedef void (GL_APIENTRY * PFN_GLCLEARDEPTHX)(GLclampx depth);
extern PFN_GLCLEARDEPTHX _glClearDepthx;

typedef void (GL_APIENTRY * PFN_GLCOLOR4X)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
extern PFN_GLCOLOR4X _glColor4x;

typedef void (GL_APIENTRY * PFN_GLDEPTHRANGEX)(GLclampx zNear, GLclampx zFar);
extern PFN_GLDEPTHRANGEX _glDepthRangex;

typedef void (GL_APIENTRY * PFN_GLFOGX)(GLenum pname, GLfixed param);
extern PFN_GLFOGX _glFogx;

typedef void (GL_APIENTRY * PFN_GLFOGXV)(GLenum pname, const GLfixed * params);
extern PFN_GLFOGXV _glFogxv;

typedef void (GL_APIENTRY * PFN_GLFRUSTUMX)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern PFN_GLFRUSTUMX _glFrustumx;

typedef void (GL_APIENTRY * PFN_GLLIGHTMODELX)(GLenum pname, GLfixed param);
extern PFN_GLLIGHTMODELX _glLightModelx;

typedef void (GL_APIENTRY * PFN_GLLIGHTMODELXV)(GLenum pname, const GLfixed * params);
extern PFN_GLLIGHTMODELXV _glLightModelxv;

typedef void (GL_APIENTRY * PFN_GLLIGHTX)(GLenum light, GLenum pname, GLfixed param);
extern PFN_GLLIGHTX _glLightx;

typedef void (GL_APIENTRY * PFN_GLLIGHTXV)(GLenum light, GLenum pname, const GLfixed * params);
extern PFN_GLLIGHTXV _glLightxv;

typedef void (GL_APIENTRY * PFN_GLLINEWIDTHX)(GLfixed width);
extern PFN_GLLINEWIDTHX _glLineWidthx;

typedef void (GL_APIENTRY * PFN_GLLOADMATRIXX)(const GLfixed * m);
extern PFN_GLLOADMATRIXX _glLoadMatrixx;

typedef void (GL_APIENTRY * PFN_GLMATERIALX)(GLenum face, GLenum pname, GLfixed param);
extern PFN_GLMATERIALX _glMaterialx;

typedef void (GL_APIENTRY * PFN_GLMATERIALXV)(GLenum face, GLenum pname, const GLfixed * params);
extern PFN_GLMATERIALXV _glMaterialxv;

typedef void (GL_APIENTRY * PFN_GLMULTMATRIXX)(const GLfixed * m);
extern PFN_GLMULTMATRIXX _glMultMatrixx;

typedef void (GL_APIENTRY * PFN_GLMULTITEXCOORD4X)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
extern PFN_GLMULTITEXCOORD4X _glMultiTexCoord4x;

typedef void (GL_APIENTRY * PFN_GLNORMAL3X)(GLfixed nx, GLfixed ny, GLfixed nz);
extern PFN_GLNORMAL3X _glNormal3x;

typedef void (GL_APIENTRY * PFN_GLORTHOX)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern PFN_GLORTHOX _glOrthox;

typedef void (GL_APIENTRY * PFN_GLPOINTSIZEX)(GLfixed size);
extern PFN_GLPOINTSIZEX _glPointSizex;

typedef void (GL_APIENTRY * PFN_GLPOLYGONOFFSETX)(GLfixed factor, GLfixed units);
extern PFN_GLPOLYGONOFFSETX _glPolygonOffsetx;

typedef void (GL_APIENTRY * PFN_GLROTATEX)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
extern PFN_GLROTATEX _glRotatex;

typedef void (GL_APIENTRY * PFN_GLSAMPLECOVERAGEX)(GLclampx value, GLboolean invert);
extern PFN_GLSAMPLECOVERAGEX _glSampleCoveragex;

typedef void (GL_APIENTRY * PFN_GLSCALEX)(GLfixed x, GLfixed y, GLfixed z);
extern PFN_GLSCALEX _glScalex;

typedef void (GL_APIENTRY * PFN_GLTEXENVX)(GLenum target, GLenum pname, GLfixed param);
extern PFN_GLTEXENVX _glTexEnvx;

typedef void (GL_APIENTRY * PFN_GLTEXENVXV)(GLenum target, GLenum pname, const GLfixed * params);
extern PFN_GLTEXENVXV _glTexEnvxv;

typedef void (GL_APIENTRY * PFN_GLTEXPARAMETERX)(GLenum target, GLenum pname, GLfixed param);
extern PFN_GLTEXPARAMETERX _glTexParameterx;

typedef void (GL_APIENTRY * PFN_GLTRANSLATEX)(GLfixed x, GLfixed y, GLfixed z);
extern PFN_GLTRANSLATEX _glTranslatex;

typedef void (GL_APIENTRY * PFN_GLCLIPPLANEX)(GLenum plane, const GLfixed * equation);
extern PFN_GLCLIPPLANEX _glClipPlanex;

typedef void (GL_APIENTRY * PFN_GLGETCLIPPLANEX)(GLenum plane, GLfixed * equation);
extern PFN_GLGETCLIPPLANEX _glGetClipPlanex;

typedef void (GL_APIENTRY * PFN_GLGETFIXEDV)(GLenum pname, GLfixed * params);
extern PFN_GLGETFIXEDV _glGetFixedv;

typedef void (GL_APIENTRY * PFN_GLGETLIGHTXV)(GLenum light, GLenum pname, GLfixed * params);
extern PFN_GLGETLIGHTXV _glGetLightxv;

typedef void (GL_APIENTRY * PFN_GLGETMATERIALXV)(GLenum face, GLenum pname, GLfixed * params);
extern PFN_GLGETMATERIALXV _glGetMaterialxv;

typedef void (GL_APIENTRY * PFN_GLGETTEXENVXV)(GLenum target, GLenum pname, GLfixed * params);
extern PFN_GLGETTEXENVXV _glGetTexEnvxv;

typedef void (GL_APIENTRY * PFN_GLGETTEXPARAMETERXV)(GLenum target, GLenum pname, GLfixed * params);
extern PFN_GLGETTEXPARAMETERXV _glGetTexParameterxv;

typedef void (GL_APIENTRY * PFN_GLPOINTPARAMETERX)(GLenum pname, GLfixed param);
extern PFN_GLPOINTPARAMETERX _glPointParameterx;

typedef void (GL_APIENTRY * PFN_GLPOINTPARAMETERXV)(GLenum pname, const GLfixed * params);
extern PFN_GLPOINTPARAMETERXV _glPointParameterxv;

typedef void (GL_APIENTRY * PFN_GLTEXPARAMETERXV)(GLenum target, GLenum pname, const GLfixed * params);
extern PFN_GLTEXPARAMETERXV _glTexParameterxv;

typedef void (GL_APIENTRY * PFN_GLBLENDEQUATIONSEPARATEOES)(GLenum modeRGB, GLenum modeAlpha);
extern PFN_GLBLENDEQUATIONSEPARATEOES _glBlendEquationSeparateOES;

typedef void (GL_APIENTRY * PFN_GLBLENDFUNCSEPARATEOES)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern PFN_GLBLENDFUNCSEPARATEOES _glBlendFuncSeparateOES;

typedef void (GL_APIENTRY * PFN_GLBLENDEQUATIONOES)(GLenum mode);
extern PFN_GLBLENDEQUATIONOES _glBlendEquationOES;

typedef GLboolean (GL_APIENTRY * PFN_GLISRENDERBUFFEROES)(GLuint renderbuffer);
extern PFN_GLISRENDERBUFFEROES _glIsRenderbufferOES;

typedef void (GL_APIENTRY * PFN_GLBINDRENDERBUFFEROES)(GLenum target, GLuint renderbuffer);
extern PFN_GLBINDRENDERBUFFEROES _glBindRenderbufferOES;

typedef void (GL_APIENTRY * PFN_GLDELETERENDERBUFFERSOES)(GLsizei n, const GLuint * renderbuffers);
extern PFN_GLDELETERENDERBUFFERSOES _glDeleteRenderbuffersOES;

typedef void (GL_APIENTRY * PFN_GLGENRENDERBUFFERSOES)(GLsizei n, GLuint * renderbuffers);
extern PFN_GLGENRENDERBUFFERSOES _glGenRenderbuffersOES;

typedef void (GL_APIENTRY * PFN_GLRENDERBUFFERSTORAGEOES)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEOES _glRenderbufferStorageOES;

typedef void (GL_APIENTRY * PFN_GLGETRENDERBUFFERPARAMETERIVOES)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETRENDERBUFFERPARAMETERIVOES _glGetRenderbufferParameterivOES;

typedef GLboolean (GL_APIENTRY * PFN_GLISFRAMEBUFFEROES)(GLuint framebuffer);
extern PFN_GLISFRAMEBUFFEROES _glIsFramebufferOES;

typedef void (GL_APIENTRY * PFN_GLBINDFRAMEBUFFEROES)(GLenum target, GLuint framebuffer);
extern PFN_GLBINDFRAMEBUFFEROES _glBindFramebufferOES;

typedef void (GL_APIENTRY * PFN_GLDELETEFRAMEBUFFERSOES)(GLsizei n, const GLuint * framebuffers);
extern PFN_GLDELETEFRAMEBUFFERSOES _glDeleteFramebuffersOES;

typedef void (GL_APIENTRY * PFN_GLGENFRAMEBUFFERSOES)(GLsizei n, GLuint * framebuffers);
extern PFN_GLGENFRAMEBUFFERSOES _glGenFramebuffersOES;

typedef GLenum (GL_APIENTRY * PFN_GLCHECKFRAMEBUFFERSTATUSOES)(GLenum target);
extern PFN_GLCHECKFRAMEBUFFERSTATUSOES _glCheckFramebufferStatusOES;

typedef void (GL_APIENTRY * PFN_GLFRAMEBUFFERTEXTURE2DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFN_GLFRAMEBUFFERTEXTURE2DOES _glFramebufferTexture2DOES;

typedef void (GL_APIENTRY * PFN_GLFRAMEBUFFERRENDERBUFFEROES)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFN_GLFRAMEBUFFERRENDERBUFFEROES _glFramebufferRenderbufferOES;

typedef void (GL_APIENTRY * PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
extern PFN_GLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES _glGetFramebufferAttachmentParameterivOES;

typedef void (GL_APIENTRY * PFN_GLGENERATEMIPMAPOES)(GLenum target);
extern PFN_GLGENERATEMIPMAPOES _glGenerateMipmapOES;

typedef void (GL_APIENTRY * PFN_GLCURRENTPALETTEMATRIXOES)(GLuint index);
extern PFN_GLCURRENTPALETTEMATRIXOES _glCurrentPaletteMatrixOES;

typedef void (GL_APIENTRY * PFN_GLLOADPALETTEFROMMODELVIEWMATRIXOES)(void);
extern PFN_GLLOADPALETTEFROMMODELVIEWMATRIXOES _glLoadPaletteFromModelViewMatrixOES;

typedef void (GL_APIENTRY * PFN_GLMATRIXINDEXPOINTEROES)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLMATRIXINDEXPOINTEROES _glMatrixIndexPointerOES;

typedef void (GL_APIENTRY * PFN_GLWEIGHTPOINTEROES)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
extern PFN_GLWEIGHTPOINTEROES _glWeightPointerOES;

typedef void (GL_APIENTRY * PFN_GLPOINTSIZEPOINTEROES)(GLenum type, GLsizei stride, const GLvoid * ptr);
extern PFN_GLPOINTSIZEPOINTEROES _glPointSizePointerOES;

typedef GLbitfield (GL_APIENTRY * PFN_GLQUERYMATRIXXOES)(GLfixed * mantissa, GLint * exponent);
extern PFN_GLQUERYMATRIXXOES _glQueryMatrixxOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENFOES)(GLenum coord, GLenum pname, GLfloat param);
extern PFN_GLTEXGENFOES _glTexGenfOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENFVOES)(GLenum coord, GLenum pname, const GLfloat * params);
extern PFN_GLTEXGENFVOES _glTexGenfvOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENIOES)(GLenum coord, GLenum pname, GLint param);
extern PFN_GLTEXGENIOES _glTexGeniOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENIVOES)(GLenum coord, GLenum pname, const GLint * params);
extern PFN_GLTEXGENIVOES _glTexGenivOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENXOES)(GLenum coord, GLenum pname, GLfixed param);
extern PFN_GLTEXGENXOES _glTexGenxOES;

typedef void (GL_APIENTRY * PFN_GLTEXGENXVOES)(GLenum coord, GLenum pname, const GLfixed * params);
extern PFN_GLTEXGENXVOES _glTexGenxvOES;

typedef void (GL_APIENTRY * PFN_GLGETTEXGENFVOES)(GLenum coord, GLenum pname, GLfloat * params);
extern PFN_GLGETTEXGENFVOES _glGetTexGenfvOES;

typedef void (GL_APIENTRY * PFN_GLGETTEXGENIVOES)(GLenum coord, GLenum pname, GLint * params);
extern PFN_GLGETTEXGENIVOES _glGetTexGenivOES;

typedef void (GL_APIENTRY * PFN_GLGETTEXGENXVOES)(GLenum coord, GLenum pname, GLfixed * params);
extern PFN_GLGETTEXGENXVOES _glGetTexGenxvOES;

typedef void (GL_APIENTRY * PFN_GLGETBUFFERPOINTERVOES)(GLenum target, GLenum pname, GLvoid * * params);
extern PFN_GLGETBUFFERPOINTERVOES _glGetBufferPointervOES;

typedef GLvoid * (GL_APIENTRY * PFN_GLMAPBUFFEROES)(GLenum target, GLenum access);
extern PFN_GLMAPBUFFEROES _glMapBufferOES;

typedef GLboolean (GL_APIENTRY * PFN_GLUNMAPBUFFEROES)(GLenum target);
extern PFN_GLUNMAPBUFFEROES _glUnmapBufferOES;

typedef void (GL_APIENTRY * PFN_GLTEXIMAGE3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXIMAGE3DOES _glTexImage3DOES;

typedef void (GL_APIENTRY * PFN_GLTEXSUBIMAGE3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
extern PFN_GLTEXSUBIMAGE3DOES _glTexSubImage3DOES;

typedef void (GL_APIENTRY * PFN_GLCOPYTEXSUBIMAGE3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFN_GLCOPYTEXSUBIMAGE3DOES _glCopyTexSubImage3DOES;

typedef void (GL_APIENTRY * PFN_GLCOMPRESSEDTEXIMAGE3DOES)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXIMAGE3DOES _glCompressedTexImage3DOES;

typedef void (GL_APIENTRY * PFN_GLCOMPRESSEDTEXSUBIMAGE3DOES)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
extern PFN_GLCOMPRESSEDTEXSUBIMAGE3DOES _glCompressedTexSubImage3DOES;

typedef void (GL_APIENTRY * PFN_GLFRAMEBUFFERTEXTURE3DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern PFN_GLFRAMEBUFFERTEXTURE3DOES _glFramebufferTexture3DOES;

typedef void (GL_APIENTRY * PFN_GLGETPROGRAMBINARYOES)(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, GLvoid * binary);
extern PFN_GLGETPROGRAMBINARYOES _glGetProgramBinaryOES;

typedef void (GL_APIENTRY * PFN_GLPROGRAMBINARYOES)(GLuint program, GLenum binaryFormat, const GLvoid * binary, GLsizei length);
extern PFN_GLPROGRAMBINARYOES _glProgramBinaryOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXFOES)(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
extern PFN_GLDRAWTEXFOES _glDrawTexfOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXFVOES)(const GLfloat * coords);
extern PFN_GLDRAWTEXFVOES _glDrawTexfvOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXIOES)(GLint x, GLint y, GLint z, GLint width, GLint height);
extern PFN_GLDRAWTEXIOES _glDrawTexiOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXIVOES)(const GLint * coords);
extern PFN_GLDRAWTEXIVOES _glDrawTexivOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXSOES)(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);
extern PFN_GLDRAWTEXSOES _glDrawTexsOES;

typedef void (GL_APIENTRY * PFN_GLDRAWTEXSVOES)(const GLshort * coords);
extern PFN_GLDRAWTEXSVOES _glDrawTexsvOES;

typedef void (GL_APIENTRY * PFN_GLBINDVERTEXARRAYOES)(GLuint array);
extern PFN_GLBINDVERTEXARRAYOES _glBindVertexArrayOES;

typedef void (GL_APIENTRY * PFN_GLDELETEVERTEXARRAYSOES)(GLsizei n, const GLuint * arrays);
extern PFN_GLDELETEVERTEXARRAYSOES _glDeleteVertexArraysOES;

typedef void (GL_APIENTRY * PFN_GLGENVERTEXARRAYSOES)(GLsizei n, GLuint * arrays);
extern PFN_GLGENVERTEXARRAYSOES _glGenVertexArraysOES;

typedef GLboolean (GL_APIENTRY * PFN_GLISVERTEXARRAYOES)(GLuint array);
extern PFN_GLISVERTEXARRAYOES _glIsVertexArrayOES;

typedef void (GL_APIENTRY * PFN_GLCOVERAGEMASKNV)(GLboolean mask);
extern PFN_GLCOVERAGEMASKNV _glCoverageMaskNV;

typedef void (GL_APIENTRY * PFN_GLCOVERAGEOPERATIONNV)(GLenum operation);
extern PFN_GLCOVERAGEOPERATIONNV _glCoverageOperationNV;

typedef void (GL_APIENTRY * PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEIMG)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLRENDERBUFFERSTORAGEMULTISAMPLEIMG _glRenderbufferStorageMultisampleIMG;

typedef void (GL_APIENTRY * PFN_GLFRAMEBUFFERTEXTURE2DMULTISAMPLEIMG)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
extern PFN_GLFRAMEBUFFERTEXTURE2DMULTISAMPLEIMG _glFramebufferTexture2DMultisampleIMG;

typedef void (GL_APIENTRY * PFN_GLDRAWBUFFERSNV)(GLsizei n, const GLenum * bufs);
extern PFN_GLDRAWBUFFERSNV _glDrawBuffersNV;

typedef void (GL_APIENTRY * PFN_GLREADBUFFERNV)(GLenum mode);
extern PFN_GLREADBUFFERNV _glReadBufferNV;

typedef void (GL_APIENTRY * PFN_GLGENQUERIESEXT)(GLsizei n, GLuint * ids);
extern PFN_GLGENQUERIESEXT _glGenQueriesEXT;

typedef void (GL_APIENTRY * PFN_GLDELETEQUERIESEXT)(GLsizei n, const GLuint * ids);
extern PFN_GLDELETEQUERIESEXT _glDeleteQueriesEXT;

typedef GLboolean (GL_APIENTRY * PFN_GLISQUERYEXT)(GLuint id);
extern PFN_GLISQUERYEXT _glIsQueryEXT;

typedef void (GL_APIENTRY * PFN_GLBEGINQUERYEXT)(GLenum target, GLuint id);
extern PFN_GLBEGINQUERYEXT _glBeginQueryEXT;

typedef void (GL_APIENTRY * PFN_GLENDQUERYEXT)(GLenum target);
extern PFN_GLENDQUERYEXT _glEndQueryEXT;

typedef void (GL_APIENTRY * PFN_GLGETQUERYIVEXT)(GLenum target, GLenum pname, GLint * params);
extern PFN_GLGETQUERYIVEXT _glGetQueryivEXT;

typedef void (GL_APIENTRY * PFN_GLGETQUERYOBJECTUIVEXT)(GLuint id, GLenum pname, GLuint * params);
extern PFN_GLGETQUERYOBJECTUIVEXT _glGetQueryObjectuivEXT;

typedef void (GL_APIENTRY * PFN_GLUSEPROGRAMSTAGESEXT)(GLuint pipeline, GLbitfield stages, GLuint program);
extern PFN_GLUSEPROGRAMSTAGESEXT _glUseProgramStagesEXT;

typedef void (GL_APIENTRY * PFN_GLACTIVESHADERPROGRAMEXT)(GLuint pipeline, GLuint program);
extern PFN_GLACTIVESHADERPROGRAMEXT _glActiveShaderProgramEXT;

typedef GLuint (GL_APIENTRY * PFN_GLCREATESHADERPROGRAMVEXT)(GLenum type, GLsizei count, const GLchar * * const strings);
extern PFN_GLCREATESHADERPROGRAMVEXT _glCreateShaderProgramvEXT;

typedef void (GL_APIENTRY * PFN_GLBINDPROGRAMPIPELINEEXT)(GLuint pipeline);
extern PFN_GLBINDPROGRAMPIPELINEEXT _glBindProgramPipelineEXT;

typedef void (GL_APIENTRY * PFN_GLDELETEPROGRAMPIPELINESEXT)(GLsizei n, const GLuint * pipelines);
extern PFN_GLDELETEPROGRAMPIPELINESEXT _glDeleteProgramPipelinesEXT;

typedef void (GL_APIENTRY * PFN_GLGENPROGRAMPIPELINESEXT)(GLsizei n, GLuint * pipelines);
extern PFN_GLGENPROGRAMPIPELINESEXT _glGenProgramPipelinesEXT;

typedef GLboolean (GL_APIENTRY * PFN_GLISPROGRAMPIPELINEEXT)(GLuint pipeline);
extern PFN_GLISPROGRAMPIPELINEEXT _glIsProgramPipelineEXT;

typedef void (GL_APIENTRY * PFN_GLGETPROGRAMPIPELINEIVEXT)(GLuint pipeline, GLenum pname, GLint * params);
extern PFN_GLGETPROGRAMPIPELINEIVEXT _glGetProgramPipelineivEXT;

typedef void (GL_APIENTRY * PFN_GLVALIDATEPROGRAMPIPELINEEXT)(GLuint pipeline);
extern PFN_GLVALIDATEPROGRAMPIPELINEEXT _glValidateProgramPipelineEXT;

typedef void (GL_APIENTRY * PFN_GLGETPROGRAMPIPELINEINFOLOGEXT)(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
extern PFN_GLGETPROGRAMPIPELINEINFOLOGEXT _glGetProgramPipelineInfoLogEXT;

typedef void (GL_APIENTRY * PFN_GLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
extern PFN_GLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXT _glFramebufferTexture2DMultisampleEXT;

typedef void (GL_APIENTRY * PFN_GLTEXSTORAGE1DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
extern PFN_GLTEXSTORAGE1DEXT _glTexStorage1DEXT;

typedef void (GL_APIENTRY * PFN_GLTEXSTORAGE2DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern PFN_GLTEXSTORAGE2DEXT _glTexStorage2DEXT;

typedef void (GL_APIENTRY * PFN_GLTEXSTORAGE3DEXT)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern PFN_GLTEXSTORAGE3DEXT _glTexStorage3DEXT;

#ifdef RETRACE
#define glFrustumf _glFrustumf
#define glOrthof _glOrthof
#define glClipPlanef _glClipPlanef
#define glGetClipPlanef _glGetClipPlanef
#define glAlphaFuncx _glAlphaFuncx
#define glClearColorx _glClearColorx
#define glClearDepthx _glClearDepthx
#define glColor4x _glColor4x
#define glDepthRangex _glDepthRangex
#define glFogx _glFogx
#define glFogxv _glFogxv
#define glFrustumx _glFrustumx
#define glLightModelx _glLightModelx
#define glLightModelxv _glLightModelxv
#define glLightx _glLightx
#define glLightxv _glLightxv
#define glLineWidthx _glLineWidthx
#define glLoadMatrixx _glLoadMatrixx
#define glMaterialx _glMaterialx
#define glMaterialxv _glMaterialxv
#define glMultMatrixx _glMultMatrixx
#define glMultiTexCoord4x _glMultiTexCoord4x
#define glNormal3x _glNormal3x
#define glOrthox _glOrthox
#define glPointSizex _glPointSizex
#define glPolygonOffsetx _glPolygonOffsetx
#define glRotatex _glRotatex
#define glSampleCoveragex _glSampleCoveragex
#define glScalex _glScalex
#define glTexEnvx _glTexEnvx
#define glTexEnvxv _glTexEnvxv
#define glTexParameterx _glTexParameterx
#define glTranslatex _glTranslatex
#define glClipPlanex _glClipPlanex
#define glGetClipPlanex _glGetClipPlanex
#define glGetFixedv _glGetFixedv
#define glGetLightxv _glGetLightxv
#define glGetMaterialxv _glGetMaterialxv
#define glGetTexEnvxv _glGetTexEnvxv
#define glGetTexParameterxv _glGetTexParameterxv
#define glPointParameterx _glPointParameterx
#define glPointParameterxv _glPointParameterxv
#define glTexParameterxv _glTexParameterxv
#define glBlendEquationSeparateOES _glBlendEquationSeparateOES
#define glBlendFuncSeparateOES _glBlendFuncSeparateOES
#define glBlendEquationOES _glBlendEquationOES
#define glIsRenderbufferOES _glIsRenderbufferOES
#define glBindRenderbufferOES _glBindRenderbufferOES
#define glDeleteRenderbuffersOES _glDeleteRenderbuffersOES
#define glGenRenderbuffersOES _glGenRenderbuffersOES
#define glRenderbufferStorageOES _glRenderbufferStorageOES
#define glGetRenderbufferParameterivOES _glGetRenderbufferParameterivOES
#define glIsFramebufferOES _glIsFramebufferOES
#define glBindFramebufferOES _glBindFramebufferOES
#define glDeleteFramebuffersOES _glDeleteFramebuffersOES
#define glGenFramebuffersOES _glGenFramebuffersOES
#define glCheckFramebufferStatusOES _glCheckFramebufferStatusOES
#define glFramebufferTexture2DOES _glFramebufferTexture2DOES
#define glFramebufferRenderbufferOES _glFramebufferRenderbufferOES
#define glGetFramebufferAttachmentParameterivOES _glGetFramebufferAttachmentParameterivOES
#define glGenerateMipmapOES _glGenerateMipmapOES
#define glCurrentPaletteMatrixOES _glCurrentPaletteMatrixOES
#define glLoadPaletteFromModelViewMatrixOES _glLoadPaletteFromModelViewMatrixOES
#define glMatrixIndexPointerOES _glMatrixIndexPointerOES
#define glWeightPointerOES _glWeightPointerOES
#define glPointSizePointerOES _glPointSizePointerOES
#define glQueryMatrixxOES _glQueryMatrixxOES
#define glTexGenfOES _glTexGenfOES
#define glTexGenfvOES _glTexGenfvOES
#define glTexGeniOES _glTexGeniOES
#define glTexGenivOES _glTexGenivOES
#define glTexGenxOES _glTexGenxOES
#define glTexGenxvOES _glTexGenxvOES
#define glGetTexGenfvOES _glGetTexGenfvOES
#define glGetTexGenivOES _glGetTexGenivOES
#define glGetTexGenxvOES _glGetTexGenxvOES
#define glGetBufferPointervOES _glGetBufferPointervOES
#define glMapBufferOES _glMapBufferOES
#define glUnmapBufferOES _glUnmapBufferOES
#define glTexImage3DOES _glTexImage3DOES
#define glTexSubImage3DOES _glTexSubImage3DOES
#define glCopyTexSubImage3DOES _glCopyTexSubImage3DOES
#define glCompressedTexImage3DOES _glCompressedTexImage3DOES
#define glCompressedTexSubImage3DOES _glCompressedTexSubImage3DOES
#define glFramebufferTexture3DOES _glFramebufferTexture3DOES
#define glGetProgramBinaryOES _glGetProgramBinaryOES
#define glProgramBinaryOES _glProgramBinaryOES
#define glDrawTexfOES _glDrawTexfOES
#define glDrawTexfvOES _glDrawTexfvOES
#define glDrawTexiOES _glDrawTexiOES
#define glDrawTexivOES _glDrawTexivOES
#define glDrawTexsOES _glDrawTexsOES
#define glDrawTexsvOES _glDrawTexsvOES
#define glBindVertexArrayOES _glBindVertexArrayOES
#define glDeleteVertexArraysOES _glDeleteVertexArraysOES
#define glGenVertexArraysOES _glGenVertexArraysOES
#define glIsVertexArrayOES _glIsVertexArrayOES
#define glCoverageMaskNV _glCoverageMaskNV
#define glCoverageOperationNV _glCoverageOperationNV
#define glRenderbufferStorageMultisampleIMG _glRenderbufferStorageMultisampleIMG
#define glFramebufferTexture2DMultisampleIMG _glFramebufferTexture2DMultisampleIMG
#define glDrawBuffersNV _glDrawBuffersNV
#define glReadBufferNV _glReadBufferNV
#define glGenQueriesEXT _glGenQueriesEXT
#define glDeleteQueriesEXT _glDeleteQueriesEXT
#define glIsQueryEXT _glIsQueryEXT
#define glBeginQueryEXT _glBeginQueryEXT
#define glEndQueryEXT _glEndQueryEXT
#define glGetQueryivEXT _glGetQueryivEXT
#define glGetQueryObjectuivEXT _glGetQueryObjectuivEXT
#define glUseProgramStagesEXT _glUseProgramStagesEXT
#define glActiveShaderProgramEXT _glActiveShaderProgramEXT
#define glCreateShaderProgramvEXT _glCreateShaderProgramvEXT
#define glBindProgramPipelineEXT _glBindProgramPipelineEXT
#define glDeleteProgramPipelinesEXT _glDeleteProgramPipelinesEXT
#define glGenProgramPipelinesEXT _glGenProgramPipelinesEXT
#define glIsProgramPipelineEXT _glIsProgramPipelineEXT
#define glGetProgramPipelineivEXT _glGetProgramPipelineivEXT
#define glValidateProgramPipelineEXT _glValidateProgramPipelineEXT
#define glGetProgramPipelineInfoLogEXT _glGetProgramPipelineInfoLogEXT
#define glFramebufferTexture2DMultisampleEXT _glFramebufferTexture2DMultisampleEXT
#define glTexStorage1DEXT _glTexStorage1DEXT
#define glTexStorage2DEXT _glTexStorage2DEXT
#define glTexStorage3DEXT _glTexStorage3DEXT
#endif /* RETRACE */


#endif /* !_GLPROC_HPP_ */

