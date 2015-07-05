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

#ifndef RTTR_VARIANT_DATA_POLICY_EMPTY_H_
#define RTTR_VARIANT_DATA_POLICY_EMPTY_H_

#include "rttr/detail/variant/variant_data_policy.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used when the variant does not contain any data. So in fact an invalid variant.
 *
 * With this approach we avoid checking for an valid variant. E.g. during destruction.
 */
struct RTTR_API variant_data_policy_empty
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, any* arg)
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
                arg->m_data = nullptr;
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                type& t = *static_cast<type*>(arg->m_data);
                t = get_invalid_type();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg->m_data = nullptr;
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                type& t = *static_cast<type*>(arg->m_data);
                t = get_invalid_type();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg->m_data = nullptr;
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = *static_cast<data_address_container*>(arg->m_data);

                data.m_type                         = get_invalid_type();
                data.m_wrapped_type                 = get_invalid_type();
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
                arg->m_data = nullptr;
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return false;
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

#endif // RTTR_VARIANT_DATA_POLICY_EMPTY_H_
