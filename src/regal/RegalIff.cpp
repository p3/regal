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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <cstring>

#include <string>
using std::string;

#include <boost/print/string_list.hpp>
typedef boost::print::string_list<string> string_list;

#include "RegalIff.h"
#include "RegalLog.h"
#include "RegalToken.h"
#include "RegalHelper.h"

#include "lookup3.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS 2

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

typedef Iff::State State;
typedef Iff::State::Texture Texture;
typedef Iff::State::Light Light;
typedef Iff::State::MaterialUniform MaterialUniform;
typedef Iff::State::Store Store;
typedef Iff::State::StoreUniform StoreUniform;
typedef Iff::Program Program;

enum FFLightEl {
  LE_Ambient = 0,
  LE_Diffuse = 1,
  LE_Specular = 2,
  LE_Position = 3,
  LE_SpotDir = 4,
  LE_Atten = 5,
  LE_Elements = 6
};
enum FFMaterialEl {
  ME_Ambient = 0,
  ME_Diffuse = 1,
  ME_Specular = 2,
  ME_Emission = 3,
  ME_Shininess = 4,
  ME_Elements = 5
};

static Iff::TextureTargetBitfield TargetToBitfield( GLenum target ) {
  switch( target ) {
    case GL_TEXTURE_1D:        return Iff::TT_1D;
    case GL_TEXTURE_2D:        return Iff::TT_2D;
    case GL_TEXTURE_RECTANGLE: return Iff::TT_Rect;
    case GL_TEXTURE_3D:        return Iff::TT_3D;
    case GL_TEXTURE_CUBE_MAP:  return Iff::TT_CubeMap;
    default: break;
  }
  return Iff::TT_None;
}

static void GenerateVertexShaderSource( const Iff * rff, const Iff::State & state, string_list & src )
{
  Internal("Regal::Iff::GenerateVertexShaderSource",rff);

  const bool gles = rff->gles;
  const bool legacy = rff->legacy;
  const Store & st = state.processed;

  bool hasNormalMap = false;
  bool hasSphereMap = false;
  bool hasReflectionMap = false;
  bool hasEyeLinearTexGen = false;
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    if( st.tex[i].enables == 0 ) {
      continue;
    }
    for( int j = 0; j < 4; j++ ) {
      if( st.tex[i].texgen[j].enable == false ) {
        continue;
      }
      Iff::TexgenMode mode = st.tex[i].texgen[j].mode;
      hasNormalMap = hasNormalMap || mode == Iff::TG_NormalMap;
      hasSphereMap = hasSphereMap || mode == Iff::TG_SphereMap;
      hasReflectionMap = hasReflectionMap || mode == Iff::TG_ReflectionMap;
      hasEyeLinearTexGen = hasEyeLinearTexGen || mode == Iff::TG_EyeLinear;
    }
  }
  bool hasClipPlanes = false;
  for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
    hasClipPlanes = hasClipPlanes || st.clip[i].enable;
  }

  if( gles ) {
#if REGAL_FORCE_DESKTOP_GLSL
    // Desktop version 140 corresponds to OpenGL 3.1,
    // which corresponds to the functionality of ES2.
    src << "#version 140\n";
#else
    src << "#version 100\n";
#endif
  } else if ( legacy ) {
    src << "#version 120\n";
  } else {
    src << "#version 140\n";
  }
  src << "// program number " << rff->progcount << "\n";

  if( gles || legacy ) {
    src << "#define in attribute\n";
    src << "#define out varying\n";
  }

  if( st.shadeModelFlat && ! legacy && ! gles ) {
    src << "#define FLAT flat\n";
  } else {
    src << "#define FLAT  \n";
  }
  if( st.lighting ) {
    src << "\n";
    src << "#define ME_AMBIENT "   << int(ME_Ambient) << "\n";
    src << "#define ME_DIFFUSE "   << int(ME_Diffuse) << "\n";
    src << "#define ME_SPECULAR "  << int(ME_Specular) << "\n";
    src << "#define ME_EMISSION "  << int(ME_Emission) << "\n";
    src << "#define ME_SHININESS " << int(ME_Shininess) << "\n";
    src << "#define ME_ELEMENTS "  << int(ME_Elements) << "\n";
    src << "\n";
    src << "#define LE_AMBIENT "  << int(LE_Ambient) << "\n";
    src << "#define LE_DIFFUSE "  << int(LE_Diffuse) << "\n";
    src << "#define LE_SPECULAR " << int(LE_Specular) << "\n";
    src << "#define LE_POSITION " << int(LE_Position) << "\n";
    src << "#define LE_SPOTDIR "  << int(LE_SpotDir) << "\n";
    src << "#define LE_ATTEN "    << int(LE_Atten) << "\n";
    src << "#define LE_ELEMENTS " << int(LE_Elements) << "\n";
    src << "\n";
  }

  if( gles ) {
    src << "precision highp float;\n";
  }
  if( ~st.attrArrayFlags & ( 1 << rff->ffAttrMap[ RFF2A_Color ] ) ) {
    src << "uniform vec4 rglAttrib[" << REGAL_EMU_IFF_VERTEX_ATTRIBS << "];\n";
  }
  src << "uniform mat4 rglModelview;\n";
  src << "uniform mat4 rglProjection;\n";
  src << "in vec4 rglVertex;\n";
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    if( st.tex[i].useMatrix ) {
      src << "uniform mat4 rglTextureMatrix" << i << ";\n";
    }
  }
  for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
    if( st.clip[i].enable ) {
      src << "uniform vec4 rglClipPlane" << i << ";\n";
      if( gles || legacy ) {
        src << "out float rglClipDistance" << i << ";\n";
      }
    }
  }
  //src << "in vec4 rglWeight;\n";
  if( st.lighting || hasNormalMap || hasSphereMap || hasReflectionMap ) {
    src << "uniform mat4 rglModelviewInverseTranspose;\n";
    src << "in vec3 rglNormal;\n";
  }
  if( st.lighting ) {
    src << "uniform vec4 rglLightModelAmbient;\n";
    src << "uniform vec4 rglMaterialFront" << "[ ME_ELEMENTS ];\n";
    if( st.lightModelTwoSide ) {
      src << "uniform vec4 rglMaterialBack" << "[ ME_ELEMENTS ];\n";
    }
    for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
      if( st.light[ i ].enable ) {
        src << "uniform vec4 rglLight" << i << "[ LE_ELEMENTS ];\n";
      }
    }
  }
  if( st.attrArrayFlags & ( 1 << rff->ffAttrMap[ RFF2A_Color ] ) && ( st.lighting == false || st.colorMaterial ) ) {
    src << "in vec4 rglColor;\n";
  } else {
    src << "#define rglColor rglAttrib[" << rff->ffAttrMap[ RFF2A_Color ] << "]\n";
  }
  if( st.colorSum && st.lighting == false ) {
    src << "in vec4 rglSecondaryColor;\n";
  }
  if( st.fog.enable ) {
    src << "uniform vec4 rglFog[2];\n";
    src << "out vec4 rglFOG;\n";
    if(  st.fog.useDepth == false ) {
      src << "in float rglFogCoord;\n";
    }
  }
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    if( st.tex[i].enables == 0 ) {
      continue;
    }
    src << "in vec4 rglMultiTexCoord" << i << ";\n";
  }
  src << "FLAT out vec4 rglFrontColor;\n";
  if( st.lighting ) {
    if( st.lightModelTwoSide ) {
      src << "FLAT out vec4 rglBackColor;\n";
    }
    if( st.lightModelSeparateSpecular ) {
      src << "out vec4 rglSCOL0;\n";
      if( st.lightModelTwoSide ) {
        src << "out vec4 rglSCOL1;\n";
      }
    }
  } else if( st.colorSum ) {
    src << "out vec4 rglSCOL0;\n";
  }

  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    const State::Texture & t = st.tex[i];
    if( t.enables == 0 ) {
      continue;
    }
    const char *tc[] = { "S", "T", "R", "Q" };
    for( int j = 0; j < 4; j++ ) {
      const State::Texgen & g = t.texgen[j];
      if( g.enable == false ) {
        continue;
      }
      switch( g.mode ) {
        case Iff::TG_EyeLinear:
          src << "uniform vec4 rglTexGen" << i << "Eye" << tc[j] << ";\n";
          break;
        case Iff::TG_ObjectLinear:
          src << "uniform vec4 rglTexGen" << i << "Obj" << tc[j] << ";\n";
          break;
        default:
          src << "//ERROR: unsupported gen mode\n";
          break;
      }
    }
    src << "out vec4 rglTEXCOORD" << i << ";\n";
  }

  if( hasNormalMap ) {
    src << "vec4 ComputeNormalMap( vec3 n ) {\n";
    src << "    return vec4( n.x, n.y, n.z, 0.0 );\n";
    src << "}\n";
  }

  if( hasReflectionMap ) {
    src << "vec4 ComputeReflectionMap( vec3 i, vec3 n ) {\n";
    src << "    vec3 r = reflect( i, n );\n";
    src << "    return vec4( r.x, r.y, r.z, 0.0 );\n";
    src << "}\n";
  }

  if( hasSphereMap ) {
    src << "vec4 ComputeSphereMap( vec3 i, vec3 n ) {\n";
    src << "    vec3 r = reflect( i, n );\n";
    src << "    float minv = 1.0 / sqrt( 2.0 * r.x + 2.0 * r.y + 2.0 * (r.z + 1.0 ) );\n";
    src << "    return vec4( r.x * minv + 0.5 , r.y * minv + 0.5, 0.0, 0.0 );\n";
    src << "}\n";
  }

  src << "void main() {\n";
  src << "    gl_Position = rglProjection * rglModelview * rglVertex;\n";
  if( st.lighting || hasNormalMap || hasReflectionMap || hasEyeLinearTexGen ||
      hasClipPlanes || ( st.fog.enable && st.fog.useDepth ) || hasSphereMap ) {
    src << "    vec4 eh = rglModelview * rglVertex;\n";
  }

  if( st.lighting || hasNormalMap || hasReflectionMap || hasSphereMap ) {
    src << "    vec4 ep = eh / eh.w;\n";
    src << "    vec3 ev = -normalize( ep.xyz );\n";
    src << "    vec3 en = mat3( rglModelviewInverseTranspose ) * rglNormal;\n";
    if( st.normalize ) {
      src << "    en = normalize( en );\n";
    }
  }
  if( st.lighting ) {
    src << "    vec4 mFront[ ME_ELEMENTS ];\n";
    src << "    mFront[ ME_AMBIENT   ] = rglMaterialFront[ ME_AMBIENT   ];\n";
    src << "    mFront[ ME_DIFFUSE   ] = rglMaterialFront[ ME_DIFFUSE   ];\n";
    src << "    mFront[ ME_SPECULAR  ] = rglMaterialFront[ ME_SPECULAR  ];\n";
    src << "    mFront[ ME_EMISSION  ] = rglMaterialFront[ ME_EMISSION  ];\n";
    src << "    mFront[ ME_SHININESS ] = rglMaterialFront[ ME_SHININESS ];\n";
    if( st.lightModelTwoSide ) {
      src << "    vec4 mBack [ ME_ELEMENTS ];\n";
      src << "    mBack [ ME_AMBIENT   ] = rglMaterialBack [ ME_AMBIENT   ];\n";
      src << "    mBack [ ME_DIFFUSE   ] = rglMaterialBack [ ME_DIFFUSE   ];\n";
      src << "    mBack [ ME_SPECULAR  ] = rglMaterialBack [ ME_SPECULAR  ];\n";
      src << "    mBack [ ME_EMISSION  ] = rglMaterialBack [ ME_EMISSION  ];\n";
      src << "    mBack [ ME_SHININESS ] = rglMaterialBack [ ME_SHININESS ];\n";
    }
    if( st.colorMaterial ) {
      switch( st.colorMaterialTarget0 ) {
        case Iff::CM_None:
          break;
        case Iff::CM_Ambient:
          src << "    mFront[ ME_AMBIENT ] = rglColor;\n";
          break;
        case Iff::CM_Diffuse:
          src << "    mFront[ ME_DIFFUSE ] = rglColor;\n";
          break;
        case Iff::CM_Specular:
          src << "    mFront[ ME_SPECULAR ] = rglColor;\n";
          break;
        case Iff::CM_Emission:
          src << "    mFront[ ME_EMISSION ] = rglColor;\n";
          break;
        case Iff::CM_AmbientAndDiffuse:
          src << "    mFront[ ME_AMBIENT ] = rglColor;\n";
          src << "    mFront[ ME_DIFFUSE ] = rglColor;\n";
          break;
        default:
          src << "//ERROR: unsupported color material[0] target\n";
          break;
      }
      if( st.lightModelTwoSide ) {
        switch( st.colorMaterialTarget1 ) {
          case Iff::CM_None:
            break;
          case Iff::CM_Ambient:
            src << "    mBack[ ME_AMBIENT ] = rglColor;\n";
            break;
          case Iff::CM_Diffuse:
            src << "    mBack[ ME_DIFFUSE ] = rglColor;\n";
            break;
          case Iff::CM_Specular:
            src << "    mBack[ ME_SPECULAR ] = rglColor;\n";
            break;
          case Iff::CM_Emission:
            src << "    mBack[ ME_EMISSION ] = rglColor;\n";
            break;
          case Iff::CM_AmbientAndDiffuse:
            src << "    mBack[ ME_AMBIENT ] = rglColor;\n";
            src << "    mBack[ ME_DIFFUSE ] = rglColor;\n";
            break;
          default:
            src << "//ERROR: unsupported color material[1] target\n";
            break;
        }
      }
    }
    src << "    rglFrontColor.xyz = mFront[ ME_EMISSION ].xyz;\n";
    src << "    rglFrontColor.w = mFront[ ME_DIFFUSE ].w;\n";
    src << "    rglFrontColor.xyz += rglLightModelAmbient.xyz * mFront[ ME_AMBIENT ].xyz;\n";
    if( st.lightModelSeparateSpecular ) {
      src << "    rglSCOL0 = vec4( 0, 0, 0, 0);\n";
    }
    if( st.lightModelTwoSide ) {
      src << "    rglBackColor.xyz = mBack[ ME_EMISSION ].xyz;\n";
      src << "    rglBackColor.w = mBack[ ME_DIFFUSE ].w;\n";
      src << "    rglBackColor.xyz += rglLightModelAmbient.xyz * mBack[ ME_AMBIENT ].xyz;\n";
      if( st.lightModelSeparateSpecular ) {
        src << "    rglSCOL1 = vec4( 0, 0, 0, 0);\n";
      }
    }
    for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
      if( st.light[ i ].enable ) {
        src << "    {\n";
        string attenmul = "";
        src << "        vec4 lvec = rglLight" << i << "[ LE_POSITION ];\n";
        if( st.light[ i ].attenuate || st.light[ i ].spotlight ) {
          src << "        float att = 1.0;\n";
          attenmul = "att *";
        }
        if( st.light[ i ].local ) {
          src << "        lvec.xyz -= ep.xyz;\n";
          src << "        float ad = sqrt( dot( lvec.xyz, lvec.xyz ) );\n";
          src << "        lvec.xyz /= ad;\n";
        }
        if( st.lightModelLocalViewer ) {
          src << "        vec3 hvec = normalize( lvec.xyz + ev );\n";
        } else {
          src << "        vec3 hvec = normalize( lvec.xyz + vec3( 0, 0, 1 ) );\n";
        }
        src << "        vec3 ambient = rglLight" << i << "[ LE_AMBIENT ].xyz * mFront[ ME_AMBIENT ].xyz;\n";
        src << "        float dc = max( dot( en, lvec.xyz ), 0.0 );\n";
        src << "        float sc = max( dot( en, hvec ), 0.0 );\n";
        src << "        vec3 diffuse = dc * rglLight" << i << "[ LE_DIFFUSE ].xyz * mFront[ ME_DIFFUSE ].xyz;\n";
        src << "        sc = ( dc > 0.0 && sc > 0.0 ) ? exp( mFront[ ME_SHININESS ].x * log( sc ) ) : 0.0;\n";
        src << "        vec3 specular = sc * rglLight" << i << "[ LE_SPECULAR ].xyz * mFront[ ME_SPECULAR ].xyz;\n";
        if( st.light[ i ].attenuate && st.light[ i ].local ) {
          src << "        vec3 dist = vec3( 1.0, ad, ad * ad );\n";
          src << "        att = 1.0 / dot( dist, rglLight" << i << "[ LE_ATTEN ].xyz );\n";
        }
        if( st.light[ i ].spotlight ) {
          src << "        float spcut = cos( radians( rglLight" << i << "[ LE_SPOTDIR ].w ) );\n";
          src << "        float sd = dot( rglLight" << i << "[ LE_SPOTDIR ].xyz, lvec.xyz );\n";
          src << "        att *= ( sd > spcut ) ? exp( rglLight" << i << "[ LE_ATTEN ].w * log( sd ) ) : 0.0;\n";
        }

        if( st.lightModelSeparateSpecular ) {
          src << "        rglFrontColor.xyz += " << attenmul << " ( ambient + diffuse );\n";
          src << "        rglSCOL0.xyz += " << attenmul << " specular;\n";
        } else {
          src << "        rglFrontColor.xyz += " << attenmul << " ( ambient + diffuse + specular );\n";
        }
        if( st.lightModelTwoSide ) {
          src << "        ambient = rglLight" << i << "[ LE_AMBIENT ].xyz * mBack[ ME_AMBIENT ].xyz;\n";
          src << "        dc = max( dot( -en, lvec.xyz ), 0.0 );\n";
          src << "        sc = max( dot( -en, hvec ), 0.0 );\n";
          src << "        diffuse = dc * rglLight" << i << "[ LE_DIFFUSE ].xyz * mBack[ ME_DIFFUSE ].xyz;\n";
          src << "        sc = ( dc > 0.0 && sc > 0.0 ) ? exp( mBack[ ME_SHININESS ].x * log( sc ) ) : 0.0;\n";
          src << "        specular = sc * rglLight" << i << "[ LE_SPECULAR ].xyz * mBack[ ME_SPECULAR ].xyz;\n";
          if( st.lightModelSeparateSpecular ) {
            src << "        rglBackColor.xyz += " << attenmul << " ( ambient + diffuse );\n";
            src << "        rglSCOL1.xyz += " << attenmul << " specular;\n";
          } else {
            src << "        rglBackColor.xyz += " << attenmul << " ( ambient + diffuse + specular );\n";
          }
        }
        src << "    }\n";
      }
    }
  } else {
    src << "    rglFrontColor = rglColor;\n";
    if( st.colorSum ) {
      src << "    rglSCOL0 = rglSecondaryColor;\n";
    }
  }

  // clamp all the output colors to (0.0, 1.0)

  src << "    rglFrontColor = clamp( rglFrontColor, 0.0, 1.0 );\n";
  if( st.lighting ) {
    if( st.lightModelTwoSide ) {
      src << "    rglBackColor = clamp( rglBackColor, 0.0, 1.0 );\n";
    }
    if( st.lightModelSeparateSpecular ) {
      src << "    rglSCOL0 = clamp( rglSCOL0, 0.0, 1.0 );\n";
      if( st.lightModelTwoSide ) {
        src << "    rglSCOL1 = clamp( rglSCOL1, 0.0, 1.0 );\n";
      }
    }
  } else if( st.colorSum ) {
    src << "    rglSCOL0 = clamp( rglSCOL0, 0.0, 1.0 );\n";
  }

  if( st.fog.enable ) {
    if( st.fog.useDepth ) {
      src << "    rglFOG = eh;\n";
    } else {
      src << "    rglFOG = vec4(0, 0, -rglFogCoord, 1);\n";
    }
  }

  if( hasNormalMap ) {
    src << "    vec4 nmTc = ComputeNormalMap( en );\n";
  }
  if( hasReflectionMap ) {
    src << "    vec4 rmTc = ComputeReflectionMap( -ev, en );\n";
  }
  if( hasSphereMap ) {
    src << "    vec4 smTc = ComputeSphereMap( -ev, en );\n";
  }

  bool tc_declared = false;
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    const State::Texture & t = state.processed.tex[i];
    if( t.enables == 0 ) {
      continue;
    }
    if ( !tc_declared ) {
      src << "    vec4 tc;\n";
      tc_declared = true;
    }
    src << "    tc = rglMultiTexCoord" << i << ";\n";
    const char *comp[] = { "x", "y", "z", "w" };
    const char *tc[] = { "S", "T", "R", "Q" };
    for( int j = 0; j < 4; j++ ) {
      if( t.texgen[j].enable == false ) {
        continue;
      }
      switch( t.texgen[j].mode ) {
        case Iff::TG_ObjectLinear:
          src << "    tc." << comp[j] << " = dot( rglTexGen" << i << "Obj" << tc[j] << ", rglVertex );\n";
          break;
        case Iff::TG_EyeLinear:
          src << "    tc." << comp[j] << " = dot( rglTexGen" << i << "Eye" << tc[j] << ", eh );\n";
          break;
        case Iff::TG_NormalMap:
          src << "    tc." << comp[j] << " = nmTc." << comp[j] << ";\n";
          break;
        case Iff::TG_ReflectionMap:
          src << "    tc." << comp[j] << " = rmTc." << comp[j] << ";\n";
          break;
        case Iff::TG_SphereMap:
          src << "    tc." << comp[j] << " = smTc." << comp[j] << ";\n";
          break;
        default:
          src << "//ERROR: unsupported tex gen mode\n";
          break;
      }
    }
    if( t.useMatrix ) {
      src << "    rglTEXCOORD" << i << " = rglTextureMatrix" << i << " * tc;\n";
    } else {
      src << "    rglTEXCOORD" << i << " = tc;\n";
    }
  }
  for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
    if( st.clip[i].enable ) {
      if( gles || legacy ) {
        src << "    rglClipDistance" << i;
      } else {
        src << "    gl_ClipDistance[" << i << "]";
      }
      src << " = dot( eh, rglClipPlane" << i << " );\n";
    }
  }
  src << "}\n";
}



