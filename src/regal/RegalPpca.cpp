/*
  Copyright (c) 2011-2012 NVIDIA Corporation
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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <math.h>

#include "RegalPpca.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu {

// ====================================
// PixelStorageState
// ====================================

const GLenum PixelStorageState::indexToPName[ PixelStorageState::STATE_COUNT ] = {
  GL_PACK_SWAP_BYTES,
  GL_PACK_LSB_FIRST,
  GL_PACK_ROW_LENGTH,
  GL_PACK_IMAGE_HEIGHT,
  GL_PACK_SKIP_ROWS,
  GL_PACK_SKIP_PIXELS,
  GL_PACK_SKIP_IMAGES,
  GL_PACK_ALIGNMENT,

  GL_UNPACK_SWAP_BYTES,
  GL_UNPACK_LSB_FIRST,
  GL_UNPACK_ROW_LENGTH,
  GL_UNPACK_IMAGE_HEIGHT,
  GL_UNPACK_SKIP_ROWS,
  GL_UNPACK_SKIP_PIXELS,
  GL_UNPACK_SKIP_IMAGES,
  GL_UNPACK_ALIGNMENT,
};

size_t PixelStorageState::PNameToIndex( GLenum pname ) {
  switch ( pname ) {
    default:                     return 0;

    case GL_PACK_SWAP_BYTES:     return 1;
    case GL_PACK_LSB_FIRST:      return 2;
    case GL_PACK_ROW_LENGTH:     return 3;
    case GL_PACK_IMAGE_HEIGHT:   return 4;
    case GL_PACK_SKIP_ROWS:      return 5;
    case GL_PACK_SKIP_PIXELS:    return 6;
    case GL_PACK_SKIP_IMAGES:    return 7;
    case GL_PACK_ALIGNMENT:      return 8;

    case GL_UNPACK_SWAP_BYTES:   return 9;
    case GL_UNPACK_LSB_FIRST:    return 10;
    case GL_UNPACK_ROW_LENGTH:   return 11;
    case GL_UNPACK_IMAGE_HEIGHT: return 12;
    case GL_UNPACK_SKIP_ROWS:    return 13;
    case GL_UNPACK_SKIP_PIXELS:  return 14;
    case GL_UNPACK_SKIP_IMAGES:  return 15;
    case GL_UNPACK_ALIGNMENT:    return 16;
  }
}

void PixelStorageState::Transition( DispatchTable& dt, const PixelStorageState& current, const PixelStorageState& target ) {
  RegalAssert( dt.glPixelStorei );

  for ( size_t i = 0; i < STATE_COUNT; ++i ) {
    if ( current.state[ i ] != target.state[ i ] ) {
      dt.glPixelStorei( indexToPName[ i ], target.state[ i ] );
    }
  }
}

void PixelStorageState::Reset() {
  for ( size_t i = 0; i < STATE_COUNT; ++i ) {
    state[ i ] = ( ( i == 7 ) || ( i == 15 ) ) ? 4 : 0;
  }
}

void PixelStorageState::Set( GLenum pname, GLint pvalue ) {
  size_t index = PNameToIndex( pname );
  if ( index == 0 ) {
    return;
  }
  state[ index - 1 ] = pvalue;
}

GLint PixelStorageState::Get( GLenum pname ) const {
  size_t index = PNameToIndex( pname );
  if ( index == 0 ) {
    return 0;
  }
  return state[ index - 1 ];
}

// ====================================
// VertexArrayState
// ====================================

bool operator != ( const VertexArrayState::VertexState& lhs, const VertexArrayState::VertexState& rhs ) {
  return ( lhs.size != rhs.size ) || ( lhs.type != rhs.type ) || ( lhs.stride != rhs.stride ) || ( lhs.pointer != rhs.pointer );
}

void TransitionVertexState( const VertexArrayState::VertexState& current, const VertexArrayState::VertexState& target, void ( *dispatch )( GLint, GLenum, GLsizei, const GLvoid* ) ) {
  if ( target != current ) {
    dispatch( target.size, target.type, target.stride, target.pointer );
  }
}

void TransitionVertexState( const VertexArrayState::VertexState& current, const VertexArrayState::VertexState& target, void ( *dispatch )( GLenum, GLsizei, const GLvoid* ) ) {
  if ( target != current ) {
    dispatch( target.type, target.stride, target.pointer );
  }
}

void TransitionVertexState( const VertexArrayState::VertexState& current, const VertexArrayState::VertexState& target, void ( *dispatch )( GLsizei, const GLvoid* ) ) {
  if ( target != current ) {
    dispatch( target.stride, target.pointer );
  }
}

size_t VertexArrayState::PNameToIndex( GLenum pname ) {
  switch ( pname ) {
    default:                        return 0;

    case GL_COLOR_ARRAY:            return 1;
    case GL_EDGE_FLAG_ARRAY:        return 2;
    case GL_FOG_COORD_ARRAY:        return 3;
    case GL_INDEX_ARRAY:            return 4;
    case GL_NORMAL_ARRAY:           return 5;
    case GL_SECONDARY_COLOR_ARRAY:  return 6;
    case GL_TEXTURE_COORD_ARRAY:    return 7;
    case GL_VERTEX_ARRAY:           return 8;
  }
}

const GLenum VertexArrayState::indexToPName[ VertexArrayState::STATE_COUNT ] = {
  GL_COLOR_ARRAY,
  GL_EDGE_FLAG_ARRAY,
  GL_FOG_COORD_ARRAY,
  GL_INDEX_ARRAY,
  GL_NORMAL_ARRAY,
  GL_SECONDARY_COLOR_ARRAY,
  GL_TEXTURE_COORD_ARRAY,
  GL_VERTEX_ARRAY,
};

void VertexArrayState::Transition( DispatchTable& dt, const VertexArrayState& current, const VertexArrayState& target ) {
  RegalAssert( dt.glEnableClientState );
  RegalAssert( dt.glDisableClientState );
  RegalAssert( dt.glColorPointer );
  RegalAssert( dt.glEdgeFlagPointer );
  RegalAssert( dt.glFogCoordPointer );
  RegalAssert( dt.glIndexPointer );
  RegalAssert( dt.glNormalPointer );
  RegalAssert( dt.glSecondaryColorPointer );
  RegalAssert( dt.glTexCoordPointer );
  RegalAssert( dt.glVertexPointer );

  for ( size_t i = 0; i < VertexArrayState::STATE_COUNT; ++i ) {
    if ( current.state[ i ].enabled != target.state[ i ].enabled ) {
      if ( target.state[ i ].enabled ) {
        dt.glEnableClientState( indexToPName[ i ] );
      } else {
        dt.glDisableClientState( indexToPName[ i ] );
      }
    }
  }

  TransitionVertexState( current.state[ 0 ], target.state [ 0 ], dt.glColorPointer );
  TransitionVertexState( current.state[ 1 ], target.state [ 1 ], dt.glEdgeFlagPointer );
  TransitionVertexState( current.state[ 2 ], target.state [ 2 ], dt.glFogCoordPointer );
  TransitionVertexState( current.state[ 3 ], target.state [ 3 ], dt.glIndexPointer );
  TransitionVertexState( current.state[ 4 ], target.state [ 4 ], dt.glNormalPointer );
  TransitionVertexState( current.state[ 5 ], target.state [ 5 ], dt.glSecondaryColorPointer );
  TransitionVertexState( current.state[ 6 ], target.state [ 6 ], dt.glTexCoordPointer );
  TransitionVertexState( current.state[ 7 ], target.state [ 7 ], dt.glVertexPointer );
}

void VertexArrayState::Reset() {
  for ( size_t i = 0; i < VertexArrayState::STATE_COUNT; ++i ) {
    state[ i ].enabled = false;
    state[ i ].size    = 4;
    state[ i ].type    = GL_FLOAT;
    state[ i ].stride  = 0;
    state[ i ].pointer = NULL;
  }
}

void VertexArrayState::SetEnable( GLenum pname, bool enabled ) {
  size_t index = PNameToIndex( pname );
  if ( index == 0 ) {
    return;
  }

  state[ index - 1 ].enabled = enabled;
}

void VertexArrayState::SetData( GLenum pname, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  size_t index = PNameToIndex( pname );
  if ( index == 0 ) {
    return;
  }

  state[ index - 1 ].size = size;
  state[ index - 1 ].type = type;
  state[ index - 1 ].stride = stride;
  state[ index - 1 ].pointer = pointer;
}

// ====================================
// Ppca
// ====================================

Ppca::Ppca() {
}

void Ppca::Init( RegalContext& ctx )
{
  UNUSED_PARAMETER( ctx );

  pss.Reset();
  vas.Reset();

  ClientAttribMaskStack tmp1;
  clientAttribMaskStack_.swap( tmp1 );
  PixelStorageStateStack tmp2;
  pixelStorageStateStack_.swap( tmp2 );
  VertexArrayStateStack tmp3;
  vertexArrayStateStack_.swap( tmp3 );
}

void Ppca::ShadowPixelStore( GLenum pname, GLint pvalue ) {
  pss.Set( pname, pvalue );
}

void Ppca::ShadowPixelStore( GLenum pname, GLfloat pvalue ) {
  pss.Set( pname, static_cast<GLint>( floorf( pvalue + 0.5f ) ) );
}

void Ppca::ShadowEnableClientState( GLenum cap ) {
  vas.SetEnable( cap, true );
}

void Ppca::ShadowDisableClientState( GLenum cap ) {
  vas.SetEnable( cap, false );
}

void Ppca::ShadowColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_COLOR_ARRAY, size, type, stride, pointer );
}

void Ppca::ShadowEdgeFlagPointer( GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_EDGE_FLAG_ARRAY, 4, GL_FLOAT, stride, pointer );
}

void Ppca::ShadowFogCoordPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_FOG_COORD_ARRAY, 4, type, stride, pointer );
}

void Ppca::ShadowIndexPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_INDEX_ARRAY, 4, type, stride, pointer );
}

void Ppca::ShadowNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_NORMAL_ARRAY, 4, type, stride, pointer );
}

void Ppca::ShadowSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_SECONDARY_COLOR_ARRAY, size, type, stride, pointer );
}

void Ppca::ShadowTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_TEXTURE_COORD_ARRAY, size, type, stride, pointer );
}

void Ppca::ShadowVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  vas.SetData( GL_VERTEX_ARRAY, size, type, stride, pointer );
}

void Ppca::PushClientAttrib( RegalContext* ctx, GLbitfield mask ) {
  clientAttribMaskStack_.push_back( mask );

  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) {
    pixelStorageStateStack_.push_back( pss );
    mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    vertexArrayStateStack_.push_back( vas );
    mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
  }

  if ( ctx->info->core || ctx->info->gles )
    return;

  if ( mask )
    ctx->dispatcher.emulation.glPushClientAttrib( mask );
}

void Ppca::PopClientAttrib( RegalContext* ctx ) {
  RegalAssert( ctx );
  RegalAssert( clientAttribMaskStack_.size() );

  GLbitfield mask = 0;
  if ( !clientAttribMaskStack_.empty() ) {
    mask = clientAttribMaskStack_.back();
    clientAttribMaskStack_.pop_back();
  }

  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) {
    PixelStorageState::Transition( ctx->dispatcher.emulation, pss, pixelStorageStateStack_.back() );
    pss = pixelStorageStateStack_.back();
    pixelStorageStateStack_.pop_back();
    mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    VertexArrayState::Transition( ctx->dispatcher.emulation, vas, vertexArrayStateStack_.back() );
    vas = vertexArrayStateStack_.back();
    vertexArrayStateStack_.pop_back();
    mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
  }

  if ( ctx->info->core || ctx->info->gles )
    return;

  if ( mask )
    ctx->dispatcher.emulation.glPopClientAttrib();
}

void Ppca::PushClientAttribDefaultEXT( RegalContext* ctx, GLbitfield mask ) {
  PushClientAttrib( ctx, mask );

  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) {
    pss.Reset();
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    vas.Reset();
  }
}

template <typename T>
bool GetImpl( RegalContext* ctx, GLenum pname, T params ) {
  RegalAssert( ctx );

  if ( ctx->info->core || ctx->info->gles )
  {
    switch ( pname )
    {
      case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        if ( params )
          params[ 0 ] = REGAL_CLIENT_ATTRIB_STACK_DEPTH;
        break;
      default:
        return false;
    }
    return true;
  }
  return false;
}


bool Ppca::Get( RegalContext* ctx, GLenum pname, GLint* params ) {
  return GetImpl(ctx, pname, params);
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLint64* params ) {
  return GetImpl(ctx, pname, params);
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLfloat* params ) {
  return GetImpl(ctx, pname, params);
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLdouble* params ) {
  return GetImpl(ctx, pname, params);
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLboolean* params ) {
  RegalAssert( ctx );

  if ( ctx->info->core || ctx->info->gles )
  {
    switch ( pname )
    {
      case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        if ( params )
          params[ 0 ] = GL_TRUE;
        break;
      default:
        return false;
    }
    return true;
  }
  return false;
}

} // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
