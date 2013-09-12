/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2013 Google Inc
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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "RegalContext.h"
#include "RegalContextInfo.h"
#include "RegalDispatchGMock.h"
#include "RegalPpca.h"

namespace {

using namespace Regal;
using namespace Regal::Emu;

using ::testing::Mock;
using ::testing::_;
using ::testing::AnyNumber;


template <typename T, size_t N> size_t arraysize( const T( & )[ N ] ) {
  return N;
}

void checkNamedVertexArrayDefaults(ClientState::NamedVertexArray& nva, GLuint n)
{
  EXPECT_EQ( GLboolean(GL_FALSE),   nva.enabled );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>(NULL), nva.pointer );
  EXPECT_EQ( GLuint(0),             nva.buffer );
  EXPECT_EQ( GLsizei(0),            nva.stride );

  if (n == ClientState::FOG_COORD ||
      n == ClientState::INDEX ||
      n == ClientState::EDGE_FLAG)
    EXPECT_EQ( GLint(1), nva.size );
  else if (n == ClientState::NORMAL ||
           n == ClientState::SECONDARY_COLOR)
    EXPECT_EQ( GLint(3), nva.size );
  else
    EXPECT_EQ( GLint(4), nva.size );

  if (n == ClientState::EDGE_FLAG)
    EXPECT_EQ( GLenum(GL_BOOL),  nva.type );
  else
    EXPECT_EQ( GLenum(GL_FLOAT), nva.type );
}

void checkVertexBufferBindPointDefaults(ClientState::VertexBufferBindPoint& vbbp)
{
  EXPECT_EQ( GLuint(0),   vbbp.buffer );
  EXPECT_EQ( GLintptr(0), vbbp.offset );
  EXPECT_EQ( GLsizei(16), vbbp.stride );
  EXPECT_EQ( GLuint(0),   vbbp.divisor );
}

void checkGenericVertexArrayDefaults(ClientState::GenericVertexArray& gva, GLuint index)
{
  EXPECT_EQ( GLboolean(GL_FALSE), gva.enabled );
  EXPECT_EQ( GLuint(4),           gva.size );
  EXPECT_EQ( GLenum(GL_FLOAT),    gva.type );
  EXPECT_EQ( GLuint(0),           gva.relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), gva.normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), gva.isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), gva.isLong );
  EXPECT_EQ( index,               gva.bindingIndex );
}

void checkVertexArrayDefaults(ClientState::VertexArray& va)
{
  EXPECT_EQ( GLuint(0),           va.elementArrayBufferBinding );
  EXPECT_EQ( GLenum(GL_TEXTURE0), va.clientActiveTexture );
  EXPECT_EQ( GLboolean(GL_FALSE), va.primitiveRestartFixedIndex );
  EXPECT_EQ( GLboolean(GL_FALSE), va.primitiveRestart );
  EXPECT_EQ( GLuint(0),           va.primitiveRestartIndex );
  EXPECT_EQ( GLuint(0),           va.arrayBufferBinding );
  EXPECT_EQ( GLuint(0),           va.vertexArrayBinding );

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
    checkNamedVertexArrayDefaults( va.named[ii], ii );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
    checkVertexBufferBindPointDefaults( va.bindings[ii] );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
    checkGenericVertexArrayDefaults( va.generic[ii], ii );
}

void checkPixelStoreDefaults(ClientState::PixelStore& ps)
{
  EXPECT_EQ( GLboolean(GL_FALSE), ps.unpackSwapBytes );
  EXPECT_EQ( GLboolean(GL_FALSE), ps.unpackLsbFirst );
  EXPECT_EQ( GLint(0),            ps.unpackImageHeight );
  EXPECT_EQ( GLint(0),            ps.unpackSkipImages );
  EXPECT_EQ( GLint(0),            ps.unpackRowLength );
  EXPECT_EQ( GLint(0),            ps.unpackSkipRows );
  EXPECT_EQ( GLint(0),            ps.unpackSkipPixels );
  EXPECT_EQ( GLint(4),            ps.unpackAlignment );
  EXPECT_EQ( GLboolean(GL_FALSE), ps.packSwapBytes );
  EXPECT_EQ( GLboolean(GL_FALSE), ps.packLsbFirst );
  EXPECT_EQ( GLint(0),            ps.packImageHeight );
  EXPECT_EQ( GLint(0),            ps.packSkipImages );
  EXPECT_EQ( GLint(0),            ps.packRowLength );
  EXPECT_EQ( GLint(0),            ps.packSkipRows );
  EXPECT_EQ( GLint(0),            ps.packSkipPixels );
  EXPECT_EQ( GLint(4),            ps.packAlignment );
  EXPECT_EQ( GLuint(0),           ps.pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint(0),           ps.pixelPackBufferBinding );
}

void checkPpcaDefaults(RegalContext& ctx, Ppca& ppca)
{
  GLint clientAttribStackDepth = 666;
  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(0) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(0),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  checkVertexArrayDefaults(ppca);
  checkPixelStoreDefaults(ppca);
}

TEST ( RegalPpca, Ppca_Defaults )
{
  RegalGMockInterface mock;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->core = false;
  ctx.info->es1 = false;
  ctx.info->es2 = false;
  ctx.info->max_client_attrib_stack_depth = 16;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  GLint maxClientAttribStackDepth = 0;
  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &maxClientAttribStackDepth ) );
  EXPECT_GE( maxClientAttribStackDepth, GLint(16) );

  const GLbitfield remainder = ~( GL_CLIENT_PIXEL_STORE_BIT | GL_CLIENT_VERTEX_ARRAY_BIT );
  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );

  ppca.glPushClientAttrib( ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  Mock::VerifyAndClear( &mock );

  GLint clientAttribStackDepth = 666;
  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( GLint(1), clientAttribStackDepth );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(1),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);
}

TEST ( RegalPpca, VertexArray_Defaults )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::elementArrayBufferBinding = GLuint(6);
  ppca.VertexArray::clientActiveTexture = GLenum(6);
  ppca.VertexArray::primitiveRestartFixedIndex = GLboolean(GL_TRUE);
  ppca.VertexArray::primitiveRestart = GLboolean(GL_TRUE);
  ppca.VertexArray::primitiveRestartIndex = GLuint(6);
  ppca.VertexArray::arrayBufferBinding = GLuint(6);
  ppca.VertexArray::vertexArrayBinding = GLuint(6);

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.VertexArray::named[ii].enabled = GLboolean(GL_TRUE);
    ppca.VertexArray::named[ii].pointer = reinterpret_cast<const GLvoid*>(77);
    ppca.VertexArray::named[ii].buffer  = GLuint(77);
    ppca.VertexArray::named[ii].size    = GLint(77);
    ppca.VertexArray::named[ii].type    = GLenum(77);
    ppca.VertexArray::named[ii].stride  = GLsizei(77);
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer  = GLuint(888);
    ppca.VertexArray::bindings[ii].offset  = GLintptr(888);
    ppca.VertexArray::bindings[ii].stride  = GLsizei(888);
    ppca.VertexArray::bindings[ii].divisor = GLuint(888);
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.VertexArray::generic[ii].enabled        = GLboolean(9999);
    ppca.VertexArray::generic[ii].size           = GLuint(9999);
    ppca.VertexArray::generic[ii].type           = GLenum(9999);
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(9999);
    ppca.VertexArray::generic[ii].normalized     = GLboolean(9999);
    ppca.VertexArray::generic[ii].isInteger      = GLboolean(9999);
    ppca.VertexArray::generic[ii].isLong         = GLboolean(9999);
    ppca.VertexArray::generic[ii].bindingIndex   = GLuint(9999);
  }

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);
}

TEST ( RegalPpca, PixelStore_Defaults )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.PixelStore::unpackSwapBytes = GL_TRUE;
  ppca.PixelStore::unpackLsbFirst = GL_TRUE;
  ppca.PixelStore::unpackImageHeight = 1;
  ppca.PixelStore::unpackSkipImages = 2;
  ppca.PixelStore::unpackRowLength = 3;
  ppca.PixelStore::unpackSkipRows = 4;
  ppca.PixelStore::unpackSkipPixels = 5;
  ppca.PixelStore::unpackAlignment = 6;
  ppca.PixelStore::packSwapBytes = GL_TRUE;
  ppca.PixelStore::packLsbFirst = GL_TRUE;
  ppca.PixelStore::packImageHeight = 7;
  ppca.PixelStore::packSkipImages = 8;
  ppca.PixelStore::packRowLength = 9;
  ppca.PixelStore::packSkipRows = 10;
  ppca.PixelStore::packSkipPixels = 11;
  ppca.PixelStore::packAlignment = 12;
  ppca.PixelStore::pixelUnpackBufferBinding = 13;
  ppca.PixelStore::pixelPackBufferBinding = 14;

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);
}

TEST ( RegalPpca, VertexArray_Swap )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca state;
  checkPpcaDefaults(ctx, state);

  state.VertexArray::elementArrayBufferBinding = GLuint(1);
  state.VertexArray::clientActiveTexture = GLenum(2);
  state.VertexArray::primitiveRestartFixedIndex = GLboolean(GL_TRUE);
  state.VertexArray::primitiveRestart = GLboolean(GL_TRUE);
  state.VertexArray::primitiveRestartIndex = GLuint(3);
  state.VertexArray::arrayBufferBinding = GLuint(4);
  state.VertexArray::vertexArrayBinding = GLuint(5);

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    state.VertexArray::named[ii].enabled = GLboolean(GL_TRUE);
    state.VertexArray::named[ii].pointer = reinterpret_cast<const GLvoid*>(77);
    state.VertexArray::named[ii].buffer  = GLuint(77);
    state.VertexArray::named[ii].size    = GLint(77);
    state.VertexArray::named[ii].type    = GLenum(77);
    state.VertexArray::named[ii].stride  = GLsizei(77);
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    state.VertexArray::bindings[ii].buffer  = GLuint(888);
    state.VertexArray::bindings[ii].offset  = GLintptr(888);
    state.VertexArray::bindings[ii].stride  = GLsizei(888);
    state.VertexArray::bindings[ii].divisor = GLuint(888);
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    state.VertexArray::generic[ii].enabled        = GLboolean(9999);
    state.VertexArray::generic[ii].size           = GLuint(9999);
    state.VertexArray::generic[ii].type           = GLenum(9999);
    state.VertexArray::generic[ii].relativeOffset = GLuint(9999);
    state.VertexArray::generic[ii].normalized     = GLboolean(9999);
    state.VertexArray::generic[ii].isInteger      = GLboolean(9999);
    state.VertexArray::generic[ii].isLong         = GLboolean(9999);
    state.VertexArray::generic[ii].bindingIndex   = GLuint(9999);
  }

  Ppca other;
  checkPpcaDefaults(ctx, other);

  other.VertexArray::swap( state );

  checkPpcaDefaults(ctx, state);

  EXPECT_EQ( GLuint( 1 ),        other.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLenum( 2 ),        other.VertexArray::clientActiveTexture );
  EXPECT_EQ( GLboolean(GL_TRUE), other.VertexArray::primitiveRestartFixedIndex );
  EXPECT_EQ( GLboolean(GL_TRUE), other.VertexArray::primitiveRestart );
  EXPECT_EQ( GLuint(3),          other.VertexArray::primitiveRestartIndex );
  EXPECT_EQ( GLuint(4),          other.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(5),          other.VertexArray::vertexArrayBinding );

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    EXPECT_EQ( GLboolean(GL_TRUE), other.VertexArray::named[ii].enabled );
    EXPECT_EQ( reinterpret_cast<const GLvoid*>(77), other.VertexArray::named[ii].pointer );
    EXPECT_EQ( GLuint(77),         other.VertexArray::named[ii].buffer );
    EXPECT_EQ( GLint(77),          other.VertexArray::named[ii].size );
    EXPECT_EQ( GLenum(77),         other.VertexArray::named[ii].type );
    EXPECT_EQ( GLsizei(77),        other.VertexArray::named[ii].stride );
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(888),   other.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(888), other.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(888),  other.VertexArray::bindings[ii].stride );
    EXPECT_EQ( GLuint(888),   other.VertexArray::bindings[ii].divisor );
  }

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    EXPECT_EQ( GLboolean(9999), other.VertexArray::generic[ii].enabled );
    EXPECT_EQ( GLuint(9999),    other.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(9999),    other.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(9999),    other.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(9999), other.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(9999), other.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(9999), other.VertexArray::generic[ii].isLong );
    EXPECT_EQ( GLuint(9999),    other.VertexArray::generic[ii].bindingIndex );
  }
}

