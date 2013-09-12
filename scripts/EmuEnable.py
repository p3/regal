#!/usr/bin/python -B

# GL_REGAL_enable implementation

formulae = {
    'Enable' : {
        'entries' : [ 'glEnable' ],
        'impl' : '''
switch(${arg0})
{
  case GL_ERROR_REGAL:
    #if REGAL_ERROR
    _context->dispatcher.enable(_context->dispatcher.error);
    #endif
    return;

  case GL_DEBUG_REGAL:
    #if REGAL_DEBUG
    _context->dispatcher.enable(_context->dispatcher.debug);
    if (!_context->dbg)
    {
      _context->dbg = new DebugInfo();
      _context->dbg->Init(_context);
    }
    #endif
    return;

  case GL_LOG_REGAL:
    #if REGAL_LOG
    _context->dispatcher.enable(_context->dispatcher.logging);
    #endif
    return;

  case GL_EMULATION_REGAL:
    #if REGAL_EMULATION
    _context->dispatcher.enable(_context->dispatcher.emulation);
    #endif
    return;

  case GL_DRIVER_REGAL:
    #if REGAL_DRIVER
    _context->dispatcher.enable(_context->dispatcher.driver);
    #endif
    return;

  case GL_MISSING_REGAL:
    #if REGAL_MISSING
    _context->dispatcher.enable(_context->dispatcher.missing);
    #endif
    return;

  case GL_TRACE_REGAL:
    #if REGAL_TRACE
    _context->dispatcher.enable(_context->dispatcher.trace);
    #endif
    return;

  case GL_CACHE_REGAL:
    #if REGAL_CACHE
    _context->dispatcher.enable(_context->dispatcher.cache);
    #endif
    return;

  case GL_CODE_REGAL:
    #if REGAL_CODE
    _context->dispatcher.enable(_context->dispatcher.code);
    #endif
    return;

  case GL_STATISTICS_REGAL:
    #if REGAL_STATISTICS
    _context->dispatcher.enable(_context->dispatcher.statistics);
    #endif
    return;

  default: break;
}''',
    },
    'Disable' : {
        'entries' : [ 'glDisable' ],
        'impl' : '''
switch(${arg0})
{
  case GL_ERROR_REGAL:
    #if REGAL_ERROR
    _context->dispatcher.disable(_context->dispatcher.error);
    #endif
    return;

  case GL_DEBUG_REGAL:
    #if REGAL_DEBUG
    _context->dispatcher.disable(_context->dispatcher.debug);
    #endif
    return;

  case GL_LOG_REGAL:
    #if REGAL_LOG
    _context->dispatcher.disable(_context->dispatcher.logging);
    #endif
    return;

  case GL_EMULATION_REGAL:
    #if REGAL_EMULATION
    _context->dispatcher.disable(_context->dispatcher.emulation);
    #endif
    return;

  case GL_DRIVER_REGAL:
    #if REGAL_DRIVER
    _context->dispatcher.disable(_context->dispatcher.driver);
    #endif
    return;

  case GL_MISSING_REGAL:
    #if REGAL_MISSING
    _context->dispatcher.disable(_context->dispatcher.missing);
    #endif
    return;

  case GL_TRACE_REGAL:
    #if REGAL_TRACE
    _context->dispatcher.disable(_context->dispatcher.trace);
    #endif
    return;

  case GL_CACHE_REGAL:
    #if REGAL_CACHE
    _context->dispatcher.disable(_context->dispatcher.cache);
    #endif
    return;

  case GL_CODE_REGAL:
    #if REGAL_CODE
    _context->dispatcher.disable(_context->dispatcher.code);
    #endif
    return;

  case GL_STATISTICS_REGAL:
    #if REGAL_STATISTICS
    _context->dispatcher.disable(_context->dispatcher.statistics);
    #endif
    return;

  default: break;
}''',
    },
    'IsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'impl' : '''
switch(${arg0})
{
  case GL_ERROR_REGAL:
    #if REGAL_ERROR
    return _context->dispatcher.isEnabled(_context->dispatcher.error) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_DEBUG_REGAL:
    #if REGAL_DEBUG
    return _context->dispatcher.isEnabled(_context->dispatcher.debug) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_LOG_REGAL:
    #if REGAL_LOG
    return _context->dispatcher.isEnabled(_context->dispatcher.logging) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_EMULATION_REGAL:
    #if REGAL_EMULATION
    return _context->dispatcher.isEnabled(_context->dispatcher.emulation) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_DRIVER_REGAL:
    #if REGAL_DRIVER
    return _context->dispatcher.isEnabled(_context->dispatcher.driver) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_MISSING_REGAL:
    #if REGAL_MISSING
    return _context->dispatcher.isEnabled(_context->dispatcher.missing) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_TRACE_REGAL:
    #if REGAL_TRACE
    return _context->dispatcher.isEnabled(_context->dispatcher.trace) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_CACHE_REGAL:
    #if REGAL_CACHE
    return _context->dispatcher.isEnabled(_context->dispatcher.cache) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_CODE_REGAL:
    #if REGAL_CODE
    return _context->dispatcher.isEnabled(_context->dispatcher.code) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  case GL_STATISTICS_REGAL:
    #if REGAL_STATISTICS
    return _context->dispatcher.isEnabled(_context->dispatcher.statistics) ? GL_TRUE : GL_FALSE;
    #else
    return GL_FALSE;
    #endif

  default: break;
}''',
    },
}
