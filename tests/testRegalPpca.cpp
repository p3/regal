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

template <typename T, size_t N> size_t arraysize( const T( & )[ N ] ) {
  return N;
}

// ====================================
// PixelStorageState
// ====================================

TEST( Ppca, PixelStorageStateWorks ) {
  PixelStorageState pss;

  ASSERT_EQ( PixelStorageState::STATE_COUNT, arraysize( pss.state ) );
  ASSERT_EQ( arraysize( pss.state ), arraysize( PixelStorageState::indexToPName ) );

  for ( size_t i = 0; i < PixelStorageState::STATE_COUNT; ++i ) {
    EXPECT_EQ( i + 1, PixelStorageState::PNameToIndex( PixelStorageState::indexToPName[ i ] ) );
  }

  EXPECT_EQ( 0u, PixelStorageState::PNameToIndex( GL_PACK_SWAP_BYTES - 1 ) );

  pss.state[ 0 ] = 123;
  EXPECT_EQ( 123, pss.Get( GL_PACK_SWAP_BYTES ) );
  pss.Set( GL_PACK_SWAP_BYTES, 456 );
  EXPECT_EQ( 456, pss.state [ 0 ] );

  pss.Set( GL_PACK_SWAP_BYTES - 1, 789 );
  EXPECT_EQ( 0, pss.Get( GL_PACK_SWAP_BYTES - 1 ) );

  pss.Reset();

  EXPECT_EQ( 0, pss.Get( GL_PACK_SWAP_BYTES     ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_LSB_FIRST      ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_ROW_LENGTH     ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_IMAGE_HEIGHT   ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_SKIP_ROWS      ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_SKIP_PIXELS    ) );
  EXPECT_EQ( 0, pss.Get( GL_PACK_SKIP_IMAGES    ) );
  EXPECT_EQ( 4, pss.Get( GL_PACK_ALIGNMENT      ) );

  EXPECT_EQ( 0, pss.Get( GL_UNPACK_SWAP_BYTES   ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_LSB_FIRST    ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_ROW_LENGTH   ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_IMAGE_HEIGHT ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_SKIP_ROWS    ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_SKIP_PIXELS  ) );
  EXPECT_EQ( 0, pss.Get( GL_UNPACK_SKIP_IMAGES  ) );
  EXPECT_EQ( 4, pss.Get( GL_UNPACK_ALIGNMENT    ) );
}

TEST( Ppca, PixelStorageStateTransition ) {
  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  PixelStorageState current;
  current.Reset();

  PixelStorageState target;
  target.Reset();

  target.Set( GL_UNPACK_SKIP_PIXELS, 123 );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_SKIP_PIXELS, 123 ) );

  PixelStorageState::Transition( dt, current, target );
}

// ====================================
// VertexArrayState
// ====================================

TEST( Ppca, VertexArrayStateWorks ) {
  VertexArrayState vas;

  ASSERT_EQ( VertexArrayState::STATE_COUNT, arraysize( vas.state ) );
  ASSERT_EQ( arraysize( vas.state ), arraysize( VertexArrayState::indexToPName ) );

  for ( size_t i = 0; i < VertexArrayState::STATE_COUNT; ++i ) {
    EXPECT_EQ( i + 1, VertexArrayState::PNameToIndex( VertexArrayState::indexToPName[ i ] ) );
  }

  EXPECT_EQ( 0u, VertexArrayState::PNameToIndex( GL_PACK_SWAP_BYTES - 1 ) );

  vas.SetEnable( GL_VERTEX_ARRAY, false );
  EXPECT_FALSE( vas.state[ 7 ].enabled );
  vas.SetEnable( GL_VERTEX_ARRAY, true );
  EXPECT_TRUE( vas.state[ 7 ].enabled );

  vas.SetData( GL_VERTEX_ARRAY, 1, 2, 3, reinterpret_cast<const GLvoid*>( 4 ) );
  EXPECT_EQ( 1, vas.state[ 7 ].size );
  EXPECT_EQ( 2u, vas.state[ 7 ].type );
  EXPECT_EQ( 3, vas.state[ 7 ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 4 ), vas.state[ 7 ].pointer );

  vas.Reset();

  for ( size_t i = 0; i < VertexArrayState::STATE_COUNT; ++i ) {
    EXPECT_FALSE( vas.state[ i ].enabled ) << "index " << i;
    EXPECT_EQ( 4,        vas.state[ i ].size )    << "index " << i;
    EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), vas.state[ i ].type )    << "index " << i;
    EXPECT_EQ( 0,        vas.state[ i ].stride )  << "index " << i;
    EXPECT_EQ( NULL,     vas.state[ i ].pointer ) << "index " << i;
  }
}

