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
#include <algorithm>

#include "RegalPpca.h"
#include "RegalContext.h"
#include "RegalContextInfo.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace {

// Helper function for swapping fixed size arrays of some type.
template <typename T, size_t N> void swap_array( T( &lhs )[ N ], T( &rhs )[ N ] ) {
  using std::swap;
  for ( size_t i = 0; i < N; ++i ) {
    swap( lhs[ i ], rhs[ i ] );
  }
}

} // namespace

// ====================================
// ClientState::PixelStore
// ====================================

namespace ClientState {
namespace PixelStore {

const GLenum State::indexToPName[ STATE_COUNT ] = {
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

void State::Reset() {
  // Most state defaults to zero.
  for ( size_t i = 0; i < STATE_COUNT; ++i ) {
    data[ i ] = 0;
  }
  // The two alignments default to four.
  data[ 7 ] = 4;
  data[ 15 ] = 4;

  pixelPackBufferBinding = 0;
  pixelUnpackBufferBinding = 0;
}

void State::Set( GLenum pname, GLint pvalue ) {
  size_t index = PNameToIndex( pname );
  if ( index >= STATE_COUNT ) {
    return;
  }
  data[ index ] = pvalue;
}

GLint State::Get( GLenum pname ) const {
  size_t index = PNameToIndex( pname );
  if ( index >= STATE_COUNT ) {
    return 0;
  }
  return data[ index ];
}

void swap( State& lhs, State& rhs ) {
  using std::swap;

  swap_array( lhs.data, rhs.data );
  swap( lhs.pixelPackBufferBinding, rhs.pixelPackBufferBinding );
  swap( lhs.pixelUnpackBufferBinding, rhs.pixelUnpackBufferBinding );
}

size_t PNameToIndex( GLenum pname ) {
  switch ( pname ) {
    default:                     return INVALID_INDEX;

    case GL_PACK_SWAP_BYTES:     return 0;
    case GL_PACK_LSB_FIRST:      return 1;
    case GL_PACK_ROW_LENGTH:     return 2;
    case GL_PACK_IMAGE_HEIGHT:   return 3;
    case GL_PACK_SKIP_ROWS:      return 4;
    case GL_PACK_SKIP_PIXELS:    return 5;
    case GL_PACK_SKIP_IMAGES:    return 6;
    case GL_PACK_ALIGNMENT:      return 7;

    case GL_UNPACK_SWAP_BYTES:   return 8;
    case GL_UNPACK_LSB_FIRST:    return 9;
    case GL_UNPACK_ROW_LENGTH:   return 10;
    case GL_UNPACK_IMAGE_HEIGHT: return 11;
    case GL_UNPACK_SKIP_ROWS:    return 12;
    case GL_UNPACK_SKIP_PIXELS:  return 13;
    case GL_UNPACK_SKIP_IMAGES:  return 14;
    case GL_UNPACK_ALIGNMENT:    return 15;
  }
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State& current, const State& target )
{
  UNUSED_PARAMETER(cap);
  RegalAssert( dt.glPixelStorei );

  for ( size_t i = 0; i < STATE_COUNT; ++i ) {
    if ( current.data[ i ] != target.data[ i ] ) {
      dt.glPixelStorei( State::indexToPName[ i ], target.data[ i ] );
    }
  }

  if ( current.pixelPackBufferBinding != target.pixelPackBufferBinding ) {
    dt.glBindBuffer( GL_PIXEL_PACK_BUFFER_BINDING, target.pixelPackBufferBinding );
  }

  if ( current.pixelUnpackBufferBinding != target.pixelUnpackBufferBinding ) {
    dt.glBindBuffer( GL_PIXEL_UNPACK_BUFFER_BINDING, target.pixelUnpackBufferBinding );
  }
}

} // namespace PixelStore
} // namespace ClientState

// ====================================
// ClientState::VertexArray::Fixed
// ====================================

namespace ClientState {
namespace VertexArray {
namespace Fixed {
namespace {

bool operator != ( const State::Source& lhs, const State::Source& rhs ) {
  return ( lhs.buffer != rhs.buffer ) || ( lhs.size != rhs.size ) || ( lhs.type != rhs.type ) || ( lhs.stride != rhs.stride ) || ( lhs.offset != rhs.offset );
}

void ApplyAttribPointer( const State::Source& target, void (REGAL_CALL *dispatchAttribPointer )( GLint, GLenum, GLsizei, const GLvoid* ) ) {
  dispatchAttribPointer( target.size, target.type, target.stride, reinterpret_cast<const GLvoid*>( target.offset ) );
}

void ApplyAttribPointer( const State::Source& target, void (REGAL_CALL *dispatchAttribPointer )( GLenum, GLsizei, const GLvoid* ) ) {
  dispatchAttribPointer( target.type, target.stride, reinterpret_cast<const GLvoid*>( target.offset ) );
}

void ApplyAttribPointer( const State::Source& target, void (REGAL_CALL *dispatchAttribPointer )( GLsizei, const GLvoid* ) ) {
  dispatchAttribPointer( target.stride, reinterpret_cast<const GLvoid*>( target.offset ) );
}

void ApplyClientStateEnable( const DispatchTableGL& dt, GLenum array, bool enable ) {
  ( enable ? dt.glEnableClientState : dt.glDisableClientState )( array );
}

template <typename T>
void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State::Attrib& current, const State::Attrib& target, GLenum array, T dispatchAttribPointer, GLuint& inoutArrayBufferBinding ) {
  if ( target.enabled != current.enabled ) {
    ApplyClientStateEnable( dt, array, target.enabled );
  }

  if ( target.source != current.source ) {
    if ( ( target.source.buffer != 0 ) || cap.driverAllowsVertexAttributeArraysWithoutBoundBuffer ) {
      if ( target.source.buffer != inoutArrayBufferBinding ) {
        dt.glBindBuffer( GL_ARRAY_BUFFER, target.source.buffer );
        inoutArrayBufferBinding = target.source.buffer;
      }
      ApplyAttribPointer( target.source, dispatchAttribPointer );
    }
  }
}

void TransitionTextureCoords( const Capabilities& cap, const DispatchTableGL& dt, const State::Attrib& current, const State::Attrib& target, GLenum texture, GLenum& inoutClientActiveTexture, GLuint& inoutArrayBufferBinding ) {
  bool sourceDelta = target.source != current.source;

  if ( ( current.enabled != target.enabled ) || sourceDelta ) {
    if ( inoutClientActiveTexture != texture ) {
      dt.glClientActiveTexture( texture );
      inoutClientActiveTexture = texture;
    }
  }

  if ( current.enabled != target.enabled ) {
    ApplyClientStateEnable( dt, GL_TEXTURE_COORD_ARRAY, target.enabled );
  }

  if ( sourceDelta ) {
    if ( ( target.source.buffer != 0 ) || cap.driverAllowsVertexAttributeArraysWithoutBoundBuffer ) {
      if ( target.source.buffer != inoutArrayBufferBinding ) {
        dt.glBindBuffer( GL_ARRAY_BUFFER, target.source.buffer );
        inoutArrayBufferBinding = target.source.buffer;
      }
      ApplyAttribPointer( target.source, dt.glTexCoordPointer );
    }
  }
}

void swap( State::Source& lhs, State::Source& rhs ) {
  using std::swap;
  swap( lhs.buffer, rhs.buffer );
  swap( lhs.size, rhs.size );
  swap( lhs.type, rhs.type );
  swap( lhs.stride, rhs.stride );
  swap( lhs.offset, rhs.offset );
}

void swap( State::Attrib& lhs, State::Attrib& rhs ) {
  using std::swap;
  swap( lhs.enabled, rhs.enabled );
  swap( lhs.source, rhs.source );
}

} // namespace

void State::Reset() {
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    attrib[ i ].enabled = false;

    attrib[ i ].source.buffer = 0;
    attrib[ i ].source.size   = 4;
    attrib[ i ].source.type   = GL_FLOAT;
    attrib[ i ].source.stride = 0;
    attrib[ i ].source.offset = 0;
  }

