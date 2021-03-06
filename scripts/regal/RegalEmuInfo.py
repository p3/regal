#!/usr/bin/python -B

from string import Template, upper, replace

from ApiUtil import outputCode

cond = { 'wgl' : 'REGAL_SYS_WGL', 'glx' : 'REGAL_SYS_GLX', 'cgl' : 'REGAL_SYS_OSX', 'egl' : 'REGAL_SYS_EGL' }

emuInfoHeaderTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

/*

 Regal emulation information and runtime emulation limits
 Nigel Stewart, Scott Nations

 */

#ifndef __${HEADER_NAME}_H__
#define __${HEADER_NAME}_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct ContextInfo;

struct EmuInfo
{
  EmuInfo();
  ~EmuInfo();

  void init(const ContextInfo &contextInfo);

  // glewGetExtension, glewIsSupported

  bool getExtension(const ContextInfo &contextInfo, const char *ext) const;
  bool isSupported(const ContextInfo &contextInfo, const char *ext) const;

  // Reported by Regal to the application

  std::string vendor;
  std::string renderer;
  std::string version;
  std::string extensions;

  //

  std::set<std::string> extensionsSet;

  // Emulated extensions and limits

${IMPL_DECLARE}
};

REGAL_NAMESPACE_END

#endif // __${HEADER_NAME}_H__
''')

emuInfoSourceTemplate = Template( '''${AUTOGENERATED}
${LICENSE}

/*

 Regal emulation information and runtime emulation limits
 Nigel Stewart, Scott Nations

 */

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include "RegalEmu.h"
#include "RegalEmuInfo.h"

using namespace std;

#include <boost/print/string_list.hpp>
using namespace boost::print;

#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

EmuInfo::EmuInfo()
:
${IMPL_INIT}
{
   Internal("EmuInfo::EmuInfo","()");
}

EmuInfo::~EmuInfo()
{
   Internal("EmuInfo::~EmuInfo","()");
}

void
EmuInfo::init(const ContextInfo &contextInfo)
{
  // TODO - filter out extensions Regal doesn't support?

#ifdef REGAL_GL_VENDOR
  vendor = REGAL_EQUOTE(REGAL_GL_VENDOR);
#else
  vendor = contextInfo.vendor;
#endif

#ifdef REGAL_GL_RENDERER
  renderer = REGAL_EQUOTE(REGAL_GL_RENDERER);
#else
  renderer = contextInfo.renderer;
#endif

#ifdef REGAL_GL_VERSION
  version = REGAL_EQUOTE(REGAL_GL_VERSION);
#else
  version = contextInfo.version;
#endif

#ifdef REGAL_GL_EXTENSIONS
  {
    string_list<string> extList;
    extList.split(REGAL_EQUOTE(REGAL_GL_EXTENSIONS),' ');
    extensionsSet.clear();
    extensionsSet.insert(extList.begin(),extList.end());
  }
#else
  static const char *ourExtensions[9] = {
    "GL_REGAL_log",
    "GL_REGAL_enable",
    "GL_REGAL_error_string",
    "GL_REGAL_extension_query",
    "GL_REGAL_ES1_0_compatibility",
    "GL_REGAL_ES1_1_compatibility",
    "GL_EXT_debug_marker",
    "GL_GREMEDY_string_marker",
    "GL_GREMEDY_frame_terminator"
  };
  extensionsSet = contextInfo.extensionsSet;
  extensionsSet.insert(&ourExtensions[0],&ourExtensions[9]);
#endif

#ifndef REGAL_NO_GETENV
  {
    getEnv("REGAL_GL_VENDOR",   vendor);
    getEnv("REGAL_GL_RENDERER", renderer);
    getEnv("REGAL_GL_VERSION",  version);

    const char *extensionsEnv = getEnv("REGAL_GL_EXTENSIONS");
    if (extensionsEnv)
    {
      string_list<string> extList;
      extList.split(extensionsEnv,' ');
      extensionsSet.clear();
      extensionsSet.insert(extList.begin(),extList.end());
    }
  }
#endif

  // Form Regal extension string from the set

  extensions = ::boost::print::detail::join(extensionsSet,string(" "));

  Info("Regal vendor     : ",vendor);
  Info("Regal renderer   : ",renderer);
  Info("Regal version    : ",version);
  Info("Regal extensions : ",extensions);

${IMPL_GET}

  if (gl_max_vertex_attribs > REGAL_EMU_MAX_VERTEX_ATTRIBS)
      gl_max_vertex_attribs = REGAL_EMU_MAX_VERTEX_ATTRIBS;

  // Qualcomm fails with float4 attribs with 256 byte stride, so artificially limit to 8 attribs (n*16 is used
  // as the stride in RegalIFF).  WebGL (and Pepper) explicitly disallows stride > 255 as well.

  if (contextInfo.vendor == "Qualcomm" || contextInfo.vendor == "Chromium" || contextInfo.webgl)
    gl_max_vertex_attribs = std::min<GLuint>(gl_max_vertex_attribs,8);

  Info("Regal  v attribs : ",gl_max_vertex_attribs);
}

