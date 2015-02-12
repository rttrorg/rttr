/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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

#ifndef __RTTR_GET_CREATE_VARIANT_FUNC_H__
#define __RTTR_GET_CREATE_VARIANT_FUNC_H__

#include "rttr/variant.h"
#include "rttr/detail/misc_type_traits.h"

namespace rttr
{

namespace detail
{

template<typename source_type>
static variant create_variant(void* ptr)
{
    return static_cast<source_type>(ptr);
}

template<typename source_type>
static variant create_invalid_variant(void*)
{
    return variant();
}

typedef variant(*create_variant_func)(void*);

template<typename T>
create_variant_func get_create_variant_func(typename std::enable_if< detail::pointer_count<T>::value == 1 >::type* = 0)
{
    return create_variant<T>;
}

template<typename T>
create_variant_func get_create_variant_func(typename std::enable_if< detail::pointer_count<T>::value != 1>::type* = 0)
{
    return create_invalid_variant<T>;
}

} // end namespace detail

} // end namespace rttr

#endif // __RTTR_GET_CREATE_VARIANT_FUNC_H__
