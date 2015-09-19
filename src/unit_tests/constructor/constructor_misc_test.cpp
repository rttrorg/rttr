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

#include <rttr/registration>
#include <catch/catch.hpp>

using namespace rttr;

struct ctor_misc_test
{
    ctor_misc_test(){}

    ctor_misc_test(int value){}

    static ctor_misc_test create_object(int value) { return ctor_misc_test(value); }

    int m_value;
};

enum class E_MetaData
{
    SCRIPTABLE  = 0,
    TOOL_TIP    = 1,
    DESCRIPTION = 2
};

RTTR_REGISTRATION
{
   registration::class_<ctor_misc_test>("ctor_misc_test")
        (
            meta_data(E_MetaData::SCRIPTABLE, true),
            meta_data(E_MetaData::TOOL_TIP, "This is a type ToolTip.")
        )
        .constructor<>()
        (
            meta_data(E_MetaData::SCRIPTABLE, true),
            meta_data(E_MetaData::TOOL_TIP, "This is a ToolTip.")
        )
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        )
        .constructor<>()
        (
            policy::ctor::as_object
        )
        .constructor(&ctor_misc_test::create_object)
        (
            meta_data(E_MetaData::SCRIPTABLE, false),
            meta_data(E_MetaData::TOOL_TIP, "This is another ToolTip.")
        );
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - get_instanciated_type", "[constructor]") 
{
    auto ctor_list = type::get<ctor_misc_test>().get_constructors();
    CHECK(ctor_list[0].get_instanciated_type() == type::get<ctor_misc_test*>());
    CHECK(ctor_list[1].get_instanciated_type() == type::get<std::shared_ptr<ctor_misc_test>>());
    CHECK(ctor_list[2].get_instanciated_type() == type::get<ctor_misc_test>());
    CHECK(ctor_list[3].get_instanciated_type() == type::get<ctor_misc_test>());
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - get_signature", "[constructor]") 
{
    constructor ctor = type::get<ctor_misc_test>().get_constructor();
    CHECK(ctor.get_signature() == "ctor_misc_test( )");

    ctor = type::get<ctor_misc_test>().get_constructor({type::get<int>()});
    CHECK(ctor.get_signature() == "ctor_misc_test( int )");
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - get_parameter_types", "[constructor]") 
{
    constructor ctor = type::get<ctor_misc_test>().get_constructor();
    CHECK(ctor.get_parameter_types() == std::vector<type>());

    ctor = type::get<ctor_misc_test>().get_constructor({type::get<int>()});
    CHECK(ctor.get_parameter_types() == std::vector<type>(1, type::get<int>()));
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("ctor - get_declaring_type", "[constructor]") 
{
    constructor ctor = type::get<ctor_misc_test>().get_constructor();
    CHECK(ctor.get_declaring_type() == type::get<ctor_misc_test>());

    ctor = type::get<ctor_misc_test>().get_constructor({type::get<int>()});
    CHECK(ctor.get_declaring_type() == type::get<ctor_misc_test>());
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - get_meta_data", "[constructor]") 
{
    SECTION("default ctor")
    {
        constructor ctor = type::get<ctor_misc_test>().get_constructor();
        variant value = ctor.get_meta_data(E_MetaData::SCRIPTABLE);
        REQUIRE(value.is_type<bool>() == true);
        CHECK(value.get_value<bool>() == true);

        value = ctor.get_meta_data(E_MetaData::TOOL_TIP);
        REQUIRE(value.is_type<std::string>() == true);
        CHECK(value.get_value<std::string>() == "This is a ToolTip.");
    }

    SECTION("function as ctor")
    {
        constructor ctor = type::get<ctor_misc_test>().get_constructor({type::get<int>()});
        variant value = ctor.get_meta_data(E_MetaData::SCRIPTABLE);
        REQUIRE(value.is_type<bool>() == true);
        CHECK(value.get_value<bool>() == false);

        value = ctor.get_meta_data(E_MetaData::TOOL_TIP);
        REQUIRE(value.is_type<std::string>() == true);
        CHECK(value.get_value<std::string>() == "This is another ToolTip.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - get_meta_data - type", "[constructor]") 
{
    variant var = type::get<ctor_misc_test>().get_meta_data(E_MetaData::SCRIPTABLE);
    REQUIRE(var.is_type<bool>() == true);
    CHECK(var.get_value<bool>() == true);

    var = type::get<ctor_misc_test>().get_meta_data(E_MetaData::TOOL_TIP);
    REQUIRE(var.is_type<std::string>() == true);
    CHECK(var.get_value<std::string>() == "This is a type ToolTip.");
}


/////////////////////////////////////////////////////////////////////////////////////////
