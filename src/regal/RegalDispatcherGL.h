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

#ifndef __REGAL_DISPATCHER_GL_H__
#define __REGAL_DISPATCHER_GL_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalDispatcher.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DispatcherGL : public Dispatcher
{
public:

#if REGAL_DEBUG
   DispatchTableGL debug;
#endif

#if REGAL_ERROR
   DispatchTableGL error;
#endif

#if REGAL_EMULATION
   DispatchTableGL emulation;
#endif

#if REGAL_CACHE
   DispatchTableGL cache;
#endif

#if REGAL_CODE
   DispatchTableGL code;
#endif

#if REGAL_STATISTICS
   DispatchTableGL statistics;
#endif

#if REGAL_LOG
   DispatchTableGL logging;
#endif

#if REGAL_TRACE
   DispatchTableGL trace;
#endif

   DispatchTableGL driver;      // Underlying OpenGL/ES implementation

#if REGAL_MISSING
   DispatchTableGL missing;     // Must have this last
#endif

public:
  DispatcherGL();
  ~DispatcherGL();

  inline void push_back(DispatchTableGL &table, bool enable)
  {
    // Disabling the missing table would be bad!
    #if REGAL_MISSING
    RegalAssert(&table!=&missing || enable==true);
    #endif

    Dispatcher::push_back(table,enable);
  }

  inline bool erase    (DispatchTableGL &table)                         { return Dispatcher::erase(table); }
  inline bool insert   (DispatchTableGL &other, DispatchTableGL &table) { return Dispatcher::insert(other,table); }

  inline void enable (DispatchTableGL &table)             { Dispatcher::enable (table); }
  inline void disable(DispatchTableGL &table)             { Dispatcher::disable(table); }

  inline bool isEnabled(DispatchTableGL &table) const     { return Dispatcher::isEnabled(table); }

  inline DispatchTableGL &operator[](const std::size_t i) { return reinterpret_cast<DispatchTableGL &>(Dispatcher::operator[](i)); }
  inline DispatchTableGL &front()                         { return reinterpret_cast<DispatchTableGL &>(Dispatcher::front());       }
  inline DispatchTableGL &back()                          { return reinterpret_cast<DispatchTableGL &>(Dispatcher::back());        }
};

// regaltest needs these declarations too

extern void InitDispatchTableCode      (DispatchTableGL &tbl);
extern void InitDispatchTableDebug     (DispatchTableGL &tbl);
extern void InitDispatchTableError     (DispatchTableGL &tbl);
extern void InitDispatchTableEmu       (DispatchTableGL &tbl);
extern void InitDispatchTableLog       (DispatchTableGL &tbl);
extern void InitDispatchTableLoader    (DispatchTableGL &tbl);
extern void InitDispatchTablePpapi     (DispatchTableGL &tbl);
extern void InitDispatchTableStatistics(DispatchTableGL &tbl);
extern void InitDispatchTableStaticES2 (DispatchTableGL &tbl);
extern void InitDispatchTableMissing   (DispatchTableGL &tbl);
extern void InitDispatchTableCache     (DispatchTableGL &tbl);
extern void InitDispatchTableTrace     (DispatchTableGL &tbl);

REGAL_NAMESPACE_END

#endif // __REGAL_DISPATCHER_GL_H__
