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

#if ! REGAL_HTTP

REGAL_GLOBAL_BEGIN
#include "RegalHttp.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

Http::Http() {}
Http::~Http() {}
void Http::Init() {}
void Http::Start() {}
void Http::Stop() {}
void Http::YieldToHttpServer( RegalContext *, bool ) {}
void Regal::Http::GlProcs::Initialize( Regal::Dispatch::GL * ) {}

REGAL_NAMESPACE_END

#else // REGAL_HTTP

#define REGAL_HTTP_LOCAL_JQUERY 0

#define REGAL_HTTP_IMAGE_WRITE_IMPL 1

#if REGAL_HTTP_IMAGE_WRITE_IMPL
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb_image_write.h"


REGAL_GLOBAL_BEGIN

//#include "pcre.h"

#include "Http.h"
#include "Favicon.h"
#include "jscode.h"

#include "RegalContext.h"
#include "RegalToken.h"
#include "RegalShaderInstance.h"

#include "RegalLog.h"
#include "RegalInit.h"
#include "RegalConfig.h"
#include "RegalThread.h"
#include "RegalContextInfo.h"
#include "RegalEmuInfo.h"

#include <map>

#include <string>
#include <cstdio>
#include <cstdarg>
using namespace std;

#include "RegalPrint.h"


#include "mongoose.h"

#ifndef REGAL_HTTP_PORT
#define REGAL_HTTP_PORT 8080
#endif

#if REGAL_SYS_WGL
extern "C" { BOOL __stdcall Sleep(DWORD); }
#endif

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

RegalContext::ParkProcs pp;

static mg_callbacks callbacks;         // Callbacks
static mg_context   *mgctx = NULL;       // Mongoose context

static bool enabled = REGAL_HTTP;
static int  port = REGAL_HTTP_PORT;

Http * GetInstanceByIndex( int idx );

struct Connection {
  Connection( mg_connection * conn ) : connection( conn ) {
    request_info = mg_get_request_info( conn );
    ParseUri( request_info->uri );
  }
  
  void ParseUri( const string & uri ) {
    size_t start = 1;
    path.clear();
    for(;;) {
      size_t end = uri.find( "/", start );
      string v = uri.substr( start, end - start );
      if( v.size() ) {
        path.push_back( v );
      }
      if( end == string::npos ) break;
      start = end+1;
    }
  }
  
  vector<string> path;
  mg_connection * connection;
  const mg_request_info *request_info;
};

struct ScopedContextAcquire {
  ScopedContextAcquire( Http * _http ) : http(_http) {
    http->AcquireAppContext();
  }
  ~ScopedContextAcquire() {
    http->ReleaseAppContext();
  }
  Http * http;
};

struct RequestHandler {
  RequestHandler() {}
  ~RequestHandler() {}
  
  virtual void HandleRequest( Connection & c ) = 0;
  virtual std::string GetHandlerString() = 0;
  bool Matches( const Connection & conn ) {
    return conn.path.size() > 0 && conn.path[0] == GetHandlerString();
  }
  
};
void Redirect( Connection & conn, const string & redirect_to );

typedef map<string, RequestHandler *> HandlerMap;
HandlerMap handlers;

void CreateHandlers();

void SendRootDocument( Connection & conn );



const char * const br = "<br/>\n";

static int log_message(const struct mg_connection * conn, const char *message)
{
  UNUSED_PARAMETER(conn);
  HTrace(message ? message : "");
  return 1;
}

static int begin_request(struct mg_connection * conn)
{
  const struct mg_request_info *request_info = mg_get_request_info( conn );
  
  HTrace(request_info->request_method ? request_info->request_method : "", " ",
         request_info->uri            ? request_info->uri            : "",
         request_info->query_string   ?                        "?"   : "",
         request_info->query_string   ? request_info->query_string   : "");
  
  Connection connection( conn );
  
  if( connection.path.size() > 0 ) {
    for( HandlerMap::iterator i = handlers.begin(); i != handlers.end(); ++i ) {
      RequestHandler *rh = i->second;
      if( rh->Matches( connection ) ) {
        rh->HandleRequest( connection );
        return 1;
      }
    }
    Redirect( connection, "/" );
  } else {
    SendRootDocument( connection );
  }
  
  return 1;  // Mark as handled for Mongoose
}

void Http::Start()
{
  Internal("Http::Start","()");
  
  if (enabled)
  {
    memset(&callbacks,0,sizeof(callbacks));
    callbacks.log_message   = log_message;
    callbacks.begin_request = begin_request;
    
    // Try listening on the configured port number (8080 by default)
    // and advance foward until one is available
    
    for (int i = 0; i<100; ++i)
    {
      string j = print_string(port+i);
      const char *options[] = { "listening_ports", j.c_str(), "num_threads", "1", NULL};
      mgctx = mg_start(&callbacks, NULL, options);
      if (mgctx)
        break;
    }
    
    const char *option = NULL;
    if (mgctx)
      option = mg_get_option(mgctx,"listening_ports");
    
    if (option)
    {
      Info("Listening for HTTP connections on port ",option);
    }
    else
    {
      Info("Not listening for HTTP connections.");
    }
  }
}

void Http::Stop()
{
  Internal("Http::Stop","()");
  
  if (mgctx)
  {
    HTrace("Closing HTTP connections.");
    
    // Currently there is a problem with shutting down mongoose
    // on Windows - so just skip the cleanup for now.
    
#if !REGAL_SYS_WGL
    mg_stop(mgctx);
#endif
    
    mgctx = NULL;
  }
}

void EraseLastComma( string & str ) {
  if( str[ str.size() - 2 ] == ',' ) {
    str.erase( str.size() - 2, 1 );
  }
}

void SendText( Connection & conn, const string & contentType, const string & str ) {
  string http = print_string(
                             "HTTP/1.1 200 OK\r\n"
                             "Content-Type: ", contentType, "\r\n"
                             "Content-Length: ", str.length(), "\r\n"
                             "\r\n" );
  http += str;
  
  mg_write( conn.connection, http.c_str(), http.length() );
}

void SendHTML( Connection & conn, const string & body, const string & head = string() ) {
  string html = print_string(
                             "<html><head>\n", head,
                             "</head><body>\n" );
  html += body;
  html += "</body></html>\n";
  SendText( conn, "text/html", html );
}

void SendRootDocument( Connection & conn ) {
  string head;
#if REGAL_HTTP_LOCAL_JQUERY
  head += "<link rel=\"stylesheet\" href=\"/script/jquery-ui.min.css\" />\n";
  head += "<script src=\"/script/jquery.min.js\"></script>\n";
  head += "<script src=\"/script/jquery-ui.min.js\"></script>\n";
#else // ! REGAL_HTTP_LOCAL_JQUERY
  head += "<link rel=\"stylesheet\" href=\"http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.3/themes/le-frog/jquery-ui.min.css\" />\n";
  head += "<script src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js\"></script>\n";
  head += "<script src=\"http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.3/jquery-ui.min.js\"></script>\n";
#endif // REGAL_HTTP_LOCAL_JQUERY
  head += "<script src=\"/script/regalweb.js\"></script>\n";
  string body;
  SendHTML( conn, body, head );
}


