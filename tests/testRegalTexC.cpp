/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc
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

#include "gtest/gtest.h"

#include <GL/Regal.h>

#include "RegalTexC.h"

namespace {

using namespace Regal::Emu;

// ====================================
// Emu::ConvertedBuffer
// ====================================

TEST( RegalTexC, RegalConvertedBuffer ) {
  PixelStorageStateGLES20 pss;
  pss.Reset();

  uint16_t orig[] = {0xf800, 0x07c0, 0x003e, 0x0001, 0x5555, 0xaaaa};

  ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_BYTE );
  ASSERT_TRUE( buffer.ConvertFrom ( 2, 2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );

  ASSERT_EQ( 16u, buffer.targetBuffer_.size() );

  const uint8_t* converted = static_cast<const uint8_t*>( buffer.Get() );

  EXPECT_EQ( 0xffu, converted[  0 ] );
  EXPECT_EQ( 0x00u, converted[  1 ] );
  EXPECT_EQ( 0x00u, converted[  2 ] );

  EXPECT_EQ( 0x00u, converted[  3 ] );
  EXPECT_EQ( 0xffu, converted[  4 ] );
  EXPECT_EQ( 0x00u, converted[  5 ] );

  EXPECT_EQ( 0x00u, converted[  6 ] );
  EXPECT_EQ( 0x00u, converted[  7 ] );

  EXPECT_EQ( 0x00u, converted[  8 ] );
  EXPECT_EQ( 0x00u, converted[  9 ] );
  EXPECT_EQ( 0xffu, converted[ 10 ] );

  EXPECT_EQ( 0x00u, converted[ 11 ] );
  EXPECT_EQ( 0x00u, converted[ 12 ] );
  EXPECT_EQ( 0x00u, converted[ 13 ] );

  EXPECT_EQ( 0x00u, converted[ 14 ] );
  EXPECT_EQ( 0x00u, converted[ 15 ] );
}

TEST( RegalTexC, RegalConvertedBufferFailsFastOnIdentity ) {
  PixelStorageStateGLES20 pss;
  pss.Reset();
  uint8_t orig[ 12 ] = { 0 };

  {
    ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_BYTE );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, 2, GL_RGB, GL_UNSIGNED_BYTE, orig ) );
  }

  {
    ConvertedBuffer buffer( pss, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1 );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, 2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }
}

TEST( RegalTexC, RegalConvertedBufferFailsFastOnError ) {
  PixelStorageStateGLES20 pss;
  pss.Reset();
  uint8_t orig[ 12 ] = { 0 };

  // Bad width
  {
    ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_BYTE );
    ASSERT_FALSE( buffer.ConvertFrom ( -2, 2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }

  // Bad height
  {
    ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_BYTE );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, -2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }

  // Bad target format
  {
    ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_SHORT_5_5_5_1 );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, 2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }

  // Bad source format
  {
    ConvertedBuffer buffer( pss, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1 );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, 2, GL_RGB, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }

  // Bad alignment
  {
    pss.alignment = 123;
    ConvertedBuffer buffer( pss, GL_RGB, GL_UNSIGNED_BYTE );
    ASSERT_FALSE( buffer.ConvertFrom ( 2, 2, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, orig ) );
  }
}

// ====================================
// TextureLevelState
// ====================================

TEST( RegalTexC, TextureLevelState ) {
  TextureLevelState state;

  // The default format and type of an TextureLevel should be GL_RGBA and
  // GL_UNSIGNED_BYTE.
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), state.format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), state.type );

  // It should be possible to set TextureLevelStates to any other format/type
  // combination.
  state = TextureLevelState( GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 );
  EXPECT_EQ( static_cast<GLenum>( GL_BGRA ), state.format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_5_5_1 ), state.type );
}

// ====================================
// TextureState
// ====================================

