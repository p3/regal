#!/usr/bin/python -B

# This exporter is undocumented - REVISIT
#
#
#

# CodeGen Imports

from string import Template, upper, replace
from optparse import OptionParser
from copy import deepcopy
import os
import re

import sys
scripts = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, scripts+'/api')
sys.path.insert(0, scripts+'/regal')

from ApiUtil import validVersion
from ApiUtil import outputCode
from ApiUtil import importAttr
from ApiUtil import hexValue
from ApiUtil import toLong
from ApiUtil import typeIsVoid

from ApiCodeGen import *

from Regal                   import *
from RegalEnum               import *
from RegalSystem             import *
from RegalContext            import *
from RegalContextInfo        import *
from RegalStatistics         import *
from RegalLookup             import *
from RegalPlugin             import *
from RegalToken              import *
from RegalDispatch           import *
from RegalDispatchCode       import *
from RegalDispatchDebug      import *
from RegalDispatchError      import *
from RegalDispatchEmu        import *
from RegalDispatchGMock      import *
from RegalDispatchGLX        import *
from RegalDispatchLog        import *
from RegalDispatchLoader     import *
from RegalDispatchMissing    import *
from RegalDispatchPpapi      import *
from RegalDispatchStatistics import *
from RegalDispatchStaticEGL  import *
from RegalDispatchStaticES2  import *
from RegalDispatchTrace      import *

regalLicense = '''
/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012-2013 Google Inc.
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
'''

emulatedExts = {
  'GL_ARB_draw_buffers':              { 'emulatedBy' : 'filt',   'emulatedIf' : '(info->gl_version_major >= 2) || info->gl_nv_draw_buffers'},
  'GL_ARB_multitexture':              { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_ARB_texture_cube_map':          { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_ARB_texture_env_combine':       { 'emulatedBy' : 'iff',    'emulatedIf' : '' },
  'GL_ARB_texture_env_dot3':          { 'emulatedBy' : 'iff',    'emulatedIf' : '' },
  'GL_ARB_texture_storage':           { 'emulatedBy' : 'texsto', 'emulatedIf' : '' },
  'GL_ATI_draw_buffers':              { 'emulatedBy' : 'filt',   'emulatedIf' : '(info->gl_version_major >= 2) || info->gl_nv_draw_buffers'},
  'GL_EXT_blend_color':               { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_EXT_blend_subtract':            { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_EXT_direct_state_access':       { 'emulatedBy' : 'dsa',    'emulatedIf' : '' },
  'GL_EXT_framebuffer_blit':          { 'emulatedBy' : 'filt',   'emulatedIf' : '(info->gl_version_major >= 3) || info->gl_nv_framebuffer_blit' },
  'GL_EXT_framebuffer_object':        { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_EXT_texture_cube_map':          { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_EXT_texture_edge_clamp':        { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_EXT_texture_env_combine':       { 'emulatedBy' : 'iff',    'emulatedIf' : '' },
  'GL_EXT_texture_env_dot3':          { 'emulatedBy' : 'iff',    'emulatedIf' : '' },
  'GL_IBM_texture_mirrored_repeat':   { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_NV_blend_square':               { 'emulatedBy' : 'filt',   'emulatedIf' : '' },
  'GL_NV_path_rendering':             { 'emulatedBy' : 'path',   'emulatedIf' : '' },
}


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

        for i in api.enums:
          if i.name=='defines':
            i.enumerantsByName = sorted(i.enumerants,key=lambda k : k.name)

        for e in api.extensions:
          if e.name in emulatedExts:
            e.emulatedBy = emulatedExts[e.name]['emulatedBy']
            e.emulatedIf = emulatedExts[e.name]['emulatedIf']

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

				# Build a dict of default typedef values

        api.defaults = {}
        for i in apis:
					for typedef in i.typedefs:
						if getattr(typedef,'default',None)!=None:
							api.defaults[typedef.name] = typedef.default

        api.defaults['int']   = '0';
        api.defaults['HDC']   = 'NULL';
        api.defaults['HGLRC'] = 'NULL';

    traverseContextInfo(apis,args)

def generate(apis, args):

  traverse(apis, args)
  generateContextHeader(apis, args)
  generateContextSource(apis, args)
  generateDispatchGLX(apis, args)
  generateTraceSource( apis, args )
  generatePublicHeader(apis, args)
  generatePluginSource(apis,args)
  generateDispatchStatistics( apis, args )
  generateStatisticsHeader(apis, args)
  generateStatisticsSource(apis, args)
  generateSource(apis, args)
  generateSystemHeader(apis, args)
  generateEmuSource( apis, args )
  generateDispatchLog( apis, args )
  generateDispatchCode( apis, args )
  generateErrorSource( apis, args )
  generateDebugSource( apis, args )
  generateLoaderSource( apis, args )
  generateMissingSource( apis, args )
  generatePpapiSource( apis, args )
  generateStaticES2Source( apis, args )
  generateStaticEGLSource( apis, args )
  generateDispatchHeader(apis, args)
  generateContextInfoHeader(apis, args)
  generateContextInfoSource(apis, args)
  generateLookupSource(apis, args)
  generateLookupHeader(apis, args)
  generateTokenSource(apis, args)
  generateTokenHeader(apis, args)
  generateEnumHeader(apis, args)

  generateGMockHeader(apis, args)
  generateGmockSource(apis, args)

  additional_exports = ['RegalSetErrorCallback', 'RegalShareContext', 'RegalMakeCurrent', 'RegalDestroyContext']

  generateDefFile( apis, args, additional_exports)

##############################################################################################


def main():

    parser = OptionParser()
    parser.add_option('-a', '--api',       dest = 'apis',      metavar = 'API VERSION', action = 'append', nargs = 2, help = 'generate loader for API and VERSION')
    parser.add_option('-c', '--copyright', dest = 'copyright',                          action = 'store_true',        help = 'include copyright notice')
    parser.add_option(      '--outdir',    dest = 'outdir',    metavar = 'DIR',                                       help = 'output directory')
    parser.set_defaults(apis = [])
    (options, args) = parser.parse_args()

    if not len(options.apis):
      parser.error('Specify an api.\n  See Export.py --help')

    if not options.outdir:
      parser.error('Specify output directory.\n  See Export.py --help')

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

    genArgs.license   = regalLicense
    genArgs.generated = autoGeneratedMessage
    genArgs.srcdir    = options.outdir + '/src/regal'
    genArgs.testdir   = options.outdir + '/tests'
    genArgs.incdir    = options.outdir + '/include/GL'

    for path in (genArgs.incdir, genArgs.srcdir):
      if not os.path.exists(path):
        os.makedirs(path)
        print 'Directory created: %s' % path

    generate(apis, genArgs)

if __name__ == '__main__':
  main()