struct ScriptHandler : public RequestHandler {
  
  virtual void HandleRequest( Connection & conn ) {
    if( conn.path.size() != 2 ) {
      return;
    }
    if( conn.path[1] == "regalweb.js" ) {
      FILE *fp = fopen( "/tmp/regalweb.js", "rb" );
      string code;
      if( fp ) {
        char buf[257];
        size_t count = 0;
        while( (count = fread( buf, 1, sizeof( buf ) - 1, fp ) ) > 0  ) {
          buf[count] = 0;
          code += buf;
          count = 0;
        }
        fclose( fp );
      } else {
        code = regalwebjs;
      }
      SendText( conn, "application/javascript", code );
#if REGAL_HTTP_LOCAL_JQUERY
    } else if( conn.path[1] == "jquery.min.js" ) {
      string code = jqueryminjs;
      SendText( conn, "application/javascript", code );
    } else if( conn.path[1] == "jquery-ui.min.js" ) {
      string code = jqueryuiminjs;
      SendText( conn, "application/javascript", code );
    } else if( conn.path[1] == "jquery-ui.min.css" ) {
      string css = jqueryuimincss;
      SendText( conn, "text/css", css );
#endif // REGAL_HTTP_LOCAL_JQUERY
    }
  }
  virtual string GetHandlerString() {
    return "script";
  }
  
};

struct FaviconHandler : public RequestHandler {
  
  virtual void HandleRequest( Connection & conn ) {
    const mg_request_info *request_info = mg_get_request_info( conn.connection );
    
    RegalAssert( strcmp("/favicon.ico",request_info->uri) == 0 );
    string http = print_string(
                               "HTTP/1.1 200 OK\r\n"
                               "Content-Type: image/x-icon\r\n"
                               "Content-Length: ", sizeof(favicon), "\r\n"
                               "\r\n");
    
    mg_write(conn.connection, http.c_str(), http.length() );
    mg_write(conn.connection, favicon, sizeof(favicon) );
  }
  virtual string GetHandlerString() {
    return "favicon.ico";
  }
  
};

struct ContextsHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    //const mg_request_info *request_info = mg_get_request_info( conn.connection );
    //RegalAssert( strcmp("/contexts",request_info->uri) == 0 );
    string body;
    ::REGAL_NAMESPACE_INTERNAL::Init::getContextListingHTML(body);
    SendHTML( conn, body );
  }
  virtual string GetHandlerString() {
    return "contexts";
  }
};

enum TextureParameterType { TPT_Float, TPT_Integer, TPT_Enum };
struct TextureObjectParameter {
  GLenum pname;
  TextureParameterType type;
  int numComponents;
  GLfloat initVal[4]; // should be able to cast a float into whatever is needed
};

const TextureObjectParameter top[] = {
  { GL_DEPTH_STENCIL_TEXTURE_MODE, TPT_Enum,    1, { GL_DEPTH_COMPONENT, 0, 0, 0 } },
  { GL_TEXTURE_BASE_LEVEL,         TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_BORDER_COLOR,       TPT_Float,   4, { 0, 0, 0, 0 } },
  { GL_TEXTURE_COMPARE_MODE,       TPT_Enum,    1, { GL_NONE, 0, 0, 0 } },
  { GL_TEXTURE_COMPARE_FUNC,       TPT_Enum,    1, { GL_LEQUAL, 0, 0, 0 } },
  { GL_TEXTURE_IMMUTABLE_FORMAT,   TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_IMMUTABLE_LEVELS,   TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_LOD_BIAS,           TPT_Float,   1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_MAG_FILTER,         TPT_Enum,    1, { GL_LINEAR, 0, 0, 0 } },
  { GL_TEXTURE_MAX_LEVEL,          TPT_Integer, 1, { 1000, 0, 0, 0 } },
  { GL_TEXTURE_MAX_LOD,            TPT_Float,   1, { 1000, 0, 0, 0 } },
  { GL_TEXTURE_MIN_FILTER,         TPT_Enum,    1, { GL_NEAREST_MIPMAP_LINEAR, 0, 0, 0 } },
  { GL_TEXTURE_MIN_LOD,            TPT_Float,   1, { -1000, 0, 0, 0 } },
  { GL_TEXTURE_SWIZZLE_RGBA,       TPT_Enum,    4, { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA } },
  { GL_TEXTURE_VIEW_MIN_LAYER,     TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_VIEW_MIN_LEVEL,     TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_VIEW_NUM_LAYERS,    TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_VIEW_NUM_LEVELS,    TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_WRAP_S,             TPT_Enum,    1, { GL_REPEAT, 0, 0, 0 } },
  { GL_TEXTURE_WRAP_T,             TPT_Enum,    1, { GL_REPEAT, 0, 0, 0 } },
  { GL_TEXTURE_WRAP_R,             TPT_Enum,    1, { GL_REPEAT, 0, 0, 0 } }
};
const TextureObjectParameter tolp[] = {
  { GL_TEXTURE_INTERNAL_FORMAT,    TPT_Enum,    1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_WIDTH,              TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_HEIGHT,             TPT_Integer, 1, { 0, 0, 0, 0 } },
  { GL_TEXTURE_DEPTH,              TPT_Integer, 1, { 0, 0, 0, 0 } }
};

string PrintTextureObjectParameter( const TextureObjectParameter & p, const GLfloat *fval ) {
  string json;
  switch( p.type ) {
    case TPT_Enum: {
      json += print_string( "\"", Token::GLenumToString( p.pname ), "\": " );
      if( p.numComponents > 1 ) { json += "[ "; }
      for( int comp = 0; comp < p.numComponents; comp++ ) {
        json += print_string( "\"", Token::GLenumToString( int( fval[comp] ) ), "\"" );
        if( comp < p.numComponents - 1 ) { json += ", "; }
      }
      if( p.numComponents > 1 ) { json += "]"; }
      json += ",\n";
    } break;
    case TPT_Integer: {
      json += print_string( "\"", Token::GLenumToString( p.pname ), "\": " );
      if( p.numComponents > 1 ) { json += "[ "; }
      for( int comp = 0; comp < p.numComponents; comp++ ) {
        json += print_string( int( fval[comp] ) );
        if( comp < p.numComponents - 1 ) { json += ", "; }
      }
      if( p.numComponents > 1 ) { json += "]"; }
      json += ",\n";
    } break;
    case TPT_Float: {
      json += print_string( "\"", Token::GLenumToString( p.pname ), "\": " );
      if( p.numComponents > 1 ) { json += "[ "; }
      for( int comp = 0; comp < p.numComponents; comp++ ) {
        json += print_string( fval[comp] );
        if( comp < p.numComponents - 1 ) { json += ", "; }
      }
      if( p.numComponents > 1 ) { json += "]"; }
      json += ",\n";
    } break;
    default: break;
  }
  return json;
}

