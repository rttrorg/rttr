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

#include "test_enumeration_reflection.h"

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

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTER
{
    class_<enum_test>()
        .constructor<>()
        .constructor<enum_test::E_Alignment, enum_test::E_Orientation>()
        .enumeration<enum_test::E_Alignment>({ 
                                               {"AlignLeft",    enum_test::E_Alignment::AlignLeft},
                                               {"AlignRight",   enum_test::E_Alignment::AlignRight},
                                               {"AlignHCenter", enum_test::E_Alignment::AlignHCenter},
                                               {"AlignJustify", enum_test::E_Alignment::AlignJustify}
                                             },{metadata(E_MetaData::SCRIPTABLE, true)})
        .enumeration<enum_test::E_Orientation>({ 
                                                 {"Horizontal", enum_test::E_Orientation::Horizontal},
                                                 {"Vertical",   enum_test::E_Orientation::Vertical}
                                               }, {metadata(E_MetaData::SCRIPTABLE, false)})
        .property("alignment", &enum_test::_alignment)
        .property("orientation", &enum_test::_orientation);

    enumeration_<E_DayOfWeek>({
                                {"Monday",    Monday},
                                {"Tuesday",   Tuesday},
                                {"Wednesday", Wednesday},
                                {"Thursday",  Thursday},
                                {"Friday",    Friday},
                                {"Saturday",  Saturday},
                                {"Sunday",    Sunday}
                              }, {metadata("Global_Tag", true)});
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test Enumeration", "[enumeration]") 
{
    type enum_type = type::get("enum_test");
    REQUIRE(enum_type.is_valid() == true);

    auto ctor_list = enum_type.get_constructors();
    REQUIRE(ctor_list.size() == 2);
    REQUIRE(ctor_list[1].get_parameter_types().size() == 2);

    auto params = ctor_list[1].get_parameter_types();
    REQUIRE(params.size() == 2);
    REQUIRE(params[0].is_enumeration() == true);
    REQUIRE(params[1].is_enumeration() == true);

    enumeration enum_info_align = params[0].get_enumeration();
    REQUIRE(enum_info_align.is_valid() == true);
    REQUIRE(enum_info_align == true);
    REQUIRE(enum_info_align.get_declaring_type() == type::get<enum_test>());
    REQUIRE(enum_info_align.get_type() == params[0]);

    enumeration enum_info_orient = params[1].get_enumeration();
    REQUIRE(enum_info_orient.is_valid() == true);

    // create instance from enum value
    variant enum_inst = enum_type.create({enum_info_align.key_to_value("AlignHCenter"), enum_info_orient.key_to_value("Vertical")});
    REQUIRE(enum_inst.is_valid() == true);

    property prop = enum_type.get_property("orientation");
    REQUIRE(prop.is_enumeration() == true );
    REQUIRE(prop.get_enumeration() == enum_info_orient );
    REQUIRE(! (prop.get_enumeration() != enum_info_orient) );

    variant orient_value = enum_type.get_property("orientation").get_value(enum_inst);
    variant align_value = enum_type.get_property("alignment").get_value(enum_inst);
    REQUIRE(orient_value.is_valid() == true);
    REQUIRE(align_value.is_valid() == true);

    REQUIRE(align_value.get_type() == enum_info_align.get_type());
    REQUIRE(orient_value.get_type() == enum_info_orient.get_type());
    
    // the underlying type is compiler specific implemented(can be int or unsigned int)
    // so simple check if one is returned
    REQUIRE(enum_info_align.get_underlying_type().is_valid() == true);

    REQUIRE(align_value.get_value<enum_test::E_Alignment>() == enum_test::AlignHCenter);
    REQUIRE(orient_value.get_value<enum_test::E_Orientation>() == enum_test::E_Orientation::Vertical);

    variant bad_value = enum_info_align.key_to_value("WrongEnum");
    std::string bad_key = enum_info_align.value_to_key(5000);
    REQUIRE(bad_value.is_valid() == false);
    REQUIRE(bad_key == string());
    
    std::vector<std::string> enum_keys = enum_info_align.get_keys();
    REQUIRE(enum_keys.size() == 4);

    std::vector<variant> enum_values = enum_info_align.get_values();
    REQUIRE(enum_values.size() == 4);

    const auto underlying_value = static_cast<std::underlying_type<enum_test::E_Alignment>::type>(2);
    std::string ret = enum_info_align.value_to_key(underlying_value);
    REQUIRE(ret == "AlignRight");

    const auto bad_underlying_value = static_cast<std::underlying_type<enum_test::E_Alignment>::type>(5);
    ret = enum_info_align.value_to_key(bad_underlying_value); // test bad value
    REQUIRE(ret == "");

    enum_type.destroy(enum_inst);
    REQUIRE(enum_inst.is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test global enum", "[enumeration]") 
{
    type enum_type = type::get("E_DayOfWeek");
    REQUIRE(enum_type.is_valid() == true);
    REQUIRE(enum_type.is_enumeration() == true);
    
    enumeration e_day = enum_type.get_enumeration();
    REQUIRE(e_day.get_declaring_type().is_valid() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test enumeration metadata", "[enumeration]") 
{
    enumeration enum_align = type::get("enum_test::E_Alignment").get_enumeration();

    variant value = enum_align.get_metadata(E_MetaData::SCRIPTABLE);
    REQUIRE(value.is_type<bool>() == true);
    REQUIRE(value.get_value<bool>() == true);
    
    // not scriptable
    enumeration enum_orient = type::get("enum_test::E_Orientation").get_enumeration();
    value = enum_orient.get_metadata(E_MetaData::SCRIPTABLE);
    REQUIRE(value.is_valid() == true);
    REQUIRE(value.get_value<bool>() == false);

    // integer metadata
    enumeration enum_day = type::get("E_DayOfWeek").get_enumeration();
    value = enum_day.get_metadata("Global_Tag");
    REQUIRE(value.is_type<bool>() == true);
    REQUIRE(value.get_value<bool>() == true);
}
