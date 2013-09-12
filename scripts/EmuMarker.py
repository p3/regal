#!/usr/bin/python -B

formulae = {

}

formulaeGlobal = {

    # GL_EXT_debug_marker

    'Insert' : {
        'entries' : [ 'glInsertEventMarkerEXT' ],
        'cond'    : '_context->info->gl_ext_debug_marker',
        'prefix'  : 'std::string _message = Marker::toStringEXT(length, marker);',
        'impl'    : '''
if (_context->marker)
  _context->marker->InsertEventMarker(*_context, _message);
RegalAssert(_context->info);'''
    },

    'Push' : {
        'entries' : [ 'glPushGroupMarkerEXT' ],
        'cond'    : '_context->info->gl_ext_debug_marker',
        'prefix'  : [ 'std::string _message = Marker::toStringEXT(length, marker);' ],
        'impl'    : 'RegalAssert(_context->info);',
        'suffix'  : '''
if (_context->marker)
  _context->marker->PushGroupMarker(*_context, _message);'''
    },

    'Pop' : {
        'entries' : [ 'glPopGroupMarkerEXT' ],
        'cond'    : '_context->info->gl_ext_debug_marker',
        'prefix'  : '''
if (_context && _context->marker)
  _context->marker->PopGroupMarker(*_context);''',
        'impl'    : 'RegalAssert(_context->info);'
    },

    # GL_KHR_debug
    # http://www.opengl.org/registry/specs/KHR/debug.txt
    #
    # glDebugMessageInsert
    # glPushDebugGroup
    # glPopDebugGroup

    'KHR_debug Push' : {
        'entries' : [ 'glPushDebugGroup' ],
        'cond'    : '_context->info->gl_khr_debug',
        'prefix'  : [ 'std::string _message = Marker::toStringKHR(length, message);' ],
        'impl'    : [ 'RegalAssert(_context->info);' ],
        'suffix'  : [ 'if (_context->marker)',
                      '  _context->marker->PushGroupMarker(*_context, _message);' ]
    },
    'KHR_debug Pop' : {
        'entries' : [ 'glPopDebugGroup' ],
        'cond'    : '_context->info->gl_khr_debug',
        'prefix'  : [ 'if (_context->marker)',
                      '  _context->marker->PopGroupMarker(*_context);' ],
        'impl'    : [ 'RegalAssert(_context->info);' ]
    },

    'KHR_debug Insert' : {
        'entries' : [ 'glDebugMessageInsert' ],
        'cond'    : '_context->info->gl_khr_debug',
        'prefix'  : [ 'std::string _message = Marker::toStringKHR(length, buf);' ],
        'impl'    : [ 'if (_context->marker)',
                      '  _context->marker->InsertEventMarker(*_context, _message);',
                      'RegalAssert(_context->info);' ]
    },

    # GL_ARB_debug_output
    # http://www.opengl.org/registry/specs/ARB/debug_output.txt
    #
    # glDebugMessageInsertARB

    'ARB_debug_output Insert' : {
        'entries' : [ 'glDebugMessageInsertARB' ],
        'cond'    : '_context->info->gl_arb_debug_output',
        'prefix'  : [ 'std::string _message = Marker::toStringEXT(length, buf);' ],
        'impl'    : [ 'if (_context->marker)',
                      '  _context->marker->InsertEventMarker(*_context, _message);',
                      'RegalAssert(_context->info);' ]
    },

    # GL_AMD_debug_output
    # http://www.opengl.org/registry/specs/AMD/debug_output.txt
    #
    # glDebugMessageInsertAMD

    'AMD_debug_output Insert' : {
        'entries' : [ 'glDebugMessageInsertAMD' ],
        'cond'    : '_context->info->gl_amd_debug_output',
        'prefix'  : [ 'std::string _message = Marker::toStringEXT(length, buf);' ],
        'impl'    : [ 'if (_context->marker)',
                      '  _context->marker->InsertEventMarker(*_context, _message);',
                      'RegalAssert(_context->info);' ]
    },

    # GL_GREMEDY_string_marker

    'GL_GREMEDY_string_marker' : {
        'entries' : [ 'glStringMarkerGREMEDY' ],
        'cond'    : '_context->info->gl_gremedy_string_marker',
        'prefix'  : [ 'std::string _message = Marker::toStringEXT(len, static_cast<const char *>(string));' ],
        'impl'    : [ 'if (_context->marker)',
                      '  _context->marker->InsertEventMarker(*_context, _message);',
                      'RegalAssert(_context->info);' ]
    }
}
