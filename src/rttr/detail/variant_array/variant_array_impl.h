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

#ifndef RTTR_VARIANT_ARRAY_IMPL_H_
#define RTTR_VARIANT_ARRAY_IMPL_H_

#include "rttr/variant.h"
#include "rttr/detail/variant_array/variant_array_policy.h"
#include "rttr/detail/variant_array/variant_array_policy_empty.h"

#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array()
:    m_data({nullptr, &detail::variant_array_policy_empty::invoke})
{
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
variant_array::variant_array(T&& val)
:   m_variant(std::forward<T>(val)),
    m_data(m_variant.to_array())

{
   static_assert(detail::can_create_array_container<T>::value, "No Array type provided, please provide a specialization with rttr::detail::array_mapper<T>.");
   static_assert(!detail::is_array<variant>::value, "Not allowed to create a variant_array from variant.");
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(const variant_array& other)
:   m_variant(other.m_variant),
    m_data(m_variant.to_array())
{

}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(variant_array&& other)
:   m_variant(std::move(other.m_variant)),
    m_data(m_variant.to_array())
{
    other.m_data = {nullptr, &detail::variant_array_policy_empty::invoke};
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(const variant& var)
:   m_variant(var),
    m_data(m_variant.to_array())

{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::variant_array(variant&& var)
:   m_variant(std::move(var)),
    m_data(m_variant.to_array())
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array::~variant_array()
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant_array::swap(variant_array& other)
{
    std::swap(m_variant, other.m_variant);
    std::swap(m_data, other.m_data);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
RTTR_INLINE variant_array& variant_array::operator=(T&& other)
{
    variant_array(std::forward<T>(other)).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array& variant_array::operator=(const variant_array& other)
{
    variant_array(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array& variant_array::operator=(variant_array&& other)
{
    other.swap(*this);
    variant_array().swap(other);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_VARIANT_ARRAY_IMPL_H_
