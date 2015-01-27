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

#ifndef __RTTR_METHOD_ACCESSOR__
#define __RTTR_METHOD_ACCESSOR__

#include "rttr/detail/function_traits.h"
#include "rttr/detail/utility.h"

namespace rttr
{
namespace detail
{

template<typename F, typename IndexSequence, typename B>
struct method_accessor_impl;

template<typename F, std::size_t... ArgCount>
struct method_accessor_impl<F, index_sequence<ArgCount...>, std::true_type>
{
    static std::vector<bool> get_is_reference()
    {
        return { std::is_reference<typename param_types<F, ArgCount>::type>::value... };
    }
    
    static std::vector<bool> get_is_const()
    {
        return { std::is_const<typename std::remove_reference<typename param_types<F, ArgCount>::type>::type>::value... };
    }

    static std::vector<type> get_parameter_types()
    {
        return { type::get<typename param_types<F, ArgCount>::type>()... };
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_impl<F, index_sequence<ArgCount...>, std::false_type>
{
    static std::vector<bool> get_is_reference()
    {
        return std::vector<bool>();
    }
    
    static std::vector<bool> get_is_const()
    {
        return std::vector<bool>();
    }

    static std::vector<type> get_parameter_types()
    {
        return std::vector<type>();
    }
};

template<typename F, typename Policy, typename Method_Type, typename IndexSequence, typename ArgCountInRange>
struct method_accessor_invoker;

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, default_invoke, void_member_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        typedef typename function_traits<F>::class_type C;
        C* ptr = obj.try_convert<C>();
        if (ptr && check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            (ptr->*func_ptr)(args.template get_value<typename param_types<F, ArgCount>::type>()...);
            return void_variant;
        }
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, default_invoke, void_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func, const instance& obj, const TArgs&...args)
    {
        if (check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            func(args.template get_value<typename param_types<F, ArgCount>::type>()...);
            return void_variant;
        }
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, default_invoke, return_member_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        typedef typename function_traits<F>::class_type C;
        C* ptr = obj.try_convert<C>();
        if (ptr && check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
            return (ptr->*func_ptr)(args.template get_value<typename param_types<F, ArgCount>::type>()...);
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, default_invoke, return_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func, const instance& obj, const TArgs&...args)
    {
        if (check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
            return func(args.template get_value<typename param_types<F, ArgCount>::type>()...);
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, discard_return, return_member_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        typedef typename function_traits<F>::class_type C;
        C* ptr = obj.try_convert<C>();
        if (ptr && check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            (ptr->*func_ptr)(args.template get_value<typename param_types<F, ArgCount>::type>()...);
            return void_variant;
        }
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, discard_return, return_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func, const instance& obj, const TArgs&...args)
    {
        if (check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            func(args.template get_value<typename param_types<F, ArgCount>::type>()...);
            return void_variant;
        }
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, return_as_ptr, return_member_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        typedef typename function_traits<F>::class_type C;
        C* ptr = obj.try_convert<C>();
        if (ptr && check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            return &(ptr->*func_ptr)(args.template get_value<typename param_types<F, ArgCount>::type>()...);
        }
        else
            return variant();
    }
};

template<typename F, std::size_t... ArgCount>
struct method_accessor_invoker<F, return_as_ptr, return_func, index_sequence<ArgCount...>, std::true_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func, const instance& obj, const TArgs&...args)
    {
        if (check_all_true(args.template is_type<typename param_types<F, ArgCount>::type>()...))
        {
            return &func(args.template get_value<typename param_types<F, ArgCount>::type>()...);
        }
        else
            return variant();
    }
};

template<typename F, typename Policy, typename MethodType, typename IndexSequence>
struct method_accessor_invoker<F, Policy, MethodType, IndexSequence, std::false_type>
{
    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        return variant();
    }
};

template<typename F, typename Policy, typename IndexSequence>
struct method_accessor_variadic;

template<typename F, typename Policy, std::size_t... ArgCount>
struct method_accessor_variadic<F, Policy, index_sequence<ArgCount...>>
{
    static variant invoke(const F& func_ptr, const instance& obj, std::vector<argument>& arg_list)
    {
        using method_type = typename detail::method_type<F>::type;
        return method_accessor_invoker<F, Policy, method_type, index_sequence<ArgCount...>, std::true_type>::invoke(func_ptr, obj, arg_list[ArgCount]...);
    }
};

template<typename MethodType>
struct method_accessor_helper_is_static
{
    static bool is_static() { return true; }
};

template<>
struct method_accessor_helper_is_static<return_member_func>
{
    static bool is_static() { return false; }
};

template<>
struct method_accessor_helper_is_static<void_member_func>
{
    static bool is_static() { return false; }
};

template<typename F, typename Policy>
struct method_accessor_helper_return_type
{
    static type get_return_type() { return type::get<typename function_traits<F>::return_type>(); }
};

template<typename F>
struct method_accessor_helper_return_type<F, return_as_ptr>
{
    using return_type = typename function_traits<F>::return_type;
    static type get_return_type() { return type::get<typename std::remove_reference<return_type>::type*>(); }
};

template<typename F>
struct method_accessor_helper_return_type<F, discard_return>
{
    static type get_return_type() { return type::get<void>(); }
};

template<typename F, typename Policy>
struct method_accessor
{
    static bool is_static()
    {
        using method_type = typename detail::method_type<F>::type;
        return method_accessor_helper_is_static<method_type>::is_static(); 
    }

    static type get_return_type()
    {
        return method_accessor_helper_return_type<F, Policy>::get_return_type();
    }

    static std::vector<bool> get_is_reference()
    {
        const std::size_t ArgCount  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, ArgCount != 0>::type;
        return method_accessor_impl<F, make_index_sequence<ArgCount>, has_arguments>::get_is_reference(); 
    }
    
    static std::vector<bool> get_is_const()
    {
        const std::size_t ArgCount  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, ArgCount != 0>::type;
        return method_accessor_impl<F, make_index_sequence<ArgCount>, has_arguments>::get_is_const(); 
    }

    static std::vector<type> get_parameter_types()
    {
        const std::size_t ArgCount  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, ArgCount != 0>::type;
        return method_accessor_impl<F, make_index_sequence<ArgCount>, has_arguments>::get_parameter_types(); 
    }

    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        const std::size_t ArgCount  = function_traits<F>::arg_count;
        using method_type           = typename detail::method_type<F>::type;
        using arg_count_in_range    = typename std::integral_constant<bool, ArgCount == sizeof...(args)>::type;
        return method_accessor_invoker<F, Policy, method_type, make_index_sequence<ArgCount>, arg_count_in_range>::invoke(func_ptr, obj, args...);
    }

    template<typename... TArgs>
    RTTR_FORCE_INLINE static variant invoke(const F& func_ptr, const instance& obj, std::vector<argument>& arg_list)
    {
        const std::size_t ArgCount  = function_traits<F>::arg_count;
        if (arg_list.size() == ArgCount)
            return method_accessor_variadic<F, Policy, make_index_sequence<ArgCount>>::invoke(func_ptr, obj, arg_list);
        else
            return variant();
    }
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_METHOD_ACCESSOR__
