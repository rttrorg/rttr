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

#include "my_visitor.h"

#include <catch/catch.hpp>
#include <rttr/type>

using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("visitor - type", "[visitor]")
{
    my_visitor vi;
    auto t = type::get_by_name("visitor_test_class");
    vi.visit(t);

    REQUIRE(vi.visited_types.size() == 4);
    // first the base class types are visited
    CHECK(vi.visited_types[0] == type::get_by_name("visitor_test_class_base"));
    CHECK(vi.visited_types[1] == type::get_by_name("visitor_test_class_base"));
    // the the type itself
    CHECK(vi.visited_types[2] == type::get_by_name("visitor_test_class"));
    CHECK(vi.visited_types[3] == type::get_by_name("visitor_test_class"));

    REQUIRE(vi.visited_meths.size() == 2);
    CHECK(vi.visited_meths[0] == t.get_method("base_method"));
    CHECK(vi.visited_meths[1] == t.get_method("some_method"));
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("visitor - method", "[visitor]")
{
    my_visitor vi;
    auto t = type::get_by_name("visitor_test_class");
    auto meth = t.get_method("some_method");
    vi.visit(meth);

    REQUIRE(vi.visited_meths.size() == 1);
    CHECK(vi.visited_meths[0] == meth);

    REQUIRE(vi.visited_types.size() == 0);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("visitor - global method", "[visitor]")
{
    my_visitor vi;
    auto meth = type::get_global_method("some_global_method");
    vi.visit(meth);

    REQUIRE(vi.visited_meths.size() == 1);
    CHECK(vi.visited_meths[0] == meth);

    REQUIRE(vi.visited_types.size() == 0);
}

/////////////////////////////////////////////////////////////////////////////////////////