TEST( Ppca, VertexArrayStateTransitionWorks ) {
  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  VertexArrayState current;
  current.Reset();

  VertexArrayState target;
  target.Reset();

  current.SetEnable( GL_COLOR_ARRAY, true );
  current.SetData( GL_COLOR_ARRAY, 1, 2, 3, reinterpret_cast<const GLvoid *>( 4 ) );

  target.SetEnable( GL_EDGE_FLAG_ARRAY, true );
  target.SetData( GL_EDGE_FLAG_ARRAY, 11, 12, 13, reinterpret_cast<const GLvoid *>( 14 ) );

  target.SetEnable( GL_FOG_COORD_ARRAY, true );
  target.SetData( GL_FOG_COORD_ARRAY, 21, 22, 23, NULL );

  target.SetEnable( GL_INDEX_ARRAY, true );
  target.SetData( GL_INDEX_ARRAY, 31, 32, 33, NULL );

  target.SetEnable( GL_NORMAL_ARRAY, true );
  target.SetData( GL_NORMAL_ARRAY, 41, 42, 43, NULL );

  target.SetEnable( GL_SECONDARY_COLOR_ARRAY, true );
  target.SetData( GL_SECONDARY_COLOR_ARRAY, 51, 52, 53, NULL );

  target.SetEnable( GL_TEXTURE_COORD_ARRAY, true );
  target.SetData( GL_TEXTURE_COORD_ARRAY, 61, 62, 63, NULL );

  target.SetEnable( GL_VERTEX_ARRAY, true );
  target.SetData( GL_VERTEX_ARRAY, 71, 72, 73, NULL );

  EXPECT_CALL( mock, glDisableClientState( GL_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_EDGE_FLAG_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_FOG_COORD_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_INDEX_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_NORMAL_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_SECONDARY_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_TEXTURE_COORD_ARRAY ) );
  EXPECT_CALL( mock, glEnableClientState ( GL_VERTEX_ARRAY ) );

  EXPECT_CALL( mock, glColorPointer          ( 4, GL_FLOAT, 0, NULL ) );
  EXPECT_CALL( mock, glEdgeFlagPointer       ( 13, reinterpret_cast<const GLvoid*>( 14 ) ) );
  EXPECT_CALL( mock, glFogCoordPointer       ( 22, 23, NULL ) );
  EXPECT_CALL( mock, glIndexPointer          ( 32, 33, NULL ) );
  EXPECT_CALL( mock, glNormalPointer         ( 42, 43, NULL ) );
  EXPECT_CALL( mock, glSecondaryColorPointer ( 51, 52, 53, NULL ) );
  EXPECT_CALL( mock, glTexCoordPointer       ( 61, 62, 63, NULL ) );
  EXPECT_CALL( mock, glVertexPointer         ( 71, 72, 73, NULL ) );

  VertexArrayState::Transition( dt, current, target );
}

// ====================================
// Ppca
// ====================================

TEST ( Ppca, PixelStoreStateShadowing ) {
  Ppca ppca;

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

TEST ( Ppca, ClientArrayStateShadowing ) {
  Ppca ppca;
  RegalContext ctx;
  ppca.Init( ctx );

  EXPECT_FALSE( ppca.vas.state [ 0 ].enabled );

  ppca.ShadowEnableClientState ( GL_COLOR_ARRAY );

  EXPECT_TRUE( ppca.vas.state [ 0 ].enabled );

  EXPECT_EQ( 4, ppca.vas.state [ 0 ].size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), ppca.vas.state [ 0 ].type );
  EXPECT_EQ( 0, ppca.vas.state [ 0 ].stride );
  EXPECT_EQ( NULL, ppca.vas.state [ 0 ].pointer );

  ppca.ShadowColorPointer ( 1, 2, 3, NULL );

  EXPECT_EQ( 1, ppca.vas.state [ 0 ].size );
  EXPECT_EQ( 2u, ppca.vas.state [ 0 ].type );
  EXPECT_EQ( 3, ppca.vas.state [ 0 ].stride );
  EXPECT_EQ( NULL, ppca.vas.state [ 0 ].pointer );
}

