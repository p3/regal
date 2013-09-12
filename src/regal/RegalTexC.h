/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc.
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

 Regal Texture Conversion implementation
 Lloyd Pique

 */

#ifndef __REGAL_TEXC_H__
#define __REGAL_TEXC_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <boost/cstdint.hpp>

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalSharedMap.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using ::boost::uint8_t;
using ::boost::uint16_t;
using ::boost::uint32_t;

namespace Emu {

struct TexC;
struct TextureUnitState;

struct PixelStorageStateGLES20
{
  GLint alignment;

  void Reset();
};

struct ConvertedBuffer
{
  ConvertedBuffer( const PixelStorageStateGLES20& pss, GLenum targetFormat, GLenum targetType );

  bool ConvertFrom( GLsizei width, GLsizei height, GLenum sourceFormat, GLenum sourceType, const GLvoid* sourcePixels );

  const GLvoid* Get() const {
    return &targetBuffer_[ 0 ];
  }

  const PixelStorageStateGLES20& pss_;
  std::vector<uint8_t> targetBuffer_;
  GLenum targetFormat_;
  GLenum targetType_;

 private:
  // Disallow copying.
  ConvertedBuffer( const ConvertedBuffer& );
  ConvertedBuffer& operator= ( const ConvertedBuffer& );
};

struct TextureLevelState
{
  TextureLevelState()
  : format( GL_RGBA )
  , type( GL_UNSIGNED_BYTE )
  {}

  TextureLevelState( GLenum format, GLenum type )
  : format( format )
  , type( type )
  {}

  GLenum format;
  GLenum type;
};

struct TextureState
{
  enum FormatLevel { DEFAULT_FORMAT_LEVEL = -1 };

  typedef std::vector<TextureUnitState*> BoundTextureUnits;
  typedef std::map<GLint, TextureLevelState> TextureLevelStateMap;

  ~TextureState();

  void Reset();

  void Bind( TextureUnitState* unit );
  void Unbind( TextureUnitState* unit );
  void UnbindAll();

  TextureLevelState& GetLevelState_( GLint level );

  void SetFormatAndType( GLint level, GLenum format, GLenum type );
  void GetFormatAndType( GLint level, GLenum* format, GLenum* type );
  void SimulateComputeMipMaps();

  BoundTextureUnits boundTextureUnits;

  TextureLevelStateMap textureLevelState;
};

struct TextureUnitState;

struct TextureStateRef
{
  TextureStateRef();
  ~TextureStateRef();
  void Reset( TextureUnitState* unit, TextureState* texture );

  TextureState* Get() const {
    return texture_;
  }

  bool IsBound( const TextureState* texture ) const {
    return texture == texture_;
  }

  TextureState* operator ->() {
    return texture_;
  }

  TextureState* texture_;
  TextureUnitState* unit_;
};

struct TextureUnitState
{
  TextureStateRef binding2D_;
  TextureStateRef bindingCubeMap_;
  TextureStateRef bindingRectangleARB_;

  void UnbindAll();
  void Unbind( TextureState* texture );
  void Bind( GLenum target, TextureState* texture );
  TextureState* GetBinding( GLenum target ) const;
};


struct TexC
{
  TexC();

  void Init( RegalContext &ctx );

  void Cleanup( RegalContext &ctx )
  {
    UNUSED_PARAMETER(ctx);
  }

  void Reset_();

  TextureState* GetBoundTexture_( GLenum textureEnum, GLenum target );
  TextureState* GetBoundTextureUnit_( GLuint textureUnit, GLenum target );
  TextureState* GetTexture_( GLuint texture );

  void GetFormatAndType( GLenum target, GLint level, GLenum* format, GLenum* type );
  void ShadowTexImage2D( GLenum target, GLint level, GLenum format, GLenum type );

  void ShadowPixelStore( GLenum pname, GLint pvalue );

  void ShadowGenTextures( GLsizei n, GLuint* textures );
  void ShadowDeleteTextures( GLsizei n, const GLuint* textures );

  void ShadowActiveTexture( GLenum texture );
  void ShadowBindTexture( GLenum target, GLuint texture );

  void ShadowGenerateMipmap( GLenum target );

  void GenTexture_( GLuint texture );
  void DeleteTexture_( GLuint texture );

  // TODO(lpique) Replace with a hashmap for speed. (But caution -- need an
  // implementation which keeps pointers to TextureStates valid, or another
  // binding mechanism)
  typedef shared_map < GLuint, TextureState > MapTextureToTextureState;

  enum { TEXTURE_ZERO = 0 };

  MapTextureToTextureState mapTextureToTextureState;

  PixelStorageStateGLES20 unpackPSS;

  GLuint currentTextureUnit;
  TextureUnitState textureUnitArrayState[ REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS ];
  TextureState textureZero;
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_TEXC_H__
