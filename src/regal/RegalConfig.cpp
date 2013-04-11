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

#include <boost/print/json.hpp>

#include "RegalLog.h"
#include "RegalConfig.h"
#include "RegalSystem.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Json { struct Output : public ::boost::print::json::output<std::string> {}; }

namespace Config {

  ::std::string configFile("");  // Don't read/write configuration by default

  bool forceES1Profile  = REGAL_FORCE_ES1_PROFILE;
  bool forceES2Profile  = REGAL_FORCE_ES2_PROFILE;
  bool forceCoreProfile = REGAL_FORCE_CORE_PROFILE;
  bool sysES1           = REGAL_SYS_ES1;
  bool sysES2           = REGAL_SYS_ES2;
  bool sysGL            = REGAL_SYS_GL;
  bool sysGLX           = REGAL_SYS_GLX;
  bool sysEGL           = REGAL_SYS_EGL && !REGAL_SYS_GLX;
  bool forceEmulation   = REGAL_FORCE_EMULATION;
  bool enableEmulation  = REGAL_EMULATION;
  bool enableTrace      = false;
  bool enableDebug      = false;
  bool enableError      = false;
  bool enableCode       = false;
  bool enableLog        = REGAL_LOG;
  bool enableDriver     = REGAL_DRIVER;

  bool enableEmuPpa     = REGAL_EMU_PPA;
  bool enableEmuPpca    = REGAL_EMU_PPCA;
  bool enableEmuObj     = REGAL_EMU_OBJ;
  bool enableEmuBin     = REGAL_EMU_BIN;
  bool enableEmuTexSto  = REGAL_EMU_TEXSTO;
  bool enableEmuXfer    = REGAL_EMU_XFER;
  bool enableEmuDsa     = REGAL_EMU_DSA;
  bool enableEmuIff     = REGAL_EMU_IFF;
  bool enableEmuSo      = REGAL_EMU_SO;
  bool enableEmuVao     = REGAL_EMU_VAO;
  bool enableEmuFilter  = REGAL_EMU_FILTER;
  bool enableEmuTexC    = REGAL_EMU_TEXC;

  bool forceEmuPpa      = REGAL_FORCE_EMU_PPA;
  bool forceEmuPpca     = REGAL_FORCE_EMU_PPCA;
  bool forceEmuObj      = REGAL_FORCE_EMU_OBJ;
  bool forceEmuBin      = REGAL_FORCE_EMU_BIN;
  bool forceEmuTexSto   = REGAL_FORCE_EMU_TEXSTO;
  bool forceEmuXfer     = REGAL_FORCE_EMU_XFER;
  bool forceEmuDsa      = REGAL_FORCE_EMU_DSA;
  bool forceEmuIff      = REGAL_FORCE_EMU_IFF;
  bool forceEmuSo       = REGAL_FORCE_EMU_SO;
  bool forceEmuVao      = REGAL_FORCE_EMU_VAO;
  bool forceEmuFilter   = REGAL_FORCE_EMU_FILTER;
  bool forceEmuTexC     = REGAL_FORCE_EMU_TEXC;

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

  bool          cache             = REGAL_CACHE;
  bool          cacheShader       = false;
  bool          cacheShaderRead   = false;
  bool          cacheShaderWrite  = false;
  bool          cacheTexture      = false;
  bool          cacheTextureRead  = false;
  bool          cacheTextureWrite = false;
  ::std::string cacheDirectory("./");

#if REGAL_CODE
  ::std::string codeSourceFile("code.cpp");
  ::std::string codeHeaderFile("code.h");
#else
  ::std::string codeSourceFile;
  ::std::string codeHeaderFile;
#endif

  bool          enableThreadLocking = REGAL_THREAD_LOCKING;

