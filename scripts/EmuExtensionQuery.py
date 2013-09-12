#!/usr/bin/python -B

formulae = {
    'GetExtension' : {
        'entries' : [ 'glGetExtensionREGAL' ],
        'impl' : [ 'RegalAssert(_context->info);',
#                  '// Emulate GL_REGAL_extension_query, if necessary.',
#                  'if (!_context->info->gl_regal_extension_query)',
                   'return _context->info->getExtension(ext) ? GL_TRUE : GL_FALSE;' ]
    },
    'IsSupported' : {
        'entries' : [ 'glIsSupportedREGAL' ],
        'impl' : [ 'RegalAssert(_context->info);',
#                  '// Emulate GL_REGAL_extension_query, if necessary.',
#                  'if (!_context->info->gl_regal_extension_query)',
                   'return _context->info->isSupported(ext) ? GL_TRUE : GL_FALSE;' ]
    },
}
