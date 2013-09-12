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

 Regal push / pop attrib
 Nigel Stewart, Scott Nations

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

namespace Emu
{

// Work in progress...

struct Ppa : public State::Stencil, State::Depth, State::Polygon, State::Transform, State::Hint,
    State::Enable, State::List, State::AccumBuffer, State::Scissor, State::Viewport, State::Line,
    State::Multisample, State::Eval, State::Fog, State::Point, State::PolygonStipple, State::ColorBuffer,
    State::PixelMode, State::Lighting
{
  void Init(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
    activeTextureUnit = 0;
  }

  void Cleanup(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  void PushAttrib(RegalContext *ctx, GLbitfield mask)
  {
    maskStack.push_back(mask);

    if (mask&GL_DEPTH_BUFFER_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_DEPTH_BUFFER_BIT ",State::Depth::toString());
      depthStack.push_back(State::Depth());
      depthStack.back() = *this;
      mask &= ~GL_DEPTH_BUFFER_BIT;
    }

    if (mask&GL_STENCIL_BUFFER_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_STENCIL_BUFFER_BIT ",State::Stencil::toString());
      stencilStack.push_back(State::Stencil());
      stencilStack.back() = *this;
      mask &= ~GL_STENCIL_BUFFER_BIT;
    }

    if (mask&GL_POLYGON_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_POLYGON_BIT ",State::Polygon::toString());
      polygonStack.push_back(State::Polygon());
      polygonStack.back() = *this;
      mask &= ~GL_POLYGON_BIT;
    }

    if (mask&GL_TRANSFORM_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_TRANSFORM_BIT ",State::Transform::toString());
      transformStack.push_back(State::Transform());
      transformStack.back() = *this;
      mask &= ~GL_TRANSFORM_BIT;
    }

    if (mask&GL_HINT_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_HINT_BIT ",State::Hint::toString());
      hintStack.push_back(State::Hint());
      hintStack.back() = *this;
      mask &= ~GL_HINT_BIT;
    }

    if (mask&GL_ENABLE_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_ENABLE_BIT ",State::Enable::toString());
      enableStack.push_back(State::Enable());
      enableStack.back() = *this;
      mask &= ~GL_ENABLE_BIT;
    }

    if (mask&GL_LIST_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_LIST_BIT ",State::List::toString());
      listStack.push_back(State::List());
      listStack.back() = *this;
      mask &= ~GL_LIST_BIT;
    }

    if (mask&GL_ACCUM_BUFFER_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_ACCUM_BUFFER_BIT ",State::AccumBuffer::toString());
      accumBufferStack.push_back(State::AccumBuffer());
      accumBufferStack.back() = *this;
      mask &= ~GL_ACCUM_BUFFER_BIT;
    }

    if (mask&GL_SCISSOR_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_SCISSOR_BIT ",State::Scissor::toString());
      if (!State::Scissor::fullyDefined())
        State::Scissor::getUndefined(ctx->dispatcher.emulation);
      scissorStack.push_back(State::Scissor());
      scissorStack.back() = *this;
      mask &= ~GL_SCISSOR_BIT;
    }

    if (mask&GL_VIEWPORT_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_VIEWPORT_BIT ",State::Viewport::toString());
      if (!State::Viewport::fullyDefined())
        State::Viewport::getUndefined(ctx->dispatcher.emulation);
      viewportStack.push_back(State::Viewport());
      viewportStack.back() = *this;
      mask &= ~GL_VIEWPORT_BIT;
    }

    if (mask&GL_LINE_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_LINE_BIT ",State::Line::toString());
      lineStack.push_back(State::Line());
      lineStack.back() = *this;
      mask &= ~GL_LINE_BIT;
    }

    if (mask&GL_MULTISAMPLE_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_MULTISAMPLE_BIT ",State::Multisample::toString());
      multisampleStack.push_back(State::Multisample());
      multisampleStack.back() = *this;
      mask &= ~GL_MULTISAMPLE_BIT;
    }

    if (mask&GL_EVAL_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_EVAL_BIT ",State::Eval::toString());
      evalStack.push_back(State::Eval());
      evalStack.back() = *this;
      mask &= ~GL_EVAL_BIT;
    }

    if (mask&GL_FOG_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_FOG_BIT ",State::Fog::toString());
      fogStack.push_back(State::Fog());
      fogStack.back() = *this;
      mask &= ~GL_FOG_BIT;
    }

    if (mask&GL_POINT_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_POINT_BIT ",State::Point::toString());
      pointStack.push_back(State::Point());
      pointStack.back() = *this;
      mask &= ~GL_POINT_BIT;
    }

    if (mask&GL_POLYGON_STIPPLE_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_POLYGON_STIPPLE_BIT ",State::PolygonStipple::toString());
      polygonStippleStack.push_back(State::PolygonStipple());
      polygonStippleStack.back() = *this;
      mask &= ~GL_POLYGON_STIPPLE_BIT;
    }

    if (mask&GL_COLOR_BUFFER_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_COLOR_BUFFER_BIT ",State::ColorBuffer::toString());
      if (!State::ColorBuffer::fullyDefined())
        State::ColorBuffer::getUndefined(ctx->dispatcher.emulation);
      colorBufferStack.push_back(State::ColorBuffer());
      colorBufferStack.back() = *this;
      mask &= ~GL_COLOR_BUFFER_BIT;
    }

    if (mask&GL_PIXEL_MODE_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_PIXEL_MODE_BIT ",State::PixelMode::toString());
      if (!State::PixelMode::fullyDefined())
        State::PixelMode::getUndefined(ctx->dispatcher.emulation);
      pixelModeStack.push_back(State::PixelMode());
      pixelModeStack.back() = *this;
      mask &= ~GL_PIXEL_MODE_BIT;
    }

    if (mask&GL_LIGHTING_BIT)
    {
      Internal("Regal::Ppa::PushAttrib GL_LIGHTING_BIT ",State::Lighting::toString());
      lightingStack.push_back(State::Lighting());
      lightingStack.back() = *this;
      mask &= ~GL_LIGHTING_BIT;
    }

    // Pass the rest through, for now

    RegalAssert(ctx);

    if (ctx->info->core || ctx->info->es1 || ctx->info->es2)
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

        Internal("Regal::Ppa::PopAttrib GL_DEPTH_BUFFER_BIT ",State::Depth::toString());

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

        Internal("Regal::Ppa::PopAttrib GL_STENCIL_BUFFER_BIT ",State::Stencil::toString());

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

        Internal("Regal::Ppa::PopAttrib GL_POLYGON_BIT ",State::Polygon::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Polygon::set(ctx->dispatcher.emulation);

        mask &= ~GL_POLYGON_BIT;
      }

      if (mask&GL_TRANSFORM_BIT)
      {
        RegalAssert(transformStack.size());
        State::Transform::swap(transformStack.back());

        Internal("Regal::Ppa::PopAttrib GL_TRANSFORM_BIT ",State::Transform::toString());

        State::Transform::transition(ctx->dispatcher.emulation, transformStack.back());
        transformStack.pop_back();

        mask &= ~GL_TRANSFORM_BIT;
      }

      if (mask&GL_HINT_BIT)
      {
        RegalAssert(hintStack.size());
        State::Hint::swap(hintStack.back());
        hintStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_HINT_BIT ",State::Hint::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Hint::set(ctx->dispatcher.emulation);

        mask &= ~GL_HINT_BIT;
      }

      if (mask&GL_ENABLE_BIT)
      {
        RegalAssert(enableStack.size());
        State::Enable::swap(enableStack.back());
        enableStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_ENABLE_BIT ",State::Enable::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Enable::set(ctx->dispatcher.emulation);

        mask &= ~GL_ENABLE_BIT;
      }

      if (mask&GL_LIST_BIT)
      {
        RegalAssert(listStack.size());
        State::List::swap(listStack.back());
        listStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_LIST_BIT ",State::List::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::List::set(ctx->dispatcher.emulation);

        mask &= ~GL_LIST_BIT;
      }

      if (mask&GL_ACCUM_BUFFER_BIT)
      {
        RegalAssert(accumBufferStack.size());
        State::AccumBuffer::swap(accumBufferStack.back());
        accumBufferStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_ACCUM_BUFFER_BIT ",State::AccumBuffer::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::AccumBuffer::set(ctx->dispatcher.emulation);

        mask &= ~GL_ACCUM_BUFFER_BIT;
      }

      if (mask&GL_SCISSOR_BIT)
      {
        RegalAssert(scissorStack.size());
        State::Scissor::swap(scissorStack.back());
        scissorStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_SCISSOR_BIT ",State::Scissor::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        if (!State::Scissor::fullyDefined())
          State::Scissor::getUndefined(ctx->dispatcher.emulation);
        State::Scissor::set(ctx->dispatcher.emulation);

        mask &= ~GL_SCISSOR_BIT;
      }

      if (mask&GL_VIEWPORT_BIT)
      {
        RegalAssert(viewportStack.size());
        State::Viewport::swap(viewportStack.back());
        viewportStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_VIEWPORT_BIT ",State::Viewport::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        if (!State::Viewport::fullyDefined())
          State::Viewport::getUndefined(ctx->dispatcher.emulation);
        State::Viewport::set(ctx->dispatcher.emulation);

        mask &= ~GL_VIEWPORT_BIT;
      }

      if (mask&GL_LINE_BIT)
      {
        RegalAssert(lineStack.size());
        State::Line::swap(lineStack.back());
        lineStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_LINE_BIT ",State::Line::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Line::set(ctx->dispatcher.emulation);

        mask &= ~GL_LINE_BIT;
      }

      if (mask&GL_MULTISAMPLE_BIT)
      {
        RegalAssert(multisampleStack.size());
        State::Multisample::swap(multisampleStack.back());
        multisampleStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_MULTISAMPLE_BIT ",State::Multisample::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Multisample::set(ctx->dispatcher.emulation);

        mask &= ~GL_MULTISAMPLE_BIT;
      }

      if (mask&GL_EVAL_BIT)
      {
        RegalAssert(evalStack.size());
        State::Eval::swap(evalStack.back());
        evalStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_EVAL_BIT ",State::Eval::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Eval::set(ctx->dispatcher.emulation);

        mask &= ~GL_EVAL_BIT;
      }

      if (mask&GL_FOG_BIT)
      {
        RegalAssert(fogStack.size());
        State::Fog::swap(fogStack.back());
        fogStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_FOG_BIT ",State::Fog::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Fog::set(ctx->dispatcher.emulation);

        mask &= ~GL_FOG_BIT;
      }

      if (mask&GL_POINT_BIT)
      {
        RegalAssert(pointStack.size());
        State::Point::swap(pointStack.back());
        pointStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_POINT_BIT ",State::Point::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Point::set(ctx->dispatcher.emulation);

        mask &= ~GL_POINT_BIT;
      }

      if (mask&GL_POLYGON_STIPPLE_BIT)
      {
        RegalAssert(polygonStippleStack.size());
        State::PolygonStipple::swap(polygonStippleStack.back());
        polygonStippleStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_POLYGON_STIPPLE_BIT ",State::PolygonStipple::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::PolygonStipple::set(ctx->dispatcher.emulation);

        mask &= ~GL_POLYGON_STIPPLE_BIT;
      }

      if (mask&GL_COLOR_BUFFER_BIT)
      {
        RegalAssert(colorBufferStack.size());
        State::ColorBuffer::swap(colorBufferStack.back());
        colorBufferStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_COLOR_BUFFER_BIT ",State::ColorBuffer::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        if (!State::ColorBuffer::fullyDefined())
          State::ColorBuffer::getUndefined(ctx->dispatcher.emulation);
        State::ColorBuffer::set(ctx->dispatcher.emulation);

        mask &= ~GL_COLOR_BUFFER_BIT;
      }

      if (mask&GL_PIXEL_MODE_BIT)
      {
        RegalAssert(pixelModeStack.size());
        State::PixelMode::swap(pixelModeStack.back());
        pixelModeStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_PIXEL_MODE_BIT ",State::PixelMode::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        if (!State::PixelMode::fullyDefined())
          State::PixelMode::getUndefined(ctx->dispatcher.emulation);
        State::PixelMode::set(ctx->dispatcher.emulation);

        mask &= ~GL_PIXEL_MODE_BIT;
      }

      if (mask&GL_LIGHTING_BIT)
      {
        RegalAssert(lightingStack.size());
        State::Lighting::swap(lightingStack.back());
        lightingStack.pop_back();

        Internal("Regal::Ppa::PopAttrib GL_LIGHTING_BIT ",State::Lighting::toString());

        // Ideally we'd only set the state that has changed
        // since the glPushAttrib() - revisit

        State::Lighting::set(ctx->dispatcher.emulation);

        mask &= ~GL_LIGHTING_BIT;
      }

      // Pass the rest through, for now

      if (ctx->info->core || ctx->info->es1 || ctx->info->es2)
        return;

      if (mask)
        ctx->dispatcher.emulation.glPopAttrib();
    }
  }

  template <typename T> bool glGetv(RegalContext *ctx, GLenum pname, T *params)
  {
    RegalAssert(ctx);

    if (ctx->info->core || ctx->info->es1 || ctx->info->es2)
    {
      switch (pname)
      {
        case GL_ACCUM_RED_BITS:
        case GL_ACCUM_GREEN_BITS:
        case GL_ACCUM_BLUE_BITS:
        case GL_ACCUM_ALPHA_BITS:
          if (params)
          {
            params[0] = 32;
          }
          break;

        case GL_RED_BITS:
        case GL_GREEN_BITS:
        case GL_BLUE_BITS:
        case GL_ALPHA_BITS:
        case GL_INDEX_BITS:
        case GL_STENCIL_BITS:
          if (params)
          {
            params[0] = 8;
          }
          break;

        case GL_DEPTH_BITS:
          if (params)
          {
            params[0] = 24;
          }
          break;

        case GL_AUX_BUFFERS:
        case GL_LINE_STIPPLE:
        case GL_MAX_PIXEL_MAP_TABLE:
        case GL_MAX_NAME_STACK_DEPTH:
        case GL_MAX_LIST_NESTING:
        case GL_MAX_EVAL_ORDER:
          if (params)
            params[0] = 0;
          break;
        case GL_ATTRIB_STACK_DEPTH:
          if (params)
            params[0] = static_cast<T>(maskStack.size());
          break;
        case GL_MAX_ATTRIB_STACK_DEPTH:
          if (params)
            params[0] = static_cast<T>(ctx->info->max_attrib_stack_depth);
          break;
        default:
          return false;
      }
      return true;
    }

    switch (pname)
    {
      case GL_ACCUM_CLEAR_VALUE:
        params[0] = static_cast<T>(State::AccumBuffer::clear[0]);
        params[1] = static_cast<T>(State::AccumBuffer::clear[1]);
        params[2] = static_cast<T>(State::AccumBuffer::clear[2]);
        params[3] = static_cast<T>(State::AccumBuffer::clear[3]);
        break;
      case GL_ALPHA_BIAS:
        params[0] = static_cast<T>(State::PixelMode::alphaBias);
        break;
      case GL_ALPHA_SCALE:
        params[0] = static_cast<T>(State::PixelMode::alphaScale);
        break;
      case GL_ALPHA_TEST_FUNC:
        params[0] = static_cast<T>(State::ColorBuffer::alphaTestFunc);
        break;
      case GL_ALPHA_TEST_REF:
        params[0] = static_cast<T>(State::ColorBuffer::alphaTestRef);
        break;
      case GL_BLEND_COLOR:
        params[0] = static_cast<T>(State::ColorBuffer::blendColor[0]);
        params[1] = static_cast<T>(State::ColorBuffer::blendColor[1]);
        params[2] = static_cast<T>(State::ColorBuffer::blendColor[2]);
        params[3] = static_cast<T>(State::ColorBuffer::blendColor[3]);
        break;
      case GL_BLUE_BIAS:
        params[0] = static_cast<T>(State::PixelMode::blueBias);
        break;
      case GL_BLUE_SCALE:
        params[0] = static_cast<T>(State::PixelMode::blueScale);
        break;
      case GL_CLAMP_FRAGMENT_COLOR:
        params[0] = static_cast<T>(State::Enable::clampFragmentColor);
        break;
      case GL_CLAMP_READ_COLOR:
        params[0] = static_cast<T>(State::Enable::clampReadColor);
        break;
      case GL_CLAMP_VERTEX_COLOR:
        params[0] = static_cast<T>(State::Enable::clampVertexColor);
        break;
      case GL_COLOR_CLEAR_VALUE:
        params[0] = static_cast<T>(State::ColorBuffer::colorClearValue[0]);
        params[1] = static_cast<T>(State::ColorBuffer::colorClearValue[1]);
        params[2] = static_cast<T>(State::ColorBuffer::colorClearValue[2]);
        params[3] = static_cast<T>(State::ColorBuffer::colorClearValue[3]);
        break;
      case GL_COLOR_MATERIAL_FACE:
        params[0] = static_cast<T>(State::Lighting::colorMaterialFace);
        break;
      case GL_COLOR_MATERIAL_PARAMETER:
        params[0] = static_cast<T>(State::Lighting::colorMaterialParameter);
        break;
      case GL_CULL_FACE_MODE:
        params[0] = static_cast<T>(State::Polygon::cullFaceMode);
        break;
      case GL_DEPTH_CLEAR_VALUE:
        params[0] = static_cast<T>(State::Depth::clear);
        break;
      case GL_DEPTH_FUNC:
        params[0] = static_cast<T>(State::Depth::func);
        break;
      case GL_DEPTH_WRITEMASK:
        params[0] = static_cast<T>(State::Depth::mask);
        break;
      case GL_DRAW_BUFFER0:
      case GL_DRAW_BUFFER1:
      case GL_DRAW_BUFFER2:
      case GL_DRAW_BUFFER3:
      case GL_DRAW_BUFFER4:
      case GL_DRAW_BUFFER5:
      case GL_DRAW_BUFFER6:
      case GL_DRAW_BUFFER7:
      case GL_DRAW_BUFFER8:
      case GL_DRAW_BUFFER9:
      case GL_DRAW_BUFFER10:
      case GL_DRAW_BUFFER11:
      case GL_DRAW_BUFFER12:
      case GL_DRAW_BUFFER13:
      case GL_DRAW_BUFFER14:
      case GL_DRAW_BUFFER15:
      {
        GLuint index = static_cast<GLuint>(pname - GL_DRAW_BUFFER0);
        if ( index < array_size( State::ColorBuffer::drawBuffers ))
        {
          if (!State::ColorBuffer::fullyDefined())
            State::ColorBuffer::getUndefined(ctx->dispatcher.emulation);
          RegalAssertArrayIndex( State::ColorBuffer::drawBuffers, index );
          params[0] = static_cast<T>(State::ColorBuffer::drawBuffers[index]);
        }
      }
      break;
      case GL_FOG_COLOR:
        params[0] = static_cast<T>(State::Fog::color[0]);
        params[1] = static_cast<T>(State::Fog::color[1]);
        params[2] = static_cast<T>(State::Fog::color[2]);
        params[3] = static_cast<T>(State::Fog::color[3]);
        break;
      case GL_FOG_COORD_SRC:
        params[0] = static_cast<T>(State::Fog::coordSrc);
        break;
      case GL_FOG_DENSITY:
        params[0] = static_cast<T>(State::Fog::density);
        break;
      case GL_FOG_END:
        params[0] = static_cast<T>(State::Fog::end);
        break;
      case GL_FOG_HINT:
        params[0] = static_cast<T>(State::Hint::fog);
        break;
      case GL_FOG_INDEX:
        params[0] = static_cast<T>(State::Fog::index);
        break;
      case GL_FOG_MODE:
        params[0] = static_cast<T>(State::Fog::mode);
        break;
      case GL_FOG_START:
        params[0] = static_cast<T>(State::Fog::start);
        break;
      case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
        params[0] = static_cast<T>(State::Hint::fragmentShaderDerivative);
        break;
      case GL_FRONT_FACE:
        params[0] = static_cast<T>(State::Polygon::frontFace);
        break;
      case GL_GENERATE_MIPMAP_HINT:
        params[0] = static_cast<T>(State::Hint::generateMipmap);
        break;
      case GL_GREEN_BIAS:
        params[0] = static_cast<T>(State::PixelMode::greenBias);
        break;
      case GL_GREEN_SCALE:
        params[0] = static_cast<T>(State::PixelMode::greenScale);
        break;
      case GL_INDEX_CLEAR_VALUE:
        params[0] = static_cast<T>(State::ColorBuffer::indexClearValue);
        break;
      case GL_INDEX_OFFSET:
        params[0] = static_cast<T>(State::PixelMode::indexOffset);
        break;
      case GL_INDEX_SHIFT:
        params[0] = static_cast<T>(State::PixelMode::indexShift);
        break;
      case GL_INDEX_WRITEMASK:
        params[0] = static_cast<T>(State::ColorBuffer::indexWritemask);
        break;
      case GL_LIGHT_MODEL_AMBIENT:
        params[0] = static_cast<T>(State::Lighting::lightModelAmbient[0]);
        params[1] = static_cast<T>(State::Lighting::lightModelAmbient[1]);
        params[2] = static_cast<T>(State::Lighting::lightModelAmbient[2]);
        params[3] = static_cast<T>(State::Lighting::lightModelAmbient[3]);
        break;
      case GL_LIGHT_MODEL_COLOR_CONTROL:
        params[0] = static_cast<T>(State::Lighting::lightModelColorControl);
        break;
      case GL_LIGHT_MODEL_LOCAL_VIEWER:
        params[0] = static_cast<T>(State::Lighting::lightModelLocalViewer);
        break;
      case GL_LIGHT_MODEL_TWO_SIDE:
        params[0] = static_cast<T>(State::Lighting::lightModelTwoSide);
        break;
      case GL_LINE_SMOOTH_HINT:
        params[0] = static_cast<T>(State::Hint::lineSmooth);
        break;
      case GL_LINE_STIPPLE_PATTERN:
        params[0] = static_cast<T>(State::Line::stipplePattern);
        break;
      case GL_LINE_STIPPLE_REPEAT:
        params[0] = static_cast<T>(State::Line::stippleRepeat);
        break;
      case GL_LINE_WIDTH:
        params[0] = static_cast<T>(State::Line::width);
        break;
      case GL_LIST_BASE:
        params[0] = static_cast<T>(State::List::base);
        break;
      case GL_LOGIC_OP_MODE:
        params[0] = static_cast<T>(State::ColorBuffer::logicOpMode);
        break;
      case GL_MAP1_GRID_DOMAIN:
        params[0] = static_cast<T>(State::Eval::map1GridDomain[0]);
        params[1] = static_cast<T>(State::Eval::map1GridDomain[1]);
        break;
      case GL_MAP1_GRID_SEGMENTS:
        params[0] = static_cast<T>(State::Eval::map1GridSegments);
        break;
      case GL_MAP2_GRID_DOMAIN:
        params[0] = static_cast<T>(State::Eval::map2GridDomain[0]);
        params[1] = static_cast<T>(State::Eval::map2GridDomain[1]);
        params[2] = static_cast<T>(State::Eval::map2GridDomain[2]);
        params[3] = static_cast<T>(State::Eval::map2GridDomain[3]);
        break;
      case GL_MAP2_GRID_SEGMENTS:
        params[0] = static_cast<T>(State::Eval::map2GridSegments[0]);
        params[1] = static_cast<T>(State::Eval::map2GridSegments[1]);
        break;
      case GL_MAP_COLOR:
        params[0] = static_cast<T>(State::PixelMode::mapColor);
        break;
      case GL_ATTRIB_STACK_DEPTH:
        params[0] = static_cast<T>(maskStack.size());
        break;
      case GL_MAX_ATTRIB_STACK_DEPTH:
        params[0] = static_cast<T>(ctx->info->max_attrib_stack_depth);
        break;
      case GL_MAX_VERTEX_ATTRIBS:
        *params = static_cast<T>(ctx->info->max_vertex_attribs);
        break;
      case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        params[0] = static_cast<T>(ctx->info->max_combined_texture_image_units);
        break;
      case GL_MAX_TEXTURE_COORDS:
        params[0] = static_cast<T>(ctx->info->max_texture_coords);
        break;
      case GL_MAX_TEXTURE_UNITS:
        params[0] = static_cast<T>(ctx->info->max_texture_units);
        break;
      case GL_MAP_STENCIL:
        params[0] = static_cast<T>(State::PixelMode::mapStencil);
        break;
      case GL_MIN_SAMPLE_SHADING_VALUE:
        params[0] = static_cast<T>(State::Multisample::minSampleShadingValue);
        break;
      case GL_PERSPECTIVE_CORRECTION_HINT:
        params[0] = static_cast<T>(State::Hint::perspectiveCorrection);
        break;
      case GL_POINT_DISTANCE_ATTENUATION:
        params[0] = static_cast<T>(State::Point::distanceAttenuation[0]);
        params[1] = static_cast<T>(State::Point::distanceAttenuation[1]);
        params[2] = static_cast<T>(State::Point::distanceAttenuation[2]);
        break;
      case GL_POINT_FADE_THRESHOLD_SIZE:
        params[0] = static_cast<T>(State::Point::fadeThresholdSize);
        break;
      case GL_POINT_SIZE_MAX:
        params[0] = static_cast<T>(State::Point::sizeMax);
        break;
      case GL_POINT_SIZE_MIN:
        params[0] = static_cast<T>(State::Point::sizeMin);
        break;
      case GL_POINT_SIZE:
        params[0] = static_cast<T>(State::Point::size);
        break;
      case GL_POINT_SMOOTH_HINT:
        params[0] = static_cast<T>(State::Hint::pointSmooth);
        break;
      case GL_POINT_SPRITE_COORD_ORIGIN:
        params[0] = static_cast<T>(State::Point::spriteCoordOrigin);
        break;
      case GL_POLYGON_MODE:
        params[0] = static_cast<T>(State::Polygon::mode[0]);
        params[1] = static_cast<T>(State::Polygon::mode[1]);
        break;
      case GL_POLYGON_OFFSET_FACTOR:
        params[0] = static_cast<T>(State::Polygon::factor);
        break;
      case GL_POLYGON_OFFSET_UNITS:
        params[0] = static_cast<T>(State::Polygon::units);
        break;
      case GL_POLYGON_SMOOTH_HINT:
        params[0] = static_cast<T>(State::Hint::polygonSmooth);
        break;
      case GL_POST_COLOR_MATRIX_ALPHA_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixAlphaBias);
        break;
      case GL_POST_COLOR_MATRIX_ALPHA_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixAlphaScale);
        break;
      case GL_POST_COLOR_MATRIX_BLUE_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixBlueBias);
        break;
      case GL_POST_COLOR_MATRIX_BLUE_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixBlueScale);
        break;
      case GL_POST_COLOR_MATRIX_GREEN_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixGreenBias);
        break;
      case GL_POST_COLOR_MATRIX_GREEN_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixGreenScale);
        break;
      case GL_POST_COLOR_MATRIX_RED_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixRedBias);
        break;
      case GL_POST_COLOR_MATRIX_RED_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postColorMatrixRedScale);
        break;
      case GL_POST_CONVOLUTION_ALPHA_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionAlphaBias);
        break;
      case GL_POST_CONVOLUTION_ALPHA_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionAlphaScale);
        break;
      case GL_POST_CONVOLUTION_BLUE_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionBlueBias);
        break;
      case GL_POST_CONVOLUTION_BLUE_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionBlueScale);
        break;
      case GL_POST_CONVOLUTION_GREEN_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionGreenBias);
        break;
      case GL_POST_CONVOLUTION_GREEN_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionGreenScale);
        break;
      case GL_POST_CONVOLUTION_RED_BIAS:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionRedBias);
        break;
      case GL_POST_CONVOLUTION_RED_SCALE:
        params[0] = static_cast<T>(State::PixelMode::postConvolutionRedScale);
        break;
      case GL_PROVOKING_VERTEX:
        params[0] = static_cast<T>(State::Lighting::provokingVertex);
        break;
      case GL_READ_BUFFER:
      {
        if (!State::PixelMode::fullyDefined())
          State::PixelMode::getUndefined(ctx->dispatcher.emulation);
        params[0] = static_cast<T>(State::PixelMode::readBuffer);
      }
      break;
      case GL_RED_BIAS:
        params[0] = static_cast<T>(State::PixelMode::redBias);
        break;
      case GL_RED_SCALE:
        params[0] = static_cast<T>(State::PixelMode::redScale);
        break;
      case GL_SAMPLE_COVERAGE_INVERT:
        params[0] = static_cast<T>(State::Multisample::sampleCoverageInvert);
        break;
      case GL_SAMPLE_COVERAGE_VALUE:
        params[0] = static_cast<T>(State::Multisample::sampleCoverageValue);
        break;
      case GL_SHADE_MODEL:
        params[0] = static_cast<T>(State::Lighting::shadeModel);
        break;
      case GL_STENCIL_BACK_FAIL:
        params[0] = static_cast<T>(State::Stencil::back.fail);
        break;
      case GL_STENCIL_BACK_FUNC:
        params[0] = static_cast<T>(State::Stencil::back.func);
        break;
      case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
        params[0] = static_cast<T>(State::Stencil::back.zfail);
        break;
      case GL_STENCIL_BACK_PASS_DEPTH_PASS:
        params[0] = static_cast<T>(State::Stencil::back.zpass);
        break;
      case GL_STENCIL_BACK_REF:
        params[0] = static_cast<T>(State::Stencil::back.ref);
        break;
      case GL_STENCIL_BACK_VALUE_MASK:
        params[0] = static_cast<T>(State::Stencil::back.valueMask);
        break;
      case GL_STENCIL_BACK_WRITEMASK:
        params[0] = static_cast<T>(State::Stencil::back.writeMask);
        break;
      case GL_STENCIL_CLEAR_VALUE:
        params[0] = static_cast<T>(State::Stencil::clear);
        break;
      case GL_STENCIL_FAIL:
        params[0] = static_cast<T>(State::Stencil::front.fail);
        break;
      case GL_STENCIL_FUNC:
        params[0] = static_cast<T>(State::Stencil::front.func);
        break;
      case GL_STENCIL_PASS_DEPTH_FAIL:
        params[0] = static_cast<T>(State::Stencil::front.zfail);
        break;
      case GL_STENCIL_PASS_DEPTH_PASS:
        params[0] = static_cast<T>(State::Stencil::front.zpass);
        break;
      case GL_STENCIL_REF:
        params[0] = static_cast<T>(State::Stencil::front.ref);
        break;
      case GL_STENCIL_VALUE_MASK:
        params[0] = static_cast<T>(State::Stencil::front.valueMask);
        break;
      case GL_STENCIL_WRITEMASK:
        params[0] = static_cast<T>(State::Stencil::front.writeMask);
        break;
      case GL_TEXTURE_COMPRESSION_HINT:
        params[0] = static_cast<T>(State::Hint::textureCompression);
        break;
      case GL_ZOOM_X:
        params[0] = static_cast<T>(State::PixelMode::zoomX);
        break;
      case GL_ZOOM_Y:
        params[0] = static_cast<T>(State::PixelMode::zoomY);
        break;

      default:
        return false;
    }
    return true;
  }

  template <typename T> bool glGeti_v(RegalContext *ctx, GLenum pname, GLuint index, T *params)
  {
    UNUSED_PARAMETER(ctx);
    switch (pname)
    {
      case GL_BLEND_DST_ALPHA:
        if (index < array_size( State::ColorBuffer::blendDstAlpha ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendDstAlpha, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendDstAlpha[index]);
        }
        break;
      case GL_BLEND_DST_RGB:
        if (index < array_size( State::ColorBuffer::blendDstRgb ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendDstRgb, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendDstRgb[index]);
        }
        break;
      case GL_BLEND_EQUATION_ALPHA:
        if (index < array_size( State::ColorBuffer::blendEquationAlpha ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendEquationAlpha, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendEquationAlpha[index]);
        }
        break;
      case GL_BLEND_EQUATION_RGB:
        if (index < array_size( State::ColorBuffer::blendEquationRgb ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendEquationRgb, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendEquationRgb[index]);
        }
        break;
      case GL_BLEND_SRC_ALPHA:
        if (index < array_size( State::ColorBuffer::blendSrcAlpha ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendSrcAlpha, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendSrcAlpha[index]);
        }
        break;
      case GL_BLEND_SRC_RGB:
        if (index < array_size( State::ColorBuffer::blendSrcRgb ))
        {
          RegalAssertArrayIndex( State::ColorBuffer::blendSrcRgb, index );
          params[0] = static_cast<T>(State::ColorBuffer::blendSrcRgb[index]);
        }
        break;
      case GL_COLOR_WRITEMASK:
        if (index < ctx->info->max_draw_buffers)
        {
          params[0] = static_cast<T>(State::ColorBuffer::colorWritemask[index][0]);
          params[1] = static_cast<T>(State::ColorBuffer::colorWritemask[index][1]);
          params[2] = static_cast<T>(State::ColorBuffer::colorWritemask[index][2]);
          params[3] = static_cast<T>(State::ColorBuffer::colorWritemask[index][3]);
        }
        break;
      case GL_DEPTH_RANGE:
        if (index < ctx->info->max_viewports)
        {
          params[0] = static_cast<T>(State::Viewport::depthRange[index][0]);
          params[1] = static_cast<T>(State::Viewport::depthRange[index][1]);
        }
        break;
      case GL_SCISSOR_BOX:
        if (index < ctx->info->max_viewports)
        {
          if (!State::Scissor::fullyDefined())
            State::Scissor::getUndefined(ctx->dispatcher.emulation);
          params[0] = static_cast<T>(State::Scissor::scissorBox[index][0]);
          params[1] = static_cast<T>(State::Scissor::scissorBox[index][1]);
          params[2] = static_cast<T>(State::Scissor::scissorBox[index][2]);
          params[3] = static_cast<T>(State::Scissor::scissorBox[index][3]);
        }
        break;
      case GL_VIEWPORT:
        if (index < ctx->info->max_viewports)
        {
          if (!State::Viewport::fullyDefined())
            State::Viewport::getUndefined(ctx->dispatcher.emulation);
          params[0] = static_cast<T>(State::Viewport::viewport[index][0]);
          params[1] = static_cast<T>(State::Viewport::viewport[index][1]);
          params[2] = static_cast<T>(State::Viewport::viewport[index][2]);
          params[3] = static_cast<T>(State::Viewport::viewport[index][3]);
        }
        break;
      default:
        return false;
    }
    return true;
  }

  bool glGetPolygonStipple(RegalContext *ctx, GLubyte *pattern)
  {
    UNUSED_PARAMETER(ctx);

    // If a non-zero named buffer object is bound to the GL_PIXEL_PACK_BUFFER target
    // (see glBindBuffer) while a polygon stipple pattern is requested, pattern is
    // treated as a byte offset into the buffer object's data store.
    //
    // TODO: need to handle this case, dammit. but for now just...

    memcpy(pattern, State::PolygonStipple::pattern, (32*4)*sizeof(GLubyte));

    return true;
  }

  template <typename T> bool glGetColorTableParameterv(RegalContext *ctx, GLenum target, GLenum pname, T *params)
  {
    UNUSED_PARAMETER(ctx);

    GLuint index = 0;
    switch (pname)
    {
      case GL_COLOR_TABLE:
        index = 0;
        break;
      case GL_POST_CONVOLUTION_COLOR_TABLE:
        index = 1;
        break;
      case GL_POST_COLOR_MATRIX_COLOR_TABLE:
        index = 2;
        break;
      default:
        return false;
    }

    GLfloat *p = NULL;
    switch (target)
    {
      case GL_COLOR_TABLE_SCALE:
        p = &State::PixelMode::colorTableScale[index][0];
        break;
      case GL_COLOR_TABLE_BIAS:
        p = &State::PixelMode::colorTableBias[index][0];
        break;
      default:
        return false;
    }

    params[0] = static_cast<T>(p[0]);
    params[1] = static_cast<T>(p[1]);
    params[2] = static_cast<T>(p[2]);
    params[3] = static_cast<T>(p[3]);

    return true;
  }

  template <typename T> bool glGetConvolutionParameterv(RegalContext *ctx, GLenum target, GLenum pname, T *params)
  {
    UNUSED_PARAMETER(ctx);

    GLuint index = 0;
    switch (pname)
    {
      case GL_CONVOLUTION_1D:
        index = 0;
        break;
      case GL_CONVOLUTION_2D:
        index = 1;
        break;
      case GL_SEPARABLE_2D:
        index = 2;
        break;
      default:
        return false;
    }

    if (target == GL_CONVOLUTION_BORDER_MODE)
    {
      params[0] = static_cast<T>(State::PixelMode::convolutionBorderMode[index]);
      return true;
    }

    GLfloat *p = NULL;
    switch (target)
    {
      case GL_CONVOLUTION_BORDER_COLOR:
        p = &State::PixelMode::convolutionBorderColor[index][0];
        break;
      case GL_CONVOLUTION_FILTER_SCALE:
        p = &State::PixelMode::convolutionFilterScale[index][0];
        break;
      case GL_CONVOLUTION_FILTER_BIAS:
        p = &State::PixelMode::convolutionFilterBias[index][0];
        break;
      default:
        return false;
    }

    params[0] = static_cast<T>(p[0]);
    params[1] = static_cast<T>(p[1]);
    params[2] = static_cast<T>(p[2]);
    params[3] = static_cast<T>(p[3]);

    return true;
  }

  template <typename T> bool glGetLightv(RegalContext *ctx, GLenum light, GLenum pname, T *params)
  {
    UNUSED_PARAMETER(ctx);

    GLint ii = light - GL_LIGHT0;
    if (ii < 0 || static_cast<size_t>(ii) >= array_size( State::Point::coordReplace ))
      return false;

    RegalAssertArrayIndex( State::Lighting::lights, ii );
    State::LightingLight l = State::Lighting::lights[ii];

    GLuint num = 0;
    GLfloat *p = NULL;
    switch (pname)
    {
      case GL_AMBIENT:
        num = 4;
        p = &l.ambient[0];
        break;
      case GL_DIFFUSE:
        num = 4;
        p = &l.diffuse[0];
        break;
      case GL_SPECULAR:
        num = 4;
        p = &l.specular[0];
        break;
      case GL_POSITION:
        num = 4;
        p = &l.position[0];
        break;
      case GL_CONSTANT_ATTENUATION:
        num = 1;
        p = &l.constantAttenuation;
        break;
      case GL_LINEAR_ATTENUATION:
        num = 1;
        p = &l.linearAttenuation;
        break;
      case GL_QUADRATIC_ATTENUATION:
        num = 1;
        p = &l.quadraticAttenuation;
        break;
      case GL_SPOT_DIRECTION:
        num = 3;
        p = &l.spotDirection[0];
        break;
      case GL_SPOT_EXPONENT:
        num = 1;
        p = &l.spotExponent;
        break;
      case GL_SPOT_CUTOFF:
        num = 1;
        p = &l.spotCutoff;
        break;
      default:
        return false;
    }

    for (size_t ii = 0; ii < num; ii++)
      params[ii] = static_cast<T>(p[ii]);

    return true;
  }

  template <typename T> bool glGetMaterialv(RegalContext *ctx, GLenum face, GLenum pname, T *params)
  {
    UNUSED_PARAMETER(ctx);

    if (face != GL_FRONT && face != GL_BACK)
      return false;

    State::LightingFace &f = (face == GL_FRONT) ? State::Lighting::front : State::Lighting::back;

    GLuint num = 0;
    GLfloat *p = NULL;
    switch (pname)
    {
      case GL_AMBIENT:
        num = 4;
        p = &f.ambient[0];
        break;
      case GL_DIFFUSE:
        num = 4;
        p = &f.diffuse[0];
        break;
      case GL_SPECULAR:
        num = 4;
        p = &f.specular[0];
        break;
      case GL_EMISSION:
        num = 4;
        p = &f.emission[0];
        break;
      case GL_SHININESS:
        num = 1;
        p = &f.shininess;
        break;
      case GL_COLOR_INDEXES:
        num = 3;
        p = &f.colorIndexes[0];
        break;
      default:
        return false;
    }

    for (size_t ii = 0; ii < num; ii++)
      params[ii] = static_cast<T>(p[ii]);

    return true;
  }

  template <typename T> bool glGetMultiTexEnvv(RegalContext *ctx, GLenum texunit, GLenum target, GLenum pname, T *params)
  {
    UNUSED_PARAMETER(ctx);

    if (target != GL_POINT_SPRITE || pname != GL_COORD_REPLACE)
      return false;

    GLint ii = texunit - GL_TEXTURE0;
    if (ii < 0 || static_cast<size_t>(ii) >= array_size( State::Point::coordReplace ))
      return false;

    RegalAssertArrayIndex( State::Point::coordReplace, ii );
    *params = static_cast<T>(State::Point::coordReplace[ii]);

    return true;
  }

  template <typename T> bool glGetTexEnvv(RegalContext *ctx, GLenum target, GLenum pname, T *params)
  {
    return glGetMultiTexEnvv(ctx, GL_TEXTURE0+activeTextureUnit, target, pname, params);
  }

  bool glIsEnabled(RegalContext *ctx, GLboolean &enabled, GLenum pname)
  {
    UNUSED_PARAMETER(ctx);

    switch (pname)
    {
      case GL_ALPHA_TEST:
        enabled = State::Enable::alphaTest;
        break;
      case GL_BLEND:
        enabled = State::Enable::blend[0];
        break;
      case GL_AUTO_NORMAL:
        enabled = State::Enable::autoNormal;
        break;
      case GL_CLIP_DISTANCE0:
      case GL_CLIP_DISTANCE1:
      case GL_CLIP_DISTANCE2:
      case GL_CLIP_DISTANCE3:
      case GL_CLIP_DISTANCE4:
      case GL_CLIP_DISTANCE5:
      case GL_CLIP_DISTANCE6:
      case GL_CLIP_DISTANCE7:
        RegalAssertArrayIndex( State::Enable::clipDistance, pname-GL_CLIP_DISTANCE0 );
        enabled = State::Enable::clipDistance[pname-GL_CLIP_DISTANCE0];
        break;
      case GL_COLOR_LOGIC_OP:
        enabled = State::Enable::colorLogicOp;
        break;
      case GL_COLOR_MATERIAL:
        enabled = State::Enable::colorMaterial;
        break;
      case GL_COLOR_SUM:
        enabled = State::Enable::colorSum;
        break;
      case GL_COLOR_TABLE:
        enabled = State::Enable::colorTable;
        break;
      case GL_CONVOLUTION_1D:
        enabled = State::Enable::convolution1d;
        break;
      case GL_CONVOLUTION_2D:
        enabled = State::Enable::convolution2d;
        break;
      case GL_CULL_FACE:
        enabled = State::Enable::cullFace;
        break;
      case GL_DEPTH_CLAMP:
        enabled = State::Enable::depthClamp;
        break;
      case GL_DEPTH_TEST:
        enabled = State::Enable::depthTest;
        break;
      case GL_DITHER:
        enabled = State::Enable::dither;
        break;
      case GL_FOG:
        enabled = State::Enable::fog;
        break;
      case GL_FRAMEBUFFER_SRGB:
        enabled = State::Enable::framebufferSRGB;
        break;
      case GL_HISTOGRAM:
        enabled = State::Enable::histogram;
        break;
      case GL_INDEX_LOGIC_OP:
        enabled = State::Enable::indexLogicOp;
        break;
      case GL_LIGHT0:
      case GL_LIGHT1:
      case GL_LIGHT2:
      case GL_LIGHT3:
      case GL_LIGHT4:
      case GL_LIGHT5:
      case GL_LIGHT6:
      case GL_LIGHT7:
        RegalAssertArrayIndex( State::Enable::light, pname-GL_LIGHT0 );
        enabled = State::Enable::light[pname-GL_LIGHT0];
        break;
      case GL_LIGHTING:
        enabled = State::Enable::lighting;
        break;
      case GL_LINE_SMOOTH:
        enabled = State::Enable::lineSmooth;
        break;
      case GL_LINE_STIPPLE:
        enabled = State::Enable::lineStipple;
        break;
      case GL_MAP1_COLOR_4:
        enabled = State::Enable::map1Color4;
        break;
      case GL_MAP1_INDEX:
        enabled = State::Enable::map1Index;
        break;
      case GL_MAP1_NORMAL:
        enabled = State::Enable::map1Normal;
        break;
      case GL_MAP1_TEXTURE_COORD_1:
        enabled = State::Enable::map1TextureCoord1;
        break;
      case GL_MAP1_TEXTURE_COORD_2:
        enabled = State::Enable::map1TextureCoord2;
        break;
      case GL_MAP1_TEXTURE_COORD_3:
        enabled = State::Enable::map1TextureCoord3;
        break;
      case GL_MAP1_TEXTURE_COORD_4:
        enabled = State::Enable::map1TextureCoord4;
        break;
      case GL_MAP1_VERTEX_3:
        enabled = State::Enable::map1Vertex3;
        break;
      case GL_MAP1_VERTEX_4:
        enabled = State::Enable::map1Vertex4;
        break;
      case GL_MAP2_COLOR_4:
        enabled = State::Enable::map2Color4;
        break;
      case GL_MAP2_INDEX:
        enabled = State::Enable::map2Index;
        break;
      case GL_MAP2_NORMAL:
        enabled = State::Enable::map2Normal;
        break;
      case GL_MAP2_TEXTURE_COORD_1:
        enabled = State::Enable::map2TextureCoord1;
        break;
      case GL_MAP2_TEXTURE_COORD_2:
        enabled = State::Enable::map2TextureCoord2;
        break;
      case GL_MAP2_TEXTURE_COORD_3:
        enabled = State::Enable::map2TextureCoord3;
        break;
      case GL_MAP2_TEXTURE_COORD_4:
        enabled = State::Enable::map2TextureCoord4;
        break;
      case GL_MAP2_VERTEX_3:
        enabled = State::Enable::map2Vertex3;
        break;
      case GL_MAP2_VERTEX_4:
        enabled = State::Enable::map2Vertex4;
        break;
      case GL_MINMAX:
        enabled = State::Enable::minmax;
        break;
      case GL_MULTISAMPLE:
        enabled = State::Enable::multisample;
        break;
      case GL_NORMALIZE:
        enabled = State::Enable::normalize;
        break;
      case GL_POINT_SMOOTH:
        enabled = State::Enable::pointSmooth;
        break;
      case GL_POINT_SPRITE:
        enabled = State::Enable::pointSprite;
        break;
      case GL_POLYGON_OFFSET_FILL:
        enabled = State::Enable::polygonOffsetFill;
        break;
      case GL_POLYGON_OFFSET_LINE:
        enabled = State::Enable::polygonOffsetLine;
        break;
      case GL_POLYGON_OFFSET_POINT:
        enabled = State::Enable::polygonOffsetPoint;
        break;
      case GL_POLYGON_SMOOTH:
        enabled = State::Enable::polygonSmooth;
        break;
      case GL_POLYGON_STIPPLE:
        enabled = State::Enable::polygonStipple;
        break;
      case GL_POST_COLOR_MATRIX_COLOR_TABLE:
        enabled = State::Enable::postColorMatrixColorTable;
        break;
      case GL_POST_CONVOLUTION_COLOR_TABLE:
        enabled = State::Enable::postConvolutionColorTable;
        break;
      case GL_PROGRAM_POINT_SIZE:
        enabled = State::Enable::programPointSize;
        break;
      case GL_RESCALE_NORMAL:
        enabled = State::Enable::rescaleNormal;
        break;
      case GL_SAMPLE_ALPHA_TO_COVERAGE:
        enabled = State::Enable::sampleAlphaToCoverage;
        break;
      case GL_SAMPLE_ALPHA_TO_ONE:
        enabled = State::Enable::sampleAlphaToOne;
        break;
      case GL_SAMPLE_COVERAGE:
        enabled = State::Enable::sampleCoverage;
        break;
      case GL_SAMPLE_SHADING:
        enabled = State::Enable::sampleShading;
        break;
      case GL_SCISSOR_TEST:
        enabled = State::Enable::scissorTest[0];
        break;
      case GL_SEPARABLE_2D:
        enabled = State::Enable::separable2d;
        break;
      case GL_STENCIL_TEST:
        enabled = State::Enable::stencilTest;
        break;
      case GL_TEXTURE_1D:
        RegalAssertArrayIndex( State::Enable::texture1d, activeTextureUnit );
        enabled = State::Enable::texture1d[activeTextureUnit];
        break;
      case GL_TEXTURE_2D:
        RegalAssertArrayIndex( State::Enable::texture2d, activeTextureUnit );
        enabled = State::Enable::texture2d[activeTextureUnit];
        break;
      case GL_TEXTURE_3D:
        RegalAssertArrayIndex( State::Enable::texture3d, activeTextureUnit );
        enabled = State::Enable::texture3d[activeTextureUnit];
        break;
      case GL_TEXTURE_CUBE_MAP:
        RegalAssertArrayIndex( State::Enable::textureCubeMap, activeTextureUnit );
        enabled = State::Enable::textureCubeMap[activeTextureUnit];
        break;
      case GL_TEXTURE_RECTANGLE:
        RegalAssertArrayIndex( State::Enable::textureRectangle, activeTextureUnit );
        enabled = State::Enable::textureRectangle[activeTextureUnit];
        break;
      case GL_TEXTURE_GEN_S:
        RegalAssertArrayIndex( State::Enable::textureGenS, activeTextureUnit );
        enabled = State::Enable::textureGenS[activeTextureUnit];
        break;
      case GL_TEXTURE_GEN_T:
        RegalAssertArrayIndex( State::Enable::textureGenT, activeTextureUnit );
        enabled = State::Enable::textureGenT[activeTextureUnit];
        break;
      case GL_TEXTURE_GEN_R:
        RegalAssertArrayIndex( State::Enable::textureGenR, activeTextureUnit );
        enabled = State::Enable::textureGenR[activeTextureUnit];
        break;
      case GL_TEXTURE_GEN_Q:
        RegalAssertArrayIndex( State::Enable::textureGenQ, activeTextureUnit );
        enabled = State::Enable::textureGenQ[activeTextureUnit];
        break;
      case GL_VERTEX_PROGRAM_TWO_SIDE:
        enabled = State::Enable::vertexProgramTwoSide;
        break;

      default:
        return false;
    }
    return true;
  }

  bool glIsEnabledi(RegalContext *ctx, GLboolean &enabled, GLenum pname, GLuint index)
  {
    UNUSED_PARAMETER(ctx);

    switch (pname)
    {
      case GL_BLEND:
        if (index >= array_size( State::Enable::blend ))
          return false;
        RegalAssertArrayIndex( State::Enable::blend, index );
        enabled = State::Enable::blend[index];
        break;

      case GL_SCISSOR_TEST:
        if (index >= array_size( State::Enable::scissorTest ))
          return false;
        RegalAssertArrayIndex( State::Enable::scissorTest, index );
        enabled = State::Enable::scissorTest[index];
        break;

      default:
        return false;
    }
    return true;
  }

  bool SetEnable(RegalContext *ctx, GLenum cap, GLboolean enabled)
  {
    switch (cap)
    {
      case GL_ALPHA_TEST:
        State::Enable::alphaTest = State::ColorBuffer::alphaTest = enabled;
        break;
      case GL_BLEND:
      {
        size_t n = array_size( State::Enable::blend );
        RegalAssert( array_size( State::ColorBuffer::blend ) == n );
        for (size_t ii=0; ii<n; ii++)
        {
          RegalAssertArrayIndex( State::Enable::blend, ii );
          RegalAssertArrayIndex( State::ColorBuffer::blend, ii );
          State::Enable::blend[ii] = State::ColorBuffer::blend[ii] = enabled;
        }
      }
      break;
      case GL_AUTO_NORMAL:
        State::Enable::autoNormal = State::Eval::autoNormal = enabled;
        break;
      case GL_CLIP_DISTANCE0:
      case GL_CLIP_DISTANCE1:
      case GL_CLIP_DISTANCE2:
      case GL_CLIP_DISTANCE3:
      case GL_CLIP_DISTANCE4:
      case GL_CLIP_DISTANCE5:
      case GL_CLIP_DISTANCE6:
      case GL_CLIP_DISTANCE7:
        RegalAssertArrayIndex( State::Enable::clipDistance, cap-GL_CLIP_DISTANCE0 );
        RegalAssertArrayIndex( State::Transform::clipPlane, cap-GL_CLIP_DISTANCE0 );
        State::Enable::clipDistance[cap-GL_CLIP_DISTANCE0] = State::Transform::clipPlane[cap-GL_CLIP_DISTANCE0].enabled = enabled;
        break;
      case GL_COLOR_LOGIC_OP:
        State::Enable::colorLogicOp    = State::ColorBuffer::colorLogicOp = enabled;
        break;
      case GL_COLOR_MATERIAL:
        State::Enable::colorMaterial   = State::Lighting::colorMaterial = enabled;
        break;
      case GL_COLOR_SUM:
        State::Enable::colorSum        = State::Fog::colorSum = enabled;
        break;
      case GL_COLOR_TABLE:
        State::Enable::colorTable      = State::PixelMode::colorTable = enabled;
        break;
      case GL_CONVOLUTION_1D:
        State::Enable::convolution1d   = State::PixelMode::convolution1d = enabled;
        break;
      case GL_CONVOLUTION_2D:
        State::Enable::convolution2d   = State::PixelMode::convolution2d = enabled;
        break;
      case GL_CULL_FACE:
        State::Enable::cullFace        = State::Polygon::cullEnable     = enabled;
        break;
      case GL_DEPTH_CLAMP:
        State::Enable::depthClamp      = State::Transform::depthClamp   = enabled;
        break;
      case GL_DEPTH_TEST:
        State::Enable::depthTest       = State::Depth::enable           = enabled;
        break;
      case GL_DITHER:
        State::Enable::dither          = State::ColorBuffer::dither = enabled;
        break;
      case GL_FOG:
        State::Enable::fog             = State::Fog::enable = enabled;
        break;
      case GL_FRAMEBUFFER_SRGB:
        State::Enable::framebufferSRGB = State::ColorBuffer::framebufferSRGB = enabled;
        break;
      case GL_HISTOGRAM:
        State::Enable::histogram       = State::PixelMode::histogram = enabled;
        break;
      case GL_INDEX_LOGIC_OP:
        State::Enable::indexLogicOp    = State::ColorBuffer::indexLogicOp = enabled;
        break;
      case GL_LIGHT0:
      case GL_LIGHT1:
      case GL_LIGHT2:
      case GL_LIGHT3:
      case GL_LIGHT4:
      case GL_LIGHT5:
      case GL_LIGHT6:
      case GL_LIGHT7:
        RegalAssertArrayIndex( State::Enable::light, cap-GL_LIGHT0 );
        RegalAssertArrayIndex( State::Lighting::lights, cap-GL_LIGHT0 );
        State::Enable::light[cap-GL_LIGHT0] = State::Lighting::lights[cap-GL_LIGHT0].enabled = enabled;
        break;
      case GL_LIGHTING:
        State::Enable::lighting = State::Lighting::lighting = enabled;
        break;
      case GL_LINE_SMOOTH:
        State::Enable::lineSmooth  = State::Line::smooth  = enabled;
        break;
      case GL_LINE_STIPPLE:
        State::Enable::lineStipple = State::Line::stipple = enabled;
        break;
      case GL_MAP1_COLOR_4:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1Color4        = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_INDEX:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1Index         = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_NORMAL:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1Normal        = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_TEXTURE_COORD_1:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1TextureCoord1 = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_TEXTURE_COORD_2:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1TextureCoord2 = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_TEXTURE_COORD_3:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1TextureCoord3 = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_TEXTURE_COORD_4:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1TextureCoord4 = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_VERTEX_3:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1Vertex3       = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP1_VERTEX_4:
        RegalAssertArrayIndex( State::Eval::map1dEnables, cap-GL_MAP1_COLOR_4 );
        State::Enable::map1Vertex4       = State::Eval::map1dEnables[cap-GL_MAP1_COLOR_4] = enabled;
        break;
      case GL_MAP2_COLOR_4:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2Color4        = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_INDEX:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2Index         = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_NORMAL:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2Normal        = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_TEXTURE_COORD_1:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2TextureCoord1 = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_TEXTURE_COORD_2:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2TextureCoord2 = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_TEXTURE_COORD_3:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2TextureCoord3 = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_TEXTURE_COORD_4:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2TextureCoord4 = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_VERTEX_3:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2Vertex3       = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MAP2_VERTEX_4:
        RegalAssertArrayIndex( State::Eval::map2dEnables, cap-GL_MAP2_COLOR_4 );
        State::Enable::map2Vertex4       = State::Eval::map2dEnables[cap-GL_MAP2_COLOR_4] = enabled;
        break;
      case GL_MINMAX:
        State::Enable::minmax = State::PixelMode::minmax = enabled;
        break;
      case GL_MULTISAMPLE:
        State::Enable::multisample = State::Multisample::multisample = enabled;
        break;
      case GL_NORMALIZE:
        State::Enable::normalize = State::Transform::normalize = enabled;
        break;
      case GL_POINT_SMOOTH:
        State::Enable::pointSmooth = State::Point::smooth = enabled;
        break;
      case GL_POINT_SPRITE:
        State::Enable::pointSprite = State::Point::sprite = enabled;
        break;
      case GL_POLYGON_OFFSET_FILL:
        State::Enable::polygonOffsetFill = State::Polygon::offsetFill = enabled;
        break;
      case GL_POLYGON_OFFSET_LINE:
        State::Enable::polygonOffsetLine = State::Polygon::offsetLine = enabled;
        break;
      case GL_POLYGON_OFFSET_POINT:
        State::Enable::polygonOffsetPoint = State::Polygon::offsetPoint = enabled;
        break;
      case GL_POLYGON_SMOOTH:
        State::Enable::polygonSmooth     = State::Polygon::smoothEnable = enabled;
        break;
      case GL_POLYGON_STIPPLE:
        State::Enable::polygonStipple    = State::Polygon::stippleEnable = enabled;
        break;
      case GL_POST_COLOR_MATRIX_COLOR_TABLE:
        State::Enable::postColorMatrixColorTable = State::PixelMode::postColorMatrixColorTable = enabled;
        break;
      case GL_POST_CONVOLUTION_COLOR_TABLE:
        State::Enable::postConvolutionColorTable  = State::PixelMode::postConvolutionColorTable = enabled;
        break;
      case GL_PROGRAM_POINT_SIZE:
        State::Enable::programPointSize   = enabled;
        break;
      case GL_RESCALE_NORMAL:
        State::Enable::rescaleNormal = State::Transform::rescaleNormal   = enabled;
        break;
      case GL_SAMPLE_ALPHA_TO_COVERAGE:
        State::Enable::sampleAlphaToCoverage = State::Multisample::sampleAlphaToCoverage = enabled;
        break;
      case GL_SAMPLE_ALPHA_TO_ONE:
        State::Enable::sampleAlphaToOne = State::Multisample::sampleAlphaToOne = enabled;
        break;
      case GL_SAMPLE_COVERAGE:
        State::Enable::sampleCoverage = State::Multisample::sampleCoverage = enabled;
        break;
      case GL_SAMPLE_SHADING:
        State::Enable::sampleShading = State::Multisample::sampleShading = enabled;
        break;
      case GL_SCISSOR_TEST:
      {
        size_t n = array_size( State::Enable::scissorTest );
        RegalAssert( array_size( State::Scissor::scissorTest ) == n );
        for (size_t ii=0; ii<n; ii++)
        {
          RegalAssertArrayIndex( State::Enable::scissorTest, ii );
          RegalAssertArrayIndex( State::Scissor::scissorTest, ii );
          State::Enable::scissorTest[ii] = State::Scissor::scissorTest[ii] = enabled;
        }
      }
      break;
      case GL_SEPARABLE_2D:
        State::Enable::separable2d = State::PixelMode::separable2d = enabled;
        break;
      case GL_STENCIL_TEST:
        State::Enable::stencilTest = State::Stencil::enable = enabled;
        break;
      case GL_TEXTURE_1D:
        RegalAssertArrayIndex( State::Enable::texture1d, activeTextureUnit );
        State::Enable::texture1d[activeTextureUnit]      = enabled;
        break;
      case GL_TEXTURE_2D:
        RegalAssertArrayIndex( State::Enable::texture2d, activeTextureUnit );
        State::Enable::texture2d[activeTextureUnit]      = enabled;
        break;
      case GL_TEXTURE_3D:
        RegalAssertArrayIndex( State::Enable::texture3d, activeTextureUnit );
        State::Enable::texture3d[activeTextureUnit]      = enabled;
        break;
      case GL_TEXTURE_CUBE_MAP:
        RegalAssertArrayIndex( State::Enable::textureCubeMap, activeTextureUnit );
        State::Enable::textureCubeMap[activeTextureUnit] = enabled;
        break;
      case GL_TEXTURE_RECTANGLE:
        RegalAssertArrayIndex( State::Enable::textureRectangle, activeTextureUnit );
        State::Enable::textureRectangle[activeTextureUnit] = enabled;
        break;
      case GL_TEXTURE_GEN_S:
        RegalAssertArrayIndex( State::Enable::textureGenS, activeTextureUnit );
        State::Enable::textureGenS[activeTextureUnit]    = enabled;
        break;
      case GL_TEXTURE_GEN_T:
        RegalAssertArrayIndex( State::Enable::textureGenT, activeTextureUnit );
        State::Enable::textureGenT[activeTextureUnit]    = enabled;
        break;
      case GL_TEXTURE_GEN_R:
        RegalAssertArrayIndex( State::Enable::textureGenR, activeTextureUnit );
        State::Enable::textureGenR[activeTextureUnit]    = enabled;
        break;
      case GL_TEXTURE_GEN_Q:
        RegalAssertArrayIndex( State::Enable::textureGenQ, activeTextureUnit );
        State::Enable::textureGenQ[activeTextureUnit]    = enabled;
        break;
      case GL_VERTEX_PROGRAM_TWO_SIDE:
        State::Enable::vertexProgramTwoSide        = enabled;
        break;

      default:
        break;
    }

    if (ctx->info->core || ctx->info->es1 || ctx->info->es2)
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

  bool SetEnablei(RegalContext *ctx, GLenum cap, GLuint index, GLboolean enabled)
  {
    UNUSED_PARAMETER(ctx);
    switch (cap)
    {
      case GL_BLEND:
        {
          size_t n = array_size( State::Enable::blend );
          RegalAssert( array_size( State::ColorBuffer::blend ) == n );
          if (index < n)
          {
            RegalAssertArrayIndex( State::Enable::blend, index );
            RegalAssertArrayIndex( State::ColorBuffer::blend, index );
            State::Enable::blend[index] = State::ColorBuffer::blend[index] = enabled;
          }
        }
        break;
      case GL_SCISSOR_TEST:
        {
          size_t n = array_size( State::Enable::scissorTest );
          RegalAssert( array_size( State::Scissor::scissorTest ) == n );
          if (index < n)
          {
            RegalAssertArrayIndex( State::Enable::scissorTest, index );
            RegalAssertArrayIndex( State::Scissor::scissorTest, index );
            State::Enable::scissorTest[index] = State::Scissor::scissorTest[index] = enabled;
          }
        }
        break;

      default:
        return false;
    }
    return true;
  }

  bool Enable(RegalContext *ctx, GLenum cap)
  {
    Internal("Regal::Ppa::Enable ",Token::toString(cap));
    return SetEnable(ctx, cap, GL_TRUE);
  }

  bool Disable(RegalContext *ctx, GLenum cap)
  {
    Internal("Regal::Ppa::Disable ",Token::toString(cap));
    return SetEnable(ctx, cap, GL_FALSE);
  }

  bool Enablei(RegalContext *ctx, GLenum cap, GLuint index)
  {
    Internal("Regal::Ppa::Enablei ",Token::toString(cap),index);
    return SetEnablei(ctx, cap, index, GL_TRUE);
  }

  bool Disablei(RegalContext *ctx, GLenum cap, GLuint index)
  {
    Internal("Regal::Ppa::Disablei ",Token::toString(cap),index);
    return SetEnablei(ctx, cap, index, GL_FALSE);
  }

  void glActiveTexture( GLenum texture )
  {
    if ( texture >= GL_TEXTURE0 && texture < static_cast<GLenum>(GL_TEXTURE0 + REGAL_EMU_MAX_TEXTURE_UNITS) )
      activeTextureUnit = texture - GL_TEXTURE0;
    else
      Warning( "Active texture out of range: ", Token::GLtextureToString(texture), " > ", Token::GLtextureToString(GL_TEXTURE0 + REGAL_EMU_MAX_TEXTURE_UNITS - 1));
  }

  inline void glClampColor( GLenum target, GLenum clamp )
  {
    switch (target)
    {
      case GL_CLAMP_FRAGMENT_COLOR:
        State::Enable::clampFragmentColor = State::ColorBuffer::clampFragmentColor = clamp;
        break;
      case GL_CLAMP_READ_COLOR:
        State::Enable::clampReadColor     = State::ColorBuffer::clampReadColor = clamp;
        break;
      case GL_CLAMP_VERTEX_COLOR:
        State::Enable::clampVertexColor   = State::Lighting::clampVertexColor = clamp;
        break;
      default:
        break;
    }
  }

  template <typename T> void glTexEnv(GLenum target, GLenum pname, T param)
  {
    if ((target == GL_POINT_SPRITE) && (pname == GL_COORD_REPLACE))
      glMultiTexEnv(static_cast<GLenum>(GL_TEXTURE0+activeTextureUnit),target,pname,param);
  }

  template <typename T> void glTexEnvv(GLenum target, GLenum pname, const T *params)
  {
    if ((target == GL_POINT_SPRITE) && (pname == GL_COORD_REPLACE))
      glMultiTexEnvv(static_cast<GLenum>(GL_TEXTURE0+activeTextureUnit),target,pname,params);
  }

  std::vector<GLbitfield>            maskStack;
  std::vector<State::Depth>          depthStack;
  std::vector<State::Stencil>        stencilStack;
  std::vector<State::Polygon>        polygonStack;
  std::vector<State::Transform>      transformStack;
  std::vector<State::Hint>           hintStack;
  std::vector<State::Enable>         enableStack;
  std::vector<State::List>           listStack;
  std::vector<State::AccumBuffer>    accumBufferStack;
  std::vector<State::Scissor>        scissorStack;
  std::vector<State::Viewport>       viewportStack;
  std::vector<State::Line>           lineStack;
  std::vector<State::Multisample>    multisampleStack;
  std::vector<State::Eval>           evalStack;
  std::vector<State::Fog>            fogStack;
  std::vector<State::Point>          pointStack;
  std::vector<State::PolygonStipple> polygonStippleStack;
  std::vector<State::ColorBuffer>    colorBufferStack;
  std::vector<State::PixelMode>      pixelModeStack;
  std::vector<State::Lighting>       lightingStack;

  GLuint activeTextureUnit;
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_PPA_H__
