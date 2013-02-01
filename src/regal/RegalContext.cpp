/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.2 --api wgl 4.0 --api glx 4.0 --api cgl 1.4 --api egl 1.0 --outdir .
*/

/*
  Copyright (c) 2011 NVIDIA Corporation
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

/*
  Intended formatting conventions:
  $ astyle --style=allman --indent=spaces=2 --indent-switches
*/

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"

#include "RegalMarker.h"
#include "RegalFrame.h"
#if REGAL_EMULATION
#include "RegalObj.h"
#include "RegalPpa.h"
#include "RegalPpca.h"
#include "RegalBin.h"
#include "RegalDsa.h"
#include "RegalIff.h"
#include "RegalSo.h"
#include "RegalVao.h"
#include "RegalTexC.h"
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace Logging;

RegalContext::RegalContext()
: initialized(false),
  dispatcher(),
  dbg(NULL),
  info(NULL),
  marker(NULL),
  frame(NULL),
#if REGAL_EMULATION
  emuLevel(0),
  obj(NULL),
  ppa(NULL),
  ppca(NULL),
  bin(NULL),
  dsa(NULL),
  iff(NULL),
  so(NULL),
  vao(NULL),
  texc(NULL),
#endif
#if REGAL_SYS_PPAPI
  ppapiES2(NULL),
  ppapiResource(NULL),
#endif
  sysCtx(NULL),
  thread(0),
  logCallback(NULL),
  depthBeginEnd(0),
  depthPushMatrix(0),
  depthPushAttrib(0),
  depthNewList(0),
  codeInputNext(0),
  codeOutputNext(0)
{
  Internal("RegalContext::RegalContext","()");

  if (Config::enableDebug)
  {
    dbg = new DebugInfo();
    dbg->Init(this);
  }

  shareGroup.push_back(this);
}

void
RegalContext::Init()
{
  Internal("RegalContext::Init","()");

  RegalAssert(!initialized);

  info = new ContextInfo();
  RegalAssert(this);
  RegalAssert(info);
  info->init(*this);

  marker = new Marker;
  frame = new Frame;

#if REGAL_EMULATION
#if !REGAL_FORCE_EMULATION
  if
  (
    Config::forceEmulation  ||
    (
      Config::enableEmulation &&
      (
        info->core ||
        info->gles ||
        ( info->compat && !info->gl_ext_direct_state_access )
      )
    )
  )
#endif
  {
    RegalAssert(info);
    // emu
    emuLevel = 10;
    #if REGAL_EMU_TEXC
    if (Config::enableEmuTexC)
    {
      texc = new Emu::TexC;
      emuLevel = 0;
      texc->Init(*this);
    }
    #endif /* REGAL_EMU_TEXC */
    #if REGAL_EMU_VAO
    if (Config::enableEmuVao && Config::enableEmuIff)
    {
      vao = new RegalVao;
      emuLevel = 2;
      vao->Init(*this);
    }
    #endif /* REGAL_EMU_VAO */
    #if REGAL_EMU_SO
    if (Config::enableEmuSo)
    {
      so = new Emu::So;
      emuLevel = 3;
      so->Init(*this);
    }
    #endif /* REGAL_EMU_SO */
    #if REGAL_EMU_IFF
    if (Config::enableEmuIff)
    {
      iff = new Emu::Iff;
      emuLevel = 4;
      iff->Init(*this);
    }
    #endif /* REGAL_EMU_IFF */
    #if REGAL_EMU_DSA
    if (Config::enableEmuDsa)
    {
      Internal("RegalContext::Init ","GL_EXT_direct_state_access");
      info->regal_ext_direct_state_access = true;
      info->regalExtensionsSet.insert("GL_EXT_direct_state_access");
      info->regalExtensions = ::boost::print::detail::join(info->regalExtensionsSet,std::string(" "));
      dsa = new RegalDsa;
      emuLevel = 5;
      dsa->Init(*this);
    }
    #endif /* REGAL_EMU_DSA */
    #if REGAL_EMU_BIN
    if (Config::enableEmuBin)
    {
      bin = new RegalBin;
      emuLevel = 6;
      bin->Init(*this);
    }
    #endif /* REGAL_EMU_BIN */
    #if REGAL_EMU_PPCA
    if (Config::enableEmuPpca)
    {
      ppca = new Emu::Ppca;
      emuLevel = 7;
      ppca->Init(*this);
    }
    #endif /* REGAL_EMU_PPCA */
    #if REGAL_EMU_PPA
    if (Config::enableEmuPpa)
    {
      ppa = new Emu::Ppa;
      emuLevel = 8;
      ppa->Init(*this);
    }
    #endif /* REGAL_EMU_PPA */
    #if REGAL_EMU_OBJ
    if (Config::enableEmuObj)
    {
      obj = new RegalObj;
      emuLevel = 9;
      obj->Init(*this);
    }
    #endif /* REGAL_EMU_OBJ */
    emuLevel = 10;

  }
#endif

  initialized = true;
}

RegalContext::~RegalContext()
{
  Internal("RegalContext::~RegalContext","()");

  // Remove this context from the share group.

  shareGroup->remove(this);

  delete info;
  delete marker;
  delete frame;

#if REGAL_EMULATION
  // emu
  #if REGAL_EMU_OBJ
  delete obj;
  #endif /* REGAL_EMU_OBJ */
  #if REGAL_EMU_PPA
  delete ppa;
  #endif /* REGAL_EMU_PPA */
  #if REGAL_EMU_PPCA
  delete ppca;
  #endif /* REGAL_EMU_PPCA */
  #if REGAL_EMU_BIN
  delete bin;
  #endif /* REGAL_EMU_BIN */
  #if REGAL_EMU_DSA
  delete dsa;
  #endif /* REGAL_EMU_DSA */
  #if REGAL_EMU_IFF
  delete iff;
  #endif /* REGAL_EMU_IFF */
  #if REGAL_EMU_SO
  delete so;
  #endif /* REGAL_EMU_SO */
  #if REGAL_EMU_VAO
  delete vao;
  #endif /* REGAL_EMU_VAO */
  #if REGAL_EMU_TEXC
  delete texc;
  #endif /* REGAL_EMU_TEXC */
#endif
}

bool
RegalContext::groupInitialized() const
{
  Internal("RegalContext::groupInitialized","()");

  for (shared_list<RegalContext *>::const_iterator i = shareGroup.begin(); i!=shareGroup.end(); ++i)
  {
    RegalAssert(*i);
    if ((*i)->initialized)
      return true;
  }

  return false;
}

RegalContext *
RegalContext::groupInitializedContext()
{
  Internal("RegalContext::groupInitializedContext","()");

  // Look for any initialized context in the share group.
  // The only way this would be expected to fail is if none
  // of the contexts have been made current, triggering
  // initialization.
  //
  // Note - linear search, but shouldn't need to look at too many
  // contexts in the share group.

  for (shared_list<RegalContext *>::iterator i = shareGroup.begin(); i!=shareGroup.end(); ++i)
  {
    RegalAssert(*i);
    if ((*i)->initialized)
      return *i;
  }

  return NULL;
}

REGAL_NAMESPACE_END