TEST ( RegalPpca, PixelStore_Swap )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca state;
  checkPpcaDefaults(ctx, state);

  Ppca other;
  checkPpcaDefaults(ctx, other);

  // Set each attribute in other to a unique value

  other.PixelStore::unpackSwapBytes = GL_TRUE;
  other.PixelStore::unpackLsbFirst = GL_TRUE;
  other.PixelStore::unpackImageHeight = 11;
  other.PixelStore::unpackSkipImages = 12;
  other.PixelStore::unpackRowLength = 13;
  other.PixelStore::unpackSkipRows = 14;
  other.PixelStore::unpackSkipPixels = 15;
  other.PixelStore::unpackAlignment = 16;
  other.PixelStore::packSwapBytes = GL_TRUE;
  other.PixelStore::packLsbFirst = GL_TRUE;
  other.PixelStore::packImageHeight = 27;
  other.PixelStore::packSkipImages = 28;
  other.PixelStore::packRowLength = 29;
  other.PixelStore::packSkipRows = 30;
  other.PixelStore::packSkipPixels = 31;
  other.PixelStore::packAlignment = 32;
  other.PixelStore::pixelUnpackBufferBinding = 123;
  other.PixelStore::pixelPackBufferBinding = 456;

  // Peform a swap then check these values are now in state

  other.PixelStore::swap( state );

  EXPECT_EQ( GLboolean(GL_TRUE), state.PixelStore::unpackSwapBytes );
  EXPECT_EQ( GLboolean(GL_TRUE), state.PixelStore::unpackLsbFirst );
  EXPECT_EQ( GLint(11),          state.PixelStore::unpackImageHeight );
  EXPECT_EQ( GLint(12),          state.PixelStore::unpackSkipImages );
  EXPECT_EQ( GLint(13),          state.PixelStore::unpackRowLength );
  EXPECT_EQ( GLint(14),          state.PixelStore::unpackSkipRows );
  EXPECT_EQ( GLint(15),          state.PixelStore::unpackSkipPixels );
  EXPECT_EQ( GLint(16),          state.PixelStore::unpackAlignment );
  EXPECT_EQ( GLboolean(GL_TRUE), state.PixelStore::packSwapBytes );
  EXPECT_EQ( GLboolean(GL_TRUE), state.PixelStore::packLsbFirst );
  EXPECT_EQ( GLint(27),          state.PixelStore::packImageHeight );
  EXPECT_EQ( GLint(28),          state.PixelStore::packSkipImages );
  EXPECT_EQ( GLint(29),          state.PixelStore::packRowLength );
  EXPECT_EQ( GLint(30),          state.PixelStore::packSkipRows );
  EXPECT_EQ( GLint(31),          state.PixelStore::packSkipPixels );
  EXPECT_EQ( GLint(32),          state.PixelStore::packAlignment );
  EXPECT_EQ( GLuint(123),        state.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint(456),        state.PixelStore::pixelPackBufferBinding );

  // Verify other contains all default values from state

  checkPixelStoreDefaults(other);
}

TEST ( RegalPpca, PixelStore_PushPop )
{
  RegalGMockInterface mock;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->max_client_attrib_stack_depth = 16;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  EXPECT_EQ( GLint(4), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  ppca.glPixelStore( GL_UNPACK_ALIGNMENT, 100 );
  ppca.glPushClientAttrib( ctx, 0 );

  EXPECT_EQ( GLint(100), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  ppca.glPixelStore( GL_UNPACK_ALIGNMENT, 101 );
  ppca.glPushClientAttrib( ctx, GL_CLIENT_PIXEL_STORE_BIT );

  EXPECT_EQ( GLint(101), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glClientAttribDefaultEXT( ctx, GL_CLIENT_PIXEL_STORE_BIT );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(4), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1), ppca.pixelStoreStack.size() );

  ppca.glPixelStore( GL_UNPACK_ALIGNMENT, 102 );

  EXPECT_EQ( GLint(102), ppca.PixelStore::unpackAlignment );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPushClientAttribDefaultEXT( ctx, GL_CLIENT_PIXEL_STORE_BIT );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(4), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(2),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(102), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(101), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(101), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLint(101), ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );
}

