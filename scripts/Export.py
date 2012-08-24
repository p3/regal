#!/usr/bin/python -B

# This exporter is undocumented - REVISIT
#
#
#

# CodeGen Imports

from string import Template, upper, replace
from optparse import OptionParser
from copy import deepcopy
import re

from ApiUtil import validVersion
from ApiUtil import outputCode
from ApiUtil import importAttr
from ApiUtil import hexValue
from ApiUtil import toLong
from ApiUtil import typeIsVoid

from ApiCodeGen import *

from EmuContextState   import formulae as contextStateFormulae
from EmuGetString      import formulae as getStringFormulae
from EmuForceCore      import formulae as forceCoreFormulae
from EmuLookup         import formulae as lookupFormulae
from EmuMarker         import formulae as markerFormulae
from EmuExtensionQuery import formulae as extensionQueryFormulae
from EmuErrorString    import formulae as errorStringFormulae
from EmuEnable         import formulae as enableFormulae

from Emu       import emuFindEntry, emuCodeGen
from EmuDsa    import dsaFormulae
from EmuVao    import vaoFormulae
from EmuPpc    import ppcFormulae
from EmuPpa    import ppaFormulae
from EmuIff    import iffFormulae
from EmuBin    import binFormulae
from EmuLog    import logFormulae
from EmuObj    import objFormulae

from DispatchDebug import debugDispatchFormulae

# Regal.cpp emulation

emuRegal = [
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : contextStateFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : getStringFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : forceCoreFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : lookupFormulae },
    { 'type' : 'Marker',   'member' : 'marker', 'conditional' : None,  'ifdef' : None,  'formulae' : markerFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : extensionQueryFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : errorStringFormulae },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : logFormulae    },
    { 'type' : None,       'member' : None,     'conditional' : None,  'ifdef' : None,  'formulae' : enableFormulae },
]

# RegalDispathEmu.cpp fixed-function emulation

emu = [
    { 'type' : 'RegalObj',    'member' : 'obj',    'conditional' : 'Config::enableEmuObj', 'ifdef' : 'REGAL_EMU_OBJ', 'formulae' : objFormulae    },
    #{ 'type' : 'RegalPpc',   'member' : 'ppc',    'conditional' : None,                   'ifdef' : '',              'formulae' : ppcFormulae    },
    { 'type' : 'RegalPpa',    'member' : 'ppa',    'conditional' : 'Config::enableEmuPpa', 'ifdef' : 'REGAL_EMU_PPA', 'formulae' : ppaFormulae    },
    { 'type' : 'RegalBin',    'member' : 'bin',    'conditional' : 'Config::enableEmuBin', 'ifdef' : 'REGAL_EMU_BIN', 'formulae' : binFormulae    },
    { 'type' : 'RegalDsa',    'member' : 'dsa',    'conditional' : 'Config::enableEmuDsa', 'ifdef' : 'REGAL_EMU_DSA', 'formulae' : dsaFormulae    },
    { 'type' : 'RegalIff',    'member' : 'iff',    'conditional' : 'Config::enableEmuIff', 'ifdef' : 'REGAL_EMU_IFF', 'formulae' : iffFormulae    },
    { 'type' : 'RegalVao',    'member' : 'vao',    'conditional' : 'Config::enableEmuVao', 'ifdef' : 'REGAL_EMU_VAO', 'formulae' : vaoFormulae    },
    { 'type' : 'void',        'member' : None,     'conditional' : None,                   'ifdef' : None,            'formulae' : None           }
]

cond = { 'wgl' : 'REGAL_SYS_WGL', 'glx' : 'REGAL_SYS_GLX', 'cgl' : 'REGAL_SYS_OSX', 'egl' : 'REGAL_SYS_ANDROID | REGAL_STATIC_EGL' }

regalLicense = '''
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
'''

def cmpCategoryName(a,b):
  if a.category==b.category:
    return cmp(a.name,b.name)
  if a.category.startswith('GL_VERSION') and b.category.startswith('GL_VERSION'):   return cmp(a.category,b.category)
  if a.category.startswith('GL_VERSION'):                                           return -1
  if b.category.startswith('GL_VERSION'):                                           return 1
  if a.category.startswith('GLX_VERSION') and b.category.startswith('GLX_VERSION'): return cmp(a.category,b.category)
  if a.category.startswith('GLX_VERSION'):                                          return -1
  if b.category.startswith('GLX_VERSION'):                                          return 1
  if a.category.startswith('CGL_VERSION') and b.category.startswith('CGL_VERSION'): return cmp(a.category,b.category)
  if a.category.startswith('CGL_VERSION'):                                          return -1
  if b.category.startswith('CGL_VERSION'):                                          return 1
  return cmp(a.category,b.category)

def traverse(apis, args):

    for i in range( len( emu ) ) :
        emu[i]['level'] = len( emu ) - 1 - i

    for api in apis:

        # Fixup GL categories

        if api.name=='gl':
          for i in api.functions:
            if len(i.category):
              i.category = i.category.replace('_DEPRECATED','',1)
            else:
              i.category = 'GL_VERSION_%s_%s'%(i.version.split('.')[0],i.version.split('.')[1])

        api.functions.sort(cmpCategoryName)
        if api.name=='gl':
          for i in api.enums:
            if i.name=='defines':
              i.enumerants.sort(cmpCategoryName)

        apiHasCtx = api.name == 'gl';
        toRemove = set()

        for function in api.functions:

            # In the database, functions can be disabled one-by-one
            if not getattr(function,'regal',True):
              toRemove.add(function)

            function.loadFunction        = True
            function.loadFuncPtrDeclare  = True
            function.loadFuncPtrLoad     = True
            function.loadGetProcAddress  = True
            function.needsContext        = apiHasCtx

        for function in toRemove:
          api.functions.remove(function)

        # In the database, typedefs can be disabled one-by-one

        toRemove = set()
        for typedef in api.typedefs:
            if not getattr(typedef,'regal',True):
              toRemove.add(typedef)
        for typedef in toRemove:
          api.typedefs.remove(typedef)

    traverseContextInfo(apis,args)

def generate(apis, args):

  traverse(apis, args)
  generateSource(apis, args)
  generateEmuSource( apis, args )
  generateDispatchLog( apis, args )
  generateErrorSource( apis, args )
  generateDebugSource( apis, args )
  generateLoaderSource( apis, args )
  generateMissingSource( apis, args )
  generateNaclSource( apis, args )
  generateStaticES2Source( apis, args )
  generateStaticEGLSource( apis, args )
  generatePublicHeader(apis, args)
  generateDispatchHeader(apis, args)
  generateContextHeader(apis, args)
  generateContextSource(apis, args)
  generateContextInfoHeader(apis, args)
  generateContextInfoSource(apis, args)
  generateLookupSource(apis, args)
  generateLookupHeader(apis, args)
  generateTokenSource(apis, args)
  generateTokenHeader(apis, args)
  generateEnumHeader(apis, args)

  additional_exports = ['RegalSetErrorCallback', 'RegalMakeCurrent']

  generateDefFile( apis, args, additional_exports)

##############################################################################################


publicHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}
#define __gl_h_
#define __GL_H__
#define __X_GL_H
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

#ifndef REGAL_DECLARATIONS_H
#define REGAL_DECLARATIONS_H

#if _WIN32
# define REGAL_SYS_WGL 1
#elif __APPLE__
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#  define REGAL_SYS_IOS 1
# else
#  define REGAL_SYS_OSX 1
# endif
#elif defined(__native_client__)
#  define REGAL_SYS_NACL 1
#elif defined(__ANDROID__)
#  define REGAL_SYS_ANDROID 1
#elif !defined(_WIN32) && !defined(__APPLE__) && !defined(__native_client__)
# define REGAL_SYS_GLX 1
#endif

#if REGAL_SYS_WGL
# define REGAL_CALL __stdcall
#else
# define REGAL_CALL
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY REGAL_CALL
#endif

#ifdef _WIN32
#  if REGAL_DECL_EXPORT
#    define REGAL_DECL
#  else
#    define REGAL_DECL __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__>=4
#  if REGAL_DECL_EXPORT
#    define REGAL_DECL __attribute__ ((visibility("default")))
#  else
#    define REGAL_DECL
#  endif
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  if REGAL_DECL_EXPORT
#    define REGAL_DECL __global
#  else
#    define REGAL_DECL
#  endif
#else
#  define REGAL_DECL
#endif

#endif // REGAL_DECLARATIONS_H

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#if REGAL_SYS_GLX
#include <X11/Xdefs.h>
#include <X11/Xutil.h>
typedef XID GLXDrawable;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#if REGAL_SYS_WGL
  typedef __int64 int64_t;
  typedef unsigned __int64 uint64_t;
  #ifdef  REGAL_SYS_WGL_DECLARE_WGL
    #ifndef _WINDEF_
      struct HDC__ {int unused;};
      typedef struct HDC__* HDC;
      struct HGLRC__ {int unused;};
      typedef struct HGLRC__* HGLRC;
    #endif
  #endif
#else
# include <inttypes.h>
#endif

${API_TYPEDEF}

// TODO: make this automatic?
typedef void (*GLDEBUGPROCAMD)(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
typedef void (*GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);

typedef void (*GLLOGPROCREGAL)(GLenum stream, GLsizei length, const GLchar *message, GLvoid *context);

${API_ENUM}

${API_FUNC_DECLARE}

#ifdef __cplusplus
}
#endif

#endif // __REGAL_H__

#ifndef REGAL_API_H
#define REGAL_API_H

#if REGAL_SYS_NACL
#include <stdint.h>
struct PPB_OpenGLES2;
#endif

// Regal-specific API... try to keep this minimal
// this is a seperate include guard to work nicely with RegalGLEW.h

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*RegalErrorCallback)(GLenum);
REGAL_DECL RegalErrorCallback RegalSetErrorCallback( RegalErrorCallback callback );

