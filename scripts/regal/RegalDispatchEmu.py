#!/usr/bin/python -B

from string import Template, upper, replace

from ApiUtil import outputCode
from ApiUtil import typeIsVoid

from ApiCodeGen import *

from RegalContext     import emu
from RegalContextInfo import cond
from RegalDispatchShared import dispatchSourceTemplate

from Emu       import emuFindEntry, emuCodeGen

##############################################################################################


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
            if getattr(function,'regalOnly',False)==True:
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

            if all(i is None for i in emue) and (getattr(function,'regalRemap',None)==None or isinstance(function.regalRemap, str) or isinstance(function.regalRemap, unicode)):
                continue

            code += '\nstatic %sREGAL_CALL %s%s(%s) \n{\n' % (rType, 'emu_', name, params)
            code += '  RegalContext *_context = REGAL_GET_CONTEXT();\n'
            code += '  RegalAssert(_context);\n'
            code += '  DispatchTableGL &_dispatch = _context->dispatcher.emulation;\n'
            code += '\n'

            level = [ (emu[i], emuFindEntry( function, emu[i]['formulae'], emu[i]['member'] )) for i in range( len( emue ) - 1 ) ]

            # PREFIX

            if not all(i[1]==None or not 'prefix' in i[1] and not 'impl' in i[1] for i in level):
              code += '  // prefix\n'
              code += '  switch( _context->emuLevel )\n'
              code += '  {\n'
              for i in level:
                  l,e = i[0], i[1]
                  code += '    case %d :\n' % l['level']
                  if l['ifdef']:
                      code += '      #if %s\n' % l['ifdef']
                  if e != None and 'prefix' in e and len(e['prefix']):

                      if l['member']:
                        code += '      if (_context->%s)\n' % l['member']
                        code += '      {\n'
                        code += '        Push<int> pushLevel(_context->emuLevel);\n'
                        code += '        _context->emuLevel = %d;\n' %( int(l['level']) - 1 )

                      if l['plugin']:
                        code += '        #if REGAL_PLUGIN\n'
                        code += '        Thread::ThreadLocal &_instance = Thread::ThreadLocal::instance();\n'
                        code += '        Push<DispatchTableGL *> pushDispatchTable(_instance.nextDispatchTable);\n'
                        code += '        _instance.nextDispatchTable = &_context->dispatcher.emulation;\n'
                        code += '        #endif\n'

                      code += listToString(indent(e['prefix'],'        '))
                      if l['member'] :
                          code += '      }\n'
                  if e!= None and 'impl' in e and l['member']:
                      code += '      if (_context->%s) break;\n' % l['member'];
                  if l['ifdef']:
                      code += '      #endif\n'
              code += '    default:\n'
              code += '      break;\n'
              code += '  }\n\n'

            # Remap, as necessary
            remap = getattr(function, 'regalRemap', None)
            es2Name = None
            if remap!=None and isinstance(remap, dict):
              es2Name = remap.get('ES2.0',None)
              es2Params = callParams
              if es2Name != None:
                j = es2Name.find('(')
                if j!=-1:
                  es2Params = es2Name[j+1:-1]
                  es2Name   = es2Name[0:j]

            # IMPL

            if not all(i[1]==None or not 'impl' in i[1] for i in level):
              code += '  // impl\n'
              code += '  switch( _context->emuLevel )\n'
              code += '  {\n'
              for i in level:
                  l,e = i[0], i[1]
                  code += '    case %d :\n' % l['level']
                  if l['ifdef']:
                      code += '      #if %s\n' % l['ifdef']
                  if e != None and 'impl' in e and len(e['impl']):

                      if l['member'] :
                        code += '      if (_context->%s)\n' % l['member']
                        code += '      {\n'
                        code += '        Push<int> pushLevel(_context->emuLevel);\n'
                        code += '        _context->emuLevel = %d;\n' %( int(l['level']) - 1 )

                      if l['plugin']:
                        code += '        #if REGAL_PLUGIN\n'
                        code += '        Thread::ThreadLocal &_instance = Thread::ThreadLocal::instance();\n'
                        code += '        Push<DispatchTableGL *> pushDispatchTable(_instance.nextDispatchTable);\n'
                        code += '        _instance.nextDispatchTable = &_context->dispatcher.emulation;\n'
                        code += '        #endif\n'

                      code += listToString(indent(e['impl'],'        '))
                      if l['member'] :
                          if l['member'] != "filt" and typeIsVoid(rType):
                              code += '        return;\n'
                          code += '      }\n'
                  if l['ifdef']:
                      code += '      #endif\n'
              code += '    default: \n'
              code += '    {\n'

              # glEnable/glDisable/glIsEnabled constraints for ES 2.0
              # http://www.khronos.org/opengles/sdk/docs/man/xhtml/glEnable.xml

              if name=='glEnable' or name=='glDisable' or name=='glIsEnabled':
                code += '       if (_context->isES2())\n'
                code += '         switch (cap)\n'
                code += '         {\n'
                for i in api.enums:
                  if i.name=='defines':
                    for j in i.enumerantsByName:
                      if getattr(j,'esVersions',None) != None and getattr(j,'enableCap',None) != None and 2.0 in j.esVersions and j.enableCap == True:
                        code += '           case %s:\n'%(j.name)
                code += '             break;\n'
                code += '           default:\n'
                code += '             Warning("%s does not support ",GLenumToString(cap)," for ES 2.0.");\n'%(name)
                if name=='glIsEnabled':
                  code += '             return GL_FALSE;\n'
                else:
                  code += '             return;\n'
                code += '         }\n'

              # glBindTexture constraints for ES 2.0
              # http://www.khronos.org/opengles/sdk/docs/man/xhtml/glBindTexture.xml

              if name=='glBindTexture':
                code += '       if (_context->isES2())\n'
                code += '         switch (target)\n'
                code += '         {\n'
                for i in api.enums:
                  if i.name=='defines':
                    for j in i.enumerantsByName:
                      if getattr(j,'esVersions',None)==None:
                        continue
                      if getattr(j,'bindTexture',None)==None:
                        continue
                      if 2.0 in j.esVersions and j.bindTexture == True:
                        code += '           case %s:\n'%(j.name)
                code += '             break;\n'
                code += '           default:\n'
                code += '             Warning("%s does not support ",GLenumToString(target)," for ES 2.0.");\n'%(name)
                code += '             return;\n'
                code += '         }\n'

              # glTexSubImage2D constraints for ES 2.0
              # http://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexSubImage2D.xml

              if name=='glTexSubImage2D':
                code += '       if (_context->isES2())\n'
                code += '         switch (target)\n'
                code += '         {\n'
                for i in api.enums:
                  if i.name=='defines':
                    for j in i.enumerantsByName:
                      if getattr(j,'esVersions',None)==None:
                        continue
                      if getattr(j,'texImage',None)==None:
                        continue
                      if 2.0 in j.esVersions and j.texImage:
                        code += '           case %s:\n'%(j.name)
                code += '             break;\n'
                code += '           default:\n'
                code += '             Warning("%s does not support ",GLenumToString(target)," for ES 2.0.");\n'%(name)
                code += '             return;\n'
                code += '         }\n'

              code += '      DispatchTableGL *_next = _dispatch.next();\n'
              code += '      RegalAssert(_next);\n'

              if es2Name != None:
                code += '      '
                code += 'if (_context->isES2())\n'
                code += '        '
                if not typeIsVoid(rType):
                    code += 'return '
                code += '_next->call(&_next->%s)(%s);\n' % ( es2Name, es2Params )
                code += '      else\n  '

              code += '      '
              if not typeIsVoid(rType):
                  code += 'return '
              code += '_next->call(&_next->%s)(%s);\n' % ( name, callParams )

              code += '      break;\n'
              code += '    }\n\n'
              code += '  }\n\n'
            else:

              # glTexImage2D internalformat constraints for ES 2.0
              # http://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml

              if name=='glTexImage2D':
                code += '  if (_context->isES2())\n'
                code += '  {\n'
                code += '    switch (internalformat)\n'
                code += '    {\n'
                for i in api.enums:
                  if i.name=='defines':
                    for j in i.enumerantsByName:
                      if getattr(j,'esVersions',None)==None:
                        continue
                      if getattr(j,'internalformat',None)==None:
                        continue
                      if 2.0 in j.esVersions and j.internalformat == True:
                        code += '      case %s:\n'%(j.name)
                code += '        break;\n'
                code += '      default:\n'
                code += '        Warning("%s does not support ",GLenumToString(internalformat)," for ES 2.0.");\n'%(name)
                code += '        return;\n'
                code += '    }\n'
                code += '    if (format!=GLenum(internalformat))\n'
                code += '    {\n'
                code += '        Warning("%s does not support mismatching format and internalformat ",GLenumToString(format),"!=",GLenumToString(internalformat)," for ES 2.0.");\n'%(name)
                code += '        return;\n'
                code += '    }\n'
                code += '  }\n'

              code += '  DispatchTableGL *_next = _dispatch.next();\n'
              code += '  RegalAssert(_next);\n'
              code += '  '

              if es2Name != None:
                code += 'if (_context->isES2())\n'
                code += '    '
                if not typeIsVoid(rType):
                    code += 'return '
                code += '_next->call(& _next->%s)(%s);\n' % ( es2Name, es2Params )
                code += '   else\n     '

              if not typeIsVoid(rType):
                  code += 'return '
              code += '_next->call(& _next->%s)(%s);\n' % ( name, callParams )
            code += '}\n\n'

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
      if getattr(function,'regalOnly',False)==True:
        continue

      name   = function.name

      emue = [ None ]
      for i in range( len( emu ) - 1 ) :
        emue.append( emuFindEntry( function, emu[i]['formulae'], emu[i]['member'] ) )

      if all(i is None for i in emue) and (getattr(function,'regalRemap',None)==None or isinstance(function.regalRemap, str) or isinstance(function.regalRemap, unicode)):
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

