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

#ifndef RTTR_DEFAULT_ARGUMENTS_H_
#define RTTR_DEFAULT_ARGUMENTS_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/argument_extractor.h"

#include <memory>
#include <type_traits>

namespace rttr
{
namespace detail
{

/*!
 * This class holds the data for the default arguments.
 */
template<typename...T>
struct default_args
{
    std::tuple<T...> m_args;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Default_Arg_List, typename Index_Sequence> 
struct find_default_args_impl;

// This type trait will check whether the function signature match the Default_Arg_List types.
// It will check the signature recursively, beginning with the complete signature,
// then removing the first argument and check against Default_Arg_List, then removing the second...
template<typename F, typename Default_Arg_List, std::size_t Index, std::size_t... Arg_Count> 
struct find_default_args_impl<F, Default_Arg_List, index_sequence<Index, Arg_Count...>>
{
    template<typename T>
    using decay_type = remove_cv_t<remove_reference_t<T>>;

    using func_args = default_args< decay_type< param_types_t<F, Index >>, 
                                    decay_type< param_types_t<F, Arg_Count>>...
                                  >;

    using type = conditional_t< std::is_same<func_args, Default_Arg_List>::value,
                                Default_Arg_List,
                                typename find_default_args_impl<F, Default_Arg_List, index_sequence<Arg_Count...> >::type
                               >;
};


template<typename F, typename Default_Arg_List>
struct find_default_args_impl<F, Default_Arg_List, index_sequence<>>
{
    using type = default_args<>;
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This type traits returns for a given function \p F
 * the given default type list of arguments \p Default_Arg_List,
 * when it is possible to call the function \p F with the given list \p Default_Arg_List.
 *
 * e.g.:
 * F => foo(int, double); Default_Arg_List => "type_list<int, double>" will return "type_list<int, double>"
 * F => foo(int, double); Default_Arg_List => "type_list<int>" will return "type_list<>" (cannot be called)
 * F => foo(int, double); Default_Arg_List => "type_list<double>" will return "type_list<double>"
 */
template<typename F, typename Default_Arg_List>
using find_default_args_t = typename find_default_args_impl<F, Default_Arg_List, 
                                                            make_index_sequence< function_traits<F>::arg_count> >::type;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct is_def_type : std::false_type { };

template<typename... TArgs>
struct is_def_type<default_args<TArgs...> > : std::true_type { };

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct get_default_args_impl;

template<typename T, typename...TArgs>
struct get_default_args_impl<type_list<T, TArgs...>>
{
    using type = conditional_t< is_def_type<T>::value,
                                T,
                                typename get_default_args_impl<type_list<TArgs...>>::type
                               >;
};

template<>
struct get_default_args_impl<type_list<>>
{
    using type = default_args<>;
};

//! This type trait will return the first type which matches the template type `default_args<T...>`
template<typename...TArgs>
using get_default_args_t = typename get_default_args_impl< type_list< TArgs... > >::type;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable = void>
struct count_default_args_impl;

template<>
struct count_default_args_impl<type_list<>>
{
    static const std::size_t value = 0;
};

template<typename T, typename...TArgs>
struct count_default_args_impl<type_list<T, TArgs...>, enable_if_t<is_def_type<T>::value>>
{
    static const std::size_t value = count_default_args_impl<type_list<TArgs...>>::value + 1;
};

template<typename T, typename...TArgs>
struct count_default_args_impl<type_list<T, TArgs...>, enable_if_t<!is_def_type<T>::value>>
{
    static const std::size_t value = count_default_args_impl<type_list<TArgs...>>::value;
};

/////////////////////////////////////////////////////////////////////////////////////////
// returns the number of types 'default_args<T...>' provided in the given list of arguments TArgs...

template<typename...TArgs>
using count_default_args = count_default_args_impl< type_list< raw_type_t<TArgs>... > >;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename... Args, typename Default_Type = find_default_args_t<F, get_default_args_t<Args...>> >
static RTTR_INLINE 
enable_if_t< std::is_same<Default_Type, default_args<>>::value, Default_Type>
get_default_args(Args&&... arg)
{
    return default_args<>(); // no default arguments provided
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename... Args, typename Default_Type = find_default_args_t<F, get_default_args_t<Args...>> >
static RTTR_INLINE 
enable_if_t< !std::is_same<Default_Type, default_args<>>::value, Default_Type> 
get_default_args(Args&&... arg)
{
    // default arguments are provided, extract them
    auto result = forward_to_vector<Default_Type>(std::forward<Args>(arg)...);
    if (result.size() != 0)
        return result[0];
    else
        return Default_Type();
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_DEFAULT_ARGUMENTS_H_
