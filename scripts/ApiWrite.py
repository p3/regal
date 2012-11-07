#!/usr/bin/python

def cmpCategory(a,b):
  if a[0].category.startswith('GL_VERSION') and b[0].category.startswith('GL_VERSION'): return cmp(a[0].category,b[0].category)
  if a[0].category.startswith('GL_VERSION'):                                            return -1
  if b[0].category.startswith('GL_VERSION'):                                            return 1
  return cmp(a[0].category,b[0].category)

# Write out header

def writeHeader(file,name):
  print >>file, '''import Api
from Api import Api
from Api import Function, Typedef, Enum
from Api import Return, Parameter, Input, Output, InputOutput
from Api import Enumerant
from Api import StateType, State'''

  print >>file, '''
%s = Api()
'''%(name)

# Write out typedef container

def writeTypedefs(file,name,typedefs):
  for j in typedefs:
    print >>file, '%s = Typedef(\'%s\',\'%s\')\n'%(j.name,j.name,j.type),
    if getattr(j,'category') != None and len(j.category):
      print >>file, '%s.category = \'%s\''%(j.name,j.category)
    if getattr(j,'default',None) != None:
      print >>file, '%s.default = \'%s\''%(j.name,j.default)
    if getattr(j,'regal',None) != None:
      print >>file, '%s.regal = %s'%(j.name,j.regal)
    print >>file, ''
  for j in typedefs:
    print >>file, '%s.add(%s)'%(name,j.name)
  print >>file, ''

# Write out enumerants container

def writeEnums(file,name,enums):

  for i in enums:
    if not i.name=='defines':
      print >>file, '''
%s = Enum('%s')
%s.add(%s)
'''%(i.name,i.name,name,i.name)
      for j in i.enumerants:
        print >>file, '%s = Enumerant(\'%s\','%(j.name,j.name),
        value = j.value
        try:
          value = long(value)
        except:
          pass
        if isinstance(value, str) or isinstance(value, unicode):
          print >>file, '\'%s\','%(value),
        else:
          print >>file, '0x%04x,'%(value),
        print >>file, ')'
      for j in i.enumerants:
        print >>file, '%s.add(%s)'%(i.name,j.name)

  enumerants = {}

  for i in enums:
    if i.name=='defines':
      for j in i.enumerants:
        if not j.category in enumerants:
          enumerants[j.category] = []
        enumerants[j.category].append(j)

  enumerants = sorted( [ sorted( enumerants[i], key = lambda k : k.name) for i in enumerants.keys() ], cmp=cmpCategory )

  print >>file, '''
defines = Enum('defines')
%s.add(defines)
'''%(name)

  for i in enumerants:
    if len(i[0].category):
      print >>file, '# %s'%i[0].category
    else:
      print >>file, '#'
    print >>file, ''
    for j in i:
      print >>file, '%s = Enumerant(\'%s\','%(j.name,j.name),
      value = j.value
      try:
        value = long(value)
      except:
        pass
      if isinstance(value, str) or isinstance(value, unicode):
        print >>file, '\'%s\','%(value),
      else:
        print >>file, '0x%04x,'%(value),
      print >>file, '\'%s\')'%(j.category)
      if getattr(j,'esVersions',None) != None:
        print >>file, '%s.esVersions = %s'%(j.name,j.esVersions)
      if getattr(j,'enableCap',None) != None:
        print >>file, '%s.enableCap = %s'%(j.name,j.enableCap)
      if getattr(j,'hint',None) != None:
        print >>file, '%s.hint = %s'%(j.name,j.hint)
      if getattr(j,'gluErrorString',None) != None:
        print >>file, '%s.gluErrorString = \'%s\''%(j.name,j.gluErrorString)
    print >>file, ''
    for j in i:
      print >>file, 'defines.add(%s)'%(j.name)
    print >>file, ''

# Write out functions container

def writeFunctions(file,name,functions):

  f = {}

  for i in functions:
    if not i.category in f:
      f[i.category] = []
    f[i.category].append(i)

  f = sorted( [ sorted( f[i], key = lambda k : k.name) for i in f.keys() ], cmp=cmpCategory )

  for i in f:
    if len(i[0].category):
      print >>file, '# %s'%i[0].category
    else:
      print >>file, '#'
    print >>file, ''
    for j in i:
      print >>file, '%s = Function(\'%s\')'%(j.name,j.name)
      print >>file, '%s.ret = Return(\'%s\')'%(j.name,j.ret.type)
      for k in j.parameters:
        print >>file, '%s.add('%(j.name),
        if k.input:
          print >>file, 'Input(',
        else:
          print >>file, 'Output(',
        print >>file, '\'%s\',\'%s\''%(k.name,k.type),
        if getattr(k,'size') != None:
          if isinstance(k.size, str) or isinstance(k.size, unicode):
            print >>file, ',size = \'%s\''%(k.size),
          else:
            print >>file, ',size = %s'%(k.size),
        if getattr(k,'cast',None) != None:
          print >>file, ',cast = \'%s\''%(k.cast),
        if getattr(k,'regalLog',None) != None:
          print >>file, ',regalLog = \'%s\''%(k.regalLog),

        print >>file, '))'
      print >>file, '%s.version = \'%s\''%(j.name,j.version)
      print >>file, '%s.category = \'%s\''%(j.name,j.category)
#      if getattr(j,'gles',None) != None:
#        print >>file, '%s.gles = \'%s\''%(j.name,j.gles)
      if getattr(j,'esVersions',None) != None:
        print >>file, '%s.esVersions = %s'%(j.name,j.esVersions)
      if getattr(j,'regal',None) != None:
        print >>file, '%s.regal = %s'%(j.name,j.regal)
      if getattr(j,'regalOnly',None) != None:
        print >>file, '%s.regalOnly = %s'%(j.name,j.regalOnly)
      if getattr(j,'regalRemap',None) != None:
        if isinstance(j.regalRemap, str) or isinstance(j.regalRemap, unicode):
          print >>file, '%s.regalRemap = \'%s\''%(j.name,j.regalRemap)
        else:
          print >>file, '%s.regalRemap = %s'%(j.name,j.regalRemap)
      if getattr(j,'trace',None) != None:
        print >>file, '%s.trace = %s'%(j.name,j.trace)
      if getattr(j,'play',None) != None:
        print >>file, '%s.play = %s'%(j.name,j.play)
      print >>file, '%s.add(%s)'%(name,j.name)
      print >>file, ''

# Write out state values

def writeStates(file, name, stateTypes, states):

  if len(stateTypes):
    print >>file, '# state types'
    print >>file, ''

    for s in stateTypes:
      if len(s.name):
        print >>file, '%s = StateType(\'%s\', \'%s\', \'%s\')'%(s.name,s.name,s.code,s.explanation)

    print >>file, ''

    for s in stateTypes:
      if len(s.name):
        print >>file, 'gl.add(%s)'%(s.name)

  if len(states):
    print >>file, ''
    print >>file, '# states'
    print >>file, ''

    for s in states:
      if len(s.getValue):
        print >>file, '%s = State(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')'%( s.getValue, s.getValue, s.type, s.getCommand, s.initialValue, s.description, s.section, s.attribute)

    print >>file, ''

    for s in states:
      if len(s.getValue):
        print >>file, 'gl.add(%s)'%(s.getValue)