emuLocalCode = '''


'''



def generateEmuSource(apis, args):

  funcDefine = apiEmuFuncDefineCode( apis, args )
  funcInit   = apiEmuDispatchFuncInitCode( apis, args )

  emuLocalInclude = '''

#include "RegalBreak.h"
#include "RegalBin.h"
#include "RegalTexSto.h"
#include "RegalXfer.h"
#include "RegalEmu.h"
#include "RegalPpa.h"
#include "RegalPpca.h"
#include "RegalRect.h"
#include "RegalHint.h"
#include "RegalIff.h"
#include "RegalQuads.h"
#include "RegalMarker.h"
#include "RegalObj.h"
#include "RegalDsa.h"
#include "RegalSo.h"
#include "RegalTexC.h"
#include "RegalVao.h"
#include "RegalBaseVertex.h"
#include "RegalFilt.h"'''

  # Output

  substitute = {}
  substitute['LICENSE']         = args.license
  substitute['AUTOGENERATED']   = args.generated
  substitute['COPYRIGHT']       = args.copyright
  substitute['DISPATCH_NAME']   = 'Emu'
  substitute['LOCAL_CODE']      = emuLocalCode
  substitute['LOCAL_INCLUDE']   = emuLocalInclude
  substitute['API_DISPATCH_FUNC_DEFINE'] = funcDefine
  substitute['API_DISPATCH_FUNC_INIT'] = funcInit
  substitute['API_DISPATCH_GLOBAL_FUNC_INIT'] = ''
  substitute['IFDEF'] = '#if REGAL_EMULATION\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchEmu.cpp' % args.srcdir, dispatchSourceTemplate.substitute(substitute))
