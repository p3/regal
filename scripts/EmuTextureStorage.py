#!/usr/bin/python -B

texstoFormulae = {
    'TextureStorage' : { 
        'entries' : [ 'glTexStorage(1|2|3)D(EXT|)' ],
        'impl' : [ '_context->texsto->TextureStorage( _context, ${arg0plus} );', ],
    },
    'GetTexParameterv' : {
        'entries' : [ 'glGetTexParameter(I|)(u|)(f|i)v' ],
        'impl' : [
            'RegalAssert(_context);',
            'if ( !_context->texsto->GetTexParameterv( _context, ${arg0plus} ) ) {',
            '   _context->dispatcher.emulation.glGetTexParameter${m1}${m2}${m3}v( ${arg0plus} );',
            '}',
        ]
    },
    'DeleteTextures' : {
        'entries' : [ 'glDeleteTextures' ],
        'prefix' : [
          'RegalAssert(_context);',
          '_context->texsto->DeleteTextures( _context, ${arg0plus} );'
        ],
    },

    #'TexImage' : { # disallow these if the object was specified with TextureStorage
    #    'entries' : [ 'gl(Copy|)TexImage(1|2|3)D(ARB|)' ],
    #    'impl' : [ '_context->texsto->${m1}TexImage( _context, ${arg0plus} );', ],
    #}
}
