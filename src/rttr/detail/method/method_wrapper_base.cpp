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

#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/type/type_database_p.h"
#include "rttr/argument.h"
#include "rttr/instance.h"

using namespace std;

static const char* is_ref_list[] = {"", " &"};
static const char* is_const_list[] = {"", " const"};

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

method_wrapper_base::method_wrapper_base()
:   m_name(nullptr),
    m_declaring_type(get_invalid_type())
{
}

/////////////////////////////////////////////////////////////////////////////////////////

method_wrapper_base::~method_wrapper_base()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

void method_wrapper_base::set_name(const char* name)
{
    m_name = name;
}

/////////////////////////////////////////////////////////////////////////////////////////

const char* method_wrapper_base::get_name() const 
{
    return m_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

void method_wrapper_base::set_declaring_type(type declaring_type)
{
    m_declaring_type = declaring_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

type method_wrapper_base::get_declaring_type() const
{
    return m_declaring_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

string method_wrapper_base::get_signature() const
{
    const auto param_list = get_parameter_infos();
    string result = std::string(get_name()) + "( ";
    auto ref_list = get_is_reference();
    auto const_list = get_is_const();
    for (const auto& param : param_list)
    {
        result += param.get_type().get_name() + string(is_const_list[const_list[param.get_index()]]) + string(is_ref_list[ref_list[param.get_index()]]);
        if (param.get_index() < param_list.size() - 1)
            result += ", ";
    }
    if (param_list.empty())
        result += ")";
    else
        result += " )";

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////


} // end namespace detail
} // end namespace rttr