TEST( RegalTexC, TextureStateBasicBinding ) {
  // Verify the texture state under a series of bind and unbind calls.

  TextureUnitState textureUnitOne;
  TextureUnitState textureUnitTwo;
  TextureState texture;

  // Verify the initial TextureState we care about.
  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );

  // Each call to texture.Bind() correctly records the textureUnit pointer
  // passed. Duplicate calls mean duplicate references.
  texture.Bind( &textureUnitOne );

  ASSERT_EQ( 1u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );

  texture.Bind( &textureUnitOne );

  ASSERT_EQ( 2u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 1 ] );

  texture.Bind( &textureUnitTwo );

  ASSERT_EQ( 3u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 1 ] );
  EXPECT_EQ( &textureUnitTwo, texture.boundTextureUnits[ 2 ] );

  // Calling TextureState::Unbind() with a pointer not being referenced does
  // nothing.
  texture.Unbind( NULL );

  ASSERT_EQ( 3u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 1 ] );
  EXPECT_EQ( &textureUnitTwo, texture.boundTextureUnits[ 2 ] );

  // Each call to TextureState::Unbind() removes a single reference.
  texture.Unbind( &textureUnitOne );

  ASSERT_EQ( 2u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );
  EXPECT_EQ( &textureUnitTwo, texture.boundTextureUnits[ 1 ] );

  texture.Unbind( &textureUnitTwo );

  ASSERT_EQ( 1u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnitOne, texture.boundTextureUnits[ 0 ] );

  texture.Unbind( &textureUnitOne );

  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );

  // Calling TextureState::Unbind() with a pointer not being referenced does
  // nothing.
  texture.Unbind( &textureUnitOne );

  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );
}

TEST( RegalTexC, TextureStateUnbindAll ) {
  // Verify the interaction between TextureState and TextureUnitState.
  TextureUnitState textureUnit;
  TextureState texture;

  // Verify the initial TextureState we care about.
  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );

  // Binding a texture one or more times to a texture unit adds an equivalent
  // number of references int the texture state to the unit.
  textureUnit.Bind( GL_TEXTURE_2D, &texture );
  textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &texture );
  textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &texture );

  ASSERT_EQ( 3u, texture.boundTextureUnits.size() );
  EXPECT_EQ( &textureUnit, texture.boundTextureUnits[ 0 ] );
  EXPECT_EQ( &textureUnit, texture.boundTextureUnits[ 1 ] );
  EXPECT_EQ( &textureUnit, texture.boundTextureUnits[ 2 ] );

  // Calling TextureState::UnbindAll() clears all references.
  texture.UnbindAll();

  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );
}

TEST( RegalTexC, UnbindOnTextureStateDestructor ) {
  // The TextureState destructor should clear all bindings.

  TextureUnitState textureUnit;

  {
    TextureState texture;
    textureUnit.Bind( GL_TEXTURE_2D, &texture );
    ASSERT_EQ( 1u, texture.boundTextureUnits.size() );
  }

  // NB: Cast needed otherwise wrong type inferred for the first argument.
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
}

