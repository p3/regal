/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012-2013 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include "RegalLog.h"
#include "RegalFilt.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

  using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
  using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

  bool Filt::BindTexture(const RegalContext &ctx, GLenum target, GLuint name)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);
    UNUSED_PARAMETER(name);

    if (ctx.isES2())
    {
      switch( target )
      {
        case GL_TEXTURE_1D:
        case GL_TEXTURE_3D:
          Warning( "Regal does not support ", GLenumToString( target ), " as target for glBindTexture for ES 2.0 profile - skipping." );
          return true;
        default:
          break;
      }
    }

    return false;
  }

  bool Filt::BindFramebuffer(const RegalContext &ctx, GLenum target, GLuint framebuffer)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);
    UNUSED_PARAMETER(framebuffer);

    fboID = framebuffer;

    return false;
  }

  bool Filt::DrawBuffers(const RegalContext &ctx, GLsizei n, const GLenum *bufs)
  {
    UNUSED_PARAMETER(ctx);

    if (!ctx.isES2())
      return false;

    for (GLsizei i = 0; i < n; ++i)
    {
      if (bufs[i] == GL_NONE || (bufs[i] >= GL_COLOR_ATTACHMENT0 && bufs[i] <= GL_COLOR_ATTACHMENT15))
        continue;

      Warning( "Regal does not support ", GLenumToString( bufs[i] ), " as target for glDrawBuffers for ES 2.0 profile - skipping." );
      return true;
    }

    return false;
  }

  bool Filt::FilterTexParameter(const RegalContext &ctx, GLenum target, GLenum pname, GLfloat param, GLfloat &newParam)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);

    if (!ctx.isES2() && !ctx.isCore())
      return false;

    switch(pname)
    {
      case GL_TEXTURE_WRAP_S:
      case GL_TEXTURE_WRAP_T:
      case GL_TEXTURE_WRAP_R:
        switch(int(param))
        {
          case GL_CLAMP:
            Warning("Regal does not support GL_CLAMP wrap mode for core or ES 2.0 profiles - remapping to equivalent GL_CLAMP_TO_EDGE");
            newParam = GL_CLAMP_TO_EDGE;
            return true;
          default: break;
        }
      default: break;
    }

    return false;
  }

   bool Filt::GetTexParameteriv(const RegalContext &ctx, GLenum target, GLenum pname, GLint *params)
   {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);
    UNUSED_PARAMETER(pname);
    UNUSED_PARAMETER(params);

    if (!ctx.isES2())
      return false;

    switch(target)
    {
      case GL_PROXY_TEXTURE_CUBE_MAP:
        Warning( "Regal does not support PROXY_TEXTURE_CUBE_MAP as target for ES 2.0 profile" );
        // Regal does not have the infrastructure for generating GL errors. Hence, pass the error to the lower layer so that the application is notified of the error.
        return false;
      default:
        break;
    }

    return false;
  }

  bool Filt::FramebufferTexture2D(const RegalContext &ctx, GLenum target, GLenum attachment,
                                  GLenum textarget, GLuint texture, GLint level)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);
    UNUSED_PARAMETER(attachment);
    UNUSED_PARAMETER(textarget);
    UNUSED_PARAMETER(texture);
    UNUSED_PARAMETER(level);

    if (!FramebufferAttachmentSupported(ctx, attachment))
    {
      return true;
    }

    if (!ctx.isES2())
      return false;

    if ((level > 0) && !ctx.info->gl_oes_fbo_render_mipmap)
    {
      Warning("glFramebufferTexture2D with level > 0 not supported for ES 2.0 without OES_fbo_render_mipmap.");
      return true;
    }

    if (texture > 0)
    {
      switch (textarget)
      {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
          break;

        default:
          Warning("glFramebufferTexture2D with ", GLenumToString(target), ") not supported for ES 2.0.");
          return true;
      }
    }

    return false;
  }

  bool Filt::GenerateMipmap(const RegalContext &ctx, GLenum target)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);

    if (!ctx.isES2())
      return false;

    switch(target)
    {
      case GL_TEXTURE_2D:
      case GL_TEXTURE_CUBE_MAP:
        break;

      case GL_TEXTURE_2D_ARRAY:
        if (!ctx.info->gl_nv_texture_array)
        {
          Warning("glGenerateMipmap(GL_TEXTURE_2D_ARRAY) not supported for ES 2.0 without NV_texture_array.");
          return true;
        }
        break;

      default:
        Warning("glGenerateMipmap(", GLenumToString(target), ") not supported for ES 2.0.");
        return true;
    }

    return false;
  }

  bool Filt::ReadBuffer(const RegalContext &ctx, GLenum src)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(src);

    if (!ctx.isES2() || !ctx.info->gl_nv_read_buffer)
      return false;

    switch(src)
    {
      // These two should always be supported w/o additional extensions
      case GL_COLOR_ATTACHMENT0:
      case GL_BACK:
        break;

      // GL_FRONT may require NV_read_buffer_front, depending whether the context is
      // double buffered. Let's output a warning but still pass it through
      case GL_FRONT:
        if (!ctx.info->gl_nv_read_buffer_front)
          Warning("glReadBuffer(GL_FRONT) may not work on ES 2 without NV_read_buffer_front, depending on context buffering.");
        break;

      case GL_COLOR_ATTACHMENT1:
      case GL_COLOR_ATTACHMENT2:
      case GL_COLOR_ATTACHMENT3:
      case GL_COLOR_ATTACHMENT4:
      case GL_COLOR_ATTACHMENT5:
      case GL_COLOR_ATTACHMENT6:
      case GL_COLOR_ATTACHMENT7:
      case GL_COLOR_ATTACHMENT8:
      case GL_COLOR_ATTACHMENT9:
      case GL_COLOR_ATTACHMENT10:
      case GL_COLOR_ATTACHMENT11:
      case GL_COLOR_ATTACHMENT12:
      case GL_COLOR_ATTACHMENT13:
      case GL_COLOR_ATTACHMENT14:
      case GL_COLOR_ATTACHMENT15:
        if (!ctx.info->gl_nv_draw_buffers)
        {
          Warning("glReadBuffer(GL_COLOR_ATTACHMENT1+) not supported for ES 2 without NV_draw_buffers.");
          return true;
        }
        break;

      default:
        Warning("glReadBuffer(", GLenumToString(src), ") not supported for ES 2.\n");
        return true;
    }

    return false;
  }

  bool Filt::RenderMode(const RegalContext &ctx, GLenum mode)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(mode);

    if (ctx.isCore() || ctx.isES2())
      if (mode!=GL_RENDER)
      {
        Warning("Regal does not support ", GLenumToString(mode), " render mode for core or ES 2.0 profiles, only GL_RENDER is supported in those profiles - skipping.");
        return true;
      }

    return false;
  }

  bool Filt::FramebufferAttachmentSupported(const RegalContext &ctx, GLenum attachment)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(attachment);

    // If we're running on a non-ES context, then all attachments from EXT_framebuffer_object
    // are supported
    if (!ctx.isES2())
      return true;

    // COLOR_ATTACHMENT1 and up not supported in base ES 2.0. Need either
    // NV_fbo_color_attachments, or EXT_draw_buffers (not yet checked by Regal)
    if ((attachment > GL_COLOR_ATTACHMENT0) &&
        (attachment <= GL_COLOR_ATTACHMENT15) &&
        !(ctx.info->gl_nv_fbo_color_attachments /*|| ctx.info->gl_ext_draw_buffers*/))
    {
      Warning("GL_COLOR_ATTACHMENT1+ not supported for ES 2.0 without NV_fbo_color_attachments or EXT_draw_buffers.");
      return false;
    }

    return true;
  }

  bool Filt::PixelStorei(const RegalContext &ctx, GLenum pname, GLint param)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(pname);
    UNUSED_PARAMETER(param);

    if (ctx.isES2())
    {
      switch(pname)
      {
        case GL_PACK_ALIGNMENT:
        case GL_UNPACK_ALIGNMENT:
          break;

        case GL_UNPACK_ROW_LENGTH_EXT:
        case GL_UNPACK_SKIP_ROWS_EXT:
        case GL_UNPACK_SKIP_PIXELS_EXT:
          if (!ctx.info->gl_ext_unpack_subimage)
          {
            Warning("glPixelStorei ", GLenumToString(pname),
                    " not supported for ES 2.0 without EXT_unpack_subimage.");
            return true;
          }
          break;

        case GL_UNPACK_SKIP_IMAGES_NV:
        case GL_UNPACK_IMAGE_HEIGHT_NV:
          if (!ctx.info->gl_ext_unpack_subimage || !ctx.info->gl_nv_texture_array)
          {
            Warning("glPixelStorei ", GLenumToString(pname),
                    " not supported for ES 2.0 without EXT_unpack_subimage and NV_texture_array.");
            return true;
          }
          break;

        case GL_PACK_ROW_LENGTH_NV:
        case GL_PACK_SKIP_ROWS_NV:
        case GL_PACK_SKIP_PIXELS_NV:
          if (!ctx.info->gl_nv_pack_subimage)
          {
            Warning("glPixelStorei ", GLenumToString(pname),
                    " not supported for ES 2.0 without NV_pack_subimage.");
            return true;
          }
          break;

        case GL_PACK_IMAGE_HEIGHT:
        case GL_PACK_SKIP_IMAGES:
          if (!ctx.info->gl_nv_pack_subimage || !ctx.info->gl_nv_texture_array)
          {
            Warning("glPixelStorei ", GLenumToString(pname),
                    " not supported for ES 2.0 without NV_pack_subimage and NV_texture_array.");
            return true;
          }
          break;

        default:
          Warning("glPixelStorei ", GLenumToString(pname), " not supported for ES 2.0.");
          return true;
      }
    }

    return false;
  }

  bool Filt::PolygonMode(const RegalContext &ctx, GLenum face, GLenum mode)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(face);
    UNUSED_PARAMETER(mode);

    if (ctx.isCore())
    {
      if (face!=GL_FRONT_AND_BACK)
      {
        Warning("Regal does not support ", GLenumToString(face), " in glPolygonMode for core profile, only GL_FRONT_AND_BACK is supported - skipping.");
        return true;
      }
    }

    if (ctx.isES2())
    {
      Warning("Regal does not support glPolygonMode for ES 2.0 - skipping.");
      return true;
    }

    return false;
  }

  bool Filt::FilterGet(const RegalContext &ctx, GLenum pname, int &retVal)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(pname);

    bool filtered = false;
    if (ctx.isCore() || ctx.isES2())
    {
      filtered = true;
      switch (pname )
      {
        case GL_MAX_PIXEL_MAP_TABLE:           retVal = 256;  break;
        case GL_MAX_NAME_STACK_DEPTH:          retVal = 128;  break;
        case GL_MAX_LIST_NESTING:              retVal = 64;   break;
        case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH: retVal = ctx.info->max_client_attrib_stack_depth;   break;
        case GL_MAX_ATTRIB_STACK_DEPTH:        retVal = ctx.info->max_attrib_stack_depth;          break;

        case GL_DEPTH_BITS:                    retVal = 24;   break;

        case GL_MAX_EVAL_ORDER:                retVal = 4;    break;

        case GL_RED_BITS:
        case GL_GREEN_BITS:
        case GL_BLUE_BITS:
        case GL_ALPHA_BITS:
        case GL_STENCIL_BITS:
          retVal = 8;
          break;

        case GL_INDEX_MODE:
        case GL_UNPACK_LSB_FIRST:
        case GL_UNPACK_SWAP_BYTES:
        case GL_PACK_LSB_FIRST:
        case GL_PACK_SWAP_BYTES:
          retVal = 0;
          break;
        default:
          filtered = false;
          break;
      }
      if (filtered)
      {
        Warning( "Regal does not support ", GLenumToString(pname), " as pname for glGet for core or ES 2.0 profiles - skipping." );
        return true;
      }
    }

    if (ctx.isCore())
    {
      filtered = true;
#if 0
      switch (pname )
      {
          /* just a test
           case GL_UNPACK_ALIGNMENT:
           retVal = 0;
           break;
           */
        default:
          filtered = false;
          break;
      }
#else
      filtered = false;
#endif
      if (filtered)
      {
        Warning( "Regal does not support ", GLenumToString(pname), " as pname for glGet for core profile - skipping." );
        return true;
      }
    }

    if (ctx.isES2())
    {
      filtered = true;
      switch (pname) {
        case GL_MAX_COLOR_ATTACHMENTS:
          if (ctx.info->gl_nv_fbo_color_attachments)
            filtered = false;
          else
            retVal = 1;
          break;

        case GL_PACK_ROW_LENGTH_NV:
        case GL_PACK_SKIP_ROWS_NV:
        case GL_PACK_SKIP_PIXELS_NV:
          if (ctx.info->gl_nv_pack_subimage)
            filtered = false;
          else
            retVal = 0;
          break;

        case GL_PACK_IMAGE_HEIGHT:
        case GL_PACK_SKIP_IMAGES:
          if (ctx.info->gl_nv_pack_subimage && ctx.info->gl_nv_texture_array)
            filtered = false;
          else
            retVal = 0;
          break;

        case GL_UNPACK_ROW_LENGTH_EXT:
        case GL_UNPACK_SKIP_ROWS_EXT:
        case GL_UNPACK_SKIP_PIXELS_EXT:
          if (ctx.info->gl_ext_unpack_subimage)
            filtered = false;
          else
            retVal = 0;
          break;

        case GL_UNPACK_IMAGE_HEIGHT_NV:
        case GL_UNPACK_SKIP_IMAGES_NV:
          if (ctx.info->gl_ext_unpack_subimage && ctx.info->gl_nv_texture_array)
            filtered = false;
          else
            retVal = 0;
          break;

        // need to filter on ES2.0 since this query returns an
        // INVALID_ENUM if no FBO is currently active
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
          if (ctx.info->gl_nv_draw_buffers)
          {
            if (fboID == 0)
              retVal = GL_NONE;
            else
              filtered = false;
          }
          else
            retVal = GL_NONE;
          break;

        default:
          filtered = false;
          break;
      }
      if (filtered)
      {
        Warning( "Regal does not support ", GLenumToString(pname), " as pname for glGet for ES 2.0 profile - skipping." );
        return true;
      }
    }

    return false;
  }

  bool Filt::TexImage2D(const RegalContext &ctx, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
  {
    UNUSED_PARAMETER(ctx);
    UNUSED_PARAMETER(target);
    UNUSED_PARAMETER(level);
    UNUSED_PARAMETER(internalformat);
    UNUSED_PARAMETER(width);
    UNUSED_PARAMETER(height);
    UNUSED_PARAMETER(border);
    UNUSED_PARAMETER(format);
    UNUSED_PARAMETER(type);
    UNUSED_PARAMETER(data);

    if (ctx.isES2())
    {
      switch( target )
      {
        case GL_PROXY_TEXTURE_CUBE_MAP:
          Warning( "Regal does not support PROXY_TEXTURE_CUBE_MAP as target for TexImage2D for ES 2.0 profile" );
          return true;
        default:
          break;
      }
    }

    return false;
  }
}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
