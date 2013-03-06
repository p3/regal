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

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <map>

#include "RegalPixelConversions.h"

REGAL_GLOBAL_END


REGAL_NAMESPACE_BEGIN

namespace Conversion {

#include "RegalPixelConversions.inl"

// ===========================================================================
// Conversion registry.
//
// Maps an OpenGL format identified by a (format, type) pair to the
// functionality needed to convert that format.
// ===========================================================================

struct RegistryBase
{
  typedef std::map < std::pair < GLenum, GLenum >, IConversion* > ConversionMap;
  static ConversionMap conversionMap;

  static inline IConversion *Lookup(GLenum format, GLenum type)
  {
    ConversionMap::const_iterator i = conversionMap.find( std::make_pair( format, type ) );
    if (i == RegistryBase::conversionMap.end())
      return NULL;
    return i->second;
  }
};

RegistryBase::ConversionMap RegistryBase::conversionMap;

template < GLenum format, GLenum type, typename T >
struct Registry : public RegistryBase, public IConversion
{
  enum {FORMAT=format, TYPE=type};

  Registry()
  {
    conversionMap[ std::make_pair( format, type ) ] = this;
  }

  virtual void Unpack32( const void* src, uint32_t* dst, size_t cnt ) const
  {
    return T::Unpack32( src, dst, cnt );
  }

  virtual void Pack32( const uint32_t* src, void* dst, size_t cnt ) const
  {
    return T::Pack32( src, dst, cnt );
  }

  virtual size_t GetPackedPixelByteSize()
  {
    return T::PACKED_BYTES;
  }

  virtual size_t GetPackedPixelAlignmentSize()
  {
    return T::ALIGNMENT;
  }

  virtual size_t GetPackedPixelComponents()
  {
    return T::COMPONENT_COUNT;
  }
};

// ===========================================================================
// Instantiate a class to register each conversion we support. This also
// generates the code for each conversion.
//
// Note: The list below is just what is needed for GLES 2.0, and can be easily
// and automatically extended by just adding new instances to describe the
// format.
//
// Note: The bit-packed pixel types use an component ordering (such as
// GL_UNSIGNED_SHORT_5_6_5 with red in the highest bits) which is different
// than that used by the non-bit-packed pixel types (such as GL_UNSIGNED_BYTE,
// with red in the lowest bits). When reading the non-bit-packed pixels from.
// memory, they are read as part of a larger native integer type, and since red
// is first in memory, red ends up in the lowest bits on a little endian
// machine.
//
// TODO: Clean up the apparent confusion in the component ordering. The GL
// specification calls for red (or whatever the first component is) in the
// highest bits in all the bit-packed formats.
//
// TODO: Support big endian ordering, doing so in a way which allows
// glPixelStore(GL_UNPACK_SWAP_BYTES, GL_TRUE) to be handled.
// ===========================================================================

// 8bpp
Registry<GL_ALPHA,           GL_UNSIGNED_BYTE,          Pixel<uint8_t,  1,      0,      0,      0,   0xff>     > a_8_;
Registry<GL_LUMINANCE,       GL_UNSIGNED_BYTE,          Pixel<uint8_t,  1,   0xff,      0,      0,      0>     > l_8_;

// 15bpp
Registry<GL_RGBA,            GL_RGB5,                   Pixel<uint16_t, 2, 0x001f, 0x03e0, 0x7c00,      0>     > rgb_555_;

// 16bpp
Registry<GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,          Pixel<uint16_t, 2, 0x00ff,      0,      0, 0xff00>     > la_88_;
Registry<GL_RGB,             GL_UNSIGNED_SHORT_5_6_5,   Pixel<uint16_t, 2, 0xf800, 0x07e0, 0x001f,      0>     > rgb_565_;
Registry<GL_RGBA,            GL_UNSIGNED_SHORT_4_4_4_4, Pixel<uint16_t, 2, 0xf000, 0x0f00, 0x00f0, 0x000f>     > rgba_4444_;
Registry<GL_RGBA,            GL_UNSIGNED_SHORT_5_5_5_1, Pixel<uint16_t, 2, 0xf800, 0x07c0, 0x003e, 0x0001>     > rgba_5551_;

// 24bpp (needs some special handling)
Registry<GL_RGB,             GL_UNSIGNED_BYTE,          PixelAny<uint24_t, 3, 0x0000ff, 0x00ff00, 0xff0000, 0> > rgb_888_;

// 32bpp
Registry<GL_RGBA,            GL_UNSIGNED_BYTE,          Pixel<uint32_t, 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000> > rgba_8888_;

}  // namespace

IConversion *GetConversionInterface(GLenum format, GLenum type)
{
  return Conversion::RegistryBase::Lookup(format, type);
}

REGAL_NAMESPACE_END
