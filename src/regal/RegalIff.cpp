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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <string>
using std::string;

#include <boost/print/string_list.hpp>
typedef boost::print::string_list<string> string_list;

#include "RegalIff.h"
#include "RegalLog.h"
#include "RegalToken.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using namespace ::REGAL_NAMESPACE_INTERNAL::Logging;
using namespace ::REGAL_NAMESPACE_INTERNAL::Token;

namespace {
    static int progcount = -1;

    typedef RegalIff RFF;
    typedef RegalIff::State State;
    typedef RegalIff::State::Texture Texture;
    typedef RegalIff::State::Light Light;
    typedef RegalIff::State::Material Material;
    typedef RegalIff::State::Store Store;
    typedef RegalIff::Program Program;

    enum FFLightEl {
        LE_Ambient = 0,
        LE_Diffuse = 1,
        LE_Specular = 2,
        LE_Position = 3,
        LE_SpotDir = 4,
        LE_Atten = 5,
        LE_Elements = 6
    };
    enum FFMaterialEl {
        ME_Ambient = 0,
        ME_Diffuse = 1,
        ME_Specular = 2,
        ME_Emission = 3,
        ME_Shininess = 4,
        ME_Elements = 5
    };

    RFF::TextureTargetBitfield TargetToBitfield( GLenum target ) {
        switch( target ) {
            case GL_TEXTURE_1D:        return RFF::TT_1D;
            case GL_TEXTURE_2D:        return RFF::TT_2D;
            case GL_TEXTURE_RECTANGLE: return RFF::TT_Rect;
            case GL_TEXTURE_3D:        return RFF::TT_3D;
            case GL_TEXTURE_CUBE_MAP:  return RFF::TT_CubeMap;
            default: break;
        }
        return RFF::TT_None;
    }