struct TextureHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    Http * http = GetInstanceByIndex( 0 );
    if( http == NULL ) {
      return;
    }
    Http & h = *http;
    RegalContext * ctx = h.GetContext();

    string json;
    if( conn.path.size() == 1 ) {
      
      json += "[ ";
      for( map<GLuint, HttpTextureInfo >::iterator i = h.texture.begin(); i != h.texture.end(); ++i ) {
        json += print_string( i->first, ", " );
      }
      EraseLastComma( json );
      json += "]\n";
      SendText( conn, "application/json", json );
      
    } else if( conn.path.size() >= 2 ) {
      
      string tex = conn.path[1];
      GLint texname = atoi( tex.c_str() );
      HttpTextureInfo & texinfo = h.texture[ texname ];
      if( conn.path.size() == 2 ) {
        int indent = 0;
        json += "{\n";
        indent += 2;
        if( ctx->emuInfo->gl_ext_direct_state_access == GL_TRUE || ctx->info->gl_ext_direct_state_access ) {
          h.AcquireAppContext();
          json += "  \"name\": " + tex + ",\n";
          GLint baseLevel = 0;
          GLint maxLevel = 0;
          json += "  \"target\": \"" + string( Token::GLenumToString( texinfo.target ) ) + "\",\n";
          for( int i = 0; i < sizeof(top)/sizeof(top[0]); i++ ) {
            GLfloat fval[4] = { 0, 0, 0, 0 };
            const TextureObjectParameter & p = top[i];
            RglGetTextureParameterfvEXT( h.gl, texname, GL_TEXTURE_2D, p.pname, fval );
            switch( p.pname ) {
              case GL_TEXTURE_BASE_LEVEL: baseLevel = GLint(fval[0]); break;
              case GL_TEXTURE_MAX_LEVEL: maxLevel = GLint(fval[0]); break;
              default: break;
            }
            if( fval[0] != p.initVal[0] ) {
              json += string( indent, ' ' ) + PrintTextureObjectParameter( p, fval );
            }
          }
          json += string( indent, ' ' ) + "\"levels\": [\n";
          indent += 2;
          for( int level = baseLevel; level <= maxLevel; level++ ) {
            GLfloat fval[4] = { 0, 0, 0, 0 };
            RglGetTextureLevelParameterfvEXT( h.gl, texname, GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, fval );
            if( int(fval[0]) == 0 ) {
              break;
            }
            json += string( indent, ' ' ) + "{\n";
            indent += 2;
            for( int i = 0; i < sizeof(tolp)/sizeof(tolp[0]); i++ ) {
              GLfloat fval[4] = { 0, 0, 0, 0 };
              const TextureObjectParameter & p = tolp[i];
              RglGetTextureLevelParameterfvEXT( h.gl, texname, GL_TEXTURE_2D, level, p.pname, fval );
              if( fval[0] != p.initVal[0] ) {
                json += string( indent, ' ' ) + PrintTextureObjectParameter( p, fval );
              }
            }
            indent -= 2;
            EraseLastComma( json );
            json += string( indent, ' ' ) + "},\n";
            if( (level - baseLevel) >= 16 ) {
              break;
            }
          }
          EraseLastComma( json );
          indent -= 2;
          json += string( indent, ' ' ) + "]\n";
          h.ReleaseAppContext();
        }
        EraseLastComma( json );
        indent -= 2;
        json += "}\n";
        
        SendText( conn, "application/json", json );
      } else if( conn.path.size() == 3 && conn.path[2] == "image" ) {
        if( ctx->emuInfo->gl_ext_direct_state_access == GL_TRUE || ctx->info->gl_ext_direct_state_access ) {
          h.AcquireAppContext();
          GLfloat fwidth, fheight;
          RglGetTextureLevelParameterfvEXT( h.gl, texname, texinfo.target, 0, GL_TEXTURE_WIDTH, &fwidth );
          RglGetTextureLevelParameterfvEXT( h.gl, texname, texinfo.target, 0, GL_TEXTURE_HEIGHT, &fheight );
          
          int width = int(fwidth);
          int height = int(fheight);
          
          if( width <= 0 || height <= 0 ) {
            // evil
            return;
          }
          
          int stride = width * 4;
          unsigned char * pixels = new unsigned char[ int(height + 1) * stride ];
          
          RglGetTextureImageEXT( h.gl, texname, texinfo.target, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
          h.ReleaseAppContext();
          
          for( int j = 0; j < height/2; j++ ) {
            unsigned char * s = pixels + (height - 1 - j) * stride;
            unsigned char * d = pixels + j * stride;
            unsigned char * t = pixels + height * stride;
            memcpy( t, d, stride );
            memcpy( d, s, stride );
            memcpy( s, t, stride );
          }
          int out_len = 0;
          unsigned char * img = stbi_write_png_to_mem(pixels, width * 4, width, height, 4, &out_len );
          
          string http = print_string(
                                     "HTTP/1.1 200 OK\r\n"
                                     "Content-Type: image/png\r\n"
                                     "Expires: Fri, 30 Oct 1998 14:19:41 GMT\r\n"
                                     "Cache-Control: no-cache, must-revalidate\r\n"
                                     "Content-Length: ", out_len, "\r\n"
                                     "\r\n");
          
          mg_write(conn.connection, http.c_str(), http.length() );
          mg_write(conn.connection, img, out_len );
          
          delete [] pixels;
          free( img );
          
        }
      }
    }
  }
  virtual string GetHandlerString() {
    return "texture";
  }
};



enum UniformApiType {
  UAT_Invalid, UAT_Double, UAT_Float, UAT_Int, UAT_UnsignedInt, UAT_DoubleMatrix, UAT_FloatMatrix
};

