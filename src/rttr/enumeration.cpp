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

#include "rttr/enumeration.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/argument.h"

#include <utility>

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

enumeration::enumeration(const detail::enumeration_wrapper_base* wrapper)
:   m_wrapper(wrapper)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::is_valid() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration::operator bool() const
{
    return (m_wrapper ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_underlying_type() const
{
    if (is_valid())
        return m_wrapper->get_underlying_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_type() const
{
    if (is_valid())
        return m_wrapper->get_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_declaring_type() const
{
    if (is_valid())
        return m_wrapper->get_declaring_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant enumeration::get_metadata(const variant& key) const
{
    if (is_valid())
        return m_wrapper->get_metadata(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<string> enumeration::get_names() const
{
    if (is_valid())
        return m_wrapper->get_names();
    else
        return vector<string>();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<variant> enumeration::get_values() const
{
    if (is_valid())
        return m_wrapper->get_values();
    else
        return vector<variant>();
}

/////////////////////////////////////////////////////////////////////////////////////////

string enumeration::value_to_name(argument value) const
{
    if (is_valid())
        return m_wrapper->value_to_name(value);
    else
        return string();
}
        
/////////////////////////////////////////////////////////////////////////////////////////

variant enumeration::name_to_value(const std::string& name) const
{
    if (is_valid())
        return m_wrapper->name_to_value(name);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::operator==(const enumeration& other) const
{
    return (m_wrapper == other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::operator!=(const enumeration& other) const
{
    return (m_wrapper != other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
