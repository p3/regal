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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include "RegalSo.h"
#include "RegalLog.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

const GLenum So::index2Enum[REGAL_NUM_TEXTURE_TARGETS] = { GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


void
So::GenSamplers(GLsizei count, GLuint *samplers)
{
    for (GLsizei ii=0; ii<count; ii++)
    {
        SamplingState* s = new SamplingState();
        s->name = nextSamplerObjectId;
        samplerObjects[ nextSamplerObjectId ] = s;
        *samplers = nextSamplerObjectId;
        samplers++;
        nextSamplerObjectId++;
    }
}

void
So::DeleteSamplers(GLsizei count, const GLuint * samplers)
{
    for (GLsizei ii=0; ii<count; ii++)
    {
        GLuint s = samplers[ii];
        if (s && samplerObjects.count(s) > 0)
        {
            SamplingState* p = samplerObjects[s];
            size_t n = array_size( textureUnits );
            for (size_t unit=0; unit < n; unit++)
            {
                RegalAssertArrayIndex( textureUnits, unit );
                if (textureUnits[unit].boundSamplerObject == p)
                    BindSampler(static_cast<GLuint>(unit), 0);
            }
            samplerObjects.erase(s);
            delete p;
        }
    }
}

GLboolean
So::IsSampler(GLuint sampler)
{
    return samplerObjects.count( sampler ) > 0 ? GL_TRUE : GL_FALSE;
}

void
So::BindSampler(GLuint unit, GLuint so)
{
    if (unit >= array_size( textureUnits ))
    {
        Warning("Texture unit out of range: ", unit, " >= ", REGAL_EMU_MAX_TEXTURE_COORDS);
        return;
    }

    if (so && samplerObjects.count(so) < 1)
    {
        Warning("Unrecognized sampler object: so = ", so);
        return;
    }

    if (so>0)
        noSamplersInUse = false;

    SamplingState *pso = so != 0 ? samplerObjects[so] : NULL;

    RegalAssertArrayIndex( textureUnits, unit );
    textureUnits[unit].boundSamplerObject = pso;
}

void
So::GenTextures(RegalContext &ctx, GLsizei count, GLuint *textures)
{
    ctx.dispatcher.emulation.glGenTextures(count, textures);

    for (GLsizei ii=0; ii<count; ii++)
    {
        GLuint to = textures[ii];
        TextureState* ts = new TextureState();
        ts->name = to;
        textureObjects[ to ] = ts;
    }
}

void
So::DeleteTextures(RegalContext &ctx, GLsizei count, const GLuint * textures)
{
    GLuint originallyActiveUnit = activeTextureUnit;

    for (GLsizei ii=0; ii<count; ii++)
    {
        GLuint t = textures[ii];
        if (t && textureObjects.count(t) > 0)
        {
            TextureState* p = textureObjects[t];

            size_t n = array_size( textureUnits );
            for (size_t unit=0; unit < n; unit++)
            {
                RegalAssertArrayIndex( textureUnits, unit );
                TextureUnit &tu = textureUnits[unit];

                size_t num = array_size( tu.boundTextureObjects );
                for (size_t jj=0; jj<num; jj++)
                {
                    RegalAssertArrayIndex( tu.boundTextureObjects, jj );
                    if (p == tu.boundTextureObjects[jj])
                    {
                        if (activeTextureUnit != unit)
                            ActiveTexture(ctx, static_cast<GLenum>(GL_TEXTURE0 + unit));

                        BindTexture(ctx, static_cast<GLuint>(unit), TT_Index2Enum(jj), 0);
                    }
                }
            }

            textureObjects.erase(t);
            delete p;
        }
    }

    if (activeTextureUnit != originallyActiveUnit)
        ActiveTexture(ctx, GL_TEXTURE0 + originallyActiveUnit );
}

bool
So::BindTexture(RegalContext &ctx, GLenum target, GLuint to)
{
    return BindTexture(ctx, activeTextureUnit, target, to);
}

bool
So::BindTexture(RegalContext &ctx, GLuint unit, GLenum target, GLuint to)
{
    Internal("Regal::So::BindTexture",&ctx," unit=",unit," target=",target," to=",to);

    if (unit >= array_size( textureUnits ))
    {
        Warning("Texture unit out of range: ", unit, " >= ", REGAL_EMU_MAX_TEXTURE_COORDS);
        return false;
    }

    GLuint tti = TT_Enum2Index(target);

    if (tti >= REGAL_NUM_TEXTURE_TARGETS)
    {
        Warning("Invalid texture target: target = ", Token::GLenumToString(target));
        return false;
    }

    TextureState* ts = NULL;

    if (to) {
      if( textureObjects.count(to) < 1)
      {
        ts = new TextureState();
        ts->target = target;
        textureObjects[ to ] = ts;
      } else {
        ts = textureObjects[ to ];
        if( ts->target == 0 ) {
          ts->target = target;
        }
        RegalAssert( ts->target == target );
      }
    }

    DispatchTableGL &tbl = ctx.dispatcher.emulation;

    GLuint originallyActiveUnit = activeTextureUnit;

    if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );

    tbl.glBindTexture(target, to);

    if (activeTextureUnit != originallyActiveUnit)
        ActiveTexture(ctx, GL_TEXTURE0 + originallyActiveUnit );

    RegalAssertArrayIndex( textureUnits, unit );
    textureUnits[unit].boundTextureObjects[tti] = ts;

    return true;
}

