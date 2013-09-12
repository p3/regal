/*
  Copyright (c) 2011-2012 NVIDIA Corporation
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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <utility>
#include <algorithm>

#include <cmath>

#include "RegalTexC.h"
#include "RegalContext.h"
#include "RegalToken.h"
#include "RegalPixelConversions.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

// ====================================
// PixelStorageState
// ====================================

void
PixelStorageStateGLES20::Reset()
{
  alignment = 4;
}

// ====================================
// ConvertedBuffer
// ====================================

ConvertedBuffer::ConvertedBuffer( const PixelStorageStateGLES20& pss, GLenum targetFormat, GLenum targetType )
: pss_( pss )
, targetFormat_( targetFormat )
, targetType_( targetType )
{
}

bool
ConvertedBuffer::ConvertFrom( GLsizei width, GLsizei height, GLenum sourceFormat, GLenum sourceType, const GLvoid* sourcePixels )
{
  // No conversion necessary. This is the happiest case, and we bail early to
  // pass the call down unmodified.
  if ( ( sourceFormat == targetFormat_ ) && ( sourceType == targetType_ ) ) {
    return false;
  }

  // If any arguments or state is out of spec, fail quickly to pass the call
  // down unmodified, and let the implementation set the error state.
  if ( ( pss_.alignment != 1 ) && ( pss_.alignment != 2 ) && ( pss_.alignment != 4 ) && ( pss_.alignment != 8 ) ) {
    return false;
  }
  if ( ( width < 0 ) || ( height < 0 ) ) {
    return false;
  }

  // Attempt to get an interface to convert to the target format.
  IConversion* targetConverter = GetConversionInterface( targetFormat_, targetType_ );
  if ( targetConverter == NULL ) {
    return false;
  }

  // Attempt to get an interface to convert from the source format.
  IConversion* sourceConverter = GetConversionInterface( sourceFormat, sourceType );
  if ( sourceConverter == NULL ) {
    return false;
  }

  // Compute the size of the buffer needed to store the fully converted, target
  // compatible pixel data.
  const size_t targetPixelSize     = targetConverter->GetPackedPixelByteSize();
  const size_t targetAlign         = 4;
  const size_t targetStride        = ( width * targetPixelSize + ( targetAlign - 1 ) ) & ~( targetAlign - 1 );

  const size_t sourcePixelSize     = sourceConverter->GetPackedPixelByteSize();
  const size_t sourceAlignment     = std::max( sourceConverter->GetPackedPixelAlignmentSize(), static_cast<size_t>( pss_.alignment ) );
  const size_t sourcePixelsPerRow  = width;
  const size_t sourceStride        = ( sourcePixelsPerRow * sourcePixelSize + sourceAlignment - 1 ) & ~( sourceAlignment - 1 );

  std::vector<uint32_t> rowBuffer;
  targetBuffer_ .resize( targetStride * height );
  rowBuffer.resize( width * sizeof( uint32_t ) );

  const uint8_t* sourceRowData = static_cast<const uint8_t*>( sourcePixels );
  uint8_t*       targetRowData = &targetBuffer_[ 0 ];

  // Perform the conversion
  while ( height-- ) {
    sourceConverter->Unpack32( sourceRowData, &rowBuffer[ 0 ], width );
    targetConverter->Pack32( &rowBuffer[ 0 ], targetRowData, width );
    sourceRowData += sourceStride;
    targetRowData += targetStride;
  }

  return true;
}

// ====================================
// TextureState
// ====================================

TextureState::~TextureState()
{
  UnbindAll();
}

void TextureState::Reset()
{
  UnbindAll();
  textureLevelState.clear();
}

void TextureState::Bind( TextureUnitState* unit )
{
  boundTextureUnits.push_back( unit );
}

void TextureState::Unbind( TextureUnitState* unit )
{
  BoundTextureUnits::iterator f = std::find( boundTextureUnits.begin(), boundTextureUnits.end(), unit );
  if ( boundTextureUnits.end() != f ) {
    boundTextureUnits.erase( f );
  }
}

void TextureState::UnbindAll()
{
  BoundTextureUnits tmp;
  tmp.swap( boundTextureUnits );
  for ( BoundTextureUnits::iterator i = tmp.begin(); i != tmp.end(); ++i ) {
    ( *i )->Unbind( this );
  }
}

TextureLevelState& TextureState::GetLevelState_( GLint level )
{
  TextureLevelStateMap::iterator f = textureLevelState.find( level );

  if ( f == textureLevelState.end() ) {
    f = textureLevelState.insert( std::make_pair ( DEFAULT_FORMAT_LEVEL, TextureLevelState() ) ).first;
  }

  return f->second;
}

void TextureState::SetFormatAndType( GLint level, GLenum format, GLenum type )
{
  textureLevelState[ level ] = TextureLevelState( format, type );
}

void TextureState::GetFormatAndType( GLint level, GLenum* format, GLenum* type )
{
  const TextureLevelState& levelState = GetLevelState_( level );
  *format = levelState.format;
  *type = levelState.type;
}

void TextureState::SimulateComputeMipMaps()
{
  TextureLevelStateMap newLevelState;
  newLevelState.insert( std::make_pair ( DEFAULT_FORMAT_LEVEL, GetLevelState_( 0 ) ) );
  newLevelState.swap( textureLevelState );
}

// ====================================
// TextureStateRef
// ====================================

TextureStateRef::TextureStateRef()
: texture_( NULL )
, unit_( NULL )
{
}

TextureStateRef::~TextureStateRef()
{
  Reset( NULL, NULL );
}

void
TextureStateRef::Reset( TextureUnitState* unit, TextureState* texture )
{
  if ( texture_ ) texture_->Unbind( unit_ );
  unit_ = unit;
  texture_ = texture;
  if ( texture_ ) texture_->Bind( unit_ );
}

// ====================================
// TextureUnitState
// ====================================

void
TextureUnitState::UnbindAll()
{
  binding2D_.Reset( this, NULL );
  bindingCubeMap_.Reset( this, NULL );
  bindingRectangleARB_.Reset( this, NULL );
}

void
TextureUnitState::Unbind( TextureState* texture )
{
  if ( texture == NULL ) {
    return;
  }
  if ( binding2D_.IsBound( texture ) ) {
    binding2D_.Reset( this, NULL );
  }
  if ( bindingCubeMap_.IsBound( texture ) ) {
    bindingCubeMap_.Reset( this, NULL );
  }
  if ( bindingRectangleARB_.IsBound( texture ) ) {
    bindingRectangleARB_.Reset( this, NULL );
  }
}

void
TextureUnitState::Bind( GLenum target, TextureState* texture )
{
  switch ( target ) {
    case GL_TEXTURE_2D:
      binding2D_.Reset( this, texture );
      break;

    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      bindingCubeMap_.Reset( this, texture );
      break;

    case GL_TEXTURE_RECTANGLE_ARB:
      bindingRectangleARB_.Reset( this, texture );
      break;
  }
}

TextureState *
TextureUnitState::GetBinding( GLenum target ) const
{
  switch ( target ) {
    case GL_TEXTURE_2D:
      return binding2D_.Get();

    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      return bindingCubeMap_.Get();

    case GL_TEXTURE_RECTANGLE_ARB:
      return bindingRectangleARB_.Get();

    default:
      return NULL;
  }
}

// ====================================
// TexC
// ====================================

TexC::TexC()
{
  Reset_();
}

void
TexC::Init( RegalContext& ctx )
{
  RegalContext* sharingWith = ctx.groupInitializedContext();
  if ( sharingWith != NULL ) {
    mapTextureToTextureState = sharingWith->texc->mapTextureToTextureState;
  }

  Reset_();
}

void
TexC::Reset_()
{
  unpackPSS.Reset();

  currentTextureUnit = 0;
  const size_t n = array_size( textureUnitArrayState );
  for ( size_t i = 0; i < n; ++i )
  {
    RegalAssertArrayIndex( textureUnitArrayState, i );
    textureUnitArrayState[ i ].UnbindAll();
  }
  textureZero = TextureState();
}

TextureState *
TexC::GetBoundTexture_( GLenum texture, GLenum target )
{
  return GetBoundTextureUnit_( static_cast<GLuint>(texture-GL_TEXTURE0), target );
}

TextureState *
TexC::GetBoundTextureUnit_( GLuint textureUnit, GLenum target )
{
  size_t i = static_cast<size_t>(textureUnit);
  const size_t n = array_size( textureUnitArrayState );
  if ( i >= n )
  {
    RegalAssert( i < n );
    return NULL;
  }
  RegalAssertArrayIndex( textureUnitArrayState, i );
  TextureState* ts = textureUnitArrayState[ i ].GetBinding( target );
  return ( ts ? ts : &textureZero );
}

TextureState *
TexC::GetTexture_( GLuint texture )
{
  if ( texture == TEXTURE_ZERO ) {
    return &textureZero;
  }

  MapTextureToTextureState::iterator tfti = mapTextureToTextureState.find( texture );
  if ( tfti == mapTextureToTextureState.end() ) {
    GenTexture_( texture );
    tfti = mapTextureToTextureState.find( texture );
    assert( tfti != mapTextureToTextureState.end() );
  }

  return &tfti->second;
}

void
TexC::GetFormatAndType( GLenum target, GLint level, GLenum* format, GLenum* type )
{
  TextureState* p = GetBoundTextureUnit_( currentTextureUnit, target );
  if (p)
    p->GetFormatAndType( level, format, type );
}

void
TexC::ShadowTexImage2D( GLenum target, GLint level, GLenum format, GLenum type )
{
  TextureState* p = GetBoundTextureUnit_( currentTextureUnit, target );
  if (p)
    p->SetFormatAndType( level, format, type );
}

void
TexC::ShadowPixelStore( GLenum pname, GLint pvalue )
{
  switch ( pname ) {
    case GL_UNPACK_ALIGNMENT:
      unpackPSS.alignment = pvalue;
      break;
    }
}

void TexC::ShadowGenTextures( GLsizei n, GLuint* textures )
{
  while ( n-- ) {
    GenTexture_( *textures++ );
  }
}

void TexC::ShadowDeleteTextures( GLsizei n, const GLuint* textures )
{
  while ( n-- ) {
    DeleteTexture_( *textures++ );
  }
}

void TexC::ShadowActiveTexture( GLenum texture )
{
  if (validTextureEnum(texture))
    currentTextureUnit = texture - GL_TEXTURE0;
}

void TexC::ShadowBindTexture( GLenum target, GLuint texture )
{
  size_t i = currentTextureUnit;
  const size_t n = array_size( textureUnitArrayState );
  if ( n <= i )
    return;

  RegalAssertArrayIndex( textureUnitArrayState, i );
  TextureUnitState& textureUnitState = textureUnitArrayState[ i ];

  if ( texture == TEXTURE_ZERO ) {
    textureUnitState.Bind( target, NULL );
  } else {
    // This is OK only because we store this in a textureUnitState, and we take
    // care to unbind them when textures are deleted.

    textureUnitState.Bind( target, GetTexture_( texture ) );
  }
}

void
TexC::ShadowGenerateMipmap( GLenum target )
{
  TextureState* p = GetBoundTextureUnit_( currentTextureUnit, target );
  if (p)
    p->SimulateComputeMipMaps();
}

void
TexC::GenTexture_( GLuint texture )
{
  if ( texture == 0 ) {
    return;
  }

  mapTextureToTextureState[ texture ].Reset();
}

void
TexC::DeleteTexture_( GLuint texture )
{
  if ( texture == 0 ) {
    return;
  }

  mapTextureToTextureState.erase( texture );
}

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