#if REGAL_SYS_NACL
typedef int32_t RegalSystemContext;
REGAL_DECL void RegalMakeCurrent( RegalSystemContext ctx, struct PPB_OpenGLES2 *interface );
#else
typedef void * RegalSystemContext;
REGAL_DECL void RegalMakeCurrent( RegalSystemContext ctx );
#endif

#ifdef __cplusplus
}
#endif

#endif // ${HEADER_NAME}_H
''')

def generatePublicHeader(apis, args):

  apiTypedef     = apiTypedefCode( apis, args )
  apiEnum        = apiEnumCode(apis, args)                 # CodeGen for API enums
  apiFuncDeclare = apiFuncDeclareCode( apis, args )        # CodeGen for API functions

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['HEADER_NAME']   = "REGAL"

  substitute['API_TYPEDEF']      = apiTypedef
  substitute['API_ENUM']         = apiEnum
  substitute['API_FUNC_DECLARE'] = apiFuncDeclare

  outputCode( 'include/GL/Regal.h', publicHeaderTemplate.substitute(substitute))

##############################################################################################

dispatchHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

${API_GLOBAL_DISPATCH_TABLE_DEFINE}

extern DispatchTableGlobal dispatchTableGlobal;

${API_DISPATCH_TABLE_DEFINE}

REGAL_NAMESPACE_END

#endif // __${HEADER_NAME}_H__
''')

def generateDispatchHeader(apis, args):

    globalDispatchTableDefine = apiGlobalDispatchTableDefineCode( apis, args )
    dispatchTableDefine = apiDispatchTableDefineCode(apis, args)

    # Output

    substitute = {}

    substitute['LICENSE'] = regalLicense
    substitute['AUTOGENERATED'] = autoGeneratedMessage
    substitute['COPYRIGHT']     = args.copyright

    substitute['HEADER_NAME'] = 'REGAL_DISPATCH'
    substitute['API_GLOBAL_DISPATCH_TABLE_DEFINE'] = globalDispatchTableDefine
    substitute['API_DISPATCH_TABLE_DEFINE'] = dispatchTableDefine

    outputCode( '%s/RegalDispatch.h' % args.outdir, dispatchHeaderTemplate.substitute(substitute))

##############################################################################################

contextHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalPrivate.h"
#include "RegalDispatcher.h"
#include "RegalDispatchError.h"

#if defined(__native_client__)
#define __gl2_h_  // HACK - revisit
#include <ppapi/c/pp_resource.h>
#include <ppapi/c/ppb_opengles2.h>
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DebugInfo;
struct ContextInfo;

${EMU_FORWARD_DECLARE}

struct RegalContext
{
  RegalContext();
  ~RegalContext();

  void Init();

  Dispatcher          dispatcher;
  DispatchErrorState  err;
  DebugInfo          *dbg;
  ContextInfo        *info;
${EMU_MEMBER_DECLARE}

  #if defined(__native_client__)
  PPB_OpenGLES2      *naclES2;
  PP_Resource         naclResource;
  #endif

  RegalSystemContext  sysCtx;
  Thread              thread;

  GLLOGPROCREGAL      logCallback;

  // State tracked via EmuContextState.py / Regal.cpp

  size_t              depthBeginEnd;   // Normally zero or one
  size_t              depthPushAttrib; //
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
: dispatcher(),
  dbg(NULL),
  info(NULL),
${MEMBER_CONSTRUCT}#if REGAL_EMULATION
${EMU_MEMBER_CONSTRUCT}#endif
#if defined(__native_client__)
  naclES2(NULL),
  naclResource(NULL),
#endif
  sysCtx(NULL),
  thread(0),
  logCallback(NULL),
  depthBeginEnd(0),
  depthPushAttrib(0)
{
  ITrace("RegalContext::RegalContext");
  if (Config::enableDebug) {
    dbg = new DebugInfo();
    dbg->Init(this);
  }
}

void
RegalContext::Init()
{
  ITrace("RegalContext::Init");

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
        info->gles ||
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
}

RegalContext::~RegalContext()
{
  ITrace("RegalContext::~RegalContext");
  delete info;
${MEMBER_CLEANUP}
#if REGAL_EMULATION
${EMU_MEMBER_CLEANUP}#endif
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
        emuForwardDeclare += 'struct %s;\n' % i['type']
        emuMemberDeclare  += '  %-18s *%s;\n' % ( i['type'], i['member'] )

    emuForwardDeclare += '#endif\n'
    emuMemberDeclare  += '#endif\n'

    # Output

    substitute = {}

    substitute['LICENSE'] = regalLicense
    substitute['AUTOGENERATED'] = autoGeneratedMessage
    substitute['COPYRIGHT']     = args.copyright

    substitute['HEADER_NAME'] = "REGAL_CONTEXT"

    substitute['EMU_FORWARD_DECLARE'] = emuForwardDeclare
    substitute['EMU_MEMBER_DECLARE'] = emuMemberDeclare

    outputCode( '%s/RegalContext.h' % args.outdir, contextHeaderTemplate.substitute(substitute))

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
      if i['member']:
        includes        += '#include "Regal%s.h"\n' % i['type']
        memberConstruct += '  %s(NULL),\n' % ( i['member'] )
        memberInit      += '  %s = new %s;\n'%(i['member'],i['type'])
        memberCleanup   += '  delete %s;\n' % i['member']

    emuMemberConstruct += '  emuLevel(0),\n'

    emuMemberInit += '    // emu\n'
    emuMemberInit += '    emuLevel = %d;\n' % ( len( emu ) - 1 )
    emuMemberCleanup += '  // emu\n'

    for i in range( len( emu ) - 1 ) :
      if emu[i]['member']:
        emuMemberConstruct += '  %s(NULL),\n' % emu[i]['member']

    for i in range( len( emu ) - 1 ) :
        if emu[i]['member']:
            emuIncludes += '#include "%s.h"\n' % emu[i]['type']
            emuMemberCleanup += '  delete %s;\n' % emu[i]['member']
        revi = len( emu ) - 2 - i;
        if emu[revi]['member']:
            init = ''
            if emu[revi]['member']=='dsa':
              init += 'ITrace("RegalContext::Init GL_EXT_direct_state_access");\n'
              init += 'info->regal_ext_direct_state_access = true;\n'
#              init += '#ifndef REGAL_GL_EXTENSIONS\n'
              init += 'info->regalExtensionsSet.insert("GL_EXT_direct_state_access");\n'
              init += 'info->regalExtensions = ::boost::print::detail::join(info->regalExtensionsSet,std::string(" "));\n'
#              init += '#endif\n'

            init += '%s = new %s;\n' % ( emu[revi]['member'], emu[revi]['type'] )
            init += '%s->emuLevel = %d;\n' % ( emu[revi]['member'], emu[revi]['level']  )
            init += '%s->Init( this );\n' % emu[revi]['member']
            emuMemberInit += indent(wrapIf(emu[revi]['ifdef'],wrapCIf(emu[revi]['conditional'],init)),'    ')

    # Output

    substitute = {}

    substitute['LICENSE'] = regalLicense
    substitute['AUTOGENERATED'] = autoGeneratedMessage
    substitute['COPYRIGHT']     = args.copyright

    substitute['INCLUDES']             = includes
    substitute['MEMBER_CONSTRUCT']     = memberConstruct
    substitute['MEMBER_INIT']          = memberInit
    substitute['MEMBER_CLEANUP']       = memberCleanup
    substitute['EMU_INCLUDES']         = emuIncludes
    substitute['EMU_MEMBER_CONSTRUCT'] = emuMemberConstruct
    substitute['EMU_MEMBER_INIT']      = emuMemberInit
    substitute['EMU_MEMBER_CLEANUP']   = emuMemberCleanup

    outputCode( '%s/RegalContext.cpp' % args.outdir, contextSourceTemplate.substitute(substitute))

##############################################################################################

contextInfoHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <set>
#include <string>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct RegalContext;

struct ContextInfo
{
  ContextInfo();
  ~ContextInfo();

  void init(const RegalContext &context);

  // glewGetExtension, glewIsSupported

  bool getExtension(const char *ext) const;
  bool isSupported(const char *ext) const;

  // As reported by OpenGL implementation

  std::string vendor;
  std::string renderer;
  std::string version;
  std::string extensions;

  // As reported by Regal

  std::string regalVendor;
  std::string regalRenderer;
  std::string regalVersion;
  std::string regalExtensions;

  bool        regal_ext_direct_state_access;

  std::set<std::string> regalExtensionsSet;

  // As supported by the OpenGL implementation

${VERSION_DECLARE}

  GLuint maxVertexAttribs;
};

REGAL_NAMESPACE_END

#endif // __${HEADER_NAME}_H__
''')

contextInfoSourceTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <string>
#include <set>
using namespace std;

#include <boost/print/string_list.hpp>
using namespace boost::print;

#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalIff.h"             // For REGAL_MAX_VERTEX_ATTRIBS

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

ContextInfo::ContextInfo()
: regal_ext_direct_state_access(false),
${VERSION_INIT}
  maxVertexAttribs(0)
{
   ITrace("ContextInfo::ContextInfo");
}

ContextInfo::~ContextInfo()
{
   ITrace("ContextInfo::~ContextInfo");
}

inline string getString(const RegalContext &context, const GLenum e)
{
  ITrace("getString ",toString(e));
  RegalAssert(context.dispatcher.driver.glGetString);
  const GLubyte *str = context.dispatcher.driver.glGetString(e);
  return str ? string(reinterpret_cast<const char *>(str)) : string();
}

void
ContextInfo::init(const RegalContext &context)
{
  // OpenGL Version.

  vendor     = getString(context, GL_VENDOR);
  renderer   = getString(context, GL_RENDERER);
  version    = getString(context, GL_VERSION);

  Info("OpenGL vendor    : ",vendor);
  Info("OpenGL renderer  : ",renderer);
  Info("OpenGL version   : ",version);

  gl_version_major = 0;
  gl_version_minor = 0;

  gles_version_major = 0;
  gles_version_minor = 0;

  // Detect GL context version

  gles = starts_with(version,"OpenGL ES ");
  if (gles)
    sscanf(version.c_str(), "OpenGL ES %d.%d", &gles_version_major, &gles_version_minor);
  else
    sscanf(version.c_str(), "%d.%d", &gl_version_major, &gl_version_minor);

  // Detect core context

  if (!gles && gl_version_major>=3)
  {
    GLint flags = 0;
    RegalAssert(context.dispatcher.driver.glGetIntegerv);
    context.dispatcher.driver.glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &flags);
    core = flags & GL_CONTEXT_CORE_PROFILE_BIT ? GL_TRUE : GL_FALSE;
  }

  compat = !core && !gles;

  // Detect extensions

  string_list<string> extList;

  if (core)
  {
    RegalAssert(context.dispatcher.driver.glGetStringi);
    RegalAssert(context.dispatcher.driver.glGetIntegerv);

    GLint n = 0;
    context.dispatcher.driver.glGetIntegerv(GL_NUM_EXTENSIONS, &n);

    for (GLint i=0; i<n; ++i)
      extList.push_back(reinterpret_cast<const char *>(context.dispatcher.driver.glGetStringi(GL_EXTENSIONS,i)));
    extensions = extList.join(" ");
  }
  else
  {
    extensions = getString(context, GL_EXTENSIONS);
    extList.split(extensions,' ');
  }

  Info("OpenGL extensions: ",extensions);

  // TODO - filter out extensions Regal doesn't support?

#ifdef REGAL_GL_VENDOR
  regalVendor = REGAL_EQUOTE(REGAL_GL_VENDOR);
#else
  regalVendor = vendor;
#endif

#ifdef REGAL_GL_RENDERER
  regalRenderer = REGAL_EQUOTE(REGAL_GL_RENDERER);
#else
  regalRenderer = renderer;
#endif

#ifdef REGAL_GL_VERSION
  regalVersion = REGAL_EQUOTE(REGAL_GL_VERSION);
#else
  regalVersion = version;
#endif

#ifdef REGAL_GL_EXTENSIONS
  regalExtensions = REGAL_EQUOTE(REGAL_GL_EXTENSIONS);
#else
  static const char *ourExtensions[5] = {
    "GL_REGAL_error_string",
    "GL_REGAL_extension_query",
    "GL_REGAL_log",
    "GL_REGAL_enable",
    "GL_EXT_debug_marker"
  };
  regalExtensionsSet.insert(extList.begin(),extList.end());
  regalExtensionsSet.insert(&ourExtensions[0],&ourExtensions[5]);
  regalExtensions = ::boost::print::detail::join(regalExtensionsSet,string(" "));
#endif

#ifndef REGAL_NO_GETENV
  {
    const char *vendorEnv = GetEnv("REGAL_GL_VENDOR");
    if (vendorEnv) regalVendor = vendorEnv;

    const char *rendererEnv = GetEnv("REGAL_GL_RENDERER");
    if (rendererEnv) regalRenderer = rendererEnv;

    const char *versionEnv = GetEnv("REGAL_GL_VERSION");
    if (versionEnv) regalVersion = versionEnv;

    const char *extensionsEnv = GetEnv("REGAL_GL_EXTENSIONS");
    if (extensionsEnv) regalExtensions = extensionsEnv;
  }
#endif

  Info("Regal vendor     : ",regalVendor);
  Info("Regal renderer   : ",regalRenderer);
  Info("Regal version    : ",regalVersion);
  Info("Regal extensions : ",regalExtensions);

${VERSION_DETECT}

  // Vendor, rendering, version, extensions reported by Regal to application

  set<string> e;
  e.insert(extList.begin(),extList.end());

${EXT_INIT}

  RegalAssert(context.dispatcher.driver.glGetIntegerv);
  context.dispatcher.driver.glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, reinterpret_cast<GLint *>(&maxVertexAttribs));

  if (maxVertexAttribs > REGAL_MAX_VERTEX_ATTRIBS)
      maxVertexAttribs = REGAL_MAX_VERTEX_ATTRIBS;

  // Qualcomm fails with float4 attribs with 256 byte stride, so artificially limit to 8 attribs
  if (vendor == "Qualcomm" || vendor == "Chromium")
    maxVertexAttribs = 8;
}

${EXT_CODE}

REGAL_NAMESPACE_END
''')

def traverseContextInfo(apis, args):