bool
So::ActiveTexture( RegalContext &ctx, GLenum texture )
{
  if (!validTextureEnum(texture))
    return false;

  activeTextureUnit = texture - GL_TEXTURE0;
  ctx.dispatcher.emulation.glActiveTexture( texture );
  return true;
}

void
So::PreDraw( RegalContext &ctx )
{
  if (noSamplersInUse)
    return;

  Internal("Regal::So::PreDraw",&ctx);

  GLuint originallyActiveUnit = activeTextureUnit;

  // ignoring sampler objects on the last unit... FIXME - cass
  const int n = static_cast<GLuint>(array_size( textureUnits ));
  for (int unit=n-1; unit >= 0; unit--)
  {
    RegalAssertArrayIndex( textureUnits, unit );
    TextureUnit &tu = textureUnits[unit];
    SamplingState *pSS = tu.boundSamplerObject;

    size_t num = array_size( tu.boundTextureObjects );
    for (size_t tt=0; tt < num; tt++)
    {
      RegalAssertArrayIndex( tu.boundTextureObjects, tt );
      TextureState* ts = tu.boundTextureObjects[tt];
      GLenum target = TT_Index2Enum( tt );
      if( target != GL_TEXTURE_2D )
        continue;

      //Internal( "RegalSo unit", unit, " texture ", ts ? ts->name : 0, " for sampler ", pSS ? pSS->name : 0 );
      if( ts == NULL ) {
        // no texture bound, don't bother trying to make samplers work with the default texture object
        continue;
      }

      if( pSS == NULL && ts->samplerName == 0 && ts->samplerVer == ts->app.ver ) {
        // this texture has no associated sampler
        //Internal( "RegalSo", "no sampler association and up-to-date ", ts->samplerVer );
        continue;
      }

      if( pSS && ts->samplerName == pSS->name && ts->samplerVer == pSS->ver ) {
        // this pair is already associated and up-to-date
        //Internal( "RegalSo", "already associated and up-to-date ", ts->samplerVer );
        continue;
      }

      // otherwise, we need to send down updates
      SamplingState *newState = pSS ? pSS : &ts->app;

      //Internal( "RegalSo", "about to send update samplerVer=", ts->samplerVer, " newState->ver=", newState->ver );
      if (SendStateToDriver(ctx, static_cast<GLuint>(unit), ts->target, *newState, ts->drv)) {
        //Internal( "RegalSo", "updated unit ", unit, " texture ", ts ? ts->name : 0, " for sampler ", pSS ? pSS->name : 0 );
      } else {
        //Internal( "RegalSo", "no update occurred" );
      }
      ts->samplerName = pSS ? pSS->name : 0;
      ts->samplerVer = newState->ver;
    }
  }

  if (activeTextureUnit != originallyActiveUnit)
    ActiveTexture(ctx, GL_TEXTURE0 + originallyActiveUnit );

  mainVer.Reset();
}