UniformApiType GetUniformApiType( GLenum type ) {
  switch( type ) {
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
      return UAT_Float;
    case GL_DOUBLE:
    case GL_DOUBLE_VEC2:
    case GL_DOUBLE_VEC3:
    case GL_DOUBLE_VEC4:
      return UAT_Double;
    case GL_INT:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
      return UAT_Int;
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_VEC2:
    case GL_UNSIGNED_INT_VEC3:
    case GL_UNSIGNED_INT_VEC4:
      return UAT_UnsignedInt;
    case GL_BOOL:
    case GL_BOOL_VEC2:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
      return UAT_Int;
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
    case GL_FLOAT_MAT2x3:
    case GL_FLOAT_MAT2x4:
    case GL_FLOAT_MAT3x2:
    case GL_FLOAT_MAT3x4:
    case GL_FLOAT_MAT4x2:
    case GL_FLOAT_MAT4x3:
      return UAT_FloatMatrix;
    case GL_DOUBLE_MAT2:
    case GL_DOUBLE_MAT3:
    case GL_DOUBLE_MAT4:
    case GL_DOUBLE_MAT2x3:
    case GL_DOUBLE_MAT2x4:
    case GL_DOUBLE_MAT3x2:
    case GL_DOUBLE_MAT3x4:
    case GL_DOUBLE_MAT4x2:
    case GL_DOUBLE_MAT4x3:
      return UAT_DoubleMatrix;
    case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_1D_SHADOW:
    case GL_SAMPLER_2D_SHADOW:
    case GL_SAMPLER_1D_ARRAY:
    case GL_SAMPLER_2D_ARRAY:
    case GL_SAMPLER_1D_ARRAY_SHADOW:
    case GL_SAMPLER_2D_ARRAY_SHADOW:
    case GL_SAMPLER_2D_MULTISAMPLE:
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_SAMPLER_CUBE_SHADOW:
    case GL_SAMPLER_BUFFER:
    case GL_SAMPLER_2D_RECT:
    case GL_SAMPLER_2D_RECT_SHADOW:
    case GL_INT_SAMPLER_1D:
    case GL_INT_SAMPLER_2D:
    case GL_INT_SAMPLER_3D:
    case GL_INT_SAMPLER_CUBE:
    case GL_INT_SAMPLER_1D_ARRAY:
    case GL_INT_SAMPLER_2D_ARRAY:
    case GL_INT_SAMPLER_2D_MULTISAMPLE:
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_INT_SAMPLER_BUFFER:
    case GL_INT_SAMPLER_2D_RECT:
    case GL_UNSIGNED_INT_SAMPLER_1D:
    case GL_UNSIGNED_INT_SAMPLER_2D:
    case GL_UNSIGNED_INT_SAMPLER_3D:
    case GL_UNSIGNED_INT_SAMPLER_CUBE:
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
    case GL_IMAGE_1D:
    case GL_IMAGE_2D:
    case GL_IMAGE_3D:
    case GL_IMAGE_2D_RECT:
    case GL_IMAGE_CUBE:
    case GL_IMAGE_BUFFER:
    case GL_IMAGE_1D_ARRAY:
    case GL_IMAGE_2D_ARRAY:
    case GL_IMAGE_2D_MULTISAMPLE:
    case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
    case GL_INT_IMAGE_1D:
    case GL_INT_IMAGE_2D:
    case GL_INT_IMAGE_3D:
    case GL_INT_IMAGE_2D_RECT:
    case GL_INT_IMAGE_CUBE:
    case GL_INT_IMAGE_BUFFER:
    case GL_INT_IMAGE_1D_ARRAY:
    case GL_INT_IMAGE_2D_ARRAY:
    case GL_INT_IMAGE_2D_MULTISAMPLE:
    case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    case GL_UNSIGNED_INT_IMAGE_1D:
    case GL_UNSIGNED_INT_IMAGE_2D:
    case GL_UNSIGNED_INT_IMAGE_3D:
    case GL_UNSIGNED_INT_IMAGE_2D_RECT:
    case GL_UNSIGNED_INT_IMAGE_CUBE:
    case GL_UNSIGNED_INT_IMAGE_BUFFER:
    case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
    case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
    case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
    case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    case GL_UNSIGNED_INT_ATOMIC_COUNTER:
      return UAT_Int;
      
    default: break;
  }
  return UAT_Invalid;
}

struct MatrixDims {
  MatrixDims( int r, int c ) : rows( r ), cols( c ) {}
  MatrixDims( const MatrixDims & rhs ) : rows( rhs.rows ), cols( rhs.cols ) {}
  int rows;
  int cols;
};

MatrixDims GetMatrixDims( GLenum type ) {
  switch( type ) {
    case GL_DOUBLE_MAT2: case GL_FLOAT_MAT2: return MatrixDims( 2, 2 );
    case GL_DOUBLE_MAT3: case GL_FLOAT_MAT3: return MatrixDims( 3, 3 );
    case GL_DOUBLE_MAT4: case GL_FLOAT_MAT4: return MatrixDims( 4, 4 );
    case GL_DOUBLE_MAT2x3: case GL_FLOAT_MAT2x3: return MatrixDims( 2, 3 );
    case GL_DOUBLE_MAT2x4: case GL_FLOAT_MAT2x4: return MatrixDims( 2, 4 );
    case GL_DOUBLE_MAT3x2: case GL_FLOAT_MAT3x2: return MatrixDims( 3, 2 );
    case GL_DOUBLE_MAT3x4: case GL_FLOAT_MAT3x4: return MatrixDims( 3, 4 );
    case GL_DOUBLE_MAT4x2: case GL_FLOAT_MAT4x2: return MatrixDims( 4, 2 );
    case GL_DOUBLE_MAT4x3: case GL_FLOAT_MAT4x3: return MatrixDims( 4, 3 );
    default: break;
  }
  return MatrixDims( 0, 0 );
}

int GetMatrixElementSize( GLenum type ) {
  switch( type ) {
    case GL_FLOAT_MAT2: case GL_FLOAT_MAT3: case GL_FLOAT_MAT4:
    case GL_FLOAT_MAT2x3: case GL_FLOAT_MAT2x4: case GL_FLOAT_MAT3x2:
    case GL_FLOAT_MAT3x4:  case GL_FLOAT_MAT4x2: case GL_FLOAT_MAT4x3:
      return sizeof( GLfloat );
    case GL_DOUBLE_MAT2: case GL_DOUBLE_MAT3: case GL_DOUBLE_MAT4:
    case GL_DOUBLE_MAT2x3: case GL_DOUBLE_MAT2x4: case GL_DOUBLE_MAT3x2:
    case GL_DOUBLE_MAT3x4: case GL_DOUBLE_MAT4x2: case GL_DOUBLE_MAT4x3:
      return sizeof( GLdouble );
    default: break;
  }
  return 0;
}


template <typename T> string PrintArrayElement( int sz, const T *p ) {
  string el;
  int count = sz / sizeof(T);
  if( count > 1 ) {
    el += "[ ";
  }
  for( int j = 0; j < count; j++ ) {
    el += print_string( p[j] );
    if( j < (count - 1) ) {
      el += ", ";
    }
  }
  if( count > 1 ) {
    el += " ]";
  }
  return el;
}

string PrintUniformValue( ShaderInstance::Procs & tbl, GLuint program, GLint location, GLsizei count, GLenum type, int indent ) {
  char buf[1<<14];
  int sz = ShaderInstance::GetTypeSize( type );
  if( (count * sz) > sizeof(buf) ) {
    return "\"uniform too large\"";
  }
  ShaderInstance::GetUniform( tbl, program, location, count, type, buf );
  
  string u;
  if( count > 1 ) {
    u += "{\n";
    indent += 2;
    u += string( indent, ' ' );
  }
  
  for( int i = 0; i < count; i++ ) {
    char *up = buf + i * sz;
    UniformApiType uat = GetUniformApiType( type );
    if( i < 0 ) {
      u += string( indent, ' ' );
    }
    switch( uat ) {
      case UAT_Double:
      case UAT_DoubleMatrix:
        u += PrintArrayElement( sz, reinterpret_cast<GLdouble *>(up) );
        break;
      case UAT_Float:
      case UAT_FloatMatrix:
        u += PrintArrayElement( sz, reinterpret_cast<GLfloat *>(up) );
        break;
      case UAT_Int:
        u += PrintArrayElement( sz, reinterpret_cast<GLint *>(up) );
        break;
        
      default:
        break;
    }
  }
  
  if( count > 1 ) {
    indent -= 2;
    u += string( indent, ' ' ) + "}\n";
  }
  return u;
}

