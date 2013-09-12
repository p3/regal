#!/usr/bin/python -B

xferFormulae = {
    'PixelStore' : {
        'entries' : [ 'glPixelStore(i|f)' ],
        'prefix' : [ '_context->xfer->PixelStore( _context, ${arg0plus} );', ],
    },
   'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'prefix' : [ '_context->xfer->ShadowActiveTexture( ${arg0} );', ],
    },
    'TexImage2D' : {
        'entries' : [ 'gl(Compressed|)Tex(Sub|)Image2D(ARB|EXT|)' ],
        'impl' : [ '_context->xfer->${m1}Tex${m2}Image2D( _context, ${arg0plus} );', ],
    }
}