bool
So::SendStateToDriver(RegalContext &ctx, GLuint unit, GLenum target, SamplingState& newState, SamplingState& drv)
{
  Internal("Regal::So::SendStateToDriver",&ctx," unit=",unit," target=",target);

  bool sent = false;
  if (target == GL_TEXTURE_2D_MULTISAMPLE ||
      target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY )
  {
    return sent;
  }

  if (REGAL_FORCE_ES2_PROFILE || ctx.info->es2)
  {
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP)
    {
      return sent;
    }
  }

  DispatchTableGL &tbl = ctx.dispatcher.emulation;

  if (newState.BorderColor[0] != drv.BorderColor[0] ||
      newState.BorderColor[1] != drv.BorderColor[1] ||
      newState.BorderColor[2] != drv.BorderColor[2] ||
      newState.BorderColor[3] != drv.BorderColor[3])
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameterfv( target, GL_TEXTURE_BORDER_COLOR, newState.BorderColor);
    drv.BorderColor[0] = newState.BorderColor[0];
    drv.BorderColor[1] = newState.BorderColor[1];
    drv.BorderColor[2] = newState.BorderColor[2];
    drv.BorderColor[3] = newState.BorderColor[3];
    sent = true;
  }

  if (target != GL_TEXTURE_RECTANGLE)
  {
    if (newState.MinFilter != drv.MinFilter)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_MIN_FILTER, newState.MinFilter);
      drv.MinFilter = newState.MinFilter;
      sent = true;
    }

    if (newState.MagFilter != drv.MagFilter)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_MAG_FILTER, newState.MagFilter);
      drv.MagFilter = newState.MagFilter;
      sent = true;
    }

    if (newState.WrapS != drv.WrapS)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_WRAP_S, newState.WrapS);
      drv.WrapS = newState.WrapS;
      sent = true;
    }
  }

  if (target == GL_TEXTURE_2D ||
      target == GL_TEXTURE_3D ||
      target == GL_TEXTURE_CUBE_MAP)
  {
    if (newState.WrapT != drv.WrapT)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_WRAP_T, newState.WrapT);
      drv.WrapT = newState.WrapT;
      sent = true;
    }
  }

  if (target == GL_TEXTURE_3D)
  {
    if (newState.WrapR != drv.WrapR)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_WRAP_R, newState.WrapR);
      drv.WrapR = newState.WrapR;
      sent = true;
    }
  }

  if (newState.MinLod != drv.MinLod)
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameterf( target, GL_TEXTURE_MIN_LOD, newState.MinLod);
    drv.MinLod = newState.MinLod;
    sent = true;
  }

  if (newState.MaxLod != drv.MaxLod)
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameterf( target, GL_TEXTURE_MAX_LOD, newState.MaxLod);
    drv.MaxLod = newState.MaxLod;
    sent = true;
  }

  if (newState.LodBias != drv.LodBias)
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameterf( target, GL_TEXTURE_LOD_BIAS, newState.LodBias);
    drv.LodBias = newState.LodBias;
    sent = true;
  }

  if (newState.CompareMode != drv.CompareMode)
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameteri( target, GL_TEXTURE_COMPARE_MODE, newState.CompareMode);
    drv.CompareMode = newState.CompareMode;
    sent = true;
  }

  if (newState.CompareFunc != drv.CompareFunc)
  {
    if (activeTextureUnit != unit)
      ActiveTexture(ctx, GL_TEXTURE0 + unit );
    tbl.glTexParameteri( target, GL_TEXTURE_COMPARE_FUNC, newState.CompareFunc);
    drv.CompareFunc = newState.CompareFunc;
    sent = true;
  }

  if (supportSrgb)
  {
    if (newState.SrgbDecodeExt != drv.SrgbDecodeExt)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameteri( target, GL_TEXTURE_SRGB_DECODE_EXT, newState.SrgbDecodeExt);
      drv.SrgbDecodeExt = newState.SrgbDecodeExt;
      sent = true;
    }
  }

  if (ctx.info->gl_ext_texture_filter_anisotropic)
  {
    if (newState.MaxAnisotropyExt != drv.MaxAnisotropyExt)
    {
      if (activeTextureUnit != unit)
        ActiveTexture(ctx, GL_TEXTURE0 + unit );
      tbl.glTexParameterf( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, newState.MaxAnisotropyExt);
      drv.MaxAnisotropyExt = newState.MaxAnisotropyExt;
      sent = true;
    }
  }

  return sent;
}

}

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