static void AddTexEnv( int i, Iff::TexenvMode env, GLenum fmt,  string_list & s )
{
  Internal("Regal::Iff::AddTexEnv","()");

  switch( env ) {
    case Iff::TEM_Replace:
      switch( fmt ) {
        case GL_ALPHA:
          s << "    p.w = s.w;\n";
          break;
        case GL_LUMINANCE:
        case GL_RGB:
          s << "    p.xyz = s.xyz;\n";
          break;
        case GL_LUMINANCE_ALPHA:
        case GL_RGBA:
          s << "    p = s;\n";
          break;
        default:
          s << "    p = s; //ERROR: Unsupported tex fmt " << Token::GLenumToString(fmt) << "\n";
          break;
      }
      break;
    case Iff::TEM_Modulate:
      switch( fmt ) {
        case GL_ALPHA:
          s << "    p.w *= s.w;\n";
          break;
        case GL_LUMINANCE:
        case GL_RGB:
          s << "    p.xyz *= s.xyz;\n";
          break;
        case GL_LUMINANCE_ALPHA:
        case GL_RGBA:
          s << "    p *= s;\n";
          break;
        default:
          s << "    p = s; //ERROR: Unsupported tex fmt" << Token::GLenumToString(fmt) << "\n";
          break;
      }
      break;
    case Iff::TEM_Blend:
      switch( fmt ) {
        case GL_ALPHA:
          s << "    p.w *= s.w;\n";
          break;
        case GL_LUMINANCE:
        case GL_RGB:
          s << "    p.xyz = mix( p.xyz, rglTexEnvColor" << i << ".xyz, s.xyz );\n";
          break;
        case GL_LUMINANCE_ALPHA:
        case GL_RGBA:
          s << "    p.xyz = mix( p.xyz, rglTexEnvColor" << i << ".xyz, s.xyz );\n";
          s << "    p.w *= s.w;\n";
          break;
        default:
          s << "    p = s; //ERROR: Unsupported tex fmt " << Token::GLenumToString(fmt) << "\n";
          break;
      }
      break;
    case Iff::TEM_Add:
      switch( fmt ) {
        case GL_ALPHA:
          s << "    p.w *= s.w;\n";
          break;
        case GL_LUMINANCE:
        case GL_RGB:
          s << "    p.xyz += s.xyz;\n";
          break;
        case GL_LUMINANCE_ALPHA:
        case GL_RGBA:
          s << "    p.xyz += s.xyz;\n";
          s << "    p.w *= s.w;\n";
          break;
        default:
          s << "    p = s; //ERROR: Unsupported tex fmt " << Token::GLenumToString(fmt) << "\n";
          break;
      }
      break;
    case Iff::TEM_Decal:
      switch( fmt ) {
        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA:
          s << "    //ERROR: tex env mode undefined for texture format " << Token::GLenumToString(fmt) << "\n";
          break;
        case GL_RGB:
          s << "    p.xyz = s.xyz;\n";
          break;
        case GL_RGBA:
          s << "    p.xyz = mix( p.xyz, s.xyz, s.w );\n";
          break;
        default:
          s << "    p = s; //ERROR: Unsupported tex fmt " << Token::GLenumToString(fmt) << "\n";
          break;
      }
      break;
    default:
      s << "    //ERROR: Unsupported tex env mode\n";
      break;
  }
}