TEST( RegalTexC, TextureStateFormatTracked ) {
  TextureState texture;
  GLenum format;
  GLenum type;

  // By default, textures should be GL_RGBA, GL_UNSIGNED_BYTE
  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  texture.GetFormatAndType( 1, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  texture.GetFormatAndType( 2, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  // If you set the format of a specific level....
  texture.SetFormatAndType( 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 );

  // Only that level is modified.
  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGB ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_6_5 ), type );

  texture.GetFormatAndType( 1, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  texture.GetFormatAndType( 2, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  // Mipmap level formats and types are independent.
  texture.SetFormatAndType( 1, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 );

  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGB ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_6_5 ), type );

  texture.GetFormatAndType( 1, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_BGRA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_5_5_1 ), type );

  texture.GetFormatAndType( 2, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  // Generating Mipmaps resets all formats and types to that of the base level.
  texture.SimulateComputeMipMaps();

  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGB ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_6_5 ), type );

  texture.GetFormatAndType( 1, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGB ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_6_5 ), type );

  texture.GetFormatAndType( 2, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGB ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_6_5 ), type );

  // Which can be subsequently modified again.
  texture.SetFormatAndType( 0, GL_RGBA, GL_UNSIGNED_BYTE );

  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_RGBA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), type );

  // And again.
  texture.SetFormatAndType( 0, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1 );

  texture.GetFormatAndType( 0, &format, &type );
  EXPECT_EQ( static_cast<GLenum>( GL_BGRA ), format );
  EXPECT_EQ( static_cast<GLenum>( GL_UNSIGNED_SHORT_5_5_5_1 ), type );

  // One more test. From a clean state, set and then get a dummy format, and
  // ensure it matches.
  TextureState textureTwo;
  textureTwo.SetFormatAndType( 13, 17, 19 );
  textureTwo.GetFormatAndType( 13, &format, &type );
  EXPECT_EQ( 17u, format );
  EXPECT_EQ( 19u, type );
}

// ====================================
// TextureUnitState
// ====================================

TEST( RegalTexC, TextureUnitStateBasic ) {
  // Verify the texture unit state under a series of calls to bind and unbind
  // a single texture to various targets.

  TextureUnitState textureUnit;
  TextureState texture;

  // Verify the initial freshly constructed state.
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );

  // Calling TextureUnitState::Bind() correctly records the reference to the
  // texture unit with the passed texture for the indicated target
  textureUnit.Bind( GL_TEXTURE_2D, &texture );

  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( 1u, texture.boundTextureUnits.size() );

  // Each texture unit tracks each target binding separately.
  textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &texture );

  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, texture.boundTextureUnits.size() );

  // Binding a texture to a target already bound does nothing.
  textureUnit.Bind( GL_TEXTURE_2D, &texture );

  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, texture.boundTextureUnits.size() );

  // A texture can be bound to all GLES2 supported targets
  textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &texture );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 3u, texture.boundTextureUnits.size() );

  // Binding to other targets is silently ignored.
  textureUnit.Bind( GL_TEXTURE_1D, &texture );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 3u, texture.boundTextureUnits.size() );

  // Getting the binding of an unsupported target returns NULL.
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_1D ) );

  // Binding NULL to a texture unit target unbinds the texture already
  // associated with that target.
  textureUnit.Bind( GL_TEXTURE_2D, NULL );

  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
  EXPECT_EQ( &texture, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, texture.boundTextureUnits.size() );
}

TEST( RegalTexC, TextureUnitStateBindAllTargets ) {
  // Verify the texture unit state under a series of calls to bind textures to
  // each/all supported targets.

  TextureUnitState textureUnit;
  TextureState textureOne;
  TextureState textureTwo;
  TextureState textureThree;

  // Verify the initial freshly constructed state.
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 0u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 0u, textureTwo.boundTextureUnits.size() );
  EXPECT_EQ( 0u, textureThree.boundTextureUnits.size() );

  // Each texture unit tracks each target binding separately.
  textureUnit.Bind( GL_TEXTURE_2D, &textureOne );
  textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &textureTwo );
  textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &textureThree );

  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &textureTwo, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &textureThree, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 1u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureTwo.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureThree.boundTextureUnits.size() );
}