  //  GL_NORMAL_ARRAY.size must be 3
  attrib[ 1 ].source.size = 3;
  //  GL_SECONDARY_COLOR_ARRAY.size must be 3
  attrib[ 3 ].source.size = 3;
  //  GL_INDEX_ARRAY.size must be 1
  attrib[ 4 ].source.size = 1;
  //  GL_EDGE_FLAG_ARRAY.size must be 1
  //  GL_EDGE_FLAG_ARRAY.type must be boolean
  attrib[ 5 ].source.size = 1;
  attrib[ 5 ].source.type = GL_BOOL;
  //  GL_FOG_COORD_ARRAY must be 1
  attrib[ 6 ].source.size = 1;
}

void State::SetEnable( size_t attribIndex, bool enabled ) {
  if ( attribIndex >= COUNT_ATTRIBS ) {
    return;
  }

  attrib[ attribIndex ].enabled = enabled;
}

void State::SetData( size_t attribIndex, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  if ( attribIndex >= COUNT_ATTRIBS ) {
    return;
  }
  State::Source& source = attrib[ attribIndex ].source;

  source.buffer = buffer;
  source.size = size;
  source.type = type;
  source.stride = stride;
  source.offset = offset;
}

void swap( State& lhs, State& rhs ) {
  swap_array( lhs.attrib, rhs.attrib );
}

size_t ArrayNameToAttribIndex( GLenum array, GLenum texunit ) {
  switch ( array ) {
    default:                        return INVALID_ATTRIB_INDEX;

    case GL_VERTEX_ARRAY:           return BASE_NAMED_ATTRIBS + 0;
    case GL_NORMAL_ARRAY:           return BASE_NAMED_ATTRIBS + 1;
    case GL_COLOR_ARRAY:            return BASE_NAMED_ATTRIBS + 2;
    case GL_SECONDARY_COLOR_ARRAY:  return BASE_NAMED_ATTRIBS + 3;
    case GL_INDEX_ARRAY:            return BASE_NAMED_ATTRIBS + 4;
    case GL_EDGE_FLAG_ARRAY:        return BASE_NAMED_ATTRIBS + 5;
    case GL_FOG_COORD_ARRAY:        return BASE_NAMED_ATTRIBS + 6;

    case GL_TEXTURE_COORD_ARRAY:
      {
        size_t index = texunit - GL_TEXTURE0;
        if ( index < COUNT_TEXTURE_COORD_ATTRIBS ) {
          return BASE_TEXTURE_COORD_ATTRIBS + index;
        }
        return INVALID_ATTRIB_INDEX;
      }
  }
}

size_t IndexedArrayNameToAttribIndex( GLenum array, GLuint index ) {
  switch ( array ) {
    default:
      return INVALID_ATTRIB_INDEX;

    case GL_TEXTURE_COORD_ARRAY:
      {
        if ( index < COUNT_TEXTURE_COORD_ATTRIBS ) {
          return BASE_TEXTURE_COORD_ATTRIBS + index;
        }
        return INVALID_ATTRIB_INDEX;
      }
  }
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State& current, const State& target, GLenum& inoutClientActiveTexture, GLuint& inoutArrayBufferBinding ) {
  RegalAssert( dt.glEnableClientState );
  RegalAssert( dt.glDisableClientState );

  RegalAssert( dt.glVertexPointer );
  RegalAssert( dt.glNormalPointer );
  RegalAssert( dt.glColorPointer );
  RegalAssert( dt.glSecondaryColorPointer );
  RegalAssert( dt.glIndexPointer );
  RegalAssert( dt.glEdgeFlagPointer );
  RegalAssert( dt.glFogCoordPointer );

  RegalAssert( dt.glClientActiveTexture );
  RegalAssert( dt.glTexCoordPointer );

  Transition( cap, dt, current.attrib[ 0 ], target.attrib [ 0 ], GL_VERTEX_ARRAY,          dt.glVertexPointer,         inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 1 ], target.attrib [ 1 ], GL_NORMAL_ARRAY,          dt.glNormalPointer,         inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 2 ], target.attrib [ 2 ], GL_COLOR_ARRAY,           dt.glColorPointer,          inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 3 ], target.attrib [ 3 ], GL_SECONDARY_COLOR_ARRAY, dt.glSecondaryColorPointer, inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 4 ], target.attrib [ 4 ], GL_INDEX_ARRAY,           dt.glIndexPointer,          inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 5 ], target.attrib [ 5 ], GL_EDGE_FLAG_ARRAY,       dt.glEdgeFlagPointer,       inoutArrayBufferBinding );
  Transition( cap, dt, current.attrib[ 6 ], target.attrib [ 6 ], GL_FOG_COORD_ARRAY,       dt.glFogCoordPointer,       inoutArrayBufferBinding );

  for ( size_t i = 0; i < COUNT_TEXTURE_COORD_ATTRIBS; ++i ) {
    TransitionTextureCoords( cap, dt, current.attrib[ 7 + i ], target.attrib[ 7 + i ], static_cast<GLenum>(GL_TEXTURE0 + i), inoutClientActiveTexture, inoutArrayBufferBinding );
  }
}

} // namespace Fixed
} // namespace VertexArray
} // namespace ClientState

