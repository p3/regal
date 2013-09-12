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

#ifndef __REGAL_DISPATCHER_GLOBAL_H__
#define __REGAL_DISPATCHER_GLOBAL_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalDispatcher.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DispatcherGlobal : public Dispatcher
{
public:

  #if REGAL_LOG
  DispatchTableGlobal logging;
  #endif

  #if REGAL_SYS_GLX && !REGAL_SYS_X11
  DispatchTableGlobal glx;
  #endif

  #if REGAL_TRACE
  DispatchTableGlobal trace;
  #endif

  DispatchTableGlobal driver;      // Underlying GLX/WGL/EGL implementation

  #if REGAL_MISSING
  DispatchTableGlobal missing;     // Must have this last
  #endif

public:
  DispatcherGlobal();
  ~DispatcherGlobal();

  inline void push_back(DispatchTableGlobal &table, bool enable)
  {
    // Disabling the missing table would be bad!
    #if REGAL_MISSING
    RegalAssert(&table!=&missing || enable==true);
    #endif

    Dispatcher::push_back(table,enable);
  }

  inline bool erase    (DispatchTableGlobal &table)                             { return Dispatcher::erase(table); }
  inline bool insert   (DispatchTableGlobal &other, DispatchTableGlobal &table) { return Dispatcher::insert(other,table); }

  inline void enable (DispatchTableGlobal &table)             { Dispatcher::enable (table); }
  inline void disable(DispatchTableGlobal &table)             { Dispatcher::disable(table); }

  inline bool isEnabled(DispatchTableGlobal &table) const     { return Dispatcher::isEnabled(table); }

  inline DispatchTableGlobal &operator[](const std::size_t i) { return reinterpret_cast<DispatchTableGlobal &>(Dispatcher::operator[](i)); }
  inline DispatchTableGlobal &front()                         { return reinterpret_cast<DispatchTableGlobal &>(Dispatcher::front());       }
  inline DispatchTableGlobal &back()                          { return reinterpret_cast<DispatchTableGlobal &>(Dispatcher::back());        }
};

extern DispatcherGlobal dispatcherGlobal;

// regaltest needs these declarations too

#if REGAL_SYS_EGL && REGAL_STATIC_EGL
extern void InitDispatchTableStaticEGL      (DispatchTableGlobal &tbl);
#endif

extern void InitDispatchTableGlobalLog       (DispatchTableGlobal &tbl);
extern void InitDispatchTableGlobalGLX       (DispatchTableGlobal &tbl);
extern void InitDispatchTableGlobalTrace     (DispatchTableGlobal &tbl);
extern void InitDispatchTableGlobalLoader    (DispatchTableGlobal &tbl);
extern void InitDispatchTableGlobalMissing   (DispatchTableGlobal &tbl);

REGAL_NAMESPACE_END

#endif // __REGAL_DISPATCHER_GLOBAL_H__
