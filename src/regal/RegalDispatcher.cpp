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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <algorithm>

#include "RegalConfig.h"
#include "RegalDispatcher.h"

using namespace ::std;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

Dispatcher::Dispatcher()
: _front(NULL),
  _size(0)
{
  #if REGAL_TRACE
  InitDispatchTableTrace(trace);
  push_back(trace,Config::enableTrace);
  #endif

  #if REGAL_DEBUG
  InitDispatchTableDebug(debug);
  push_back(debug,Config::enableDebug);
  #endif

  #if REGAL_ERROR
  InitDispatchTableError(error);
  push_back(error,Config::enableError);
  #endif

  #if REGAL_EMULATION
  ::memset(&emulation,0,sizeof(DispatchTable));
  InitDispatchTableEmu(emulation);               // emulated functions only
  push_back(emulation,Config::enableEmulation || Config::forceEmulation);
  #endif

  #if REGAL_CACHE
  ::memset(&cache,0,sizeof(DispatchTable));
  InitDispatchTableCache(cache);
  push_back(cache,true);
  #endif

  #if REGAL_CODE
  ::memset(&code,0,sizeof(DispatchTable));
  InitDispatchTableCode(code);
  push_back(code,Config::enableCode);
  #endif

  #if REGAL_LOG
  InitDispatchTableLog(logging);
  push_back(logging,Config::enableLog);
  #endif

  #if REGAL_DRIVER
  #if REGAL_STATIC_ES2
  ::memset(&driver,0,sizeof(DispatchTable));
  InitDispatchTableStaticES2(driver);           // ES 2.0 functions only
  #elif REGAL_SYS_PPAPI
  ::memset(&driver,0,sizeof(DispatchTable));
  InitDispatchTablePpapi(driver);               // ES 2.0 functions only
  #else
  InitDispatchTableLoader(driver);              // Desktop/ES2.0 lazy loader
  #endif
  push_back(driver,Config::enableDriver);
  #endif

  InitDispatchTableMissing(missing);
  push_back(missing,true);

  // Optionally move the error checking dispatch to downstream of emulation.

  #if defined(REGAL_ERROR_POST_EMU)
  if (erase(error))
    insert(cache,error);
  #endif
}

Dispatcher::~Dispatcher()
{
}

void
Dispatcher::push_back(DispatchTable &table, bool enabled)
{
  // Disabling the missing table would be bad!
  RegalAssert(&table!=&missing || enabled==true);

  table._enabled = enabled;
  table._next = NULL;
  table._prev = NULL;

  if (size())
  {
    table._prev = &back();
    back()._next = &table;
  }

  _table.push_back(&table);

  // Cached front() and size()

  if (!_size++)
    _front = &table;
}

bool
Dispatcher::erase(DispatchTable &table)
{
  // O(n) time, oh well.

  vector<DispatchTable *>::iterator i = find(_table.begin(),_table.end(),&table);
  if (i!=_table.end())
  {
    // Linked list adjustment

    if (table._next)
      table._next->_prev = table._prev;
    if (table._prev)
      table._prev->_next = table._next;
    table._next = NULL;
    table._prev = NULL;

    _table.erase(i);

    _size--;
    _front = _size ? _table.front() : NULL;
    return true;
  }

  return false;
}

bool
Dispatcher::insert(DispatchTable &other, DispatchTable &table)
{
  vector<DispatchTable *>::iterator i = find(_table.begin(),_table.end(),&other);
  if (i!=_table.end())
  {
    table._next = &other;
    table._prev = other._prev;
    if (table._next)
      table._next->_prev = &table;
    if (table._prev)
      table._prev->_next = &table;

    _table.insert(i,&table);

    _size++;
    _front = _table.front();
    return true;
  }
  return false;
}

REGAL_NAMESPACE_END
