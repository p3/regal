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
#include "RegalHttp.h"

#if REGAL_NO_HTTP

REGAL_NAMESPACE_BEGIN

namespace Http
{
  void Init()  {}
  void Start() {}
  void Stop()  {}
}

REGAL_NAMESPACE_END

#else

REGAL_GLOBAL_BEGIN

#include "RegalPpa.h"
#include "RegalLog.h"
#include "RegalThread.h"
#include "RegalFavicon.h"
#include "RegalContextInfo.h"

#include <map>

#include <string>
#include <cstdio>
#include <cstdarg>
using namespace std;

#include <boost/print/print_string.hpp>
using boost::print::print_string;

#include "mongoose.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

extern map<Thread::Thread, RegalContext *> th2rc;

namespace Http
{
  bool enabled = !REGAL_NO_HTTP;  // Enabled by default
  int  port    = REGAL_HTTP_PORT; // HTTP listening port - 8080 by default
  mg_context *ctx = NULL;         // Mongoose context

  void Init()
  {
    Internal("Http::Init","()");

    // Environment variable HTTP configuration

    #ifndef REGAL_NO_GETENV
    const char *n = GetEnv("REGAL_NO_HTTP");
    if (n) enabled = !atoi(n);

    const char *p = GetEnv("REGAL_HTTP_PORT");
    if (p) port = atoi(p);
    #endif

    // Compile-time HTTP configuration

    #ifdef REGAL_HTTP_PORT
    port = REGAL_HTTP_PORT;
    #endif
  }

  const char * const br = "<br/>\n";

  static void *callback(enum mg_event event, struct mg_connection *conn)
  {
    const struct mg_request_info *request_info = mg_get_request_info(conn);

    switch (event)
    {
      case MG_EVENT_LOG:
      {
        HTrace(mg_get_log_message(conn));
        break;
      }

      case MG_NEW_REQUEST:
      {
        HTrace(request_info->request_method ? request_info->request_method : "", " ",
               request_info->uri            ? request_info->uri            : "",
               request_info->query_string   ?                        "?"   : "",
               request_info->query_string   ? request_info->query_string   : "");

        if (!strcmp("/favicon.ico",request_info->uri))
        {
          string http = print_string(
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: image/x-icon\r\n"
                    "Content-Length: ", sizeof(favicon), "\r\n"
                    "\r\n");

          mg_write(conn,http.c_str(),http.length());
          mg_write(conn,favicon,sizeof(favicon));
          return (void *) true;
        }

        string body;

        if (!strcmp("/log",request_info->uri))
        {
          if (Logging::buffer)
            for (list<string>::const_iterator i = Logging::buffer->begin(); i!=Logging::buffer->end(); ++i)
              body += print_string(*i,br);
        }
        else if (!strcmp("/glEnable",request_info->uri))
        {
               if (!strcmp("GL_LOG_INFO_REGAL",    request_info->query_string)) Logging::enableError    = true;
          else if (!strcmp("GL_LOG_WARNING_REGAL", request_info->query_string)) Logging::enableWarning  = true;
          else if (!strcmp("GL_LOG_ERROR_REGAL",   request_info->query_string)) Logging::enableInfo     = true;
          else if (!strcmp("GL_LOG_APP_REGAL",     request_info->query_string)) Logging::enableApp      = true;
          else if (!strcmp("GL_LOG_DRIVER_REGAL",  request_info->query_string)) Logging::enableDriver   = true;
          else if (!strcmp("GL_LOG_INTERNAL_REGAL",request_info->query_string)) Logging::enableInternal = true;
          else if (!strcmp("GL_LOG_HTTP_REGAL",    request_info->query_string)) Logging::enableHttp     = true;

          body += print_string("glEnable(", request_info->query_string, ");",br,br);
          body += print_string("<a href=\"/glDisable?",request_info->query_string,"\">toggle</a>");
        }
        else if (!strcmp("/glDisable",request_info->uri))
        {
               if (!strcmp("GL_LOG_INFO_REGAL",    request_info->query_string)) Logging::enableError    = false;
          else if (!strcmp("GL_LOG_WARNING_REGAL", request_info->query_string)) Logging::enableWarning  = false;
          else if (!strcmp("GL_LOG_ERROR_REGAL",   request_info->query_string)) Logging::enableInfo     = false;
          else if (!strcmp("GL_LOG_APP_REGAL",     request_info->query_string)) Logging::enableApp      = false;
          else if (!strcmp("GL_LOG_DRIVER_REGAL",  request_info->query_string)) Logging::enableDriver   = false;
          else if (!strcmp("GL_LOG_INTERNAL_REGAL",request_info->query_string)) Logging::enableInternal = false;
          else if (!strcmp("GL_LOG_HTTP_REGAL",    request_info->query_string)) Logging::enableHttp     = false;

          body += print_string("glDisable(", request_info->query_string, ");",br,br);
          body += print_string("<a href=\"/glEnable?",request_info->query_string,"\">toggle</a>");
        }
        else
        {
          for (map<Thread::Thread, RegalContext *>::const_iterator i = th2rc.begin(); i!=th2rc.end(); ++i)
          {
            RegalContext *ctx = i->second;

            // Need a per-context read-lock?

            body += print_string("ctx = ",ctx,br);
            body += br;
            if (ctx)
            {
              if (ctx->info)
              {
                body += print_string("<b>Vendor     </b>:",ctx->info->regalVendor,br);
                body += print_string("<b>Renderer   </b>:",ctx->info->regalRenderer,br);
                body += print_string("<b>Version    </b>:",ctx->info->regalVersion,br);
                body += print_string("<b>Extensions </b>:",ctx->info->regalExtensions,br);
                body += br;
              }

#if REGAL_EMULATION
              if (ctx->ppa)
              {
                body += print_string("<b>GL_STENCIL_BIT</b><br/>",ctx->ppa->State::Stencil::toString(br),br);
                body += print_string("<b>GL_DEPTH_BIT</b><br/>",  ctx->ppa->State::Depth::toString(br),br);
                body += print_string("<b>GL_POLYGON_BIT</b><br/>",ctx->ppa->State::Polygon::toString(br),br);
                body += br;
              }
#endif
            }
          }
        }

        string html = print_string(
          "<html><body>\n",
          body,
          "</body></html>\n");

        string http = print_string(
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/html\r\n"
                  "Content-Length: ", html.length(), "\r\n"
                  "\r\n",
                  html);

        mg_write(conn,http.c_str(),http.length());
        break;
      }

      default:
        return NULL;
    }

    return (void *) true;  // Mark as handled for Mongoose
  }

  void Start()
  {
    Internal("Http::Start","()");

    if (enabled && !ctx)
    {
      // Try listening on the configured port number (8080 by default)
      // and advance foward until one is available

      for (int i = 0; i<100; ++i)
      {
        string j = print_string(port+i);
        const char *options[3] = { "listening_ports", j.c_str(), NULL};
        ctx = mg_start(&callback, NULL, options);
        if (ctx)
          break;
      }

      const char *option = NULL;
      if (ctx)
        option = mg_get_option(ctx,"listening_ports");

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

  void Stop()
  {
    Internal("Http::Stop","()");

    if (ctx)
    {
      HTrace("Closing HTTP connections.");

      // Currently there is a problem with shutting down mongoose
      // on Windows - so just skip the cleanup for now.

      #ifndef REGAL_SYS_WGL
      mg_stop(ctx);
      #endif

      ctx = NULL;
    }
  }

}

REGAL_NAMESPACE_END

#endif

