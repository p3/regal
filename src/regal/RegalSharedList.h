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

#ifndef __REGAL_SHARED_LIST_H__
#define __REGAL_SHARED_LIST_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalSharedPtr.h"

#include <list>

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

//
// shared_list - a shared_ptr to a std::list with std::list API
//
// shared_list extends std::list to support reference-counted
// sharing.  The shared_list constructs a std::list by default,
// but can be assigned to some other shared_list in order to
// share that.
//
// Assignment of a std::list results in a (deep) copy.
// Assignment of a shared_list is an additional reference.
//
// In principle std::list and shared_list should be API
// compatible, with the exception of copying vs referencing.
//
// Example:
//
//   shared_list<int> a;
//   a.push_back(1);
//   a.push_back(2);
//   a.push_back(3);
//
//   shared_list<int> b;
//   b = a;                /* a and b now share the same list */
//

template<typename T>
class shared_list : public shared_ptr< ::std::list<T> >
{
  public:

    typedef          ::std::list<T>       list;
    typedef          shared_ptr<list>     parent;
    typedef          ::std::size_t        size_type;
    typedef typename list::iterator       iterator;
    typedef typename list::const_iterator const_iterator;

    inline shared_list()  : parent(new list()) {}
    inline ~shared_list() {}

    // Expose std::list API

    inline void push_front(const T &x)   { RegalAssert(parent::get()); parent::get()->push_front(x);  }
    inline void push_back (const T &x)   { RegalAssert(parent::get()); parent::get()->push_back(x);   }

    inline       iterator begin ()       { RegalAssert(parent::get()); return parent::get()->begin(); }
    inline const_iterator begin () const { RegalAssert(parent::get()); return parent::get()->begin(); }

    inline       iterator end ()         { RegalAssert(parent::get()); return parent::get()->end();   }
    inline const_iterator end ()   const { RegalAssert(parent::get()); return parent::get()->end();   }

    inline void remove(const T &x)       { RegalAssert(parent::get()); parent::get()->remove(x);      }
};

REGAL_NAMESPACE_END

#endif
