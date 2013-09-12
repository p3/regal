/*
  Copyright (c) 2011-2013 NVIDIA Corporation
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

 Regal glHint emu layer
 Scott Nations, Nigel Stewart

 */

#ifndef __REGAL_HINT_H__
#define __REGAL_HINT_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

struct Hint
{
  GLenum lineSmooth;
  GLenum polygonSmooth;
  GLenum textureCompression;
  GLenum fragmentShaderDerivative;
  GLenum perspectiveCorrection;
  GLenum pointSmooth;
  GLenum fog;
  GLenum generateMipmap;

  void Init(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);

    lineSmooth               = GL_DONT_CARE;
    polygonSmooth            = GL_DONT_CARE;
    textureCompression       = GL_DONT_CARE;
    fragmentShaderDerivative = GL_DONT_CARE;
    perspectiveCorrection    = GL_DONT_CARE;
    pointSmooth              = GL_DONT_CARE;
    fog                      = GL_DONT_CARE;
    generateMipmap           = GL_DONT_CARE;
  }

  void Cleanup(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  bool glHint(RegalContext &ctx, GLenum target, GLenum mode)
  {
    // just go straight to the driver if this is not ES
    //<> what about core contexts?

    if (!ctx.isES1() && !ctx.isES2())
      return false;

    // these are the only valid choices for mode

    switch (mode)
    {
      case GL_FASTEST:
      case GL_NICEST:
      case GL_DONT_CARE:
        break;
      default:
        return false;
    }

    switch (target)
    {
      // eat any hints that aren't supported by ES, tracking
      // the state so we can respond to Get() calls

      case GL_LINE_SMOOTH_HINT:
        lineSmooth = mode;
        break;
      case GL_POLYGON_SMOOTH_HINT:
        polygonSmooth = mode;
        break;
      case GL_TEXTURE_COMPRESSION_HINT:
        textureCompression = mode;
        break;
      case GL_PERSPECTIVE_CORRECTION_HINT :
        perspectiveCorrection = mode;
        break;
      case GL_POINT_SMOOTH_HINT:
        pointSmooth = mode;
        break;
      case GL_FOG_HINT:
        fog = mode;
        break;

      // ES supported hints go on down to the driver

      case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
      case GL_GENERATE_MIPMAP_HINT:
      default:
        return false;
    }
    Warning("glHint does not support ", Token::GLenumToString(target), " on ES 2.0");
    return true;
  }

  template <typename T> bool glGetv(RegalContext &ctx, GLenum pname, T *params)
  {
    // just go straight to the driver if this is not ES
    //<> what about core contexts?

    if (!ctx.isES1() && !ctx.isES2())
      return false;

    switch (pname)
    {
      // return a value for any hints that aren't supported by ES

      case GL_LINE_SMOOTH_HINT:
        params[0] = static_cast<T>( lineSmooth );
        break;
      case GL_POLYGON_SMOOTH_HINT:
        params[0] = static_cast<T>( polygonSmooth );
        break;
      case GL_TEXTURE_COMPRESSION_HINT:
        params[0] = static_cast<T>( textureCompression );
        break;
      case GL_PERSPECTIVE_CORRECTION_HINT :
        params[0] = static_cast<T>( perspectiveCorrection );
        break;
      case GL_POINT_SMOOTH_HINT:
        params[0] = static_cast<T>( pointSmooth );
        break;
      case GL_FOG_HINT:
        params[0] = static_cast<T>( fog );
        break;

      // ES supported hints go on down to the driver

      case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
      case GL_GENERATE_MIPMAP_HINT:
      default:
        return false;
    }
    return true;
  }
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_HINT_H__
