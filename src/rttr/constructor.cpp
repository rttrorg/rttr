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

#include "rttr/constructor.h"
#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/argument.h"

#include <utility>


using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

constructor::constructor(const detail::constructor_wrapper_base* container)
:   m_container(container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool constructor::is_valid() const
{
    return (m_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor::operator bool() const
{
    return (m_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

type constructor::get_instanciated_type() const
{
    if (is_valid())
        return m_container->get_instanciated_type();
    else
        return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type constructor::get_declaring_type() const
{
    if (is_valid())
        return m_container->get_declaring_type();
    else
        return detail::get_invalid_type();
}


/////////////////////////////////////////////////////////////////////////////////////////

string constructor::get_signature() const
{
    if (is_valid())
        return m_container->get_signature();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<parameter_info> constructor::get_parameter_infos() const
{
    if (is_valid())
        return m_container->get_parameter_infos();
    else
        return std::vector<parameter_info>();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::get_meta_data(const variant& key) const
{
    if (is_valid())
        return m_container->get_meta_data(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke() const
{
    if (is_valid())
        return m_container->invoke();
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1) const
{
    if (is_valid())
        return m_container->invoke(arg1);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1, argument arg2) const
{
    if (is_valid())
        return m_container->invoke(arg1, arg2);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1, argument arg2, argument arg3) const
{
    if (is_valid())
        return m_container->invoke(arg1, arg2, arg3);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4) const
{
    if (is_valid())
        return m_container->invoke(arg1, arg2, arg3, arg4);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4,
                            argument arg5) const
{
    if (is_valid())
        return m_container->invoke(arg1, arg2, arg3, arg4, arg5);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4,
                            argument arg5, argument arg6) const
{
    if (is_valid())
        return m_container->invoke(arg1, arg2, arg3, arg4, arg5, arg6);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor::invoke_variadic(std::vector<argument> args) const
{
    if (is_valid())
        return m_container->invoke_variadic(args);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool constructor::operator==(const constructor& other) const
{
    return (m_container == other.m_container);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool constructor::operator!=(const constructor& other) const
{
    return (m_container != other.m_container);
}

/////////////////////////////////////////////////////////////////////////////////////////


} // end namespace rttr
