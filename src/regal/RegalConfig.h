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

#ifndef __REGAL_CONFIG_H__
#define __REGAL_CONFIG_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Json { struct Output; }

namespace Config
{
  void Init();

  extern void writeJSON(Json::Output &jo);

  //

  extern ::std::string configFile;

  //

  extern bool forceES1Profile;
  extern bool forceES2Profile;
  extern bool forceCoreProfile;

  //

  extern bool sysES1;
  extern bool sysES2;
  extern bool sysGL;

  // Use GLX or EGL, but not both

  extern bool sysGLX;
  extern bool sysEGL;

  // Initial dispatch enable/disable state

  extern bool forceEmulation;
  extern bool enableEmulation;
  extern bool enableTrace;
  extern bool enableDebug;
  extern bool enableError;
  extern bool enableCode;
  extern bool enableLog;
  extern bool enableDriver;

  // Initial emulation layer enable/disable

  extern bool enableEmuPpa;
  extern bool enableEmuPpca;
  extern bool enableEmuObj;
  extern bool enableEmuBin;
  extern bool enableEmuTexSto;
  extern bool enableEmuXfer;
  extern bool enableEmuDsa;
  extern bool enableEmuIff;
  extern bool enableEmuSo;
  extern bool enableEmuVao;
  extern bool enableEmuFilter;
  extern bool enableEmuTexC;

  // Force emulation layer enable/disable

  extern bool forceEmuPpa;
  extern bool forceEmuPpca;
  extern bool forceEmuObj;
  extern bool forceEmuBin;
  extern bool forceEmuTexSto;
  extern bool forceEmuXfer;
  extern bool forceEmuDsa;
  extern bool forceEmuIff;
  extern bool forceEmuSo;
  extern bool forceEmuVao;
  extern bool forceEmuFilter;
  extern bool forceEmuTexC;

  // Initial context configuration

  extern int  frameLimit;        // Maximum number of frames

  extern bool frameMd5Color;     // Log md5 hash of color buffer
  extern bool frameMd5Stencil;
  extern bool frameMd5Depth;

  extern unsigned char frameMd5ColorMask;   // bitwise mask for md5 hashing purposes
  extern unsigned char frameMd5StencilMask; //
  extern size_t        frameMd5DepthMask;   //

  extern bool frameSaveColor;    // Save color buffer to PNG file
  extern bool frameSaveStencil;
  extern bool frameSaveDepth;

  // Caching

  extern bool          cache;
  extern bool          cacheShader;
  extern bool          cacheShaderRead;
  extern bool          cacheShaderWrite;
  extern bool          cacheTexture;
  extern bool          cacheTextureRead;
  extern bool          cacheTextureWrite;
  extern ::std::string cacheDirectory;

  // Code dispatch

  extern ::std::string codeSourceFile;
  extern ::std::string codeHeaderFile;

  // Thread locking

  extern bool          enableThreadLocking;
};

REGAL_NAMESPACE_END

#endif
