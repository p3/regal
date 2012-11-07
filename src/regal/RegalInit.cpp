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
#include "RegalHttp.h"
#include "RegalToken.h"
#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalThread.h"
#include "RegalDispatcher.h"
#include "RegalContextInfo.h"

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

//////////////////////////////////////////////////////////////////////

void RegalCheckForGLErrors( RegalContext *ctx )
{
    RegalAssert(ctx);
    GLenum err = ctx->dispatcher.driver.glGetError();
    if (err != GL_NO_ERROR)
        Error("GL error = ",toString(err));
}

#if REGAL_SYS_WGL
extern "C" { DWORD __stdcall GetCurrentThreadId(void); }
#endif

namespace Thread {

// Single-threaded RegalContext

RegalContext *currentContext = NULL;

#if REGAL_NO_TLS
inline Thread Self() { return 1; }
#else

#if REGAL_SYS_WGL
#if REGAL_WIN_TLS
    DWORD currentContextIndex = DWORD(~0);
    struct TlsInit
    {
        TlsInit()
        {
            currentContextIndex = TlsAlloc();
        }
        ~TlsInit()
        {
            TlsFree( currentContextIndex );
        }
    };
    TlsInit tlsInit;
#else
    __declspec( thread ) void * currentContext = NULL;
#endif

    inline Thread Self()
    {
        return GetCurrentThreadId();
    }
#else
    pthread_key_t currentContextKey = 0;

    struct TlsInit
    {
      TlsInit()
      {
        pthread_key_create( &currentContextKey, NULL );
      }
    };

    TlsInit tlsInit;

    inline Thread Self()
    {
        return pthread_self();
    }

#endif
#endif

}

static void SetContextInTls(RegalContext *ctx)
{
#if REGAL_NO_TLS
  // Without thread local storage, simply set the
  // current Regal context
  Thread::currentContext = ctx;
#else
# if REGAL_SYS_WGL
  // For Windows....
#  if REGAL_WIN_TLS
  if (Thread::currentContextIndex == ~0)
    Thread::currentContextIndex = TlsAlloc();
  TlsSetValue( Thread::currentContextIndex, ctx );
#  else
  Thread::currentContext = ctx;
#  endif
# else
  // For Linux and Mac...
  if (Thread::currentContextKey == 0) {
    pthread_key_create( & Thread::currentContextKey, NULL );
  }
  pthread_setspecific( Thread::currentContextKey, ctx );
# endif
#endif
}

//

typedef map<RegalSystemContext, RegalContext *> SC2RC;
typedef map<Thread::Thread,     RegalContext *> TH2RC;

SC2RC sc2rc;
TH2RC th2rc;

REGAL_NAMESPACE_END

using namespace ::REGAL_NAMESPACE_INTERNAL;

REGAL_GLOBAL_BEGIN

//
// RegalSetErrorCallback
//

RegalErrorCallback RegalSetErrorCallback(RegalErrorCallback callback)
{
  RegalContext *ctx = GET_REGAL_CONTEXT();
  RegalAssert(ctx);
  return ctx->err.callback = callback;
}

//
// RegalShareContext
//

REGAL_DECL void RegalShareContext(RegalSystemContext a, RegalSystemContext b)
{
  ::REGAL_NAMESPACE_INTERNAL::Init::init();

  // NOTE: Access to sc2rc and other parts of the function (including
  // various one-time-init in RegalCreateContext) are not thread-safe.

  SC2RC::iterator iA = sc2rc.find(a);
  SC2RC::iterator iB = sc2rc.find(b);

  RegalContext *contextA = iA!=sc2rc.end() ? iA->second : NULL;
  RegalContext *contextB = iB!=sc2rc.end() ? iB->second : NULL;

  // Either of the groups of contexts needs to be uninitialized.
  // In principle Regal might be able to merge the shared
  // containers together, but that's not currently implemented.

  if (contextA && contextA->groupInitializedContext() && contextB && contextB->groupInitializedContext())
  {
    Warning("Regal can't share initialized context groups.");
    return;
  }

  // Create the Regal contexts, as necessary

  if (!contextA)
  {
    contextA = new RegalContext();
    RegalAssert(contextA);
    sc2rc[a] = contextA;
    contextA->sysCtx = a;
  }

  if (!contextB)
  {
    contextB = new RegalContext();
    RegalAssert(contextB);
    sc2rc[b] = contextB;
    contextB->sysCtx = b;
  }

  // Share all the Regal contexts in b into a

  std::list<RegalContext *> tmp = *contextB->shareGroup;

  for (std::list<RegalContext *>::iterator i = tmp.begin(); i!=tmp.end(); ++i)
  {
    RegalAssert(*i);
    contextA->shareGroup->push_back(*i);
    (*i)->shareGroup = contextA->shareGroup;
  }
}