TEST( RegalTexC, TextureUnitStateUnbind ) {
  // Verify the texture unit state under a series of calls to bind and unbind
  // multiple textures.

  TextureUnitState textureUnit;
  TextureState textureOne;
  TextureState textureTwo;

  // Verify the initial freshly constructed state.
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 0u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 0u, textureTwo.boundTextureUnits.size() );

  textureUnit.Bind( GL_TEXTURE_2D, &textureOne );
  textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &textureOne );
  textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &textureTwo );

  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &textureTwo, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureTwo.boundTextureUnits.size() );

  // Calling Unbind() with a NULL pointer does nothing.
  textureUnit.Unbind( NULL );

  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &textureTwo, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureTwo.boundTextureUnits.size() );

  // Calling Unbind() with an texture that is not bound does nothing.
  {
    TextureState notBound;
    textureUnit.Unbind( &notBound );
  }

  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( &textureOne, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &textureTwo, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 2u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureTwo.boundTextureUnits.size() );

  // Calling Unbind with a specific texture unbinds just that texture from all
  // targets it is bound to.
  textureUnit.Unbind( &textureOne );

  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D )  );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &textureTwo, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 0u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 1u, textureTwo.boundTextureUnits.size() );

  // Calling unbind will clear out all supported targets.
  textureUnit.Unbind( &textureTwo );

  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D )  );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
  EXPECT_EQ( 0u, textureOne.boundTextureUnits.size() );
  EXPECT_EQ( 0u, textureTwo.boundTextureUnits.size() );
}

TEST( RegalTexC, TextureUnitStateUnbindAll ) {
  // Calling TextureUnitState::UnbindAll() clears all bindings.

  TextureUnitState textureUnit;
  TextureState texture;

  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );

  textureUnit.Bind( GL_TEXTURE_2D, &texture );
  textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &texture );
  textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &texture );

  textureUnit.UnbindAll();

  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, textureUnit.GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
}

TEST( RegalTexC, TextureUnitStateUnbindAllInDestructor ) {
  // When a TextureUnitState instance is destroyed, it must clear all textures
  // bound to it.

  TextureState texture;

  {
    TextureUnitState textureUnit;

    textureUnit.Bind( GL_TEXTURE_2D, &texture );
    textureUnit.Bind( GL_TEXTURE_CUBE_MAP, &texture );
    textureUnit.Bind( GL_TEXTURE_RECTANGLE_ARB, &texture );
  }

  EXPECT_EQ( 0u, texture.boundTextureUnits.size() );
}

// ====================================
// Regal::Emu::TexC
// ====================================

TEST ( RegalTexC, ResetUnbindsTextureUnits ) {
  TexC texc;

  TextureState texture;
  texc.textureUnitArrayState[ 0 ].Bind( GL_TEXTURE_2D, &texture );
  texc.textureUnitArrayState[ 0 ].Bind( GL_TEXTURE_CUBE_MAP, &texture );
  texc.textureUnitArrayState[ 0 ].Bind( GL_TEXTURE_RECTANGLE_ARB, &texture );

  texc.Reset_();

  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 0 ].GetBinding( GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 0 ].GetBinding( GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 0 ].GetBinding( GL_TEXTURE_RECTANGLE_ARB ) );
}