static void AddTexEnvCombine( Iff::TextureEnv & env, string_list & s )
{
  Internal("Regal::Iff::AddTexEnvCombine","()");

  bool skipAlpha = env.rgb.mode == Iff::TEC_Dot3Rgba;
  int rgbSources = 0;
  int aSources = 0;
  s << "    {\n";
  switch( env.rgb.mode ) {
    case Iff::TEC_Replace:
      rgbSources = 1;
      break;
    case Iff::TEC_Modulate:
    case Iff::TEC_Add:
    case Iff::TEC_AddSigned:
    case Iff::TEC_Dot3Rgb:
    case Iff::TEC_Dot3Rgba:
      rgbSources = 2;
      break;
    case Iff::TEC_Interpolate:
      rgbSources = 3;
      break;
    default:
      break;
  }
  if( skipAlpha == false ) {
    switch( env.a.mode ) {
      case Iff::TEC_Replace:
        aSources = 1;
        break;
      case Iff::TEC_Modulate:
      case Iff::TEC_Add:
      case Iff::TEC_AddSigned:
      case Iff::TEC_Subtract:
      case Iff::TEC_Dot3Rgb:
      case Iff::TEC_Dot3Rgba:
        aSources = 2;
        break;
      case Iff::TEC_Interpolate:
        aSources = 3;
        break;
      default:
        break;
    }
  }
  for( int i = 0; i < rgbSources; i++ ) {
    bool skip = false;
    string source;
    Iff::TexenvCombineSrc src = i == 0 ? env.rgb.src0 : i == 1 ? env.rgb.src1 : env.rgb.src2;
    switch( src ) {
      case Iff::TCS_PrimaryColor:
        source = "rglFrontColor";
        break;
      case Iff::TCS_Constant:
        source = "rglConstantColor";
        break;
      case Iff::TCS_Previous:
        source = "p";
        break;
      case Iff::TCS_Texture:
        source = "s";
        break;
      default:
        skip = true;
        break;
    }
    string suffix;
    Iff::TexenvCombineOp op = i == 0 ? env.rgb.op0 : i == 1 ? env.rgb.op1 : env.rgb.op2;
    switch( op ) {
      case Iff::TCO_Alpha:
      case Iff::TCO_OneMinusAlpha:
        suffix = ".www";
        break;
      case Iff::TCO_Color:
      case Iff::TCO_OneMinusColor:
        suffix = ".xyz";
        break;
      default:
        skip = true;
        break;
    }
    if ( skip ) {
      s << "        vec3 csrc" << i << " = vec3(0.0f, 0.0f, 0.0f);\n";
    } else {
      s << "        vec3 csrc" << i << " = ";
      if( op == Iff::TCO_OneMinusColor || op == Iff::TCO_OneMinusAlpha ) {
        s << "1 - ";
      }
      s << source << suffix << ";\n";
    }
  }
  switch( env.rgb.mode ) {
    case Iff::TEC_Replace:
      s << "        p.xyz = csrc0;\n";
      break;
    case Iff::TEC_Modulate:
      s << "        p.xyz = csrc0 * csrc1;\n";
      break;
    case Iff::TEC_Add:
      s << "        p.xyz = csrc0 + csrc1;\n";
      break;
    case Iff::TEC_AddSigned:
      s << "        p.xyz = csrc0 + csrc1 - 0.5;\n";
      break;
    case Iff::TEC_Subtract:
      s << "        p.xyz = csrc0 - csrc1;\n";
      break;
    case Iff::TEC_Dot3Rgb:
    case Iff::TEC_Dot3Rgba:
      s << "        p.xyz = dot( ( 2.0 * csrc0 - 1.0 ), ( 2.0 * csrc1 - 1.0 ) );\n";
      break;
    case Iff::TEC_Interpolate:
      s << "        p.xyz = mix( csrc0, csrc1, csrc2);\n";
      break;
    default:
      s << "//ERROR: Unsupported tex env combine rgb mode\n"; break;
      break;
  }
  if( skipAlpha ) {
    s << "        p.w = p.x;\n";
  } else {
    for( int i = 0; i < aSources; i++ ) {
      bool skip = false;
      string source;
      Iff::TexenvCombineSrc src = i == 0 ? env.a.src0 : i == 1 ? env.a.src1 : env.a.src2;
      switch( src ) {
        case Iff::TCS_PrimaryColor:
          source = "rglFrontColor";
          break;
        case Iff::TCS_Constant:
          source = "rglConstantColor";
          break;
        case Iff::TCS_Previous:
          source = "p";
          break;
        case Iff::TCS_Texture:
          source = "s";
          break;
        default:
          skip = true;
          break;
      }
      if ( skip ) {
        s << "        float asrc" << i << " = 0.0f;\n";
      } else {
        Iff::TexenvCombineOp op = i == 0 ? env.a.op0 : i == 1 ? env.a.op1 : env.a.op2;
        s << "        float asrc" << i << " = ";
        if( op == Iff::TCO_OneMinusAlpha ) {
          s << "1 - ";
        }
        s << source << ".w;\n";
      }
    }
    switch( env.a.mode ) {
      case Iff::TEC_Replace:
        s << "        p.w = asrc0;\n";
        break;
      case Iff::TEC_Modulate:
        s << "        p.w = asrc0 * asrc1;\n";
        break;
      case Iff::TEC_Add:
        s << "        p.w = asrc0 + asrc1;\n";
        break;
      case Iff::TEC_AddSigned:
        s << "        p.w = asrc0 + asrc1 - 0.5;\n";
        break;
      case Iff::TEC_Subtract:
        s << "        p.w = asrc0 - asrc1;\n";
        break;
      case Iff::TEC_Interpolate:
        s << "        p.w = mix( asrc0, asrc1, asrc2 );\n";
        break;
      default:
        s << "//ERROR: Unsupported tex env combine alpha mode\n"; break;
        break;
    }
  }
  s << "    }\n";
}

#if 0
string TexEnvModeName( GLenum mode ) {
  switch( mode ) {
    case GL_MODULATE:    return "Modulate";
    case GL_ADD:         return "Add";
    case GL_REPLACE:     return "Replace";
    case GL_COMBINE:     return "Combine";
    default: break;
  }
  return "";
}
string TexFormatName( GLenum format ) {
  switch( format ) {
    case GL_ALPHA:           return "A";
    case GL_LUMINANCE:       return "L";
    case GL_LUMINANCE_ALPHA: return "LA";
    case GL_RGB:             return "RGB";
    case GL_RGBA:            return "RGBA";
    default: break;
  }
  return "";
}
string TexEnvFuncName( GLenum mode, GLenum format ) {
  string s;
  s += "TexEnv";
  s += TexEnvModeName( mode );
  s += TexFormatName( format );
  return s;
}
#endif

static string TargetSuffix( GLubyte ttb ) {
  switch( ttb ) {
    case Iff::TT_1D: return "1D";
    case Iff::TT_2D: return "2D";
    case Iff::TT_Rect: return "Rect";
    case Iff::TT_3D: return "3D";
    case Iff::TT_CubeMap: return "Cube";
    default: break;
  }
  return "";
}

static string TextureFetch( bool es, bool legacy, Iff::TextureTargetBitfield b ) {
  if( es || legacy ) {
    switch( b ) {
      case Iff::TT_1D:      return "texture1D";
      case Iff::TT_2D:      return "texture2D";
      case Iff::TT_CubeMap: return "textureCube";
      default: break;
    }
  }
  return "texture";
}
static string TextureFetchSwizzle( bool es, bool legacy, Iff::TextureTargetBitfield b ) {
  if( es || legacy ) {
    switch( b ) {
      case Iff::TT_1D:      return ".x";
      case Iff::TT_2D:      return ".xy";
      case Iff::TT_Rect:    return ".xy";
      case Iff::TT_3D:      return ".xyz";
      case Iff::TT_CubeMap: return ".xyz";
      default: break;
    }
    return "";
  }
  switch( b ) {
    case Iff::TT_1D:      return ".x";
    case Iff::TT_2D:      return ".xy";
    case Iff::TT_Rect:    return ".xy";
    case Iff::TT_3D:      return ".xyz";
    case Iff::TT_CubeMap: return ".xyz";
    default: break;
  }
  return "";
}

