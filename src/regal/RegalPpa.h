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

 Regal push / pop attrib
 Nigel Stewart

 */

#ifndef __REGAL_PPA_H__
#define __REGAL_PPA_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalState.h"
#include "RegalEmu.h"
#include "RegalLog.h"
#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

// Only glPushAttrib(GL_STENCIL_BUFFER_BIT) so far...

struct RegalPpa : public RegalEmu, State::Stencil, State::Depth, State::Polygon, State::Transform
{
  void Init(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  void PushAttrib(RegalContext *ctx, GLbitfield mask)
  {
    maskStack.push_back(mask);

    if (mask&GL_DEPTH_BUFFER_BIT)
    {
      depthStack.push_back(State::Depth());
      depthStack.back() = *this;
      mask &= ~GL_DEPTH_BUFFER_BIT;
    }

    if (mask&GL_STENCIL_BUFFER_BIT)
    {
      stencilStack.push_back(State::Stencil());
      stencilStack.back() = *this;
      mask &= ~GL_STENCIL_BUFFER_BIT;
    }

    if (mask&GL_POLYGON_BIT)
    {
      polygonStack.push_back(State::Polygon());
      polygonStack.back() = *this;
      mask &= ~GL_POLYGON_BIT;
    }

    if (mask&GL_TRANSFORM_BIT)
    {
      transformStack.push_back(State::Transform());
      transformStack.back() = *this;
      mask &= ~GL_TRANSFORM_BIT;
    }

    // Pass the rest through, for now

    RegalAssert(ctx);

    if (ctx->info->core || ctx->info->gles)
      return;

    if (mask)
      ctx->dispatcher.emulation.glPushAttrib(mask);
  }

  void PopAttrib(RegalContext *ctx)
  {
    RegalAssert(ctx);
    RegalAssert(maskStack.size());

    if (maskStack.size())
    {
      GLbitfield mask = maskStack.back();
      maskStack.pop_back();

      if (mask&GL_DEPTH_BUFFER_BIT)
      {
        RegalAssert(depthStack.size());
        State::Depth::swap(depthStack.back());
        depthStack.pop_back();

        Internal("RegalPpa::PopAttrib GL_DEPTH_BUFFER_BIT ",State::Depth::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Depth::set(ctx->dispatcher.emulation);

        mask &= ~GL_DEPTH_BUFFER_BIT;
      }

      if (mask&GL_STENCIL_BUFFER_BIT)
      {
        RegalAssert(stencilStack.size());
        State::Stencil::swap(stencilStack.back());
        stencilStack.pop_back();

        Internal("RegalPpa::PopAttrib GL_STENCIL_BUFFER_BIT ",State::Stencil::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Stencil::set(ctx->dispatcher.emulation);

        mask &= ~GL_STENCIL_BUFFER_BIT;
      }

      if (mask&GL_POLYGON_BIT)
      {
        RegalAssert(polygonStack.size());
        State::Polygon::swap(polygonStack.back());
        polygonStack.pop_back();

        Internal("RegalPpa::PopAttrib GL_POLYGON_BIT ",State::Polygon::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Polygon::set(ctx->dispatcher.emulation);

        mask &= ~GL_POLYGON_BIT;
      }

      if (mask&GL_TRANSFORM_BIT)
      {
        RegalAssert(transformStack.size());
        State::Transform::swap(transformStack.back());

        Internal("RegalPpa::PopAttrib GL_TRANSFORM_BIT ",State::Transform::toString());

        State::Transform::transition(ctx->dispatcher.emulation, transformStack.back());
        transformStack.pop_back();

        mask &= ~GL_TRANSFORM_BIT;
      }

      // Pass the rest through, for now

      if (ctx->info->core || ctx->info->gles)
        return;

      if (mask)
        ctx->dispatcher.emulation.glPopAttrib();
    }
  }

  template <typename T> bool Get(RegalContext *ctx, GLenum pname, T *params)
  {
    RegalAssert(ctx);

    if (ctx->info->core || ctx->info->gles)
    {
      switch (pname)
      {
        case GL_ACCUM_RED_BITS:
        case GL_ACCUM_GREEN_BITS:
        case GL_ACCUM_BLUE_BITS:
        case GL_ACCUM_ALPHA_BITS:
        case GL_ALPHA_BITS:
        case GL_AUX_BUFFERS:
        case GL_BLUE_BITS:
        case GL_DEPTH_BITS:
        case GL_GREEN_BITS:
        case GL_INDEX_BITS:
        case GL_LINE_STIPPLE:
        case GL_MAX_PIXEL_MAP_TABLE:
        case GL_MAX_NAME_STACK_DEPTH:
        case GL_MAX_LIST_NESTING:
        case GL_MAX_EVAL_ORDER:
        case GL_MAX_ATTRIB_STACK_DEPTH:
        case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        case GL_RED_BITS:
        case GL_STENCIL_BITS:
          if (params)
            params[0] = 0;
          break;
        default:
          return false;
      }
      return true;
    }
    return false;
  }

  bool SetEnable(RegalContext *ctx, GLenum cap, GLboolean enabled)
  {
    switch (cap)
    {
      case GL_DEPTH_TEST:           State::Depth::enable            = enabled; break;
      case GL_STENCIL_TEST:         State::Stencil::enable          = enabled; break;
      case GL_CULL_FACE:            State::Polygon::cullEnable      = enabled; break;
      case GL_POLYGON_SMOOTH:       State::Polygon::smoothEnable    = enabled; break;
      case GL_POLYGON_STIPPLE:      State::Polygon::stippleEnable   = enabled; break;
      case GL_POLYGON_OFFSET_FILL:  State::Polygon::offsetFill      = enabled; break;
      case GL_POLYGON_OFFSET_LINE:  State::Polygon::offsetLine      = enabled; break;
      case GL_POLYGON_OFFSET_POINT: State::Polygon::offsetPoint     = enabled; break;
      case GL_NORMALIZE:            State::Transform::normalize     = enabled; break;
      case GL_RESCALE_NORMAL:       State::Transform::rescaleNormal = enabled; break;
      default:                                                                 break;
    }

    if (ctx->info->core || ctx->info->gles)
    {
      switch( cap )
      {
        case GL_POINT_SMOOTH:
        case GL_LINE_STIPPLE:
          return true;
        default:
          break;
      }
    }

    return false;
  }

  bool Enable(RegalContext *ctx, GLenum cap)
  {
    Internal("RegalPpa::Enable ",Token::toString(cap));
    return SetEnable(ctx, cap, GL_TRUE);
  }

  bool Disable(RegalContext * ctx, GLenum cap)
  {
    Internal("RegalPpa::Disable ",Token::toString(cap));
    return SetEnable(ctx, cap, GL_FALSE);
  }

  std::vector<GLbitfield>     maskStack;
  std::vector<State::Depth>   depthStack;
  std::vector<State::Stencil> stencilStack;
  std::vector<State::Polygon> polygonStack;
  std::vector<State::Transform> transformStack;
};

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_PPA_H__
