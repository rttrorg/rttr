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

#include "rttr/detail/constructor/constructor_wrapper_base.h"

using namespace std;

static const char* is_ref_list[] = {"", " &"};
static const char* is_const_list[] = {"", " const"};

namespace rttr
{
namespace detail
{
/////////////////////////////////////////////////////////////////////////////////////////

constructor_wrapper_base::constructor_wrapper_base()
{

}

/////////////////////////////////////////////////////////////////////////////////////////

constructor_wrapper_base::~constructor_wrapper_base()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<bool> constructor_wrapper_base::get_is_reference() const
{
    return{};
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<bool> constructor_wrapper_base::get_is_const() const
{
    return{};
}

/////////////////////////////////////////////////////////////////////////////////////////

string constructor_wrapper_base::get_signature() const
{
    auto param_info_list = get_parameter_infos();
    string result = get_instanciated_type().get_raw_type().get_name() + "( ";
    auto ref_list = get_is_reference();
    auto const_list = get_is_const();
    for (const auto& param : param_info_list)
    {
        result += param.get_type().get_name() + string(is_const_list[const_list[param.get_index()]]) + string(is_ref_list[ref_list[param.get_index()]]);
        if (param.get_index() < param_info_list.size() - 1)
            result += ", ";
    }
    if (param_info_list.empty())
        result += ")";
    else
        result += " )";

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke() const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1, argument& arg2) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1, argument& arg2, argument& arg3) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4,
                                           argument& arg5) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant constructor_wrapper_base::invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4,
                                           argument& arg5, argument& arg6) const
{
    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