static void GenerateFragmentShaderSource( Iff * rff, string_list &src )
{
  Internal("Regal::Iff::GenerateFragmentShaderSource",rff);

  const Store & st = rff->ffstate.processed;
  if( rff->gles ) {
#if REGAL_FORCE_DESKTOP_GLSL
    src << "#version 140\n";
#else
    src << "#version 100\n";
#endif
  } else if( rff->legacy ) {
    src << "#version 120\n";
  } else {
    src << "#version 140\n";
  }
  src << "// program number " << rff->progcount << "\n";
  if( rff->gles || rff->legacy ) {
    src << "#define in varying\n";
    src << "#define rglFragColor gl_FragColor\n";
  } else {
    src << "out vec4 rglFragColor;\n";
  }
  if( st.shadeModelFlat && ! rff->legacy && ! rff->gles ) {
    src << "#define FLAT flat\n";
  } else {
    src << "#define FLAT  \n";
  }
  if( rff->gles ) {
    src << "precision highp float;\n";
  }
  src << "FLAT in vec4 rglFrontColor;\n";
  if( st.lighting ) {
    if( st.lightModelTwoSide ) {
      src << "FLAT in vec4 rglBackColor;\n";
    }
    if( st.lightModelSeparateSpecular ) {
      src << "in vec4 rglSCOL0;\n";
      if( st.lightModelTwoSide ) {
        src << "in vec4 rglSCOL1;\n";
      }
    }
  } else if( st.colorSum ) {
    src << "in vec4 rglSCOL0;\n";
  }
  if( st.fog.enable ) {
    src << "uniform vec4 rglFog[2];\n";
    src << "in vec4 rglFOG;\n";
  }
  bool needsConstantColor = false;
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    Texture t = rff->ffstate.processed.tex[i];
    if( t.enables == 0 ) {
      continue;
    }
    src << "uniform sampler" << TargetSuffix( t.enables ) << " rglSampler" << i << ";\n";
    src << "in vec4 rglTEXCOORD" << i << ";\n";
    Iff::TextureEnv & env = t.unit.env;
    if( env.mode == Iff::TEM_Combine ) {
      needsConstantColor =
      env.rgb.src0 == Iff::TCS_Constant ||
      env.rgb.src1 == Iff::TCS_Constant ||
      env.rgb.src2 == Iff::TCS_Constant ;
    }
    if( env.mode == Iff::TEM_Blend ) {
      src << "uniform vec4 rglTexEnvColor" << i << ";\n";
    }
  }
  if( needsConstantColor ) {
    src << "uniform vec4 rglConstantColor;\n";
  }
  for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
    if( ( rff->gles || rff->legacy ) && st.clip[i].enable ) {
      src << "in float rglClipDistance" << i << ";\n";
    }
  }
  if( st.alphaTest.enable ) {
    if (st.alphaTest.comp != Iff::CF_Never && st.alphaTest.comp != Iff::CF_Always ) {
      src << "uniform float rglAlphaRef;\n";
    }
  }
  src << "void main() {\n";

  if( rff->gles || rff->legacy ) {
    for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
      if( st.clip[i].enable ) {
        src << "    if( rglClipDistance" << i << " < 0.0 ) discard;\n";
      }
    }
  }
  if( st.lighting && st.lightModelTwoSide ) {
    src << "    vec4 p = gl_FrontFacing ? rglFrontColor : rglBackColor;\n";
  } else {
    src << "    vec4 p = rglFrontColor;\n";
  }
  bool s_declared = false;
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    Texture t = rff->ffstate.processed.tex[ i ];
    Iff::TextureTargetBitfield b = rff->ffstate.GetTextureEnable( i );
    if( b == 0 ) {
      continue;
    }
    if ( !s_declared ) {
      src << "    vec4 s;\n";
      s_declared = true;
    }
    switch( b ) {
      case Iff::TT_1D:
      case Iff::TT_2D:
      {
        src << "    s = " << TextureFetch( rff->gles, rff->legacy, b )
            << "( rglSampler" << i << ", rglTEXCOORD" << i
            << TextureFetchSwizzle( rff->gles, rff->legacy, b ) << " / rglTEXCOORD" << i << ".w );\n";
        break;
      }
      case Iff::TT_CubeMap:
      {
        src << "    s = " << TextureFetch( rff->gles, rff->legacy, b )
            << "( rglSampler" << i << ", rglTEXCOORD" << i
            << TextureFetchSwizzle( rff->gles, rff->legacy, b ) << " );\n";
        break;
      }
      default:
        src << "// ERROR: Unsupported texture target\n";
        src << "    s = vec4( 0.0f, 0.0f, 0.0f, 0.0f );\n";
        break;
    }
    switch( t.unit.env.mode ) {
      case Iff::TEM_Replace:
      case Iff::TEM_Modulate:
      case Iff::TEM_Add:
      case Iff::TEM_Decal:
      case Iff::TEM_Blend:
        AddTexEnv( i, t.unit.env.mode, t.unit.fmt, src );
        break;
      case Iff::TEM_Combine:
        AddTexEnvCombine( t.unit.env, src );
        break;
      default:
        src << "//ERROR: Unsupported texture env mode\n";
        break;
    }
  }
  src << "    rglFragColor = p;\n";
  if( st.lighting ) {
    if( st.lightModelSeparateSpecular ) {
      if( st.lightModelTwoSide ) {
        src << "    rglFragColor += gl_FrontFacing ? rglSCOL0 : rglSCOL1;\n";
      } else {
        src << "    rglFragColor += rglSCOL0;\n";
      }
    }
  } else if( st.colorSum ) {
    src << "    rglFragColor.xyz += rglSCOL0.xyz;\n";
  }
  if( st.fog.enable ) {
    src << "    float f = abs( -rglFOG.z / rglFOG.w );\n";
    switch( st.fog.mode ) {
      case Iff::FG_Linear:
        src << "    float fogFactor = ( rglFog[0].z - f ) / ( rglFog[0].z - rglFog[0].y );\n";
        break;
      case Iff::FG_Exp:
        src << "    float fogFactor = exp( -( rglFog[0].x * f ) );\n";
        break;
      case Iff::FG_Exp2:
        src << "    float fogFactor = exp( -( rglFog[0].x * rglFog[0].x * f * f ) );\n";
        break;
      default:
        src << "//ERROR: Unsupported fog mode\n";
        src << "    float fogFactor = 0.0f;\n";
        break;
    }
    src << "    fogFactor = clamp( fogFactor, 0.0, 1.0 );\n";
    src << "    rglFragColor.xyz = mix( rglFog[1].xyz, rglFragColor.xyz, fogFactor );\n";
  }
  if( st.alphaTest.enable ) {
    switch( st.alphaTest.comp ) {
      case Iff::CF_Never:    src << "    discard;\n"; break;
      case Iff::CF_Less:     src << "    if( rglFragColor.w >= rglAlphaRef ) discard;\n"; break;
      case Iff::CF_Greater:  src << "    if( rglFragColor.w <= rglAlphaRef ) discard;\n"; break;
      case Iff::CF_Lequal:   src << "    if( rglFragColor.w > rglAlphaRef ) discard;\n"; break;
      case Iff::CF_Gequal:   src << "    if( rglFragColor.w < rglAlphaRef ) discard;\n"; break;
      case Iff::CF_Equal:    src << "    if( rglFragColor.w != rglAlphaRef ) discard;\n"; break;
      case Iff::CF_NotEqual: src << "    if( rglFragColor.w == rglAlphaRef ) discard;\n"; break;
      case Iff::CF_Always: break;
      default: src << "//ERROR: Unsupported alpha comp func\n"; break;
    }
  }

  src << "}\n";
}

static void Copy( Float4 & dst, const GLfloat * src ) {
  dst.x = src[0]; dst.y = src[1]; dst.z = src[2]; dst.w = src[3];
}

static void Copy( GLfloat * dst, Float4 & src ) {
  dst[0] = src.x; dst[1] = src.y; dst[2] = src.z; dst[3] = src.w;
}

static void Transform( Float4 & dst, const r3::Matrix4f & m, const GLfloat * src ) {
  r3::Vec4f v( src );
  m.MultMatrixVec( v );
  dst.x = v.x; dst.y = v.y; dst.z = v.z; dst.w = v.w;
}

// Not currently used
#if 0
void TransformDir( Float4 & dst, const r3::Matrix4f & m, const GLfloat * src ) {
  r3::Vec3f v( src );
  m.MultMatrixDir( v );
  v.Normalize();
  dst.x = v.x; dst.y = v.y; dst.z = v.z;
}
#endif

static r3::Matrix4f RescaleNormal( const r3::Matrix4f & m ) {
  r3::Matrix4f r = m;
  for( int i = 0; i < 3; i++ ) {
    r3::Vec3f v( r( i, 0 ), r( i, 1 ), r( i, 2 ) );
    v.Normalize();
    r( i, 0 ) = v.x; r( i, 1 ) = v.y; r( i, 2 ) = v.z;
  }
  return r;
}

bool State::SetEnable( Iff * ffn, bool enable, GLenum cap )
{
  Internal("Regal::State::SetEnable",enable," ",Token::GLenumToString(cap));

  Iff::Version & ver = ffn->ver;
  int activeTex = ffn->activeTextureIndex;
  int shift;
  switch( cap ) {
    case GL_TEXTURE_1D: shift = TP_1D; break;
    case GL_TEXTURE_2D: shift = TP_2D; break;
    case GL_TEXTURE_RECTANGLE: shift = TP_Rect; break;
    case GL_TEXTURE_3D: shift = TP_3D; break;
    case GL_TEXTURE_CUBE_MAP: shift = TP_CubeMap; break;
    case GL_COLOR_SUM: raw.ver = ver.Update(); raw.colorSum = enable; return true;
    case GL_FOG: raw.ver = ver.Update(); raw.fog.enable = enable; return true;
    case GL_LIGHTING: raw.ver = ver.Update(); raw.lighting = enable; return true;
    case GL_LIGHT0:
    case GL_LIGHT1:
    case GL_LIGHT2:
    case GL_LIGHT3:
    case GL_LIGHT4:
    case GL_LIGHT5:
    case GL_LIGHT6:
    case GL_LIGHT7: raw.ver = ver.Update(); raw.light[ cap - GL_LIGHT0 ].enable = enable; return true;
    case GL_COLOR_MATERIAL: raw.ver = ver.Update(); raw.colorMaterial = enable; return true;
    case GL_RESCALE_NORMAL: raw.ver = ver.Update(); raw.rescaleNormal = enable; return true;
    case GL_NORMALIZE: raw.ver = ver.Update(); raw.normalize = enable; return true;
    case GL_TEXTURE_GEN_S:
    case GL_TEXTURE_GEN_T:
    case GL_TEXTURE_GEN_R:
    case GL_TEXTURE_GEN_Q: {
      raw.ver = ver.Update();
      int idx = cap - GL_TEXTURE_GEN_S;
      raw.tex[ ffn->activeTextureIndex ].texgen[ idx ].enable = enable;
      return true;
    }
    case GL_CLIP_PLANE0:
    case GL_CLIP_PLANE1:
    case GL_CLIP_PLANE2:
    case GL_CLIP_PLANE3:
    case GL_CLIP_PLANE4:
    case GL_CLIP_PLANE5:
    case GL_CLIP_PLANE0+6:
    case GL_CLIP_PLANE0+7:
      raw.ver = ver.Update();
      raw.clip[ cap - GL_CLIP_PLANE0 ].enable = enable;
      return false;
    case GL_ALPHA_TEST: raw.ver = ver.Update(); raw.alphaTest.enable = enable; return true;
    default:
      return false;
  }
  if( activeTex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
    Warning( "Active texture index is too large: ", activeTex, " >= ", REGAL_EMU_IFF_TEXTURE_UNITS );
    return true;
  }
  Texture & t = raw.tex[ activeTex ];
  GLuint v = 1 << shift;
  if( enable ) {
    t.enables |= v;
  } else {
    t.enables &= ~v;
  }
  Internal("Regal::State::SetEnable","activeTex=",activeTex," enables=",t.enables);
  raw.ver = ver.Update();
  return true;
}

void State::SetLight( Iff * ffn, GLenum light, GLenum pname, const GLfloat * params )
{
  Internal("Regal::State::SetLight",light,Token::GLenumToString(pname));

  Iff::Version & ver = ffn->ver;
  int idx = light - GL_LIGHT0;
  if( idx < 0 || idx >= REGAL_FIXED_FUNCTION_MAX_LIGHTS ) {
    return;
  }
  State::LightUniform & lu = uniform.light[ idx ];
  switch( pname ) {
    case GL_AMBIENT: Copy( lu.ambient, params ); break;
    case GL_DIFFUSE: Copy( lu.diffuse, params ); break;
    case GL_SPECULAR: Copy( lu.specular, params ); break;
    case GL_POSITION:
    {
      Transform( lu.position, ffn->modelview.Top(), params );
      if (lu.position.w == 0.0) {
        lu.position.w = 1.0f/sqrtf(lu.position.x * lu.position.x +
                                   lu.position.y * lu.position.y +
                                   lu.position.z * lu.position.z);
        lu.position.x *= lu.position.w;
        lu.position.y *= lu.position.w;
        lu.position.z *= lu.position.w;
        lu.position.w = 0.0;
        raw.light[ idx ].local = false;
      } else {
        lu.position.x /= lu.position.w;
        lu.position.y /= lu.position.w;
        lu.position.z /= lu.position.w;
        lu.position.w = 1.0;
        raw.light[ idx ].local = true;
      }
      break;
    }
    case GL_SPOT_DIRECTION:
    {
      Float4 spdir( params[0], params[1], params[2], 1.0f );
      r3::Matrix4f & m = ffn->modelview.Top();
      float x = m.element(0,0) * spdir.x + m.element(0,1) * spdir.y + m.element(0,2) * spdir.z;
      float y = m.element(1,0) * spdir.x + m.element(1,1) * spdir.y + m.element(1,2) * spdir.z;
      float z = m.element(2,0) * spdir.x + m.element(2,1) * spdir.y + m.element(2,2) * spdir.z;
      float d = -1.0f/sqrtf(x*x + y*y + z*z);
      lu.spotDirection.x = x * d;
      lu.spotDirection.y = y * d;
      lu.spotDirection.z = z * d;
    }
      break;

    case GL_SPOT_EXPONENT: lu.attenuation.w = params[0]; break;
    case GL_SPOT_CUTOFF: lu.spotDirection.w = params[0]; break;
    case GL_CONSTANT_ATTENUATION: lu.attenuation.x = params[0]; break;
    case GL_LINEAR_ATTENUATION: lu.attenuation.y = params[0]; break;
    case GL_QUADRATIC_ATTENUATION: lu.attenuation.z = params[0]; break;
    default: return;
  }

  // some raw state is derived from uniform state
  bool updateRawVersion = false;
  switch( pname ) {
    case GL_POSITION:
    case GL_SPOT_CUTOFF:
    case GL_CONSTANT_ATTENUATION:
    case GL_LINEAR_ATTENUATION:
    case GL_QUADRATIC_ATTENUATION:
      updateRawVersion = true;
      break;
    default:
      break;
  }
  lu.ver = uniform.ver = ver.Update();
  if( updateRawVersion ) {
    raw.ver = uniform.ver;
  }
}