TEST ( RegalPpca, VertexArray_PushPop )
{
  RegalGMockInterface mock;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->max_client_attrib_stack_depth = 16;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  EXPECT_EQ( GLsizei( 0 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );

  ppca.glColorPointer ( 4, GL_FLOAT, 100, NULL );
  ppca.glPushClientAttrib( ctx, 0 );

  EXPECT_EQ( GLsizei( 100 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );

  ppca.glColorPointer ( 4, GL_FLOAT, 101, NULL );
  ppca.glPushClientAttrib( ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( GLsizei( 101 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glClientAttribDefaultEXT( ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 0 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );

  ppca.glColorPointer ( 4, GL_FLOAT, 102, NULL );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPushClientAttribDefaultEXT( ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 0 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(2), ppca.vertexArrayStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 102 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 101 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 101 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );

  ppca.glPopClientAttrib( ctx );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( GLsizei( 101 ), ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
}

TEST ( RegalPpca, ClientAttrib_PushPop )
{
  RegalGMockInterface mock;

  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->core = false;
  ctx.info->es1 = false;
  ctx.info->es2 = false;
  ctx.info->max_client_attrib_stack_depth = 16;
  InitDispatchTableGMock( ctx.dispatcher.emulation );

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  const GLbitfield remainder = ~( GL_CLIENT_PIXEL_STORE_BIT | GL_CLIENT_VERTEX_ARRAY_BIT );

  GLint clientAttribStackDepth = 666;
  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(0) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(0),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  ppca.glPushClientAttrib( ctx, GL_CLIENT_PIXEL_STORE_BIT );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(1) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(1),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  ppca.glPushClientAttrib( ctx, GL_CLIENT_VERTEX_ARRAY_BIT );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(2) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(2),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );

  ppca.glPushClientAttrib( ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(3) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(3),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(2), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(2),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPushClientAttrib(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPopClientAttrib() ).Times(AnyNumber());

  ctx.info->es2 = true;
  ppca.glPushClientAttrib( ctx, GL_CLIENT_ALL_ATTRIB_BITS );
  ppca.glPopClientAttrib( ctx );
  ctx.info->es2 = false;
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(3) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(3),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(2), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(2),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  EXPECT_CALL( mock, glPopClientAttrib() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(2) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(2),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(1) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(1),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPopClientAttrib( ctx );
  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( true, ppca.glGetv(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH, &clientAttribStackDepth));
  EXPECT_EQ( clientAttribStackDepth, GLint(0) );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(0),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glClientAttribDefaultEXT( remainder ) );
  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glClientAttribDefaultEXT( ctx, GL_CLIENT_ALL_ATTRIB_BITS );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(0),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(0), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(0),  ppca.pixelStoreStack.size() );

  EXPECT_CALL( mock, glClientAttribDefaultEXT( remainder ) );
  EXPECT_CALL( mock, glPushClientAttrib( remainder ) );
  EXPECT_CALL( mock, glBindBuffer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glBindVertexBuffer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glClientActiveTexture(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableClientStateiEXT(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glDisableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEdgeFlagPointer(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnable(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableClientState(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glEnableVertexAttribArray(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glFogCoordPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glIndexPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glMultiTexCoordPointerEXT(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glNormalPointer(_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPixelStorei(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glPrimitiveRestartIndex(_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glSecondaryColorPointer(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribBinding(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribFormat(_,_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribIFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexAttribLFormat(_,_,_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexBindingDivisor(_,_) ).Times(AnyNumber());
  EXPECT_CALL( mock, glVertexPointer(_,_,_,_) ).Times(AnyNumber());

  ppca.glPushClientAttribDefaultEXT( ctx, GL_CLIENT_ALL_ATTRIB_BITS );

  Mock::VerifyAndClear( &mock );

  EXPECT_EQ( std::vector<GLbitfield>::size_type(1),               ppca.maskStack.size() );
  EXPECT_EQ( std::vector<ClientState::VertexArray>::size_type(1), ppca.vertexArrayStack.size() );
  EXPECT_EQ( std::vector<ClientState::PixelStore>::size_type(1),  ppca.pixelStoreStack.size() );
}

TEST ( RegalPpca, VertexArray_Named_BasicOperations )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca state;
  checkPpcaDefaults(ctx, state);

  // Set unique data for the array source for all attributes.

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    if ( ii == 4 ) {
      state.VertexArray::named[ii].enabled = GLboolean(GL_TRUE);
    } else {
      state.VertexArray::named[ii].enabled = GLboolean(GL_FALSE);
    }
    state.VertexArray::named[ii].pointer = reinterpret_cast<const GLvoid*>(ii * 10 + 5);
    state.VertexArray::named[ii].buffer  = GLuint(ii * 10 + 1);
    state.VertexArray::named[ii].size    = GLint( ii * 10 + 2);
    state.VertexArray::named[ii].type    = GLenum(ii * 10 + 3);
    state.VertexArray::named[ii].stride  = GLsizei(ii * 10 + 4);
  }

  // Calls with out-of-bound values should silently return as a no-op.
  // This is done here so if we affect the explicitly set state it will be
  // detected soon.

  state.glEnableClientState( GLenum(~0) );
  state.glEnableClientStateiEXT( GL_TEXTURE_COORD_ARRAY, GLuint(~0) );

  state.glDisableClientState( GLenum(~0) );
  state.glDisableClientStateiEXT( GL_TEXTURE_COORD_ARRAY, GLuint(~0) );

  state.glEnable( GLenum(~0) );
  state.glEnable( GLenum(ClientState::nNamedArrays) );

  state.glDisable( GLenum(~0) );
  state.glDisable( GLenum(ClientState::nNamedArrays) );

  // Peform a swap, so that it is effectively tested too

  Ppca other;
  checkPpcaDefaults(ctx, other);

  state.VertexArray::swap( other );

  // Verify the expected default state ended up in the original

  checkPpcaDefaults(ctx, state);

  // Verify the modified data ended up in the other state

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    if ( ii == 4 ) {
      EXPECT_EQ( GLboolean(GL_TRUE),  other.VertexArray::named[ii].enabled );
    } else {
      EXPECT_EQ( GLboolean(GL_FALSE), other.VertexArray::named[ii].enabled );
    }
    EXPECT_EQ( reinterpret_cast<const GLvoid*>(ii * 10 + 5), other.VertexArray::named[ ii ].pointer );
    EXPECT_EQ( GLuint(ii * 10 + 1), other.VertexArray::named[ ii ].buffer );
    EXPECT_EQ( GLint( ii * 10 + 2), other.VertexArray::named[ ii ].size );
    EXPECT_EQ( GLenum(ii * 10 + 3), other.VertexArray::named[ ii ].type );
    EXPECT_EQ( GLsizei(ii * 10 + 4), other.VertexArray::named[ ii ].stride );
  }
}

TEST ( RegalPpca, VertexArray_Generic_BasicOperations )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca state;
  checkPpcaDefaults(ctx, state);

  // Set unique data for the arrays

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    if ( ii == 4 ) {
      state.VertexArray::generic[ii].enabled = GLboolean(GL_TRUE);
    } else {
      state.VertexArray::generic[ii].enabled = GLboolean(GL_FALSE);
    }
    state.VertexArray::generic[ii].size           = GLuint(ii * 100 + 1);
    state.VertexArray::generic[ii].type           = GLenum(ii * 100 + 2);
    state.VertexArray::generic[ii].relativeOffset = GLuint(ii * 100 + 3);
    state.VertexArray::generic[ii].normalized     = GLboolean(ii * 100 + 4);
    state.VertexArray::generic[ii].isInteger      = GLboolean(ii * 100 + 5);
    state.VertexArray::generic[ii].isLong         = GLboolean(ii * 100 + 6);
    state.VertexArray::generic[ii].bindingIndex   = GLuint(ii * 100 + 7);
  }

  // Set unique data for the bindings

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    state.VertexArray::bindings[ii].buffer  = GLuint(ii * 1000 + 1);
    state.VertexArray::bindings[ii].offset  = GLintptr(ii * 1000 + 2);
    state.VertexArray::bindings[ii].stride  = GLsizei(ii * 1000 + 3);
    state.VertexArray::bindings[ii].divisor = GLuint(ii * 1000 + 4);
  }

  // Calls with out-of-bound values should silently return as a no-op.
  // This is done here so if we affect the explicitly set state it will be
  // detected soon.

  state.glEnableVertexArrayAttribEXT( 0, GLuint(~0) );
  state.glDisableVertexArrayAttribEXT( 0, GLuint(~0) );
  state.glVertexAttribFormat( GLuint(~0), 0, 0, GL_TRUE, 0 );
  state.glVertexAttribFormat( GLuint(~0), 0, 0, GL_FALSE, 0 );
  state.glVertexAttribLFormat( GLuint(~0), 0, 0, 0 );
  state.glVertexAttribIFormat( GLuint(~0), 0, 0, 0 );
  state.glVertexAttribBinding( GLuint(~0), 0 );
  state.glEnableVertexAttribArray( GLuint(~0) );
  state.glDisableVertexAttribArray( GLuint(~0) );

  state.glEnableVertexArrayAttribEXT( 0, REGAL_EMU_MAX_VERTEX_ATTRIBS );
  state.glDisableVertexArrayAttribEXT( 0, REGAL_EMU_MAX_VERTEX_ATTRIBS );
  state.glVertexAttribFormat( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0, 0, GL_TRUE, 0 );
  state.glVertexAttribFormat( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0, 0, GL_FALSE, 0 );
  state.glVertexAttribLFormat( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0, 0, 0 );
  state.glVertexAttribIFormat( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0, 0, 0 );
  state.glVertexAttribBinding( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0 );
  state.glEnableVertexAttribArray( REGAL_EMU_MAX_VERTEX_ATTRIBS );
  state.glDisableVertexAttribArray( REGAL_EMU_MAX_VERTEX_ATTRIBS );

  state.glVertexAttribDivisor( GLuint(~0), 0 );
  state.glVertexBindingDivisor( GLuint(~0), 0 );
  state.glBindVertexBuffers( GLuint(~0), 0, 0, 0, 0);
  state.glBindVertexBuffers( GLuint(~0), 0, (const GLuint*)(0xdead), 0, 0);
  state.glBindVertexBuffers( 0, GLuint(~0), 0, 0, 0);
  state.glBindVertexBuffers( 0, GLuint(~0), (const GLuint*)(0xdead), 0, 0);

  state.glBindVertexBuffer( GLuint(GLuint(~0) - 1), 0, 0, 0);
  state.glBindVertexBuffer( GLuint(~0), 0, 0, 0);

  state.glVertexArrayMultiTexCoordOffsetEXT(0, 0, GLenum(~0), 0, GLenum(0), 0, GLintptr(0) );

  state.glVertexAttribDivisor( REGAL_EMU_MAX_VERTEX_ATTRIBS, 0 );
  state.glVertexAttribDivisor( REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0 );
  state.glVertexBindingDivisor( REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0 );
  state.glBindVertexBuffers( REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0, 0, 0, 0);
  state.glBindVertexBuffers( REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0, (const GLuint*)(0xdead), 0, 0);

  // don't change this 1 to a 0. if you do, among other things glBindVertexBuffer will try to access
  // the bogus pointers being passed in here and you get what you deserve.

  state.glBindVertexBuffers( 1, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0, 0, 0);
  state.glBindVertexBuffers( 1, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, (const GLuint*)(0xdead), 0, 0);

  state.glBindVertexBuffer( REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, 0, 0, 0);

  // Peform a swap, so that it is effectively tested too

  Ppca other;
  checkPpcaDefaults(ctx, other);

  state.VertexArray::swap( other );

  // Verify the expected default state ended up in the original

  checkPpcaDefaults(ctx, state);

  // Verify the modified array data ended up in the other state

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    if ( ii == 4 ) {
      EXPECT_EQ( GLboolean(GL_TRUE),  other.VertexArray::generic[ ii ].enabled );
    } else {
      EXPECT_EQ( GLboolean(GL_FALSE), other.VertexArray::generic[ ii ].enabled );
    }
    EXPECT_EQ( GLuint(ii * 100 + 1),  other.VertexArray::generic[ ii ].size );
    EXPECT_EQ( GLenum(ii * 100 + 2),  other.VertexArray::generic[ ii ].type );
    EXPECT_EQ( GLuint(ii * 100 + 3),  other.VertexArray::generic[ ii ].relativeOffset );
    EXPECT_EQ( GLboolean(ii * 100 + 4),  other.VertexArray::generic[ ii ].normalized );
    EXPECT_EQ( GLboolean(ii * 100 + 5),  other.VertexArray::generic[ ii ].isInteger );
    EXPECT_EQ( GLboolean(ii * 100 + 6),  other.VertexArray::generic[ ii ].isLong );
    EXPECT_EQ( GLuint(ii * 100 + 7),  other.VertexArray::generic[ ii ].bindingIndex );
  }

  // Verify the modified bindings data ended up in the other state

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(ii * 1000 + 1),    other.VertexArray::bindings[ ii ].buffer );
    EXPECT_EQ( GLintptr( ii * 1000 + 2), other.VertexArray::bindings[ ii ].offset );
    EXPECT_EQ( GLsizei(ii * 1000 + 3),   other.VertexArray::bindings[ ii ].stride );
    EXPECT_EQ( GLuint(ii * 1000 + 4),    other.VertexArray::bindings[ ii ].divisor );
  }
}

TEST ( RegalPpca, VertexArrayGenericState_Transition )
{
  RegalGMockInterface mock;

  DispatchTableGL dt;
  ::memset(&dt,0,sizeof(DispatchTableGL));
  dt._enabled = true;
  InitDispatchTableGMock( dt );

  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca current;
  checkPpcaDefaults(ctx, current);

  Ppca target;
  checkPpcaDefaults(ctx, target);

  // An attribute array can be enabled.

  target.glEnableVertexAttribArray( 0 );

  EXPECT_CALL( mock, glEnableVertexAttribArray( 0 ) );

  // An attribute array can be configured.

  target.glVertexAttribFormat ( 1, 11, GLenum(12), GL_FALSE, 13 );
  target.glVertexAttribFormat ( 2, 21, GLenum(22), GL_TRUE,  23 );
  target.glVertexAttribIFormat( 3, 31, GLenum(32), 33 );

  EXPECT_CALL( mock, glVertexAttribFormat ( 1, 11, 12, GL_FALSE, 13 ) );
  EXPECT_CALL( mock, glVertexAttribFormat ( 2, 21, 22, GL_TRUE,  23 ) );
  EXPECT_CALL( mock, glVertexAttribIFormat( 3, 31, 32, 33 ) );

  // An attribute buffer can be configured.

  target.glBindVertexBuffer( 4, 41, 42, 43 );

  EXPECT_CALL( mock, glBindVertexBuffer( 4, 41, 42, 43 ) );

  // An attribute buffer divisor can be configured.

  target.glVertexBindingDivisor( 5, 51 );
  EXPECT_CALL( mock, glVertexBindingDivisor( 5, 51 ) );

  target.glVertexAttribBinding( 6, 7 );
  EXPECT_CALL( mock, glVertexAttribBinding( 6, 7 ) );

  target.glEnableVertexAttribArray( 8 );

  EXPECT_CALL( mock, glEnableVertexAttribArray ( 8 ) );

  // transition, verify, and reset

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  // An attribute can be disabled and all data reset to default.

  target.glDisableVertexAttribArray( 8 );
  target.glVertexAttribFormat( 8, 61, 62, GL_FALSE, 63 );
  target.glBindVertexBuffer( 8, 64, 65, 66 );
  target.glVertexBindingDivisor( 8, 67 );
  target.glVertexAttribBinding( 8, 9 );

  EXPECT_CALL( mock, glDisableVertexAttribArray ( 8 ) );
  EXPECT_CALL( mock, glVertexAttribFormat ( 8, 61, 62, GL_FALSE, 63 ) );
  EXPECT_CALL( mock, glBindVertexBuffer( 8, 64, 65, 66 ) );
  EXPECT_CALL( mock, glVertexBindingDivisor( 8, 67 ) );
  EXPECT_CALL( mock, glVertexAttribBinding( 8, 9 ) );

  // transition, verify, and reset

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  // Identical state is a no-op in terms of calls.

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  current.VertexArray::transition( dt, current, true );
  Mock::VerifyAndClear( &mock );

  target.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  target.Reset(ctx);
  current.Reset(ctx);

  target.glEnableVertexAttribArray( 0 );
  target.glVertexAttribFormat ( 0, 11, GLenum(12), GL_FALSE, 13 );
  target.glBindVertexBuffer( 0, 14, 15, 16 );
  target.glVertexBindingDivisor( 0, 17 );
  target.glVertexAttribBinding( 0, 1 );

  current.glEnableVertexAttribArray( 0 );
  current.glVertexAttribFormat ( 0, 11, GLenum(12), GL_FALSE, 13 );
  current.glBindVertexBuffer( 0, 14, 15, 16 );
  current.glVertexBindingDivisor( 0, 17 );
  current.glVertexAttribBinding( 0, 1 );

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );
}

TEST ( RegalPpca, VertexArray_Transition )
{
  RegalGMockInterface mock;

  DispatchTableGL dt;
  ::memset(&dt,0,sizeof(DispatchTableGL));
  dt._enabled = true;
  InitDispatchTableGMock( dt );

  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca current;
  checkPpcaDefaults(ctx, current);

  Ppca target;
  checkPpcaDefaults(ctx, target);

  // Set up a simple non-default state, focusing on state unique this structure.

  target.elementArrayBufferBinding = GLuint(1);
  target.clientActiveTexture = GLenum(2);
  target.primitiveRestartFixedIndex = GLboolean(GL_TRUE);
  target.primitiveRestart = GLboolean(GL_TRUE);
  target.primitiveRestartIndex = GLuint(3);
  target.arrayBufferBinding = GLuint(4);
  target.vertexArrayBinding = GLuint(5);

  // Set up expectations.

  EXPECT_CALL( mock, glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 1 ) );
  EXPECT_CALL( mock, glClientActiveTexture( 2 ) );
  EXPECT_CALL( mock, glEnable( GL_PRIMITIVE_RESTART_FIXED_INDEX ) );
  EXPECT_CALL( mock, glEnable( GL_PRIMITIVE_RESTART ) );
  EXPECT_CALL( mock, glPrimitiveRestartIndex( 3 ) );
  EXPECT_CALL( mock, glBindBuffer( GL_ARRAY_BUFFER, 4 ) );
  EXPECT_CALL( mock, glBindVertexArray( 5 ) );

  // Perform the requested state transition.

  current.VertexArray::transition( dt, target, true );

  // Verify the call expectations, and reset for another test.

  Mock::VerifyAndClear( &mock );

  // unfortunately even though this call to transition() won't actually
  // do anything, the current implementation will clear the bound vertex
  // array, do the changes (which turns out to be none), then bind back
  // to the original vertex array

  EXPECT_CALL( mock, glBindVertexArray( 0 ) );
  EXPECT_CALL( mock, glBindVertexArray( 5 ) );

  // Perform the requested state transition.

  current.VertexArray::transition( dt, target, true );

  // Verify the call expectations, and reset for another test.

  Mock::VerifyAndClear( &mock );

  // A transition with no differences should make no calls, but...
  // see above comment.

  EXPECT_CALL( mock, glBindVertexArray( 0 ) );
  EXPECT_CALL( mock, glBindVertexArray( 5 ) );

  current.VertexArray::transition( dt, current, true );
  Mock::VerifyAndClear( &mock );

  EXPECT_CALL( mock, glBindVertexArray( 0 ) );
  EXPECT_CALL( mock, glBindVertexArray( 5 ) );

  target.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  current.Reset(ctx);
  target.Reset(ctx);

  // Now there really won't be any calls since both the
  // current and target vertex array to bind is 0

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );

  // set some state other than the vertex array binding

  target.elementArrayBufferBinding = GLuint(1);
  target.clientActiveTexture = GLenum(2);
  target.primitiveRestartFixedIndex = GLboolean(GL_TRUE);
  target.primitiveRestart = GLboolean(GL_TRUE);
  target.primitiveRestartIndex = GLuint(3);
  target.arrayBufferBinding = GLuint(4);

  current.elementArrayBufferBinding = GLuint(1);
  current.clientActiveTexture = GLenum(2);
  current.primitiveRestartFixedIndex = GLboolean(GL_TRUE);
  current.primitiveRestart = GLboolean(GL_TRUE);
  current.primitiveRestartIndex = GLuint(3);
  current.arrayBufferBinding = GLuint(4);

  // and again no calls.

  current.VertexArray::transition( dt, target, true );
  Mock::VerifyAndClear( &mock );
}

TEST ( RegalPpca, PixelStore_Transition )
{
  RegalGMockInterface mock;

  DispatchTableGL dt;
  ::memset(&dt,0,sizeof(DispatchTableGL));
  dt._enabled = true;
  InitDispatchTableGMock( dt );

  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca current;
  checkPpcaDefaults(ctx, current);

  Ppca target;
  checkPpcaDefaults(ctx, target);

  // Set some of the named attributes, and expect that calls will be made
  // appropriately to the backend to transition to those value.

  current.PixelStore::unpackSkipPixels        = GLuint(123);
  current.PixelStore::unpackAlignment         = GLuint(456);

  target.PixelStore::pixelPackBufferBinding   = GLuint(321);
  target.PixelStore::pixelUnpackBufferBinding = GLuint(654);

  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_SKIP_PIXELS ,   0 ) );
  EXPECT_CALL( mock, glPixelStorei( GL_UNPACK_ALIGNMENT   ,   4 ) );
  EXPECT_CALL( mock, glBindBuffer ( GL_PIXEL_PACK_BUFFER  , 321 ) );
  EXPECT_CALL( mock, glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, 654 ) );

  // Perform the state transition and verify expectations

  current.PixelStore::transition( dt, target );
  Mock::VerifyAndClear( &mock );

  // A transition with no differences should make no calls.

  current.PixelStore::transition( dt, current );
  Mock::VerifyAndClear( &mock );

  target.PixelStore::transition( dt, target );
  Mock::VerifyAndClear( &mock );

  current.Reset(ctx);
  target.Reset(ctx);

  current.PixelStore::transition( dt, target );
  Mock::VerifyAndClear( &mock );

  target.PixelStore::unpackSkipPixels          = GLuint(123);
  target.PixelStore::unpackAlignment           = GLuint(456);
  target.PixelStore::pixelPackBufferBinding    = GLuint(321);
  target.PixelStore::pixelUnpackBufferBinding  = GLuint(654);

  current.PixelStore::unpackSkipPixels         = GLuint(123);
  current.PixelStore::unpackAlignment          = GLuint(456);
  current.PixelStore::pixelPackBufferBinding   = GLuint(321);
  current.PixelStore::pixelUnpackBufferBinding = GLuint(654);

  current.PixelStore::transition( dt, target );
  Mock::VerifyAndClear( &mock );
}

TEST ( RegalPpca, VertexArray_Generic )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // VertexAttribFormat

  ppca.glVertexAttribFormat ( 3, 4, GL_FLOAT, GL_TRUE, 123 );

  EXPECT_EQ( GLuint(4),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(123),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );

  ppca.Reset(ctx);
  ppca.glVertexAttribFormat ( 3, 4, GL_FLOAT, GL_FALSE, 123 );

  EXPECT_EQ( GLuint(4),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(123),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );

  ppca.Reset(ctx);
  ppca.glVertexAttribIFormat( 3, 1, GL_INT, 456 );

  EXPECT_EQ( GLuint(1),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_INT),      ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(456),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );

  ppca.Reset(ctx);
  ppca.glVertexAttribLFormat( 3, 2, GL_DOUBLE, 789 );

  EXPECT_EQ( GLuint(2),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_DOUBLE),   ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(789),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isLong );

  // BindVertexBuffer

  ppca.Reset(ctx);
  ppca.glBindVertexBuffer( 4, 5, 6, 7 );

  EXPECT_EQ( GLuint(5),   ppca.VertexArray::bindings[4].buffer );
  EXPECT_EQ( GLintptr(6), ppca.VertexArray::bindings[4].offset );
  EXPECT_EQ( GLsizei(7),  ppca.VertexArray::bindings[4].stride );
  EXPECT_EQ( GLuint(0),   ppca.VertexArray::bindings[4].divisor );

  // VertexAttribBinding

  ppca.Reset(ctx);
  ppca.glVertexAttribBinding( 3, 4 );

  EXPECT_EQ( GLuint(4), ppca.VertexArray::generic[3].bindingIndex );

  // VertexAttribPointer

  ppca.Reset(ctx);
  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribPointer ( 3, 1, GL_FLOAT, GL_TRUE, 123, reinterpret_cast<const GLvoid *>( 321 ) );

  EXPECT_EQ( GLuint(1),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(321), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(123),  ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribPointer ( 3, 1, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>( 321 ) );

  EXPECT_EQ( GLuint(1),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(321), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(4),    ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribIPointer ( 3, 2, GL_INT, 456, reinterpret_cast<GLvoid *>( 654 ) );

  EXPECT_EQ( GLuint(2),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_INT),      ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(654), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(456),  ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribIPointer( 3, 2, GL_INT, 0, reinterpret_cast<GLvoid *>( 654 ) );

  EXPECT_EQ( GLuint(2),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_INT),      ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(654), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(8),    ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribLPointer( 3, 3, GL_DOUBLE, 789, reinterpret_cast<GLvoid *>( 987 ) );

  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_DOUBLE),   ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(987), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(789),  ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.VertexArray::arrayBufferBinding = 8888;
  ppca.glVertexAttribLPointer( 3, 3, GL_DOUBLE, 0, reinterpret_cast<GLvoid *>( 987 ) );

  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_DOUBLE),   ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(8888),  ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(987), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(24),   ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  // Enable/DisableVertexAttribArray

  ppca.Reset(ctx);
  ppca.glEnableVertexAttribArray( 3 );

  EXPECT_EQ( GLboolean( GL_TRUE ), ppca.VertexArray::generic[ 3 ].enabled );

  ppca.glDisableVertexAttribArray( 3 );

  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::generic[ 3 ].enabled );

  // VertexBindingDivisor

  ppca.Reset(ctx);
  ppca.glVertexBindingDivisor( 4, 123 );

  EXPECT_EQ( GLuint(123), ppca.VertexArray::bindings[4].divisor );

  // VertexAttribDivisor

  ppca.Reset(ctx);
  ppca.glVertexAttribDivisor( 3, 456 );

  EXPECT_EQ( GLuint(3),   ppca.VertexArray::generic[3].bindingIndex );
  EXPECT_EQ( GLuint(456), ppca.VertexArray::bindings[3].divisor );

  // ShadowVertexArrayVertexAttribOffsetEXT

  ppca.Reset(ctx);
  ppca.glVertexArrayVertexAttribOffsetEXT( 0, 987, 3, 1, GL_FLOAT, GL_TRUE, 123, 321 );

  EXPECT_EQ( GLuint(1),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint( 0 ),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(987),   ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(321), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(123),  ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.glVertexArrayVertexAttribOffsetEXT( 0, 987, 3, 1, GL_FLOAT, GL_FALSE, 0, 321 );

  EXPECT_EQ( GLuint(1),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_FLOAT),    ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint( 0 ),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(987),   ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(321), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(4),    ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.glVertexArrayVertexAttribIOffsetEXT( 0, 987, 3, 2, GL_INT, 456, 654 );

  EXPECT_EQ( GLuint(2),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_INT),      ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint( 0 ),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(987),   ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(654), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(456),  ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  ppca.Reset(ctx);
  ppca.glVertexArrayVertexAttribIOffsetEXT( 0, 987, 3, 2, GL_INT, 0, 654 );

  EXPECT_EQ( GLuint(2),           ppca.VertexArray::generic[3].size );
  EXPECT_EQ( GLenum(GL_INT),      ppca.VertexArray::generic[3].type );
  EXPECT_EQ( GLuint( 0 ),         ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].normalized );
  EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[3].isInteger );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[3].isLong );
  EXPECT_EQ( GLuint(3),           ppca.VertexArray::generic[3].bindingIndex );

  EXPECT_EQ( GLuint(987),   ppca.VertexArray::bindings[3].buffer );
  EXPECT_EQ( GLintptr(654), ppca.VertexArray::bindings[3].offset );
  EXPECT_EQ( GLsizei(8),    ppca.VertexArray::bindings[3].stride );
  EXPECT_EQ( GLuint(0),     ppca.VertexArray::bindings[3].divisor );

  // EnableDisableVertexArrayAttribEXT

  ppca.Reset(ctx);
  ppca.glEnableVertexArrayAttribEXT( 0, 3 );

  EXPECT_EQ( GLboolean( GL_TRUE ), ppca.VertexArray::generic [ 3 ].enabled );

  ppca.glDisableVertexArrayAttribEXT( 0, 3 );

  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::generic [ 3 ].enabled );

  // If the vertex array binding is nonzero, none of these calls should do
  // anything (since we do not actually track internal state for vertex array
  // objects.

  ppca.Reset(ctx);
  ppca.VertexArray::generic[ 3 ].relativeOffset = GLuint(123);
  ppca.VertexArray::generic[ 3 ].bindingIndex = 4;
  ppca.VertexArray::bindings[ 4 ].buffer = 456;
  ppca.VertexArray::bindings[ 4 ].divisor = 789;

  ppca.VertexArray::vertexArrayBinding = 1;

  ppca.glVertexAttribFormat ( 3, 3, GL_FLOAT, GL_TRUE, 0 );
  ppca.glVertexAttribIFormat( 3, 3, GL_INT, 0 );
  ppca.glVertexAttribLFormat( 3, 3, GL_DOUBLE, 0 );
  ppca.glBindVertexBuffer( 4, 0, 0, 0 );
  ppca.glVertexAttribBinding( 3, 0 );
  ppca.glVertexAttribPointer ( 3, 1, GL_FLOAT,  GL_TRUE, 0, NULL );
  ppca.glVertexAttribIPointer( 3, 2, GL_INT,    0, NULL );
  ppca.glVertexAttribLPointer( 3, 3, GL_DOUBLE, 0, NULL );
  ppca.glVertexBindingDivisor( 4, 0 );
  ppca.glVertexArrayVertexAttribOffsetEXT ( 1, 0, 3, 1, GL_FLOAT, GL_TRUE, 0, 0 );
  ppca.glVertexArrayVertexAttribIOffsetEXT( 1, 0, 3, 2, GL_INT, 0, 0 );

  EXPECT_EQ( GLuint(123), ppca.VertexArray::generic[3].relativeOffset );
  EXPECT_EQ( GLuint(4),   ppca.VertexArray::generic[3].bindingIndex );
  EXPECT_EQ( GLuint(456), ppca.VertexArray::bindings[4].buffer );
  EXPECT_EQ( GLuint(789), ppca.VertexArray::bindings[4].divisor );

  ppca.VertexArray::generic [ 3 ].enabled = GL_FALSE;
  ppca.glEnableVertexAttribArray( 3 );
  ppca.glEnableVertexArrayAttribEXT( 1, 3 );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::generic [ 3 ].enabled );

  ppca.VertexArray::generic [ 3 ].enabled = GL_TRUE;
  ppca.glDisableVertexAttribArray( 3 );
  ppca.glDisableVertexArrayAttribEXT( 1, 3 );
  EXPECT_EQ( GLboolean( GL_TRUE ), ppca.VertexArray::generic [ 3 ].enabled );
}

