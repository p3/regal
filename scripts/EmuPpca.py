#!/usr/bin/python -B

ppcaFormulae = {
  'ShadowPixelStore' : {
    'entries'  : [ 'glPixelStore(i|f)' ],
    'prefix'   : [ '_context->ppca->ShadowPixelStore( ${arg0plus} );', ],
  },

  'ShadowEnableDisableClientState' : {
    'entries'  : [ 'gl(Enable|Disable)ClientState' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}ClientState( ${arg0plus} );', ],
  },

  'ShadowVertexDataPointer' : {
    'entries'  : [ 'gl(Color|EdgeFlag|FogCoord|Index|Normal|SecondaryColor|TexCoord|Vertex)Pointer' ],
    'prefix'   : [ '_context->ppca->Shadow${m1}Pointer( ${arg0plus} );', ],
  },

  'PushClientAtrrib' : {
    'entries'  : [ 'glPushClientAttrib' ],
    'impl'     : [ '_context->ppca->PushClientAttrib( _context, ${arg0} );', ],
  },

  'PopClientAttrib' : {
    'entries' : [ 'glPopClientAttrib' ],
    'impl'    : [ '_context->ppca->PopClientAttrib( _context );', ],
  },

  'PushClientAtrribDefaultEXT' : {
    'entries'  : [ 'glPushClientAttribDefaultEXT' ],
    'impl'     : [ '_context->ppca->PushClientAttribDefaultEXT( _context, ${arg0} );', ],
  },

  'Get'       : {
    'entries' : [ 'glGet(Integer|Integer64|Float|Double|Boolean)v' ],
    'impl'    : [
      'if( ! _context->ppca->Get( _context, ${arg0plus} ) ) {',
      '  _context->dispatcher.emulation.glGet${m1}v( ${arg0plus} );',
      '}',
    ],
  },
}