void State::SetMaterial( Iff * ffn, GLenum face, GLenum pname, const GLfloat * params )
{
  Internal("Regal::State::SetMaterial",face,Token::GLenumToString(pname));

  Iff::Version & ver = ffn->ver;
  for( int i = 0; i < 2; i++ ) {
    if( ( i == 0 && face == GL_BACK ) || ( i == 1 && face == GL_FRONT ) ) {
      continue;
    }
    State::MaterialUniform & m = uniform.mat[ i ];
    switch( pname ) {
      case GL_AMBIENT: Copy( m.ambient, params ); break;
      case GL_DIFFUSE: Copy( m.diffuse, params ); break;
      case GL_AMBIENT_AND_DIFFUSE: Copy( m.ambient, params ); m.diffuse = m.ambient; break;
      case GL_SPECULAR: Copy( m.specular, params ); break;
      case GL_EMISSION: Copy( m.emission, params ); break;
      case GL_SHININESS: m.shininess.x = params[0]; break;
      default: return;
    }
    m.ver = uniform.ver = ver.Update();
  }
}

void State::GetMaterial( Iff * ffn, GLenum face, GLenum pname, GLfloat * params )
{
  Internal("Regal::State::GetMaterial",face,Token::GLenumToString(pname));

  UNUSED_PARAMETER(ffn);

  for( int i = 0; i < 2; i++ ) {
    if( ( i == 0 && face == GL_BACK ) || ( i == 1 && face == GL_FRONT ) ) {
      continue;
    }
    State::MaterialUniform & m = uniform.mat[ i ];
    switch( pname ) {
      case GL_AMBIENT: Copy( params, m.ambient ); break;
      case GL_DIFFUSE: Copy( params, m.diffuse ); break;
      case GL_SPECULAR: Copy( params, m.specular ); break;
      case GL_EMISSION: Copy( params, m.emission ); break;
      case GL_SHININESS: params[0] = m.shininess.x; break;
      default: return;
    }
  }
}

void State::SetTexgen( Iff * ffn, int coord, GLenum space, const GLfloat * params )
{
  Internal("State::SetTexgen ",ffn,coord,toString(space),boost::print::array(params,4));

  r3::Matrix4f ident;
  TexgenUniform & tgu = uniform.tex[ ffn->activeTextureIndex ].texgen[ coord ];
  GLuint64 *tguver = NULL;
  switch( space ) {
    case GL_OBJECT_PLANE: Transform( tgu.obj, ident, params ); tguver = & tgu.objVer; break;
    case GL_EYE_PLANE: Transform( tgu.eye, ffn->modelview.Top().Inverse().Transpose(), params ); tguver = & tgu.eyeVer; break;
    default: return;
  }
  *tguver = uniform.ver = ffn->ver.Update();
}

void State::GetTexgen( Iff * ffn, int coord, GLenum space, GLfloat * params )
{
  Internal("State::GetTexgen ",ffn," ",coord," ",toString(space));

  TexgenUniform & tgu = uniform.tex[ ffn->activeTextureIndex ].texgen[ coord ];
  switch( space ) {
    case GL_OBJECT_PLANE:
      params[0] = tgu.obj.x;
      params[1] = tgu.obj.y;
      params[2] = tgu.obj.z;
      params[3] = tgu.obj.w;
      break;
    case GL_EYE_PLANE:
      params[0] = tgu.eye.x;
      params[1] = tgu.eye.y;
      params[2] = tgu.eye.z;
      params[3] = tgu.eye.w;
      break;
    default:
      break;
  }
}

void State::SetAlphaFunc( Iff * ffn, Iff::CompareFunc comp, GLfloat alphaRef )
{
  raw.alphaTest.comp = comp;
  uniform.alphaTest.alphaRef = std::min( 1.0f, std::max( 0.0f, alphaRef ) );
  uniform.alphaTest.ver = uniform.ver = raw.ver = ffn->ver.Update();
}

void State::SetClip( Iff * ffn, GLenum plane, const GLfloat * equation )
{
  int idx = plane - GL_CLIP_PLANE0;
  if( idx >= REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ) {
    return;
  }
  Transform( uniform.clip[ idx ].plane, ffn->modelview.Top().Inverse().Transpose(), equation );
  uniform.clip[ idx ].ver = uniform.ver = ffn->ver.Update();
}

void Program::Init( RegalContext * ctx, const Store & sstore, const GLchar *vsSrc, const GLchar *fsSrc )
{
  Internal("Regal::Program::Init","()");

  ver = ::std::numeric_limits<GLuint64>::max();
  progcount = 0;
  RegalAssert(ctx);
  DispatchTable & tbl = ctx->dispatcher.emulation;
  store = sstore;
  pg = tbl.call(&tbl.glCreateProgram)();
  Shader( ctx, tbl, GL_VERTEX_SHADER, vs, vsSrc );
  Shader( ctx, tbl, GL_FRAGMENT_SHADER, fs, fsSrc );
  Attribs( ctx );
  tbl.call(&tbl.glLinkProgram)( pg );

  #ifndef NDEBUG
  GLint status = 0;
  tbl.call(&tbl.glGetProgramiv)( pg, GL_LINK_STATUS, &status );
  if (!status)
  {
    std::string log;
    if (helper::getInfoLog(log,tbl.call(&tbl.glGetProgramInfoLog),tbl.call(&tbl.glGetProgramiv),pg))
      Warning( "Regal::Program::Init", log);
  }
  #endif

  tbl.call(&tbl.glUseProgram)( pg );
  Samplers( ctx, tbl );
  Uniforms( ctx, tbl );
  tbl.call(&tbl.glUseProgram)( ctx->iff->program );
}

void Program::Shader( RegalContext * ctx, DispatchTable & tbl, GLenum type, GLuint & shader, const GLchar *src )
{
  Internal("Regal::Program::Shader","()");

  UNUSED_PARAMETER(ctx);

  const GLchar *srcs[] = { src };
  GLint len[] = { 0 };
  len[0] = (GLint)strlen( src );
  shader = tbl.call(&tbl.glCreateShader)(type);
  tbl.call(&tbl.glShaderSource)( shader, 1, srcs, len );
  tbl.call(&tbl.glCompileShader)( shader );

  #ifndef NDEBUG
  GLint status = 0;
  tbl.call(&tbl.glGetShaderiv)( shader, GL_COMPILE_STATUS, &status );
  if (!status)
  {
    std::string log;
    if (helper::getInfoLog(log,tbl.call(&tbl.glGetShaderInfoLog),tbl.call(&tbl.glGetShaderiv),shader))
      Warning("Regal::Program::Shader", log);
  }
  #endif

  tbl.call(&tbl.glAttachShader)( pg, shader );
}

void Program::Attribs( RegalContext * ctx )
{
  Internal("Regal::Program::Attribs","()");

  DispatchTable & tbl = ctx->dispatcher.emulation;

  tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Vertex ], "rglVertex" );
  //tbl.call(&tbl.glBindAttribLocation)( pg, 1, "rglWeight" );
  if( store.lighting ) {
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Normal ], "rglNormal" );
  }
  if( store.attrArrayFlags & ( 1 << ctx->iff->ffAttrMap[ RFF2A_Color ] ) && ( store.lighting == false || store.colorMaterial ) ) {
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Color ], "rglColor" );
  }
  if( store.colorSum  && store.lighting == false ) {
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_SecondaryColor ], "rglSecondaryColor" );
  }
  if( store.fog.enable && store.fog.useDepth == false ) {
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_FogCoord ], "rglFogCoord" );
  }
  GLuint units = std::min( (GLuint)ctx->iff->ffAttrNumTex, (GLuint)REGAL_EMU_IFF_TEXTURE_UNITS );
  for( GLuint i = 0; i < units; i++ ) {
#ifndef REGAL_HACK_SET_001
    if( store.tex[i].enables == 0 ) {
      continue;
    }
#endif
    string_list ss;
    ss << "rglMultiTexCoord" << i;
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrTexBegin + i, ss.str().c_str() );
  }
}

// seth: for user program mode just do all the bind attribs, aliasing is OK in GL
void Program::UserShaderModeAttribs( RegalContext * ctx )
{
  Internal("Regal::Program::UserShaderModeAttribs","()");

  DispatchTable & tbl = ctx->dispatcher.emulation;

  tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Vertex ], "rglVertex" );
  tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Normal ], "rglNormal" );
  tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_Color ], "rglColor" );
  if ( ctx->iff->ffAttrMap[ RFF2A_SecondaryColor ] != RFF2A_Invalid ) {
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_SecondaryColor ], "rglSecondaryColor" );
  }
  tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrMap[ RFF2A_FogCoord ], "rglFogCoord" );
  GLuint units = std::min( /*(GLuint)ctx->iff->ffAttrNumTex*/(GLuint)16, (GLuint)REGAL_EMU_IFF_TEXTURE_UNITS );
  for( GLuint i = 0; i < units; i++ ) {
    if( store.tex[i].enables == 0 ) {
      continue;
    }
    string_list ss;
    ss << "rglMultiTexCoord" << i;
    tbl.call(&tbl.glBindAttribLocation)( pg, ctx->iff->ffAttrTexBegin + i, ss.str().c_str() );
  }
}


void Program::Samplers( RegalContext * ctx, DispatchTable & tbl )
{
  Internal("Regal::Program::Samplers","()");

  UNUSED_PARAMETER(ctx);

  GLchar samp[64];
  strcpy( samp, "rglSamplerN" );
  int len = (int)strlen( samp );
  for( GLchar i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    samp[ len - 1 ] = '0' + i;
    GLint slot = tbl.call(&tbl.glGetUniformLocation)( pg, samp );
    if( slot >= 0 ) {
      tbl.call(&tbl.glUniform1i)( slot, i );
    }
  }
}

void Program::Uniforms( RegalContext * ctx, DispatchTable & tbl )
{
  Internal("Regal::Program::Uniforms","()");

  UNUSED_PARAMETER(ctx);

  for( size_t i = 1; i < sizeof(regalFFUniformInfo)/sizeof(regalFFUniformInfo[0]); i++ ) {
    const RegalFFUniformInfo & ri = regalFFUniformInfo[i];
    GLint slot = tbl.call(&tbl.glGetUniformLocation)( pg, ri.name );
    if (slot > -1)
      uniforms[ ri.val ] = UniformInfo(slot,~GLuint64(0));
  }
}

