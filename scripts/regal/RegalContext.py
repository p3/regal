#!/usr/bin/python -B

from string import Template, upper, replace

from ApiUtil import outputCode
from ApiCodeGen import *

from EmuInit           import formulae       as initFormulae
from EmuInit           import formulaeGlobal as initFormulaeGlobal
from EmuContextState   import formulae       as contextStateFormulae
from EmuGetString      import formulae       as getStringFormulae
from EmuForceCore      import formulae       as forceCoreFormulae
from EmuLookup         import formulae       as lookupFormulae
from EmuMarker         import formulae       as markerFormulae
from EmuMarker         import formulaeGlobal as markerFormulaeGlobal
from EmuFrame          import formulae       as frameFormulae
from EmuFrame          import formulaeGlobal as frameFormulaeGlobal
from EmuExtensionQuery import formulae       as extensionQueryFormulae
from EmuErrorString    import formulae       as errorStringFormulae
from EmuEnable         import formulae       as enableFormulae
from EmuCache          import formulaeGlobal as cacheFormulaeGlobal

from EmuLog    import logFormulae

from Emu       import emuFindEntry, emuCodeGen
from EmuDsa    import dsaFormulae
from EmuVao    import vaoFormulae
from EmuSo     import soFormulae
from EmuPpc    import ppcFormulae
from EmuPpca   import ppcaFormulae
from EmuPpa    import ppaFormulae
from EmuIff    import iffFormulae
from EmuBin    import binFormulae
from EmuObj    import objFormulae
from EmuFilter import formulae as filterFormulae
from EmuTexC   import texCFormulae
from EmuTextureStorage import texstoFormulae

from EmuPixelTransfer import xferFormulae

# Regal.cpp emulation

emuRegal = [
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : initFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : initFormulaeGlobal },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : contextStateFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : getStringFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : forceCoreFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : lookupFormulae },
    { 'type' : 'Marker',   'include' : 'RegalMarker.h', 'member' : 'marker', 'conditional' : None,  'ifdef' : None,  'formulae' : markerFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : markerFormulaeGlobal },
    { 'type' : 'Frame',    'include' : 'RegalFrame.h',  'member' : 'frame',  'conditional' : None,  'ifdef' : None,  'formulae' : frameFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : frameFormulaeGlobal },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : extensionQueryFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : errorStringFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : logFormulae    },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : enableFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : cacheFormulaeGlobal },
]


# RegalDispathEmu.cpp fixed-function emulation