    void GenerateVertexShaderSource( const RFF * rff, const RFF::State & state, string_list & src ) {
        const bool es = rff->es;
        const bool legacy = rff->legacy;
        const Store & st = state.processed;

        bool hasNormalMap = false;
        bool hasSphereMap = false;
        bool hasReflectionMap = false;
        bool hasEyeLinearTexGen = false;
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            if( st.tex[i].enables == 0 ) {
                continue;
            }
            for( int j = 0; j < 4; j++ ) {
                if( st.tex[i].texgen[j].enable == false ) {
                    continue;
                }
                RFF::TexgenMode mode = st.tex[i].texgen[j].mode;
                hasNormalMap = hasNormalMap || mode == RFF::TG_NormalMap;
                hasSphereMap = hasSphereMap || mode == RFF::TG_SphereMap;
                hasReflectionMap = hasReflectionMap || mode == RFF::TG_ReflectionMap;
                hasEyeLinearTexGen = hasEyeLinearTexGen || mode == RFF::TG_EyeLinear;
            }
        }
        bool hasClipPlanes = false;
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
            hasClipPlanes = hasClipPlanes || st.clip[i].enable;
        }

        if( es ) {
            src << "#version 100\n";
        } else if ( legacy ) {
            src << "#version 120\n";
        } else {
            src << "#version 140\n";
        }
        src << "// program number " << progcount << "\n";

        if( es || legacy ) {
            src << "#define in attribute\n";
            src << "#define out varying\n";
        }

        if( st.shadeModelFlat && ! legacy && ! es ) {
            src << "#define FLAT flat\n";
        } else {
            src << "#define FLAT  \n";
        }
        if( st.lighting ) {
            src << "\n";
            src << "#define ME_AMBIENT " << ME_Ambient << "\n";
            src << "#define ME_DIFFUSE " << ME_Diffuse << "\n";
            src << "#define ME_SPECULAR " << ME_Specular << "\n";
            src << "#define ME_EMISSION " << ME_Emission << "\n";
            src << "#define ME_SHININESS " << ME_Shininess << "\n";
            src << "#define ME_ELEMENTS " << ME_Elements << "\n";
            src << "\n";
            src << "#define LE_AMBIENT " << LE_Ambient << "\n";
            src << "#define LE_DIFFUSE " << LE_Diffuse << "\n";
            src << "#define LE_SPECULAR " << LE_Specular << "\n";
            src << "#define LE_POSITION " << LE_Position << "\n";
            src << "#define LE_SPOTDIR " << LE_SpotDir << "\n";
            src << "#define LE_ATTEN " << LE_Atten << "\n";
            src << "#define LE_ELEMENTS " << LE_Elements << "\n";
            src << "\n";
        }

        if( es ) {
            src << "precision highp float;\n";
        }
        if( ~st.attrArrayFlags & ( 1 << rff->ffAttrMap[ RFF2A_Color ] ) ) {
            src << "uniform vec4 rglAttrib[" << REGAL_MAX_VERTEX_ATTRIBS << "];\n";
        }
        src << "uniform mat4 rglModelview;\n";
        src << "uniform mat4 rglProjection;\n";
        src << "in vec4 rglVertex;\n";
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            if( st.tex[i].useMatrix ) {
                src << "uniform mat4 rglTextureMatrix" << i << ";\n";
            }
        }
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
            if( st.clip[i].enable ) {
                src << "uniform vec4 rglClipPlane" << i << ";\n";
                if( es || legacy ) {
                    src << "out float rglClipDistance" << i << ";\n";
                }
            }
        }
        //src << "in vec4 rglWeight;\n";
        if( st.lighting || hasNormalMap || hasSphereMap || hasReflectionMap ) {
            src << "uniform mat4 rglModelviewInverseTranspose;\n";
            src << "in vec3 rglNormal;\n";
        }
        if( st.lighting ) {
            src << "uniform vec4 rglLightModelAmbient;\n";
            src << "uniform vec4 rglMaterialFront" << "[ ME_ELEMENTS ];\n";
            if( st.lightModelTwoSide ) {
                src << "uniform vec4 rglMaterialBack" << "[ ME_ELEMENTS ];\n";
            }
            for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
                if( st.light[ i ].enable ) {
                    src << "uniform vec4 rglLight" << i << "[ LE_ELEMENTS ];\n";
                }
            }
        }
        if( st.attrArrayFlags & ( 1 << rff->ffAttrMap[ RFF2A_Color ] ) && ( st.lighting == false || st.colorMaterial ) ) {
            src << "in vec4 rglColor;\n";
        } else {
            src << "#define rglColor rglAttrib[" << rff->ffAttrMap[ RFF2A_Color ] << "]\n";
        }
        if( st.colorSum && st.lighting == false ) {
            src << "in vec4 rglSecondaryColor;\n";
        }
        if( st.fog.enable ) {
            src << "uniform vec4 rglFog[2];\n";
            src << "out vec4 rglFOG;\n";
            if(  st.fog.useDepth == false ) {
                src << "in float rglFogCoord;\n";
            }
        }
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            if( st.tex[i].enables == 0 ) {
                continue;
            }
            src << "in vec4 rglMultiTexCoord" << i << ";\n";
        }
        src << "FLAT out vec4 rglFrontColor;\n";
        if( st.lighting ) {
            if( st.lightModelTwoSide ) {
                src << "FLAT out vec4 rglBackColor;\n";
            }
            if( st.lightModelSeparateSpecular ) {
                src << "out vec4 rglSCOL0;\n";
                if( st.lightModelTwoSide ) {
                    src << "out vec4 rglSCOL1;\n";
                }
            }
        } else if( st.colorSum ) {
            src << "out vec4 rglSCOL0;\n";
        }

        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            const State::Texture & t = st.tex[i];
            if( t.enables == 0 ) {
                continue;
            }
            const char *tc[] = { "S", "T", "R", "Q" };
            for( int j = 0; j < 4; j++ ) {
                const State::Texgen & g = t.texgen[j];
                if( g.enable == false ) {
                    continue;
                }
                switch( g.mode ) {
                    case RFF::TG_EyeLinear:
                        src << "uniform vec4 rglTexGen" << i << "Eye" << tc[j] << ";\n";
                        break;
                    case RFF::TG_ObjectLinear:
                        src << "uniform vec4 rglTexGen" << i << "Obj" << tc[j] << ";\n";
                        break;
                    default:
                        src << "//ERROR: unsupported gen mode\n";
                        break;
                }
            }
            src << "out vec4 rglTEXCOORD" << i << ";\n";
        }

        if( hasNormalMap ) {
            src << "vec4 ComputeNormalMap( vec3 n ) {\n";
            src << "    return vec4( n.x, n.y, n.z, 0.0 );\n";
            src << "}\n";
        }

        if( hasReflectionMap ) {
            src << "vec4 ComputeReflectionMap( vec3 i, vec3 n ) {\n";
            src << "    vec3 r = reflect( i, n );\n";
            src << "    return vec4( r.x, r.y, r.z, 0.0 );\n";
            src << "}\n";
        }

        if( hasSphereMap ) {
            src << "vec4 ComputeSphereMap( vec3 i, vec3 n ) {\n";
            src << "    vec3 r = reflect( i, n );\n";
            src << "    float minv = 1.0 / sqrt( 2 * r.x + 2 * r.y + 2 * (r.z + 1 ) );\n";
            src << "    return vec4( r.x * minv + 0.5 , r.y * minv + 0.5, 0.0, 0.0 );\n";
            src << "}\n";
        }

        src << "void main() {\n";
        src << "    gl_Position = rglProjection * rglModelview * rglVertex;\n";
        if( st.lighting || hasEyeLinearTexGen || hasClipPlanes || ( st.fog.enable && st.fog.useDepth ) ) {
            src << "    vec4 eh = rglModelview * rglVertex;\n";
        }

        if( st.lighting || hasNormalMap || hasReflectionMap ) {
            src << "    vec4 ep = eh / eh.w;\n";
            src << "    vec3 ev = -normalize( ep.xyz );\n";
            src << "    vec3 en = mat3( rglModelviewInverseTranspose ) * rglNormal;\n";
            if( st.normalize ) {
                src << "    en = normalize( en );\n";
            }
        }
        if( st.lighting ) {
            src << "    vec4 mFront[ ME_ELEMENTS ];\n";
            src << "    mFront[ ME_AMBIENT   ] = rglMaterialFront[ ME_AMBIENT   ];\n";
            src << "    mFront[ ME_DIFFUSE   ] = rglMaterialFront[ ME_DIFFUSE   ];\n";
            src << "    mFront[ ME_SPECULAR  ] = rglMaterialFront[ ME_SPECULAR  ];\n";
            src << "    mFront[ ME_EMISSION  ] = rglMaterialFront[ ME_EMISSION  ];\n";
            src << "    mFront[ ME_SHININESS ] = rglMaterialFront[ ME_SHININESS ];\n";
            if( st.lightModelTwoSide ) {
                src << "    vec4 mBack [ ME_ELEMENTS ];\n";
                src << "    mBack [ ME_AMBIENT   ] = rglMaterialBack [ ME_AMBIENT   ];\n";
                src << "    mBack [ ME_DIFFUSE   ] = rglMaterialBack [ ME_DIFFUSE   ];\n";
                src << "    mBack [ ME_SPECULAR  ] = rglMaterialBack [ ME_SPECULAR  ];\n";
                src << "    mBack [ ME_EMISSION  ] = rglMaterialBack [ ME_EMISSION  ];\n";
                src << "    mBack [ ME_SHININESS ] = rglMaterialBack [ ME_SHININESS ];\n";
            }
            if( st.colorMaterial ) {
                switch( st.colorMaterialTarget[0] ) {
                    case RFF::CM_None:
                        break;
                    case RFF::CM_Ambient:
                        src << "    mFront[ ME_AMBIENT ] = rglColor;\n";
                        break;
                    case RFF::CM_Diffuse:
                        src << "    mFront[ ME_DIFFUSE ] = rglColor;\n";
                        break;
                    case RFF::CM_Specular:
                        src << "    mFront[ ME_SPECULAR ] = rglColor;\n";
                        break;
                    case RFF::CM_Emission:
                        src << "    mFront[ ME_EMISSION ] = rglColor;\n";
                        break;
                    case RFF::CM_AmbientAndDiffuse:
                        src << "    mFront[ ME_AMBIENT ] = rglColor;\n";
                        src << "    mFront[ ME_DIFFUSE ] = rglColor;\n";
                        break;
                    default:
                        src << "//ERROR: unsupported color material[0] target\n";
                        break;
                }
                if( st.lightModelTwoSide ) {
                    switch( st.colorMaterialTarget[1] ) {
                        case RFF::CM_None:
                            break;
                        case RFF::CM_Ambient:
                            src << "    mBack[ ME_AMBIENT ] = rglColor;\n";
                            break;
                        case RFF::CM_Diffuse:
                            src << "    mBack[ ME_DIFFUSE ] = rglColor;\n";
                            break;
                        case RFF::CM_Specular:
                            src << "    mBack[ ME_SPECULAR ] = rglColor;\n";
                            break;
                        case RFF::CM_Emission:
                            src << "    mBack[ ME_EMISSION ] = rglColor;\n";
                            break;
                        case RFF::CM_AmbientAndDiffuse:
                            src << "    mBack[ ME_AMBIENT ] = rglColor;\n";
                            src << "    mBack[ ME_DIFFUSE ] = rglColor;\n";
                            break;
                        default:
                            src << "//ERROR: unsupported color material[1] target\n";
                            break;
                    }
                }
            }
            src << "    rglFrontColor.xyz = mFront[ ME_EMISSION ].xyz;\n";
            src << "    rglFrontColor.w = mFront[ ME_DIFFUSE ].w;\n";
            src << "    rglFrontColor.xyz += rglLightModelAmbient.xyz * mFront[ ME_AMBIENT ].xyz;\n";
            if( st.lightModelSeparateSpecular ) {
                src << "    rglSCOL0 = vec4( 0, 0, 0, 0);\n";
            }
            if( st.lightModelTwoSide ) {
                src << "    rglBackColor.xyz = mBack[ ME_EMISSION ].xyz;\n";
                src << "    rglBackColor.w = mBack[ ME_DIFFUSE ].w;\n";
                src << "    rglBackColor.xyz += rglLightModelAmbient.xyz * mBack[ ME_AMBIENT ].xyz;\n";
                if( st.lightModelSeparateSpecular ) {
                    src << "    rglSCOL1 = vec4( 0, 0, 0, 0);\n";
                }
            }
            for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
                if( st.light[ i ].enable ) {
                    src << "    {\n";
                    string attenmul = "";
                    src << "        vec4 lvec = rglLight" << i << "[ LE_POSITION ];\n";
                    if( st.light[ i ].attenuate || st.light[ i ].spotlight ) {
                        src << "        float att = 1.0;\n";
                        attenmul = "att *";
                    }
                    if( st.light[ i ].position.w != 0.0 ) {
                        src << "        lvec.xyz -= ep.xyz;\n";
                        src << "        float ad = sqrt( dot( lvec.xyz, lvec.xyz ) );\n";
                        src << "        lvec.xyz /= ad;\n";
                    }
                    if( st.lightModelLocalViewer ) {
                        src << "        vec3 hvec = normalize( lvec.xyz + ev );\n";
                    } else {
                        src << "        vec3 hvec = normalize( lvec.xyz + vec3( 0, 0, 1 ) );\n";
                    }
                    src << "        vec3 ambient = rglLight" << i << "[ LE_AMBIENT ].xyz * mFront[ ME_AMBIENT ].xyz;\n";
                    src << "        float dc = max( dot( en, lvec.xyz ), 0.0 );\n";
                    src << "        float sc = max( dot( en, hvec ), 0.0 );\n";
                    src << "        vec3 diffuse = dc * rglLight" << i << "[ LE_DIFFUSE ].xyz * mFront[ ME_DIFFUSE ].xyz;\n";
                    src << "        sc = ( dc > 0.0 && sc > 0.0 ) ? exp( mFront[ ME_SHININESS ].x * log( sc ) ) : 0.0;\n";
                    src << "        vec3 specular = sc * rglLight" << i << "[ LE_SPECULAR ].xyz * mFront[ ME_SPECULAR ].xyz;\n";
                    if( st.light[ i ].attenuate && st.light[ i ].position.w != 0.0 ) {
                        src << "        vec3 dist = vec3( 1.0, ad, ad * ad );\n";
                        src << "        att = 1.0 / dot( dist, rglLight" << i << "[ LE_ATTEN ].xyz );\n";
                    }
                    if( st.light[ i ].spotlight ) {
                        src << "        float spcut = rglLight" << i << "[ LE_SPOTDIR ].w;\n";
                        src << "        float sd = dot( rglLight" << i << "[ LE_SPOTDIR ].xyz, lvec.xyz );\n";
                        src << "        att *= ( sd > spcut ) ? exp( rglLight" << i << "[ LE_ATTEN ].w * log( sd ) ) : 0.0;\n";
                    }

                    if( st.lightModelSeparateSpecular ) {
                        src << "        rglFrontColor.xyz += " << attenmul << " ( ambient + diffuse );\n";
                        src << "        rglSCOL0.xyz += " << attenmul << " specular;\n";
                    } else {
                        src << "        rglFrontColor.xyz += " << attenmul << " ( ambient + diffuse + specular );\n";
                    }
                    if( st.lightModelTwoSide ) {
                        src << "        ambient = rglLight" << i << "[ LE_AMBIENT ].xyz * mBack[ ME_AMBIENT ].xyz;\n";
                        src << "        dc = max( dot( -en, lvec.xyz ), 0.0 );\n";
                        src << "        sc = max( dot( -en, hvec ), 0.0 );\n";
                        src << "        diffuse = dc * rglLight" << i << "[ LE_DIFFUSE ].xyz * mBack[ ME_DIFFUSE ].xyz;\n";
                        src << "        sc = ( dc > 0.0 && sc > 0.0 ) ? exp( mBack[ ME_SHININESS ].x * log( sc ) ) : 0.0;\n";
                        src << "        specular = sc * rglLight" << i << "[ LE_SPECULAR ].xyz * mBack[ ME_SPECULAR ].xyz;\n";
                        if( st.lightModelSeparateSpecular ) {
                            src << "        rglBackColor.xyz += " << attenmul << " ( ambient + diffuse );\n";
                            src << "        rglSCOL1.xyz += " << attenmul << " specular;\n";
                        } else {
                            src << "        rglBackColor.xyz += " << attenmul << " ( ambient + diffuse + specular );\n";
                        }
                    }
                    src << "    }\n";
                }
            }
        } else {
            src << "    rglFrontColor = rglColor;\n";
            if( st.colorSum ) {
                src << "    rglSCOL0 = rglSecondaryColor;\n";
            }
        }

        // clamp all the output colors to (0.0, 1.0)

        src << "    rglFrontColor = clamp( rglFrontColor, 0.0, 1.0 );\n";
        if( st.lighting ) {
            if( st.lightModelTwoSide ) {
                src << "    rglBackColor = clamp( rglBackColor, 0.0, 1.0 );\n";
            }
            if( st.lightModelSeparateSpecular ) {
                src << "    rglSCOL0 = clamp( rglSCOL0, 0.0, 1.0 );\n";
                if( st.lightModelTwoSide ) {
                    src << "    rglSCOL1 = clamp( rglSCOL1, 0.0, 1.0 );\n";
                }
            }
        } else if( st.colorSum ) {
            src << "    rglSCOL0 = clamp( rglSCOL0, 0.0, 1.0 );\n";
        }

        if( st.fog.enable ) {
            if( st.fog.useDepth ) {
                src << "    rglFOG = eh;\n";
            } else {
                src << "    rglFOG = vec4(0, 0, -rglFogCoord, 1);\n";
            }
        }

        if( hasNormalMap ) {
            src << "    vec4 nmTc = ComputeNormalMap( en );\n";
        }
        if( hasReflectionMap ) {
            src << "    vec4 rmTc = ComputeReflectionMap( -ev, en );\n";
        }
        if( hasSphereMap ) {
            src << "    vec4 smTc = ComputeSphereMap( -ev, en );\n";
        }

        bool tc_declared = false;
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            const State::Texture & t = state.processed.tex[i];
            if( t.enables == 0 ) {
                continue;
            }
            if ( !tc_declared ) {
                src << "    vec4 tc;\n";
                tc_declared = true;
            }
            src << "    tc = rglMultiTexCoord" << i << ";\n";
            const char *comp[] = { "x", "y", "z", "w" };
            const char *tc[] = { "S", "T", "R", "Q" };
            for( int j = 0; j < 4; j++ ) {
                if( t.texgen[j].enable == false ) {
                    continue;
                }
                switch( t.texgen[j].mode ) {
                    case RFF::TG_ObjectLinear:
                        src << "    tc." << comp[j] << " = dot( rglTexGen" << i << "Obj" << tc[j] << ", rglVertex );\n";
                        break;
                    case RFF::TG_EyeLinear:
                        src << "    tc." << comp[j] << " = dot( rglTexGen" << i << "Eye" << tc[j] << ", eh );\n";
                        break;
                    case RFF::TG_NormalMap:
                        src << "    tc." << comp[j] << " = nmTc." << comp[j] << ";\n";
                        break;
                    case RFF::TG_ReflectionMap:
                        src << "    tc." << comp[j] << " = rmTc." << comp[j] << ";\n";
                        break;
                    case RFF::TG_SphereMap:
                        src << "    tc." << comp[j] << " = smTc." << comp[j] << ";\n";
                        break;
                    default:
                        src << "//ERROR: unsupported tex gen mode\n";
                        break;
                }
            }
            if( t.useMatrix ) {
                src << "    rglTEXCOORD" << i << " = rglTextureMatrix" << i << " * tc;\n";
            } else {
                src << "    rglTEXCOORD" << i << " = tc;\n";
            }
        }
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
            if( st.clip[i].enable ) {
                if( es || legacy ) {
                    src << "    rglClipDistance" << i;
                } else {
                    src << "    gl_ClipDistance[" << i << "]";
                }
                src << " = dot( eh, rglClipPlane" << i << " );\n";
            }
        }
        src << "}\n";
    }



    void AddTexEnv( int i, RFF::TexenvMode env, GLenum fmt,  string_list & s )
    {
        switch( env ) {
            case RFF::TEM_Replace:
                switch( fmt ) {
                    case GL_ALPHA:
                        s << "    p.w = s.w;\n";
                        break;
                    case GL_LUMINANCE:
                    case GL_RGB:
                        s << "    p.xyz = s.xyz;\n";
                        break;
                    case GL_LUMINANCE_ALPHA:
                    case GL_RGBA:
                        s << "    p = s;\n";
                        break;
                    default:
                        s << "    //ERROR: Unsupported tex fmt\n";
                        break;
                }
                break;
            case RFF::TEM_Modulate:
                switch( fmt ) {
                    case GL_ALPHA:
                        s << "    p.w *= s.w;\n";
                        break;
                    case GL_LUMINANCE:
                    case GL_RGB:
                         s << "    p.xyz *= s.xyz;\n";
                        break;
                    case GL_LUMINANCE_ALPHA:
                    case GL_RGBA:
                        s << "    p *= s;\n";
                        break;
                    default:
                        s << "    //ERROR: Unsupported tex fmt\n";
                        break;
                }
                break;
            case RFF::TEM_Blend:
                switch( fmt ) {
                    case GL_ALPHA:
                        s << "    p.w *= s.w;\n";
                        break;
                    case GL_LUMINANCE:
                    case GL_RGB:
                        s << "    p.xyz = mix( p.xyz, rglTexEnvColor" << i << ".xyz, s.xyz );\n";
                        break;
                    case GL_LUMINANCE_ALPHA:
                    case GL_RGBA:
                        s << "    p.xyz = mix( p.xyz, rglTexEnvColor" << i << ".xyz, s.xyz );\n";
                        s << "    p.w *= s.w;\n";
                        break;
                    default:
                        s << "    //ERROR: Unsupported tex fmt\n"; break;
                        break;
                }
                break;
            case RFF::TEM_Add:
                switch( fmt ) {
                    case GL_ALPHA:
                        s << "    p.w *= s.w;\n";
                        break;
                    case GL_LUMINANCE:
                    case GL_RGB:
                        s << "    p.xyz += s.xyz;\n";
                        break;
                    case GL_LUMINANCE_ALPHA:
                    case GL_RGBA:
                        s << "    p.xyz += s.xyz;\n";
                        s << "    p.w *= s.w;\n";
                        break;
                    default:
                        s << "    //ERROR: Unsupported tex fmt\n";
                        break;
                }
                break;
            case RFF::TEM_Decal:
                switch( fmt ) {
                    case GL_ALPHA:
                    case GL_LUMINANCE:
                    case GL_LUMINANCE_ALPHA:
                        s << "    //ERROR: tex env mode undefined for this texture format\n";
                        break;
                    case GL_RGB:
                        s << "    p.xyz = s.xyz;\n";
                        break;
                    case GL_RGBA:
                        s << "    p.xyz = mix( p.xyz, s.xyz, s.w );\n";
                        break;
                    default:
                        s << "    //ERROR: Unsupported tex fmt\n";
                        break;
                }
                break;
            default:
                s << "    //ERROR: Unsupported tex env mode\n";
                break;
        }
    }

    void AddTexEnvCombine( RFF::TextureEnv & env, string_list & s )
    {
        bool skipAlpha = env.rgb.mode == RFF::TEC_Dot3Rgba;
        int rgbSources = 0;
        int aSources = 0;
        s << "    {\n";
        switch( env.rgb.mode ) {
            case RFF::TEC_Replace:
                rgbSources = 1;
                break;
            case RFF::TEC_Modulate:
            case RFF::TEC_Add:
            case RFF::TEC_AddSigned:
            case RFF::TEC_Dot3Rgb:
            case RFF::TEC_Dot3Rgba:
                rgbSources = 2;
                break;
            case RFF::TEC_Interpolate:
                rgbSources = 3;
                break;
            default:
                break;
        }
        if( skipAlpha == false ) {
            switch( env.a.mode ) {
                case RFF::TEC_Replace:
                    aSources = 1;
                    break;
                case RFF::TEC_Modulate:
                case RFF::TEC_Add:
                case RFF::TEC_AddSigned:
                case RFF::TEC_Subtract:
                case RFF::TEC_Dot3Rgb:
                case RFF::TEC_Dot3Rgba:
                    aSources = 2;
                    break;
                case RFF::TEC_Interpolate:
                    aSources = 3;
                    break;
                default:
                    break;
            }
        }
        for( int i = 0; i < rgbSources; i++ ) {
            bool skip = false;
            string source;
            switch( env.rgb.src[i] ) {
                case RFF::TCS_PrimaryColor:
                    source = "rglFrontColor";
                    break;
                case RFF::TCS_Constant:
                    source = "rglConstantColor";
                    break;
                case RFF::TCS_Previous:
                    source = "p";
                    break;
                case RFF::TCS_Texture:
                    source = "s";
                    break;
                default:
                    skip = true;
                    break;
            }
            string suffix;
            switch( env.rgb.op[i] ) {
                case RFF::TCO_Alpha:
                case RFF::TCO_OneMinusAlpha:
                    suffix = ".www";
                    break;
                case RFF::TCO_Color:
                case RFF::TCO_OneMinusColor:
                    suffix = ".xyz";
                    break;
                default:
                    skip = true;
                    break;
            }
            if ( skip ) {
                s << "        vec3 csrc" << i << " = vec3(0.0f, 0.0f, 0.0f);\n";
            } else {
                s << "        vec3 csrc" << i << " = ";
                if( env.rgb.op[i] == RFF::TCO_OneMinusColor ||
                    env.rgb.op[i] == RFF::TCO_OneMinusAlpha ) {
                    s << "1 - ";
                }
                s << source << suffix << ";\n";
            }
        }
        switch( env.rgb.mode ) {
            case RFF::TEC_Replace:
                s << "        p.xyz = csrc0;\n";
                break;
            case RFF::TEC_Modulate:
                s << "        p.xyz = csrc0 * csrc1;\n";
                break;
            case RFF::TEC_Add:
                s << "        p.xyz = csrc0 + csrc1;\n";
                break;
            case RFF::TEC_AddSigned:
                s << "        p.xyz = csrc0 + csrc1 - 0.5;\n";
                break;
            case RFF::TEC_Subtract:
                s << "        p.xyz = csrc0 - csrc1;\n";
                break;
            case RFF::TEC_Dot3Rgb:
            case RFF::TEC_Dot3Rgba:
                s << "        p.xyz = dot( ( 2.0 * csrc0 - 1.0 ), ( 2.0 * csrc1 - 1.0 ) );\n";
                break;
            case RFF::TEC_Interpolate:
                s << "        p.xyz = mix( csrc0, csrc1, csrc2);\n";
                break;
            default:
                s << "//ERROR: Unsupported tex env combine rgb mode\n"; break;
                break;
        }
        if( skipAlpha ) {
            s << "        p.w = p.x;\n";
        } else {
            for( int i = 0; i < aSources; i++ ) {
                bool skip = false;
                string source;
                switch( env.a.src[i] ) {
                    case RFF::TCS_PrimaryColor:
                        source = "rglFrontColor";
                        break;
                    case RFF::TCS_Constant:
                            source = "rglConstantColor";
                        break;
                    case RFF::TCS_Previous:
                            source = "p";
                        break;
                    case RFF::TCS_Texture:
                             source = "s";
                        break;
                    default:
                        skip = true;
                        break;
                }
                if ( skip ) {
                    s << "        float asrc" << i << " = 0.0f;\n";
                } else {
                    s << "        float asrc" << i << " = ";
                    if( env.a.op[i] == RFF::TCO_OneMinusAlpha ) {
                        s << "1 - ";
                    }
                    s << source << ".w;\n";
                }
            }
            switch( env.a.mode ) {
                case RFF::TEC_Replace:
                    s << "        p.w = asrc0;\n";
                    break;
                case RFF::TEC_Modulate:
                    s << "        p.w = asrc0 * asrc1;\n";
                    break;
                case RFF::TEC_Add:
                    s << "        p.w = asrc0 + asrc1;\n";
                    break;
                case RFF::TEC_AddSigned:
                    s << "        p.w = asrc0 + asrc1 - 0.5;\n";
                    break;
                case RFF::TEC_Subtract:
                    s << "        p.w = asrc0 - asrc1;\n";
                    break;
                case RFF::TEC_Interpolate:
                    s << "        p.w = mix( asrc0, asrc1, asrc2 );\n";
                    break;
                default:
                    s << "//ERROR: Unsupported tex env combine alpha mode\n"; break;
                    break;
            }
        }
        s << "    }\n";
    }