// ====================================
// ClientState::VertexArray::Generic
// ====================================

namespace ClientState {
namespace VertexArray {
namespace Generic {
namespace {

bool operator != ( const State::Source lhs, const State::Source rhs ) {
  return ( lhs.size != rhs.size ) || ( lhs.type != rhs.type ) || ( lhs.normalized != rhs.normalized ) || ( lhs.pureInteger != rhs.pureInteger ) || ( lhs.relativeOffset != rhs.relativeOffset );
}

void swap( State::Source& lhs, State::Source& rhs ) {
  using std::swap;
  swap( lhs.size, rhs.size );
  swap( lhs.type, rhs.type );
  swap( lhs.normalized, rhs.normalized );
  swap( lhs.pureInteger, rhs.pureInteger );
  swap( lhs.relativeOffset, rhs.relativeOffset );
}

void swap( State::Attrib& lhs, State::Attrib& rhs ) {
  using std::swap;
  swap( lhs.enabled, rhs.enabled );
  swap( lhs.bindingIndex, rhs.bindingIndex );
  swap( lhs.source, rhs.source );
}

void swap( State::Buffer& lhs, State::Buffer& rhs ) {
  using std::swap;
  swap( lhs.buffer, rhs.buffer );
  swap( lhs.offset, rhs.offset );
  swap( lhs.stride, rhs.stride );
  swap( lhs.divisor, rhs.divisor );
}

void ApplyVertexAttribArrayEnable( const DispatchTableGL& dt, GLuint index, bool enable ) {
  ( enable ? dt.glEnableVertexAttribArray : dt.glDisableVertexAttribArray )( index );
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State::Attrib& current, const State::Attrib& target, GLuint index )
{
  UNUSED_PARAMETER(cap);

  if ( target.enabled != current.enabled ) {
    ApplyVertexAttribArrayEnable( dt, index, target.enabled );
  }

  if ( target.source != current.source ) {
    if ( target.source.pureInteger ) {
      dt.glVertexAttribIFormat( index, target.source.size, target.source.type, target.source.relativeOffset );
    } else {
      dt.glVertexAttribFormat( index, target.source.size, target.source.type, target.source.normalized ? GL_TRUE : GL_FALSE, target.source.relativeOffset );
    }
  }

  if ( target.bindingIndex != current.bindingIndex ) {
    dt.glVertexAttribBinding( index, target.bindingIndex );
  }
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State::Buffer& current, const State::Buffer& target, GLuint index )
{
  UNUSED_PARAMETER(cap);

  if ( ( target.buffer != current.buffer ) || ( target.offset != current.offset ) || ( target.stride != current.stride ) ) {
    dt.glBindVertexBuffer( index, target.buffer, target.offset, target.stride );
  }
  if ( target.divisor != current.divisor ) {
    dt.glVertexBindingDivisor( index, target.divisor );
  }
}

} // namespace

void State::Reset() {
  for ( size_t i = 0; i < COUNT_ATTRIBS; ++i ) {
    State::Attrib& attrib = this->attrib[ i ];
    attrib.enabled = false;
    attrib.bindingIndex = static_cast<GLuint>(i);

    State::Source& source = attrib.source;
    source.size = 4;
    source.type = GL_FLOAT;
    source.normalized = false;
    source.pureInteger = false;
    source.relativeOffset = 0;
  }

  for ( size_t i = 0; i < COUNT_BUFFERS; ++i ) {
    State::Buffer& buffer = this->buffer[ i ];
    buffer.buffer = 0;
    buffer.offset = 0;
    buffer.stride = 16;
    buffer.divisor = 0;
  }
}

void State::SetAttribSource( GLuint attribIndex, GLint size, GLenum type, bool normalized, bool pureInteger, GLuint relativeOffset ) {
  if ( attribIndex >= COUNT_ATTRIBS ) {
    return;
  }

  attrib[ attribIndex ].source.size = size;
  attrib[ attribIndex ].source.type = type;
  attrib[ attribIndex ].source.normalized = normalized;
  attrib[ attribIndex ].source.pureInteger = pureInteger;
  attrib[ attribIndex ].source.relativeOffset = relativeOffset;
}

void State::SetBuffer( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride ) {
  if ( bindingIndex >= COUNT_BUFFERS ) {
    return;
  }

  this->buffer[ bindingIndex ].buffer = buffer;
  this->buffer[ bindingIndex ].offset = offset;
  this->buffer[ bindingIndex ].stride = stride;
}

void State::SetBufferDivisor( GLuint bindingIndex, GLuint divisor ) {
  if ( bindingIndex >= COUNT_BUFFERS ) {
    return;
  }

  this->buffer[ bindingIndex ].divisor = divisor;
}

void State::SetAttribBinding( GLuint attribIndex, GLuint bindingIndex ) {
  if ( attribIndex >= COUNT_ATTRIBS ) {
    return;
  }
  if ( bindingIndex >= COUNT_BUFFERS ) {
    return;
  }

  attrib [ attribIndex ].bindingIndex = bindingIndex;
}

void State::SetEnable( GLuint attribIndex, bool enabled ) {
  if ( attribIndex >= COUNT_ATTRIBS ) {
    return;
  }

  attrib [ attribIndex ].enabled = enabled;
}

void swap( State& lhs, State& rhs ) {
  swap_array( lhs.attrib, rhs.attrib );
  swap_array( lhs.buffer, rhs.buffer );
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State& current, const State& target ) {
  for ( GLuint i = 0; i < COUNT_ATTRIBS; ++i ) {
    Transition( cap, dt, current.attrib[ i ], target.attrib[ i ], i );
  }
  for ( GLuint i = 0; i < COUNT_BUFFERS; ++i ) {
    Transition( cap, dt, current.buffer[ i ], target.buffer[ i ], i );
  }
}

} // namespace Generic
} // namespace VertexArray
} // namespace ClientState