TEST ( RegalTexC, GetBoundTexture ) {
  // Verify that the internal TexC::GetBoundTexture_() behaves as expected, as
  // it is used by other calls.

  TexC texc;
  TextureState boundToCubemapUnit0;
  TextureState boundToCubemapUnitMax;

  // Setup
  texc.textureUnitArrayState[ 0 ].Bind( GL_TEXTURE_CUBE_MAP, &boundToCubemapUnit0 );
  texc.textureUnitArrayState[ REGAL_EMU_MAX_TEXTURE_UNITS - 1 ].Bind( GL_TEXTURE_CUBE_MAP, &boundToCubemapUnitMax );

  // Test expectations.
  EXPECT_EQ( &texc.textureZero, texc.GetBoundTexture_( GL_TEXTURE0, GL_TEXTURE_2D ) );
  EXPECT_EQ( &boundToCubemapUnit0, texc.GetBoundTexture_( GL_TEXTURE0, GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( &boundToCubemapUnitMax, texc.GetBoundTexture_( GL_TEXTURE0 + REGAL_EMU_MAX_TEXTURE_UNITS - 1, GL_TEXTURE_CUBE_MAP ) );
  EXPECT_EQ( NULL, texc.GetBoundTexture_( GL_TEXTURE0 + REGAL_EMU_MAX_TEXTURE_UNITS, GL_TEXTURE_2D ) );
  EXPECT_EQ( NULL, texc.GetBoundTexture_( GL_TEXTURE0 - 1, GL_TEXTURE_2D ) );
}

TEST ( RegalTexC, GetTexture ) {
  // Verify that the internal TexC::GetTexture_() behaves as expected, as
  // it is used by other calls.

  TexC texc;

  // Getting texture #0 should always return the special textureZero
  EXPECT_EQ( &texc.textureZero, texc.GetTexture_( 0 ) );

  // Getting a texture by an index not already known should bring a new
  // textureState into existence to shadow that textures state.
  TextureState* demandCreatedTexture = texc.GetTexture_( 1 );
  EXPECT_EQ( demandCreatedTexture, &texc.mapTextureToTextureState[ 1 ] );

  // Getting a texture by an index that is known should just return that
  // texture.
  TextureState* knownTexture = &texc.mapTextureToTextureState[ 123 ];
  EXPECT_EQ( knownTexture, texc.GetTexture_( 123 ) );
}

TEST ( RegalTexC, PixelStoreStateShadowing ) {
  TexC texc;

  // Verify that the initial state is as expected.
  EXPECT_EQ( 4, texc.unpackPSS.alignment );

  // Reset_() should reset the pixel store state to an expected set of values.
  texc.unpackPSS.alignment = 123;
  texc.Reset_();
  EXPECT_EQ( 4, texc.unpackPSS.alignment );

  // For values back by integer storage, the value should be rounded to the
  // nearest integer.

  EXPECT_EQ( 4, texc.unpackPSS.alignment );
  texc.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 1 );
  EXPECT_EQ( 1, texc.unpackPSS.alignment );

  // Unexpected parameters are ignored.
  texc.ShadowPixelStore( GL_UNPACK_IMAGE_HEIGHT, 123 );
  EXPECT_EQ( 1, texc.unpackPSS.alignment );
}

TEST ( RegalTexC, GetFormatAndType ) {
  TexC texc;
  GLenum format;
  GLenum type;

  // Force special texture zero to a known state.
  texc.textureZero.SetFormatAndType( 0, 100, 101 );

  // Force bind a special texture to texture unit 1 in a known state.
  TextureState boundToUnit1;
  boundToUnit1.SetFormatAndType( 0, 102, 103 );
  texc.textureUnitArrayState[ 1 ].Bind( GL_TEXTURE_2D, &boundToUnit1 );

  // Get the format of the current texture unit. This should be the special
  // texture zero, with the format we just forced above.
  texc.GetFormatAndType( GL_TEXTURE_2D, 0, &format, &type );
  EXPECT_EQ( 100u, format );
  EXPECT_EQ( 101u, type );

  // Tell TexC to assume texture unit 1 is the current texture unit, and get
  // the format of the texture in it. This will be the format of texture we
  // forced into that unit above.
  texc.ShadowActiveTexture( GL_TEXTURE1 );
  texc.GetFormatAndType( GL_TEXTURE_2D, 0, &format, &type );
  EXPECT_EQ( 102u, format );
  EXPECT_EQ( 103u, type );
}

TEST ( RegalTexC, ShadowTexImage2D ) {
  // Test TexC::ShadowTexImage2D (2x overloaded)
  // Note for simplicity we use TexC::GetFormatAndType for verification.

  TexC texc;
  TextureState boundTextureState;
  GLenum format;
  GLenum type;

  // Set the current texture unit away from default.
  texc.currentTextureUnit = GL_TEXTURE1;

  // Test the first overload, affecting textureZero.
  // Setting the format/type and then getting it should match.
  texc.ShadowTexImage2D( GL_TEXTURE_2D, 11, 17, 23 );
  texc.GetFormatAndType( GL_TEXTURE_2D, 11, &format, &type );
  EXPECT_EQ( 17u, format );
  EXPECT_EQ( 23u, type );

  // Force bind a texture
  texc.textureUnitArrayState[ 1 ].Bind( GL_TEXTURE_2D, &boundTextureState );

  // Test the first overload, affecting the bound texture.
  // Setting the format/type and then getting it should match.
  texc.ShadowTexImage2D( GL_TEXTURE_2D, 11, 29, 31 );
  boundTextureState.GetFormatAndType( 11, &format, &type );
  EXPECT_EQ( 29u, format );
  EXPECT_EQ( 31u, type );

  // If the texture unit is out of range, the call silently is ignored.
  texc.currentTextureUnit = GL_TEXTURE0 - 1;
  texc.ShadowTexImage2D( GL_TEXTURE_2D, 15, 0, 0 );
}

TEST ( RegalTexC, ShadowGenTextures ) {
  // Test TexC::ShadowGenTextures

  TexC texc;

  GLuint textures[ 4 ] = { 11, 13, 17, 23 };
  texc.ShadowGenTextures( 4, textures );

  // We should now have entries for those four textures.
  EXPECT_EQ( 4u, texc.mapTextureToTextureState.size() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 11 ) != texc.mapTextureToTextureState.end() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 13 ) != texc.mapTextureToTextureState.end() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 17 ) != texc.mapTextureToTextureState.end() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 23 ) != texc.mapTextureToTextureState.end() );

  // We only test one here, but each of the textures should be set to a
  // default state.
  TextureState* texture = &texc.mapTextureToTextureState[ 11 ];
  EXPECT_EQ( 0u, texture->boundTextureUnits.size() );
  EXPECT_EQ( 0u, texture->textureLevelState.size() );

  // Pollute the state, so that the next part of this test correctly verifies
  // things get reset.
  texture->SetFormatAndType( 0, 0, 0 );
  EXPECT_EQ( 1u, texture->textureLevelState.size() );

  // Try to create texture zero, and recreate texture 11.
  GLuint textures2[ 2 ] = { 0, 11 };
  texc.ShadowGenTextures( 2, textures2 );

  EXPECT_EQ( 4u, texc.mapTextureToTextureState.size() );

  // Texture 11 should have been reset to a default sate.
  texture = &texc.mapTextureToTextureState[ 11 ];
  EXPECT_EQ( 0u, texture->boundTextureUnits.size() );
  EXPECT_EQ( 0u, texture->textureLevelState.size() );

  // We should not have created texture zero, as it is special.
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 0 ) == texc.mapTextureToTextureState.end() );

  // Tricky final function verification. No matter how many textures we track,
  // pointers to textures allocated earlier should always be valid.
  texture = &texc.mapTextureToTextureState[ 11 ];
  for ( GLuint i = 0; i < 128; ++i ) {
    GLuint t = 1000 + i;
    texc.ShadowGenTextures( 1, &t );
  }
  EXPECT_EQ( texture, &texc.mapTextureToTextureState[ 11 ] );
}

