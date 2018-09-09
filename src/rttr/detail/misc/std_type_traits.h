/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_STD_TYPE_TRAITS_H_
#define RTTR_STD_TYPE_TRAITS_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <type_traits>

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// shortcuts to avoid writing typename my_traits<T>::type over and over again
// C++14 has support for this, but we support at the moment C++11

template< bool B, class T, class F >
using conditional_t = typename std::conditional<B, T, F>::type;

template<typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template<typename T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

template<typename T>
using remove_const_t = typename std::remove_const<T>::type;

template<typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<typename T>
using add_pointer_t = typename std::add_pointer<T>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T>
using decay_t = typename std::decay<T>::type;

template<typename T>
using add_const_t = typename std::add_const<T>::type;

template<typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

/////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_STD_TYPE_TRAITS_H_
