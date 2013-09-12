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

 Regal DSA implementation
 Cass Everitt

 */

#ifndef __REGAL_DSA_H__
#define __REGAL_DSA_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalEmu.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

#define REGAL_DSA_NUM_ASM_PROG_TYPES 5
#define REGAL_DSA_NUM_BUFFER_TYPES 2
#define REGAL_DSA_INVALID 0xffffffff

namespace Emu {

template<typename T> inline void DsaGetv( DispatchTableGL & tbl, GLenum pname, T * params ) { UNUSED_PARAMETER(tbl); UNUSED_PARAMETER(pname); UNUSED_PARAMETER(params);}
template<> inline void DsaGetv( DispatchTableGL & tbl, GLenum pname, GLfloat * params ) { tbl.glGetFloatv( pname, params ); }
template<> inline void DsaGetv( DispatchTableGL & tbl, GLenum pname, GLdouble * params ) { tbl.glGetDoublev( pname, params ); }


struct Dsa
{
    int callDepth;
    struct Cache {
        GLenum matrixMode;
        GLenum activeTexture;
        GLenum clientActiveTexture;
        GLenum textureTarget;
        GLuint texture;
        GLuint asmProgram[ REGAL_DSA_NUM_ASM_PROG_TYPES ];
        GLuint vao;
        GLuint buffer;
        GLuint glslProgram;
        GLuint framebuffer;
        GLuint framebufferTarget;
        GLuint renderbuffer;
        GLuint renderbufferTarget;
    };
    Cache drv;
    Cache dsa;

    void Init( RegalContext &ctx )
    {
        UNUSED_PARAMETER(ctx);

        callDepth = 0;
        drv.matrixMode = GL_MODELVIEW;
        drv.activeTexture = GL_TEXTURE0;
        drv.clientActiveTexture = GL_TEXTURE0;
        drv.textureTarget = GL_TEXTURE_2D;
        drv.texture = 0;
        for( int i = 0; i < REGAL_DSA_NUM_ASM_PROG_TYPES; i++ ) {
            drv.asmProgram[i] = 0;
        }
        drv.vao = 0;
        drv.buffer = 0;
        drv.glslProgram = 0;
        drv.framebuffer = 0;
        drv.framebufferTarget = GL_FRAMEBUFFER;
        drv.renderbuffer = 0;
        drv.renderbufferTarget = GL_RENDERBUFFER;

        dsa.matrixMode = REGAL_DSA_INVALID;
        dsa.activeTexture = REGAL_DSA_INVALID;
        dsa.clientActiveTexture = REGAL_DSA_INVALID;
        dsa.textureTarget = REGAL_DSA_INVALID;
        dsa.texture = REGAL_DSA_INVALID;
        for( int i = 0; i < REGAL_DSA_NUM_ASM_PROG_TYPES; i++ ) {
            dsa.asmProgram[i] = REGAL_DSA_INVALID;
        }
        dsa.vao = REGAL_DSA_INVALID;
        dsa.buffer = REGAL_DSA_INVALID;
        dsa.glslProgram = REGAL_DSA_INVALID;
        dsa.framebuffer = REGAL_DSA_INVALID;
        dsa.framebufferTarget = REGAL_DSA_INVALID;
        dsa.renderbuffer = REGAL_DSA_INVALID;
        dsa.renderbufferTarget = REGAL_DSA_INVALID;
    }

    void Cleanup( RegalContext &ctx )
    {
        UNUSED_PARAMETER(ctx);
    }

    void Restore( RegalContext * ctx ) {
        RestoreMatrixMode( ctx );
        RestoreGlslProgram( ctx );
        RestoreFramebuffer( ctx );
        RestoreAsmProgram( ctx, GL_VERTEX_PROGRAM_ARB );
        RestoreAsmProgram( ctx, GL_FRAGMENT_PROGRAM_ARB );
        RestoreAsmProgram( ctx, GL_GEOMETRY_PROGRAM_NV );
        RestoreAsmProgram( ctx, GL_TESS_CONTROL_PROGRAM_NV );
        RestoreAsmProgram( ctx, GL_TESS_EVALUATION_PROGRAM_NV );
        RestoreVao( ctx );
        RestoreBuffer( ctx );
        RestoreTexture( ctx );
        RestoreActiveTexture( ctx );  // flushTexture() may dirty this
        RestoreClientActiveTexture( ctx );
    }


