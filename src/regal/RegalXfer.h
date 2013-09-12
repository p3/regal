/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*

 Regal pixel transfer emulation
 Cass Everitt

 */

#ifndef __REGAL_XFER_H__
#define __REGAL_XFER_H__

#include "RegalUtil.h"

#if REGAL_EMU_XFER

REGAL_GLOBAL_BEGIN

#include "RegalEmu.h"
#include "RegalPrivate.h"
#include "RegalContext.h"
#include "RegalToken.h"
#include "RegalContextInfo.h"

#include <map>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

struct Xfer
{
  void Init( RegalContext &ctx )
  {
    UNUSED_PARAMETER(ctx);

    activeTextureIndex = 0;
    for( int i  = 0; i < REGAL_EMU_MAX_TEXTURE_UNITS; i++ ) {
      textureBinding2D[ i ] = 0;
    }
    unpackRowLength = 0;
    unpackSkipRows = 0;
    unpackSkipPixels = 0;
  }

  void Cleanup( RegalContext &ctx )
  {
    UNUSED_PARAMETER(ctx);
  }

  void PixelStore( RegalContext * ctx, GLenum pname, GLint param );

  void PixelStore( RegalContext * ctx, GLenum pname, GLfloat param )
  {
    PixelStore( ctx, pname, GLint( param ) );
  }

  void ShadowActiveTexture( GLenum texture )
  {
    if (validTextureEnum(texture))
      activeTextureIndex = texture - GL_TEXTURE0;
  }

  void ShadowBindTexture( GLenum target, GLuint name )
  {
    UNUSED_PARAMETER(target);
    textureBinding2D[ activeTextureIndex ] = name;
  }

  void TexImage2D( RegalContext * ctx, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
  void TexSubImage2D( RegalContext * ctx, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );

  void CompressedTexImage2D( RegalContext * ctx, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
  void CompressedTexSubImage2D( RegalContext * ctx, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );

  // State tracking

  GLint unpackRowLength;
  GLint unpackSkipRows;
  GLint unpackSkipPixels;

  int activeTextureIndex;
  GLuint textureBinding2D[REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS];
  std::map< GLuint, GLuint > name2ifmt;
};

}

REGAL_NAMESPACE_END

#endif

#endif // ! __REGAL_XFER_H__
