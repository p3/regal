#!/usr/bin/python -B

#
# Utility script for converting GLEW extension
# specifications into .py Python-format API
# database.
#
# For help:
#   glew2py.py -help
#
# Example usage:
#   private/glew2py.py -a gl -o gl.py ~/dev/glew/auto/extensions/gles/*

import sys
sys.path.insert(0,'scripts/api')

from ApiGLEW import *
from optparse import OptionParser
import re

options = None

# paramSplit - split a parameter into (name,type) tuple

paramRE1 = re.compile('((const )?[a-zA-Z][a-zA-Z0-9_]*( [a-zA-Z][a-zA-Z0-9_]+)?[ \*]+(const )?)(.+)(\[.*\])?')
paramRE2 = re.compile('((const )?[a-zA-Z][a-zA-Z0-9_]*)')

def splitParam(param):
  if param=='void':
    return None, 'void'

  m = paramRE1.match(param)
  if m:
    if m.group(6):
      return m.group(5), '%s%s'%(m.group(1),m.group(6))
    else:
      return m.group(5), m.group(1)

  m = paramRE2.match(param)
  if m:
    return '', m.group(1)
  return None, None

def tidyNameType(name, type):
  tmp = ''
  if type:
    for i in type:
      if i=='*' and tmp[-1]!=' ' and tmp[-1]!='*':
        tmp += ' *'
      else:
        tmp += i

  # Fixups for GLEW parsespec bugs

  if name and type and name.strip()=='GLsync' and type.strip()=='GLsync':
    return 'sync','GLsync'

  if type:
    tmp = tmp.strip()
    if tmp=='void *':
      return name, 'GLvoid *'
    if tmp=='void **':
      return name, 'GLvoid **'
    if tmp=='const void *':
      return name, 'const GLvoid *'
    if tmp=='const void **':
      return name, 'const GLvoid **'
    if tmp.startswith('void*'):
      return name, 'GLvoid *%s'%tmp[5:]
    if tmp=='GLchar*':
      return name, 'GLchar *'
    if tmp=='const GLchar*':
      return name, 'const GLchar *'
    return name, tmp.strip()
  else:
    return name,type

# splitFunction

functionRE = re.compile('((const )?[a-zA-Z][a-zA-Z0-9_ ]*[ \*]+(const )?)([^\(]+)(\([^\)]*\))')

def splitFunction(f):
  m = functionRE.match(f)
  if m:
    return (m.group(1).strip(),m.group(4).strip(),m.group(5).strip())
  else:
    return ()

# extensionToPyCode
#
# Return a tuple Python code representation of an
# extension specification:
#
# ( enums, enumsAdd, functions )
#

def extensionToPyCode(ext):

  category  = ext[0]
  enums     = ext[3]
  functions = ext[4]

  enums    =  [ (i.split(' ')[0].strip(), i.split(' ')[1].strip()) for i in enums     ]
  functions = [ splitFunction(i)                                   for i in functions ]

  pyEnums = []
  for i in enums:
    code = []
    value = i[1]
    if i[1].startswith('0x'):
        value = '0x%04x'%(long(value,16))
    else:
      try:
        value = '0x%04x'%(long(value))
      except:
        value = '\'%s\''%i[1]
      i = ( i[0], '\'%s\''%i[1] )
    code.append( '%s = Enumerant(\'%s\', %s, \'%s\')'%(i[0],i[0],value.lower(),category) )
    pyEnums.append( tuple(code) )

  pyEnumsAdd = []
  for i in enums:
    code = []
    code.append( 'defines.add(%s)'%(i[0]) )
    pyEnumsAdd.append( tuple(code) )

  pyFunctions = []
  for i in functions:
    ret    = i[0]
    ret    = tidyNameType('foo',i[0])[1]
    name   = i[1]
    params = i[2].replace('(','').replace(')','').split(',')
    params = [ j.strip()               for j in params ]
    params = [ splitParam(j)           for j in params ]
    params = [ tidyNameType(j[0],j[1]) for j in params ]
    code   = []
    code.append( '%s = Function(\'%s\')'%(name,name) )
    code.append( '%s.ret = Return(\'%s\')'%(name,ret) )
    for i in params:
      if i[1]!=None and not (i[0]==None and i[1]=='void'):
        code.append( '%s.add( Input( \'%s\',\'%s\' ))'%(name,i[0],i[1]) )
    code.append( '%s.category = \'%s\''%(name,category) )
    code.append( '%s.trace = True'%(name) )
    code.append( '%s.play = True'%(name) )
    code.append( '%s.add(%s)'%(options.api,name) )
    pyFunctions.append( tuple(code) )

  return tuple(pyEnums), tuple(pyEnumsAdd), tuple(pyFunctions)

