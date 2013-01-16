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

#ifndef REGAL_CLIENT_ATTRIB_STACK_DEPTH
#define REGAL_CLIENT_ATTRIB_STACK_DEPTH 16
#endif

#if REGAL_EMULATION

REGAL_GLOBAL_BEGIN

#include <vector>

#include <GL/Regal.h>

#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

class DispatchTable;

namespace Emu {

// ====================================
// PixelStorageState
// ====================================

struct PixelStorageState {
  enum { STATE_COUNT = 16 };
  GLint state[ STATE_COUNT ];

  static const GLenum indexToPName[ STATE_COUNT ];
  static size_t PNameToIndex( GLenum pname );
  static void Transition( DispatchTable& dt, const PixelStorageState& current, const PixelStorageState& target );

  void Reset();
  void Set( GLenum pname, GLint pvalue );
  GLint Get( GLenum pname ) const;
};

// ====================================
// VertexArrayState
// ====================================

struct VertexArrayState {
  enum { STATE_COUNT = 8 };

  struct VertexState {
    bool enabled;
    GLint size;
    GLenum type;
    GLsizei stride;
    const GLvoid* pointer;
  };

  VertexState state[ STATE_COUNT ];

  static const GLenum indexToPName[ STATE_COUNT ];
  static size_t PNameToIndex( GLenum pname );
  static void Transition( DispatchTable& dt, const VertexArrayState& current, const VertexArrayState& target );

  void Reset();
  void SetEnable( GLenum pname, bool enabled );
  void SetData( GLenum pname, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
};

// ====================================
// Ppca
// ====================================

struct Ppca : public RegalEmu {
  typedef std::vector<GLbitfield> ClientAttribMaskStack;
  typedef std::vector<PixelStorageState> PixelStorageStateStack;
  typedef std::vector<VertexArrayState> VertexArrayStateStack;

  Ppca();

  void Init( RegalContext &ctx );

  void ShadowPixelStore( GLenum pname, GLint pvalue );
  void ShadowPixelStore( GLenum pname, GLfloat pvalue );

  void ShadowEnableClientState( GLenum cap );
  void ShadowDisableClientState( GLenum cap );

  void ShadowColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowEdgeFlagPointer( GLsizei stride, const GLvoid* pointer );
  void ShadowFogCoordPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowIndexPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  void ShadowVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );

  void PushClientAttrib( RegalContext* ctx, GLbitfield mask );
  void PopClientAttrib( RegalContext* ctx );
  void PushClientAttribDefaultEXT( RegalContext* ctx, GLbitfield mask );

  bool Get( RegalContext* ctx, GLenum pname, GLint* params );
  bool Get( RegalContext* ctx, GLenum pname, GLint64* params );
  bool Get( RegalContext* ctx, GLenum pname, GLfloat* params );
  bool Get( RegalContext* ctx, GLenum pname, GLdouble* params );
  bool Get( RegalContext* ctx, GLenum pname, GLboolean* params );

  PixelStorageState pss;
  VertexArrayState vas;

  ClientAttribMaskStack clientAttribMaskStack_;
  PixelStorageStateStack pixelStorageStateStack_;
  VertexArrayStateStack vertexArrayStateStack_;
};

} // namespace Emu

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION

#endif // ! __REGAL_PPCA_H__
