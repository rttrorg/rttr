/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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
#include "rttr/detail/method_container_base.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

method::method(const detail::method_container_base* container)
:   _container(container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::is_valid() const
{
    return (_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

method::operator bool() const
{
    return (_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

string method::get_name() const
{
    if (is_valid())
        return _container->get_name();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::is_static() const
{
    if (is_valid())
        return _container->is_static();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

type method::get_return_type() const
{
    if (is_valid())
        return _container->get_return_type();
    else
        return impl::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type method::get_declaring_type() const
{
    if (is_valid())
        return _container->get_declaring_type();
    else
        return impl::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<type> method::get_parameter_types() const
{
    if (is_valid())
        return _container->get_parameter_types();
    else
        return vector<type>();
}

/////////////////////////////////////////////////////////////////////////////////////////

string method::get_signature() const
{
    if (is_valid())
        return _container->get_signature();
    else
        return string();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::get_metadata(int key) const
{
    if (is_valid())
        return _container->get_metadata(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::get_metadata(const std::string& key) const
{
    if (is_valid())
        return _container->get_metadata(key);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object) const
{
    if (is_valid())
        return _container->invoke(object);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1) const
{
    if (is_valid())
        return _container->invoke(object, arg1);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1, detail::argument arg2) const
{
    if (is_valid())
        return _container->invoke(object, arg1, arg2);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1, detail::argument arg2, detail::argument arg3) const
{
    if (is_valid())
        return _container->invoke(object, arg1, arg2, arg3);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4) const
{
    if (is_valid())
        return _container->invoke(object, arg1, arg2, arg3, arg4);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4, 
                       detail::argument arg5) const
{
    if (is_valid())
        return _container->invoke(object, arg1, arg2, arg3, arg4, arg5);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke(detail::instance object, detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4, 
                       detail::argument arg5, detail::argument arg6) const
{
    if (is_valid())
        return _container->invoke(object, arg1, arg2, arg3, arg4, arg5, arg6);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant method::invoke_variadic(detail::instance object, std::vector<detail::argument> args) const
{
    if (is_valid())
        return _container->invoke_variadic(object, args);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::operator==(const method& other) const
{
    return (_container == other._container);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool method::operator!=(const method& other) const
{
    return (_container != other._container);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
