/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc.
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

#include <limits>

#include <boost/print/print_string.hpp>
using boost::print::print_string;

#include "RegalLog.h"
#include "RegalConfig.h"
#include "RegalSystem.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Config {

  bool forceCoreProfile = REGAL_FORCE_CORE_PROFILE;
  bool forceES2Profile  = REGAL_FORCE_ES2_PROFILE;
  bool sysGLX           = REGAL_SYS_GLX;
  bool sysEGL           = REGAL_SYS_EGL && !REGAL_SYS_GLX;
  bool forceEmulation   = REGAL_FORCE_EMULATION;
  bool enableEmulation  = REGAL_EMULATION;
  bool enableDebug      = false;
  bool enableError      = false;
  bool enableCode       = false;
  bool enableLog        = REGAL_LOG;
  bool enableDriver     = REGAL_DRIVER;

  bool enableEmuPpa     = REGAL_EMU_PPA;
  bool enableEmuPpca    = REGAL_EMU_PPCA;
  bool enableEmuObj     = REGAL_EMU_OBJ;
  bool enableEmuBin     = REGAL_EMU_BIN;
  bool enableEmuDsa     = REGAL_EMU_DSA;
  bool enableEmuIff     = REGAL_EMU_IFF;
  bool enableEmuSo      = REGAL_EMU_SO;
  bool enableEmuVao     = REGAL_EMU_VAO;
  bool enableEmuFilter  = REGAL_EMU_FILTER;
  bool enableEmuTexC    = REGAL_EMU_TEXC;

  int  frameLimit       = 0;  // Unlimited

  bool frameMd5Color    = false;
  bool frameMd5Stencil  = false;
  bool frameMd5Depth    = false;

  unsigned char frameMd5ColorMask   = std::numeric_limits<unsigned char>::max();
  unsigned char frameMd5StencilMask = std::numeric_limits<unsigned char>::max();
  size_t        frameMd5DepthMask   = std::numeric_limits<size_t       >::max();

  bool frameSaveColor   = false;
  bool frameSaveStencil = false;
  bool frameSaveDepth   = false;

  bool        cache            = REGAL_CACHE;
  bool        cacheShader      = false;
  bool        cacheShaderRead  = false;
  bool        cacheShaderWrite = false;
  std::string cacheDirectory("./");

  void Init()
  {
    Internal("Config::Init","()");

#ifndef REGAL_NO_GETENV
    const char *tmp;

#if !REGAL_FORCE_CORE_PROFILE
    tmp = GetEnv( "REGAL_FORCE_CORE_PROFILE" );
    if (tmp) forceCoreProfile = atoi(tmp)!=0;
#endif

#if !REGAL_FORCE_ES2_PROFILE
    tmp = GetEnv( "REGAL_FORCE_ES2_PROFILE" );
    if (tmp) forceES2Profile = atoi(tmp)!=0;
#endif

  // With REGAL_SYS_GLX && REGAL_SYS_EGL
  // we infer each from other, if specified,
  // to behave as a toggle.

#if REGAL_SYS_GLX
    tmp = GetEnv( "REGAL_SYS_GLX" );
    if (tmp)
    {
      sysGLX = atoi(tmp)!=0;
#if REGAL_SYS_EGL
      sysEGL = !sysGLX;
#endif
    }
#endif

#if REGAL_SYS_EGL
    tmp = GetEnv( "REGAL_SYS_EGL" );
    if (tmp)
    {
      sysEGL = atoi(tmp)!=0;
#if REGAL_SYS_GLX
      sysGLX = !sysEGL;
#endif
     }
#endif

    // Default to GLX, if necessary

#if REGAL_SYS_GLX && REGAL_SYS_EGL
    if (sysGLX && sysEGL)
      sysEGL = false;
#endif

#if !REGAL_FORCE_EMULATION
    tmp = GetEnv( "REGAL_FORCE_EMULATION" );
    if (tmp) forceEmulation = atoi(tmp)!=0;
#endif

#if REGAL_EMULATION
    tmp = GetEnv( "REGAL_EMULATION" );
    if (tmp) enableEmulation = atoi(tmp)!=0;
#endif

    // Deprecated

    tmp = GetEnv( "REGAL_NO_EMULATION" );
    if (tmp) enableEmulation = atoi(tmp)==0;

#if REGAL_DEBUG
    tmp = GetEnv( "REGAL_DEBUG" );
    if (tmp) enableDebug = atoi(tmp)!=0;
#endif

#if REGAL_ERROR
    tmp = GetEnv( "REGAL_ERROR" );
    if (tmp) enableError = atoi(tmp)!=0;
#endif

#if REGAL_CODE
    tmp = GetEnv( "REGAL_CODE" );
    if (tmp) enableCode = atoi(tmp)!=0;
#endif

#if REGAL_LOG
    tmp = GetEnv( "REGAL_LOG" );
    if (tmp) enableLog = atoi(tmp)!=0;
#endif

#if REGAL_DRIVER
    tmp = GetEnv( "REGAL_DRIVER" );
    if (tmp) enableDriver = atoi(tmp)!=0;
#endif

#if REGAL_EMU_PPA
    tmp = GetEnv( "REGAL_EMU_PPA" );
    if (tmp) enableEmuPpa = atoi(tmp)!=0;
#endif

#if REGAL_EMU_PPCA
    tmp = GetEnv( "REGAL_EMU_PPCA" );
    if (tmp) enableEmuPpca = atoi(tmp)!=0;
#endif

#if REGAL_EMU_OBJ
    tmp = GetEnv( "REGAL_EMU_OBJ" );
    if (tmp) enableEmuObj = atoi(tmp)!=0;
#endif

#if REGAL_EMU_BIN
    tmp = GetEnv( "REGAL_EMU_BIN" );
    if (tmp) enableEmuBin = atoi(tmp)!=0;
#endif

#if REGAL_EMU_DSA
    tmp = GetEnv( "REGAL_EMU_DSA" );
    if (tmp) enableEmuDsa = atoi(tmp)!=0;
#endif

#if REGAL_EMU_IFF
    tmp = GetEnv( "REGAL_EMU_IFF" );
    if (tmp) enableEmuIff = atoi(tmp)!=0;
#endif

#if REGAL_EMU_SO
    tmp = GetEnv( "REGAL_EMU_SO" );
    if (tmp) enableEmuSo = atoi(tmp)!=0;
#endif

#if REGAL_EMU_VAO
    tmp = GetEnv( "REGAL_EMU_VAO" );
    if (tmp) enableEmuVao = atoi(tmp)!=0;
#endif

#if REGAL_EMU_TEXC
    tmp = GetEnv( "REGAL_EMU_TEXC" );
    if (tmp) enableEmuTexC = atoi(tmp)!=0;
#endif

    tmp = GetEnv( "REGAL_EMU_FILTER" );
    if (tmp) enableEmuFilter = atoi(tmp)!=0;

  //

    tmp = GetEnv( "REGAL_FRAME_LIMIT" );
    if (tmp) frameLimit = atoi(tmp);

  //

    tmp = GetEnv( "REGAL_MD5_COLOR" );
    if (tmp) frameMd5Color = atoi(tmp)!=0;

    tmp = GetEnv( "REGAL_MD5_STENCIL" );
    if (tmp) frameMd5Stencil = atoi(tmp)!=0;

    tmp = GetEnv( "REGAL_MD5_DEPTH" );
    if (tmp) frameMd5Depth = atoi(tmp)!=0;

    tmp = GetEnv( "REGAL_MD5_COLOR_MASK" );
    if (tmp) frameMd5ColorMask = static_cast<unsigned char>(atoi(tmp));

    tmp = GetEnv( "REGAL_MD5_STENCIL_MASK" );
    if (tmp) frameMd5StencilMask = static_cast<unsigned char>(atoi(tmp));

    tmp = GetEnv( "REGAL_MD5_DEPTH_MASK" );
    if (tmp) frameMd5DepthMask = atoi(tmp);

    //

    tmp = GetEnv( "REGAL_SAVE_COLOR" );
    if (tmp) frameSaveColor = atoi(tmp)!=0;

    tmp = GetEnv( "REGAL_SAVE_STENCIL" );
    if (tmp) frameSaveStencil = atoi(tmp)!=0;

    tmp = GetEnv( "REGAL_SAVE_DEPTH" );
    if (tmp) frameSaveDepth = atoi(tmp)!=0;

    // Caching

#if REGAL_CACHE
    tmp = GetEnv( "REGAL_CACHE" );
    if (tmp) cache = atoi(tmp)!=0;

#if REGAL_CACHE_SHADER
    tmp = GetEnv( "REGAL_CACHE_SHADER" );
    if (tmp) cacheShader = atoi(tmp)!=0;
#endif

#if REGAL_CACHE_SHADER_WRITE
    tmp = GetEnv( "REGAL_CACHE_SHADER_WRITE" );
    if (tmp) cacheShaderWrite = atoi(tmp)!=0;
#endif

#if REGAL_CACHE_SHADER_READ
    tmp = GetEnv( "REGAL_CACHE_SHADER_READ" );
    if (tmp) cacheShaderRead = atoi(tmp)!=0;
#endif

    tmp = GetEnv( "REGAL_CACHE_DIRECTORY" );
    if (tmp) cacheDirectory = tmp;
#endif

#endif

    // REGAL_NO_EMULATION is deprecated, use REGAL_EMULATION=0 instead.

#if REGAL_EMULATION && defined(REGAL_NO_EMULATION) && REGAL_NO_EMULATION
    enableEmulation = false;
#endif

    Info("REGAL_FORCE_CORE_PROFILE ", forceCoreProfile ? "enabled" : "disabled");
    Info("REGAL_FORCE_ES2_PROFILE  ", forceES2Profile  ? "enabled" : "disabled");

#if REGAL_SYS_GLX
    Info("REGAL_SYS_GLX            ", sysGLX           ? "enabled" : "disabled");
#endif

#if REGAL_SYS_EGL
    Info("REGAL_SYS_EGL            ", sysEGL           ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_EMULATION    ", forceEmulation   ? "enabled" : "disabled");
    Info("REGAL_DEBUG              ", enableDebug      ? "enabled" : "disabled");
    Info("REGAL_ERROR              ", enableError      ? "enabled" : "disabled");
    Info("REGAL_CODE               ", enableCode       ? "enabled" : "disabled");
    Info("REGAL_EMULATION          ", enableEmulation  ? "enabled" : "disabled");
    Info("REGAL_LOG                ", enableLog        ? "enabled" : "disabled");
    Info("REGAL_DRIVER             ", enableDriver     ? "enabled" : "disabled");

    Info("REGAL_EMU_PPA            ", enableEmuPpa     ? "enabled" : "disabled");
    Info("REGAL_EMU_PPCA           ", enableEmuPpca    ? "enabled" : "disabled");
    Info("REGAL_EMU_OBJ            ", enableEmuObj     ? "enabled" : "disabled");
    Info("REGAL_EMU_BIN            ", enableEmuBin     ? "enabled" : "disabled");
    Info("REGAL_EMU_DSA            ", enableEmuDsa     ? "enabled" : "disabled");
    Info("REGAL_EMU_IFF            ", enableEmuIff     ? "enabled" : "disabled");
    Info("REGAL_EMU_SO             ", enableEmuSo      ? "enabled" : "disabled");
    Info("REGAL_EMU_VAO            ", enableEmuVao     ? "enabled" : "disabled");
    Info("REGAL_EMU_FILTER         ", enableEmuFilter  ? "enabled" : "disabled");
    Info("REGAL_EMU_TEXC           ", enableEmuTexC    ? "enabled" : "disabled");

    Info("REGAL_FRAME_LIMIT        ", frameLimit                               );

    Info("REGAL_MD5_COLOR          ", frameMd5Color    ? "enabled" : "disabled");
    Info("REGAL_MD5_STENCIL        ", frameMd5Stencil  ? "enabled" : "disabled");
    Info("REGAL_MD5_DEPTH          ", frameMd5Depth    ? "enabled" : "disabled");

    Info("REGAL_SAVE_COLOR         ", frameSaveColor   ? "enabled" : "disabled");
    Info("REGAL_SAVE_STENCIL       ", frameSaveStencil ? "enabled" : "disabled");
    Info("REGAL_SAVE_DEPTH         ", frameSaveDepth   ? "enabled" : "disabled");
  }

}

REGAL_NAMESPACE_END
