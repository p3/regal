/*
  Copyright (c) 2012 Igor Chernyshev
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

#ifndef __REGAL_SHARED_PTR_H__
#define __REGAL_SHARED_PTR_H__

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#define SHARED_PTR_MODE_NONE  0
#define SHARED_PTR_MODE_TR1   1

#if defined(__GNUC__)
// There is no way to check for TR1, assuming it is here.
#include <tr1/memory>
#define SHARED_PTR_MODE SHARED_PTR_MODE_TR1
#else
#define SHARED_PTR_MODE SHARED_PTR_MODE_NONE
#endif  // __GNUC__

// TODO: Support Windows and Mac.

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN


#if SHARED_PTR_MODE == SHARED_PTR_MODE_TR1

template<typename Value>
struct shared_ptr
{
  typedef std::tr1::shared_ptr<Value> type;
};

#else  // SHARED_PTR_MODE_NONE

namespace internal {

// A dummy impl of TR1 shared_ptr that does not allow sharing.
template<typename Value>
class shared_ptr {
 public:
  inline shared_ptr() : value_(NULL) {}

  template<typename Value1>
  inline explicit shared_ptr(Value1* v) : value_(v) {}

  template<typename Value1>
  inline shared_ptr(const shared_ptr<Value1>& src) : value_(NULL) {
    // Allow compilation but prevent use at runtime.
    RegalAssert(false);
    value_ = src.value_;  // Check for incompatible types.
  }

  template<typename Value1>
  inline shared_ptr& operator=(const shared_ptr<Value1>& src) {
    // Allow compilation but prevent use at runtime.
    RegalAssert(false);
    value_ = src.value_;  // Check for incompatible types.
    return *this;
  }

  template<typename Value1>
  inline void reset(Value1* v) { value_ = v; }

  inline Value& operator*() const { return *value_; }

  inline Value* operator->() const { return value_; }

  inline Value* get() const { return value_; }

 private:
  Value* value_;
};

}  // internal

template<typename Value>
struct shared_ptr
{
  typedef internal::shared_ptr<Value> type;
};

#endif


REGAL_NAMESPACE_END

#endif // REGAL_SHARED_PTR_H
