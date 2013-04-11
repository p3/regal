/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
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

 Regal Emulation layer base class.
 Cass Everitt

 */

#ifndef __REGAL_EMU_H__
#define __REGAL_EMU_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <cassert>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

// In ES2 mode, 16 texture units only?

#ifndef REGAL_EMU_MAX_TEXTURE_UNITS
#define REGAL_EMU_MAX_TEXTURE_UNITS 32
#endif

#ifndef REGAL_FIXED_FUNCTION_MAX_CLIP_DISTANCES
#define REGAL_FIXED_FUNCTION_MAX_CLIP_DISTANCES 8
#endif

#ifndef REGAL_MAX_VIEWPORTS
#define REGAL_MAX_VIEWPORTS 16
#endif

#ifndef REGAL_MAX_DRAW_BUFFERS
#define REGAL_MAX_DRAW_BUFFERS 8
#endif


struct Float4
{
    Float4()
    : x(0.f),
      y(0.f),
      z(0.f),
      w(1.f)
    {
    }

    Float4( const GLfloat X, const GLfloat Y, const GLfloat Z, const GLfloat W )
    : x(X),
      y(Y),
      z(Z),
      w(W)
    {
    }

    Float4( const GLdouble X, const GLdouble Y, const GLdouble Z, const GLdouble W )
    : x(static_cast<GLfloat>(X)),
      y(static_cast<GLfloat>(Y)),
      z(static_cast<GLfloat>(Z)),
      w(static_cast<GLfloat>(W))
    {
    }

    Float4( const GLint X, const GLint Y, const GLint Z, const GLint W )
    : x(static_cast<GLfloat>(X)),
      y(static_cast<GLfloat>(Y)),
      z(static_cast<GLfloat>(Z)),
      w(static_cast<GLfloat>(W))
    {
    }

    Float4( const GLuint X, const GLuint Y, const GLuint Z, const GLuint W )
    : x(static_cast<GLfloat>(X)),
      y(static_cast<GLfloat>(Y)),
      z(static_cast<GLfloat>(Z)),
      w(static_cast<GLfloat>(W))
    {
    }

    GLfloat x, y, z, w;
};


struct RegalEmu
{
};

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_EMU_H__