#if 0
    string TexEnvModeName( GLenum mode ) {
        switch( mode ) {
            case GL_MODULATE:    return "Modulate";
            case GL_ADD:         return "Add";
            case GL_REPLACE:     return "Replace";
            case GL_COMBINE:     return "Combine";
            default: break;
        }
        return "";
    }
    string TexFormatName( GLenum format ) {
        switch( format ) {
            case GL_ALPHA:           return "A";
            case GL_LUMINANCE:       return "L";
            case GL_LUMINANCE_ALPHA: return "LA";
            case GL_RGB:             return "RGB";
            case GL_RGBA:            return "RGBA";
            default: break;
        }
        return "";
    }
    string TexEnvFuncName( GLenum mode, GLenum format ) {
        string s;
        s += "TexEnv";
        s += TexEnvModeName( mode );
        s += TexFormatName( format );
        return s;
    }
#endif

    string TargetSuffix( GLubyte ttb ) {
        switch( ttb ) {
            case RFF::TT_1D: return "1D";
            case RFF::TT_2D: return "2D";
            case RFF::TT_Rect: return "Rect";
            case RFF::TT_3D: return "3D";
            case RFF::TT_CubeMap: return "Cube";
            default: break;
        }
        return "";
    }

    string TextureFetch( bool es, bool legacy, RFF::TextureTargetBitfield b ) {
        if( es || legacy ) {
            switch( b ) {
                case RFF::TT_1D:      return "texture1D";
                case RFF::TT_2D:      return "texture2D";
                case RFF::TT_CubeMap: return "textureCube";
                default: break;
            }
        }
        return "texture";
    }
    string TextureFetchSwizzle( bool es, bool legacy, RFF::TextureTargetBitfield b ) {
        if( es || legacy ) {
            switch( b ) {
                case RFF::TT_1D:      return ".x";
                case RFF::TT_2D:      return ".xy";
                case RFF::TT_Rect:    return ".xy";
                case RFF::TT_3D:      return ".xyz";
                case RFF::TT_CubeMap: return ".xyz";
                default: break;
            }
            return "";
        }
        switch( b ) {
            case RFF::TT_1D:      return ".x";
            case RFF::TT_2D:      return ".xy";
            case RFF::TT_Rect:    return ".xy";
            case RFF::TT_3D:      return ".xyz";
            case RFF::TT_CubeMap: return ".xyz";
            default: break;
        }
        return "";
    }

    void GenerateFragmentShaderSource( bool es, bool legacy, std::map<GLuint, GLint> obj2fmt, const RFF::State & state, string_list & src ) {

        const Store & st = state.processed;
        if( es ) {
            src << "#version 100\n";
        } else if( legacy ) {
            src << "#version 120\n";
        } else {
            src << "#version 140\n";
        }
        src << "// program number " << progcount << "\n";
        if( es || legacy ) {
            src << "#define in varying\n";
            src << "#define rglFragColor gl_FragColor\n";
        } else {
            src << "out vec4 rglFragColor;\n";
        }
        if( st.shadeModelFlat & ! legacy & ! es ) {
            src << "#define FLAT flat\n";
        } else {
            src << "#define FLAT  \n";
        }
        if( es ) {
            src << "precision highp float;\n";
        }
        src << "FLAT in vec4 rglFrontColor;\n";
        if( st.lighting ) {
            if( st.lightModelTwoSide ) {
                src << "FLAT in vec4 rglBackColor;\n";
            }
            if( st.lightModelSeparateSpecular ) {
                src << "in vec4 rglSCOL0;\n";
                if( st.lightModelTwoSide ) {
                    src << "in vec4 rglSCOL1;\n";
                }
            }
        } else if( st.colorSum ) {
            src << "in vec4 rglSCOL0;\n";
        }
        if( st.fog.enable ) {
            src << "uniform vec4 rglFog[2];\n";
            src << "in vec4 rglFOG;\n";
        }
        bool needsConstantColor = false;
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            Texture t = state.processed.tex[i];
            if( t.enables == 0 ) {
                continue;
            }
            src << "uniform sampler" << TargetSuffix( t.enables ) << " rglSampler" << i << ";\n";
            src << "in vec4 rglTEXCOORD" << i << ";\n";
            RFF::TextureEnv & env = t.unit.env;
            if( env.mode == RFF::TEM_Combine ) {
                for( int i = 0; i < 3; i++ ) {
                    needsConstantColor = needsConstantColor || ( env.rgb.src[i] == RFF::TCS_Constant );
                }
            }
            if( env.mode == RFF::TEM_Blend ) {
                src << "uniform vec4 rglTexEnvColor" << i << ";\n";
            }
        }
        if( needsConstantColor ) {
            src << "uniform vec4 rglConstantColor;\n";
        }
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
            if( ( es || legacy ) && st.clip[i].enable ) {
                src << "in float rglClipDistance" << i << ";\n";
            }
        }
        if( st.alphaTest.enable ) {
            if (st.alphaTest.comp != RFF::CF_Never && st.alphaTest.comp != RFF::CF_Always ) {
                src << "uniform float rglAlphaRef;\n";
            }
        }
        src << "void main() {\n";

        if( es || legacy ) {
            for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES; i++ ) {
                if( st.clip[i].enable ) {
                    src << "    if( rglClipDistance" << i << " < 0.0 ) discard;\n";
                }
            }
        }
        if( st.lighting && st.lightModelTwoSide ) {
            src << "    vec4 p = gl_FrontFacing ? rglFrontColor : rglBackColor;\n";
        } else {
            src << "    vec4 p = rglFrontColor;\n";
        }
        bool s_declared = false;
        for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
            Texture t = state.processed.tex[ i ];
            RFF::TextureTargetBitfield b = state.GetTextureEnable( i );
            if( b == 0 ) {
                continue;
            }
            if ( !s_declared ) {
                src << "    vec4 s;\n";
                s_declared = true;
            }
            switch( b ) {
                case RFF::TT_1D:
                case RFF::TT_2D:
                case RFF::TT_CubeMap:
                {
                    src << "    s = " << TextureFetch( es, legacy, b ) << "( rglSampler" << i << ", rglTEXCOORD" << i << TextureFetchSwizzle( es, legacy, b ) << " );\n";
                    break;
                }
                default:
                    src << "// ERROR: Unsupported texture target\n";
                    src << "    s = vec4( 0.0f, 0.0f, 0.0f, 0.0f );\n";
                    break;
            }
            switch( t.unit.env.mode ) {
                case RFF::TEM_Replace:
                case RFF::TEM_Modulate:
                case RFF::TEM_Add:
                case RFF::TEM_Decal:
                case RFF::TEM_Blend:
                    AddTexEnv( i, t.unit.env.mode, obj2fmt[t.unit.obj], src );
                    break;
                case RFF::TEM_Combine:
                    AddTexEnvCombine( t.unit.env, src );
                    break;
                default:
                    src << "//ERROR: Unsupported texture env mode\n";
                    break;
            }
        }
        src << "    rglFragColor = p;\n";
        if( st.lighting ) {
            if( st.lightModelSeparateSpecular ) {
                if( st.lightModelTwoSide ) {
                    src << "    rglFragColor += gl_FrontFacing ? rglSCOL0 : rglSCOL1;\n";
                } else {
                    src << "    rglFragColor += rglSCOL0;\n";
                }
            }
        } else if( st.colorSum ) {
            src << "    rglFragColor.xyz += rglSCOL0.xyz;\n";
        }
        if( st.fog.enable ) {
            src << "    float f = abs( -rglFOG.z / rglFOG.w );\n";
            switch( st.fog.mode ) {
                case RFF::FG_Linear:
                    src << "    float fogFactor = ( rglFog[0].z - f ) / ( rglFog[0].z - rglFog[0].y );\n";
                    break;
                case RFF::FG_Exp:
                    src << "    float fogFactor = exp( -( rglFog[0].x * f ) );\n";
                    break;
                case RFF::FG_Exp2:
                    src << "    float fogFactor = exp( -( rglFog[0].x * rglFog[0].x * f * f ) );\n";
                    break;
                default:
                    src << "//ERROR: Unsupported fog mode\n";
                    src << "    float fogFactor = 0.0f;\n";
                    break;
            }
            src << "    fogFactor = clamp( fogFactor, 0.0, 1.0 );\n";
            src << "    rglFragColor.xyz = mix( rglFog[1].xyz, rglFragColor.xyz, fogFactor );\n";
        }
        if( st.alphaTest.enable ) {
            switch( st.alphaTest.comp ) {
                case RFF::CF_Never:    src << "    discard;\n"; break;
                case RFF::CF_Less:     src << "    if( rglFragColor.w >= rglAlphaRef ) discard;\n"; break;
                case RFF::CF_Greater:  src << "    if( rglFragColor.w <= rglAlphaRef ) discard;\n"; break;
                case RFF::CF_Lequal:   src << "    if( rglFragColor.w > rglAlphaRef ) discard;\n"; break;
                case RFF::CF_Gequal:   src << "    if( rglFragColor.w < rglAlphaRef ) discard;\n"; break;
                case RFF::CF_Equal:    src << "    if( rglFragColor.w != rglAlphaRef ) discard;\n"; break;
                case RFF::CF_NotEqual: src << "    if( rglFragColor.w == rglAlphaRef ) discard;\n"; break;
                case RFF::CF_Always: break;
                default: src << "//ERROR: Unsupported alpha comp func\n"; break;
            }
        }

        src << "}\n";
    }

    void Copy( Float4 & dst, const GLfloat * src ) {
        dst.x = src[0]; dst.y = src[1]; dst.z = src[2]; dst.w = src[3];
    }

    void Copy( GLfloat * dst, Float4 & src ) {
        dst[0] = src.x; dst[1] = src.y; dst[2] = src.z; dst[3] = src.w;
    }

    void Transform( Float4 & dst, const r3::Matrix4f & m, const GLfloat * src ) {
        r3::Vec4f v( src );
        m.MultMatrixVec( v );
        dst.x = v.x; dst.y = v.y; dst.z = v.z; dst.w = v.w;
    }

