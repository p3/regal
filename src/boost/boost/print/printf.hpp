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

#ifndef BOOST_PRINT_PRINTF_HPP
#define BOOST_PRINT_PRINTF_HPP

#if defined(_MSC_VER) && (_MSC_VER>=1200)
#pragma once
#endif

#include <cstdarg>
#include <cstddef>

#include <boost/print/detail.hpp>
#include <boost/print/interface.hpp>

#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#include <varargs.h>
#endif

//
// boost::print namespace for public API
//

namespace boost { namespace print {

#if defined(_MSC_VER) && _MSC_VER < 1800
inline void va_copy(std::va_list &d, std::va_list &s) { d = s; }
#endif

// Generic printf

template<typename T>
size_t printf(T &output, const char *fmt, std::va_list &args)
{
  using boost::print::extend::length;
  using boost::print::extend::write;

  output.clear();

  if (!fmt)
    return 0;

  std::va_list tmp;
  va_copy(tmp,args);

  std::size_t size = 0;
  for (const char *i = fmt; *i; )
  {
    while (*i!='%' && *i)
    {
      ++i;
      ++size;
    }
    if (i[0]=='%')
    {
      if (i[1]=='%')
      {
        size += length('%');
        i += 2;
        continue;
      }
      if (i[1]=='c')
      {
        size += length(static_cast<char>(va_arg(tmp,int)));
        i += 2;
        continue;
      }
      if (i[1]=='s')
      {
        size += length(va_arg(tmp,char *));
        i += 2;
        continue;
      }
      if (i[1]=='d')
      {
        size += length(va_arg(tmp,int));
        i += 2;
        continue;
      }
      if (i[1]=='i')
      {
        size += length(va_arg(tmp,int));
        i += 2;
        continue;
      }
      if (i[1]=='u')
      {
        size += length(va_arg(tmp,unsigned int));
        i += 2;
        continue;
      }
      if (i[1]=='l' && (i[2]=='d' || i[2]=='i'))
      {
        size += length(va_arg(tmp,long));
        i += 3;
        continue;
      }
      if (i[1]=='x')
      {
        size += length(hex(va_arg(tmp,unsigned int)));
        i += 2;
        continue;
      }
      if (i[1]=='X')
      {
        size += length(hex(va_arg(tmp,unsigned int),true));
        i += 2;
        continue;
      }
      if (i[1]=='#' && i[2]=='x')
      {
        size += length(hex(va_arg(tmp,unsigned int))) + 2;
        i += 3;
        continue;
      }
      if (i[1]=='#' && i[2]=='X')
      {
        size += length(hex(va_arg(tmp,unsigned int),true)) + 2;
        i += 3;
        continue;
      }
      if (i[1]=='o')
      {
        size += length(octal(va_arg(tmp,unsigned int)));
        i += 2;
        continue;
      }
      if (i[1]=='#' && i[2]=='o')
      {
        size += length(octal(va_arg(tmp,unsigned int))) + 1;
        i += 3;
        continue;
      }
      if (i[1]=='e' || i[1]=='E' || i[1]=='f' || i[1]=='F' || i[1]=='g' || i[1]=='G')
      {
        size += length(va_arg(tmp,double));
        i += 2;
        continue;
      }
      if (i[1]=='p')
      {
        size += length(va_arg(tmp,void *)) + 2;
        i += 2;
        continue;
      }
      if (i[1]=='t' && (i[2]=='d' || i[2]=='i'))
      {
        size += length(va_arg(tmp,std::ptrdiff_t));
        i += 3;
        continue;
      }
      if (i[1]=='t' && i[2]=='u')
      {
        size += length(va_arg(tmp,std::size_t));
        i += 3;
        continue;
      }
      if (i[1]=='z' && (i[2]=='d' || i[2]=='i'))
      {
        size += length(va_arg(tmp,std::ptrdiff_t));
        i += 3;
        continue;
      }
      if (i[1]=='z' && i[2]=='u')
      {
        size += length(va_arg(tmp,std::size_t));
        i += 3;
        continue;
      }
    }
    if (*i)
    {
      ++i;
      ++size;
    }
  }

  va_end(tmp);

  output.resize(size,' ');

  typename T::iterator j = output.begin();

  va_copy(tmp,args);

  for (const char *i = fmt; *i; )
  {
    while (*i!='%' && *i)
    {
      write(j,*i);
      ++i;
    }
    if (i[0]=='%')
    {
      if (i[1]=='%')
      {
        write(j,'%');
        i += 2;
        continue;
      }
      if (i[1]=='c')
      {
        write(j,static_cast<char>(va_arg(tmp,int)));
        i += 2;
        continue;
      }
      if (i[1]=='s')
      {
        write(j,va_arg(tmp,const char *));
        i += 2;
        continue;
      }
      if (i[1]=='d')
      {
        write(j,va_arg(tmp,int));
        i += 2;
        continue;
      }
      if (i[1]=='i')
      {
        write(j,va_arg(tmp,int));
        i += 2;
        continue;
      }
      if (i[1]=='u')
      {
        write(j,va_arg(tmp,unsigned int));
        i += 2;
        continue;
      }
      if (i[1]=='l' && (i[2]=='d' || i[2]=='i'))
      {
        write(j,va_arg(tmp,long));
        i += 3;
        continue;
      }
      if (i[1]=='x')
      {
        write(j,hex(va_arg(tmp,unsigned int)));
        i += 2;
        continue;
      }
      if (i[1]=='X')
      {
        write(j,hex(va_arg(tmp,unsigned int),true));
        i += 2;
        continue;
      }
      if (i[1]=='#' && i[2]=='x')
      {
        write(j,"0x");
        write(j,hex(va_arg(tmp,unsigned int)));
        i += 3;
        continue;
      }
      if (i[1]=='#' && i[2]=='X')
      {
        write(j,"0X");
        write(j,hex(va_arg(tmp,unsigned int),true));
        i += 3;
        continue;
      }
      if (i[1]=='o')
      {
        write(j,octal(va_arg(tmp,unsigned int)));
        i += 2;
        continue;
      }
      if (i[1]=='#' && i[2]=='o')
      {
        write(j,'0');
        write(j,octal(va_arg(tmp,unsigned int)));
        i += 3;
        continue;
      }
      if (i[1]=='e' || i[1]=='E' || i[1]=='f' || i[1]=='F' || i[1]=='g' || i[1]=='G')
      {
        write(j,va_arg(tmp,double));
        i += 2;
        continue;
      }
      if (i[1]=='p')
      {
        write(j,"0x");
        write(j,va_arg(tmp,void *));
        i += 2;
        continue;
      }
      if (i[1]=='t' && (i[2]=='d' || i[2]=='i'))
      {
        write(j,va_arg(tmp,std::ptrdiff_t));
        i += 3;
        continue;
      }
      if (i[1]=='t' && i[2]=='u')
      {
        write(j,va_arg(tmp,std::size_t));
        i += 3;
        continue;
      }
      if (i[1]=='z' && (i[2]=='d' || i[2]=='i'))
      {
        write(j,va_arg(tmp,std::ptrdiff_t));
        i += 3;
        continue;
      }
      if (i[1]=='z' && i[2]=='u')
      {
        write(j,va_arg(tmp,std::size_t));
        i += 3;
        continue;
      }
    }
    if (*i)
    {
      write(j,*i);
      ++i;
    }
  }

  va_end(tmp);

  return size;
}

template<typename T>
size_t printf(T &output, const char *fmt, ...)
{
  std::va_list args;
  va_start(args,fmt);
  size_t ret = boost::print::printf(output,fmt,args);
  va_end(args);

  return ret;
}

//

template<typename T>
T printf_cast(const char *fmt, std::va_list &args)
{
  T tmp;
  boost::print::printf(tmp,fmt,args);
  return tmp;
}

template<typename T>
T printf_cast(const char *fmt, ...)
{
  std::va_list args;
  va_start(args,fmt);
  T tmp;
  boost::print::printf(tmp,fmt,args);
  va_end(args);
  return tmp;
}

}}

#endif