// ====================================
// ClientState::VertexArray
// ====================================

namespace ClientState {
namespace VertexArray {
namespace {

void swap( VertexArrayObjectState& lhs, VertexArrayObjectState& rhs ) {
  using std::swap;

  swap( lhs.fixed, rhs.fixed );
  swap( lhs.generic, rhs.generic );

  swap( lhs.elementArrayBufferBinding, rhs.elementArrayBufferBinding );
}

} // namespace

void State::Reset() {
  vertexArrayObjectZero.fixed.Reset();
  vertexArrayObjectZero.generic.Reset();
  vertexArrayObjectZero.elementArrayBufferBinding = 0;

  clientActiveTexture = GL_TEXTURE0;
  arrayBufferBinding = 0;
  drawIndirectBufferBinding = 0;
  vertexArrayBinding = 0;
  primitiveRestartEnabled = false;
  primitiveRestartFixedIndexEnabled = false;
  primitiveRestartIndex = 0;
}

VertexArrayObjectState* State::GetVertexArrayObject( GLuint vao ) {
  // TODO: Optionally support vertex array objects other than zero
  // properly.
  //
  // Option 1: Track their allocation/deallocation alone so we know which
  // integers are actually valid, so the code here can ignore the ones that are
  // not. This would be some code added to the ShadowBindVertexArray call
  //
  // Option 2: Track their internal state as well, so that we can shadow
  // everything about them as needed. This means returning a pointer to a valid
  // VertexArrayObjectState object here, so that it is then set by the various
  // places that call this function.

  if ( vao != 0 ) {
    return NULL;
  }

  return &vertexArrayObjectZero;
}

VertexArrayObjectState* State::GetVertexArrayObject() {
  return GetVertexArrayObject( vertexArrayBinding );
}

void swap( State& lhs, State& rhs ) {
  using std::swap;
  swap( lhs.vertexArrayObjectZero, rhs.vertexArrayObjectZero );
  swap( lhs.clientActiveTexture, rhs.clientActiveTexture );
  swap( lhs.arrayBufferBinding, rhs.arrayBufferBinding );
  swap( lhs.drawIndirectBufferBinding, rhs.drawIndirectBufferBinding );
  swap( lhs.vertexArrayBinding, rhs.vertexArrayBinding );
  swap( lhs.primitiveRestartEnabled, rhs.primitiveRestartEnabled );
  swap( lhs.primitiveRestartFixedIndexEnabled, rhs.primitiveRestartFixedIndexEnabled );
  swap( lhs.primitiveRestartIndex, rhs.primitiveRestartIndex );
}

void Transition( const Capabilities& cap, const DispatchTableGL& dt, const State& current, const State& target ) {
  RegalAssert( dt.glBindVertexArray );
  RegalAssert( dt.glClientActiveTexture );
  RegalAssert( dt.glBindBuffer );
  RegalAssert( dt.glEnable );
  RegalAssert( dt.glDisable );
  RegalAssert( dt.glPrimitiveRestartIndex );

  // To properly restore client vertex array state for vertexArrayObjectZero,
  // we need to bind vertex array zero!.
  if ( current.vertexArrayBinding != 0 ) {
    dt.glBindVertexArray( 0 );
  }

  GLenum clientActiveTexture = current.clientActiveTexture;
  GLuint arrayBufferBinding = current.arrayBufferBinding;

  Transition( cap, dt, current.vertexArrayObjectZero.fixed, target.vertexArrayObjectZero.fixed, clientActiveTexture, arrayBufferBinding );
  Transition( cap, dt, current.vertexArrayObjectZero.generic, target.vertexArrayObjectZero.generic );

  if ( target.vertexArrayObjectZero.elementArrayBufferBinding != current.vertexArrayObjectZero.elementArrayBufferBinding ) {
    dt.glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, target.vertexArrayObjectZero.elementArrayBufferBinding );
  }

  // We are done with calls involving vertexArrayObjectZero state, so we can
  // now restore the actual vertex array binding object.
  if ( target.vertexArrayBinding != 0 ) {
    dt.glBindVertexArray( target.vertexArrayBinding );
  }

  if ( target.clientActiveTexture != clientActiveTexture ) {
    dt.glClientActiveTexture( target.clientActiveTexture );
  }

  if ( target.arrayBufferBinding != arrayBufferBinding ) {
    dt.glBindBuffer( GL_ARRAY_BUFFER, target.arrayBufferBinding );
  }

  if ( target.drawIndirectBufferBinding != current.drawIndirectBufferBinding ) {
    dt.glBindBuffer( GL_DRAW_INDIRECT_BUFFER, target.drawIndirectBufferBinding );
  }

  if ( target.primitiveRestartEnabled != current.primitiveRestartEnabled ) {
    if ( target.primitiveRestartEnabled ) {
      dt.glEnable( GL_PRIMITIVE_RESTART );
    } else {
      dt.glDisable( GL_PRIMITIVE_RESTART );
    }
  }

  if ( target.primitiveRestartFixedIndexEnabled != current.primitiveRestartFixedIndexEnabled ) {
    if ( target.primitiveRestartFixedIndexEnabled ) {
      dt.glEnable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
    } else {
      dt.glDisable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
    }
  }

  if ( target.primitiveRestartIndex != current.primitiveRestartIndex ) {
    dt.glPrimitiveRestartIndex( target.primitiveRestartIndex );
  }
}

} // namespace VertexArray
} // namespace ClientState

// ====================================
// Ppca
// ====================================

