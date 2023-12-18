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

#include <rttr/registration>
#include <rttr/registration_friend>
#include <iostream>
#include <memory>
#include <functional>

#include <catch/catch.hpp>

using namespace rttr;
using namespace std;

static int g_invalid_instance = 0;

using func_ptr = void(*)(int);
struct property_member_func_test
{
    property_member_func_test() : m_int_value(12)
    {
    }

    const std::string& get_text() const { return m_text; }
    void set_text(const std::string& text) { m_text = text; }

    int get_int_value() { return m_int_value; }

    int& get_int_ref() { return m_int_value; }

    void set_function_cb(func_ptr cb) { m_funcPtr = cb; }
    func_ptr get_function_cb() const { return m_funcPtr; }

    std::string     m_text;
    int             m_int_value;
    func_ptr        m_funcPtr;

    RTTR_REGISTRATION_FRIEND;
};

static void my_callback(int)
{
}

int int_getter_global(property_member_func_test* self) { return self->get_int_value(); }
void int_setter_global(property_member_func_test* self, int value) { self->get_int_ref() = value; }
 
/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    int i = 3;
    auto int_getter = [i](property_member_func_test* self) { return self->get_int_value(); };
    auto int_setter = [i](property_member_func_test* self, int value) { self->get_int_ref() = value; };
    registration::class_<property_member_func_test>("property_member_func_test")
        .property("p1", &property_member_func_test::get_text, &property_member_func_test::set_text)
        (
            metadata("Description", "Some Text")
        )
        .property_readonly("p2", &property_member_func_test::get_int_value)
        (
            metadata("Description", "Some Text")
        )
        .property_readonly("p2_lambda", int_getter)
        .property_readonly("p2_global", int_getter_global)
        .property_readonly("p2_function", std::function<int(property_member_func_test*)>(int_getter))
        .property("p3", &property_member_func_test::get_text, &property_member_func_test::set_text)
        (
            metadata("Description", "Some Text"),
            policy::prop::bind_as_ptr
        )
        .property_readonly("p4", &property_member_func_test::get_int_ref)
        (
            metadata("Description", "Some Text"),
            policy::prop::bind_as_ptr
        )
        .property("callback", &property_member_func_test::get_function_cb, &property_member_func_test::set_function_cb)
        .property("p5", &property_member_func_test::get_text, &property_member_func_test::set_text)
        (
            metadata("Description", "Some Text"),
            policy::prop::as_reference_wrapper
        )
        .property_readonly("p6", &property_member_func_test::get_int_ref)
        (
            metadata("Description", "Some Text"),
            policy::prop::as_reference_wrapper
        )
        .property("p7_lambda", int_getter, int_setter)
        .property("p7_global", int_getter_global, int_setter_global)
        .property("p7_function", std::function<int(property_member_func_test*)>(int_getter), std::function<void(property_member_func_test*, int)>(int_setter))
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);
    REQUIRE(prop_type.is_valid() == true);

    property prop = prop_type.get_property("p1");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == false);
    CHECK(prop.is_static() == false);
    CHECK(prop.get_type() == type::get<std::string>());
    CHECK(prop.get_declaring_type() == type::get<property_member_func_test>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // valid invoke
    CHECK(prop.set_value(obj, std::string("New Text")) == true);
    CHECK(prop.get_value(obj).is_type<std::string>() == true);
    CHECK(prop.get_value(obj).get_value<std::string>() == "New Text");

    // invalid invoke
    CHECK(prop.set_value(obj, 42) == false);
    CHECK(prop.set_value(instance(), 42) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - read only", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p2");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == false);
    CHECK(prop.get_type() == type::get<int>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    CHECK(prop.get_value(obj).is_type<int>() == true);
    CHECK(prop.get_value(obj).get_value<int>() == 12);

    // invalid invoke
    CHECK(prop.set_value(obj, 23) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - functors - read only", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);
    
    for (const char* name : { "p2_lambda", "p2_global", "p2_function" })
    {
        property prop = prop_type.get_property(name);
        REQUIRE(prop.is_valid() == true);

        // metadata
        CHECK(prop.is_readonly() == true);
        CHECK(prop.is_static() == false);
        CHECK(prop.get_type() == type::get<int>());
        CHECK(prop.get_access_level() == rttr::access_levels::public_access);

        // invoke
        CHECK(prop.get_value(obj).is_type<int>() == true);
        CHECK(prop.get_value(obj).get_value<int>() == 12);

        // invalid invoke
        CHECK(prop.set_value(obj, 23) == false);
        CHECK(prop.get_value(g_invalid_instance).is_valid() == false);

    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - bind as ptr", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p3");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == false);
    CHECK(prop.is_static() == false);
    CHECK(prop.get_type() == type::get<const std::string*>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // valid invoke
    const std::string text("Hello World");
    CHECK(prop.set_value(obj, &text) == true);
    CHECK(prop.get_value(obj).is_type<const std::string*>() == true);
    CHECK(*prop.get_value(obj).get_value<const std::string*>() == "Hello World");

    // invalid invoke
    CHECK(prop.set_value(obj, 42) == false);
    CHECK(prop.set_value(instance(), 42) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - read only - bind as ptr", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p4");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == false);

    CHECK(prop.get_type() == type::get<const int*>());
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    REQUIRE(prop.get_value(obj).is_type<const int*>() == true);
    CHECK(*prop.get_value(obj).get_value<const int*>() == 12);

    // invalid invoke
    CHECK(prop.set_value(obj, 23) == false);
    CHECK(prop.set_value("wrong instance", 23) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}

TEST_CASE("property - functors", "[property]")
{
    for (const char* name : { "p7_lambda", "p7_global", "p7_function" })
    {
        property_member_func_test obj;
        type prop_type = type::get(obj);

        property prop = prop_type.get_property(name);
        REQUIRE(prop.is_valid() == true);

        // metadata
        CHECK(prop.is_readonly() == false);
        CHECK(prop.is_static() == false);

        CHECK(prop.get_type() == type::get<int>());
        CHECK(prop.get_access_level() == rttr::access_levels::public_access);

        // invoke get
        REQUIRE(prop.get_value(obj).is_type<int>() == true);
        CHECK(prop.get_value(obj).get_value<int>() == 12);

        // invoke set
        CHECK(prop.set_value("wrong instance", 23) == false);
        REQUIRE(prop.set_value(obj, 23) == true);
        REQUIRE(prop.get_value(obj).is_type<int>() == true);
        CHECK(prop.get_value(obj).get_value<int>() == 23);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - function pointer", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("callback");

    auto cb = &my_callback;
    bool ret = prop.set_value(obj, cb);
    CHECK(ret == true);

    variant var = prop.get_value(obj);
    REQUIRE(var.is_type<func_ptr>() == true);
    CHECK(var.get_value<func_ptr>() == cb);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - as_reference_wrapper", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p5");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == false);
    CHECK(prop.is_static() == false);
    CHECK(prop.get_type() == type::get<std::reference_wrapper<const std::string>>());
    CHECK(prop.get_type().is_wrapper() == true);
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // valid invoke
    const std::string text("Hello World");
    CHECK(prop.set_value(obj, std::cref(text)) == true);
    CHECK(prop.get_value(obj).is_type<std::reference_wrapper<const std::string>>() == true);
    CHECK(prop.get_value(obj).get_value<std::reference_wrapper<const std::string>>().get() == "Hello World");

    // invalid invoke
    CHECK(prop.set_value(obj, 42) == false);
    CHECK(prop.set_value(instance(), 42) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class function - read only - as_reference_wrapper", "[property]")
{
    property_member_func_test obj;
    type prop_type = type::get(obj);

    property prop = prop_type.get_property("p6");
    REQUIRE(prop.is_valid() == true);

    // metadata
    CHECK(prop.is_readonly() == true);
    CHECK(prop.is_static() == false);
    CHECK(prop.get_type() == type::get<std::reference_wrapper<const int>>());
    CHECK(prop.get_type().is_wrapper() == true);
    CHECK(prop.get_access_level() == rttr::access_levels::public_access);
    CHECK(prop.get_metadata("Description") == "Some Text");

    // invoke
    REQUIRE(prop.get_value(obj).is_type<std::reference_wrapper<const int>>() == true);
    CHECK(prop.get_value(obj).get_value<std::reference_wrapper<const int>>().get() == 12);

    // invalid invoke
    CHECK(prop.set_value(obj, 23) == false);
    CHECK(prop.set_value("wrong instance", 23) == false);
    CHECK(prop.get_value(g_invalid_instance).is_valid() == false);
}


/////////////////////////////////////////////////////////////////////////////////////////
