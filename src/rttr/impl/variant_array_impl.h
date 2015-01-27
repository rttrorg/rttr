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

#include "rttr/detail/array_container.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

namespace rttr
{

namespace detail
{
template<typename T>
array_container_base* create_array_container(const T& value);

template<typename T>
array_container_base* create_array_container_moved(T&& value);
} 

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array()
:   _container(nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant_array::variant_array(const T& param)
:   _container(detail::create_array_container(param))

{
   static_assert(detail::is_array<T>::value, "No Array type provided, please provide a specialization with rttr::detail::array_mapper<T>.");
   static_assert(!detail::is_array<variant>::value, "No allowed to create a variant_array from variant.");
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant_array::variant_array(T&& param,
                typename std::enable_if<!std::is_same<variant_array&, T>::value >::type*,
                typename std::enable_if<!std::is_const<T>::value >::type*
                )
:   _container(detail::create_array_container_moved(std::move(param)))
{
   using type = typename detail::raw_type<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type;
   static_assert(detail::is_array<type>::value, "No Array type provided, please provide a specialization with rttr::detail::array_mapper<T>.");
   static_assert(!detail::is_array<variant>::value, "No allowed to create a variant_array from variant.");
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(const variant_array& other)
:   _container(other._container ? other._container->clone() : nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(variant_array&& other)
:   _container(other._container)
{
    other._container = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::~variant_array()
{
    delete _container;
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    #if RTTR_COMP_VER <= 1800
        _container = nullptr;
    #else
        #error "Please check if this lead to still to a crash."
    #endif
#endif
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant_array::swap(variant_array& other)
{
    std::swap(_container, other._container);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE variant_array& variant_array::operator=(T&& other)
{
    variant_array(static_cast<T&&>(other)).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array& variant_array::operator =(const variant_array& other)
{
    variant_array(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array& variant_array::operator=(variant_array&& other)
{
    other.swap(*this);
    variant_array().swap(other);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
