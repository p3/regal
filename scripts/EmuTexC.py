#!/usr/bin/python -B

# Copyright (c) 2012 Scott Nations
# Copyright (c) 2012 Mathias Schott
# Copyright (c) 2012 Nigel Stewart
# Copyright (c) 2012 Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#   Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
#   Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.


"""Emulates texture conversions on calls to glTexSubImage2D and other similar
OpenGL calls.

This is needed by a strict GLES2 backend, as the intent of the function under
GLES2 is to just provide a simple copy of texture data with the same format to
the target texture.

However in order to perform the conversion, we need to track most calls that
create, load, and destroy textures as well, so we know what internal format
they are so as to convert the texture correctly."""

# FIXME(lpique) glCopyTexImage2D can also set a texture format.

texCFormulae = {
    'ShadowActiveTextureUnit' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'prefix' : '_context->texc->ShadowActiveTexture( ${arg0plus} );',
    },

    'ShadowBindTexture' : {
        'entries' : [ 'glBindTexture(EXT|)' ],
        'prefix' : '_context->texc->ShadowBindTexture( ${arg0plus} );',
    },

    'ShadowDeleteTexture' : {
        'entries' : [ 'glDeleteTextures(EXT|)' ],
        'prefix' : '_context->texc->ShadowDeleteTextures( ${arg0plus} );',
    },

    'ShadowGenTextures' : {
        'entries' : [ 'glGenTextures(EXT|)' ],
        'impl' :
'''
_context->dispatcher.emulation.glGenTextures( ${arg0plus} );
_context->texc->ShadowGenTextures( ${arg0}, ${arg1} );'''
    },

    'ShadowGenerateMipmap' : {
        'entries' : [ 'glGenerateMipmap(EXT|)' ],
        'prefix' : '_context->texc->ShadowGenerateMipmap( ${arg0plus} );',
    },

    'ShadowPixelStorei' : {
        'entries' : [ 'glPixelStorei' ],
        'prefix' : '_context->texc->ShadowPixelStore( ${arg0plus} );',
    },

    'ShadowTexImage2D' : {
        'entries' : [ 'glTexImage2D' ],
        'prefix' : '_context->texc->ShadowTexImage2D( ${arg0}, ${arg1}, ${arg6}, ${arg7} );',
    },

    'ConvertTexSubImage2D' : {
        'entries' : [ 'glTexSubImage2D' ],
        'impl' :
'''
GLenum targetFormat;
GLenum targetType;
_context->texc->GetFormatAndType( ${arg0}, ${arg1}, &targetFormat, &targetType );
Emu::ConvertedBuffer _buffer( _context->texc->unpackPSS, targetFormat, targetType );
if ( _buffer.ConvertFrom( ${arg4}, ${arg5}, ${arg6}, ${arg7}, ${arg8} ) )
{
  if (_context->texc->unpackPSS.alignment != 4)
    _context->dispatcher.emulation.glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
  _context->dispatcher.emulation.glTexSubImage2D( ${arg0}, ${arg1}, ${arg2}, ${arg3}, ${arg4}, ${arg5}, targetFormat, targetType, _buffer.Get() );
  if (_context->texc->unpackPSS.alignment != 4)
    _context->dispatcher.emulation.glPixelStorei( GL_UNPACK_ALIGNMENT, _context->texc->unpackPSS.alignment );
}
else
{
  _context->dispatcher.emulation.glTexSubImage2D( ${arg0plus} );
}'''
    },
}