//
// RegalMakeCurrent
//

#if REGAL_SYS_NACL
REGAL_DECL void RegalMakeCurrent( RegalSystemContext sysCtx, PPB_OpenGLES2 *interface)
#else
REGAL_DECL void RegalMakeCurrent( RegalSystemContext sysCtx )
#endif
{
  ::REGAL_NAMESPACE_INTERNAL::Init::init();

  ::REGAL_NAMESPACE_INTERNAL::Thread::Thread thread = ::REGAL_NAMESPACE_INTERNAL::Thread::Self();

  if (sysCtx)
  {
    // NOTE: Access to sc2rc and other parts of the function (including
    // various one-time-init in RegalCreateContext) are not thread-safe.

    SC2RC::iterator i = sc2rc.find(sysCtx);
    RegalContext *ctx = i!=sc2rc.end() ? i->second : NULL;

    // Create the Regal context, as necessary

    if (!ctx)
    {
      ctx = new RegalContext();
      RegalAssert(ctx);
      sc2rc[sysCtx] = ctx;
      ctx->sysCtx = sysCtx;
    }

    // Set current context

    SetContextInTls(ctx);

    // Do RegalContext initialization, if necessary.

    if (!ctx->initialized)
    {
#if REGAL_SYS_NACL
      ctx->naclResource = sysCtx;
      ctx->naclES2      = interface;
#endif

      // RegalContextInfo init makes GL calls, need an
      // active OpenGL context.

      ctx->Init();

      RegalAssert(ctx->initialized);
    }

    // If there is another Regal context associated with
    // this thread, disassociate it.

    TH2RC::iterator j = th2rc.find(thread);
    if (j!=th2rc.end())
    {
      RegalContext *&c = j->second;
      if (c)
      {
        RegalAssert(c->thread==thread);
        c->thread = 0;
        c = NULL;
      }
    }

    RegalAssert(!th2rc.count(thread) || !th2rc[thread]);
    RegalAssert(!ctx->thread);

    // Associate this thread with the Regal context

    th2rc[thread] = ctx;
    ctx->thread = thread;
    SetContextInTls(ctx);

    //

    Internal("RegalMakeCurrent ",ctx," ",ctx->info->version);
  }
  else
  {
    // If there is a Regal context associated with
    // this thread, disassociate it.

    TH2RC::iterator j = th2rc.find(thread);
    if (j!=th2rc.end())
    {
      RegalContext *&c = j->second;
      if (c)
      {
        RegalAssert(c->thread==thread);
        c->thread = 0;
        c = NULL;
      }
    }

    SetContextInTls(NULL);
  }
}

//
// RegalDestroyContext
//
// Cleanup all the resources associated with sysCtx
// Otherwise, Regal contexts would never be deleted

REGAL_DECL void RegalDestroyContext(RegalSystemContext sysCtx)
{
  if (sysCtx)
  {
    // NOTE: Access to sc2rc and other parts of the function (including
    // various one-time-init in RegalCreateContext) are not thread-safe.

    SC2RC::iterator i = sc2rc.find(sysCtx);
    RegalContext *ctx = i!=sc2rc.end() ? i->second : NULL;

    if (ctx)
    {
      RegalAssert(ctx->sysCtx==sysCtx);

      th2rc.erase(ctx->thread);
      sc2rc.erase(sysCtx);

      delete ctx;
    }
  }
}

REGAL_GLOBAL_END
