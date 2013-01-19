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

#include <GL/Regal.h>

#include "gmock/gmock.h"

#include <stddef.h>

#include <GL/Regal.h>

#include "RegalDispatchGMock.h"
#include "RegalDispatch.h"
#include "RegalState.h"

namespace {

using namespace Regal;

using ::testing::ElementsAre;
using ::testing::Mock;

// ====================================
// Regal::State::Depth
// ====================================

TEST( RegalStateDepth, SetDispatchesCorrectly ) {
  // Note: This test was written to demonstrate the first use of gmock in
  // Regal.

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  State::Depth depth;

  // Set some non-default state to look for.
  depth.enable = GL_TRUE;
  depth.func = GL_LEQUAL;
  depth.clear = 0.5;
  depth.mask = GL_FALSE;

  // Set up call expectations on the gmock interface. Note that by default calls
  // are allowed to be made in any order.
  EXPECT_CALL( mock, glEnable( GL_DEPTH_TEST ) );
  EXPECT_CALL( mock, glDepthFunc( GL_LEQUAL ) );
  EXPECT_CALL( mock, glClearDepth( 0.5 ) );
  EXPECT_CALL( mock, glDepthMask( GL_FALSE ) );

  // Invoke the function under test, to ensure it is dispatching calls
  // correctly.
  depth.set( dt );

  // The expectations set above are implicitly checked here by the mock interface
  // destructor.
}

// ====================================
// Regal::State::Transform
// ====================================

TEST( RegalStateTransform, WorksAsExpected ) {
  State::Transform t1;
  State::Transform t2;

  for ( size_t i = 0; i < State::Transform::CLIP_PLANE_COUNT; ++i ) {
    t1.clipPlane[ i ].enabled = 50 + i;
    for ( size_t j = 0; j < 4; ++j ) {
      t1.clipPlane[ i ].equation.data[ j ] = i * 4 + j;
    }
  }

  t1.matrixMode = 100;
  t1.normalize = 101;
  t1.rescaleNormal = 102;

  t2.swap( t1 );

  // Test that t1 now has the correct initial state

  for ( size_t i = 0; i < State::Transform::CLIP_PLANE_COUNT; ++i ) {
    EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), t1.clipPlane[ i ].enabled );
    for ( size_t j = 0; j < 4; ++j ) {
      EXPECT_EQ( 0, t1.clipPlane[ i ].equation.data[ j ] );
    }
  }

  EXPECT_EQ( static_cast<GLenum>( GL_MODELVIEW ), t1.matrixMode );
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), t1.normalize );
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), t1.rescaleNormal );

  // Test that t2 has the artificial state

  for ( size_t i = 0; i < State::Transform::CLIP_PLANE_COUNT; ++i ) {
    EXPECT_EQ( static_cast<GLboolean>( 50 + i ), t2.clipPlane[ i ].enabled );
    for ( size_t j = 0; j < 4; ++j ) {
      EXPECT_EQ( i * 4 + j, t2.clipPlane[ i ].equation.data[ j ] );
    }
  }

  EXPECT_EQ( static_cast<GLenum>( 100 ), t2.matrixMode );
  EXPECT_EQ( static_cast<GLboolean>( 101 ), t2.normalize );
  EXPECT_EQ( static_cast<GLboolean>( 102 ), t2.rescaleNormal );

  t2.glMatrixMode( GL_MODELVIEW );
  EXPECT_EQ( static_cast<GLenum>( GL_MODELVIEW ), t2.matrixMode );

  double equation[ 4 ] = { 200, 201, 202, 203 };

  t2.glClipPlane( GL_CLIP_PLANE0, equation );
  EXPECT_EQ( 200, t2.clipPlane[ 0 ].equation.data[ 0 ] );
  EXPECT_EQ( 201, t2.clipPlane[ 0 ].equation.data[ 1 ] );
  EXPECT_EQ( 202, t2.clipPlane[ 0 ].equation.data[ 2 ] );
  EXPECT_EQ( 203, t2.clipPlane[ 0 ].equation.data[ 3 ] );

  t2.glClipPlane( GL_CLIP_PLANE5, equation );
  EXPECT_EQ( 200, t2.clipPlane[ 5 ].equation.data[ 0 ] );
  EXPECT_EQ( 201, t2.clipPlane[ 5 ].equation.data[ 1 ] );
  EXPECT_EQ( 202, t2.clipPlane[ 5 ].equation.data[ 2 ] );
  EXPECT_EQ( 203, t2.clipPlane[ 5 ].equation.data[ 3 ] );

  // If somehow a bad clip pane index is passed, it should be silently ignored.
  t2.glClipPlane( GL_CLIP_PLANE0 - 1, equation );
  t2.glClipPlane( GL_CLIP_PLANE0 + 6, equation );
}

MATCHER_P4( IsDouble4, v1, v2, v3, v4, "" ) {
  return ( arg[ 0 ] == v1 ) && ( arg[ 1 ] == v2 ) && ( arg[ 2 ] == v3 ) && ( arg[ 3 ] == v4 );
}

TEST( RegalStateTransform, TransitionSetsStateCorrectly ) {
  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock( dt );

  // No state difference means no calls are made.
  {
    State::Transform current;

    current.transition( dt, current );

    Mock::VerifyAndClear( &mock );
  }

  // Specific tests for clip plane state changes
  {
    State::Transform current;
    State::Transform target;

    target.clipPlane[ 0 ].enabled = True;
    target.clipPlane[ 0 ].equation.data[ 0 ] = 100;
    current.clipPlane[ 4 ].equation.data[ 3 ] = 101;
    current.clipPlane[ 5 ].enabled = True;

    EXPECT_CALL( mock, glEnable( GL_CLIP_PLANE0 ) );
    EXPECT_CALL( mock, glClipPlane( GL_CLIP_PLANE0, IsDouble4(  100,  0,  0,  0 ) ) );
    EXPECT_CALL( mock, glClipPlane( GL_CLIP_PLANE4, IsDouble4(    0,  0,  0,  0 ) ) );
    EXPECT_CALL( mock, glDisable( GL_CLIP_PLANE5 ) );

    target.transition( dt, current );

    Mock::VerifyAndClear( &mock );
  }

  {
    State::Transform current;
    State::Transform target;

    target.matrixMode = 108;
    target.normalize = GL_TRUE;
    target.rescaleNormal = GL_TRUE;

    EXPECT_CALL( mock, glMatrixMode( static_cast<GLenum> ( 108 ) ) );
    EXPECT_CALL( mock, glEnable( GL_NORMALIZE ) );
    EXPECT_CALL( mock, glEnable( GL_RESCALE_NORMAL ) );

    target.transition( dt, current );

    Mock::VerifyAndClear( &mock );
  }

  {
    State::Transform current;
    State::Transform target;

    current.normalize = GL_TRUE;
    current.rescaleNormal = GL_TRUE;

    EXPECT_CALL( mock, glDisable( GL_NORMALIZE ) );
    EXPECT_CALL( mock, glDisable( GL_RESCALE_NORMAL ) );

    target.transition( dt, current );

    Mock::VerifyAndClear( &mock );
  }
}

}  // namespace
