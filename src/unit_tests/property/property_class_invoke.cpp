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

using func_ptr = void(*)(int);
struct property_invoke
{
    property_invoke()
    {
    }

    const std::string& get_text() const { return m_text; }
    void set_text(const std::string& text) { m_text = text; }

    void set_function_cb(func_ptr cb) { m_funcPtr = cb; }
    func_ptr get_function_cb() const { return m_funcPtr; }

    int                 _p1;
    static int          _p2;
    static const int    _p3;
    const int           _p4 = 12;
    std::string         m_text;
    func_ptr            m_funcPtr;


    RTTR_REGISTRATION_FRIEND;
};


/////////////////////////////////////////////////////////////////////////////////////////
// init static variables and some global functions added as properties to the test class

int property_invoke::_p2= 23;
const int property_invoke::_p3 = 12;
static float singleton_property = 3.1425f;
static std::string g_name;

int get_value() { return 512; }

static const std::string& get_name()
{
    return g_name;
}

static void set_name(const std::string& text)
{
    g_name = text;
}

static void my_callback(int)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::class_<property_invoke>("property_invoke")
        .constructor<>()
        .property("p1",    &property_invoke::_p1)
        .property("p2",    &property_invoke::_p2)
        .property("p3",    &property_invoke::get_text, &property_invoke::set_text)
        .property_readonly("p4",     &property_invoke::_p4) ( metadata("Description", "Some Text") )
        .property_readonly("p5",     &property_invoke::_p3) ( metadata("Description", "Some Text") )
        .property("p6",    &singleton_property)
        .property_readonly("p7",     &get_value) ( metadata("Description", "Some Text") )
        .property_readonly("p8",    std::function<int()>([](){ return 45;}))
        .property("p9",   std::function<const string&(void)>(&get_name), std::function<void(const std::string&)>(&set_name))
        .property("callback", &property_invoke::get_function_cb, &property_invoke::set_function_cb)
        .property_readonly("p10", &property_invoke::get_text)
        (
            metadata("Description", "Some Text")
        )
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - variable", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);
    REQUIRE(prop_type.is_valid() == true);

    property prop = prop_type.get_property("p1");
    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == false);
    CHECK(prop.is_static() == false);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<int>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);

    // invoke
    CHECK(prop.set_value(obj, 42) == true);
    CHECK(prop.get_value(obj).is_type<int>() == true);
    CHECK(prop.get_value(obj).get_value<int>() == 42);

    // negative invoke
    CHECK(prop.set_value(obj, "test") == false);
    CHECK(prop.get_value(23).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - static variable", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p2");
    CHECK(prop.is_static() == true);

    bool ret = prop.set_value(obj, 2);
    CHECK(ret == true);

    variant var = prop.get_value(obj);
    CHECK(var.is_type<int>() == true);
    CHECK(var.get_value<int>() == 2);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - function", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p3");
    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == false);
    CHECK(prop.is_static() == false);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<std::string>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);

    // invoke
    CHECK(prop.set_value(obj, std::string("some text")) == true);
    CHECK(prop.get_value(obj).is_type<std::string>() == true);
    CHECK(prop.get_value(obj).get_value<std::string>() == "some text");

    // negative invoke
    CHECK(prop.get_value(23).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - global variable", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p6");
    CHECK(prop.is_static() == true);

    bool ret = prop.set_value(obj, 23.0f);
    CHECK(ret == true);

    variant var = prop.get_value(obj);
    CHECK(var.is_type<float>() == true);
    CHECK(var.get_value<float>() == 23.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - std::function", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p9");
    CHECK(prop.is_static() == true);

    std::string new_value("text");
    bool ret = prop.set_value(obj, new_value);
    CHECK(ret == true);

    variant var = prop.get_value(obj);
    CHECK(var.is_type<std::string>() == true);
    CHECK(var.get_value<std::string>() == new_value);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - function pointer", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("callback");

    auto cb = &my_callback;
    bool ret = prop.set_value(obj, cb);
    CHECK(ret == true);

    variant var = prop.get_value(obj);
    CHECK(var.is_type<func_ptr>() == true);
    CHECK(var.get_value<func_ptr>() == cb);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - read only - member object", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p4");
    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == false);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<int>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    CHECK(prop.set_value(obj, 23) == false);
    CHECK(prop.get_value(obj).is_type<int>() == true);
    CHECK(prop.get_value(obj).get_value<int>() == 12);

    // negative invoke
    CHECK(prop.get_value(23).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - read only - member function", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);
    obj.set_text("Text");

    property prop = prop_type.get_property("p10");
    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == false);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<std::string>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    CHECK(prop.set_value(obj, 23) == false);
    CHECK(prop.get_value(obj).is_type<std::string>() == true);
    CHECK(prop.get_value(obj).get_value<std::string>() == "Text");

    // negative invoke
    CHECK(prop.get_value(23).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - read only - static member", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p5");

    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == true);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<int>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    CHECK(prop.set_value(obj, 23) == false);
    REQUIRE(prop.get_value(instance()).is_type<int>() == true);
    CHECK(prop.get_value(instance()).get_value<int>() == 12);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - read only - global function", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p7");
    REQUIRE(bool(prop) == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == true);
    CHECK(prop.is_array() == false);
    CHECK(prop.get_type() == type::get<int>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    CHECK(prop.set_value(instance(), 23) == false);
    CHECK(prop.get_value(instance()).is_type<int>() == true);
    CHECK(prop.get_value(instance()).get_value<int>() == 512);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - read only - std::function", "[property]")
{
    property_invoke obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p8");
    CHECK(prop.is_static() == true);

    bool ret = prop.set_value(obj, 23);
    CHECK(ret == false);

    variant var = prop.get_value(obj);
    CHECK(var.is_type<int>() == true);
    CHECK(var.get_value<int>() == 45);
}

/////////////////////////////////////////////////////////////////////////////////////////