${GET_EXT}

REGAL_NAMESPACE_END
''')

def emuInfoDeclareCode(apis, args):

  code = ''
  for api in apis:
    name = api.name.lower()

    if name == 'gl':

      code += '\n'
      for ext in api.extensions:
        if len(ext.emulatedBy):
          code += '  GLboolean %s : 1;\n' % (ext.name.lower())

      code += '\n'
      states = sorted([ state.getValue.lower() for state in api.states ])
      for state in states:
        code += '  GLuint gl_%s;\n' % (state)

      code += '\n'
      code += '  GLuint gl_max_varying_floats;\n'

  return code

def emuInfoInitCode(apis, args):

  code = ''
  for api in apis:
    name = api.name.lower()

    if name == 'gl':

      for ext in api.extensions:
        if len(ext.emulatedBy):
          code += '  %s(false),\n' % (ext.name.lower())

      code += '\n'
      states = sorted([ state.getValue.lower() for state in api.states ])
      for state in states:
        code += '  gl_%s(0),\n' % (state)

      code += '  gl_max_varying_floats(0)\n'

  return code

def emuInfoGetCode(apis, args):

  code = ''
  for api in apis:
    name = api.name.lower()

    if name == 'gl':

      for ext in api.extensions:
        if len(ext.emulatedBy):
          code += '  %-37s = false;\n' % (ext.name.lower())

      code += '\n'
      code += '  gl_max_vertex_attribs = 8;\n'

      states = sorted([ state.getValue.lower() for state in api.states ])
      for state in states:
        code += '  gl_%-34s = contextInfo.gl_%s;\n' % (state.lower(), state.lower())

      code += '\n'

  return code

def emuInfoGetExtensionCode(apis, args):

  code = '''
bool
EmuInfo::getExtension(const ContextInfo &contextInfo, const char *ext) const
{
  Internal("EmuInfo::getExtension ",boost::print::quote(ext,\'"\'));

  // If the context supports it, we're done.

  if (contextInfo.getExtension(ext))
    return true;

'''

  for api in apis:

    emulatedExtensions = [extension.name for extension in api.extensions if len(extension.emulatedBy)]

    name = api.name.lower()
    if name in cond:
      code += '#if %s\n'%cond[name]
    for c in sorted(api.categories):
      if c.startswith('GL_REGAL_') or c=='GL_EXT_debug_marker':
        code += '  if %-50s return true;\n' % ('(!strcmp(ext,"%s"))'%c)
      elif c in emulatedExtensions:
        code += '  if %-50s return %s;\n' % ('(!strcmp(ext,"%s"))'%c,c.lower())
    if name in cond:
      code += '#endif\n'
    code += '\n'

  code += '''
  return false;
}
'''

  code += '''
bool
EmuInfo::isSupported(const ContextInfo &contextInfo, const char *ext) const
{
  Internal("EmuInfo::isSupported ",boost::print::quote(ext,\'"\'));

  string_list<string> e;
  e.split(ext,\' \');
  for (string_list<string>::const_iterator i=e.begin(); i!=e.end(); ++i)
    if (i->length() && !getExtension(contextInfo, i->c_str())) return false;
  return true;
}'''

  return code

def generateEmuInfoHeader(apis, args):

    substitute = {}
    substitute['LICENSE']         = args.license
    substitute['AUTOGENERATED']   = args.generated
    substitute['COPYRIGHT']       = args.copyright
    substitute['HEADER_NAME']     = "REGAL_EMU_INFO"
    substitute['IMPL_DECLARE']    = emuInfoDeclareCode(apis,args)
    outputCode( '%s/RegalEmuInfo.h' % args.srcdir, emuInfoHeaderTemplate.substitute(substitute))

def generateEmuInfoSource(apis, args):

    substitute = {}
    substitute['LICENSE']        = args.license
    substitute['AUTOGENERATED']  = args.generated
    substitute['COPYRIGHT']      = args.copyright
    substitute['IMPL_INIT']      = emuInfoInitCode(apis,args)
    substitute['IMPL_GET']       = emuInfoGetCode(apis,args)
    substitute['GET_EXT']        = emuInfoGetExtensionCode(apis,args)
    outputCode( '%s/RegalEmuInfo.cpp' % args.srcdir, emuInfoSourceTemplate.substitute(substitute))