TEST ( RegalTexC, ShadowDeleteTextures ) {
  // Test TexC::ShadowDeleteTextures

  TexC texc;

  // Setup, and setup verification.
  texc.mapTextureToTextureState[ 0 ] = TextureState();
  texc.textureUnitArrayState[ 5 ].Bind( GL_TEXTURE_2D, &texc.mapTextureToTextureState[ 11 ] );
  EXPECT_EQ( 2u, texc.mapTextureToTextureState.size() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 0 ) != texc.mapTextureToTextureState.end() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 13 ) == texc.mapTextureToTextureState.end() );

  GLuint textures[ 3 ] = { 0, 11, 13 };
  texc.ShadowDeleteTextures( 3, textures );

  // We should only have one texture, texture index zero [the implementation
  // should ignore requests to delete it].
  EXPECT_EQ( 1u, texc.mapTextureToTextureState.size() );
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 0 ) != texc.mapTextureToTextureState.end() );

  // Texture 11 should no longer be known or bound.
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 11 ) == texc.mapTextureToTextureState.end() );
  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 5 ].GetBinding( GL_TEXTURE_2D ) );

  // Texture 13 was not known, and should still be not known.
  EXPECT_TRUE( texc.mapTextureToTextureState.find( 13 ) == texc.mapTextureToTextureState.end() );
}

