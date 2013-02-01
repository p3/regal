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


/*

 Regal Emulation of glPushClientAttrib/glPopClientAttrib
 Lloyd Pique

 */

#ifndef __REGAL_PPCA_H__
#define __REGAL_PPCA_H__

#include "RegalUtil.h"

#define REGAL_PPCA_MAX_CLIENT_ATTRIB_STACK_DEPTH 16

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

class DispatchTable;

// ====================================
// ClientState::PixelStore
// ====================================

namespace ClientState {
namespace PixelStore {

struct State {
  enum Constants {
    STATE_COUNT = 16,
    INVALID_INDEX = ~0u,
  };

  static const GLenum indexToPName[ STATE_COUNT ];

  void Reset();
  void Set( GLenum pname, GLint pvalue );
  GLint Get( GLenum pname ) const;

  GLint data[ STATE_COUNT ];
  GLuint pixelPackBufferBinding;
  GLuint pixelUnpackBufferBinding;
};

void swap( State& lhs, State& rhs );

size_t PNameToIndex( GLenum pname );

void Transition( const DispatchTable& dt, const State& current, const State& target );

} // namespace PixelStore
} // namespace ClientState

// ====================================
// ClientState::VertexArray::Fixed
// ====================================

namespace ClientState {
namespace VertexArray {
namespace Fixed {

struct State {
  enum Constants {
    COUNT_NAMED_ATTRIBS = 7,
    COUNT_TEXTURE_COORD_ATTRIBS = 16,
    COUNT_ATTRIBS = COUNT_NAMED_ATTRIBS + COUNT_TEXTURE_COORD_ATTRIBS,

    BASE_NAMED_ATTRIBS = 0,
    BASE_TEXTURE_COORD_ATTRIBS = BASE_NAMED_ATTRIBS + COUNT_NAMED_ATTRIBS,

    INVALID_ATTRIB_INDEX = ~0u,
  };

  struct Source {
    GLint size;
    GLenum type;
    GLsizei stride;
    GLintptr offset;
  };

  struct Attrib {
    bool enabled;
    Source source;
  };

  void Reset();

  void SetEnable ( size_t attribIndex, bool enabled );
  void SetData   ( size_t attribIndex, GLint size, GLenum type, GLsizei stride, GLintptr offset );

  Attrib attrib[ COUNT_ATTRIBS ];
};

void swap( State& lhs, State& rhs );

size_t ArrayNameToAttribIndex( GLenum array, GLenum texunit=GL_TEXTURE0 );
size_t IndexedArrayNameToAttribIndex( GLenum array, GLuint index );

void Transition( const DispatchTable& dt, const State& current, const State& target, GLenum& inoutClientActiveTexture );

} // namespace Fixed
} // namespace VertexArray
} // namespace ClientState

// ====================================
// ClientState::VertexArray::Generic
// ====================================

namespace ClientState {
namespace VertexArray {
namespace Generic {

struct State {
  enum Constants {
    COUNT_ATTRIBS = 16,
    COUNT_BUFFERS = 16,
    INVALID_INDEX = ~0u,
  };

  struct Source {
    GLint size;
    GLenum type;
    bool normalized;
    bool pureInteger;
    GLuint relativeOffset;
  };

  struct Attrib {
    bool enabled;
    GLuint bindingIndex;
    Source source;
  };

  struct Buffer {
    GLuint buffer;
    GLintptr offset;
    GLsizei stride;
    GLuint divisor;
  };

  void Reset();

  void SetAttribSource( GLuint attribIndex, GLint size, GLenum type, bool normalized, bool pureInteger, GLuint relativeOffset );
  void SetBuffer( GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride );
  void SetBufferDivisor( GLuint bindingIndex, GLuint divisor );
  void SetAttribBinding( GLuint attribIndex, GLuint bindingIndex );
  void SetEnable( GLuint attribIndex, bool enabled );

  Attrib attrib[ COUNT_ATTRIBS ];
  Buffer buffer[ COUNT_BUFFERS ];
};

void swap( State& lhs, State& rhs );

void Transition( const DispatchTable& dt, const State& current, const State& target );

} // namespace Generic
} // namespace VertexArray
} // namespace ClientState

// ====================================
// Regal::ClientState::VertexArray
// ====================================

namespace ClientState {
namespace VertexArray {

struct VertexArrayObjectState {
  Fixed::State fixed;
  Generic::State generic;
  GLuint elementArrayBufferBinding;
};

struct State {
  void Reset();
  VertexArrayObjectState* GetVertexArrayObject( GLuint vao );
  VertexArrayObjectState* GetVertexArrayObject();

  VertexArrayObjectState vertexArrayObjectZero;

  GLenum clientActiveTexture;
  GLuint arrayBufferBinding;
  GLuint drawIndirectBufferBinding;
  GLuint vertexArrayBinding;
  bool primitiveRestartEnabled;
  // NB: primitiveRestartFixedIndexEnabled is not included in the OpenGL 4.3
  // Compatability Profile Table 23.8 (20120806), but indicated by the textual
  // description in section 10.7
  bool primitiveRestartFixedIndexEnabled;
  GLuint primitiveRestartIndex;
};

void swap( State& lhs, State& rhs );

void Transition( const DispatchTable& dt, const State& current, const State& target );

} // namespace VertexArray
} // namespace ClientState

