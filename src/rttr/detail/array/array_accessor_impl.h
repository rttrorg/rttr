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

#ifndef RTTR_ARRAY_ACCESSOR_IMPL_H_
#define RTTR_ARRAY_ACCESSOR_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/argument.h"
#include "rttr/array_mapper.h"
#include "rttr/variant.h"
#include "rttr/type.h"
#include <type_traits>
#include <cstddef>

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type, typename B>
struct get_size_from_array_impl;

template<typename Array_Type>
struct get_size_from_array_impl<Array_Type, std::true_type>
{
    template<typename... Indices>
    static std::size_t get_size(const Array_Type& array, std::size_t index, Indices... args)
    {
        using sub_type = typename array_mapper<Array_Type>::sub_type;
        using go_one_dim_deeper = typename std::integral_constant<bool,  (sizeof...(Indices) > 0)>::type;
        if (index < array_mapper<Array_Type>::get_size(array))
            return get_size_from_array_impl<sub_type, go_one_dim_deeper>::get_size(array_mapper<Array_Type>::get_value(array, index), args...);
        else
            return 0;
    }
};

template<typename Array_Type>
struct get_size_from_array_impl<Array_Type, std::false_type>
{
    static std::size_t get_size(const Array_Type& array)
    {
        return array_mapper<Array_Type>::get_size(array);
    }
};