void Iff::Cleanup( RegalContext &ctx )
{
  Internal("Regal::Iff::Cleanup","()");

  RestoreVao(&ctx);
  DispatchTable &tbl = ctx.dispatcher.emulation;

  tbl.call(&tbl.glDeleteBuffers)(1, &immVbo);
  tbl.call(&tbl.glDeleteBuffers)(1, &immQuadsVbo);
  tbl.call(&tbl.glDeleteVertexArrays)(1, &immVao);

  for (int i = 0; i < (1 << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS); ++i)
  {
    const Program &pgm = ffprogs[i];
    if (pgm.pg)
    {
      tbl.call(&tbl.glDeleteShader)(pgm.vs);
      tbl.call(&tbl.glDeleteShader)(pgm.fs);
      tbl.call(&tbl.glDeleteProgram)(pgm.pg);
    }
  }

  bool isPepperGLES = (ctx.info->vendor == "Chromium");

  tbl.glBindBuffer(GL_ARRAY_BUFFER, 0);
  tbl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  for (GLuint i=0; i<ctx.info->maxVertexAttribs; ++i)
  {
    // Chromium/PepperAPI GLES generates an error (visible through glGetError)
    // and logs a message if a call is made to glVertexAttribPointer and no
    // GL_ARRAY_BUFFER is bound.
    if (!isPepperGLES) {
      tbl.glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    tbl.glDisableVertexAttribArray(i);
  }
}

void Iff::InitFixedFunction(RegalContext &ctx)
{
  Internal("Regal::Iff::InitFixedFunction","()");

  RegalAssert(ctx.info);

  gles   = ctx.info->es2;
  legacy = ctx.info->compat && ctx.info->gl_version_major<=2;

  shadowMatrixMode = GL_MODELVIEW;
  currMatrixStack = &modelview;
  program = 0;
  activeTextureIndex = 0;
  shadowActiveTextureIndex = 0;

  currprog = NULL;
  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    textureUnit[ i ] = TextureUnit();
    textureEnvColor[i] = Float4( 0.0f, 0.0f, 0.0f, 1.0f );
    textureEnvColorVer[i] = 0;
  }

  for( int i = 0; i < REGAL_EMU_MAX_TEXTURE_UNITS; i++ ) {
    textureBinding[ i ] = 0;
  }

  currVao = 0;
  vaoAttrMap[0] = 0;

  fmtmap[ 1 ] = GL_LUMINANCE;
  fmtmap[ 2 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ 3 ] = GL_RGB;
  fmtmap[ 4 ] = GL_RGBA;
  fmtmap[ GL_ALPHA ] = GL_ALPHA;
  fmtmap[ GL_ALPHA4 ] = GL_ALPHA;
  fmtmap[ GL_ALPHA8 ] = GL_ALPHA;
  fmtmap[ GL_ALPHA12 ] = GL_ALPHA;
  fmtmap[ GL_ALPHA16 ] = GL_ALPHA;
  fmtmap[ GL_LUMINANCE ] = GL_LUMINANCE;
  fmtmap[ GL_LUMINANCE4 ] = GL_LUMINANCE;
  fmtmap[ GL_LUMINANCE8 ] = GL_LUMINANCE;
  fmtmap[ GL_LUMINANCE12 ] = GL_LUMINANCE;
  fmtmap[ GL_LUMINANCE16 ] = GL_LUMINANCE;
  fmtmap[ GL_LUMINANCE_ALPHA ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE4_ALPHA4 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE6_ALPHA2 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE8_ALPHA8 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE12_ALPHA4 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE12_ALPHA12 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_LUMINANCE16_ALPHA16 ] = GL_LUMINANCE_ALPHA;
  fmtmap[ GL_INTENSITY ] = GL_INTENSITY;
  fmtmap[ GL_INTENSITY4 ] = GL_INTENSITY;
  fmtmap[ GL_INTENSITY8 ] = GL_INTENSITY;
  fmtmap[ GL_INTENSITY12 ] = GL_INTENSITY;
  fmtmap[ GL_INTENSITY12 ] = GL_INTENSITY;
  fmtmap[ GL_INTENSITY16 ] = GL_INTENSITY;
  fmtmap[ GL_RGB ] = GL_RGB;
  fmtmap[ GL_RGB4 ] = GL_RGB;
  fmtmap[ GL_RGB5 ] = GL_RGB;
  fmtmap[ GL_RGB8 ] = GL_RGB;
  fmtmap[ GL_RGB10 ] = GL_RGB;
  fmtmap[ GL_RGB12 ] = GL_RGB;
  fmtmap[ GL_RGB16 ] = GL_RGB;
  fmtmap[ GL_RGBA ] = GL_RGBA;
  fmtmap[ GL_RGBA4 ] = GL_RGBA;
  fmtmap[ GL_RGB5_A1 ] = GL_RGBA;
  fmtmap[ GL_RGBA8 ] = GL_RGBA;
  fmtmap[ GL_RGB10_A2 ] = GL_RGBA;
  fmtmap[ GL_RGBA12 ] = GL_RGBA;
  fmtmap[ GL_RGBA16 ] = GL_RGBA;

  fmtmap[ GL_RGBA16F ] = GL_RGBA16F;
  fmtmap[ GL_SRGB ] = GL_SRGB;
  fmtmap[ GL_SRGB8 ] = GL_SRGB;
  fmtmap[ GL_SRGB8_ALPHA8 ] = GL_SRGB8_ALPHA8;
  fmtmap[ GL_SLUMINANCE ] = GL_SLUMINANCE;
  fmtmap[ GL_SLUMINANCE8 ] = GL_SLUMINANCE;

  fmtmap[ GL_RGB16F_ARB ]       = GL_RGB;
  fmtmap[ GL_RGBA32F_ARB ]      = GL_RGB;

  fmtmap[ GL_DEPTH_COMPONENT16 ] = GL_DEPTH_COMPONENT16;

  fmtmap[ GL_INTENSITY16F_ARB ] = GL_INTENSITY;

  // GL_ARB_ES2_compatibility

  fmtmap[ GL_RGB565 ]           = GL_RGB;
}

void Iff::ShadowMultiTexBinding( GLenum texunit, GLenum target, GLuint obj )
{
  Internal("Regal::Iff::ShadowMultiTexBinding",toString(texunit)," ",toString(target)," ",obj);

  activeTextureIndex = texunit - GL_TEXTURE0;
  if( activeTextureIndex > ( REGAL_EMU_MAX_TEXTURE_UNITS - 1 ) ) {
    return;
  }
  // assert( textureInfo.count( obj ) == 0 || target == textureInfo[activeTextureIndex].target );
  GLint fmt = textureObjToFmt[ obj ];
  TextureUnit & tu = textureUnit[ activeTextureIndex ];
  textureBinding[ activeTextureIndex ] = obj;
  tu.fmt = fmt;
  tu.ttb = static_cast<GLubyte>(TargetToBitfield( target ));
  ffstate.raw.ver = ver.Update();
}

void Iff::ShadowTextureInfo( GLuint obj, GLenum target, GLint internalFormat )
{
  Internal("Regal::Iff::ShadowTextureInfo",obj," ",GLenumToString(target)," ",GLenumToString(internalFormat));

  UNUSED_PARAMETER(target);
  // assert( target == tip->tgt );
  if( fmtmap.count( internalFormat ) == 0 ) {
    Warning( "Unknown internal format: ", GLenumToString(internalFormat) );
  }
  GLint fmt = fmtmap[ internalFormat ];
  textureObjToFmt[ obj ] = fmt;
  ffstate.raw.ver = ver.Update();
}

void Iff::ShadowMultiTexInfo( GLenum texunit, GLenum target, GLint internalFormat )
{
  activeTextureIndex = texunit - GL_TEXTURE0;
  ShadowTexInfo( target, internalFormat );
}

void Iff::ShadowTexInfo( GLenum target, GLint internalFormat )
{
  if( shadowActiveTextureIndex > ( REGAL_EMU_MAX_TEXTURE_UNITS - 1 ) ) {
    return;
  }
  ShadowTextureInfo( textureBinding[ shadowActiveTextureIndex ], target, internalFormat );
  textureUnit[ shadowActiveTextureIndex ].fmt = fmtmap[ internalFormat ];
}

void Iff::TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLfloat *v )
{
  activeTextureIndex = texunit - GL_TEXTURE0;
  switch( target ) {
    case GL_TEXTURE_ENV:
      switch( pname ) {
        case GL_TEXTURE_ENV_COLOR: {
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          Copy( textureEnvColor[ activeTextureIndex ], v );
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          textureEnvColorVer[ activeTextureIndex ] = ffstate.uniform.ver = ver.Update();
          return;
        }
      }
  }
  GLint iv[4];
  iv[0] = static_cast<GLint>(v[0]);
  iv[1] = static_cast<GLint>(v[1]);
  iv[2] = static_cast<GLint>(v[2]);
  iv[3] = static_cast<GLint>(v[3]);
  TexEnv( texunit, target, pname, iv );
}

void Iff::TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLint *v )
{
  Internal("Regal::Iff::TexEnv",GLenumToString(texunit)," ",GLenumToString(target)," ",GLenumToString(pname));

  activeTextureIndex = texunit - GL_TEXTURE0;
  switch( target ) {
    case GL_TEXTURE_ENV:
      switch( pname ) {
        case GL_TEXTURE_ENV_MODE: {
          if( activeTextureIndex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
            return;
          }
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          TexenvMode & m = tup->env.mode;
          switch( v[0] ) {
            case GL_REPLACE:  m = TEM_Replace; break;
            case GL_MODULATE: m = TEM_Modulate; break;
            case GL_ADD:      m = TEM_Add; break;
            case GL_DECAL:    m = TEM_Decal; break;
            case GL_BLEND:    m = TEM_Blend; break;
            case GL_COMBINE:  m = TEM_Combine; break;
            default: return; // error?
          }
          // assert( target == tip->tgt );
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          break;
        }
        case GL_COMBINE_RGB: {
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          TexenvCombineState & s = tup->env.rgb;
          switch( v[0] ) {
            case GL_REPLACE:     s.mode = TEC_Replace; break;
            case GL_MODULATE:    s.mode = TEC_Modulate; break;
            case GL_ADD:         s.mode = TEC_Add; break;
            case GL_ADD_SIGNED:  s.mode = TEC_AddSigned; break;
            case GL_SUBTRACT:    s.mode = TEC_Subtract; break;
            case GL_DOT3_RGB:    s.mode = TEC_Dot3Rgb; break;
            case GL_DOT3_RGBA:   s.mode = TEC_Dot3Rgba; break;
            case GL_INTERPOLATE: s.mode = TEC_Interpolate; break;
            default: return; // error?
          }
          // assert( target == tip->tgt );
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          break;
        }
        case GL_COMBINE_ALPHA: {
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          TexenvCombineState & s = tup->env.a;
          switch( v[0] ) {
            case GL_REPLACE:     s.mode = TEC_Replace; break;
            case GL_MODULATE:    s.mode = TEC_Modulate; break;
            case GL_ADD:         s.mode = TEC_Add; break;
            case GL_ADD_SIGNED:  s.mode = TEC_AddSigned; break;
            case GL_SUBTRACT:    s.mode = TEC_Subtract; break;
            case GL_INTERPOLATE: s.mode = TEC_Interpolate; break;
            default: return; // error?
          }
          // assert( target == tip->tgt );
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          break;
        }
        case GL_SRC0_RGB:
        case GL_SRC1_RGB:
        case GL_SRC2_RGB:
        case GL_SRC0_ALPHA:
        case GL_SRC1_ALPHA:
        case GL_SRC2_ALPHA: {
          int idx = pname - GL_SRC0_RGB;
          bool isRgb = true;
          if( idx > 3 ) {
            isRgb = false;
            idx = pname - GL_SRC0_ALPHA;
          }
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          TexenvCombineState & c = isRgb ? tup->env.rgb : tup->env.a;
          if( idx  == 0 ) {
            switch( v[0] ) {
              case GL_PREVIOUS:      c.src0 = TCS_Previous; break;
              case GL_CONSTANT:      c.src0 = TCS_Constant; break;
              case GL_TEXTURE:       c.src0 = TCS_Texture; break;
              case GL_PRIMARY_COLOR: c.src0 = TCS_PrimaryColor; break;
              default: return; // error?
            }
          } else if( idx  == 1 ) {
            switch( v[0] ) {
              case GL_PREVIOUS:      c.src1 = TCS_Previous; break;
              case GL_CONSTANT:      c.src1 = TCS_Constant; break;
              case GL_TEXTURE:       c.src1 = TCS_Texture; break;
              case GL_PRIMARY_COLOR: c.src1 = TCS_PrimaryColor; break;
              default: return; // error?
            }
          } else if( idx  == 2 ) {
            switch( v[0] ) {
              case GL_PREVIOUS:      c.src2 = TCS_Previous; break;
              case GL_CONSTANT:      c.src2 = TCS_Constant; break;
              case GL_TEXTURE:       c.src2 = TCS_Texture; break;
              case GL_PRIMARY_COLOR: c.src2 = TCS_PrimaryColor; break;
              default: return; // error?
            }
          }
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          break;
        }
        case GL_OPERAND0_RGB:
        case GL_OPERAND1_RGB:
        case GL_OPERAND2_RGB:
        case GL_OPERAND0_ALPHA:
        case GL_OPERAND1_ALPHA:
        case GL_OPERAND2_ALPHA: {
          int idx = pname - GL_OPERAND0_RGB;
          bool isRgb = true;
          if( idx > 3 ) {
            isRgb = false;
            idx = pname - GL_OPERAND0_ALPHA;
          }
          TextureUnit *tup = & textureUnit[ activeTextureIndex ];
          TexenvCombineState & c = isRgb ? tup->env.rgb : tup->env.a;
          if( idx == 0 ) {
            switch( v[0] ) {
              case GL_SRC_COLOR:           c.op0 = TCO_Color; break;
              case GL_SRC_ALPHA:           c.op0 = TCO_Alpha; break;
              case GL_ONE_MINUS_SRC_COLOR: c.op0 = TCO_OneMinusColor; break;
              case GL_ONE_MINUS_SRC_ALPHA: c.op0 = TCO_OneMinusAlpha; break;
              default: return; // error?
            }
          } else if( idx == 1 ) {
            switch( v[0] ) {
              case GL_SRC_COLOR:           c.op1 = TCO_Color; break;
              case GL_SRC_ALPHA:           c.op1 = TCO_Alpha; break;
              case GL_ONE_MINUS_SRC_COLOR: c.op1 = TCO_OneMinusColor; break;
              case GL_ONE_MINUS_SRC_ALPHA: c.op1 = TCO_OneMinusAlpha; break;
              default: return; // error?
            }
          } else if( idx == 2 ) {
            switch( v[0] ) {
              case GL_SRC_COLOR:           c.op2 = TCO_Color; break;
              case GL_SRC_ALPHA:           c.op2 = TCO_Alpha; break;
              case GL_ONE_MINUS_SRC_COLOR: c.op2 = TCO_OneMinusColor; break;
              case GL_ONE_MINUS_SRC_ALPHA: c.op2 = TCO_OneMinusAlpha; break;
              default: return; // error?
            }
          }
          ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
          break;
        }
        default:
          break;
      }
      break;
    default:
      break;
  }
}

