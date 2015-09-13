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

#include "unit_tests/constructor/test_constructor_reflection.h"

#include <rttr/registration>

using namespace rttr;
using namespace std;

#include <iostream>
#include <memory>
#include <functional>

#include <catch/catch.hpp>

enum E_MetaData
{
    SCRIPTABLE  = 0,
    TOOL_TIP    = 1,
    DESCRIPTION = 2
};



RTTR_REGISTRATION
{
    registration::class_<constructor_test>("constructor_test")
        (
            meta_data("CUSTOM_DATA", 42),
            meta_data(DESCRIPTION, "some text")
        )
        .constructor<>()
        .constructor<const constructor_test&>()
        .constructor<int, int>()
        .constructor<const std::string&>()
        (
            meta_data("SCRIPTABLE", true),
            meta_data(TOOL_TIP, "This is a ToolTip")
        )
        .constructor<int, int, int, int, int, int, const int* const>();

    registration::class_<factory_test>("factory_test")
        .constructor(&factory_test::create);
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor/destructor", "[constructor]") 
{
    type p_type = type::get<constructor_test>();
    REQUIRE(p_type.is_valid() == true);

    SECTION("Test default ctor")
    {
        constructor default_ctor = p_type.get_constructor();
    
        REQUIRE(bool(default_ctor) == true);
        CHECK(default_ctor.is_valid() == true);
        CHECK(default_ctor.get_declaring_type() == type::get<constructor_test>());

        variant instance = default_ctor.invoke();
        CHECK(instance.is_valid() == true);
        CHECK(instance.is_type<constructor_test*>() == true);
    
        CHECK(default_ctor.get_instanciated_type() == type::get<constructor_test*>());
        CHECK(default_ctor.get_parameter_types().empty() == true);
        auto const ctor_name = type::get<constructor_test>().get_name();
        CHECK(default_ctor.get_signature() == ctor_name + "( )");

        destructor dtor = p_type.get_destructor();
        REQUIRE(dtor.is_valid() == true);
        CHECK(bool(dtor) == true);
        CHECK(dtor == dtor);
        CHECK( !(dtor != dtor) );
        CHECK(dtor.get_destructed_type() == type::get<constructor_test*>());
    
        dtor.invoke(instance);
        CHECK(instance.is_valid() == false);
    }

    SECTION("Test copy ctor")
    {
        constructor copy_ctor = p_type.get_constructor({ p_type } );
        REQUIRE(copy_ctor.is_valid() == true);

        CHECK(copy_ctor.get_instanciated_type() == type::get<constructor_test*>());
        const auto param_list = copy_ctor.get_parameter_types();
        REQUIRE(param_list.size() == 1);
        CHECK(param_list[0] == p_type);

        constructor default_ctor = p_type.get_constructor();
        variant var = copy_ctor.invoke(constructor_test());
        CHECK(var.is_valid() == true);
    }

    vector<constructor> ctors = p_type.get_constructors();
    REQUIRE(ctors.size() == 5);
    for (const auto& ctor : ctors)
    {
        CHECK(ctor.is_valid() == true);
        CHECK(bool(ctor) == true);
        CHECK(ctor.get_instanciated_type() == type::get<constructor_test*>());
        // negative test
        CHECK(ctor.invoke_variadic({"This will not work"}).is_valid() == false);
    }
    
    variant var = ctors[4].invoke();

    CHECK(ctors[0] == ctors[0]);
    CHECK(ctors[0] != ctors[1]);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test function constructor", "[constructor]") 
{
    type t = type::get<factory_test>();
    constructor ctor_no_args = t.get_constructor();
    CHECK(bool(ctor_no_args) == true);
    CHECK(ctor_no_args.get_declaring_type() == t);
    CHECK(ctor_no_args.get_parameter_types() == std::vector<type>()); // no arguments

    variant var = ctor_no_args.invoke(23);
    CHECK(var.is_valid() == false); // we cannot create the type when providing an argument

    var = ctor_no_args.invoke();
    CHECK(var.is_valid() == true);
    CHECK(var.get_type() == t);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor argument forwarding", "[constructor]") 
{
    type p_type = type::get<constructor_test>();
    REQUIRE(p_type.is_valid() == true);
    
    constructor ctor_string = p_type.get_constructor({type::get<std::string>()});
    variant instance = ctor_string.invoke();
    REQUIRE(instance.is_valid() == false); // not correctly invoked, one argument needed
    
    string test_string("rttr is working");
    instance = ctor_string.invoke(test_string);
    CHECK(instance.is_type<constructor_test*>() == true);
    constructor_test* obj = instance.get_value<constructor_test*>();
    CHECK(obj->_text == test_string);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test create/destroy via type", "[constructor]") 
{
    type t = type::get<constructor_test>();
    variant instance = t.create({2, 4});
    
    REQUIRE(instance.is_valid() == true);
    CHECK(instance.is_type<constructor_test*>() == true);
    
    t.destroy(instance);
    CHECK(instance.is_valid() == false);
    // negative test
    CHECK(t.create({2, true}).is_valid() == false);
    
    ////////////////////////////////////////////////
    instance = t.create({string("hello")});
    REQUIRE(instance.is_type<constructor_test*>() == true);
    t.destroy(instance);
    REQUIRE(instance.is_valid() == false);

    ////////////////////////////////////////////////
    instance = t.create({});
    REQUIRE(instance.is_type<constructor_test*>() == true);
    t.destroy(instance);
    CHECK(instance.is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor signature", "[constructor]") 
{
    const auto ctors = type::get<constructor_test>().get_constructors();
    REQUIRE(ctors.size() == 5);
    auto const ctor_name = type::get<constructor_test>().get_name();

    CHECK(ctors[0].get_signature() == ctor_name + "( )");
    CHECK(ctors[1].get_signature() == ctor_name + "( constructor_test const & )");
    CHECK(ctors[2].get_signature() == ctor_name + "( int, int )");
    CHECK(ctors[3].get_signature() == ctor_name + "( " + type::get<const std::string&>().get_name() + " const & )");
    CHECK(ctors[4].get_signature() == ctor_name + "( int, int, int, int, int, int, const int* const )");
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test meta data in constructor", "[constructor]") 
{
    constructor ctor_string = type::get<constructor_test>().get_constructor({type::get<std::string>()});
    variant value = ctor_string.get_meta_data("SCRIPTABLE");
    REQUIRE(value.is_type<bool>() == true);
    CHECK(value.get_value<bool>() == true);
    // integer meta data
    value = ctor_string.get_meta_data(TOOL_TIP);
    REQUIRE(value.is_type<std::string>() == true);
    CHECK(value.get_value<std::string>() == "This is a ToolTip");

    // no meta data
    constructor ctor_int = type::get<constructor_test>().get_constructor({type::get<int>(), type::get<int>()});
    REQUIRE(ctor_int.is_valid() == true);
    CHECK(ctor_int.get_meta_data("SCRIPTABLE").is_valid() == false);

    value = type::get<constructor_test>().get_meta_data("CUSTOM_DATA");
    REQUIRE(value.is_type<int>() == true);
    CHECK(value.get_value<int>() == 42);

    value = type::get<constructor_test>().get_meta_data(DESCRIPTION);

    REQUIRE(value.is_type<std::string>() == true);
    CHECK(std::string(value.get_value<std::string>()) == std::string("some text"));
}

/////////////////////////////////////////////////////////////////////////////////////////
