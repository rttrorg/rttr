/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
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

#include "rttr/method.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/argument.h"
#include "rttr/instance.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

method::method(const detail::method_wrapper_base* wrapper)
:   m_wrapper(wrapper)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::is_valid() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

method::operator bool() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

string method::get_name() const
{
    if (is_valid())
        return m_wrapper->get_name();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

access_levels method::get_access_level() const
{
    if (is_valid())
        return m_wrapper->get_access_level();
    else
        return access_levels::public_access;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::is_static() const
{
    if (is_valid())
        return m_wrapper->is_static();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

type method::get_return_type() const
{
    if (is_valid())
        return m_wrapper->get_return_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type method::get_declaring_type() const
{
    if (is_valid())
        return m_wrapper->get_declaring_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<parameter_info> method::get_parameter_infos() const
{
    if (is_valid())
        return m_wrapper->get_parameter_infos();
    else
        return std::vector<parameter_info>();
}

/////////////////////////////////////////////////////////////////////////////////////////

string method::get_signature() const
{
    if (is_valid())
        return m_wrapper->get_signature();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::get_metadata(const variant& key) const
{
    if (is_valid())
        return m_wrapper->get_metadata(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object) const
{
    if (is_valid())
        return m_wrapper->invoke(object);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1, argument arg2) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1, arg2);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1, argument arg2, argument arg3) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1, arg2, arg3);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1, argument arg2, argument arg3, argument arg4) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1, arg2, arg3, arg4);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1, argument arg2, argument arg3, argument arg4, 
                       argument arg5) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1, arg2, arg3, arg4, arg5);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(instance object, argument arg1, argument arg2, argument arg3, argument arg4, 
                       argument arg5, argument arg6) const
{
    if (is_valid())
        return m_wrapper->invoke(object, arg1, arg2, arg3, arg4, arg5, arg6);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke_variadic(instance object, std::vector<argument> args) const
{
    if (is_valid())
        return m_wrapper->invoke_variadic(object, args);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::operator==(const method& other) const
{
    return (m_wrapper == other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::operator!=(const method& other) const
{
    return (m_wrapper != other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