  for api in apis:
    if api.name == 'gles':
      api.versions =  [ [2, 0] ]
    if api.name == 'gl':
      api.versions =  [ [4,2], [4, 1], [4, 0] ]
      api.versions += [ [3, 3], [3, 2], [3, 1], [3, 0] ]
      api.versions += [ [2, 1], [2, 0] ]
      api.versions += [ [1, 5], [1, 4], [1, 3], [1, 2], [1, 1], [1, 0] ]
    if api.name == 'glx':
      api.versions = [ [1, 4], [1, 3], [1, 2], [1, 1], [1, 0] ]
    c = set()
    c.update([i.category for i in api.functions])
    c.update([i.category for i in api.typedefs])
    c.update([i.category for i in api.enums])
    api.categories = [i for i in c if i and len(i) and i.find('_VERSION_')==-1 and i.find('WGL_core')==-1]

def versionDeclareCode(apis, args):

  code = ''
  for api in apis:
    name = api.name.lower()

    if name == 'gl':
      code += '  GLboolean compat : 1;\n'
      code += '  GLboolean core   : 1;\n'
      code += '  GLboolean gles   : 1;\n\n'

    if name in ['gl', 'glx']:
      code += '  GLint     %s_version_major;\n' % name
      code += '  GLint     %s_version_minor;\n' % name
      code += '\n'

    if hasattr(api, 'versions'):
      for version in sorted(api.versions):
        code += '  GLboolean %s_version_%d_%d : 1;\n' % (name, version[0], version[1])
      code += '\n'

    if name == 'gl':
      code += '  GLint     gles_version_major;\n'
      code += '  GLint     gles_version_minor;\n'
      code += '\n'
      code += '  GLint     glsl_version_major;\n'
      code += '  GLint     glsl_version_minor;\n'
      code += '\n'

  for api in apis:
    name = api.name.lower()
    if name in cond:
      code += '#if %s\n'%cond[name]
    for c in sorted(api.categories):
      code += '  GLboolean %s : 1;\n' % (c.lower())
    if name in cond:
      code += '#endif\n'
    code += '\n'

  return code

def versionInitCode(apis, args):

  code = ''
  for api in apis:
    name = api.name.lower()

    if name == 'gl':
      code += '  compat(false),\n'
      code += '  core(false),\n'
      code += '  gles(false),\n'

    if name in ['gl', 'glx']:
      code += '  %s_version_major(-1),\n' % name
      code += '  %s_version_minor(-1),\n' % name

    if hasattr(api, 'versions'):
      for version in sorted(api.versions):
        code += '  %s_version_%d_%d(false),\n' % (name, version[0], version[1])

    if name == 'gl':
      code += '  gles_version_major(-1),\n'
      code += '  gles_version_minor(-1),\n'
      code += '  glsl_version_major(-1),\n'
      code += '  glsl_version_minor(-1),\n'

  for api in apis:
    name = api.name.lower()
    if name in cond:
      code += '#if %s\n'%cond[name]
    for c in sorted(api.categories):
      code += '  %s(false),\n' % (c.lower())
    if name in cond:
      code += '#endif\n'

  return code

def versionDetectCode(apis, args):

  code = ''

  for api in apis:
    name = api.name.lower()
    if not hasattr(api, 'versions'):
      continue

    indent = ''
    if api.name=='gl':
      indent = '  '
      code += '  if (!gles)\n  {\n'

    for i in range(len(api.versions)):
      version = api.versions[i]
      versionMajor = version[0]
      versionMinor = version[1]

      # Latest version

      if i is 0:
        code += '%s  %s_version_%d_%d = '%(indent, name, versionMajor, versionMinor)
        if versionMinor > 0:
          code += '%s_version_major > %d || (%s_version_major == %d && %s_version_minor >= %d);\n' % (name, versionMajor, name, versionMajor, name, versionMinor)
        else:
          code += '%s_version_major >= %d;\n' % (name, versionMajor)
        continue

      versionLast = api.versions[i-1]

      code += '%s  %s_version_%d_%d = %s_version_%d_%d || '%(indent,name,versionMajor,versionMinor,name,versionLast[0],versionLast[1])
      if versionMinor > 0:
        code += '(%s_version_major == %d && %s_version_minor == %d);\n' % (name, versionMajor, name, versionMinor)
      else:
        code += '%s_version_major == %d;\n' % (name, versionMajor)

    if len(indent):
      code += '  }\n'

    code += '\n'

  return code

def extensionStringCode(apis, args):

  code = ''

  for api in apis:
    name = api.name.lower()
    if name in cond:
      code += '#if %s\n'%cond[name]
    for c in sorted(api.categories):
      code += '  %s = e.find("%s")!=e.end();\n' % (c.lower(),c)
    if name in cond:
      code += '#endif\n'
    code += '\n'

  return code

def getExtensionCode(apis, args):

  code = ''
  code += 'bool\n'
  code += 'ContextInfo::getExtension(const char *ext) const\n'
  code += '{\n'
  code += '  ITrace("ContextInfo::getExtension ",boost::print::quote(ext,\'"\'));\n'
  code += '\n'

