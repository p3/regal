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

 Regal Vertex Array Object implementation
 Cass Everitt

 */

#ifndef __REGAL_VAO_H__
#define __REGAL_VAO_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>

#include "RegalEmu.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalSharedMap.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu
{

struct Vao
{

  struct Array
  {

    Array()
      : enabled( GL_FALSE )
      , buffer( 0 )
      , size( 4 )
      , type( GL_FLOAT )
      , normalized( GL_FALSE )
      , integer( GL_FALSE )
      , stride( 0 )
      , pointer( NULL )
    {}

    GLboolean enabled;
    GLuint buffer;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLboolean integer;
    GLsizei stride;
    const GLvoid *pointer;
  };

  struct Object
  {
    Object() : vertexBuffer( 0 ), indexBuffer( 0 ) {}
    GLuint vertexBuffer;
    GLuint indexBuffer;
    Array a[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  };

  shared_map<GLuint, Object> objects;

  GLenum clientActiveTexture;

  GLuint current;
  GLuint enables;
  Object *currObject;

  GLuint coreVao;
  GLuint maxName;

  // to alias vertex arrays to generic attribs
  GLuint ffAttrMap[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  GLuint ffAttrInvMap[ REGAL_EMU_MAX_VERTEX_ATTRIBS ];
  GLuint ffAttrTexBegin;
  GLuint ffAttrTexEnd;
  GLuint ffAttrNumTex;
  GLuint max_vertex_attribs;

  void Init( RegalContext &ctx )
  {
    maxName = 0;
    clientActiveTexture = GL_TEXTURE0;

    max_vertex_attribs = ctx.info->gl_max_vertex_attribs;
    RegalAssert( max_vertex_attribs <= REGAL_EMU_MAX_VERTEX_ATTRIBS );
    if (max_vertex_attribs > REGAL_EMU_MAX_VERTEX_ATTRIBS)
      max_vertex_attribs = REGAL_EMU_MAX_VERTEX_ATTRIBS;

    RegalContext *sharingWith = ctx.groupInitializedContext();
    if (sharingWith)
      objects = sharingWith->vao->objects;

    // we have RFF2A maps for sets of 8 and 16 attributes. if
    // REGAL_EMU_MAX_VERTEX_ATTRIBS > 16 a new map needs to be added

    RegalAssert( REGAL_EMU_MAX_VERTEX_ATTRIBS <= 16 );

    if ( max_vertex_attribs >= 16 )
    {
      RegalAssert( REGAL_EMU_MAX_VERTEX_ATTRIBS == 16);
      //RegalOutput( "Setting up for %d Vertex Attribs\n", max_vertex_attribs );
      for( int i = 0; i < 16; i++ )
      {
        ffAttrMap[i] = RFF2AMap16[i];
        ffAttrInvMap[i] = RFF2AInvMap16[i];
      }
      ffAttrTexBegin = RFF2ATexBegin16;
      ffAttrTexEnd = RFF2ATexEnd16;
      if (max_vertex_attribs > 16)
        max_vertex_attribs = 16;
    }
    else
    {
      RegalAssert( max_vertex_attribs >= 8 );
      //RegalOutput( "Setting up for 8 Vertex Attribs" );
      for( int i = 0; i < 8; i++ )
      {
        ffAttrMap[i] = RFF2AMap8[i];
        ffAttrInvMap[i] = RFF2AInvMap8[i];
      }
      for( int i = 8; i < REGAL_EMU_MAX_VERTEX_ATTRIBS; i++ )
      {
        ffAttrMap[i] = GLuint(-1);
        ffAttrInvMap[i] = GLuint(-1);
      }
      ffAttrTexBegin = RFF2ATexBegin8;
      ffAttrTexEnd = RFF2ATexEnd8;
      if (max_vertex_attribs > 8)
        max_vertex_attribs = 8;
    }
    ffAttrNumTex = ffAttrTexEnd - ffAttrTexBegin;

    if( ctx.info->core )
    {
      maxName = 1;
      ctx.dispatcher.driver.glGenVertexArrays( 1, & coreVao );
      RegalAssert( coreVao != 0 );
      ctx.dispatcher.driver.glBindVertexArray( coreVao );
    }
    else
      coreVao = 0;

    current = 9999999; // this is only to force the bind...
    currObject = NULL;
    BindVertexArray( &ctx, 0 );
  }

  void Cleanup( RegalContext &ctx )
  {
    UNUSED_PARAMETER(ctx);
  }

  void ShadowBufferBinding( GLenum target, GLuint bufferBinding )
  {
    RegalAssert( currObject != NULL );
    if( target == GL_ARRAY_BUFFER )
    {
      currObject->vertexBuffer = bufferBinding;
    }
    else if( target == GL_ELEMENT_ARRAY_BUFFER )
    {
      currObject->indexBuffer = bufferBinding;
    }
  }

  void BindVertexArray( RegalContext *ctx, GLuint name )
  {
    if( name == current )
    {
      return;
    }

    current = name;

    Object &vao = objects[current];  // force VAO construction
    currObject = & vao;
    if( maxName < current )
    {
      maxName = current;
    }
    DispatchTableGL &tbl = ctx->dispatcher.emulation;
    tbl.glBindBuffer( GL_ARRAY_BUFFER, vao.vertexBuffer );
    tbl.glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vao.indexBuffer );
    GLuint lastBuffer = vao.vertexBuffer;
    RegalAssert( max_vertex_attribs <= REGAL_EMU_MAX_VERTEX_ATTRIBS );
    for( GLuint i = 0; i < max_vertex_attribs; i++ )
    {
      Array &a = vao.a[ i ];
      if( a.buffer != lastBuffer )
      {
        tbl.glBindBuffer( GL_ARRAY_BUFFER, a.buffer );
        lastBuffer = a.buffer;
      }

      EnableDisableVertexAttribArray( ctx, a.enabled, i );
      if( a.pointer || a.buffer )
      {
        tbl.glVertexAttribPointer( i, a.size, a.type, a.normalized, a.stride, a.pointer );
      }
    }
    if( lastBuffer != vao.vertexBuffer )
    {
      tbl.glBindBuffer( GL_ARRAY_BUFFER, vao.vertexBuffer );
    }
  }

  void GenVertexArrays( GLsizei n, GLuint *arrays )
  {
    if( maxName < 0x80000000 )   // fast gen for sequential allocation
    {
      for( GLsizei i = 0; i < n; i++ )
      {
        arrays[ i ] = ++maxName;
        objects[ maxName ]; // gen allocates
      }
    }
    else     // otherwise plod through
    {
      GLsizei i = 0;
      GLuint name = 1;
      while( i < n )
      {
        if( objects.count( name ) == 0 )
        {
          arrays[ i++ ] = name;
          objects[ name ]; // gen allocates
        }
        name++;
      }
    }
  }

  void DeleteVertexArrays( GLsizei n, const GLuint *arrays )
  {
    for( GLsizei i = 0; i < n; i++ )
    {
      GLuint name = arrays[ i ];
      if( name != coreVao && objects.count( name ) > 0 )
      {
        objects.erase( name );
      }
    }
  }

  GLboolean IsVertexArray( GLuint name )
  {
    return objects.count( name ) > 0 ? GL_TRUE : GL_FALSE;
  }

  void EnableDisableVertexAttribArray( RegalContext *ctx, GLboolean enable, GLuint index )
  {
    RegalAssert( index < REGAL_EMU_MAX_VERTEX_ATTRIBS );
    RegalAssert( index < max_vertex_attribs );
    if (index >= max_vertex_attribs || index >= REGAL_EMU_MAX_VERTEX_ATTRIBS)
      return;

    DispatchTableGL &tbl = ctx->dispatcher.emulation;
    Array &a = objects[current].a[index];
    a.enabled = enable;
    if( a.enabled == GL_TRUE )
    {
      tbl.glEnableVertexAttribArray( index );
      enables |= 1 << index;
    }
    else
    {
      tbl.glDisableVertexAttribArray( index );
      enables &= ~( 1 << index );
    }
  }

  void EnableVertexAttribArray( RegalContext *ctx, GLuint index )
  {
    EnableDisableVertexAttribArray( ctx, GL_TRUE, index );
  }

  void DisableVertexAttribArray( RegalContext *ctx, GLuint index )
  {
    EnableDisableVertexAttribArray( ctx, GL_FALSE, index );
  }

  void AttribPointer( RegalContext *ctx, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer )
  {
    // do nothing for these various error conditions

    if ( ctx->depthBeginEnd )
      return;

    RegalAssert( index < REGAL_EMU_MAX_VERTEX_ATTRIBS );
    RegalAssert( index < max_vertex_attribs );
    if (index >= max_vertex_attribs || index >= REGAL_EMU_MAX_VERTEX_ATTRIBS )
      return;

    switch (size)
    {
      case 1:
      case 2:
      case 3:
      case 4:
        break;
      case GL_BGRA:
        switch (type)
        {
          case GL_UNSIGNED_BYTE:
          case GL_INT_2_10_10_10_REV:
          case GL_UNSIGNED_INT_2_10_10_10_REV:
            break;
          default:
            return;
        }
        break;
      default:
        return;
    }

    switch (type)
    {
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
      case GL_HALF_FLOAT:
      case GL_FLOAT:
      case GL_DOUBLE:
        break;
      case GL_INT_2_10_10_10_REV:
      case GL_UNSIGNED_INT_2_10_10_10_REV:
        if (size != 4 && size != GL_BGRA)
          return;
        break;
      default:
        RegalAssert( "Unknown <type> in for attrib." );
        return;
    }

    if (size == GL_BGRA && normalized == GL_FALSE)
    {
      return;
    }

    if (stride < 0)
    {
      return;
    }

    //<> if ( ( currentClientState.vertexArrayState.vertexArrayObject != 0 ) &&
    //<> ( currentClientState.vertexArrayState.vertexBuffer == 0 ) &&
    //<> ( pointer != NULL ) ) {
    //<> return;
    //<> }

    RegalAssert( index < max_vertex_attribs );
    RegalAssert( index < REGAL_EMU_MAX_VERTEX_ATTRIBS );
    Array &a = objects[current].a[index];

    RegalAssert( currObject != NULL );
    a.buffer = currObject->vertexBuffer;
    a.size = size;
    a.type = type;
    a.normalized = normalized;
    a.stride = stride;
    a.pointer = pointer;

    RegalAssert( a.buffer == 0 || GLuint64( a.pointer ) < ( 1 << 22 ) );

    ctx->dispatcher.emulation.glVertexAttribPointer( index, size, type, normalized, stride, pointer );
  }

  void Validate( RegalContext *ctx )
  {
    UNUSED_PARAMETER(ctx);
    RegalAssert( currObject != NULL );
    RegalAssert( max_vertex_attribs <= REGAL_EMU_MAX_VERTEX_ATTRIBS );
    for( GLuint i = 0; i < max_vertex_attribs; i++ )
    {
#if !REGAL_NO_ASSERT
      const Array &a = currObject->a[ i ];
      RegalAssert( !( a.enabled && a.buffer == 0 && GLuint64( a.pointer ) < 1024 ) );
#endif
    }
  }

  template <typename T> void GetAttrib( GLint index, GLenum pname, T *params )
  {
    if (index >= REGAL_EMU_MAX_VERTEX_ATTRIBS)
      return;

    Array &a = objects[current].a[index];
    switch( pname )
    {
      case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
        *params = static_cast<T>(a.enabled);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
        *params = static_cast<T>(a.buffer);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        *params = static_cast<T>(a.size);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        *params = static_cast<T>(a.type);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
        *params = static_cast<T>(a.normalized);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        *params = static_cast<T>(a.stride);
        break;
      case GL_VERTEX_ATTRIB_ARRAY_DIVISOR:
        *params = static_cast<T>(0);
      default:
        break;
    }
  }

  template <typename T> void GetAttrib( GLint index, GLenum pname, T **params )
  {
    if (index >= REGAL_EMU_MAX_VERTEX_ATTRIBS)
      return;

    Array &a = objects[current].a[index];
    switch( pname )
    {
      case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        *params = const_cast<T *>(a.pointer);
        break;
      default:
        break;
    }
  }

  template <typename T> bool Get( GLenum pname, T *params )
  {

    switch (pname)
    {
      case GL_VERTEX_ARRAY_BINDING:
        *params = static_cast<T>( current );
        break;

      case GL_ARRAY_BUFFER_BINDING:
        *params = static_cast<T>(currObject->vertexBuffer);
        break;

      case GL_ELEMENT_ARRAY_BUFFER_BINDING:
        *params = static_cast<T>(currObject->indexBuffer);
        break;

      default:
        return false;
    }
    return true;
  }

  bool GetVertexAttribPointerv( GLuint index, GLenum pname, GLvoid **pointer)
  {
    if (index >= REGAL_EMU_MAX_VERTEX_ATTRIBS)
      return false;

    if ( pname != GL_VERTEX_ATTRIB_ARRAY_POINTER )
      return false;

    Array &a = objects[current].a[index];
    switch( pname )
    {
      case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        *pointer = const_cast<GLvoid *>(a.pointer);
        break;
      default:
        return false;
    }
    return true;
  }

  GLuint ClientStateToAttribIndex( GLenum array )
  {
    switch ( array )
    {
      case GL_VERTEX_ARRAY:
        return ffAttrMap[ RFF2A_Vertex ];
      case GL_NORMAL_ARRAY:
        return ffAttrMap[ RFF2A_Normal ];
      case GL_COLOR_ARRAY:
        return ffAttrMap[ RFF2A_Color ];
      case GL_SECONDARY_COLOR_ARRAY:
        return ffAttrMap[ RFF2A_SecondaryColor ];
      case GL_FOG_COORD_ARRAY:
        return ffAttrMap[ RFF2A_FogCoord ];
      case GL_TEXTURE_COORD_ARRAY:
      {
        GLuint index = clientActiveTexture - GL_TEXTURE0;
        RegalAssert(index < REGAL_EMU_MAX_VERTEX_ATTRIBS);
        if ( index >= ffAttrNumTex )
        {
          Warning("Texture unit out of range: ", index, " >= ", ffAttrNumTex, ". Clamping to supported maximum.");
          index = ffAttrNumTex - 1;
        }
        return ffAttrTexBegin + index;
      }
      break;
      default:
        break;
    }
    return GLuint(~0);
  }

  void ShadowVertexArrayPointer( RegalContext *ctx, GLenum array, GLint size,
                                 GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    //<> if ( ( currentClientState.vertexArrayState.vertexArrayObject != 0 ) &&
    //<> ( currentClientState.vertexArrayState.vertexBuffer == 0 ) &&
    //<> ( pointer != NULL ) ) {
    //<> return;
    //<> }

    GLuint index = ClientStateToAttribIndex( array );
    AttribPointer( ctx, index, size, type, GL_TRUE, stride, pointer );
  }

  void ColorPointer(RegalContext *ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    switch (size)
    {
      case 3:
      case 4:
        break;
      default:
        return;
    }

    switch (type)
    {
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
      case GL_FLOAT:
      case GL_DOUBLE:
        break;
      default:
        return;
    }

    if (stride < 0)
      return;

    ShadowVertexArrayPointer(ctx, GL_COLOR_ARRAY, size, type, stride, pointer);
  }

  void SecondaryColorPointer(RegalContext *ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    if (size != 3)
      return;

    switch (type)
    {
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
      case GL_FLOAT:
      case GL_DOUBLE:
        break;
      default:
        return;
    }

    if (stride < 0)
      return;

    ShadowVertexArrayPointer(ctx, GL_SECONDARY_COLOR_ARRAY, size, type, stride, pointer);
  }

  void TexCoordPointer(RegalContext *ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    switch (size)
    {
      case 1:
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

    ShadowVertexArrayPointer(ctx, GL_TEXTURE_COORD_ARRAY, size, type, stride, pointer);
  }

  void VertexPointer(RegalContext *ctx, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

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

    ShadowVertexArrayPointer(ctx, GL_VERTEX_ARRAY, size, type, stride, pointer);
  }

  void NormalPointer(RegalContext *ctx, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    switch (type)
    {
      case GL_BYTE:
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

    ShadowVertexArrayPointer(ctx, GL_NORMAL_ARRAY, 0, type, stride, pointer);
  }

  void FogCoordPointer(RegalContext *ctx, GLenum type, GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    switch (type)
    {
      case GL_FLOAT:
      case GL_DOUBLE:
        break;
      default:
        return;
    }

    if (stride < 0)
      return;

    ShadowVertexArrayPointer(ctx, GL_FOG_COORD_ARRAY, 0, type, stride, pointer);
  }

  void ClientActiveTexture( RegalContext *ctx, GLenum _texture)
  {
    UNUSED_PARAMETER(ctx);
    if ( ctx->depthBeginEnd )
      return;

    GLint index = _texture - GL_TEXTURE0;

    if (index >= 0 && index < REGAL_EMU_MAX_VERTEX_ATTRIBS)
      clientActiveTexture = _texture;
  }

  void ShadowEnableDisableClientState( RegalContext *ctx, GLenum array, GLboolean enable )
  {
    GLuint index = ClientStateToAttribIndex( array );
    EnableDisableVertexAttribArray( ctx, enable, index );
  }

  void EnableClientState( RegalContext *ctx, GLenum array )
  {
    if ( ctx->depthBeginEnd )
      return;

    ShadowEnableDisableClientState( ctx, array, GL_TRUE );
  }

  void DisableClientState( RegalContext *ctx, GLenum array )
  {
    if ( ctx->depthBeginEnd )
      return;

    ShadowEnableDisableClientState( ctx, array, GL_FALSE );
  }

  void InterleavedArrays( RegalContext *ctx, GLenum format,
                          GLsizei stride, const GLvoid *pointer)
  {
    if ( ctx->depthBeginEnd )
      return;

    //<> how can stride be invalid?
    //<> if ( stride is invalid )
    //<> return;

    switch ( format )
    {
      case GL_V2F:
      case GL_V3F:
      case GL_C4UB_V2F:
      case GL_C4UB_V3F:
      case GL_C3F_V3F:
      case GL_N3F_V3F:
      case GL_C4F_N3F_V3F:
      case GL_T2F_V3F:
      case GL_T4F_V4F:
      case GL_T2F_C4UB_V3F:
      case GL_T2F_C3F_V3F:
      case GL_T2F_N3F_V3F:
      case GL_T2F_C4F_N3F_V3F:
      case GL_T4F_C4F_N3F_V4F:
        break;
      default:
        RegalAssert( !"unhandled format value" );
        return;
    }

    GLsizei f = sizeof(GL_FLOAT);
    GLsizei c = 4 * sizeof(GL_UNSIGNED_BYTE);
    //<> need to round up c to the nearest multiple of f

    GLsizei pc = 0;
    GLsizei pn = 0;
    GLsizei pv = 0;
    GLsizei s  = 0;

    switch ( format )
    {
      case GL_V2F:
        pc = 0;
        pn = 0;
        pv = 0;
        s  = 2 * f;
        break;
      case GL_V3F:
        pc = 0;
        pn = 0;
        pv = 0;
        s  = 3 * f;
        break;
      case GL_C4UB_V2F:
        pc = 0;
        pn = 0;
        pv = c + 0;
        s  = c + 2 * f;
        break;
      case GL_C4UB_V3F:
        pc = 0;
        pn = 0;
        pv = c + 0;
        s  = c + 3 * f;
        break;
      case GL_C3F_V3F:
        pc = 0;
        pn = 0;
        pv = 3 * f;
        s  = 6 * f;
        break;
      case GL_N3F_V3F:
        pc = 0;
        pn = 0;
        pv = 3 * f;
        s  = 6 * f;
        break;
      case GL_C4F_N3F_V3F:
        pc = 0;
        pn = 4 * f;
        pv = 7 * f;
        s  = 10;
        break;
      case GL_T2F_V3F:
        pc = 0;
        pn = 0;
        pv = 2 * f;
        s  = 5 * f;
        break;
      case GL_T4F_V4F:
        pc = 0;
        pn = 0;
        pv = 4 * f;
        s  = 8 * f;
        break;
      case GL_T2F_C4UB_V3F:
        pc = 2 * f;
        pn = 0;
        pv = c + 2 * f;
        s  = c + 5 * f;
        break;
      case GL_T2F_C3F_V3F:
        pc = 2 * f;
        pn = 0;
        pv = 5 * f;
        s  = 8 * f;
        break;
      case GL_T2F_N3F_V3F:
        pc = 0;
        pn = 2 * f;
        pv = 5 * f;
        s  = 8 * f;
        break;
      case GL_T2F_C4F_N3F_V3F:
        pc = 2 * f;
        pn = 6 * f;
        pv = 9 * f;
        s  = 12 * f;
        break;
      case GL_T4F_C4F_N3F_V4F:
        pc = 4 * f;
        pn = 8 * f;
        pv = 11 * f;
        s  = 15 * f;
        break;
      default:
        RegalAssert( !"unhandled format value" );
        break;
    }

    GLubyte *pointerc = static_cast<GLubyte *>( const_cast<GLvoid *>(pointer) ) + pc;
    GLubyte *pointern = static_cast<GLubyte *>( const_cast<GLvoid *>(pointer) ) + pn;
    GLubyte *pointerv = static_cast<GLubyte *>( const_cast<GLvoid *>(pointer) ) + pv;

    if ( stride == 0 )
    {
      stride = s;
    }

    //<> ShadowEnableDisableClientState( ctx, GL_EDGE_FLAG_ARRAY, GL_FALSE );
    //<> ShadowEnableDisableClientState( ctx, GL_INDEX_ARRAY, GL_FALSE );
    ShadowEnableDisableClientState( ctx, GL_SECONDARY_COLOR_ARRAY, GL_FALSE );
    ShadowEnableDisableClientState( ctx, GL_FOG_COORD_ARRAY, GL_FALSE );

    GLint size = 0;
    GLenum type = GL_FLOAT;

    switch ( format )
    {
      case GL_T2F_V3F:
      case GL_T4F_V4F:
      case GL_T2F_C4UB_V3F:
      case GL_T2F_C3F_V3F:
      case GL_T2F_N3F_V3F:
      case GL_T2F_C4F_N3F_V3F:
      case GL_T4F_C4F_N3F_V4F:
        size = ((format == GL_T4F_V4F) || (format == GL_T4F_C4F_N3F_V4F)) ? 4 : 2;
        ShadowEnableDisableClientState( ctx, GL_TEXTURE_COORD_ARRAY, GL_TRUE );
        TexCoordPointer(ctx, size, GL_FLOAT, stride, pointer);
        break;
      default:
        ShadowEnableDisableClientState( ctx, GL_TEXTURE_COORD_ARRAY, GL_FALSE );
        break;
    }

    switch ( format )
    {
      case GL_C4UB_V2F:
      case GL_C4UB_V3F:
      case GL_C3F_V3F:
      case GL_C4F_N3F_V3F:
      case GL_T2F_C4UB_V3F:
      case GL_T2F_C3F_V3F:
      case GL_T2F_C4F_N3F_V3F:
      case GL_T4F_C4F_N3F_V4F:
        size = ((format == GL_C3F_V3F) || (format == GL_T2F_C3F_V3F)) ? 3 : 4;
        ShadowEnableDisableClientState( ctx, GL_COLOR_ARRAY, GL_TRUE );
        ColorPointer(ctx, size, type, stride, pointerc);
        break;
      default:
        ShadowEnableDisableClientState( ctx, GL_COLOR_ARRAY, GL_FALSE );
        break;
    }

    switch ( format )
    {
      case GL_N3F_V3F:
      case GL_C4F_N3F_V3F:
      case GL_T2F_N3F_V3F:
      case GL_T2F_C4F_N3F_V3F:
      case GL_T4F_C4F_N3F_V4F:
        ShadowEnableDisableClientState( ctx, GL_NORMAL_ARRAY, GL_TRUE );
        NormalPointer(ctx, GL_FLOAT, stride, pointern);
        break;
      default:
        ShadowEnableDisableClientState( ctx, GL_NORMAL_ARRAY, GL_FALSE );
        break;
    }

    switch ( format )
    {
      case GL_V2F:
      case GL_C4UB_V2F:
        size = 2;
        break;
      case GL_V3F:
      case GL_C4UB_V3F:
      case GL_C3F_V3F:
      case GL_N3F_V3F:
      case GL_C4F_N3F_V3F:
      case GL_T2F_V3F:
      case GL_T2F_C4UB_V3F:
      case GL_T2F_C3F_V3F:
      case GL_T2F_N3F_V3F:
      case GL_T2F_C4F_N3F_V3F:
        size = 3;
        break;
      case GL_T4F_V4F:
      case GL_T4F_C4F_N3F_V4F:
        size = 4;
        break;
      default:
        RegalAssert( !"unhandled format value" );
        break;
    }

    ShadowEnableDisableClientState( ctx, GL_VERTEX_ARRAY, GL_TRUE );
    VertexPointer(ctx, size, GL_FLOAT, stride, pointerv);
  }
};

}

REGAL_NAMESPACE_END

#endif // ! __REGAL_VAO_H__