string PrintMultiLine( char * bigstring, int len, int indent ) {
  string json;
  json += "[\n";
  indent += 2;
  char * p = bigstring;
  int i = 0;
  while( i < len ) {
    if( p[i] == '\n' ) {
      p[i] = 0;
      json += string( indent, ' ' ) + "\"" + string( p ) + "\",\n";
      p += i+1;
      len -= i+1;
      i = -1;
    }
    i++;
  }
  EraseLastComma( json );
  indent -= 2;
  json += string( indent, ' ' ) + print_string( "],\n" );
  
  return json;
}

struct ProgramHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    Http * http = GetInstanceByIndex( 0 );
    if( http == NULL ) {
      return;
    }
    Http & h = *http;
    string json;
    if( conn.path.size() == 1 ) {
      
      json += "[ ";
      for( set<GLuint>::iterator i = h.program.begin(); i != h.program.end(); ++i ) {
        json += print_string( *i, ", " );
      }
      EraseLastComma( json );
      json += "]\n";
      SendText( conn, "application/json", json );
    } else if ( conn.path.size() == 2 ) {
      string progNameString = conn.path[1];
      GLint prog = atoi( progNameString.c_str() );
      int indent = 0;
      json += "{\n";
      indent += 2;
      Http & h = *http;
      h.AcquireAppContext();
      json += string( indent, ' ' ) + "\"name\": " + progNameString + ",\n";
      GLuint shaders[8];
      GLsizei numShaders = 0;
      RglGetAttachedShaders( h.gl, prog, (GLsizei)array_size(shaders), &numShaders, shaders );
      if( numShaders > 0 ) {
        json += string( indent, ' ' ) + "\"shaders\": [ ";
        for( int i = 0; i < numShaders; i++ ) {
          json += print_string( shaders[i] );
          if( i < numShaders - 1 ) {
            json += ", ";
          }
        }
        json += " ],\n";
      }
      GLint activeUniforms = 0;
      RglGetProgramiv( h.gl, prog, GL_ACTIVE_UNIFORMS, &activeUniforms );
      if( activeUniforms > 0 ) {
        json += string( indent, ' ' ) + "\"GL_ACTIVE_UNIFORMS\": " + print_string( activeUniforms, ",\n" );
        json += string( indent, ' ' ) + "\"uniforms\": {\n";
        indent += 2;
        ShaderInstance::Procs sip;
        sip.Initialize( h.next );
        for( int i = 0; i < activeUniforms; i++ ) {
          GLchar name[80];
          GLsizei nameLen = 0;
          GLint count;
          GLenum type;
          RglGetActiveUniform( h.gl, prog, i, 80, &nameLen, &count, &type, name );
          name[nameLen] = 0;
          GLint loc = RglGetUniformLocation( h.gl, prog, name );
          json +=string( indent, ' ' ) + "\"" + string(name) + "\": {\n";
          indent += 2;
          json +=string( indent, ' ' ) + print_string( "\"location\": ", loc, ",\n" );
          json +=string( indent, ' ' ) + print_string( "\"type\": \"", Token::GLenumToString( type ), "\",\n" );
          if( count > 1 ) {
            json +=string( indent, ' ' ) + print_string( "\"count\" :", count, ",\n" );
          }
          json +=string( indent, ' ' ) + print_string( "\"value\": ", PrintUniformValue( sip, prog, loc, count, type, indent ), ",\n" );
          EraseLastComma( json );
          indent -= 2;
          json += string( indent, ' ' ) + "},\n";
        }
        EraseLastComma( json );
        indent -= 2;
        json += string( indent, ' ' ) + "},\n";
      }
      {
        char dbgLog[1<<15];
        int dbgLogLen = 0;
        RglGetProgramInfoLog( h.gl, prog, (1<<15) - 2, &dbgLogLen, dbgLog );
        dbgLog[ dbgLogLen ] = 0;
        if( dbgLogLen > 0 ) {
          json += string( indent, ' ' ) + print_string( "\"infoLog\": " );
          json += PrintMultiLine( dbgLog, dbgLogLen, indent );
        }
      }
      h.ReleaseAppContext();
      EraseLastComma( json );
      indent -= 2;
      json += "}\n";
      
      SendText( conn, "application/json", json );
    }
  }
  virtual string GetHandlerString() {
    return "program";
  }
};

struct ShaderHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    Http * http = GetInstanceByIndex( 0 );
    if( http == NULL ) {
      return;
    }
    Http & h = *http;
    string json;
    if( conn.path.size() == 1 ) {
      
      json += "[ ";
      for( set<GLuint>::iterator i = h.shader.begin(); i != h.shader.end(); ++i ) {
        json += print_string( *i, ", " );
      }
      EraseLastComma( json );
      json += "]\n";
      SendText( conn, "application/json", json );
    } else if ( conn.path.size() == 2 ) {
      string shaderNameString = conn.path[1];
      GLint shader = atoi( shaderNameString.c_str() );
      int indent = 0;
      json += "{\n";
      indent += 2;
      Http & h = *http;
      h.AcquireAppContext();
      json += string( indent, ' ' ) + "\"name\": " + shaderNameString + ",\n";
      GLint ival;
      RglGetShaderiv( h.gl, shader, GL_SHADER_TYPE, &ival );
      json += string( indent, ' ' ) + "\"GL_SHADER_TYPE\": \"" + Token::GLenumToString( ival ) + "\",\n";
      RglGetShaderiv( h.gl, shader, GL_DELETE_STATUS, &ival );
      json += string( indent, ' ' ) + "\"GL_DELETE_STATUS\": " + ( ival ? "true" : "false" ) + ",\n";
      RglGetShaderiv( h.gl, shader, GL_COMPILE_STATUS, &ival );
      json += string( indent, ' ' ) + "\"GL_COMPILE_STATUS\": " + ( ival ? "true" : "false" ) + ",\n";
      {
        char str[1<<15];
        int strLen = 0;
        RglGetShaderSource( h.gl, shader, sizeof(str) - 2, &strLen, str );
        str[ strLen ] = 0;
        if( strLen > 0 ) {
          json += string( indent, ' ' ) + print_string( "\"source\": " );
          json += PrintMultiLine( str, strLen, indent );
        }
        RglGetShaderInfoLog( h.gl, shader, sizeof(str) - 2, &strLen, str );
        str[ strLen ] = 0;
        if( strLen > 0 ) {
          json += string( indent, ' ' ) + print_string( "\"infoLog\": " );
          json += PrintMultiLine( str, strLen, indent );
        }
      }
      h.ReleaseAppContext();
      EraseLastComma( json );
      indent -= 2;
      json += "}\n";
      
      SendText( conn, "application/json", json );
    }
  }
  virtual string GetHandlerString() {
    return "shader";
  }
};

