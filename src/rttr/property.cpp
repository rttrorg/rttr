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

#include "rttr/property.h"

#include "rttr/detail/property/property_wrapper_base.h"
#include "rttr/variant.h"
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/enumeration.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

property::property(const detail::property_wrapper_base* wrapper)
:   m_wrapper(wrapper)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::is_valid() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

property::operator bool() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

access_levels property::get_access_level() const
{
    if (is_valid())
        return m_wrapper->get_access_level();
    else
        return access_levels::public_access;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::is_readonly() const
{
    if (is_valid())
        return m_wrapper->is_readonly();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::is_static() const
{
    if (is_valid())
        return m_wrapper->is_static();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::is_enumeration() const
{
    if (is_valid())
        return m_wrapper->get_type().is_enumeration();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration property::get_enumeration() const
{
    if (is_valid())
        return m_wrapper->get_type().get_enumeration();
    else
        return detail::get_invalid_type().get_enumeration();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::is_array() const
{
    if (is_valid())
        return m_wrapper->is_array();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

string property::get_name() const
{
    if (is_valid())
        return m_wrapper->get_name();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

type property::get_type() const
{
    if (is_valid())
        return m_wrapper->get_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type property::get_declaring_type() const
{
    if (is_valid())
        return m_wrapper->get_declaring_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::set_value(instance object, argument arg) const
{
    if (is_valid())
        return m_wrapper->set_value(object, arg);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant property::get_value(instance object) const
{
    if (is_valid())
        return m_wrapper->get_value(object);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant property::get_metadata(const variant& key) const
{
    if (is_valid())
        return m_wrapper->get_metadata(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::operator==(const property& other) const
{
    return (m_wrapper == other.m_wrapper); 
}

/////////////////////////////////////////////////////////////////////////////////////////

bool property::operator!=(const property& other) const
{
    return (m_wrapper != other.m_wrapper); 
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
