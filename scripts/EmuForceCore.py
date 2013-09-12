#!/usr/bin/python -B

#
# TODO - For wgl and GLX too
#

formulae = {
    'ForceCoreMac' : {
        'entries' : [ 'CGLChoosePixelFormat' ],
        'impl' : [
                    'scoped_array<CGLPixelFormatAttribute> nattribs;',
                    'if (Config::forceCoreProfile)',
                    '  attribs = nattribs = Mac::forceCoreAttribs(attribs);' ]
    }
}
