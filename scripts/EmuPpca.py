#!/usr/bin/python -B

ppcaFormulae = {

  # Push/Pop ClientAttrib & Get

  'PushClientAttrib' : {
    'entries'  : [ 'glPushClientAttrib' ],
    'impl'     : [ '_context->ppca->glPushClientAttrib( *_context, ${arg0} );', ],
  },

  'PopClientAttrib' : {
    'entries' : [ 'glPopClientAttrib' ],
    'impl'    : [ '_context->ppca->glPopClientAttrib( *_context );', ],
  },

  'PushClientAttribDefaultEXT' : {
    'entries'  : [ 'gl(Push|)ClientAttribDefaultEXT' ],
    'impl'     : [ '_context->ppca->gl${m1}ClientAttribDefaultEXT( *_context, ${arg0} );', ],
  },

  'Get'       : {
    'entries' : [ 'glGet(Integer|Float|Double|Boolean)v(EXT|)' ],
    'impl'    : [
      'if ( ! _context->ppca->glGetv( *_context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}v${m2}( ${arg0plus} );',
      '}',
    ],
  },
  # shadow all the client state

  'Bind'      : {
    'entries' : [ 'glBind(Vertex|)(Buffer|Array)(s|)' ],
    'prefix'  : [ '_context->ppca->glBind${m1}${m2}${m3}( ${arg0plus} );', ],
  },
  'Delete'    : {
    'entries' : [ 'glDelete(VertexArrays|Buffers)' ],
    'prefix'  : [ '_context->ppca->glDelete${m1}( ${arg0plus} );', ],
  },
  'EnableDisable' : {
    'entries' : [ 'gl(Enable|Disable)(ClientState|VertexAttribArray|)(i|Indexed|)(EXT|)' ],
    'prefix'  : [ '_context->ppca->gl${m1}${m2}${m3}${m4}( ${arg0plus} );', ],
  },
  'Pointer'   : {
    'entries' : [ 'gl(Color|EdgeFlag|FogCoord|Index|Normal|SecondaryColor|TexCoord|Vertex)Pointer' ],
    'prefix'  : [ '_context->ppca->gl${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexAttrib' : {
    'entries' : [ 'glVertexAttrib(I|L|)(Binding|Format)' ],
    'prefix'  : [ '_context->ppca->glVertexAttrib${m1}${m2}( ${arg0plus} );', ],
  },
  'MultiTexCoordPointerEXT': {
    'entries' : [ 'glMultiTexCoordPointerEXT' ],
    'prefix'  : [ '_context->ppca->glMultiTexCoordPointerEXT( ${arg0plus} );', ],
  },
  'ClientActiveTexture' : {
    'entries' : [ 'glClientActiveTexture(ARB|)' ],
    'prefix'  : [ '_context->ppca->glClientActiveTexture( ${arg0plus} );', ],
  },
  'PrimitiveRestartIndex' : {
    'entries' : [ 'glPrimitiveRestartIndex' ],
    'prefix'  : [ '_context->ppca->glPrimitiveRestartIndex( ${arg0plus} );', ],
  },
  'VertexDivisor' : {
    'entries' : [ 'glVertex(Attrib|Binding)Divisor' ],
    'prefix'  : [ '_context->ppca->glVertex${m1}Divisor( ${arg0plus} );', ],
  },
  'VertexAttribPointer' : {
    'entries' : [ 'glVertexAttrib(I|L|)Pointer' ],
    'prefix'  : [ '_context->ppca->glVertexAttrib${m1}Pointer( ${arg0plus} );', ],
  },
  'VertexArrayEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayEXT' ],
      'prefix'  : [ '_context->ppca->gl${m1}VertexArrayEXT( ${arg0plus} );', ],
  },
  'VertexArrayAttribEnable' : {
      'entries' : [ 'gl(Enable|Disable)VertexArrayAttribEXT' ],
      'prefix'  : [ '_context->ppca->gl${m1}VertexArrayAttribEXT( ${arg0plus} );', ],
  },
  'DsaVertexArrayOffsetCommands' : {
      'entries' : [ 'glVertexArray(EdgeFlag|Index|Normal|FogCoord|Vertex|Color|TexCoord|SecondaryColor|VertexAttribI|MultiTexCoord|VertexAttrib)OffsetEXT' ],
      'prefix'  : [ '_context->ppca->glVertexArray${m1}OffsetEXT( ${arg0plus} );', ],
  },
  'InterleavedArrays ' : {
      'entries' : [ 'glInterleavedArrays' ],
      'prefix'  : [ '_context->ppca->glInterleavedArrays( ${arg0plus} );', ],
  },
  'PixelStore' : {
      'entries' : [ 'glPixelStore(i|f)' ],
      'prefix'  : [ '_context->ppca->glPixelStore( ${arg0plus} );', ],
  },
}
