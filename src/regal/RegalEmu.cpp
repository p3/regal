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

/*

 Regal Emulation layer helpers
 Scott Nations

 */

#include "RegalUtil.h"

#if REGAL_EMULATION

#include "RegalEmu.h"
#include "RegalToken.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <cassert>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Emu
{

// From the GL spec for ERRORS associated with glActiveTexture

// An INVALID_ENUM error is generated if an invalid texture is specified.
// texture is a symbolic constant of the form TEXTUREi, indicating that texture
// unit i is to be modified. The constants obey TEXTUREi = TEXTURE0 +i where
// i is in the range 0 to k - 1, and k is the larger of the values of
// MAX_TEXTURE_COORDS and MAX_COMBINED_TEXTURE_IMAGE_UNITS).

// For backwards compatibility, the implementation-dependent constant
// MAX_TEXTURE_UNITS specifies the number of conventional texture units
// supported by the implementation. Its value must be no larger than the
// minimum of MAX_TEXTURE_COORDS and MAX_COMBINED_TEXTURE_IMAGE_UNITS.

bool validTextureEnum(GLenum texture)
{
  GLint unit = texture - GL_TEXTURE0;
  return validTextureUnit( static_cast<GLuint>(unit) );
}

bool validTextureUnit(GLuint unit)
{
  RegalAssert(REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS >= REGAL_EMU_MAX_TEXTURE_COORDS);
  if (unit >= REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
  {
    Warning( "Active texture out of range: ",
             Token::GLtextureToString(static_cast<GLenum>(GL_TEXTURE0 + unit)),
             " > ",
             Token::GLtextureToString(static_cast<GLenum>(GL_TEXTURE0 + REGAL_EMU_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1)));
    return false;
  }
  return true;
}

};

REGAL_NAMESPACE_END

#endif // REGAL_EMULATION
