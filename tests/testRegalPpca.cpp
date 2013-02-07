/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2013 Google Inc
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
#include "gmock/gmock.h"

#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalDispatchGMock.h"
#include "RegalPpca.h"

namespace {

using namespace Regal;
using namespace Regal::Emu;

using ::testing::Mock;

template <typename T, size_t N> size_t arraysize( const T( & )[ N ] ) {
  return N;
}

// ====================================
// ClientState::PixelStore::State
// ====================================

TEST( RegalClientStatePixelStore, SizesAndMappings ) {
  using namespace ClientState::PixelStore;
  using ClientState::PixelStore::State;
  using ClientState::PixelStore::PNameToIndex;

  // The arrays for the state data and mapping must match the number of
  // named attributes the state supports.
  ASSERT_EQ( STATE_COUNT, arraysize( static_cast<const State *>( NULL )->data ) );
  ASSERT_EQ( STATE_COUNT, arraysize( State::indexToPName ) );

  // Test the round trip mapping from index to name and back)
  for ( size_t i = 0; i < STATE_COUNT; ++i ) {
    EXPECT_EQ( i, PNameToIndex( State::indexToPName[ i ] ) );
  }

  // Unexpected names should return an invalid index.
  EXPECT_EQ( INVALID_INDEX, PNameToIndex( GL_PACK_SWAP_BYTES - 1 ) );
}

TEST( RegalClientStatePixelStore, BasicOperations ) {
  using ClientState::PixelStore::State;
  using ClientState::PixelStore::PNameToIndex;

  State state;
  state.Reset();

  State other;
  other.Reset();

  // It should be possible to set each attribute in a state to a unique
  // value.
  other.Set( GL_PACK_SWAP_BYTES     , 11 );
  other.Set( GL_PACK_LSB_FIRST      , 12 );
  other.Set( GL_PACK_ROW_LENGTH     , 13 );
  other.Set( GL_PACK_IMAGE_HEIGHT   , 14 );
  other.Set( GL_PACK_SKIP_ROWS      , 15 );
  other.Set( GL_PACK_SKIP_PIXELS    , 16 );
  other.Set( GL_PACK_SKIP_IMAGES    , 17 );
  other.Set( GL_PACK_ALIGNMENT      , 18 );
  other.Set( GL_UNPACK_SWAP_BYTES   , 21 );
  other.Set( GL_UNPACK_LSB_FIRST    , 22 );
  other.Set( GL_UNPACK_ROW_LENGTH   , 23 );
  other.Set( GL_UNPACK_IMAGE_HEIGHT , 24 );
  other.Set( GL_UNPACK_SKIP_ROWS    , 25 );
  other.Set( GL_UNPACK_SKIP_PIXELS  , 26 );
  other.Set( GL_UNPACK_SKIP_IMAGES  , 27 );
  other.Set( GL_UNPACK_ALIGNMENT    , 28 );
  other.pixelPackBufferBinding   = 100;
  other.pixelUnpackBufferBinding = 101;

  // Setting with an invalid name should silenty do nothing
  // This is done here so if we affect the explicitly set state it will be
  // detected soon.
  other.Set( GL_TEXTURE0, 0xdead );

  // Getting with an invalid name should just return zero
  EXPECT_EQ( 0, state.Get( GL_TEXTURE0 ) );

  // Peform a swap, so that it is effectively tested too
  swap( state, other );

  // It should be possible to get the unique value set previously in the
  // swapped state.
  EXPECT_EQ( 11, state.Get( GL_PACK_SWAP_BYTES     ) );
  EXPECT_EQ( 12, state.Get( GL_PACK_LSB_FIRST      ) );
  EXPECT_EQ( 13, state.Get( GL_PACK_ROW_LENGTH     ) );
  EXPECT_EQ( 14, state.Get( GL_PACK_IMAGE_HEIGHT   ) );
  EXPECT_EQ( 15, state.Get( GL_PACK_SKIP_ROWS      ) );
  EXPECT_EQ( 16, state.Get( GL_PACK_SKIP_PIXELS    ) );
  EXPECT_EQ( 17, state.Get( GL_PACK_SKIP_IMAGES    ) );
  EXPECT_EQ( 18, state.Get( GL_PACK_ALIGNMENT      ) );
  EXPECT_EQ( 21, state.Get( GL_UNPACK_SWAP_BYTES   ) );
  EXPECT_EQ( 22, state.Get( GL_UNPACK_LSB_FIRST    ) );
  EXPECT_EQ( 23, state.Get( GL_UNPACK_ROW_LENGTH   ) );
  EXPECT_EQ( 24, state.Get( GL_UNPACK_IMAGE_HEIGHT ) );
  EXPECT_EQ( 25, state.Get( GL_UNPACK_SKIP_ROWS    ) );
  EXPECT_EQ( 26, state.Get( GL_UNPACK_SKIP_PIXELS  ) );
  EXPECT_EQ( 27, state.Get( GL_UNPACK_SKIP_IMAGES  ) );
  EXPECT_EQ( 28, state.Get( GL_UNPACK_ALIGNMENT    ) );

  EXPECT_EQ( 100u, state.pixelPackBufferBinding );
  EXPECT_EQ( 101u, state.pixelUnpackBufferBinding );

  // Verify the expected default state set previously ended up in the swapped
  // state.
  EXPECT_EQ( 0, other.Get( GL_PACK_SWAP_BYTES     ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_LSB_FIRST      ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_ROW_LENGTH     ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_IMAGE_HEIGHT   ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_SKIP_ROWS      ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_SKIP_PIXELS    ) );
  EXPECT_EQ( 0, other.Get( GL_PACK_SKIP_IMAGES    ) );
  EXPECT_EQ( 4, other.Get( GL_PACK_ALIGNMENT      ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_SWAP_BYTES   ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_LSB_FIRST    ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_ROW_LENGTH   ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_IMAGE_HEIGHT ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_SKIP_ROWS    ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_SKIP_PIXELS  ) );
  EXPECT_EQ( 0, other.Get( GL_UNPACK_SKIP_IMAGES  ) );
  EXPECT_EQ( 4, other.Get( GL_UNPACK_ALIGNMENT    ) );

  EXPECT_EQ( 0u, other.pixelPackBufferBinding );
  EXPECT_EQ( 0u, other.pixelUnpackBufferBinding );
}

TEST( RegalClientStatePixelStore, Transition ) {
  using ClientState::PixelStore::State;
  using ClientState::PixelStore::PNameToIndex;
  using ClientState::PixelStore::Transition;

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  State current;
  current.Reset();

  State target;
  target.Reset();

  // Set some of the named attributes, and expect that calls will be made
  // appropriately to the backend to transition to those value.
  target.Set( GL_UNPACK_SKIP_PIXELS, 123 );
  target.Set( GL_UNPACK_ALIGNMENT  , 456 );
  target.pixelPackBufferBinding   = 321;
  target.pixelUnpackBufferBinding = 654;

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_SKIP_PIXELS         , 123 ) );
  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT           , 456 ) );
  EXPECT_CALL( mock, glBindBuffer ( GL_PIXEL_PACK_BUFFER_BINDING  , 321 ) );
  EXPECT_CALL( mock, glBindBuffer ( GL_PIXEL_UNPACK_BUFFER_BINDING, 654 ) );

  // Perform the state transition
  Transition( dt, current, target );
  Mock::VerifyAndClear( &mock );

  // A transition with no differences should make no calls.

  current.Reset();
  target.Reset();

  target.Set( GL_UNPACK_SKIP_PIXELS, 123 );
  target.Set( GL_UNPACK_ALIGNMENT  , 456 );
  target.pixelPackBufferBinding   = 321;
  target.pixelUnpackBufferBinding = 654;

  current.Set( GL_UNPACK_SKIP_PIXELS, 123 );
  current.Set( GL_UNPACK_ALIGNMENT  , 456 );
  current.pixelPackBufferBinding   = 321;
  current.pixelUnpackBufferBinding = 654;

  // Perform the state transition
  Transition( dt, current, target );
  Mock::VerifyAndClear( &mock );
}

// ====================================
// ClientState::VertexArray::Fixed::State
// ====================================

TEST( RegalClientStateVertexArrayFixedState, SizesAndMappings ) {
  using namespace ClientState::VertexArray::Fixed;
  using ClientState::VertexArray::Fixed::State;
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;
  using ClientState::VertexArray::Fixed::IndexedArrayNameToAttribIndex;

  ASSERT_EQ( COUNT_ATTRIBS, arraysize( static_cast<State *>( NULL )->attrib ) );

  // An expected attribute name returns the expected index
  // (note: the full range of names effectively tested elsewhere)
  EXPECT_EQ( BASE_NAMED_ATTRIBS, ArrayNameToAttribIndex( GL_VERTEX_ARRAY ) );
  // For most attributes that do not use it, passing in non-default texture unit makes no difference.
  EXPECT_EQ( BASE_NAMED_ATTRIBS, ArrayNameToAttribIndex( GL_VERTEX_ARRAY, GL_TEXTURE5 ) );
  // An unexpected name should give an invalid index
  EXPECT_EQ( INVALID_ATTRIB_INDEX, ArrayNameToAttribIndex( GL_PACK_SWAP_BYTES ) );
  EXPECT_EQ( INVALID_ATTRIB_INDEX, ArrayNameToAttribIndex( GL_PACK_SWAP_BYTES, GL_TEXTURE5 ) );
  // Texture coordinate attributes do use the texture unit.
  EXPECT_EQ( BASE_TEXTURE_COORD_ATTRIBS + 5u, ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE5 ) );
  // Passing in a bad texture unit gives an invalid index
  EXPECT_EQ( INVALID_ATTRIB_INDEX, ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE0 - 1 ) );
  EXPECT_EQ( INVALID_ATTRIB_INDEX, ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE0 + COUNT_TEXTURE_COORD_ATTRIBS ) );
  EXPECT_EQ( INVALID_ATTRIB_INDEX, ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, 0 ) );

  // Most attribute names are not indexed, and should return an invalid index.
  EXPECT_EQ( INVALID_ATTRIB_INDEX, IndexedArrayNameToAttribIndex( GL_VERTEX_ARRAY, 0 ) );
  // Unexpected names should also give an invalid index.
  EXPECT_EQ( INVALID_ATTRIB_INDEX, IndexedArrayNameToAttribIndex( GL_PACK_SWAP_BYTES, 0 ) );
  // Texture coordinates are indexed, and should return valid output indices for valid input indices.
  EXPECT_EQ( BASE_TEXTURE_COORD_ATTRIBS, IndexedArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, 0 ) );
  EXPECT_EQ( BASE_TEXTURE_COORD_ATTRIBS + COUNT_TEXTURE_COORD_ATTRIBS - 1, IndexedArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, COUNT_TEXTURE_COORD_ATTRIBS - 1 ) );
  // But even for texture coordinates, input indices outside the valid range return an invalid index.
  EXPECT_EQ( INVALID_ATTRIB_INDEX, IndexedArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, -1 ) );
  EXPECT_EQ( INVALID_ATTRIB_INDEX, IndexedArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, COUNT_TEXTURE_COORD_ATTRIBS ) );
}