// Not currently used
#if 0
    void TransformDir( Float4 & dst, const r3::Matrix4f & m, const GLfloat * src ) {
        r3::Vec3f v( src );
        m.MultMatrixDir( v );
        v.Normalize();
        dst.x = v.x; dst.y = v.y; dst.z = v.z;
    }
#endif

    r3::Matrix4f RescaleNormal( const r3::Matrix4f & m ) {
        r3::Matrix4f r = m;
        for( int i = 0; i < 3; i++ ) {
            r3::Vec3f v( r( i, 0 ), r( i, 1 ), r( i, 2 ) );
            v.Normalize();
            r( i, 0 ) = v.x; r( i, 1 ) = v.y; r( i, 2 ) = v.z;
        }
        return r;
    }

    // debug
    GLchar dbgLog[1<<15];
    GLsizei dbgLogLen;
}

bool State::SetEnable( RFF * ffn, bool enable, GLenum cap ) {
    RFF::Version & ver = ffn->ver;
    int activeTex = ffn->activeTextureIndex;
    int shift;
    switch( cap ) {
        case GL_TEXTURE_1D: shift = TP_1D; break;
        case GL_TEXTURE_2D: shift = TP_2D; break;
        case GL_TEXTURE_RECTANGLE: shift = TP_Rect; break;
        case GL_TEXTURE_3D: shift = TP_3D; break;
        case GL_TEXTURE_CUBE_MAP: shift = TP_CubeMap; break;
        case GL_COLOR_SUM: raw.ver = ver.Update(); raw.colorSum = enable; return true;
        case GL_FOG: raw.ver = ver.Update(); raw.fog.enable = enable; return true;
        case GL_LIGHTING: raw.ver = ver.Update(); raw.lighting = enable; return true;
        case GL_LIGHT0:
        case GL_LIGHT1:
        case GL_LIGHT2:
        case GL_LIGHT3:
        case GL_LIGHT4:
        case GL_LIGHT5:
        case GL_LIGHT6:
        case GL_LIGHT7: raw.ver = ver.Update(); raw.light[ cap - GL_LIGHT0 ].enable = enable; return true;
        case GL_COLOR_MATERIAL: raw.ver = ver.Update(); raw.colorMaterial = enable; return true;
        case GL_RESCALE_NORMAL: raw.ver = ver.Update(); raw.rescaleNormal = enable; return true;
        case GL_NORMALIZE: raw.ver = ver.Update(); raw.normalize = enable; return true;
        case GL_TEXTURE_GEN_S:
        case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_GEN_R:
        case GL_TEXTURE_GEN_Q: {
            raw.ver = ver.Update();
            int idx = cap - GL_TEXTURE_GEN_S;
            raw.tex[ ffn->activeTextureIndex ].texgen[ idx ].enable = enable;
            return true;
        }
        case GL_CLIP_PLANE0:
        case GL_CLIP_PLANE1:
        case GL_CLIP_PLANE2:
        case GL_CLIP_PLANE3:
        case GL_CLIP_PLANE4:
        case GL_CLIP_PLANE5:
        case GL_CLIP_PLANE0+6:
        case GL_CLIP_PLANE0+7:
            raw.ver = ver.Update();
            raw.clip[ cap - GL_CLIP_PLANE0 ].enable = enable;
            return false;
        case GL_ALPHA_TEST: raw.ver = ver.Update(); raw.alphaTest.enable = enable; return true;
        default:
            return false;
    }
    if( activeTex >= REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS ) {
        return true;
    }
    Texture & t = raw.tex[ activeTex ];
    GLuint v = 1 << shift;
    if( enable ) {
       t.enables |= v;
    } else {
       t.enables &= ~v;
    }
    //Logging::Output( "%d %d\n", activeTex, t.enables );
    raw.ver = ver.Update();
    return true;
}

