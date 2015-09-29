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

#ifndef RTTR_METHOD_ACCESSOR_H_
#define RTTR_METHOD_ACCESSOR_H_

#include "rttr/detail/misc/function_traits.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/method/method_invoker.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////
// this is a workaround because MSVC2013 cannot handle sizeof...(T) with enable_if
template<typename F, typename...TArgs>
struct does_argument_count_match : std::integral_constant<bool, (sizeof...(TArgs) == function_traits<F>::arg_count)>
{
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename List1, typename List2>
struct is_arg_count_in_valid_range;

template<typename F, typename...TArgs, typename...Default_Args>
struct is_arg_count_in_valid_range<F, type_list<TArgs...>, type_list<Default_Args...>>
:   std::integral_constant<bool, (sizeof...(TArgs) <= function_traits<F>::arg_count) && ( sizeof...(TArgs) + sizeof...(Default_Args) >= function_traits<F>::arg_count) >
{
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy, typename Method_Type, std::size_t... Def_Idx, typename... Def_Types, typename... TArgs>
static RTTR_FORCE_INLINE variant invoke_with_defaults_helper(const F& func_ptr, const instance& obj, index_sequence<Def_Idx...>, 
                                                              const std::tuple<Def_Types...>& def_args, const TArgs&...args)
{
    static const std::size_t arg_count = sizeof...(Def_Idx) + sizeof...(TArgs);
    return method_invoker<F, Policy, Method_Type, make_index_sequence<arg_count>>::invoke(func_ptr, obj, args..., argument(std::get<Def_Idx>(def_args))...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy, std::size_t... Arg_Idx>
static RTTR_FORCE_INLINE variant invoke_variadic_helper(const F& func_ptr, const instance& obj, index_sequence<Arg_Idx...>, const std::vector<argument>& arg_list)
{
    using method_type = typename detail::method_type<F>::type;
    return method_invoker<F, Policy, method_type, index_sequence<Arg_Idx...>>::invoke(func_ptr, obj, arg_list[Arg_Idx]...);
}

/////////////////////////////////////////////////////////////////////////////////////////

// functions cannot be partial specialized, thats why we using here a struct
template<typename F, typename Policy, typename IndexSequence>
struct invoke_variadic_with_defaults_helper;

template<typename F, typename Policy, std::size_t... Arg_Idx>
struct invoke_variadic_with_defaults_helper<F, Policy, index_sequence<Arg_Idx...>>
{
    template<typename... Def_Types>
    static RTTR_FORCE_INLINE variant invoke(const F& func_ptr, const instance& obj, const std::tuple<Def_Types...>& def_args, const std::vector<argument>& arg_list);
};

template<typename F, typename Policy>
struct invoke_variadic_with_defaults_helper<F, Policy, index_sequence<>>
{
    template<typename... Def_Types>
    static RTTR_FORCE_INLINE variant invoke(const F& func_ptr, const instance& obj, const std::tuple<Def_Types...>& def_args, const std::vector<argument>& arg_list);
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy>
struct method_accessor
{
    static bool is_static()
    {
        using method_type = typename detail::method_type<F>::type;
        return method_accessor_helper_is_static<method_type>::is_static(); 
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static type get_return_type()
    {
        return method_accessor_helper_return_type<F, Policy>::get_return_type();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static std::vector<bool> get_is_reference()
    {
        const std::size_t arg_count  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, arg_count != 0>::type;
        return method_accessor_impl<F, make_index_sequence<arg_count>, has_arguments>::get_is_reference(); 
    }
    
    /////////////////////////////////////////////////////////////////////////////////////

    static std::vector<bool> get_is_const()
    {
        const std::size_t arg_count  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, arg_count != 0>::type;
        return method_accessor_impl<F, make_index_sequence<arg_count>, has_arguments>::get_is_const(); 
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static std::vector<type> get_parameter_types()
    {
        const std::size_t arg_count  = function_traits<F>::arg_count;
        using has_arguments         = typename std::integral_constant<bool, arg_count != 0>::type;
        return method_accessor_impl<F, make_index_sequence<arg_count>, has_arguments>::get_parameter_types(); 
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... TArgs>
    static RTTR_FORCE_INLINE 
    enable_if_t< does_argument_count_match<F, TArgs...>::value, variant>
    invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        static const std::size_t arg_count = function_traits<F>::arg_count;
        using method_type = typename detail::method_type<F>::type;
        return method_invoker<F, Policy, method_type, make_index_sequence<arg_count>>::invoke(func_ptr, obj, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... TArgs>
    static RTTR_FORCE_INLINE 
    enable_if_t< !does_argument_count_match<F, TArgs...>::value, variant>
    invoke(const F& func_ptr, const instance& obj, const TArgs&...args)
    {
        return variant();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... Def_Types, typename... TArgs>
    static RTTR_FORCE_INLINE
    enable_if_t< is_arg_count_in_valid_range<F, type_list<TArgs...>, type_list<Def_Types...>>::value, variant>
    invoke_with_defaults(const F& func_ptr, const instance& obj, const std::tuple<Def_Types...>& def_args, const TArgs&...args)
    {
        using method_type = typename detail::method_type<F>::type;
        static const std::size_t arg_count = function_traits<F>::arg_count;
        // here we calculate the integer sequence for retrieving the data from the tuple,
        // this depends on the number of arguments provided by the caller
        static const std::size_t start_index = sizeof...(TArgs) + sizeof...(Def_Types) - arg_count;
        using idx_seq = typename erase_sequence_till<index_sequence_for<Def_Types...>, start_index>::type;
        return invoke_with_defaults_helper<F, Policy, method_type>(func_ptr, obj, idx_seq(), def_args, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... Def_Types, typename... TArgs>
    static RTTR_FORCE_INLINE
    enable_if_t< !is_arg_count_in_valid_range<F, type_list<TArgs...>, type_list<Def_Types...>>::value, variant>
    invoke_with_defaults(const F& func_ptr, const instance& obj, const std::tuple<Def_Types...>& def_args, const TArgs&...args)
    {
        return variant();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static RTTR_FORCE_INLINE variant invoke_variadic(const F& func_ptr, const instance& obj, const std::vector<argument>& arg_list)
    {
        const std::size_t arg_count = function_traits<F>::arg_count;
        if (arg_list.size() == arg_count)
            return invoke_variadic_helper<F, Policy>(func_ptr, obj, make_index_sequence<arg_count>(), arg_list);
        else
            return variant();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename... Def_Types>
    static RTTR_FORCE_INLINE variant invoke_variadic(const F& func_ptr, const instance& obj, 
                                                     const std::tuple<Def_Types...>& def_args, const std::vector<argument>& arg_list)
    {
        const std::size_t arg_count  = function_traits<F>::arg_count;
        if (arg_list.size() <= arg_count)
            return invoke_variadic_with_defaults_helper<F, Policy, make_index_sequence<arg_count>>::invoke(func_ptr, obj, def_args, arg_list);
        else
            return variant();
    }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy, std::size_t... Arg_Idx>
template<typename... Def_Types>
RTTR_FORCE_INLINE variant invoke_variadic_with_defaults_helper<F, Policy, index_sequence<Arg_Idx...>>::invoke(const F& func_ptr, const instance& obj, 
                                                                                                               const std::tuple<Def_Types...>& def_args,
                                                                                                               const std::vector<argument>& arg_list)
{
    static const std::size_t Arg_Count = sizeof...(Arg_Idx);
    if (arg_list.size() == Arg_Count)
        return method_accessor<F, Policy>::invoke_with_defaults(func_ptr, obj, def_args, arg_list[Arg_Idx]...);
    else
        return invoke_variadic_with_defaults_helper<F, Policy, make_index_sequence<Arg_Count - 1>>::invoke(func_ptr, obj, def_args, arg_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy>
template<typename... Def_Types>
RTTR_FORCE_INLINE variant invoke_variadic_with_defaults_helper<F, Policy, index_sequence<>>::invoke(const F& func_ptr, const instance& obj, 
                                                                                                     const std::tuple<Def_Types...>& def_args,
                                                                                                     const std::vector<argument>& arg_list)
{
    return method_accessor<F, Policy>::invoke_with_defaults(func_ptr, obj, def_args);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_METHOD_ACCESSOR_H_
