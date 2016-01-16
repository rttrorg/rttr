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


/////////////////////////////////////////////////////////////////////////////////////////
// test derived properties

namespace ns_property
{
struct top
{
    virtual ~top() {}
    top() : _p1(12){}
    int _p1;
    RTTR_ENABLE()
};

/////////////////////////////////////////////////////////////////////////////////////////

struct left : virtual top
{

    left() : _p2(true){}
    bool _p2;

    RTTR_ENABLE(top)
};

/////////////////////////////////////////////////////////////////////////////////////////

struct right : virtual top
{

    right() : _p3(true){}
    bool _p3;

    RTTR_ENABLE(top)
};

/////////////////////////////////////////////////////////////////////////////////////////

struct right_2
{
    virtual ~right_2() {}
    right_2() : _p4(true){}
    bool _p4;
    RTTR_ENABLE()
};

/////////////////////////////////////////////////////////////////////////////////////////

struct bottom : left, right, right_2
{
    bottom() : _p5(23.0){}

    double _p5;

    RTTR_ENABLE(left, right, right_2)
};

}

static double g_name;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    registration::class_<ns_property::top>("ns_property::top")
        .property("p1", &ns_property::top::_p1);

    registration::class_<ns_property::left>("ns_property::left")
        .constructor<>()
        .property("p2", &ns_property::left::_p2);

    registration::class_<ns_property::right>("ns_property::right")
        .constructor<>()
        .property("p3", &ns_property::right::_p3)
        .property("p2", &g_name); // double property

    registration::class_<ns_property::right_2>("ns_property::right_2")
        .constructor<>()
        .property("p4", &ns_property::right_2::_p4);

    registration::class_<ns_property::bottom>("ns_property::bottom")
        .constructor<>()
        .property("p5", &ns_property::bottom::_p5)
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - inheritance - order", "[property]")
{
    type t = type::get<ns_property::bottom>();
    auto props = t.get_properties();
    REQUIRE(props.size() == 6);

    CHECK(props[0].get_name() == "p1"); // top
    CHECK(props[1].get_name() == "p2"); // left
    CHECK(props[2].get_name() == "p3"); // right
    CHECK(props[3].get_name() == "p2"); // right
    CHECK(props[4].get_name() == "p4"); // right2
    CHECK(props[5].get_name() == "p5"); // bottom
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("property - class - inheritance - invoke", "[property]")
{
    type t = type::get<ns_property::bottom>();
    auto props = t.get_properties();

    ns_property::bottom instance;
    ns_property::top& top = instance;
    // try access from top instance a property in the most derived class (bottom)
    variant ret = props[0].get_value(top);
    REQUIRE(ret.is_type<int>() == true);
    CHECK(ret.get_value<int>() == 12);
    // try to change the value
    props[0].set_value(top, 2000);
    CHECK(instance._p1 == 2000);

    // and now the other way around, from bottom a top property
    ret = props[5].get_value(instance);
    REQUIRE(ret.is_type<double>() == true);
    CHECK(ret.get_value<double>() == 23.0);
    // try to change the value
    props[5].set_value(top, 42.0);
    CHECK(instance._p5 == 42.0);

    // check double declared property is from left class
    CHECK(props[1].get_declaring_type() == type::get<ns_property::left>());
    // the right class has still its property?
    CHECK(type::get<ns_property::right>().get_property("p2").is_valid() == true);

    property p1 = type::get<ns_property::bottom>().get_property("p1");
    CHECK(bool(p1) == true);
}

/////////////////////////////////////////////////////////////////////////////////////////