  for api in apis:
    name = api.name.lower()
    if name in cond:
      code += '#if %s\n'%cond[name]
    for c in sorted(api.categories):
      if c.startswith('GL_REGAL_') or c=='GL_EXT_debug_marker':
        code += '  if (!strcmp(ext,"%s")) return true;\n' % (c)
      elif c=='GL_EXT_direct_state_access':
        code += '  if (!strcmp(ext,"%s")) return regal_ext_direct_state_access || %s;\n' % (c,c.lower())
      else:
        code += '  if (!strcmp(ext,"%s")) return %s;\n' % (c,c.lower())
    if name in cond:
      code += '#endif\n'
    code += '\n'

  code += 'return false;\n'
  code += '}\n\n'

  code += 'bool\n'
  code += 'ContextInfo::isSupported(const char *ext) const\n'
  code += '{\n'
  code += '  ITrace("ContextInfo::isSupported ",boost::print::quote(ext,\'"\'));\n'
  code += '\n'
  code += '  string_list<string> e;\n'
  code += '  e.split(ext,\' \');\n'
  code += '  for (string_list<string>::const_iterator i=e.begin(); i!=e.end(); ++i)\n'
  code += '    if (i->length() && !getExtension(i->c_str())) return false;\n'
  code += '  return true;\n'
  code += '}\n'

  return code

def generateContextInfoHeader(apis, args):

    substitute = {}
    substitute['LICENSE']         = regalLicense
    substitute['AUTOGENERATED']   = autoGeneratedMessage
    substitute['COPYRIGHT']       = args.copyright
    substitute['HEADER_NAME']     = "REGAL_CONTEXT_INFO"
    substitute['VERSION_DECLARE'] = versionDeclareCode(apis,args)

    outputCode( '%s/RegalContextInfo.h' % args.outdir, contextInfoHeaderTemplate.substitute(substitute))

def generateContextInfoSource(apis, args):

    substitute = {}
    substitute['LICENSE']        = regalLicense
    substitute['AUTOGENERATED']  = autoGeneratedMessage
    substitute['COPYRIGHT']      = args.copyright
    substitute['VERSION_INIT']   = versionInitCode(apis,args)
    substitute['VERSION_DETECT'] = versionDetectCode(apis,args)
    substitute['EXT_INIT']       = extensionStringCode(apis,args)
    substitute['EXT_CODE']       = getExtensionCode(apis,args)
    outputCode( '%s/RegalContextInfo.cpp' % args.outdir, contextInfoSourceTemplate.substitute(substitute))


##############################################################################################

lookupSourceTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalPrivate.h"
#include "RegalLookup.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Lookup {

${CODE}

}

REGAL_NAMESPACE_END

''')

def generateLookupSource(apis, args):

  code = []

  for i in apis:
    names = []
    for j in i.functions:
      names.append(j.name)

    if i.name in cond:
      code.append( '#if %s'%cond[i.name] )

    code.extend(pointerLookupByNameCode([ (j,j) for j in names ],("%s_Name"%i.name,"%s_Value"%i.name),valueCast = '(void *)(%s)'))

    # offset table
    table = "DispatchTableGlobal"
    if( i.name == "gl" ):
        table = "DispatchTable"

    val = [ (j,"offsetof(%s,%s)/sizeof(void *)"%(table,j)) for j in names ]
    val = sorted( val )
    code.append( 'const size_t %s_Offset[%d] = {'       % (i.name, len(val)+1) ) # terminating NULL
    for j in val:
      code.append( '  ' + j[1] + ',' )
    code.append( '  0')
    code.append('};')
    code.append('')



    if i.name in cond:
      code.append( '#endif' )
    code.append( '' )

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['CODE']          = '\n'.join(code)

  outputCode( '%s/RegalLookup.cpp' % args.outdir, lookupSourceTemplate.substitute(substitute))

##############################################################################################

lookupHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <cstdlib>
#include <cstring>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Lookup {

inline int NameCmp(const void *a, const void *b)
{
  return std::strcmp(*(const char **) a, *(const char **) b);
}

${CODE}

}

REGAL_NAMESPACE_END

#endif
''')

def generateLookupHeader(apis, args):

  code = []

  for i in apis:

    if i.name in cond:
      code.append( '#if %s'%cond[i.name] )

    names = []
    for j in i.functions:
      names.append( j.name )

    lName  = '%s_Name'%i.name
    lValue = '%s_Value'%i.name
    lOffset = '%s_Offset'%i.name
    lSize   = len(names)+1

    code.append( '' )
    code.append( 'extern const char * const %s[%d];'%(lName,lSize) )
    code.append( 'extern const void *%s[%d];'%(lValue,lSize) )
    code.append( 'extern const size_t %s[%d];'%(lOffset,lSize) )

    code.append('''

template<typename T>
T
%s(const char *name, T def = NULL)
{
  const char **res = (const char **) std::bsearch(&name, %s, %d, sizeof(const char *), NameCmp);
  return res ? reinterpret_cast<T>(const_cast<void *>(%s[(size_t) (res - %s)])) : def;
}

'''%('%s_Lookup'%i.name,lName,lSize-1,lValue,lName))

    code.append('''

inline size_t %s(const char *name)
{
  const char **res = (const char **) std::bsearch(&name, %s, %d, sizeof(const char *), NameCmp);
  return res ? %s[(size_t) (res - %s)] : 0;
}

'''%('%s_LookupOffset'%i.name,lName,lSize-1,lOffset,lName))

    if i.name in cond:
      code.append( '#endif' )
    code.append( '' )

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['HEADER_NAME']   = "REGAL_LOOKUP"
  substitute['CODE']          = '\n'.join(code)

  outputCode( '%s/RegalLookup.h' % args.outdir, lookupHeaderTemplate.substitute(substitute))

##############################################################################################

tokenSourceTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalPrivate.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Token {

  const char * GLbooleanToString(GLboolean v)
  {
    return v==GL_FALSE ? "GL_FALSE" : "GL_TRUE";
  }

  const char * internalFormatToString(GLint v)
  {
    const char *integer[5] = { "", "1", "2", "3", "4" };
    return 1<=v && v<=4 ? integer[v] : GLenumToString(v);
  }

${CODE}

}

REGAL_NAMESPACE_END

''')

def generateTokenSource(apis, args):

  code = []

  e = {}

  code.append('  const char * GLenumToString( GLenum e ) {')
  code.append('    switch( e ) {')

  for i in apis:
    if i.name != 'gl':
      continue
    for j in i.enums:
      if j.name != 'defines':
        continue
      for k in j.enumerants:
        value = toLong(k.value)
        if value != None:
          if not value in e:
            e[value] = set()
          e[value].add(k.name)

    e = sorted([ (i,sorted(list(e[i]))) for i in e.iterkeys() ])

    e = [ i for i in e if i[0] < 0xfffffffff ]

    # Filter out extension duplicates

    u = e
    for i in ['_ARB','_EXT','_NV','_ATI','_PGI','_OES','_IBM','_SUN','_SGI','_SGIX','_SGIS','_APPLE']:
      u = [ (j[0], [ k for k in j[1] if not k.endswith(i)  ]) for j in u ]

    # Filter out _BIT duplicates

    for i in ['_BIT','_BITS']:
      u = [ (j[0], [ k for k in j[1] if not k.endswith(i)  ]) for j in u ]

    u = [ (i[0], [ j for j in i[1] if not j.startswith('GL_KTX_') ]) for i in u  ]

    # Form tuple of value, filtered names, all names, per GLenum

    e = [ (e[i][0], u[i][1], e[i][1]) for i in xrange(len(e)) ]

    for i in e:
      value = i[0]
      if len(i[1]):
        name = i[1][0]
      else:
        name = i[2][0]

      if value==0:
        name = 'GL_ZERO'
      if value==1:
        name = 'GL_ONE'

      code.append('      case %s: return "%s";'%(hexValue(value,'0x%08x'),name))

  code.append('      default: break;')
  code.append('    }')
  code.append('  return "unknown_gl_enum";')
  code.append('  }')

  # GLerrorToString

  code.append('')
  code.append('  const char * GLerrorToString( GLenum e ) {')
  code.append('    switch( e ) {')
  for i in apis:
    if i.name != 'gl':
      continue
    for j in i.enums:
      if j.name != 'defines':
        continue
      for k in j.enumerants:
        if getattr(k,'gluErrorString',None):
          code.append('      case %s: return "%s";'%(k.name,k.gluErrorString))
  code.append('      default: break;')
  code.append('    }')
  code.append('  return NULL;')
  code.append('  }')


  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['CODE']          = '\n'.join(code)

  outputCode( '%s/RegalToken.cpp' % args.outdir, tokenSourceTemplate.substitute(substitute))

##############################################################################################

tokenHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Token {

  const char * GLenumToString        (GLenum    v);
  const char * GLerrorToString       (GLenum    v); // gluErrorString
  const char * GLbooleanToString     (GLboolean v);
  const char * internalFormatToString(GLint     v);

  inline const char *toString(const GLenum    v) { return GLenumToString(v);    }
  inline const char *toString(const GLboolean v) { return GLbooleanToString(v); }
}

REGAL_NAMESPACE_END

#endif
''')

