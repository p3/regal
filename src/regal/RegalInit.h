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

#ifndef __REGAL_INIT_H__
#define __REGAL_INIT_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>  // Needed for RegalSystemContext, mainly

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct RegalContext;

struct Init
{
public:

  Init();
  ~Init();

  static void init();
  static void atExit();

  //
  // Internal methods
  //

private:

  // Factory method for RegalContext lookup and/or construction.
  // Initialization of Regal contexts is deferred.

  static inline RegalContext *getContext(RegalSystemContext sysCtx);

  // Set the per-thread Regal context

  static inline void setContext(RegalContext *context);

  // Set current TLS Regal context

  static inline void setContextTLS(RegalContext *context);

  // Check for OpenGL errors

  static void checkForGLErrors(RegalContext *context);

public:

  //
  // API methods
  //

  static RegalErrorCallback setErrorCallback(RegalErrorCallback callback);

  static void               configure(const char *json);

  static void               shareContext(RegalSystemContext a, RegalSystemContext b);

  #if REGAL_SYS_PPAPI
  static void              makeCurrent(RegalSystemContext sysCtx, PPB_OpenGLES2 *ppb_interface);
  #else
  static void              makeCurrent(RegalSystemContext sysCtx);
  #endif

  static void              destroyContext(RegalSystemContext sysCtx);

  static void              getContextListingHTML(std::string &text);
};

REGAL_NAMESPACE_END

#endif