#
#
#

# main

if __name__ == "__main__":

  parser = OptionParser('usage: %prog [options] [SOURCES...]')
  parser.add_option('-a', '--api',    dest = 'api',                                         help = 'API name',                               default = 'gl')
  parser.add_option('-i', '--input',  dest = 'input',  metavar = 'FILE', action = 'append', help = 'input file(s) in GLEW extension format', default = [])
  parser.add_option('-o', '--output', dest = 'output', metavar = 'FILE',                    help = 'Python output file',                     default = [])
  (options, args) = parser.parse_args()

  options.input.extend(args)

  if not options.input:
    options.input = '-'

  # Read input files, convert to list of strings,  and strip line endings

  if options.input!='-':
    files = [ [ j.rstrip() for j in open(i, 'r').readlines() ] for i in options.input ]
  else:
    files = [ j.rstrip() for j in sys.stdin.readlines() ]

  # convert to .py
  # ( category, URL, namestring, ( enums ), ( functions ), (handles) , (typedefs))

  ext = []
  for i in files:
    e = readGLEWextension(i)
    e = ( e[0], e[1], e[2], sorted(e[3]), sorted(e[4],key=lambda x: splitFunction(x)[1]), sorted(e[5]), sorted(e[6]))
    ext.append(tuple([e,(extensionToPyCode(e))]))

  ext = sorted(ext)

  # output file

  if options.output:
    file = open(options.output, 'w')
  else:
    file = sys.stdout

  print >>file, '''import Api
from Api import Api
from Api import Function, Typedef, Enum
from Api import Return, Parameter, Input, Output, InputOutput
from Api import Enumerant
from Api import Extension
from Api import StateType, State

%s = Api()
defines = Enum('defines')
%s.add(defines)

'''%(options.api,options.api)

  for i in ext:
    if len(i[1][0]):
      print >>file, '# %s'%(i[0][0])
      print >>file, ''
      for j in i[1][0]:
        print >>file, '\n'.join(j)
      print >>file, ''
      for j in i[1][1]:
        print >>file, '\n'.join(j)
      print >>file, ''

  for i in ext:
    if len(i[1][2]):
      print >>file, '# %s'%(i[0][0])
      print >>file, ''
      for j in i[1][2]:
        print >>file, '%s\n'%('\n'.join(j))
      print >>file, ''
#      for j in i[1][3]:
#        print >>file, '\n'.join(j)
#      print >>file, ''

  # Output Extensions
  for i in ext:
    print >>file, '%s = Extension(\'%s\')'%(i[0][0],i[0][0])
    if len(i[0][1]):
      print >>file, '%s.url = \'%s\''%(i[0][0],i[0][1])
    if len(i[0][3]):
      print >>file, '%s.enumerants = [\'%s\']'%(i[0][0],'\',\''.join([j.split(' ')[0] for j in i[0][3]]))
    if len(i[0][4]):
      print >>file, '%s.functions = [\'%s\']'%(i[0][0],'\',\''.join([j.split(' ')[1] for j in i[0][4]]))
    print >>file, '%s.add(%s)'%(options.api,i[0][0])
    print >>file, ''


