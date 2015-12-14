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

#ifndef RTTR_FUNCTION_TRAITS_H_
#define RTTR_FUNCTION_TRAITS_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/std_type_traits.h"

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
        using signature = T;
    };

    template<typename T>
    struct is_std_function<std::function<T>> : std::true_type
    {
        using signature = T;
    };

    template<typename T>
    struct get_std_function_signature
    {
        using type = T;
    };

    template<typename T>
    struct get_std_function_signature<std::function<T>>
    {
        using type = T;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct is_function_ptr : std::integral_constant<bool, std::is_pointer<T>::value && 
                                                          std::is_function<::rttr::detail::remove_pointer_t<T>>::value>
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... Args>
    struct function_args
    {
        using arg_types = std::tuple<Args...>;
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T> 
    struct function_traits_func_ptr;

    template<typename R, typename... Args>
    struct function_traits_func_ptr<R (*)(Args...)> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        using return_type = R;
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct function_traits_mem_ptr;

    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...)> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        using return_type   = R;
        using class_type    = C;
    };
    
    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...) const> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        using return_type   = R;
        using class_type    = C;
    };

    template<typename R, typename C, typename... Args>
    struct function_traits_mem_ptr<R (C::*)(Args...) const volatile> : function_args<Args...>
    {
        static const size_t arg_count = sizeof...(Args);
        using return_type   = R;
        using class_type    = C;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T> 
    struct function_traits : conditional_t< std::is_member_function_pointer< T >::value, 
                                            function_traits_mem_ptr< T >,
                                            conditional_t< std::is_function< T >::value,
                                                           function_traits_func_ptr< add_pointer_t< T > >,
                                                           conditional_t< is_std_function< T >::value,
                                                                          function_traits_func_ptr< add_pointer_t< typename get_std_function_signature< T >::type > >,
                                                                          function_traits_func_ptr< T >
                                                                        >
                                                         >
                                          >
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////
    // use it like e.g:
    // param_types<F, 0>::type

    template<typename F, size_t Index>
    struct param_types
    {
        using type = typename std::tuple_element<Index, typename function_traits<F>::arg_types>::type;
    };

    template<typename F, size_t Index>
    using param_types_t = typename param_types<F, Index>::type;

    /////////////////////////////////////////////////////////////////////////////////////
    
    template<typename F>
    struct is_void_func : conditional_t< std::is_same<typename function_traits<F>::return_type, void>::value,
                                         std::true_type,
                                         std::false_type
                                       >
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////
    // returns an std::true_type, when the given type F is a function type; otherwise an std::false_type.
    template<typename F>
    using is_function = std::integral_constant<bool, std::is_member_function_pointer<F>::value ||
                                                     std::is_function<F>::value ||
                                                     is_std_function<F>::value ||
                                                     is_function_ptr<F>::value
                                              >;

    /////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_FUNCTION_TRAITS_H_