struct FboHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    Http * http = GetInstanceByIndex( 0 );
    if( http == NULL ) {
      return;
    }
    Http & h = *http;
    RegalContext * ctx = h.GetContext();
    if( conn.path.size() == 1 ) {
      string json;
      json += "[ ";
      if( ctx ) {
        Http & h = *http;
        for( map<GLuint, HttpFboInfo>::iterator it = h.fbo.begin(); it != h.fbo.end(); ++it ) {
          json += print_string( it->first, ", " );
        }
        EraseLastComma( json );
      }
      json += "]\n";
      
      SendText( conn, "application/json", json );
      return;
    }
    if( conn.path.size() == 2 ) {
      string redir = "/fbo/" + conn.path[1] + "/color0";
      Redirect( conn, redir );
      return;
    }
    if( conn.path.size() != 3 ) {
      // send 404
      return;
    }
    GLint fbo = atoi( conn.path[1].c_str() );
    
    if( ctx && h.fbo.count( fbo ) > 0 ) {
      ScopedContextAcquire sca( http );
      GLint currFbo = -1;
      RglGetIntegerv( h.gl, GL_READ_FRAMEBUFFER_BINDING, & currFbo );
      if( fbo != currFbo ) {
        SendText( conn, "application/json", "\"can't query the non-current FBO yet\"" );
        return;
      }
      HttpFboInfo & fi = h.fbo[ fbo ];
      
      RglFinish( h.gl );
      if( fbo == 0 ) {
        GLint vp[4];
        RglGetIntegerv( h.gl, GL_VIEWPORT, vp );
        fi.width = vp[0] + vp[2];
        fi.height = vp[1] + vp[3];
      }
      
      int stride = fi.width * 4;
      
      unsigned char * pixels = new unsigned char[ (fi.height+1) * stride ];
      
      RglReadPixels( h.gl, 0, 0, fi.width, fi.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
      for( int j = 0; j < fi.height/2; j++ ) {
        unsigned char * s = pixels + (fi.height -1 - j) * stride;
        unsigned char * d = pixels + j * stride;
        unsigned char * t = pixels + fi.height * stride;
        memcpy( t, d, stride );
        memcpy( d, s, stride );
        memcpy( s, t, stride );
      }
      int out_len = 0;
      unsigned char * img = stbi_write_png_to_mem(pixels, fi.width * 4, fi.width, fi.height, 4, &out_len );
      
      string http = print_string(
                                 "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: image/png\r\n"
                                 "Expires: Fri, 30 Oct 1998 14:19:41 GMT\r\n"
                                 "Cache-Control: no-cache, must-revalidate\r\n"
                                 "Content-Length: ", out_len, "\r\n"
                                 "\r\n");
      
      mg_write(conn.connection, http.c_str(), http.length() );
      mg_write(conn.connection, img, out_len );
      
      delete [] pixels;
      free( img );
      
      return;
    }
    
    SendText( conn, "application/json", "\"invalid context\"" );
  }
  
  virtual string GetHandlerString() {
    return "fbo";
  }
};

string EventCountToJson( Http::EventCount & count, int indent ) {
  string json;
  json += "{\n";
  indent += 2;
  json += string( indent, ' ' ) + "\"call\": " + print_string( count.call, ",\n" );
  json += string( indent, ' ' ) + "\"draw\": " + print_string( count.draw, ",\n" );
  json += string( indent, ' ' ) + "\"group\": " + print_string( count.group, ",\n" );
  json += string( indent, ' ' ) + "\"fbo\": " + print_string( count.fbo, ",\n" );
  json += string( indent, ' ' ) + "\"frame\": " + print_string( count.frame, ",\n" );
  json += string( indent, ' ' ) + "\"lastDraw\": " + print_string( count.lastDraw, ",\n" );
  json += string( indent, ' ' ) + "\"lastGroup\": " + print_string( count.lastGroup, ",\n" );
  json += string( indent, ' ' ) + "\"lastFbo\": " + print_string( count.lastFbo, ",\n" );
  json += string( indent, ' ' ) + "\"lastFrame\": " + print_string( count.lastFrame, ",\n" );
  indent -= 2;
  EraseLastComma( json );
  json += string( indent, ' ' ) + "}";
  return json;
}

string EscapeJson( const string & s ) {
  string r;
  for( size_t i = 0; i < s.size(); i++ ) {
    char c = s[i];
    switch( c ) {
      case '"':
        r.push_back( '\\' );
        break;
      default:
        break;
    }
    r.push_back( c );
  }
  return r;
}

struct LogHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    string json;
    GLint64 startLine = conn.path.size() > 1 ? atoi( conn.path[1].c_str() ) : 0;
    GLint64 numLines = conn.path.size() > 2 ? atoi( conn.path[2].c_str() ) : 10000;
    int indent = 0;
    json += string( indent, ' ' ) + "{\n";
    indent += 2;
    Http * http = GetInstanceByIndex( 0 );
    if( http ) {
      ScopedContextAcquire sca( http );
      Http & h = *http;
      json += string( indent, ' ' ) + "\"count\": " + EventCountToJson( h.count, indent ) + ",\n";
      GLint64 front = h.count.call - h.callLog.size() + 1;
      if( startLine < 0 ) {
        startLine = int(GLint64(h.count.call) + GLint64(startLine) + 1 );
      }
      startLine = max( startLine, front );
      numLines = min( numLines, GLint64( h.count.call - startLine + 1) );
      json += string( indent, ' ' ) + "\"startLine\": " + print_string( startLine, ",\n" );
      json += string( indent, ' ' ) + "\"numLines\": " + print_string( numLines, ",\n" );
      json += string( indent, ' ' ) + "\"maxLines\": " + print_string( h.callLog.size(), ",\n" );
      
      json += string( indent, ' ' ) + "\"log\": [\n";
      indent += 2;
      size_t base = size_t(startLine - front);
      for( size_t i = 0; i < numLines; i++ ) {
        string esc = EscapeJson( h.callLog[ base + i ] );
        json += string( indent, ' ' ) + print_string( "\"", esc, "\",\n");
      }
      indent -= 2;
      EraseLastComma( json );
      json += string( indent, ' ' ) + "],\n";
    }
    indent -= 2;
    EraseLastComma( json );
    json += string( indent, ' ' ) + "}\n";
    
    SendText( conn, "application/json", json );
  }
  virtual string GetHandlerString() {
    return "log";
  }
};