def generateTokenHeader(apis, args):

  substitute = {}

  used = {}

  for i in apis:
    if i.name != 'gl':
      continue
    for enum in i.enums:
      if enum.name != 'defines':
        continue
      for enumerant in enum.enumerants:
        value = enumerant.value
        if isinstance(value, str) or isinstance(value, unicode):
          try:
            value = long(value)
          except:
            value = enumerant.value

        if value > pow(2,32):
          continue

        if (not value in used) or (len(used[ value ].name) > len(enumerant.name)):
            used[ value ] = enumerant

    sorted_used = [ x for x in used ]
    sorted_used.sort(key=lambda x: x)
    for u in sorted_used:
      value = u
      name = used[u].name

      # use hex value, if possible
      valueHex = value
      if isinstance(value, int):
        valueHex = '0x%x'%(value)
      if isinstance(value, long):
        valueHex = '0x%x'%(value)

      # Don't bother with decimal for 0-10
      if isinstance(value, int) or isinstance(value, long):
        if value>=0 and value<10:
            value = valueHex

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['HEADER_NAME']   = "REGAL_TOKEN"

  outputCode( '%s/RegalToken.h' % args.outdir, tokenHeaderTemplate.substitute(substitute))

##############################################################################################

enumHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

enum Enum {
${REGAL_ENUM}
};

REGAL_NAMESPACE_END

#endif
''')

def generateEnumHeader(apis, args):

  regalEnumSet = set()
  regalEnum = []

  for i in apis:
    if i.name == 'gl':
      for enum in i.enums:
        if enum.name == 'defines':
          for enumerant in enum.enumerants:
            if not enumerant.name in regalEnumSet:
              regalEnumSet.add(enumerant.name)
              regalEnum.append(enumerant.name)

  # GL_TIMEOUT_IGNORED 0xffffffffffffffff can't be represented as an enum

  regalEnum = [ '   R%s = %s,'%(i,i) for i in regalEnum if i not in [ 'GL_TIMEOUT_IGNORED' ] ]

  substitute = {}
  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['REGAL_ENUM']    = '\n'.join(regalEnum)
  substitute['HEADER_NAME']   = "REGAL_ENUM"

  outputCode( '%s/RegalEnum.h' % args.outdir, enumHeaderTemplate.substitute(substitute))

##############################################################################################

dispatchLogTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_LOG

REGAL_GLOBAL_BEGIN

#include "RegalLog.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"

using namespace REGAL_NAMESPACE_INTERNAL;
using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

#ifdef __cplusplus
extern "C" {
#endif

${API_FUNC_DEFINE}

#ifdef __cplusplus
}
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

void InitDispatchTableLog(DispatchTable &tbl)
{
${API_GLOBAL_DISPATCH_INIT}
}

REGAL_NAMESPACE_END

#endif
''')


def generateDispatchLog(apis, args):

  # CodeGen for API functions.

  code = ''
  categoryPrev = None

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'log_', name, params)
      code += '    %s\n' % debugPrintFunction( function, 'GTrace' )
      code += '    RegalContext *_context = GET_REGAL_CONTEXT();\n'
      code += '    RegalAssert(_context);\n'
      code += '    Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
      code += '    '
      if not typeIsVoid(rType):
        code += '%s ret = '%(rType)
      code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
      if not typeIsVoid(rType):
        code += '    return ret;\n'
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  funcInit   = apiDispatchFuncInitCode( apis, args, 'log' )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
# substitute['COPYRIGHT']     = args.copyright
  substitute['API_FUNC_DEFINE'] = code
  substitute['API_GLOBAL_DISPATCH_INIT'] = funcInit

  outputCode( '%s/RegalDispatchLog.cpp' % args.outdir, dispatchLogTemplate.substitute(substitute))

##############################################################################################

def generateDefFile(apis, args, additional_exports):

  code1 = []
  code2 = []
  code3 = []

  for i in apis:
    if i.name=='wgl' or i.name=='gl':
      for j in i.functions:
        code1.append('  %s'%j.name)
        code2.append('  r%s'%j.name)
    if i.name=='cgl' or i.name=='gl':
      for j in i.functions:
        code3.append('_%s'%j.name)
  code1.sort()
  code2.sort()
  code3.sort()

  code1.insert( 0, '  SetPixelFormat' )
  code2.insert( 0, '  SetPixelFormat' )

  # RegalMakeCurrent, RegalSetErrorCallback
  code1 += ['  %s' % export for export in additional_exports]
  code2 += ['  %s' % export for export in additional_exports]
  code3 += ['_%s' % export for export in additional_exports]

  outputCode( '%s/Regal.def'  % args.outdir, 'EXPORTS\n' + '\n'.join(code1))
  outputCode( '%s/Regalm.def' % args.outdir, 'EXPORTS\n' + '\n'.join(code2))
  outputCode( '%s/export_list_mac.txt' % args.outdir, '# File: export_list\n' + '\n'.join(code3))

##############################################################################################

sourceTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalLog.h"
#include "RegalIff.h"
#include "RegalToken.h"
#include "RegalState.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"
#include "RegalDebugInfo.h"
#include "RegalContextInfo.h"

#include "RegalMarker.h"

using namespace REGAL_NAMESPACE_INTERNAL;
using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

#ifdef __cplusplus
extern "C" {
#endif

${API_FUNC_DEFINE}

#ifdef __cplusplus
}
#endif

REGAL_GLOBAL_END
''')


def generateSource(apis, args):

  # CodeGen for API functions.

  apiFuncDefine = apiFuncDefineCode( apis, args )
  globalDispatch = apiGlobalDispatchFuncInitCode( apis, args )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_FUNC_DEFINE'] = apiFuncDefine
  substitute['API_GLOBAL_DISPATCH_INIT'] = globalDispatch

  outputCode( '%s/Regal.cpp' % args.outdir, sourceTemplate.substitute(substitute))


##############################################################################################

dispatchSourceTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

${IFDEF}REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalLog.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"
#include "RegalContext.h"
${LOCAL_INCLUDE}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

${LOCAL_CODE}

${API_DISPATCH_FUNC_DEFINE}

void InitDispatchTable${DISPATCH_NAME}(DispatchTable &tbl)
{
  ${API_DISPATCH_FUNC_INIT}
}

REGAL_NAMESPACE_END

${ENDIF}''')

##############################################################################################


loaderLocalCode = ''

def generateLoaderSource(apis, args):

  funcDefine = apiLoaderFuncDefineCode( apis, args )
  funcInit   = apiDispatchFuncInitCode( apis, args, 'loader' )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Loader'
  substitute['LOCAL_CODE']    = loaderLocalCode
  substitute['LOCAL_INCLUDE'] = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['IFDEF'] = '#if REGAL_DRIVER && !defined(__native_client__)\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchLoader.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))


##############################################################################################


emuLocalCode = '''


'''



def generateEmuSource(apis, args):

  funcDefine = apiEmuFuncDefineCode( apis, args )
  funcInit   = apiEmuDispatchFuncInitCode( apis, args )

  emuLocalInclude = '''

#include "RegalBin.h"
#include "RegalEmu.h"
#include "RegalPpa.h"
#include "RegalIff.h"
#include "RegalMarker.h"
#include "RegalObj.h"
#include "RegalDsa.h"
#include "RegalVao.h"'''

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Emu'
  substitute['LOCAL_CODE']    = emuLocalCode
  substitute['LOCAL_INCLUDE'] = emuLocalInclude
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['IFDEF'] = '#if REGAL_EMULATION\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchEmu.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))


##############################################################################################

def generateErrorSource(apis, args):

  funcDefine = apiErrorFuncDefineCode( apis, args )
  funcInit   = apiDispatchFuncInitCode( apis, args, 'error' )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Error'
  substitute['LOCAL_INCLUDE'] = ''
  substitute['LOCAL_CODE']    = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['IFDEF'] = '#if REGAL_ERROR\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchError.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))

##############################################################################################

# CodeGen for NaCL dispatch functions

def apiNaclFuncDefineCode(apis, args):

  code = ''

  for api in apis:

    if api.name=='gl':

      for function in api.functions:
        if not function.needsContext:
          continue
        if getattr(function,'esVersions',None)==None or 2.0 not in function.esVersions:
          continue

        name   = function.name
        params = paramsDefaultCode(function.parameters, True)
        callParams = paramsNameCode(function.parameters)
        rType  = typeCode(function.ret.type)
        naclName = name
        if naclName.startswith('gl'):
          naclName = naclName[2:]

        code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'nacl_', name, params)
        code += '  ITrace("nacl_%s");\n' % name
        code += '  RegalContext * rCtx = GET_REGAL_CONTEXT();\n'
        code += '  RegalAssert(rCtx)\n'
        code += '  RegalAssert(rCtx->naclES2)\n'
        code += '  RegalAssert(rCtx->naclES2->%s)\n'%(naclName)
        code += '  RegalAssert(rCtx->naclResource)\n'
        if not typeIsVoid(rType):
          code += '  %s ret = ' % rType
        else:
          code += '  '
        if len(callParams):
          callParams = 'rCtx->naclResource, %s'%callParams
        else:
          callParams = 'rCtx->naclResource'
        code += 'rCtx->naclES2->%s(%s);\n' % ( naclName, callParams )
        if not typeIsVoid(rType):
          code += '  return ret;\n'
        code += '}\n\n'

  return code

def apiNaclFuncInitCode(apis, args):

  code = '// OpenGL ES 2.0 only\n'

  for api in apis:

    if api.name=='gl':

      for function in api.functions:
        if not function.needsContext:
          continue
        if getattr(function,'esVersions',None)==None or 2.0 not in function.esVersions:
          continue

        name   = function.name
        params = paramsDefaultCode(function.parameters, True)
        callParams = paramsNameCode(function.parameters)
        rType  = typeCode(function.ret.type)

        code += '  tbl.%s = %s_%s;\n' % ( name, 'nacl', name )

  return code

def generateNaclSource(apis, args):

  funcDefine = apiNaclFuncDefineCode( apis, args )
  funcInit   = apiNaclFuncInitCode  ( apis, args )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Nacl'
  substitute['LOCAL_INCLUDE'] = '#include <ppapi/c/ppb_opengles2.h>'
  substitute['LOCAL_CODE']    = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['IFDEF'] = '#if REGAL_DRIVER && defined(__native_client__)\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchNacl.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))