TEST( RegalClientStateVertexArrayFixedState, BasicOperations ) {
  using namespace ClientState::VertexArray::Fixed;
  using ClientState::VertexArray::Fixed::State;
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;

  const size_t normalAttribIndex         = ArrayNameToAttribIndex ( GL_NORMAL_ARRAY );
  const size_t secondaryColorAttribIndex = ArrayNameToAttribIndex ( GL_SECONDARY_COLOR_ARRAY );
  const size_t indexAttribIndex          = ArrayNameToAttribIndex ( GL_INDEX_ARRAY );
  const size_t fogCoordAttribIndex       = ArrayNameToAttribIndex ( GL_FOG_COORD_ARRAY );
  const size_t edgeFlagAttribIndex       = ArrayNameToAttribIndex ( GL_EDGE_FLAG_ARRAY );

  State state;
  state.Reset();

  State other;
  other.Reset();

  // Enable a specific attribute array.
  state.SetEnable( 4, true );

  // Set unique data for the array source for all attributes.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    state.SetData( i, i * 10 + 1, i * 10 + 2, i * 10 + 3, i * 10 + 4, i * 10 + 5 );
  }

  // Calls with out-of-bound values should silently return as a no-op.
  // This is done here so if we affect the explicitly set state it will be
  // detected soon.
  state.SetEnable( INVALID_ATTRIB_INDEX, true );
  state.SetEnable( COUNT_ATTRIBS, true );
  state.SetData( INVALID_ATTRIB_INDEX, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead );
  state.SetData( COUNT_ATTRIBS, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead );

  // Peform a swap, so that it is effectively tested too
  swap( state, other );

  // Verify the unique data that was set is in the swapped state.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    const State::Attrib& attrib = other.attrib[ i ];

    if ( i == 4 ) {
      EXPECT_TRUE( attrib.enabled );
    } else {
      EXPECT_FALSE( attrib.enabled );
    }

    EXPECT_EQ( static_cast<GLuint>( i * 10 + 1 ), attrib.source.buffer );
    EXPECT_EQ( static_cast<GLint> ( i * 10 + 2 ), attrib.source.size   );
    EXPECT_EQ( static_cast<GLenum>( i * 10 + 3 ), attrib.source.type   );
    EXPECT_EQ( static_cast<GLint> ( i * 10 + 4 ), attrib.source.stride );
    EXPECT_EQ( static_cast<GLint> ( i * 10 + 5 ), attrib.source.offset );
  }

  // Verify the expected default state set previously ended up in the swapped
  // state.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    const State::Attrib& attrib = state.attrib[ i ];
    EXPECT_FALSE( attrib.enabled ) << "index " << i;
    EXPECT_EQ( 0u, attrib.source.buffer ) << "index " << i;
    if ( ( i == indexAttribIndex ) || ( i == fogCoordAttribIndex ) || ( i == edgeFlagAttribIndex ) ) {
      EXPECT_EQ( 1, attrib.source.size ) << "index " << i;
    } else if ( ( i == secondaryColorAttribIndex ) || ( i == normalAttribIndex ) ) {
      EXPECT_EQ( 3, attrib.source.size ) << "index " << i;
    } else {
      EXPECT_EQ( 4, attrib.source.size ) << "index " << i;
    }
    if ( i == edgeFlagAttribIndex ) {
      EXPECT_EQ( static_cast<GLenum>( GL_BOOL ), attrib.source.type ) << "index " << i;
    } else {
      EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), attrib.source.type ) << "index " << i;
    }
    EXPECT_EQ( 0, attrib.source.stride ) << "index " << i;
    EXPECT_EQ( 0, attrib.source.offset ) << "index " << i;
  }
}

TEST( RegalClientStateVertexArrayFixedState, Transition ) {
  using ClientState::VertexArray::Fixed::State;
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;
  using ClientState::VertexArray::Fixed::IndexedArrayNameToAttribIndex;
  using ClientState::VertexArray::Fixed::Transition;

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  State current;
  current.Reset();

  State target;
  target.Reset();

  // The mapping between named fixed vertex attributes and the function to set
  // their attributes is messy. We explicitly test every one to ensure the
  // correct behavior.

  // The vertex position attribute array uses all the array state we have.
  target.SetEnable ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), true );
  target.SetData   ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), 10, 11, 12, 13, 14 );
  EXPECT_CALL( mock, glEnableClientState( GL_VERTEX_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 10 ) );
  EXPECT_CALL( mock, glVertexPointer    ( 11, 12, 13, reinterpret_cast<const GLvoid*>( 14 ) ) );

  // The vertex normal attribute array does not need the first size argument.
  target.SetEnable( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), 20, 21, 22, 23, 24 );
  EXPECT_CALL( mock, glEnableClientState( GL_NORMAL_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 20 ) );
  EXPECT_CALL( mock, glNormalPointer    ( 22, 23, reinterpret_cast<const GLvoid*>( 24 ) ) );

  // The color attribute array uses all the array state we have.
  target.SetEnable( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), 30, 31, 32, 33, 34 );
  EXPECT_CALL( mock, glEnableClientState( GL_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 30 ) );
  EXPECT_CALL( mock, glColorPointer     ( 31, 32, 33, reinterpret_cast<const GLvoid*>( 34 ) ) );

  // The secondary color attribute array uses all the array state we have.
  target.SetEnable( ArrayNameToAttribIndex( GL_SECONDARY_COLOR_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_SECONDARY_COLOR_ARRAY ), 40, 41, 42, 43, 44 );
  EXPECT_CALL( mock, glEnableClientState    ( GL_SECONDARY_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 40 ) );
  EXPECT_CALL( mock, glSecondaryColorPointer( 41, 42, 43, reinterpret_cast<const GLvoid*>( 44 ) ) );

  // The index attribute array does not need the first size argument.
  target.SetEnable( ArrayNameToAttribIndex( GL_INDEX_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_INDEX_ARRAY ), 50, 51, 52, 53, 54 );
  EXPECT_CALL( mock, glEnableClientState( GL_INDEX_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 50 ) );
  EXPECT_CALL( mock, glIndexPointer     ( 52, 53, reinterpret_cast<const GLvoid*>( 54 ) ) );

  // The edge flag attribute array does not need the size or the type
  // arguments.
  target.SetEnable( ArrayNameToAttribIndex( GL_EDGE_FLAG_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_EDGE_FLAG_ARRAY ), 60, 61, 62, 63, 64 );
  EXPECT_CALL( mock, glEnableClientState( GL_EDGE_FLAG_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 60 ) );
  EXPECT_CALL( mock, glEdgeFlagPointer  ( 63, reinterpret_cast<const GLvoid*>( 64 ) ) );

  // The fog coordiante attribute array does not need the size argument.
  target.SetEnable( ArrayNameToAttribIndex( GL_FOG_COORD_ARRAY ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_FOG_COORD_ARRAY ), 70, 71, 72, 73, 74 );
  EXPECT_CALL( mock, glEnableClientState( GL_FOG_COORD_ARRAY ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 70 ) );
  EXPECT_CALL( mock, glFogCoordPointer  ( 72, 73, reinterpret_cast<const GLvoid*>( 74 ) ) );

  // There are multiple texture coordinate arrays, one for each texture unit.
  // They use all the array state we have.
  // They need a call to glClientActiveTexture to select the texture unit, if
  // not already seleced.
  // Note below we indicate that GL_TEXTURE0 is already selected, so there
  // should be no call to select it again.
  target.SetEnable( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE0 ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE0 ), 80, 81, 82, 83, 84 );
  target.SetEnable( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE5 ), true );
  target.SetData  ( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE5 ), 90, 91, 92, 93, 94 );
  EXPECT_CALL( mock, glEnableClientState  ( GL_TEXTURE_COORD_ARRAY ) ).Times( 2 );
  EXPECT_CALL( mock, glClientActiveTexture( GL_TEXTURE5 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 80 ) );
  EXPECT_CALL( mock, glTexCoordPointer    ( 81, 82, 83, reinterpret_cast<const GLvoid*>( 84 ) ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 90 ) );
  EXPECT_CALL( mock, glTexCoordPointer    ( 91, 92, 93, reinterpret_cast<const GLvoid*>( 94 ) ) );

  // Perform the requested transition
  GLenum clientActiveTexture = GL_TEXTURE0;
  GLuint arrayBufferBinding = 0;
  Transition( dt, current, target, clientActiveTexture, arrayBufferBinding );
  // Verify that we've updated the shadowed selected texture unit as expected.
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE5 ), clientActiveTexture );
  EXPECT_EQ( static_cast<GLuint>( 90 ), arrayBufferBinding );
  // Verify the call expectations, and reset for another test.
  Mock::VerifyAndClear( &mock );
  target.Reset();
  current.Reset();

  // Verify state gets disabled/reset to default from non-default.
  current.SetEnable( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), true );
  current.SetData  ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), 1, 2, 3, 4, 5 );
  current.SetEnable( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE1 ), true );
  current.SetData  ( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, GL_TEXTURE1 ), 80, 81, 82, 83, 84 );
  EXPECT_CALL( mock, glDisableClientState ( GL_VERTEX_ARRAY ) );
  EXPECT_CALL( mock, glVertexPointer      ( 4, GL_FLOAT, 0, NULL ) );
  EXPECT_CALL( mock, glClientActiveTexture( GL_TEXTURE1 ) );
  EXPECT_CALL( mock, glDisableClientState ( GL_TEXTURE_COORD_ARRAY ) );
  EXPECT_CALL( mock, glTexCoordPointer    ( 4, GL_FLOAT, 0, NULL ) );

  // Enabling/Disabling an array is a distinct operation.
  current.SetEnable( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), true );
  EXPECT_CALL( mock, glDisableClientState( GL_NORMAL_ARRAY ) );

  // Setting array data is a distinct operation
  current.SetData  ( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), 30, 31, 32, 33, 34 );
  EXPECT_CALL( mock, glColorPointer     ( 4, GL_FLOAT, 0, NULL ) );

  // Perform the requested transition
  clientActiveTexture = GL_TEXTURE0;
  arrayBufferBinding = 0;
  Transition( dt, current, target, clientActiveTexture, arrayBufferBinding );
  // Verify that we've updated the shadowed selected texture unit as expected.
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE1 ), clientActiveTexture );
  EXPECT_EQ( static_cast<GLuint>( 0 ), arrayBufferBinding );
  // Verify the call expectations, and reset for another test.
  Mock::VerifyAndClear( &mock );
  target.Reset();
  current.Reset();

  // Identical state is a no-op in terms of calls.
  current.SetEnable( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), true );
  current.SetData  ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), 1, 2, 3, 4, 5 );
  target.SetEnable ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), true );
  target.SetData   ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), 1, 2, 3, 4, 5 );

  clientActiveTexture = GL_TEXTURE4;
  arrayBufferBinding = 123;
  Transition( dt, current, target, clientActiveTexture, arrayBufferBinding );
  // Verify that the shadowed selected texture unit is not updated.
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE4 ), clientActiveTexture );
  EXPECT_EQ( static_cast<GLuint>( 123 ), arrayBufferBinding );
}

