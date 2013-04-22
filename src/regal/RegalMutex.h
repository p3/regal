/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2013 Mark Adams
  Copyright (c) 2013 Nigel Stewart
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

 Regal threading
 Mark Adams, Nigel Stewart

 */

#ifndef __REGAL_MUTEX_H__
#define __REGAL_MUTEX_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#if !REGAL_SYS_WGL && !(REGAL_SYS_PPAPI && !defined(__native_client__))
#include <pthread.h>
#endif

#if (REGAL_SYS_WGL && defined(REGAL_SYS_WGL_DECLARE_WGL)) || (REGAL_SYS_PPAPI && !defined(__native_client__))

#if (REGAL_SYS_PPAPI && !defined(__native_client__)) // windows PPAPI build
typedef long LONG;
typedef unsigned long * ULONG_PTR;
typedef void VOID;
typedef void * PVOID;
typedef PVOID HANDLE;
#endif

typedef struct _CRITICAL_SECTION
{
    void *DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} CRITICAL_SECTION, *LPCRITICAL_SECTION;

extern "C" { void __stdcall InitializeCriticalSection(LPCRITICAL_SECTION); }
extern "C" { void __stdcall DeleteCriticalSection(LPCRITICAL_SECTION);     }
extern "C" { void __stdcall EnterCriticalSection(LPCRITICAL_SECTION);      }
extern "C" { void __stdcall LeaveCriticalSection(LPCRITICAL_SECTION);      }

#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Thread
{
  //
  // Mutex
  //

  struct Mutex
  {
  public:
    inline Mutex()
    {
#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
      InitializeCriticalSection(&_cs);
#else
      pthread_mutexattr_init(&_mutexattr);
      pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_RECURSIVE);
      pthread_mutex_init(&_mutex, &_mutexattr);
#endif
    }

    inline ~Mutex()
    {
#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
      DeleteCriticalSection(&_cs);
#else
      pthread_mutex_destroy(&_mutex);
      pthread_mutexattr_destroy(&_mutexattr);
#endif
    }

    inline void acquire()
    {
#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
      EnterCriticalSection(&_cs);
#else
      pthread_mutex_lock(&_mutex);
#endif
    }

    inline void release()
    {
#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
      LeaveCriticalSection(&_cs);
#else
      pthread_mutex_unlock(&_mutex);
#endif
    };

  private:

#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
    CRITICAL_SECTION _cs;
#else
    pthread_mutex_t _mutex;
    pthread_mutexattr_t _mutexattr;
#endif
  };

  //
  // ScopedLock
  //

  struct ScopedLock
  {
  public:
    inline ScopedLock(Mutex *mutex = NULL)
    : _mutex(mutex)
    {
      if (_mutex)
        _mutex->acquire();
    }

    inline ~ScopedLock()
    {
      if (_mutex)
        _mutex->release();
    }

    inline void release()
    {
      if (!_mutex)
        return;
      _mutex->release();
      _mutex = NULL;
    }

  private:
    Mutex *_mutex;

    ScopedLock(const ScopedLock &other);
    ScopedLock &operator=(const ScopedLock &other);
  };

}

REGAL_NAMESPACE_END

#endif
