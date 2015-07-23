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

#ifndef RTTR_VARIANT_ARRAY_POLICY_H_
#define RTTR_VARIANT_ARRAY_POLICY_H_

#include "rttr/type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/variant/variant_data_policy.h"
#include "rttr/detail/array/array_mapper.h"
#include "rttr/detail/array/array_accessor.h"
#include "rttr/wrapper_mapper.h"
#include "rttr/detail/variant_array_view/variant_array_view_traits.h"

#include <tuple>

#include <cstdint>

namespace rttr
{
namespace detail
{

struct data_address_container;
struct variant_array_policy_empty;

template<typename T, typename Array_Type = wrapper_address_return_type_t<T>>
struct variant_array_policy_impl;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using variant_array_policy = conditional_t<can_create_array_container<T>::value,
                                           variant_array_policy_impl<T>,
                                           variant_array_policy_empty>;

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief This enum is used in the invoke function to delegate
 *        the execution of certain operations.
 *
 */
enum class variant_array_policy_operation : uint8_t
{
    IS_DYNAMIC,
    GET_RANK,
    GET_RANK_TYPE,
    GET_TYPE,
    IS_VALID,

    GET_SIZE,
    GET_SIZE_1,
    GET_SIZE_2,
    GET_SIZE_VARIADIC,

    SET_SIZE,
    SET_SIZE_1,
    SET_SIZE_2,
    SET_SIZE_VARIADIC,
    
    SET_VALUE,
    SET_VALUE_1,
    SET_VALUE_2,
    SET_VALUE_3,
    SET_VALUE_VARIADIC,

    GET_VALUE,
    GET_VALUE_1,
    GET_VALUE_2,
    GET_VALUE_VARIADIC,

    INSERT_VALUE,
    INSERT_VALUE_1,
    INSERT_VALUE_2,
    INSERT_VALUE_VARIADIC,

    REMOVE_VALUE,
    REMOVE_VALUE_1,
    REMOVE_VALUE_2,
    REMOVE_VALUE_VARIADIC

};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used for raw array types of arbitrary rank, which fit NOT into \p variant_data.
 *
 * A copy of the given array value will be allocated on the heap. The pointer to the array is stored in \p variant_data.
 */
template<typename T, typename Address_Type>
struct variant_array_policy_impl
{
    static RTTR_INLINE typename remove_pointer<Address_Type>::type& get_value(void* const& data)
    {
        return *reinterpret_cast<Address_Type>(const_cast<void*>(data));
    }

