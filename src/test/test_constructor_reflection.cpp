/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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

#include "test_constructor_reflection.h"

#include <rttr/reflect>

using namespace rttr;
using namespace std;

#include <iostream>
#include <memory>
#include <functional>

#include "catch.hpp"

enum E_MetaData
{
    SCRIPTABLE  = 0,
    TOOL_TIP    = 1,
    DESCRIPTION = 2
};

RTTR_REGISTER
{
    class_<constructor_test>()
        .constructor<>()
        .constructor<int, int>()
        .constructor<const std::string&>({metadata("SCRIPTABLE", true), metadata(TOOL_TIP, string("This is a ToolTip"))})
        .constructor<int, int, int, int, int, int, const int* const>();
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor/destructor", "[constructor]") 
{
    type p_type = type::get("constructor_test");
    REQUIRE(p_type.is_valid() == true);

    constructor default_ctor = p_type.get_constructor();
    REQUIRE(default_ctor == true);
    REQUIRE(default_ctor.is_valid() == true);
    REQUIRE(default_ctor.get_declaring_type() == type::get<constructor_test>());

    variant instance = default_ctor.invoke();
    REQUIRE(instance.is_valid() == true);
    REQUIRE(instance.is_type<constructor_test*>() == true);
    
    REQUIRE(default_ctor.get_instanciated_type() == type::get<constructor_test*>());
    REQUIRE(default_ctor.get_parameter_types().empty() == true);
    REQUIRE(default_ctor.get_signature() == std::string("constructor_test( )"));

    destructor dtor = p_type.get_destructor();
    REQUIRE(dtor.is_valid() == true);
    REQUIRE(dtor == true);
    REQUIRE(dtor == dtor);
    REQUIRE( !(dtor != dtor) );
    REQUIRE(dtor.get_destructed_type() == type::get<constructor_test*>());
    
    dtor.invoke(instance);
    REQUIRE(instance.is_valid() == false);

    vector<constructor> ctors = p_type.get_constructors();
    REQUIRE(ctors.size() == 4);
    for (const auto& ctor : ctors)
    {
        REQUIRE(ctor.get_instanciated_type() == type::get<constructor_test*>());
        REQUIRE(ctor.is_valid() == true);
        REQUIRE(ctor == true);
        // negative test
        REQUIRE(ctor.invoke_variadic({"This will not work"}).is_valid() == false);
    }

    REQUIRE(ctors[0] == ctors[0]);
    REQUIRE(ctors[0] != ctors[1]);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor argument forwarding", "[constructor]") 
{
    type p_type = type::get("constructor_test");
    REQUIRE(p_type.is_valid() == true);
    
    constructor ctor_string = p_type.get_constructor({type::get<std::string>()});
    variant instance = ctor_string.invoke();
    REQUIRE(instance.is_valid() == false); // not correctly invoked, one argument needed
    
    string test_string("rttr is working");
    instance = ctor_string.invoke(test_string);
    REQUIRE(instance.is_type<constructor_test*>() == true);
    constructor_test* obj = instance.get_value<constructor_test*>();
    REQUIRE(obj->_text == test_string);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test create/destroy via type", "[constructor]") 
{
    type t = type::get("constructor_test");
    variant instance = t.create({2, 4});
    
    REQUIRE(instance.is_valid() == true);
    REQUIRE(instance.is_type<constructor_test*>() == true);
    
    t.destroy(instance);
    REQUIRE(instance.is_valid() == false);
    // negative test
    REQUIRE(t.create({2, true}).is_valid() == false);
    
    ////////////////////////////////////////////////
    instance = t.create({string("hello")});
    REQUIRE(instance.is_type<constructor_test*>() == true);
    t.destroy(instance);
    REQUIRE(instance.is_valid() == false);

    ////////////////////////////////////////////////
    instance = t.create({});
    REQUIRE(instance.is_type<constructor_test*>() == true);
    t.destroy(instance);
    REQUIRE(instance.is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test constructor signature", "[constructor]") 
{
    const auto ctors = type::get("constructor_test").get_constructors();
    REQUIRE(ctors.size() == 4);

    REQUIRE(ctors[0].get_signature() == "constructor_test( )");
    REQUIRE(ctors[1].get_signature() == "constructor_test( int, int )");
    REQUIRE(ctors[2].get_signature() == "constructor_test( std::string const & )");
    REQUIRE(ctors[3].get_signature() == "constructor_test( int, int, int, int, int, int, const int* const )");
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test metadata in constructor", "[constructor]") 
{
    constructor ctor_string = type::get("constructor_test").get_constructor({type::get<std::string>()});
    variant value = ctor_string.get_metadata("SCRIPTABLE");
    REQUIRE(value.is_type<bool>() == true);
    REQUIRE(value.get_value<bool>() == true);
    // integer metadata
    value = ctor_string.get_metadata(TOOL_TIP);
    REQUIRE(value.is_type<std::string>() == true);
    REQUIRE(value.get_value<std::string>() == "This is a ToolTip");

    // no metadata
    constructor ctor_int = type::get("constructor_test").get_constructor({type::get<int>(), type::get<int>()});
    REQUIRE(ctor_int.is_valid() == true);
    REQUIRE(ctor_int.get_metadata("SCRIPTABLE").is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////