// ====================================
// ClientState::VertexArray::Generic::State
// ====================================

TEST( RegalClientStateVertexArrayGenericState, Sizes ) {
  using namespace ClientState::VertexArray::Generic;
  using ClientState::VertexArray::Generic::State;

  ASSERT_EQ( COUNT_ATTRIBS, arraysize( static_cast<State *>( NULL )->attrib ) );
  ASSERT_EQ( COUNT_BUFFERS, arraysize( static_cast<State *>( NULL )->buffer ) );
}

TEST( RegalClientStateVertexArrayGenericState, BasicOperations ) {
  using namespace ClientState::VertexArray::Generic;
  using ClientState::VertexArray::Generic::State;

  State state;
  state.Reset();

  State other;
  other.Reset();

  // Enable a specific attribute array.
  state.SetEnable( 4, true );

  // Set unique data for the array source and binding for all attributes.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    state.SetAttribSource( i, i * 10 + 1, i * 10 + 2, i == 1, i == 2, i * 10 + 3 );
    state.SetAttribBinding( i, ( i + 1 ) % COUNT_BUFFERS );
  }

  // Set unique data for the array buffers.
  for ( size_t i = 0; i < COUNT_BUFFERS; ++i ) {
    state.SetBuffer( i, 1000 + i * 10 + 1, 1000 + i * 10 + 2, 1000 + i * 10 + 3 );
    state.SetBufferDivisor( i, 1000 + i * 10 + 4 );
  }

  // Calls with out-of-bound values should silently return as a no-op.
  // This is done here so if we affect the explicitly set state it will be
  // detected soon.
  state.SetEnable( INVALID_INDEX, true );
  state.SetEnable( COUNT_ATTRIBS, true );

  state.SetAttribSource( INVALID_INDEX, 0xdead, 0xdead, false, false, 0xdead );
  state.SetAttribSource( COUNT_ATTRIBS, 0xdead, 0xdead, false, false, 0xdead );

  state.SetAttribBinding( INVALID_INDEX, 5 );
  state.SetAttribBinding( COUNT_ATTRIBS, 5 );
  state.SetAttribBinding( 0, INVALID_INDEX );
  state.SetAttribBinding( 0, COUNT_BUFFERS );

  state.SetBuffer( INVALID_INDEX, 0xdead, 0xdead, 0xdead );
  state.SetBuffer( COUNT_BUFFERS, 0xdead, 0xdead, 0xdead );

  state.SetBufferDivisor( INVALID_INDEX, 0xdead );
  state.SetBufferDivisor( COUNT_BUFFERS, 0xdead );

  // Peform a swap, so that it is effectively tested too.
  swap( state, other );

  // Verify the unique data that was set is in the swapped state.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    const State::Attrib& attrib = other.attrib[ i ];

    if ( i == 4 ) {
      EXPECT_TRUE( attrib.enabled );
    } else {
      EXPECT_FALSE( attrib.enabled );
    }

    EXPECT_EQ( static_cast<GLint> ( i * 10 + 1 ),  attrib.source.size );
    EXPECT_EQ( static_cast<GLenum>( i * 10 + 2 ), attrib.source.type );
    EXPECT_EQ( i * 10 + 3,  attrib.source.relativeOffset );

    if ( i == 1 ) {
      EXPECT_TRUE( attrib.source.normalized );
    } else {
      EXPECT_FALSE( attrib.source.normalized );
    }

    if ( i == 2 ) {
      EXPECT_TRUE( attrib.source.pureInteger );
    } else {
      EXPECT_FALSE( attrib.source.pureInteger );
    }

    EXPECT_EQ( ( i + 1 ) % COUNT_BUFFERS, attrib.bindingIndex );
  }

  for ( size_t i = 0; i < COUNT_BUFFERS; ++i ) {
    const State::Buffer& buffer = other.buffer[ i ];
    EXPECT_EQ( 1000 + i * 10 + 1, buffer.buffer );
    EXPECT_EQ( static_cast<GLint> ( 1000 + i * 10 + 2 ), buffer.offset );
    EXPECT_EQ( static_cast<GLint> ( 1000 + i * 10 + 3 ), buffer.stride );
    EXPECT_EQ( 1000 + i * 10 + 4, buffer.divisor );
  }

  // Verify the expected default state set previously ended up in the swapped
  // state.
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    const State::Attrib& attrib = state.attrib[ i ];

    EXPECT_FALSE( attrib.enabled ) << "index " << i;

    EXPECT_EQ( 4, attrib.source.size ) << "index " << i;
    EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), attrib.source.type ) << "index " << i;
    EXPECT_EQ( 0u, attrib.source.relativeOffset ) << "index " << i;
    EXPECT_FALSE( attrib.source.normalized ) << "index " << i;
    EXPECT_FALSE( attrib.source.pureInteger ) << "index " << i;

    EXPECT_EQ( i, attrib.bindingIndex ) << "index " << i;
  }

  for ( size_t i = 0; i < COUNT_BUFFERS; ++i ) {
    const State::Buffer& buffer = state.buffer[ i ];
    EXPECT_EQ( 0u, buffer.buffer );
    EXPECT_EQ( 0, buffer.offset );
    EXPECT_EQ( 16, buffer.stride );
    EXPECT_EQ( 0u, buffer.divisor );
  }
}

TEST( RegalClientStateVertexArrayGenericState, Transition ) {
  using ClientState::VertexArray::Generic::State;
  using ClientState::VertexArray::Generic::Transition;

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  State current;
  current.Reset();

  State target;
  target.Reset();

  // An attribute array can be enabled.
  target.SetEnable ( 0, true );
  EXPECT_CALL( mock, glEnableVertexAttribArray( 0 ) );

  // An attribute array can be configured.
  target.SetAttribSource( 1, 11, 12, false, false, 13 );
  target.SetAttribSource( 2, 21, 22, true, false, 23 );
  target.SetAttribSource( 3, 31, 32, false, true, 33 );
  EXPECT_CALL( mock, glVertexAttribFormat ( 1, 11, 12, GL_FALSE, 13 ) );
  EXPECT_CALL( mock, glVertexAttribFormat ( 2, 21, 22, GL_TRUE, 23 ) );
  EXPECT_CALL( mock, glVertexAttribIFormat( 3, 31, 32, 33 ) );

  // An attribute buffer can be configured.
  target.SetBuffer( 4, 41, 42, 43 );
  EXPECT_CALL( mock, glBindVertexBuffer( 4, 41, 42, 43 ) );

  // An attribute buffer divisor can be configured.
  target.SetBufferDivisor( 5, 51 );
  EXPECT_CALL( mock, glVertexBindingDivisor( 5, 51 ) );

  target.SetAttribBinding( 6, 7 );
  EXPECT_CALL( mock, glVertexAttribBinding( 6, 7 ) );

  // An attribute can be disabled and all data reset to default.
  current.SetEnable       ( 8, true );
  current.SetAttribSource ( 8, 61, 62, false, false, 63 );
  current.SetBuffer       ( 8, 64, 65, 66 );
  current.SetBufferDivisor( 8, 67 );
  current.SetAttribBinding( 8, 9 );
  EXPECT_CALL( mock, glDisableVertexAttribArray ( 8 ) );
  EXPECT_CALL( mock, glVertexAttribFormat ( 8, 4, GL_FLOAT, GL_FALSE, 0 ) );
  EXPECT_CALL( mock, glBindVertexBuffer( 8, 0, 0, 16 ) );
  EXPECT_CALL( mock, glVertexBindingDivisor( 8, 0 ) );
  EXPECT_CALL( mock, glVertexAttribBinding( 8, 8 ) );

  // Perform the requested state transition.
  Transition( dt, current, target );
  // Verify the call expectations, and reset for another test.
  Mock::VerifyAndClear( &mock );
  target.Reset();
  current.Reset();

  // Identical state is a no-op in terms of calls.
  current.SetEnable       ( 0, true );
  current.SetAttribSource ( 0, 11, 12, false, false, 13 );
  current.SetBuffer       ( 0, 14, 15, 16 );
  current.SetBufferDivisor( 0, 17 );
  current.SetAttribBinding( 0, 1 );
  target.SetEnable        ( 0, true );
  target.SetAttribSource  ( 0, 11, 12, false, false, 13 );
  target.SetBuffer        ( 0, 14, 15, 16 );
  target.SetBufferDivisor ( 0, 17 );
  target.SetAttribBinding ( 0, 1 );

  // Perform the requested state transition.
  Transition( dt, current, target );
}

