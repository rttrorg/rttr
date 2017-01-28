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

#include <catch/catch.hpp>

#include <rttr/type>

#include <vector>
#include <map>
#include <string>

using namespace rttr;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_associative_view::ctor", "[variant_associative_view]")
{
   SECTION("empty")
   {
        variant_associative_view a;
        CHECK(a.is_valid() == false);

        variant_associative_view b(a);
        CHECK(b.is_valid() == false);
        a.swap(b);
        CHECK(a.is_valid() == false);
        CHECK(b.is_valid() == false);

        variant var;
        variant_associative_view c = var.create_associative_view();

        CHECK(c.is_valid() == false);
        CHECK(c.get_size() == 0);
        CHECK(c.get_type().is_valid() == false);
        CHECK(c.erase(23) == 0);
   }

   SECTION("invalid")
   {
       variant var = 2;
       variant_associative_view a = var.create_associative_view();
       CHECK(a.is_valid() == false);
   }

   SECTION("valid")
   {
        variant var = std::set<int>({1, 2, 3});
        variant_associative_view a = var.create_associative_view();
        CHECK(a.is_valid() == true);

        variant_associative_view b(a);
        CHECK(b.is_valid() == true);
   }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_associative_view::get_type", "[variant_associative_view]")
{
    variant var = std::set<int>({1, 2, 3});
    variant_associative_view view = var.create_associative_view();

    CHECK(view.get_type() == type::get<std::set<int>>());
    CHECK(view.get_type() == var.get_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_associative_view::is_associative_container", "[variant_associative_view]")
{
    SECTION("invalid")
    {
        variant var;
        CHECK(var.is_associative_container() == false);

        var = 23;
        CHECK(var.is_associative_container() == false);

        var = std::vector<int>({1, 2, 3});
        CHECK(var.is_associative_container() == false);

        auto vec = std::vector<int>({ 1, 2, 3 });
        var = std::ref(vec);
        CHECK(var.is_associative_container() == false);
    }


    SECTION("valid")
    {
        auto set = std::set<int>({ 1, 2, 3 });

        variant var = set;
        CHECK(var.is_associative_container() == true);

        var = std::ref(set);
        CHECK(var.is_associative_container() == true);

        var = &set;
        CHECK(var.is_associative_container() == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
