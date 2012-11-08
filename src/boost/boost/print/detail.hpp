/*
 * Copyright (c) 2010-2012 NVIDIA Corporation
 * Copyright (c) 2010-2012 Nigel Stewart
 * All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost::print is a library for string conversion.
 * boost::print is not currently a part of boost.
 */

#ifndef BOOST_PRINT_DETAIL_HPP
#define BOOST_PRINT_DETAIL_HPP

#if defined(_MSC_VER) && (_MSC_VER>=1200)
#pragma once
#endif

#include <cstddef>  // diffptr_t, size_t
#include <cstring>  // memcpy
#include <cassert>  // assert

#include <cstdarg>  // args for float and double, for now
#include <cstdio>   // sprintf for float and double, for now
#include <cstring>  // strlen for sprintf length

#include <string>

#include <boost/cstdint.hpp>

#if defined(__SUNPRO_CC) && defined(__SunOS)
#include <stdio.h>
#endif

// for __ARM_ARCH__

#if defined(__ANDROID__) && !defined(__i386__)
#include <machine/cpu-features.h>
#endif

//
// boost::print::detail namespace for internal implementation purposes
//

namespace boost { namespace print { namespace detail {

using std::size_t;

// Binary formatting of integers

template<typename T>
struct binary
{
  binary(const T &val) : _val(&val) {}
  binary<T> &operator=(const binary<T> &other) { _val = other._val; }
  const T * const _val;
};

// Octal formatting of integers

template<typename T>
struct octal
{
  octal(const T &val) : _val(&val) {}
  octal<T> &operator=(const octal<T> &other) { _val = other._val; }
  const T * const _val;
};

// Hex formatting of integers

template<typename T>
struct hex
{
  hex(const T &val, const bool upperCase = false)
  : _val(&val), _upperCase(upperCase)
  {
  }

  hex<T> &operator=(const hex<T> &other) { _val = other._val; _upperCase = other._upperCase; }

  const T * const _val;
  bool            _upperCase;
};

// Left and right alignment of generic types

template<typename T, typename C = char>
struct left
{
  left(const T &val, const size_t width, const C padding = ' ')
  : _val(&val), _width(width), _padding(padding)
  {
  }

  left<T,C> &operator=(const left<T,C> &other) { _val = other._val; _width = other._width; _padding = other._padding; }

  const T * const _val;
  size_t          _width;
  C               _padding;
};

template<typename T, typename C = char>
struct right
{
  right(const T &val, const size_t width, const C padding = ' ')
  : _val(&val), _width(width), _padding(padding)
  {
  }

  right<T,C> &operator=(const right<T,C> &other) { _val = other._val; _width = other._width; _padding = other._padding; }

  const T * const _val;
  size_t          _width;
  C               _padding;
};

// Quoting

template<typename T, typename C = char>
struct quote
{
  quote(const T &val, const C q = '"')
  : _val(&val), _q(q)
  {
  }

  const T * const _val;
  C               _q;

private:
  quote();
  quote<T,C> &operator=(const quote<T,C> &other);
};

// Array printing

template<typename T, typename U>
struct array
{
  array(const T *data, const size_t size, const U &quote, const U &open, const U &close, const U &delim)
  : _data(data), _size(size), _quote(quote), _open(open), _close(close), _delim(delim) {}

  const T *_data;
  size_t   _size;

  const U _quote;
  const U _open;
  const U _close;
  const U _delim;

private:
  array();
  array<T,U> &operator=(const array<T,U> &other);
};

// Iterator printing for containers

template<typename T, typename U>
struct iterator
{
  iterator(const T &begin, const T &end, const U &quote, const U &open, const U &close, const U &delim)
  : _begin(begin), _end(end), _quote(quote), _open(open), _close(close), _delim(delim) {}

  const T &_begin;
  const T &_end;

  const U _quote;
  const U _open;
  const U _close;
  const U _delim;

private:
  iterator();
  iterator<T,U> &operator=(const iterator<T,U> &other);
};

// Print the first n delimited items
//
// str    - input string
// delim  - delimiter character, such as \n
// n      - Maximum number of items to output
// prefix - Per-line prefix
// suffix - Output suffix for trimmed lists

template<typename T,typename U>
struct trim
{
  trim(const T &str, const U delim, const size_t n, const T &prefix, const T &suffix)
  : _str(str), _delim(delim), _n(n), _prefix(prefix), _suffix(suffix) {}