emu = [
    { 'type' : 'Emu::Obj',    'include' : 'RegalObj.h',    'member' : 'obj',    'conditional' : 'Config::enableEmuObj               || Config::forceEmuObj    || REGAL_FORCE_EMU_OBJ',    'ifdef' : 'REGAL_EMU_OBJ',    'formulae' : objFormulae    },
    #{ 'type' : 'RegalPpc',   'include' : 'RegalPpc.h',    'member' : 'ppc',    'conditional' : None,                                                                                     'ifdef' : '',                 'formulae' : ppcFormulae    },
    { 'type' : 'Emu::Ppa',    'include' : 'RegalPpa.h',    'member' : 'ppa',    'conditional' : 'Config::enableEmuPpa               || Config::forceEmuPpa    || REGAL_FORCE_EMU_PPA',    'ifdef' : 'REGAL_EMU_PPA',    'formulae' : ppaFormulae    },
    { 'type' : 'Emu::Ppca',   'include' : 'RegalPpca.h',   'member' : 'ppca',   'conditional' : 'Config::enableEmuPpca              || Config::forceEmuPpca   || REGAL_FORCE_EMU_PPCA',   'ifdef' : 'REGAL_EMU_PPCA',   'formulae' : ppcaFormulae   },
    { 'type' : 'Emu::Bin',    'include' : 'RegalBin.h',    'member' : 'bin',    'conditional' : 'Config::enableEmuBin               || Config::forceEmuBin    || REGAL_FORCE_EMU_BIN',    'ifdef' : 'REGAL_EMU_BIN',    'formulae' : binFormulae    },
    { 'type' : 'Emu::Xfer',   'include' : 'RegalXfer.h',   'member' : 'xfer',   'conditional' : '(isES2() && Config::enableEmuXfer) || Config::forceEmuXfer   || REGAL_FORCE_EMU_XFER',   'ifdef' : 'REGAL_EMU_XFER',   'formulae' : xferFormulae   },
    { 'type' : 'Emu::Dsa',    'include' : 'RegalDsa.h',    'member' : 'dsa',    'conditional' : 'Config::enableEmuDsa               || Config::forceEmuDsa    || REGAL_FORCE_EMU_DSA',    'ifdef' : 'REGAL_EMU_DSA',    'formulae' : dsaFormulae    },
    { 'type' : 'Emu::TexSto', 'include' : 'RegalTexSto.h', 'member' : 'texsto', 'conditional' : 'Config::enableEmuTexSto            || Config::forceEmuTexSto || REGAL_FORCE_EMU_TEXSTO', 'ifdef' : 'REGAL_EMU_TEXSTO', 'formulae' : texstoFormulae },
    { 'type' : 'Emu::Iff',    'include' : 'RegalIff.h',    'member' : 'iff',    'conditional' : 'Config::enableEmuIff               || Config::forceEmuIff    || REGAL_FORCE_EMU_IFF',    'ifdef' : 'REGAL_EMU_IFF',    'formulae' : iffFormulae    },
    { 'type' : 'Emu::So',     'include' : 'RegalSo.h',     'member' : 'so',     'conditional' : '(Config::enableEmuSo && !info->gl_arb_sampler_objects) || Config::forceEmuSo || REGAL_FORCE_EMU_SO',                                                           'ifdef' : 'REGAL_EMU_SO',  'formulae' : soFormulae  },
    { 'type' : 'Emu::Vao',    'include' : 'RegalVao.h',    'member' : 'vao',    'conditional' : '(Config::enableEmuVao              || Config::enableEmuVao   || REGAL_FORCE_EMU_VAO) && (Config::enableEmuIff || Config::forceEmuIff || REGAL_FORCE_EMU_IFF)', 'ifdef' : 'REGAL_EMU_VAO', 'formulae' : vaoFormulae },
    { 'type' : 'Emu::TexC',   'include' : 'RegalTexC.h',   'member' : 'texc',   'conditional' : '(isES2() && Config::enableEmuTexC) || Config::forceEmuTexC   || REGAL_FORCE_EMU_TEXC',   'ifdef' : 'REGAL_EMU_TEXC',   'formulae' : texCFormulae   },
    { 'type' : 'Emu::Filt',   'include' : 'RegalFilt.h',   'member' : 'filt',   'conditional' : 'Config::enableEmuFilter            || Config::forceEmuFilter || REGAL_FORCE_EMU_FILTER', 'ifdef' : 'REGAL_EMU_FILTER', 'formulae' : filterFormulae },
    { 'type' : 'void',        'include' : None,            'member' : None,     'conditional' : None,                                                                         'ifdef' : None,               'formulae' : None           }
]

contextHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalThread.h"
#include "RegalPrivate.h"
#include "RegalContextInfo.h"
#include "RegalDispatcher.h"
#include "RegalDispatchError.h"
#include "RegalSharedList.h"

#if REGAL_SYS_PPAPI
#define __gl2_h_  // HACK - revisit
#include <ppapi/c/pp_resource.h>
#include <ppapi/c/ppb_opengles2.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DebugInfo;

${EMU_FORWARD_DECLARE}

struct RegalContext
{
  RegalContext();
  ~RegalContext();

  void Init();

  // If profile is forced at build-time, no need to check runtime flag

  inline bool isES1()    const { RegalAssert(info); return REGAL_SYS_ES1 && ( REGAL_FORCE_ES1_PROFILE  || info->es1  ); }
  inline bool isES2()    const { RegalAssert(info); return REGAL_SYS_ES2 && ( REGAL_FORCE_ES2_PROFILE  || info->es2  ); }
  inline bool isCore()   const { RegalAssert(info); return REGAL_SYS_GL  && ( REGAL_FORCE_CORE_PROFILE || info->core ); }
  inline bool isCompat() const { RegalAssert(info); return REGAL_SYS_GL  &&                               info->compat; }

  bool                initialized;
  Dispatcher          dispatcher;
  DispatchErrorState  err;
  DebugInfo          *dbg;
  ContextInfo        *info;

  //
  // Emulation
  //

${EMU_MEMBER_DECLARE}

  #if REGAL_SYS_PPAPI
  PPB_OpenGLES2      *ppapiES2;
  PP_Resource         ppapiResource;
  #endif

  RegalSystemContext  sysCtx;
  Thread::Thread      thread;

  #if REGAL_SYS_X11
  Display            *x11Display;
  #endif

