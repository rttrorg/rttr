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

#ifndef RTTR_VARIANT_DATA_POLICY_ARITHMETIC_H_
#define RTTR_VARIANT_DATA_POLICY_ARITHMETIC_H_

#include "rttr/detail/variant/variant_data.h"

namespace rttr
{
namespace detail
{

enum class variant_policy_operation : uint8_t;

/*!
 * This policy is used for all arithmetic types, which fit into the storage of variant_data.
 *
 * The data will be raw copied into the variant_data.
 */
template<typename T, typename Converter>
struct variant_data_policy_arithmetic
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        #define EXTRACT_DATA(src) reinterpret_cast<T&>(const_cast<variant_data&>(src))

        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            {
                break;
            }
            case variant_policy_operation::CLONE:
            {
                arg.get_value<variant_data>() = src_data;
                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<T>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<typename raw_type<T>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();

                data.m_type                         = type::get< raw_addressof_return_type_t<T> >();
                data.m_wrapped_type                 = type::get< wrapper_address_return_type_t<T> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return ::rttr::detail::is_array<typename raw_type<T>::type>::value;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<variant_array_data&>() = create_variant_array(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            case variant_policy_operation::CONVERT:
            {
                argument& arg_1 = arg.get_value<argument>();
                const type target_type = arg_1.get_type();
                bool result = false;
                if (target_type == type::get<bool>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<bool>());
                else if (target_type == type::get<char>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<char>());
                else if (target_type == type::get<int8>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<int8>());
                else if (target_type == type::get<int16>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<int16>());
                else if (target_type == type::get<int32>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<int32>());
                else if (target_type == type::get<int64>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<int64>());
                else if (target_type == type::get<uint8>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<uint8>());
                else if (target_type == type::get<uint16>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<uint16>());
                else if (target_type == type::get<uint32>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<uint32>());
                else if (target_type == type::get<uint64>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<uint64>());
                else if (target_type == type::get<float>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<float>());
                else if (target_type == type::get<double>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<double>());
                else if (target_type == type::get<std::string>())
                    return Converter::convert(EXTRACT_DATA(src_data), arg_1.get_value<std::string>());
                else
                    return false;
            }
        }

        return true;
        #undef EXTRACT_DATA
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {
        reinterpret_cast<T&>(dest) = value;
    }
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_DATA_POLICY_ARITHMETIC_H_
