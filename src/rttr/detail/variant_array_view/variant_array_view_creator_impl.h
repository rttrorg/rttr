/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel@rttr.org>                                 *
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

#ifndef RTTR_VARIANT_ARRAY_CREATOR_IMPL_H_
#define RTTR_VARIANT_ARRAY_CREATOR_IMPL_H_

#include "rttr/detail/array/array_wrapper.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
typename std::enable_if<can_create_array_container<T>::value, std::unique_ptr<array_wrapper_base>>::type
create_variant_array_view(T&& value)
{
    return detail::make_unique<array_wrapper<Tp>>(wrapped_raw_addressof(value));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
typename std::enable_if<!can_create_array_container<T>::value, std::unique_ptr<array_wrapper_base>>::type
create_variant_array_view(T&& value)
{
    return detail::make_unique<array_wrapper_base>();
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_ARRAY_CREATOR_IMPL_H_
