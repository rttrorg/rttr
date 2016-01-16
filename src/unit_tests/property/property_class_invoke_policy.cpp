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

struct property_member_policy
{
    property_member_policy()
    {
        _array.resize(1000);
        for (int i = 0; i < 100; ++i)
            _other_array[i] = i;
    }
    
    virtual ~property_member_policy() {}

    const std::string& get_text() const { return m_text; }
    void set_text(const std::string& text) { m_text = text; }

    std::vector<int>    _array;
    int                 _other_array[100];

    std::string m_text;
};

using namespace rttr;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    using namespace rttr;

     registration::class_<property_member_policy>("property_member_policy")
        .property("array", &property_member_policy::_array)
        (
            policy::prop::bind_as_ptr
        )
        .property("raw_array", &property_member_policy::_other_array)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p1_as_ptr", &property_member_policy::get_text)
        (
            policy::prop::bind_as_ptr
        )
        .property("p2_as_ptr", &property_member_policy::get_text, &property_member_policy::set_text)
        (
            policy::prop::bind_as_ptr
        )
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - bind_as_ptr - array", "[property]")
{
    property_member_policy obj;
    type prop_type = type::get(obj);
    property array_prop = prop_type.get_property("array");

    REQUIRE(array_prop.is_array() == true);
    variant array_obj = array_prop.get_value(obj); // the array is returned by pointer
    variant_array_view vec_array = array_obj.create_array_view();
    REQUIRE(vec_array.is_valid() == true);
    REQUIRE(array_obj.is_type<std::vector<int>*>() == true);
    REQUIRE(array_obj.get_value<std::vector<int>*>() == &obj._array);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - bind_as_ptr - raw_array", "[property]")
{
    property_member_policy obj;
    type prop_type = type::get(obj);
    
    property array_prop = prop_type.get_property("raw_array");
    REQUIRE(array_prop.is_array() == true);
    variant array_obj = array_prop.get_value(obj); // the array is returned by pointer
    variant_array_view other_array = array_obj.create_array_view();
    REQUIRE(other_array.is_valid() == true);
    
    variant ret = other_array.get_value(23);
    REQUIRE(ret.is_type<int>() == true);
    REQUIRE(ret.get_value<int>() == 23);
    
    for (std::size_t i = 0; i < other_array.get_size(); ++i)
        other_array.set_value(i, static_cast<int>(i * 2));
    
    // did we really set the value?
    for (std::size_t i = 0; i < other_array.get_size(); ++i)
        REQUIRE(obj._other_array[i] == i * 2);
    
    bool wasSet = array_prop.set_value(obj, array_obj);
    REQUIRE(wasSet == true);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - bind_as_ptr - readonly function", "[property]")
{
    property_member_policy obj;
    type prop_type = type::get(obj);
    property p7_as_ptr = prop_type.get_property("p1_as_ptr");
    REQUIRE(bool(p7_as_ptr) == true);
    REQUIRE(p7_as_ptr.is_readonly() == true);
    REQUIRE(p7_as_ptr.get_type() == type::get<const std::string*>());
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - get/set - bind_as_ptr - function", "[property]")
{
    property_member_policy obj;
    type prop_type = type::get(obj);
    property p7_as_ptr = prop_type.get_property("p2_as_ptr");
    REQUIRE(bool(p7_as_ptr) == true);
    REQUIRE(p7_as_ptr.is_readonly() == false);
    REQUIRE(p7_as_ptr.get_type() == type::get<const std::string*>());
}

/////////////////////////////////////////////////////////////////////////////////////////