// ====================================
// Regal::ClientState::VertexArray::State
// ====================================

TEST( RegalClientStateVertexArrayState, ResetAndSwap ) {
  using ClientState::VertexArray::State;

  State state;
  state.Reset();

  State other;
  other.Reset();

  state.clientActiveTexture = 1;
  state.arrayBufferBinding = 2;
  state.drawIndirectBufferBinding = 3;
  state.vertexArrayBinding = 4;
  state.primitiveRestartEnabled = true;
  state.primitiveRestartFixedIndexEnabled = true;
  state.primitiveRestartIndex = 5;
  state.vertexArrayObjectZero.elementArrayBufferBinding = 6;

  swap( state, other );

  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE0 ), state.clientActiveTexture );
  EXPECT_EQ( 0u, state.arrayBufferBinding );
  EXPECT_EQ( 0u, state.drawIndirectBufferBinding );
  EXPECT_EQ( 0u, state.vertexArrayBinding );
  EXPECT_FALSE( state.primitiveRestartEnabled );
  EXPECT_FALSE( state.primitiveRestartFixedIndexEnabled );
  EXPECT_EQ( 0u, state.primitiveRestartIndex );
  EXPECT_EQ( 0u, state.vertexArrayObjectZero.elementArrayBufferBinding );

  EXPECT_EQ( 1u, other.clientActiveTexture );
  EXPECT_EQ( 2u, other.arrayBufferBinding );
  EXPECT_EQ( 3u, other.drawIndirectBufferBinding );
  EXPECT_EQ( 4u, other.vertexArrayBinding );
  EXPECT_TRUE( other.primitiveRestartEnabled );
  EXPECT_TRUE( other.primitiveRestartFixedIndexEnabled );
  EXPECT_EQ( 5u, other.primitiveRestartIndex );
  EXPECT_EQ( 6u, other.vertexArrayObjectZero.elementArrayBufferBinding );
}

TEST( RegalClientStateVertexArrayState, GetVertexArrayObject ) {
  using ClientState::VertexArray::State;

  State state;
  state.Reset();

  EXPECT_EQ( &state.vertexArrayObjectZero, state.GetVertexArrayObject( 0 ) );
  EXPECT_EQ( NULL, state.GetVertexArrayObject( 1 ) );

  state.vertexArrayBinding = 0;
  EXPECT_EQ( &state.vertexArrayObjectZero, state.GetVertexArrayObject() );
  state.vertexArrayBinding = 1;
  EXPECT_EQ( NULL, state.GetVertexArrayObject() );
}

TEST( RegalClientStateVertexArrayState, Transition ) {
  using ClientState::VertexArray::State;
  using ClientState::VertexArray::Transition;

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  State current;
  current.Reset();

  State target;
  target.Reset();

  // Set up a simple non-default state, focusing on state unique this structure.
  target.clientActiveTexture = 1;
  target.arrayBufferBinding = 2;
  target.drawIndirectBufferBinding = 3;
  target.vertexArrayBinding = 4;
  target.primitiveRestartEnabled = true;
  target.primitiveRestartFixedIndexEnabled = true;
  target.primitiveRestartIndex = 5;
  target.vertexArrayObjectZero.elementArrayBufferBinding = 6;

  // Set up expectations.
  EXPECT_CALL( mock, glClientActiveTexture( 1 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 2 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_DRAW_INDIRECT_BUFFER, 3 ) );
  EXPECT_CALL( mock, glBindVertexArray( 4 ) );
  EXPECT_CALL( mock, glEnable( GL_PRIMITIVE_RESTART ) );
  EXPECT_CALL( mock, glEnable( GL_PRIMITIVE_RESTART_FIXED_INDEX ) );
  EXPECT_CALL( mock, glPrimitiveRestartIndex( 5 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 6 ) );

  // Perform the requested state transition.
  Transition( dt, current, target );
  // Verify the call expectations, and reset for another test.
  Mock::VerifyAndClear( &mock );

  // Reverse the transition.
  swap( current, target );

  // Set up expectations.
  EXPECT_CALL( mock, glClientActiveTexture( GL_TEXTURE0 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 ) );
  EXPECT_CALL( mock, glBindVertexArray( 0 ) );
  EXPECT_CALL( mock, glDisable( GL_PRIMITIVE_RESTART ) );
  EXPECT_CALL( mock, glDisable( GL_PRIMITIVE_RESTART_FIXED_INDEX ) );
  EXPECT_CALL( mock, glPrimitiveRestartIndex( 0 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );

  // Perform the requested state transition.
  Transition( dt, current, target );
  // Verify the call expectations, and reset for another test.
  Mock::VerifyAndClear( &mock );
}

// ====================================
// Regal::Ppca
// ====================================

TEST ( RegalPpca, ClientPixelStoreStateShadowing ) {
  using ClientState::PixelStore::PNameToIndex;

  Ppca ppca;
  ppca.Reset();

  // The value stored should be rounded to the nearest integer.

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, 123 );
  EXPECT_EQ( 123, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, 123 );
  EXPECT_EQ( 123, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, 123.1f );
  EXPECT_EQ( 123, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, 123.5f );
  EXPECT_EQ( 124, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, 123.9f );
  EXPECT_EQ( 124, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, -123.1f );
  EXPECT_EQ( -123, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, -123.5f );
  EXPECT_EQ( -123, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );

  ppca.ShadowPixelStore( GL_UNPACK_SKIP_PIXELS, -123.9f );
  EXPECT_EQ( -124, ppca.pss.Get( GL_UNPACK_SKIP_PIXELS ) );
}