inline size_t compute_hash(const Store &val)
{
  return Lookup3::hashlittle(reinterpret_cast<const char *>(&val) + sizeof(GLuint), sizeof(Store)-sizeof(GLuint64)-sizeof(GLuint), 0);
}

void Iff::State::Process( Iff * ffn )
{
  Internal("Regal::State::Process","()");

  const Store & r = raw;
  Store & p = processed;
  StoreUniform & u = uniform;

  if( r.ver > 0 && r.ver == p.ver ) {
    return;
  }

  p = r;

  r3::Matrix4f identity;
  if( p.fog.enable == false ) {
    p.fog.useDepth = true;
    p.fog.mode = FG_Exp;
  }

  // alpha testing is done at the precision of the color buffer so adjust
  // alpharef to an 8-bit range

  u.alphaTest.alphaRef = floor( u.alphaTest.alphaRef * 255.0f + 0.5f ) / 255.0f;

  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    TextureUnit & ti = ffn->textureUnit[ i ];
    raw.tex[i].unit = ti;
  }

  for( int i = 0; i < REGAL_EMU_IFF_TEXTURE_UNITS; i++ ) {
    Texture & pt = p.tex[i];
    const Texture & rt= r.tex[i];
    // for processed state, only the highest priority texture enable set (others are dont-care)
    //<> dsn:
    //<> but do we need to track and check against ttb?  If no texture was
    //<> explicitly bound to a texture target then the default texture (which
    //<> has the name 0) for that target will be used, right?
    //<> pt.enables = HighestPriorityTextureEnable( rt.enables & rt.unit.ttb );
    pt.enables = HighestPriorityTextureEnable( rt.enables );
    pt.unit = rt.unit;
    pt.useMatrix = pt.enables != 0 && ffn->texture[i].Top() != identity;
    if( pt.unit.env.mode != TEM_Combine ) {
      pt.unit.env.rgb = pt.unit.env.a = TexenvCombineState();
    }
  }
  if( p.lighting == false ) {
    p.rescaleNormal = false;
    p.normalize = false;
    for ( int i = 0; i <REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
      Light & l = p.light[i];
      l.enable = l.spotlight = l.attenuate = false;
    }
  } else {
    for ( int i = 0; i <REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
      Light & l = p.light[i];
      LightUniform & lu = u.light[i];
      if( l.enable == true ) {
        l.spotlight = lu.spotDirection.w != 180.0f;
        l.attenuate = lu.attenuation.x != 1.0f || lu.attenuation.y != 0.0f || lu.attenuation.z != 0.0f;
      }
    }
  }
  p.hash = (GLuint) compute_hash(p);
}

void Iff::UpdateUniforms( RegalContext * ctx )
{
  Internal("Regal::Iff::UpdateUniforms",ctx);

  Program & pgm = *currprog;
  DispatchTable & tbl = ctx->dispatcher.emulation;
  if( pgm.ver != ffstate.Ver() ) {
    pgm.ver = ffstate.Ver();
    const State::Store & p = ffstate.processed;
    const State::StoreUniform & u = ffstate.uniform;
    for( std::map< RegalFFUniformEnum, Program::UniformInfo>::iterator i = pgm.uniforms.begin(); i != pgm.uniforms.end(); ++i ) {
      Program::UniformInfo & ui = (*i).second;
      switch( (*i).first ) {
        case FFU_ModelviewInverseTranspose: {
          if( ui.ver != modelview.Ver() ) {
            ui.ver = modelview.Ver();
            r3::Matrix4f mvi = modelview.Top().Inverse();
            if( p.rescaleNormal ) {
              mvi = RescaleNormal( mvi );
            }
            tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, mvi.Transpose().m );
          }
          break;
        }
        case FFU_ModelviewInverse: {
          if( ui.ver != modelview.Ver() ) {
            ui.ver = modelview.Ver();
            r3::Matrix4f mvi = modelview.Top().Inverse();
            if( p.rescaleNormal ) {
              mvi = RescaleNormal( mvi );
            }
            tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, mvi.Ptr() );
          }
          break;
        }
        case FFU_Modelview: {
          if( ui.ver != modelview.Ver() ) {
            ui.ver = modelview.Ver();
            tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, modelview.Top().m );
          }
          break;
        }
        case FFU_Projection: {
          if( ui.ver != projection.Ver() ) {
            ui.ver = projection.Ver();
            tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, projection.Top().m );
          }
          break;
        }
        case FFU_TextureMatrix0:
        case FFU_TextureMatrix1:
        case FFU_TextureMatrix2:
        case FFU_TextureMatrix3:
        case FFU_TextureMatrix4:
        case FFU_TextureMatrix5:
        case FFU_TextureMatrix6:
        case FFU_TextureMatrix7:
        case FFU_TextureMatrix8:
        case FFU_TextureMatrix9:
        case FFU_TextureMatrix10:
        case FFU_TextureMatrix11:
        case FFU_TextureMatrix12:
        case FFU_TextureMatrix13:
        case FFU_TextureMatrix14:
        case FFU_TextureMatrix15: {
          int idx = ( ((*i).first) - FFU_TextureMatrix0 );
          if( ui.ver != texture[ idx ].Ver() ) {
            ui.ver = texture[ idx ].Ver();
            r3::Matrix4f m = texture[ idx ].Top();
            tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, m.m );
          }
          break;
        }
        case FFU_TextureEnvColor0:
        case FFU_TextureEnvColor1:
        case FFU_TextureEnvColor2:
        case FFU_TextureEnvColor3:
        case FFU_TextureEnvColor4:
        case FFU_TextureEnvColor5:
        case FFU_TextureEnvColor6:
        case FFU_TextureEnvColor7:
        case FFU_TextureEnvColor8:
        case FFU_TextureEnvColor9:
        case FFU_TextureEnvColor10:
        case FFU_TextureEnvColor11:
        case FFU_TextureEnvColor12:
        case FFU_TextureEnvColor13:
        case FFU_TextureEnvColor14:
        case FFU_TextureEnvColor15: {
          int idx = ( ((*i).first) - FFU_TextureEnvColor0 );
          if( ui.ver != textureEnvColorVer[ idx ] ) {
            ui.ver = textureEnvColorVer[ idx ];
            tbl.glUniform4fv( ui.slot, 1, &textureEnvColor[ idx ].x);
          }
          break;
        }
        case FFU_Light0:
        case FFU_Light1:
        case FFU_Light2:
        case FFU_Light3:
        case FFU_Light4:
        case FFU_Light5:
        case FFU_Light6:
        case FFU_Light7: {
          int idx = ( ((*i).first) - FFU_Light0 );
          if( ui.ver != u.light[ idx ].ver ) {
            ui.ver = u.light[ idx ].ver;
            tbl.glUniform4fv( ui.slot, LE_Elements, &u.light[ idx ].ambient.x);
          }
          break;
        }
        case FFU_MaterialFront:
        case FFU_MaterialBack: {
          int idx = ( ((*i).first) - FFU_MaterialFront );
          if( ui.ver != u.mat[ idx ].ver ) {
            ui.ver = u.mat[ idx ].ver;
            tbl.glUniform4fv( ui.slot, ME_Elements, &u.mat[ idx ].ambient.x);
          }
          break;
        }
        case FFU_LightModelAmbient: {
          tbl.glUniform4fv( ui.slot, 1, &u.lightModelAmbient.x);
          break;
        }
        case FFU_Texgen0ObjS: case FFU_Texgen0ObjT: case FFU_Texgen0ObjR: case FFU_Texgen0ObjQ:
        case FFU_Texgen0EyeS: case FFU_Texgen0EyeT: case FFU_Texgen0EyeR: case FFU_Texgen0EyeQ:
        case FFU_Texgen1ObjS: case FFU_Texgen1ObjT: case FFU_Texgen1ObjR: case FFU_Texgen1ObjQ:
        case FFU_Texgen1EyeS: case FFU_Texgen1EyeT: case FFU_Texgen1EyeR: case FFU_Texgen1EyeQ:
        case FFU_Texgen2ObjS: case FFU_Texgen2ObjT: case FFU_Texgen2ObjR: case FFU_Texgen2ObjQ:
        case FFU_Texgen2EyeS: case FFU_Texgen2EyeT: case FFU_Texgen2EyeR: case FFU_Texgen2EyeQ:
        case FFU_Texgen3ObjS: case FFU_Texgen3ObjT: case FFU_Texgen3ObjR: case FFU_Texgen3ObjQ:
        case FFU_Texgen3EyeS: case FFU_Texgen3EyeT: case FFU_Texgen3EyeR: case FFU_Texgen3EyeQ: {
          int idx = ( (*i).first ) - FFU_Texgen0ObjS;
          int comp = idx % 4;
          int unit = idx >> 3;
          const State::TexgenUniform & tg = u.tex[unit].texgen[comp];
          if( idx & 4 ) {
            if( ui.ver != tg.eyeVer ) {
              ui.ver = tg.eyeVer;
              tbl.glUniform4fv( ui.slot, 1, & tg.eye.x );
            }
          } else {
            if( ui.ver != tg.objVer ) {
              ui.ver = tg.objVer;
              tbl.glUniform4fv( ui.slot, 1, & tg.obj.x );
            }
          }
          break;
        }
        case FFU_ClipPlane0: case FFU_ClipPlane1: case FFU_ClipPlane2: case FFU_ClipPlane3:
        case FFU_ClipPlane4: case FFU_ClipPlane5: case FFU_ClipPlane6: case FFU_ClipPlane7: {
          int idx = ( (*i).first ) - FFU_ClipPlane0;
          if( ui.ver != u.clip[idx].ver ) {
            ui.ver = u.clip[idx].ver;
            tbl.glUniform4fv( ui.slot, 1, & u.clip[idx].plane.x );
          }
          break;
        }
        case FFU_Fog: {
          if( ui.ver != u.fog.ver ) {
            ui.ver = u.fog.ver;
            tbl.glUniform4fv( ui.slot, 2, &u.fog.params[0].x);
          }
          break;
        }
        case FFU_AlphaRef: {
          if( ui.ver != u.alphaTest.ver ) {
            ui.ver = u.alphaTest.ver;
            tbl.glUniform1f( ui.slot, u.alphaTest.alphaRef );
          }
          break;
        }
        case FFU_Attrib: {
          if( ui.ver != u.vabVer ) {
            ui.ver = u.vabVer;
            tbl.glUniform4fv( ui.slot, REGAL_EMU_IFF_VERTEX_ATTRIBS, & immVab.attr[0].x );
          }
          break;
        }
        default:
          break;
      }
    }
  }
}

inline bool operator == ( const Iff::State::Store & lhs, const Iff::State::Store & rhs )
{
  return memcmp(&lhs,&rhs,sizeof( Iff::State::Store ) - sizeof( lhs.ver ))==0;
}

inline size_t ProgHashToSet( size_t hash ) {
  return hash & ( ( 1 << ( REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS - REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ) ) - 1 );
}

