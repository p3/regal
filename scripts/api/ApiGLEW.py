#!/usr/bin/python

# readGLEWextension
#
# Return a tuple representation of a GLEW 1.7.0
# extension specification:
#
# ( category, URL, namestring, ( enums ), ( functions ), (handles) , (typedefs))
#

def readGLEWextension(input):
  category   = ''
  url        = ''
  namestring = ''
  enums     = []
  functions = []
  handles   = []
  typedefs  = []
  n         = 0

  for line in input:
    n = n + 1
    line = ' '.join(line.strip().split())  # rationalize whitespace

    if n==1:
      category = line
      continue

    if n==2:
      url = line
      continue

    if n==3:
      namestring = line
      continue

    if len(line):
      if line.startswith('DECLARE_HANDLE'):
        handles.append(line)
        continue
      if line.find('typedef')!=-1:
        typedefs.append(line)
        continue
      if line.find('(')==-1:
        enums.append(line)
        continue
      functions.append(line)

  return category, url, namestring, tuple(enums), tuple(functions), tuple(handles), tuple(typedefs)

# writeGLEWextension
#
# Return list of strings representation of a GLEW 1.7.0
# extension specification, from the tuple:
#
# ( category, URL, namestring, ( enums ), ( functions ), (handles) )
#

def writeGLEWextension(input):
  code = []
  code.append(input[0])
  code.append(input[1])
  code.append(input[2])
  for i in input[3]:
    code.append( '\t%s'%i )
  for i in input[4]:
    code.append( '\t%s'%i )
  for i in input[5]:
    code.append( '\t%s'%i )
  for i in input[6]:
    code.append( '\t%s'%i )

  return code

# diffGLEWextension
#
# Return True if different

def diffGLEWextension(a,b):
  # First three lines ought to match exactly
  for i in [0,1,2]:
    if a[i]!=b[i]:
      return True
  # Order of enums, functions, typedefs is arbitrary
  aSet = set(a[3:])
  bSet = set(b[3:])
  removed = aSet.difference(bSet)
  added   = bSet.difference(aSet)
  return len(removed)>0 or len(added)>0

#
# Test mode
#

# main

if __name__ == "__main__":

  from optparse import OptionParser
  import os

  parser = OptionParser('usage: %prog [options] [SOURCES...]')
  parser.add_option('-i', '--input',  dest = 'input',   metavar = 'FILE', action = 'append',     help = 'input file(s) in GLEW extension format', default = [])
  parser.add_option('-o', '--output', dest = 'output',  metavar = 'DIR',                         help = 'optional output directory',              default = None)
  (options, args) = parser.parse_args()

  options.input.extend(args)

  # Read input files, convert to list of strings,  and strip line endings

  input = [ [ j.rstrip() for j in open(i, 'r').readlines() ] for i in options.input ]

  # convert

  output = []
  for i in input:
    ext = readGLEWextension(i)
    output.append(writeGLEWextension(ext))

  # output

  if options.output and len(options.output):
    for i in range(len(options.input)):
      f = open( os.path.join(options.output,os.path.basename(options.input[i])), 'w' )
      f.write('\n'.join(output[i]))
      f.write('\n')

  # compare

  for i in range(len(options.input)):
    print options.input[i]
    if diffGLEWextension(input[i],output[i]):
      print 'before:'
      print '\n'.join(input[i])
      print 'after:'
      print '\n'.join(output[i])
