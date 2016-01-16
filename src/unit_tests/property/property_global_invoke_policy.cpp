

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

static const double pi = 3.124256;
static std::string g_text;

static std::string& get_global_text() { return g_text; }
static std::vector<int> global_array(1000, 42);

static std::string g_name;

static const std::string& get_global_name()
{
    return g_name;
}

static void set_global_name(const std::string& text)
{
    g_name = text;
}


/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::property("p1_as_ptr", &global_array)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p2_as_ptr", &get_global_text)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p3_as_ptr", std::function<const string&(void)>(&get_global_name))
        (
            policy::prop::bind_as_ptr
        )
        .property("p4_as_ptr", std::function<const string&(void)>(&get_global_name), std::function<void(const std::string&)>(&set_global_name))
        (
            policy::prop::bind_as_ptr
        );
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - bind_as_ptr", "[property]")
{
    property p1 = type::get_global_property("p1_as_ptr");
    REQUIRE(p1.get_type() == type::get<std::vector<int>*>());
    REQUIRE(p1.is_readonly() == false);
    REQUIRE(p1.is_array() == true);
    
    variant ret = p1.get_value(instance());
    REQUIRE(ret.is_valid() == true);
    REQUIRE(ret.is_type<std::vector<int>*>() == true);
    bool could_set_value = p1.set_value(instance(), ret);
    REQUIRE(could_set_value == true);
    
    std::vector<int>* vec = ret.get_value<std::vector<int>*>();
    REQUIRE(vec == &global_array);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - read only function - bind_as_ptr", "[property]")
{
    property p2 = type::get_global_property("p2_as_ptr");
    REQUIRE(p2.get_type() == type::get<const std::string*>());
    REQUIRE(p2.is_readonly() == true);

    variant ret = p2.get_value(instance());
    REQUIRE(ret.is_valid() == true);
    REQUIRE(ret.is_type<const std::string*>() == true);
    p2.set_value(instance(), ret);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - read only std::function - bind_as_ptr", "[property]")
{
    property p3 = type::get_global_property("p3_as_ptr");
    REQUIRE(p3.get_type() == type::get<const std::string*>());
    REQUIRE(p3.is_readonly() == true);
    
    variant ret = p3.get_value(instance());
    REQUIRE(ret.is_valid() == true);
    REQUIRE(ret.is_type<const std::string*>() == true);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - global - setter/getter function - bind_as_ptr", "[property]")
{
    property p4 = type::get_global_property("p4_as_ptr");
    REQUIRE(p4.is_array() == false);
    REQUIRE(p4.get_type() == type::get<const std::string*>());
    REQUIRE(p4.is_readonly() == false);
    
    variant ret = p4.get_value(instance());
    REQUIRE(ret.is_valid() == true);
    REQUIRE(ret.is_type<const std::string*>() == true);
    
    bool could_set_value = p4.set_value(instance(), ret);
    REQUIRE(could_set_value == true);
}

/////////////////////////////////////////////////////////////////////////////////////////
