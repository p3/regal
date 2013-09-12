/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2013 Google Inc.
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

 Regal Emulation of glPushClientAttrib/glPopClientAttrib
 Lloyd Pique, Nigel Stewart, Scott Nations

 */

#ifndef __REGAL_PPCA_H__
#define __REGAL_PPCA_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalClientState.h"
#include "RegalEmu.h"
#include "RegalLog.h"
#include "RegalToken.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu
{

struct Ppca : public ClientState::VertexArray, ClientState::PixelStore
{
  Ppca()
  : driverAllowsVertexAttributeArraysWithoutBoundBuffer(true)
  {
  }

  void Init(RegalContext &ctx)
  {
    Reset(ctx);
  }

  void Reset(RegalContext &ctx)
  {
    ClientState::VertexArray::Reset();
    ClientState::PixelStore::Reset();

    maskStack.clear();
    vertexArrayStack.clear();
    pixelStoreStack.clear();

    // Chromium/PepperAPI GLES generates an error (visible through glGetError) and
    // logs a message if a call is made to glVertexAttribPointer and no
    // GL_ARRAY_BUFFER is bound.

    driverAllowsVertexAttributeArraysWithoutBoundBuffer = ( !ctx.isES2() || ctx.info->vendor != "Chromium" );
  }

  void Cleanup(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  void glPushClientAttrib(RegalContext &ctx, GLbitfield mask)
  {
    // from glspec43.compatibility.20130214.withchanges.pdf Sec. 21.6, p. 622
    //
    // A STACK_OVERFLOW error is generated if PushClientAttrib is called
    // and the client attribute stack depth is equal to the value of
    // MAX_CLIENT_ATTRIB_STACK_DEPTH.
    //
    // TODO: set correct GL error here

    if (maskStack.size() >= ctx.info->max_client_attrib_stack_depth)
      return;

    maskStack.push_back(mask);

    if (mask&GL_CLIENT_VERTEX_ARRAY_BIT)
    {
      Internal("Regal::Ppca::PushClientAttrib GL_CLIENT_VERTEX_ARRAY_BIT ",ClientState::VertexArray::toString());
      vertexArrayStack.push_back(ClientState::VertexArray());
      vertexArrayStack.back() = *this;
      mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
    }

    if (mask&GL_CLIENT_PIXEL_STORE_BIT)
    {
      Internal("Regal::Ppca::PushClientAttrib GL_CLIENT_PIXEL_STORE_BIT ",ClientState::PixelStore::toString());
      pixelStoreStack.push_back(ClientState::PixelStore());
      pixelStoreStack.back() = *this;
      mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
    }

    // Pass the rest through, for now

    if (ctx.isCore() || ctx.isES1() || ctx.isES2())
      return;

    if (mask)
      ctx.dispatcher.emulation.glPushClientAttrib(mask);
  }

  void glPopClientAttrib(RegalContext &ctx)
  {
    // from glspec43.compatibility.20130214.withchanges.pdf Sec. 21.6, p. 622
    //
    // A STACK_UNDERFLOW error is generated if PopClientAttrib is called
    // and the client attribute stack depth is zero.
    //
    // TODO: set correct GL error here

    if (!maskStack.size())
      return;

    GLbitfield mask = maskStack.back();
    maskStack.pop_back();

    if (mask&GL_CLIENT_VERTEX_ARRAY_BIT)
    {
      RegalAssert(vertexArrayStack.size());
      ClientState::VertexArray::transition(ctx.dispatcher.emulation, vertexArrayStack.back(), driverAllowsVertexAttributeArraysWithoutBoundBuffer);
      vertexArrayStack.pop_back();

      Internal("Regal::Ppca::PopClientAttrib GL_CLIENT_VERTEX_ARRAY_BIT ",ClientState::VertexArray::toString());

      mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
    }

    if (mask&GL_CLIENT_PIXEL_STORE_BIT)
    {
      RegalAssert(pixelStoreStack.size());
      ClientState::PixelStore::transition(ctx.dispatcher.emulation, pixelStoreStack.back());
      pixelStoreStack.pop_back();

      Internal("Regal::Ppca::PopClientAttrib GL_CLIENT_PIXEL_STORE_BIT ",ClientState::PixelStore::toString());

      mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
    }

    // Pass the rest through, for now

    if (ctx.isCore() || ctx.isES1() || ctx.isES2())
      return;

    if (mask)
      ctx.dispatcher.emulation.glPopClientAttrib();
  }

  void glClientAttribDefaultEXT(RegalContext &ctx, GLbitfield mask)
  {
    if (mask&GL_CLIENT_VERTEX_ARRAY_BIT)
    {
      ClientState::VertexArray::Reset();

      Internal("Regal::Ppca::glClientAttribDefaultEXT GL_CLIENT_VERTEX_ARRAY_BIT ",ClientState::VertexArray::toString());

      // Ideally we'd only set the state that has changed - revisit

      ClientState::VertexArray::set(ctx.dispatcher.emulation,driverAllowsVertexAttributeArraysWithoutBoundBuffer);

      mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
    }

    if (mask&GL_CLIENT_PIXEL_STORE_BIT)
    {
      ClientState::PixelStore::Reset();

      Internal("Regal::Ppca::PopClientAttrib GL_CLIENT_PIXEL_STORE_BIT ",ClientState::PixelStore::toString());

      // Ideally we'd only set the state that has changed - revisit

      ClientState::PixelStore::set(ctx.dispatcher.emulation);

      mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
    }

    // Pass the rest through, for now

    if (ctx.isCore() || ctx.isES1() || ctx.isES2())
      return;

    if (mask)
      ctx.dispatcher.emulation.glClientAttribDefaultEXT(mask);
  }

  void glPushClientAttribDefaultEXT(RegalContext &ctx, GLbitfield mask)
  {
    GLbitfield tmpMask = mask;
    glPushClientAttrib(ctx, tmpMask);
    glClientAttribDefaultEXT(ctx, mask);
  }

  void glBindBuffer( GLenum target, GLuint buffer )
  {
    ClientState::VertexArray::glBindBuffer(target,buffer);
    ClientState::PixelStore::glBindBuffer(target,buffer);
  }

  void glDeleteBuffers( GLsizei n, const GLuint *buffers )
  {
    ClientState::VertexArray::glDeleteBuffers(n,buffers);
    ClientState::PixelStore::glDeleteBuffers(n,buffers);
  }

  bool glGetv(RegalContext &ctx, GLenum pname, GLboolean *params)
  {
    switch (pname)
    {
      case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        params[0] = GL_TRUE;
        break;
      case GL_CLIENT_ATTRIB_STACK_DEPTH:
        params[0] = (maskStack.size() != 0);
        break;
     case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        params[0] = ctx.info->max_combined_texture_image_units != 0;
        break;
     case GL_MAX_TEXTURE_COORDS:
        params[0] = ctx.info->max_texture_coords != 0;
        break;
     case GL_MAX_TEXTURE_UNITS:
        params[0] = ctx.info->max_texture_units != 0;
        break;

      default:
        return false;
    }
    return true;
  }

  template <typename T> bool glGetv(RegalContext &ctx, GLenum pname, T *params)
  {
    switch (pname)
    {
      case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        params[0] = static_cast<T>(ctx.info->max_client_attrib_stack_depth);
        break;
      case GL_CLIENT_ATTRIB_STACK_DEPTH:
        params[0] = static_cast<T>(maskStack.size());
        break;
      case GL_MAX_VERTEX_ATTRIBS:
        params[0] = static_cast<T>(ctx.info->max_vertex_attribs);
        break;
      case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        params[0] = static_cast<T>(ctx.info->max_combined_texture_image_units);
        break;
      case GL_MAX_TEXTURE_COORDS:
        params[0] = static_cast<T>(ctx.info->max_texture_coords);
        break;
      case GL_MAX_TEXTURE_UNITS:
        params[0] = static_cast<T>(ctx.info->max_texture_units);
        break;

      default:
        return false;
    }
    return true;
  }

  Ppca &swap(Ppca &other)
  {
    std::swap(driverAllowsVertexAttributeArraysWithoutBoundBuffer,other.driverAllowsVertexAttributeArraysWithoutBoundBuffer);
    std::swap(maskStack,other.maskStack);
    std::swap(vertexArrayStack,other.vertexArrayStack);
    std::swap(pixelStoreStack,other.pixelStoreStack);

    VertexArray::swap(other);
    PixelStore::swap(other);

    return *this;
  }

  bool driverAllowsVertexAttributeArraysWithoutBoundBuffer;
  std::vector<GLbitfield>                maskStack;
  std::vector<ClientState::VertexArray>  vertexArrayStack;
  std::vector<ClientState::PixelStore>   pixelStoreStack;
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_PPCA_H__