namespace Emu {
namespace {

GLsizei EffectiveStride( GLsizei stride, GLint size, GLenum type ) {
  if ( stride != 0 ) {
    return stride;
  }

  if ( size == GL_BGRA ) {
    size = 4;
  }

  switch ( type ) {
    default:
      // This should not happen, and is probably an error.
      RegalAssert( false );
      return 16;

    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
      return size;

    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_HALF_FLOAT:
      return size * 2;

    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
    case GL_FIXED:
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      return size * 4;

    case GL_DOUBLE:
      return size * 8;
  }
}

} // namespace

using ClientState::PixelStore::PNameToIndex;
using ClientState::VertexArray::VertexArrayObjectState;
using ClientState::VertexArray::Fixed::ArrayNameToAttribIndex;
using ClientState::VertexArray::Fixed::IndexedArrayNameToAttribIndex;


Ppca::Ppca() {
}

void Ppca::Init( RegalContext& ctx )
{
  Reset();

  PFNGLGETINTEGERVPROC getIntegerv = ctx.dispatcher.emulation.call(&ctx.dispatcher.emulation.glGetIntegerv);
  RegalAssert(getIntegerv);

  if ( ctx.info->gl_arb_vertex_attrib_binding && !ctx.info->es2)
  {
    getIntegerv( GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, reinterpret_cast<GLint *>( &capabilities.maxVertexAttribRelativeOffset ) );
    getIntegerv( GL_MAX_VERTEX_ATTRIB_BINDINGS, reinterpret_cast<GLint *>( &capabilities.maxVertexAttribBindings ) );
  }

  if ( !ctx.info->es2 && !ctx.info->core )
    getIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint *>( &capabilities.maxVertexTextureImageUnits ) );

  capabilities.maxVertexAttribs = 0;
  getIntegerv( GL_MAX_VERTEX_ATTRIBS, reinterpret_cast<GLint *>( &capabilities.maxVertexAttribs ) );

  if ( !ctx.info->es2 && !ctx.info->core )
    getIntegerv( GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, reinterpret_cast<GLint *>( &capabilities.maxClientAttribStackDepth ) );

  capabilities.maxVertexTextureImageUnits = std::min ( capabilities.maxVertexTextureImageUnits, static_cast<GLuint>( ClientState::VertexArray::Fixed::COUNT_TEXTURE_COORD_ATTRIBS ) );
  capabilities.maxVertexAttribs           = std::min ( capabilities.maxVertexAttribs,           static_cast<GLuint>( ClientState::VertexArray::Generic::COUNT_ATTRIBS ) );
  capabilities.maxVertexAttribBindings    = std::min ( capabilities.maxVertexAttribBindings,    static_cast<GLuint>( ClientState::VertexArray::Generic::COUNT_BUFFERS ) );
  capabilities.maxClientAttribStackDepth  = std::min ( capabilities.maxClientAttribStackDepth,  static_cast<GLuint>( REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH ) );

  capabilities.maxVertexAttribBindings    = std::min ( capabilities.maxVertexAttribBindings,    capabilities.maxVertexAttribs );

  DetectVertexAttributeArrayWithoutBoundBufferSupport( ctx );
}

void Ppca::Reset() {
  capabilities.maxVertexAttribRelativeOffset = ~0u;
  capabilities.maxVertexTextureImageUnits    = ClientState::VertexArray::Fixed::COUNT_TEXTURE_COORD_ATTRIBS;
  capabilities.maxVertexAttribs              = ClientState::VertexArray::Generic::COUNT_ATTRIBS;
  capabilities.maxVertexAttribBindings       = ClientState::VertexArray::Generic::COUNT_BUFFERS;
  capabilities.maxClientAttribStackDepth     = REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH;
  capabilities.driverAllowsVertexAttributeArraysWithoutBoundBuffer = true;

  pss.Reset();
  vas.Reset();

  ClientAttribMaskStack tmp1;
  clientAttribMaskStack_.swap( tmp1 );
  PixelStoreStateStack tmp2;
  pixelStoreStateStack_.swap( tmp2 );
  VertexArrayStateStack tmp3;
  vertexArrayStateStack_.swap( tmp3 );
}

void Ppca::DetectVertexAttributeArrayWithoutBoundBufferSupport( RegalContext& ctx ) {
  if ( !ctx.info->es2 ) {
    return;
  }

  // Chromium/PepperAPI/WebGL GLES generates an error (visible through glGetError) and
  // logs a message if a call is made to glVertexAttribPointer and no
  // GL_ARRAY_BUFFER is bound.
  if ( ctx.info->vendor == "Chromium" || ctx.info->webgl ) {
    capabilities.driverAllowsVertexAttributeArraysWithoutBoundBuffer = false;
  }
}

void Ppca::ShadowPixelStore( GLenum pname, GLint pvalue ) {
  pss.Set( pname, pvalue );
}

void Ppca::ShadowPixelStore( GLenum pname, GLfloat pvalue ) {
  pss.Set( pname, static_cast<GLint>( floorf( pvalue + 0.5f ) ) );
}

// Generic Vertex Attributes
void Ppca::ShadowVertexAttribFormat( GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      if ( normalized == GL_FALSE ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FIXED:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( attribindex >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( relativeoffset > capabilities.maxVertexAttribRelativeOffset ) {
    return;
  }

  vao->generic.SetAttribSource( attribindex, size, type, normalized == GL_TRUE, false, relativeoffset );
}

void Ppca::ShadowVertexAttribIFormat( GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
      break;
  }

  if ( attribindex >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( relativeoffset > capabilities.maxVertexAttribRelativeOffset ) {
    return;
  }

  vao->generic.SetAttribSource ( attribindex, size, type, false, true, relativeoffset );
}

void Ppca::ShadowVertexAttribLFormat( GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_DOUBLE:
      break;
  }

  if ( attribindex >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( relativeoffset > capabilities.maxVertexAttribRelativeOffset ) {
    return;
  }

  vao->generic.SetAttribSource ( attribindex, size, type, false, false, relativeoffset );
}

void Ppca::ShadowBindVertexBuffer( GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( ( offset < 0 ) || ( stride < 0 ) ) {
    return;
  }
  if ( bindingindex >= capabilities.maxVertexAttribBindings ) {
    return;
  }

  vao->generic.SetBuffer( bindingindex, buffer, offset, stride );
}

void Ppca::ShadowVertexAttribBinding( GLuint attribindex, GLuint bindingindex ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( attribindex >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( bindingindex >= capabilities.maxVertexAttribBindings ) {
    return;
  }

  vao->generic.SetAttribBinding( attribindex, bindingindex );
}

void Ppca::ShadowVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      if ( normalized == GL_FALSE ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FIXED:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( stride < 0 ) {
    return;
  }

  vao->generic.SetAttribSource( index, size, type, normalized == GL_TRUE, false, 0 );
  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBuffer( index, vas.arrayBufferBinding, reinterpret_cast<GLintptr>( pointer ), EffectiveStride ( stride, size, type ) );
}

void Ppca::ShadowVertexIAttribPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
      break;
  }

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( stride < 0 ) {
    return;
  }

  vao->generic.SetAttribSource( index, size, type, false, true, 0 );
  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBuffer( index, vas.arrayBufferBinding, reinterpret_cast<GLintptr>( pointer ), EffectiveStride ( stride, size, type ) );
}

void Ppca::ShadowVertexLAttribPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_DOUBLE:
      break;
  }

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( stride < 0 ) {
    return;
  }

  vao->generic.SetAttribSource( index, size, type, false, false, 0 );
  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBuffer( index, vas.arrayBufferBinding, reinterpret_cast<GLintptr>( pointer ), EffectiveStride ( stride, size, type ) );
}

