#!/usr/bin/python -B

# RegalDispatchGLX - GLX emulation

formulaeGlobal = {

  # glXQueryExtensionsString

  'glXQueryExtensionsString' : {
    'entries' : [ 'glXQueryExtensionsString' ],
    'impl' : '''
const char *extensions = "";
return extensions;
'''
  },

  # glXChooseFBConfig

  'glXChooseFBConfig' : {
    'entries' : [ 'glXChooseFBConfig' ],
    'impl' : '''
static GLXFBConfig configs[1];
return configs;
'''
  },

  # glXGetVisualFromFBConfig

  'glXGetVisualFromFBConfig' : {
    'entries' : [ 'glXGetVisualFromFBConfig' ],
    'impl' : '''
static XVisualInfo vi;
return &vi;
'''
  },
}
