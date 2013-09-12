/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2013 Nigel Stewart
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

#include "RegalLog.h"
#include "RegalConfig.h"

#include "RegalJson.h"
#include "RegalJson.inl"

#include <jsonsl.h>

using namespace std;

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

namespace Json {

const Object parent[JSON_UNDEFINED+1] =
{
  JSON_ROOT,
  JSON_ROOT,
  JSON_REGAL,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG_CACHE,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_DISPATCH,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE,
  JSON_REGAL_CONFIG_DISPATCH,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH_ENABLE,
  JSON_REGAL_CONFIG_DISPATCH,
  JSON_REGAL_CONFIG_DISPATCH_FORCE,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_FORCE,
  JSON_REGAL_CONFIG_FORCE,
  JSON_REGAL_CONFIG_FORCE,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_FRAME,
  JSON_REGAL_CONFIG_FRAME,
  JSON_REGAL_CONFIG_FRAME_MD5,
  JSON_REGAL_CONFIG_FRAME_MD5,
  JSON_REGAL_CONFIG_FRAME_MD5,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK,
  JSON_REGAL_CONFIG_FRAME_MD5_MASK,
  JSON_REGAL_CONFIG_FRAME_MD5,
  JSON_REGAL_CONFIG_FRAME,
  JSON_REGAL_CONFIG_FRAME_SAVE,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE,
  JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE,
  JSON_REGAL_CONFIG_FRAME_SAVE,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX,
  JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG_SYSTEM,
  JSON_REGAL_CONFIG,
  JSON_REGAL_CONFIG_TRACE,
  JSON_REGAL,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING_ENABLE,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,
  JSON_REGAL_LOGGING,

  JSON_UNDEFINED
};

Parser::Parser()
: depth(0),
  current(JSON_ROOT)
{
}

Parser::~Parser()
{
}

void
Parser::onPush(const string &name)
{
  Internal("Regal::Json::Parser::onPush","name=",name);

  switch (current)
  {
    case JSON_ROOT:
      if (name=="regal"       ) { current = JSON_REGAL;                                        return; }
      break;

    case JSON_REGAL:
      if (name=="config"      ) { current = JSON_REGAL_CONFIG;                                 return; }
      if (name=="logging"     ) { current = JSON_REGAL_LOGGING;                                return; }
      break;

    case JSON_REGAL_CONFIG:
      if (name=="cache"       ) { current = JSON_REGAL_CONFIG_CACHE;                           return; }
      if (name=="configFile"  ) { current = JSON_REGAL_CONFIG_CONFIGFILE;                      return; }
      if (name=="dispatch"    ) { current = JSON_REGAL_CONFIG_DISPATCH;                        return; }
      if (name=="force"       ) { current = JSON_REGAL_CONFIG_FORCE;                           return; }
      if (name=="frame"       ) { current = JSON_REGAL_CONFIG_FRAME;                           return; }
      if (name=="system"      ) { current = JSON_REGAL_CONFIG_SYSTEM;                          return; }
      if (name=="trace"       ) { current = JSON_REGAL_CONFIG_TRACE;                           return; }
      break;

    case JSON_REGAL_CONFIG_CACHE:
      if (name=="directory"   ) { current = JSON_REGAL_CONFIG_CACHE_DIRECTORY;                 return; }
      if (name=="enable"      ) { current = JSON_REGAL_CONFIG_CACHE_ENABLE;                    return; }
      if (name=="shader"      ) { current = JSON_REGAL_CONFIG_CACHE_SHADER;                    return; }
      if (name=="shaderRead"  ) { current = JSON_REGAL_CONFIG_CACHE_SHADERREAD;                return; }
      if (name=="shaderWrite" ) { current = JSON_REGAL_CONFIG_CACHE_SHADERWRITE;               return; }
      if (name=="texture"     ) { current = JSON_REGAL_CONFIG_CACHE_TEXTURE;                   return; }
      if (name=="textureRead" ) { current = JSON_REGAL_CONFIG_CACHE_TEXTUREREAD;               return; }
      if (name=="textureWrite") { current = JSON_REGAL_CONFIG_CACHE_TEXTUREWRITE;              return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH:
      if (name=="emulation"   ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION;              return; }
      if (name=="enable"      ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE;                 return; }
      if (name=="force"       ) { current = JSON_REGAL_CONFIG_DISPATCH_FORCE;                  return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH_EMULATION:
      if (name=="enable"      ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE;       return; }
      if (name=="force"       ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE;        return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE:
      if (name=="bin"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BIN;   return; }
      if (name=="bv"          ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BV;    return; }
      if (name=="dsa"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_DSA;   return; }
      if (name=="filter"      ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_FILTER; return; }
      if (name=="hint"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_HINT;  return; }
      if (name=="iff"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_IFF;   return; }
      if (name=="obj"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_OBJ;   return; }
      if (name=="path"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PATH;  return; }
      if (name=="ppa"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPA;   return; }
      if (name=="ppca"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPCA;  return; }
      if (name=="rect"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_RECT;  return; }
      if (name=="so"          ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_SO;    return; }
      if (name=="texc"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXC;  return; }
      if (name=="texsto"      ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXSTO; return; }
      if (name=="vao"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_VAO;   return; }
      if (name=="xfer"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_XFER;  return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE:
      if (name=="bin"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BIN;    return; }
      if (name=="bv"          ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BV;     return; }
      if (name=="dsa"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_DSA;    return; }
      if (name=="filter"      ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_FILTER; return; }
      if (name=="hint"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_HINT;   return; }
      if (name=="iff"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_IFF;    return; }
      if (name=="obj"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_OBJ;    return; }
      if (name=="path"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PATH;   return; }
      if (name=="ppa"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPA;    return; }
      if (name=="ppca"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPCA;   return; }
      if (name=="rect"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_RECT;   return; }
      if (name=="so"          ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_SO;     return; }
      if (name=="texc"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXC;   return; }
      if (name=="texsto"      ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXSTO; return; }
      if (name=="vao"         ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_VAO;    return; }
      if (name=="xfer"        ) { current = JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_XFER;   return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH_ENABLE:
      if (name=="code"        ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_CODE;            return; }
      if (name=="debug"       ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_DEBUG;           return; }
      if (name=="driver"      ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_DRIVER;          return; }
      if (name=="emulation"   ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_EMULATION;       return; }
      if (name=="error"       ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_ERROR;           return; }
      if (name=="log"         ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_LOG;             return; }
      if (name=="missing"     ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_MISSING;         return; }
      if (name=="statistics"  ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_STATISTICS;      return; }
      if (name=="trace"       ) { current = JSON_REGAL_CONFIG_DISPATCH_ENABLE_TRACE;           return; }
      break;

    case JSON_REGAL_CONFIG_DISPATCH_FORCE:
      if (name=="emulation"   ) { current = JSON_REGAL_CONFIG_DISPATCH_FORCE_EMULATION;        return; }
      break;

    case JSON_REGAL_CONFIG_FORCE:
      if (name=="Core"        ) { current = JSON_REGAL_CONFIG_FORCE_CORE;                      return; }
      if (name=="ES1"         ) { current = JSON_REGAL_CONFIG_FORCE_ES1;                       return; }
      if (name=="ES2"         ) { current = JSON_REGAL_CONFIG_FORCE_ES2;                       return; }
      break;

    case JSON_REGAL_CONFIG_FRAME:
      if (name=="limit"       ) { current = JSON_REGAL_CONFIG_FRAME_LIMIT;                     return; }
      if (name=="md5"         ) { current = JSON_REGAL_CONFIG_FRAME_MD5;                       return; }
      if (name=="save"        ) { current = JSON_REGAL_CONFIG_FRAME_SAVE;                      return; }
      break;

    case JSON_REGAL_CONFIG_FRAME_MD5:
      if (name=="color"       ) { current = JSON_REGAL_CONFIG_FRAME_MD5_COLOR;                 return; }
      if (name=="depth"       ) { current = JSON_REGAL_CONFIG_FRAME_MD5_DEPTH;                 return; }
      if (name=="mask"        ) { current = JSON_REGAL_CONFIG_FRAME_MD5_MASK;                  return; }
      if (name=="stencil"     ) { current = JSON_REGAL_CONFIG_FRAME_MD5_STENCIL;               return; }
      break;

    case JSON_REGAL_CONFIG_FRAME_MD5_MASK:
      if (name=="color"       ) { current = JSON_REGAL_CONFIG_FRAME_MD5_MASK_COLOR;            return; }
      if (name=="depth"       ) { current = JSON_REGAL_CONFIG_FRAME_MD5_MASK_DEPTH;            return; }
      if (name=="stencil"     ) { current = JSON_REGAL_CONFIG_FRAME_MD5_MASK_STENCIL;          return; }
      break;

    case JSON_REGAL_CONFIG_FRAME_SAVE:
      if (name=="enable"      ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE;               return; }
      if (name=="prefix"      ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX;               return; }
      break;

    case JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE:
      if (name=="color"       ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_COLOR;         return; }
      if (name=="depth"       ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_DEPTH;         return; }
      if (name=="stencil"     ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_STENCIL;       return; }
      break;

    case JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX:
      if (name=="color"       ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_COLOR;         return; }
      if (name=="depth"       ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_DEPTH;         return; }
      if (name=="stencil"     ) { current = JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_STENCIL;       return; }
      break;

    case JSON_REGAL_CONFIG_SYSTEM:
      if (name=="EGL"         ) { current = JSON_REGAL_CONFIG_SYSTEM_EGL;                      return; }
      if (name=="ES1"         ) { current = JSON_REGAL_CONFIG_SYSTEM_ES1;                      return; }
      if (name=="ES2"         ) { current = JSON_REGAL_CONFIG_SYSTEM_ES2;                      return; }
      if (name=="GL"          ) { current = JSON_REGAL_CONFIG_SYSTEM_GL;                       return; }
      if (name=="GLX"         ) { current = JSON_REGAL_CONFIG_SYSTEM_GLX;                      return; }
      break;

    case JSON_REGAL_CONFIG_TRACE:
      if (name=="file"        ) { current = JSON_REGAL_CONFIG_TRACE_FILE;                      return; }
      break;

    case JSON_REGAL_LOGGING:
      if (name=="bufferLimit" ) { current = JSON_REGAL_LOGGING_BUFFERLIMIT;                    return; }
      if (name=="callback"    ) { current = JSON_REGAL_LOGGING_CALLBACK;                       return; }
      if (name=="enable"      ) { current = JSON_REGAL_LOGGING_ENABLE;                         return; }
      if (name=="filename"    ) { current = JSON_REGAL_LOGGING_FILENAME;                       return; }
      if (name=="frameStatistics") { current = JSON_REGAL_LOGGING_FRAMESTATISTICS;                return; }
      if (name=="frameTime"   ) { current = JSON_REGAL_LOGGING_FRAMETIME;                      return; }
      if (name=="json"        ) { current = JSON_REGAL_LOGGING_JSON;                           return; }
      if (name=="jsonFile"    ) { current = JSON_REGAL_LOGGING_JSONFILE;                       return; }
      if (name=="log"         ) { current = JSON_REGAL_LOGGING_LOG;                            return; }
      if (name=="maxBytes"    ) { current = JSON_REGAL_LOGGING_MAXBYTES;                       return; }
      if (name=="maxLines"    ) { current = JSON_REGAL_LOGGING_MAXLINES;                       return; }
      if (name=="once"        ) { current = JSON_REGAL_LOGGING_ONCE;                           return; }
      if (name=="pointers"    ) { current = JSON_REGAL_LOGGING_POINTERS;                       return; }
      if (name=="process"     ) { current = JSON_REGAL_LOGGING_PROCESS;                        return; }
      if (name=="thread"      ) { current = JSON_REGAL_LOGGING_THREAD;                         return; }
      break;

    case JSON_REGAL_LOGGING_ENABLE:
      if (name=="app"         ) { current = JSON_REGAL_LOGGING_ENABLE_APP;                     return; }
      if (name=="driver"      ) { current = JSON_REGAL_LOGGING_ENABLE_DRIVER;                  return; }
      if (name=="error"       ) { current = JSON_REGAL_LOGGING_ENABLE_ERROR;                   return; }
      if (name=="http"        ) { current = JSON_REGAL_LOGGING_ENABLE_HTTP;                    return; }
      if (name=="info"        ) { current = JSON_REGAL_LOGGING_ENABLE_INFO;                    return; }
      if (name=="internal"    ) { current = JSON_REGAL_LOGGING_ENABLE_INTERNAL;                return; }
      if (name=="warning"     ) { current = JSON_REGAL_LOGGING_ENABLE_WARNING;                 return; }
      break;

    default:
      break;
  }
  ++depth;
}

void
Parser::onPop()
{
  Internal("Regal::Json::Parser::onPop","()");

  if (depth)
    --depth;
  else
    current = parent[current];
}

void
Parser::onValue()
{
  Internal("Regal::Json::Parser::onValue","NULL");

  switch (current)
  {
    default:
      Warning("Ignoring JSON value NULL");
      break;
  }
}

void
Parser::onValue(const bool value)
{
  Internal("Regal::Json::Parser::onValue","value=",value);

  switch (current)
  {
    case JSON_REGAL_CONFIG_CACHE_ENABLE                    : { set_json_regal_config_cache_enable(value);                   return; }
    case JSON_REGAL_CONFIG_CACHE_SHADER                    : { set_json_regal_config_cache_shader(value);                   return; }
    case JSON_REGAL_CONFIG_CACHE_SHADERREAD                : { set_json_regal_config_cache_shaderread(value);               return; }
    case JSON_REGAL_CONFIG_CACHE_SHADERWRITE               : { set_json_regal_config_cache_shaderwrite(value);              return; }
    case JSON_REGAL_CONFIG_CACHE_TEXTURE                   : { set_json_regal_config_cache_texture(value);                  return; }
    case JSON_REGAL_CONFIG_CACHE_TEXTUREREAD               : { set_json_regal_config_cache_textureread(value);              return; }
    case JSON_REGAL_CONFIG_CACHE_TEXTUREWRITE              : { set_json_regal_config_cache_texturewrite(value);             return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BIN   : { set_json_regal_config_dispatch_emulation_enable_bin(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_BV    : { set_json_regal_config_dispatch_emulation_enable_bv(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_DSA   : { set_json_regal_config_dispatch_emulation_enable_dsa(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_FILTER: { set_json_regal_config_dispatch_emulation_enable_filter(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_HINT  : { set_json_regal_config_dispatch_emulation_enable_hint(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_IFF   : { set_json_regal_config_dispatch_emulation_enable_iff(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_OBJ   : { set_json_regal_config_dispatch_emulation_enable_obj(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PATH  : { set_json_regal_config_dispatch_emulation_enable_path(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPA   : { set_json_regal_config_dispatch_emulation_enable_ppa(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_PPCA  : { set_json_regal_config_dispatch_emulation_enable_ppca(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_RECT  : { set_json_regal_config_dispatch_emulation_enable_rect(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_SO    : { set_json_regal_config_dispatch_emulation_enable_so(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXC  : { set_json_regal_config_dispatch_emulation_enable_texc(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_TEXSTO: { set_json_regal_config_dispatch_emulation_enable_texsto(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_VAO   : { set_json_regal_config_dispatch_emulation_enable_vao(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_ENABLE_XFER  : { set_json_regal_config_dispatch_emulation_enable_xfer(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BIN    : { set_json_regal_config_dispatch_emulation_force_bin(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_BV     : { set_json_regal_config_dispatch_emulation_force_bv(value);    return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_DSA    : { set_json_regal_config_dispatch_emulation_force_dsa(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_FILTER : { set_json_regal_config_dispatch_emulation_force_filter(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_HINT   : { set_json_regal_config_dispatch_emulation_force_hint(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_IFF    : { set_json_regal_config_dispatch_emulation_force_iff(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_OBJ    : { set_json_regal_config_dispatch_emulation_force_obj(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PATH   : { set_json_regal_config_dispatch_emulation_force_path(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPA    : { set_json_regal_config_dispatch_emulation_force_ppa(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_PPCA   : { set_json_regal_config_dispatch_emulation_force_ppca(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_RECT   : { set_json_regal_config_dispatch_emulation_force_rect(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_SO     : { set_json_regal_config_dispatch_emulation_force_so(value);    return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXC   : { set_json_regal_config_dispatch_emulation_force_texc(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_TEXSTO : { set_json_regal_config_dispatch_emulation_force_texsto(value); return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_VAO    : { set_json_regal_config_dispatch_emulation_force_vao(value);   return; }
    case JSON_REGAL_CONFIG_DISPATCH_EMULATION_FORCE_XFER   : { set_json_regal_config_dispatch_emulation_force_xfer(value);  return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_CODE            : { set_json_regal_config_dispatch_enable_code(value);           return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_DEBUG           : { set_json_regal_config_dispatch_enable_debug(value);          return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_DRIVER          : { set_json_regal_config_dispatch_enable_driver(value);         return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_EMULATION       : { set_json_regal_config_dispatch_enable_emulation(value);      return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_ERROR           : { set_json_regal_config_dispatch_enable_error(value);          return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_LOG             : { set_json_regal_config_dispatch_enable_log(value);            return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_MISSING         : { set_json_regal_config_dispatch_enable_missing(value);        return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_STATISTICS      : { set_json_regal_config_dispatch_enable_statistics(value);     return; }
    case JSON_REGAL_CONFIG_DISPATCH_ENABLE_TRACE           : { set_json_regal_config_dispatch_enable_trace(value);          return; }
    case JSON_REGAL_CONFIG_DISPATCH_FORCE_EMULATION        : { set_json_regal_config_dispatch_force_emulation(value);       return; }
    case JSON_REGAL_CONFIG_FORCE_CORE                      : { set_json_regal_config_force_core(value);                     return; }
    case JSON_REGAL_CONFIG_FORCE_ES1                       : { set_json_regal_config_force_es1(value);                      return; }
    case JSON_REGAL_CONFIG_FORCE_ES2                       : { set_json_regal_config_force_es2(value);                      return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_COLOR                 : { set_json_regal_config_frame_md5_color(value);                return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_DEPTH                 : { set_json_regal_config_frame_md5_depth(value);                return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_STENCIL               : { set_json_regal_config_frame_md5_stencil(value);              return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_COLOR         : { set_json_regal_config_frame_save_enable_color(value);        return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_DEPTH         : { set_json_regal_config_frame_save_enable_depth(value);        return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_ENABLE_STENCIL       : { set_json_regal_config_frame_save_enable_stencil(value);      return; }
    case JSON_REGAL_CONFIG_SYSTEM_EGL                      : { set_json_regal_config_system_egl(value);                     return; }
    case JSON_REGAL_CONFIG_SYSTEM_ES1                      : { set_json_regal_config_system_es1(value);                     return; }
    case JSON_REGAL_CONFIG_SYSTEM_ES2                      : { set_json_regal_config_system_es2(value);                     return; }
    case JSON_REGAL_CONFIG_SYSTEM_GL                       : { set_json_regal_config_system_gl(value);                      return; }
    case JSON_REGAL_CONFIG_SYSTEM_GLX                      : { set_json_regal_config_system_glx(value);                     return; }
    case JSON_REGAL_LOGGING_CALLBACK                       : { set_json_regal_logging_callback(value);                      return; }
    case JSON_REGAL_LOGGING_FRAMESTATISTICS                : { set_json_regal_logging_framestatistics(value);               return; }
    case JSON_REGAL_LOGGING_FRAMETIME                      : { set_json_regal_logging_frametime(value);                     return; }
    case JSON_REGAL_LOGGING_JSON                           : { set_json_regal_logging_json(value);                          return; }
    case JSON_REGAL_LOGGING_LOG                            : { set_json_regal_logging_log(value);                           return; }
    case JSON_REGAL_LOGGING_ONCE                           : { set_json_regal_logging_once(value);                          return; }
    case JSON_REGAL_LOGGING_POINTERS                       : { set_json_regal_logging_pointers(value);                      return; }
    case JSON_REGAL_LOGGING_PROCESS                        : { set_json_regal_logging_process(value);                       return; }
    case JSON_REGAL_LOGGING_THREAD                         : { set_json_regal_logging_thread(value);                        return; }
    case JSON_REGAL_LOGGING_ENABLE_APP                     : { set_json_regal_logging_enable_app(value);                    return; }
    case JSON_REGAL_LOGGING_ENABLE_DRIVER                  : { set_json_regal_logging_enable_driver(value);                 return; }
    case JSON_REGAL_LOGGING_ENABLE_ERROR                   : { set_json_regal_logging_enable_error(value);                  return; }
    case JSON_REGAL_LOGGING_ENABLE_HTTP                    : { set_json_regal_logging_enable_http(value);                   return; }
    case JSON_REGAL_LOGGING_ENABLE_INFO                    : { set_json_regal_logging_enable_info(value);                   return; }
    case JSON_REGAL_LOGGING_ENABLE_INTERNAL                : { set_json_regal_logging_enable_internal(value);               return; }
    case JSON_REGAL_LOGGING_ENABLE_WARNING                 : { set_json_regal_logging_enable_warning(value);                return; }
    default:
      Warning("Ignoring JSON value: ",value);
      break;
  }
}

void
Parser::onValue(const long value)
{
  Internal("Regal::Json::Parser::onValue","value=",value);

  switch (current)
  {
    case JSON_REGAL_CONFIG_FRAME_LIMIT                     : { set_json_regal_config_frame_limit(value);                    return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_MASK_COLOR            : { set_json_regal_config_frame_md5_mask_color(value);           return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_MASK_DEPTH            : { set_json_regal_config_frame_md5_mask_depth(value);           return; }
    case JSON_REGAL_CONFIG_FRAME_MD5_MASK_STENCIL          : { set_json_regal_config_frame_md5_mask_stencil(value);         return; }
    case JSON_REGAL_LOGGING_BUFFERLIMIT                    : { set_json_regal_logging_bufferlimit(value);                   return; }
    case JSON_REGAL_LOGGING_MAXBYTES                       : { set_json_regal_logging_maxbytes(value);                      return; }
    case JSON_REGAL_LOGGING_MAXLINES                       : { set_json_regal_logging_maxlines(value);                      return; }
    default:
      Warning("Ignoring JSON value: ",value);
      break;
  }
}

void
Parser::onValue(const string &value)
{
  Internal("Regal::Json::Parser::onValue","value=",value);

  switch (current)
  {
    case JSON_REGAL_CONFIG_CONFIGFILE                      : { set_json_regal_config_configfile(value);                     return; }
    case JSON_REGAL_CONFIG_CACHE_DIRECTORY                 : { set_json_regal_config_cache_directory(value);                return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_COLOR         : { set_json_regal_config_frame_save_prefix_color(value);        return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_DEPTH         : { set_json_regal_config_frame_save_prefix_depth(value);        return; }
    case JSON_REGAL_CONFIG_FRAME_SAVE_PREFIX_STENCIL       : { set_json_regal_config_frame_save_prefix_stencil(value);      return; }
    case JSON_REGAL_CONFIG_TRACE_FILE                      : { set_json_regal_config_trace_file(value);                     return; }
    case JSON_REGAL_LOGGING_FILENAME                       : { set_json_regal_logging_filename(value);                      return; }
    case JSON_REGAL_LOGGING_JSONFILE                       : { set_json_regal_logging_jsonfile(value);                      return; }
    default:
      Warning("Ignoring JSON value: ",value);
      break;
  }
}

////////////

static void
jsonslAction(jsonsl_t jsn, jsonsl_action_t action, struct jsonsl_state_st *state, const char *at)
{
  Parser *parser = reinterpret_cast<Parser *>(jsn->data);
  RegalAssert(parser);
  UNUSED_PARAMETER(at);

  switch (action)
  {
    case JSONSL_ACTION_PUSH:
      break;

    case JSONSL_ACTION_POP:
      switch (state->type)
      {
        case JSONSL_T_HKEY:
          parser->onPush(std::string(jsn->base+state->pos_begin+1,state->pos_cur-state->pos_begin-1));
          return;

        case JSONSL_T_OBJECT:
          break;

        case JSONSL_T_SPECIAL:
          if (state->special_flags&JSONSL_SPECIALf_TRUE)
            parser->onValue(true);
          else if (state->special_flags&JSONSL_SPECIALf_FALSE)
            parser->onValue(false);
          else if (state->special_flags&JSONSL_SPECIALf_NUMERIC)
          {
            if (jsn->base[state->pos_begin]=='-')
              parser->onValue(strtol(jsn->base+state->pos_begin,NULL,0));
            else
              parser->onValue((long)strtoul(jsn->base+state->pos_begin,NULL,0));
          }
          break;

        case JSONSL_T_STRING:
          parser->onValue(std::string(jsn->base+state->pos_begin+1,state->pos_cur-state->pos_begin-1));
          break;

        default:
          break;
      }
      parser->onPop();
      break;

    default:
      break;
  }
}

static int
jsonslError(jsonsl_t jsn, jsonsl_error_t err, struct jsonsl_state_st *state, char *at)
{
  Parser REGAL_UNUSED *parser = reinterpret_cast<Parser *>(jsn->data);
  RegalAssert(parser);
  UNUSED_PARAMETER(parser);
  UNUSED_PARAMETER(err);
  UNUSED_PARAMETER(state);
  UNUSED_PARAMETER(at);
  Warning("JSON parsing error: ",jsonsl_strerror(err));
  return 0;  // zero to give up
}

bool
Parser::parseFile(const string &filename)
{
  FILE *f = fopen(filename.c_str(), "rt");
  if (!f)
    return false;

  jsonsl_t jsn = jsonsl_new(0x1000);
  jsonsl_enable_all_callbacks(jsn);

  jsn->action_callback = jsonslAction;
  jsn->error_callback  = jsonslError;

  Parser parser;
  jsn->data = &parser;

  const size_t bufferSize = 4096;
  char buffer[bufferSize];

  for (;;)
  {
    size_t n = fread(buffer,1,bufferSize,f);
    jsonsl_feed(jsn, buffer, n);
    if (n<bufferSize)
      break;
  }

  jsonsl_destroy(jsn);
  fclose(f);
  return true;
}

bool
Parser::parseString(const char *json)
{
  jsonsl_t jsn = jsonsl_new(0x1000);
  jsonsl_enable_all_callbacks(jsn);

  jsn->action_callback = jsonslAction;
  jsn->error_callback  = jsonslError;

  Parser parser;
  jsn->data = &parser;
  jsonsl_feed(jsn, json, strlen(json));
  jsonsl_destroy(jsn);
  return true;
}

}

REGAL_NAMESPACE_END

