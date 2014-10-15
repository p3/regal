/*
 * Copyright (c) 2010-2014 NVIDIA Corporation
 * Copyright (c) 2010-2014 Nigel Stewart
 * All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost::print is a library for string conversion.
 * boost::print is not currently a part of boost.
 */

#ifndef BOOST_PRINT_STRING_LIST_HPP
#define BOOST_PRINT_STRING_LIST_HPP

#include <cstdio>
#include <deque>
#include <vector>
#include <string>
#include <algorithm>

#include <cassert>

#include "core.hpp"

//
// boost::print namespace for public API
//

namespace boost { namespace print {

template<typename T = std::string>
struct string_list
{
public:
  typedef const T                                          value_type;
  typedef const T                                         &const_reference;
  typedef char                                             char_type;
  typedef typename ::std::deque<T>::iterator               iterator;
  typedef typename ::std::deque<T>::const_iterator         const_iterator;
  typedef typename ::std::deque<T>::reverse_iterator       reverse_iterator;
  typedef typename ::std::deque<T>::const_reverse_iterator const_reverse_iterator;
  typedef ::std::size_t                                    size_type;

  string_list();
  string_list(const string_list &other);
  string_list(const T &input, const char_type delim = ' ');
  string_list(const size_t count, const char_type * const *string, const int *length = NULL, const typename string_list<T>::char_type delim = 0);
  ~string_list();

  string_list &operator=(const string_list &other);

  void clear();

  void push_front     (const typename string_list<T>::char_type *string);
  void push_front     (const typename string_list<T>::char_type *string, const size_t n);
  void push_front     (const T                                  &string);
  void push_front_swap(      T                                  &string);
  void push_front     (const string_list                        &other );

  void push_back      (const typename string_list<T>::char_type *string);
  void push_back      (const typename string_list<T>::char_type *string, const size_t n);
  void push_back      (const T                                  &string);
  void push_back_swap (      T                                  &string);
  void push_back      (const string_list                        &other );

  void push_back(const size_t count, const char_type * const *string, const int *length = NULL, const typename string_list<T>::char_type delim = 0);

  void insert(const size_type i, const value_type &x);
  void insert(const size_type i, const string_list &other);
  void insert(const size_type i, const size_type n, const value_type &x);

  void pop_front();
  void pop_back();

  template<typename I>
  string_list<T> &
  operator<<(I i)
  {
    PushBack p(*this);
    ::boost::print::print(static_cast<T &>(p),i);
    return *this;
  }

  template<typename I>
  string_list<T> &
  operator+=(I i)
  {
    PushBack p(*this);
    ::boost::print::print(static_cast<T &>(p),i);
    return *this;
  }

  void sort();

  void split(const T                                  &input,                 const char_type delim = ' ');
  void split(const typename string_list<T>::char_type *input,                 const char_type delim = ' ');
  void split(const typename string_list<T>::char_type *input, const size_t n, const char_type delim = ' ');

  T join(const char_type *delim = NULL) const;
  T join(const T         &delim       ) const;
  T str() const;

  operator T () const { return str(); }

  size_type size()  const;
  size_type count() const;

  typename string_list<T>::const_iterator begin() const;
  typename string_list<T>::const_iterator end  () const;

        T &operator[](const size_type i);
  const T &operator[](const size_type i) const;

  // Zero-copy push_front to string_list

  struct PushFront : public T
  {
  public:
    inline PushFront(string_list<T> &list) : _list(list) {}
    inline ~PushFront()
    {
      if (T::length())
      {
        _list._list.push_front(T());
        _list._list.front().swap(*this);
      }
    }

  private:
    PushFront();                                  // Not implemented
    PushFront &operator=(const PushFront &other);  // Not implemented

    string_list<T> &_list;
  };

  // Zero-copy push_back to string_list

  struct PushBack : public T
  {
  public:
    inline PushBack(string_list<T> &list) : _list(list) {}
    inline ~PushBack()
    {
      if (T::length())
      {
        _list._list.push_back(T());
        _list._list.back().swap(*this);
      }
    }

  private:
    PushBack();                                  // Not implemented
    PushBack &operator=(const PushBack &other);  // Not implemented

    string_list<T> &_list;
  };

  static const char_type endl = '\n';

