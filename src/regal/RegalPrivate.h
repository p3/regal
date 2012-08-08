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

#ifndef __REGAL_PRIVATE_H__
#define __REGAL_PRIVATE_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <cstdio>
#include <cstring>

#ifndef REGAL_SYS_WGL
#include <pthread.h>
#endif

#include "RegalUtil.h"
#include "RegalConfig.h"
#include "RegalLookup.h"
#include "RegalDispatch.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using ::RegalErrorCallback;

#define REGAL_DECL_EXPORT 1
#define REGAL_SYS_WGL_DECLARE_WGL 1

// attrib mapping

enum RegalFixedFunctionAttrib {
    RFF2A_Vertex = 0,
    RFF2A_Weights = 1,
    RFF2A_Normal = 2,
    RFF2A_Color = 3,
    RFF2A_SecondaryColor = 4,
    RFF2A_FogCoord = 5,
    RFF2A_EdgeFlag = 6,
    RFF2A_TexCoord = 7,
    RFF2A_Invalid = 99
};

const GLenum RFFA2Enum[] = {
    GL_VERTEX_ARRAY,
    GL_WEIGHT_ARRAY_ARB,
    GL_NORMAL_ARRAY,
    GL_COLOR_ARRAY,
    GL_SECONDARY_COLOR_ARRAY,
    GL_FOG_COORDINATE_ARRAY,
    GL_EDGE_FLAG_ARRAY,
    GL_TEXTURE_COORD_ARRAY
};

const GLuint RFF2AMap8[] = {  0, 99, 2, 1, 99, 7, 99, 3 };
const GLuint RFF2AInvMap8[] = {
    RFF2A_Vertex,
    RFF2A_Color,
    RFF2A_Normal,
    RFF2A_TexCoord,
    RFF2A_TexCoord,
    RFF2A_TexCoord,
    RFF2A_SecondaryColor,
    RFF2A_FogCoord
};
const GLuint RFF2ATexBegin8 = 3;
const GLuint RFF2ATexEnd8 = 6;
const GLuint RFF2AMap16[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const GLuint RFF2AInvMap16[] = {
    RFF2A_Vertex,
    RFF2A_Weights,
    RFF2A_Normal,
    RFF2A_Color,
    RFF2A_SecondaryColor,
    RFF2A_FogCoord,
    RFF2A_EdgeFlag,
    RFF2A_Invalid,
    RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord,
    RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord, RFF2A_TexCoord
};
const GLuint RFF2ATexBegin16 = 8;
const GLuint RFF2ATexEnd16 = 16;

// end attrib mapping

extern void RegalPrivateMakeCurrent(RegalSystemContext sysCtx);

#if REGAL_SYS_WGL
    typedef DWORD Thread;
#if REGAL_WIN_TLS

    extern "C" { DWORD __stdcall TlsAlloc    (void);          }
    extern "C" { int   __stdcall TlsFree     (DWORD);         }
    extern "C" { void* __stdcall TlsGetValue (DWORD);         }
    extern "C" { int   __stdcall TlsSetValue (DWORD, void *); }

    extern DWORD regalCurrentContextTLSIDX;

    inline void * RegalPrivateGetCurrentContext()
    {
        return TlsGetValue( regalCurrentContextTLSIDX );
    }
#else
    extern __declspec( thread ) void * regalCurrentContext;

    inline void * RegalPrivateGetCurrentContext()
    {
        return regalCurrentContext;
    }
#endif
#else
    typedef pthread_t Thread;

    extern pthread_key_t regalPrivateCurrentContextKey;

    inline void * RegalPrivateGetCurrentContext()
    {
#if ! REGAL_SYS_OSX
        return pthread_getspecific( regalPrivateCurrentContextKey );
#else
        void * v = pthread_getspecific( regalPrivateCurrentContextKey );
        if( v == NULL ) {
           RegalMakeCurrent( CGLGetCurrentContext() );
           return pthread_getspecific( regalPrivateCurrentContextKey );
        }
        return v;
#endif
    }
#endif

struct RegalContext;

REGAL_NAMESPACE_END

#include "RegalContext.h"

#define GET_REGAL_CONTEXT() ((::REGAL_NAMESPACE_INTERNAL::RegalContext *) ::REGAL_NAMESPACE_INTERNAL::RegalPrivateGetCurrentContext())

#endif // REGAL_PRIVATE_H
