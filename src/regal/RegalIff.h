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

 Regal fixed-function emulation.
 Cass Everitt

 */

#ifndef __REGAL_IFF_H__
#define __REGAL_IFF_H__

#include "RegalUtil.h"

#define REGAL_IMMEDIATE_BUFFER_SIZE 8192

// Configurable

#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS 8

// Derived

#define REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE      (1<<REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS)

// OpenGL spec is four fixed-function texture units.
// Regal can emulate eight, default is four

#ifndef REGAL_EMU_IFF_TEXTURE_UNITS
#define REGAL_EMU_IFF_TEXTURE_UNITS 4
#endif

#ifndef REGAL_EMU_IFF_VERTEX_ATTRIBS
#define REGAL_EMU_IFF_VERTEX_ATTRIBS 16
#endif

#define REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH 128
#define REGAL_FIXED_FUNCTION_MAX_LIGHTS           8
#define REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES      8

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <climits>
#include <cstring>

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "RegalEmu.h"
#include "RegalPrivate.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalSharedMap.h"
#include "linear.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

const GLenum texenvModeGL[] = {
  GL_FALSE,
  GL_REPLACE,
  GL_MODULATE,
  GL_ADD,
  GL_DECAL,
  GL_BLEND,
  GL_COMBINE
};

enum RegalFFUniformEnum {
  FFU_foo = 0,
  FFU_Modelview,
  FFU_ModelviewInverse,
  FFU_ModelviewInverseTranspose,
  FFU_Projection,
  FFU_ProjectionInverse,
  FFU_ModelviewProjection,
  FFU_ModelviewProjectionInverse,
  FFU_TextureMatrix0,
  FFU_TextureMatrix1,
  FFU_TextureMatrix2,
  FFU_TextureMatrix3,
  FFU_TextureMatrix4,
  FFU_TextureMatrix5,
  FFU_TextureMatrix6,
  FFU_TextureMatrix7,
  FFU_TextureMatrix8,
  FFU_TextureMatrix9,
  FFU_TextureMatrix10,
  FFU_TextureMatrix11,
  FFU_TextureMatrix12,
  FFU_TextureMatrix13,
  FFU_TextureMatrix14,
  FFU_TextureMatrix15,
  FFU_TextureEnvColor0,
  FFU_TextureEnvColor1,
  FFU_TextureEnvColor2,
  FFU_TextureEnvColor3,
  FFU_TextureEnvColor4,
  FFU_TextureEnvColor5,
  FFU_TextureEnvColor6,
  FFU_TextureEnvColor7,
  FFU_TextureEnvColor8,
  FFU_TextureEnvColor9,
  FFU_TextureEnvColor10,
  FFU_TextureEnvColor11,
  FFU_TextureEnvColor12,
  FFU_TextureEnvColor13,
  FFU_TextureEnvColor14,
  FFU_TextureEnvColor15,
  FFU_Texgen0ObjS,
  FFU_Texgen0ObjT,
  FFU_Texgen0ObjR,
  FFU_Texgen0ObjQ,
  FFU_Texgen0EyeS,
  FFU_Texgen0EyeT,
  FFU_Texgen0EyeR,
  FFU_Texgen0EyeQ,
  FFU_Texgen1ObjS,
  FFU_Texgen1ObjT,
  FFU_Texgen1ObjR,
  FFU_Texgen1ObjQ,
  FFU_Texgen1EyeS,
  FFU_Texgen1EyeT,
  FFU_Texgen1EyeR,
  FFU_Texgen1EyeQ,
  FFU_Texgen2ObjS,
  FFU_Texgen2ObjT,
  FFU_Texgen2ObjR,
  FFU_Texgen2ObjQ,
  FFU_Texgen2EyeS,
  FFU_Texgen2EyeT,
  FFU_Texgen2EyeR,
  FFU_Texgen2EyeQ,
  FFU_Texgen3ObjS,
  FFU_Texgen3ObjT,
  FFU_Texgen3ObjR,
  FFU_Texgen3ObjQ,
  FFU_Texgen3EyeS,
  FFU_Texgen3EyeT,
  FFU_Texgen3EyeR,
  FFU_Texgen3EyeQ,
  FFU_Light0,
  FFU_Light1,
  FFU_Light2,
  FFU_Light3,
  FFU_Light4,
  FFU_Light5,
  FFU_Light6,
  FFU_Light7,
  FFU_MaterialFront,
  FFU_MaterialBack,
  FFU_LightModelAmbient,
  FFU_ClipPlane0,
  FFU_ClipPlane1,
  FFU_ClipPlane2,
  FFU_ClipPlane3,
  FFU_ClipPlane4,
  FFU_ClipPlane5,
  FFU_ClipPlane6,
  FFU_ClipPlane7,
  FFU_Fog,
  FFU_ConstantColor,
  FFU_Attrib,
  FFU_AlphaRef,
};

struct RegalFFUniformInfo {
  RegalFFUniformEnum val;
  const GLchar * name;
};

static const RegalFFUniformInfo regalFFUniformInfo[] = {
  { FFU_foo, "foo" },
  { FFU_Modelview, "rglModelview" },
  { FFU_ModelviewInverse, "rglModelviewInverse" },
  { FFU_ModelviewInverseTranspose, "rglModelviewInverseTranspose" },
  { FFU_Projection, "rglProjection" },
  { FFU_ProjectionInverse, "rglProjectionInverse" },
  { FFU_ModelviewProjection, "rglModelviewProjection" },
  { FFU_ModelviewProjectionInverse, "rglModelviewProjectionInverse" },
  { FFU_TextureMatrix0, "rglTextureMatrix0" },
  { FFU_TextureMatrix1, "rglTextureMatrix1" },
  { FFU_TextureMatrix2, "rglTextureMatrix2" },
  { FFU_TextureMatrix3, "rglTextureMatrix3" },
  { FFU_TextureMatrix4, "rglTextureMatrix4" },
  { FFU_TextureMatrix5, "rglTextureMatrix5" },
  { FFU_TextureMatrix6, "rglTextureMatrix6" },
  { FFU_TextureMatrix7, "rglTextureMatrix7" },
  { FFU_TextureMatrix8, "rglTextureMatrix8" },
  { FFU_TextureMatrix9, "rglTextureMatrix9" },
  { FFU_TextureMatrix10, "rglTextureMatrix10" },
  { FFU_TextureMatrix11, "rglTextureMatrix11" },
  { FFU_TextureMatrix12, "rglTextureMatrix12" },
  { FFU_TextureMatrix13, "rglTextureMatrix13" },
  { FFU_TextureMatrix14, "rglTextureMatrix14" },
  { FFU_TextureMatrix15, "rglTextureMatrix15" },
  { FFU_TextureEnvColor0, "rglTexEnvColor0" },
  { FFU_TextureEnvColor1, "rglTexEnvColor1" },
  { FFU_TextureEnvColor2, "rglTexEnvColor2" },
  { FFU_TextureEnvColor3, "rglTexEnvColor3" },
  { FFU_TextureEnvColor4, "rglTexEnvColor4" },
  { FFU_TextureEnvColor5, "rglTexEnvColor5" },
  { FFU_TextureEnvColor6, "rglTexEnvColor6" },
  { FFU_TextureEnvColor7, "rglTexEnvColor7" },
  { FFU_TextureEnvColor8, "rglTexEnvColor8" },
  { FFU_TextureEnvColor9, "rglTexEnvColor9" },
  { FFU_TextureEnvColor10, "rglTexEnvColor10" },
  { FFU_TextureEnvColor11, "rglTexEnvColor11" },
  { FFU_TextureEnvColor12, "rglTexEnvColor12" },
  { FFU_TextureEnvColor13, "rglTexEnvColor13" },
  { FFU_TextureEnvColor14, "rglTexEnvColor14" },
  { FFU_TextureEnvColor15, "rglTexEnvColor15" },
  { FFU_Texgen0ObjS, "rglTexGen0ObjS" },
  { FFU_Texgen0ObjT, "rglTexGen0ObjT" },
  { FFU_Texgen0ObjR, "rglTexGen0ObjR" },
  { FFU_Texgen0ObjQ, "rglTexGen0ObjQ" },
  { FFU_Texgen0EyeS, "rglTexGen0EyeS" },
  { FFU_Texgen0EyeT, "rglTexGen0EyeT" },
  { FFU_Texgen0EyeR, "rglTexGen0EyeR" },
  { FFU_Texgen0EyeQ, "rglTexGen0EyeQ" },
  { FFU_Texgen1ObjS, "rglTexGen1ObjS" },
  { FFU_Texgen1ObjT, "rglTexGen1ObjT" },
  { FFU_Texgen1ObjR, "rglTexGen1ObjR" },
  { FFU_Texgen1ObjQ, "rglTexGen1ObjQ" },
  { FFU_Texgen1EyeS, "rglTexGen1EyeS" },
  { FFU_Texgen1EyeT, "rglTexGen1EyeT" },
  { FFU_Texgen1EyeR, "rglTexGen1EyeR" },
  { FFU_Texgen1EyeQ, "rglTexGen1EyeQ" },
  { FFU_Texgen2ObjS, "rglTexGen2ObjS" },
  { FFU_Texgen2ObjT, "rglTexGen2ObjT" },
  { FFU_Texgen2ObjR, "rglTexGen2ObjR" },
  { FFU_Texgen2ObjQ, "rglTexGen2ObjQ" },
  { FFU_Texgen2EyeS, "rglTexGen2EyeS" },
  { FFU_Texgen2EyeT, "rglTexGen2EyeT" },
  { FFU_Texgen2EyeR, "rglTexGen2EyeR" },
  { FFU_Texgen2EyeQ, "rglTexGen2EyeQ" },
  { FFU_Texgen3ObjS, "rglTexGen3ObjS" },
  { FFU_Texgen3ObjT, "rglTexGen3ObjT" },
  { FFU_Texgen3ObjR, "rglTexGen3ObjR" },
  { FFU_Texgen3ObjQ, "rglTexGen3ObjQ" },
  { FFU_Texgen3EyeS, "rglTexGen3EyeS" },
  { FFU_Texgen3EyeT, "rglTexGen3EyeT" },
  { FFU_Texgen3EyeR, "rglTexGen3EyeR" },
  { FFU_Texgen3EyeQ, "rglTexGen3EyeQ" },
  { FFU_Light0, "rglLight0[0]" },
  { FFU_Light1, "rglLight1[0]" },
  { FFU_Light2, "rglLight2[0]" },
  { FFU_Light3, "rglLight3[0]" },
  { FFU_Light4, "rglLight4[0]" },
  { FFU_Light5, "rglLight5[0]" },
  { FFU_Light6, "rglLight6[0]" },
  { FFU_Light7, "rglLight7[0]" },
  { FFU_MaterialFront, "rglMaterialFront" },
  { FFU_MaterialBack, "rglMaterialBack" },
  { FFU_LightModelAmbient, "rglLightModelAmbient" },
  { FFU_ClipPlane0, "rglClipPlane0" },
  { FFU_ClipPlane1, "rglClipPlane1" },
  { FFU_ClipPlane2, "rglClipPlane2" },
  { FFU_ClipPlane3, "rglClipPlane3" },
  { FFU_ClipPlane4, "rglClipPlane4" },
  { FFU_ClipPlane5, "rglClipPlane5" },
  { FFU_ClipPlane6, "rglClipPlane6" },
  { FFU_ClipPlane7, "rglClipPlane7" },
  { FFU_Fog, "rglFog" },
  { FFU_ConstantColor, "rglConstantColor" },
  { FFU_Attrib, "rglAttrib[0]" },
  { FFU_AlphaRef, "rglAlphaRef" },
};