struct EnableHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    RegalAssert( conn.path.size() == 1 && conn.path[0] == "glEnable" );
    string body;
    if      (!strcmp("GL_LOG_INFO_REGAL",    conn.request_info->query_string)) Logging::enableError    = true;
    else if (!strcmp("GL_LOG_WARNING_REGAL", conn.request_info->query_string)) Logging::enableWarning  = true;
    else if (!strcmp("GL_LOG_ERROR_REGAL",   conn.request_info->query_string)) Logging::enableInfo     = true;
    else if (!strcmp("GL_LOG_APP_REGAL",     conn.request_info->query_string)) Logging::enableApp      = true;
    else if (!strcmp("GL_LOG_DRIVER_REGAL",  conn.request_info->query_string)) Logging::enableDriver   = true;
    else if (!strcmp("GL_LOG_INTERNAL_REGAL",conn.request_info->query_string)) Logging::enableInternal = true;
    else if (!strcmp("GL_LOG_HTTP_REGAL",    conn.request_info->query_string)) Logging::enableHttp     = true;
    
    else if (!strcmp("REGAL_FRAME_TIME",     conn.request_info->query_string)) Logging::frameTime      = true;
    
    else if (!strcmp("REGAL_MD5_COLOR",      conn.request_info->query_string)) Config::frameMd5Color    = true;
    else if (!strcmp("REGAL_MD5_STENCIL",    conn.request_info->query_string)) Config::frameMd5Stencil  = true;
    else if (!strcmp("REGAL_MD5_DEPTH",      conn.request_info->query_string)) Config::frameMd5Depth    = true;
    else if (!strcmp("REGAL_SAVE_COLOR",     conn.request_info->query_string)) Config::frameSaveColor   = true;
    else if (!strcmp("REGAL_SAVE_STENCIL",   conn.request_info->query_string)) Config::frameSaveStencil = true;
    else if (!strcmp("REGAL_SAVE_DEPTH",     conn.request_info->query_string)) Config::frameSaveDepth   = true;
    
    body += print_string("glEnable(", conn.request_info->query_string, ");",br,br);
    body += print_string("<a href=\"/glDisable?",conn.request_info->query_string,"\">toggle</a>");
    SendHTML( conn, body );
  }
  virtual string GetHandlerString() {
    return "glEnable";
  }
};

struct DisableHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    RegalAssert( conn.path.size() == 1 && conn.path[0] == "glEnable" );
    string body;
    if      (!strcmp("GL_LOG_INFO_REGAL",    conn.request_info->query_string)) Logging::enableError    = false;
    else if (!strcmp("GL_LOG_WARNING_REGAL", conn.request_info->query_string)) Logging::enableWarning  = false;
    else if (!strcmp("GL_LOG_ERROR_REGAL",   conn.request_info->query_string)) Logging::enableInfo     = false;
    else if (!strcmp("GL_LOG_APP_REGAL",     conn.request_info->query_string)) Logging::enableApp      = false;
    else if (!strcmp("GL_LOG_DRIVER_REGAL",  conn.request_info->query_string)) Logging::enableDriver   = false;
    else if (!strcmp("GL_LOG_INTERNAL_REGAL",conn.request_info->query_string)) Logging::enableInternal = false;
    else if (!strcmp("GL_LOG_HTTP_REGAL",    conn.request_info->query_string)) Logging::enableHttp     = false;
    
    else if (!strcmp("REGAL_FRAME_TIME",     conn.request_info->query_string)) Logging::frameTime      = false;
    
    else if (!strcmp("REGAL_MD5_COLOR",      conn.request_info->query_string)) Config::frameMd5Color    = false;
    else if (!strcmp("REGAL_MD5_STENCIL",    conn.request_info->query_string)) Config::frameMd5Stencil  = false;
    else if (!strcmp("REGAL_MD5_DEPTH",      conn.request_info->query_string)) Config::frameMd5Depth    = false;
    else if (!strcmp("REGAL_SAVE_COLOR",     conn.request_info->query_string)) Config::frameSaveColor   = false;
    else if (!strcmp("REGAL_SAVE_STENCIL",   conn.request_info->query_string)) Config::frameSaveStencil = false;
    else if (!strcmp("REGAL_SAVE_DEPTH",     conn.request_info->query_string)) Config::frameSaveDepth   = false;
    
    body += print_string("glDisable(", conn.request_info->query_string, ");",br,br);
    body += print_string("<a href=\"/glEnable?",conn.request_info->query_string,"\">toggle</a>");
    SendHTML( conn, body );
  }
  virtual string GetHandlerString() {
    return "glDisable";
  }
};

struct DebugHandler : public RequestHandler {
  virtual void HandleRequest( Connection & conn ) {
    RegalAssert( conn.path.size() >= 1 && conn.path[0] == "debug" );
    bool waitForPause = false;
    if( conn.path.size() == 2 ) {
      const string & cmd = conn.path[1];
      Http * http = GetInstanceByIndex( 0 );
      if( http ) {
        waitForPause = true;
        Http & h = *http;
        h.AcquireAppContext();
        if( cmd == "play" ) {
          h.ContinueFromBreakpoint( RS_Run );
          waitForPause = false;
        } else if( cmd == "next" ) {
          h.ContinueFromBreakpoint( RS_Next );
        } else if( cmd == "nextDraw" ) {
          h.ContinueFromBreakpoint( RS_NextDraw );
        } else if( cmd == "nextFbo" ) {
          h.ContinueFromBreakpoint( RS_NextFbo );
        } else if( cmd == "nextGroup" ) {
          h.ContinueFromBreakpoint( RS_NextGroup );
        } else if( cmd == "nextFrame" ) {
          h.ContinueFromBreakpoint( RS_NextFrame );
        } else if( cmd == "begin" && h.runState != RS_Pause ) {
          h.ContinueFromBreakpoint( RS_NextFrame );
        }
        h.ReleaseAppContext();
      }
    }
    
    if( waitForPause ) {
      Http * http = GetInstanceByIndex( 0 );
      // wait up to 16 seconds for the frame to finish
      for( int i = 0; i < 16000; i++ ) {
        if( http->runState == RS_Pause ) {
          break;
        }
#if ! REGAL_SYS_WGL
        usleep( 1000 );
#else
		Sleep(1);
#endif
      }
    }
    
    
    string body;
    body += "<a href=\"/debug/continue\">continue</a><br>\n";
    body += "<a href=\"/debug/pause\">pause</a><br>\n";
    body += "<a href=\"/debug/next\">next</a><br>\n";
    body += "<a href=\"/debug/nextDraw\">next draw</a><br>\n";
    body += "<a href=\"/debug/nextFbo\">next fbo</a><br>\n";
    body += "<a href=\"/debug/nextGroup\">next group</a><br>\n";
    SendHTML( conn, body );
  }
  virtual string GetHandlerString() {
    return "debug";
  }
};



void CreateHandlers() {
  RequestHandler *h[] = { new DebugHandler, new TextureHandler, new ContextsHandler, new FboHandler, new ProgramHandler, new ShaderHandler, new LogHandler, new EnableHandler, new DisableHandler, new FaviconHandler, new ScriptHandler };
  for( int i = 0; i < sizeof(h)/sizeof(h[0]); i++ ) {
    handlers[ h[i]->GetHandlerString() ] = h[i];
  }
}

