/*
  Copyright (c) 2011-2012 NVIDIA Corporation
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

#include "md5.h"

#ifndef REGAL_NO_PNG
#include <zlib.h>
#include <png.h>
#include <string>
#include <boost/print/print_string.hpp>
using namespace ::std;
using ::boost::print::print_string;
#endif

#include "RegalMarker.h"
#include "RegalPrivate.h"
#include "RegalDispatcher.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

void Marker::FrameTerminator(RegalContext &context)
{
  Internal("Regal::Marker::FrameTerminator ",&context);

  if (Logging::frameTime)
  {
    Timer::Value elapsed = context.frameTimer.restart();
    Info("Frame ",context.frame,' ',elapsed/1000," msec, ",1000000.0/elapsed," FPS.");
  }

  if
  (
    Config::frameMd5Color    ||
    Config::frameMd5Stencil  ||
    Config::frameMd5Depth    ||
    Config::frameSaveColor   ||
    Config::frameSaveStencil ||
    Config::frameSaveDepth
  )
  {
    DispatchTable *_next = &context.dispatcher.front();
    RegalAssert(_next);

    // Get the current viewport

    GLint viewport[4];
    memset(viewport,0,sizeof(viewport));
    _next->call(&_next->glGetIntegerv)(GL_VIEWPORT, viewport);

    const GLint &width  = viewport[2];
    const GLint &height = viewport[3];

    if (width>0 && height>0)
    {
      if (Config::frameMd5Color || Config::frameSaveColor)
      {
        // Read the pixels as RGBA

        const GLint bufferSize = width*height*4*sizeof(GLbyte);
        GLbyte *buffer = (GLbyte *) calloc(bufferSize,1);

       _next->call(&_next->glReadPixels)(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        // Do once we have the pixels, could we do the rest in another
        // thread?

        if (Config::frameMd5Color)
        {
          // Compute pixel md5sum

          MD5Context md5c;
          MD5Init(&md5c);
          MD5Update(&md5c, buffer, bufferSize);

          unsigned char digest[16];
          MD5Final(digest, &md5c);

          // Convert md5sum to string

          char md5sum[sizeof(digest)*2+1];
          for (size_t i=0; i<sizeof(digest); ++i)
            sprintf(md5sum+i*2,"%02x",digest[i]);

          Info("Color md5sum ",md5sum);
        }

#ifndef REGAL_NO_PNG
        if (Config::frameSaveColor)
        {
          static png_color_8 pngSBIT = {8, 8, 8, 0, 8};

          string filename = print_string("color_",boost::print::right(context.frame,3,'0'),".png");
          FILE *fp = fopen(filename.c_str(), "wb");
          if (fp)
          {
            png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
            if (pngPtr)
            {
              png_infop pngInfo = png_create_info_struct(pngPtr);
              if (pngInfo)
              {
                png_init_io(pngPtr, fp);

                // Z_NO_COMPRESSION, Z_BEST_SPEED, Z_BEST_COMPRESSION,
                // Z_DEFAULT_COMPRESSION

                png_set_compression_level(pngPtr, Z_BEST_COMPRESSION);
                png_set_IHDR(pngPtr, pngInfo, width, height,
                             8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                             PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
                png_set_pHYs(pngPtr, pngInfo, 72 * 40, 72 * 40, PNG_RESOLUTION_METER);
                png_set_sBIT(pngPtr, pngInfo, &pngSBIT);
                png_write_info(pngPtr, pngInfo);
                for (int y = height; y--; )
                  png_write_row(pngPtr, reinterpret_cast<png_byte *>(buffer + y * width * 4));
                png_write_end(pngPtr, pngInfo);
              }
              png_destroy_write_struct(&pngPtr, &pngInfo);
            }
            fclose(fp);
          }
        }
#endif
        free(buffer);
      }
    }
  }

  // Exit from the application if Config::frameLimit is reached.

  if ( Config::frameLimit>0 && context.frame>=size_t(Config::frameLimit))
    exit(0);
}

REGAL_NAMESPACE_END
