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

#include <map>
using namespace std;

#include "RegalLog.h"
#include "RegalInit.h"
#include "RegalConfig.h"
#include "RegalHttp.h"
#include "RegalToken.h"
#include "RegalDispatcher.h"
#include "RegalPrivate.h"
#include "RegalContextInfo.h"

RegalErrorCallback RegalSetErrorCallback(RegalErrorCallback callback)
{
  ::REGAL_NAMESPACE_INTERNAL::RegalContext * ctx = GET_REGAL_CONTEXT();
  RegalAssert(ctx);
  return ctx->err.callback = callback;
}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using Token::toString;

static ::REGAL_NAMESPACE_INTERNAL::Init *_init = NULL;

#if !defined(REGAL_NAMESPACE) && REGAL_SYS_WGL
// Phony advapi32.dll, gdi32.dll and user32.dll dependencies for
// closely matching opengl32.dll

extern "C" { void __stdcall RegCloseKey(void *); }
extern "C" { void __stdcall DeleteDC   (void *); }
extern "C" { void __stdcall GetFocus   (void); }
extern "C" { static void (__stdcall * myRegCloseKey)(void *) = RegCloseKey; }
extern "C" { static void (__stdcall * myDeleteDC   )(void *) = DeleteDC;    }
extern "C" { static void (__stdcall * myGetFocus   )(void  ) = GetFocus;    }
#endif

Init::Init()
{
  atexit(atExit);

#if !defined(REGAL_NAMESPACE) && REGAL_SYS_WGL
  // Check our phony advapi32.dll, gdi32.dll and user32.dll dependencies
  // to prevent them being optimized out of a release-mode binary.
  // NOTE - these function pointers should _not_ be called, ever!

  if (!myRegCloseKey || !myDeleteDC || !myGetFocus)
    return;
#endif

  Logging::Init();
  Config::Init();
  Http::Init();

  Http::Start();
}

Init::~Init()
{
  Http::Stop();
  Logging::Cleanup();
}

void
Init::init()
{
  if (!_init)
    _init = new ::REGAL_NAMESPACE_INTERNAL::Init();
}

void
Init::atExit()
{
  if (_init)
  {
    delete _init;
    _init = NULL;
  }
}

#if REGAL_SYS_WGL
extern "C" { DWORD __stdcall GetCurrentThreadId(void); }
#endif

// Single-threaded RegalContext

RegalContext *currentContext = NULL;

#if REGAL_NO_TLS
inline Thread RegalPrivateThreadSelf() { return 1; }
#else

#if REGAL_SYS_WGL
#if REGAL_WIN_TLS
    DWORD regalCurrentContextTLSIDX = DWORD(~0);
    struct RegalPrivateTlsInit
    {
        RegalPrivateTlsInit()
        {
            regalCurrentContextTLSIDX = TlsAlloc();
        }
        ~RegalPrivateTlsInit()
        {
            TlsFree( regalCurrentContextTLSIDX );
        }
    };
    RegalPrivateTlsInit regalPrivateTlsInit;
#else
    __declspec( thread ) void * regalCurrentContext = NULL;
#endif

    inline Thread RegalPrivateThreadSelf()
    {
        return GetCurrentThreadId();
    }
#else
    pthread_key_t regalPrivateCurrentContextKey = 0;

    struct RegalPrivateTlsInit {
        RegalPrivateTlsInit()
        {
            pthread_key_create( &regalPrivateCurrentContextKey, NULL );
        }
    };
    RegalPrivateTlsInit regalPrivateTlsInit;

    inline Thread RegalPrivateThreadSelf()
    {
        return pthread_self();
    }

#endif
#endif

static void SetContextInTls(RegalContext * ctx) {
#if REGAL_NO_TLS
  currentContext = ctx;
#else
#if REGAL_SYS_WGL
#if REGAL_WIN_TLS
  if (regalCurrentContextTLSIDX == ~0)
    regalCurrentContextTLSIDX = TlsAlloc();
  TlsSetValue( regalCurrentContextTLSIDX, ctx );
#else
  regalCurrentContext = ctx;
#endif
#else
  if (regalPrivateCurrentContextKey == 0) {
    pthread_key_create( & regalPrivateCurrentContextKey, NULL );
  }
  pthread_setspecific( regalPrivateCurrentContextKey, ctx );
#endif
#endif
}

