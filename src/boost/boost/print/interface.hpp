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

#ifndef BOOST_PRINT_INTERFACE_HPP
#define BOOST_PRINT_INTERFACE_HPP

#if defined(_MSC_VER) && (_MSC_VER>=1200)
#pragma once
#endif

#include <cstring>  // strlen
#include <algorithm>
#include <string>

#include <boost/print/detail.hpp>

namespace boost { namespace print {

template<typename T> detail::binary<T> binary(const T &val) { return detail::binary<T>(val); }
template<typename T> detail::octal<T>  octal(const T &val)  { return detail::octal<T>(val); }
template<typename T> detail::hex<T>    hex(const T &val, const bool upperCase = false) { return detail::hex<T>(val,upperCase); }

inline               detail::pad<char>     pad   (              const size_t width) { return detail::pad<char>(width,' '); }
template<typename T> detail::left<T,char>  left  (const T &val, const size_t width) { return detail::left<T,char>(val,width,' '); }
template<typename T> detail::right<T,char> right (const T &val, const size_t width) { return detail::right<T,char>(val,width,' '); }

template<            typename C> detail::pad<C>      pad   (              const size_t width, const C padding) { return detail::pad<C>(width,padding); }
template<typename T, typename C> detail::left<T,C>   left  (const T &val, const size_t width, const C padding) { return detail::left<T,C>(val,width,padding); }
template<typename T, typename C> detail::right<T,C>  right (const T &val, const size_t width, const C padding) { return detail::right<T,C>(val,width,padding); }

template<typename T, typename C> detail::quote<T,C>  quote (const T &val, const C q) { return detail::quote<T,C>(val,q); }

template<typename T>             detail::optional<T> optional(const T &val, const bool enabled) { return detail::optional<T>(val, enabled); }

template<typename T>
detail::array<T,const char * const> array(const T *data, const std::size_t size)
{
  static const char * const quote = "";
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = ", ";
  return detail::array<T,const char * const>(data,size,quote,open,close,delim);
}

template<typename T>
detail::array<T,const char * const> array(const T *data, const std::size_t size, const char *const quote)
{
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = ", ";
  return detail::array<T,const char * const>(data,size,quote,open,close,delim);
}

template<typename T,typename U>
detail::array<T,U> array(const T *data, const std::size_t size, const U &quote, const U &open, const U &close, const U &delim)
{ return detail::array<T,U>(data,size,quote,open,close,delim); }

inline
detail::raw<const char * const> raw(const void *data, const std::size_t size)
{
  static const char * const quote = "";
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = " ";
  return detail::raw<const char * const>(data,size,quote,open,close,delim);
}

template<typename T>
detail::raw<const char * const> raw(const T *data, const std::size_t size)
{
  static const char * const quote = "";
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = " ";
  return detail::raw<const char * const>(static_cast<const void *>(data),size,quote,open,close,delim);
}

template<typename T>
detail::iterator<T,const char * const> iterator(const T &begin, const T &end)
{
  static const char * const quote = "";
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = ", ";
  return detail::iterator<T,const char * const>(begin,end,quote,open,close,delim);
}

template<typename T,typename U>
detail::iterator<T,const char * const> iterator(const T &begin, const T &end, const char * const quote)
{
  static const char * const open  = "[ ";
  static const char * const close = " ]";
  static const char * const delim = ", ";
  return detail::iterator<T,U>(begin,end,quote,open,close,delim);
}

template<typename T,typename U>
detail::iterator<T,U> iterator(const T &begin, const T &end, const U &quote, const U &open, const U &close, const U &delim)
{
  return detail::iterator<T,U>(begin,end,quote,open,close,delim);
}

template<typename T,typename U>
detail::trim<T,U> trim(const T &str, const U delim, const std::size_t n, const T &prefix = T(""), const T &suffix = T(""))
{
  return detail::trim<T,U>(str,delim,n,prefix,suffix);
}

}}

//
// boost::print::extend namespace for implementing length and write per supported type
//

