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

#ifndef __RTTR_UTILITY_H__
#define __RTTR_UTILITY_H__

#include "rttr/base/core_prerequisites.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// This will add the c++14 integer sequence to c++11

    template <class T, T... I>
    struct integer_sequence
    {
        template <T N> using append = integer_sequence<T, I..., N>;
        static std::size_t size() { return sizeof...(I); }
        using next = append<sizeof...(I)>;
        using type = T;
    };

    template <class T, T Index, std::size_t N>
    struct sequence_generator
    {
        using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
    };

    template <class T, T Index>
    struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

    template <std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;

#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    // workaround for a compiler bug of nested aliases (#1085630)
    template <class T, T N>
    struct make_integer_sequence_impl
    {
        typedef typename sequence_generator<T, N, N>::type type;
    };

    template <class T, T N>
    struct make_index_sequence_impl
    {
        typedef typename make_integer_sequence_impl<T, N>::type type;
    };

    template <class T, T N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

    template <std::size_t N>
    using make_index_sequence = typename make_integer_sequence_impl<std::size_t, N>::type;
#else
    template <class T, T N>
    using make_integer_sequence = typename sequence_generator<T, N, N>::type;

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;
#endif

    template<class... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct remove_first_index_impl
{
    using type = index_sequence<>;
};

template<std::size_t First, std::size_t... I>
struct remove_first_index_impl<detail::index_sequence<First, I...>>
{
    using type = detail::index_sequence<I...>;
};

template<typename T>
using remove_first_index = typename remove_first_index_impl<T>::type;


/////////////////////////////////////////////////////////////////////////////////////////

template<typename, typename>
struct concat_index_sequence { };

template<std::size_t... Ts, std::size_t... Us>
struct concat_index_sequence<index_sequence<Ts...>, index_sequence<Us...>>
{
    using type = index_sequence<Ts..., Us...>;
};

template <class T>
struct remove_last_index_impl;

template <size_t Last>
struct remove_last_index_impl<index_sequence<Last>>
{
    using type = index_sequence<>;
};

template<std::size_t First, std::size_t... I>
struct remove_last_index_impl<index_sequence<First, I...>>
{
    using type = typename concat_index_sequence<
                index_sequence<First>,
                typename remove_last_index_impl<index_sequence<I...>>::type
                >::type;
};

template<typename T>
using remove_last_index = typename remove_last_index_impl<T>::type;



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// This will add the c++14 integer sequence to c++11


static RTTR_FORCE_INLINE bool check_all_true() { return true; }

template<typename... BoolArgs>
static RTTR_INLINE bool check_all_true(bool arg1, BoolArgs... args) { return arg1 & check_all_true(args...); }

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// copy the content of any arbitrary array, use it like: 
// copy_array(in, out);
// works with every dimension

template<typename ElementType>
struct copy_array_helper_impl
{
    void operator()(const ElementType &in, ElementType &out)
    {
        out = in;
    }
};

template<typename ElementType, std::size_t Count>
struct copy_array_helper_impl<ElementType[Count]> {
    void operator()(const ElementType (&in)[Count], ElementType (&out)[Count])
    {
        for(std::size_t i = 0; i < Count; ++i)
            copy_array_helper_impl<ElementType>()(in[i], out[i]);
    }
};

template<typename ElementType, std::size_t Count>
auto copy_array(const ElementType (&in)[Count], ElementType (&out)[Count])
    -> ElementType (&)[Count]
{
    copy_array_helper_impl<ElementType[Count]>()(in, out);
    return out;
}

} // end namespace detail
} // end namespace rttr

#endif //__RTTR_UTILITY_H__
