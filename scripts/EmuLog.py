#!/usr/bin/python -B

# GL_REGAL_log implementation

logFormulae = {
    'Enable' : {
        'entries' : [ 'glEnable' ],
        'impl' : [
'switch(${arg0})',
'{',
'  case GL_LOG_ERROR_REGAL:    Logging::enableError    = true; return;',
'  case GL_LOG_WARNING_REGAL:  Logging::enableWarning  = true; return;',
'  case GL_LOG_INFO_REGAL:     Logging::enableInfo     = true; return;',
'  case GL_LOG_APP_REGAL:      Logging::enableApp      = true; return;',
'  case GL_LOG_DRIVER_REGAL:   Logging::enableDriver   = true; return;',
'  case GL_LOG_INTERNAL_REGAL: Logging::enableInternal = true; return;',
'  case GL_LOG_HTTP_REGAL:     Logging::enableHttp     = true; return;',
'  default: break;',
'}', ],
    },
    'Disable' : {
        'entries' : [ 'glDisable' ],
        'impl' : [
'switch(${arg0})',
'{',
'  case GL_LOG_ERROR_REGAL:    Logging::enableError    = false; return;',
'  case GL_LOG_WARNING_REGAL:  Logging::enableWarning  = false; return;',
'  case GL_LOG_INFO_REGAL:     Logging::enableInfo     = false; return;',
'  case GL_LOG_APP_REGAL:      Logging::enableApp      = false; return;',
'  case GL_LOG_DRIVER_REGAL:   Logging::enableDriver   = false; return;',
'  case GL_LOG_INTERNAL_REGAL: Logging::enableInternal = false; return;',
'  case GL_LOG_HTTP_REGAL:     Logging::enableHttp     = false; return;',
'  default: break;',
'}', ],
    },
    'IsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'impl' : [
'switch(${arg0})',
'{',
'  case GL_LOG_ERROR_REGAL:    return Logging::enableError    ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_WARNING_REGAL:  return Logging::enableWarning  ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_INFO_REGAL:     return Logging::enableInfo     ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_APP_REGAL:      return Logging::enableApp      ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_DRIVER_REGAL:   return Logging::enableDriver   ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_INTERNAL_REGAL: return Logging::enableInternal ? GL_TRUE : GL_FALSE;',
'  case GL_LOG_HTTP_REGAL:     return Logging::enableHttp     ? GL_TRUE : GL_FALSE;',
'  default: break;',
'}', ],
    },
    'Callback' : {
        'entries' : [ 'glLogMessageCallbackREGAL' ],
        'impl' : [
          '// Emulate GL_REGAL_log, if necessary.',
          'if (!_context->info->gl_regal_log)',
          '{',
          '  _context->logCallback = callback;',
          '  return;',
          '}',
        ],
    },
}
