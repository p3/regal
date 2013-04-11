/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2013 Jason Allen
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

 Regal texture storage support
 Jason Allen

 */

#ifndef __REGAL_TEX_STO_H__
#define __REGAL_TEX_STO_H__

#include "RegalUtil.h"

#if REGAL_EMU_TEXSTO

REGAL_GLOBAL_BEGIN

#include "RegalEmu.h"
#include <utility>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

  struct TexSto : public RegalEmu
  {
    void Init( RegalContext &ctx )
    {
      UNUSED_PARAMETER(ctx); 
    }

    GLenum BindingFromTarget(GLenum target)
    {
      switch (target) {
        case GL_TEXTURE_1D: return GL_TEXTURE_BINDING_1D;
        case GL_TEXTURE_2D: return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_CUBE_MAP: return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_1D_ARRAY: return GL_TEXTURE_BINDING_1D_ARRAY;
        case GL_TEXTURE_RECTANGLE: return GL_TEXTURE_BINDING_RECTANGLE;
        case GL_TEXTURE_3D: return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_2D_ARRAY: return GL_TEXTURE_BINDING_2D_ARRAY;
        case GL_TEXTURE_CUBE_MAP_ARRAY: return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        default:
            Warning("Unknown binding.");
            break;
      }

      return 0;
    }

    void TextureStorage( RegalContext * ctx, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width )
    {
      DispatchTable & tbl = ctx->dispatcher.emulation;
      for (GLsizei i = 0; i < levels; i++)
      {
        tbl.call(&tbl.glTexImage1D)( target, i, internalformat, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        width = (GLsizei)std::max(1.0f, floor(width / 2.0f));
      }

      GLint id;
      tbl.call(&tbl.glGetIntegerv)( BindingFromTarget(target), &id );
      immutableTextures.insert( id );
    }

    void TextureStorage( RegalContext * ctx, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height )
    {
      DispatchTable & tbl = ctx->dispatcher.emulation;
      for (GLsizei i = 0; i < levels; i++)
      {
        if (target == GL_TEXTURE_CUBE_MAP)
        {
          for (int f = 0; f < 6; f++)
          {
            GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
            tbl.call(&tbl.glTexImage2D)( face, i, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
          }
        }
        else
          tbl.call(&tbl.glTexImage2D)( target, i, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        width = (GLsizei)std::max(1.0f, floor(width / 2.0f));
        if (target != GL_TEXTURE_1D_ARRAY)
          height = (GLsizei)std::max(1.0f, floor(height / 2.0f));
      }

      GLint id;
      tbl.call(&tbl.glGetIntegerv)( BindingFromTarget(target), &id );
      immutableTextures.insert( id );
    }

    void TextureStorage( RegalContext * ctx, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth )
    {
      DispatchTable & tbl = ctx->dispatcher.emulation;
      for (GLsizei i = 0; i < levels; i++)
      {
        tbl.call(&tbl.glTexImage3D)( target, i, internalformat, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        width = (GLsizei)std::max(1.0f, floor(width / 2.0f));
        height = (GLsizei)std::max(1.0f, floor(height / 2.0f));
        if (target != GL_TEXTURE_2D_ARRAY && target != GL_TEXTURE_CUBE_MAP_ARRAY)
          depth = (GLsizei)std::max(1.0f, floor(depth / 2.0f));
      }

      GLint id;
      tbl.call(&tbl.glGetIntegerv)( BindingFromTarget(target), &id );
      immutableTextures.insert( id );
    }

    template< typename T >
    bool GetTexParameterv( RegalContext * ctx, GLenum target, GLenum pname, T * params )
    {
      if (pname != GL_TEXTURE_IMMUTABLE_FORMAT)
        return false;

      DispatchTable & tbl = ctx->dispatcher.emulation;

      GLint id;
      tbl.call(&tbl.glGetIntegerv)( BindingFromTarget(target), &id );
      
      if (immutableTextures.find( id ) != immutableTextures.end())
        *params = static_cast<T>(GL_TRUE);
      else
        *params = static_cast<T>(GL_FALSE);

      return true;
    }

    void DeleteTextures( RegalContext * ctx, GLsizei n, const GLuint *textures )
    {
      UNUSED_PARAMETER( ctx );

      if (immutableTextures.empty())
        return;

      for( int i  = 0; i < n; i++ ) {
        if( immutableTextures.find(textures[i]) != immutableTextures.end() ) {
           immutableTextures.erase(textures[i]);
        }
      }
    }

    std::set<GLuint> immutableTextures;
  };

}

REGAL_NAMESPACE_END

#endif // REGAL_EMU_TEXSTO
#endif // ! __REGAL_TEX_STO_H__
