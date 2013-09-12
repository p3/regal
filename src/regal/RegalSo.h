/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2013 Scott Nations
  Copyright (c) 2013 Mathias Schott
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

/*

 Regal sampler object emulation.
 Scott Nations

 */

#ifndef __REGAL_SO_H__
#define __REGAL_SO_H__

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <map>

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#define REGAL_NUM_TEXTURE_TARGETS 16

namespace Emu {

struct So
{
    So()
    : activeTextureUnit(0),
      nextSamplerObjectId(1),
      supportSrgb(false)
    {
    }

    ~So()
    {
    }

    void Init( RegalContext &ctx )
    {
        activeTextureUnit = 0;
        nextSamplerObjectId = 1;

        // Enable or disable emulation for SRGB textures.
        //
        // Desktop - http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
        // ES 2.0  - http://www.khronos.org/registry/gles/extensions/EXT/EXT_sRGB.txt

        supportSrgb = ctx.info->gl_ext_texture_srgb_decode || ctx.info->gl_ext_srgb;
        noSamplersInUse = true;
    }

    void Cleanup( RegalContext &ctx )
    {
        UNUSED_PARAMETER(ctx);
    }

    static GLenum TT_Index2Enum(size_t index)
    {
      if (index < array_size( index2Enum ))
      {
        RegalAssertArrayIndex( index2Enum, index );
        return index2Enum[index];
      }

      Warning( "Unhandled texture target index: index = ", index);
      return GL_TEXTURE_2D;
    }

    static GLuint TT_Enum2Index(GLenum texture)
    {
        switch (texture)
        {
            case GL_TEXTURE_1D:                   return 0;
            case GL_TEXTURE_2D:                   return 1;
            case GL_TEXTURE_3D:                   return 2;
            case GL_TEXTURE_1D_ARRAY:             return 3;
            case GL_TEXTURE_2D_ARRAY:             return 4;
            case GL_TEXTURE_RECTANGLE:            return 5;
            case GL_TEXTURE_CUBE_MAP:             return 6;
            case GL_TEXTURE_CUBE_MAP_ARRAY:       return 7;
            case GL_TEXTURE_2D_MULTISAMPLE:       return 8;
            case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return 9;
            case GL_TEXTURE_CUBE_MAP_POSITIVE_X:  return 10;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:  return 11;
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:  return 12;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:  return 13;
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:  return 14;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:  return 15;
            default:
                Warning( "Unhandled texture target enum: texture = ", Token::GLenumToString(texture));
                break;
        }
        return ~(GLuint(0));
    }

    struct Version
    {
        Version()
        : val( 0 )
        , updated( false )
        {}
        GLuint64 Current() const {
            return val;
        }
        GLuint64 Update() {
            if( updated == false ) {
                val++;
                updated = true;
            }
          return val;
        }
        void Reset() {
            updated = false;
        }
        GLuint64 val;
        bool updated;
    };

    struct SamplingState
    {
        GLuint64 ver;
        GLuint name;

        GLfloat BorderColor[4];   //  Border color
        GLenum MinFilter;         //  Minification function
        GLenum MagFilter;         //  Magnification function
        GLenum WrapS;             //  Texcoord s wrap mode
        GLenum WrapT;             //  Texcoord t wrap mode
        GLenum WrapR;             //  Texcoord r wrap mode
        GLfloat MinLod;           //  Minimum level of detail
        GLfloat MaxLod;           //  Maximum level of detail
        GLfloat LodBias;          //  Texture level of detail bias (biastexobj)
        GLenum CompareMode;       //  Comparison mode
        GLenum CompareFunc;       //  Comparison function
        GLenum SrgbDecodeExt;     //  sRGB decode mode
        GLfloat MaxAnisotropyExt; //  maximum degree of anisotropy

        SamplingState()
        : ver(0)
        , MinFilter(GL_NEAREST_MIPMAP_LINEAR)
        , MagFilter(GL_LINEAR)
        , WrapS(GL_REPEAT)
        , WrapT(GL_REPEAT)
        , WrapR(GL_REPEAT)
        , MinLod(-1000.0)
        , MaxLod(1000.0)
        , LodBias(0.0)
        , CompareMode(GL_NONE)
        , CompareFunc(GL_LEQUAL)
        , SrgbDecodeExt(GL_DECODE_EXT)
        , MaxAnisotropyExt(1.0)
        {
            BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0;
        }

    };

