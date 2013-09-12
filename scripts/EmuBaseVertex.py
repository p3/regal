#!/usr/bin/python -B

baseVertexFormulae = {
  'DrawElements'       : {
    'entries' : [ 'gl(Multi|)Draw(Range|)Elements(Instanced|)BaseVertex(BaseInstance|)' ],
    'impl'    : [
      'if( ! _context->bv->gl${m1}Draw${m2}Elements${m3}BaseVertex${m4}( *_context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.gl${m1}Draw${m2}Elements${m3}BaseVertex${m4}( ${arg0plus} );',
      '}',
    ],
  },
  'Bind'      : {
    'entries' : [ 'glBind(Vertex|)(Buffer|Array)' ],
    'prefix'  : [ '_context->bv->glBind${m1}${m2}( ${arg0plus} );', ],
  },
  'EnableDisable' : {
    'entries' : [ 'gl(Enable|Disable)(ClientState|VertexAttribArray|)(i|Indexed|)(EXT|)' ],
    'prefix'  : [ '_context->bv->gl${m1}${m2}${m3}${m4}( ${arg0plus} );', ],
  },
  'Pointer'   : {
    'entries' : [ 'gl(Color|EdgeFlag|FogCoord|Index|Normal|SecondaryColor|TexCoord|Vertex)Pointer' ],
    'prefix'  : [ '_context->bv->gl${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexAttrib' : {
    'entries' : [ 'glVertexAttrib(I|L|)(Binding|Format)' ],
    'prefix'  : [ '_context->bv->glVertexAttrib${m1}${m2}( ${arg0plus} );', ],
  },
  'MultiTexCoordPointerEXT': {
    'entries' : [ 'glMultiTexCoordPointerEXT' ],
    'prefix'  : [ '_context->bv->glMultiTexCoordPointerEXT( ${arg0plus} );', ],
  },
  'ClientActiveTexture' : {
    'entries' : [ 'glClientActiveTexture(ARB|)' ],
    'prefix'  : [ '_context->bv->glClientActiveTexture( ${arg0plus} );', ],
  },
  'PrimitiveRestartIndex' : {
    'entries' : [ 'glPrimitiveRestartIndex' ],
    'prefix'  : [ '_context->bv->glPrimitiveRestartIndex( ${arg0plus} );', ],
  },
  'VertexDivisor' : {
    'entries' : [ 'glVertex(Attrib|Binding)Divisor' ],
    'prefix'  : [ '_context->bv->glVertex${m1}Divisor( ${arg0plus} );', ],
  },
  'VertexAttribPointer' : {
    'entries' : [ 'glVertexAttrib(I|L|)Pointer' ],
    'prefix'  : [ '_context->bv->glVertexAttrib${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexArrayEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayEXT' ],
      'prefix'  : [ '_context->bv->gl${m1}VertexArrayEXT( ${arg0plus} );', ],
  },
  'VertexArrayAttribEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayAttribEXT' ],
      'prefix'  : [ '_context->bv->gl${m1}VertexArrayAttribEXT( ${arg0plus} );', ],
  },
  'DsaVertexArrayOffsetCommands' : {
      'entries' : [ 'glVertexArray(EdgeFlag|Index|Normal|FogCoord|Vertex|Color|TexCoord|SecondaryColor|VertexAttribI|MultiTexCoord|VertexAttrib)OffsetEXT' ],
      'prefix'  : [ '_context->bv->glVertexArray${m1}OffsetEXT( ${arg0plus} );', ],
  },
  'InterleavedArrays ' : {
      'entries' : [ 'glInterleavedArrays' ],
      'prefix'  : [ '_context->bv->glInterleavedArrays( ${arg0plus} );', ],
  },
}