TEST ( RegalPpca, ClientVertexArrayStateGenericShadowing ) {
  using ClientState::VertexArray::Generic::State;

  Ppca ppca;
  ppca.Reset();

  State& state = ppca.vas.vertexArrayObjectZero.generic;

  // VertexAttribFormat

  state.Reset();
  ppca.ShadowVertexAttribFormat ( 3, 4, GL_FLOAT, GL_TRUE, 123 );

  EXPECT_EQ   ( 4, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ 3 ].source.type );
  EXPECT_TRUE ( state.attrib[ 3 ].source.normalized );
  EXPECT_FALSE( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 123u, state.attrib[ 3 ].source.relativeOffset );

  state.Reset();
  ppca.ShadowVertexAttribFormat ( 3, 4, GL_FLOAT, GL_FALSE, 123 );

  EXPECT_FALSE ( state.attrib[ 3 ].source.normalized );

  state.Reset();
  ppca.ShadowVertexAttribIFormat( 3, 1, GL_INT, 456 );

  EXPECT_EQ   ( 1, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_INT ), state.attrib[ 3 ].source.type );
  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_TRUE ( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 456u, state.attrib[ 3 ].source.relativeOffset );

  state.Reset();
  ppca.ShadowVertexAttribLFormat( 3, 2, GL_DOUBLE, 789 );

  EXPECT_EQ   ( 2, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_DOUBLE ), state.attrib[ 3 ].source.type );
  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_FALSE( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 789u, state.attrib[ 3 ].source.relativeOffset );

  // BindVertexBuffer

  state.Reset();
  ppca.ShadowBindVertexBuffer( 4, 5, 6, 7 );

  EXPECT_EQ   ( 5u, state.buffer[ 4 ].buffer );
  EXPECT_EQ   ( static_cast<GLintptr>( 6 ), state.buffer[ 4 ].offset );
  EXPECT_EQ   ( 7, state.buffer[ 4 ].stride );
  EXPECT_EQ   ( 0u, state.buffer[ 4 ].divisor );

  // VertexAttribBinding

  state.Reset();
  ppca.ShadowVertexAttribBinding( 3, 4 );

  EXPECT_EQ   ( 4u, state.attrib[ 3 ].bindingIndex );

  // VertexAttribPointer

  state.Reset();
  ppca.vas.arrayBufferBinding = 8888;
  ppca.ShadowVertexAttribPointer ( 3, 1, GL_FLOAT,  GL_TRUE, 123, reinterpret_cast<GLvoid *>( 321 ) );

  EXPECT_EQ   ( 1, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ 3 ].source.type );
  EXPECT_TRUE ( state.attrib[ 3 ].source.normalized );
  EXPECT_FALSE( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 0u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 8888u, state.buffer[ 3 ].buffer );
  EXPECT_EQ   ( static_cast<GLintptr>( 321 ), state.buffer[ 3 ].offset );
  EXPECT_EQ   ( 123, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexAttribPointer ( 3, 1, GL_FLOAT,  GL_FALSE, 0, reinterpret_cast<GLvoid *>( 321 ) );

  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_EQ   ( 4, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexIAttribPointer( 3, 2, GL_INT,    456, reinterpret_cast<GLvoid *>( 654 ) );

  EXPECT_EQ   ( 2, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_INT ), state.attrib[ 3 ].source.type );
  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_TRUE ( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 0u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 8888u, state.buffer[ 3 ].buffer );
  EXPECT_EQ   ( 654u, state.buffer[ 3 ].offset );
  EXPECT_EQ   ( 456, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexIAttribPointer( 3, 2, GL_INT, 0, reinterpret_cast<GLvoid *>( 654 ) );

  EXPECT_EQ   ( 8, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexLAttribPointer( 3, 3, GL_DOUBLE, 789, reinterpret_cast<GLvoid *>( 987 ) );

  EXPECT_EQ   ( 3, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_DOUBLE ), state.attrib[ 3 ].source.type );
  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_FALSE( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 0u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 8888u, state.buffer[ 3 ].buffer );
  EXPECT_EQ   ( 987u, state.buffer[ 3 ].offset );
  EXPECT_EQ   ( 789, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexLAttribPointer( 3, 3, GL_DOUBLE, 0, reinterpret_cast<GLvoid *>( 987 ) );

  EXPECT_EQ   ( 24, state.buffer[ 3 ].stride );

  // Enable/DisableVertexAttribArray

  state.Reset();
  ppca.ShadowEnableVertexAttribArray( 3 );

  EXPECT_TRUE( state.attrib [ 3 ].enabled );

  ppca.ShadowDisableVertexAttribArray( 3 );

  EXPECT_FALSE( state.attrib [ 3 ].enabled );

  // VertexBindingDivisor

  state.Reset();
  ppca.ShadowVertexBindingDivisor( 4, 123 );

  EXPECT_EQ( 123u, state.buffer[ 4 ].divisor );

  // VertexAttribDivisor

  state.Reset();
  ppca.ShadowVertexAttribDivisor( 3, 456 );

  EXPECT_EQ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ( 456u, state.buffer[ 3 ].divisor );

  // ShadowVertexArrayVertexAttribOffsetDSA

  state.Reset();
  ppca.ShadowVertexArrayVertexAttribOffsetDSA( 0, 987, 3, 1, GL_FLOAT, GL_TRUE, 123, 321 );

  EXPECT_EQ   ( 1, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ 3 ].source.type );
  EXPECT_TRUE ( state.attrib[ 3 ].source.normalized );
  EXPECT_FALSE( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 0u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 987u, state.buffer[ 3 ].buffer );
  EXPECT_EQ   ( 321u, state.buffer[ 3 ].offset );
  EXPECT_EQ   ( 123, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexArrayVertexAttribOffsetDSA( 0, 987, 3, 1, GL_FLOAT, GL_FALSE, 0, 321 );

  EXPECT_FALSE ( state.attrib[ 3 ].source.normalized );
  EXPECT_EQ   ( 4, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexArrayVertexAttribIOffsetDSA( 0, 987, 3, 2, GL_INT, 456, 654 );

  EXPECT_EQ   ( 2, state.attrib[ 3 ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_INT ), state.attrib[ 3 ].source.type );
  EXPECT_FALSE( state.attrib[ 3 ].source.normalized );
  EXPECT_TRUE ( state.attrib[ 3 ].source.pureInteger );
  EXPECT_EQ   ( 0u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 3u, state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 987u, state.buffer[ 3 ].buffer );
  EXPECT_EQ   ( 654u, state.buffer[ 3 ].offset );
  EXPECT_EQ   ( 456, state.buffer[ 3 ].stride );

  state.Reset();
  ppca.ShadowVertexArrayVertexAttribIOffsetDSA( 0, 987, 3, 2, GL_INT, 0, 654 );

  EXPECT_EQ   ( 8, state.buffer[ 3 ].stride );

  // EnableDisableVertexArrayAttribDSA

  state.Reset();
  ppca.ShadowEnableVertexArrayAttribDSA( 0, 3 );

  EXPECT_TRUE( state.attrib [ 3 ].enabled );

  ppca.ShadowDisableVertexArrayAttribDSA( 0, 3 );

  EXPECT_FALSE( state.attrib [ 3 ].enabled );



  // If the vertex array binding is nonzero, none of these calls should do
  // anything (since we do not actually track internal state for vertex array
  // objects.
  state.Reset();
  state.attrib[ 3 ].source.relativeOffset = 123;
  state.attrib[ 3 ].bindingIndex = 4;
  state.buffer[ 4 ].buffer = 456;
  state.buffer[ 4 ].divisor = 789;

  ppca.vas.vertexArrayBinding = 1;

  ppca.ShadowVertexAttribFormat ( 3, 3, GL_FLOAT, GL_TRUE, 0 );
  ppca.ShadowVertexAttribIFormat( 3, 3, GL_INT, 0 );
  ppca.ShadowVertexAttribLFormat( 3, 3, GL_DOUBLE, 0 );

  ppca.ShadowBindVertexBuffer( 4, 0, 0, 0 );

  ppca.ShadowVertexAttribBinding( 3, 0 );

  ppca.ShadowVertexAttribPointer ( 3, 1, GL_FLOAT,  GL_TRUE, 0, NULL );
  ppca.ShadowVertexIAttribPointer( 3, 2, GL_INT,    0, NULL );
  ppca.ShadowVertexLAttribPointer( 3, 3, GL_DOUBLE, 0, NULL );

  ppca.ShadowVertexBindingDivisor( 4, 0 );

  ppca.ShadowVertexArrayVertexAttribOffsetDSA ( 1, 0, 3, 1, GL_FLOAT, GL_TRUE, 0, 0 );
  ppca.ShadowVertexArrayVertexAttribIOffsetDSA( 1, 0, 3, 2, GL_INT, 0, 0 );

  EXPECT_EQ   ( 123u, state.attrib[ 3 ].source.relativeOffset );
  EXPECT_EQ   ( 4u,   state.attrib[ 3 ].bindingIndex );
  EXPECT_EQ   ( 456u, state.buffer[ 4 ].buffer );
  EXPECT_EQ   ( 789u, state.buffer[ 4 ].divisor );



  state.attrib [ 3 ].enabled = false;
  ppca.ShadowEnableVertexAttribArray( 3 );
  ppca.ShadowEnableVertexArrayAttribDSA( 1, 3 );
  EXPECT_FALSE( state.attrib [ 3 ].enabled );

  state.attrib [ 3 ].enabled = true;
  ppca.ShadowDisableVertexAttribArray( 3 );
  ppca.ShadowDisableVertexArrayAttribDSA( 1, 3 );
  EXPECT_TRUE( state.attrib [ 3 ].enabled );
}

TEST ( RegalPpca, ClientVertexArrayStateFFShadowing ) {
  using namespace ClientState::VertexArray::Fixed;
  using ClientState::VertexArray::Fixed::State;
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;

  Ppca ppca;
  ppca.Reset();

  const size_t colorAttribIndex          = ArrayNameToAttribIndex ( GL_COLOR_ARRAY );
  const size_t edgeFlagAttribIndex       = ArrayNameToAttribIndex ( GL_EDGE_FLAG_ARRAY );
  const size_t fogCoordAttribIndex       = ArrayNameToAttribIndex ( GL_FOG_COORD_ARRAY );
  const size_t indexAttribIndex          = ArrayNameToAttribIndex ( GL_INDEX_ARRAY );
  const size_t normalAttribIndex         = ArrayNameToAttribIndex ( GL_NORMAL_ARRAY );
  const size_t secondaryColorAttribIndex = ArrayNameToAttribIndex ( GL_SECONDARY_COLOR_ARRAY );
  const size_t vertexAttribIndex         = ArrayNameToAttribIndex ( GL_VERTEX_ARRAY );
  const size_t texture0AttribIndex       = ArrayNameToAttribIndex ( GL_TEXTURE_COORD_ARRAY );

  State& state = ppca.vas.vertexArrayObjectZero.fixed;

  ppca.ShadowEnableClientState( GL_COLOR_ARRAY );
  EXPECT_TRUE( state.attrib[ colorAttribIndex ].enabled );

  ppca.ShadowDisableClientState( GL_COLOR_ARRAY );
  EXPECT_FALSE( state.attrib[ colorAttribIndex ].enabled );

  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE0 ), ppca.vas.clientActiveTexture );
  ppca.ShadowClientActiveTexture( GL_TEXTURE2 );
  EXPECT_EQ( static_cast<GLenum>( GL_TEXTURE2 ), ppca.vas.clientActiveTexture );

  ppca.ShadowEnableClientState( GL_TEXTURE_COORD_ARRAY );
  EXPECT_TRUE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowDisableClientState( GL_TEXTURE_COORD_ARRAY );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowEnableVertexArrayDSA( 0, GL_TEXTURE_COORD_ARRAY );
  EXPECT_TRUE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowDisableVertexArrayDSA( 0, GL_TEXTURE_COORD_ARRAY );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowEnableVertexArrayDSA( 1, GL_TEXTURE_COORD_ARRAY );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowDisableVertexArrayDSA( 1, GL_TEXTURE_COORD_ARRAY );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex + 2 ].enabled );

  ppca.ShadowEnableClientStateIndexedDSA( GL_TEXTURE_COORD_ARRAY, 5 );
  EXPECT_TRUE( state.attrib[ texture0AttribIndex + 5 ].enabled );

  ppca.ShadowDisableClientStateIndexedDSA( GL_TEXTURE_COORD_ARRAY, 5 );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex + 5 ].enabled );

  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    state.attrib[ i ].source.buffer = 0;
    state.attrib[ i ].source.size = 0;
    state.attrib[ i ].source.type = 0;
    state.attrib[ i ].source.stride = 0;
  }

  ppca.vas.arrayBufferBinding = 123;

  ppca.ShadowVertexPointer( 4, GL_FLOAT, 1001, NULL );
  EXPECT_EQ( 123u, state.attrib[ vertexAttribIndex ].source.buffer );
  EXPECT_EQ( 4, state.attrib[ vertexAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ vertexAttribIndex ].source.type );
  EXPECT_EQ( 1001, state.attrib[ vertexAttribIndex ].source.stride );

  ppca.ShadowNormalPointer( GL_FLOAT, 1002, NULL );
  EXPECT_EQ( 123u, state.attrib[ normalAttribIndex ].source.buffer );
  EXPECT_EQ( 3, state.attrib[ normalAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ normalAttribIndex ].source.type );
  EXPECT_EQ( 1002, state.attrib[ normalAttribIndex ].source.stride );

  ppca.ShadowColorPointer( 4, GL_FLOAT, 1003, NULL );
  EXPECT_EQ( 123u, state.attrib[ colorAttribIndex ].source.buffer );
  EXPECT_EQ( 4, state.attrib[ colorAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex ].source.type );
  EXPECT_EQ( 1003, state.attrib[ colorAttribIndex ].source.stride );

  ppca.ShadowSecondaryColorPointer( 3, GL_FLOAT, 1004, NULL );
  EXPECT_EQ( 123u, state.attrib[ secondaryColorAttribIndex ].source.buffer );
  EXPECT_EQ( 3, state.attrib[ secondaryColorAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ secondaryColorAttribIndex ].source.type );
  EXPECT_EQ( 1004, state.attrib[ secondaryColorAttribIndex ].source.stride );

  ppca.ShadowIndexPointer( GL_INT, 1005, NULL );
  EXPECT_EQ( 123u, state.attrib[ indexAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ indexAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_INT ), state.attrib[ indexAttribIndex ].source.type );
  EXPECT_EQ( 1005, state.attrib[ indexAttribIndex ].source.stride );

  ppca.ShadowEdgeFlagPointer( 1006, NULL );
  EXPECT_EQ( 123u, state.attrib[ edgeFlagAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ edgeFlagAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_BOOL ), state.attrib[ edgeFlagAttribIndex ].source.type );
  EXPECT_EQ( 1006, state.attrib[ edgeFlagAttribIndex ].source.stride );

  ppca.ShadowFogCoordPointer( GL_FLOAT, 1007, NULL );
  EXPECT_EQ( 123u, state.attrib[ fogCoordAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ fogCoordAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ fogCoordAttribIndex ].source.type );
  EXPECT_EQ( 1007, state.attrib[ fogCoordAttribIndex ].source.stride );

  ppca.ShadowTexCoordPointer( 2, GL_FLOAT, 1008, NULL );
  EXPECT_EQ( 123u, state.attrib[ texture0AttribIndex + 2 ].source.buffer );
  EXPECT_EQ( 2, state.attrib[ texture0AttribIndex + 2 ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ texture0AttribIndex + 2 ].source.type );
  EXPECT_EQ( 1008, state.attrib[ texture0AttribIndex + 2 ].source.stride );



  ppca.ShadowMultiTexCoordPointerDSA( GL_TEXTURE5, 2, GL_FLOAT, 2005, NULL );
  EXPECT_EQ( 123u, state.attrib[ texture0AttribIndex + 5 ].source.buffer );
  EXPECT_EQ( 2, state.attrib[ texture0AttribIndex + 5 ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ texture0AttribIndex + 5 ].source.type );
  EXPECT_EQ( 2005, state.attrib[ texture0AttribIndex + 5 ].source.stride );



  ppca.ShadowVertexArrayVertexOffsetDSA( 0, 3001, 3, GL_FLOAT, 3002, 0 );
  EXPECT_EQ( 3001u, state.attrib[ vertexAttribIndex ].source.buffer );
  EXPECT_EQ( 3, state.attrib[ vertexAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ vertexAttribIndex ].source.type );
  EXPECT_EQ( 3002, state.attrib[ vertexAttribIndex ].source.stride );

  ppca.ShadowVertexArrayColorOffsetDSA ( 0, 3003, 4, GL_FLOAT, 3004, 0 );
  EXPECT_EQ( 3003u, state.attrib[ colorAttribIndex ].source.buffer );
  EXPECT_EQ( 4, state.attrib[ colorAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex ].source.type );
  EXPECT_EQ( 3004, state.attrib[ colorAttribIndex ].source.stride );

  ppca.ShadowVertexArrayEdgeFlagOffsetDSA ( 0, 3005, 3006, 0 );
  EXPECT_EQ( 3005u, state.attrib[ edgeFlagAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ edgeFlagAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_BOOL ), state.attrib[ edgeFlagAttribIndex ].source.type );
  EXPECT_EQ( 3006, state.attrib[ edgeFlagAttribIndex ].source.stride );

  ppca.ShadowVertexArrayIndexOffsetDSA ( 0, 3007, GL_INT, 3008, 0 );
  EXPECT_EQ( 3007u, state.attrib[ indexAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ indexAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_INT ), state.attrib[ indexAttribIndex ].source.type );
  EXPECT_EQ( 3008, state.attrib[ indexAttribIndex ].source.stride );

  ppca.ShadowVertexArrayNormalOffsetDSA ( 0, 3009, GL_FLOAT, 3010, 0 );
  EXPECT_EQ( 3009u, state.attrib[ normalAttribIndex ].source.buffer );
  EXPECT_EQ( 3, state.attrib[ normalAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ normalAttribIndex ].source.type );
  EXPECT_EQ( 3010, state.attrib[ normalAttribIndex ].source.stride );

  ppca.ShadowVertexArrayTexCoordOffsetDSA( 0, 3011, 2, GL_FLOAT, 3012, 0 );
  EXPECT_EQ( 3011u, state.attrib[ texture0AttribIndex + 2 ].source.buffer );
  EXPECT_EQ( 2, state.attrib[ texture0AttribIndex + 2 ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ texture0AttribIndex + 2 ].source.type );
  EXPECT_EQ( 3012, state.attrib[ texture0AttribIndex + 2 ].source.stride );

  ppca.ShadowVertexArrayMultiTexCoordOffsetDSA( 0, 3013, GL_TEXTURE5, 2, GL_FLOAT, 3014, 0 );
  EXPECT_EQ( 3013u, state.attrib[ texture0AttribIndex + 5 ].source.buffer );
  EXPECT_EQ( 2, state.attrib[ texture0AttribIndex + 5 ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ texture0AttribIndex + 5 ].source.type );
  EXPECT_EQ( 3014, state.attrib[ texture0AttribIndex + 5 ].source.stride );

  ppca.ShadowVertexArrayFogCoordOffsetDSA ( 0, 3015, GL_FLOAT, 3016, 0 );
  EXPECT_EQ( 3015u, state.attrib[ fogCoordAttribIndex ].source.buffer );
  EXPECT_EQ( 1, state.attrib[ fogCoordAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ fogCoordAttribIndex ].source.type );
  EXPECT_EQ( 3016, state.attrib[ fogCoordAttribIndex ].source.stride );

  ppca.ShadowVertexArraySecondaryColorOffsetDSA ( 0, 3017, 3, GL_FLOAT, 3018, 0 );
  EXPECT_EQ( 3017u, state.attrib[ secondaryColorAttribIndex ].source.buffer );
  EXPECT_EQ( 3, state.attrib[ secondaryColorAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ secondaryColorAttribIndex ].source.type );
  EXPECT_EQ( 3018, state.attrib[ secondaryColorAttribIndex ].source.stride );
}

