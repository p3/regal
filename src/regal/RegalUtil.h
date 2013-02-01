/*
  Copyright (c) 2011-2012 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
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

// Visual Studio with /W4 complains about unused function
// parameters.

#ifndef UNUSED_PARAMETER
#ifdef _MSC_VER
#define UNUSED_PARAMETER(p) (p)
#else
#define UNUSED_PARAMETER(p)
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

#ifdef _MSC_VER
# define REGAL_GLOBAL_BEGIN         \
  __pragma(pack(push))              \
  __pragma(pack(8))                 \
  __pragma(warning(push))           \
  __pragma(warning(disable : 4127)) \
  __pragma(warning(disable : 4512)) \
  __pragma(warning(disable : 4996))
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

// Debug dispatch disabled by default in
// release mode

#ifndef REGAL_DEBUG
#ifdef NDEBUG
#define REGAL_DEBUG 0
#else
#define REGAL_DEBUG 1
#endif
#endif

// Error dispatch disabled by default in
// release mode

#ifndef REGAL_ERROR
#ifdef NDEBUG
#define REGAL_ERROR 0
#else
#define REGAL_ERROR 1
#endif
#endif

// Code logging disabled by default in
// release mode, or embedded

#ifndef REGAL_CODE
# if defined(NDEBUG) || REGAL_SYS_IOS || REGAL_SYS_PPAPI || REGAL_SYS_ANDROID
#  define REGAL_CODE 0
# else
#  define REGAL_CODE 0
# endif
#endif

// Emulation dispatch enabled by default

#ifndef REGAL_EMULATION
#define REGAL_EMULATION 1
#endif

// Driver dispatch enabled by default

#ifndef REGAL_DRIVER
#define REGAL_DRIVER 1
#endif

#ifndef REGAL_LOG
#define REGAL_LOG 1
#endif

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

//

// Caching enabled by default
// ... except for release-mode and embedded platforms

#ifndef REGAL_CACHE
# if defined(NDEBUG) || REGAL_SYS_IOS || REGAL_SYS_PPAPI || REGAL_SYS_ANDROID
#  define REGAL_CACHE 0
# else
#  define REGAL_CACHE 1
# endif
#endif


// Shader caching support by default

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

#ifndef REGAL_FORCE_ES2_PROFILE
#define REGAL_FORCE_ES2_PROFILE 0
#endif

#if REGAL_FORCE_CORE_PROFILE && REGAL_FORCE_ES2_PROFILE
#error Both core and ES2 profiles can not be forced: REGAL_FORCE_CORE_PROFILE && REGAL_FORCE_ES2_PROFILE
#endif

//

#ifndef REGAL_STATIC_ES2
#define REGAL_STATIC_ES2 0
#endif

#ifndef REGAL_STATIC_EGL
#define REGAL_STATIC_EGL 0
#endif

#ifndef REGAL_EMU_TEXC
#if REGAL_SYS_PPAPI
#define REGAL_EMU_TEXC 1
#else
#define REGAL_EMU_TEXC 0
#endif
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

#if defined(NDEBUG) && !defined(REGAL_NO_ASSERT)
#  define REGAL_NO_ASSERT
#endif

#ifdef REGAL_NO_ASSERT
#  define RegalAssert( foo )
#else
#  define RegalAssert( foo ) if (!(foo)) ::REGAL_NAMESPACE_INTERNAL::AssertFunction( __FILE__ , __LINE__ , #foo);
#endif

#ifndef REGAL_NO_ASSERT
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

extern std::string makePath(const std::string &dir, const std::string &filename);

//

extern std::string fileRead(FILE *file);

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

REGAL_NAMESPACE_END

#endif