void Redirect( Connection & conn, const string & redirect_to ) {
  string html = print_string(
                             "<html><body>\nRedirecting to: ",
                             redirect_to,
                             "</body></html>\n");
  
  string http = print_string(
                             "HTTP/1.0 302 Found\r\n"
                             "Location: ", redirect_to, "\r\n"
                             "Content-Type: text/html\r\n"
                             "Content-Length: ", html.length(), "\r\n"
                             "\r\n",
                             html);
  
  mg_write( conn.connection, http.c_str(), http.length() );
}


static void HttpInit()
{
#if REGAL_SYS_OSX
  pp.CGLSetCurrentContext = dispatchGlobal.CGLSetCurrentContext;
#elif REGAL_SYS_WGL
  pp.wglMakeCurrent = dispatchGlobal.wglMakeCurrent;
#elif REGAL_SYS_GLX
  pp.glXMakeCurrent = dispatchGlobal.glxMakeCurrent;
#elif REGAL_SYS_EGL
  pp.eglMakeCurrent = dispatchGlobal.eglMakeCurrent;
#endif
  
  Internal("Http::Init","()");
  
  // Environment variable HTTP configuration
  
#ifndef REGAL_NO_GETENV
  getEnv("REGAL_HTTP", enabled);
  getEnv("REGAL_HTTP_PORT", port);
#endif
  
  // Compile-time HTTP configuration
  
#ifdef REGAL_HTTP_PORT
  port = REGAL_HTTP_PORT;
#endif
  
  CreateHandlers();
  
}



int instanceCount;
map<int, Http *> instances;

Thread::Mutex instCountMutex( Thread::MT_Normal );

Http * GetInstanceByIndex( int idx ) {
  Thread::ScopedLock sl( &instCountMutex );
  if( instances.count( idx ) == 0 ) {
    return NULL;
  }
  return instances[ idx ];
}

int addInstance( Http * inst ) {
  Thread::ScopedLock sl( &instCountMutex );
  if( inst == NULL ) {
    return -1;
  }
  int c = instanceCount;
  instanceCount++;
  instances[ c ] = inst;
  return c;
}

void delInstance( int idx ) {
  Thread::ScopedLock sl( &instCountMutex );
  if( instances.count( idx ) == 0 ) {
    return;
  }
  instances.erase( idx );
}

Http::Http( RegalContext * ctx ) : Layer( ctx ), runState( RS_Run ), debugGroupStackDepth( -1 ), stepOverGroupDepth( -1 ), inBeginEnd( 0 )
{
  instanceNum = addInstance( this );
  contextMutex = new Thread::Mutex( Thread::MT_Normal );
  contextMutex->acquire();
  breakpointMutex = new Thread::Mutex( Thread::MT_Normal );
  breakpointMutex->acquire();
  httpServerWantsContext = false;
  fbo[0] = HttpFboInfo();
  /*
   Breakpoint * b = new Breakpoint;
   b->SetRegularExpression( "(glDraw.*|glEnd)" );
   breakpoint.push_back( b );
   currentBreakpoint = -1;
   */
}

Http::~Http()
{
  for( size_t i = 0; i < breakpoint.size(); i++ ) {
    delete breakpoint[i];
  }
  delete contextMutex;
  delete breakpointMutex;
}

void Http::YieldToHttpServer( bool log )
{
  RegalContext * ctx = GetContext();
  if( log ) {
    callLog.push_back( callString );
    static GLuint64 sz = 0;
    sz = max( sz, ( count.call - count.lastFrame + 1 ) );
    if( callLog.size() > sz ) {
      callLog.erase( callLog.begin(), callLog.begin() + size_t( callLog.size() - sz  ) );
    }
  }
  
  // invoked by the app thread from the http dispatch
  if( runState == RS_Pause && inBeginEnd == 0 ) {
    ctx->parkContext( pp );
    contextMutex->release();
    breakpointMutex->acquire(); // this will stall until the server releases the breakpoint
    breakpointMutex->release();
    contextMutex->acquire();
    ctx->unparkContext( pp );
  }
  
  /*
   // check any breakpoints for hits
   for( size_t i = 0; i < breakpoint.size(); i++ ) {
   Breakpoint * b = breakpoint[i];
   if( b == NULL || b->re == NULL || b->enabled == false ) {
   continue;
   }
   // get the string for this API call
   string callString;
   int subStr[30];
   int reRet = pcre_exec( b->re, NULL, callString.c_str(), (int)callString.size(), 0, 0, subStr, sizeof( subStr ) / sizeof( subStr[0] ) );
   if( reRet ) {
   if( inBeginEnd ) {
   runState = RS_Pause; // stop asap, but not right now, otherwise we have problems with other queries
   } else {
   currentBreakpoint = (int)i;
   ctx->parkContext( *dispatcherGlobal.http.next() );
   contextMutex->release();
   breakpointMutex->acquire(); // this will stall until the server releases the breakpoint
   breakpointMutex->release();
   contextMutex->acquire();
   ctx->unparkContext( *dispatcherGlobal.http.next() );
   }
   }
   }
   */
  currentBreakpoint = -1;
  if( httpServerWantsContext && inBeginEnd == 0 ) {
    httpServerWantsContext = false;
    ctx->parkContext( pp );
    contextMutex->release();
    // wait a bit?
    contextMutex->acquire();
    ctx->unparkContext( pp );
  }
  if( log ) {
    count.call++;
  }
}

void Http::AcquireAppContext()
{
  httpServerWantsContext = true;
  contextMutex->acquire();
  GetContext()->unparkContext( pp );
}

void Http::ReleaseAppContext()
{
  GetContext()->parkContext( pp );
  contextMutex->release();
}

void Http::ContinueFromBreakpoint( HttpRunState rs )
{
	if( runState == RS_Pause ) {
    breakpointMutex->release();
    breakpointMutex->acquire();
  }
  runState = rs;
}

void Http::GlProcs::Initialize( Dispatch::GL * tbl ) {
  glFinish                        = tbl->glFinish ;
  glGetActiveUniform              = tbl->glGetActiveUniform ;
  glGetAttachedShaders            = tbl->glGetAttachedShaders ;
  glGetIntegerv                   = tbl->glGetIntegerv ;
  glGetProgramInfoLog             = tbl->glGetProgramInfoLog ;
  glGetProgramiv                  = tbl->glGetProgramiv ;
  glGetShaderInfoLog              = tbl->glGetShaderInfoLog ;
  glGetShaderSource               = tbl->glGetShaderSource ;
  glGetShaderiv                   = tbl->glGetShaderiv ;
  glGetTextureImageEXT            = tbl->glGetTextureImageEXT ;
  glGetTextureLevelParameterfvEXT = tbl->glGetTextureLevelParameterfvEXT ;
  glGetTextureParameterfvEXT      = tbl->glGetTextureParameterfvEXT ;
  glGetUniformLocation            = tbl->glGetUniformLocation ;
  glReadPixels                    = tbl->glReadPixels ;
}

REGAL_NAMESPACE_END

#endif
