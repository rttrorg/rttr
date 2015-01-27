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

#include "rttr/destructor.h"
#include "rttr/detail/destructor_container_base.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

destructor::destructor(const detail::destructor_container_base* container)
:   _container(container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool destructor::is_valid() const
{
    return (_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

destructor::operator bool() const
{
    return (_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

type destructor::get_destructed_type() const
{
    if (is_valid())
        return _container->get_destructed_type();
    else
        return impl::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

void destructor::invoke(variant& obj) const
{
    if (is_valid())
        _container->invoke(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool destructor::operator==(const destructor& other) const
{
    return (_container == other._container);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool destructor::operator!=(const destructor& other) const
{
    return (_container != other._container);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
