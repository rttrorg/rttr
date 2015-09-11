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

#include "rttr/detail/property/property_wrapper_base.h"
#include "rttr/detail/type/type_database_p.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

property_wrapper_base::property_wrapper_base()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

property_wrapper_base::~property_wrapper_base()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

void property_wrapper_base::set_name(const char* name) const
{
    type_database::instance().set_item_name(get_meta_index(), name);
}

/////////////////////////////////////////////////////////////////////////////////////////

const char* property_wrapper_base::get_name() const
{
    return type_database::instance().get_item_name(get_meta_index());
}

/////////////////////////////////////////////////////////////////////////////////////////

void property_wrapper_base::set_declaring_type(type declaring_type) const
{
    type_database::instance().set_declaring_item_type(get_meta_index(), declaring_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

type property_wrapper_base::get_declaring_type() const
{
    return type_database::instance().get_declaring_item_type(get_meta_index());
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
