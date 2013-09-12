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

 Regal Draw Elements Base Vertex emu layer
 Nigel Stewart, Scott Nations

 TODO:

   - add logic to handle aliasing of named arrays onto generic arrays

 */

#ifndef __REGAL_BASEVERTEX_H__
#define __REGAL_BASEVERTEX_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalClientState.h"
#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

struct BaseVertex : public ClientState::VertexArray
{
  void Init(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  void Cleanup(RegalContext &ctx)
  {
    UNUSED_PARAMETER(ctx);
  }

  void adjust(RegalContext &ctx, DispatchTableGL &dt, GLint basevertex)
  {
    UNUSED_PARAMETER(ctx);

    GLuint currentVBO = ClientState::VertexArray::arrayBufferBinding;

    size_t num = array_size( ClientState::VertexArray::named );
    for (size_t ii=0; ii<num; ii++)
    {
      RegalAssertArrayIndex( ClientState::VertexArray::named, ii );
      ClientState::NamedVertexArray &n = ClientState::VertexArray::named[ii];
      if (n.enabled)
      {
        if (currentVBO != n.buffer)
        {
          currentVBO = n.buffer;
          dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER, currentVBO);
        }

        if (ii < 7)
        {
          switch (ii)
          {
            case ClientState::VERTEX:
              dt.call(&dt.glVertexPointer)(n.size, n.type, n.stride, n.pointer);
              break;
            case ClientState::NORMAL:
              dt.call(&dt.glNormalPointer)(n.type, n.stride, n.pointer);
              break;
            case ClientState::FOG_COORD:
              dt.call(&dt.glFogCoordPointer)(n.type, n.stride, n.pointer);
              break;
            case ClientState::COLOR:
              dt.call(&dt.glColorPointer)(n.size, n.type, n.stride, n.pointer);
              break;
            case ClientState::SECONDARY_COLOR:
              dt.call(&dt.glSecondaryColorPointer)(n.size, n.type, n.stride, n.pointer);
              break;
            case ClientState::INDEX:
              dt.call(&dt.glIndexPointer)(n.type, n.stride, n.pointer);
              break;
            case ClientState::EDGE_FLAG:
              dt.call(&dt.glEdgeFlagPointer)(n.stride, n.pointer);
              break;
            default:
              break;
          }
        }
        else
        {
          GLuint index = static_cast<GLuint>(ii - 7);
          dt.call(&dt.glMultiTexCoordPointerEXT)(GL_TEXTURE0+index, n.size, n.type, n.stride, n.pointer);
        }
      }
    }

    num = array_size( ClientState::VertexArray::generic );
    for (size_t ii=0; ii<num; ii++)
    {
      RegalAssertArrayIndex( ClientState::VertexArray::generic, ii );
      ClientState::GenericVertexArray &g = ClientState::VertexArray::generic[ii];
      if (g.enabled && g.bindingIndex < array_size( ClientState::VertexArray::bindings ))
      {
        RegalAssertArrayIndex( ClientState::VertexArray::bindings, g.bindingIndex );
        ClientState::VertexBufferBindPoint &b = ClientState::VertexArray::bindings[g.bindingIndex];
        GLvoid *p = reinterpret_cast<GLvoid *>(b.offset + (b.stride*basevertex));

        if (currentVBO != b.buffer)
        {
          currentVBO = b.buffer;
          dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER, currentVBO);
        }

        GLsizei index = static_cast<GLsizei>(ii);
        if (g.isInteger)
          dt.call(&dt.glVertexAttribIPointer)(index, g.size, g.type, b.stride, p);
        else if (g.isLong)
          dt.call(&dt.glVertexAttribLPointer)(index, g.size, g.type, b.stride, p);
        else
          dt.call(&dt.glVertexAttribPointer)(index, g.size, g.type, g.normalized, b.stride, p);
      }
    }

    if (currentVBO != ClientState::VertexArray::arrayBufferBinding)
      dt.call(&dt.glBindBuffer)(GL_ARRAY_BUFFER, ClientState::VertexArray::arrayBufferBinding);
  }

  bool glDrawElementsBaseVertex(RegalContext &ctx, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex)
  {
    DispatchTableGL &dt = ctx.dispatcher.emulation;
    if (basevertex)
      adjust(ctx, dt, basevertex);
    dt.call(&dt.glDrawElements)(mode, count, type, indices);
    if (basevertex)
      adjust(ctx, dt, 0);
    return true;
  }

  bool glDrawRangeElementsBaseVertex(RegalContext &ctx, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex)
  {
    DispatchTableGL &dt = ctx.dispatcher.emulation;
    if (basevertex)
      adjust(ctx, dt, basevertex);
    dt.call(&dt.glDrawRangeElements)(mode, start, end, count, type, indices);
    if (basevertex)
      adjust(ctx, dt, 0);
    return true;
  }

  bool glDrawElementsInstancedBaseVertex(RegalContext &ctx, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex)
  {
    DispatchTableGL &dt = ctx.dispatcher.emulation;
    if (basevertex)
      adjust(ctx, dt, basevertex);
    dt.call(&dt.glDrawElementsInstanced)(mode, count, type, indices, primcount);
    if (basevertex)
      adjust(ctx, dt, 0);
    return true;
  }

  bool glDrawElementsInstancedBaseVertexBaseInstance(RegalContext &ctx, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance)
  {
    DispatchTableGL &dt = ctx.dispatcher.emulation;
    if (basevertex)
      adjust(ctx, dt, basevertex);
    dt.call(&dt.glDrawElementsInstancedBaseInstance)(mode, count, type, indices, primcount, baseinstance);
    if (basevertex)
      adjust(ctx, dt, 0);
    return true;
  }

  bool glMultiDrawElementsBaseVertex(RegalContext &ctx, GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const* indices, GLsizei primcount, const GLint *basevertex)
  {
    DispatchTableGL &dt = ctx.dispatcher.emulation;
    for (GLsizei ii=0; ii<primcount; ii++)
    {
      if (basevertex[ii])
        adjust(ctx, dt, basevertex[ii]);
      dt.call(&dt.glDrawElements)(mode, count[ii], type, indices[ii]);
      if (basevertex[ii])
        adjust(ctx, dt, 0);
    }
    return true;
  }
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_BASEVERTEX_H__