##############################################################################################

def apiStaticES2FuncInitCode(apis, args):

  code = '// OpenGL ES 2.0 only\n\n'

  for api in apis:
    if api.name=='gl':
      for function in api.functions:
        if not function.needsContext:
          continue
        if getattr(function,'esVersions',None)==None or 2.0 not in function.esVersions:
          continue
        name   = function.name
        code += '  tbl.r%s = %s;\n' % ( name, name )

  return code

def generateStaticES2Source(apis, args):

  code  = '\n'

  code += '#ifndef REGAL_NAMESPACE\n'
  code += '#error REGAL_STATIC_ES2 requires REGAL_NAMESPACE\n'
  code += '#endif\n\n'

  for api in apis:
    if api.name=='gl':

      for function in api.functions:
        if not function.needsContext:
          continue
        if getattr(function,'esVersions',None)==None or 2.0 not in function.esVersions:
          continue
        name   = function.name
        code += '#undef %s\n' % ( name )

      code += '\n'
      code += 'extern "C"\n'
      code += '{\n'

      for function in api.functions:
        if not function.needsContext:
          continue
        if getattr(function,'esVersions',None)==None or 2.0 not in function.esVersions:
          continue
        params = paramsDefaultCode(function.parameters, True)
        rType  = typeCode(function.ret.type)
        code += '  extern %s REGAL_CALL %s(%s);\n' % (rType, function.name, params)

      code += '}\n'

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'StaticES2'
  substitute['LOCAL_INCLUDE'] = code
  substitute['LOCAL_CODE']    = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = ''
  substitute['API_DISPATCH_FUNC_INIT'] = apiStaticES2FuncInitCode( apis, args )
  substitute['IFDEF'] = '#if REGAL_DRIVER && REGAL_STATIC_ES2\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchStaticES2.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))

##############################################################################################

dispatchGlobalSourceTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

${IFDEF}REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalDispatch.h"
${LOCAL_INCLUDE}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

${LOCAL_CODE}

${API_DISPATCH_FUNC_DEFINE}

void InitDispatchTable${DISPATCH_NAME}(DispatchTableGlobal &tbl)
{
  ${API_DISPATCH_FUNC_INIT}
}

REGAL_NAMESPACE_END

