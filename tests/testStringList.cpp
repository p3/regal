/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2012-2014 Nigel Stewart
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
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

#include <string>

#include "boost/print/string_list.hpp"

namespace {

typedef ::std::string                                string;
typedef ::boost::print::string_list< ::std::string > string_list;

// ====================================
// boost::print::string_list
// ====================================

TEST( StringList, Constructor )
{
  {
    string_list x;
    EXPECT_EQ(0u, x.size());
    EXPECT_EQ(0u, x.count());
  }

  // space delimiter

  {
    string_list x(string("a b c d e f"),' ');
    EXPECT_EQ(6u, x.size());
    EXPECT_EQ(6u, x.count());

    string_list y(x);
    EXPECT_EQ(x.size(),  y.size());
    EXPECT_EQ(x.count(), y.count());
//    EXPECT_EQ(x, y);
  }

  // . delimiter

  {
    string_list x(string("a.b.c.d.e.f"),'.');
    EXPECT_EQ(6u, x.size());
    EXPECT_EQ(6u, x.count());
  }

  // TODO - There ought to be a c-string constructor with delimiter, too

  {
    const char *s[5]  = { "a", "b", "c", "d", "e" };
    const int   l[5] = { 1,   1,   1,   -1,  -1  };
    string_list x(5,s,l);
    EXPECT_EQ(5u, x.size());
    EXPECT_EQ(5u, x.count());

    EXPECT_EQ(string("abcde"), x.str());
    EXPECT_EQ(string("a b c d e"), x.join(" "));
  }

  {
    const char *s[5]  = { "A", "B", "C", "D", "E" };
    string_list x(5,s);
    EXPECT_EQ(5u, x.size());
    EXPECT_EQ(5u, x.count());

    EXPECT_EQ(string("ABCDE"), x.str());
    EXPECT_EQ(string("A B C D E"), x.join(" "));
  }

  {
    const char *s[5]  = { "A\nB\n", "C", "D\nE\n" };
    string_list x(3,s,NULL,'\n');
    EXPECT_EQ(5u, x.size());
    EXPECT_EQ(5u, x.count());

    EXPECT_EQ(string("ABCDE"), x.str());
    EXPECT_EQ(string("A B C D E"), x.join(" "));
  }
}

TEST( StringList, Insert )
{
  {
    string_list x(string("a b c d e f"),' ');
    EXPECT_EQ(string("a b c d e f"), x.join(" "));

    x.insert(0,"0");    EXPECT_EQ(string("0 a b c d e f"),                                               x.join(" "));
    x.insert(1,"1");    EXPECT_EQ(string("0 1 a b c d e f"),                                             x.join(" "));
    x.insert(7,"7");    EXPECT_EQ(string("0 1 a b c d e 7 f"),                                           x.join(" "));
    x.insert(9,"9");    EXPECT_EQ(string("0 1 a b c d e 7 f 9"),                                         x.join(" "));
    x.insert(0,3,"x");  EXPECT_EQ(string("x x x 0 1 a b c d e 7 f 9"),                                   x.join(" "));
    x.insert(2,3,"y");  EXPECT_EQ(string("x x y y y x 0 1 a b c d e 7 f 9"),                             x.join(" "));
    x.insert(6,3,"z");  EXPECT_EQ(string("x x y y y x z z z 0 1 a b c d e 7 f 9"),                       x.join(" "));
    x.insert(18,3,"-"); EXPECT_EQ(string("x x y y y x z z z 0 1 a b c d e 7 f - - - 9"),                 x.join(" "));
    x.insert(17,3,"+"); EXPECT_EQ(string("x x y y y x z z z 0 1 a b c d e 7 + + + f - - - 9"),           x.join(" "));
    x.insert(16,3,"="); EXPECT_EQ(string("x x y y y x z z z 0 1 a b c d e = = = 7 + + + f - - - 9"),     x.join(" "));
    x.insert(14,2,"<"); EXPECT_EQ(string("x x y y y x z z z 0 1 a b c < < d e = = = 7 + + + f - - - 9"), x.join(" "));
    x.insert(14,0,">"); EXPECT_EQ(string("x x y y y x z z z 0 1 a b c < < d e = = = 7 + + + f - - - 9"), x.join(" "));
  }
}

TEST( StringList, Split )
{
  {
    string_list x;
    x.split("a b c d e f");
    EXPECT_EQ(x.size(),6);
    EXPECT_EQ(x[0],"a");
    EXPECT_EQ(x[1],"b");
    EXPECT_EQ(x[2],"c");
    EXPECT_EQ(x[3],"d");
    EXPECT_EQ(x[4],"e");
    EXPECT_EQ(x[5],"f");
  }

  {
    string_list x;
    x.split(" a b c d e f ");
    EXPECT_EQ(x.size(),6);
    EXPECT_EQ(x[0],"a");
    EXPECT_EQ(x[1],"b");
    EXPECT_EQ(x[2],"c");
    EXPECT_EQ(x[3],"d");
    EXPECT_EQ(x[4],"e");
    EXPECT_EQ(x[5],"f");
  }

  {
    string_list x;
    x.split(" a  b   c     d  e  f ");
    EXPECT_EQ(x.size(),6);
    EXPECT_EQ(x[0],"a");
    EXPECT_EQ(x[1],"b");
    EXPECT_EQ(x[2],"c");
    EXPECT_EQ(x[3],"d");
    EXPECT_EQ(x[4],"e");
    EXPECT_EQ(x[5],"f");
  }

  {
    string_list x;
    x.split("a b c d e f",8,' ');
    EXPECT_EQ(x.size(),4);
    EXPECT_EQ(x[0],"a");
    EXPECT_EQ(x[1],"b");
    EXPECT_EQ(x[2],"c");
    EXPECT_EQ(x[3],"d");
  }

  {
    string_list x;
    x.split("a,b,c,d",',');
    EXPECT_EQ(x.size(),4);
    EXPECT_EQ(x[0],"a");
    EXPECT_EQ(x[1],"b");
    EXPECT_EQ(x[2],"c");
    EXPECT_EQ(x[3],"d");

    EXPECT_EQ(x.join(" "),"a b c d");
    EXPECT_EQ(x.join("x"),"axbxcxd");
    EXPECT_EQ(x.join("."),"a.b.c.d");
  }
}

}