  friend struct string_list<T>::PushFront;
  friend struct string_list<T>::PushBack;

private:
  ::std::deque<T> _list;
};

namespace detail {

// Join any container of std::string

template<typename C, typename T>
T
join(const C &container, const T &delim)
{
  // Determine the required length

  ::std::size_t size = 0;
  for (typename C::const_iterator i = container.begin(); i!=container.end(); ++i)
    size += i->length() + delim.length();

  if (size)
    size -= delim.length();

  if (!size)
    return T();

  // Allocate sufficient space up front

  T tmp;
  tmp.reserve(size);

#ifndef NDEBUG
  const ::std::size_t capacity = tmp.capacity();
#endif

  // Copy the strings into the output

  for (typename C::const_iterator i = container.begin(); i!=container.end(); ++i)
  {
    if (i!=container.begin())
      tmp += delim;
    tmp += *i;
  }

  assert(tmp.length() == size);       // Length is as expected.
  assert(tmp.capacity() == capacity); // Did not need to realloc.

  return tmp;
}

}

template<typename T> string_list<T>::string_list() { }
template<typename T> string_list<T>::string_list(const string_list<T> &other) : _list(other._list) { }

template<typename T> string_list<T>::string_list(const T &input, const char_type delim)
{
  split(input,delim);
}

template<typename T> string_list<T>::string_list(const size_t count, const char_type * const *string, const int *length, const typename string_list<T>::char_type delim)
{
  push_back(count,string,length,delim);
}

template<typename T> string_list<T>::~string_list() { _list.clear(); }

template<typename T> string_list<T> &
string_list<T>::operator=(const string_list<T> &other)
{
  if (this!=&other)
    _list = other._list;

  return *this;
}

template<typename T> void string_list<T>::clear()   { _list.clear(); }

template<typename T> void
string_list<T>::push_front(const typename string_list<T>::char_type *string)
{
  PushFront(*this).assign(string ? string : T(NULL));
}

template<typename T> void
string_list<T>::push_front(const typename string_list<T>::char_type *string, const size_t n)
{
  if (string && n)
  {
    PushFront(*this).assign(string,n);
  }
  else
  {
    PushBack(*this);
  }
}

template<typename T> void string_list<T>::push_front(const T &string)
{
  PushFront(*this).assign(string);
}

template<typename T> void string_list<T>::push_front_swap(T &string)
{
  PushFront(*this).swap(string);
}

template<typename T> void string_list<T>::push_front(const string_list &other)
{
  for (const_iterator i = other.begin(); i!=other.end(); ++i)
  {
    PushFront(*this).assign(*i);
  }
}

template<typename T> void
string_list<T>::push_back(const typename string_list<T>::char_type *string)
{
  PushBack(*this).assign(string ? string : T(NULL));
}

template<typename T> void
string_list<T>::push_back(const typename string_list<T>::char_type *string, const size_t n)
{
  if (string && n)
  {
    PushBack(*this).assign(string,n);
  }
  else
  {
    PushBack(*this);
  }
}

template<typename T> void string_list<T>::push_back(const T &string)
{
  PushBack(*this).assign(string);
}

template<typename T> void string_list<T>::push_back_swap(T &string)
{
  PushBack(*this).swap(string);
}

template<typename T> void string_list<T>::push_back(const string_list &other)
{
  for (const_iterator i = other.begin(); i!=other.end(); ++i)
  {
    PushBack(*this).assign(*i);
  }
}

template<typename T> void string_list<T>::push_back(const size_t count, const char_type * const *string, const int *length, const typename string_list<T>::char_type delim)
{
  if (delim)
  {
    for (size_t i=0; i<count; ++i)
      if (length && length[i]>=0)
        split(string[i],length[i], delim);
      else
        split(string[i],           delim);
  }
  else
  {
    for (size_t i=0; i<count; ++i)
      if (length && length[i]>=0)
      {
        PushBack(*this).assign(string[i] ? T(string[i],length[i]) : T());
      }
      else
      {
        PushBack(*this).assign(string[i] ? T(string[i])           : T());
      }
  }
}

template<typename T> void string_list<T>::insert(const size_type i, const value_type &x)
{
  if (i<=_list.size())
  {
    _list.push_back(x);                     // Append to the end

    const size_type m = _list.size()-i-1;
    reverse_iterator i = _list.rbegin();
    reverse_iterator j = _list.rbegin();
    ++j;

    for (size_type k=0; k<m; ++i, ++j, ++k)
      swap(*i,*j);
  }
}

template<typename T> void string_list<T>::insert(const size_type i, const string_list &other)
{
  const size_type n = other.size();

  if (this!=&other && i>=0 && i<=_list.size() && n>0)
  {
    const size_type m = _list.size()-i;
    for (size_type k=0; k<n; ++k)
      _list.push_back(T());                // Append to the end

    reverse_iterator i = _list.rbegin();
    reverse_iterator j = _list.rbegin();

    for (size_type k=0; k<n; ++k)
      ++j;

    for (size_type k=0; k<m; ++i, ++j, ++k)
      swap(*i,*j);

    for (const_iterator k=other.begin(); k!=other.end(); )
      *(--j) = *(k++);
  }
}

template<typename T> void string_list<T>::insert(const size_type i, const size_type n, const value_type &x)
{
  if (i<=_list.size() && n>0)
  {
    const size_type m = _list.size()-i;
    for (size_type k=0; k<n; ++k)
      _list.push_back(x);                // Append to the end

    reverse_iterator i = _list.rbegin();
    reverse_iterator j = _list.rbegin();

    for (size_type k=0; k<n; ++k)
      ++j;

    for (size_type k=0; k<m; ++i, ++j, ++k)
      swap(*i,*j);
  }
}

template<typename T> void string_list<T>::pop_front() { _list.pop_front(); }
template<typename T> void string_list<T>::pop_back () { _list.pop_back();  }

template<typename T> void string_list<T>::sort()
{
  typename ::std::deque<T>::iterator  i;
  typename ::std::vector<T>::iterator j;

  // swap-copy the strings into a std::vector<T>

  ::std::vector<T> tmp;
  tmp.resize(_list.size());
  const typename ::std::vector<T>::iterator jEnd = tmp.end();

  i = _list.begin();
  j = tmp.begin();
  for (; j!=jEnd; ++i, ++j)
    i->swap(*j);

  // Do the sort

  ::std::sort(tmp.begin(),tmp.end());

  // swap-copy the strings back into the std::deque<T>

  i = _list.begin();
  j = tmp.begin();
  for (; j!=jEnd; ++i, ++j)
    i->swap(*j);
}

template<typename T> void
string_list<T>::split(const T &input, const typename string_list<T>::char_type delim)
{
  // Skip past any delims at the beginning of the string
  typename T::size_type start = input.find_first_not_of( delim, 0 );
  typename T::size_type end;

  // input was a null string
  if (start==T::npos)
      return;

  for (;;)
  {
    end = input.find(delim, start);

    std::string tmp = input.substr(start,end - start);
    PushBack(*this).swap(tmp);

    // Last token if there were no trailing delims
    if (end==T::npos)
      break;

    // Skip past any extra delims
    start = input.find_first_not_of(delim,end+1);

    // We had trailing delims and we're now at the end of the string
    if (start==T::npos)
      break;
  }
}

template<typename T> void
string_list<T>::split(const typename string_list<T>::char_type *input, const char_type delim)
{
  // input was a null string
  if (!input || !(*input))
      return;

  // Skip past any delims at the beginning of the string

  const typename string_list<T>::char_type *i = input;
  while (*i && (*i)==delim)
    ++i;

  for (;;)
  {
    const typename string_list<T>::char_type *j = i;
    while (*j && (*j)!=delim)
      ++j;

    PushBack(*this).assign(i,j-i);

    // Last token if there were no trailing delims
    if (!(*j))
      break;

    // Skip past any extra delims
    i = j+1;
    while (*i && (*i)==delim)
      ++i;

    // We had trailing delims and we're now at the end of the string
    if (!(*i))
      break;
  }
}

template<typename T> void
string_list<T>::split(const typename string_list<T>::char_type *input, const size_t n, const char_type delim)
{
  // input was a null string
  if (!input || !(*input) || !n)
      return;

  // Skip past any delims at the beginning of the string

  size_t i = 0;
  while (i<n && input[i]==delim)
    ++i;

  for (;;)
  {
    size_t j = i;
    while (j<n && input[j]!=delim)
      ++j;

    PushBack(*this).assign(input+i,j-i);

    // Last token if there were no trailing delims
    if (j==n)
      break;

    // Skip past any extra delims
    i = j+1;
    while (i<n && input[i]==delim)
      ++i;

    // We had trailing delims and we're now at the end of the string
    if (i==n)
      break;
  }
}

template<typename T> T string_list<T>::join(const typename string_list<T>::char_type *delim) const
{
  return boost::print::detail::join(_list,delim ? T(delim) : T());
}

template<typename T> T string_list<T>::join(const T &delim) const
{
  return join(delim.c_str());
}

template<typename T> T string_list<T>::str() const
{
  return join();
}

template<typename T> typename string_list<T>::size_type string_list<T>::size()  const { return _list.size(); }

template<typename T> typename string_list<T>::size_type
string_list<T>::count() const
{
  typename string_list<T>::size_type tmp = 0;
  for (typename ::std::deque<T>::const_iterator i=_list.begin(); i!=_list.end(); ++i)
    tmp += i->length();
  return tmp;
}

template<typename T> typename string_list<T>::const_iterator string_list<T>::begin() const { return _list.begin(); }
template<typename T> typename string_list<T>::const_iterator string_list<T>::end()   const { return _list.end();   }

template<typename T>       T &string_list<T>::operator[](const size_type i)       { assert(i<size()); return _list[i]; }
template<typename T> const T &string_list<T>::operator[](const size_type i) const { assert(i<size()); return _list[i]; }

} }

#endif