TEST ( Ppca, ClientAttribStackShadowing ) {
  RegalGMockInterface mock;

  Ppca ppca;
  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->core = false;
  ctx.info->gles = false;
  ppca.Init( ctx );

  InitDispatchTableGMock( ctx.dispatcher.emulation );

  const GLbitfield remainder = ~( GL_CLIENT_PIXEL_STORE_BIT | GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( 4, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 100 );

  EXPECT_EQ( 100, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.PushClientAttrib( &ctx, GL_CLIENT_PIXEL_STORE_BIT );

  EXPECT_EQ( 100, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.ShadowPixelStore( GL_UNPACK_ALIGNMENT, 101 );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.PushClientAttrib( &ctx, 0 );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );

  ppca.PushClientAttrib( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  EXPECT_FALSE( ppca.vas.state [ 0 ].enabled );
  EXPECT_EQ( 4, ppca.vas.state [ 0 ].size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), ppca.vas.state [ 0 ].type );
  EXPECT_EQ( 0, ppca.vas.state [ 0 ].stride );
  EXPECT_EQ( NULL, ppca.vas.state [ 0 ].pointer );

  ppca.ShadowEnableClientState ( GL_COLOR_ARRAY );
  ppca.ShadowColorPointer ( 1, 2, 3, NULL );

  EXPECT_TRUE( ppca.vas.state [ 0 ].enabled );
  EXPECT_EQ( 1, ppca.vas.state [ 0 ].size );
  EXPECT_EQ( 2u, ppca.vas.state [ 0 ].type );
  EXPECT_EQ( 3, ppca.vas.state [ 0 ].stride );
  EXPECT_EQ( NULL, ppca.vas.state [ 0 ].pointer );

  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );
  EXPECT_CALL( mock, glDisableClientState( GL_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glColorPointer( 4, GL_FLOAT, 0, NULL ) );

  ppca.PushClientAttribDefaultEXT( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );

  EXPECT_EQ( 4, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  EXPECT_FALSE( ppca.vas.state [ 0 ].enabled );
  EXPECT_EQ( 4, ppca.vas.state [ 0 ].size );
  EXPECT_EQ( static_cast<GLenum>( GL_FLOAT ), ppca.vas.state [ 0 ].type );
  EXPECT_EQ( 0, ppca.vas.state [ 0 ].stride );
  EXPECT_EQ( NULL, ppca.vas.state [ 0 ].pointer );

  ctx.info->gles = true;

  ppca.PushClientAttrib( &ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  ppca.PopClientAttrib( &ctx );

  ctx.info->gles = false;

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 101 ) );
  EXPECT_CALL( mock, glEnableClientState( GL_COLOR_ARRAY ) );
  EXPECT_CALL( mock, glColorPointer( 1, 2, 3, NULL ) );
  EXPECT_CALL( mock, glPopClientAttrib() );

  ppca.PopClientAttrib( &ctx );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  EXPECT_CALL( mock, glPopClientAttrib() );

  ppca.PopClientAttrib( &ctx );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.PopClientAttrib( &ctx );

  EXPECT_EQ( 101, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT, 100 ) );

  ppca.PopClientAttrib( &ctx );

  EXPECT_EQ( 100, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );

  ppca.PopClientAttrib( &ctx );
  ppca.PopClientAttrib( &ctx );
  ppca.PopClientAttrib( &ctx );
  ppca.PopClientAttrib( &ctx );

  EXPECT_EQ( 100, ppca.pss.Get( GL_UNPACK_ALIGNMENT ) );
}

TEST ( Ppca, Get ) {
  Ppca ppca;
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
  EXPECT_EQ( REGAL_CLIENT_ATTRIB_STACK_DEPTH, resulti[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti64 ) );
  EXPECT_EQ( REGAL_CLIENT_ATTRIB_STACK_DEPTH, resulti64[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultf ) );
  EXPECT_EQ( REGAL_CLIENT_ATTRIB_STACK_DEPTH, resultf[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultd ) );
  EXPECT_EQ( REGAL_CLIENT_ATTRIB_STACK_DEPTH, resultd[ 0 ] );

  EXPECT_TRUE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultb ) );
  EXPECT_EQ( GL_TRUE, resultb[ 0 ] );

  // If the backend appears to be compatible with the request, the emulation
  // should just defer to the backend.

  ctx.info->gles = ctx.info->core = false;
  EXPECT_FALSE( ppca.Get( &ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti ) );
}

}  // namespace
