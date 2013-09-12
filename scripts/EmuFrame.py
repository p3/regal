#!/usr/bin/python -B

formulae = {

}

formulaeGlobal = {

    # GL_GREMEDY_frame_terminator

    'GL_GREMEDY_frame_terminator' : {
        'entries' : [ 'glFrameTerminatorGREMEDY' ],
        'impl' : [
                   '// Notify Regal::Frame about the frame terminator event.',
                   'if (_context && _context->frame)',
                   '  _context->frame->glFrameTerminatorGREMEDY(*_context);',
                   'RegalAssert(_context->info);',
                   '// Return to application unless GL_GREMEDY_frame_terminator is supported by the driver.',
                   'if (!_context->info->gl_gremedy_frame_terminator) return;' ]
    },

    # wglSwapBuffers

    'wglSwapBuffers' : {
        'entries' : [ 'wglSwapBuffers' ],
        'impl' : [ 'RegalContext *_context = REGAL_GET_CONTEXT();',
                   '// Notify Regal::Frame about the swap buffers event.',
                   'if (_context && _context->frame)',
                   '    _context->frame->wglSwapBuffers(*_context);'
                 ]
    },

    # glXSwapBuffers

    'glXSwapBuffers' : {
        'entries' : [ 'glXSwapBuffers' ],
        'impl' : [ '// Keep track of X11 Display and GLXDrawable for logging purposes.',
                   'RegalContext *_context = REGAL_GET_CONTEXT();',
                   'if (_context)',
                   '{',
                   '    #if REGAL_SYS_X11',
                   '    _context->x11Display  = dpy;',
                   '    #endif',
                   '    _context->x11Drawable = drawable;',
                   '}',
                   '// Notify Regal::Frame about the swap buffers event.',
                   'if (_context && _context->frame)',
                   '    _context->frame->glXSwapBuffers(*_context);'
                 ]
    },

    # glXMakeCurrent

    'glXMakeCurrent' : {
        'entries' : [ 'glXMakeCurrent' ],
        'impl' : [ '// Keep track of X11 Display and GLXDrawable for logging purposes.',
                   'RegalContext *_context = REGAL_GET_CONTEXT();',
                   'if (_context)',
                   '{',
                   '    #if REGAL_SYS_X11',
                   '    _context->x11Display  = dpy;',
                   '    #endif',
                   '    _context->x11Drawable = drawable;',
                   '}'
                 ]
    },

    # eglSwapBuffers

    'eglSwapBuffers' : {
        'entries' : [ 'eglSwapBuffers' ],
        'impl' : [ 'RegalContext *_context = REGAL_GET_CONTEXT();',
                   '// Notify Regal::Frame about the swap buffers event.',
                   'if (_context && _context->frame)',
                   '    _context->frame->eglSwapBuffers(*_context);'
                 ]
    },

    # CGLFlushDrawable

    'CGLFlushDrawable' : {
        'entries' : [ 'CGLFlushDrawable' ],
        'impl' : [ 'RegalContext *_context = REGAL_GET_CONTEXT();',
                   '// Notify Regal::Frame about the flush drawable event.',
                   'if (_context && _context->frame)',
                   '    _context->frame->CGLFlushDrawable(*_context);'
                 ]
    }
}