  #if REGAL_SYS_GLX
  GLXDrawable         x11Drawable;
  #endif

  GLLOGPROCREGAL      logCallback;

  //
  // Regal context sharing
  //

  shared_list<RegalContext *> shareGroup;

  // Query that any of the contexts in the share
  // group are already initialized

  bool groupInitialized() const;

  // Get any context in the share group that is
  // already initialized

  RegalContext *groupInitializedContext();

  // For RegalDispatchCode

#if REGAL_CODE
  FILE               *codeSource;
  FILE               *codeHeader;
  size_t              codeInputNext;
  size_t              codeOutputNext;
  size_t              codeShaderNext;  // glCreateShader/glCreateShaderObjectARB
  size_t              codeProgramNext; // glCreateProgram/glCreateProgramObjectARB
#endif

  // State tracked via EmuContextState.py / Regal.cpp

  size_t              depthBeginEnd;   // Normally zero or one
  size_t              depthPushMatrix; //
  size_t              depthPushAttrib; //
  size_t              depthNewList;    //
};

REGAL_NAMESPACE_END

#endif // __${HEADER_NAME}_H__
''')

contextSourceTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"

${INCLUDES}#if REGAL_EMULATION
${EMU_INCLUDES}#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace Logging;

RegalContext::RegalContext()
: initialized(false),
  dispatcher(),
  dbg(NULL),
  info(NULL),
${MEMBER_CONSTRUCT}#if REGAL_EMULATION
${EMU_MEMBER_CONSTRUCT}#endif
#if REGAL_SYS_PPAPI
  ppapiES2(NULL),
  ppapiResource(NULL),
#endif
  sysCtx(NULL),
  thread(0),
#if REGAL_SYS_X11
  x11Display(NULL),
#endif
#if REGAL_SYS_GLX
  x11Drawable(0),
#endif
  logCallback(NULL),
#if REGAL_CODE
  codeSource(NULL),
  codeHeader(NULL),
  codeInputNext(0),
  codeOutputNext(0),
  codeShaderNext(0),
  codeProgramNext(0),
#endif
  depthBeginEnd(0),
  depthPushMatrix(0),
  depthPushAttrib(0),
  depthNewList(0)
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

${MEMBER_INIT}

#if REGAL_EMULATION
#if !REGAL_FORCE_EMULATION
  if
  (
    Config::forceEmulation  ||
    (
      Config::enableEmulation &&
      (
        info->core ||
        info->es2  ||
        ( info->compat && !info->gl_ext_direct_state_access )
      )
    )
  )
#endif
  {
    RegalAssert(info);
${EMU_MEMBER_INIT}
  }
#endif

#if REGAL_CODE
  if (Config::enableCode)
  {
    if (Config::codeSourceFile.length())
    {
      codeSource = fopen(Config::codeSourceFile.c_str(),"wt");      
      if (!codeSource)
        Warning("Failed to open file ",Config::codeSourceFile," for writing code source.");
    }
    if (Config::codeHeaderFile.length())
    {
      if (Config::codeHeaderFile==Config::codeSourceFile)
        codeHeader = codeSource;
      else
        codeHeader = fopen(Config::codeHeaderFile.c_str(),"wt");
      if (!codeHeader)
        Warning("Failed to open file ",Config::codeHeaderFile," for writing code header.");      
    }
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
${MEMBER_CLEANUP}
#if REGAL_EMULATION
${EMU_MEMBER_CLEANUP}#endif

#if REGAL_CODE
  if (codeSource)
    fclose(codeSource);

  if (codeHeader)
    fclose(codeHeader);
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
''')

def generateContextHeader(apis, args):

    emuForwardDeclare = ''
    emuMemberDeclare  = ''

    for i in emuRegal:
      if i.get('member')!=None:
        emuForwardDeclare += 'struct %s;\n' % i['type']
        emuMemberDeclare  += '  %-18s *%s;\n' % ( i['type'], i['member'] )

    emuForwardDeclare += '#if REGAL_EMULATION\n'
    emuMemberDeclare  += '#if REGAL_EMULATION\n'

    emuMemberDeclare += '  // Fixed function emulation\n'
    emuMemberDeclare += '  int emuLevel;\n'

    for i in emu:
      if i.get('member')!=None:
        if i['type'].startswith('Emu::'):
          emuForwardDeclare += 'namespace Emu { struct %s; };\n' % i['type'][5:]
        else:
          emuForwardDeclare += 'struct %s;\n' % i['type']
        emuMemberDeclare  += '  %-18s *%s;\n' % ( i['type'], i['member'] )

    emuForwardDeclare += '#endif\n'
    emuMemberDeclare  += '#endif\n'

    # Output

    substitute = {}

    substitute['LICENSE']       = args.license
    substitute['AUTOGENERATED'] = args.generated
    substitute['COPYRIGHT']     = args.copyright

    substitute['HEADER_NAME'] = "REGAL_CONTEXT"

    substitute['EMU_FORWARD_DECLARE'] = emuForwardDeclare
    substitute['EMU_MEMBER_DECLARE'] = emuMemberDeclare

    outputCode( '%s/RegalContext.h' % args.srcdir, contextHeaderTemplate.substitute(substitute))

