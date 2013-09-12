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
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)(i_|)v(EXT|)' ],
    'impl'    : [
      'if( ! _context->ppa->glGet${m2}v( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}${m2}v${m3}( ${arg0plus} );',
      '}',
    ],
  },
  'GetPolygonStipple'       : {
    'entries' : [ 'glGetPolygonStipple' ],
    'impl'    : [
      'if( ! _context->ppa->glGetPolygonStipple( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGetPolygonStipple( ${arg0plus} );',
      '}',
    ],
  },
  'glGetColorTableOrConvolutionParameterv'       : {
    'entries' : [ 'glGet(ColorTable|Convolution)Parameter(i|f)v' ],
    'impl'    : [
      'if( ! _context->ppa->glGet${m1}Parameterv( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}Parameter${m2}v( ${arg0plus} );',
      '}',
    ],
  },
  'GetLightOrMaterialv'       : {
    'entries' : [ 'glGet(Light|Material)(f|i|x)v' ],
    'impl'    : [
      'if( ! _context->ppa->glGet${m1}v( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}${m2}v( ${arg0plus} );',
      '}',
    ],
  },
  'GetTexEnviv'       : {
    'entries' : [ 'glGet(Multi|)TexEnv(f|i)v(EXT|)' ],
    'impl'    : [
      'if( ! _context->ppa->glGet${m1}TexEnvv( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}TexEnv${m2}v${m3}( ${arg0plus} );',
      '}',
    ],
  },
  'IsEnabled'       : {
    'entries' : [ 'glIsEnabled(i|)' ],
    'impl'    : [
      '{',
      '  GLboolean enabled;',
      '  if( ! _context->ppa->glIsEnabled${m1}( _context, enabled, ${arg0plus} ) )',
      '    return _context->dispatcher.emulation.glIsEnabled${m1}( ${arg0plus} );',
      '  return enabled;',
      '}',
    ],
  },
  'Enable'    : {
    'entries' : [ 'gl(Enable|Disable)(i|)' ],
    'impl'    : [
      'if( ! _context->ppa->${m1}${m2}( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.gl${m1}${m2}( ${arg0plus} );',
      '}',
    ],
  },
  'TrackDepth' : {
    'entries'    : [ '(glClearDepth|glDepthFunc|glDepthMask)(f|)' ],
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
    'entries'    : [ 'glDrawBuffer(s|)(ARB|EXT|NV|)' ],
    'impl'    : [
      'if( !_context->isES2() ) {',
      '  _context->ppa->glDrawBuffer${m1}( ${arg0plus} );',
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
  'TrackHint' : {
    'entries'    : [ 'glHint' ],
    'prefix'     : [ '_context->ppa->glHint( ${arg0plus} );', ],
  },
  'TrackClampColor': {
    'entries'    : [ 'glClampColor' ],
    'prefix'     : [ '_context->ppa->glClampColor( ${arg0plus} );',
    ],
  },
  'TrackActiveTexture' : {
    'entries'    : [ 'glActiveTexture(ARB|)' ],
    'prefix'     : [ '_context->ppa->glActiveTexture( ${arg0plus} );', ],
  },
  'TrackListBase' : {
    'entries'    : [ 'glListBase' ],
    'prefix'     : [ '_context->ppa->glListBase( ${arg0plus} );', ],
  },
  'TrackClearAccum' : {
    'entries'    : [ 'glClearAccum' ],
    'prefix'     : [ '_context->ppa->glClearAccum( ${arg0plus} );', ],
  },
  'TrackScissor' : {
    'entries'    : [ 'glScissor(Array|Indexed|)(v|)' ],
    'prefix'     : [ '_context->ppa->glScissor${m1}${m2}( ${arg0plus} );', ],
  },
  'TrackViewport' : {
    'entries'    : [ 'glViewport(Array|Indexedf|)(v|)' ],
    'prefix'     : [ '_context->ppa->glViewport${m1}${m2}( ${arg0plus} );', ],
  },
  'TrackDepthRange' : {
    'entries'    : [ 'glDepthRange(Array|Indexed|)(f|)(v|)' ],
    'prefix'     : [ '_context->ppa->glDepthRange${m1}${m3}( ${arg0plus} );', ],
  },
  'TrackLine' : {
    'entries'    : [ 'glLine(Width|Stipple|)' ],
    'prefix'     : [ '_context->ppa->glLine${m1}( ${arg0plus} );', ],
  },
  'TrackSampleCoverage' : {
    'entries'    : [ 'glSampleCoverage' ],
    'prefix'     : [ '_context->ppa->glSampleCoverage( ${arg0plus} );', ],
  },
  'TrackMinSampleShading' : {
    'entries'    : [ 'glMinSampleShading' ],
    'prefix'     : [ '_context->ppa->glMinSampleShading( ${arg0plus} );', ],
  },
  'TrackMapGrid' : {
    'entries'    : [ 'glMapGrid(1|2)(f|d)' ],
    'prefix'     : [ '_context->ppa->glMapGrid${m1}( ${arg0plus} );', ],
  },
  'TrackPointSize' : {
    'entries'    : [ 'glPointSize' ],
    'prefix'     : [ '_context->ppa->glPointSize( ${arg0plus} );', ],
  },
  'TrackPointParameter' : {
    'entries'    : [ 'glPointParameter(i|f)(v|)' ],
    'prefix'     : [ '_context->ppa->glPointParameter${m2}( ${arg0plus} );', ],
  },
  'TrackTexEnv' : {
    'entries'    : [ 'gl(Multi|)TexEnv(i|f)(v|)(EXT|)' ],
    'prefix'     : [ '_context->ppa->gl${m1}TexEnv${m3}( ${arg0plus} );', ],
  },
  'TrackPolygonStipple' : {
    'entries'    : [ 'glPolygonStipple' ],
    'prefix'     : [ '_context->ppa->glPolygonStipple( ${arg0plus} );', ],
  },
  'TrackAlphaFunc' : {
    'entries'    : [ 'glAlphaFunc' ],
    'prefix'     : [ '_context->ppa->glAlphaFunc( ${arg0plus} );', ],
  },
  'TrackBlend' : {
    'entries'    : [ 'glBlend(Func|Equation|Color)(Separate|)(i|)' ],
    'prefix'     : [ '_context->ppa->glBlend${m1}${m2}${m3}( ${arg0plus} );', ],
  },
  'TrackLogicOp' : {
    'entries'    : [ 'glLogicOp' ],
    'prefix'     : [ '_context->ppa->glLogicOp( ${arg0plus} );', ],
  },
  'TrackMask' : {
    'entries'    : [ 'gl(Index|Color)Mask(i|)' ],
    'prefix'     : [ '_context->ppa->gl${m1}Mask${m2}( ${arg0plus} );', ],
  },
  'TrackClear' : {
    'entries'    : [ 'glClear(Index|Color)' ],
    'prefix'     : [ '_context->ppa->glClear${m1}( ${arg0plus} );', ],
  },
  'TrackPixelZoom' : {
    'entries'    : [ 'glPixelZoom' ],
    'prefix'     : [ '_context->ppa->glPixelZoom( ${arg0plus} );', ],
  },
  'TrackColorTableParameterv' : {
    'entries'    : [ 'glColorTableParameter(i|f)v' ],
    'prefix'     : [ '_context->ppa->glColorTableParameterv( ${arg0plus} );', ],
  },
  'TrackConvolutionParameter' : {
    'entries'    : [ 'glConvolutionParameter(i|f)(v|)' ],
    'prefix'     : [ '_context->ppa->glConvolutionParameter${m2}( ${arg0plus} );', ],
  },
  'TrackPixelTransfer' : {
    'entries'    : [ 'glPixelTransfer(i|f)' ],
    'prefix'     : [ '_context->ppa->glPixelTransfer( ${arg0plus} );', ],
  },
  'TrackReadBuffer' : {
    'entries'    : [ 'glReadBuffer' ],
    'prefix'     : [ '_context->ppa->glReadBuffer( ${arg0plus} );', ],
  },
  'TrackShadeModel' : {
    'entries'    : [ 'glShadeModel' ],
    'prefix'     : [ '_context->ppa->glShadeModel( ${arg0plus} );', ],
  },
  'TrackProvokingVertex' : {
    'entries'    : [ 'glProvokingVertex' ],
    'prefix'     : [ '_context->ppa->glProvokingVertex( ${arg0plus} );', ],
  },
  'TrackColorMaterial' : {
    'entries'    : [ 'glColorMaterial' ],
    'prefix'     : [ '_context->ppa->glColorMaterial( ${arg0plus} );', ],
  },
  'TrackMaterialLight' : {
    'entries'    : [ 'gl(Material|Light)(Model|)(i|f)(v|)' ],
    'prefix'     : [ '_context->ppa->gl${m1}${m2}${m4}( ${arg0plus} );', ],
  },
}