TEST ( RegalPpca, ClientVertexArrayStatePrimitiveRestart ) {
  Ppca ppca;
  ppca.Reset();

  EXPECT_FALSE( ppca.vas.primitiveRestartEnabled );
  EXPECT_FALSE( ppca.vas.primitiveRestartFixedIndexEnabled );
  EXPECT_EQ( 0u, ppca.vas.primitiveRestartIndex );

  ppca.ShadowEnable( GL_PRIMITIVE_RESTART );
  EXPECT_TRUE( ppca.vas.primitiveRestartEnabled );
  ppca.ShadowDisable( GL_PRIMITIVE_RESTART );
  EXPECT_FALSE( ppca.vas.primitiveRestartEnabled );

  ppca.ShadowEnable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
  EXPECT_TRUE( ppca.vas.primitiveRestartFixedIndexEnabled );
  ppca.ShadowDisable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
  EXPECT_FALSE( ppca.vas.primitiveRestartFixedIndexEnabled );

  ppca.ShadowEnable( GL_TEXTURE_GEN_S );
  EXPECT_FALSE( ppca.vas.primitiveRestartEnabled );
  EXPECT_FALSE( ppca.vas.primitiveRestartFixedIndexEnabled );
  ppca.ShadowDisable( GL_TEXTURE_GEN_S );

  ppca.ShadowPrimitiveRestartIndex( ~0u );
  EXPECT_EQ( ~0u, ppca.vas.primitiveRestartIndex );
}

TEST ( RegalPpca, ClientVertexArrayStateBindBuffer ) {
  using ClientState::VertexArray::Generic::State;

  Ppca ppca;
  ppca.Reset();

  ppca.ShadowBindBuffer( GL_ARRAY_BUFFER, 123 );
  ppca.ShadowBindBuffer( GL_DRAW_INDIRECT_BUFFER, 456 );
  ppca.ShadowBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 789 );
  ppca.ShadowBindBuffer( GL_PIXEL_PACK_BUFFER_BINDING, 321 );
  ppca.ShadowBindBuffer( GL_PIXEL_UNPACK_BUFFER_BINDING, 654 );

  EXPECT_EQ( 123u, ppca.vas.arrayBufferBinding );
  EXPECT_EQ( 456u, ppca.vas.drawIndirectBufferBinding );
  EXPECT_EQ( 789u, ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding );
  EXPECT_EQ( 321u, ppca.pss.pixelPackBufferBinding );
  EXPECT_EQ( 654u, ppca.pss.pixelUnpackBufferBinding );

  ppca.Reset();
  ppca.vas.vertexArrayBinding = 1;
  ppca.ShadowBindBuffer( GL_ARRAY_BUFFER, 123 );
  ppca.ShadowBindBuffer( GL_DRAW_INDIRECT_BUFFER, 456 );
  ppca.ShadowBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 789 );
  ppca.ShadowBindBuffer( GL_PIXEL_PACK_BUFFER_BINDING, 321 );
  ppca.ShadowBindBuffer( GL_PIXEL_UNPACK_BUFFER_BINDING, 654 );

  EXPECT_EQ( 123u, ppca.vas.arrayBufferBinding );
  EXPECT_EQ( 456u, ppca.vas.drawIndirectBufferBinding );
  EXPECT_EQ( 0u,   ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding );
  EXPECT_EQ( 321u, ppca.pss.pixelPackBufferBinding );
  EXPECT_EQ( 654u, ppca.pss.pixelUnpackBufferBinding );
}