void State::SetLight( RFF * ffn, GLenum light, GLenum pname, const GLfloat * params ) {
    RFF::Version & ver = ffn->ver;
    int idx = light - GL_LIGHT0;
    if( idx < 0 || idx >= REGAL_FIXED_FUNCTION_MAX_LIGHTS ) {
        return;
    }
    State::Light & l = raw.light[ idx ];
    switch( pname ) {
        case GL_AMBIENT: Copy( l.ambient, params ); break;
        case GL_DIFFUSE: Copy( l.diffuse, params ); break;
        case GL_SPECULAR: Copy( l.specular, params ); break;
        case GL_POSITION:
            {
                Transform( l.position, ffn->modelview.Top(), params );
                if (l.position.w == 0.0) {
                  l.position.w = 1.0f/sqrtf(l.position.x * l.position.x +
                                            l.position.y * l.position.y +
                                            l.position.z * l.position.z);
                  l.position.x *= l.position.w;
                  l.position.y *= l.position.w;
                  l.position.z *= l.position.w;
                  l.position.w = 0.0;
                } else {
                  l.position.x /= l.position.w;
                  l.position.y /= l.position.w;
                  l.position.z /= l.position.w;
                  l.position.w = 1.0;
                }
                break;
            }
        case GL_SPOT_DIRECTION:
            {
                Float4 spdir( params[0], params[1], params[2], 1.0f );
                r3::Matrix4f & m = ffn->modelview.Top();
                float x = m.element(0,0) * spdir.x + m.element(0,1) * spdir.y + m.element(0,2) * spdir.z;
                float y = m.element(1,0) * spdir.x + m.element(1,1) * spdir.y + m.element(1,2) * spdir.z;
                float z = m.element(2,0) * spdir.x + m.element(2,1) * spdir.y + m.element(2,2) * spdir.z;
                float d = -1.0f/sqrtf(x*x + y*y + z*z);
                l.spotDirection.x = x * d;
                l.spotDirection.y = y * d;
                l.spotDirection.z = z * d;
            }
            break;

        case GL_SPOT_EXPONENT: l.attenuation.w = params[0]; break;
        case GL_SPOT_CUTOFF: l.spotDirection.w = params[0]; break;
        case GL_CONSTANT_ATTENUATION: l.attenuation.x = params[0]; break;
        case GL_LINEAR_ATTENUATION: l.attenuation.y = params[0]; break;
        case GL_QUADRATIC_ATTENUATION: l.attenuation.z = params[0]; break;
        default: return;
    }
    l.ver = raw.ver = ver.Update();
}

void State::SetMaterial( RFF * ffn, GLenum face, GLenum pname, const GLfloat * params ) {
    RFF::Version & ver = ffn->ver;
    for( int i = 0; i < 2; i++ ) {
        if( ( i == 0 && face == GL_BACK ) || ( i == 1 && face == GL_FRONT ) ) {
            continue;
        }
        State::Material & m = raw.mat[ i ];
        switch( pname ) {
            case GL_AMBIENT: Copy( m.ambient, params ); break;
            case GL_DIFFUSE: Copy( m.diffuse, params ); break;
            case GL_AMBIENT_AND_DIFFUSE: Copy( m.ambient, params ); m.diffuse = m.ambient; break;
            case GL_SPECULAR: Copy( m.specular, params ); break;
            case GL_EMISSION: Copy( m.emission, params ); break;
            case GL_SHININESS: m.shininess.x = params[0]; break;
            default: return;
        }
        m.ver = raw.ver = ver.Update();
    }
}

void State::GetMaterial( RFF * ffn, GLenum face, GLenum pname, GLfloat * params )
{
    UNUSED_PARAMETER(ffn);

    for( int i = 0; i < 2; i++ ) {
        if( ( i == 0 && face == GL_BACK ) || ( i == 1 && face == GL_FRONT ) ) {
            continue;
        }
        State::Material & m = raw.mat[ i ];
        switch( pname ) {
            case GL_AMBIENT: Copy( params, m.ambient ); break;
            case GL_DIFFUSE: Copy( params, m.diffuse ); break;
            case GL_SPECULAR: Copy( params, m.specular ); break;
            case GL_EMISSION: Copy( params, m.emission ); break;
            case GL_SHININESS: params[0] = m.shininess.x; break;
            default: return;
        }
    }
}

void State::SetTexgen( RegalIff * ffn, int coord, GLenum space, const GLfloat * params )
{
    ITrace("State::SetTexgen ",ffn,coord,toString(space),boost::print::array(params,4));

    r3::Matrix4f ident;
    Texgen & tg = raw.tex[ ffn->activeTextureIndex ].texgen[ coord ];
    GLuint64 *tgver = NULL;
    switch( space ) {
        case GL_OBJECT_PLANE: Transform( tg.obj, ident, params ); tgver = & tg.objVer; break;
        case GL_EYE_PLANE: Transform( tg.eye, ffn->modelview.Top().Inverse().Transpose(), params ); tgver = & tg.eyeVer; break;
        default: return;
    }
    *tgver = raw.ver = ffn->ver.Update();
}

void State::GetTexgen( RegalIff * ffn, int coord, GLenum space, GLfloat * params )
{
    ITrace("State::GetTexgen ",ffn,coord,toString(space));

    Texgen & tg = raw.tex[ ffn->activeTextureIndex ].texgen[ coord ];
    switch( space ) {
        case GL_OBJECT_PLANE:
            params[0] = tg.obj.x;
            params[1] = tg.obj.y;
            params[2] = tg.obj.z;
            params[3] = tg.obj.w;
            break;
        case GL_EYE_PLANE:
            params[0] = tg.eye.x;
            params[1] = tg.eye.y;
            params[2] = tg.eye.z;
            params[3] = tg.eye.w;
            break;
        default:
            break;
    }
}

void State::SetAlphaFunc( RegalIff * ffn, RegalIff::CompareFunc comp, GLfloat alphaRef ) {
    raw.alphaTest.comp = comp;
    raw.alphaTest.alphaRef = std::min( 1.0f, std::max( 0.0f, alphaRef ) );
    raw.alphaTest.ver = raw.ver = ffn->ver.Update();
}

