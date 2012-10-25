/*

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

- Created by Cass Everitt on 3/16/11.

*/

#include <GL/Regal.h>

#include "render.h"

#include <cstdio>
#include <cmath>

#ifndef M_PI
#define M_PI 3.141572654
#endif

static GLenum texunit = GL_TEXTURE1;

struct Torus {
    float circleRadius;
    float tubeRadius;
    Torus( float c, float t ) : circleRadius( c ), tubeRadius( t ) {}
    void Vertex( float u, float v )
    {
        glPushGroupMarkerEXT(0, "Torus::Vertex");

        float theta = (float) (u * 2.0 * M_PI);
        float rho   = (float) (v * 2.0 * M_PI);
        float x     = (float) (cos( theta ) * ( circleRadius + cos( rho ) * tubeRadius ));
        float y     = (float) (sin( theta ) * ( circleRadius + cos( rho ) * tubeRadius ));
        float z     = (float) (sin( rho ) * tubeRadius);
        float nx    = (float) (cos( rho ) * cos(theta));
        float ny    = (float) (cos( rho ) * sin(theta));
        float nz    = (float) (sin( rho ));

        glNormal3f( nx, ny, nz );
        glMultiTexCoord2f( texunit, u, v );
        glVertex3f( x, y, z );

        glPopGroupMarkerEXT();
    }
};

static void drawAnObject()
{
  glPushGroupMarkerEXT(0, "drawAnObject");

  Torus t( 0.7f, 0.2f );
  int I = 30;
  int J = 30;
  glColor3f( 0.8f, 0.8f, 0.8f );
  for(int j = 0; j < J / 2 - 1; j++)
  {
    float v0 = (j+0.0f)/(J-1.0f);
    float v1 = (j+1.0f)/(J-1.0f);
    glBegin( GL_TRIANGLE_STRIP );
    for(int i = 0; i < I; i++)
    {
      float u = i/(I-1.0f);
      t.Vertex( u, v1 );
      t.Vertex( u, v0 );
    }
    glEnd();
  }

  glPopGroupMarkerEXT();
}

static int width;
static int height;

void dreamTorusReshape( int w, int h ) {
    width = w;
    height = h;

    float a = float(w)/float(h);

    glPushGroupMarkerEXT(0, "reshape");

    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    if( a > 1 ) {
        glFrustum( -0.1 * a, 0.1 * a, -0.1, 0.1, 0.1, 100.0 );
    } else {
        glFrustum( -0.1, 0.1, -0.1 / a, 0.1 / a, 0.1, 100.0 );
    }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0, 0, -2 );

    glPopGroupMarkerEXT();
}


static void regalerr( GLenum err ) {
    const char * errstr = NULL;
    switch( err ) {
        case GL_INVALID_ENUM: errstr = "INVALID ENUM"; break;
        case GL_INVALID_OPERATION: errstr = "INVALID OPERATION"; break;
        case GL_INVALID_VALUE: errstr = "INVALID VALUE"; break;
        default:
            fprintf( stderr, "Got a GL error: %d!\n", err );
            break;
    }
    if( errstr ) {
        fprintf( stderr, "Got a GL error: %s\n", errstr );
    }
    ;
}

static GLuint tex;

