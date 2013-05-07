/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  Copyright (c) 2013 Google Inc
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

#include "gtest/gtest.h"

#include <GL/Regal.h>

#include "RegalPixelConversions.h"

namespace {

using Regal::IConversion;
using Regal::GetConversionInterface;

#include "RegalPixelConversions.inl"

TEST( RegalPixelConversions, StaticLZC ) {
  EXPECT_EQ( 0, StaticLZC<0>::value );
  EXPECT_EQ( 0, StaticLZC<1>::value );
  EXPECT_EQ( 1, StaticLZC<2>::value );
  EXPECT_EQ( 0, StaticLZC<3>::value );
  EXPECT_EQ( 2, StaticLZC<4>::value );
  EXPECT_EQ( 0, StaticLZC<5>::value );
  EXPECT_EQ( 1, StaticLZC<6>::value );
  EXPECT_EQ( 0, StaticLZC<7>::value );
  EXPECT_EQ( 3, StaticLZC<8>::value );
}

TEST( RegalPixelConversions, StaticIsPow2 ) {
  EXPECT_EQ( 1, StaticIsPow2<0>::value );  // Well....

  EXPECT_EQ( 1, StaticIsPow2<1>::value );
  EXPECT_EQ( 1, StaticIsPow2<2>::value );
  EXPECT_EQ( 0, StaticIsPow2<3>::value );
  EXPECT_EQ( 1, StaticIsPow2<4>::value );
  EXPECT_EQ( 0, StaticIsPow2<5>::value );
  EXPECT_EQ( 0, StaticIsPow2<6>::value );
  EXPECT_EQ( 0, StaticIsPow2<7>::value );
  EXPECT_EQ( 1, StaticIsPow2<8>::value );
}

TEST( RegalPixelConversions, Component ) {
  // Verify the properties computed from the mask for a demonstrative 5-bit
  // component.
  EXPECT_EQ( 0x1f0, Component<0x1f0>::COMPONENT_MASK );
  EXPECT_EQ( 4,     Component<0x1f0>::LEADING_BIT_COUNT );
  EXPECT_EQ( 5,     Component<0x1f0>::COMPONENT_BIT_COUNT );

  // A zero bit component should only ever pack/unpack to zero.
  EXPECT_EQ( 0x00u, Component<0>::u8(  0 ) );
  EXPECT_EQ( 0x00u, Component<0>::u8( ~0 ) );
  EXPECT_EQ( 0x00u, Component<0>::p8(  0 ) );
  EXPECT_EQ( 0x00u, Component<0>::p8( ~0 ) );

  // A one bit component should unpack to all zeros or all ones.
  EXPECT_EQ( 0x00u, Component<0x100>::u8( 0x000 ) );
  EXPECT_EQ( 0xffu, Component<0x100>::u8( 0x100 ) );

  // A one bit component should pack to a single bit.
  EXPECT_EQ( 0x000u, Component<0x100>::p8( 0x00 ) );
  EXPECT_EQ( 0x000u, Component<0x100>::p8( 0x7f ) );
  EXPECT_EQ( 0x100u, Component<0x100>::p8( 0x80 ) );
  EXPECT_EQ( 0x100u, Component<0x100>::p8( 0xff ) );

  // Test unpacking for all values of an demonstrative 5-bit component, to show
  // proper expansion to a larger bit size.
  EXPECT_EQ( 0x00u, Component<0x1f0>::u8( 0x000 ) );
  EXPECT_EQ( 0x08u, Component<0x1f0>::u8( 0x010 ) );
  EXPECT_EQ( 0x10u, Component<0x1f0>::u8( 0x020 ) );
  EXPECT_EQ( 0x18u, Component<0x1f0>::u8( 0x030 ) );
  EXPECT_EQ( 0x21u, Component<0x1f0>::u8( 0x040 ) );
  EXPECT_EQ( 0x29u, Component<0x1f0>::u8( 0x050 ) );
  EXPECT_EQ( 0x31u, Component<0x1f0>::u8( 0x060 ) );
  EXPECT_EQ( 0x39u, Component<0x1f0>::u8( 0x070 ) );
  EXPECT_EQ( 0x42u, Component<0x1f0>::u8( 0x080 ) );
  EXPECT_EQ( 0x4au, Component<0x1f0>::u8( 0x090 ) );
  EXPECT_EQ( 0x52u, Component<0x1f0>::u8( 0x0a0 ) );
  EXPECT_EQ( 0x5au, Component<0x1f0>::u8( 0x0b0 ) );
  EXPECT_EQ( 0x63u, Component<0x1f0>::u8( 0x0c0 ) );
  EXPECT_EQ( 0x6bu, Component<0x1f0>::u8( 0x0d0 ) );
  EXPECT_EQ( 0x73u, Component<0x1f0>::u8( 0x0e0 ) );
  EXPECT_EQ( 0x7bu, Component<0x1f0>::u8( 0x0f0 ) );
  EXPECT_EQ( 0x84u, Component<0x1f0>::u8( 0x100 ) );
  EXPECT_EQ( 0x8cu, Component<0x1f0>::u8( 0x110 ) );
  EXPECT_EQ( 0x94u, Component<0x1f0>::u8( 0x120 ) );
  EXPECT_EQ( 0x9cu, Component<0x1f0>::u8( 0x130 ) );
  EXPECT_EQ( 0xa5u, Component<0x1f0>::u8( 0x140 ) );
  EXPECT_EQ( 0xadu, Component<0x1f0>::u8( 0x150 ) );
  EXPECT_EQ( 0xb5u, Component<0x1f0>::u8( 0x160 ) );
  EXPECT_EQ( 0xbdu, Component<0x1f0>::u8( 0x170 ) );
  EXPECT_EQ( 0xc6u, Component<0x1f0>::u8( 0x180 ) );
  EXPECT_EQ( 0xceu, Component<0x1f0>::u8( 0x190 ) );
  EXPECT_EQ( 0xd6u, Component<0x1f0>::u8( 0x1a0 ) );
  EXPECT_EQ( 0xdeu, Component<0x1f0>::u8( 0x1b0 ) );
  EXPECT_EQ( 0xe7u, Component<0x1f0>::u8( 0x1c0 ) );
  EXPECT_EQ( 0xefu, Component<0x1f0>::u8( 0x1d0 ) );
  EXPECT_EQ( 0xf7u, Component<0x1f0>::u8( 0x1e0 ) );
  EXPECT_EQ( 0xffu, Component<0x1f0>::u8( 0x1f0 ) );

  // An eight bit component should pack/unpack as itself.
  for ( size_t i = 0; i < 255; ++i ) {
    EXPECT_EQ( i, Component<0xff0>::u8( i * 16 ) );
    EXPECT_EQ( i * 16u, Component<0xff0>::p8( i ) );
  }

  // Ensure we can decode a component in the high bits correctly.
  EXPECT_EQ( 0xff000000, Component<0xff000000>::COMPONENT_MASK );
  EXPECT_EQ( 24u,        Component<0xff000000>::LEADING_BIT_COUNT );
  EXPECT_EQ( 8u,         Component<0xff000000>::COMPONENT_BIT_COUNT );
  EXPECT_EQ( 0xffu,      Component<0xff000000>::u8( 0xff000000 ) );
}

TEST( RegalPixelConversions, PackUnpack8888 ) {
  // Test packing and unpacking RGBA 8888 data.

  // Get the conversion interface for this type.
  IConversion* conversion = GetConversionInterface( GL_RGBA, GL_UNSIGNED_BYTE );
  ASSERT_NE ( static_cast<IConversion*>( NULL ), conversion );

  // Verify we have the expected packed pixel information
  EXPECT_EQ ( 4u, conversion->GetPackedPixelByteSize() );
  EXPECT_EQ ( 4u, conversion->GetPackedPixelAlignmentSize() );
  EXPECT_EQ ( 4u, conversion->GetPackedPixelComponents() );

  // Set up some data arrays.
  uint8_t orig[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x18};
  uint32_t intermediate[ 4 ] = { 0 };  // Extra room for testing
  uint8_t packed[ 16 ] = { 0 }; // Extra room for testing

  // Convert from the packed to the unpacked intermediate format.
  conversion->Unpack32( orig, intermediate, 2 );

  EXPECT_EQ( 0x78563412u, intermediate[ 0 ] );
  EXPECT_EQ( 0xf0debc9au, intermediate[ 1 ] );
  EXPECT_EQ( 0u, intermediate[ 2 ] );
  EXPECT_EQ( 0u, intermediate[ 3 ] );

  // This should work regardless of starting position
  conversion->Unpack32( orig + 1, intermediate, 2 );

  EXPECT_EQ( 0x9a785634u, intermediate[ 0 ] );
  EXPECT_EQ( 0x18f0debcu, intermediate[ 1 ] );
  EXPECT_EQ( 0u, intermediate[ 2 ] );
  EXPECT_EQ( 0u, intermediate[ 3 ] );

  // Converting to the packed should also work.
  conversion->Pack32( intermediate, packed, 2 );

  EXPECT_EQ( 0x34u, packed[ 0 ] );
  EXPECT_EQ( 0x56u, packed[ 1 ] );
  EXPECT_EQ( 0x78u, packed[ 2 ] );
  EXPECT_EQ( 0x9au, packed[ 3 ] );
  EXPECT_EQ( 0xbcu, packed[ 4 ] );
  EXPECT_EQ( 0xdeu, packed[ 5 ] );
  EXPECT_EQ( 0xf0u, packed[ 6 ] );
  EXPECT_EQ( 0x18u, packed[ 7 ] );
  EXPECT_EQ( 0u,    packed[ 8 ] );
}

TEST( RegalPixelConversions, PackUnpack888 ) {
  // Test packing and unpacking RGB 888 data.

  // Get the conversion interface for this type.
  IConversion* conversion = GetConversionInterface( GL_RGB, GL_UNSIGNED_BYTE );
  ASSERT_NE ( static_cast<IConversion*>( NULL ), conversion );

  // Verify we have the expected packed pixel information
  EXPECT_EQ ( 3u, conversion->GetPackedPixelByteSize() );
  EXPECT_EQ ( 1u, conversion->GetPackedPixelAlignmentSize() );
  EXPECT_EQ ( 3u, conversion->GetPackedPixelComponents() );

  // Set up some data arrays.
  uint8_t orig[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x18};
  uint32_t intermediate[ 4 ] = { 0 };  // Extra room for testing
  uint8_t packed[ 16 ] = { 0 }; // Extra room for testing

  // Convert from the packed to the unpacked intermediate format.
  conversion->Unpack32( orig, intermediate, 2 );

  EXPECT_EQ( 0x00563412u, intermediate[ 0 ] );
  EXPECT_EQ( 0x00bc9a78u, intermediate[ 1 ] );
  EXPECT_EQ( 0u, intermediate[ 2 ] );
  EXPECT_EQ( 0u, intermediate[ 3 ] );

  // This should work regardless of starting position
  conversion->Unpack32( orig + 1, intermediate, 2 );

  EXPECT_EQ( 0x00785634u, intermediate[ 0 ] );
  EXPECT_EQ( 0x00debc9au, intermediate[ 1 ] );
  EXPECT_EQ( 0u, intermediate[ 2 ] );
  EXPECT_EQ( 0u, intermediate[ 3 ] );

  // Converting to the packed should also work.
  conversion->Pack32( intermediate, packed, 2 );

  EXPECT_EQ( 0x34u, packed[ 0 ] );
  EXPECT_EQ( 0x56u, packed[ 1 ] );
  EXPECT_EQ( 0x78u, packed[ 2 ] );
  EXPECT_EQ( 0x9au, packed[ 3 ] );
  EXPECT_EQ( 0xbcu, packed[ 4 ] );
  EXPECT_EQ( 0xdeu, packed[ 5 ] );
  EXPECT_EQ( 0u,    packed[ 6 ] );
  EXPECT_EQ( 0u,    packed[ 7 ] );
  EXPECT_EQ( 0u,    packed[ 8 ] );
}

TEST( RegalPixelConversions, PackUnpack565 ) {
  // Test packing and unpacking RGB 565 data.

  // Get the conversion interface for this type.
  IConversion* conversion = GetConversionInterface( GL_RGB, GL_UNSIGNED_SHORT_5_6_5 );
  ASSERT_NE ( static_cast<IConversion*>( NULL ), conversion ) ;

  // Verify we have the expected packed pixel information
  EXPECT_EQ ( 2u, conversion->GetPackedPixelByteSize() );
  EXPECT_EQ ( 2u, conversion->GetPackedPixelAlignmentSize() );
  EXPECT_EQ ( 3u, conversion->GetPackedPixelComponents() );

  uint16_t orig[] = {0xf800, 0x07e0, 0x001f, 0x5555, 0xaaaa};
  uint32_t intermediate[ 8 ] = { 0 };  // Extra room for testing
  uint8_t  packed[ 16 ] = { 0 }; // Extra room for testing

  // Convert from the packed to the unpacked intermediate format.
  conversion->Unpack32( orig, intermediate, 5 );

  EXPECT_EQ( 0x000000ffu, intermediate[ 0 ] );
  EXPECT_EQ( 0x0000ff00u, intermediate[ 1 ] );
  EXPECT_EQ( 0x00ff0000u, intermediate[ 2 ] );
  EXPECT_EQ( 0x00adaa52u, intermediate[ 3 ] );
  EXPECT_EQ( 0x005255adu, intermediate[ 4 ] );
  EXPECT_EQ( 0u, intermediate[ 5 ] );
  EXPECT_EQ( 0u, intermediate[ 6 ] );

  // Convert back to packed.
  conversion->Pack32( intermediate, packed, 5 );

  EXPECT_EQ( 0x00u, packed[ 0 ] );
  EXPECT_EQ( 0xf8u, packed[ 1 ] );
  EXPECT_EQ( 0xe0u, packed[ 2 ] );
  EXPECT_EQ( 0x07u, packed[ 3 ] );
  EXPECT_EQ( 0x1fu, packed[ 4 ] );
  EXPECT_EQ( 0x00u, packed[ 5 ] );
  EXPECT_EQ( 0x55u, packed[ 6 ] );
  EXPECT_EQ( 0x55u, packed[ 7 ] );
  EXPECT_EQ( 0xaau, packed[ 8 ] );
  EXPECT_EQ( 0xaau, packed[ 9 ] );
  EXPECT_EQ( 0u, packed[ 10 ] );

  // This should work even if not-aligned!
  conversion->Pack32( intermediate, packed + 1, 5 );

  EXPECT_EQ( 0x00u, packed[ 1 ] );
  EXPECT_EQ( 0xf8u, packed[ 2 ] );
  EXPECT_EQ( 0xe0u, packed[ 3 ] );
  EXPECT_EQ( 0x07u, packed[ 4 ] );
  EXPECT_EQ( 0x1fu, packed[ 5 ] );
  EXPECT_EQ( 0x00u, packed[ 6 ] );
  EXPECT_EQ( 0x55u, packed[ 7 ] );
  EXPECT_EQ( 0x55u, packed[ 8 ] );
  EXPECT_EQ( 0xaau, packed[ 9 ] );
  EXPECT_EQ( 0xaau, packed[ 10 ] );
  EXPECT_EQ( 0u, packed[ 11 ] );

  // And Unpacking not-aligned should also work.
  conversion->Unpack32( packed + 1, intermediate, 5 );

  EXPECT_EQ( 0x000000ffu, intermediate[ 0 ] );
  EXPECT_EQ( 0x0000ff00u, intermediate[ 1 ] );
  EXPECT_EQ( 0x00ff0000u, intermediate[ 2 ] );
  EXPECT_EQ( 0x00adaa52u, intermediate[ 3 ] );
  EXPECT_EQ( 0x005255adu, intermediate[ 4 ] );
  EXPECT_EQ( 0u, intermediate[ 5 ] );
  EXPECT_EQ( 0u, intermediate[ 6 ] );
}

TEST( RegalPixelConversions, PackUnpack1555 ) {
  // Test packing and unpacking RGBA 1555 reversed data.

  // Get the conversion interface for this type.
  IConversion* conversion = GetConversionInterface( GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1 );
  ASSERT_NE ( static_cast<IConversion*>( NULL ), conversion );

  // Verify we have the expected packed pixel information
  EXPECT_EQ ( 2u, conversion->GetPackedPixelByteSize() );
  EXPECT_EQ ( 2u, conversion->GetPackedPixelAlignmentSize() );
  EXPECT_EQ ( 4u, conversion->GetPackedPixelComponents() );

  uint16_t orig[] = {0x0001, 0xf800, 0x07c0, 0x003e, 0x5555, 0xaaaa};
  uint32_t intermediate[ 8 ] = { 0 };  // Extra room for testing
  uint8_t  packed[ 16 ] = { 0 }; // Extra room for testing

  // Convert from the packed to the unpacked intermediate format.
  conversion->Unpack32( orig, intermediate, 6 );

  EXPECT_EQ( 0xff000000u, intermediate[ 0 ] );
  EXPECT_EQ( 0x000000ffu, intermediate[ 1 ] );
  EXPECT_EQ( 0x0000ff00u, intermediate[ 2 ] );
  EXPECT_EQ( 0x00ff0000u, intermediate[ 3 ] );
  EXPECT_EQ( 0xff52ad52u, intermediate[ 4 ] );
  EXPECT_EQ( 0x00ad52adu, intermediate[ 5 ] );
  EXPECT_EQ( 0u, intermediate[ 6 ] );
  EXPECT_EQ( 0u, intermediate[ 7 ] );

  // Convert back to packed.
  conversion->Pack32( intermediate, packed, 6 );

  EXPECT_EQ( 0x01u, packed[ 0 ] );
  EXPECT_EQ( 0x00u, packed[ 1 ] );
  EXPECT_EQ( 0x00u, packed[ 2 ] );
  EXPECT_EQ( 0xf8u, packed[ 3 ] );
  EXPECT_EQ( 0xc0u, packed[ 4 ] );
  EXPECT_EQ( 0x07u, packed[ 5 ] );
  EXPECT_EQ( 0x3eu, packed[ 6 ] );
  EXPECT_EQ( 0x00u, packed[ 7 ] );
  EXPECT_EQ( 0x55u, packed[ 8 ] );
  EXPECT_EQ( 0x55u, packed[ 9 ] );
  EXPECT_EQ( 0xaau, packed[ 10 ] );
  EXPECT_EQ( 0xaau, packed[ 11 ] );
}

TEST( RegalPixelConversions, UnsupportedConversions ) {
  // Some conversions are just not supported.

  // Nonsensical.
  EXPECT_EQ( NULL, GetConversionInterface( GL_RGB, GL_UNSIGNED_SHORT_5_5_5_1 ) );

  // Intermediate larger than 32 bits required.
  EXPECT_EQ( NULL, GetConversionInterface( GL_RGBA, GL_UNSIGNED_INT_10_10_10_2 ) );
}

}  // namespace
