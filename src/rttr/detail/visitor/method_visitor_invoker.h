/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_METHOD_VISITOR_INVOKER_H_
#define RTTR_METHOD_VISITOR_INVOKER_H_

#include "rttr/detail/base/core_prerequisites.h"


namespace rttr
{
class method;

namespace detail
{

struct invalid_type;

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 *
 */
template<typename T>
class method_visitor_invoker
{

private:
    using declaring_type_t = typename visitor::method_info<T>::declaring_type;

    template<typename U>
    using is_global_method = std::is_same<U, invalid_type>;

    template<typename V, typename U>
    enable_if_t<is_global_method<V>::value, void>
    call_impl(U& visitor) const
    {
        visitor.visit_global_method(m_info);
    }

    template<typename V, typename U>
    enable_if_t<!is_global_method<V>::value, void>
    call_impl(U& visitor) const
    {
        visitor.visit_method(m_info);
    }

public:
    method_visitor_invoker(const visitor::method_info<T>& info)
    :   m_info(info)
    {
    }

    template<typename U>
    void call(U& visitor) const
    {
        call_impl<declaring_type_t>(visitor);
    }

private:
    const visitor::method_info<T>& m_info;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static method_visitor_invoker<T> make_method_visitor_invoker(const visitor::method_info<T>& info)
{
    return method_visitor_invoker<T>(info);
}

} // end namespace detail
} // end namespace rttr

#endif // RTTR_METHOD_VISITOR_INVOKER_H_