  void Init()
  {
    Internal("Config::Init","()");

#ifndef REGAL_NO_GETENV
    const char *tmp;

#if !REGAL_FORCE_ES1_PROFILE
    tmp = GetEnv( "REGAL_FORCE_ES1_PROFILE" );
    if (tmp) forceES1Profile = atoi(tmp)!=0;
#endif

#if !REGAL_FORCE_ES2_PROFILE
    tmp = GetEnv( "REGAL_FORCE_ES2_PROFILE" );
    if (tmp) forceES2Profile = atoi(tmp)!=0;
#endif

#if !REGAL_FORCE_CORE_PROFILE
    tmp = GetEnv( "REGAL_FORCE_CORE_PROFILE" );
    if (tmp) forceCoreProfile = atoi(tmp)!=0;
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

#if REGAL_TRACE
    tmp = GetEnv( "REGAL_TRACE" );
    if (tmp) enableTrace = atoi(tmp)!=0;
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

#if REGAL_EMU_TEXSTO
    tmp = GetEnv( "REGAL_EMU_TEXSTO" );
    if (tmp) enableEmuTexSto = atoi(tmp)!=0;
#endif

#if REGAL_EMU_XFER
    tmp = GetEnv( "REGAL_EMU_XFER" );
    if (tmp) enableEmuXfer = atoi(tmp)!=0;
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

#if REGAL_EMU_FILTER
    tmp = GetEnv( "REGAL_EMU_FILTER" );
    if (tmp) enableEmuFilter = atoi(tmp)!=0;
#endif

    //

#if REGAL_EMU_PPA
    tmp = GetEnv( "REGAL_FORCE_EMU_PPA" );
    if (tmp) forceEmuPpa = atoi(tmp)!=0;
#endif

#if REGAL_EMU_PPCA
    tmp = GetEnv( "REGAL_FORCE_EMU_PPCA" );
    if (tmp) forceEmuPpca = atoi(tmp)!=0;
#endif

#if REGAL_EMU_OBJ
    tmp = GetEnv( "REGAL_FORCE_EMU_OBJ" );
    if (tmp) forceEmuObj = atoi(tmp)!=0;
#endif

#if REGAL_EMU_BIN
    tmp = GetEnv( "REGAL_FORCE_EMU_BIN" );
    if (tmp) forceEmuBin = atoi(tmp)!=0;
#endif

#if REGAL_EMU_TEXSTO
    tmp = GetEnv( "REGAL_FORCE_EMU_TEXSTO" );
    if (tmp) forceEmuTexSto = atoi(tmp)!=0;
#endif

#if REGAL_EMU_XFER
    tmp = GetEnv( "REGAL_FORCE_EMU_XFER" );
    if (tmp) forceEmuXfer = atoi(tmp)!=0;
#endif

#if REGAL_EMU_DSA
    tmp = GetEnv( "REGAL_FORCE_EMU_DSA" );
    if (tmp) forceEmuDsa = atoi(tmp)!=0;
#endif

#if REGAL_EMU_IFF
    tmp = GetEnv( "REGAL_FORCE_EMU_IFF" );
    if (tmp) forceEmuIff = atoi(tmp)!=0;
#endif

#if REGAL_EMU_SO
    tmp = GetEnv( "REGAL_FORCE_EMU_SO" );
    if (tmp) forceEmuSo = atoi(tmp)!=0;
#endif

#if REGAL_EMU_VAO
    tmp = GetEnv( "REGAL_FORCE_EMU_VAO" );
    if (tmp) forceEmuVao = atoi(tmp)!=0;
#endif

#if REGAL_EMU_TEXC
    tmp = GetEnv( "REGAL_FORCE_EMU_TEXC" );
    if (tmp) forceEmuTexC = atoi(tmp)!=0;
#endif

#if REGAL_EMU_FILTER
    tmp = GetEnv( "REGAL_FORCE_EMU_FILTER" );
    if (tmp) forceEmuFilter = atoi(tmp)!=0;
#endif

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

    // GLSL shader caching

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

    // Teture caching

#if REGAL_CACHE_TEXTURE
    tmp = GetEnv( "REGAL_CACHE_TEXTURE" );
    if (tmp) cacheTexture = atoi(tmp)!=0;
#endif

#if REGAL_CACHE_TEXTURE_WRITE
    tmp = GetEnv( "REGAL_CACHE_TEXTURE_WRITE" );
    if (tmp) cacheTextureWrite = atoi(tmp)!=0;
#endif

#if REGAL_CACHE_TEXTURE_READ
    tmp = GetEnv( "REGAL_CACHE_TEXTURE_READ" );
    if (tmp) cacheTextureRead = atoi(tmp)!=0;
#endif

    tmp = GetEnv( "REGAL_CACHE_DIRECTORY" );
    if (tmp) cacheDirectory = tmp;
#endif

#if REGAL_CODE
    tmp = GetEnv( "REGAL_CODE_SOURCE" );
    if (tmp) codeSourceFile = tmp;

    tmp = GetEnv( "REGAL_CODE_HEADER" );
    if (tmp) codeHeaderFile = tmp;
#endif

#endif

#if REGAL_THREAD_LOCKING
    tmp = GetEnv( "REGAL_THREAD_LOCKING" );
    if (tmp) enableThreadLocking = atoi(tmp)!=0;
#else
    enableThreadLocking = false;
#endif

    // REGAL_NO_EMULATION is deprecated, use REGAL_EMULATION=0 instead.

#if REGAL_EMULATION && defined(REGAL_NO_EMULATION) && REGAL_NO_EMULATION
    enableEmulation = false;
#endif

#if REGAL_SYS_ES1
    Info("REGAL_FORCE_ES1_PROFILE   ", forceES1Profile  ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_FORCE_ES2_PROFILE   ", forceES2Profile  ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_CORE_PROFILE  ", forceCoreProfile ? "enabled" : "disabled");

#if REGAL_SYS_ES1
    Info("REGAL_SYS_ES1             ", sysES1           ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_SYS_ES2             ", sysES2           ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GL
    Info("REGAL_SYS_GL              ", sysGL            ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GLX
    Info("REGAL_SYS_GLX             ", sysGLX           ? "enabled" : "disabled");
#endif

#if REGAL_SYS_EGL
    Info("REGAL_SYS_EGL             ", sysEGL           ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_EMULATION     ", forceEmulation   ? "enabled" : "disabled");
    Info("REGAL_TRACE               ", enableTrace      ? "enabled" : "disabled");
    Info("REGAL_DEBUG               ", enableDebug      ? "enabled" : "disabled");
    Info("REGAL_ERROR               ", enableError      ? "enabled" : "disabled");
#if REGAL_CODE
    Info("REGAL_CODE                ", enableCode       ? "enabled" : "disabled");
#endif
    Info("REGAL_EMULATION           ", enableEmulation  ? "enabled" : "disabled");
    Info("REGAL_LOG                 ", enableLog        ? "enabled" : "disabled");
    Info("REGAL_DRIVER              ", enableDriver     ? "enabled" : "disabled");

    Info("REGAL_EMU_PPA             ", enableEmuPpa     ? "enabled" : "disabled");
    Info("REGAL_EMU_PPCA            ", enableEmuPpca    ? "enabled" : "disabled");
    Info("REGAL_EMU_OBJ             ", enableEmuObj     ? "enabled" : "disabled");
    Info("REGAL_EMU_BIN             ", enableEmuBin     ? "enabled" : "disabled");
    Info("REGAL_EMU_TEXSTO          ", enableEmuTexSto  ? "enabled" : "disabled");
    Info("REGAL_EMU_XFER            ", enableEmuXfer    ? "enabled" : "disabled");
    Info("REGAL_EMU_DSA             ", enableEmuDsa     ? "enabled" : "disabled");
    Info("REGAL_EMU_IFF             ", enableEmuIff     ? "enabled" : "disabled");
    Info("REGAL_EMU_SO              ", enableEmuSo      ? "enabled" : "disabled");
    Info("REGAL_EMU_VAO             ", enableEmuVao     ? "enabled" : "disabled");
    Info("REGAL_EMU_FILTER          ", enableEmuFilter  ? "enabled" : "disabled");
    Info("REGAL_EMU_TEXC            ", enableEmuTexC    ? "enabled" : "disabled");

    Info("REGAL_FORCE_EMU_PPA       ", forceEmuPpa      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_PPCA      ", forceEmuPpca     ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_OBJ       ", forceEmuObj      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_BIN       ", forceEmuBin      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_TEXSTO    ", forceEmuTexSto   ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_XFER      ", forceEmuXfer     ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_DSA       ", forceEmuDsa      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_IFF       ", forceEmuIff      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_SO        ", forceEmuSo       ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_VAO       ", forceEmuVao      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_FILTER    ", forceEmuFilter   ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_TEXC      ", forceEmuTexC     ? "enabled" : "disabled");

    Info("REGAL_FRAME_LIMIT         ", frameLimit                               );

    Info("REGAL_MD5_COLOR           ", frameMd5Color    ? "enabled" : "disabled");
    Info("REGAL_MD5_STENCIL         ", frameMd5Stencil  ? "enabled" : "disabled");
    Info("REGAL_MD5_DEPTH           ", frameMd5Depth    ? "enabled" : "disabled");

    Info("REGAL_SAVE_COLOR          ", frameSaveColor   ? "enabled" : "disabled");
    Info("REGAL_SAVE_STENCIL        ", frameSaveStencil ? "enabled" : "disabled");
    Info("REGAL_SAVE_DEPTH          ", frameSaveDepth   ? "enabled" : "disabled");

#if REGAL_CACHE
    Info("REGAL_CACHE               ", cache             ? "enabled" : "disabled");
    Info("REGAL_CACHE_TEXTURE       ", cacheTexture      ? "enabled" : "disabled");
    Info("REGAL_CACHE_TEXTURE_WRITE ", cacheTextureWrite ? "enabled" : "disabled");
#endif

#if REGAL_CODE
    Info("REGAL_CODE_SOURCE         ", codeSourceFile                            );
    Info("REGAL_CODE_HEADER         ", codeHeaderFile                            );
#endif

    Info("REGAL_THREAD_LOCKING      ", enableThreadLocking ? "enabled" : "disabled");
  }

  void
  writeJSON(Json::Output &jo)
  {
#if !REGAL_NO_JSON
    jo.object("config");

      jo.member("configFile", configFile);

      jo.object("system");
        jo.member("ES1", sysES1);
        jo.member("ES2", sysES2);
        jo.member("GL",  sysGL);
        jo.member("GLX", sysGLX);
        jo.member("EGL", sysEGL);
      jo.end();

      jo.object("force");
        jo.member("ES1",  forceES1Profile);
        jo.member("ES2",  forceES2Profile);
        jo.member("Core", forceCoreProfile);
      jo.end();

      jo.object("dispatch");

        jo.object("enable");
          jo.member("debug",     enableDebug);
          jo.member("error",     enableError);
          jo.member("code",      enableCode);
          jo.member("emulation", enableEmulation);
          jo.member("log",       enableLog);
          jo.member("driver",    enableDriver);
        jo.end();

        jo.object("force");
          jo.member("emulation", forceEmulation);
        jo.end();

        jo.object("emulation");

          jo.object("enable");
            jo.member("ppa",    enableEmuPpa);
            jo.member("ppca",   enableEmuPpca);
            jo.member("obj",    enableEmuObj);
            jo.member("bin",    enableEmuBin);
            jo.member("texsto", enableEmuTexSto);
            jo.member("xfer",   enableEmuXfer);
            jo.member("dsa",    enableEmuDsa);
            jo.member("iff",    enableEmuIff);
            jo.member("so",     enableEmuSo);
            jo.member("vao",    enableEmuVao);
            jo.member("texc",   enableEmuTexC);
            jo.member("filter", enableEmuFilter);
          jo.end();

          jo.object("force");
            jo.member("ppa",    forceEmuPpa);
            jo.member("ppca",   forceEmuPpca);
            jo.member("obj",    forceEmuObj);
            jo.member("bin",    forceEmuBin);
            jo.member("texsto", forceEmuTexSto);
            jo.member("xfer",   forceEmuXfer);
            jo.member("dsa",    forceEmuDsa);
            jo.member("iff",    forceEmuIff);
            jo.member("so",     forceEmuSo);
            jo.member("vao",    forceEmuVao);
            jo.member("texc",   forceEmuTexC);
            jo.member("filter", forceEmuFilter);
          jo.end();

        jo.end();

      jo.end();

      jo.object("frame");
        jo.member("limit",     frameLimit);
        jo.object("md5");
          jo.member("color",   frameMd5Color);
          jo.member("stencil", frameMd5Stencil);
          jo.member("depth",   frameMd5Depth);
          jo.object("mask");
            jo.member("color",   frameMd5ColorMask);
            jo.member("stencil", frameMd5StencilMask);
            jo.member("depth",   frameMd5DepthMask);
          jo.end();
        jo.end();
        jo.object("save");
          jo.member("color",   frameSaveColor);
          jo.member("stencil", frameSaveStencil);
          jo.member("depth",   frameSaveDepth);
        jo.end();
      jo.end();

      jo.object("cache");
        jo.member("enable",       cache);
        jo.member("shader",       cacheShader);
        jo.member("shaderWrite",  cacheShaderWrite);
        jo.member("shaderRead",   cacheShaderRead);
        jo.member("texture",      cacheShader);
        jo.member("textureWrite", cacheShaderWrite);
        jo.member("textureRead",  cacheShaderRead);
        jo.member("directory",    cacheDirectory);
      jo.end();

    jo.end();
#endif // !REGAL_NO_JSON
  }
}

REGAL_NAMESPACE_END
