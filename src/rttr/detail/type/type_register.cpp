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

#include "rttr/detail/type/type_register.h"

#include "rttr/detail/type/type_database_p.h"
#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/destructor/destructor_wrapper_base.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/metadata/metadata.h"

using namespace std;

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::property(const type& t, unique_ptr<property_wrapper_base> prop)
{
   type_database::instance().register_property(t, move(prop));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::method(const type& t, std::unique_ptr<detail::method_wrapper_base> method)
{
    type_database::instance().register_method(t, move(method));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::constructor(const type& t, std::unique_ptr<detail::constructor_wrapper_base> ctor)
{
    type::register_constructor(t, move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::destructor(const type& t, std::unique_ptr<detail::destructor_wrapper_base> dtor)
{
    type::register_destructor(t, move(dtor));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::enumeration(const type& t, std::unique_ptr<detail::enumeration_wrapper_base> enum_item)
{
    type_database::instance().register_enumeration(t, move(enum_item));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::custom_name(type& t, string_view custom_name)
{
    type_database::instance().register_custom_name(t, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::metadata(const type& t, std::vector< ::rttr::detail::metadata > data)
{
    type_database::instance().register_metadata(t, move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::converter(const type& t, std::unique_ptr<detail::type_converter_base> converter)
{
     type_database::instance().register_converter(t, move(converter));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::comparator(const type& t, type_comparator_base* comparator)
{
    type_database::instance().register_comparator(t, comparator);
}

/////////////////////////////////////////////////////////////////////////////////////////

type type_register::type_reg(type_data& info) RTTR_NOEXCEPT
{
    return type_database::instance().register_type(info);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end