void State::SetClip( RegalIff * ffn, GLenum plane, const GLfloat * equation ) {
    int idx = plane - GL_CLIP_PLANE0;
    if( idx >= REGAL_FIXED_FUNCTION_MAX_CLIP_PLANES ) {
        return;
    }
    Transform( raw.clip[ idx ].plane, ffn->modelview.Top().Inverse().Transpose(), equation );
    raw.clip[ idx ].ver = raw.ver = ffn->ver.Update();
}

void Program::Init( RegalContext * ctx, const Store & sstore, const GLchar *vsSrc, const GLchar *fsSrc ) {
    ver = 0;
    progcount = 0;
    DispatchTable & tbl = ctx->dsp->emuTbl;
    store = sstore;
    pg = tbl.glCreateProgram();
    Shader( tbl, GL_VERTEX_SHADER, vs, vsSrc );
    Shader( tbl, GL_FRAGMENT_SHADER, fs, fsSrc );
    Attribs( ctx );
    tbl.glLinkProgram( pg );
    tbl.glGetProgramInfoLog( pg, (1<<15) - 2, &dbgLogLen, dbgLog );
    dbgLog[ dbgLogLen ] = 0;
    if( dbgLogLen > 0 ) {
        ITrace( dbgLog );
    }

    tbl.glUseProgram( pg );
    Samplers( tbl );
    Uniforms( tbl );
    tbl.glUseProgram( ctx->iff->program );
}
void Program::Init( RegalContext * ctx, const Store & sstore ) {
    ver = 0;
    progcount = 0;
    DispatchTable & tbl = ctx->dsp->emuTbl;
    store = sstore;
    Attribs( ctx );
    ctx->dsp->emuTbl.glLinkProgram( pg );
    ctx->dsp->emuTbl.glUseProgram( pg );
    Samplers( tbl );
    Uniforms( tbl );
    ctx->dsp->emuTbl.glUseProgram( ctx->iff->program );
}

void Program::Shader( DispatchTable & tbl, GLenum type, GLuint & shader, const GLchar *src ) {
    const GLchar *srcs[] = { src };
    GLint len[] = { 0 };
    len[0] = (GLint)strlen( src );
    shader = tbl.glCreateShader( type );
    tbl.glShaderSource( shader, 1, srcs, len );
    tbl.glCompileShader( shader );
    tbl.glGetShaderInfoLog( shader, (1<<15) - 2, &dbgLogLen, dbgLog );
    dbgLog[ dbgLogLen ] = 0;
    if( dbgLogLen > 0 ) {
        ITrace( dbgLog );
    }
    tbl.glAttachShader( pg, shader );
}
void Program::Attribs( RegalContext * ctx ) {
    DispatchTable & tbl = ctx->dsp->emuTbl;

    tbl.glBindAttribLocation( pg, ctx->iff->ffAttrMap[ RFF2A_Vertex ], "rglVertex" );
    //tbl.glBindAttribLocation( pg, 1, "rglWeight" );
    if( store.lighting ) {
        tbl.glBindAttribLocation( pg, ctx->iff->ffAttrMap[ RFF2A_Normal ], "rglNormal" );
    }
    if( store.attrArrayFlags & ( 1 << ctx->iff->ffAttrMap[ RFF2A_Color ] ) && ( store.lighting == false || store.colorMaterial ) ) {
        tbl.glBindAttribLocation( pg, ctx->iff->ffAttrMap[ RFF2A_Color ], "rglColor" );
    }
    if( store.colorSum  && store.lighting == false ) {
        tbl.glBindAttribLocation( pg, ctx->iff->ffAttrMap[ RFF2A_SecondaryColor ], "rglSecondaryColor" );
    }
    if( store.fog.enable && store.fog.useDepth == false ) {
        tbl.glBindAttribLocation( pg, ctx->iff->ffAttrMap[ RFF2A_FogCoord ], "rglFogCoord" );
    }
    GLuint units = std::min( (GLuint)ctx->iff->ffAttrNumTex, (GLuint)REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS );
    for( GLuint i = 0; i < units; i++ ) {
        #ifndef REGAL_HACK_SET_001
        if( store.tex[i].enables == 0 ) {
            continue;
        }
        #endif
        string_list ss;
        ss << "rglMultiTexCoord" << i;
        tbl.glBindAttribLocation( pg, ctx->iff->ffAttrTexBegin + i, ss.str().c_str() );
    }
}
void Program::Samplers( DispatchTable & tbl ) {
    GLchar samp[64];
    strcpy( samp, "rglSamplerN" );
    int len = (int)strlen( samp );
    for( GLchar i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
        samp[ len - 1 ] = '0' + i;
        GLint slot = tbl.glGetUniformLocation( pg, samp );
        if( slot >= 0 ) {
            tbl.glUniform1i( slot, i );
        }
    }
}

void Program::Uniforms( DispatchTable & tbl ) {
    for( size_t i = 1; i < sizeof(regalFFUniformInfo)/sizeof(regalFFUniformInfo[0]); i++ ) {
        const RegalFFUniformInfo & ri = regalFFUniformInfo[i];
        GLint slot = tbl.glGetUniformLocation( pg, ri.name );
        if( slot > -1 ) {
            UniformInfo ui;
            ui.slot = slot;
            ui.ver = 0;
            ui.ver = ~ui.ver;
            uniforms[ ri.val ] = ui;
        }
    }
}

void RFF::InitFixedFunction( RegalContext * ctx ) {

#if RGL_SYS_ANDROID || RGL_SYS_IOS
    es = true;
#else
    es = GetProcAddress( "glClearDepth" ) == NULL;
#endif
    const char* v = (const char *)ctx->dsp->emuTbl.glGetString( GL_VERSION );
    // FIXME: Handle detecting legacy more generally...
    legacy = !v || (v[0] == '2' && v[1] == '.');
    //legacy = v.find( "2.1 " ) != string::npos;

    shadowMatrixMode = GL_MODELVIEW;
    currMatrixStack = &modelview;
    program = 0;
    activeTextureIndex = 0;
    shadowActiveTextureIndex = 0;

    currprog = NULL;
    for( int i = 0; i < REGAL_EMU_MAX_TEXTURE_UNITS; i++ ) {
        textureUnit[ i ] = TextureUnit();
    }

    currVao = 0;
    vaoAttrMap[0] = 0;

    fmtmap[ 1 ] = GL_LUMINANCE;
    fmtmap[ 2 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ 3 ] = GL_RGB;
    fmtmap[ 4 ] = GL_RGBA;
    fmtmap[ GL_ALPHA ] = GL_ALPHA;
    fmtmap[ GL_ALPHA4 ] = GL_ALPHA;
    fmtmap[ GL_ALPHA8 ] = GL_ALPHA;
    fmtmap[ GL_ALPHA12 ] = GL_ALPHA;
    fmtmap[ GL_ALPHA16 ] = GL_ALPHA;
    fmtmap[ GL_LUMINANCE ] = GL_LUMINANCE;
    fmtmap[ GL_LUMINANCE4 ] = GL_LUMINANCE;
    fmtmap[ GL_LUMINANCE8 ] = GL_LUMINANCE;
    fmtmap[ GL_LUMINANCE12 ] = GL_LUMINANCE;
    fmtmap[ GL_LUMINANCE16 ] = GL_LUMINANCE;
    fmtmap[ GL_LUMINANCE_ALPHA ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE4_ALPHA4 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE6_ALPHA2 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE8_ALPHA8 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE12_ALPHA4 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE12_ALPHA12 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_LUMINANCE16_ALPHA16 ] = GL_LUMINANCE_ALPHA;
    fmtmap[ GL_INTENSITY ] = GL_INTENSITY;
    fmtmap[ GL_INTENSITY4 ] = GL_INTENSITY;
    fmtmap[ GL_INTENSITY8 ] = GL_INTENSITY;
    fmtmap[ GL_INTENSITY12 ] = GL_INTENSITY;
    fmtmap[ GL_INTENSITY12 ] = GL_INTENSITY;
    fmtmap[ GL_INTENSITY16 ] = GL_INTENSITY;
    fmtmap[ GL_RGB ] = GL_RGB;
    fmtmap[ GL_RGB4 ] = GL_RGB;
    fmtmap[ GL_RGB5 ] = GL_RGB;
    fmtmap[ GL_RGB8 ] = GL_RGB;
    fmtmap[ GL_RGB10 ] = GL_RGB;
    fmtmap[ GL_RGB12 ] = GL_RGB;
    fmtmap[ GL_RGB16 ] = GL_RGB;
    fmtmap[ GL_RGBA ] = GL_RGBA;
    fmtmap[ GL_RGBA4 ] = GL_RGBA;
    fmtmap[ GL_RGB5_A1 ] = GL_RGBA;
    fmtmap[ GL_RGBA8 ] = GL_RGBA;
    fmtmap[ GL_RGB10_A2 ] = GL_RGBA;
    fmtmap[ GL_RGBA12 ] = GL_RGBA;
    fmtmap[ GL_RGBA16 ] = GL_RGBA;

	fmtmap[ GL_RGBA16F ] = GL_RGBA16F;
	fmtmap[ GL_SRGB8_ALPHA8 ] = GL_SRGB8_ALPHA8;

  fmtmap[ GL_RGB16F_ARB ]       = GL_RGB;
  fmtmap[ GL_RGBA32F_ARB ]      = GL_RGB;
  fmtmap[ GL_INTENSITY16F_ARB ] = GL_INTENSITY;
}

void RFF::ShadowMultiTexBinding( GLenum texunit, GLenum target, GLuint obj ) {
    activeTextureIndex = texunit - GL_TEXTURE0;
    if( activeTextureIndex > ( REGAL_EMU_MAX_TEXTURE_UNITS - 1 ) ) {
        return;
    }
    // assert( textureInfo.count( obj ) == 0 || target == textureInfo[activeTextureIndex].target );
    GLint fmt = textureObjToFmt[ obj ];
    TextureUnit & tu = textureUnit[ activeTextureIndex ];
    tu.obj = obj;
    tu.fmt = fmt;
    tu.ttb = static_cast<GLubyte>(TargetToBitfield( target ));
    tu.ver = ffstate.raw.ver = ver.Update();
}

