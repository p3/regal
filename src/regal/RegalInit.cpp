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

//

typedef map<RegalSystemContext, RegalContext *> SC2RC;
typedef map<Thread::Thread,     RegalContext *> TH2RC;

SC2RC sc2rc;
TH2RC th2rc;

// NOTE: Access to sc2rc and other parts of the function (including
// various one-time-init in RegalMakeCurrent) are not thread-safe.

RegalContext *
Init::getContext(RegalSystemContext sysCtx)
{
  RegalAssert(sysCtx);

  SC2RC::iterator i = sc2rc.find(sysCtx);
  if (i!=sc2rc.end())
  {
    Internal("Init::context lookup for ",sysCtx);
    return i->second;
  }
  else
  {
    Internal("Init::context factory for ",sysCtx);

    RegalContext *context = new RegalContext();
    RegalAssert(context);
    sc2rc[sysCtx] = context;
    context->sysCtx = sysCtx;
    return context;
  }
}

void
Init::setContext(RegalContext *context)
{
  Thread::Thread thread = Thread::Self();

  Internal("Init::setContext ",thread," to ",context," ",context ? context->info->version : "");

  // First do the lookup

  TH2RC::iterator i = th2rc.find(thread);
  if (i!=th2rc.end())
  {
    // Early out if this context and thread are
    // already associated.

    if (i->second==context)
    {
      RegalAssert(!context || context->thread==thread);
      return;
    }

    // If some other context is associated
    // with this thread, disassociate it.

    if (i->second)      
    {
      RegalAssert(i->second->thread==thread);
      i->second->thread = 0;
    }
    
    i->second = context;
    if (context)
      context->thread = thread;
  }
  else
  {
    // Associate this thread with the Regal context  
    th2rc[thread] = context;
  }

  if (context)
    context->thread = thread;

  setContextTLS(context);
}

//
// TLS Stuff
//

#if REGAL_SYS_WGL
extern "C" { DWORD __stdcall GetCurrentThreadId(void); }
#endif

namespace Thread
{

#if REGAL_NO_TLS
RegalContext *currentContext = NULL;
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
#endif
#endif

}

void 
Init::setContextTLS(RegalContext *context)
{
  Internal("Init::setContextTLS ",context);

  // Without thread local storage, simply set the
  // current Regal context

#if REGAL_NO_TLS
  Thread::currentContext = context;
#else

  // For Windows....

# if REGAL_SYS_WGL
#  if REGAL_WIN_TLS
  if (Thread::currentContextIndex == ~0)
    Thread::currentContextIndex = TlsAlloc();
  TlsSetValue( Thread::currentContextIndex, context );
#  else
  Thread::currentContext = context;
#  endif
# else

  // For Linux and Mac...

  if (!Thread::currentContextKey)
    pthread_key_create( &Thread::currentContextKey, NULL );
  pthread_setspecific( Thread::currentContextKey, context );
# endif
#endif
}

void
Init::checkForGLErrors(RegalContext *context)
{
  RegalAssert(context);
  GLenum err = context->dispatcher.driver.glGetError();
  if (err!=GL_NO_ERROR)
    Error("GL error = ",toString(err));
}

//
// API Methods
//

RegalErrorCallback
Init::setErrorCallback(RegalErrorCallback callback)
{
  init();

  // TODO - warning or error for context==NULL ?

  RegalContext *context = REGAL_GET_CONTEXT();
  RegalAssert(context);
  return context ? context->err.callback = callback : NULL;
}

void
Init::shareContext(RegalSystemContext a, RegalSystemContext b)
{
  init();

  RegalContext *contextA = getContext(a);
  RegalContext *contextB = getContext(b);

  RegalAssert(contextA);
  RegalAssert(contextB);
  
  // Either of the groups of contexts needs to be uninitialized.
  // In principle Regal might be able to merge the shared
  // containers together, but that's not currently implemented.

  if (contextA->groupInitializedContext() && contextB->groupInitializedContext())
  {
    Warning("Regal can't share initialized context groups.");
    RegalAssert(false);
    return;
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

void
#if REGAL_SYS_PPAPI
Init::makeCurrent(RegalSystemContext sysCtx, PPB_OpenGLES2 *interface)
#else
Init::makeCurrent(RegalSystemContext sysCtx)
#endif
{
  init();

  if (sysCtx)
  {
    RegalContext *context = getContext(sysCtx);
    RegalAssert(context);

    // Do RegalContext initialization, if necessary.

    if (!context->initialized)
    {
      // Set regal context TLS for initialization purposes
      // This is needed for Thread::CurrentContext on Mac OSX

      setContextTLS(context);

#if REGAL_SYS_PPAPI
      context->ppapiResource = sysCtx;
      context->ppapiES2      = interface;
#endif

      // RegalContextInfo init makes GL calls, need an
      // active OpenGL context.

      context->Init();

      RegalAssert(context->initialized);
    }

    setContext(context);

    Internal("Init::makeCurrent ",context," ",context->info->version);
    
    return;
  }

  setContext(NULL);
  setContextTLS(NULL);  // Need to do this in addition to setContext?
}

// Cleanup all the resources associated with sysCtx
// Otherwise, Regal contexts would never be deleted

void
Init::destroyContext(RegalSystemContext sysCtx)
{
  init();

  if (sysCtx)
  {
    RegalContext *context = getContext(sysCtx);

    if (context)
    {
      RegalAssert(context->sysCtx==sysCtx);

      th2rc.erase(context->thread);
      sc2rc.erase(sysCtx);
      
      // TODO - clear TLS for other threads too?
      
      if (context==Thread::CurrentContext())
        setContextTLS(NULL);

      delete context;
    }
  }
}

REGAL_NAMESPACE_END

//
// Regal API entry points
//
// These should _not_ be used internally
// Global locking could be implemented in this layer

REGAL_GLOBAL_BEGIN

RegalErrorCallback RegalSetErrorCallback(RegalErrorCallback callback)
{
  return ::REGAL_NAMESPACE_INTERNAL::Init::setErrorCallback(callback);
}

REGAL_DECL void RegalShareContext(RegalSystemContext a, RegalSystemContext b)
{
  ::REGAL_NAMESPACE_INTERNAL::Init::shareContext(a,b);
}

#if REGAL_SYS_PPAPI
REGAL_DECL void RegalMakeCurrent(RegalSystemContext sysCtx, PPB_OpenGLES2 *interface)
#else
REGAL_DECL void RegalMakeCurrent(RegalSystemContext sysCtx)
#endif
{
#if REGAL_SYS_PPAPI
  ::REGAL_NAMESPACE_INTERNAL::Init::makeCurrent(sysCtx,interface);
#else
  ::REGAL_NAMESPACE_INTERNAL::Init::makeCurrent(sysCtx);
#endif
}

REGAL_DECL void RegalDestroyContext(RegalSystemContext sysCtx)
{
  ::REGAL_NAMESPACE_INTERNAL::Init::destroyContext(sysCtx);
}

REGAL_GLOBAL_END
