/*
  Copyright (c) 2011 NVIDIA Corporation
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

#ifndef __REGAL_TIMER_H__
#define __REGAL_TIMER_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))
  extern "C" { int __stdcall QueryPerformanceFrequency (signed long long *lpFrequency);        }
  extern "C" { int __stdcall QueryPerformanceCounter   (signed long long *lpPerformanceCount); }
#else
#include <sys/time.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct Timer
{
  typedef unsigned long long Value;

  inline Timer();
  inline ~Timer() {}

  inline Timer::Value restart() { Timer::Value end = _start; _start = now(); return _start - end; }

  inline Timer::Value now();    /* Time in micro-seconds */
  inline Timer::Value elapsed() { return now() - _start; }

  Timer::Value _freq;
  Timer::Value _start;    /* Zero by default */
};

#if REGAL_SYS_WGL || (REGAL_SYS_PPAPI && !defined(__native_client__))

inline
Timer::Timer()
: _start(0)
{
  signed long long f;
  ::QueryPerformanceFrequency(&f);
  _freq = Timer::Value(f);
}

inline
Timer::Value Timer::now()
{
  signed long long time;
  QueryPerformanceCounter(&time);
  return (Timer::Value) time * Timer::Value(1000000) / _freq;
}

#else

inline
Timer::Timer()
: _freq(1),
  _start(0)
{
}

inline
Timer::Value Timer::now()
{
  struct timeval val;
  gettimeofday(&val, NULL);
  return Timer::Value(val.tv_sec) * Timer::Value(1000000) + Timer::Value(val.tv_usec);
}

#endif

REGAL_NAMESPACE_END

#endif
