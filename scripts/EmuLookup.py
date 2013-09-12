#!/usr/bin/python -B

formulae = {
    'wglGetProcAddress' : {
        'entries' : [ '(wglGetProcAddress)()' ],
        'impl' : [
                  'ret = _next->call(&_next->${m1}${m2})(${arg0});',
                  'if (!ret)',
                  '  return NULL;',
                  'ret = Lookup::gl_Lookup<PROC>(${arg0});',
                  'if (ret)',
                  '  return ret;',
                  'ret = Lookup::wgl_Lookup<PROC>(${arg0});',
                  'if (ret)',
                  '  return ret;',
                   ],
    },

    'glXGetProcAddress' : {
        'entries' : [ '(glXGetProcAddress)(|ARB)' ],
        'impl' : [
                  'ret = Lookup::gl_Lookup<__GLXextFuncPtr>(reinterpret_cast<const char *>(${arg0}));',
                  'if (ret)',
                  '  return ret;',
                  'ret = Lookup::glx_Lookup<__GLXextFuncPtr>(reinterpret_cast<const char *>(${arg0}));',
                  'if (ret)',
                  '  return ret;',
                   ]
    },

    'eglGetProcAddress' : {
        'entries' : [ '(eglGetProcAddress)(|ARB)' ],
        'impl' : [
                  'ret = Lookup::gl_Lookup<__eglMustCastToProperFunctionPointerType>(reinterpret_cast<const char *>(${arg0}));',
                  'if (ret)',
                  '  return ret;',
                  'ret = Lookup::egl_Lookup<__eglMustCastToProperFunctionPointerType>(reinterpret_cast<const char *>(${arg0}));',
                  'if (ret)',
                  '  return ret;',
                   ]
    }
}