TEST ( RegalPpca, VertexArray_Named )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glEnableClientState( GL_COLOR_ARRAY );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );

  ppca.glDisableClientState( GL_COLOR_ARRAY );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );

  EXPECT_EQ( GLenum( GL_TEXTURE0 ), ppca.VertexArray::clientActiveTexture );

  ppca.glClientActiveTexture( GL_TEXTURE2 );
  EXPECT_EQ( GLenum( GL_TEXTURE2 ), ppca.VertexArray::clientActiveTexture );

  ppca.glEnableClientState( GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_TRUE ),   ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glEnableVertexArrayEXT( 0, GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_TRUE ),   ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glDisableVertexArrayEXT( 0, GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glEnableVertexArrayEXT( 1, GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glDisableVertexArrayEXT( 1, GL_TEXTURE_COORD_ARRAY );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].enabled );

  ppca.glEnableClientStateIndexedEXT( GL_TEXTURE_COORD_ARRAY, 5 );
  EXPECT_EQ( GLboolean( GL_TRUE ),   ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].enabled );

  ppca.glDisableClientStateIndexedEXT( GL_TEXTURE_COORD_ARRAY, 5 );
  EXPECT_EQ( GLboolean( GL_FALSE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].enabled );

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.VertexArray::named[ii].enabled = GLboolean(GL_FALSE);
    ppca.VertexArray::named[ii].pointer = reinterpret_cast<const GLvoid*>(ii);
    ppca.VertexArray::named[ii].buffer  = GLuint(ii);
    ppca.VertexArray::named[ii].size    = GLint(ii);
    ppca.VertexArray::named[ii].type    = GLenum(ii);
    ppca.VertexArray::named[ii].stride  = GLsizei(ii);
  }

  ppca.VertexArray::arrayBufferBinding = 123;

  ppca.glVertexPointer( 4, GL_FLOAT, 1001, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::VERTEX ].buffer );
  EXPECT_EQ( GLint(4),           ppca.VertexArray::named[ ClientState::VERTEX ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::VERTEX ].type );
  EXPECT_EQ( GLsizei(1001),      ppca.VertexArray::named[ ClientState::VERTEX ].stride );

  ppca.glNormalPointer( GL_FLOAT, 1002, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::NORMAL ].buffer );
  EXPECT_EQ( GLint(3),           ppca.VertexArray::named[ ClientState::NORMAL ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::NORMAL ].type );
  EXPECT_EQ( GLsizei(1002),      ppca.VertexArray::named[ ClientState::NORMAL ].stride );

  ppca.glColorPointer( 4, GL_FLOAT, 1003, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::COLOR ].buffer );
  EXPECT_EQ( GLint(4),           ppca.VertexArray::named[ ClientState::COLOR ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::COLOR ].type );
  EXPECT_EQ( GLsizei(1003),      ppca.VertexArray::named[ ClientState::COLOR ].stride );

  ppca.glSecondaryColorPointer( 3, GL_FLOAT, 1004, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].buffer );
  EXPECT_EQ( GLint(3),           ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].type );
  EXPECT_EQ( GLsizei(1004),      ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].stride );

  ppca.glIndexPointer( GL_INT, 1005, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::INDEX ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::INDEX ].size );
  EXPECT_EQ( GLenum( GL_INT ),   ppca.VertexArray::named[ ClientState::INDEX ].type );
  EXPECT_EQ( GLsizei(1005),      ppca.VertexArray::named[ ClientState::INDEX ].stride );

  ppca.glEdgeFlagPointer( 1006, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::EDGE_FLAG ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::EDGE_FLAG ].size );
  EXPECT_EQ( GLenum( GL_BOOL ),  ppca.VertexArray::named[ ClientState::EDGE_FLAG ].type );
  EXPECT_EQ( GLsizei(1006),      ppca.VertexArray::named[ ClientState::EDGE_FLAG ].stride );

  ppca.glFogCoordPointer( GL_FLOAT, 1007, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::FOG_COORD ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::FOG_COORD ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::FOG_COORD ].type );
  EXPECT_EQ( GLsizei(1007),      ppca.VertexArray::named[ ClientState::FOG_COORD ].stride );

  ppca.glTexCoordPointer( 2, GL_FLOAT, 1008, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::TEX_COORD + 2].buffer );
  EXPECT_EQ( GLint(2),           ppca.VertexArray::named[ ClientState::TEX_COORD + 2].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::TEX_COORD + 2].type );
  EXPECT_EQ( GLsizei(1008),      ppca.VertexArray::named[ ClientState::TEX_COORD + 2].stride );

  ppca.glMultiTexCoordPointerEXT( GL_TEXTURE5, 2, GL_FLOAT, 2005, NULL );
  EXPECT_EQ( GLuint(123),        ppca.VertexArray::named[ ClientState::TEX_COORD + 5].buffer );
  EXPECT_EQ( GLint(2),           ppca.VertexArray::named[ ClientState::TEX_COORD + 5].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::TEX_COORD + 5].type );
  EXPECT_EQ( GLsizei(2005),      ppca.VertexArray::named[ ClientState::TEX_COORD + 5].stride );

  ppca.glVertexArrayVertexOffsetEXT( 0, 3001, 3, GL_FLOAT, 3002, 0 );
  EXPECT_EQ( GLuint(3001),       ppca.VertexArray::named[ ClientState::VERTEX ].buffer );
  EXPECT_EQ( GLint(3),           ppca.VertexArray::named[ ClientState::VERTEX ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::VERTEX ].type );
  EXPECT_EQ( GLsizei(3002),      ppca.VertexArray::named[ ClientState::VERTEX ].stride );

  ppca.glVertexArrayColorOffsetEXT ( 0, 3003, 4, GL_FLOAT, 3004, 0 );
  EXPECT_EQ( GLuint(3003),       ppca.VertexArray::named[ ClientState::COLOR ].buffer );
  EXPECT_EQ( GLint(4),           ppca.VertexArray::named[ ClientState::COLOR ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::COLOR ].type );
  EXPECT_EQ( GLsizei(3004),      ppca.VertexArray::named[ ClientState::COLOR ].stride );

  ppca.glVertexArrayEdgeFlagOffsetEXT ( 0, 3005, 3006, 0 );
  EXPECT_EQ( GLuint(3005),       ppca.VertexArray::named[ ClientState::EDGE_FLAG ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::EDGE_FLAG ].size );
  EXPECT_EQ( GLenum( GL_BOOL ),  ppca.VertexArray::named[ ClientState::EDGE_FLAG ].type );
  EXPECT_EQ( GLsizei(3006),      ppca.VertexArray::named[ ClientState::EDGE_FLAG ].stride );

  ppca.glVertexArrayIndexOffsetEXT ( 0, 3007, GL_INT, 3008, 0 );
  EXPECT_EQ( GLuint(3007),       ppca.VertexArray::named[ ClientState::INDEX ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::INDEX ].size );
  EXPECT_EQ( GLenum( GL_INT ),   ppca.VertexArray::named[ ClientState::INDEX ].type );
  EXPECT_EQ( GLsizei(3008),      ppca.VertexArray::named[ ClientState::INDEX ].stride );

  ppca.glVertexArrayNormalOffsetEXT ( 0, 3009, GL_FLOAT, 3010, 0 );
  EXPECT_EQ( GLuint(3009),       ppca.VertexArray::named[ ClientState::NORMAL ].buffer );
  EXPECT_EQ( GLint(3),           ppca.VertexArray::named[ ClientState::NORMAL ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::NORMAL ].type );
  EXPECT_EQ( GLsizei(3010),      ppca.VertexArray::named[ ClientState::NORMAL ].stride );

  ppca.glVertexArrayTexCoordOffsetEXT( 0, 3011, 2, GL_FLOAT, 3012, 0 );
  EXPECT_EQ( GLuint(3011),       ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].buffer );
  EXPECT_EQ( GLint(2),           ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].type );
  EXPECT_EQ( GLsizei(3012),      ppca.VertexArray::named[ ClientState::TEX_COORD + 2 ].stride );

  ppca.glVertexArrayMultiTexCoordOffsetEXT( 0, 3013, GL_TEXTURE5, 2, GL_FLOAT, 3014, 0 );
  EXPECT_EQ( GLuint(3013),       ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].buffer );
  EXPECT_EQ( GLint(2),           ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].type );
  EXPECT_EQ( GLsizei(3014),      ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].stride );

  ppca.glVertexArrayFogCoordOffsetEXT ( 0, 3015, GL_FLOAT, 3016, 0 );
  EXPECT_EQ( GLuint(3015),       ppca.VertexArray::named[ ClientState::FOG_COORD ].buffer );
  EXPECT_EQ( GLint(1),           ppca.VertexArray::named[ ClientState::FOG_COORD ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::FOG_COORD ].type );
  EXPECT_EQ( GLsizei(3016),      ppca.VertexArray::named[ ClientState::FOG_COORD ].stride );

  ppca.glVertexArraySecondaryColorOffsetEXT ( 0, 3017, 3, GL_FLOAT, 3018, 0 );
  EXPECT_EQ( GLuint(3017),       ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].buffer );
  EXPECT_EQ( GLint(3),           ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ), ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].type );
  EXPECT_EQ( GLsizei(3018),      ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].stride );
}

