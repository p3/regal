#!/usr/bin/python -B

vaoFormulae = {
    # TODO - GL_ARB_base_instance ?
    'Validate' : {
        'entries' : [ 'gl(Multi|)Draw(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [  '// _context->vao->Validate( _context );' ],
    },
    'BufferBinding' : {
        'entries' : [ 'glBindBuffer(ARB|)' ],
        'prefix' : [ '_context->vao->ShadowBufferBinding( ${arg0}, ${arg1} );' ],
    },
    'BindVertexArray' : {
        'entries' : [ 'glBindVertexArray(ARB|)' ],
        'impl' : [ '_context->vao->BindVertexArray( _context, ${arg0} );' ],
    },
   'GenVertexArrays' : {
        'entries' : [ 'glGenVertexArrays(ARB|)' ],
        'impl' : [ '_context->vao->GenVertexArrays( ${arg0}, ${arg1} );' ],
    },
   'DeleteVertexArrays' : {
        'entries' : [ 'glDeleteVertexArrays(ARB|)' ],
        'impl' : [ '_context->vao->DeleteVertexArrays( ${arg0}, ${arg1} );' ],
    },
   'IsVertexArray' : {
        'entries' : [ 'glIsVertexArray(ARB|)' ],
        'impl' : [ 'return _context->vao->IsVertexArray( ${arg0} );' ],
    },
   'EnableVertexAttribArray' : {
        'entries' : [ 'gl(Enable|Disable)VertexAttribArray(ARB|)' ],
        'impl' : [ 'return _context->vao->${m1}VertexAttribArray( _context, ${arg0} );' ],
    },
    'EnableDisableClientState' : {
        'entries' : [ 'gl(Enable|Disable)ClientState' ],
        'impl' : [ '_context->vao->${m1}ClientState( _context, ${arg0} );' ],
    },
   'AttribPointer' : {
        'entries' : [ 'glVertexAttribPointer(ARB|)' ],
        'impl' : [ 'return _context->vao->AttribPointer( _context, ${arg0}, ${arg1plus} );' ],
    },
    'GetAttrib' : {
        'entries' : [ 'glGetVertexAttrib(d|f|i|Pointer)v(ARB|)' ],
        'impl' : [ '_context->vao->GetAttrib( ${arg0}, ${arg1}, ${arg2} );' ],
    },
#    'GetVertexAttribPointerv' : {
#        'entries' : [ 'glGetVertexAttribPointerv(ARB|)' ],
#        'impl' : [
#            'if ( !_context->vao->GetVertexAttribPointerv( ${arg0}, ${arg1plus} ) ) {',
#            '   _context->dispatcher.emulation.glGetVertexAttribPointerv${m1}( ${arg0}, ${arg1plus} );',
#            '}',
#        ]
#    },
    'Get' : {
        'entries' : [ 'glGet(Boolean|Double|Float|Integer|Integer64)v' ],
        'impl' : [
            'if( !_context->vao->Get( ${arg0}, ${arg1} ) ) {',
            '   _context->dispatcher.emulation.glGet${m1}v( ${arg0}, ${arg1} );',
            '}',
        ]
    },
    'InterleavedArrays' : {
        'entries' : [ 'glInterleavedArrays' ],
        'impl' : [ '_context->vao->InterleavedArrays( _context, ${arg0}, ${arg1plus} );' ],
    },
    'Pointer4EXT' : {
        'entries' : [ 'gl(Color|TexCoord|Vertex)PointerEXT' ],
        'impl' : [ '_context->vao->${m1}Pointer( _context, ${arg0}, ${arg1}, ${arg2}, ${arg4} );' ],
    },
    'Pointer4' : {
        'entries' : [ 'gl(Color|SecondaryColor|TexCoord|Vertex)Pointer' ],
        'impl' : [ '_context->vao->${m1}Pointer( _context, ${arg0}, ${arg1}, ${arg2}, ${arg3} );' ],
    },
    'glSecondaryColorPointerEXT' : {
        'entries' : [ 'gl(SecondaryColor)PointerEXT' ],
        'impl' : [ '_context->vao->${m1}Pointer( _context, ${arg0}, ${arg1}, ${arg2}, ${arg3} );' ],
    },
    'NormalPointer3EXT' : {
        'entries' : [ 'glNormalPointerEXT' ],
        'impl' : [ '_context->vao->NormalPointer( _context, ${arg0}, ${arg1}, ${arg3} );' ],
    },
    'FogCoordPointer3EXT' : {
        'entries' : [ 'glFogCoordPointerEXT' ],
        'impl' : [ '_context->vao->FogCoordPointer( _context, ${arg0}, ${arg1}, ${arg2} );' ],
    },
    'Pointer3' : {
        'entries' : [ 'gl(FogCoord|Normal)Pointer' ],
        'impl' : [ '_context->vao->${m1}Pointer( _context, ${arg0}, ${arg1}, ${arg2} );' ],
    },
    'ClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)' ],
        'prefix' : [ '_context->vao->ClientActiveTexture( _context, ${arg0} );' ],
    },
}
