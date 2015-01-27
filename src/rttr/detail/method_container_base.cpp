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

#include "rttr/detail/method_container_base.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

using namespace std;

static const char* is_ref_list[] = {"", " &"};
static const char* is_const_list[] = {"", " const"};

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

method_container_base::method_container_base(const std::string& name, const type decalaring_type)
:   _name(name),
    _decalaring_type(decalaring_type)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

method_container_base::~method_container_base()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

string method_container_base::get_name() const 
{
    return _name; 
}

/////////////////////////////////////////////////////////////////////////////////////////

type method_container_base::get_declaring_type() const
{
    return _decalaring_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

string method_container_base::get_signature() const
{
    auto params = get_parameter_types();
    string result = _name + "( ";
    std::size_t index = 0;
    auto ref_list = get_is_reference();
    auto const_list = get_is_const();
    for (const auto& type : params)
    {
        result += type.get_name() + string(is_const_list[const_list[index]]) + string(is_ref_list[ref_list[index]]);
        if (index < params.size() - 1)
            result += ", ";

        ++index;
    }
    if (params.empty())
        result += ")";
    else
        result += " )";

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
