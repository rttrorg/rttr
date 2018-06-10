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

#ifndef RTTR_CREATE_TYPE_VISITOR_FUNC_IMPL_H_
#define RTTR_CREATE_TYPE_VISITOR_FUNC_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/visitor.h"
#include "rttr/detail/visitor/visitor_iterator.h"
#include "rttr/detail/visitor/type_visitor_invoker.h"

namespace rttr
{
namespace detail
{


/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Visitor_list, typename...Base_Classes>
RTTR_INLINE void visit_type_impl(type_of_visit visit_type, visitor& vi, const type& t, type_list<Base_Classes...>)
{
    auto obj = make_type_visitor_info<T, Base_Classes...>(t);
    visitor_iterator<Visitor_list>::visit(vi, make_type_visitor_invoker(obj, visit_type));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Visitor_list>
RTTR_INLINE
enable_if_t<has_base_class_list<T>::value, void>
visit_type(type_of_visit visit_type, visitor& vi, const type& t)
{
    visit_type_impl<T, Visitor_list>(visit_type, vi, t, typename T::base_class_list{});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Visitor_list>
RTTR_INLINE
enable_if_t<!has_base_class_list<T>::value, void>
visit_type(type_of_visit visit_type, visitor& vi, const type& t)
{
    auto obj = make_type_visitor_info<T>(t);
    visitor_iterator<Visitor_list>::visit(vi, make_type_visitor_invoker(obj, visit_type));
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_CREATE_TYPE_VISITOR_FUNC_H_
