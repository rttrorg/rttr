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

#ifndef RTTR_CONSTRUCTOR_VISITOR_INVOKER_H_
#define RTTR_CONSTRUCTOR_VISITOR_INVOKER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/visitor.h"

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
class constructor_visitor_invoker
{
public:
    constructor_visitor_invoker(const visitor::constructor_info<T>& info)
    :   m_info(info)
    {
    }

    template<typename U, typename...Ctor_Args>
    void call_impl(U& visitor, type_list<Ctor_Args...>) const
    {
        visitor.template visit_constructor<T, Ctor_Args...>(m_info);
    }

    template<typename U>
    void call(U& visitor) const
    {
        using ctor_args = typename visitor::constructor_info<T>::ctor_args;
        call_impl(visitor, ctor_args{});
    }

private:
    const visitor::constructor_info<T>& m_info;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static constructor_visitor_invoker<T> make_ctor_visitor_invoker(const visitor::constructor_info<T>& info)
{
    return constructor_visitor_invoker<T>(info);
}

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 *
 */
template<typename T>
class constructor_function_visitor_invoker
{
public:
    constructor_function_visitor_invoker(const visitor::constructor_function_info<T>& info)
    :   m_info(info)
    {
    }

    template<typename U>
    void call(U& visitor) const
    {
        visitor.visit_constructor_function(m_info);
    }

private:
    const visitor::constructor_function_info<T>& m_info;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static constructor_function_visitor_invoker<T> make_ctor_visitor_invoker_func(const visitor::constructor_function_info<T>& info)
{
    return constructor_function_visitor_invoker<T>(info);
}

} // end namespace detail
} // end namespace rttr

#endif // RTTR_CONSTRUCTOR_VISITOR_INVOKER_H_