map<RegalSystemContext, RegalContext *> sc2rc;
map<Thread, RegalContext *> th2rc;

void RegalCheckForGLErrors( RegalContext *ctx )
{
    RegalAssert(ctx);
    GLenum err = ctx->dispatcher.driver.glGetError();
    if (err != GL_NO_ERROR)
        Error("GL error = ",toString(err));
}

REGAL_NAMESPACE_END

using namespace ::REGAL_NAMESPACE_INTERNAL;

REGAL_GLOBAL_BEGIN

#if REGAL_SYS_NACL
REGAL_DECL void RegalCreateContext( RegalSystemContext sysCtx,
                                    PPB_OpenGLES2 *interface,
                                    PP_Resource share_group )
#else
REGAL_DECL void RegalCreateContext( RegalSystemContext sysCtx,
                                    RegalSystemContext share_group )
#endif
{
  ::REGAL_NAMESPACE_INTERNAL::Init::init();

  RegalAssert( sc2rc.count( sysCtx ) == 0 );  // Should be unused.

  RegalContext * share_ctx = NULL;
  if (share_group != 0) {
    share_ctx = sc2rc[ share_group ];
    RegalAssert(share_ctx != NULL);
  }

  RegalContext * ctx = new RegalContext();
#if REGAL_SYS_NACL
  ctx->naclResource = sysCtx;
  ctx->naclES2      = interface;
#endif
  void *old_ctx = RegalPrivateGetCurrentContext();
  if (old_ctx == NULL) {
    // We need some context active during Init(), because
    // RegalContextInfo init makes GL calls.
    SetContextInTls(ctx);
  }
  ctx->Init(share_ctx);
  if (old_ctx != NULL) {
    SetContextInTls((RegalContext*) old_ctx);
  }
  sc2rc[ sysCtx ] = ctx;
  ctx->sysCtx = sysCtx;
}

#if REGAL_SYS_NACL
REGAL_DECL void RegalMakeCurrent( RegalSystemContext sysCtx,
                                  PPB_OpenGLES2 *interface )
#else
REGAL_DECL void RegalMakeCurrent( RegalSystemContext sysCtx )
#endif
{
  ::REGAL_NAMESPACE_INTERNAL::Init::init();

//  Trace("RegalPrivateMakeCurrent ",sysCtx);
  Thread thread = RegalPrivateThreadSelf();
  if (sysCtx) {
    // NOTE: Access to sc2rc and other parts of the function (including
    // various one-time-init in RegalCreateContext) are not thread-safe.
    RegalContext * ctx = sc2rc.count( sysCtx ) > 0 ? sc2rc[ sysCtx ] : NULL;
    if (!ctx) {
#if REGAL_SYS_NACL
      RegalCreateContext(sysCtx, interface, 0);
#else
      RegalCreateContext(sysCtx, 0);
#endif
      ctx = sc2rc[ sysCtx ];
    }

    SetContextInTls(ctx);

    if( th2rc.count( thread ) != 0 ) {
      RegalContext * & c = th2rc[ thread ];
      if( c ) {
        RegalAssert( c->thread == thread );
        c->thread = 0;
        c = NULL;
      }
    }
    RegalAssert( th2rc.count( thread ) == 0 || th2rc[ thread ] == NULL );
    RegalAssert( ctx->thread == 0 );
    th2rc[ thread ] = ctx;
    ctx->thread = thread;

    SetContextInTls(ctx);
    Internal("RegalMakeCurrent ",ctx," ",ctx->info->version);
  }
  else
  {
    if( th2rc.count( thread ) ) {
      RegalContext * & ctx = th2rc[ thread ];
      if( ctx != NULL ) {
        RegalAssert( ctx->thread == thread );
        ctx->thread = 0;
        ctx = NULL;
        RegalAssert( th2rc[ thread ] == NULL );
      }
    }
    SetContextInTls(NULL);
  }
}

REGAL_GLOBAL_END
