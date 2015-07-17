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

#ifndef RTTR_VARIANT_DATA_POLICY_VOID_H_
#define RTTR_VARIANT_DATA_POLICY_VOID_H_

#include "rttr/detail/variant/variant_data_policy.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used when the variant does contain a `void`.
 *
 * A `void` variant is a special variant to indicate that a function call was successful.
 * So in fact it does not contain any data, but the returned type of \ref variant::get_type() is a `void`.
 */
struct RTTR_API variant_data_policy_void
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            case variant_policy_operation::CLONE:
            {
                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = nullptr;
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<void>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = nullptr;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<void>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = nullptr;
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();

                data.m_type                         = type::get<void>();
                data.m_wrapped_type                 = type::get<void>();
                data.m_data_address                 = nullptr;
                data.m_data_address_wrapped_type    = nullptr;
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return false;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<void*>() = nullptr;
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            case variant_policy_operation::CONVERT:
            {
                return false;
            }
        }
        return true;
    }

    template<typename U>
    static RTTR_INLINE void create(U&&, variant_data&)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_DATA_POLICY_VOID_H_
