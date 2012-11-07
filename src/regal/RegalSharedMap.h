/*
  Copyright (c) 2012 Igor Chernyshev
  Copyright (c) 2012 Nigel Stewart, NVIDIA Corporation
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

#ifndef __REGAL_SHARED_MAP_H__
#define __REGAL_SHARED_MAP_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalSharedPtr.h"

#include <map>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

//
// shared_map - a shared_ptr to a std::map with std::map API
//
// shared_map extends std::map to support reference-counted
// sharing.  The shared_map constructs a std::map by default,
// but can be assigned to some other shared_map in order to
// share that.
//
// Assignment of a std::map results in a (deep) copy.
// Assignment of a shared_map is an additional reference.
//
// In principle std::map and shared_map should be API
// compatible, with the exception of copying vs referencing.
//
// Example:
//
//   shared_map<int,int> a;
//   a[1] = 2;
//   a[2] = 4;
//   a[3] = 8;
//
//   shared_map<int,int> b;
//   b = a;                /* a and b now share the same map */
//

template<typename K, typename V>
class shared_map : public shared_ptr< ::std::map<K,V> >
{
  public:

    typedef ::std::map<K,V>    map;
    typedef shared_ptr<map>    parent;
    typedef ::std::size_t      size_type;

    inline shared_map()  : parent(new map()) {}
    inline ~shared_map() {}

    // Expose std::map API

    inline       V &operator[](const K &k)       { RegalAssert(parent::get()); return parent::get()->operator[](k); }
    inline const V &operator[](const K &k) const { RegalAssert(parent::get()); return parent::get()->operator[](k); }

    inline size_type erase( const K &k)          { RegalAssert(parent::get()); return parent::get()->erase(k);      }

    inline size_type count(const K &k) const     { RegalAssert(parent::get()); return parent::get()->count(k);      }
};

REGAL_NAMESPACE_END

#endif