template <typename T> bool RFFIsVector( const T p ) { UNUSED_PARAMETER(p); return false; }
template <typename T> bool RFFIsVector( const T * p ) { UNUSED_PARAMETER(p); return true; }

template <typename T> GLfloat RFFToFloat( int i, const T p ) { UNUSED_PARAMETER(i); return GLfloat( p ); }
template <typename T> GLfloat RFFToFloat( int i, const T * p ) { return GLfloat( p[i] ); }
template <typename T> GLfloat RFFToFloatN( int i, const T p ) { UNUSED_PARAMETER(i); return GLfloat( p ); }
template <typename T> GLfloat RFFToFloatN( int i, const T * p ) { return GLfloat( p[i] ); }
template <> inline GLfloat RFFToFloatN( int i, const GLint p ) { UNUSED_PARAMETER(i); return GLfloat( double( p ) / double( INT_MAX ) ); }
template <> inline GLfloat RFFToFloatN( int i, const int * p ) { return GLfloat( double( p[i] ) / double( INT_MAX ) ); }



struct Iff
{
  Iff()
  {
  }

  Iff(const Iff &other)
  {
    UNUSED_PARAMETER(other);
  }

  Iff &operator=(const Iff &other)
  {
    UNUSED_PARAMETER(other);
    if (&other!=this)
    {
    }
    return *this;
  }

  ~Iff()
  {
  }

  // Vertex arrays
  GLuint catIndex;
  GLuint ffAttrMap[ REGAL_EMU_IFF_VERTEX_ATTRIBS ];
  GLuint ffAttrInvMap[ REGAL_EMU_IFF_VERTEX_ATTRIBS ];
  GLuint ffAttrTexBegin;
  GLuint ffAttrTexEnd;
  GLuint ffAttrNumTex;
  GLuint maxVertexAttribs;

  void InitVertexArray( RegalContext * ctx )
  {
    maxVertexAttribs = ctx->info->maxVertexAttribs;

    if( maxVertexAttribs >= 16 ) {
      RegalAssert( REGAL_EMU_IFF_VERTEX_ATTRIBS == 16);
      //RegalOutput( "Setting up for %d Vertex Attribs\n", maxVertexAttribs );
      for( int i = 0; i < 16; i++ ) {
        ffAttrMap[i] = RFF2AMap16[i];
        ffAttrInvMap[i] = RFF2AInvMap16[i];
      }
      ffAttrTexBegin = RFF2ATexBegin16;
      ffAttrTexEnd = RFF2ATexEnd16;
    } else {
      RegalAssert( maxVertexAttribs >= 8 );
      //RegalOutput( "Setting up for 8 Vertex Attribs" );
      for( int i = 0; i < 8; i++ ) {
        ffAttrMap[i] = RFF2AMap8[i];
        ffAttrInvMap[i] = RFF2AInvMap8[i];
      }
      for( int i = 8; i < REGAL_EMU_IFF_VERTEX_ATTRIBS; i++ ) {
        ffAttrMap[i] = GLuint(-1);
        ffAttrInvMap[i] = GLuint(-1);
      }
      ffAttrTexBegin = RFF2ATexBegin8;
      ffAttrTexEnd = RFF2ATexEnd8;
    }
    ffAttrNumTex = ffAttrTexEnd - ffAttrTexBegin;
    catIndex = 0;
  }

  GLuint ClientStateToIndex( GLenum state ) {
    switch( state ) {
      case GL_VERTEX_ARRAY: return ffAttrMap[ RFF2A_Vertex ];
      case GL_NORMAL_ARRAY: return ffAttrMap[ RFF2A_Normal ];
      case GL_COLOR_ARRAY:  return ffAttrMap[ RFF2A_Color ];
      case GL_SECONDARY_COLOR_ARRAY: return ffAttrMap[ RFF2A_SecondaryColor ];
      case GL_FOG_COORD_ARRAY: return ffAttrMap[ RFF2A_FogCoord ];
      case GL_EDGE_FLAG_ARRAY: return ffAttrMap[ RFF2A_EdgeFlag ];
      case GL_TEXTURE_COORD_ARRAY: {
        if( catIndex < ffAttrNumTex ) {
          return ffAttrTexBegin + catIndex;
        }
        break;
      }
      default: break;
    }
    return ~0u;
  }

  void EnableClientState( RegalContext * ctx, GLenum state ) {
    const GLuint idx = ClientStateToIndex( state );
    if( idx == GLuint(~0) ) {
      return;
    }
    RestoreVao( ctx );
    RegalAssert( idx < maxVertexAttribs );
    if ( idx < maxVertexAttribs ) {
      ctx->dispatcher.emulation.glEnableVertexAttribArray( idx );
      EnableArray( ctx, idx ); // keep ffn up to date
    }
  }

  void DisableClientState( RegalContext * ctx, GLenum state ) {
    const GLuint idx = ClientStateToIndex( state );
    if( idx == GLuint(~0) ) {
      return;
    }
    RestoreVao( ctx );
    RegalAssert( idx < maxVertexAttribs );
    if ( idx < maxVertexAttribs ) {
      ctx->dispatcher.emulation.glDisableVertexAttribArray( idx );
      DisableArray( ctx, idx ); // keep ffn up to date
    }
  }