namespace boost { namespace print { namespace extend {

using ::std::size_t;
using ::std::strlen;

using ::boost::print::detail::binary;
using ::boost::print::detail::octal;
using ::boost::print::detail::hex;
using ::boost::print::detail::pad;
using ::boost::print::detail::left;
using ::boost::print::detail::right;
using ::boost::print::detail::quote;
using ::boost::print::detail::optional;
using ::boost::print::detail::array;
using ::boost::print::detail::raw;
using ::boost::print::detail::iterator;
using ::boost::print::detail::trim;

using ::boost::print::detail::signed_length;
using ::boost::print::detail::unsigned_length;
using ::boost::print::detail::sprintf_length;
using ::boost::print::detail::write_signed;
using ::boost::print::detail::write_unsigned;
using ::boost::print::detail::write_sprintf;
using ::boost::print::detail::write_binary;
using ::boost::print::detail::write_octal;
using ::boost::print::detail::write_hex;
using ::boost::print::detail::write_iterator;

// Determine the length (number of characters) for various types

inline size_t length(bool               val) { return val ? 4 : 5; /* true or false */ }

inline size_t length(unsigned char      val) { return unsigned_length(val); }
inline size_t length(unsigned short     val) { return unsigned_length(val); }
inline size_t length(unsigned int       val) { return unsigned_length(val); }
inline size_t length(unsigned long      val) { return unsigned_length(val); }
inline size_t length(unsigned long long val) { return unsigned_length(val); }

inline size_t length(signed char      val) { return signed_length<unsigned char     >(val); }
inline size_t length(signed short     val) { return signed_length<unsigned short    >(val); }
inline size_t length(signed int       val) { return signed_length<unsigned int      >(val); }
inline size_t length(signed long      val) { return signed_length<unsigned long     >(val); }
inline size_t length(signed long long val) { return signed_length<unsigned long long>(val); }

inline size_t length(char             )  { return 1;            }
inline size_t length(const char   *val)  { return strlen(val);  }

inline size_t length(const std::string &val) { return val.length(); }

// Relying on sprintf for float and double, for now

inline size_t length(const float   val) { return sprintf_length("%g",val); }
inline size_t length(const double  val) { return sprintf_length("%g",val); }

// For binary output the length is the number of bytes times eight.

inline size_t length(const binary<char>             &) { return sizeof(char)<<3; }

inline size_t length(const binary<signed char>      &) { return sizeof(signed char     )<<3; }
inline size_t length(const binary<signed short>     &) { return sizeof(signed short    )<<3; }
inline size_t length(const binary<signed int>       &) { return sizeof(signed int      )<<3; }
inline size_t length(const binary<signed long>      &) { return sizeof(signed long     )<<3; }
inline size_t length(const binary<signed long long> &) { return sizeof(signed long long)<<3; }

inline size_t length(const binary<unsigned char>      &) { return sizeof(unsigned char     )<<3; }
inline size_t length(const binary<unsigned short>     &) { return sizeof(unsigned short    )<<3; }
inline size_t length(const binary<unsigned int>       &) { return sizeof(unsigned int      )<<3; }
inline size_t length(const binary<unsigned long>      &) { return sizeof(unsigned long     )<<3; }
inline size_t length(const binary<unsigned long long> &) { return sizeof(unsigned long long)<<3; }

// For octal output the length is the number of bits divided by three

inline std::size_t octalDigits(const std::size_t bytes) { return ((bytes<<3)+2)/3; }

inline size_t length(const octal<char>               &) { return octalDigits(sizeof(char              )); }

inline size_t length(const octal<signed char>        &) { return octalDigits(sizeof(signed char       )); }
inline size_t length(const octal<signed short>       &) { return octalDigits(sizeof(signed short      )); }
inline size_t length(const octal<signed int>         &) { return octalDigits(sizeof(signed int        )); }
inline size_t length(const octal<signed long>        &) { return octalDigits(sizeof(signed long       )); }
inline size_t length(const octal<signed long long>   &) { return octalDigits(sizeof(signed long long  )); }

inline size_t length(const octal<unsigned char>      &) { return octalDigits(sizeof(unsigned char     )); }
inline size_t length(const octal<unsigned short>     &) { return octalDigits(sizeof(unsigned short    )); }
inline size_t length(const octal<unsigned int>       &) { return octalDigits(sizeof(unsigned int      )); }
inline size_t length(const octal<unsigned long>      &) { return octalDigits(sizeof(unsigned long     )); }
inline size_t length(const octal<unsigned long long> &) { return octalDigits(sizeof(unsigned long long)); }

// For hex output the length is the number of bytes times two.

inline size_t length(const hex<char>              &) { return sizeof(char)<<1; }

inline size_t length(const hex<signed char>       &) { return sizeof(signed char) <<1; }
inline size_t length(const hex<signed short>      &) { return sizeof(signed short)<<1; }
inline size_t length(const hex<signed int>        &) { return sizeof(signed int)  <<1; }
inline size_t length(const hex<signed long>       &) { return sizeof(signed long) <<1; }
inline size_t length(const hex<signed long long>  &) { return sizeof(signed long long) <<1; }

inline size_t length(const hex<unsigned char>      &) { return sizeof(unsigned char     )<<1; }
inline size_t length(const hex<unsigned short>     &) { return sizeof(unsigned short    )<<1; }
inline size_t length(const hex<unsigned int>       &) { return sizeof(unsigned int      )<<1; }
inline size_t length(const hex<unsigned long>      &) { return sizeof(unsigned long     )<<1; }
inline size_t length(const hex<unsigned long long> &) { return sizeof(unsigned long long)<<1; }

inline size_t length(const void *) { return (sizeof(void *)<<1); }

// Pad

template<typename C> size_t length(const pad<C> &val) { return val._width; }

// Left and right

template<typename T, typename C> size_t length(const left<T,C>   &val) { return std::max(length(*val._val),val._width); }
template<typename T, typename C> size_t length(const right<T,C>  &val) { return std::max(length(*val._val),val._width); }

// Quoting

template<typename T, typename C> size_t length(const quote<T,C>  &val) { return *val._val ? length(*val._val) + 2*length(val._q) : 4; /* NULL */ }

// Optional

template<typename T>             size_t length(const optional<T> &val) { return val._enabled ? length(val._val) : 0; }

// Array

template<typename T, typename U> size_t length(const array<T,U> &val)
{
  if (!val._data)
      return 4;   // NULL

  size_t len = length(val._open) + length(val._close) + length(val._quote)*val._size*2;

  if (val._size)
  {
    const size_t dl = length(val._delim);

    len += length(val._data[0]);
    for (size_t j=1; j<val._size; ++j)
      len += dl + length(val._data[j]);
  }

  return len;
}

// Raw

template<typename U> size_t length(const raw<U> &val)
{
  if (val._data)
  {
    const size_t words = (val._size+3)/4;
    size_t len = length(val._open) + length(val._close);
    len += length(val._quote)*2*words;
    len += val._size*2;
    if (words>1)
      len += (words-1)*length(val._delim);
    return len;
  }

  return 4; // NULL
}

// Iterator

template<typename T, typename U> size_t length(const iterator<T,U> &val)
{
  size_t len = length(val._open) + length(val._close);

  if (val._begin!=val._end)
  {
    const size_t dl = length(val._delim);
    const size_t ql = length(val._quote)*2;
    T i = val._begin;
    len += length(*i) + ql;
    for (++i; i!=val._end; ++i)
      len += dl + length(*i) + ql;
  }

  return len;
}

// Trim

template<typename T, typename U> size_t length(const trim<T,U> &val)
{
  size_t   n = val._n;
  const U *i = &val._str[0];

  if (!val._n || !i)
    return 0;

  const size_t pLength = length(val._prefix);  // per-line prefix length

  size_t len = pLength;
  while (*i)
  {
    if (*i==val._delim)
    {
      if ((--n)==0)
        return len + length(val._suffix);
      len += pLength;
    }

    len++;
    ++i;
  }
  return len;
}

// Write to a std::string (or similar) for various types

template<typename Iterator> inline void write(Iterator &i, const bool           val) { if (val) { *(i++) = 't'; *(i++) = 'r'; *(i++) = 'u'; *(i++) = 'e'; } else { *(i++) = 'f'; *(i++) = 'a'; *(i++) = 'l'; *(i++) = 's'; *(i++) = 'e'; } }

template<typename Iterator> inline void write(Iterator &i, const char           val) { *i = val; ++i; }

template<typename Iterator> inline void write(Iterator &i, const signed char      val) { write_signed<unsigned char     >(i,val); }
template<typename Iterator> inline void write(Iterator &i, const signed int       val) { write_signed<unsigned int      >(i,val); }
template<typename Iterator> inline void write(Iterator &i, const signed short     val) { write_signed<unsigned short    >(i,val); }
template<typename Iterator> inline void write(Iterator &i, const signed long      val) { write_signed<unsigned long     >(i,val); }
template<typename Iterator> inline void write(Iterator &i, const signed long long val) { write_signed<unsigned long long>(i,val); }

template<typename Iterator> inline void write(Iterator &i, const unsigned char      val) { write_unsigned(i,val); }
template<typename Iterator> inline void write(Iterator &i, const unsigned int       val) { write_unsigned(i,val); }
template<typename Iterator> inline void write(Iterator &i, const unsigned short     val) { write_unsigned(i,val); }
template<typename Iterator> inline void write(Iterator &i, const unsigned long      val) { write_unsigned(i,val); }
template<typename Iterator> inline void write(Iterator &i, const unsigned long long val) { write_unsigned(i,val); }

template<typename Iterator> inline
void write(Iterator &i, const char *val)
{
  const char *j = val;
  for (; *j; ++j)
    *i = *j;

  i += (j-val);
}

template<> inline
void write(char *&i, const char *val)
{
  const ::std::size_t size = ::std::strlen(val);  // We waste ~33% time here in duplicate strlen
  ::std::memcpy(i,val,size);
  i += size;
}

// Unless it's STLPort we can specialize for std::string::iterator

#ifndef _STLPORT_VERSION
template<> inline
void write(std::string::iterator &i, const char *val)
{
  // if i==string::end() it's a runtime error on Windows VC10
  // to dereference

  if (val[0]!='\0')
  {
    char *j = &(*i);
    write<char *>(j,val);
    i += j-&(*i);
  }
}
#endif

template<typename Iterator> inline void write(Iterator &i, const ::std::string &val) { write_iterator(i,val.begin(),val.end()); }

template<typename Iterator> inline
void write(Iterator &i, const float val)
{
  write_sprintf(i,"%g",val);
}

template<typename Iterator> inline
void write(Iterator &i, const double val)
{
  write_sprintf(i,"%g",val);
}

// Binary output for known integer types

template<typename Iterator> inline void write(Iterator &i, const binary<char> &val)           { write_binary(i,*val._val,sizeof(char)<<3); }

template<typename Iterator> inline void write(Iterator &i, const binary<signed char>      &val) { write_binary(i,*val._val,sizeof(signed char     )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<signed short>     &val) { write_binary(i,*val._val,sizeof(signed short    )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<signed int>       &val) { write_binary(i,*val._val,sizeof(signed int      )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<signed long>      &val) { write_binary(i,*val._val,sizeof(signed long     )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<signed long long> &val) { write_binary(i,*val._val,sizeof(signed long long)<<3); }

template<typename Iterator> inline void write(Iterator &i, const binary<unsigned char     > &val) { write_binary(i,*val._val,sizeof(unsigned char     )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<unsigned short    > &val) { write_binary(i,*val._val,sizeof(unsigned short    )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<unsigned int      > &val) { write_binary(i,*val._val,sizeof(unsigned int      )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<unsigned long     > &val) { write_binary(i,*val._val,sizeof(unsigned long     )<<3); }
template<typename Iterator> inline void write(Iterator &i, const binary<unsigned long long> &val) { write_binary(i,*val._val,sizeof(unsigned long long)<<3); }

// Octal output for known integer types

template<typename Iterator> inline void write(Iterator &i, const octal<char>               &val) { write_octal(i,*val._val,octalDigits(sizeof(char))); }

template<typename Iterator> inline void write(Iterator &i, const octal<signed char>        &val) { write_octal(i,*val._val,octalDigits(sizeof(signed char     ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<signed short>       &val) { write_octal(i,*val._val,octalDigits(sizeof(signed short    ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<signed int>         &val) { write_octal(i,*val._val,octalDigits(sizeof(signed int      ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<signed long>        &val) { write_octal(i,*val._val,octalDigits(sizeof(signed long     ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<signed long long>   &val) { write_octal(i,*val._val,octalDigits(sizeof(signed long long))); }

template<typename Iterator> inline void write(Iterator &i, const octal<unsigned char>      &val) { write_octal(i,*val._val,octalDigits(sizeof(unsigned char     ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<unsigned short>     &val) { write_octal(i,*val._val,octalDigits(sizeof(unsigned short    ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<unsigned int>       &val) { write_octal(i,*val._val,octalDigits(sizeof(unsigned int      ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<unsigned long>      &val) { write_octal(i,*val._val,octalDigits(sizeof(unsigned long     ))); }
template<typename Iterator> inline void write(Iterator &i, const octal<unsigned long long> &val) { write_octal(i,*val._val,octalDigits(sizeof(unsigned long long))); }

// Hexadecimal output for known integer types

template<typename Iterator> inline void write(Iterator &i, const hex<char> &val)           { write_hex(i,*val._val,sizeof(char)<<1,val._upperCase); }

template<typename Iterator> inline void write(Iterator &i, const hex<signed char>      &val) { write_hex(i,*val._val,sizeof(signed char     )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<signed short>     &val) { write_hex(i,*val._val,sizeof(signed short    )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<signed int>       &val) { write_hex(i,*val._val,sizeof(signed int      )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<signed long>      &val) { write_hex(i,*val._val,sizeof(signed long     )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<signed long long> &val) { write_hex(i,*val._val,sizeof(signed long long)<<1,val._upperCase); }

template<typename Iterator> inline void write(Iterator &i, const hex<unsigned char>      &val) { write_hex(i,*val._val,sizeof(unsigned char     )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<unsigned short>     &val) { write_hex(i,*val._val,sizeof(unsigned short    )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<unsigned int>       &val) { write_hex(i,*val._val,sizeof(unsigned int      )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<unsigned long>      &val) { write_hex(i,*val._val,sizeof(unsigned long     )<<1,val._upperCase); }
template<typename Iterator> inline void write(Iterator &i, const hex<unsigned long long> &val) { write_hex(i,*val._val,sizeof(unsigned long long)<<1,val._upperCase); }

template<typename Iterator> inline void write(Iterator &i, const void *val)
{
  write_hex(i,reinterpret_cast<size_t>(val),sizeof(size_t)<<1,false);
}

// Pad

template<typename Iterator,typename C>
inline void write(Iterator &i, const pad<C> &val)
{
  for (size_t j=0; j<val._width; ++j)
    write(i,val._padding);
}

// Left and right

template<typename Iterator, typename T, typename C>
inline void write(Iterator &i, const left<T,C> &val)
{
  write(i,*val._val);
  const size_t len = length(*val._val);
  for (size_t j=len; j<val._width; ++j)
    write(i,val._padding);
}

template<typename Iterator, typename T, typename C>
inline void write(Iterator &i, const right<T,C> &val)
{
  const size_t len = length(*val._val);
  for (size_t j=len; j<val._width; ++j)
    write(i,val._padding);
  write(i,*val._val);
}

// Quote

template<typename Iterator, typename T, typename C>
inline void write(Iterator &i, const quote<T,C> &val)
{
  if (*val._val)
  {
    write(i,val._q);
    write(i,*val._val);
    write(i,val._q);
  }
  else
    write(i,"NULL");
}

// Optional

template<typename Iterator,typename T>
inline void write(Iterator &i, const optional<T> &val)
{
  if (val._enabled)
    write(i,val._val);
}

// Array

template<typename Iterator, typename T, typename U>
inline void write(Iterator &i, const array<T,U> &val)
{
  if (val._data)
  {
    write(i,val._open);
    if (val._size)
    {
      write(i,val._quote);
      write(i,val._data[0]);
      write(i,val._quote);
      for (size_t j=1; j<val._size; ++j)
      {
        write(i,val._delim);
        write(i,val._quote);
        write(i,val._data[j]);
        write(i,val._quote);
      }
    }
    write(i,val._close);
  }
  else
    write(i,"NULL");
}

// Raw

template<typename Iterator, typename U>
inline void write(Iterator &i, const raw<U> &val)
{
  write(i,val._open);
  if (val._size)
  {
    const uint8_t *k = (const uint8_t *) val._data;

    for (size_t j=0; j<val._size;)
    {
      if (j>0)
        write(i,val._delim);
      write(i,val._quote);
      write(i,hex<uint8_t>(k[j++]));
      if (j<val._size) write(i,hex<uint8_t>(k[j++]));
      if (j<val._size) write(i,hex<uint8_t>(k[j++]));
      if (j<val._size) write(i,hex<uint8_t>(k[j++]));
      write(i,val._quote);
    }
  }
  write(i,val._close);
}

// Iterator

template<typename Iterator, typename T, typename U>
inline void write(Iterator &i, const iterator<T,U> &val)
{
  write(i,val._open);
  if (val._begin!=val._end)
  {
    T j = val._begin;

    write(i,val._quote);
    write(i,*j);
    write(i,val._quote);

    for (++j; j!=val._end; ++j)
    {
      write(i,val._delim);
      write(i,val._quote);
      write(i,*j);
      write(i,val._quote);
    }
  }
  write(i,val._close);
}

// Trim

template<typename Iterator, typename T, typename U>
inline void write(Iterator &i, const trim<T,U> &val)
{
  size_t   n = val._n;
  const U *j = &val._str[0];

  if (!val._n || !j)
    return;

  write(i,val._prefix);

  while (*j)
  {
    if (*j==val._delim)
    {
      if ((--n)==0)
      {
        write(i,val._suffix);
        return;
      }
      *(i++) = *(j++);
      write(i,val._prefix);
    }
    else
      *(i++) = *(j++);
  }
}

// Convenience functions for the length of multiple items

template<typename A1, typename A2>
inline size_t length(const A1 &a1, const A2 &a2)
{ return length(a1)+length(a2); }

template<typename A1, typename A2, typename A3>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3)
{ return length(a1)+length(a2)+length(a3); }

template<typename A1, typename A2, typename A3, typename A4>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4)
{ return length(a1)+length(a2)+length(a3)+length(a4); }

template<typename A1, typename A2, typename A3, typename A4, typename A5>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15)+length(a16); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15)+length(a16)+length(a17); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15)+length(a16)+length(a17)+length(a18); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18, const A19 &a19)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15)+length(a16)+length(a17)+length(a18)+length(a19); }

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
inline size_t length(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18, const A19 &a19, const A20 &a20)
{ return length(a1)+length(a2)+length(a3)+length(a4)+length(a5)+length(a6)+length(a7)+length(a8)+length(a9)+length(a10)+length(a11)+length(a12)+length(a13)+length(a14)+length(a15)+length(a16)+length(a17)+length(a18)+length(a19)+length(a20); }

// Convenience functions for writing multiple items

template<typename Iterator, typename A1, typename A2>
inline void write(Iterator &i, const A1 &a1, const A2 &a2)
{
  write(i,a1);
  write(i,a2);
}

template<typename Iterator, typename A1, typename A2, typename A3>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12,const A13 &a13)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12,const A13 &a13,const A14 &a14)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12,const A13 &a13,const A14 &a14,const A15 &a15)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
  write(i,a16);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
  write(i,a16);
  write(i,a17);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
  write(i,a16);
  write(i,a17);
  write(i,a18);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18, const A19 &a19)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
  write(i,a16);
  write(i,a17);
  write(i,a18);
  write(i,a19);
}

template<typename Iterator, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
inline void write(Iterator &i, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12, const A13 &a13, const A14 &a14, const A15 &a15, const A16 &a16, const A17 &a17, const A18 &a18, const A19 &a19, const A20 &a20)
{
  write(i,a1);
  write(i,a2);
  write(i,a3);
  write(i,a4);
  write(i,a5);
  write(i,a6);
  write(i,a7);
  write(i,a8);
  write(i,a9);
  write(i,a10);
  write(i,a11);
  write(i,a12);
  write(i,a13);
  write(i,a14);
  write(i,a15);
  write(i,a16);
  write(i,a17);
  write(i,a18);
  write(i,a19);
  write(i,a20);
}

}}}

#endif