TEST ( RegalPpca, glDeleteBuffers_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  GLuint buffers[ 2 ] = { 0, 123 };

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer  = GLuint(123);
  }
  ppca.VertexArray::elementArrayBufferBinding = GLuint(123);
  ppca.VertexArray::arrayBufferBinding = GLuint(123);
  ppca.PixelStore::pixelPackBufferBinding = GLuint(123);
  ppca.PixelStore::pixelUnpackBufferBinding = GLuint(123);

  ppca.glDeleteBuffers( 2, buffers );

  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer  = GLuint(456);
  }
  ppca.VertexArray::elementArrayBufferBinding = GLuint(456);
  ppca.VertexArray::arrayBufferBinding = GLuint(456);
  ppca.PixelStore::pixelPackBufferBinding = GLuint(456);
  ppca.PixelStore::pixelUnpackBufferBinding = GLuint(456);

  ppca.glDeleteBuffers( 2, buffers );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(456), ppca.VertexArray::bindings[ii].buffer );
  }
  EXPECT_EQ( GLuint(456), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint(456), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(456), ppca.PixelStore::pixelPackBufferBinding );
  EXPECT_EQ( GLuint(456), ppca.PixelStore::pixelUnpackBufferBinding );
}

TEST ( RegalPpca, glDeleteVertexArrays_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  GLuint buffers[ 2 ] = { 0, 123 };

  ppca.VertexArray::vertexArrayBinding = 123;

  ppca.glDeleteVertexArrays( 2, buffers );

  EXPECT_EQ( GLuint(0), ppca.VertexArray::vertexArrayBinding );

  ppca.VertexArray::vertexArrayBinding = 456;

  ppca.glDeleteVertexArrays( 2, buffers );

  EXPECT_EQ( GLuint(456), ppca.VertexArray::vertexArrayBinding );
}

TEST ( RegalPpca, glPrimitiveRestart_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  EXPECT_EQ( GLboolean(GL_FALSE), ppca.primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.primitiveRestartFixedIndex );
  EXPECT_EQ( GLuint(0),           ppca.VertexArray::primitiveRestartIndex );

  // test glEnable & glDisable

  ppca.glEnable( GL_PRIMITIVE_RESTART );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisable( GL_PRIMITIVE_RESTART );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glEnable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisable( GL_PRIMITIVE_RESTART_FIXED_INDEX );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );

  ppca.glEnable( GL_TEXTURE_GEN_S );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisable( GL_TEXTURE_GEN_S );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.VertexArray::primitiveRestart = GL_TRUE;
  ppca.VertexArray::primitiveRestartFixedIndex = GL_TRUE;
  ppca.glEnable( GL_TEXTURE_GEN_S );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisable( GL_TEXTURE_GEN_S );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );

  // test glEnablei & glDisablei

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glEnablei( GL_PRIMITIVE_RESTART, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisablei( GL_PRIMITIVE_RESTART, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glEnablei( GL_PRIMITIVE_RESTART_FIXED_INDEX, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisablei( GL_PRIMITIVE_RESTART_FIXED_INDEX, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );

  ppca.glEnablei( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisablei( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.VertexArray::primitiveRestart = GL_TRUE;
  ppca.VertexArray::primitiveRestartFixedIndex = GL_TRUE;
  ppca.glEnablei( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisablei( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );

  // test glEnableIndexedEXT & glDisableIndexedEXT

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glEnableIndexedEXT( GL_PRIMITIVE_RESTART, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisableIndexedEXT( GL_PRIMITIVE_RESTART, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glEnableIndexedEXT( GL_PRIMITIVE_RESTART_FIXED_INDEX, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisableIndexedEXT( GL_PRIMITIVE_RESTART_FIXED_INDEX, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );

  ppca.glEnableIndexedEXT( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisableIndexedEXT( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.VertexArray::primitiveRestart = GL_TRUE;
  ppca.VertexArray::primitiveRestartFixedIndex = GL_TRUE;
  ppca.glEnableIndexedEXT( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );
  ppca.glDisableIndexedEXT( GL_TEXTURE_GEN_S, 0 );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestart );
  EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::primitiveRestartFixedIndex );

  // test glPrimitiveRestartIndex

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<13; ii++)
  {
    ppca.glPrimitiveRestartIndex( ii );
    EXPECT_EQ( GLuint(ii), ppca.VertexArray::primitiveRestartIndex );
  }

  ppca.glPrimitiveRestartIndex( GLuint(~0) );
  EXPECT_EQ( GLuint(~0), ppca.VertexArray::primitiveRestartIndex );
}

TEST ( RegalPpca, glInterleavedArrays_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // Do a comprehensive test on all settings for GL_T4F_C4F_N3F_V4F

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.VertexArray::named[ ii ].enabled = GLboolean( ( ii & 1 ) == 0 );
    ppca.VertexArray::named[ ii ].size    = GLint( 987 );
    ppca.VertexArray::named[ ii ].type    = GLenum( 987 );
    ppca.VertexArray::named[ ii ].stride  = GLsizei( 987 );
    ppca.VertexArray::named[ ii ].pointer = reinterpret_cast<const GLvoid*>( 987 );
  }

  ppca.glClientActiveTexture( GL_TEXTURE5 );
  ppca.glInterleavedArrays( GL_T4F_C4F_N3F_V4F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX ].enabled );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::VERTEX ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX ].type );
  EXPECT_EQ( GLsizei( 60 ),         ppca.VertexArray::named[ ClientState::VERTEX ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 44 ), ppca.VertexArray::named[ ClientState::VERTEX ].pointer );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::NORMAL ].enabled );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL ].type );
  EXPECT_EQ( GLsizei( 60 ),         ppca.VertexArray::named[ ClientState::NORMAL ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 32 ), ppca.VertexArray::named[ ClientState::NORMAL ].pointer );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR ].enabled );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR ].type );
  EXPECT_EQ( GLsizei( 60 ),         ppca.VertexArray::named[ ClientState::COLOR ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 16 ), ppca.VertexArray::named[ ClientState::COLOR ].pointer );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].enabled );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].size );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].type );
  EXPECT_EQ( GLsizei( 60 ),         ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].pointer );

  // The other non-texture coordinate arrays should be disabled

  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::EDGE_FLAG       ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::FOG_COORD       ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::INDEX           ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].enabled );

  // The other non-texture coordinate arrays should not otherwise be touched.

  EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ClientState::EDGE_FLAG       ].size );
  EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ClientState::EDGE_FLAG       ].type );
  EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ClientState::EDGE_FLAG       ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ClientState::EDGE_FLAG       ].pointer );

  EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ClientState::FOG_COORD       ].size );
  EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ClientState::FOG_COORD       ].type );
  EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ClientState::FOG_COORD       ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ClientState::FOG_COORD       ].pointer );

  EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ClientState::INDEX           ].size );
  EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ClientState::INDEX           ].type );
  EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ClientState::INDEX           ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ClientState::INDEX           ].pointer );

  EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].size );
  EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].type );
  EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ClientState::SECONDARY_COLOR ].pointer );

  // Verify other texture coordinate settings unaffected.

  for (GLuint ii=0; ii<REGAL_EMU_MAX_TEXTURE_COORDS; ii++)
  {
    if ( ii == 5 )
      continue;

    GLboolean b = ( ( ( ClientState::TEX_COORD + ii ) & 1 ) == 0 ) ? GL_TRUE : GL_FALSE;

    EXPECT_EQ( GLboolean( b ),         ppca.VertexArray::named[ ClientState::TEX_COORD + ii ].enabled );
    EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ClientState::TEX_COORD + ii ].size );
    EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ClientState::TEX_COORD + ii ].type );
    EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ClientState::TEX_COORD + ii ].stride );
    EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ClientState::TEX_COORD + ii ].pointer );
  }

  // Ensure if stride is nonzero, it is used as is, and ensure the pointer passed in is used as a base address.

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glClientActiveTexture( GL_TEXTURE5 );
  ppca.glInterleavedArrays( GL_T4F_C4F_N3F_V4F, 321, reinterpret_cast<GLvoid *>( 5000 ) );

  EXPECT_EQ( GLsizei( 321 ),          ppca.VertexArray::named[ ClientState::VERTEX        ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 5044 ), ppca.VertexArray::named[ ClientState::VERTEX        ].pointer );
  EXPECT_EQ( GLsizei( 321 ),          ppca.VertexArray::named[ ClientState::NORMAL        ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 5032 ), ppca.VertexArray::named[ ClientState::NORMAL        ].pointer );
  EXPECT_EQ( GLsizei( 321 ),          ppca.VertexArray::named[ ClientState::COLOR         ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 5016 ), ppca.VertexArray::named[ ClientState::COLOR         ].pointer );
  EXPECT_EQ( GLsizei( 321 ),          ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].stride );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 5000 ), ppca.VertexArray::named[ ClientState::TEX_COORD + 5 ].pointer );

  // Do a quick run through the remaining formats, and do some quick verifications.

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_V2F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 8 ),          ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 12 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_C4UB_V2F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_UNSIGNED_BYTE ), ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 12 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 12 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 4 ),  ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_C4UB_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_UNSIGNED_BYTE ), ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 16 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 16 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 4 ),  ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_C3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 12 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_N3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 12 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_C4F_N3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 40 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 40 ),         ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 40 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 28 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 16 ), ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T2F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 20 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 20 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 8 ),  ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T4F_V4F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 16 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T2F_C4UB_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_UNSIGNED_BYTE ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 24 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 12 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 8 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T2F_C3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 20 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 8 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T2F_N3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_FALSE ), ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 0 ),          ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 32 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 20 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 8 ),  ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glInterleavedArrays( GL_T2F_C4F_N3F_V3F, 0, NULL );

  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::VERTEX    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::NORMAL    ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::COLOR     ].enabled );
  EXPECT_EQ( GLboolean( GL_TRUE ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].enabled );

  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::VERTEX    ].size );
  EXPECT_EQ( GLint( 3 ),            ppca.VertexArray::named[ ClientState::NORMAL    ].size );
  EXPECT_EQ( GLint( 4 ),            ppca.VertexArray::named[ ClientState::COLOR     ].size );
  EXPECT_EQ( GLint( 2 ),            ppca.VertexArray::named[ ClientState::TEX_COORD ].size );

  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::VERTEX    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::NORMAL    ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::COLOR     ].type );
  EXPECT_EQ( GLenum( GL_FLOAT ),    ppca.VertexArray::named[ ClientState::TEX_COORD ].type );

  EXPECT_EQ( GLsizei( 48 ),         ppca.VertexArray::named[ ClientState::VERTEX    ].stride );
  EXPECT_EQ( GLsizei( 48 ),         ppca.VertexArray::named[ ClientState::NORMAL    ].stride );
  EXPECT_EQ( GLsizei( 48 ),         ppca.VertexArray::named[ ClientState::COLOR     ].stride );
  EXPECT_EQ( GLsizei( 48 ),         ppca.VertexArray::named[ ClientState::TEX_COORD ].stride );

  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 36 ), ppca.VertexArray::named[ ClientState::VERTEX    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 24 ), ppca.VertexArray::named[ ClientState::NORMAL    ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 8 ),  ppca.VertexArray::named[ ClientState::COLOR     ].pointer );
  EXPECT_EQ( reinterpret_cast<const GLvoid*>( 0 ),  ppca.VertexArray::named[ ClientState::TEX_COORD ].pointer );

  // Pass in an unsupported "format", which should do nothing.

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.VertexArray::named[ ii ].enabled = GLboolean( ( ii & 1 ) == 0 );
    ppca.VertexArray::named[ ii ].size    = GLint( 987 );
    ppca.VertexArray::named[ ii ].type    = GLenum( 987 );
    ppca.VertexArray::named[ ii ].stride  = GLsizei( 987 );
    ppca.VertexArray::named[ ii ].pointer = reinterpret_cast<const GLvoid*>( 987 );
  }

  ppca.glInterleavedArrays( GL_RGBA, 0, NULL );

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    GLboolean b = ( ( ( ii ) & 1 ) == 0 ) ? GL_TRUE : GL_FALSE;

    EXPECT_EQ( GLboolean( b ),         ppca.VertexArray::named[ ii ].enabled );
    EXPECT_EQ( GLint( 987 ),           ppca.VertexArray::named[ ii ].size );
    EXPECT_EQ( GLenum( 987 ),          ppca.VertexArray::named[ ii ].type );
    EXPECT_EQ( GLsizei( 987 ),         ppca.VertexArray::named[ ii ].stride );
    EXPECT_EQ( reinterpret_cast<const GLvoid*>( 987 ), ppca.VertexArray::named[ ii ].pointer );
  }
}

