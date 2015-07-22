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

#ifndef RTTR_VARIANT_ARRAY_VIEW_IMPL_H_
#define RTTR_VARIANT_ARRAY_VIEW_IMPL_H_

#include "rttr/variant.h"
#include "rttr/detail/variant_array_view/variant_array_view_policy.h"
#include "rttr/detail/variant_array_view/variant_array_view_policy_empty.h"

#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"
#include "rttr/variant.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::variant_array_view()
:    m_data({nullptr, &detail::variant_array_view_policy_empty::invoke})
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::variant_array_view(const variant_array_view& other)
:   m_data(other.m_data)
{

}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::~variant_array_view()
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant_array_view::swap(variant_array_view& other)
{
    std::swap(m_data, other.m_data);
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view& variant_array_view::operator=(const variant_array_view& other)
{
    variant_array_view(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_VARIANT_ARRAY_VIEW_IMPL_H_
