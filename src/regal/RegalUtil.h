/*
  Copyright (c) 2011-2013 NVIDIA Corporation
  Copyright (c) 2011-2013 Cass Everitt
  Copyright (c) 2012-2013 Scott Nations
  Copyright (c) 2012-2013 Mathias Schott
  Copyright (c) 2012-2013 Nigel Stewart
  Copyright (c) 2012 Google Inc.
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

#ifndef REGAL_UTIL_H
#define REGAL_UTIL_H

#include <cstdlib>
#include <cstdio>

#include <limits>

#include "RegalSystem.h"

#if REGAL_SYS_X11
#include <X11/Xlib.h>
#endif

// Visual Studio with /W4 complains about unused function
// parameters.

#ifndef UNUSED_PARAMETER
#ifdef _MSC_VER
#define UNUSED_PARAMETER(p) (p)
#else
#define UNUSED_PARAMETER(p)
#endif
#endif

#ifndef REGAL_UNUSED
#ifdef __GNUC__
#define REGAL_UNUSED __attribute__(unused)
#else
#define REGAL_UNUSED
#endif
#endif

#define REGAL_QUOTE(str) #str
#define REGAL_EQUOTE(str) REGAL_QUOTE(str)

#define REGAL_NAMESPACE_INTERNAL Regal

// REGAL_GLOBAL_BEGIN and REGAL_GLOBAL_END for scoping externals
// such as C, C++ and boost includes.

// VC8, VC9  - C4127: conditional expression is constant in std::list
// VC10      - C4512:
// VC9, VC10 - C4996: 'vsprintf': This function or variable may be unsafe
// VC10      - C4127: conditional expression is constant
// VC10      - C4706: assignment within conditional expression

#ifdef _MSC_VER
# define REGAL_GLOBAL_BEGIN         \
  __pragma(pack(push))              \
  __pragma(pack(8))                 \
  __pragma(warning(push))           \
  __pragma(warning(disable : 4127)) \
  __pragma(warning(disable : 4512)) \
  __pragma(warning(disable : 4996)) \
  __pragma(warning(disable : 4706))
# define REGAL_GLOBAL_END           \
  __pragma(warning(pop))            \
  __pragma(pack(pop))
#else
# define REGAL_GLOBAL_BEGIN
# define REGAL_GLOBAL_END
#endif

// REGAL_NAMESPACE_BEGIN and REGAL_NAMESPACE_END for scoping Regal internals

#ifdef _MSC_VER
# define REGAL_NAMESPACE_BEGIN         \
  __pragma(pack(push))                 \
  __pragma(pack(8))                    \
  __pragma(warning(push))              \
  __pragma(warning(disable : 4996))    \
  __pragma(warning(disable : 4127))    \
  __pragma(warning(disable : 4706))    \
  namespace REGAL_NAMESPACE_INTERNAL   \
  {
# define REGAL_NAMESPACE_END           \
  }                                    \
  __pragma(warning(pop))               \
  __pragma(pack(pop))
#else
# define REGAL_NAMESPACE_BEGIN         \
  namespace REGAL_NAMESPACE_INTERNAL   \
  {
# define REGAL_NAMESPACE_END           \
  }
#endif

// Compile-time configuration
// - Emulation not forced by default
// - All emulation layers enabled by default

#ifndef REGAL_FORCE_EMULATION
#define REGAL_FORCE_EMULATION 0
#endif

// Debug dispatch unsupported by default in
// release mode

#ifndef REGAL_DEBUG
#ifdef NDEBUG
#define REGAL_DEBUG 0
#else
#define REGAL_DEBUG 1
#endif
#endif

// Error dispatch unsupported by default in
// release mode

#ifndef REGAL_ERROR
#ifdef NDEBUG
#define REGAL_ERROR 0
#else
#define REGAL_ERROR 1
#endif
#endif

// Code logging unsupported by default in
// release mode, or embedded

#ifndef REGAL_CODE
# if defined(NDEBUG) || REGAL_SYS_IOS || REGAL_SYS_PPAPI || REGAL_SYS_ANDROID
#  define REGAL_CODE 0
# else
#  define REGAL_CODE 0
# endif
#endif

// Emulation dispatch supported by default

#ifndef REGAL_EMULATION
#define REGAL_EMULATION 1
#endif

// Driver dispatch supported by default

#ifndef REGAL_DRIVER
#define REGAL_DRIVER 1
#endif

#ifndef REGAL_LOG
#define REGAL_LOG 1
#endif

// Emulation layers supported by default

#ifndef REGAL_EMU_PPA
#define REGAL_EMU_PPA 1
#endif

#ifndef REGAL_EMU_PPCA
#define REGAL_EMU_PPCA 1
#endif

#ifndef REGAL_EMU_OBJ
#define REGAL_EMU_OBJ 1
#endif

#ifndef REGAL_EMU_BIN
#define REGAL_EMU_BIN 1
#endif

#ifndef REGAL_EMU_XFER
#define REGAL_EMU_XFER 1
#endif

#ifndef REGAL_EMU_DSA
#define REGAL_EMU_DSA 1
#endif

#ifndef REGAL_EMU_IFF
#define REGAL_EMU_IFF 1
#endif

#ifndef REGAL_EMU_SO
#define REGAL_EMU_SO 1
#endif

#ifndef REGAL_EMU_VAO
#define REGAL_EMU_VAO 1
#endif

#ifndef REGAL_EMU_FILTER
#define REGAL_EMU_FILTER 1
#endif

#ifndef REGAL_EMU_TEXC
#if REGAL_SYS_PPAPI
#define REGAL_EMU_TEXC 1
#else
#define REGAL_EMU_TEXC 0
#endif
#endif

// Emulation layer forcing disabled by default

#ifndef REGAL_FORCE_EMU_PPA
#define REGAL_FORCE_EMU_PPA 0
#endif

#ifndef REGAL_FORCE_EMU_PPCA
#define REGAL_FORCE_EMU_PPCA 0
#endif

#ifndef REGAL_FORCE_EMU_OBJ
#define REGAL_FORCE_EMU_OBJ 0
#endif

#ifndef REGAL_FORCE_EMU_BIN
#define REGAL_FORCE_EMU_BIN 0
#endif

#ifndef REGAL_FORCE_EMU_XFER
#define REGAL_FORCE_EMU_XFER 0
#endif

#ifndef REGAL_FORCE_EMU_DSA
#define REGAL_FORCE_EMU_DSA 0
#endif

#ifndef REGAL_FORCE_EMU_IFF
#define REGAL_FORCE_EMU_IFF 0
#endif

#ifndef REGAL_FORCE_EMU_SO
#define REGAL_FORCE_EMU_SO 0
#endif

#ifndef REGAL_FORCE_EMU_VAO
#define REGAL_FORCE_EMU_VAO 0
#endif

#ifndef REGAL_FORCE_EMU_FILTER
#define REGAL_FORCE_EMU_FILTER 0
#endif

#ifndef REGAL_FORCE_EMU_TEXC
#define REGAL_FORCE_EMU_TEXC 0
#endif

// RegalBreak callbacks supported by default, except in release mode

#ifndef REGAL_BREAK
# if defined(NDEBUG)
#  define REGAL_BREAK 0
# else
#  define REGAL_BREAK 1
# endif
#endif

// Caching supported by default
// ... except for release-mode and embedded platforms

#ifndef REGAL_CACHE
# if defined(NDEBUG) || REGAL_SYS_IOS || REGAL_SYS_PPAPI || REGAL_SYS_ANDROID
#  define REGAL_CACHE 0
# else
#  define REGAL_CACHE 1
# endif
#endif

// Shader caching supported by default

#ifndef REGAL_CACHE_SHADER
#define REGAL_CACHE_SHADER REGAL_CACHE
#endif

// Cache writing supported by default

#ifndef REGAL_CACHE_SHADER_WRITE
#define REGAL_CACHE_SHADER_WRITE REGAL_CACHE_SHADER
#endif

// Cache reading supported by default

#ifndef REGAL_CACHE_SHADER_READ
#define REGAL_CACHE_SHADER_READ REGAL_CACHE_SHADER
#endif

//

#ifndef REGAL_FORCE_CORE_PROFILE
#define REGAL_FORCE_CORE_PROFILE 0
#endif

#ifndef REGAL_FORCE_ES1_PROFILE
#define REGAL_FORCE_ES1_PROFILE 0
#endif

#ifndef REGAL_FORCE_ES2_PROFILE
#define REGAL_FORCE_ES2_PROFILE 0
#endif

#if (REGAL_FORCE_CORE_PROFILE + REGAL_FORCE_ES1_PROFILE + REGAL_FORCE_ES2_PROFILE) > 1
#error Can not force more than one mode: REGAL_FORCE_CORE_PROFILE, REGAL_FORCE_ES1_PROFILE, REGAL_FORCE_ES2_PROFILE
#endif

//

#ifndef REGAL_STATIC_ES2
#define REGAL_STATIC_ES2 0
#endif

#ifndef REGAL_STATIC_EGL
#define REGAL_STATIC_EGL 0
#endif

// Defaults for REGAL_NO_...

#ifndef REGAL_NO_ASSERT
#  if defined(NDEBUG)
#    define REGAL_NO_ASSERT 1
#  else
#    define REGAL_NO_ASSERT 0
#  endif
#endif

#ifndef REGAL_NO_PNG
#  define REGAL_NO_PNG 0
#endif

#ifndef REGAL_NO_SQUISH
#  define REGAL_NO_SQUISH 1
#endif

#ifndef REGAL_NO_TLS
#  define REGAL_NO_TLS 0
#endif

// AssertFunction depends on Error log, but
// ideally we wouldn't depend on RegalLog.h here

REGAL_GLOBAL_BEGIN

#include <string>

#include "RegalLog.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

inline const char * GetEnv(const char * const varname)
{
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

    return std::getenv(varname);

#ifdef _MSC_VER
#pragma warning (pop)
#endif
}

//
// RegalCheckGLError
//

#ifdef NDEBUG
#  define RegalCheckGLError( ctx )
#else
#  define RegalCheckGLError( ctx ) ::REGAL_NAMESPACE_INTERNAL::Init::checkForGLErrors(ctx)
#endif

//
// RegalAssert
//

#if REGAL_NO_ASSERT
#  define RegalAssert( foo )
#else
#  define RegalAssert( foo ) if (!(foo)) ::REGAL_NAMESPACE_INTERNAL::AssertFunction( __FILE__ , __LINE__ , #foo);
#endif

#if !REGAL_NO_ASSERT
void AssertFunction(const char *file, const std::size_t line, const char *expr);
#endif

//
//
//

struct RegalContext;

void RegalCheckForGLErrors( RegalContext * ctx );

void *GetProcAddress(const char *entry);

template <typename T>
T *GetProcAddress(T *&f, const char *entry )
{
  f = reinterpret_cast<T *>(GetProcAddress(entry));
  return f;
}

// Lightweight boost::algorithm::string::starts_with

inline bool starts_with(const std::string &input, const std::string &test) { return std::strncmp(input.c_str(),test.c_str(),test.length())==0; }
inline bool starts_with(const std::string &input, const char * const test) { return std::strncmp(input.c_str(),test,        strlen(test) )==0; }

// Mac OSX lacks strndup

inline char *
strndup(const char *str, size_t n)
{
  char *tmp = static_cast<char *>(malloc(n+1));
  if (n)
    memcpy(tmp,str,n);
  tmp[n] = '\0';
  return tmp;
}

//

extern bool  fileExists(const char *filename);
extern FILE *fileOpen  (const char *filename, const char *mode);
extern void  fileClose (FILE **file);

//

extern std::string fileRead(FILE *file);

//

extern std::string makePath(const std::string &dir, const std::string &filename);

// X11 stuff

#if REGAL_SYS_X11
extern std::string windowManagerStateDescription(Display *display, Window w);
#endif

// ToFloat for integer -> float

template <bool Norm, typename T> inline float ToFloat(const T v ) {
  return float(v);
}

template <bool Norm> inline float ToFloat(const signed char v ) {
  return Norm ? float(v)/float(std::numeric_limits<signed char>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const unsigned char v ) {
  return Norm ? float(v)/float(std::numeric_limits<unsigned char>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const signed short v ) {
  return Norm ? float(v)/float(std::numeric_limits<signed short>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const unsigned short v ) {
  return Norm ? float(v)/float(std::numeric_limits<unsigned short>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const signed int v ) {
  return Norm ? float(v)/float(std::numeric_limits<signed int>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const unsigned int v ) {
  return Norm ? float(v)/float(std::numeric_limits<unsigned int>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const signed long int v ) {
  return Norm ? float(v)/float(std::numeric_limits<signed long int>::max()) : float(v);
}

template <bool Norm> inline float ToFloat(const unsigned long int v ) {
  return Norm ? float(v)/float(std::numeric_limits<unsigned long int>::max()) : float(v);
}

// ToDouble for integer -> double

template <bool Norm, typename T> inline double ToDouble(const T v ) {
  return double(v);
}

template <bool Norm> inline double ToDouble(const signed char v ) {
  return Norm ? double(v)/double(std::numeric_limits<signed char>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const unsigned char v ) {
  return Norm ? double(v)/double(std::numeric_limits<unsigned char>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const signed short v ) {
  return Norm ? double(v)/double(std::numeric_limits<signed short>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const unsigned short v ) {
  return Norm ? double(v)/double(std::numeric_limits<unsigned short>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const signed int v ) {
  return Norm ? double(v)/double(std::numeric_limits<signed int>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const unsigned int v ) {
  return Norm ? double(v)/double(std::numeric_limits<unsigned int>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const signed long int v ) {
  return Norm ? double(v)/double(std::numeric_limits<signed long int>::max()) : double(v);
}

template <bool Norm> inline double ToDouble(const unsigned long int v ) {
  return Norm ? double(v)/double(std::numeric_limits<unsigned long int>::max()) : double(v);
}

REGAL_NAMESPACE_END

#endif