void RFF::ShadowTextureInfo( GLuint obj, GLenum target, GLint internalFormat )
{
    UNUSED_PARAMETER(target);
    // assert( target == tip->tgt );
    if( fmtmap.count( internalFormat ) == 0 ) {
        Warning( "Unknown internal format: ", internalFormat );
    }
    GLint fmt = fmtmap[ internalFormat ];
    textureObjToFmt[ obj ] = fmt;
    ffstate.raw.ver = ver.Update();
}

void RFF::ShadowMultiTexInfo( GLenum texunit, GLenum target, GLint internalFormat ) {
    activeTextureIndex = texunit - GL_TEXTURE0;
    ShadowTexInfo( target, internalFormat );
}

void RFF::ShadowTexInfo( GLenum target, GLint internalFormat ) {
    if( shadowActiveTextureIndex > ( REGAL_EMU_MAX_TEXTURE_UNITS - 1 ) ) {
        return;
    }
    ShadowTextureInfo( textureUnit[ shadowActiveTextureIndex ].obj, target, internalFormat );
}

void RFF::TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLfloat *v ) {
    activeTextureIndex = texunit - GL_TEXTURE0;
    switch( target ) {
        case GL_TEXTURE_ENV:
            switch( pname ) {
                case GL_TEXTURE_ENV_COLOR: {
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TextureEnv & env = tup->env;
                    Copy( env.color, v );
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    return;
                }
            }
    }
    GLint iv[4];
    iv[0] = static_cast<GLint>(v[0]);
    iv[1] = static_cast<GLint>(v[1]);
    iv[2] = static_cast<GLint>(v[2]);
    iv[3] = static_cast<GLint>(v[3]);
    TexEnv( texunit, target, pname, iv );
}

