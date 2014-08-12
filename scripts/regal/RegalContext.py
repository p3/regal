#!/usr/bin/python -B

from string import Template, upper, replace, capwords

from ApiUtil import outputCode
from ApiCodeGen import *

from EmuInit           import formulae       as initFormulae
from EmuInit           import formulaeGlobal as initFormulaeGlobal
from EmuContextShare   import formulae       as contextShareFormulae
from EmuContextState   import formulae       as contextStateFormulae
from EmuGetString      import formulae       as getStringFormulae
from EmuForceCore      import formulae       as forceCoreFormulae
from EmuLookup         import formulae       as lookupFormulae
from EmuMarker         import formulae       as markerFormulae
from EmuMarker         import formulaeGlobal as markerFormulaeGlobal
from EmuFrame          import formulae       as frameFormulae
from EmuFrame          import formulaeGlobal as frameFormulaeGlobal
from EmuExtensionQuery import formulae       as extensionQueryFormulae
from EmuProcAddress    import formulae       as procAddressFormulae
from EmuErrorString    import formulae       as errorStringFormulae
from EmuEnable         import formulae       as enableFormulae
from EmuCache          import formulaeGlobal as cacheFormulaeGlobal

from EmuLog    import logFormulae

from Emu       import emuFindEntry, emuCodeGen
from EmuDsa    import dsaFormulae
from EmuVao    import vaoFormulae
from EmuSo     import soFormulae
from EmuPpca   import ppcaFormulae
from EmuPpa    import ppaFormulae
from EmuIff    import iffFormulae
from EmuQuads  import quadsFormulae
from EmuBin    import binFormulae
from EmuObj    import objFormulae
from EmuFilter import formulae as filterFormulae
from EmuTexC   import texCFormulae
from EmuTextureStorage import texstoFormulae
from EmuBaseVertex import baseVertexFormulae
from EmuRect   import rectFormulae
from EmuHint   import hintFormulae

from EmuPixelTransfer import xferFormulae

# Regal.cpp emulation

emuRegal = [
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : initFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : initFormulaeGlobal },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : contextShareFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : contextStateFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : getStringFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : forceCoreFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : lookupFormulae },
    { 'type' : 'Marker',   'include' : 'RegalMarker.h', 'member' : 'marker', 'suffix' : None,  'ifdef' : None,  'formulae' : markerFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : markerFormulaeGlobal },
    { 'type' : 'Frame',    'include' : 'RegalFrame.h',  'member' : 'frame',  'suffix' : None,  'ifdef' : 'REGAL_FRAME',  'formulae' : frameFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : 'REGAL_FRAME',  'formulae' : frameFormulaeGlobal },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : extensionQueryFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : procAddressFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : errorStringFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : logFormulae    },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : enableFormulae },
    { 'type' : None,       'include' : None,            'member' : None,     'suffix' : None,  'ifdef' : None,  'formulae' : cacheFormulaeGlobal },
]


# RegalDispatchEmu.cpp fixed-function emulation

