/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#include "visitor_test_class.h"

#include "my_visitor.h" // IMPORTANT!! All visitors must be included before the include of <registration>
#include <rttr/registration>


static int some_global_property = 12;

static bool get_prop_as_function() { return true; }

static std::string g_text;

static void set_global_value(std::string text) { g_text = text; }
static std::string get_global_value() { return g_text; }

using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::class_<visitor_test_class_base>("visitor_test_class_base")
        .constructor()
        .method("base_method", &visitor_test_class_base::base_method)
        .property("base_property", &visitor_test_class_base::base_property)
        ;

    registration::class_<visitor_test_class>("visitor_test_class")
        .constructor()
        .constructor<int>()
        .constructor(&visitor_test_class::create_object)
        .method("some_method", &visitor_test_class::some_method)
        .property("derived_property", &visitor_test_class::derived_property)
        .property_readonly("readonly_property", &visitor_test_class::readonly_property)
        .property("getter_setter_prop", &visitor_test_class::get_value, &visitor_test_class::set_value)
        ;

    registration::method("some_global_method", &some_global_method);

    registration::property("some_global_property", &some_global_property)
                 .property("global_setter_getter", &get_global_value, &set_global_value)
                 .property_readonly("get_prop_as_function", &get_prop_as_function);
}

/////////////////////////////////////////////////////////////////////////////////////////