// ====================================
// Ppca
// ====================================

namespace Emu {

struct Ppca : public RegalEmu {
  typedef std::vector<GLbitfield> ClientAttribMaskStack;
  typedef std::vector<ClientState::PixelStore::State> PixelStoreStateStack;
  typedef std::vector<ClientState::VertexArray::State> VertexArrayStateStack;

  Ppca();

  void Init( RegalContext &ctx );

  void Reset();

  // Pixel Storage State

  void ShadowPixelStore( GLenum pname, GLint pvalue );
  void ShadowPixelStore( GLenum pname, GLfloat pvalue );

  // Generic Vertex Attributes

  void ShadowVertexAttribFormat( GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset );
  void ShadowVertexAttribIFormat( GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset );
  void ShadowVertexAttribLFormat( GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset );

  void ShadowBindVertexBuffer( GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride );


  void ShadowVertexAttribBinding( GLuint attribindex, GLuint bindingindex );

  void ShadowVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );
  void ShadowVertexIAttribPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowVertexLAttribPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );

  void ShadowEnableVertexAttribArray( GLuint index );
  void ShadowDisableVertexAttribArray( GLuint index );

  // Fixed-Function AZttributes

  void ShadowVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowIndexPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowEdgeFlagPointer( GLsizei stride, const GLvoid* pointer );
  void ShadowFogCoordPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );

  void ShadowEnableClientState( GLenum cap );
  void ShadowDisableClientState( GLenum cap );

  void ShadowClientActiveTexture( GLenum texture );

  // Vertex Attribute Divisors

  void ShadowVertexBindingDivisor( GLuint bindingindex, GLuint divisor );
  void ShadowVertexAttribDivisor( GLuint index, GLuint divisor );

  // Primitive Restart

  void ShadowEnableDisable_( GLenum target, bool enable );
  void ShadowEnable( GLenum target );
  void ShadowDisable( GLenum target );

  void ShadowPrimitiveRestartIndex( GLuint index );

  // Vertex Arrays in Buffer Objects

  void ShadowBindBuffer( GLenum target, GLuint buffer );

  // Vertex Array Objects

  void ShadowBindVertexArray( GLuint array );

  // Interleaved Arrays

  void ShadowInterleavedArrays( GLenum format, GLsizei stride, const GLvoid* pointer );

  // Direct State Access

  void ClientAttribDefaultDSA( RegalContext* ctx, GLbitfield mask );
  void PushClientAttribDefaultDSA( RegalContext* ctx, GLbitfield mask );

  void ShadowMultiTexCoordPointerDSA( GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );

  void ShadowEnableClientStateIndexedDSA( GLenum cap, GLuint index );
  void ShadowDisableClientStateIndexedDSA( GLenum cap, GLuint index );

  void ShadowVertexArrayVertexOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayColorOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayEdgeFlagOffsetDSA( GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayIndexOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayNormalOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayTexCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayMultiTexCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayFogCoordOffsetDSA( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArraySecondaryColorOffsetDSA( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayVertexAttribOffsetDSA( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset );
  void ShadowVertexArrayVertexAttribIOffsetDSA( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset );

  void ShadowEnableVertexArrayDSA( GLuint vaobj, GLenum array );
  void ShadowDisableVertexArrayDSA( GLuint vaobj, GLenum array );

  void ShadowEnableVertexArrayAttribDSA( GLuint vaobj, GLuint index );
  void ShadowDisableVertexArrayAttribDSA( GLuint vaobj, GLuint index );

  void ShadowDeleteBuffer_( GLuint buffer );
  void ShadowDeleteVertexArray_( GLuint array );
  void ShadowDeleteBuffers( GLsizei n, const GLuint* buffers );
  void ShadowDeleteVertexArrays( GLsizei n, const GLuint* arrays );

  // Push/Pop Client attribute state

  void PushClientAttrib( RegalContext* ctx, GLbitfield mask );
  void PopClientAttrib( RegalContext* ctx );

  // Get

  bool Get( RegalContext* ctx, GLenum pname, GLint* params );
  bool Get( RegalContext* ctx, GLenum pname, GLint64* params );
  bool Get( RegalContext* ctx, GLenum pname, GLfloat* params );
  bool Get( RegalContext* ctx, GLenum pname, GLdouble* params );
  bool Get( RegalContext* ctx, GLenum pname, GLboolean* params );

  GLuint maxVertexAttribRelativeOffset;
  GLuint maxVertexTextureImageUnits;
  GLuint maxVertexAttribs;
  GLuint maxVertexAttribBindings;
  GLuint maxClientAttribStackDepth;

  ClientState::PixelStore::State pss;
  ClientState::VertexArray::State vas;

  ClientAttribMaskStack clientAttribMaskStack_;
  PixelStoreStateStack pixelStoreStateStack_;
  VertexArrayStateStack vertexArrayStateStack_;
};

} // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_PPCA_H__