TEST ( RegalPpca, glGet_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();
  ctx.info->max_client_attrib_stack_depth = 16;

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  GLint resulti[ 1 ] = { 123 };
  GLint64 resulti64[ 1 ] = { 123 };
  GLfloat resultf[ 1 ] = { 123.f };
  GLdouble resultd[ 1 ] = { 123. };
  GLboolean resultb[ 1 ] = { GL_FALSE };

  // First ensure getting an unimplemented value works (does nothing).

  EXPECT_EQ( false, ppca.glGetv( ctx, GL_FLOAT, resulti ) );
  EXPECT_EQ( 123, resulti[ 0 ] );

  EXPECT_EQ( false, ppca.glGetv( ctx, GL_FLOAT, resulti64 ) );
  EXPECT_EQ( 123, resulti64[ 0 ] );

  EXPECT_EQ( false, ppca.glGetv( ctx, GL_FLOAT, resultf ) );
  EXPECT_EQ( 123, resultf[ 0 ] );

  EXPECT_EQ( false, ppca.glGetv( ctx, GL_FLOAT, resultd ) );
  EXPECT_EQ( 123, resultd[ 0 ] );

  EXPECT_EQ( false, ppca.glGetv( ctx, GL_FLOAT, resultb ) );
  EXPECT_EQ( GLboolean(GL_FALSE), resultb[ 0 ] );

  // Next verify that getting an implemented value gets the value.

  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti ) );
  EXPECT_EQ( REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti[ 0 ] );

  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti64 ) );
  EXPECT_EQ( REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH, resulti64[ 0 ] );

  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultf ) );
  EXPECT_EQ( REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultf[ 0 ] );

  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultd ) );
  EXPECT_EQ( REGAL_EMU_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultd[ 0 ] );

  EXPECT_EQ( true, ppca.glGetv( ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, resultb ) );
  EXPECT_EQ( GLboolean(GL_TRUE), resultb[ 0 ] );
}

TEST ( RegalPpca, glPixelStore_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // first test glPixelStorei

  ppca.glPixelStore( GL_UNPACK_SWAP_BYTES, GLint(GL_TRUE) );
  ppca.glPixelStore( GL_UNPACK_LSB_FIRST, GLint(GL_TRUE) );
  ppca.glPixelStore( GL_UNPACK_IMAGE_HEIGHT, GLint(11) );
  ppca.glPixelStore( GL_UNPACK_SKIP_IMAGES, GLint(12) );
  ppca.glPixelStore( GL_UNPACK_ROW_LENGTH, GLint(13) );
  ppca.glPixelStore( GL_UNPACK_SKIP_ROWS, GLint(14) );
  ppca.glPixelStore( GL_UNPACK_SKIP_PIXELS, GLint(15) );
  ppca.glPixelStore( GL_UNPACK_ALIGNMENT, GLint(16) );
  ppca.glPixelStore( GL_PACK_SWAP_BYTES, GLint(GL_TRUE) );
  ppca.glPixelStore( GL_PACK_LSB_FIRST, GLint(GL_TRUE) );
  ppca.glPixelStore( GL_PACK_IMAGE_HEIGHT, GLint(27) );
  ppca.glPixelStore( GL_PACK_SKIP_IMAGES, GLint(28) );
  ppca.glPixelStore( GL_PACK_ROW_LENGTH, GLint(29) );
  ppca.glPixelStore( GL_PACK_SKIP_ROWS, GLint(30) );
  ppca.glPixelStore( GL_PACK_SKIP_PIXELS, GLint(31) );
  ppca.glPixelStore( GL_PACK_ALIGNMENT, GLint(32) );

  ppca.glBindBuffer( GL_PIXEL_UNPACK_BUFFER, GLuint(123) );
  ppca.glBindBuffer( GL_PIXEL_PACK_BUFFER, GLuint(456) );

  EXPECT_EQ( GLint(GL_TRUE), ppca.PixelStore::unpackSwapBytes );
  EXPECT_EQ( GLint(GL_TRUE), ppca.PixelStore::unpackLsbFirst );
  EXPECT_EQ( GLint(11),      ppca.PixelStore::unpackImageHeight );
  EXPECT_EQ( GLint(12),      ppca.PixelStore::unpackSkipImages );
  EXPECT_EQ( GLint(13),      ppca.PixelStore::unpackRowLength );
  EXPECT_EQ( GLint(14),      ppca.PixelStore::unpackSkipRows );
  EXPECT_EQ( GLint(15),      ppca.PixelStore::unpackSkipPixels );
  EXPECT_EQ( GLint(16),      ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( GLint(GL_TRUE), ppca.PixelStore::packSwapBytes );
  EXPECT_EQ( GLint(GL_TRUE), ppca.PixelStore::packLsbFirst );
  EXPECT_EQ( GLint(27),      ppca.PixelStore::packImageHeight );
  EXPECT_EQ( GLint(28),      ppca.PixelStore::packSkipImages );
  EXPECT_EQ( GLint(29),      ppca.PixelStore::packRowLength );
  EXPECT_EQ( GLint(30),      ppca.PixelStore::packSkipRows );
  EXPECT_EQ( GLint(31),      ppca.PixelStore::packSkipPixels );
  EXPECT_EQ( GLint(32),      ppca.PixelStore::packAlignment );
  EXPECT_EQ( GLuint(123),    ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint(456),    ppca.PixelStore::pixelPackBufferBinding );

  // reset just the pixel store state then verify that
  // the entire ppca is now back to default state

  ppca.PixelStore::Reset();
  checkPpcaDefaults(ctx, ppca);

  // now test glPixelStoref

  ppca.glPixelStore( GL_UNPACK_SWAP_BYTES, GLfloat(1.11) );
  ppca.glPixelStore( GL_UNPACK_LSB_FIRST, GLfloat(0) );
  ppca.glPixelStore( GL_UNPACK_IMAGE_HEIGHT, GLfloat(11.0) );
  ppca.glPixelStore( GL_UNPACK_SKIP_IMAGES, GLfloat(12.1) );
  ppca.glPixelStore( GL_UNPACK_ROW_LENGTH, GLfloat(13.2) );
  ppca.glPixelStore( GL_UNPACK_SKIP_ROWS, GLfloat(14.3) );
  ppca.glPixelStore( GL_UNPACK_SKIP_PIXELS, GLfloat(15.4) );
  ppca.glPixelStore( GL_UNPACK_ALIGNMENT, GLfloat(16.5) );
  ppca.glPixelStore( GL_PACK_SWAP_BYTES, GLfloat(GL_TRUE) );
  ppca.glPixelStore( GL_PACK_LSB_FIRST, GLfloat(GL_TRUE) );
  ppca.glPixelStore( GL_PACK_IMAGE_HEIGHT, GLfloat(27.01) );
  ppca.glPixelStore( GL_PACK_SKIP_IMAGES, GLfloat(28.02) );
  ppca.glPixelStore( GL_PACK_ROW_LENGTH, GLfloat(29.03) );
  ppca.glPixelStore( GL_PACK_SKIP_ROWS, GLfloat(30.04) );
  ppca.glPixelStore( GL_PACK_SKIP_PIXELS, GLfloat(31.05) );
  ppca.glPixelStore( GL_PACK_ALIGNMENT, GLfloat(32.06) );

  ppca.glBindBuffer( GL_PIXEL_UNPACK_BUFFER, GLuint(123) );
  ppca.glBindBuffer( GL_PIXEL_PACK_BUFFER, GLuint(456) );

  EXPECT_EQ( GLfloat(GL_TRUE),  ppca.PixelStore::unpackSwapBytes );
  EXPECT_EQ( GLfloat(GL_FALSE), ppca.PixelStore::unpackLsbFirst );
  EXPECT_EQ( GLfloat(11),       ppca.PixelStore::unpackImageHeight );
  EXPECT_EQ( GLfloat(12),       ppca.PixelStore::unpackSkipImages );
  EXPECT_EQ( GLfloat(13),       ppca.PixelStore::unpackRowLength );
  EXPECT_EQ( GLfloat(14),       ppca.PixelStore::unpackSkipRows );
  EXPECT_EQ( GLfloat(15),       ppca.PixelStore::unpackSkipPixels );
  EXPECT_EQ( GLfloat(16),       ppca.PixelStore::unpackAlignment );
  EXPECT_EQ( GLfloat(GL_TRUE),  ppca.PixelStore::packSwapBytes );
  EXPECT_EQ( GLfloat(GL_TRUE),  ppca.PixelStore::packLsbFirst );
  EXPECT_EQ( GLfloat(27),       ppca.PixelStore::packImageHeight );
  EXPECT_EQ( GLfloat(28),       ppca.PixelStore::packSkipImages );
  EXPECT_EQ( GLfloat(29),       ppca.PixelStore::packRowLength );
  EXPECT_EQ( GLfloat(30),       ppca.PixelStore::packSkipRows );
  EXPECT_EQ( GLfloat(31),       ppca.PixelStore::packSkipPixels );
  EXPECT_EQ( GLfloat(32),       ppca.PixelStore::packAlignment );
  EXPECT_EQ( GLuint(123),       ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint(456),       ppca.PixelStore::pixelPackBufferBinding );
}

TEST ( RegalPpca, glBindBuffer_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::arrayBufferBinding = 0;
  ppca.VertexArray::elementArrayBufferBinding = 0;
  ppca.PixelStore::pixelUnpackBufferBinding = 0;
  ppca.PixelStore::pixelPackBufferBinding = 0;

  ppca.glBindBuffer( GL_ARRAY_BUFFER, GLuint(12) );
  EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.PixelStore::pixelPackBufferBinding );

  ppca.glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, GLuint(34) );
  EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(34), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.PixelStore::pixelPackBufferBinding );

  ppca.glBindBuffer( GL_PIXEL_UNPACK_BUFFER, GLuint(21) );
  EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(34), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint(21), ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint( 0), ppca.PixelStore::pixelPackBufferBinding );

  ppca.glBindBuffer( GL_PIXEL_PACK_BUFFER, GLuint(43) );
  EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(34), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint(21), ppca.PixelStore::pixelUnpackBufferBinding );
  EXPECT_EQ( GLuint(43), ppca.PixelStore::pixelPackBufferBinding );

  GLuint validBuffers[] = { GL_ATOMIC_COUNTER_BUFFER,
                            GL_COPY_READ_BUFFER,
                            GL_COPY_WRITE_BUFFER,
                            GL_DRAW_INDIRECT_BUFFER,
                            GL_DISPATCH_INDIRECT_BUFFER,
                            GL_SHADER_STORAGE_BUFFER,
                            GL_TEXTURE_BUFFER,
                            GL_TRANSFORM_FEEDBACK_BUFFER,
                            GL_UNIFORM_BUFFER };
  GLuint nValidBuffers = sizeof(validBuffers)/sizeof(GLuint);

  for (GLuint n=0; n<nValidBuffers; n++)
  {
    ppca.glBindBuffer( validBuffers[n], GLuint(56) );
    EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
    EXPECT_EQ( GLuint(34), ppca.VertexArray::elementArrayBufferBinding );
    EXPECT_EQ( GLuint(21), ppca.PixelStore::pixelUnpackBufferBinding );
    EXPECT_EQ( GLuint(43), ppca.PixelStore::pixelPackBufferBinding );
  }

  GLuint invalidBuffers[] = { GL_ARRAY_BUFFER_BINDING,
                              GL_ATOMIC_COUNTER_BUFFER_BINDING,
                              GL_DRAW_INDIRECT_BUFFER_BINDING,
                              GL_DISPATCH_INDIRECT_BUFFER_BINDING,
                              GL_ELEMENT_ARRAY_BUFFER_BINDING,
                              GL_PIXEL_PACK_BUFFER_BINDING,
                              GL_PIXEL_UNPACK_BUFFER_BINDING,
                              GL_SHADER_STORAGE_BUFFER_BINDING,
                              GL_TEXTURE_BUFFER_DATA_STORE_BINDING,
                              GL_TEXTURE_BUFFER_FORMAT,
                              GL_TEXTURE_BUFFER_OFFSET,
                              GL_TEXTURE_BUFFER_SIZE,
                              GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT,
                              GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
                              GL_UNIFORM_BUFFER_BINDING };
  GLuint nInvalidBuffers = sizeof(invalidBuffers)/sizeof(GLuint);

  for (GLuint n=0; n<nInvalidBuffers; n++)
  {
    ppca.glBindBuffer( invalidBuffers[n], GLuint(78) );
    EXPECT_EQ( GLuint(12), ppca.VertexArray::arrayBufferBinding );
    EXPECT_EQ( GLuint(34), ppca.VertexArray::elementArrayBufferBinding );
    EXPECT_EQ( GLuint(21), ppca.PixelStore::pixelUnpackBufferBinding );
    EXPECT_EQ( GLuint(43), ppca.PixelStore::pixelPackBufferBinding );
  }

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindBuffer( GL_ARRAY_BUFFER, GLuint(123) );
  ppca.glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, GLuint(789) );
  ppca.glBindBuffer( GL_PIXEL_PACK_BUFFER, GLuint(321) );
  ppca.glBindBuffer( GL_PIXEL_UNPACK_BUFFER, GLuint(654) );

  EXPECT_EQ( GLuint(123), ppca.VertexArray::arrayBufferBinding );
  EXPECT_EQ( GLuint(  0), ppca.VertexArray::elementArrayBufferBinding );
  EXPECT_EQ( GLuint(321), ppca.PixelStore::pixelPackBufferBinding );
  EXPECT_EQ( GLuint(654), ppca.PixelStore::pixelUnpackBufferBinding );
}