std::vector<GLuint> wayhist(1 << ( REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS - REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ),0);
std::vector<GLuint> evicthist(1 << ( REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS - REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ),0);

void Iff::UseFixedFunctionProgram( RegalContext * ctx )
{
  Internal("Regal::Iff::UseFixedFunctionProgram",ctx);

  if( currprog != NULL && currprog->ver == ver.Current() ) {
    return;
  }
  ffstate.Process( this );
  size_t base = ProgHashToSet( ffstate.processed.hash ) << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS;
  int match = -1;
  for( int i = 0; i < (1 << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS); i++ ) {
    assert(i<(1 << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS));
    if( ffprogs[ base + i ].store == ffstate.processed ) {
      match = i;
      break;
    }
  }
  Program * p = NULL;
  if( match < 0 ) {
    wayhist[ base >> REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ]++;

#if 0
    fprintf( stderr, "wayhist: " );
    GLuint sum = 0;
    for( int i = 0; i < 1 << ( REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS - REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ); i++ ) {
      fprintf( stderr, "%4d ", wayhist[i] );
      sum += wayhist[i];
    }
    fprintf( stderr, "-- %d\n", sum );
#endif
    match = 0;
    progcount++;
    for( int i = 0; i < 1 << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS; i++ ) {
      if( ffprogs[ base + i ].ver < ffprogs[ base + match ].ver ) {
        match = i;
      }
    }
    p = & ffprogs[ base + match ];
    // delete this program
    if( p->pg != 0 ) {
      DispatchTable & tbl = ctx->dispatcher.emulation;
      tbl.call(&tbl.glDeleteShader)( p->vs );
      tbl.call(&tbl.glDeleteShader)( p->fs );
      tbl.call(&tbl.glDeleteProgram)( p->pg );
      *p = Program();
      evicthist[ base >> REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ]++;
#if 0
      fprintf( stderr, "evicthist: " );
      GLuint sum = 0;
      for( int i = 0; i < 1 << ( REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS - REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SET_BITS ); i++ ) {
        fprintf( stderr, "%4d ", evicthist[i] );
        sum += evicthist[i];
      }
      fprintf( stderr, "-- %d\n", sum );
#endif
    }
    string_list vsSrc;
    GenerateVertexShaderSource( this, ffstate, vsSrc );
    string_list fsSrc;
    GenerateFragmentShaderSource( this, fsSrc );
    p->Init( ctx, ffstate.processed, vsSrc.str().c_str(), fsSrc.str().c_str() );
    p->progcount = progcount;
  }
  p = & ffprogs[ base + match ];
  currprog = p;
  ctx->dispatcher.emulation.glUseProgram( currprog->pg );
  UpdateUniforms( ctx );
}

void Iff::UseShaderProgram( RegalContext * ctx )
{
  Internal("Regal::Iff::UseShaderProgram",ctx);

  if( currprog != NULL && currprog->ver == ver.Current() ) {
    return;
  }
  ffstate.Process( this );
  RegalAssert( shprogmap.count( program ) != 0 );
  currprog = & shprogmap[ program ];
  if( currprog->pg == 0 ) {
    Warning( "The program is 0. That can't be right.\n" );
  }
  UpdateUniforms( ctx );
}

static int remove_version(GLchar *str)
{
  GLchar version[4];

  if (!str)
    return -1;

  GLchar *i = str;
  while ((i = strstr(i,"#version ")))
  {
    if (i==str || i[-1]=='\n')
    {
      i[0] = '/';
      i[1] = '/';

      // return version number
      i+=9;
      while (*i == ' ') i++;
      for (int j=0; j<3; i++,j++)
        version[j] = *i;
      version[3] = '\0';
      return atoi(version);
    }
    ++i;
  }
  return -1;
}

static void remove_precision(GLchar *str)
{
  if (!str)
    return;

  GLchar *i = str;
  while ((i = strstr(i,"precision ")))
  {
    if (i==str || i[-1]=='\n')
    {
      i[0] = '/';
      i[1] = '/';
    }
    i+=9;
  }
}

// replace ftransform with "rgl_ftform" in order to avoid conflict with possibly deprecated ftransform
static void replace_ftransform(GLchar *str)
{
  if (!str)
    return;

  GLchar *i = str;
  const GLchar *replacement = "rgl_ftform";
  while ((i = strstr(i,"ftransform()")))
  {
    memcpy( i, replacement, 10 );
    i+=10;
  }
}

static bool uses_string( const char * str, int count, const GLchar * const * srcstr, const GLint *length ) {
    for( int i = 0; i < count; i++ ) {
       const size_t len = (length && length[i] >= 0) ? length[i] : strlen(srcstr[i]);
       std::string seg(srcstr[i],len);
       if( seg.find( str ) != std::string::npos ) {
           return true;
       }
    }
    return false;
}

void Iff::ShaderSource( RegalContext *ctx, GLuint shader, GLsizei count, const GLchar * const * srcstr, const GLint *length)
{
  bool uses_ftransform = false;
  for( int i = 0; i < count && uses_ftransform == false; i++ ) {
    std::string seg;
    if( length != NULL ) {
      seg = std::string( srcstr[i], length[i] );
    } else {
      seg = std::string( srcstr[i] );
    }
    if( seg.find( "ftransform()" ) != std::string::npos ) {
      uses_ftransform = true;
    }
  }
  //uses_ftransform = false; // hack
  if( srcstr[0][0] == '#' && srcstr[0][1] == 'v' ) {
    ctx->dispatcher.emulation.glShaderSource( shader, count, srcstr, length );
    return;
  }

  // Make room for Regal preamble

  std::vector<GLchar *> s(count + 1);
  std::vector<GLint   > l(count + 1);

  // Copy the input shader code so we can change it in-place
  int version = -1;
  for (GLsizei i=0; i<count; ++i)
  {
    l[i+1] = length ? length[i] : static_cast<GLint>(strlen(static_cast<const char *>(srcstr[i])));
    if (srcstr)
      s[i+1] = strndup(srcstr[i],l[i+1]);
    if (version<0)
      version = remove_version(s[i+1]);
    if (uses_ftransform && gles )
      replace_ftransform(s[i+1]);
    if( legacy )
      remove_precision(s[i+1]);
  }


  // Preamble

  string_list ss;
  if (gles)
  {
    // hack around #version 100 on x86 failing compilation
    if( ctx->info->gl_version_major >= 3 ) {
      ss << "#version 120\n";
      ss << "#define precision\n";
    } else {
#if REGAL_FORCE_DESKTOP_GLSL
      ss << "#version 140\n";
#else
      ss << "#version 100\n";
#endif
    }
  }
  else if (legacy)
  {
    ss << "#version 120\n";
    ss << "#define precision\n";
  }
  else
  {
    if (version > 0) {
      // We should honor the version in the original shader if we can, but leave out for now.
      //ss << "#version " << version << "\n";
      if (shaderTypeMap[ shader ] == GL_VERTEX_SHADER)
      {
        ss << "#define in attribute\n";
        ss << "#define out varying\n";
      }
      else
      {
        ss << "#define in varying\n";
      }
    } else {
      ss << "#version 140\n";
    }
  }
  if (gles || legacy)
  {
    if (shaderTypeMap[ shader ] == GL_VERTEX_SHADER)
    {
      ss << "#define in attribute\n";
      ss << "#define out varying\n";
    }
    else
    {
      ss << "#define in varying\n";
    }
  } else {
    if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {

    } else {
      ss << "#define gl_FragColor           rglFragColor\n";
      ss << "out vec4 rglFragColor;\n";
      ss << "#define texture1D texture\n";
      ss << "#define texture2D texture\n";
      ss << "#define textureCube texture\n";
    }
  }
  if( gles ) {
    ss << "precision highp float;\n";
  }

  ss << "#define centroid                              \n";
  ss << "#define gl_FogFragCoord        rglFogFragCoord\n";
  if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {
    ss << "#define gl_Color               rglColor\n";
  } else {
    ss << "#define gl_Color               rglFrontColor\n";
  }
  ss << "#define gl_FrontColor          rglFrontColor\n";
  if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {
    ss << "#define gl_SecondaryColor               rglSecondaryColor\n";
  } else {
    ss << "#define gl_SecondaryColor               rglFrontSecondaryColor\n";
  }

  ss << "#define gl_FrontSecondaryColor rglFrontSecondaryColor\n";
  ss << "#define gl_ClipVertex          rglClipVertex\n";
  ss << "#define gl_FragDepth           rglFragDepth\n";
if( gles ) {
  ss << "#define texture3d              texture2d\n";
  ss << "#define texture3D(a,b)         texture2D(a,b.xy)\n";
  ss << "#define sampler3D              sampler2D\n";
}

if( uses_string( "gl_FogFragCoord", count, srcstr, length ) ) {
  ss << "out float rglFogFragCoord;\n";
}

// NOTE: ES 2.0 does not support gl_FragDepth, so we just discard it, for now
// See: http://www.khronos.org/registry/gles/specs/2.0/GLSL_ES_Specification_1.0.17.pdf

if( uses_string( "gl_FragDepth", count, srcstr, length ) ) {
  ss << (shaderTypeMap[shader]==GL_VERTEX_SHADER ? "out" : " ") <<  " float rglFragDepth;\n";
}


if( uses_string( "gl_Color", count, srcstr, length ) ) {
  if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {
    ss << "in vec4 rglColor;\n";
  } else {
    ss << "in vec4 rglFrontColor;\n";
  }
}
if( uses_string( "gl_FrontColor", count, srcstr, length ) ) {
  ss << "out vec4 rglFrontColor;\n";
}

// NOTE: gl_SecondaryColor can be a vertex shader output, or a fragment shader input.
// See: http://www.opengl.org/registry/doc/GLSLangSpec.4.30.7.pdf

if( uses_string( "gl_SecondaryColor", count, srcstr, length ) ) {
  if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {
    ss << "in vec4 rglSecondaryColor;\n";
  } else {
    ss << "in vec4 rglFrontSecondaryColor;\n";
  }
}

if( uses_string( "gl_FrontSecondaryColor", count, srcstr, length ) ) {
  ss << "out vec4 rglFrontSecondaryColor;\n";
}
if( uses_string( "gl_ClipVertex", count, srcstr, length ) ) {
  // should be "out", but temporarily disabled due to register pressure concerns
  // ss << "out vec4 rglClipVertex;\n";
  ss << "vec4 rglClipVertex;\n";
}

  if ( uses_ftransform  && gles ) {
    ss << "uniform mat4 rglModelview;\n";
    ss << "uniform mat4 rglProjection;\n";
    // should be "in", but temporarily disabled due to register pressure concerns
    // ss << "in vec4 rglVertex;\n";
    ss << "vec4 rglVertex;\n";
  }

  ss << "#define gl_Modelview rglModelview\n";
  ss << "#define gl_Projection rglProjection\n";
  ss << "#define gl_TextureMatrix0 rglTextureMatrix0\n";
  ss << "#define gl_Sampler0 rglSampler0\n\n";

  if ( uses_ftransform && gles ) {
    ss << "vec4 rgl_ftform() { return gl_Projection * gl_Modelview * rglVertex; }\n\n";
  }
  //Logging::Output( "foo:\n%s", ss.str().c_str() );

  string preamble = ss.str();
  s[0] = const_cast<char *>(preamble.c_str());
  l[0] = (int)preamble.length();
  ctx->dispatcher.emulation.glShaderSource( shader, count + 1, const_cast<const GLchar**>(&s[0]), &l[0] );
}

void Iff::LinkProgram( RegalContext *ctx, GLuint program ) {
  // need to bind attribs just before link for Regal-generated attribs
  if ( program != 0 ) {
    if( shprogmap.count( program ) == 0 ) {
      ffstate.Process( this );
      Program & p = shprogmap[ program ];
      p.pg = program;
      p.UserShaderModeAttribs(ctx);
    }
  }
  ctx->dispatcher.emulation.glLinkProgram( program );
  Program & p = shprogmap[ program ];
  DispatchTable & tbl = ctx->dispatcher.emulation;
  p.Samplers( ctx, tbl );
  p.Uniforms( ctx, tbl );
}

}; // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