void dreamTorusInit()
{
    glPushGroupMarkerEXT(0, "init");

    glGenTextures( 1, &tex );
    GLubyte pix[] = {
        0x60, 0xff, 0x00, 0xff,  0xff, 0x00, 0x00, 0xff,  0xff, 0x00, 0xff, 0xff,  0x00, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,  0x00, 0x00, 0xff, 0xff,  0x00, 0x00, 0x00, 0xff,  0xff, 0x80, 0x00, 0xff,
        0x80, 0x80, 0xff, 0xff,  0xff, 0x00, 0x00, 0xff,  0x80, 0x80, 0x80, 0xff,  0x00, 0x80, 0x80, 0xff,
        0x00, 0xff, 0xff, 0xff,  0x00, 0x80, 0x00, 0xff,  0x80, 0x00, 0xff, 0xff,  0x00, 0x00, 0x80, 0xff
    };
    glTextureImage2DEXT( tex, GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix );
    glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindMultiTextureEXT( texunit, GL_TEXTURE_2D, tex );

    GLfloat mat_specular[]   = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[]  = { 50.0f };
    GLfloat light_position[] = { 1.0f, 1.0f, 0.2f, 1.0f };
    GLfloat light_atten[]    = { 1.0f, 1.0f, 1.0f };
    GLfloat light_diffuse[]  = { 10.0f, 10.0f, 10.0f, 10.0f };
    GLfloat light_specular[] = { 10.0f, 10.0f, 10.0f, 10.0f };
    GLfloat light_spotdir[]  = { -0.1f, -0.1f, -1.0f };
    GLfloat light_spotcut[]  = { 30.0f };
    GLfloat light_spotexp[]  = { 3.0f };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //glShadeModel (GL_SMOOTH);

    glMatrixPushEXT( GL_MODELVIEW );
    glMatrixLoadIdentityEXT( GL_MODELVIEW );

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    //glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, light_atten[1] );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light_atten[2] );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );
    glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, light_spotdir );
    glLightfv( GL_LIGHT0, GL_SPOT_CUTOFF, light_spotcut );
    glLightfv( GL_LIGHT0, GL_SPOT_EXPONENT, light_spotexp );
    //GLfloat light_ambient[] = { 0.0, -1.0, 0.0, 0.0 };
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glMatrixPopEXT( GL_MODELVIEW );

    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
    glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

    glEnable( GL_COLOR_MATERIAL ) ;
    glColorMaterial( GL_BACK, GL_SPECULAR );

    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glFogi( GL_FOG_MODE, GL_LINEAR );
    glFogf( GL_FOG_START, 2.0f );
    glFogf( GL_FOG_END, 4.0f );
    GLfloat fog_color[] = { 1.0, 1.0, 0.0, 0.0 };
    glFogfv( GL_FOG_COLOR, fog_color );

    glEnable( GL_CLIP_PLANE3 );
    GLdouble clip[] = { 1, 1, -1, 0 };
    glClipPlane( GL_CLIP_PLANE3, clip );

    glPopGroupMarkerEXT();
}

void dreamTorusDisplay( bool clear )
{
    static float r = 0.0f;
    static int count = 0;

    if( count == 0 ) {
        dreamTorusInit();
    }

    glPushGroupMarkerEXT(0, "display");

#if 1
    if( count == 0 ) {
        RegalSetErrorCallback( regalerr );
    } else if( count == 11 ) {
        RegalSetErrorCallback( NULL );
    }
    count++;
#endif

    glUseProgram( 0 );
    if( clear ) {
        glClearDepth( 1.0 );
        glClearColor( r / 400.f, 0, 0, 0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    glEnable( GL_DEPTH_TEST );

    for( int i = 0; i < 8; i++ ) {
        glActiveTexture( GL_TEXTURE0 + i );
        glDisable( GL_TEXTURE_2D );
    }

    glBindMultiTextureEXT( texunit, GL_TEXTURE_2D, tex );
    glActiveTexture( texunit );
    glEnable( GL_TEXTURE_2D );


    float sc = 2.0f * fabs( ( count % 800 ) / 400.0f - 1.0f ) + 1.0f;

    glMatrixLoadIdentityEXT( texunit );
    glMatrixScalefEXT( texunit, sc, 1, 1 );

    float osc = sc * 0.5f;


    glEnable( GL_FOG );

    if( ( count % 7 ) == 0 || ( count % 17 ) == 0 ) {
        glDisable( GL_NORMALIZE );
        glEnable( GL_RESCALE_NORMAL );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glRotatef( (float) count, -1.0f, 1.0f, 0.0f );
        glScalef( osc, osc, osc );
        drawAnObject();
        glPopMatrix();
    } else {
        glEnable( GL_NORMALIZE );
        glDisable( GL_RESCALE_NORMAL );
        glMultiTexEnviEXT( texunit, GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        glMatrixPushEXT( GL_MODELVIEW );
        glMatrixRotatefEXT( GL_MODELVIEW, (float) count, -1.0f, 1.0f, 0.0f );
        glMatrixScalefEXT( GL_MODELVIEW, osc, osc, osc );
        drawAnObject();
        glMatrixPopEXT( GL_MODELVIEW );
    }

    r += 1.0f;
    if( r > 399 ) {
        r = 0.0f;
    }

    glPopGroupMarkerEXT();

    glFrameTerminatorGREMEDY();

  //printf( "Draw with r=%f\n", r );
}