TEST ( RegalPpca, ClientVertexArrayStateInterleavedArrays ) {
  using namespace ClientState::VertexArray::Fixed;
  using ClientState::VertexArray::Fixed::State;
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;

  Ppca ppca;
  ppca.Reset();

  State& state = ppca.vas.vertexArrayObjectZero.fixed;

  const size_t vertexAttribIndex         = ArrayNameToAttribIndex ( GL_VERTEX_ARRAY );
  const size_t normalAttribIndex         = ArrayNameToAttribIndex ( GL_NORMAL_ARRAY );
  const size_t colorAttribIndex          = ArrayNameToAttribIndex ( GL_COLOR_ARRAY );
  const size_t texture0AttribIndex       = ArrayNameToAttribIndex ( GL_TEXTURE_COORD_ARRAY );

  const size_t edgeFlagAttribIndex       = ArrayNameToAttribIndex ( GL_EDGE_FLAG_ARRAY );
  const size_t fogCoordAttribIndex       = ArrayNameToAttribIndex ( GL_FOG_COORD_ARRAY );
  const size_t indexAttribIndex          = ArrayNameToAttribIndex ( GL_INDEX_ARRAY );
  const size_t secondaryColorAttribIndex = ArrayNameToAttribIndex ( GL_SECONDARY_COLOR_ARRAY );

  // Do a comprehensive test on all settings for GL_T4F_C4F_N3F_V4F

  state.Reset();
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    state.attrib[ i ].enabled       = ( i & 1 ) == 0;
    state.attrib[ i ].source.size   = 987;
    state.attrib[ i ].source.type   = 987;
    state.attrib[ i ].source.stride = 987;
    state.attrib[ i ].source.offset = 987;
  }

  ppca.ShadowClientActiveTexture( GL_TEXTURE5 );
  ppca.ShadowInterleavedArrays( GL_T4F_C4F_N3F_V4F, 0, NULL );
  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE ( state.attrib[ texture0AttribIndex + 5 ].enabled );

  EXPECT_EQ( 4,   state.attrib[ vertexAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ vertexAttribIndex ].source.type );
  EXPECT_EQ( 60,  state.attrib[ vertexAttribIndex ].source.stride );
  EXPECT_EQ( 44u, state.attrib[ vertexAttribIndex ].source.offset );

  EXPECT_EQ( 3,   state.attrib[ normalAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ normalAttribIndex ].source.type );
  EXPECT_EQ( 60,  state.attrib[ normalAttribIndex ].source.stride );
  EXPECT_EQ( 32u, state.attrib[ normalAttribIndex ].source.offset );

  EXPECT_EQ( 4,   state.attrib[ colorAttribIndex ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex ].source.type );
  EXPECT_EQ( 60,  state.attrib[ colorAttribIndex ].source.stride );
  EXPECT_EQ( 16u, state.attrib[ colorAttribIndex ].source.offset );

  EXPECT_EQ( 4,   state.attrib[ texture0AttribIndex + 5 ].source.size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ texture0AttribIndex + 5 ].source.type );
  EXPECT_EQ( 60,  state.attrib[ texture0AttribIndex + 5 ].source.stride );
  EXPECT_EQ( 0u,  state.attrib[ texture0AttribIndex + 5 ].source.offset );

  // The other non-texture coordinate arrays should be disabled

  EXPECT_FALSE( state.attrib[ edgeFlagAttribIndex ].enabled );
  EXPECT_FALSE( state.attrib[ fogCoordAttribIndex ].enabled );
  EXPECT_FALSE( state.attrib[ indexAttribIndex ].enabled );
  EXPECT_FALSE( state.attrib[ secondaryColorAttribIndex ].enabled );

  // The other non-texture coordinate arrays should not otherwise be touched.

  EXPECT_EQ( 987,  state.attrib[ edgeFlagAttribIndex ].source.size );
  EXPECT_EQ( 987u, state.attrib[ edgeFlagAttribIndex ].source.type );
  EXPECT_EQ( 987,  state.attrib[ edgeFlagAttribIndex ].source.stride );
  EXPECT_EQ( 987u, state.attrib[ edgeFlagAttribIndex ].source.offset );

  EXPECT_EQ( 987,  state.attrib[ fogCoordAttribIndex ].source.size );
  EXPECT_EQ( 987u, state.attrib[ fogCoordAttribIndex ].source.type );
  EXPECT_EQ( 987,  state.attrib[ fogCoordAttribIndex ].source.stride );
  EXPECT_EQ( 987u, state.attrib[ fogCoordAttribIndex ].source.offset );

  EXPECT_EQ( 987,  state.attrib[ indexAttribIndex ].source.size );
  EXPECT_EQ( 987u, state.attrib[ indexAttribIndex ].source.type );
  EXPECT_EQ( 987,  state.attrib[ indexAttribIndex ].source.stride );
  EXPECT_EQ( 987u, state.attrib[ indexAttribIndex ].source.offset );

  EXPECT_EQ( 987,  state.attrib[ secondaryColorAttribIndex ].source.size );
  EXPECT_EQ( 987u, state.attrib[ secondaryColorAttribIndex ].source.type );
  EXPECT_EQ( 987,  state.attrib[ secondaryColorAttribIndex ].source.stride );
  EXPECT_EQ( 987u, state.attrib[ secondaryColorAttribIndex ].source.offset );

  // Verify other texture coordinate settings unaffected.

  for ( size_t i = 0; i < COUNT_TEXTURE_COORD_ATTRIBS; ++i ) {
    if ( i == 5 ) continue;
    State::Attrib& attrib = state.attrib[ BASE_TEXTURE_COORD_ATTRIBS + i ];
    if ( ( ( BASE_TEXTURE_COORD_ATTRIBS + i ) & 1 ) == 0 ) {
      EXPECT_TRUE( attrib.enabled ) << "Index " << i;
    } else {
      EXPECT_FALSE( attrib.enabled ) << "Index " << i;
    }
    EXPECT_EQ( 987,  attrib.source.size ) << "Index " << i;
    EXPECT_EQ( 987u, attrib.source.type ) << "Index " << i;
    EXPECT_EQ( 987,  attrib.source.stride ) << "Index " << i;
    EXPECT_EQ( 987u, attrib.source.offset ) << "Index " << i;
  }

  // Ensure if stride is nonzero, it is used as is, and ensure the pointer passed in is used as a base address.
  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T4F_C4F_N3F_V4F, 321, reinterpret_cast<GLvoid *>( 5000 ) );

  EXPECT_EQ( 321,   state.attrib[ vertexAttribIndex ].source.stride );
  EXPECT_EQ( 5044u, state.attrib[ vertexAttribIndex ].source.offset );
  EXPECT_EQ( 321,   state.attrib[ normalAttribIndex ].source.stride );
  EXPECT_EQ( 5032u, state.attrib[ normalAttribIndex ].source.offset );
  EXPECT_EQ( 321,   state.attrib[ colorAttribIndex ].source.stride );
  EXPECT_EQ( 5016u, state.attrib[ colorAttribIndex ].source.offset );
  EXPECT_EQ( 321,   state.attrib[ texture0AttribIndex + 5 ].source.stride );
  EXPECT_EQ( 5000u, state.attrib[ texture0AttribIndex + 5 ].source.offset );

  // Do a quick run through the remaining formats, and do some quick verifications.
  ppca.Reset();

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_V2F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 2,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 8,   state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 0u,  state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 12,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 0u,  state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_C4UB_V2F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 2,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 12,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 4u,  state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_C4UB_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 16,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 4u,  state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_C3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 3,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 24,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 12u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_N3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 24,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 12u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_C4F_N3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_FALSE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 40,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 28u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 16u, state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T2F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 2,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 20,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 8u,  state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T4F_V4F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE ( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 4,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 32,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 16u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T2F_C4UB_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE ( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 2,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_UNSIGNED_BYTE ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 24,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 12u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 8u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T2F_C3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE ( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 3,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 2,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 32,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 20u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 8u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T2F_N3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_FALSE( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 2,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 32,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 20u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 8u,  state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  state.Reset();
  ppca.ShadowInterleavedArrays( GL_T2F_C4F_N3F_V3F, 0, NULL );

  EXPECT_TRUE ( state.attrib[ vertexAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ normalAttribIndex   ].enabled );
  EXPECT_TRUE ( state.attrib[ colorAttribIndex    ].enabled );
  EXPECT_TRUE ( state.attrib[ texture0AttribIndex ].enabled );
  EXPECT_EQ   ( 3,   state.attrib[ vertexAttribIndex   ].source.size );
  EXPECT_EQ   ( 4,   state.attrib[ colorAttribIndex    ].source.size );
  EXPECT_EQ   ( 2,   state.attrib[ texture0AttribIndex ].source.size );
  EXPECT_EQ   ( static_cast<GLenum>( GL_FLOAT ), state.attrib[ colorAttribIndex    ].source.type );
  EXPECT_EQ   ( 48,  state.attrib[ vertexAttribIndex   ].source.stride );
  EXPECT_EQ   ( 36u, state.attrib[ vertexAttribIndex   ].source.offset );
  EXPECT_EQ   ( 24u, state.attrib[ normalAttribIndex   ].source.offset );
  EXPECT_EQ   ( 8u,  state.attrib[ colorAttribIndex    ].source.offset );
  EXPECT_EQ   ( 0u,  state.attrib[ texture0AttribIndex ].source.offset );

  // Pass in an unsupported "format", which should do nothing.

  state.Reset();
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    state.attrib[ i ].enabled       = ( i & 1 ) == 0;
    state.attrib[ i ].source.size   = 987;
    state.attrib[ i ].source.type   = 987;
    state.attrib[ i ].source.stride = 987;
    state.attrib[ i ].source.offset = 987;
  }

  ppca.ShadowInterleavedArrays( GL_RGBA, 0, NULL );

  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    if ( i == 5 ) continue;
    State::Attrib& attrib = state.attrib[ i ];
    if ( ( i & 1 ) == 0 ) {
      EXPECT_TRUE( attrib.enabled ) << "Index " << i;
    } else {
      EXPECT_FALSE( attrib.enabled ) << "Index " << i;
    }
    EXPECT_EQ( 987,  attrib.source.size ) << "Index " << i;
    EXPECT_EQ( 987u, attrib.source.type ) << "Index " << i;
    EXPECT_EQ( 987,  attrib.source.stride ) << "Index " << i;
    EXPECT_EQ( 987u, attrib.source.offset ) << "Index " << i;
  }
}



TEST ( RegalPpca, ShadowDeleteBuffers ) {
  Ppca ppca;
  ppca.Reset();

  GLuint buffers[ 2 ] = { 0, 123 };

  for ( size_t i = 0; i < ClientState::VertexArray::Generic::COUNT_BUFFERS; ++i ) {
    ppca.vas.vertexArrayObjectZero.generic.buffer[ i ].buffer = 123;
  }
  ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding = 123;
  ppca.vas.arrayBufferBinding = 123;
  ppca.vas.drawIndirectBufferBinding = 123;
  ppca.pss.pixelPackBufferBinding = 123;
  ppca.pss.pixelUnpackBufferBinding = 123;

  ppca.ShadowDeleteBuffers( 2, buffers );

  for ( size_t i = 0; i < ClientState::VertexArray::Generic::COUNT_BUFFERS; ++i ) {
    EXPECT_EQ( 0u, ppca.vas.vertexArrayObjectZero.generic.buffer[ i ].buffer ) << "Index " << i;
  }
  EXPECT_EQ( 0u, ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding );
  EXPECT_EQ( 0u, ppca.vas.arrayBufferBinding );
  EXPECT_EQ( 0u, ppca.vas.drawIndirectBufferBinding );
  EXPECT_EQ( 0u, ppca.pss.pixelPackBufferBinding );
  EXPECT_EQ( 0u, ppca.pss.pixelUnpackBufferBinding );

  for ( size_t i = 0; i < ClientState::VertexArray::Generic::COUNT_BUFFERS; ++i ) {
    ppca.vas.vertexArrayObjectZero.generic.buffer[ i ].buffer = 456;
  }
  ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding = 456;
  ppca.vas.arrayBufferBinding = 456;
  ppca.vas.drawIndirectBufferBinding = 456;
  ppca.pss.pixelPackBufferBinding = 456;
  ppca.pss.pixelUnpackBufferBinding = 456;

  ppca.ShadowDeleteBuffers( 2, buffers );

  for ( size_t i = 0; i < ClientState::VertexArray::Generic::COUNT_BUFFERS; ++i ) {
    EXPECT_EQ( 456u, ppca.vas.vertexArrayObjectZero.generic.buffer[ i ].buffer ) << "Index " << i;
  }
  EXPECT_EQ( 456u, ppca.vas.vertexArrayObjectZero.elementArrayBufferBinding );
  EXPECT_EQ( 456u, ppca.vas.arrayBufferBinding );
  EXPECT_EQ( 456u, ppca.vas.drawIndirectBufferBinding );
  EXPECT_EQ( 456u, ppca.pss.pixelPackBufferBinding );
  EXPECT_EQ( 456u, ppca.pss.pixelUnpackBufferBinding );
}

TEST ( RegalPpca, ShadowDeleteVertexArrays ) {
  Ppca ppca;
  ppca.Reset();

  GLuint buffers[ 2 ] = { 0, 123 };

  ppca.vas.vertexArrayBinding = 123;

  ppca.ShadowDeleteVertexArrays( 2, buffers );

  EXPECT_EQ( 0u, ppca.vas.vertexArrayBinding );

  ppca.vas.vertexArrayBinding = 456;

  ppca.ShadowDeleteVertexArrays( 2, buffers );

  EXPECT_EQ( 456u, ppca.vas.vertexArrayBinding );
}



TEST ( RegalPpca, ClientAttribStackPixelState ) {
  using ClientState::PixelStore::PNameToIndex;

  RegalGMockInterface mock;

  Ppca ppca;
  ppca.Reset();

  RegalContext ctx;
  ctx.info = new ContextInfo();
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  EXPECT_EQ( 4, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );

  ppca.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 100 );
  ppca.PushClientAttrib( &ctx, 0 );

  EXPECT_EQ( 100, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );

  ppca.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 101 );
  ppca.PushClientAttrib( &ctx, GL_CLIENT_PIXEL_STORE_BIT );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
  ppca.ClientAttribDefaultDSA( &ctx, GL_CLIENT_PIXEL_STORE_BIT );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 4, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );

  ppca.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 102 );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );

  ppca.PushClientAttribDefaultDSA( &ctx, GL_CLIENT_PIXEL_STORE_BIT );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 4, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 2u, ppca.pixelStoreStateStack_.size() );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 102 ) );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 102, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 101 ) );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );
}

