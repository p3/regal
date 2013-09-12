#!/usr/bin/python -B

formulae = {
    'GetString' : {
        'entries' : [ 'glGetString' ],
        'impl' : [ 'RegalAssert(_context->info);',
                   '// Regal interceptions',
                   'switch (name) ',
                   '{',
                   '  case GL_VENDOR:     return reinterpret_cast<const GLubyte *>(_context->info->regalVendor.c_str());',
                   '  case GL_RENDERER:   return reinterpret_cast<const GLubyte *>(_context->info->regalRenderer.c_str());',
                   '  case GL_VERSION:    return reinterpret_cast<const GLubyte *>(_context->info->regalVersion.c_str());',
                   '  case GL_EXTENSIONS: return reinterpret_cast<const GLubyte *>(_context->info->regalExtensions.c_str());',
                   '  default:',
                   '    break;',
                   '}' ]
    }
}