TEST ( RegalPpca, glClientActiveTexture_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<(REGAL_EMU_MAX_TEXTURE_COORDS * 2); ii++)
  {
    GLenum test = GLenum(GL_TEXTURE0 + ii);

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.VertexArray::clientActiveTexture = GLenum(0);
    ppca.glClientActiveTexture( test );

    if (ii < REGAL_EMU_MAX_TEXTURE_COORDS)
      EXPECT_EQ( test, ppca.VertexArray::clientActiveTexture );
    else
      EXPECT_EQ( GLenum(0), ppca.VertexArray::clientActiveTexture );

    ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
    checkPpcaDefaults(ctx, ppca);
  }

  for (GLuint ii=0; ii<(REGAL_EMU_MAX_TEXTURE_COORDS * 2); ii++)
  {
    GLenum test = GLenum(GL_TEXTURE0 + ii);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.VertexArray::clientActiveTexture = GLenum(0);
    ppca.glClientActiveTexture( test );

    if (ii < REGAL_EMU_MAX_TEXTURE_COORDS)
      EXPECT_EQ( test, ppca.VertexArray::clientActiveTexture );
    else
      EXPECT_EQ( GLenum(0), ppca.VertexArray::clientActiveTexture );

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glEnableDisableClientState_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glEnableClientState & glDisableClientState with no vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    if (ii < 7)
    {
      ppca.glEnableClientState( Regal::ClientState::vaEnum[ii][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientState( Regal::ClientState::vaEnum[ii][0] );
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.glEnableClientState( Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientState( Regal::ClientState::vaEnum[7][0] );
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableClientStateiEXT & glDisableClientStateiEXT with no vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    if (ii < 7)
    {
      ppca.glEnableClientStateiEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateiEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.glEnableClientStateiEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateiEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableClientStateIndexedEXT & glDisableClientStateIndexedEXT with no vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    if (ii < 7)
    {
      ppca.glEnableClientStateIndexedEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateIndexedEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.glEnableClientStateIndexedEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateIndexedEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableClientState & glDisableClientState with a vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    if (ii < 7)
    {
      ppca.glEnableClientState( Regal::ClientState::vaEnum[ii][0] );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientState( Regal::ClientState::vaEnum[ii][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.glEnableClientState( Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientState( Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableClientStateiEXT & glDisableClientStateiEXT with a vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    if (ii < 7)
    {
      ppca.glEnableClientStateiEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateiEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.glEnableClientStateiEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateiEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableClientStateIndexedEXT & glDisableClientStateIndexedEXT with a vertex array bound

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    if (ii < 7)
    {
      ppca.glEnableClientStateIndexedEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateIndexedEXT( Regal::ClientState::vaEnum[ii][0], 0 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.glEnableClientStateIndexedEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::vertexArrayBinding = 1;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableClientStateIndexedEXT( Regal::ClientState::vaEnum[7][0], ii - 7 );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::vertexArrayBinding = 0;
      checkPpcaDefaults(ctx, ppca);
    }
  }
}

TEST ( RegalPpca, glBindVertexArray_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.glBindVertexArray( GLuint(13579) );
  EXPECT_EQ( GLuint(13579), ppca.VertexArray::vertexArrayBinding );

  ppca.VertexArray::vertexArrayBinding = 0;
  checkPpcaDefaults(ctx, ppca);
}

TEST ( RegalPpca, glEnableDisableVertexAttribArray_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glEnableVertexAttribArray & glDisableVertexAttribArray with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.glEnableVertexAttribArray( ii );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].enabled );
    ppca.VertexArray::generic[ii].enabled = GL_FALSE;
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].enabled = GL_TRUE;
    ppca.glDisableVertexAttribArray( ii );
    checkPpcaDefaults(ctx, ppca);
  }

  // test glEnableVertexAttribArray & glDisableVertexAttribArray with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    ppca.glEnableVertexAttribArray( ii );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].enabled );
    ppca.VertexArray::vertexArrayBinding = 0;
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.VertexArray::generic[ii].enabled = GL_TRUE;
    ppca.glDisableVertexAttribArray( ii );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].enabled );
    ppca.VertexArray::generic[ii].enabled = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glEnableDisableVertexArrayAttribEXT_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glEnableVertexArrayAttribEXT & glDisableVertexArrayAttribEXT with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    ppca.glEnableVertexArrayAttribEXT( 0, ii );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].enabled );
    ppca.VertexArray::generic[ii].enabled = GL_FALSE;
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].enabled = GL_TRUE;
    ppca.glDisableVertexArrayAttribEXT( 0, ii );
    checkPpcaDefaults(ctx, ppca);
  }

  // test glEnableVertexAttribArray & glDisableVertexAttribArray with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    ppca.glEnableVertexArrayAttribEXT( ii+1, ii );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].enabled );
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].enabled = GL_TRUE;
    ppca.glDisableVertexArrayAttribEXT( ii+1, ii );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].enabled );
    ppca.VertexArray::generic[ii].enabled = GL_FALSE;
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glEnableDisableVertexArrayEXT_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glEnableVertexArrayEXT & glDisableVertexArrayEXT with no vertex array object

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    if (ii < 7)
    {
      ppca.glEnableVertexArrayEXT( 0, Regal::ClientState::vaEnum[ii][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableVertexArrayEXT( 0, Regal::ClientState::vaEnum[ii][0] );
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.glEnableVertexArrayEXT( 0, Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableVertexArrayEXT( 0, Regal::ClientState::vaEnum[7][0] );
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);
    }
  }

  // test glEnableVertexArrayEXT & glDisableVertexArrayEXT with a vertex array object

  for (GLuint ii=0; ii<ClientState::nNamedArrays; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    if (ii < 7)
    {
      ppca.glEnableVertexArrayEXT( ii+1, Regal::ClientState::vaEnum[ii][0] );
      checkPpcaDefaults(ctx, ppca);

      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableVertexArrayEXT( ii+1, Regal::ClientState::vaEnum[ii][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      checkPpcaDefaults(ctx, ppca);
    }
    else
    {
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.glEnableVertexArrayEXT( ii+1, Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);

      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0 + ii - 7;
      ppca.VertexArray::named[ii].enabled = GL_TRUE;
      ppca.glDisableVertexArrayEXT( ii+1, Regal::ClientState::vaEnum[7][0] );
      EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::named[ii].enabled );
      ppca.VertexArray::named[ii].enabled = GL_FALSE;
      ppca.VertexArray::clientActiveTexture = GL_TEXTURE0;
      checkPpcaDefaults(ctx, ppca);
    }
  }
}

TEST ( RegalPpca, glVertexAttribBinding_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glVertexAttribBinding with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.glVertexAttribBinding( ii, ii+1 );
    EXPECT_EQ( GLuint(ii+1), ppca.VertexArray::generic[ii].bindingIndex );
    ppca.VertexArray::generic[ii].bindingIndex = ii;
    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexAttribBinding with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    ppca.VertexArray::generic[ii].bindingIndex = ii+1;
    ppca.glVertexAttribBinding( ii, 0 );
    EXPECT_EQ( GLuint(ii+1), ppca.VertexArray::generic[ii].bindingIndex );
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.VertexArray::generic[ii].bindingIndex = ii;
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glVertexBindingDivisor_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glVertexBindingDivisor with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.glVertexBindingDivisor( ii, 333 );
    EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].divisor );
    ppca.VertexArray::bindings[ii].divisor = 0;
    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexBindingDivisor with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    ppca.VertexArray::bindings[ii].divisor = 333;
    ppca.glVertexBindingDivisor( ii, 555 );
    EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].divisor );
    ppca.VertexArray::bindings[ii].divisor = 0;
    ppca.VertexArray::vertexArrayBinding = 0;
    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexAttribDivisor with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    if (ii<REGAL_EMU_MAX_VERTEX_ATTRIBS)
    {
      ppca.glVertexAttribDivisor( ii, 555 );
      EXPECT_EQ( GLuint(555), ppca.VertexArray::bindings[ii].divisor );
      EXPECT_EQ( GLuint(ii), ppca.VertexArray::generic[ii].bindingIndex );
      ppca.VertexArray::bindings[ii].divisor = 0;
      ppca.VertexArray::generic[ii].bindingIndex = ii;
    }
    else
    {
      ppca.VertexArray::bindings[ii].divisor = 333;
      ppca.glVertexAttribDivisor( ii, 555 );
      EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].divisor );
      ppca.VertexArray::bindings[ii].divisor = 0;
      ppca.VertexArray::vertexArrayBinding = 0;
    }
    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexAttribDivisor with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    if (ii<REGAL_EMU_MAX_VERTEX_ATTRIBS)
    {
      ppca.VertexArray::bindings[ii].divisor = 333;
      ppca.VertexArray::generic[ii].bindingIndex = 333;
      ppca.glVertexAttribDivisor( ii, 555 );
      EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].divisor );
      EXPECT_EQ( GLuint(333), ppca.VertexArray::generic[ii].bindingIndex );
      ppca.VertexArray::bindings[ii].divisor = 0;
      ppca.VertexArray::generic[ii].bindingIndex = ii;
      ppca.VertexArray::vertexArrayBinding = 0;
    }
    else
    {
      ppca.VertexArray::bindings[ii].divisor = 333;
      ppca.glVertexAttribDivisor( ii, 555 );
      EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].divisor );
      ppca.VertexArray::bindings[ii].divisor = 0;
      ppca.VertexArray::vertexArrayBinding = 0;
    }
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glBindVertexBuffer_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glBindVertexBuffer with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.glBindVertexBuffer( ii, GLuint(2 * ii + 1), GLintptr(0xbeef), ii+3 );

    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr( 0xbeef), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(ii + 3), ppca.VertexArray::bindings[ii].stride );
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
    checkPpcaDefaults(ctx, ppca);
  }

  // test glBindVertexBuffer with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);
    ppca.VertexArray::vertexArrayBinding = 1;

    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);

    ppca.glBindVertexBuffer( ii, 1, 1, 1 );

    EXPECT_EQ( GLuint(333), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11), ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::vertexArrayBinding = GLuint(0);
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glBindVertexBuffers_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  GLuint    buffers[REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS];
  GLintptr  offsets[REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS];
  GLsizei   strides[REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS];

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    buffers[ii] = GLuint(2 * ii + 1);
    offsets[ii] = GLintptr(0xbeef);
    strides[ii] = GLsizei(ii+3);
  }

  // first test with "real" buffers, offsets, and strides

  // test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 0, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, buffers, offsets, strides );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr( 0xbeef), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(ii + 3), ppca.VertexArray::bindings[ii].stride );
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 0, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, buffers, offsets, strides );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // test partial glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 5, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS-5, &(buffers[5]), &(offsets[5]), &(strides[5]) );

  for (GLuint ii=5; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr( 0xbeef), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(ii + 3), ppca.VertexArray::bindings[ii].stride );
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // test partial glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=5; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 5, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS-5, &(buffers[5]), &(offsets[5]), &(strides[5]) );

  for (GLuint ii=5; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // a second partial test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 3, 3, &(buffers[3]), &(offsets[3]), &(strides[3]) );

  for (GLuint ii=3; ii<6; ii++)
  {
    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr( 0xbeef),  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(ii + 3),    ppca.VertexArray::bindings[ii].stride );
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // a second partial test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=3; ii<6; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 3, 3, &(buffers[3]), &(offsets[3]), &(strides[3]) );

  for (GLuint ii=3; ii<6; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // a third partial test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 0, 4, buffers, offsets, strides );

  for (GLuint ii=0; ii<4; ii++)
  {
    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr( 0xbeef),  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(ii + 3),    ppca.VertexArray::bindings[ii].stride );
    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // a third partial test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<4; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 0, 4, buffers, offsets, strides );

  for (GLuint ii=0; ii<4; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // now test passing NULL for buffers

  // test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 0, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, NULL, offsets, strides );

  checkPpcaDefaults(ctx, ppca);

  // test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 0, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS, NULL, offsets, strides );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // test partial glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 5, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS-5, NULL, &(offsets[5]), &(strides[5]) );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    if (ii < 5)
    {
      EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
      EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
      EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );
    }

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // test partial glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 5, REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS-5, NULL, &(offsets[5]), &(strides[5]) );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // a second partial test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 3, 3, NULL, &(offsets[3]), &(strides[3]) );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    if (ii >= 3 && ii < 6)
    {
      EXPECT_EQ( GLuint(0),   ppca.VertexArray::bindings[ii].buffer );
      EXPECT_EQ( GLintptr(0), ppca.VertexArray::bindings[ii].offset );
      EXPECT_EQ( GLsizei(16), ppca.VertexArray::bindings[ii].stride );
    }
    else
    {
      EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
      EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
      EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );
    }

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  checkPpcaDefaults(ctx, ppca);

  // a second partial test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 3, 3, NULL, &(offsets[3]), &(strides[3]) );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);

  // a third partial test glBindVertexBuffers with no vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 0;
  ppca.glBindVertexBuffers( 0, 4, NULL, offsets, strides );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    if (ii < 4)
    {
      EXPECT_EQ( GLuint(0),   ppca.VertexArray::bindings[ii].buffer );
      EXPECT_EQ( GLintptr(0), ppca.VertexArray::bindings[ii].offset );
      EXPECT_EQ( GLsizei(16), ppca.VertexArray::bindings[ii].stride );
    }
    else
    {
      EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
      EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
      EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );
    }

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }

  checkPpcaDefaults(ctx, ppca);

  // a third partial test glBindVertexBuffers with a vertex array bound

  ppca.Reset(ctx);
  checkPpcaDefaults(ctx, ppca);

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.VertexArray::bindings[ii].buffer = GLuint(333);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0xdead);
    ppca.VertexArray::bindings[ii].stride = GLsizei(11);
  }

  ppca.VertexArray::vertexArrayBinding = 1;
  ppca.glBindVertexBuffers( 0, 4, NULL, offsets, strides );

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    EXPECT_EQ( GLuint(333),      ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( GLintptr(0xdead), ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( GLsizei(11),      ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);
  }
  ppca.VertexArray::vertexArrayBinding = GLuint(0);
  checkPpcaDefaults(ctx, ppca);
}

