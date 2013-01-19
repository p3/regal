
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

#include <stddef.h>

#include <GL/Regal.h>

#include "RegalDispatchGMock.h"
#include "RegalDispatch.h"
#include "RegalPpa.h"

namespace {

using namespace Regal;
using ::testing::Mock;

TEST( RegalPpa, Enable ) {
  RegalPpa ppa;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->gles = true;

  EXPECT_TRUE ( ppa.Enable( &ctx, GL_POINT_SMOOTH ) );
  EXPECT_TRUE ( ppa.Enable( &ctx, GL_LINE_STIPPLE ) );

  ctx.info->gles = ctx.info->core = false;

  EXPECT_FALSE ( ppa.Enable( &ctx, GL_POINT_SMOOTH ) );
  EXPECT_FALSE ( ppa.Enable( &ctx, GL_LINE_STIPPLE ) );

  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), ppa.State::Depth::enable );
  EXPECT_FALSE( ppa.Enable( &ctx, GL_DEPTH_TEST ) );
  EXPECT_EQ( static_cast<GLboolean>( GL_TRUE ), ppa.State::Depth::enable );
  EXPECT_FALSE( ppa.Disable( &ctx, GL_DEPTH_TEST ) );
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), ppa.State::Depth::enable );
  
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), ppa.State::Stencil::enable );
  EXPECT_FALSE( ppa.Enable( &ctx, GL_STENCIL_TEST ) );
  EXPECT_EQ( static_cast<GLboolean>( GL_TRUE ), ppa.State::Stencil::enable );
  
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), ppa.State::Polygon::cullEnable );
  EXPECT_FALSE( ppa.Enable( &ctx, GL_CULL_FACE ) );
  EXPECT_EQ( static_cast<GLboolean>( GL_TRUE ), ppa.State::Polygon::cullEnable );
  
  EXPECT_EQ( static_cast<GLboolean>( GL_FALSE ), ppa.State::Polygon::smoothEnable );
  EXPECT_FALSE( ppa.Enable( &ctx, GL_POLYGON_SMOOTH ) );
  EXPECT_EQ( static_cast<GLboolean>( GL_TRUE ), ppa.State::Polygon::smoothEnable );
}

TEST( RegalPpa, PushPopAttrib ) {
  RegalGMockInterface mock;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->gles = ctx.info->core = false;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  RegalPpa ppa;

  EXPECT_EQ( 0u, ppa.depthStack.size() );
  EXPECT_EQ( 0u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PushAttrib( &ctx, GL_DEPTH_BUFFER_BIT );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 0u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PushAttrib( &ctx, GL_STENCIL_BUFFER_BIT );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PushAttrib( &ctx, GL_POLYGON_BIT );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 1u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PushAttrib( &ctx, GL_TRANSFORM_BIT );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 1u, ppa.polygonStack.size() );
  EXPECT_EQ( 1u, ppa.transformStack.size() );

  EXPECT_CALL( mock, glPushAttrib( GL_ACCUM_BUFFER_BIT )  );
  ppa.PushAttrib( &ctx, GL_ACCUM_BUFFER_BIT );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 1u, ppa.polygonStack.size() );
  EXPECT_EQ( 1u, ppa.transformStack.size() );

  EXPECT_CALL( mock, glPopAttrib() );
  ppa.PopAttrib( &ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 1u, ppa.polygonStack.size() );
  EXPECT_EQ( 1u, ppa.transformStack.size() );

  ppa.PopAttrib( &ctx );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 1u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PopAttrib( &ctx );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 1u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PopAttrib( &ctx );

  EXPECT_EQ( 1u, ppa.depthStack.size() );
  EXPECT_EQ( 0u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );

  ppa.PopAttrib( &ctx );

  EXPECT_EQ( 0u, ppa.depthStack.size() );
  EXPECT_EQ( 0u, ppa.stencilStack.size() );
  EXPECT_EQ( 0u, ppa.polygonStack.size() );
  EXPECT_EQ( 0u, ppa.transformStack.size() );
}

} // namespace
