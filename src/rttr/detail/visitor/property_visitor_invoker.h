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

#ifndef RTTR_PROPERTY_VISITOR_INVOKER_H_
#define RTTR_PROPERTY_VISITOR_INVOKER_H_

#include "rttr/detail/base/core_prerequisites.h"


namespace rttr
{
class method;

namespace detail
{

struct invalid_type;
struct read_only;

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 *
 */
template<typename T, typename Policy = void>
class property_visitor_invoker
{

private:
    using declaring_type_t = typename visitor::property_info<T>::declaring_type;

    template<typename U>
    using is_global_property = std::is_same<U, invalid_type>;

    template<typename U>
    using is_read_only = std::is_same<U, read_only>;


    /////////////////////////////////////////////////////////////////////////////////////

    template<typename U, typename P, typename V>
    enable_if_t<!is_global_property<U>::value && !is_read_only<P>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_property(m_info);
    }

    template<typename U, typename P, typename V>
    enable_if_t<!is_global_property<U>::value && is_read_only<P>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_readonly_property(m_info);
    }

    template<typename U, typename P, typename V>
    enable_if_t<is_global_property<U>::value && !is_read_only<P>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_global_property(m_info);
    }

    template<typename U, typename P, typename V>
    enable_if_t<is_global_property<U>::value && is_read_only<P>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_global_readonly_property(m_info);
    }

    /////////////////////////////////////////////////////////////////////////////////////

public:
    property_visitor_invoker(const visitor::property_info<T>& info)
    :   m_info(info)
    {
    }

    template<typename W>
    void call(W& visitor) const
    {
        call_impl<declaring_type_t, Policy>(visitor);
    }

private:
    const visitor::property_info<T>& m_info;
};


/////////////////////////////////////////////////////////////////////////////////////////
//! \param Policy Specifies whether the property is read only or not.
template<typename Policy = void, typename T = void>
static property_visitor_invoker<T, Policy> make_property_visitor_invoker(const visitor::property_info<T>& info)
{
    return property_visitor_invoker<T, Policy>(info);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * The visitor invoker for getter/setter based properties.
 */
template<typename T>
class property_getter_setter_visitor_invoker
{

private:
    using declaring_type_t = typename visitor::property_getter_setter_info<T>::declaring_type;

    template<typename U>
    using is_global_property = std::is_same<U, invalid_type>;

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename U, typename V>
    enable_if_t<!is_global_property<U>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_getter_setter_property(m_info);
    }


    template<typename U, typename V>
    enable_if_t<is_global_property<U>::value, void>
    call_impl(V& visitor) const
    {
        visitor.visit_global_getter_setter_property(m_info);
    }

    /////////////////////////////////////////////////////////////////////////////////////

public:
    property_getter_setter_visitor_invoker(const visitor::property_getter_setter_info<T>& info)
    :   m_info(info)
    {
    }

    template<typename V>
    void call(V& visitor) const
    {
        call_impl<declaring_type_t>(visitor);
    }

private:
    const visitor::property_getter_setter_info<T>& m_info;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static property_getter_setter_visitor_invoker<T>
make_property_getter_setter_visitor_invoker(const visitor::property_getter_setter_info<T>& info)
{
    return property_getter_setter_visitor_invoker<T>(info);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_PROPERTY_VISITOR_INVOKER_H_
