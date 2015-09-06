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

#include "rttr/enumeration.h"
#include "rttr/detail/enumeration/enumeration_container_base.h"
#include "rttr/detail/argument.h"

#include <utility>

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

enumeration::enumeration(const detail::enumeration_container_base* container)
:   m_container(container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::is_valid() const
{
    return (m_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration::operator bool() const
{
    return (m_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_underlying_type() const
{
    if (is_valid())
        return m_container->get_underlying_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_type() const
{
    if (is_valid())
        return m_container->get_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type enumeration::get_declaring_type() const
{
    if (is_valid())
        return m_container->get_declaring_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant enumeration::get_metadata(const variant& key) const
{
    if (is_valid())
        return m_container->get_meta_data(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<string> enumeration::get_keys() const
{
    if (is_valid())
        return m_container->get_keys();
    else
        return vector<string>();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<variant> enumeration::get_values() const
{
    if (is_valid())
        return m_container->get_values();
    else
        return vector<variant>();
}

/////////////////////////////////////////////////////////////////////////////////////////

string enumeration::value_to_key(detail::argument value) const
{
    if (is_valid())
        return m_container->value_to_key(value);
    else
        return string();
}
        
/////////////////////////////////////////////////////////////////////////////////////////

variant enumeration::key_to_value(const std::string& key) const
{
    if (is_valid())
        return m_container->key_to_value(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::operator==(const enumeration& other) const
{
    return (m_container == other.m_container);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool enumeration::operator!=(const enumeration& other) const
{
    return (m_container != other.m_container);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
