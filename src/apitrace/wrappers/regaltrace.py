##########################################################################
#
# Copyright 2011 VMware, Inc.
# Copyright 2013 Cass Everitt
# All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
##########################################################################/


"""Regal tracing generator."""


from gltrace import GlTracer
import specs.stdapi as stdapi
from specs.stdapi import Module, API

# graphics library
from specs.glapi import glapi
from specs.glesapi import glesapi

# window system integration
from specs.cglapi import cglapi
from specs.eglapi import eglapi
from specs.glxapi import glxapi
from specs.wglapi import wglapi

class RegalTracer(GlTracer):

    name2guard = { 'cgl' : 'REGAL_SYS_OSX', 'egl' : 'REGAL_SYS_EGL', 'glx' : 'REGAL_SYS_GLX', 'wgl' : 'REGAL_SYS_WGL' }

    def traceModuleGuardBegin(self, module):
        if module.name in self.name2guard:
            print '#if %s' % self.name2guard[module.name]

    def traceModuleGuardEnd(self, module):
        if module.name in self.name2guard:
            print '#endif // %s' % self.name2guard[module.name]

    def traceFunctionImplBegin(self, module):
        self.traceModuleGuardBegin( module )
        print
        print 'namespace Regal { namespace Trace {'
        print

    def traceFunctionImplEnd(self, module):
        print
        print '} /* namespace Trace */ } /* namespace Regal */'
        print
        self.traceModuleGuardEnd( module )

    def prototype(self, function):
        s = '%s %s( ' % ( function.type, function.name )
        s += ', '.join( [ '%s %s' % (arg.type, arg.name) for arg in function.args ] );
        s += ' )'
        return s

    def isFunctionPublic(self, function):
        # calling via Regal is through dispatch tables, so no public functions needed
        return False

    def invokeFunction(self, function):
        ret = ''
        if function.type != stdapi.Void:
            ret = '_result = '
        print '    %s_%s(%s);' % (ret, function.name, ', '.join([ '%s' % arg.name for arg in function.args ]))

    def traceFunctionImpl(self, function):
        print self.prototype(function) + ' {'
        if function.type is not stdapi.Void:
            print '    %s _result;' % function.type
        self.traceFunctionImplBody(function)
        if function.type is not stdapi.Void:
            print '    return _result;'
        print '}'
        print


    def traceFunctionImplBody(self, function):
        if function.name == 'CGLReleaseContext':
            # Unlike other GL APIs like EGL or GLX, CGL will make the context
            # not current if it's the current context.
            print '    if (_CGLGetContextRetainCount(ctx) == 1) {'
            print '        if (gltrace::releaseContext((uintptr_t)ctx)) {'
            print '            if (_CGLGetCurrentContext() == ctx) {'
            print '                gltrace::clearContext();'
            print '            }'
            print '        }'
            print '    }'

        if function.name == 'CGLDestroyContext':
            # The same rule applies here about the  as for CGLReleaseContext.
            print '    if (gltrace::releaseContext((uintptr_t)ctx)) {'
            print '        if (_CGLGetCurrentContext() == ctx) {'
            print '            gltrace::clearContext();'
            print '        }'
            print '    }'

        GlTracer.traceFunctionImplBody(self, function)

        if function.name == 'CGLCreateContext':
            print '    if (_result == kCGLNoError) {'
            print '        gltrace::createContext((uintptr_t)*ctx);'
            print '    }'

        if function.name == 'CGLSetCurrentContext':
            print '    if (_result == kCGLNoError) {'
            print '        if (ctx != NULL) {'
            print '            gltrace::setContext((uintptr_t)ctx);'
            print '        } else {'
            print '            gltrace::clearContext();'
            print '        }'
            print '    }'

        if function.name == 'CGLRetainContext':
            print '    gltrace::retainContext((uintptr_t)ctx);'


if __name__ == '__main__':
    print '''
// This file was generated by "python regaltrace.py > regaltrace.cpp"
// You should edit regaltrace.py, not regaltrace.cpp directly.
// Cass Everitt - 2013

#include <stdlib.h>
#include <string.h>

#include "trace_writer_regal.hpp"
#define localWriter regalWriter

#include "glproc.hpp"
#include "glsize.hpp"

#include <RegalSystem.h>

// Workaround for GLX emulation no supported for apitrace, yet

#if REGAL_SYS_OSX || REGAL_SYS_ANDROID
# if REGAL_SYS_GLX
# undef REGAL_SYS_GLX
# define REGAL_SYS_GLX 0
# endif
#endif

'''

    cglmodule = Module('cgl')
    cglmodule.mergeModule(cglapi)
    eglmodule = Module('egl')
    eglmodule.mergeModule(eglapi)
    glxmodule = Module('glx')
    glxmodule.mergeModule(glxapi)
    wglmodule = Module('wgl')
    wglmodule.mergeModule(wglapi)

    gfxmodule = Module('gfx')
    gfxmodule.mergeModule(glapi)
    gfxmodule.mergeModule(glesapi)
    api = API()
    api.addModule(gfxmodule)
    api.addModule(cglmodule)
    api.addModule(eglmodule)
    api.addModule(glxmodule)
    api.addModule(wglmodule)
    tracer = RegalTracer()
    tracer.traceApi(api)