void Ppca::ShadowEnableVertexAttribArray( GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }

  vao->generic.SetEnable( index, true );
}

void Ppca::ShadowDisableVertexAttribArray( GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }

  vao->generic.SetEnable( index, false );
}



void Ppca::ShadowVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), vas.arrayBufferBinding, size, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), vas.arrayBufferBinding, 3, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 3:
    case 4:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
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
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), vas.arrayBufferBinding, size, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 3:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
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
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_SECONDARY_COLOR_ARRAY ), vas.arrayBufferBinding, size, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowIndexPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_INT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_INDEX_ARRAY ), vas.arrayBufferBinding, 1, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowEdgeFlagPointer( GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_EDGE_FLAG_ARRAY ), vas.arrayBufferBinding, 1, GL_BOOL, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowFogCoordPointer( GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_FOG_COORD_ARRAY ), vas.arrayBufferBinding, 1, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, vas.clientActiveTexture ), vas.arrayBufferBinding, size, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}


void Ppca::ShadowEnableClientState( GLenum cap ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( ArrayNameToAttribIndex( cap, vas.clientActiveTexture ), true );
}

void Ppca::ShadowDisableClientState( GLenum cap ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( ArrayNameToAttribIndex( cap, vas.clientActiveTexture ), false );
}

void Ppca::ShadowClientActiveTexture( GLenum texture ) {
  // Validate the arguments. Return silently if invalid.

  if ( ( texture < GL_TEXTURE0 ) || ( texture >= GL_TEXTURE0 + capabilities.maxVertexTextureImageUnits ) ) {
    return;
  }

  vas.clientActiveTexture = texture;
}



void Ppca::ShadowVertexBindingDivisor( GLuint bindingindex, GLuint divisor ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( bindingindex >= capabilities.maxVertexAttribBindings ) {
    return;
  }

  vao->generic.SetBufferDivisor( bindingindex, divisor );
}

void Ppca::ShadowVertexAttribDivisor( GLuint index, GLuint divisor ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }

  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBufferDivisor( index, divisor );
}

void Ppca::ShadowEnableDisable_( GLenum target, bool enable ) {
  switch ( target ) {
    default:
      break;
    case GL_PRIMITIVE_RESTART:
      vas.primitiveRestartEnabled = enable;
      break;
    case GL_PRIMITIVE_RESTART_FIXED_INDEX:
      vas.primitiveRestartFixedIndexEnabled = enable;
      break;
  }
}

void Ppca::ShadowEnable( GLenum target ) {
  ShadowEnableDisable_( target, true );
}

void Ppca::ShadowDisable( GLenum target ) {
  ShadowEnableDisable_( target, false );
}

void Ppca::ShadowPrimitiveRestartIndex( GLuint index ) {
  vas.primitiveRestartIndex = index;
}

void Ppca::ShadowBindBuffer( GLenum target, GLuint buffer ) {
  // TODO: If buffer was not allocated or was deleted, it should be ignored (a
  // real GL backend implementation would set an error).

  switch ( target ) {
    case GL_PIXEL_PACK_BUFFER_BINDING:
      pss.pixelPackBufferBinding = buffer;
      break;
    case GL_PIXEL_UNPACK_BUFFER_BINDING:
      pss.pixelUnpackBufferBinding = buffer;
      break;
    case GL_ARRAY_BUFFER:
      vas.arrayBufferBinding = buffer;
      break;
    case GL_DRAW_INDIRECT_BUFFER:
      vas.drawIndirectBufferBinding = buffer;
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      VertexArrayObjectState* vao = vas.GetVertexArrayObject();
      if ( vao != NULL ) {
        vao->elementArrayBufferBinding = buffer;
      }
      break;
  }
}

void Ppca::ShadowBindVertexArray( GLuint vertexArray ) {
  // TODO: If vertexArray was not allocated or was deleted, it should be
  // ignored (a real GL backend implementation would set an error).

  vas.vertexArrayBinding = vertexArray;
}



