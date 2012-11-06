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

#ifndef __REGAL_THREAD_H__
#define __REGAL_THREAD_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#if !REGAL_SYS_WGL
#include <pthread.h>
#endif

// Windows TLS API

#if REGAL_SYS_WGL && REGAL_WIN_TLS
extern "C" { DWORD __stdcall TlsAlloc    (void);          }
extern "C" { int   __stdcall TlsFree     (DWORD);         }
extern "C" { void* __stdcall TlsGetValue (DWORD);         }
extern "C" { int   __stdcall TlsSetValue (DWORD, void *); }
#endif

// Windows process and thread ID

#if REGAL_SYS_WGL
extern "C" { DWORD __stdcall GetCurrentProcessId (void);          }
extern "C" { DWORD __stdcall GetCurrentThreadId  (void);          }
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct RegalContext;

namespace Thread {

#if REGAL_NO_TLS

extern RegalContext *currentContext;

typedef int Thread;

inline void *CurrentContext()
{
  return currentContext;
}

#else // REGAL_NO_TLS

#if REGAL_SYS_WGL
#if REGAL_WIN_TLS

  typedef DWORD Thread;

  extern DWORD currentContextIndex;

  inline void *CurrentContext()
  {
    return TlsGetValue( currentContextIndex );
  }

#else // REGAL_WIN_TLS

  // Windows __declspec( thread )

  typedef DWORD Thread;

  extern __declspec( thread ) void *regalCurrentContext;

  inline void *CurrentContext()
  {
    return regalCurrentContext;
  }

#endif // REGAL_WIN_TLS
#else  // REGAL_SYS_WGL
#if REGAL_SYS_OSX

  // Mac OS X

  typedef pthread_t Thread;
  extern pthread_key_t currentContextKey;

  inline void *CurrentContext()
  {
    void *v = pthread_getspecific(currentContextKey);
    if (!v) {
      RegalMakeCurrent(CGLGetCurrentContext());
      return pthread_getspecific(currentContextKey);
    }
    return v;
  }

#else  // REGAL_SYS_OSX

  // Linux, etc

  typedef pthread_t Thread;
  extern pthread_key_t currentContextKey;

  inline void *CurrentContext()
  {
    return pthread_getspecific(currentContextKey);
  }

#endif // REGAL_SYS_OSX
#endif // REGAL_SYS_WGL
#endif // REGAL_NO_TLS

// Current process ID

inline std::size_t procId()
{
#if REGAL_SYS_WGL
	return static_cast<std::size_t>(GetCurrentProcessId());
#elif REGAL_SYS_OSX || REGAL_SYS_OSX || REGAL_SYS_IOS || REGAL_SYS_ANDROID || REGAL_SYS_GLX
	return (std::size_t) getpid();
#else
  return 0;
#endif
}

// Current thread ID

inline std::size_t threadId()
{
#if REGAL_SYS_WGL
	return static_cast<std::size_t>(GetCurrentThreadId());
#elif REGAL_SYS_OSX || REGAL_SYS_OSX || REGAL_SYS_IOS || REGAL_SYS_ANDROID || REGAL_SYS_GLX

  // Revisit - pthread_self returns an opaque handle which isn't truely
  // a thread identifier.

	return (std::size_t) pthread_self();
#else
  return 0;
#endif
}

}

REGAL_NAMESPACE_END

#endif // __REGAL_THREAD_H__