  void VertexPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )
  {
    //<> if ( insideBeginEnd == true )
    //<> return;

    switch (size)
    {
      case 2:
      case 3:
      case 4:
        break;
      default:
        return;
    }

    switch (type)
    {
      case GL_SHORT:
      case GL_INT:
      case GL_FLOAT:
      case GL_DOUBLE:
        break;
      default:
        return;
    }

    if (stride < 0)
      return;

    RestoreVao( ctx );
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrMap[ RFF2A_Vertex ], size, type, GL_FALSE, stride, pointer );
  }

  void NormalPointer( RegalContext * ctx, GLenum type, GLsizei stride, const GLvoid *pointer ) {
    RestoreVao( ctx );
    GLboolean n = type == GL_FLOAT ? GL_FALSE : GL_TRUE;
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrMap[ RFF2A_Normal ], 3, type, n, stride, pointer );
  }

  void ColorPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer ) {
    RestoreVao( ctx );
    GLboolean n = type == GL_FLOAT ? GL_FALSE : GL_TRUE;
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrMap[ RFF2A_Color ], size, type, n, stride, pointer );
  }

  void SecondaryColorPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer ) {
    RestoreVao( ctx );
    GLboolean n = type == GL_FLOAT ? GL_FALSE : GL_TRUE;
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrMap[ RFF2A_SecondaryColor ], size, type, n, stride, pointer );
  }

  void FogCoordPointer( RegalContext * ctx, GLenum type, GLsizei stride, const GLvoid *pointer ) {
    RestoreVao( ctx );
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrMap[ RFF2A_FogCoord ], 1, type, GL_FALSE, stride, pointer );
  }

  void EdgeFlagPointer( RegalContext * ctx, GLsizei stride, const GLvoid *pointer ) {
    RestoreVao( ctx );
    GLuint index = ffAttrMap[ RFF2A_EdgeFlag ];
    if( index == RFF2A_Invalid ) {
      return;
    }
    ctx->dispatcher.emulation.glVertexAttribPointer( index, 1, GL_UNSIGNED_BYTE, GL_FALSE, stride, pointer );
  }

  void TexCoordPointer( RegalContext * ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer ) {
    if( catIndex >= ffAttrNumTex ) {
      // FIXME: set an error here!
      return;
    }
    RestoreVao( ctx );
    ctx->dispatcher.emulation.glVertexAttribPointer( ffAttrTexBegin + catIndex, size, type, GL_FALSE, stride, pointer );
  }

  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLdouble * d ) {
    ctx->dispatcher.emulation.glGetVertexAttribdv( index, pname, d );
  }

  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLfloat * f ) {
    ctx->dispatcher.emulation.glGetVertexAttribfv( index, pname, f );
  }

  void GetAttrib( RegalContext * ctx, GLuint index, GLenum pname, GLint * i ) {
    ctx->dispatcher.emulation.glGetVertexAttribiv( index, pname, i );
  }

  template <typename T> bool VaGet( RegalContext * ctx, GLenum pname, T * params ) {

    GLuint index = 0;
    switch (pname) {
      case GL_VERTEX_ARRAY_BUFFER_BINDING:
      case GL_VERTEX_ARRAY_SIZE:
      case GL_VERTEX_ARRAY_TYPE:
      case GL_VERTEX_ARRAY_STRIDE:
      case GL_VERTEX_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Vertex ];
        break;
      case GL_NORMAL_ARRAY_BUFFER_BINDING:
      case GL_NORMAL_ARRAY_TYPE:
      case GL_NORMAL_ARRAY_STRIDE:
      case GL_NORMAL_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Normal ];
        break;
      case GL_COLOR_ARRAY_BUFFER_BINDING:
      case GL_COLOR_ARRAY_SIZE:
      case GL_COLOR_ARRAY_TYPE:
      case GL_COLOR_ARRAY_STRIDE:
      case GL_COLOR_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_Color ];
        //if( pname == GL_COLOR_ARRAY_SIZE ) RegalOutput( "Passing index = %d to VA GetAttrib for COLOR_INDEX_SIZE\n", index );
        break;
      case GL_SECONDARY_COLOR_ARRAY_SIZE:
        // This is a convenient lie. --Cass
        *params = 3;
        break;
      case GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING:
      case GL_SECONDARY_COLOR_ARRAY_TYPE:
      case GL_SECONDARY_COLOR_ARRAY_STRIDE:
      case GL_SECONDARY_COLOR_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_SecondaryColor ];
        break;
      case GL_FOG_COORD_ARRAY_BUFFER_BINDING:
      case GL_FOG_COORD_ARRAY_TYPE:
      case GL_FOG_COORD_ARRAY_STRIDE:
      case GL_FOG_COORD_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_FogCoord ];
        break;

      case GL_EDGE_FLAG_ARRAY_BUFFER_BINDING:
      case GL_EDGE_FLAG_ARRAY_STRIDE:
      case GL_EDGE_FLAG_ARRAY_POINTER:
        index = ffAttrMap[ RFF2A_EdgeFlag ];
        break;

      case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
      case GL_TEXTURE_COORD_ARRAY_SIZE:
      case GL_TEXTURE_COORD_ARRAY_TYPE:
      case GL_TEXTURE_COORD_ARRAY_STRIDE:
      case GL_TEXTURE_COORD_ARRAY_POINTER: {
        if( catIndex >= ffAttrNumTex ) {
          // FIXME need to set an error here!
          return false;
        }
        index = ffAttrTexBegin + catIndex;
      }
        break;

        // INDEX arrays are not supported
      case GL_INDEX_ARRAY_TYPE:
        *params = GL_FLOAT;
        break;
      case GL_INDEX_ARRAY_BUFFER_BINDING:
      case GL_INDEX_ARRAY_STRIDE:
      case GL_INDEX_ARRAY_POINTER:
      default:
        return false;
    }

    switch (pname) {
      case GL_VERTEX_ARRAY_BUFFER_BINDING:
      case GL_NORMAL_ARRAY_BUFFER_BINDING:
      case GL_COLOR_ARRAY_BUFFER_BINDING:
      case GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING:
      case GL_FOG_COORD_ARRAY_BUFFER_BINDING:
      case GL_EDGE_FLAG_ARRAY_BUFFER_BINDING:
      case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
        pname = GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
        break;
      case GL_VERTEX_ARRAY_SIZE:
      case GL_COLOR_ARRAY_SIZE:
      case GL_TEXTURE_COORD_ARRAY_SIZE:
        pname = GL_VERTEX_ATTRIB_ARRAY_SIZE;
        break;

      case GL_VERTEX_ARRAY_TYPE:
      case GL_NORMAL_ARRAY_TYPE:
      case GL_COLOR_ARRAY_TYPE:
      case GL_SECONDARY_COLOR_ARRAY_TYPE:
      case GL_FOG_COORD_ARRAY_TYPE:
      case GL_TEXTURE_COORD_ARRAY_TYPE:
        pname = GL_VERTEX_ATTRIB_ARRAY_TYPE;
        break;

      case GL_VERTEX_ARRAY_STRIDE:
      case GL_NORMAL_ARRAY_STRIDE:
      case GL_COLOR_ARRAY_STRIDE:
      case GL_SECONDARY_COLOR_ARRAY_STRIDE:
      case GL_FOG_COORD_ARRAY_STRIDE:
      case GL_EDGE_FLAG_ARRAY_STRIDE:
      case GL_TEXTURE_COORD_ARRAY_STRIDE:
        pname = GL_VERTEX_ATTRIB_ARRAY_STRIDE;
        break;

      case GL_VERTEX_ARRAY_POINTER:
      case GL_NORMAL_ARRAY_POINTER:
      case GL_COLOR_ARRAY_POINTER:
      case GL_SECONDARY_COLOR_ARRAY_POINTER:
      case GL_FOG_COORD_ARRAY_POINTER:
      case GL_EDGE_FLAG_ARRAY_POINTER:
      case GL_TEXTURE_COORD_ARRAY_POINTER:
        pname = GL_VERTEX_ATTRIB_ARRAY_POINTER;
        break;

      default:
        return false;
    }
    RestoreVao( ctx );
    if( index == RFF2A_Invalid ) {
      // What to return in this case?
      return false;
    }
    GetAttrib( ctx, index, pname, params );
    return true;
  }

  bool IsEnabled( RegalContext * ctx, GLenum pname, GLboolean &enabled )
  {
    State::Store & st = ffstate.raw;
    int idx = 0;
    switch( pname ) {
      case GL_TEXTURE_GEN_S:
      case GL_TEXTURE_GEN_T:
      case GL_TEXTURE_GEN_R:
      case GL_TEXTURE_GEN_Q:
        idx = pname - GL_TEXTURE_GEN_S;
        if( activeTextureIndex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
          return false;
        }
        enabled = st.tex[ activeTextureIndex ].texgen[ idx ].enable;
        return true;
      default:
        break;
    }

    const GLuint index = ClientStateToIndex( pname );
    if( index == GLuint(~0) ) {
      return false;
    }
    RegalAssert( index < maxVertexAttribs );
    GLint ret;
    GetAttrib( ctx, index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &ret );
    enabled = static_cast<GLboolean>(ret);
    return true;
  }

  // immediate mode

  struct Attributes { Float4 attr[ REGAL_EMU_IFF_VERTEX_ATTRIBS ]; };

  bool immActive;
  GLuint immProvoking;
  int immCurrent;
  GLenum immPrim;
  Attributes immVab;
  GLubyte immArray[ REGAL_IMMEDIATE_BUFFER_SIZE * REGAL_EMU_IFF_VERTEX_ATTRIBS * 16 ];

  GLuint immVbo;
  GLuint immVao;
  GLuint immQuadsVbo;
  GLuint immShadowVao;

  void InitImmediate( RegalContext * ctx )
  {
    immActive = false;
    immProvoking = 0;
    immCurrent = 0;
    immPrim = GL_POINTS;
    for( GLuint i = 0; i < maxVertexAttribs; i++ ) {
      Float4 & a = immVab.attr[i];
      a.x = a.y = a.z = 0.0f; a.w = 1.0f;
    }

    memset(immArray,0,sizeof(immArray));
    memset(&immVab, 0, sizeof(immVab));

    immShadowVao = 0;

    DispatchTable &tbl = ctx->dispatcher.emulation;
    tbl.glGenVertexArrays( 1, & immVao );
    tbl.glBindVertexArray( immVao );
    BindVertexArray( ctx, immVao ); // to keep ffn current
    tbl.glGenBuffers( 1, & immVbo );
    tbl.glGenBuffers( 1, & immQuadsVbo );
    tbl.glBindBuffer( GL_ARRAY_BUFFER, immVbo );
    for( GLuint i = 0; i < maxVertexAttribs; i++ ) {
      EnableArray( ctx, i ); // to keep ffn current
      tbl.glEnableVertexAttribArray( i );
      tbl.glVertexAttribPointer( i, 4, GL_FLOAT, GL_FALSE, maxVertexAttribs * 16, (GLubyte *)NULL + i * 16 );
    }
    tbl.glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, immQuadsVbo );
    GLushort quadIndexes[ REGAL_IMMEDIATE_BUFFER_SIZE * 3 / 2 ];
    for ( GLushort i = 0; i < REGAL_IMMEDIATE_BUFFER_SIZE / 4; i++ )  {
      quadIndexes[ i * 6 + 0 ] = i * 4 + 0;  // first triangle
      quadIndexes[ i * 6 + 1 ] = i * 4 + 1;
      quadIndexes[ i * 6 + 2 ] = i * 4 + 2;
      quadIndexes[ i * 6 + 3 ] = i * 4 + 0;  // second triangle
      quadIndexes[ i * 6 + 4 ] = i * 4 + 2;
      quadIndexes[ i * 6 + 5 ] = i * 4 + 3;
    }
    tbl.glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( quadIndexes ), quadIndexes, GL_STATIC_DRAW );
    tbl.glBindVertexArray( 0 );
    BindVertexArray( ctx, 0 ); // to keep ffn current

    // The initial texture coordinates are (s; t; r; q) = (0; 0; 0; 1)
    // for each texture coordinate set.

    for( catIndex = 0; catIndex < REGAL_EMU_MAX_TEXTURE_UNITS; catIndex++ ) {
      Attr<4>( ctx, AttrIndex( RFF2A_TexCoord ), 0, 0, 0, 1 );
    }
    catIndex = 0;

    // The initial current normal has coordinates (0; 0; 1).

    Attr<3>( ctx, AttrIndex( RFF2A_Normal ), 0, 0, 1 );

    // The initial RGBA color is (R;G;B;A) = (1; 1; 1; 1) and
    // the initial RGBA secondary color is (0; 0; 0; 1).

    Attr<4>( ctx, AttrIndex( RFF2A_Color ), 1, 1, 1, 1 );
    Attr<4>( ctx, AttrIndex( RFF2A_SecondaryColor ), 0, 0, 0, 1 );

    // The initial fog coordinate is zero.

    // ... so nothing to do for fog coordinate

    // The initial color index is 1.
    // The initial values for all generic vertex attributes are (0:0; 0:0; 0:0; 1:0).
  }

  void DeleteVertexArrays( RegalContext * ctx, GLsizei n, const GLuint * arrays )
  {
    RegalAssert( ctx != NULL );
    for( GLsizei i = 0; i < n; i++ ) {
      GLuint name = arrays[ i ];
      if( name != immVao ) {
        ctx->dispatcher.emulation.glDeleteVertexArrays( 1, &name );
      }
    }
  }

  GLboolean IsVertexArray( RegalContext * ctx, GLuint name )
  {
    RegalAssert( ctx != NULL );
    if (name == immVao )
      return GL_FALSE;
    return ctx->dispatcher.emulation.glIsVertexArray( name );
  }

  void ShadowVao( RegalContext *ctx, GLuint vao ) {
    immShadowVao = vao;
    if( immActive == false ) {
      BindVertexArray( ctx, vao );
    }
  }

  void ShadowClientActiveTexture( GLenum unit ) {
    catIndex = unit - GL_TEXTURE0;
  }

  void Begin( RegalContext * ctx, GLenum mode ) {
    if( immActive == false ) {
      immActive = true;
      ctx->dispatcher.emulation.glBindVertexArray( immVao );
      BindVertexArray( ctx, immVao );  // keep ffn current
    }
    PreDraw( ctx );
    immCurrent = 0;
    immPrim = mode;
  }

  void End( RegalContext * ctx ) {
    Flush( ctx );
  }

  void RestoreVao( RegalContext * ctx ) {
    if( immActive == true ) {
      //RegalOutput( "Restoring vao to %d in Iff\n", immShadowVao );
      ctx->dispatcher.emulation.glBindVertexArray( immShadowVao );
      BindVertexArray( ctx, immShadowVao );
      immActive = false;
    }
  }

  void Flush( RegalContext * ctx )
  {
    if (immCurrent>0) {  // Do nothing for empty buffer
      DispatchTable &tbl = ctx->dispatcher.emulation;
      tbl.glBufferData( GL_ARRAY_BUFFER, immCurrent * sizeof( Attributes ), immArray, GL_DYNAMIC_DRAW );
      if( ( ctx->info->core == true || ctx->info->gles ) && immPrim == GL_QUADS ) {
        tbl.glDrawElements( GL_TRIANGLES, immCurrent * 3 / 2, GL_UNSIGNED_SHORT, 0 );
      } else {
        tbl.glDrawArrays( immPrim, 0, immCurrent );
      }
    }
  }

  void Provoke( RegalContext * ctx ) {
    memcpy( immArray + immCurrent * maxVertexAttribs * 16, &immVab.attr[0].x, maxVertexAttribs * 16 );
    immCurrent++;

    if ( immCurrent >= REGAL_IMMEDIATE_BUFFER_SIZE ) {
      Flush( ctx );
      int restartVerts = 0;
      switch( immPrim ) {
        case GL_QUADS:
          restartVerts = REGAL_IMMEDIATE_BUFFER_SIZE % 4;
          break;
        case GL_TRIANGLES:
          restartVerts = REGAL_IMMEDIATE_BUFFER_SIZE % 3;
          break;
        case GL_LINES:
          restartVerts = REGAL_IMMEDIATE_BUFFER_SIZE % 2;
          break;
        case GL_QUAD_STRIP:
          restartVerts = 2;
          break;
        case GL_TRIANGLE_STRIP:
          restartVerts = 2;
          break;
        case GL_LINE_STRIP:
          restartVerts = 1;
          break;
        default:
          break;
      }

      // For triangle fan we need the first and last vertices
      // for restarting.  All others concern the most recent n.

      if (immPrim==GL_TRIANGLE_FAN)
      {
        memcpy( immArray + maxVertexAttribs * 16, immArray + (REGAL_IMMEDIATE_BUFFER_SIZE - 1) * maxVertexAttribs * 16, maxVertexAttribs * 16);
        immCurrent = 2;
      }
      else
      {
        int offset = REGAL_IMMEDIATE_BUFFER_SIZE - restartVerts;
        memcpy( immArray, immArray + offset * maxVertexAttribs * 16, restartVerts * maxVertexAttribs * 16);
        immCurrent = restartVerts;
      }
    }
  }

  template <int N, bool Norm, typename T> void Attribute( RegalContext * ctx, GLuint idx, const T * v ) {
    if( idx >= maxVertexAttribs ) {
      // FIXME: set an error
      return;
    }
    Float4 & a = immVab.attr[ idx ];
    a.x = ToFloat<Norm>( v[0] );
    a.y = N > 1 ? ToFloat<Norm>( v[1] ) : 0.0f;
    a.z = N > 2 ? ToFloat<Norm>( v[2] ) : 0.0f;
    a.w = N > 3 ? ToFloat<Norm>( v[3] ) : 1.0f;
    ffstate.uniform.vabVer = ver.Update();
    if( idx == immProvoking ) {
      Provoke( ctx );
    }
  }


  template <int N, typename T> void Attr( RegalContext *ctx, GLuint idx, T x, T y = 0, T z = 0, T w = 1 ) {
    T v[4] = { x, y, z, w };
    Attribute<N,false>( ctx, idx,v );
  }

  template <int N, typename T> void AttrN( RegalContext *ctx, GLuint idx, T x, T y = 0, T z = 0, T w = 1 ) {
    T v[4] = { x, y, z, w };
    Attribute<N,true>( ctx, idx, v );
  }
  template <int N, typename T> void Attr( RegalContext *ctx, GLuint idx, const T * v ) {
    Attribute<N,false>( ctx, idx, v );
  }

  template <int N, typename T> void AttrN( RegalContext *ctx, GLuint idx, const T * v ) {
    Attribute<N,true>( ctx, idx, v );
  }

  GLuint AttrIndex( RegalFixedFunctionAttrib attr, int cat = -1 ) const {
    if( attr < RFF2A_TexCoord ) {
      return ffAttrMap[ attr ];
    }
    if( cat < 0 ) {
      cat = catIndex;
    }
    if( attr == RFF2A_TexCoord && GLuint(cat) < ffAttrNumTex ) {
      return ffAttrTexBegin + cat;
    }
    return ~0u;
  }


  // fixed function

  enum CompareFunc {
    CF_Invalid,
    CF_Never,
    CF_Less,
    CF_Equal,
    CF_Lequal,
    CF_Greater,
    CF_NotEqual,
    CF_Gequal,
    CF_Always
  };

  enum TexturePriority {
    TP_1D = 0,
    TP_2D = 1,
    TP_Rect = 2,
    TP_3D = 3,
    TP_CubeMap = 4
  };

  enum TextureTargetBitfield {
    TT_None    = 0,
    TT_1D      = 1<<TP_1D,
    TT_2D      = 1<<TP_2D,
    TT_Rect    = 1<<TP_Rect,
    TT_3D      = 1<<TP_3D,
    TT_CubeMap = 1<<TP_CubeMap
  };

  enum TexgenMode {
    TG_ObjectLinear = 1,
    TG_EyeLinear = 2,
    TG_SphereMap = 3,
    TG_NormalMap = 4,
    TG_ReflectionMap = 5
  };

  enum FogMode {
    FG_Linear = 1,
    FG_Exp = 2,
    FG_Exp2 = 3
  };

  enum ColorMaterialMode {
    CM_None = 0,
    CM_Emission = 1,
    CM_Ambient = 2,
    CM_Diffuse = 3,
    CM_Specular = 4,
    CM_AmbientAndDiffuse = 5
  };

  enum TexenvMode {
    TEM_Invalid,
    TEM_Replace,
    TEM_Modulate,
    TEM_Add,
    TEM_Decal,
    TEM_Blend,
    TEM_Combine
  };


  enum TexenvCombine {
    TEC_Invalid,
    TEC_Replace,
    TEC_Modulate,
    TEC_Add,
    TEC_AddSigned,
    TEC_Interpolate,
    TEC_Subtract,
    TEC_Dot3Rgb,
    TEC_Dot3Rgba
  };

  enum TexenvCombineSrc {
    TCS_Invalid,
    TCS_Constant,
    TCS_PrimaryColor,
    TCS_Previous,
    TCS_Texture,
    TCS_Texture0,
    TCS_Texture1,
    TCS_Texture2,
    TCS_Texture3,
  };

  enum TexenvCombineOp {
    TCO_Invalid,
    TCO_Color,
    TCO_OneMinusColor,
    TCO_Alpha,
    TCO_OneMinusAlpha
  };

  struct TexenvCombineState
  {
    TexenvCombineState()
    {
      memset( this, 0, sizeof(TexenvCombineState) );
      mode = TEC_Invalid;
      src0 = src1 = src2 = TCS_Invalid;
      op0 = op1 = op2 = TCO_Invalid;
    }

    TexenvCombineState( const TexenvCombineState &other )
    {
      memcpy(this, &other, sizeof(TexenvCombineState) );
    }

    TexenvCombineState &operator=(const TexenvCombineState &other)
    {
      if (&other!=this)
        memcpy(this, &other, sizeof(TexenvCombineState));
      return *this;
    }

    TexenvCombine mode;
    TexenvCombineSrc src0;
    TexenvCombineSrc src1;
    TexenvCombineSrc src2;
    TexenvCombineOp op0;
    TexenvCombineOp op1;
    TexenvCombineOp op2;
  };

  struct TextureEnv
  {
    TextureEnv()
    : mode(TEM_Modulate)
    {
    }

    TexenvMode mode;
    TexenvCombineState rgb, a;
  };

  struct TextureUnit
  {
    TextureUnit()
    {
      memset(this,0,sizeof(TextureUnit));
      env = TextureEnv();
    }

    TextureUnit(const TextureUnit &other)
    {
      memcpy(this,&other,sizeof(TextureUnit));
    }

    TextureUnit &operator=(const TextureUnit &other)
    {
      if (&other!=this)
        memcpy(this,&other,sizeof(TextureUnit));
      return *this;
    }

    GLubyte ttb;
    GLint fmt;
    TextureEnv env;
  };

  struct Version {
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

  struct State {

    struct Texgen
    {
      Texgen()
      {
        memset(this, 0, sizeof(Texgen));
        mode = TG_EyeLinear;
      }

      Texgen(const Texgen &other)
      {
        memcpy(this, &other, sizeof(Texgen));
      }

      Texgen &operator=(const Texgen &other)
      {
        if (&other!=this)
          memcpy(this, &other, sizeof(Texgen));
        return *this;
      }

      GLboolean enable;
      TexgenMode mode;
    };

    struct TexgenUniform
    {
      TexgenUniform()
      {
        memset(this, 0, sizeof(TexgenUniform));
      }
      Float4 obj;
      Float4 eye;
      GLuint64 objVer;
      GLuint64 eyeVer;
    };

    struct Texture
    {
      Texture()
      {
        memset(this, 0, sizeof(Texture));
        texgen[0] = texgen[1] = texgen[2] = texgen[3] = Texgen();
        unit = TextureUnit();
      }

      Texture(const Texture &other)
      {
        memcpy(this, &other, sizeof(Texture));
      }

      Texture &operator=(const Texture &other)
      {
        if (&other!=this)
          memcpy(this, &other, sizeof(Texture));
        return *this;
      }

      GLubyte enables;
      bool useMatrix;
      Texgen texgen[4];
      TextureUnit unit;
    };

    struct TextureUniform {
      TextureUniform()
      {
        texgen[0].obj = texgen[0].eye = Float4( 1, 0, 0, 0 );
        texgen[1].obj = texgen[1].eye = Float4( 0, 1, 0, 0 );
      }
      TexgenUniform texgen[4];
    };

    struct AlphaTest {
      AlphaTest()
      {
        memset( this, 0, sizeof( *this ) );
        enable = false;
        comp = CF_Always;
      }
      AlphaTest( const AlphaTest & cpy ) {
        memcpy( this, &cpy, sizeof( *this) );
      }

      GLboolean enable;
      CompareFunc comp;
    };

    struct AlphaTestUniform {
      AlphaTestUniform()
      : alphaRef( 0 )
      , ver( 0 )
      {}
      GLfloat alphaRef;
      GLuint64 ver;
    };

    struct Clip {
      Clip()
      : enable( false )
      {
      }
      bool enable;
    };

    struct ClipUniform {
      ClipUniform()
      : plane( 0, 0, 0, 0 )
      , ver( 0 )
      {}
      Float4 plane;
      GLuint64 ver;
    };

    struct Fog {
      Fog()
      {
        memset( this, 0, sizeof( *this ) );
        enable = false;
        useDepth = true;
        mode = FG_Exp;
      }
      Fog( const Fog & cpy ) {
        memcpy( this, &cpy, sizeof( *this) );
      }
      bool enable;
      bool useDepth;
      FogMode mode : 4;
    };

    struct FogUniform {
      FogUniform()
      : ver( 0 )
      {
        params[0] = Float4( 1, 0, 1, 0 );
        params[1] = Float4( 0, 0, 0, 0 );
      }
      Float4 params[2]; // .x = density, .y = start, .z = end, .w = d/c
      GLuint64 ver;
    };

    struct Light {
      Light()
      {
        memset( this, 0, sizeof( *this ) );
        enable = false;
        spotlight = false;
        attenuate = false;
        local = false;
      }
      Light( const Light & cpy ) {
        memcpy( this, &cpy, sizeof( *this) );
      }

      bool enable;
      bool spotlight;
      bool attenuate;
      bool local;
    };

    struct LightUniform {
      LightUniform()
      : position( 0, 0, 1, 0 )
      , spotDirection( 0.0f, 0.0f, -1.0f, 180.0f )
      , attenuation( 1, 0, 0, 0 )
      , ver( 0 )
      {}
      Float4 ambient;
      Float4 diffuse;
      Float4 specular;
      Float4 position;
      Float4 spotDirection; // spotCutoff is in .w
      Float4 attenuation;   // spotExponent   is in .w
      GLuint64 ver;
    };

    struct MaterialUniform {
      MaterialUniform()
      : ambient( 0.2f, 0.2f, 0.2f, 1.0f )
      , diffuse( 0.8f, 0.8f, 0.8f, 1.0f )
      , specular( 0, 0, 0, 1 )
      , emission( 0, 0, 0, 1 )
      , shininess( 0, 0, 0, 0 )
      , ver( 0 )
      {}
      Float4 ambient;
      Float4 diffuse;
      Float4 specular;
      Float4 emission;
      Float4 shininess;  // shininess is in .x
      GLuint64 ver;
    };

    // Iff::State::Store

    struct Store
    {
      Store()
      {
        memset(this, 0, sizeof(Store));

        // all the booleans and uints want zero initialization anyway, so don't bother doing them individually
        alphaTest = AlphaTest();
        fog = Fog();
        colorMaterialTarget0 = colorMaterialTarget1 = CM_AmbientAndDiffuse;
        for (int ii=0; ii<REGAL_FIXED_FUNCTION_MAX_LIGHTS; ii++) {
          light[ii] = Light();
        }
        for (int ii=0; ii<REGAL_EMU_IFF_TEXTURE_UNITS; ii++) {
          tex[ii] = Texture();
          for (int jj=0; jj<4; jj++) {
            tex[ii].texgen[jj].enable = false;
            tex[ii].texgen[jj].mode = TG_EyeLinear;
          }
        }
        for (int ii=0; ii<REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; ii++) {
          clip[ii] = Clip();
        }
      }

      Store(const Store &other)
      {
        memcpy(this, &other, sizeof(Store));
      }

      Store &operator=(const Store &other)
      {
        if (&other!=this)
          memcpy(this, &other, sizeof(Store));
        return *this;
      }

      GLuint hash;
      bool colorSum;
      bool rescaleNormal;
      bool normalize;
      bool shadeModelFlat;
      bool lighting;
      bool lightModelLocalViewer;
      bool lightModelTwoSide;
      bool lightModelSeparateSpecular;
      bool colorMaterial;
      GLuint attrArrayFlags;
      ColorMaterialMode colorMaterialTarget0;
      ColorMaterialMode colorMaterialTarget1;
      AlphaTest alphaTest;
      Fog fog;
      Light light[ REGAL_FIXED_FUNCTION_MAX_LIGHTS ];
      Texture tex[ REGAL_EMU_IFF_TEXTURE_UNITS ];
      Clip clip[ REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ];
      GLuint64 ver;
    };

    struct StoreUniform {
      StoreUniform()
      : ver( 0 )
      {
        for (int ii=0; ii<REGAL_EMU_IFF_TEXTURE_UNITS; ii++) {
          tex[ii].texgen[0].obj = tex[ii].texgen[0].eye = Float4( 1, 0, 0, 0 );
          tex[ii].texgen[1].obj = tex[ii].texgen[1].eye = Float4( 0, 1, 0, 0 );
        }
        light[0].ambient = Float4( 0, 0, 0, 1 );
        light[0].diffuse = light[0].specular = Float4( 1, 1, 1, 1 );
        for (int ii=1; ii<REGAL_FIXED_FUNCTION_MAX_LIGHTS; ii++) {
          light[ii].ambient = light[ii].diffuse = light[ii].specular = Float4( 0, 0, 0, 1 );
        }
        lightModelAmbient = mat[0].ambient = mat[1].ambient = Float4( 0.2f, 0.2f, 0.2f, 1.0f );
        mat[0].diffuse = mat[1].diffuse = Float4( 0.8f, 0.8f, 0.8f, 1.0f );
      }

      TextureUniform tex[ REGAL_EMU_IFF_TEXTURE_UNITS ];
      AlphaTestUniform alphaTest;
      ClipUniform clip[ REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ];
      LightUniform light[ REGAL_FIXED_FUNCTION_MAX_LIGHTS ];
      MaterialUniform mat[ 2 ];
      FogUniform fog;
      Float4 lightModelAmbient;
      GLuint64 vabVer;
      GLuint64 ver;
    };

    State()                   : raw(), processed() {                          }
    State(const State &other) : raw(), processed() { UNUSED_PARAMETER(other); }
    ~State()                  {}

    Store raw;
    Store processed;
    StoreUniform uniform;

    bool SetEnable( Iff * ffn, bool enable, GLenum cap );

    void SetTexInfo( Version & ver, GLuint activeTex, TextureUnit & unit ) {
      if( activeTex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
        return;
      }
      raw.tex[ activeTex ].unit = unit;
      raw.ver = ver.Update();
    }

    void SetLight( Iff * ffn, GLenum light, GLenum pname, const GLfloat * params );
    void SetMaterial( Iff * ffn, GLenum face, GLenum pname, const GLfloat * params );
    void GetMaterial( Iff * ffn, GLenum face, GLenum pname, GLfloat * params );
    void SetTexgen( Iff * ffn, int coord, GLenum space, const GLfloat * params );
    void GetTexgen( Iff * ffn, int coord, GLenum space, GLfloat * params );
    void SetAlphaFunc( Iff * ffn, CompareFunc comp, GLfloat ref );
    void SetClip( Iff * ffn, GLenum plane, const GLfloat * equation );

    TextureTargetBitfield GetTextureEnable( int unit ) const {
      return TextureTargetBitfield( ( processed.tex[unit].enables ) );
    }

    GLuint64 Ver() const {
      return uniform.ver;
    }

    GLubyte HighestPriorityTextureEnable( GLubyte enables ) {
      for( int i = TP_CubeMap; i >= 0; i-- ) {
        if( enables & ( 1 << i ) ) {
          return static_cast<GLubyte>(1 << i);
        }
      }
      return 0;
    }
    void Process( Iff * ffn );
  };

  struct MatrixStack {
    struct El {
      El()
      : ver( 1 )
      {}
      r3::Matrix4f mat;
      GLuint64 ver;
    };
    MatrixStack() {
      stack.push_back( El() );
    }
    void Push() {
      RegalAssert( stack.size() < REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH );
      if( stack.size() < REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH ) {
        stack.push_back( stack.back() );
      }
    }

    void Pop() {
      /* The stack size warning messages ought to be the responsibility
       of the debug layer, rather than emulation.  (Opt-in only)
       Should emulation set an error in the underflow situation here? */

      /* RegalAssert( stack.size()>1 ); */
      if ( stack.size()>1 )
        stack.pop_back();
    }

    r3::Matrix4f &Top()       { RegalAssert( stack.size() ); return stack.back().mat; }
    const r3::Matrix4f &Top() const { RegalAssert( stack.size() ); return stack.back().mat; }

    GLuint64 &Ver()       { RegalAssert( stack.size() ); return stack.back().ver; }
    const GLuint64 &Ver() const { RegalAssert( stack.size() ); return stack.back().ver; }

    std::size_t size() const { return stack.size(); }

    El &operator[](const std::size_t i)       { RegalAssert( stack.size() ); return stack[i]; }
    const El &operator[](const std::size_t i) const { RegalAssert( stack.size() ); return stack[i]; }

  private:
    std::vector<El> stack;
  };

  // Iff::Program

  struct Program
  {

    // Iff::Program::UniformInfo

    struct UniformInfo
    {
      UniformInfo(const GLint s = -1, const GLuint64 v = 0)
      {
        memset(this, 0, sizeof(UniformInfo));
        slot = s;
        ver = v;
      }

      UniformInfo(const UniformInfo &other)
      {
        memcpy(this, &other, sizeof(UniformInfo));
      }

      UniformInfo &operator=(const UniformInfo &other)
      {
        if (&other!=this)
          memcpy(this, &other, sizeof(UniformInfo));
        return *this;
      }

      GLint    slot;
      GLuint64 ver;
    };

   // Iff::Program

    Program()
    : uniforms(),
      store()
    {
      // Clear plain-old-data (POD) memory
      memset(this, 0, reinterpret_cast<char *>(&this->uniforms)-reinterpret_cast<char *>(this));
    }

    Program(const Program &other)
    : uniforms(other.uniforms),
      store(other.store)
    {
      // Copy plain-old-data (POD) memory
      memcpy(this, &other, reinterpret_cast<char *>(&this->uniforms)-reinterpret_cast<char *>(this));
    }

    Program &operator=(const Program &other)
    {
      if (&other!=this)
      {
        // Copy plain-old-data (POD) memory
        memcpy(this, &other, reinterpret_cast<char *>(&this->uniforms)-reinterpret_cast<char *>(this));
        uniforms = other.uniforms;
        store    = other.store;
      }
      return *this;
    }

    // POD

    GLuint pg;
    GLuint vs, fs;
    GLuint64 ver;
    int progcount;

    // non-POD

    std::map< RegalFFUniformEnum, UniformInfo> uniforms;
    State::Store store;

    void Init( RegalContext * ctx, const State::Store & sstore, const GLchar *vsSrc, const GLchar *fsSrc );
    void Init( RegalContext * ctx, const State::Store & sstore );
    void Shader( RegalContext * ctx, DispatchTable & tbl, GLenum type, GLuint & shader, const GLchar *src );
    void Attribs( RegalContext * ctx );
    void Samplers( RegalContext * ctx, DispatchTable & tbl );
    void Uniforms( RegalContext * ctx, DispatchTable & tbl );
  };

  MatrixStack modelview;
  MatrixStack projection;
  MatrixStack texture[ REGAL_EMU_IFF_TEXTURE_UNITS ];

  GLenum shadowMatrixMode;
  TextureUnit textureUnit[ REGAL_EMU_IFF_TEXTURE_UNITS ];
  Float4 textureEnvColor[ REGAL_EMU_IFF_TEXTURE_UNITS ];
  GLuint64 textureEnvColorVer[ REGAL_EMU_IFF_TEXTURE_UNITS ];
  GLuint textureBinding[ REGAL_EMU_MAX_TEXTURE_UNITS];
  GLuint shadowActiveTextureIndex;
  GLuint activeTextureIndex;
  GLuint program;
  Program * currprog;

  MatrixStack *currMatrixStack;

  Version ver;
  State ffstate;

  Program  ffprogs[ 1 << REGAL_FIXED_FUNCTION_PROGRAM_CACHE_SIZE_BITS ];

  shared_map<GLuint, GLint> textureObjToFmt;

  // Program uniforms are tied to context state, so we cannot share IFF
  // programs, however we share user programs in general.
  std::map<GLenum, GLenum>  fmtmap;
  std::map<GLuint, GLenum>  shaderTypeMap;
  std::map<GLuint, Program> shprogmap;

  GLuint currVao;
  std::map<GLuint, GLuint> vaoAttrMap;

  bool gles;
  bool legacy; // 2.x mac

  void InitFixedFunction( RegalContext * ctx );

  void PreDraw( RegalContext * ctx ) {
    ver.Reset();
    if( program != 0 ) {
      UseShaderProgram( ctx );
    } else {
      UseFixedFunctionProgram( ctx );
    }
  }

  void SetCurrentMatrixStack( GLenum mode ) {
    switch( mode ) {
      case GL_MODELVIEW: currMatrixStack = &modelview; break;
      case GL_PROJECTION: currMatrixStack = &projection; break;
      case GL_TEXTURE:
        if( activeTextureIndex > GLuint( REGAL_EMU_IFF_TEXTURE_UNITS - 1 ) ) {
          break;
        }
        currMatrixStack = &texture[ activeTextureIndex ];
        break;
      case GL_TEXTURE0:
      case GL_TEXTURE1:
      case GL_TEXTURE2:
      case GL_TEXTURE3: {
        GLuint idx = mode - GL_TEXTURE0;
        if( idx > GLuint( REGAL_EMU_IFF_TEXTURE_UNITS - 1 ) ) {
          break;
        }
        currMatrixStack = &texture[ idx ];
        break;
      }
      default:
        RegalAssert( true && "WTF." );
        break;
    }
  }

  bool ShadowMatrixMode( GLenum mode ) {
    shadowMatrixMode = mode;
    return true;
  }

  bool ShadowActiveTexture( GLenum tex ) {
    shadowActiveTextureIndex = tex - GL_TEXTURE0;
    return false;
  }

  bool ShadowEnable( GLenum cap ) {
    return EnableIndexed( cap, shadowActiveTextureIndex );
  }

  bool ShadowDisable( GLenum cap ) {
    return DisableIndexed( cap, shadowActiveTextureIndex );
  }

  bool EnableIndexed( GLenum cap, GLuint index ) {
    activeTextureIndex = index;
    bool ret = ffstate.SetEnable( this, true, cap );
    return ret;
  }

  bool DisableIndexed( GLenum cap, GLuint index ) {
    activeTextureIndex = index;
    bool ret = ffstate.SetEnable( this, false, cap );
    return ret;
  }

  bool ShadowUseProgram( GLuint prog ) {
    program = prog;
    return prog == 0;  // pass the call along only if it's non-zero
  }

  void ShadowMultiTexBinding( GLenum texunit, GLenum target, GLuint obj );
  void ShadowTexBinding( GLenum target, GLuint obj ) {
    ShadowMultiTexBinding( GL_TEXTURE0 + shadowActiveTextureIndex, target, obj );
  }

  void ShadowTextureInfo( GLuint obj, GLenum target, GLint internalFormat );
  void ShadowMultiTexInfo( GLenum texunit, GLenum target, GLint internalFormat );
  void ShadowTexInfo( GLenum target, GLint internalFormat );

  void TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLfloat *v );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLint *v );
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, GLfloat v ) { TexEnv( texunit, target, pname, &v ); }
  void TexEnv( GLenum texunit, GLenum target, GLenum pname, GLint v ) { TexEnv( texunit, target, pname, &v ); }

  template <typename T>
  void TexEnv( GLenum target, GLenum pname, T v ) { TexEnv( GL_TEXTURE0 + shadowActiveTextureIndex, target, pname, v ); }

  template <typename T>
  void GetTexEnv( GLenum target, GLenum pname, T * params ) {
    if( target != GL_TEXTURE_ENV ) {
      return;
    }
    switch( pname ) {
      case GL_TEXTURE_ENV_MODE: {
        RegalAssert(activeTextureIndex<REGAL_EMU_IFF_TEXTURE_UNITS);
        *params =  static_cast<T>(texenvModeGL[ textureUnit[ activeTextureIndex ].env.mode ]);
        break;
      }
      case GL_TEXTURE_ENV_COLOR: {
        RegalAssert(activeTextureIndex<REGAL_EMU_IFF_TEXTURE_UNITS);
        Float4 & c = textureEnvColor[ activeTextureIndex ];
        params[0] = T( c.x );
        params[1] = T( c.y );
        params[2] = T( c.z );
        params[3] = T( c.w );
        break;
      }

      default:
        break;
    }
  }

  void ShadeModel( GLenum mode ) {
    State::Store & r = ffstate.raw;
    switch( mode ) {
      case GL_FLAT:
        if ( r.shadeModelFlat == false ) {
          r.shadeModelFlat = true;
          r.ver = ver.Update();
        }
        break;
      case GL_SMOOTH:
        if ( r.shadeModelFlat == true ) {
          r.shadeModelFlat = false;
          r.ver = ver.Update();
        }
        break;
    }
  }

  template <typename T> void Light( GLenum light, GLenum pname, const T param ) {
    int comp = 4;
    switch( pname ) {
      case GL_AMBIENT: case GL_DIFFUSE: case GL_SPECULAR: case GL_POSITION: break;
      case GL_SPOT_DIRECTION: comp = 3; break;
      case GL_SPOT_EXPONENT: case GL_SPOT_CUTOFF:
      case GL_CONSTANT_ATTENUATION: case GL_LINEAR_ATTENUATION:
      case GL_QUADRATIC_ATTENUATION: comp = 1; break;
      default: return;
    }
    GLfloat v[4];
    for( int i = 0; i < comp; i++ ) v[ i ] = RFFToFloatN( i, param );
    ffstate.SetLight( this, light, pname, v );
  }

  template <typename T> void Material( GLenum face, GLenum pname, const T param ) {
    int comp = 4;
    switch( pname ) {
      case GL_AMBIENT: case GL_DIFFUSE: case GL_SPECULAR:
      case GL_AMBIENT_AND_DIFFUSE: case GL_EMISSION: break;
      case GL_SHININESS: comp = 1; break;
      default: return;
    }
    GLfloat v[4];
    for( int i = 0; i < comp; i++ ) v[ i ] = RFFToFloatN( i, param );
    ffstate.SetMaterial( this, face, pname, v );
  }

  template <typename T> void GetMaterial( GLenum face, GLenum pname, T *params ) {
    int comp = 4;
    switch( pname ) {
      case GL_AMBIENT: case GL_DIFFUSE: case GL_SPECULAR:
      case GL_AMBIENT_AND_DIFFUSE: case GL_EMISSION: break;
      case GL_SHININESS: comp = 1; break;
      default: return;
    }
    GLfloat v[4];
    ffstate.GetMaterial( this, face, pname, v );
    for( int i = 0; i < comp; i++ ) params[i] = static_cast<T>(v[i]);
  }

  template <typename T> void LightModel( GLenum pname, const T param ) {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;
    switch( pname ) {
      case GL_LIGHT_MODEL_AMBIENT: {
        GLfloat *v = & u.lightModelAmbient.x;
        for( int i = 0; i < 4; i++ ) v[i] = RFFToFloatN( i, param );
        break;
      }
      case GL_LIGHT_MODEL_LOCAL_VIEWER: r.lightModelLocalViewer = RFFToFloat( 0, param ) != 0.0f; break;
      case GL_LIGHT_MODEL_TWO_SIDE: r.lightModelTwoSide = RFFToFloat( 0, param ) != 0.0f; break;
      case GL_LIGHT_MODEL_COLOR_CONTROL: r.lightModelSeparateSpecular = RFFToFloat(0, param) == GL_SEPARATE_SPECULAR_COLOR; break;
      default: return;
    }
    u.ver = r.ver = ver.Update();
  }

  void ColorMaterial( GLenum face, GLenum mode ) {
    ColorMaterialMode m;
    switch( mode ) {
      case GL_EMISSION: m = CM_Emission; break;
      case GL_AMBIENT: m = CM_Ambient; break;
      case GL_DIFFUSE: m = CM_Diffuse; break;
      case GL_SPECULAR: m = CM_Specular; break;
      case GL_AMBIENT_AND_DIFFUSE: m = CM_AmbientAndDiffuse; break;
      default: return;
    }
    switch( face ) {
      case GL_FRONT:
        ffstate.raw.colorMaterialTarget0 = m;
        ffstate.raw.colorMaterialTarget1 = CM_None;
        break;
      case GL_BACK:
        ffstate.raw.colorMaterialTarget0 = CM_None;
        ffstate.raw.colorMaterialTarget1 = m;
        break;
      case GL_FRONT_AND_BACK:
        ffstate.raw.colorMaterialTarget0 = m;
        ffstate.raw.colorMaterialTarget1 = m;
        break;
      default:
        return;
    }
    ffstate.raw.ver = ver.Update();
  }

  template <typename T> void TexGen( GLenum coord, GLenum pname, const T param ) {
    RegalAssert(activeTextureIndex < REGAL_EMU_IFF_TEXTURE_UNITS);
    if( activeTextureIndex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
      return;
    }
    State::Store & st = ffstate.raw;
    int idx = 0;
    switch( coord ) {
      case GL_S: case GL_T: case GL_R: case GL_Q: idx = coord - GL_S; break;
      default:
        RegalAssert(coord==GL_S || coord==GL_T || coord==GL_R || coord==GL_Q || coord==GL_S);
        return;
    }
    switch( pname ) {
      case GL_TEXTURE_GEN_MODE: {
        State::Texgen & tg = st.tex[ activeTextureIndex ].texgen[ idx ];
        switch( GLenum( RFFToFloat( 0, param ) ) ) {
          case GL_OBJECT_LINEAR: tg.mode = TG_ObjectLinear; break;
          case GL_EYE_LINEAR: tg.mode = TG_EyeLinear; break;
          case GL_SPHERE_MAP: tg.mode = TG_SphereMap; break;
          case GL_NORMAL_MAP: tg.mode = TG_NormalMap; break;
          case GL_REFLECTION_MAP: tg.mode = TG_ReflectionMap; break;
          default:
            RegalAssert(0);
            return;
        }
        break;
      }
      case GL_EYE_PLANE:
      case GL_OBJECT_PLANE: {
        if (!RFFIsVector( param ))
        {
          RegalAssert(0);
          return;
        }
        Float4 plane;
        plane.x = RFFToFloat( 0, param );
        plane.y = RFFToFloat( 1, param );
        plane.z = RFFToFloat( 2, param );
        plane.w = RFFToFloat( 3, param );
        ffstate.SetTexgen( this, idx, pname, & plane.x );
        return;
      }
      default:
        RegalAssert(0);
        return;

    }
    ffstate.uniform.ver = st.ver = ver.Update();
  }

  void AlphaFunc( GLenum comp, const GLfloat ref ) {
    CompareFunc cf = CF_Always;
    switch( comp ) {
      case GL_NEVER:    cf = CF_Never; break;
      case GL_LESS:     cf = CF_Less; break;
      case GL_EQUAL:    cf = CF_Equal; break;
      case GL_LEQUAL:   cf = CF_Lequal; break;
      case GL_NOTEQUAL: cf = CF_NotEqual; break;
      case GL_GREATER:  cf = CF_Greater; break;
      case GL_GEQUAL:   cf = CF_Gequal; break;
      case GL_ALWAYS:   cf = CF_Always; break;
      default: break; // should be an error...
    }
    ffstate.SetAlphaFunc( this, cf, ref );
  }

  void ClipPlane( GLenum plane, const GLdouble * equation ) {
    Float4 eqn( equation[0], equation[1], equation[2], equation[3] );
    ffstate.SetClip( this, plane, & eqn.x );
  }

  template <typename T> void Fog( GLenum pname, const T param ) {
    State::Store & r = ffstate.raw;
    State::StoreUniform & u = ffstate.uniform;
    switch( pname ) {
      case GL_FOG_MODE: {
        FogMode m = FG_Exp;
        switch( GLenum( RFFToFloat( 0, param ) ) ) {
          case GL_LINEAR: m = FG_Linear; break;
          case GL_EXP: m = FG_Exp; break;
          case GL_EXP2: m = FG_Exp2; break;
          default: return;
        }
        r.fog.mode = m;
        break;
      }
      case GL_FOG_DENSITY: {
        u.fog.params[0].x = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      }
      case GL_FOG_START: {
        u.fog.params[0].y = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      }
      case GL_FOG_END: {
        u.fog.params[0].z = RFFToFloat( 0, param );
        u.fog.ver = ver.Update();
        break;
      }
      case GL_FOG_COLOR: {
        u.fog.params[1].x = RFFToFloat( 0, param );
        u.fog.params[1].y = RFFToFloat( 1, param );
        u.fog.params[1].z = RFFToFloat( 2, param );
        u.fog.params[1].w = RFFToFloat( 3, param );
        u.fog.ver = ver.Update();
        break;
      }
      case GL_FOG_COORD_SRC: {
        bool d = true;
        switch( GLenum( RFFToFloat( 0, param ) ) ) {
          case GL_FRAGMENT_DEPTH: d = true; break;
          case GL_FOG_COORD: d = false; break;
          default: return;
        }
        r.fog.useDepth = d;
        break;
      }
      default: return;
    }
    u.ver = r.ver = ver.Update();
  }

  template <typename T> bool GetIndexedTexGenv( RegalContext * ctx, GLuint textureIndex,
                                               GLenum coord, GLenum pname, T * params )
  {
    UNUSED_PARAMETER(ctx);

    if( textureIndex >= REGAL_EMU_IFF_TEXTURE_UNITS ) {
      return false;
    }
    int idx = 0;
    switch( coord ) {
      case GL_S: case GL_T: case GL_R: case GL_Q: idx = coord - GL_S; break;
      default: return false;
    }
    switch( pname )
    {
      case GL_TEXTURE_GEN_MODE:
      {
        GLenum glmode = GL_EYE_LINEAR;
        switch ( ffstate.raw.tex[ textureIndex ].texgen[ idx ].mode )
        {
          case TG_ObjectLinear:
            glmode = GL_OBJECT_LINEAR;
            break;
          case TG_EyeLinear:
            glmode = GL_EYE_LINEAR;
            break;
          case TG_SphereMap:
            glmode = GL_SPHERE_MAP;
            break;
          case TG_NormalMap:
            glmode = GL_NORMAL_MAP;
            break;
          case TG_ReflectionMap:
            glmode = GL_REFLECTION_MAP;
            break;
          default:
            return false;
        }
        *params = static_cast<T>(glmode);
        break;
      }
      case GL_OBJECT_PLANE:
      case GL_EYE_PLANE:
      {
        Float4 plane;
        ffstate.GetTexgen( this, idx, pname, & plane.x );
        *(params+0) = static_cast<T>(plane.x);
        *(params+1) = static_cast<T>(plane.y);
        *(params+2) = static_cast<T>(plane.z);
        *(params+3) = static_cast<T>(plane.w);
        break;
      }
      default:
        return false;
    }
    return true;
  }

  template <typename T> bool GetTexGenv( RegalContext * ctx, GLenum coord,
                                        GLenum pname, T * params )
  {
    return GetIndexedTexGenv( ctx, shadowActiveTextureIndex, coord, pname, params );
  }

  template <typename T> bool GetMultiTexGenv( RegalContext * ctx, GLenum texunit,
                                             GLenum coord, GLenum pname, T * params )
  {
    return GetIndexedTexGenv( ctx, texunit - GL_TEXTURE0, coord, pname, params );
  }

  template <typename T> bool Get( RegalContext * ctx, GLenum pname, T * params )
  {
    // FIXME: implement all FF gets!
    if( VaGet( ctx, pname, params ) )
      return true;

    switch( pname ) {
      case GL_CURRENT_PROGRAM:
        *params = static_cast<T>(program);
        break;
      case GL_MAX_MODELVIEW_STACK_DEPTH:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH);
        break;
      case GL_MAX_PROJECTION_STACK_DEPTH:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH);
        break;
      case GL_MAX_TEXTURE_STACK_DEPTH:
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MATRIX_STACK_DEPTH);
        break;
      case GL_SMOOTH_POINT_SIZE_RANGE:
        // FIXME: Pass through actual GL's limit.
        *params = static_cast<T>(1);
        break;
      case GL_SMOOTH_LINE_WIDTH_RANGE:
        // FIXME: Pass through actual GL's limit.
        *params = static_cast<T>(1);
        break;
      case GL_MODELVIEW_STACK_DEPTH:
        *params = static_cast<T>(modelview.size());
        break;
      case GL_PROJECTION_STACK_DEPTH:
        *params = static_cast<T>(projection.size());
        break;
      case GL_TEXTURE_STACK_DEPTH:
        RegalAssert(activeTextureIndex<REGAL_EMU_IFF_TEXTURE_UNITS);
        *params = static_cast<T>(texture[activeTextureIndex].size());
        break;
      case GL_MODELVIEW_MATRIX: {
        const GLfloat * p = modelview.Top().Ptr();
        for( int i = 0; i < 16; i++ ) params[i] = static_cast<T>(p[i]);
        break;
      }
      case GL_PROJECTION_MATRIX: {
        const GLfloat * p = projection.Top().Ptr();
        for( int i = 0; i < 16; i++ ) params[i] = static_cast<T>(p[i]);
        break;
      }
      case GL_TEXTURE_MATRIX: {
        RegalAssert(activeTextureIndex<REGAL_EMU_IFF_TEXTURE_UNITS);
        const GLfloat * p = texture[ activeTextureIndex ].Top().Ptr();
        for( int i = 0; i < 16; i++ ) params[i] = static_cast<T>(p[i]);
        break;
      }
      case GL_MATRIX_MODE: {
        *params = static_cast<T>(shadowMatrixMode);
        break;
      }
      case GL_MAX_LIGHTS: {
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MAX_LIGHTS);
        break;
      }
      case GL_MAX_TEXTURE_UNITS: {
        *params = static_cast<T>(REGAL_EMU_IFF_TEXTURE_UNITS);
        break;
      }
      case GL_MAX_CLIP_PLANES: {
        *params = static_cast<T>(REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES);
        break;
      }
      default:
        return false;
    }
    return true;
  }

  void MatrixPush( GLenum mode ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Push();
  }

  void MatrixPop( GLenum mode ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Pop();
    UpdateMatrixVer();
  }

  void UpdateMatrixVer() {
    currMatrixStack->Ver() = ffstate.uniform.ver = ver.Update();
    if( currMatrixStack != &modelview && currMatrixStack != &projection ) {
      ffstate.raw.ver = ffstate.uniform.ver;
    }
  }

  void MatrixLoadIdentity( GLenum mode ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MakeIdentity();
    UpdateMatrixVer();
  }

  void MatrixLoad( GLenum mode, const r3::Matrix4f & m ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top() = m;
    UpdateMatrixVer();
  }

  void MatrixLoadTranspose( GLenum mode, const r3::Matrix4f & m ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top() = m.Transpose();
    UpdateMatrixVer();
  }

  void MatrixMult( GLenum mode, const r3::Matrix4f & m ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( m );
    UpdateMatrixVer();
  }

  void MatrixMultTranspose( GLenum mode, const r3::Matrix4f & m ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( m.Transpose() );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixRotate( GLenum mode, T angle, T x, T y, T z ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Rotationf( r3::Vec3f( x, y, z ), r3::ToRadians( static_cast<float>(angle) ) ).GetMatrix4() );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixTranslate( GLenum mode, T x, T y, T z ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Matrix4f::Translate( r3::Vec3f( x, y, z ) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixScale( GLenum mode, T x, T y, T z ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Matrix4f::Scale( r3::Vec3f( x, y, z ) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixFrustum( GLenum mode, T left, T right, T bottom, T top, T zNear, T zFar ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Frustum<float>( static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom), static_cast<float>(top), static_cast<float>(zNear), static_cast<float>(zFar) ) );
    UpdateMatrixVer();
  }

  template <typename T> void MatrixOrtho( GLenum mode, T left, T right, T bottom, T top, T zNear, T zFar ) {
    SetCurrentMatrixStack( mode );
    currMatrixStack->Top().MultRight( r3::Ortho<float>( static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom), static_cast<float>(top), static_cast<float>(zNear), static_cast<float>(zFar) ) );
    UpdateMatrixVer();
  }

  void PushMatrix() { MatrixPush( shadowMatrixMode ); }
  void PopMatrix() { MatrixPop( shadowMatrixMode ); }
  void LoadIdentity() { MatrixLoadIdentity( shadowMatrixMode ); }
  void LoadMatrix( const r3::Matrix4f & m ) { MatrixLoad( shadowMatrixMode, m ); }
  void LoadTransposeMatrix( const r3::Matrix4f & m ) { MatrixLoadTranspose( shadowMatrixMode, m ); }
  void MultMatrix( const r3::Matrix4f & m ) { MatrixMult( shadowMatrixMode, m ); }
  void MultTransposeMatrix( const r3::Matrix4f & m ) { MatrixMultTranspose( shadowMatrixMode, m ); }
  template <typename T> void Rotate( T angle, T x, T y, T z ) { MatrixRotate( shadowMatrixMode, angle, x, y, z ); }
  template <typename T> void Translate( T x, T y, T z ) { MatrixTranslate( shadowMatrixMode, x, y, z ); }
  template <typename T> void Scale( T x, T y, T z ) { MatrixScale( shadowMatrixMode, x, y, z ); }
  template <typename T> void Frustum( T left, T right, T bottom, T top, T zNear, T zFar ) { MatrixFrustum( shadowMatrixMode, left, right, bottom, top, zNear, zFar ); }
  template <typename T> void Ortho( T left, T right, T bottom, T top, T zNear, T zFar ) { MatrixOrtho( shadowMatrixMode, left, right, bottom, top, zNear, zFar ); }


  void BindVertexArray( RegalContext * ctx, GLuint vao ) {
    UNUSED_PARAMETER(ctx);
    vaoAttrMap[ currVao ] = ffstate.raw.attrArrayFlags;
    currVao = vao;
    ffstate.raw.attrArrayFlags = vaoAttrMap[ currVao ];
    ffstate.uniform.vabVer = ver.Update();
  }

  void EnableArray( RegalContext * ctx, GLuint index ) {
    RestoreVao( ctx );
    ffstate.raw.attrArrayFlags |= 1 << index;
    ffstate.raw.ver = ffstate.uniform.vabVer = ver.Update();
  }

  void DisableArray( RegalContext * ctx, GLuint index ) {
    RestoreVao( ctx );
    ffstate.raw.attrArrayFlags &= ~( 1 << index );
    ffstate.raw.ver = ffstate.uniform.vabVer = ver.Update();
  }

  void UpdateUniforms( RegalContext * ctx );
  void UseFixedFunctionProgram( RegalContext * ctx );
  void UseShaderProgram( RegalContext * ctx );

  void ShaderSource( RegalContext *ctx, GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
  void LinkProgram( RegalContext *ctx, GLuint program );

  GLuint CreateShader( RegalContext *ctx, GLenum shaderType ) {
    GLuint sh = ctx->dispatcher.emulation.glCreateShader( shaderType );
    shaderTypeMap[ sh ] = shaderType;
    return sh;
  }

  //
  void Init( RegalContext &ctx )
  {
    shadowMatrixMode = 0;
    shadowActiveTextureIndex = 0;
    activeTextureIndex = 0;
    program = 0;
    currprog = NULL;
    currVao = 0;
    gles = false;
    legacy = false;

    RegalContext *sharingWith = ctx.groupInitializedContext();
    if (sharingWith)
      textureObjToFmt = sharingWith->iff->textureObjToFmt;

    InitVertexArray( &ctx );
    InitFixedFunction( &ctx );
    InitImmediate( &ctx );
  }
};

}; // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // __REGAL_FIXED_FUNCTION_H__