void RFF::TexEnv( GLenum texunit, GLenum target, GLenum pname, const GLint *v ) {
    activeTextureIndex = texunit - GL_TEXTURE0;
    switch( target ) {
        case GL_TEXTURE_ENV:
            switch( pname ) {
                case GL_TEXTURE_ENV_MODE: {
                    if( activeTextureIndex >= REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS ) {
                        return;
                    }
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TexenvMode & m = tup->env.mode;
                    switch( v[0] ) {
                        case GL_REPLACE:  m = TEM_Replace; break;
                        case GL_MODULATE: m = TEM_Modulate; break;
                        case GL_ADD:      m = TEM_Add; break;
                        case GL_DECAL:    m = TEM_Decal; break;
                        case GL_BLEND:    m = TEM_Blend; break;
                        case GL_COMBINE:  m = TEM_Combine; break;
                        default: return; // error?
                    }
                    // assert( target == tip->tgt );
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    break;
                }
                case GL_COMBINE_RGB: {
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TexenvCombine & m = tup->env.rgb.mode;
                    switch( v[0] ) {
                        case GL_REPLACE:     m = TEC_Replace; break;
                        case GL_MODULATE:    m = TEC_Modulate; break;
                        case GL_ADD:         m = TEC_Add; break;
                        case GL_ADD_SIGNED:  m = TEC_AddSigned; break;
                        case GL_SUBTRACT:    m = TEC_Subtract; break;
                        case GL_DOT3_RGB:    m = TEC_Dot3Rgb; break;
                        case GL_DOT3_RGBA:   m = TEC_Dot3Rgba; break;
                        case GL_INTERPOLATE: m = TEC_Interpolate; break;
                        default: return; // error?
                    }
                    // assert( target == tip->tgt );
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    break;
                }
                case GL_COMBINE_ALPHA: {
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TexenvCombine & m = tup->env.a.mode;
                    switch( v[0] ) {
                        case GL_REPLACE:     m = TEC_Replace; break;
                        case GL_MODULATE:    m = TEC_Modulate; break;
                        case GL_ADD:         m = TEC_Add; break;
                        case GL_ADD_SIGNED:  m = TEC_AddSigned; break;
                        case GL_SUBTRACT:    m = TEC_Subtract; break;
                        case GL_INTERPOLATE: m = TEC_Interpolate; break;
                        default: return; // error?
                    }
                    // assert( target == tip->tgt );
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    break;
                }
                case GL_SRC0_RGB:
                case GL_SRC1_RGB:
                case GL_SRC2_RGB:
                case GL_SRC0_ALPHA:
                case GL_SRC1_ALPHA:
                case GL_SRC2_ALPHA: {
                    int idx = pname - GL_SRC0_RGB;
                    bool isRgb = true;
                    if( idx > 3 ) {
                        isRgb = false;
                        idx = pname - GL_SRC0_ALPHA;
                    }
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TexenvCombineSrc & src = isRgb ? tup->env.rgb.src[idx] : tup->env.a.src[idx];
                    switch( v[0] ) {
                        case GL_PREVIOUS:      src = TCS_Previous; break;
                        case GL_CONSTANT:      src = TCS_Constant; break;
                        case GL_TEXTURE:       src = TCS_Texture; break;
                        case GL_PRIMARY_COLOR: src = TCS_PrimaryColor; break;
                        default: return; // error?
                    }
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    break;
                }
                case GL_OPERAND0_RGB:
                case GL_OPERAND1_RGB:
                case GL_OPERAND2_RGB:
                case GL_OPERAND0_ALPHA:
                case GL_OPERAND1_ALPHA:
                case GL_OPERAND2_ALPHA: {
                    int idx = pname - GL_OPERAND0_RGB;
                    bool isRgb = true;
                    if( idx > 3 ) {
                        isRgb = false;
                        idx = pname - GL_OPERAND0_ALPHA;
                    }
                    TextureUnit *tup = & textureUnit[ activeTextureIndex ];
                    TexenvCombineOp & op = isRgb ? tup->env.rgb.op[idx] : tup->env.a.op[idx];
                    switch( v[0] ) {
                        case GL_SRC_COLOR:           op = TCO_Color; break;
                        case GL_SRC_ALPHA:           op = TCO_Alpha; break;
                        case GL_ONE_MINUS_SRC_COLOR: op = TCO_OneMinusColor; break;
                        case GL_ONE_MINUS_SRC_ALPHA: op = TCO_OneMinusAlpha; break;
                        default: return; // error?
                    }
                    ffstate.SetTexInfo( ver, activeTextureIndex, *tup );
                    break;
                }
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void RFF::State::Process( RegalIff * ffn ) {
    const Store & r = raw;
    Store & p = processed;
    if( r.ver == p.ver ) {
        return;
    }
    p = r;
    r3::Matrix4f identity;
    if( p.fog.enable == false ) {
        p.fog.useDepth = true;
        p.fog.mode = FG_Exp;
    }

    // alpha testing is done at the precision of the color buffer so adjust
    // alpharef to an 8-bit range

    p.alphaTest.alphaRef = floor( r.alphaTest.alphaRef * 255.0f + 0.5f ) / 255.0f;

    for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
        TextureUnit & ti = ffn->textureUnit[ i ];
        raw.tex[i].unit = ti;
    }

    for( int i = 0; i < REGAL_FIXED_FUNCTION_MAX_TEXTURE_UNITS; i++ ) {
        Texture & pt = p.tex[i];
        const Texture & rt= r.tex[i];
        // for processed state, only the highest priority texture enable set (others are dont-care)
        //<> dsn:
        //<> but do we need to track and check against ttb?  If no texture was
        //<> explicitly bound to a texture target then the default texture (which
        //<> has the name 0) for that target will be used, right?
        //<> pt.enables = HighestPriorityTextureEnable( rt.enables & rt.unit.ttb );
        pt.enables = HighestPriorityTextureEnable( rt.enables );
        pt.unit = rt.unit;
        pt.useMatrix = pt.enables != 0 && ffn->texture[i].Top() != identity;
        if( pt.unit.env.mode != TEM_Combine ) {
            pt.unit.env.rgb = pt.unit.env.a = TexenvCombineState();
        }
    }
    if( p.lighting == false ) {
        p.rescaleNormal = false;
        p.normalize = false;
        for ( int i = 0; i <REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
            Light & l = p.light[i];
            l.enable = l.spotlight = l.attenuate = false;
        }
    } else {
        for ( int i = 0; i <REGAL_FIXED_FUNCTION_MAX_LIGHTS; i++ ) {
            Light & l = p.light[i];
            if( l.enable == true ) {
                l.attenuation = r.light[i].attenuation;
                l.spotDirection = r.light[i].spotDirection;
                l.position = r.light[i].position;
                l.spotlight = r.light[i].spotDirection.w != 180.0f;
                l.spotDirection.w = cos( r3::ToRadians( r.light[i].spotDirection.w ) );
                l.attenuate = l.attenuation.x != 1.0f || l.attenuation.y != 0.0f || l.attenuation.z != 0.0f;
            }
        }
    }
}

void RFF::UpdateUniforms( RegalContext * ctx ) {
    Program & pgm = *currprog;
    DispatchTable & tbl = ctx->dsp->emuTbl;
    if( pgm.ver != ffstate.Ver() ) {
        pgm.ver = ffstate.Ver();
        const State::Store & p = ffstate.processed;
        for( std::map< RegalFFUniformEnum, Program::UniformInfo>::iterator i = pgm.uniforms.begin(); i != pgm.uniforms.end(); ++i ) {
            Program::UniformInfo & ui = (*i).second;
            switch( (*i).first ) {
                case FFU_ModelviewInverseTranspose: {
                    if( ui.ver != modelview.Ver() ) {
                        ui.ver = modelview.Ver();
                        r3::Matrix4f mvi = modelview.Top().Inverse();
                        if( p.rescaleNormal ) {
                            mvi = RescaleNormal( mvi );
                        }
                        tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, mvi.Transpose().m );
                    }
                    break;
                }
                case FFU_ModelviewInverse: {
                    if( ui.ver != modelview.Ver() ) {
                        ui.ver = modelview.Ver();
                        r3::Matrix4f mvi = modelview.Top().Inverse();
                        if( p.rescaleNormal ) {
                            mvi = RescaleNormal( mvi );
                        }
                        tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, mvi.Ptr() );
                    }
                    break;
                }
                case FFU_Modelview: {
                    if( ui.ver != modelview.Ver() ) {
                        ui.ver = modelview.Ver();
                        tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, modelview.Top().m );
                    }
                    break;
                }
                case FFU_Projection: {
                    if( ui.ver != projection.Ver() ) {
                        ui.ver = projection.Ver();
                        tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, projection.Top().m );
                    }
                    break;
                }
                case FFU_TextureMatrix0:
                case FFU_TextureMatrix1:
                case FFU_TextureMatrix2:
                case FFU_TextureMatrix3:
                case FFU_TextureMatrix4:
                case FFU_TextureMatrix5:
                case FFU_TextureMatrix6:
                case FFU_TextureMatrix7:
                case FFU_TextureMatrix8:
                case FFU_TextureMatrix9:
                case FFU_TextureMatrix10:
                case FFU_TextureMatrix11:
                case FFU_TextureMatrix12:
                case FFU_TextureMatrix13:
                case FFU_TextureMatrix14:
                case FFU_TextureMatrix15: {
                    int idx = ( ((*i).first) - FFU_TextureMatrix0 );
                    if( ui.ver != texture[ idx ].Ver() ) {
                        ui.ver = texture[ idx ].Ver();
                        tbl.glUniformMatrix4fv( ui.slot, 1, GL_FALSE, texture[ idx ].Top().m );
                    }
                    break;
                }
                case FFU_TextureEnvColor0:
                case FFU_TextureEnvColor1:
                case FFU_TextureEnvColor2:
                case FFU_TextureEnvColor3:
                case FFU_TextureEnvColor4:
                case FFU_TextureEnvColor5:
                case FFU_TextureEnvColor6:
                case FFU_TextureEnvColor7:
                case FFU_TextureEnvColor8:
                case FFU_TextureEnvColor9:
                case FFU_TextureEnvColor10:
                case FFU_TextureEnvColor11:
                case FFU_TextureEnvColor12:
                case FFU_TextureEnvColor13:
                case FFU_TextureEnvColor14:
                case FFU_TextureEnvColor15: {
                    int idx = ( ((*i).first) - FFU_TextureEnvColor0 );
                    if( ui.ver != textureUnit[ idx ].ver ) {
                        ui.ver = textureUnit[ idx ].ver;
                        tbl.glUniform4fv( ui.slot, 1, &p.tex[ idx ].unit.env.color.x);
                    }
                    break;
                }
                case FFU_Light0:
                case FFU_Light1:
                case FFU_Light2:
                case FFU_Light3:
                case FFU_Light4:
                case FFU_Light5:
                case FFU_Light6:
                case FFU_Light7: {
                    int idx = ( ((*i).first) - FFU_Light0 );
                    if( ui.ver != p.light[ idx ].ver ) {
                        ui.ver = p.light[ idx ].ver;
                        tbl.glUniform4fv( ui.slot, LE_Elements, &p.light[ idx ].ambient.x);
                    }
                    break;
                }
                case FFU_MaterialFront:
                case FFU_MaterialBack: {
                    int idx = ( ((*i).first) - FFU_MaterialFront );
                    if( ui.ver != p.mat[ idx ].ver ) {
                        ui.ver = p.mat[ idx ].ver;
                        tbl.glUniform4fv( ui.slot, ME_Elements, &p.mat[ idx ].ambient.x);
                    }
                    break;
                }
                case FFU_LightModelAmbient: {
                    tbl.glUniform4fv( ui.slot, 1, &p.lightModelAmbient.x);
                    break;
                }
                case FFU_Texgen0ObjS: case FFU_Texgen0ObjT: case FFU_Texgen0ObjR: case FFU_Texgen0ObjQ:
                case FFU_Texgen0EyeS: case FFU_Texgen0EyeT: case FFU_Texgen0EyeR: case FFU_Texgen0EyeQ:
                case FFU_Texgen1ObjS: case FFU_Texgen1ObjT: case FFU_Texgen1ObjR: case FFU_Texgen1ObjQ:
                case FFU_Texgen1EyeS: case FFU_Texgen1EyeT: case FFU_Texgen1EyeR: case FFU_Texgen1EyeQ:
                case FFU_Texgen2ObjS: case FFU_Texgen2ObjT: case FFU_Texgen2ObjR: case FFU_Texgen2ObjQ:
                case FFU_Texgen2EyeS: case FFU_Texgen2EyeT: case FFU_Texgen2EyeR: case FFU_Texgen2EyeQ:
                case FFU_Texgen3ObjS: case FFU_Texgen3ObjT: case FFU_Texgen3ObjR: case FFU_Texgen3ObjQ:
                case FFU_Texgen3EyeS: case FFU_Texgen3EyeT: case FFU_Texgen3EyeR: case FFU_Texgen3EyeQ: {
                    int idx = ( (*i).first ) - FFU_Texgen0ObjS;
                    int comp = idx % 4;
                    int unit = idx >> 3;
                    const State::Texgen & tg = p.tex[unit].texgen[comp];
                    if( idx & 4 ) {
                        if( ui.ver != tg.eyeVer ) {
                            ui.ver = tg.eyeVer;
                            tbl.glUniform4fv( ui.slot, 1, & tg.eye.x );
                        }
                    } else {
                        if( ui.ver != tg.objVer ) {
                            ui.ver = tg.objVer;
                            tbl.glUniform4fv( ui.slot, 1, & tg.obj.x );
                        }
                    }
                    break;
                }
                case FFU_ClipPlane0: case FFU_ClipPlane1: case FFU_ClipPlane2: case FFU_ClipPlane3:
                case FFU_ClipPlane4: case FFU_ClipPlane5: case FFU_ClipPlane6: case FFU_ClipPlane7: {
                    int idx = ( (*i).first ) - FFU_ClipPlane0;
                    if( ui.ver != p.clip[idx].ver ) {
                        ui.ver = p.clip[idx].ver;
                        tbl.glUniform4fv( ui.slot, 1, & p.clip[idx].plane.x );
                    }
                    break;
                }
                case FFU_Fog: {
                    if( ui.ver != p.fog.ver ) {
                        ui.ver = p.fog.ver;
                        tbl.glUniform4fv( ui.slot, 2, &p.fog.params[0].x);
                    }
                    break;
                }
                case FFU_AlphaRef: {
                    if( ui.ver != p.alphaTest.ver ) {
                        ui.ver = p.alphaTest.ver;
                        tbl.glUniform1f( ui.slot, p.alphaTest.alphaRef );
                    }
                    break;
                }
                case FFU_Attrib: {
                    if( ui.ver != p.vabVer ) {
                        ui.ver = p.vabVer;
                        tbl.glUniform4fv( ui.slot, REGAL_MAX_VERTEX_ATTRIBS, & immVab.attr[0].x );
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void RFF::UseFixedFunctionProgram( RegalContext * ctx ) {
    if( currprog != NULL && currprog->ver == ver.Current() ) {
        return;
    }
    ffstate.Process( this );
    if( ffprogmap.count( ffstate ) == 0 ) {
        Program & p = ffprogmap[ ffstate ];
        progcount++;
        string_list vsSrc;
        GenerateVertexShaderSource( this, ffstate, vsSrc );
        string_list fsSrc;
        GenerateFragmentShaderSource( es, legacy, textureObjToFmt, ffstate, fsSrc );
        p.Init( ctx, ffstate.processed, vsSrc.str().c_str(), fsSrc.str().c_str() );
        p.progcount = progcount;
        if( p.pg == 0 ) {
            Warning( "The program is 0. That can't be right.\n" );
        }
    }
    currprog = & ffprogmap[ ffstate ];
    if( currprog->pg == 0 ) {
        Warning( "The program is 0. That can't be right.\n" );
    }
    ctx->dsp->emuTbl.glUseProgram( currprog->pg );
    UpdateUniforms( ctx );
}

void RFF::UseShaderProgram( RegalContext * ctx ) {
    if( currprog != NULL && currprog->ver == ver.Current() ) {
        return;
    }
    ffstate.Process( this );
    if( shprogmap.count( program ) == 0 ) {
        Program & p = shprogmap[ program ];
        p.pg = program;
        p.Init( ctx, ffstate.processed );
    }
    currprog = & shprogmap[ program ];
    if( currprog->pg == 0 ) {
        Warning( "The program is 0. That can't be right.\n" );
    }
    UpdateUniforms( ctx );
}

void RFF::ShaderSource( RegalContext *ctx, GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
	if( string[0][0] == '#' && string[0][1] == 'v' ) {
		ctx->dsp->emuTbl.glShaderSource( shader, count, string, length );
		return;
	}
    std::vector< const GLchar * > s;
    s.resize( count + 1 );
    std::vector< GLint > l;
    l.resize( count + 1 );
    for( size_t i = 1; i < s.size(); i++ )
    {
        if (string) {
            s[i] = string[i - 1];
        }
        l[i] = length ? length[i - 1] : static_cast<GLint>( strlen( string[ i - 1 ] ));
    }
    string_list ss;
    if( es ) {
        ss << "#version 100\n";
    } else if ( legacy ) {
        ss << "#version 120\n";
    } else {
        ss << "#version 140\n";
    }
    if( es || legacy ) {
        if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {
            ss << "#define in attribute\n";
            ss << "#define out varying\n";
        } else {
            ss << "#define in varying\n";
        }
    } else {
        if( shaderTypeMap[ shader ] == GL_VERTEX_SHADER ) {

        } else {
            ss << "#define gl_FragColor rglFragColor\n";
            ss << "#define texture1D texture\n";
            ss << "#define texture2D texture\n";
            ss << "#define textureCube texture\n";
            ss << "out vec4 rglFragColor;\n";
        }
    }
    if( es ) {
        ss << "precision highp float;\n";
    }

    ss << "#define gl_Modelview rglModelview\n";
    ss << "#define gl_Projection rglProjection\n";
    ss << "#define gl_TextureMatrix0 rglTextureMatrix0\n";
    ss << "#define gl_Sampler0 rglSampler0\n\n";

    //Logging::Output( "foo:\n%s", ss.str().c_str() );
    std::string preamble = ss.str();
    s[0] = preamble.c_str();
    l[0] = static_cast<GLint>( strlen( s[0] ) );
    ctx->dsp->emuTbl.glShaderSource( shader, count + 1, &s[0], &l[0] );
}

void RFF::LinkProgram( RegalContext *ctx, GLuint program ) {
    ctx->dsp->emuTbl.glLinkProgram( program );
}

REGAL_NAMESPACE_END
