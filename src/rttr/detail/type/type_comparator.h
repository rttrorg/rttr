/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_TYPE_COMPARATOR_H_
#define RTTR_TYPE_COMPARATOR_H_

#include "rttr/detail/base/core_prerequisites.h"

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

struct RTTR_LOCAL type_comparator_base
{
    using equal_func = bool (*)(const void* lhs, const void* rhs);
    using less_than_func = bool (*)(const void* lhs, const void* rhs);

    type_comparator_base(equal_func equal_f = nullptr, less_than_func less_than_f = nullptr)
    :   equal(equal_f), less_than(less_than_f)
    {
    }
    
    equal_func      equal;
    less_than_func  less_than;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct type_comparator : type_comparator_base
{
    type_comparator() : type_comparator_base(equal, less_than) {}

    static bool equal(const void* lhs, const void* rhs)
    {
        const T& l = *static_cast<const T*>(lhs);
        const T& r = *static_cast<const T*>(rhs);
        return (l == r);
    }

    static bool less_than(const void* lhs, const void* rhs)
    {
        const T& l = *static_cast<const T*>(lhs);
        const T& r = *static_cast<const T*>(rhs);
        return (l < r);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_COMPARATOR_H_