    // state shadow/dirty/restore methods

    ////////////////////////////////////////////////////////////////////////
    bool NotMatrixMode( GLenum mode ) const {
        return mode != ( dsa.matrixMode != REGAL_DSA_INVALID ? dsa.matrixMode : drv.matrixMode );
    }
    bool ShadowMatrixMode( GLenum realMatrixMode ) {
        drv.matrixMode = realMatrixMode;
        return dsa.matrixMode != REGAL_DSA_INVALID;
    }
    void DsaMatrixMode( RegalContext * ctx, GLenum mode ) {
        if( NotMatrixMode( mode ) ) {
            dsa.matrixMode = mode;
            switch( mode ) {
                //<>  dsn: wtf?  Why are these accepted?  And why only up to GL_TEXTURE3?
                case GL_TEXTURE0: case GL_TEXTURE1: case GL_TEXTURE2: case GL_TEXTURE3:
                    DsaActiveTexture( ctx, mode );
                    dsa.matrixMode = GL_TEXTURE;
                    break;
            }
            ctx->dispatcher.emulation.glMatrixMode( dsa.matrixMode );
        }
    }
    void RestoreMatrixMode( RegalContext * ctx ) {
        if( dsa.matrixMode != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glMatrixMode( drv.matrixMode );
            RestoreActiveTexture( ctx );
            dsa.matrixMode = REGAL_DSA_INVALID;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotActiveTexture( GLenum tex ) const {
        return tex != ( dsa.activeTexture != REGAL_DSA_INVALID ? dsa.activeTexture : drv.activeTexture );
    }
    bool ShadowActiveTexture( GLenum realActiveTexture ) {
        if ( !validTextureEnum(realActiveTexture) )
            return false;
        drv.activeTexture = realActiveTexture;
        bool usingDsa = dsa.activeTexture != REGAL_DSA_INVALID;
        return usingDsa;
    }
    void DsaActiveTexture( RegalContext * ctx, GLenum tex) {
        if ( !validTextureEnum(tex) )
            return;
        if( NotActiveTexture( tex ) ) {
            dsa.activeTexture = tex;
            ctx->dispatcher.emulation.glActiveTexture( dsa.activeTexture );
        }
    }
    void RestoreActiveTexture( RegalContext * ctx ) {
        if( dsa.activeTexture != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glActiveTexture( drv.activeTexture );
            dsa.activeTexture = REGAL_DSA_INVALID;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotClientActiveTexture( GLenum tex ) const {
        return tex != ( dsa.clientActiveTexture != REGAL_DSA_INVALID ? dsa.clientActiveTexture : drv.clientActiveTexture );
    }
    bool ShadowClientActiveTexture( GLenum realClientActiveTexture ) {
        drv.clientActiveTexture = realClientActiveTexture;
        return dsa.clientActiveTexture != REGAL_DSA_INVALID;
    }
    void DsaClientActiveTexture( RegalContext * ctx, GLenum tex ) {
        if( NotClientActiveTexture( tex ) ) {
            dsa.clientActiveTexture = tex;
            ctx->dispatcher.emulation.glClientActiveTexture( dsa.clientActiveTexture );
        }
    }
    void RestoreClientActiveTexture( RegalContext * ctx ) {
        if( dsa.clientActiveTexture != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glClientActiveTexture( drv.clientActiveTexture );
            dsa.clientActiveTexture = REGAL_DSA_INVALID;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotGlslProgram( GLuint program ) const {
        return program != ( dsa.glslProgram != REGAL_DSA_INVALID ? dsa.glslProgram : drv.glslProgram );
    }
    bool ShadowGlslProgram( GLuint realGlslProgram ) {
        drv.glslProgram = realGlslProgram;
        return dsa.glslProgram != REGAL_DSA_INVALID;
    }
    void DsaGlslProgram( RegalContext * ctx, GLuint program ) {
        if( NotGlslProgram( program ) ) {
            dsa.glslProgram = program;
            ctx->dispatcher.emulation.glUseProgram( dsa.glslProgram );
        }
    }
    void RestoreGlslProgram( RegalContext * ctx ) {
        if( dsa.glslProgram != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glUseProgram( drv.glslProgram );
            dsa.glslProgram = REGAL_DSA_INVALID;
        }
    }
    void DeleteGlslProgram( RegalContext * ctx, GLuint program ) {
        if( drv.glslProgram == program ) {
            drv.glslProgram = 0;
        }
        RestoreGlslProgram( ctx );
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotFramebuffer( GLenum target, GLuint framebuffer ) const {
        return dsa.framebuffer != REGAL_DSA_INVALID ?
        ( ( target != dsa.framebufferTarget ) || ( framebuffer != dsa.framebuffer ) ) :
        ( ( target != drv.framebufferTarget ) || ( framebuffer != drv.framebuffer ) ) ;

    }
    bool ShadowFramebuffer( GLenum realFramebufferTarget, GLuint realFramebuffer ) {
        drv.framebufferTarget = realFramebufferTarget;
        drv.framebuffer = realFramebuffer;
        return dsa.framebuffer != REGAL_DSA_INVALID;
    }
    void DsaFramebuffer( RegalContext * ctx, GLenum target, GLuint framebuffer ) {
        if( NotFramebuffer( target, framebuffer ) ) {
            dsa.framebufferTarget = target;
            dsa.framebuffer = framebuffer;
            ctx->dispatcher.emulation.glBindFramebuffer( dsa.framebufferTarget, dsa.framebuffer );
        }
    }
    void RestoreFramebuffer( RegalContext * ctx ) {
        if( dsa.framebuffer != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glBindFramebuffer( drv.framebufferTarget, drv.framebuffer );
            dsa.framebufferTarget = REGAL_DSA_INVALID;
            dsa.framebuffer = REGAL_DSA_INVALID;
        }
    }
    void DeleteFramebuffers( RegalContext * ctx, GLsizei n, const GLuint * framebuffers ) {
        for( GLsizei i = 0; i < n; i++ ) {
            if( drv.framebuffer == framebuffers[i] ) {
                drv.framebuffer = 0;
            }
        }
        RestoreFramebuffer( ctx );
    }
    ////////////////////////////////////////////////////////////////////////
    bool NotRenderbuffer( GLenum target, GLuint renderbuffer ) const {
        return dsa.renderbuffer != REGAL_DSA_INVALID ?
        ( ( target != dsa.renderbufferTarget ) || ( renderbuffer != dsa.renderbuffer ) ) :
        ( ( target != drv.renderbufferTarget ) || ( renderbuffer != drv.renderbuffer ) ) ;

    }
    bool ShadowRenderbuffer( GLenum realRenderbufferTarget, GLuint realRenderbuffer ) {
        drv.renderbufferTarget = realRenderbufferTarget;
        drv.renderbuffer = realRenderbuffer;
        return dsa.renderbuffer != REGAL_DSA_INVALID;
    }
    void DsaRenderbuffer( RegalContext * ctx, GLenum target, GLuint renderbuffer ) {
        if( NotRenderbuffer( target, renderbuffer ) ) {
            dsa.renderbufferTarget = target;
            dsa.renderbuffer = renderbuffer;
            ctx->dispatcher.emulation.glBindRenderbuffer( dsa.renderbufferTarget, dsa.renderbuffer );
        }
    }
    void RestoreRenderbuffer( RegalContext * ctx ) {
        if( dsa.renderbuffer != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glBindRenderbuffer( drv.renderbufferTarget, drv.renderbuffer );
            dsa.renderbufferTarget = REGAL_DSA_INVALID;
            dsa.renderbuffer = REGAL_DSA_INVALID;
        }
    }
    void DeleteRenderbuffers( RegalContext * ctx, GLsizei n, const GLuint *renderbuffers ) {
        for( int i  = 0; i < n; i++ ) {
            if( renderbuffers[i] == drv.renderbuffer ) {
                drv.renderbuffer = 0;
            }
        }
        RestoreRenderbuffer( ctx );
    }

#define REGAL_DSA_NUM_ASM_TARGET_INDEXES 5
    static GLenum IndexToAsmTarget( int i ) {
        const GLenum array[] = {
            GL_VERTEX_PROGRAM_ARB,
            GL_FRAGMENT_PROGRAM_ARB,
            GL_GEOMETRY_PROGRAM_NV,
            GL_TESS_CONTROL_PROGRAM_NV,
            GL_TESS_EVALUATION_PROGRAM_NV
        };
        return array[i];
    }
    static int AsmTargetIndex( GLenum target ) {
        switch (target) {
            case GL_VERTEX_PROGRAM_ARB: return 0;
            case GL_FRAGMENT_PROGRAM_ARB: return 1;
            case GL_GEOMETRY_PROGRAM_NV: return 2;
            case GL_TESS_CONTROL_PROGRAM_NV: return 3;
            case GL_TESS_EVALUATION_PROGRAM_NV: return 4;
            default:
                break;
        }
        // freak out
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotAsmProgram( GLenum target, GLuint prog ) const {
        int idx = AsmTargetIndex( target );
        return prog != ( dsa.asmProgram[idx] != REGAL_DSA_INVALID ? dsa.asmProgram[idx] : drv.asmProgram[idx] );
    }
    bool ShadowAsmProgram( GLenum target, GLuint prog ) {
        int idx = AsmTargetIndex( target );
        drv.asmProgram[idx] = prog;
        return dsa.asmProgram[idx] != REGAL_DSA_INVALID;
    }
    void DsaAsmProgram( RegalContext * ctx, GLenum target, GLuint prog ) {
        int idx = AsmTargetIndex( target );
        if( NotAsmProgram( target, prog ) ) {
            dsa.asmProgram[idx] = prog;
            ctx->dispatcher.emulation.glBindProgramARB( target, dsa.asmProgram[idx] );
        }
    }
    void RestoreAsmProgram( RegalContext * ctx, GLenum target ) {
        int idx = AsmTargetIndex( target );
        if( dsa.asmProgram[idx] != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glBindProgramARB( target, drv.asmProgram[idx] );
            dsa.asmProgram[idx] = REGAL_DSA_INVALID;
        }
    }
    void DeleteAsmPrograms( RegalContext * ctx, GLsizei n, const GLuint *progs ) {
        for( GLsizei i  = 0; i < n; i++ ) {
            for( int j = 0; j < REGAL_DSA_NUM_ASM_TARGET_INDEXES; j++ ) {
                if( progs[i] == drv.asmProgram[ j ] ) {
                    drv.asmProgram[ j ] = 0;
                    RestoreAsmProgram( ctx, IndexToAsmTarget( j ) );
                }
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////
    bool NotVao( GLuint vao ) const {
        return vao != ( dsa.vao != REGAL_DSA_INVALID ? dsa.vao : drv.vao );
    }
    bool ShadowVao( GLuint vao ) {
        drv.vao = vao;
        return dsa.vao != REGAL_DSA_INVALID;
    }
    void DsaVao( RegalContext * ctx, GLuint vao ) {
        if( NotVao( vao ) ) {
            dsa.vao = vao;
            ctx->dispatcher.emulation.glBindVertexArray( dsa.vao );
        }
    }
    void RestoreVao( RegalContext * ctx ) {
        if( dsa.vao != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glBindVertexArray( drv.vao );
            dsa.vao = REGAL_DSA_INVALID;
        }
    }
    void DeleteVaos( RegalContext * ctx, GLsizei n, const GLuint * arrays ) {
        for( GLsizei i = 0; i < n; i++ ) {
            if( drv.vao == arrays[i] ) {
                drv.vao = 0;
            }
        }
        RestoreVao( ctx );
    }



    ////////////////////////////////////////////////////////////////////////
    bool NotBuffer( GLuint buf ) const {
        return buf != ( dsa.buffer != REGAL_DSA_INVALID ? dsa.buffer : drv.buffer );
    }
    bool ShadowBuffer( GLenum target, GLuint buf ) {
        if( target == GL_ARRAY_BUFFER ) {
            drv.buffer = buf;
            return dsa.buffer != REGAL_DSA_INVALID;
        }
        return false;
    }
    void DsaBuffer( RegalContext * ctx, GLuint buf ) {
        if( NotBuffer( buf ) ) {
            dsa.buffer = buf;
            ctx->dispatcher.emulation.glBindBuffer( GL_ARRAY_BUFFER, dsa.buffer );
        }
    }
    void RestoreBuffer( RegalContext * ctx ) {
        if( dsa.buffer != REGAL_DSA_INVALID ) {
            ctx->dispatcher.emulation.glBindBuffer( GL_ARRAY_BUFFER, drv.buffer );
            dsa.buffer = REGAL_DSA_INVALID;
        }
    }
    void DeleteBuffers( RegalContext * ctx, GLsizei n, const GLuint * buffers ) {
        for( GLsizei i = 0; i < n; i++ ) {
            if( drv.buffer == buffers[i] ) {
                drv.buffer = 0;
            }
        }
        RestoreBuffer( ctx );
    }

    ////////////////////////////////////////////////////////////////////////
    bool NotTexture( GLenum target, GLuint texture ) const {
        return dsa.texture != REGAL_DSA_INVALID ?
        ( target != dsa.textureTarget || texture != dsa.texture ) :
        ( target != drv.textureTarget || texture != drv.texture ) ;
    }

    bool ShadowTexture( GLenum target, GLuint texture ) {
        if( drv.activeTexture == GL_TEXTURE0 ) {
            drv.textureTarget = target;
            drv.texture = texture;
            return dsa.texture != REGAL_DSA_INVALID;
        }
        return false;
    }
    void ShadowDsaTexture( GLenum target, GLuint texture ) {
        if( NotActiveTexture( GL_TEXTURE0 ) == false ) {
            drv.textureTarget = target;
            drv.texture = texture;
        }
    }
    void DsaTexture( RegalContext * ctx, GLenum target, GLuint texture ) {
        switch( target ) {
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
            case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                target = GL_TEXTURE_CUBE_MAP;
            default:
                break;
        }
        if( NotTexture( target, texture ) ) {
            dsa.textureTarget = target;
            dsa.texture = texture;
            DsaActiveTexture( ctx, GL_TEXTURE0 );
            ctx->dispatcher.emulation.glBindTexture( dsa.textureTarget, dsa.texture );
        }
    }
    void RestoreTexture( RegalContext * ctx ) {
        if( dsa.texture != REGAL_DSA_INVALID ) {
            DsaActiveTexture( ctx, GL_TEXTURE0 );
            ctx->dispatcher.emulation.glBindTexture( drv.textureTarget, drv.texture );
            dsa.textureTarget = REGAL_DSA_INVALID;
            dsa.texture = REGAL_DSA_INVALID;
        }
    }
    void DeleteTextures( RegalContext * ctx, GLsizei n, const GLuint *textures ) {
        for( int i  = 0; i < n; i++ ) {
            if( textures[i] == drv.texture ) {
                drv.texture = 0;
            }
        }
        RestoreTexture( ctx );
        RestoreActiveTexture( ctx );
    }

    void ClientAttribDefault( RegalContext * ctx, GLbitfield mask )
    {
        DispatchTableGL &tbl = ctx->dispatcher.emulation;

        if (mask&GL_CLIENT_PIXEL_STORE_BIT)
        {
            PFNGLPIXELSTOREIPROC pixelStorei = tbl.call(&tbl.glPixelStorei);
            PFNGLPIXELSTOREFPROC pixelStoref = tbl.call(&tbl.glPixelStoref);

            RegalAssert(pixelStorei);
            RegalAssert(pixelStoref);

            pixelStorei( GL_UNPACK_SWAP_BYTES, 0 );
            pixelStorei( GL_UNPACK_LSB_FIRST, 0 );
            pixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
            pixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
            pixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
            pixelStorei( GL_UNPACK_ALIGNMENT, 4 );
            pixelStorei( GL_UNPACK_IMAGE_HEIGHT, 0 );
            pixelStorei( GL_UNPACK_SKIP_IMAGES, 0 );
            pixelStorei( GL_PACK_SWAP_BYTES, 0 );
            pixelStorei( GL_PACK_LSB_FIRST, 0 );
            pixelStorei( GL_PACK_IMAGE_HEIGHT, 0 );
            pixelStorei( GL_PACK_SKIP_IMAGES, 0 );
            pixelStorei( GL_PACK_ROW_LENGTH, 0 );
            pixelStorei( GL_PACK_SKIP_ROWS, 0 );
            pixelStorei( GL_PACK_SKIP_PIXELS, 0 );
            pixelStorei( GL_PACK_ALIGNMENT, 4 );
            pixelStorei( GL_PIXEL_PACK_BUFFER_BINDING, 0 );
            pixelStorei( GL_PIXEL_UNPACK_BUFFER_BINDING, 0 );
            pixelStorei( GL_MAP_COLOR, 0 );
            pixelStorei( GL_MAP_STENCIL, 0 );
            pixelStorei( GL_INDEX_SHIFT, 0 );
            pixelStorei( GL_INDEX_OFFSET, 0 );

            pixelStoref( GL_RED_SCALE, 1.0f );
            pixelStoref( GL_GREEN_SCALE, 1.0f );
            pixelStoref( GL_BLUE_SCALE, 1.0f );
            pixelStoref( GL_ALPHA_SCALE, 1.0f );
            pixelStoref( GL_DEPTH_SCALE, 1.0f );
            pixelStoref( GL_RED_BIAS, 0.0f );
            pixelStoref( GL_GREEN_BIAS, 0.0f );
            pixelStoref( GL_BLUE_BIAS, 0.0f );
            pixelStoref( GL_ALPHA_BIAS, 0.0f );
            pixelStoref( GL_DEPTH_BIAS, 0.0f );
        }

        if (mask&GL_CLIENT_VERTEX_ARRAY_BIT)
        {
            // FIXME: need number of texture units
            int maxTextureUnit = 7;
            for( int i = maxTextureUnit; i >= 0; i-- ) {
                tbl.call(&tbl.glClientActiveTexture)( GL_TEXTURE0 + i );
                tbl.call(&tbl.glDisableClientState) ( GL_TEXTURE_COORD_ARRAY );
            }
            for( int i = 0; i < 16; i++ ) {
                tbl.call(&tbl.glDisableVertexAttribArray)( i );
                tbl.call(&tbl.glVertexAttribPointer)     ( i, 4, GL_FLOAT, GL_FALSE, 0, NULL );
            }
            tbl.call(&tbl.glDisableClientState)   ( GL_VERTEX_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_NORMAL_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_FOG_COORD_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_COLOR_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_SECONDARY_COLOR_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_INDEX_ARRAY );
            tbl.call(&tbl.glDisableClientState)   ( GL_EDGE_FLAG_ARRAY );
            tbl.call(&tbl.glVertexPointer)        ( 4, GL_FLOAT, 0, NULL );
            tbl.call(&tbl.glNormalPointer)        ( GL_FLOAT, 0, NULL );
            tbl.call(&tbl.glFogCoordPointer)      ( GL_FLOAT, 0, NULL );
            tbl.call(&tbl.glColorPointer)         ( 4, GL_FLOAT, 0, NULL );
            tbl.call(&tbl.glSecondaryColorPointer)( 3, GL_FLOAT, 0, NULL );
            tbl.call(&tbl.glIndexPointer)         ( GL_FLOAT, 0, NULL );
        }
    }

    template< typename T >
    bool GetIndexedv( RegalContext * ctx, GLenum pname, GLuint index, T * params ) {
        DispatchTableGL &tbl = ctx->dispatcher.emulation;
        switch( pname ) {
            case GL_PROGRAM_MATRIX_EXT:
                ctx->dsa->DsaMatrixMode( ctx, GL_MATRIX0_ARB + index );
                DsaGetv( tbl, GL_CURRENT_MATRIX_ARB, params );
                break;
            case GL_TRANSPOSE_PROGRAM_MATRIX_EXT:
                ctx->dsa->DsaMatrixMode( ctx, GL_MATRIX0_ARB + index );
                DsaGetv( tbl, GL_TRANSPOSE_CURRENT_MATRIX_ARB, params );
                break;
            case GL_PROGRAM_MATRIX_STACK_DEPTH_EXT:
                ctx->dsa->DsaMatrixMode( ctx, GL_MATRIX0_ARB + index );
                DsaGetv( tbl, GL_CURRENT_MATRIX_STACK_DEPTH_ARB, params );
                break;
            case GL_CURRENT_MATRIX_NV:
            case GL_CURRENT_MATRIX_STACK_DEPTH_NV:
            case GL_CURRENT_RASTER_TEXTURE_COORDS:
            case GL_CURRENT_TEXTURE_COORDS:
            case GL_TEXTURE_BINDING_1D:
            case GL_TEXTURE_BINDING_1D_ARRAY:
            case GL_TEXTURE_BINDING_2D:
            case GL_TEXTURE_BINDING_2D_ARRAY:
            case GL_TEXTURE_BINDING_3D:
            case GL_TEXTURE_BINDING_BUFFER_EXT:
            case GL_TEXTURE_BINDING_CUBE_MAP:
            case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
            case GL_TEXTURE_BINDING_RECTANGLE_ARB:
            case GL_TEXTURE_BINDING_RENDERBUFFER_NV:
            case GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV:
            case GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT:
            case GL_TEXTURE_BUFFER_FORMAT_EXT:
            case GL_TEXTURE_GEN_Q:
            case GL_TEXTURE_GEN_R:
            case GL_TEXTURE_GEN_S:
            case GL_TEXTURE_GEN_T:
            case GL_TEXTURE_MATRIX:
            case GL_TEXTURE_STACK_DEPTH:
            case GL_TRANSPOSE_TEXTURE_MATRIX:
            case GL_TEXTURE_1D: case GL_TEXTURE_2D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_RECTANGLE_ARB:
                ctx->dsa->DsaActiveTexture( ctx, GL_TEXTURE0 + index );
                DsaGetv( tbl, pname, params );
                break;
            case GL_TEXTURE_COORD_ARRAY:
            case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
                //case GL_TEXTURE_COORD_ARRAY_COUNT:
            case GL_TEXTURE_COORD_ARRAY_SIZE:
            case GL_TEXTURE_COORD_ARRAY_STRIDE:
            case GL_TEXTURE_COORD_ARRAY_TYPE:
                ctx->dsa->DsaClientActiveTexture( ctx, GL_TEXTURE0 + index );
                DsaGetv( tbl, pname, params );
                break;
            default:
                return false;
        }
        return true;
    }

    void RestoreGet( RegalContext * ctx, GLenum pname ) {
        switch( pname ) {
            case GL_CURRENT_PROGRAM:
                ctx->dsa->RestoreGlslProgram( ctx );
                break;
            case GL_MATRIX_MODE:
            case GL_PROGRAM_MATRIX_EXT:
            case GL_TRANSPOSE_PROGRAM_MATRIX_EXT:
            case GL_PROGRAM_MATRIX_STACK_DEPTH_EXT:
                ctx->dsa->RestoreMatrixMode( ctx );
                break;
            case GL_ACTIVE_TEXTURE:
            case GL_CURRENT_MATRIX_NV:
            case GL_CURRENT_MATRIX_STACK_DEPTH_NV:
            case GL_CURRENT_RASTER_TEXTURE_COORDS:
            case GL_CURRENT_TEXTURE_COORDS:
            case GL_TEXTURE_BINDING_1D:
            case GL_TEXTURE_BINDING_1D_ARRAY:
            case GL_TEXTURE_BINDING_2D:
            case GL_TEXTURE_BINDING_2D_ARRAY:
            case GL_TEXTURE_BINDING_3D:
            case GL_TEXTURE_BINDING_BUFFER_EXT:
            case GL_TEXTURE_BINDING_CUBE_MAP:
            case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
            case GL_TEXTURE_BINDING_RECTANGLE_ARB:
            case GL_TEXTURE_BINDING_RENDERBUFFER_NV:
            case GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV:
            case GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT:
            case GL_TEXTURE_BUFFER_FORMAT_EXT:
            case GL_TEXTURE_GEN_Q:
            case GL_TEXTURE_GEN_R:
            case GL_TEXTURE_GEN_S:
            case GL_TEXTURE_GEN_T:
            case GL_TEXTURE_MATRIX:
            case GL_TEXTURE_STACK_DEPTH:
            case GL_TRANSPOSE_TEXTURE_MATRIX:
            case GL_TEXTURE_1D: case GL_TEXTURE_2D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_RECTANGLE_ARB:
                ctx->dsa->RestoreActiveTexture( ctx );
                break;
            case GL_CLIENT_ACTIVE_TEXTURE:
            case GL_TEXTURE_COORD_ARRAY:
            case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
                //case GL_TEXTURE_COORD_ARRAY_COUNT:
            case GL_TEXTURE_COORD_ARRAY_SIZE:
            case GL_TEXTURE_COORD_ARRAY_STRIDE:
            case GL_TEXTURE_COORD_ARRAY_TYPE:
                ctx->dsa->RestoreClientActiveTexture( ctx );
                break;
            case GL_DRAW_BUFFER:
            case GL_READ_BUFFER:
            case GL_DRAW_BUFFER0:
            case GL_DRAW_BUFFER1:
            case GL_DRAW_BUFFER2:
            case GL_DRAW_BUFFER3:
            case GL_DRAW_BUFFER4:
            case GL_DRAW_BUFFER5:
            case GL_DRAW_BUFFER6:
            case GL_DRAW_BUFFER7:
            case GL_DRAW_BUFFER8:
            case GL_DRAW_BUFFER9:
            case GL_DRAW_BUFFER10:
            case GL_DRAW_BUFFER11:
            case GL_DRAW_BUFFER12:
            case GL_DRAW_BUFFER13:
            case GL_DRAW_BUFFER14:
            case GL_DRAW_BUFFER15:
                ctx->dsa->RestoreFramebuffer( ctx );

            default:
                break;
        }
    }

    GLboolean IsEnabledIndexed( RegalContext * ctx, GLenum pname, GLuint index ) {
        DispatchTableGL &tbl = ctx->dispatcher.emulation;
        switch( pname ) {
            case GL_TEXTURE_1D:
            case GL_TEXTURE_2D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_RECTANGLE_ARB:
            case GL_TEXTURE_GEN_S:
            case GL_TEXTURE_GEN_T:
            case GL_TEXTURE_GEN_R:
            case GL_TEXTURE_GEN_Q:
                ctx->dsa->DsaActiveTexture( ctx, GL_TEXTURE0 + index );
                return tbl.glIsEnabled( pname );
                break;
            case GL_TEXTURE_COORD_ARRAY:
                ctx->dsa->DsaClientActiveTexture( ctx, GL_TEXTURE0 + index );
                return tbl.glIsEnabled( pname );
            default:
                break;
        }
        return GL_FALSE;
    }

    void RestoreIsEnabled( RegalContext * ctx, GLenum pname ) {
        switch( pname ) {
            case GL_TEXTURE_1D:
            case GL_TEXTURE_2D:
            case GL_TEXTURE_3D:
            case GL_TEXTURE_CUBE_MAP:
            case GL_TEXTURE_RECTANGLE_ARB:
            case GL_TEXTURE_GEN_S:
            case GL_TEXTURE_GEN_T:
            case GL_TEXTURE_GEN_R:
            case GL_TEXTURE_GEN_Q:
                ctx->dsa->RestoreActiveTexture( ctx );
                break;
            case GL_TEXTURE_COORD_ARRAY:
                ctx->dsa->RestoreClientActiveTexture( ctx );
            default:
                break;
        }
    }

};

}

REGAL_NAMESPACE_END

#endif // ! __REGAL_DSA_H__







