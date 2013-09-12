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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <algorithm>

#include "RegalConfig.h"
#include "RegalDispatcherGlobal.h"

using namespace ::std;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

DispatcherGlobal dispatcherGlobal;

DispatcherGlobal::DispatcherGlobal()
: Dispatcher()
{
  #if REGAL_LOG
  InitDispatchTableGlobalLog(logging);
  push_back(logging,Config::enableLog);
  #endif

  #if REGAL_SYS_GLX && !REGAL_SYS_X11
  memset(&glx,0,sizeof(glx));
  InitDispatchTableGlobalGLX(glx);
  push_back(glx,true);
  #endif

  // have to check this early for the global dispatches, otherwise we'd use Config

  #if REGAL_TRACE
  {
    getEnv( "REGAL_TRACE", Config::enableTrace);
    ::memset(&trace, 0, sizeof(trace) );
    InitDispatchTableGlobalTrace(trace);
    push_back(trace,Config::enableTrace);
  }
  #endif

  #if REGAL_DRIVER
  memset(&driver,0,sizeof(driver));

  #if REGAL_SYS_EGL && REGAL_STATIC_EGL
  InitDispatchTableStaticEGL(driver);
  #else
  InitDispatchTableGlobalLoader(driver);     // GLX/WGL/EGL lazy loader
  #endif

  push_back(driver,Config::enableDriver);
  #endif

  #if REGAL_MISSING
  memset(&missing,0,sizeof(missing));
  InitDispatchTableGlobalMissing(missing);
  push_back(missing,Config::enableMissing);
  #endif
}

DispatcherGlobal::~DispatcherGlobal()
{
}

REGAL_NAMESPACE_END