TEST ( RegalPpca, ClientAttribStackVertexState ) {
  using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;

  RegalGMockInterface mock;

  Ppca ppca;
  ppca.Reset();

  RegalContext ctx;
  ctx.info = new ContextInfo();
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  GLint& stride = ppca.vas.vertexArrayObjectZero.fixed.attrib [ ArrayNameToAttribIndex( GL_COLOR_ARRAY ) ].source.stride;

  EXPECT_EQ( 0, stride );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.ShadowColorPointer ( 4, GL_FLOAT, 100, NULL );
  ppca.PushClientAttrib( &ctx, 0 );

  EXPECT_EQ( 100, stride );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.ShadowColorPointer ( 4, GL_FLOAT, 101, NULL );
  ppca.PushClientAttrib( &ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( 101, stride );
  EXPECT_EQ( 1u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glColorPointer( 4, GL_FLOAT, 0, NULL ) );
  ppca.ClientAttribDefaultDSA( &ctx, GL_CLIENT_VERTEX_ARRAY_BIT );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 0, stride );
  EXPECT_EQ( 1u, ppca.vertexArrayStateStack_.size() );

  ppca.ShadowColorPointer ( 4, GL_FLOAT, 102, NULL );

  EXPECT_CALL( mock, glColorPointer( 4, GL_FLOAT, 0, NULL ) );

  ppca.PushClientAttribDefaultDSA( &ctx, GL_CLIENT_VERTEX_ARRAY_BIT );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 0, stride );
  EXPECT_EQ( 2u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glColorPointer( 4, GL_FLOAT, 102, NULL ) );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 102, stride );
  EXPECT_EQ( 1u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glColorPointer( 4, GL_FLOAT, 101, NULL ) );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, stride );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, stride );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 101, stride );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );
}

TEST ( RegalPpca, ClientAttribStackGeneral ) {
  RegalGMockInterface mock;

  Ppca ppca;
  ppca.Reset();

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->core = false;
  ctx.info->gles = false;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  const GLbitfield remainder = ~( GL_CLIENT_PIXEL_STORE_BIT | GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PushClientAttrib( &ctx, GL_CLIENT_PIXEL_STORE_BIT );

  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PushClientAttrib( &ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 1u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );

  ppca.PushClientAttrib( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 2u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 2u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glClientAttribDefaultEXT( remainder ) );

  ppca.ClientAttribDefaultDSA( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 2u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 2u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glClientAttribDefaultEXT( remainder ) );
  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );

  ppca.PushClientAttribDefaultDSA( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 3u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 3u, ppca.vertexArrayStateStack_.size() );

  ctx.info->gles = true;
  ppca.PushClientAttrib( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  ppca.PopClientAttrib( &ctx );
  ctx.info->gles = false;
  Mock::VerifyAndClear( &mock );

  EXPECT_CALL( mock, glPopClientAttrib() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 2u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 2u, ppca.vertexArrayStateStack_.size() );

  EXPECT_CALL( mock, glPopClientAttrib() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 1u, ppca.vertexArrayStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 1u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );

  ppca.PopClientAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 0u, ppca.pixelStoreStateStack_.size() );
  EXPECT_EQ( 0u, ppca.vertexArrayStateStack_.size() );
}

TEST ( RegalPpca, Get ) {
  Ppca ppca;
  ppca.Reset();

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->gles = true;

  GLint resulti[ 1 ] = { 123 };
  GLint64 resulti64[ 1 ] = { 123 };
  GLfloat resultf[ 1 ] = { 123.f };
  GLdouble resultd[ 1 ] = { 123. };
  GLboolean resultb[ 1 ] = { GL_FALSE };

  // First ensure getting an unimplemented value works (does nothing).

  EXPECT_FALSE( ppca.Get( &ctx, GL_FLOAT, resulti ) );
  EXPECT_EQ( 123, resulti[ 0 ] );

  EXPECT_FALSE( ppca.Get( &ctx, GL_FLOAT, resulti64 ) );
  EXPECT_EQ( 123, resulti64[ 0 ] );

  EXPECT_FALSE( ppca.Get( &ctx, GL_FLOAT, resultf ) );
  EXPECT_EQ( 123, resultf[ 0 ] );

  EXPECT_FALSE( ppca.Get( &ctx, GL_FLOAT, resultd ) );
  EXPECT_EQ( 123, resultd[ 0 ] );

  EXPECT_FALSE( ppca.Get( &ctx, GL_FLOAT, resultb ) );
  EXPECT_EQ( GL_FALSE, resultb[ 0 ] );

  // Next verify that getting an implemented value gets the value.

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti ) );
  EXPECT_EQ( REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti64 ) );
  EXPECT_EQ( REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti64[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultf ) );
  EXPECT_EQ( REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultf[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultd ) );
  EXPECT_EQ( REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultd[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultb ) );
  EXPECT_EQ( GL_TRUE, resultb[ 0 ] );

  // If the backend appears to be compatible with the request, the emulation
  // should just defer to the backend.

  ctx.info->gles = ctx.info->core = false;
  EXPECT_FALSE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti ) );
}

}  // namespace