template<typename Array_Type, typename... Indices>
std::size_t get_size_from_array(const Array_Type& array, Indices... args)
{
    using go_one_dim_deeper = typename std::integral_constant<bool,  (sizeof...(Indices) > 0) >::type;
    return get_size_from_array_impl<Array_Type, go_one_dim_deeper>::get_size(array, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type, typename B>
struct set_size_to_array_impl;

template<typename Array_Type>
struct set_size_to_array_impl<Array_Type, std::true_type>
{
    template<typename... Indices>
    static bool set_size(Array_Type& array, std::size_t new_size, std::size_t index, Indices... args)
    {
        using sub_type = typename array_mapper<Array_Type>::sub_type;
        using go_one_dim_deeper = typename std::integral_constant<bool,  (sizeof...(Indices) > 0)>::type;
        if (index < array_mapper<Array_Type>::get_size(array))
            return set_size_to_array_impl<sub_type, go_one_dim_deeper>::set_size(array_mapper<Array_Type>::get_value(array, index), new_size, args...);
        else
            return false;
    }
};

template<typename Array_Type>
struct set_size_to_array_impl<Array_Type, std::false_type>
{
    static bool set_size(Array_Type& array, std::size_t new_size)
    {
        return array_mapper<Array_Type>::set_size(array, new_size);
    }
};


template<typename Array_Type, typename... Indices>
bool set_size_to_array(Array_Type& array, std::size_t new_size, Indices... args)
{
    using go_one_dim_deeper = typename std::integral_constant<bool, (sizeof...(Indices) > 0) >::type;
    return set_size_to_array_impl<Array_Type, go_one_dim_deeper>::set_size(array, new_size, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Return_Type, typename Array_Type, typename B>
struct get_value_from_array_impl;

template<typename Return_Type, typename Array_Type>
struct get_value_from_array_impl<Return_Type, Array_Type, std::true_type>
{
    template<typename... Indices>
    static const Return_Type& get_value(const Array_Type& array, std::size_t index, Indices... args)
    {
        using sub_type = typename array_mapper<Array_Type>::sub_type;
        using arg_count_valid = typename std::integral_constant<bool,  sizeof...(Indices) != 0>::type;
        return get_value_from_array_impl<Return_Type, sub_type, arg_count_valid>::get_value(array_mapper<Array_Type>::get_value(array, index), args...);
    }
};

template<typename Return_Type, typename T>
struct get_value_from_array_impl<Return_Type, T, std::false_type>
{
    template<typename... Indices>
    static const T& get_value(const T& value, Indices... args)
    {
        return value;
    }
};

template<typename Array_Type, typename... Indices>
variant get_value_from_array(const Array_Type& array, std::size_t index, Indices... args)
{
    static_assert(rank<Array_Type>::value >= sizeof...(Indices) + 1, "Invalid return type! The number of specified indices are out of range.");
    using return_type = typename rank_type<Array_Type, sizeof...(Indices) + 1>::type;
    using sub_type = typename array_mapper<Array_Type>::sub_type;
    using go_one_dim_deeper = typename std::integral_constant<bool,  sizeof...(Indices) != 0>::type;
    return variant(get_value_from_array_impl<return_type, sub_type, go_one_dim_deeper>::get_value(array_mapper<Array_Type>::get_value(array, index), args...));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ArrayValue, typename B>
struct set_value_to_array_impl;

template<typename ArrayValue>
struct set_value_to_array_impl<ArrayValue, std::true_type>
{
    template<typename T1,  typename... Indices>
    static void set_value(ArrayValue& array, const T1& value, std::size_t index, Indices... args)
    {
        using sub_type = typename std::remove_reference<decltype(array_mapper<ArrayValue>::get_value(array, index))>::type;
        using arg_count_valid = typename std::integral_constant<bool,  sizeof...(Indices) != 0>::type;
        set_value_to_array_impl<sub_type, arg_count_valid>::set_value(array_mapper<ArrayValue>::get_value(array, index), value, args...);
    }
};

template<typename ArrayValue>
struct set_value_to_array_impl<ArrayValue, std::false_type>
{
    template<typename T>
    static void set_value(ArrayValue& array, const T& value)
    {
       array = value;
    }
};

/*!
 * Specialization for std::vector<bool>::reference, 
 * because we cannot call a function with an rvalue by reference, like in the default implementation.
 *
 */
template<>
struct set_value_to_array_impl<std::vector<bool>::reference, std::false_type>
{
    template<typename T>
    static void set_value(std::vector<bool>::reference array, const T& value)
    {
       array = value;
    }
};


template<typename T, size_t N>
struct set_value_to_array_impl<T[N], std::false_type>
{
    static void set_value(T (& arr)[N], const T (& value)[N])
    {
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
        // MSVC is to dumb to let use use the above code, so we have to develop a workaround
        // https://connect.microsoft.com/VisualStudio/feedback/details/884930/strange-ambiguous-compile-error-when-forwarding-multi-dimensional-arrays
        copy_array(const_cast< T (&)[N]>(value), arr);
#else
        copy_array(value, arr);
#endif
    }
};

template<typename Array_Type, typename F, typename... Indices>
bool set_value_to_array(Array_Type& array, const F& value, std::size_t index, Indices... args)
{
    // MSVC is to dumb to let us put this in the argument list
    static_assert(std::is_same<typename rank_type<Array_Type, sizeof...(Indices) + 1>::type, F>::value, "invalid type!");
    using sub_type = typename std::remove_reference<decltype(array_mapper<Array_Type>::get_value(array, index))>::type;
    using go_one_dim_deeper = typename std::integral_constant<bool,  sizeof...(Indices) != 0>::type;
    set_value_to_array_impl<sub_type, go_one_dim_deeper>::set_value(array_mapper<Array_Type>::get_value(array, index), value, args...);
    return true;
}

template<typename ArrayValue, typename T>
bool set_value_to_array(ArrayValue& arg, const T& value)
{
    set_value_to_array_impl<ArrayValue, std::false_type>::set_value(arg, value);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////


template<typename Array_Type, typename B>
struct insert_value_to_array_impl;

template<typename Array_Type>
struct insert_value_to_array_impl<Array_Type, std::true_type>
{
    template<typename T, typename... Indices>
    static bool insert_value(Array_Type& array, const T& value, std::size_t index, Indices... args)
    {
        using sub_type = typename array_mapper<Array_Type>::sub_type;
        using arg_count_valid = typename std::integral_constant<bool,  (sizeof...(Indices) > 1)>::type;
        return insert_value_to_array_impl<sub_type, arg_count_valid>::insert_value(array_mapper<Array_Type>::get_value(array, index), value, args...);
    }
};

template<typename Array_Type>
struct insert_value_to_array_impl<Array_Type, std::false_type>
{
    template<typename T>
    static bool insert_value(Array_Type& array, const T& value, std::size_t index)
    {
        return array_mapper<Array_Type>::insert_value(array, value, index);
    }
};


template<typename Array_Type, typename F, typename... Indices>
bool insert_value_to_array(Array_Type& array, const F& value, Indices... args)
{
    using go_one_dim_deeper = typename std::integral_constant<bool,  (sizeof...(Indices) > 1) >::type;
    return insert_value_to_array_impl<Array_Type, go_one_dim_deeper>::insert_value(array, value, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////


template<typename Array_Type, typename B>
struct remove_value_from_array_impl;

template<typename Array_Type>
struct remove_value_from_array_impl<Array_Type, std::true_type>
{
    template<typename... Indices>
    static bool remove_value(Array_Type& array, std::size_t index, Indices... args)
    {
        using sub_type = typename array_mapper<Array_Type>::sub_type;
        using arg_count_valid = typename std::integral_constant<bool,  (sizeof...(Indices) > 1) >::type;
        if (index < array_mapper<Array_Type>::get_size(array))
            return remove_value_from_array_impl<sub_type, arg_count_valid>::remove_value(array_mapper<Array_Type>::get_value(array, index), args...);
        else
            return false;
    }
};

template<typename Array_Type>
struct remove_value_from_array_impl<Array_Type, std::false_type>
{
    static bool remove_value(Array_Type& array, std::size_t index)
    {
        if (index < array_mapper<Array_Type>::get_size(array))
            return array_mapper<Array_Type>::remove_value(array, index);
        else
            return false;
    }
};


template<typename Array_Type, typename... Indices>
bool remove_value_from_array(Array_Type& array, Indices... args)
{
    using go_one_dim_deeper = typename std::integral_constant<bool,  (sizeof...(Indices) > 1) >::type;
    return remove_value_from_array_impl<Array_Type, go_one_dim_deeper>::remove_value(array, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type, typename A>
struct array_accessor_impl;

template<typename Array_Type>
struct array_accessor_impl<Array_Type, std::true_type>
{
    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static std::size_t get_size(const Array_Type& obj, Indices... args)
    {
        return get_size_from_array<Array_Type>(obj, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool set_size(Array_Type& obj, std::size_t new_size, Indices... args)
    {
        return set_size_to_array<Array_Type>(obj, new_size, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    
    template<typename... Indices>
    static variant get_value(const Array_Type& obj, Indices... indices)
    {
        return get_value_from_array<Array_Type>(obj, indices...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool set_value(Array_Type& obj, argument& arg, Indices... indices)
    {
        using arg_type = typename rank_type<Array_Type, sizeof...(Indices)>::type;
        if (arg.is_type<arg_type>())
            return set_value_to_array<Array_Type>(obj, arg.get_value<arg_type>(), indices...);
        else
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool insert_value(Array_Type& obj, argument& arg, Indices... indices)
    {
        using arg_type = typename rank_type<Array_Type, sizeof...(Indices)>::type;
        if (arg.is_type<arg_type>())
            return insert_value_to_array<Array_Type>(obj, arg.get_value<arg_type>(), indices...);
        else
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool remove_value(Array_Type& obj, Indices... indices)
    {
        return remove_value_from_array<Array_Type>(obj, indices...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
};

// index count is bigger then the array rank => invalid call
template<typename Array_Type>
struct array_accessor_impl<Array_Type, std::false_type>
{
    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static std::size_t get_size(const Array_Type& obj, Indices... args)
    {
        return 0;
    }

    template<typename... Indices>
    static bool set_size(Array_Type& obj, std::size_t new_size, Indices... args)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    
    template<typename... Indices>
    static variant get_value(const Array_Type& obj, Indices... indices)
    {
        return variant();
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool set_value(Array_Type& obj, argument& arg, Indices... indices)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool insert_value(Array_Type& obj, argument& arg, Indices... indices)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool remove_value(Array_Type& obj, Indices... indices)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
};


template<typename T, typename IndexCount>
struct array_accessor_variadic;

template<typename Array_Type, std::size_t... N>
struct array_accessor_variadic<Array_Type, index_sequence<N...>>
{
    /////////////////////////////////////////////////////////////////////////////////////////

    static std::size_t get_size(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant< bool, (sizeof...(N) < rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::get_size(obj, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_size(Array_Type& obj, std::size_t new_size, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant< bool, (sizeof...(N) < rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::set_size(obj, new_size, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static variant get_value(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(N) <= rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::get_value(obj, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(N) <= rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::set_value(obj, arg, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool insert_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(N) <= rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::insert_value(obj, arg, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool remove_value(Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(N) <= rank<Array_Type>::value) >::type;
        return array_accessor_impl<Array_Type, is_rank_in_range>::remove_value(obj, index_list[N]...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
};

template<typename Array_Type, std::size_t N>
struct array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N>>
{
    /////////////////////////////////////////////////////////////////////////////////////////

    static std::size_t get_size(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::get_size(obj, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::get_size(obj, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_size(Array_Type& obj, std::size_t new_size, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::set_size(obj, new_size, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::set_size(obj, new_size, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static variant get_value(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::get_value(obj, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::get_value(obj, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::set_value(obj, arg, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::set_value(obj, arg, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool insert_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::insert_value(obj, arg, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::insert_value(obj, arg, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool remove_value(Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        if (index_list.size() == N)
            return array_accessor_variadic<Array_Type, make_index_sequence<N>>::remove_value(obj, index_list);
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::remove_value(obj, index_list);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static type get_ranke_type(std::size_t index)
    {
        if (index == N)
            return type::get<typename rank_type<Array_Type, N>::type>();
        else
            return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, N - 1>>::get_ranke_type(index);
    }
};

template<typename Array_Type>
struct array_accessor_impl<Array_Type, std::integral_constant<std::size_t, 0>>
{
    /////////////////////////////////////////////////////////////////////////////////////////

    static std::size_t get_size(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        return array_accessor_impl<Array_Type, std::true_type>::get_size(obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_size(Array_Type& obj, std::size_t new_size, const std::vector<std::size_t>& index_list)
    {
        return array_accessor_impl<Array_Type, std::true_type>::set_size(obj, new_size);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static variant get_value(const Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        return variant(); // one index at least needed, otherwise the whole array would be returned
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool set_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        // copy the whole array
        return array_accessor_impl<Array_Type, std::true_type>::set_value(obj, arg);
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool insert_value(Array_Type& obj, argument& arg, const std::vector<std::size_t>& index_list)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static bool remove_value(Array_Type& obj, const std::vector<std::size_t>& index_list)
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    static type get_ranke_type(std::size_t index)
    {
        return type::get<typename rank_type<Array_Type, 0>::type>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
std::size_t array_accessor<Array_Type>::get_size(const Array_Type& array, Indices... args)
{
    using is_rank_in_range = typename std::integral_constant< bool, (sizeof...(Indices) < rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::get_size(array, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
std::size_t array_accessor<Array_Type>::get_size(const Array_Type& array, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() < rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value - 1>>::get_size(array, index_list);
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::set_size(const Array_Type&, std::size_t, Indices... indices) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::set_size(const Array_Type&, std::size_t, const std::vector<std::size_t>&) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::set_size(Array_Type& array, std::size_t new_size, Indices... args)
{
    using is_rank_in_range = typename std::integral_constant< bool, (sizeof...(Indices) < rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::set_size(array, new_size, args...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::set_size(Array_Type& array, std::size_t new_size, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() < rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value - 1>>::set_size(array, new_size, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
variant array_accessor<Array_Type>::get_value(const Array_Type& array, Indices... indices)
{
    using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(Indices) <= rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::get_value(array, indices...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
variant array_accessor<Array_Type>::get_value(const Array_Type& array, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() <= rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value>>::get_value(array, index_list);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::set_value(const Array_Type&, argument&, Indices... indices) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::set_value(const Array_Type&, argument&, const std::vector<std::size_t>&) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::set_value(Array_Type& array, argument& arg, Indices... indices)
{
    using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(Indices) <= rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::set_value(array, arg, indices...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::set_value(Array_Type& array, argument& arg, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() <= rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value>>::set_value(array, arg, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::insert_value(const Array_Type&, argument&, Indices... indices) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::insert_value(const Array_Type&, argument&, const std::vector<std::size_t>&) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::insert_value(Array_Type& array, argument& arg, Indices... indices)
{
    using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(Indices) <= rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::insert_value(array, arg, indices...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::insert_value(Array_Type& array, argument& arg, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() <= rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value>>::insert_value(array, arg, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::remove_value(const Array_Type&, Indices... indices) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::remove_value(const Array_Type&, const std::vector<std::size_t>&) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
template<typename... Indices>
bool array_accessor<Array_Type>::remove_value(Array_Type& array, Indices... indices)
{
    using is_rank_in_range = typename std::integral_constant<bool, (sizeof...(Indices) <= rank<Array_Type>::value) >::type;
    return array_accessor_impl<Array_Type, is_rank_in_range>::remove_value(array, indices...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
bool array_accessor<Array_Type>::remove_value(Array_Type& array, const std::vector<std::size_t>& index_list)
{
    if (index_list.size() <= rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value>>::remove_value(array, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
type array_accessor<Array_Type>::get_ranke_type(std::size_t index)
{
    if (index <= rank<Array_Type>::value)
        return array_accessor_impl<Array_Type, std::integral_constant<std::size_t, rank<Array_Type>::value>>::get_ranke_type(index);
    else
        return get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ARRAY_ACCESSOR_IMPL_H_