void Ppca::ShadowInterleavedArrays( GLenum format, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  GLintptr data = reinterpret_cast<GLintptr>( pointer );

  GLint vsize = 0;
  GLint nsize = 0;
  GLint csize = 0;
  GLenum ctype = GL_FLOAT;
  GLint tsize = 0;

  // Validate the arguments. Return silently if invalid.

  if ( stride < 0 ) {
    return;
  }

  switch ( format ) {
    default:
      return;
    case GL_V2F:
      vsize = 2;
      break;
    case GL_V3F:
      vsize = 3;
      break;
    case GL_C4UB_V2F:
      vsize = 2;
      csize = 4; ctype=GL_UNSIGNED_BYTE;
      break;
    case GL_C4UB_V3F:
      vsize = 3;
      csize = 4; ctype=GL_UNSIGNED_BYTE;
      break;
    case GL_C3F_V3F:
      vsize = 3;
      csize = 3; ctype=GL_FLOAT;
      break;
    case GL_N3F_V3F:
      vsize = 3;
      nsize = 3;
      break;
    case GL_C4F_N3F_V3F:
      vsize = 3;
      nsize = 3;
      csize = 4; ctype=GL_FLOAT;
      break;
    case GL_T2F_V3F:
      vsize = 3;
      tsize = 2;
      break;
    case GL_T4F_V4F:
      vsize = 4;
      tsize = 4;
      break;
    case GL_T2F_C4UB_V3F:
      vsize = 3;
      csize = 4; ctype=GL_UNSIGNED_BYTE;
      tsize = 2;
      break;
    case GL_T2F_C3F_V3F:
      vsize = 3;
      csize = 3; ctype=GL_FLOAT;
      tsize = 2;
      break;
    case GL_T2F_N3F_V3F:
      vsize = 3;
      nsize = 3;
      tsize = 2;
      break;
    case GL_T2F_C4F_N3F_V3F:
      vsize = 3;
      nsize = 3;
      csize = 4; ctype=GL_FLOAT;
      tsize = 2;
      break;
    case GL_T4F_C4F_N3F_V4F:
      vsize = 4;
      nsize = 3;
      csize = 4; ctype=GL_FLOAT;
      tsize = 4;
      break;
  }

  if ( stride == 0 ) {
    stride += vsize * sizeof( GLfloat );
    if ( nsize != 0 ) {
      stride += nsize * sizeof( GLfloat );
    }
    if ( csize != 0 ) {
      if ( ctype == GL_FLOAT ) {
        stride += csize * sizeof( GLfloat );
      } else {
        stride += csize * sizeof( GLubyte );
      }
    }
    if ( tsize != 0 ) {
      stride += tsize * sizeof( GLfloat );
    }
  }

  if ( tsize != 0 ) {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, vas.clientActiveTexture ), true );
    vao->fixed.SetData  ( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, vas.clientActiveTexture ), vas.arrayBufferBinding, tsize, GL_FLOAT, stride, data );
    data += tsize * sizeof( GLfloat );
  } else {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, vas.clientActiveTexture ), false );
  }

  if ( csize != 0 ) {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), true );
    vao->fixed.SetData  ( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), vas.arrayBufferBinding, csize, ctype, stride, data );
    if ( ctype == GL_FLOAT ) {
      data += csize * sizeof( GLfloat );
    } else {
      data += csize * sizeof( GLubyte );
    }
  } else {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), false );
  }

  if ( nsize != 0 ) {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), true );
    vao->fixed.SetData  ( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), vas.arrayBufferBinding, 3, GL_FLOAT, stride, data );
    data += nsize * sizeof( GLfloat );
  } else {
    vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), false );
  }

  vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), true );
  vao->fixed.SetData  ( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), vas.arrayBufferBinding, vsize, GL_FLOAT, stride, data );
  data += vsize * sizeof( GLfloat );

  // Disable all other non-texture coordinate fixed-function arrays.
  vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_SECONDARY_COLOR_ARRAY ), false );
  vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_INDEX_ARRAY ), false );
  vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_EDGE_FLAG_ARRAY ), false );
  vao->fixed.SetEnable( ArrayNameToAttribIndex( GL_FOG_COORD_ARRAY ), false );
}



void Ppca::ClientAttribDefaultDSA( RegalContext* ctx, GLbitfield mask ) {
  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) {
    ClientState::PixelStore::State tmp;
    tmp.Reset();
    Transition( capabilities, ctx->dispatcher.emulation, pss, tmp );
    swap( pss, tmp );
    mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    ClientState::VertexArray::State tmp;
    tmp.Reset();
    Transition( capabilities, ctx->dispatcher.emulation, vas, tmp );
    swap( vas, tmp );
    mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
  }

  if ( ctx->info->core || ctx->info->es2 )
    return;

  if ( mask )
    ctx->dispatcher.emulation.glClientAttribDefaultEXT( mask );
}

void Ppca::PushClientAttribDefaultDSA( RegalContext* ctx, GLbitfield mask ) {
  PushClientAttrib( ctx, mask );
  ClientAttribDefaultDSA( ctx, mask );
}

void Ppca::ShadowMultiTexCoordPointerDSA( GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( ( texunit < GL_TEXTURE0 ) || ( texunit >= GL_TEXTURE0 + capabilities.maxVertexTextureImageUnits ) ) {
    return;
  }

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, texunit ), vas.arrayBufferBinding, size, type, stride, reinterpret_cast<GLintptr>( pointer ) );
}

void Ppca::ShadowEnableClientStateIndexedDSA( GLenum cap, GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( IndexedArrayNameToAttribIndex( cap , index ), true );
}

void Ppca::ShadowDisableClientStateIndexedDSA( GLenum cap, GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject();
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( IndexedArrayNameToAttribIndex( cap, index ), false );
}


void Ppca::ShadowVertexArrayVertexOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_VERTEX_ARRAY ), buffer, size, type, stride, offset );
}

void Ppca::ShadowVertexArrayColorOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 3:
    case 4:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
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
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_COLOR_ARRAY ), buffer, size, type, stride, offset );
}

void Ppca::ShadowVertexArrayEdgeFlagOffsetDSA( GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_EDGE_FLAG_ARRAY ), buffer, 1, GL_BOOL, stride, offset );
}

void Ppca::ShadowVertexArrayIndexOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_INT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_INDEX_ARRAY ), buffer, 1, type, stride, offset );
}

void Ppca::ShadowVertexArrayNormalOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_NORMAL_ARRAY ), buffer, 3, type, stride, offset );
}

void Ppca::ShadowVertexArrayTexCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, vas.clientActiveTexture ), buffer, size, type, stride, offset );
}

void Ppca::ShadowVertexArrayMultiTexCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( ( texunit < GL_TEXTURE0 ) || ( texunit >= GL_TEXTURE0 + capabilities.maxVertexTextureImageUnits ) ) {
    return;
  }

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_SHORT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_TEXTURE_COORD_ARRAY, texunit ), buffer, size, type, stride, offset );
}

void Ppca::ShadowVertexArrayFogCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( type ) {
    default:
      return;
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_FOG_COORD_ARRAY ), buffer, 1, type, stride, offset );
}

void Ppca::ShadowVertexArraySecondaryColorOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 3:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
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
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( stride < 0 ) {
    return;
  }

  vao->fixed.SetData( ArrayNameToAttribIndex( GL_SECONDARY_COLOR_ARRAY ), buffer, size, type, stride, offset );
}

void Ppca::ShadowVertexArrayVertexAttribOffsetDSA( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
    case GL_BGRA:
      if ( ( type != GL_UNSIGNED_BYTE ) || ( type != GL_INT_2_10_10_10_REV ) || ( type != GL_UNSIGNED_INT_2_10_10_10_REV ) ) {
        return;
      }
      if ( normalized == GL_FALSE ) {
        return;
      }
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FIXED:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_DOUBLE:
      break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      if ( ( size != 4 ) || ( size != GL_BGRA ) ) {
        return;
      }
      break;
  }

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( stride < 0 ) {
    return;
  }

  vao->generic.SetAttribSource( index, size, type, normalized == GL_TRUE, false, 0 );
  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBuffer( index, buffer, offset, EffectiveStride ( stride, size, type ) );
}