  const T       _str;
  const U       _delim;
  const size_t  _n;
  const T       _prefix;
  const T       _suffix;

private:
  trim();
  trim<T,U> &operator=(const iterator<T,U> &other);
};

// Determine the number of digits of an unsigned integer
// Use binary search to minimise comparisons.

// 8-bit unsigned integer

inline size_t unsigned_length(const boost::uint8_t val)
{
  return val<10 ? 1 : (val<100 ? 2 : 3);
}

// 16-bit unsigned integer

inline size_t unsigned_length(const boost::uint16_t val)
{
  if (val<1000)
    return val<10 ? 1 : (val<100 ? 2 : 3);
  else
    return val<10000 ? 4 : 5;
}

// 32-bit unsigned integer

inline size_t unsigned_length(const boost::uint32_t val)
{
  if (val<10000)
    return val<100 ? (val<10 ? 1 : 2) : (val<1000 ? 3 : 4);
  else
    return val<10000000 ?
      ( val < 100000 ? 5 : (val<1000000 ? 6 : 7)) :
      ( val < 100000000 ? 8 : (val<1000000000 ? 9 : 10));
}

// 64-bit unsigned integer

inline size_t unsigned_length(const boost::uint64_t val)
{
  if (val<100000000ll)
  {
    if (val<10000)
      return val<100 ? (val<10 ? 1 : 2) : (val<1000 ? 3 : 4);
    else
      return val<1000000 ? (val<100000 ? 5 : 6) : (val<10000000 ? 7 : 8);
  }
  else
  {
    if (val<1000000000000ll)
      return val<10000000000ll ? (val<1000000000ll ? 9 : 10) : (val<100000000000ll ? 11 : 12);
    else
    {
      if (val<10000000000000000ll)
        return val<100000000000000ll ? (val<10000000000000ll ? 13 : 14) : (val<1000000000000000ll ? 15 : 16);
      else
        return val<1000000000000000000ll ? (val<100000000000000000ll ? 17 : 18) : (val<10000000000000000000ull ? 19 : 20);
    }
  }
}

#if defined(__APPLE__) && defined(__i386)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__APPLE__) && defined(__x86_64)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint64_t>(val)); }
#endif

#if defined(__APPLE__) && defined(__ppc__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__APPLE__) && defined(__arm__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__native_client__) && (defined(__i386) || defined(__x86_64) || defined(__arm__))
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__ANDROID__) && defined(__i386__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__ANDROID__) && defined(__ARM_ARCH__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__linux) && defined(__i386) && !defined(__native_client__) && !defined(__ANDROID__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(__linux) && defined(__x86_64) && !defined(__native_client__) && !defined(__ANDROID__)
inline size_t unsigned_length(const unsigned long long val) { return unsigned_length(static_cast<boost::uint64_t>(val)); }
#endif

#if defined(__linux) && defined(__arm__) && !defined(__native_client__) && !defined(__ANDROID__)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if (defined(__SUNPRO_C) || defined(__SUNPRO_CC) || defined(__sun)) && defined(__i386)
inline size_t unsigned_length(const unsigned int val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if (defined(__SUNPRO_C) || defined(__SUNPRO_CC) || defined(__sun)) && defined(__x86_64)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint64_t>(val)); }
#endif

#if defined(WIN32) && !defined(_WIN64)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint32_t>(val)); }
#endif

#if defined(WIN32) && defined(_WIN64)
inline size_t unsigned_length(const unsigned long val) { return unsigned_length(static_cast<boost::uint64_t>(val)); }
#endif

// Determine the number of digits of a signed integer

template<typename U, typename S>
inline size_t signed_length(S val)
{
  const bool negative = val<0;
  if (negative)
    val = -val;
  return unsigned_length(static_cast<U>(val)) + (negative ? 1 : 0);
}

inline size_t sprintf_length(const char *fmt, ...)
{
  std::va_list args;
  va_start(args,fmt);
  char buffer[1024];
  ::std::vsprintf(buffer,fmt,args);
  va_end(args);
  return ::std::strlen(buffer);
}

// Output unsigned integer

template<typename Iterator, typename T>
inline void write_unsigned(Iterator &i, T val)
{
  if (val==0) {
    *i = '0';
    ++i;
    return;
  }

  const size_t len = unsigned_length(val);

  i += len;
  while (val) {
    --i;
    *i = '0' + static_cast<unsigned char>(val%10);
    val /= 10;
  }
  i += len;
}

// Output signed integer
// Corresponding unsigned type U needs to be specified

template<typename U, typename Iterator, typename S>
inline void write_signed(Iterator &i, S val)
{
  if (val<0) {
    *i = '-';
    ++i;
    val = -val;
  }

  write_unsigned(i,static_cast<U>(val));
}

template<typename Iterator>
inline size_t write_sprintf(Iterator &i, const char *fmt, ...)
{
  std::va_list args;
  va_start(args,fmt);
  vsprintf(&(*i),fmt,args);
  va_end(args);
  const ::std::size_t len = ::std::strlen(&(*i));
  i += len;
  return len;
}

// Output string of binary zeros and ones

template<typename Iterator, typename T> inline
void write_binary(Iterator &i, const T &val, const size_t n)
{
  for (size_t j=0; j<n; ++j, ++i)
    *i = (val>>(n-1-j))&1 ? '1' : '0';
}

// Output string of octal 01234567 digits

template<typename Iterator, typename T> inline
void write_octal(Iterator &i, const T &val, const size_t n)
{
  for (size_t j=0; j<n; ++j, ++i)
  {
    const T oct = static_cast<T>((val>>((n-1-j)*3))&7);
    *i = static_cast<unsigned char>('0' + oct);
  }
}

// Output string of hexadecimal 0123456789ABCDEF digits

template<typename Iterator, typename T> inline
void write_hex(Iterator &i, const T &val, const size_t n, const bool upperCase)
{
  for (size_t j=0; j<n; ++j, ++i)
  {
    const T hex = static_cast<T>((val>>((n-1-j)<<2))&15);
    *i = static_cast<unsigned char>(hex<10 ? '0' + hex : (upperCase ? 'A' : 'a') + hex - 10);
  }
}

// Write characters from a [begin,end) iterator pair
// For example: std::string::begin(), std::string::end()

template<typename Iterator, typename ConstInputIterator>
inline void write_iterator(Iterator &i, const ConstInputIterator &begin, const ConstInputIterator &end)
{
  for (ConstInputIterator j=begin; j!=end; ++i,++j)
    *i = *j;
}

template<>
inline void write_iterator(std::string::iterator &i, const std::string::const_iterator &begin, const std::string::const_iterator &end)
{
  // Avoid dereferencing possibly invalid iterators for std::string

  if (begin!=end)
  {
    std::memcpy(reinterpret_cast<void *>(&(*i)),static_cast<const void *>(&(*begin)),end-begin);
    i += end-begin;
  }
}

}}}

#endif