emu = [
    { 'type' : 'Emu::Obj',        'include' : 'RegalObj.h',        'member' : 'obj',    'plugin' : False, 'suffix' : 'Obj',        'ifdef' : 'REGAL_EMU_OBJ',        'formulae' : objFormulae        },
    { 'type' : 'Emu::Hint',       'include' : 'RegalHint.h',       'member' : 'hint',   'plugin' : False, 'suffix' : 'Hint',       'ifdef' : 'REGAL_EMU_HINT',       'formulae' : hintFormulae       },
    { 'type' : 'Emu::Ppa',        'include' : 'RegalPpa.h',        'member' : 'ppa',    'plugin' : False, 'suffix' : 'Ppa',        'ifdef' : 'REGAL_EMU_PPA',        'formulae' : ppaFormulae        },
    { 'type' : 'Emu::Ppca',       'include' : 'RegalPpca.h',       'member' : 'ppca',   'plugin' : False, 'suffix' : 'Ppca',       'ifdef' : 'REGAL_EMU_PPCA',       'formulae' : ppcaFormulae       },
    { 'type' : 'Emu::Bin',        'include' : 'RegalBin.h',        'member' : 'bin',    'plugin' : False, 'suffix' : 'Bin',        'ifdef' : 'REGAL_EMU_BIN',        'formulae' : binFormulae        },
    { 'type' : 'Emu::Xfer',       'include' : 'RegalXfer.h',       'member' : 'xfer',   'plugin' : False, 'suffix' : 'Xfer',       'ifdef' : 'REGAL_EMU_XFER',       'formulae' : xferFormulae       },
    { 'type' : 'Emu::TexSto',     'include' : 'RegalTexSto.h',     'member' : 'texsto', 'plugin' : False, 'suffix' : 'TexSto',     'ifdef' : 'REGAL_EMU_TEXSTO',     'formulae' : texstoFormulae     },
    { 'type' : 'Emu::BaseVertex', 'include' : 'RegalBaseVertex.h', 'member' : 'bv',     'plugin' : False, 'suffix' : 'BaseVertex', 'ifdef' : 'REGAL_EMU_BASEVERTEX', 'formulae' : baseVertexFormulae },
    { 'type' : 'Emu::Rect',       'include' : 'RegalRect.h',       'member' : 'rect',   'plugin' : False, 'suffix' : 'Rect',       'ifdef' : 'REGAL_EMU_RECT',       'formulae' : rectFormulae       },
    { 'type' : 'Emu::Iff',        'include' : 'RegalIff.h',        'member' : 'iff',    'plugin' : False, 'suffix' : 'Iff',        'ifdef' : 'REGAL_EMU_IFF',        'formulae' : iffFormulae        },
    { 'type' : 'Emu::Quads',      'include' : 'RegalQuads.h',      'member' : 'quads',  'plugin' : False, 'suffix' : 'Quads',      'ifdef' : 'REGAL_EMU_QUADS',      'formulae' : quadsFormulae      },
    { 'type' : 'Emu::So',         'include' : 'RegalSo.h',         'member' : 'so',     'plugin' : False, 'suffix' : 'So',         'ifdef' : 'REGAL_EMU_SO',         'formulae' : soFormulae         },
    { 'type' : 'Emu::Dsa',        'include' : 'RegalDsa.h',        'member' : 'dsa',    'plugin' : False, 'suffix' : 'Dsa',        'ifdef' : 'REGAL_EMU_DSA',        'formulae' : dsaFormulae        },
    { 'type' : 'Emu::Vao',        'include' : 'RegalVao.h',        'member' : 'vao',    'plugin' : False, 'suffix' : 'Vao',        'ifdef' : 'REGAL_EMU_VAO',        'formulae' : vaoFormulae        },
    { 'type' : 'Emu::TexC',       'include' : 'RegalTexC.h',       'member' : 'texc',   'plugin' : False, 'suffix' : 'TexC',       'ifdef' : 'REGAL_EMU_TEXC',       'formulae' : texCFormulae       },
    { 'type' : 'Emu::Filt',       'include' : 'RegalFilt.h',       'member' : 'filt',   'plugin' : False, 'suffix' : 'Filter',     'ifdef' : 'REGAL_EMU_FILTER',     'formulae' : filterFormulae     },
    { 'type' : 'void',            'include' : None,                'member' : None,     'plugin' : False, 'suffix' : None,         'ifdef' : None,                   'formulae' : None               }
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
#include "RegalDispatcherGL.h"
#include "RegalDispatcherGlobal.h"
#include "RegalDispatchError.h"
#include "RegalDispatchHttp.h"
#include "RegalScopedPtr.h"
#include "RegalSharedList.h"

#if REGAL_SYS_PPAPI
#define __gl2_h_  // HACK - revisit
#include <ppapi/c/pp_resource.h>
#include <ppapi/c/ppb_opengles2.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct EmuInfo;
struct DebugInfo;
struct Statistics;

${EMU_FORWARD_DECLARE}

struct RegalContext
{
  RegalContext();
  ~RegalContext();

  void Init();
  void Cleanup();

  // If profile is forced at build-time, no need to check runtime flag

  inline bool isES1()    const { RegalAssert(info); return REGAL_SYS_ES1 && ( REGAL_FORCE_ES1_PROFILE  || info->es1  ); }
  inline bool isES2()    const { RegalAssert(info); return REGAL_SYS_ES2 && ( REGAL_FORCE_ES2_PROFILE  || info->es2  ); }
  inline bool isCore()   const { RegalAssert(info); return REGAL_SYS_GL  && ( REGAL_FORCE_CORE_PROFILE || info->core ); }
  inline bool isCompat() const { RegalAssert(info); return REGAL_SYS_GL  &&                               info->compat; }

  bool                    initialized;
  DispatcherGL            dispatcher;
  DispatchErrorState      err;

#if REGAL_HTTP
  DispatchHttpState       http;
#endif

  scoped_ptr<DebugInfo>   dbg;
  scoped_ptr<ContextInfo> info;
  scoped_ptr<EmuInfo>     emuInfo;
  std::set<std::string>   extensionsSet;
  std::string             extensions;


  inline void numExtensions(GLboolean *params)
  {
    *params = !!extensionsSet.size();
  }

  template <typename T> inline void numExtensions(T *params)
  {
    *params = static_cast<T>(extensionsSet.size());
  }

#if REGAL_STATISTICS
  scoped_ptr<Statistics>  statistics;
#endif

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

  #if REGAL_SYS_WGL
  HDC                 hdc;
  HGLRC               hglrc;
  #endif

  GLLOGPROCREGAL      logCallback;

  //
  // Regal context sharing
  //

  shared_list<RegalContext *> shareGroup;

  // Query that any of the contexts in the share
  // group are already initialized

  bool groupInitialized() const;

  // The http and perhaps other threads need to be able brief, temporary access the context.
  // parkContext() makes the calling thread release the context
  // unparkContext() makes it current to the calling thread

  void parkContext( DispatchTableGlobal & tbl );
  void unparkContext( DispatchTableGlobal & tbl );


  // For RegalDispatchCode

#if REGAL_CODE
  FILE               *codeSource;
  FILE               *codeHeader;
  size_t              codeInputNext;
  size_t              codeOutputNext;
  size_t              codeShaderNext;  // glCreateShader/glCreateShaderObjectARB
  size_t              codeProgramNext; // glCreateProgram/glCreateProgramObjectARB
  size_t              codeTextureNext; // glTexImage2D etc.
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

#include <boost/print/string_list.hpp>
using namespace boost::print;

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalContext.h"
#include "RegalEmuInfo.h"
#include "RegalLayerInfo.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"
#include "RegalStatistics.h"


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
#if REGAL_STATISTICS
  statistics(new Statistics()),
#endif
${MEMBER_CONSTRUCT}#if REGAL_EMULATION
${EMU_MEMBER_CONSTRUCT}#endif
#if REGAL_SYS_PPAPI
  ppapiES2(NULL),
  ppapiResource(0),
  sysCtx(0),
#else
  sysCtx(NULL),
#endif
  thread(0),
#if REGAL_SYS_X11
  x11Display(NULL),
#endif
#if REGAL_SYS_GLX
  x11Drawable(0),
#endif
#if REGAL_SYS_WGL
  hdc(0),
  hglrc(0),
#endif
  logCallback(NULL),
#if REGAL_CODE
  codeSource(NULL),
  codeHeader(NULL),
  codeInputNext(0),
  codeOutputNext(0),
  codeShaderNext(0),
  codeProgramNext(0),
  codeTextureNext(0),
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

  RegalAssert(this);
  if (!info)
  {
    info = new ContextInfo();
    RegalAssert(info);
    info->init(*this);
  }

  if (!emuInfo)
  {
    emuInfo = new EmuInfo();
    RegalAssert(emuInfo);
    emuInfo->init(*info);
  }

${MEMBER_INIT}

#if REGAL_EMULATION

${EMULATION_ENABLED}

#if !REGAL_FORCE_EMULATION
  if (Config::enableEmulation || Config::forceEmulation)
#endif
  {
${EMU_MEMBER_INIT}
  }
#endif

#if REGAL_CODE
  if (Config::enableCode && !codeSource && !codeHeader)
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

#if REGAL_HTTP
  http.Init( this );
#endif

  if (info && emuInfo)
  {
    extensionsSet.insert(info->extensionsSet.begin(), info->extensionsSet.end());
    extensionsSet.insert(emuInfo->extensionsSet.begin(), emuInfo->extensionsSet.end());
    extensions = ::boost::print::detail::join(extensionsSet,std::string(" "));
  }

  initialized = true;
}

// Note that Cleanup() may or may not have been called prior to destruction
RegalContext::~RegalContext()
{
  Internal("RegalContext::~RegalContext","()");

  #if REGAL_STATISTICS
  if (statistics && !Logging::frameStatistics)
  {
    statistics->log();
    statistics->reset();
  }
  #endif

  // Remove this context from the share group.

  shareGroup->remove(this);

#if REGAL_CODE
  if (codeSource)
    fclose(codeSource);

  if (codeHeader)
    fclose(codeHeader);
#endif
}

// Called prior to deletion, if this context is still set for this thread.
// Need to:
// 1) clean up GL state we've modified
// 2) leave the RegalContext in a state where Init() could be called again
void
RegalContext::Cleanup()
{
  Internal("RegalContext::Cleanup","()");

#if REGAL_EMULATION
${EMU_MEMBER_CLEANUP}#endif

  initialized = false;
}

bool
RegalContext::groupInitialized() const
{
  Internal("RegalContext::groupInitialized","()");

  // The first context is always the first initialized context in the group.
  return shareGroup->front()->initialized;
}

void RegalContext::parkContext( DispatchTableGlobal & tbl )
{
  #if REGAL_SYS_OSX
  tbl.call(&tbl.CGLSetCurrentContext)( NULL );
  #else
  UNUSED_PARAMETER(tbl);
  //<> # error "Implement me!"
  #endif
#if REGAL_SYS_PPAPI
  Init::makeCurrent(NULL,NULL);
#else
  Init::makeCurrent(NULL);
#endif
}

void RegalContext::unparkContext( DispatchTableGlobal & tbl )
{
  #if REGAL_SYS_OSX
  tbl.call(&tbl.CGLSetCurrentContext)( reinterpret_cast<CGLContextObj>(sysCtx) );
  #else
  UNUSED_PARAMETER(tbl);
  //<> # error "Implement me!"
  #endif
#if REGAL_SYS_PPAPI
  Init::makeCurrent(sysCtx,ppapiES2);
#else
  Init::makeCurrent(sysCtx);
#endif
}

REGAL_NAMESPACE_END
''')

def generateContextHeader(apis, args):

    emuForwardDeclare = ''
    emuMemberDeclare  = ''

    for i in emuRegal:
      if i.get('member')!=None:
        emuForwardDeclare += 'struct %s;\n' % i['type']
        emuMemberDeclare  += wrapIf(i['ifdef'],'  scoped_ptr<%-18s> %s;\n' % ( i['type'], i['member'] ))


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
        emuMemberDeclare  += '  scoped_ptr<%-18s> %s;\n' % ( i['type'], i['member'] )

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

def addEmulatedExtensions(extensions, emuLayer):
    str = ''

    for extension in extensions:
      if extension.emulatedBy == emuLayer:
        emuStr = ''

        # Strip 'GL_' prefix
        name = extension.name[3:]

        emuStr += 'Info("Activating %s emulation.");\n' % name
        emuStr += 'emuInfo->gl_%s = true;\n' % name.lower()
        emuStr += 'emuInfo->extensionsSet.insert("GL_%s");\n' % name

        supportStr = '!info->gl_%s' % name.lower()
        if (len(extension.emulatedIf)):
          supportStr += ' && (%s)' % extension.emulatedIf

        str += wrapCIf(supportStr, emuStr)

    if str != '':
      str += 'emuInfo->extensions = ::boost::print::detail::join(emuInfo->extensionsSet,std::string(" "));\n'

    return str

def generateContextSource(apis, args):

    includes           = ''
    memberConstruct    = ''
    memberInit         = ''
    memberCleanup      = ''
    emuIncludes        = ''
    emuMemberConstruct = ''
    emuMemberInit      = ''
    emuMemberCleanup   = ''
    emulatedExtensions = []
    emuEmulationEnabled = ''

    for i in emuRegal:
      if i['include']:
        includes        += '#include "%s"\n' % i['include']
      if i['member']:
        memberConstruct += wrapIf(i['ifdef'],'  %s(NULL),\n' %i['member'])
        memberInit      += indent(wrapIf(i['ifdef'],wrapCIf('!%s' % i['member'],'%s = new %s;\n'%(i['member'],i['type']))),'  ')

    emuMemberConstruct += '  emuLevel(0),\n'

    emuMemberInit += '    // emu\n'
    emuMemberInit += '    emuLevel = %d;\n' % ( len( emu ) - 1 )
    emuMemberCleanup  += '  // emu\n'

    for i in range( len( emu ) - 1 ) :
      if emu[i]['member']:
        emuMemberConstruct += '  %s(NULL),\n' % emu[i]['member']

    for i in range( len( emu ) - 1 ) :
      if emu[i]['include']:
        emuIncludes += '#include "%s"\n' % emu[i]['include']
      if emu[i]['member']:
        cleanup = ''
        cleanup += 'emuLevel = %d;\n' % ( int(emu[i]['level']) - 1)
        cleanup += '%s->Cleanup(*this);\n' % emu[i]['member']
        cleanup += '%s.reset(NULL);\n' % emu[i]['member']
        emuMemberCleanup += indent(wrapIf(emu[i]['ifdef'],wrapCIf(emu[i]['member'],cleanup)),'  ')

      revi = len( emu ) - 2 - i;
      if emu[revi]['member']:

        init = '{\n'
        init += '  Emu::LayerInfo layer;\n'
        init += '  Emu::Get%sLayerInfo( *this, layer );\n' % emu[revi]['suffix']
        init += '  bool enable = false;\n'
        emuMemberInit += indent(init,'    ')

        # Info logging of activated emulation layers

        init = ''
        init += 'if( Config::enableEmu%s ) {\n' % emu[revi]['suffix']
        init += '  enable = layer.emulationSupported && ( layer.emulationNeeded || Config::forceEmu%s ); \n' % emu[revi]['suffix']
        init += '  if( enable ) {\n'
        if len(emu[revi]['ifdef']):
          init += '    Info("Activating emulation layer %s");\n'%(emu[revi]['ifdef'])
        init += '    %s = new %s;\n' % ( emu[revi]['member'], emu[revi]['type'] )
        init += '    emuLevel = %d;\n' % ( int(emu[revi]['level']) - 1)
        init += '    %s->Init(*this);\n' % emu[revi]['member']
        init += '  }\n'
        init += '}\n'
        emuMemberInit += indent(wrapIf(emu[revi]['ifdef'], init),'      ')

        init = ''
        init += '  Emu::Set%sEmuInfo( enable, this->emuInfo, layer );\n' % emu[revi]['suffix']
        init += '}\n'
        emuMemberInit += indent(init,'    ')

    emuMemberInit += '    emuLevel = %d;\n' % ( len( emu ) - 1 )

    # Output

    substitute = {}

    substitute['LICENSE']       = args.license
    substitute['AUTOGENERATED'] = args.generated
    substitute['COPYRIGHT']     = args.copyright

    substitute['INCLUDES']             = includes
    substitute['MEMBER_CONSTRUCT']     = memberConstruct
    substitute['MEMBER_INIT']          = memberInit
    substitute['EMU_INCLUDES']         = emuIncludes
    substitute['EMU_MEMBER_CONSTRUCT'] = emuMemberConstruct
    substitute['EMU_MEMBER_INIT']      = emuMemberInit
    substitute['EMU_MEMBER_CLEANUP']   = emuMemberCleanup
    substitute['EMULATION_ENABLED']    = emuEmulationEnabled

    outputCode( '%s/RegalContext.cpp' % args.srcdir, contextSourceTemplate.substitute(substitute))
