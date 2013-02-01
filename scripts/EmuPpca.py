#!/usr/bin/python -B

# Section numbers below are based on this version of the specficiation
# http://www.opengl.org/registry/doc/glspec43.compatibility.20120806.pdf

ppcaFormulae = {
  # Client Pixel state
  'ShadowPixelStore' : {
    'entries'  : [ 'glPixelStore(i|f)' ],
    'prefix'   : [ '_context->ppca->ShadowPixelStore( ${arg0plus} );', ],
  },

  # Client Vertex Array state

  # 10.3.1 - Arrays for Generic Vertex Attributes

  'ShadowVertexAttribFormat' : {
    'entries'  : [ 'glVertexAttrib(|I|L)Format' ],
    'prefix'   : [ '_context->ppca->ShadowVertexAttrib${m1}Format( ${arg0plus} );', ],
  },

  'ShadowBindVertexBuffer' : {
    'entries'  : [ 'glBindVertexBuffer' ],
    'prefix'   : [ '_context->ppca->ShadowBindVertexBuffer( ${arg0plus} );', ],
  },

  'ShadowVertexAttribBinding' : {
    'entries'  : [ 'glVertexAttribBinding' ],
    'prefix'   : [ '_context->ppca->ShadowVertexAttribBinding( ${arg0plus} );', ],
  },

  'ShadowVertexAttribPointer' : {
    'entries'  : [ 'glVertex(|I|L)Pointer' ],
    'prefix'   : [ '_context->ppca->ShadowVertexAttrib${m1}Pointer( ${arg0plus} );', ],
  },

  'ShadowEnableDisableVertexAttribAttribArray' : {
    'entries'  : [ 'gl(Enable|Disable)VertexAttribArray' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}VertexAttribArray( ${arg0plus} );', ],
  },

  # 10.3.2 - Arrays for Fixed-Function Attributes

  'ShadowVertexDataPointer' : {
    'entries'  : [ 'gl(Color|EdgeFlag|FogCoord|Index|Normal|SecondaryColor|TexCoord|Vertex)Pointer' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}Pointer( ${arg0plus} );', ],
  },

  'ShadowEnableDisableClientState' : {
    'entries'  : [ 'gl(Enable|Disable)ClientState' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}ClientState( ${arg0plus} );', ],
  },

  'ShadowClientActiveTexture' : {
    'entries'  : [ 'glClientActiveTexture' ],
    'prefix'   : [ '_context->ppca->ShadowClientActiveTexture( ${arg0plus} );', ],
  },

  # 10.3.3 - Vertex Attribute Divisors

  'ShadowVertexBindingDivisor' : {
    'entries'  : [ 'glVertexBindingDivisor' ],
    'prefix'   : [ '_context->ppca->ShadowVertexBindingDivisor( ${arg0plus} );', ],
  },

  'ShadowVertexAttribDivisor' : {
    'entries'  : [ 'glVertexAttribDivisor' ],
    'prefix'   : [ '_context->ppca->ShadowVertexAttribDivisor( ${arg0plus} );', ],
  },

  # 10.3.5 - Primitive Restart

  'ShadowEnableDisable' : {
    'entries'  : [ 'gl(Enable|Disable)' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}( ${arg0plus} );', ],
  },

  'ShadowPrimitiveRestartIndex' : {
    'entries'  : [ 'glPrimitiveRestartIndex' ],
    'prefix'   : [ '_context->ppca->ShadowPrimitiveRestartIndex( ${arg0plus} );', ],
  },

  # 10.3.8 - Vertex Arrays in Buffer Objects

  'ShadowBindBuffer' : {
    'entries'  : [ 'glBindBuffer' ],
    'prefix'   : [ '_context->ppca->ShadowBindBuffer( ${arg0plus} );', ],
  },

  # 10.4 - Vertex Array Objects

  'ShadowBindVertexArray' : {
    'entries'  : [ 'glBindVertexArray' ],
    'prefix'   : [ '_context->ppca->ShadowBindVertexArray( ${arg0plus} );', ],
  },

  # 10.5.1 - Interleaved Arrays

  'ShadowInterleavedArrays' : {
    'entries'  : [ 'glInterleavedArrays' ],
    'prefix'   : [ '_context->ppca->ShadowInterleavedArrays( ${arg0plus} );', ],
  },

  # Direct State Access Extension
  # http://www.opengl.org/registry/specs/EXT/direct_state_access.txt

  'ClientAttribDefaultDSA' : {
    'entries'  : [ 'gl(|Push)ClientAttribDefaultEXT' ],
    'impl'     : [ '_context->ppca->${m1}ClientAttribDefaultDSA( _context, ${arg0} );', ],
  },

  'ShadowMultiTexCoordPointerDSA' : {
    'entries'  : [ 'glMultiTexCoordPointerEXT' ],
    'prefix'   : [ '_context->ppca->ShadowMultiTexCoordPointerDSA( ${arg0plus} );', ],
  },

  'ShadowEnableDisableClientStateIndexedDSA' : {
    'entries'  : [ 'gl(Enable|Disable)ClientState(i|Indexed)EXT', ],
    'prefix'   : [ '_context->ppca->Shadow${m1}ClientStateIndexedDSA( ${arg0plus} );', ],
  },

  'ShadowVertexArrayOffsetDSA' : {
    'entries'  : ['glVertexArray(Vertex|Color|EdgeFlag|Index|Normal|TexCoord|MultiTexCoord|FogCoord|SecondaryColor|VertexAttrib|VertexAttribI)OffsetEXT' ],
    'prefix'   : [ '_context->ppca->ShadowVertexArray${m1}OffsetDSA( ${arg0plus} );', ],
  },

  'ShadowEnableDisableVertexArrayDSA' : {
    'entries'  : [ 'gl(Enable|Disable)VertexArrayEXT'],
    'prefix'   : [ '_context->ppca->Shadow${m1}VertexArrayDSA( ${arg0plus} );', ],
  },

  'ShadowEnableDisableVertexArrayAttribDSA' : {
    'entries'  : [ 'gl(Enable|Disable)VertexArrayAttribEXT'],
    'prefix'   : [ '_context->ppca->Shadow${m1}VertexArrayAttribDSA( ${arg0plus} );', ],
  },

  # Deleting buffers

  'ShadowDeleteBuffers' : {
    'entries'  : [ 'glDeleteBuffers' ],
    'prefix'   : [ '_context->ppca->ShadowDeleteBuffers( ${arg0plus} );', ],
  },

  'ShadowDeleteVertexArrays' : {
    'entries'  : [ 'glDeleteVertexArrays' ],
    'prefix'   : [ '_context->ppca->ShadowDeleteVertexArrays( ${arg0plus} );', ],
  },

  # Push/PopClientAttrib

  'PushClientAtrrib' : {
    'entries'  : [ 'glPushClientAttrib' ],
    'impl'     : [ '_context->ppca->PushClientAttrib( _context, ${arg0} );', ],
  },

  'PopClientAttrib' : {
    'entries' : [ 'glPopClientAttrib' ],
    'impl'    : [ '_context->ppca->PopClientAttrib( _context );', ],
  },

  # Get

  'Get'       : {
    'entries' : [ 'glGet(Integer|Integer64|Float|Double|Boolean)v' ],
    'impl'    : [
      'if( ! _context->ppca->Get( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}v( ${arg0plus} );',
      '}',
    ],
  },
}
