#!/usr/bin/python

class Api:

  def __init__(self):

    self.name       = ''
    self.functions  = []
    self.typedefs   = []
    self.enums      = []
    self.variables  = []
    self.extensions = []
    self.states     = []
    self.stateTypes = []
    self.conditional = ''

  def add(self, item):

    if isinstance(item, Function):
      self.functions.append(item)
    elif isinstance(item, Typedef):
      self.typedefs.append(item)
    elif isinstance(item, Enum):
      self.enums.append(item)
    elif isinstance(item, Parameter):
      self.variables.append(item)
    elif isinstance(item, Extension):
      self.extensions.append(item)
    elif isinstance(item, StateType):
      self.stateTypes.append(item)
    elif isinstance(item, State):
      self.states.append(item)
    else:
      raise TypeError, 'Unsupported Api type'

class Extension:

  def __init__(self, name = '', url = ''):

    self.name       = name
    self.url        = url
    self.category   = ''
    self.functions  = []        # function names
    self.enumerants = []        # enumerant names
    self.emulatedBy = ''        # Regal emulation layer that adds support for this extension
    self.emulatedIf = ''        # optional condition determing if extension can be emulated

class Function:

  def __init__(self, name = ''):

    self.name       = name
    self.ret        = None
    self.parameters = []
    self.version    = ''
    self.extension  = ''
    self.deprecated = ''
    self.category   = ''
    self.public     = True
    self.trace      = True   # Include in text trace
    self.play       = True   # Playback supported

  def add(self, parameter):

    if isinstance(parameter, Parameter):
      self.parameters.append(parameter)
    else:
      raise TypeError, 'Invalid Parameter type'

class Typedef:

  def __init__(self, name = '', type = ''):

    self.name       = name
    self.type       = type
    self.function   = ''
    self.parameters = []
    self.version    = ''
    self.extension  = ''
    self.deprecated = ''
    self.category   = ''

  def add(self, parameter):

    if isinstance(parameter, Parameter):
      self.parameters.append(parameter)
    else:
      raise TypeError, 'Invalid Parameter type'

class Return:

  def __init__(self, type = '', default = '', cast = None, binary = False, size = None, maxSize = None, intercept = None, trace = True, play = True, filter = None, regalLog = None):

    self.type      = type           # Type string, e.g. int
    self.default   = default        # Default value, e.g. 0
    self.cast      = cast           # Cast for trace and APIC purposes, e.g. void * -> char *
    self.binary    = binary         # Always treat as binary for trace purposes (always base64 encode)
    self.size      = size           # Array size expression, via helper functions etc
    self.maxSize   = maxSize        # Maximum array size
    self.intercept = intercept      # Intercept the value
    self.trace     = trace          # Include in text trace
    self.play      = play           # Include in playback
    self.filter    = filter         # Filtering function
    self.regalLog  = None           # Function for Regal logging purposes

class Parameter:

  def __init__(self, name = '', type = '', default = '', cast = None, binary = False, size = None, maxSize = None, trace = True, play = True, filter = None, regalLog = None):

    self.name     = name             # Parameter name string, e.g. foo
    self.type     = type             # Type string, e.g. int
    self.default  = default          # Default value, e.g. 0
    self.cast     = cast             # Cast for trace and APIC purposes, e.g. void * -> char *
    self.binary   = binary           # Always treat as binary for trace purposes (always base64 encode)
    self.size     = size             # Array size expression, via helper functions etc
    self.maxSize  = maxSize          # Maximum array size
    self.input    = True             # Input parameter?
    self.output   = False            # Output parameter?
    self.trace    = trace            # Include in text trace
    self.play     = play             # Include in playback
    self.filter   = filter           # Filtering function
    self.regalLog = regalLog         # Function for Regal logging purposes

def Input(name = '', type = '', default = '', cast = None, binary = False, size = None, maxSize = None, lookup = None, intercept = None, trace = True, play = True, filter = None, regalLog = None):

  parameter = Parameter(name, type, default, cast, binary, size, maxSize, trace, play, filter, regalLog)
  parameter.input     = True
  parameter.output    = False
  parameter.lookup    = lookup
  parameter.intercept = intercept   # Intercept the value
  return parameter

def Output(name = '', type = '', default = '', cast = None, binary = False, size = None, maxSize = None, intercept = None, trace = True, play = True, filter = None, regalLog = None):

  parameter = Parameter(name, type, default, cast, binary, size, maxSize, trace, play, filter, regalLog)
  parameter.input     = False
  parameter.output    = True
  parameter.intercept = intercept   # Intercept the value
  return parameter

def InputOutput(name = '', type = '', default = '', cast = None, binary = False, size = None, maxSize = None, lookup = None, intercept = None, trace = True, play = True, filter = None, regalLog = None):

  parameter = Parameter(name, type, default, cast, binary, size, maxSize, trace, play, filter, regalLog)
  parameter.input     = True
  parameter.output    = True
  parameter.lookup    = None
  parameter.intercept = None        # Intercept the value
  return parameter

class Enum:

  def __init__(self, name = ''):

    self.name       = name
    self.enumerants = []
    self.version    = ''
    self.extension  = ''
    self.deprecated = ''
    self.category   = ''
    self.default    = None

  def add(self, enumerant):

    if isinstance(enumerant, Enumerant):
      self.enumerants.append(enumerant)
    else:
      raise TypeError, 'Invalid Enumerant type'

class Enumerant:

  def __init__(self, name = '', value = '', category = ''):

    self.name       = name
    self.value      = value
    self.version    = ''
    self.extension  = ''
    self.deprecated = ''
    self.category   = category
    self.public     = True

class StateType:

  def __init__(self, name = '', code = '', explanation = ''):

    self.name = name
    self.code = code
    self.explanation = explanation
    self.ctype = []
    self.size = 1
    self.params = 0

class State:

  def __init__(self, getValue, type, getCommand, initialValue, description, section, attribute):

    self.getValue = getValue
    self.type = type
    self.getCommand = getCommand
    self.initialValue = initialValue
    self.description = description
    self.section = section
    self.attribute = attribute
