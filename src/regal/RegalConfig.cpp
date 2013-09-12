/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
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

  bool forceES1Profile     = REGAL_FORCE_ES1_PROFILE;
  bool forceES2Profile     = REGAL_FORCE_ES2_PROFILE;
  bool forceCoreProfile    = REGAL_FORCE_CORE_PROFILE;
  bool sysES1              = REGAL_SYS_ES1;
  bool sysES2              = REGAL_SYS_ES2;
  bool sysGL               = REGAL_SYS_GL;
  bool sysGLX              = REGAL_SYS_GLX;
  bool sysEGL              = REGAL_SYS_EGL && !REGAL_SYS_GLX;
  bool forceEmulation      = REGAL_FORCE_EMULATION;
  bool enableEmulation     = REGAL_EMULATION;
  bool enableTrace         = false;
  bool enableDebug         = false;
  bool enableError         = false;
  bool enableCode          = false;
  bool enableStatistics    = false;
  bool enableLog           = REGAL_LOG;
  bool enableDriver        = REGAL_DRIVER;
  bool enableMissing       = REGAL_MISSING;

  bool enableEmuHint       = REGAL_EMU_HINT;
  bool enableEmuPpa        = REGAL_EMU_PPA;
  bool enableEmuPpca       = REGAL_EMU_PPCA;
  bool enableEmuObj        = REGAL_EMU_OBJ;
  bool enableEmuBin        = REGAL_EMU_BIN;
  bool enableEmuTexSto     = REGAL_EMU_TEXSTO;
  bool enableEmuXfer       = REGAL_EMU_XFER;
  bool enableEmuDsa        = REGAL_EMU_DSA;
  bool enableEmuPath       = REGAL_EMU_PATH;
  bool enableEmuRect       = REGAL_EMU_RECT;
  bool enableEmuBaseVertex = REGAL_EMU_BASEVERTEX;
  bool enableEmuIff        = REGAL_EMU_IFF;
  bool enableEmuSo         = REGAL_EMU_SO;
  bool enableEmuVao        = REGAL_EMU_VAO;
  bool enableEmuFilter     = REGAL_EMU_FILTER;
  bool enableEmuTexC       = REGAL_EMU_TEXC;

  bool forceEmuHint        = REGAL_FORCE_EMU_HINT;
  bool forceEmuPpa         = REGAL_FORCE_EMU_PPA;
  bool forceEmuPpca        = REGAL_FORCE_EMU_PPCA;
  bool forceEmuObj         = REGAL_FORCE_EMU_OBJ;
  bool forceEmuBin         = REGAL_FORCE_EMU_BIN;
  bool forceEmuTexSto      = REGAL_FORCE_EMU_TEXSTO;
  bool forceEmuXfer        = REGAL_FORCE_EMU_XFER;
  bool forceEmuDsa         = REGAL_FORCE_EMU_DSA;
  bool forceEmuPath        = REGAL_FORCE_EMU_PATH;
  bool forceEmuRect        = REGAL_FORCE_EMU_RECT;
  bool forceEmuBaseVertex  = REGAL_FORCE_EMU_BASEVERTEX;
  bool forceEmuIff         = REGAL_FORCE_EMU_IFF;
  bool forceEmuSo          = REGAL_FORCE_EMU_SO;
  bool forceEmuVao         = REGAL_FORCE_EMU_VAO;
  bool forceEmuFilter      = REGAL_FORCE_EMU_FILTER;
  bool forceEmuTexC        = REGAL_FORCE_EMU_TEXC;

  int  frameLimit          = 0;  // Unlimited

  bool frameMd5Color       = false;
  bool frameMd5Stencil     = false;
  bool frameMd5Depth       = false;

  unsigned char frameMd5ColorMask   = std::numeric_limits<unsigned char>::max();
  unsigned char frameMd5StencilMask = std::numeric_limits<unsigned char>::max();
  size_t        frameMd5DepthMask   = std::numeric_limits<size_t       >::max();

  bool frameSaveColor             = false;
  bool frameSaveStencil           = false;
  bool frameSaveDepth             = false;

  ::std::string frameSaveColorPrefix  ("color_");
  ::std::string frameSaveStencilPrefix("stencil_");
  ::std::string frameSaveDepthPrefix  ("depth_");

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

  ::std::string traceFile;

  bool          enableThreadLocking = REGAL_THREAD_LOCKING;

  void Init()
  {
    Internal("Config::Init","()");

#ifndef REGAL_NO_GETENV
    getEnv( "REGAL_FORCE_ES1_PROFILE",  forceES1Profile,  !REGAL_FORCE_ES1_PROFILE  );
    getEnv( "REGAL_FORCE_ES2_PROFILE",  forceES2Profile,  !REGAL_FORCE_ES2_PROFILE  );
    getEnv( "REGAL_FORCE_CORE_PROFILE", forceCoreProfile, !REGAL_FORCE_CORE_PROFILE );

    // With REGAL_SYS_GLX && REGAL_SYS_EGL
    // we infer each from other, if specified,
    // to behave as a toggle.

    const char *tmp;

#if REGAL_SYS_GLX
    tmp = getEnv( "REGAL_SYS_GLX" );
    if (tmp)
    {
      sysGLX = atoi(tmp)!=0;
#if REGAL_SYS_EGL
      sysEGL = !sysGLX;
#endif
    }
#endif

#if REGAL_SYS_EGL
    tmp = getEnv( "REGAL_SYS_EGL" );
    if (tmp)
    {
      sysEGL = atoi(tmp)!=0;
#if REGAL_SYS_GLX
      sysGLX = !sysEGL;
#endif
     }
#endif

    // Default to GLX, if necessary
    //
    // This situation can arise if REGAL_SYS_GLX and REGAL_SYS_EGL environment variables
    // are unset, or via JSON settings.

#if REGAL_SYS_GLX && REGAL_SYS_EGL
    if (sysGLX && sysEGL)
      sysEGL = false;
#endif

    // Default to GL, if necessary
    //
    // This situation can arise if REGAL_SYS_GL and REGAL_SYS_ES2 environment variables
    // are unset, or via JSON settings.

#if REGAL_SYS_GL && REGAL_SYS_ES2
    if (sysGL && sysES2)
      sysES2 = false;
#endif

    getEnv( "REGAL_FORCE_EMULATION", forceEmulation, !REGAL_FORCE_EMULATION);
    getEnv( "REGAL_EMULATION",       enableEmulation, REGAL_EMULATION);

    // Deprecated

    tmp = getEnv( "REGAL_NO_EMULATION" );
    if (tmp) enableEmulation = atoi(tmp)==0;

    getEnv( "REGAL_DEBUG",      enableDebug,      REGAL_DEBUG);
    getEnv( "REGAL_TRACE",      enableTrace,      REGAL_TRACE);
    getEnv( "REGAL_ERROR",      enableError,      REGAL_ERROR);
    getEnv( "REGAL_CODE",       enableCode,       REGAL_CODE);
    getEnv( "REGAL_STATISTICS", enableStatistics, REGAL_STATISTICS);
    getEnv( "REGAL_LOG",        enableLog,        REGAL_LOG);
    getEnv( "REGAL_DRIVER",     enableDriver,     REGAL_DRIVER);
    getEnv( "REGAL_MISSING",    enableMissing,    REGAL_MISSING);

    getEnv( "REGAL_EMU_HINT",       enableEmuHint,       REGAL_EMU_HINT);
    getEnv( "REGAL_EMU_PPA",        enableEmuPpa,        REGAL_EMU_PPA);
    getEnv( "REGAL_EMU_PPCA",       enableEmuPpca,       REGAL_EMU_PPCA);
    getEnv( "REGAL_EMU_OBJ",        enableEmuObj,        REGAL_EMU_OBJ);
    getEnv( "REGAL_EMU_BIN",        enableEmuBin,        REGAL_EMU_BIN);
    getEnv( "REGAL_EMU_TEXSTO",     enableEmuTexSto,     REGAL_EMU_TEXSTO);
    getEnv( "REGAL_EMU_XFER",       enableEmuXfer,       REGAL_EMU_XFER);
    getEnv( "REGAL_EMU_DSA",        enableEmuDsa,        REGAL_EMU_DSA);
    getEnv( "REGAL_EMU_PATH",       enableEmuPath,       REGAL_EMU_PATH);
    getEnv( "REGAL_EMU_RECT",       enableEmuRect,       REGAL_EMU_RECT);
    getEnv( "REGAL_EMU_BASEVERTEX", enableEmuBaseVertex, REGAL_EMU_BASEVERTEX );
    getEnv( "REGAL_EMU_IFF",        enableEmuIff,        REGAL_EMU_IFF);
    getEnv( "REGAL_EMU_SO",         enableEmuSo,         REGAL_EMU_SO);
    getEnv( "REGAL_EMU_VAO",        enableEmuVao,        REGAL_EMU_VAO);
    getEnv( "REGAL_EMU_TEXC",       enableEmuTexC,       REGAL_EMU_TEXC);
    getEnv( "REGAL_EMU_FILTER",     enableEmuFilter,     REGAL_EMU_FILTER);

    getEnv( "REGAL_FORCE_EMU_HINT",       forceEmuHint,        REGAL_EMU_HINT       && !REGAL_FORCE_EMU_HINT);
    getEnv( "REGAL_FORCE_EMU_PPA",        forceEmuPpa,         REGAL_EMU_PPA        && !REGAL_FORCE_EMU_PPA);
    getEnv( "REGAL_FORCE_EMU_PPCA",       forceEmuPpca,        REGAL_EMU_PPCA       && !REGAL_FORCE_EMU_PPCA);
    getEnv( "REGAL_FORCE_EMU_OBJ",        forceEmuObj,         REGAL_EMU_OBJ        && !REGAL_FORCE_EMU_OBJ);
    getEnv( "REGAL_FORCE_EMU_BIN",        forceEmuBin,         REGAL_EMU_BIN        && !REGAL_FORCE_EMU_BIN);
    getEnv( "REGAL_FORCE_EMU_TEXSTO",     forceEmuTexSto,      REGAL_EMU_TEXSTO     && !REGAL_FORCE_EMU_TEXSTO);
    getEnv( "REGAL_FORCE_EMU_XFER",       forceEmuXfer,        REGAL_EMU_XFER       && !REGAL_FORCE_EMU_XFER);
    getEnv( "REGAL_FORCE_EMU_DSA",        forceEmuDsa,         REGAL_EMU_DSA        && !REGAL_FORCE_EMU_DSA);
    getEnv( "REGAL_FORCE_EMU_PATH",       forceEmuPath,        REGAL_EMU_PATH       && !REGAL_FORCE_EMU_PATH);
    getEnv( "REGAL_FORCE_EMU_RECT",       forceEmuRect,        REGAL_EMU_RECT       && !REGAL_FORCE_EMU_RECT);
    getEnv( "REGAL_FORCE_EMU_BASEVERTEX", forceEmuBaseVertex,  REGAL_EMU_BASEVERTEX && !REGAL_FORCE_EMU_BASEVERTEX);
    getEnv( "REGAL_FORCE_EMU_IFF",        forceEmuIff,         REGAL_EMU_IFF        && !REGAL_FORCE_EMU_IFF);
    getEnv( "REGAL_FORCE_EMU_SO",         forceEmuSo,          REGAL_EMU_SO         && !REGAL_FORCE_EMU_SO);
    getEnv( "REGAL_FORCE_EMU_VAO",        forceEmuVao,         REGAL_EMU_VAO        && !REGAL_FORCE_EMU_VAO);
    getEnv( "REGAL_FORCE_EMU_TEXC",       forceEmuTexC,        REGAL_EMU_TEXC       && !REGAL_FORCE_EMU_TEXC);
    getEnv( "REGAL_FORCE_EMU_FILTER",     forceEmuFilter,      REGAL_EMU_FILTER     && !REGAL_FORCE_EMU_FILTER);

    //

    getEnv( "REGAL_FRAME_LIMIT",      frameLimit);

    //

    getEnv( "REGAL_MD5_COLOR",        frameMd5Color);
    getEnv( "REGAL_MD5_STENCIL",      frameMd5Stencil);
    getEnv( "REGAL_MD5_DEPTH",        frameMd5Depth);
    getEnv( "REGAL_MD5_COLOR_MASK",   frameMd5ColorMask);
    getEnv( "REGAL_MD5_STENCIL_MASK", frameMd5StencilMask);
    getEnv( "REGAL_MD5_DEPTH_MASK",   frameMd5DepthMask);

    //

    getEnv( "REGAL_SAVE_COLOR",   frameSaveColor);
    getEnv( "REGAL_SAVE_STENCIL", frameSaveStencil);
    getEnv( "REGAL_SAVE_DEPTH",   frameSaveDepth);

    // Caching

#if REGAL_CACHE
    getEnv( "REGAL_CACHE", cache );

    // GLSL shader caching

    getEnv( "REGAL_CACHE_SHADER",       cacheShader,      REGAL_CACHE_SHADER);
    getEnv( "REGAL_CACHE_SHADER_WRITE", cacheShaderWrite, REGAL_CACHE_SHADER_WRITE);
    getEnv( "REGAL_CACHE_SHADER_READ",  cacheShaderRead,  REGAL_CACHE_SHADER_READ);

    // Teture caching

    getEnv( "REGAL_CACHE_TEXTURE",       cacheTexture,      REGAL_CACHE_TEXTURE);
    getEnv( "REGAL_CACHE_TEXTURE_WRITE", cacheTextureWrite, REGAL_CACHE_TEXTURE_WRITE);
    getEnv( "REGAL_CACHE_TEXTURE_READ",  cacheTextureRead,  REGAL_CACHE_TEXTURE_READ);

    getEnv( "REGAL_CACHE_DIRECTORY", cacheDirectory );
#endif

    getEnv( "REGAL_CODE_SOURCE", codeSourceFile, REGAL_CODE);
    getEnv( "REGAL_CODE_HEADER", codeHeaderFile, REGAL_CODE);

    getEnv( "REGAL_TRACE_FILE", traceFile, REGAL_TRACE);

#if REGAL_THREAD_LOCKING
    getEnv( "REGAL_THREAD_LOCKING", enableThreadLocking );
#else
    enableThreadLocking = false;
#endif

#endif

    // REGAL_NO_EMULATION is deprecated, use REGAL_EMULATION=0 instead.

#if REGAL_EMULATION && defined(REGAL_NO_EMULATION) && REGAL_NO_EMULATION
    enableEmulation = false;
#endif

#if REGAL_SYS_ES1
    Info("REGAL_FORCE_ES1_PROFILE   ", forceES1Profile     ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_FORCE_ES2_PROFILE   ", forceES2Profile     ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_CORE_PROFILE  ", forceCoreProfile    ? "enabled" : "disabled");

#if REGAL_SYS_ES1
    Info("REGAL_SYS_ES1             ", sysES1              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_ES2
    Info("REGAL_SYS_ES2             ", sysES2              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GL
    Info("REGAL_SYS_GL              ", sysGL               ? "enabled" : "disabled");
#endif

#if REGAL_SYS_GLX
    Info("REGAL_SYS_GLX             ", sysGLX              ? "enabled" : "disabled");
#endif

#if REGAL_SYS_EGL
    Info("REGAL_SYS_EGL             ", sysEGL              ? "enabled" : "disabled");
#endif

    Info("REGAL_FORCE_EMULATION     ", forceEmulation      ? "enabled" : "disabled");
#if REGAL_TRACE
    Info("REGAL_TRACE               ", enableTrace         ? "enabled" : "disabled");
#endif
    Info("REGAL_DEBUG               ", enableDebug         ? "enabled" : "disabled");
    Info("REGAL_ERROR               ", enableError         ? "enabled" : "disabled");
#if REGAL_CODE
    Info("REGAL_CODE                ", enableCode          ? "enabled" : "disabled");
#endif
#if REGAL_STATISTICS
    Info("REGAL_STATISTICS          ", enableStatistics    ? "enabled" : "disabled");
#endif
    Info("REGAL_EMULATION           ", enableEmulation     ? "enabled" : "disabled");
    Info("REGAL_LOG                 ", enableLog           ? "enabled" : "disabled");
    Info("REGAL_DRIVER              ", enableDriver        ? "enabled" : "disabled");
    Info("REGAL_MISSING             ", enableMissing       ? "enabled" : "disabled");

    Info("REGAL_EMU_HINT            ", enableEmuHint       ? "enabled" : "disabled");
    Info("REGAL_EMU_PPA             ", enableEmuPpa        ? "enabled" : "disabled");
    Info("REGAL_EMU_PPCA            ", enableEmuPpca       ? "enabled" : "disabled");
    Info("REGAL_EMU_OBJ             ", enableEmuObj        ? "enabled" : "disabled");
    Info("REGAL_EMU_BIN             ", enableEmuBin        ? "enabled" : "disabled");
    Info("REGAL_EMU_TEXSTO          ", enableEmuTexSto     ? "enabled" : "disabled");
    Info("REGAL_EMU_XFER            ", enableEmuXfer       ? "enabled" : "disabled");
    Info("REGAL_EMU_DSA             ", enableEmuDsa        ? "enabled" : "disabled");
    Info("REGAL_EMU_PATH            ", enableEmuPath       ? "enabled" : "disabled");
    Info("REGAL_EMU_RECT            ", enableEmuRect       ? "enabled" : "disabled");
    Info("REGAL_EMU_BASEVERTEX      ", enableEmuBaseVertex ? "enabled" : "disabled");
    Info("REGAL_EMU_IFF             ", enableEmuIff        ? "enabled" : "disabled");
    Info("REGAL_EMU_SO              ", enableEmuSo         ? "enabled" : "disabled");
    Info("REGAL_EMU_VAO             ", enableEmuVao        ? "enabled" : "disabled");
    Info("REGAL_EMU_FILTER          ", enableEmuFilter     ? "enabled" : "disabled");
    Info("REGAL_EMU_TEXC            ", enableEmuTexC       ? "enabled" : "disabled");

    Info("REGAL_FORCE_EMU_HINT      ", forceEmuHint        ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_PPA       ", forceEmuPpa         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_PPCA      ", forceEmuPpca        ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_OBJ       ", forceEmuObj         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_BIN       ", forceEmuBin         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_TEXSTO    ", forceEmuTexSto      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_XFER      ", forceEmuXfer        ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_DSA       ", forceEmuDsa         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_PATH      ", forceEmuPath        ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_RECT      ", forceEmuRect        ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_BASEVERTEX", forceEmuBaseVertex  ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_IFF       ", forceEmuIff         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_SO        ", forceEmuSo          ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_VAO       ", forceEmuVao         ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_FILTER    ", forceEmuFilter      ? "enabled" : "disabled");
    Info("REGAL_FORCE_EMU_TEXC      ", forceEmuTexC        ? "enabled" : "disabled");

    Info("REGAL_FRAME_LIMIT         ", frameLimit                                  );

    Info("REGAL_MD5_COLOR           ", frameMd5Color       ? "enabled" : "disabled");
    Info("REGAL_MD5_STENCIL         ", frameMd5Stencil     ? "enabled" : "disabled");
    Info("REGAL_MD5_DEPTH           ", frameMd5Depth       ? "enabled" : "disabled");

    Info("REGAL_SAVE_COLOR          ", frameSaveColor      ? "enabled" : "disabled");
    Info("REGAL_SAVE_STENCIL        ", frameSaveStencil    ? "enabled" : "disabled");
    Info("REGAL_SAVE_DEPTH          ", frameSaveDepth      ? "enabled" : "disabled");

#if REGAL_CACHE
    Info("REGAL_CACHE               ", cache               ? "enabled" : "disabled");
    Info("REGAL_CACHE_TEXTURE       ", cacheTexture        ? "enabled" : "disabled");
    Info("REGAL_CACHE_TEXTURE_WRITE ", cacheTextureWrite   ? "enabled" : "disabled");
#endif

#if REGAL_CODE
    Info("REGAL_CODE_SOURCE         ", codeSourceFile                              );
    Info("REGAL_CODE_HEADER         ", codeHeaderFile                              );
#endif

#if REGAL_TRACE
    Info("REGAL_TRACE_FILE          ", traceFile                                   );
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
          jo.member("debug",      enableDebug);
          jo.member("error",      enableError);
          jo.member("code",       enableCode);
          jo.member("statistics", enableStatistics);
          jo.member("emulation",  enableEmulation);
          jo.member("trace",      enableTrace);
          jo.member("log",        enableLog);
          jo.member("driver",     enableDriver);
          jo.member("missing",    enableMissing);
        jo.end();

        jo.object("force");
          jo.member("emulation", forceEmulation);
        jo.end();

        jo.object("emulation");

          jo.object("enable");
            jo.member("hint",   enableEmuHint);
            jo.member("ppa",    enableEmuPpa);
            jo.member("ppca",   enableEmuPpca);
            jo.member("obj",    enableEmuObj);
            jo.member("bin",    enableEmuBin);
            jo.member("texsto", enableEmuTexSto);
            jo.member("xfer",   enableEmuXfer);
            jo.member("dsa",    enableEmuDsa);
            jo.member("path",   enableEmuPath);
            jo.member("rect",   enableEmuRect);
            jo.member("bv",     enableEmuBaseVertex);
            jo.member("iff",    enableEmuIff);
            jo.member("so",     enableEmuSo);
            jo.member("vao",    enableEmuVao);
            jo.member("texc",   enableEmuTexC);
            jo.member("filter", enableEmuFilter);
          jo.end();

          jo.object("force");
            jo.member("hint",   forceEmuHint);
            jo.member("ppa",    forceEmuPpa);
            jo.member("ppca",   forceEmuPpca);
            jo.member("obj",    forceEmuObj);
            jo.member("bin",    forceEmuBin);
            jo.member("texsto", forceEmuTexSto);
            jo.member("xfer",   forceEmuXfer);
            jo.member("dsa",    forceEmuDsa);
            jo.member("path",   forceEmuPath);
            jo.member("rect",   forceEmuRect);
            jo.member("bv",     forceEmuBaseVertex);
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
          jo.object("enable");
            jo.member("color",   frameSaveColor);
            jo.member("stencil", frameSaveStencil);
            jo.member("depth",   frameSaveDepth);
          jo.end();
          jo.object("prefix");
            jo.member("color",   frameSaveColorPrefix);
            jo.member("stencil", frameSaveStencilPrefix);
            jo.member("depth",   frameSaveDepthPrefix);
          jo.end();
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

      jo.object("trace");
        jo.member("file",         traceFile);
      jo.end();

    jo.end();
#endif // !REGAL_NO_JSON
  }
}

REGAL_NAMESPACE_END
