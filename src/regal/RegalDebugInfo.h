/*
 Copyright (c) 2011 NVIDIA Corporation
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

 Regal debug dispatch support
 Cass Everitt

 */

#ifndef __REGAL_DEBUG_INFO_H__
#define __REGAL_DEBUG_INFO_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <map>
#include <string>

#include "RegalLog.h"
#include "RegalEnum.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct DebugTexImage {
    Enum internalFormat;
    GLsizei width;
    GLsizei height;
    GLint border;
};

struct DebugTexObject {
    const char * label;
    GLuint name;
    Enum target;
    DebugTexImage mips[16];
    GLfloat minLod;
    GLfloat maxLod;
    GLfloat maxAniso;
};

struct DebugBufferObject {
    const char * label;
    GLuint name;
};

struct DebugShaderObject {
    const char * label;
    GLuint name;
};

struct DebugProgramObject {
    const char * label;
    GLuint name;
};

struct DebugInfo {

    Enum matrixMode;
    GLint clientActiveTextureIndex;
    GLint activeTextureIndex;

    std::map< GLuint, DebugTexObject > textures;
    std::map< GLuint, DebugBufferObject> buffers;

    DebugInfo()
    : clientActiveTextureIndex( 0 )
    , activeTextureIndex( 0 ) {
    }

    void Init( RegalContext * ctx ) {
        UNUSED_PARAMETER(ctx);
        matrixMode = GL_MODELVIEW;
    }

    void MatrixMode( RegalContext * ctx, GLenum mode ) {
        UNUSED_PARAMETER(ctx);
        matrixMode = static_cast<Enum>(mode);
    }

    void ClientActiveTexture( RegalContext * ctx, GLenum texture ) {
        UNUSED_PARAMETER(ctx);
        clientActiveTextureIndex = texture - GL_TEXTURE0;
    }

    void ActiveTexture( RegalContext * ctx, GLenum texture ) {
        UNUSED_PARAMETER(ctx);
        activeTextureIndex = texture - GL_TEXTURE0;
    }

};

REGAL_NAMESPACE_END

#endif // ! __REGAL_DEBUG_H__







