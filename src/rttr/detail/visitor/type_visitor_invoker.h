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

#ifndef RTTR_TYPE_VISITOR_INVOKER_H_
#define RTTR_TYPE_VISITOR_INVOKER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/visitor.h"
#include "rttr/detail/visitor/visitor_iterator.h"

namespace rttr
{
namespace detail
{

/*!
 *
 */
template<typename T>
class type_visitor_invoker
{
public:
    type_visitor_invoker(const visitor::type_info<T>& info, type_of_visit visit_type)
    :   m_info(info), m_visit_type(visit_type)
    {
    }

    template<typename U, typename...Base_Classes>
    void call_with_base_classes(U& visitor, type_list<Base_Classes...>) const
    {
        if (m_visit_type == type_of_visit::begin_visit_type)
            visitor.template visit_type_begin<T, Base_Classes...>(m_info);
        else
            visitor.template visit_type_end<T, Base_Classes...>(m_info);
    }

    template<typename U, typename V>
    enable_if_t<has_base_class_list<typename visitor::type_info<U>::declaring_type>::value, void>
    call_impl(V& visitor) const
    {
        using declaring_type_t = typename visitor::type_info<U>::declaring_type;
        call_with_base_classes(visitor, typename declaring_type_t::base_class_list{});
    }

    template<typename U, typename V>
    enable_if_t<!has_base_class_list<typename visitor::type_info<U>::declaring_type>::value, void>
    call_impl(V& visitor) const
    {
        if (m_visit_type == type_of_visit::begin_visit_type)
            visitor.visit_type_begin(m_info);
        else
            visitor.visit_type_end(m_info);
    }


    template<typename U>
    void call(U& visitor) const
    {
        call_impl<T>(visitor);
    }

private:
    const visitor::type_info<T>& m_info;
    type_of_visit m_visit_type;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static type_visitor_invoker<T> make_type_visitor_invoker(const visitor::type_info<T>& info, type_of_visit visit_type)
{
    return type_visitor_invoker<T>(info, visit_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_VISITOR_INVOKER_H_
