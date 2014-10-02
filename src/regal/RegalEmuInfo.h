/* NOTE: Do not edit this file, it is generated by a script:
   Export.py --api gl 4.5 --api wgl 4.5 --api glx 4.5 --api cgl 1.4 --api egl 1.0 --outdir .
*/

/*
  Copyright (c) 2011-2014 NVIDIA Corporation
  Copyright (c) 2012-2014 Scott Nations
  Copyright (c) 2012-2014 Nigel Stewart
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012-2013 Google Inc.
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
  Intended formatting conventions:
  $ astyle --style=allman --indent=spaces=2 --indent-switches
*/

/*

 Regal emulation information and runtime emulation limits
 Nigel Stewart, Scott Nations

 */

#ifndef __REGAL_EMU_INFO_H__
#define __REGAL_EMU_INFO_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include <GL/Regal.h>

#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

struct ContextInfo;

struct EmuInfo
{
  EmuInfo();
  ~EmuInfo();

  void init(const ContextInfo &contextInfo);

  // glewGetExtension, glewIsSupported

  bool getExtension(const ContextInfo &contextInfo, const char *ext) const;
  bool isSupported(const ContextInfo &contextInfo, const char *ext) const;

  // Reported by Regal to the application

  std::string vendor;
  std::string renderer;
  std::string version;
  std::string extensions;

  //

  std::set<std::string> extensionsSet;

  // Emulated extensions and limits

  GLboolean gl_arb_draw_buffers : 1;
  GLboolean gl_arb_draw_elements_base_vertex : 1;
  GLboolean gl_arb_multitexture : 1;
  GLboolean gl_arb_sampler_objects : 1;
  GLboolean gl_arb_texture_cube_map : 1;
  GLboolean gl_arb_texture_env_combine : 1;
  GLboolean gl_arb_texture_env_dot3 : 1;
  GLboolean gl_arb_texture_storage : 1;
  GLboolean gl_arb_vertex_array_object : 1;
  GLboolean gl_ati_draw_buffers : 1;
  GLboolean gl_ext_blend_color : 1;
  GLboolean gl_ext_blend_subtract : 1;
  GLboolean gl_ext_direct_state_access : 1;
  GLboolean gl_ext_framebuffer_blit : 1;
  GLboolean gl_ext_framebuffer_object : 1;
  GLboolean gl_ext_texture_cube_map : 1;
  GLboolean gl_ext_texture_edge_clamp : 1;
  GLboolean gl_ext_texture_env_combine : 1;
  GLboolean gl_ext_texture_env_dot3 : 1;
  GLboolean gl_ibm_texture_mirrored_repeat : 1;
  GLboolean gl_nv_blend_square : 1;
  GLboolean gl_nv_path_rendering : 1;

  GLuint gl_max_attrib_stack_depth;
  GLuint gl_max_client_attrib_stack_depth;
  GLuint gl_max_combined_texture_image_units;
  GLuint gl_max_debug_message_length;
  GLuint gl_max_draw_buffers;
  GLuint gl_max_texture_coords;
  GLuint gl_max_texture_units;
  GLuint gl_max_vertex_attrib_bindings;
  GLuint gl_max_vertex_attribs;
  GLuint gl_max_viewports;

  GLuint gl_max_varying_floats;

};

REGAL_NAMESPACE_END

#endif // __REGAL_EMU_INFO_H__
