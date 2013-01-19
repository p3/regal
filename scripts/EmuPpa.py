#!/usr/bin/python -B

ppaFormulae = {
  'PushAtrrib' : {
    'entries'  : [ 'glPushAttrib' ],
    'impl'     : [ '_context->ppa->PushAttrib( _context, ${arg0} );', ],
  },
  'PopAttrib' : {
    'entries' : [ 'glPopAttrib' ],
    'impl'    : [ '_context->ppa->PopAttrib( _context );', ],
  },
  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Boolean)v' ],
    'impl'    : [
      'if( ! _context->ppa->Get( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}v( ${arg0plus} );',
      '}',
    ],
  },
  'Enable'    : {
    'entries' : [ 'gl(Enable|Disable)' ],
    'impl'    : [
      'if( ! _context->ppa->${m1}( _context, ${arg0} ) ) {',
      '  _context->dispatcher.emulation.gl${m1}( ${arg0} );',
      '}',
    ],
  },
  'TrackDepth' : {
    'entries'    : [ '(glClearDepth|glDepthFunc|glDepthMask)' ],
    'prefix'     : [ '_context->ppa->${m1}( ${arg0plus} );', ],
  },
  'TrackStencil' : {
    'entries'    : [ '(glClearStencil|glStencilFunc|glStencilFuncSeparate|glStencilMask|glStencilMaskSeparate|glStencilOp|glStencilOpSeparate)' ],
    'prefix'     : [ '_context->ppa->${m1}( ${arg0plus} );', ],
  },
  'TrackPolygon' : {
    'entries'    : [ '(glCullFace|glFrontFace|glPolygonMode|glPolygonOffset)' ],
    'prefix'     : [ '_context->ppa->${m1}( ${arg0plus} );', ],
  },
  'TrackDrawBuffer' : {
    'entries'    : [ 'glDrawBuffer(s|)' ],
    'impl'    : [
      'if( ! _context->info->core && !_context->info->gles ) {',
      '  _context->dispatcher.emulation.glDrawBuffer${m1}( ${arg0plus} );',
      '}',
    ],
  },
  'TrackMatrixMode' : {
    'entries'    : [ 'glMatrixMode' ],
    'prefix'     : [ '_context->ppa->glMatrixMode( ${arg0plus} );', ],
  },
  'TrackClipPlane' : {
    'entries'    : [ 'glClipPlane' ],
    'prefix'     : [ '_context->ppa->glClipPlane( ${arg0plus} );', ],
  },
}
