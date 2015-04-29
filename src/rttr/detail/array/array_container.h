/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
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

#ifndef RTTR_ARRAY_CONTAINER_H_
#define RTTR_ARRAY_CONTAINER_H_

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/array/array_mapper.h"
#include "rttr/wrapper_mapper.h"

#include <type_traits>
#include <cstddef>

namespace rttr
{

namespace detail
{

class array_container_base;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using is_raw_array_type = ::rttr::detail::is_array<typename raw_type<T>::type>;

template<typename T>
using is_wrapper_array_type = std::integral_constant<bool, is_raw_array_type<wrapper_mapper_t<T>>::value && is_wrapper<T>::value>;

template<typename T>
using is_pointer_array_type = std::integral_constant<bool, is_raw_array_type<T>::value && std::is_pointer<typename std::remove_reference<T>::type>::value>;

template<typename T>
using can_create_array_container = std::integral_constant<bool, ::rttr::detail::is_array<T>::value || is_wrapper_array_type<T>::value || is_pointer_array_type<T>::value>;


/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!can_create_array_container<T>::value,
                        array_container_base*>::type 
create_array_container_impl(T&& value);

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<::rttr::detail::is_array<T>::value, 
                        array_container_base*>::type 
create_array_container_impl(T&& value);

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<is_wrapper_array_type<T>::value && 
                        !is_pointer_array_type<T>::value, 
                        array_container_base*>::type 
create_array_container_impl(T&& value);

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!is_wrapper_array_type<T>::value && 
                         is_pointer_array_type<T>::value,
                         array_container_base*>::type 
create_array_container_impl(T&& value);

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
array_container_base* create_array_container(T&& value);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#include "rttr/detail/array/array_container_impl.h"

#endif // RTTR_ARRAY_CONTAINER_H_
