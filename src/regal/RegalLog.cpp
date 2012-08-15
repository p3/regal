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

#include <cstdio>
#include <cstdarg>
using namespace std;

#include <boost/print/print_string.hpp>
using boost::print::print_string;
using boost::print::trim;

#include "RegalLog.h"
#include "RegalMarker.h"
#include "RegalContext.h"
#include "RegalPrivate.h"

#ifndef REGAL_SYS_WGL
#include <pthread.h>
#endif

// Otherwise we'd need to #include <windows.h>

#ifdef REGAL_SYS_WGL
extern "C"
{
  __declspec(dllimport) void __stdcall OutputDebugStringA( __in_opt const char* lpOutputString);
}
#endif

#if REGAL_SYS_ANDROID
#include <android/log.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Logging {

  bool enableError    = true;
  bool enableWarning  = true;
  bool enableInfo     = true;
  bool enableApp      = false;
  bool enableDriver   = false;
  bool enableInternal = false;
  bool enableHttp     = true;

  int maxLines = (REGAL_LOG_MAX_LINES);

  std::list<std::string> *buffer = NULL;
  std::size_t             bufferSize  = 0;
  std::size_t             bufferLimit = 500;

  void Init()
  {
#ifndef REGAL_NO_GETENV

    const char *error    = GetEnv("REGAL_LOG_ERROR");
    const char *warning  = GetEnv("REGAL_LOG_WARNING");
    const char *info     = GetEnv("REGAL_LOG_INFO");
    const char *app      = GetEnv("REGAL_LOG_APP");
    const char *driver   = GetEnv("REGAL_LOG_DRIVER");
    const char *internal = GetEnv("REGAL_LOG_INTERNAL");
    const char *http     = GetEnv("REGAL_LOG_HTTP");

    if (error)    enableError    = atoi(error)!=0;
    if (warning)  enableWarning  = atoi(warning)!=0;
    if (info)     enableInfo     = atoi(info)!=0;
    if (app)      enableApp      = atoi(app)!=0;
    if (driver)   enableDriver   = atoi(driver)!=0;
    if (internal) enableInternal = atoi(internal)!=0;
    if (http)     enableHttp     = atoi(http)!=0;

    const char *api = GetEnv("REGAL_LOG_API");
    const char *all = GetEnv("REGAL_LOG_ALL");

    if (api && atoi(api))
      enableApp = enableDriver = true;

    if (all && atoi(all))
      enableError = enableWarning = enableInfo = enableApp = enableDriver = enableInternal = enableHttp = true;

    const char *ml = GetEnv("REGAL_LOG_MAX_LINES");
    if (ml) maxLines = atoi(ml);

    const char *bl = GetEnv("REGAL_HTTP_LOG_LIMIT");
    if (bl) bufferLimit = atoi(bl);
#endif

#ifdef REGAL_HTTP_LOG_LIMIT
    bufferLimit = REGAL_HTTP_LOG_LIMIT;
#endif

    // TODO - clean this up at shutdown...

    if (bufferLimit)
      buffer = new list<string>();

    ITrace("Logging::Init");

#if REGAL_LOG_ERROR
    Info("REGAL_LOG_ERROR    ", enableError    ? "enabled" : "disabled");
#endif

#if REGAL_LOG_WARNING
    Info("REGAL_LOG_WARNING  ", enableWarning  ? "enabled" : "disabled");
#endif

#if REGAL_LOG_INFO
    Info("REGAL_LOG_INFO     ", enableInfo     ? "enabled" : "disabled");
#endif

#if REGAL_LOG_APP
    Info("REGAL_LOG_APP      ", enableApp      ? "enabled" : "disabled");
#endif

#if REGAL_LOG_DRIVER
    Info("REGAL_LOG_DRIVER   ", enableDriver   ? "enabled" : "disabled");
#endif

#if REGAL_LOG_INTERNAL
    Info("REGAL_LOG_INTERNAL ", enableInternal ? "enabled" : "disabled");
#endif

#if REGAL_LOG_HTTP
    Info("REGAL_LOG_HTTP     ", enableHttp     ? "enabled" : "disabled");
#endif
  }

  inline size_t indent()
  {
    // For OSX we need avoid GET_REGAL_CONTEXT implicitly
    // trying to create a RegalContext and triggering more
    // (recursive) logging.

#ifndef REGAL_SYS_WGL
    if (!regalPrivateCurrentContextKey || !pthread_getspecific(regalPrivateCurrentContextKey))
      return 0;
#endif

    RegalContext *rCtx = GET_REGAL_CONTEXT();

    size_t indent = 0;
    if (rCtx)
    {
      indent += (rCtx->depthBeginEnd + rCtx->depthPushAttrib)*2;
      indent += rCtx->marker ? rCtx->marker->indent() : 0;
    }
    return indent;
  }

  inline string message(const char *prefix, const char *delim, const string &str)
  {
    const static string trimSuffix(" ...");

    std::string trimPrefix = print_string(prefix ? prefix : "", delim ? delim : "", string(indent(),' '));

    return print_string(trim(str,'\n',maxLines>0 ? maxLines : ~0,trimPrefix,trimSuffix), '\n');
  }

  // Append to the log buffer

  inline void append(string &str)
  {
    if (buffer)
    {
      buffer->push_back(string());
      buffer->back().swap(str);
      bufferSize++;

      // Prune the buffer list, as necessary

      while (bufferSize>bufferLimit)
      {
        buffer->pop_front();
        --bufferSize;
      }
    }
  }

#ifndef REGAL_LOG_TAG
#define REGAL_LOG_TAG "Regal"
#endif

  void Output(const char *prefix, const char *delim, const string &str)
  {
    if (str.length())
    {
      string m = message(prefix,delim,str);
      
      RegalContext *rCtx = NULL;

#ifndef REGAL_SYS_WGL
      if (regalPrivateCurrentContextKey && pthread_getspecific(regalPrivateCurrentContextKey))
        rCtx = GET_REGAL_CONTEXT();
#else
      rCtx = GET_REGAL_CONTEXT();
#endif

      if (rCtx && rCtx->logCallback)
        rCtx->logCallback(GL_LOG_INFO_REGAL, (GLsizei) m.length(), m.c_str(), reinterpret_cast<void *>(rCtx->sysCtx));
      else
      {
#if defined(REGAL_SYS_WGL)
        OutputDebugStringA(m.c_str());
        fprintf(stderr, "%s", m.c_str());
        fflush(stderr);
#elif defined(REGAL_SYS_ANDROID)
        // ANDROID_LOG_INFO
        // ANDROID_LOG_WARN
        // ANDROID_LOG_ERROR
        __android_log_print(ANDROID_LOG_INFO, REGAL_LOG_TAG, m.c_str());
#elif REGAL_SYS_NACL
#else
        fprintf(stdout, "%s", m.c_str());
        fflush(stdout);
#endif
      }
      append(m);
    }
  }
}

REGAL_NAMESPACE_END
