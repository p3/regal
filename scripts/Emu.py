#!/usr/bin/python -B

#
#  When tracing the driver side, all loader functions must have their proc addresses finalized
#  so we can hijack the loadedTbl.
#
#  void driver_entry( arg0, arg1, ... ) {
#     if( layer_1 && layer_1_inactive ) {
#        layer_1_activate
#        layer_1_prefix
#        layer_1_deactivate
#     }
#     if( layer_0 && layer_0_inactive ) {
#        layer_0_activate
#        layer_0_prefix
#        layer_0_deactivate
#     }
#
#     if( layer_1 && layer_1_inactive ) {
#        layer_1_activate
#        layer_1_impl
#        layer_1_deactivate
#     } else if( layer_0 && layer_0_inactive ) {
#        layer_0_activate
#        layer_0_impl
#        layer_0_deactivate
#     } else {
#        loaded.entry(...)
#     }
#  }

from ApiCodeGen import typeCode
from ApiUtil import typeIsVoid
import re
from string import Template
from string import join
from copy import deepcopy

#
# Apply per-section substitutions
#
# Inputs:
#
#   entry    - the "emue" dictionary
#   formula  - formula dictionary
#   section  - section name
#   subs     - substitutions for string.Template.substitute
#

def substitute(entry, formula, section, subs):

  if not section in formula:
    return

  entry[section] = []

  # Turn a string into a list, if necessary

  tmp = formula[section]
  if isinstance(tmp,str) or isinstance(tmp,unicode):
    tmp = [ tmp ]

  for i in tmp:
    j = Template(i)
    entry[section].append(j.substitute(subs))

#
# Add a substitution for string.Template.substitute purposes
#
# Inputs:
#
#   name    - entry point name
#   formula - formula dictionary
#   subs    - string.Template.substitute substitutions
#
# Outputs:
#
#   updated subs

def addSubstitution(name, formula, subs):

  if not 'subst' in formula:
    return

  s = deepcopy( subs )
  for newdef in formula['subst'] :
    dd = formula['subst'][newdef]

  r = None
  for k in dd :
    m = re.match( '^%s$' % k, name )
    if m :
      r = dd[k]
      #print 'matched! - result is %s' % r
      break

  if not r :
    r = dd['default']
  r = Template( r ).substitute( s )
  subs[newdef]= r

#
# Inputs:
#
#   func        - Api function to match
#   emuFormulae - Emulation formulae (list?)
#   member      - Name of the RegalContext member to check for not-NULL
#
# Output:
#
#   A dictionary of stuff, the "emue"
#   { 'name' : name, 'member' : member, 'impl' : { ... }, ... }

def emuFindEntry(func, emuFormulae, member):

  if emuFormulae==None:
    return None

  name = func.name

  # list of function parameter names

  arglist = [ i.name.strip() for i in func.parameters ]

  # arg is a mapping from arg0 to function parameter name...

  arg = {}
  for i in range(len(arglist)):
    arg['arg%d' % i] = arglist[i]

  # ... and mappings from arg0plus to lists of function parameters

  for i in range(0,5):
    label = 'arg%dplus' % i;
    if len(arglist) > 0 :
      arg[label] = ', '.join(arglist)
      arglist.pop(0)
    else :
      arg[label] = ''

  # Iterator over the formulae
  #
  # k is the key
  # i is the formula

  for k,i in emuFormulae.iteritems():

    # Cache the compiled regular expressions, as needed

    if 'entries_re' not in i:
      i['entries_re'] = [ re.compile( '^%s$' % j ) for j in i['entries'] ]

  # A list of matches containing (match object, formula name, formula)
  # Look for matches, ideally only one

  m = [ [j.match(name),k,i] for k,i in emuFormulae.iteritems() for j in i['entries_re'] ]
  m = [ j for j in m if j[0] ]

  assert len(m)<=1, 'Ambiguous match (%s) for %s - giving up.'%(', '.join([j[1] for j in m]),name)

  if len(m):
    match   = m[0][0]
    formula = m[0][2]
    rType  = typeCode(func.ret.type)
    dummyRetVal = ''
    if not typeIsVoid(rType):
      dummyRetVal = '(( %s )0)' % rType
    emue = { 'name' : name, 'member' : member, 'dummyretval' : dummyRetVal }
    subs = deepcopy(arg)
    for l in range( len(match.groups()) + 1):
      subs['m%d' % l] = match.group( l )
    subs['name'] = name
    subs['dummyretval'] = dummyRetVal
    addSubstitution( name, formula, subs )
    substitute( emue, formula, 'impl',   subs )
    substitute( emue, formula, 'init',   subs )
    substitute( emue, formula, 'cond',   subs )
    substitute( emue, formula, 'prefix', subs )
    substitute( emue, formula, 'suffix', subs )
    substitute( emue, formula, 'pre',    subs )
    substitute( emue, formula, 'post',   subs )

    emue['cond'] = None
    if 'cond' in formula:
      emue['cond'] = formula['cond']

    # plugin is optional, default to False

    emue['plugin'] = False
    if 'plugin' in formula:
      emue['plugin'] = formula['plugin']

    return emue

  return None

#
# Generate code for prefix, init, cond, impl or suffix
#

def emuCodeGen(emue,section):

  tmp = []
  for i in emue:
    if i!=None and i.get(section)!=None:
      if i.get('member')!=None:
        tmp.append('if (_context->%s)\n' % i['member'])
        tmp.append('{\n')
        tmp.extend(['  %s' % j for j in i[section] ])
        tmp.append('}\n')
      else:
        tmp.extend(['%s' % j for j in i[section] ])
  return tmp