    struct TextureState
    {
        TextureState()
        : name( 0 )
        , target( 0 )
        , samplerName( 0 )
        , samplerVer( 0 )
        {}
        GLuint name;
        GLenum target;
        GLuint samplerName;
        GLuint64 samplerVer;
        SamplingState app;
        SamplingState drv;
    };

    struct TextureUnit
    {
        SamplingState* boundSamplerObject;
        TextureState* boundTextureObjects[REGAL_NUM_TEXTURE_TARGETS];

        TextureUnit()
        : boundSamplerObject(NULL)
        {
            size_t n = array_size( boundTextureObjects );
            for (size_t tti = 0; tti < n; tti++)
            {
                RegalAssertArrayIndex( boundTextureObjects, tti );
                boundTextureObjects[tti] = NULL;
            }
        }

    };

    template <typename T> bool SamplerParameter( RegalContext &ctx, GLuint sampler, GLenum pname, T param )
    {
        if ( pname == GL_TEXTURE_BORDER_COLOR)
            return false;
        return SamplerParameterv(ctx, sampler, pname, &param);
    }

    template <typename T> bool SamplerParameterv( RegalContext &ctx, GLuint sampler, GLenum pname, T * params )
    {
        if (!sampler || samplerObjects.count(sampler) < 1)
            return false;

        SamplingState *ss = samplerObjects[sampler];

        if (!ss)
            return false;

        switch (pname)
        {
            case GL_TEXTURE_BORDER_COLOR:
                ss->BorderColor[0] = static_cast<GLfloat>(params[0]);
                ss->BorderColor[1] = static_cast<GLfloat>(params[1]);
                ss->BorderColor[2] = static_cast<GLfloat>(params[2]);
                ss->BorderColor[3] = static_cast<GLfloat>(params[3]);
                break;

            case GL_TEXTURE_COMPARE_FUNC:
                ss->CompareFunc = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_COMPARE_MODE:
                ss->CompareMode = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_LOD_BIAS:
                ss->LodBias = static_cast<GLfloat>(*params);
                break;

            case GL_TEXTURE_MAX_ANISOTROPY_EXT:
                if (!ctx.info->gl_ext_texture_filter_anisotropic)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (GL_EXT_texture_filter_anisotropic extension not available), skipping.");
                    return true;
                }
                ss->MaxAnisotropyExt = static_cast<GLfloat>(*params);
                break;

            case GL_TEXTURE_MAG_FILTER:
                ss->MagFilter = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_MAX_LOD:
                ss->MaxLod = static_cast<GLfloat>(*params);
                break;

            case GL_TEXTURE_MIN_FILTER:
                ss->MinFilter = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_MIN_LOD:
                ss->MinLod = static_cast<GLfloat>(*params);
                break;

            case GL_TEXTURE_WRAP_R:
                ss->WrapR = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_WRAP_S:
                ss->WrapS = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_WRAP_T:
                ss->WrapT = static_cast<GLenum>(*params);
                break;

            case GL_TEXTURE_SRGB_DECODE_EXT:
                if (!supportSrgb)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (sRGB extension not available), skipping.");
                    return true;
                }
                ss->SrgbDecodeExt = static_cast<GLenum>(*params);
                break;

            default:
                Warning( "Unhandled texture parameter enum: pname = ", Token::GLenumToString(pname));
                return false;
        }

        ss->ver = mainVer.Update();