    static bool invoke(variant_array_policy_operation op, void* const& src_data, argument_wrapper arg)
    {
        using Array_Type = raw_type_t<Address_Type>;

        switch (op)
        {
            case variant_array_policy_operation::IS_DYNAMIC:
            {
                return array_mapper<Array_Type>::is_dynamic();
                break;
            }
            case variant_array_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<Array_Type>();
                break;
            }
            case variant_array_policy_operation::GET_RANK:
            {
                arg.get_value<std::size_t>() = rank<Array_Type>::value;
                break;
            }
            case variant_array_policy_operation::GET_RANK_TYPE:
            {
                auto& param = arg.get_value<std::tuple<std::size_t, type>>();
                std::get<1>(param) = array_accessor<Array_Type>::get_ranke_type(std::get<0>(param));
                break;
            }
            case variant_array_policy_operation::IS_VALID:
            {
                return true;
                break;
            }
            case variant_array_policy_operation::GET_SIZE:
            {
                arg.get_value<std::size_t>() = array_accessor<Array_Type>::get_size(get_value(src_data));
                break;
            }
            case variant_array_policy_operation::GET_SIZE_1:
            {
                auto& index = arg.get_value<std::size_t>();
                // a little "trick" to avoid unnecessary data cpy, the incoming index will be reused for the result
                index = array_accessor<Array_Type>::get_size(get_value(src_data), index);
                break;
            }
            case variant_array_policy_operation::GET_SIZE_2:
            {
                auto& param = arg.get_value<std::tuple<std::size_t&, std::size_t&>>();
                // a little "trick" to avoid unnecessary data cpy, the incoming index will be reused for the result
                std::get<1>(param) = array_accessor<Array_Type>::get_size(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::GET_SIZE_VARIADIC:
            {
                auto& param = arg.get_value<std::tuple<const std::vector<std::size_t>&, std::size_t&>>();
                std::get<1>(param) = array_accessor<Array_Type>::get_size(get_value(src_data), std::get<0>(param));
                break;
            }

            case variant_array_policy_operation::SET_SIZE:
            {
                return array_accessor<Array_Type>::set_size(get_value(src_data), arg.get_value<std::size_t>());
            }
            case variant_array_policy_operation::SET_SIZE_1:
            {
                auto& param = arg.get_value<std::tuple<std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::set_size(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::SET_SIZE_2:
            {
                auto& param = arg.get_value<std::tuple<std::size_t&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::set_size(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param));
                break;
            }
            case variant_array_policy_operation::SET_SIZE_VARIADIC:
            {
                auto& param = arg.get_value<std::tuple<const std::vector<std::size_t>&, std::size_t&>>();
                return array_accessor<Array_Type>::set_size(get_value(src_data), std::get<1>(param), std::get<0>(param));
                break;
            }

            case variant_array_policy_operation::SET_VALUE:
            {
                return array_accessor<Array_Type>::set_value(get_value(src_data), arg.get_value<argument>());
                break;
            }
            case variant_array_policy_operation::SET_VALUE_1:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&>>();
                return array_accessor<Array_Type>::set_value(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::SET_VALUE_2:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::set_value(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param));
                break;
            }
            case variant_array_policy_operation::SET_VALUE_3:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::set_value(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param), std::get<3>(param));
                break;
            }
            case variant_array_policy_operation::SET_VALUE_VARIADIC:
            {
                auto& param = arg.get_value<std::tuple<detail::argument&, const std::vector<std::size_t>&>>();
                return array_accessor<Array_Type>::set_value(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }

            case variant_array_policy_operation::GET_VALUE:
            {
                auto& param = arg.get_value<std::tuple<variant, std::size_t>>();
                std::get<0>(param) = array_accessor<Array_Type>::get_value(get_value(src_data), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::GET_VALUE_1:
            {
                auto& param = arg.get_value<std::tuple<variant, std::size_t, std::size_t>>();
                std::get<0>(param) = array_accessor<Array_Type>::get_value(get_value(src_data), std::get<1>(param), std::get<2>(param));
                break;
            }
            case variant_array_policy_operation::GET_VALUE_2:
            {
                auto& param = arg.get_value<std::tuple<variant, std::size_t, std::size_t, std::size_t, std::size_t>>();
                std::get<0>(param) = array_accessor<Array_Type>::get_value(get_value(src_data), std::get<1>(param), std::get<2>(param), std::get<3>(param));
                break;
            }
            case variant_array_policy_operation::GET_VALUE_VARIADIC:
            {
                auto& param = arg.get_value<std::tuple<variant, const std::vector<std::size_t>&>>();
                std::get<0>(param) = array_accessor<Array_Type>::get_value(get_value(src_data), std::get<1>(param));
                break;
            }

            case variant_array_policy_operation::INSERT_VALUE:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&>>();
                return array_accessor<Array_Type>::insert_value(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::INSERT_VALUE_1:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::insert_value(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param));
                break;
            }
            case variant_array_policy_operation::INSERT_VALUE_2:
            {
                auto& param = arg.get_value<std::tuple<argument&, std::size_t&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::insert_value(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param), std::get<3>(param));
                break;
            }
            case variant_array_policy_operation::INSERT_VALUE_VARIADIC:
            {
                auto& param = arg.get_value<std::tuple<argument&, const std::vector<std::size_t>&>>();
                return array_accessor<Array_Type>::insert_value(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }

            case variant_array_policy_operation::REMOVE_VALUE:
            {
                return array_accessor<Array_Type>::remove_value(get_value(src_data), arg.get_value<std::size_t>());
                break;
            }
            case variant_array_policy_operation::REMOVE_VALUE_1:
            {
                auto& param = arg.get_value<std::tuple<std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::remove_value(get_value(src_data), std::get<0>(param), std::get<1>(param));
                break;
            }
            case variant_array_policy_operation::REMOVE_VALUE_2:
            {
                auto& param = arg.get_value<std::tuple<std::size_t&, std::size_t&, std::size_t&>>();
                return array_accessor<Array_Type>::remove_value(get_value(src_data), std::get<0>(param), std::get<1>(param), std::get<2>(param));
                break;
            }
            case variant_array_policy_operation::REMOVE_VALUE_VARIADIC:
            {
                return array_accessor<Array_Type>::remove_value(get_value(src_data), arg.get_value<std::vector<std::size_t>>());
                break;
            }
            default: return false;
        }

        return true;
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, void*& dest)
    {
        dest = as_void_ptr(wrapped_raw_addressof(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_ARRAY_POLICY_H_
