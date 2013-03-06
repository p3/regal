/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2012 Google Inc
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

 Regal Pixel Conversion Functions
 Lloyd Pique

 */

#ifndef __REGAL_PIXEL_CONVERSIONS_H__
#define __REGAL_PIXEL_CONVERSIONS_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <boost/cstdint.hpp>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

using ::boost::uint8_t;
using ::boost::uint16_t;
using ::boost::uint32_t;

// The IConversion interface hides the implementation details of the conversion
// module behind a standard interface.
class IConversion
{
 public:
  // Unpacks pixel data from an arbitrary format into a destination RGBA_8888
  // format.
  virtual void Unpack32 ( const void*     src, uint32_t* dst, size_t cnt ) const = 0;

  // Packs the pixel data from a source RGBA_8888 format into an arbitary
  // format.
  virtual void Pack32   ( const uint32_t* src, void*     dst, size_t cnt ) const = 0;

  // Utility function. Gets the size in bytes of a packed pixel in the format
  // the interface deals with.
  virtual size_t GetPackedPixelByteSize() = 0;

  // Utility function. Gets the size in bytes
  virtual size_t GetPackedPixelAlignmentSize() = 0;

  // Utility function. Gets the number of components in each pixel. For example
  // a luminosity only texture would have 1, an RGB texture would have three,
  // and an RGBA texture would have four.
  virtual size_t GetPackedPixelComponents() = 0;

 protected:
  // The conversion interfaces are actually static. There is no need to destroy
  // them.
  virtual ~IConversion() {};
};

// Gets the conversion interface for the given OpenGL format and type values,
// or NULL if there is no conversion defined.
// Note that there is no need to delete or otherwise free the interface.
IConversion *GetConversionInterface(GLenum format, GLenum type);

REGAL_NAMESPACE_END

#endif // ! __REGAL_PIXEL_CONVERSIONS_H__
