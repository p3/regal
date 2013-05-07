/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2013 Nigel Stewart
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

#include <string>
#include <algorithm>

#include <boost/print/json.hpp>

#include "RegalJson.h"
#include "RegalConfig.h"
#include "RegalLog.h"

namespace REGAL_NAMESPACE_INTERNAL { namespace Json { struct Output : public ::boost::print::json::output<std::string> {}; } }

using namespace std;
using namespace ::REGAL_NAMESPACE_INTERNAL;

namespace {

static string json()
{
  Json::Output jo;
  jo.object();
    jo.object("regal");
      Config::writeJSON(jo);
      Logging::writeJSON(jo);
    jo.end();
  jo.end();
  return jo.str();
}

// ====================================
// Regal::Json
// ====================================

TEST( RegalJson, Parser )
{
  const string state0 = json();

  EXPECT_GT(state0.length(),0u);
  EXPECT_NE(string::npos,state0.find("\"regal\""));
  EXPECT_NE(string::npos,state0.find("\"config\""));
  EXPECT_NE(string::npos,state0.find("\"logging\""));

  // Toggle some Regal::Config state

  Config::frameMd5Color = !Config::frameMd5Color;

  const string state1 = json();
  EXPECT_NE(state0,state1);

  // Toggle it back again

  Config::frameMd5Color = !Config::frameMd5Color;

  const string state2 = json();
  EXPECT_NE(state1,state2);
  EXPECT_EQ(state0,state2);

  // Increment an integer

  Config::frameMd5DepthMask += 100;

  const string state3 = json();
  EXPECT_NE(state3,state0);
  EXPECT_NE(state3,state1);

  // Set a string

  Config::cacheDirectory = "foo";

  const string state4 = json();
  EXPECT_NE(string::npos,state4.find("\"foo\""));
  EXPECT_NE(state4,state0);
  EXPECT_NE(state4,state1);
  EXPECT_NE(state4,state3);

  // Reset to state3

  RegalConfigure(state3.c_str());
  EXPECT_EQ(json(),state3);

  // Reset to state1

  RegalConfigure(state1.c_str());
  EXPECT_EQ(state1,json());

  // Reset to state0

  RegalConfigure(state0.c_str());
  EXPECT_EQ(state0,json());
}

TEST( RegalJson, Subset )
{
  const string state0 = json();

  const char *test1 = "{ \"regal\" : { \"config\" : { \"dispatch\" : { \"enable\" : { \"emulation\" : false }, \"force\" : { \"emulation\" : false } } } } }";
  const char *test2 = "{ \"regal\" : { \"config\" : { \"dispatch\" : { \"enable\" : { \"emulation\" : true }, \"force\" : { \"emulation\" : true } } } } }";

  RegalConfigure(test1);
  EXPECT_EQ(Config::enableEmulation,false);
  EXPECT_EQ(Config::forceEmulation,false);

  RegalConfigure(test2);
  EXPECT_EQ(Config::enableEmulation,true);
  EXPECT_EQ(Config::forceEmulation,true);

  // Reset to state0

  RegalConfigure(state0.c_str());
  EXPECT_EQ(state0,json());
}

TEST( RegalJson, Trace )
{
  const string state0 = json();

  const char *traceTrue  = "{ \"regal\" : { \"config\" : { \"dispatch\" : { \"enable\" : { \"trace\" : true } } } } }";
  const char *traceFalse = "{ \"regal\" : { \"config\" : { \"dispatch\" : { \"enable\" : { \"trace\" : false } } } } }";

  RegalConfigure(traceTrue);
  EXPECT_EQ(Config::enableTrace,true);

  RegalConfigure(traceFalse);
  EXPECT_EQ(Config::enableTrace,false);

  // Reset to state0

  RegalConfigure(state0.c_str());
  EXPECT_EQ(state0,json());
}

}
