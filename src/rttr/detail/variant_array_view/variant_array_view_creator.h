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

#ifndef RTTR_VARIANT_ARRAY_CREATOR_H_
#define RTTR_VARIANT_ARRAY_CREATOR_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/array/array_wrapper_base.h"
#include "rttr/detail/variant_array_view/variant_array_view_traits.h"

#include <memory>

namespace rttr
{
namespace detail
{

template<typename T, typename Tp = decay_except_array_t<T>>
typename std::enable_if<can_create_array_container<T>::value, std::unique_ptr<array_wrapper_base>>::type create_variant_array_view(T&& value);

template<typename T, typename Tp = decay_except_array_t<T>>
typename std::enable_if<!can_create_array_container<T>::value, std::unique_ptr<array_wrapper_base>>::type create_variant_array_view(T&& value);

} // end namespace detail
} // end namespace rttr

#include "rttr/detail/variant_array_view/variant_array_view_creator_impl.h"

#endif // RTTR_VARIANT_ARRAY_CREATOR_H_