void Ppca::ShadowVertexArrayVertexAttribIOffsetDSA( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  switch ( size ) {
    default:
      return;
    case 1:
    case 2:
    case 3:
    case 4:
      break;
  }

  switch ( type ) {
    default:
      return;
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
      break;
  }

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }
  if ( stride < 0 ) {
    return;
  }

  vao->generic.SetAttribSource( index, size, type, false, true, 0 );
  vao->generic.SetAttribBinding( index, index );
  vao->generic.SetBuffer( index, buffer, offset, EffectiveStride ( stride, size, type ) );
}

void Ppca::ShadowEnableVertexArrayDSA( GLuint vaobj, GLenum array ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( ArrayNameToAttribIndex( array, vas.clientActiveTexture ), true );
}

void Ppca::ShadowDisableVertexArrayDSA( GLuint vaobj, GLenum array ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }
  vao->fixed.SetEnable( ArrayNameToAttribIndex( array, vas.clientActiveTexture ), false );
}

void Ppca::ShadowEnableVertexArrayAttribDSA( GLuint vaobj, GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }

  vao->generic.SetEnable( index, true  );
}

void Ppca::ShadowDisableVertexArrayAttribDSA( GLuint vaobj, GLuint index ) {
  VertexArrayObjectState* vao = vas.GetVertexArrayObject( vaobj );
  if ( vao == NULL ) {
    return;
  }

  // Validate the arguments. Return silently if invalid.

  if ( index >= capabilities.maxVertexAttribs ) {
    return;
  }

  vao->generic.SetEnable( index, false );
}

void Ppca::ShadowDeleteBuffer_( GLuint buffer ) {
  if ( pss.pixelPackBufferBinding == buffer ) {
    pss.pixelPackBufferBinding = 0;
  }
  if ( pss.pixelUnpackBufferBinding == buffer ) {
    pss.pixelUnpackBufferBinding = 0;
  }

  for ( size_t i = 0; i < ClientState::VertexArray::Generic::COUNT_BUFFERS; ++i ) {
    ClientState::VertexArray::Generic::State::Buffer& gsbuffer = vas.vertexArrayObjectZero.generic.buffer[ i ];
    if ( gsbuffer.buffer == buffer ) {
      gsbuffer.buffer = 0;
    }
  }
  if ( vas.vertexArrayObjectZero.elementArrayBufferBinding == buffer ) {
    vas.vertexArrayObjectZero.elementArrayBufferBinding = 0;
  }
  if ( vas.arrayBufferBinding == buffer ) {
    vas.arrayBufferBinding = 0;
  }
  if ( vas.drawIndirectBufferBinding == buffer ) {
    vas.drawIndirectBufferBinding = 0;
  }
}

void Ppca::ShadowDeleteVertexArray_( GLuint array ) {
  if ( vas.vertexArrayBinding == array ) {
    vas.vertexArrayBinding = 0;
  }
}

void Ppca::ShadowDeleteBuffers( GLsizei n, const GLuint* buffers ) {
  while ( n-- ) {
    GLuint buffer = *buffers++;
    if ( buffer != 0 ) {
      ShadowDeleteBuffer_( buffer );
    }
  }
}

void Ppca::ShadowDeleteVertexArrays( GLsizei n, const GLuint* arrays ) {
  while ( n-- ) {
    GLuint array = *arrays++;
    if ( array != 0 ) {
      ShadowDeleteVertexArray_( array );
    }
  }
}



void Ppca::PushClientAttrib( RegalContext* ctx, GLbitfield mask ) {
  clientAttribMaskStack_.push_back( mask );

  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) {
    pixelStoreStateStack_.push_back( pss );
    mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    vertexArrayStateStack_.push_back( vas );
    mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
  }

  if ( ctx->info->core || ctx->info->es2 )
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
    Transition( capabilities, ctx->dispatcher.emulation, pss, pixelStoreStateStack_.back() );
    swap( pss, pixelStoreStateStack_.back() );
    pixelStoreStateStack_.pop_back();
    mask &= ~GL_CLIENT_PIXEL_STORE_BIT;
  }

  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT ) {
    Transition( capabilities, ctx->dispatcher.emulation, vas, vertexArrayStateStack_.back() );
    swap( vas, vertexArrayStateStack_.back() );
    vertexArrayStateStack_.pop_back();
    mask &= ~GL_CLIENT_VERTEX_ARRAY_BIT;
  }

  if ( ctx->info->core || ctx->info->es2 )
    return;

  if ( mask )
    ctx->dispatcher.emulation.glPopClientAttrib();
}



template <typename T>
bool GetImpl( const Ppca& ppca, RegalContext* ctx, GLenum pname, T *params ) {
  RegalAssert( ctx );

  if ( ctx->info->core || ctx->info->es2 )
  {
    switch ( pname )
    {
      case GL_MAX_CLIENT_ATTRIB_STACK_DEPTH:
        if ( params )
          params[ 0 ] = static_cast<T>(ppca.capabilities.maxClientAttribStackDepth);
        return true;
      case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        if ( params )
          params[ 0 ] = static_cast<T>(ppca.capabilities.maxVertexTextureImageUnits);
        return true;
      case GL_MAX_VERTEX_ATTRIBS:
        if ( params )
          params[ 0 ] = static_cast<T>(ppca.capabilities.maxVertexAttribs);
        return true;
      case GL_MAX_VERTEX_ATTRIB_BINDINGS:
        if ( params )
          params[ 0 ] = static_cast<T>(ppca.capabilities.maxVertexAttribBindings);
        return true;
      default:
        return false;
    }
  }
  return false;
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLint* params ) {
  return GetImpl( *this, ctx, pname, params );
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLint64* params ) {
  return GetImpl( *this, ctx, pname, params );
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLfloat* params ) {
  return GetImpl( *this, ctx, pname, params );
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLdouble* params ) {
  return GetImpl( *this, ctx, pname, params );
}

bool Ppca::Get( RegalContext* ctx, GLenum pname, GLboolean* params ) {
  GLint paramsi[ 1 ];
  if ( !GetImpl( *this, ctx, pname, paramsi ) ) {
    return false;
  }

  if ( params ) {
    params[ 0 ] = ( paramsi[ 0 ] != 0 ) ? GL_TRUE : GL_FALSE;
  }
  return true;
}

} // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
