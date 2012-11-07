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


import re
from string import Template
from string import join
from copy import deepcopy

def emuSubstitute( entry, emutmpl, codeseg, subst ) :
    if not codeseg in emutmpl :
        return
    lines = []
    for line in emutmpl[codeseg] :
        tmpl = Template( line )
        #print 'line: %s' % line
        newline = tmpl.substitute( subst )
        lines.append( newline )
    entry[codeseg] = lines

def emuAddSubst( name, emutmpl, subst ) :
    if not 'subst' in emutmpl :
        return
    s = deepcopy( subst )
    for newdef in emutmpl['subst'] :
        dd = emutmpl['subst'][newdef]
        r = None
        for k in dd :
            #print 'try match %s to %s' %  ( k, name )
            m = re.match( '^%s$' % k, name )
            if m :
                r = dd[k]
                #print 'matched! - result is %s' % r
                break
        if not r :
            r = dd['default']
        r = Template( r ).substitute( s )
        subst[newdef]= r
        #print "adding %s as %s" % ( newdef, r )


def emuFindEntry( func, emuFormulae, member ) :

    if emuFormulae==None:
      return None

    name = func.name

    #print "iff name = %s\n" % name
    arglist = [ i.name.strip()           for i in func.parameters ]
    #arg     = { 'arg%d' % i : arglist[i] for i in range(len(arglist)) }
    arg     = {}
    for i in range(len(arglist)):
        arg['arg%d' % i] = arglist[i]

    for i in range( 0, 3 ) :
        label = 'arg%dplus' % i;
        if len(arglist) > 0 :
            arg[label] = ', '.join( arglist )
            arglist.pop(0)
        else :
            arg[label] = ''

    for k,i in emuFormulae.iteritems():

      # Cache the compiled regular expressions, as needed

      if 'entries_re' not in i:
        i['entries_re'] = [ re.compile( '^%s$' % j ) for j in i['entries'] ]

      for j in i['entries_re']:
        m = j.match( name )
        if m :
          emue = { 'name' : name, 'member' : member }
          subst = deepcopy( arg )
          for l in range( len(m.groups()) + 1) :
            subst['m%d' % l] = m.group( l )
          emuAddSubst( name, i, subst )
          emuSubstitute( emue, i, 'impl', subst )
          emuSubstitute( emue, i, 'prefix', subst )
          emuSubstitute( emue, i, 'suffix', subst )
          return emue

    return None

# Generate code for prefix, impl or suffix

def emuCodeGen(emue,section):

  tmp = []
  for i in emue:
    if i != None and i.get(section)!=None:
      if i.get('member')!=None:
        tmp.append('if (_context->%s)\n' % i['member'])
        tmp.append('{\n')
        tmp.extend(['  %s' % j for j in i[section] ])
        tmp.append('}\n')
      else:
        tmp.extend(['%s' % j for j in i[section] ])
  return tmp