        return true;
    }

    template <typename T> bool GetSamplerParameterv( RegalContext &ctx, GLuint sampler, GLenum pname, T * params )
    {
        if (!sampler || samplerObjects.count(sampler) < 1)
            return false;

        SamplingState *ss = samplerObjects[sampler];

        switch (pname)
        {
            case GL_TEXTURE_BORDER_COLOR:
                params[0] = static_cast<T>(ss->BorderColor[0]);
                params[1] = static_cast<T>(ss->BorderColor[1]);
                params[2] = static_cast<T>(ss->BorderColor[2]);
                params[3] = static_cast<T>(ss->BorderColor[3]);
                break;

            case GL_TEXTURE_COMPARE_FUNC:
                *params = static_cast<T>(ss->CompareFunc);
                break;

            case GL_TEXTURE_COMPARE_MODE:
                *params = static_cast<T>(ss->CompareMode);
                break;

            case GL_TEXTURE_LOD_BIAS:
                *params = static_cast<T>(ss->LodBias);
                break;

            case GL_TEXTURE_MAX_ANISOTROPY_EXT:
                if (!ctx.info->gl_ext_texture_filter_anisotropic)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (GL_EXT_texture_filter_anisotropic extension not available), skipping.");
                    return true;
                }
                *params = static_cast<T>(ss->MaxAnisotropyExt);
                break;

            case GL_TEXTURE_MAG_FILTER:
                *params = static_cast<T>(ss->MagFilter);
                break;

            case GL_TEXTURE_MAX_LOD:
                *params = static_cast<T>(ss->MaxLod);
                break;

            case GL_TEXTURE_MIN_FILTER:
                *params = static_cast<T>(ss->MinFilter);
                break;

            case GL_TEXTURE_MIN_LOD:
                *params = static_cast<T>(ss->MinLod);
                break;

            case GL_TEXTURE_WRAP_R:
                *params = static_cast<T>(ss->WrapR);
                break;

            case GL_TEXTURE_WRAP_S:
                *params = static_cast<T>(ss->WrapS);
                break;

            case GL_TEXTURE_WRAP_T:
                *params = static_cast<T>(ss->WrapT);
                break;

            case GL_TEXTURE_SRGB_DECODE_EXT:
                if (!supportSrgb)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (sRGB extension not available), skipping.");
                    return true;
                }
                *params = static_cast<T>(ss->SrgbDecodeExt);
                break;

            default:
                Warning( "Unhandled sampler parameter enum: pname = ", Token::GLenumToString(pname));
                return false;
        }
        return true;
    }

    template <typename T> bool TexParameter( RegalContext &ctx, GLenum target, GLenum pname, T param )
    {
        if ( target == GL_TEXTURE_BORDER_COLOR)
            return false;
        return TexParameterv(ctx, target, pname, &param);
    }

    template <typename T> bool TexParameterv( RegalContext &ctx, GLenum target, GLenum pname, T * params )
    {
        Internal("Regal::So::TexParameterv",&ctx," target=",Token::GLenumToString(target)," pname=",Token::GLenumToString(pname));

        GLuint tti = TT_Enum2Index(target);

        if (tti >= REGAL_NUM_TEXTURE_TARGETS)
            return false;

        RegalAssertArrayIndex( textureUnits, activeTextureUnit );
        if (activeTextureUnit >= array_size( textureUnits ))
            return false;

        TextureUnit &tu = textureUnits[activeTextureUnit];

        RegalAssertArrayIndex( tu.boundTextureObjects, tti );
        TextureState* ts = tu.boundTextureObjects[tti];

        SamplingState *as = NULL;

        if (!ts)
          return false;

        as = &ts->app;

        switch (pname)
        {
            case GL_TEXTURE_BORDER_COLOR:
                as->BorderColor[0] = static_cast<GLfloat>(params[0]);
                as->BorderColor[1] = static_cast<GLfloat>(params[1]);
                as->BorderColor[2] = static_cast<GLfloat>(params[2]);
                as->BorderColor[3] = static_cast<GLfloat>(params[3]);
                break;

            case GL_TEXTURE_MIN_FILTER:
                as->MinFilter = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_MAG_FILTER:
                as->MagFilter = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_WRAP_S:
                as->WrapS = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_WRAP_T:
                as->WrapT = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_WRAP_R:
                as->WrapR = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_COMPARE_MODE:
                as->CompareMode = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_COMPARE_FUNC:
                as->CompareFunc = static_cast<GLint>(params[0]);
                break;

            case GL_TEXTURE_MIN_LOD:
                as->MinLod = static_cast<GLfloat>(params[0]);
                break;

            case GL_TEXTURE_MAX_LOD:
                as->MaxLod = static_cast<GLfloat>(params[0]);
                break;

            case GL_TEXTURE_LOD_BIAS:
                as->LodBias = static_cast<GLfloat>(params[0]);
                break;

            case GL_TEXTURE_MAX_ANISOTROPY_EXT:
                if (!ctx.info->gl_ext_texture_filter_anisotropic)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (GL_EXT_texture_filter_anisotropic extension not available), skipping.");
                    return true;
                }
                as->MaxAnisotropyExt = static_cast<GLfloat>(params[0]);
                break;

            case GL_TEXTURE_SRGB_DECODE_EXT:
                if (!supportSrgb)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (sRGB extension not available), skipping.");
                    return true;
                }
                as->SrgbDecodeExt = static_cast<GLenum>(params[0]);
                break;

            default:
                return false;
        }
        bool passthru = ts->samplerName == 0 && ts->samplerVer == as->ver;
        as->ver = mainVer.Update();
        if (passthru) {
          ts->samplerVer = as->ver;
        }
        return !passthru;
    }

    template <typename T> bool GetTexParameterv( RegalContext &ctx, GLenum target, GLenum pname, T * params )
    {
        GLuint tti = TT_Enum2Index(target);

        if (tti >= REGAL_NUM_TEXTURE_TARGETS)
            return false;

        RegalAssertArrayIndex( textureUnits, activeTextureUnit );
        if (activeTextureUnit >= array_size( textureUnits ))
            return false;

        TextureUnit &tu = textureUnits[activeTextureUnit];

        RegalAssertArrayIndex( tu.boundTextureObjects, tti );
        TextureState* txs = tu.boundTextureObjects[tti];

        if (!txs)
            return false;

        GLuint tex = txs->name;

        if (!tex || textureObjects.count(tex) < 1)
            return false;

        SamplingState *ts = &textureObjects[tex]->app;

        switch (pname)
        {
            case GL_MAX_VERTEX_ATTRIBS:
                *params = static_cast<T>(ctx.info->max_vertex_attribs);
                break;
            case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
                *params = static_cast<T>(ctx.info->max_combined_texture_image_units);
                break;
            case GL_MAX_TEXTURE_COORDS:
                *params = static_cast<T>(ctx.info->max_texture_coords);
                break;
            case GL_MAX_TEXTURE_UNITS:
                *params = static_cast<T>(ctx.info->max_texture_units);
                break;
            case GL_TEXTURE_BORDER_COLOR:
                params[0] = static_cast<T>(ts->BorderColor[0]);
                params[1] = static_cast<T>(ts->BorderColor[1]);
                params[2] = static_cast<T>(ts->BorderColor[2]);
                params[3] = static_cast<T>(ts->BorderColor[3]);
                break;

            case GL_TEXTURE_COMPARE_FUNC:
                *params = static_cast<T>(ts->CompareFunc);
                break;

            case GL_TEXTURE_COMPARE_MODE:
                *params = static_cast<T>(ts->CompareMode);
                break;

            case GL_TEXTURE_LOD_BIAS:
                *params = static_cast<T>(ts->LodBias);
                break;

            case GL_TEXTURE_MAX_ANISOTROPY_EXT:
                if (!ctx.info->gl_ext_texture_filter_anisotropic)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (GL_EXT_texture_filter_anisotropic extension not available), skipping.");
                    return true;
                }
                *params = static_cast<T>(ts->MaxAnisotropyExt);
                break;

            case GL_TEXTURE_MAG_FILTER:
                *params = static_cast<T>(ts->MagFilter);
                break;

            case GL_TEXTURE_MAX_LOD:
                *params = static_cast<T>(ts->MaxLod);
                break;

            case GL_TEXTURE_MIN_FILTER:
                *params = static_cast<T>(ts->MinFilter);
                break;

            case GL_TEXTURE_MIN_LOD:
                *params = static_cast<T>(ts->MinLod);
                break;

            case GL_TEXTURE_WRAP_R:
                *params = static_cast<T>(ts->WrapR);
                break;

            case GL_TEXTURE_WRAP_S:
                *params = static_cast<T>(ts->WrapS);
                break;

            case GL_TEXTURE_WRAP_T:
                *params = static_cast<T>(ts->WrapT);
                break;

            case GL_TEXTURE_SRGB_DECODE_EXT:
                if (!supportSrgb)
                {
                    Warning("Unsupported sampler parameter ",Token::GLenumToString(pname)," (sRGB extension not available), skipping.");
                    return true;
                }
                *params = static_cast<T>(ts->SrgbDecodeExt);
                break;

            default:
                return false;
        }
        return true;
    }

    template <typename T> bool Get( GLenum pname, T * params )
    {
        GLint tti = -1;

        switch (pname)
        {
            case GL_ACTIVE_TEXTURE:
                *params = static_cast<T>(GL_TEXTURE0+activeTextureUnit);
                return true;

            case GL_SAMPLER_BINDING:
                {
                    RegalAssertArrayIndex( textureUnits, activeTextureUnit );
                    if (activeTextureUnit >= array_size( textureUnits ))
                        return false;
                    SamplingState *pso = textureUnits[activeTextureUnit].boundSamplerObject;
                    *params = static_cast<T>(pso ? pso->name : 0);
                }
                return true;

            case GL_TEXTURE_BINDING_1D:
                tti = TT_Enum2Index(GL_TEXTURE_1D);
                break;

            case GL_TEXTURE_BINDING_1D_ARRAY:
                tti = TT_Enum2Index(GL_TEXTURE_1D_ARRAY);
                break;

            case GL_TEXTURE_BINDING_2D:
                tti = TT_Enum2Index(GL_TEXTURE_2D);
                break;

            case GL_TEXTURE_BINDING_2D_ARRAY:
                tti = TT_Enum2Index(GL_TEXTURE_2D_ARRAY);
                break;

            case GL_TEXTURE_BINDING_2D_MULTISAMPLE:
                tti = TT_Enum2Index(GL_TEXTURE_2D_MULTISAMPLE);
                break;

            case GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY:
                tti = TT_Enum2Index(GL_TEXTURE_2D_MULTISAMPLE_ARRAY);
                break;

            case GL_TEXTURE_BINDING_3D:
                tti = TT_Enum2Index(GL_TEXTURE_3D);
                break;

            case GL_TEXTURE_BINDING_CUBE_MAP:
                tti = TT_Enum2Index(GL_TEXTURE_CUBE_MAP);
                break;

            case GL_TEXTURE_CUBE_MAP_ARRAY:
                tti = TT_Enum2Index(GL_TEXTURE_CUBE_MAP_ARRAY);
                break;

            case GL_TEXTURE_BINDING_RECTANGLE:
                tti = TT_Enum2Index(GL_TEXTURE_RECTANGLE);
                break;

            default:
                return false;
        }

        if (tti >= REGAL_NUM_TEXTURE_TARGETS)
            return false;

        RegalAssertArrayIndex( textureUnits, activeTextureUnit );
        if (activeTextureUnit >= array_size( textureUnits ))
          return false;

        TextureUnit &tu = textureUnits[activeTextureUnit];

        RegalAssertArrayIndex( tu.boundTextureObjects, tti );
        TextureState* ts = tu.boundTextureObjects[tti];

        *params = static_cast<T>(ts ? ts->name : 0);

        return true;
    }

    void GenTextures(RegalContext &ctx, GLsizei count, GLuint *textures);
    void DeleteTextures(RegalContext &ctx, GLsizei count, const GLuint * textures);
    bool BindTexture(RegalContext &ctx, GLuint unit, GLenum target, GLuint texture);
    bool BindTexture(RegalContext &ctx, GLenum target, GLuint texture);

    void GenSamplers(GLsizei count, GLuint *samplers);
    void DeleteSamplers(GLsizei count, const GLuint * samplers);
    GLboolean IsSampler(GLuint sampler);
    void BindSampler(GLuint unit, GLuint sampler);

    bool ActiveTexture(RegalContext &ctx, GLenum tex);
    void PreDraw(RegalContext &ctx);
    bool SendStateToDriver(RegalContext &ctx, GLuint unit, GLenum target, SamplingState& newSS, SamplingState& oldSS);

    Version mainVer;
    GLuint activeTextureUnit;
    GLuint nextSamplerObjectId;
    bool   supportSrgb;
    bool   noSamplersInUse;
    TextureUnit textureUnits[REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS];
    std::map<GLuint, SamplingState*> samplerObjects;
    std::map<GLuint, TextureState*> textureObjects;
    static const GLenum index2Enum[REGAL_NUM_TEXTURE_TARGETS];
};

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // __REGAL_SO_H__
