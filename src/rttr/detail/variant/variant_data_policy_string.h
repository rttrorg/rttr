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

#ifndef RTTR_VARIANT_DATA_POLICY_STRING_H_
#define RTTR_VARIANT_DATA_POLICY_STRING_H_

#include "rttr/type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/variant/variant_data_policy.h"
#include "rttr/detail/variant/variant_data_converter.h"

namespace rttr
{
namespace detail
{

/*!
 * This policy will manage the type std::string.
 *
 */
template<typename Converter>
struct variant_data_policy_string
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, any* arg)
    {
        #define EXTRACT_DATA(src) *reinterpret_cast<std::string*&>(const_cast<variant_data&>(src))

        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            {
                delete reinterpret_cast<std::string*&>(const_cast<variant_data&>(src_data));
                reinterpret_cast<std::string*&>(const_cast<variant_data&>(src_data)) = nullptr;
                break;
            }
            case variant_policy_operation::CLONE:
            {
                variant_data& dest = *static_cast<variant_data*>(arg->m_data);
                reinterpret_cast<std::string*&>(dest) = new std::string(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg->m_data = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                type& t = *static_cast<type*>(arg->m_data);
                t = type::get<std::string>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg->m_data = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                type& t = *static_cast<type*>(arg->m_data);
                t = type::get<typename raw_type<std::string>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg->m_data = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = *static_cast<data_address_container*>(arg->m_data);

                data.m_type                         = type::get< raw_addressof_return_type_t<std::string> >();
                data.m_wrapped_type                 = type::get< wrapper_adress_return_type_t<std::string> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return ::rttr::detail::is_array<typename raw_type<std::string>::type>::value;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg->m_data = create_array_container(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            case variant_policy_operation::CONVERT:
            {
                argument& arg_1 = *static_cast<argument*>(arg->m_data);
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

                break;
            }
            default: return false;
        }

        return true;
        #undef EXTRACT_DATA
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {
        reinterpret_cast<std::string*&>(dest) = new std::string(std::forward<U>(value));
    }

    template<std::size_t N>
    static RTTR_INLINE void create(const char (&value)[N], variant_data& dest)
    {
        reinterpret_cast<std::string*&>(dest) = new std::string(value, N - 1);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_DATA_POLICY_STRING_H_
