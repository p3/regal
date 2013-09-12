#!/usr/bin/python -B

formulae = {
    'ErrorString' : {
        'entries' : [ 'glErrorStringREGAL' ],
        'impl' : [ 'RegalAssert(_context->info);',
#                  '// Emulate GL_REGAL_error_string, if necessary.',
#                  'if (!_context->info->gl_regal_error_string)',
                   'return Token::GLerrorToString(error);' ]
    }
}