def generateContextSource(apis, args):

    includes           = ''
    memberConstruct    = ''
    memberInit         = ''
    memberCleanup      = ''
    emuIncludes        = ''
    emuMemberConstruct = ''
    emuMemberInit      = ''
    emuMemberCleanup   = ''

    for i in emuRegal:
      if i['include']:
        includes        += '#include "%s"\n' % i['include']
      if i['member']:
        memberConstruct += '  %s(NULL),\n' % ( i['member'] )
        memberInit      += '  %s = new %s;\n'%(i['member'],i['type'])
        memberCleanup   += indent(wrapIf(i['ifdef'],'delete %s;\n' % i['member']),'  ')

    emuMemberConstruct += '  emuLevel(0),\n'

    emuMemberInit += '    // emu\n'
    emuMemberInit += '    emuLevel = %d;\n' % ( len( emu ) - 1 )
    emuMemberCleanup += '  // emu\n'

    for i in range( len( emu ) - 1 ) :
      if emu[i]['member']:
        emuMemberConstruct += '  %s(NULL),\n' % emu[i]['member']

    for i in range( len( emu ) - 1 ) :
        if emu[i]['include']:
            emuIncludes += '#include "%s"\n' % emu[i]['include']
        if emu[i]['member']:
            emuMemberCleanup += indent(wrapIf(emu[i]['ifdef'],'delete %s;\n' % emu[i]['member']),'  ')
        revi = len( emu ) - 2 - i;
        if emu[revi]['member']:
            init = ''
            if emu[revi]['member']=='dsa':
              init += 'Internal("RegalContext::Init ","GL_EXT_direct_state_access");\n'
              init += 'info->regal_ext_direct_state_access = true;\n'
              init += 'info->regalExtensionsSet.insert("GL_EXT_direct_state_access");\n'
              init += 'info->regalExtensions = ::boost::print::detail::join(info->regalExtensionsSet,std::string(" "));\n'
            if emu[revi]['member']=='texsto':
              init += 'Internal("RegalContext::Init ","GL_ARB_texture_storage");\n'
              init += 'info->regal_arb_texture_storage = true;\n'
              init += 'info->regalExtensionsSet.insert("GL_ARB_texture_storage");\n'
              init += 'info->regalExtensions = ::boost::print::detail::join(info->regalExtensionsSet,std::string(" "));\n'

            init += '%s = new %s;\n' % ( emu[revi]['member'], emu[revi]['type'] )
            init += 'emuLevel = %d;\n' % ( int(emu[revi]['level']) - 1)
            init += '%s->Init(*this);\n' % emu[revi]['member']
            emuMemberInit += indent(wrapIf(emu[revi]['ifdef'],wrapCIf(emu[revi]['conditional'],init)),'    ')

    emuMemberInit += '    emuLevel = %d;\n' % ( len( emu ) - 1 )

    # Output

    substitute = {}

    substitute['LICENSE']       = args.license
    substitute['AUTOGENERATED'] = args.generated
    substitute['COPYRIGHT']     = args.copyright

    substitute['INCLUDES']             = includes
    substitute['MEMBER_CONSTRUCT']     = memberConstruct
    substitute['MEMBER_INIT']          = memberInit
    substitute['MEMBER_CLEANUP']       = memberCleanup
    substitute['EMU_INCLUDES']         = emuIncludes
    substitute['EMU_MEMBER_CONSTRUCT'] = emuMemberConstruct
    substitute['EMU_MEMBER_INIT']      = emuMemberInit
    substitute['EMU_MEMBER_CLEANUP']   = emuMemberCleanup

    outputCode( '%s/RegalContext.cpp' % args.srcdir, contextSourceTemplate.substitute(substitute))
