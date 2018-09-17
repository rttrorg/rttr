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
#include <catch/catch.hpp>
#include <string>

using namespace rttr;

struct property_order_test_base
{
    property_order_test_base()
        : _whoami( "I am a base property" )
    {}
    virtual ~property_order_test_base() = default ;

    std::string _whoami ;

    RTTR_ENABLE()

};

struct property_order_test_derived : property_order_test_base
{
    property_order_test_derived()
        : _whoami( "I am a derived property with same name" )
    {}
    ~property_order_test_derived() override = default ;

    // member with SAME name as base
    std::string _whoami ;

    RTTR_ENABLE(property_order_test_base)
};

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::class_<property_order_test_base>("property_order_test_base")
        .property("whoami", &property_order_test_base::_whoami)
        ;
    registration::class_<property_order_test_derived>("property_order_test_derived")
        .property("whoami", &property_order_test_derived::_whoami)
        ;
}

// approach 1 looks up the "property instance"
// and gets the value from the property
std::string rttr_prop_approach1(
        const rttr::instance& vinst,
        const rttr::string_view& prop_name )
{
    const auto& inst_t = vinst.get_type();

    // find property
    const auto& prop = inst_t.get_property(prop_name);
    REQUIRE(prop.is_valid() == true);

    // get value from property
    const auto& iam_var = prop.get_value(vinst);
    REQUIRE(iam_var.is_valid() == true);
    REQUIRE(iam_var.is_type<std::string>() == true) ;

    return iam_var.get_value<std::string>();
}

// approach 2 gets the property value from rttr::type
std::string rttr_prop_approach2(
        const rttr::instance& vinst,
        const rttr::string_view& prop_name )
{
    const auto& inst_t = vinst.get_type();

    // get_value via type
    const auto& iam_var = inst_t.get_property_value(prop_name, vinst);
    REQUIRE(iam_var.is_valid() == true);
    REQUIRE(iam_var.is_type<std::string>() == true) ;

    return iam_var.get_value<std::string>();
}

template<class TT>
void check_prop_order1()
{
    TT inst;
    REQUIRE( inst._whoami == rttr_prop_approach1(inst, "whoami") ) ;
}

template<class TT>
void check_prop_order2()
{
    TT inst;
    REQUIRE( inst._whoami == rttr_prop_approach2(inst, "whoami") ) ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - approach1_order_properties_base()", "[property]")
{
    check_prop_order1<property_order_test_base>();
}

TEST_CASE("property - approach1_order_properties_derived()", "[property]")
{
    check_prop_order1<property_order_test_derived>();
}

TEST_CASE("property - approach2_order_properties_base()", "[property]")
{
    check_prop_order2<property_order_test_base>();
}

TEST_CASE("property - approach2_order_properties_derived()", "[property]")
{
    check_prop_order2<property_order_test_derived>();
}
