/**************************************************************************
 *
 * Copyright 2014 VMware, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

#ifndef COM_PTR_HPP
#define COM_PTR_HPP


#include <assert.h>


/**
 * Simple smart pointer template for COM interfaces.
 */
template< class T >
class com_ptr
{
private:
    T *p;

public:
    com_ptr(void) {
        p = NULL;
    }

    ~com_ptr() {
        if (p) {
            p->Release();
        }
    }

    // Used when initializing
    T **
    operator & () {
        assert(p == NULL);
        return &p;
    }

    // Implict cast to T*
    operator T * () const {
        return p;
    }

    // Methods
    T *
    operator -> () const {
        return p;
    }

    T *
    operator = (T *q) {
        if (p) {
            p->Release();
        }
        if (q) {
            q->AddRef();
        }
        return p = q;
    }

private:
    com_ptr(const com_ptr &);
    com_ptr & operator= (const com_ptr &);
};



#endif // COM_PTR_HPP