TEST ( RegalTexC, ShadowActiveTexture ) {
  // Test TexC::ShadowActiveTexture

  TexC texc;

  // Verify initial state
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE0 ), texc.currentTextureUnit );

  // Each call should just simply change currentTextureUnit.
  texc.ShadowActiveTexture( GL_TEXTURE1 );
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE1 ), texc.currentTextureUnit );

  // No range checking is done.
  texc.ShadowActiveTexture( GL_TEXTURE0 - 1 );
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE0 - 1 ), texc.currentTextureUnit );
}

TEST ( RegalTexC, ShadowBindTexture ) {
  // Test TexC::ShadowBindTexture

  TexC texc;

  // Verify initial state.
  EXPECT_TRUE( texc.mapTextureToTextureState.find ( 123 ) == texc.mapTextureToTextureState.end() );
  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 1 ].GetBinding( GL_TEXTURE_2D ) );

  // Set the current texture unit to non-default
  texc.currentTextureUnit = GL_TEXTURE1;

  // Binding texture 123 ...
  texc.ShadowBindTexture( GL_TEXTURE_2D, 123 );

  // ... should have created texture 123
  EXPECT_TRUE( texc.mapTextureToTextureState.find ( 123 ) != texc.mapTextureToTextureState.end() );
  TextureState* texture = &texc.mapTextureToTextureState[ 123 ];

  // .. and bound it as the 2D texture on unit 1
  EXPECT_EQ( texture, texc.textureUnitArrayState[ 1 ].GetBinding( GL_TEXTURE_2D ) );

  // Binding texture zero ...
  texc.ShadowBindTexture( GL_TEXTURE_2D, 0 );

  // ... should have unbound the 2D texture on unit 1
  EXPECT_EQ( NULL, texc.textureUnitArrayState[ 1 ].GetBinding( GL_TEXTURE_2D ) );

  // If the texture unit is out of the valid range ...
  texc.currentTextureUnit = GL_TEXTURE0 - 1;

  // The call should silently do nothing.
  texc.ShadowBindTexture( GL_TEXTURE_2D, 123 );
}

TEST ( RegalTexC, ShadowGenerateMipmap ) {
  // Test TexC::ShadowGenerateMipmap

  TexC texc;

  // Setup - Create a texture in mixed format, and bind it to texture unit 1.
  TextureState boundToUnit1;
  boundToUnit1.SetFormatAndType( 0, 11, 13 );
  boundToUnit1.SetFormatAndType( 1, 17, 19 );
  EXPECT_TRUE( boundToUnit1.textureLevelState.find( TextureState::DEFAULT_FORMAT_LEVEL ) == boundToUnit1.textureLevelState.end() );
  texc.textureUnitArrayState[ 1 ].Bind( GL_TEXTURE_2D, &boundToUnit1 );

  // Invoke the function under test to emulate mipmap generation.
  texc.currentTextureUnit = GL_TEXTURE1;
  texc.ShadowGenerateMipmap( GL_TEXTURE_2D );

  // We expect the texture state to indicate a single default format.
  EXPECT_EQ( 1u, boundToUnit1.textureLevelState.size() );
  EXPECT_TRUE( boundToUnit1.textureLevelState.find( TextureState::DEFAULT_FORMAT_LEVEL ) != boundToUnit1.textureLevelState.end() );
}

}  // namespace