TEST ( RegalPpca, glVertexAttribFormat_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glVertexAttrib(I|L|)Format with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribIFormat( ii, GLuint(2 * ii + 1), GLenum(ii), GLuint(ii + 3) );

    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(ii), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(ii + 3), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribLFormat( ii, GLuint(2 * ii + 1), GLenum(ii), ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(ii), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(ii + 3), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.glVertexAttribFormat( ii, GLuint(2 * ii + 1), GLenum(ii), GL_TRUE, ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(ii), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(ii + 3), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.glVertexAttribFormat( ii, GLuint(2 * ii + 1), GLenum(ii), GL_FALSE, ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 1), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(ii), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(ii + 3), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);

    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexAttrib(I|L|)Format with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].size = 2 * ii + 3;
    ppca.VertexArray::generic[ii].type = GLenum(2 * ii + 4);
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(2 * ii + 5);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribIFormat( ii, ii + 1, GLenum(ii + 2), ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 3), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(2 * ii + 4), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(2 * ii + 5), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].size = 2 * ii + 3;
    ppca.VertexArray::generic[ii].type = GLenum(2 * ii + 4);
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(2 * ii + 5);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribLFormat( ii, ii + 1, GLenum(ii + 2), ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 3), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(2 * ii + 4), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(2 * ii + 5), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].size = 2 * ii + 3;
    ppca.VertexArray::generic[ii].type = GLenum(2 * ii + 4);
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(2 * ii + 5);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribFormat( ii, ii + 1, GLenum(ii + 2), GL_FALSE, ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 3), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(2 * ii + 4), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(2 * ii + 5), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].size = 2 * ii + 3;
    ppca.VertexArray::generic[ii].type = GLenum(2 * ii + 4);
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(2 * ii + 5);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribFormat( ii, ii + 1, GLenum(ii + 2), GL_TRUE, ii + 3 );

    EXPECT_EQ( GLuint(2 * ii + 3), ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( GLenum(2 * ii + 4), ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint(2 * ii + 5), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glVertexAttribPointer_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glVertexAttrib(I|L|)Pointer with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS && ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    GLuint   testSize   = GLuint(2 * ii + 1);
    GLenum   testType   = GLenum(ii);
    GLsizei  testStride = GLsizei(ii + 3);
    GLintptr testOffset = (GLintptr)(ii + 4);
    GLuint   testBuffer = ppca.VertexArray::arrayBufferBinding;

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribIPointer( ii, testSize, testType, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribLPointer( ii, testSize, testType, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribPointer( ii, testSize, testType, GL_FALSE, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE),  ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexAttribPointer( ii, testSize, testType, GL_TRUE, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexAttrib(I|L|)Pointer with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS && ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    GLuint   testSize   = GLuint(2 * ii + 1);
    GLenum   testType   = GLenum(ii);
    GLsizei  testStride = GLsizei(ii + 3);
    GLintptr testOffset = (GLintptr)(ii + 4);

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribIPointer( ii, testSize, testType, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::vertexArrayBinding );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribLPointer( ii, testSize, testType, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::vertexArrayBinding );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribPointer( ii, testSize, testType, GL_FALSE, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( GLuint( 1 ),        ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLuint( 1 ),        ppca.VertexArray::vertexArrayBinding );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexAttribPointer( ii, testSize, testType, GL_TRUE, testStride, reinterpret_cast<const GLvoid*>(testOffset) );

    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLuint( 1 ),         ppca.VertexArray::vertexArrayBinding );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::vertexArrayBinding = 0;

    checkPpcaDefaults(ctx, ppca);
  }
}

TEST ( RegalPpca, glVertexArrayVertexAttribOffsetEXT_Shadowing )
{
  RegalContext ctx;
  ctx.info = new ContextInfo();

  Ppca ppca;
  ppca.Init(ctx);
  checkPpcaDefaults(ctx, ppca);

  // test glVertexArrayVertexAttrib(I|)OffsetEXT with no vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS && ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    GLuint   testSize   = GLuint(2 * ii + 1);
    GLenum   testType   = GLenum(ii);
    GLsizei  testStride = GLsizei(ii + 3);
    GLintptr testOffset = (GLintptr)(ii + 4);
    GLuint   testBuffer = ppca.VertexArray::arrayBufferBinding + 1;

    // first test passing in 0 for vertex array object

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribIOffsetEXT( 0, testBuffer, ii, testSize, testType, testStride, testOffset );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribOffsetEXT( 0, testBuffer, ii, testSize, testType, GL_FALSE, testStride, testOffset );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribOffsetEXT( 0, testBuffer, ii, testSize, testType, GL_TRUE, testStride, testOffset );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    // now test passing in 1 for vertex array object

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribIOffsetEXT( 1, testBuffer, ii, testSize, testType, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );

    ppca.VertexArray::generic[ii].relativeOffset = 0;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribOffsetEXT( 1, testBuffer, ii, testSize, testType, GL_FALSE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::vertexArrayBinding = 0;
    ppca.glVertexArrayVertexAttribOffsetEXT( 1, testBuffer, ii, testSize, testType, GL_TRUE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);
  }

  // test glVertexArrayVertexAttrib(I|)OffsetEXT with a vertex array bound

  for (GLuint ii=0; ii<REGAL_EMU_MAX_VERTEX_ATTRIBS && ii<REGAL_EMU_MAX_VERTEX_ATTRIB_BINDINGS; ii++)
  {
    ppca.Reset(ctx);
    checkPpcaDefaults(ctx, ppca);

    GLuint   testSize   = GLuint(2 * ii + 1);
    GLenum   testType   = GLenum(ii);
    GLsizei  testStride = GLsizei(ii + 3);
    GLintptr testOffset = (GLintptr)(ii + 4);
    GLuint   testBuffer = ppca.VertexArray::arrayBufferBinding + 1;

    // first test passing in 0 for vertex array object

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexArrayVertexAttribIOffsetEXT( 0, testBuffer, ii, testSize, testType, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ),  ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexArrayVertexAttribOffsetEXT( 0, testBuffer, ii, testSize, testType, GL_FALSE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexArrayVertexAttribOffsetEXT( 0, testBuffer, ii, testSize, testType, GL_TRUE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( testSize,    ppca.VertexArray::generic[ii].size );
    EXPECT_EQ( testType,    ppca.VertexArray::generic[ii].type );
    EXPECT_EQ( GLuint( 0 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isLong );
    EXPECT_EQ( ii,          ppca.VertexArray::generic[ii].bindingIndex );

    EXPECT_EQ( testBuffer,  ppca.VertexArray::bindings[ii].buffer );
    EXPECT_EQ( testOffset,  ppca.VertexArray::bindings[ii].offset );
    EXPECT_EQ( testStride,  ppca.VertexArray::bindings[ii].stride );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].size = 4;
    ppca.VertexArray::generic[ii].type = GL_FLOAT;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;
    ppca.VertexArray::generic[ii].bindingIndex = ii;

    ppca.VertexArray::bindings[ii].buffer = GLuint(0);
    ppca.VertexArray::bindings[ii].offset = GLintptr(0);
    ppca.VertexArray::bindings[ii].stride = GLsizei(16);

    checkPpcaDefaults(ctx, ppca);

    // now test passing in 1 for vertex array object

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexArrayVertexAttribIOffsetEXT( 1, testBuffer, ii, testSize, testType, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ),  ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_TRUE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;
    ppca.glVertexArrayVertexAttribOffsetEXT( 1, testBuffer, ii, testSize, testType, GL_FALSE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE), ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);

    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_TRUE;
    ppca.VertexArray::generic[ii].isLong = GL_TRUE;
    ppca.VertexArray::generic[ii].relativeOffset = GLuint(1);

    ppca.VertexArray::vertexArrayBinding = 1;

    ppca.glVertexArrayVertexAttribOffsetEXT( 1, testBuffer, ii, testSize, testType, GL_TRUE, testStride, testOffset );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::vertexArrayBinding );

    EXPECT_EQ( GLuint( 1 ), ppca.VertexArray::generic[ii].relativeOffset );
    EXPECT_EQ( GLboolean(GL_FALSE), ppca.VertexArray::generic[ii].normalized );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isInteger );
    EXPECT_EQ( GLboolean(GL_TRUE),  ppca.VertexArray::generic[ii].isLong );

    ppca.VertexArray::vertexArrayBinding = 0;

    ppca.VertexArray::generic[ii].relativeOffset = GLuint(0);
    ppca.VertexArray::generic[ii].normalized = GL_FALSE;
    ppca.VertexArray::generic[ii].isInteger = GL_FALSE;
    ppca.VertexArray::generic[ii].isLong = GL_FALSE;

    checkPpcaDefaults(ctx, ppca);
  }
}

}  // namespace

/*
glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
*/
