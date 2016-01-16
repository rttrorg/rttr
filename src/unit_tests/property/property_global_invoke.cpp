

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

#include <rttr/registration>

#include <iostream>
#include <memory>
#include <functional>

#include <catch/catch.hpp>

using namespace rttr;
using namespace std;

static std::string g_name;

static const std::string& get_global_name()
{
    return g_name;
}

static void set_global_name(const std::string& text)
{
    g_name = text;
}

static const double pi = 3.124256;
static int g_value = 23;


/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::property("g_value", &g_value)
        .property("global_name", &get_global_name, &set_global_name)
        .property_readonly("PI", &pi)
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - get/set - variable", "[property]")
{
    property prop = type::get_global_property("g_value");
    CHECK(prop.is_valid() == true);

    variant var = prop.get_value(instance());

    CHECK(var.is_type<int>() == true);
    CHECK(var.get_value<int>() == 23);

    int new_value = 42;
    bool result = prop.set_value(instance(), new_value);
    CHECK(result == true);
    CHECK(g_value == new_value);

    var = prop.get_value(instance());
    CHECK(var.is_type<int>() == true);
    CHECK(var.get_value<int>() == new_value);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - get/set - functions", "[property]")
{
    property prop = type::get_global_property("global_name");
    CHECK(prop.is_valid() == true);

    variant var = prop.get_value(instance());

    CHECK(var.is_type<std::string>() == true);
    CHECK(var.get_value<std::string>() == std::string());

    std::string new_value("test");
    bool result = prop.set_value(instance(), new_value);
    CHECK(result == true);
    CHECK(get_global_name() == new_value);

    var = prop.get_value(instance());
    CHECK(var.is_type<std::string>() == true);
    CHECK(var.get_value<std::string>() == new_value);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - get/set - readonly", "[property]")
{
    property prop = type::get_global_property("PI");
    CHECK(prop.is_valid() == true);

    variant var = prop.get_value(instance());

    CHECK(var.is_type<double>() == true);
    CHECK(var.get_value<double>() == 3.124256);

    bool result = prop.set_value(instance(), 2.0);
    CHECK(result == false);
}

/////////////////////////////////////////////////////////////////////////////////////////
