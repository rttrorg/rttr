/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
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
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/detail/array/array_wrapper_base.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::variant_array_view() RTTR_NOEXCEPT
:    m_array_wrapper(detail::make_unique<detail::array_wrapper_base>())
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::variant_array_view(const variant_array_view& other) RTTR_NOEXCEPT
:   m_array_wrapper(std::move(other.m_array_wrapper->clone()))
{

}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::variant_array_view(variant_array_view&& other) RTTR_NOEXCEPT
:   m_array_wrapper(std::move(other.m_array_wrapper))
{

}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view::~variant_array_view() RTTR_NOEXCEPT
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant_array_view::swap(variant_array_view& other) RTTR_NOEXCEPT
{
    std::swap(m_array_wrapper, other.m_array_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant_array_view& variant_array_view::operator=(const variant_array_view& other) RTTR_NOEXCEPT
{
    variant_array_view(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_VARIANT_ARRAY_VIEW_IMPL_H_