${ENDIF}''')

def apiStaticEGLFuncInitCode(apis, args):

  code = ''

  code += '// EGL global dispatch\n'

  for api in apis:
    if api.name=='egl':
      for function in api.functions:
        name   = function.name
        code += '  tbl.r%s = ::%s;\n' % ( name, name )

  return code

def generateStaticEGLSource(apis, args):

  code  = '\n'

  code += '#ifndef REGAL_NAMESPACE\n'
  code += '#error REGAL_STATIC_EGL requires REGAL_NAMESPACE\n'
  code += '#endif\n\n'

  for api in apis:
    if api.name=='egl':

      for function in api.functions:
        name   = function.name
        code += '#undef %s\n' % ( name )

      code += '\n'
      code += 'extern "C"\n'
      code += '{\n'

      for function in api.functions:
        params = paramsDefaultCode(function.parameters, True)
        rType  = typeCode(function.ret.type)
        code += '  extern %s REGAL_CALL %s(%s);\n' % (rType, function.name, params)

      code += '}\n'

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'StaticEGL'
  substitute['LOCAL_INCLUDE'] = code
  substitute['LOCAL_CODE']    = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = ''
  substitute['API_DISPATCH_FUNC_INIT'] = apiStaticEGLFuncInitCode( apis, args )
  substitute['IFDEF'] = '#if REGAL_SYS_ANDROID && REGAL_DRIVER && REGAL_STATIC_EGL\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchStaticEGL.cpp' % args.outdir, dispatchGlobalSourceTemplate.substitute(substitute))

##############################################################################################

# CodeGen for missing dispatch functions

def apiMissingFuncDefineCode(apis, args):

  code = ''
  categoryPrev = None

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += '%sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'missing_', name, params)
      for param in function.parameters:
        code += '  UNUSED_PARAMETER(%s);\n' % param.name
      code += '  Warning( "%s not available." );\n' % name
      if not typeIsVoid(rType):
        if rType[-1] != '*':
          code += '  return (%s)0;\n' % ( rType )
        else:
          code += '  return NULL;\n'
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  return code

def generateMissingSource(apis, args):

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Missing'
  substitute['LOCAL_INCLUDE'] = ''
  substitute['LOCAL_CODE']    = ''
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = apiMissingFuncDefineCode( apis, args )
  substitute['API_DISPATCH_FUNC_INIT']   = apiDispatchFuncInitCode( apis, args, 'missing' )
  substitute['IFDEF'] = ''
  substitute['ENDIF'] = ''

  outputCode( '%s/RegalDispatchMissing.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))


##############################################################################################

debugGlobalCode = '''
#include "RegalDebugInfo.h"
'''

debugLocalCode = '''
'''

def generateDebugSource(apis, args):

  funcDefine = apiDebugFuncDefineCode( apis, args )
  funcInit   = apiDispatchFuncInitCode( apis, args, 'debug' )

  # Output

  substitute = {}

  substitute['LICENSE'] = regalLicense
  substitute['DISPATCH_NAME'] = 'Debug'
  substitute['LOCAL_INCLUDE'] = debugGlobalCode
  substitute['LOCAL_CODE']    = debugLocalCode
  substitute['AUTOGENERATED'] = autoGeneratedMessage
  substitute['COPYRIGHT']     = args.copyright
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['IFDEF'] = '#if REGAL_DEBUG\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchDebug.cpp' % args.outdir, dispatchSourceTemplate.substitute(substitute))


##############################################################################################


def apiTypedefCode( apis, args ):

  code = ''
  for api in apis:
    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]
    if api.name == 'wgl':
      code += '#ifdef  REGAL_SYS_WGL_DECLARE_WGL\n'
      code += '#ifndef _WINDEF_\n'
    for typedef in api.typedefs:

      if api.name == 'wgl' and typedef.name=='GLYPHMETRICSFLOAT':
        code += '#endif\n'
        code += '#ifndef _WINGDI_\n'
      if api.name == 'wgl' and typedef.name=='HPBUFFERARB':
        code += '#endif\n'

      if re.search( '\(\s*\*\s*\)', typedef.type ):
        code += 'typedef %s;\n' % ( re.sub( '\(\s*\*\s*\)', '(*%s)' % typedef.name, typedef.type ) )
      else:
        code += 'typedef %s %s;\n' % ( typedef.type, typedef.name )

    if api.name == 'wgl':
      code += '#endif // REGAL_SYS_WGL_DECLARE_WGL\n'
    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  return code


# CodeGen for custom API definitions.

def apiEnumCode( apis, args ):

  code = ''
  for api in apis:
    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]
    if api.name == 'wgl':
      code += '#if REGAL_SYS_WGL_DECLARE_WGL\n'
    for enum in api.enums:
      if enum.name == 'defines':
        pass
      else:
        code += '\ntypedef enum _%s {\n' % enum.name
        for enumerant in enum.enumerants:
          code += '  %s = %s,\n' % ( enumerant.name, enumerant.value )
        code += '} %s;\n\n' % enum.name
    if api.name == 'wgl':
      code += '#endif // REGAL_SYS_WGL_DECLARE_WGL\n'
    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  return code

def apiGlobalDispatchTableDefineCode(apis, args):
  categoryPrev = None
  code = ''

  code += 'struct DispatchTableGlobal {\n'

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:

      if function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev if block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new if block.
      if category and not (category == categoryPrev):
        code += '    // %s\n\n' % category

      code += '    %s(REGAL_CALL *%s)(%s);\n' % (rType, name, params)

      categoryPrev = category

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'


  # Close pending if block.
  if categoryPrev:
    code += '\n'

  code += '};\n'

  return code

def apiDispatchTableDefineCode(apis, args):
  categoryPrev = None
  code = ''

  code += 'struct DispatchTable {\n'

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:

      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev if block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new if block.
      if category and not (category == categoryPrev):
        code += '    // %s\n\n' % category

      code += '    %s(REGAL_CALL *%s)(%s);\n' % (rType, name, params)

      categoryPrev = category

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'


  # Close pending if block.
  if categoryPrev:
    code += '\n'

  code += '};\n'

  return code

# CodeGen for API function declaration.

def apiFuncDeclareCode(apis, args):
  code = ''

  for api in apis:

    d = [] # defines
    e = [] # enums
    t = [] # function pointer typedefs
    m = [] # mangled names for REGAL_NAMESPACE support
    f = [] # gl names

    for enum in api.enums:
      if enum.name == 'defines':
        for enumerant in enum.enumerants:
          value = toLong(enumerant.value)
          if value==None:
            value = enumerant.value

          # Don't bother with decimal for 0-10
          if isinstance(value, long) and value>=10:
            e.append((enumerant.category, '#define %s %s /* %s */'%(enumerant.name,hexValue(value),value)))
          else:
            e.append((enumerant.category, '#define %s %s'%(enumerant.name,hexValue(value))))

    for function in api.functions:

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      t.append((category,funcProtoCode(function, version, 'REGAL_CALL', True)))
      m.append((category,'#define %-35s r%-35s' % (name, name) ))
      f.append((category,'REGAL_DECL %sREGAL_CALL %s(%s);' % (rType, name, params) ))

    def cmpEnum(a,b):
      if a[0]==b[0]:
        aValue = a[1].split(' ')[2]
        bValue = b[1].split(' ')[2]
        if aValue==bValue:
          return cmp(a[1].split(' ')[1], b[1].split(' ')[1])
        else:
          return cmp(aValue,bValue)
      return cmp(a[0],b[0])

    def enumIfDef(category):
      return '#ifndef REGAL_NO_ENUM_%s'%(upper(category).replace(' ','_'))

    def typedefIfDef(category):
      return '#ifndef REGAL_NO_TYPEDEF_%s'%(upper(category).replace(' ','_'))

    def namespaceIfDef(category):
      return '#ifndef REGAL_NO_NAMESPACE_%s'%(upper(category).replace(' ','_'))

    def declarationIfDef(category):
      return '#ifndef REGAL_NO_DECLARATION_%s'%(upper(category).replace(' ','_'))

    categories = set()
    categories.update([ i[0] for i in e ])
    categories.update([ i[0] for i in t ])
    categories.update([ i[0] for i in m ])
    categories.update([ i[0] for i in f ])

    for i in categories:
      if len(i):
        cat = upper(i).replace(' ','_')

        d.append((i,'#if (defined(%s) || defined(REGAL_NO_ENUM) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_ENUM_%s)'%(cat,cat,cat)))
        d.append((i,'#define REGAL_NO_ENUM_%s'%(cat)))
        d.append((i,'#endif'))
        d.append((i,''))

        d.append((i,'#if (defined(%s) || defined(REGAL_NO_TYPEDEF) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_TYPEDEF_%s)'%(cat,cat,cat)))
        d.append((i,'#define REGAL_NO_TYPEDEF_%s'%(cat)))
        d.append((i,'#endif'))
        d.append((i,''))

        d.append((i,'#if (defined(%s) || !defined(REGAL_NAMESPACE) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_NAMESPACE_%s)'%(cat,cat,cat)))
        d.append((i,'#define REGAL_NO_NAMESPACE_%s'%(cat)))
        d.append((i,'#endif'))
        d.append((i,''))

        d.append((i,'#if (defined(%s) || defined(REGAL_NO_DECLARATION) || defined(REGAL_NO_%s)) && !defined(REGAL_NO_DECLARATION_%s)'%(cat,cat,cat)))
        d.append((i,'#define REGAL_NO_DECLARATION_%s'%(cat)))
        d.append((i,'#endif'))
        d.append((i,''))

        d.append((i,'#ifndef %s'%(i)))
        d.append((i,'#define %s 1'%(i)))
        d.append((i,'#endif'))
        d.append((i,''))

    e.sort(cmpEnum)
    e = alignDefineCategory(e)
    e = ifCategory(e,enumIfDef)
    e = spaceCategory(e)

    t.sort()
    t = ifCategory(t,typedefIfDef)
    t = spaceCategory(t)

    m.sort()
    m = ifCategory(m,namespaceIfDef)
    m = spaceCategory(m)

    f.sort()
    f = ifCategory(f,declarationIfDef)
    f = spaceCategory(f)

    d.extend(e)
    d.extend(t)
    d.extend(m)
    d.extend(f)

    tmp = listToString(unfoldCategory(d,'/**\n ** %s\n **/',lambda x,y: cmp(x[0], y[0])))

    if api.name == 'wgl':
      tmp = wrapIf('REGAL_SYS_WGL_DECLARE_WGL',tmp)
    if api.name in cond:
      tmp = wrapIf(cond[api.name], tmp)

    code += '%s\n'%(tmp)

  return code

## Map gl.py helper functions to Regal namespace

helpers = {
  'helperGLCallListsSize'         : 'helper::size::callLists',
  'helperGLFogvSize'              : 'helper::size::fogv',
  'helperGLLightvSize'            : 'helper::size::lightv',
  'helperGLLightModelvSize'       : 'helper::size::lightModelv',
  'helperGLMaterialvSize'         : 'helper::size::materialv',
  'helperGLTexParametervSize'     : 'helper::size::texParameterv',
  'helperGLTexEnvvSize'           : 'helper::size::texEnvv',
  'helperGLTexGenvSize'           : 'helper::size::texGenv',
  'helperGLNamedStringSize'       : 'helper::size::namedString',
#  'helperGLDrawElementsSize'      : 'helper::size::drawElements',
  'helperGLNamedStringParamsSize' : 'helper::size::namedStringParams'
}

# debug print function
def debugPrintFunction(function, trace = 'ITrace'):
  c =  ''
  args = []
  for i in function.parameters:

    if i.output:
      continue

    # Use a cast, if necessary

    t = i.type
    n = i.name
    if i.cast != None:
      t = i.cast
      n = 'reinterpret_cast<%s>(%s)'%(t,n)

    # If it's array of strings, quote each string

    quote = ''
    if t == 'char **' or t == 'const char **' or t == 'GLchar **' or t == 'const GLchar **' or t == 'LPCSTR *':
      quote = ',"\\\""'

    if i.regalLog != None:
      args.append('%s'%i.regalLog)
    elif t == 'GLenum':
      args.append('toString(%s)'%n)
    elif t == 'GLboolean' or t == 'const GLboolean':
      args.append('toString(%s)'%n)
    elif t == 'char *' or t == 'const char *' or t == 'GLchar *' or t == 'const GLchar *' or t == 'LPCSTR':
      args.append('boost::print::quote(%s,\'"\')'%n)
    elif i.input and i.size!=None and (isinstance(i.size,int) or isinstance(i.size, long)) and t.find('void')==-1 and t.find('PIXELFORMATDESCRIPTOR')==-1:
      args.append('boost::print::array(%s,%s)'%(n,i.size))
    elif i.input and i.size!=None and (isinstance(i.size, str) or isinstance(i.size, unicode)) and t.find('void')==-1 and t.find('PIXELFORMATDESCRIPTOR')==-1 and i.size.find('helper')==-1:
      args.append('boost::print::array(%s,%s%s)'%(n,i.size,quote))
    elif i.input and i.size!=None and (isinstance(i.size, str) or isinstance(i.size, unicode)) and t.find('void')==-1 and t.find('PIXELFORMATDESCRIPTOR')==-1 and i.size.find('helper')==0:
      h = i.size.split('(')[0]
      if h in helpers:
        args.append('boost::print::array(%s,%s(%s%s)'%(n,helpers[h],i.size.split('(',1)[1],quote))
      else:
        args.append(n)
    elif t.startswith('GLDEBUG'):
      pass
    elif t.startswith('GLLOGPROC'):
      pass
    else:
      args.append(n)

  args = args[:9]
  if len(args):
    c += '%s("%s(", ' % (trace, function.name)
    c += ', ", ", '.join(args)
    c += ', ")");'
  else:
    c += '%s("%s()");' % (trace, function.name)
  return c

def apiFuncDefineCode(apis, args):

  code = ''
  for api in apis:

    tmp = []
    for function in api.functions:

      name       = function.name
      params     = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType      = typeCode(function.ret.type)
      category   = getattr(function, 'category', None)
      version    = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      c = ''
      c += 'REGAL_DECL %sREGAL_CALL %s(%s) \n{\n' % (rType, name, params)

      emue = [ emuFindEntry( function, i['formulae'], i['member'] ) for i in emuRegal ]

      if function.needsContext:
        c += '  RegalContext *_context = GET_REGAL_CONTEXT();\n'
        c += '  %s\n' % debugPrintFunction( function, 'RTrace' )
        c += '  if (!_context) return'
        if typeIsVoid(rType):
          c += ';\n'
        else:
          if rType[-1] != '*':
            c += ' (%s)0;\n' % ( rType )
          else:
            c += ' NULL;\n'

        c += listToString(indent(emuCodeGen(emue,'impl'),'  '))

        c += '  '
        if not typeIsVoid(rType):
          c += 'return '
        c += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
      else:
        c += '  %s\n' % debugPrintFunction(function, 'RTrace' )
        c += '  if (dispatchTableGlobal.%s == NULL) {\n' % name
        c += '    GetProcAddress( dispatchTableGlobal.%s, "%s" );\n' % ( name, name )
        c += '    RegalAssert(dispatchTableGlobal.%s!=%s);\n' % ( name, name )
        c += '    if (dispatchTableGlobal.%s==%s)\n' % ( name, name )
        c += '      dispatchTableGlobal.%s = NULL;\n' % ( name )
        c += '  }\n'

        if not typeIsVoid(rType):
          if rType[-1] != '*':
            c += '  %s ret = (%s)0;\n' % ( rType, rType )
          else:
            c += '  %s ret = NULL;\n' % rType

        c += listToString(indent(emuCodeGen(emue,'impl'),'  '))

        c += '  if (dispatchTableGlobal.%s) {\n' % name
        c += '    %s\n' % debugPrintFunction( function, 'GTrace' )
        c += '    '
        if not typeIsVoid(rType):
          c += 'ret = '
        c += 'dispatchTableGlobal.%s(%s);\n' % ( name, callParams )
        if name == 'wglMakeCurrent':
          c += '    RegalMakeCurrent(RegalSystemContext(hglrc));\n'
        elif name == 'CGLSetCurrentContext':
          c += '    RegalMakeCurrent( ctx );\n'
        elif name == 'glXMakeCurrent':
          c += '    RegalMakeCurrent( RegalSystemContext(ctx) );\n'
        elif name == 'eglMakeCurrent':
          c += '    RegalMakeCurrent( ctx );\n'
        c += '  }\n'
        c += '  else\n'
        c += '    Warning( "%s not available." );\n' % name

        c += listToString(indent(emuCodeGen(emue,'suffix'),'  '))

        if not typeIsVoid(rType):
          c += '  return ret;\n'
      c += '}\n\n'

      tmp.append( (category, c) )

    tmp = listToString(unfoldCategory(tmp))

    if api.name in cond:
      tmp = wrapIf(cond[api.name], tmp)

    code += tmp

  return code

# CodeGen for API loader function definition.

def apiLoaderFuncDefineCode(apis, args):
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'loader_', name, params)
      code += '  RegalContext * _context = GET_REGAL_CONTEXT();\n'
      code += '  RegalAssert(_context);\n'
      code += '  DispatchTable &_driver = _context->dispatcher.driver;\n'
      code += '  GetProcAddress(_driver.%s, "%s");\n' % (name, name)
      code += '  if (_driver.%s) {\n    ' % name
      if not typeIsVoid(rType):
        code += 'return '
      code += '_driver.%s(%s);\n' % ( name, callParams )
      if typeIsVoid(rType):
        code += '    return;\n'
      code += '  }\n'
      code += '  Dispatcher::ScopedStep stepDown(_context->dispatcher);\n  '
      if not typeIsVoid(rType):
        code += 'return '
      code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n'%(name, callParams)
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code


# CodeGen for API emu function definition.

def apiEmuFuncDefineCode(apis, args):
    categoryPrev = None
    code = ''

    for api in apis:

        code += '\n'
        if api.name in cond:
            code += '#if %s\n' % cond[api.name]

        for function in api.functions:
            if not function.needsContext:
                continue

            name   = function.name
            params = paramsDefaultCode(function.parameters, True)
            callParams = paramsNameCode(function.parameters)
            rType  = typeCode(function.ret.type)
            category  = getattr(function, 'category', None)
            version   = getattr(function, 'version', None)

            if category:
                category = category.replace('_DEPRECATED', '')
            elif version:
                category = version.replace('.', '_')
                category = 'GL_VERSION_' + category

            # Close prev category block.
            if categoryPrev and not (category == categoryPrev):
                code += '\n'

            # Begin new category block.
            if category and not (category == categoryPrev):
                code += '// %s\n\n' % category

            categoryPrev = category

            emue = [ emuFindEntry( function, i['formulae'], i['member'] ) for i in emu ]

            if all(i is None for i in emue):
                continue

            code += '\nstatic %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'emu_', name, params)
            code += '   RegalContext *_context = GET_REGAL_CONTEXT();\n'
            #code += '   RegalCheckGLError( rCtx );\n'
            code += '\n'

            level = [ (emu[i], emuFindEntry( function, emu[i]['formulae'], emu[i]['member'] )) for i in range( len( emue ) - 1 ) ]

            if not all(i[1]==None or not 'prefix' in i[1] and not 'impl' in i[1] for i in level):
              code += '   // prefix\n'
              code += '   switch( _context->emuLevel ) {\n'
              for i in level:
                  l,e = i[0], i[1]
                  code += '       case %d :\n' % l['level']
                  if l['ifdef']:
                      code += '         #if %s\n' % l['ifdef']
                  if e != None and 'prefix' in e :
                      if l['member'] :
                          code += '         if (_context->%s) {\n' % l['member']
                          code += '             RegalEmuScopedActivate activate( _context, _context->%s );\n' % l['member']
                      for j in e['prefix'] :
                          code += '             %s\n' % j
                      if l['member'] :
                          code += '         }\n'
                  if e!= None and 'impl' in e and l['member']:
                      code += '         if (_context->%s) break;\n' % l['member'];
                  if l['ifdef']:
                      code += '         #endif\n'
              code += '       default:\n'
              code += '           break;\n'
              code += '   }\n\n'

            if not all(i[1]==None or not 'impl' in i[1] for i in level):
              code += '   // impl\n'
              code += '   switch( _context->emuLevel ) {\n'
              for i in level:
                  l,e = i[0], i[1]
                  code += '       case %d :\n' % l['level']
                  if l['ifdef']:
                      code += '         #if %s\n' % l['ifdef']
                  if e != None and 'impl' in e :
                      if l['member'] :
                        code += '         if (_context->%s) {\n' % l['member']
                        code += '             RegalEmuScopedActivate activate( _context, _context->%s );\n' % l['member']
                      for j in e['impl'] :
                          code += '             %s\n' % j
                      if l['member'] :
                          if typeIsVoid(rType):
                              code += '             return;\n'
                          code += '         }\n'
                  if l['ifdef']:
                      code += '         #endif\n'
              code += '       default: {\n'
              # debug print
              # code += '           %s\n' % debugPrintFunction( function, 'GTrace' )
              # debug print
              code += '         Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
              code += '         '
              if not typeIsVoid(rType):
                  code += 'return '

              code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
              code += '         break;\n'
              code += '       }\n\n'
              code += '   }\n\n'
            else:
              code += '   Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
              code += '   '
              if not typeIsVoid(rType):
                  code += 'return '
              code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
            code += '}\n\n'

        if api.name in cond:
            code += '#endif // %s\n' % cond[api.name]
        code += '\n'

    # Close pending if block.
    if categoryPrev:
        code += '\n'

    return code


# CodeGen for API error checking function definition.

def apiErrorFuncDefineCode(apis, args):
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'error_', name, params)
      code += '  ITrace("error_%s");\n' % name
      code += '  RegalContext *_context = GET_REGAL_CONTEXT();\n'
      code += '  RegalAssert(_context);\n'
      if name != 'glGetError':
        code += '  GLenum _error = GL_NO_ERROR;\n'
        code += '  Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
        code += '  if (!_context->depthBeginEnd)\n'
        code += '    _error = _context->dispatcher.call(&_context->dispatcher.table().glGetError)();\n'
        code += '  RegalAssert(_error==GL_NO_ERROR);\n'
        code += '  '
        if not typeIsVoid(rType):
          code += '%s ret = ' % rType
        code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
        code += '  if (!_context->depthBeginEnd) {\n'
        code += '    _error = _context->dispatcher.call(&_context->dispatcher.table().glGetError)();\n'
        code += '    if (_error!=GL_NO_ERROR) {\n'
        code += '      Error("%s : ",Token::GLerrorToString(_error));\n'%(name)
        code += '      if (_context->err.callback)\n'
        code += '        _context->err.callback( _error );\n'
        code += '    }\n'
        code += '  }\n'
        if not typeIsVoid(rType):
          code += 'return ret;\n'
      else:
        code += '  Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
        code += '  GLenum error = _context->dispatcher.call(&_context->dispatcher.table().glGetError)();\n'
        code += '  return error;\n'
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code


# CodeGen for API debug function definition.

def apiDebugFuncDefineCode(apis, args):
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += 'static %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'debug_', name, params)
      code += '  RegalContext *_context = GET_REGAL_CONTEXT();\n'
      code += '  RegalAssert(_context);\n'
      code += '  Dispatcher::ScopedStep stepDown(_context->dispatcher);\n'
      e = emuFindEntry( function, debugDispatchFormulae, '' )
      if e != None and 'prefix' in e :
        for l in e['prefix'] :
          code += '  %s\n' % l
      code += '  '
      if not typeIsVoid(rType):
        code += '%s ret = ' % rType
      code += '_context->dispatcher.call(&_context->dispatcher.table().%s)(%s);\n' % ( name, callParams )
      if not typeIsVoid(rType):
        code += '  return ret;\n'
      code += '}\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code

# CodeGen for dispatch table init.

def apiGlobalDispatchFuncInitCode(apis, args):
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += '  dispatchTableGlobal.%s = %s_%s;\n' % ( name, 'loader', name )

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code


# CodeGen for dispatch table init.

def apiDispatchFuncInitCode(apis, args, dispatchName):
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '  // %s\n\n' % category

      categoryPrev = category

      code += '  tbl.%s = %s_%s;\n' % ( name, dispatchName, name )

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code

# CodeGen for dispatch table init.

def apiEmuDispatchFuncInitCode(apis, args):
  dispatchName = 'emu'
  categoryPrev = None
  code = ''

  for api in apis:

    code += '\n'
    if api.name in cond:
      code += '#if %s\n' % cond[api.name]

    for function in api.functions:
      if not function.needsContext:
        continue

      name   = function.name

      emue = [ None ]
      for i in range( len( emu ) - 1 ) :
        emue.append( emuFindEntry( function, emu[i]['formulae'], emu[i]['member'] ) )

      if all(i is None for i in emue):
        continue

      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType  = typeCode(function.ret.type)
      category  = getattr(function, 'category', None)
      version   = getattr(function, 'version', None)

      if category:
        category = category.replace('_DEPRECATED', '')
      elif version:
        category = version.replace('.', '_')
        category = 'GL_VERSION_' + category

      # Close prev category block.
      if categoryPrev and not (category == categoryPrev):
        code += '\n'

      # Begin new category block.
      if category and not (category == categoryPrev):
        code += '// %s\n\n' % category

      categoryPrev = category

      code += '   tbl.%s = %s_%s;\n' % ( name, dispatchName, name )

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  # Close pending if block.
  if categoryPrev:
    code += '\n'

  return code

# Class Export

class Export:

  def __init__(self):

    parser = OptionParser()
    parser.add_option('-a', '--api',       dest = 'apis',      metavar = 'API VERSION', action = 'append', nargs = 2, help = 'generate loader for API and VERSION')
    parser.add_option('-c', '--copyright', dest = 'copyright',                          action = 'store_true',        help = 'include copyright notice')
    parser.add_option(      '--outdir',    dest = 'outdir',    metavar = 'DIR',                                       help = 'output directory')
    parser.set_defaults(apis = [])
    (options, args) = parser.parse_args()

    if not len(options.apis):
      parser.error('Specify an api.\n  See Export.py --help')

    apis = []
    for apiItem in options.apis:

      # Some fakery for gles - use the gl database

      if apiItem[0]=='gles':
        api = deepcopy(importAttr('gl'))
        api.name = 'gles'
      else:
        api = importAttr(apiItem[0])

      api.version = float(apiItem[1])
      api.name = '%s' % apiItem[0]
      apis.append(api)

    class Args(object) : pass
    genArgs = Args()

    genArgs.copyright = ''
    if options.copyright:
      genArgs.copyright = copyrightMessage

    genArgs.outdir    = options.outdir.strip()

    generate(apis, genArgs)

export = Export()
