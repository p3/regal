
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

#include "RegalDispatchGMock.h"
#include "RegalDispatch.h"
#include "RegalState.h"

namespace {

using namespace Regal;

// ====================================
// Regal::State::Depth
// ====================================

TEST( RegalStateDepth, SetDispatchesCorrectly ) {
  // Note: This test was written to demonstrate the first use of gmock in
  // Regal.

  RegalGMockInterface mock;

  DispatchTable dt;
  InitDispatchTableGMock(dt);

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
  depth.set(dt);

  // The expectations set above are implicitly checked here by the mock interface
  // destructor.
}

}  // namespace
