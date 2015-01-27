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

#ifndef __RTTR_FUNCTION_TRAITS_H__
#define __RTTR_FUNCTION_TRAITS_H__

#include "rttr/base/core_prerequisites.h"

#include <type_traits>
#include <functional>
#include <tuple>

namespace rttr
{
namespace detail
{

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct is_std_function : std::false_type
    {
        typedef T signature;
    };

    template<typename T>
    struct is_std_function<std::function<T>> : std::true_type
    {
        typedef T signature;
    };

    template<typename T>
    struct get_std_function_signature
    {
        typedef T type;
    };

    template<typename T>
    struct get_std_function_signature<std::function<T>>
    {
        typedef T type;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct is_function_ptr : std::integral_constant<bool, std::is_pointer<T>::value && 
                                                          std::is_function<typename std::remove_pointer<T>::type>::value>
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... Args>
    struct function_args
    {
        typedef std::tuple<Args...> arg_types;
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T> 
    struct function_traits_func_ptr;

    template<typename R, typename... Args>
    struct function_traits_func_ptr<R (*)(Args...)> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        typedef R   return_type;
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct function_traits_mem_ptr;

    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...)> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        typedef R   return_type;
        typedef C   class_type;
    };

    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...) const> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        typedef R   return_type;
        typedef C   class_type;
    };

    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...) const volatile> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        typedef R   return_type;
        typedef C   class_type;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T> 
    struct function_traits : std::conditional<std::is_member_function_pointer<T>::value, 
                                             function_traits_mem_ptr<T>,
                                             typename std::conditional<std::is_function<T>::value,
                                                                       function_traits_func_ptr<typename std::add_pointer<T>::type>,
                                                                       typename std::conditional<is_std_function<T>::value,
                                                                                                 function_traits_func_ptr<typename std::add_pointer<typename get_std_function_signature<T>::type>::type>,
                                                                                                 function_traits_func_ptr<T>
                                                                                                >::type
                                                                      >::type 
                                             >::type
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, size_t Index>
    struct param_types
    {
        typedef typename std::tuple_element<Index, typename function_traits<T>::arg_types>::type type;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    // use it like e.g:
    // param_types<F, 0>::type

    template<typename T>
    struct is_void_func : std::conditional<std::is_same<typename function_traits<T>::return_type, void>::value,
                                           std::true_type,
                                           std::false_type
                                          >::type
    {};
} // end namespace detail
} // end namespace rttr

#endif // __RTTR_FUNCTION_TRAITS_H__
