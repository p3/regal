#!/usr/bin/python -B

from string import Template, upper, replace

from ApiUtil import outputCode
from ApiUtil import typeIsVoid
from ApiUtil import typeIsVoidPointer

from ApiCodeGen import *

from RegalContextInfo import cond

from RegalDispatchShared import apiDispatchFuncInitCode
from RegalDispatchShared import apiDispatchGlobalFuncInitCode

missingSourceTemplate = Template('''${AUTOGENERATED}
${LICENSE}

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

${IFDEF}REGAL_GLOBAL_BEGIN

#include <string>
using namespace std;

#include "RegalLog.h"
#include "RegalBreak.h"
#include "RegalPush.h"
#include "RegalToken.h"
#include "RegalHelper.h"
#include "RegalPrivate.h"
#include "RegalContext.h"
${LOCAL_INCLUDE}

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

namespace Missing
{

${LOCAL_CODE}

${API_DISPATCH_FUNC_DEFINE}

  void Init(DispatchTableGL &tbl)
  {
${API_DISPATCH_FUNC_INIT}
  }

${API_DISPATCH_GLOBAL_FUNC_INIT}

} // namespace Missing

REGAL_NAMESPACE_END

${ENDIF}''')

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

      if getattr(function,'regalOnly',False)==True:
        continue

      name   = function.name
      params = paramsDefaultCode(function.parameters, True)
      callParams = paramsNameCode(function.parameters)
      rType     = typeCode(function.ret.type)
      rTypes    = rType.strip()
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

      code += '  static %sREGAL_CALL %s(%s) \n{\n' % (rType, name, params)
      for param in function.parameters:
        code += '    UNUSED_PARAMETER(%s);\n' % param.name
      code += '    Warning( "%s", " not available." );\n' % name

      if not typeIsVoid(rType):
        if rTypes in api.defaults:
          code += '    return %s;\n' % ( api.defaults[rTypes] )
        else:
          if rType[-1]=='*' or typeIsVoidPointer(rType):
            code += '    return NULL;\n'
          else:
            code += '    return (%s) 0;\n' % ( rTypes )

      code += '  }\n\n'

    if api.name in cond:
      code += '#endif // %s\n' % cond[api.name]
    code += '\n'

  return code

def generateMissingSource(apis, args):

  # Output

  substitute = {}

  substitute['LICENSE']         = args.license
  substitute['AUTOGENERATED']   = args.generated
  substitute['COPYRIGHT']       = args.copyright
  substitute['DISPATCH_NAME'] = 'Missing'
  substitute['LOCAL_INCLUDE'] = ''
  substitute['LOCAL_CODE']    = ''
  substitute['API_DISPATCH_FUNC_DEFINE'] = apiMissingFuncDefineCode( apis, args )
  substitute['API_DISPATCH_FUNC_INIT']   = apiDispatchFuncInitCode( apis, args, None )
  substitute['API_DISPATCH_GLOBAL_FUNC_INIT']   = apiDispatchGlobalFuncInitCode( apis, args, None )
  substitute['IFDEF'] = '#if REGAL_MISSING\n\n'
  substitute['ENDIF'] = '#endif\n'

  outputCode( '%s/RegalDispatchMissing.cpp' % args.srcdir, missingSourceTemplate.substitute(substitute))

