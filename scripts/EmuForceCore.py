#!/usr/bin/python -B

#
# TODO - For wgl and GLX too
#

formulae = {
    'ForceCoreMac' : {
        'entries' : [ 'CGLChoosePixelFormat' ],
        'impl' : [
                    'Init::init();',
                    'static const CGLPixelFormatAttribute nattribs[] =',
                    '{',
                    '  kCGLPFAOpenGLProfile,',
                    '  (CGLPixelFormatAttribute)0x3200,',
                    '  (CGLPixelFormatAttribute)0',
                    '};',
                    'if (Config::forceCoreProfile)',
                    '  attribs = nattribs;' ]
    }
}
