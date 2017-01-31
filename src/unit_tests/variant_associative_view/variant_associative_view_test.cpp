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

    SECTION("valid test")
    {
        variant var = std::set<int>({ 1, 2, 3 });
        variant_associative_view view = var.create_associative_view();

        CHECK(view.get_type() == type::get<std::set<int>>());
        CHECK(view.get_type() == var.get_type());
    }


    SECTION("invalid test")
    {
        variant var = 23;
        variant_associative_view view = var.create_associative_view();

        CHECK(view.get_type().is_valid() == false);

        var = variant();
        view = var.create_associative_view();
        CHECK(view.get_type().is_valid() == false);
    }
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


    SECTION("valid - set")
    {
        auto set = std::set<int>({ 1, 2, 3 });

        variant var = set;
        CHECK(var.is_associative_container() == true);

        var = std::ref(set);
        CHECK(var.is_associative_container() == true);

        var = &set;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - map")
    {
        auto map = std::map<int, std::string>{ { 1, "one" }, { 2, "two" }, { 3, "three" } };

        variant var = map;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - multiset")
    {
        auto multiset = std::multiset<int>{ 1, 2, 2, 3, 4 };

        variant var = multiset;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - multimap")
    {
        auto multimap = std::multimap<int, std::string>{ { 1, "one" }, { 2, "two" },
                                                         { 2, "two" }, { 3, "three" } };

        variant var = multimap;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - unordered_set")
    {
        auto unordered_set = std::unordered_set<int>{ 1, 2, 2, 3, 4 };

        variant var = unordered_set;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - unordered_map")
    {
        auto unordered_set = std::unordered_set<int>{ 1, 2, 2, 3, 4 };

        variant var = unordered_set;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - unordered_multiset")
    {
        auto unordered_multiset = std::unordered_multiset<int>{ 1, 2, 2, 3, 4 };

        variant var = unordered_multiset;
        CHECK(var.is_associative_container() == true);
    }

    SECTION("valid - unordered_multimap")
    {
        auto unordered_multimap = std::unordered_multimap<int, std::string>{ { 1, "one" }, { 2, "two" },
                                                                             { 2, "two" }, { 3, "three" } };

        variant var = unordered_multimap;
        CHECK(var.is_associative_container() == true);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_associative_view::iterator operations", "[variant_associative_view]")
{
    auto unordered_set = std::set<int>{ 1, 2, 3, 4 };

    variant var = unordered_set;

    auto view = var.create_associative_view();

    auto itr = view.begin();
    REQUIRE(itr != view.end());

    itr++;
    CHECK(itr.value().extract_wrapped_value().to_int() == 2);
    itr--;
    CHECK(itr.value().extract_wrapped_value().to_int() == 1);
    ++itr;
    CHECK(itr.value().extract_wrapped_value().to_int() == 2);
    --itr;
    CHECK(itr.value().extract_wrapped_value().to_int() == 1);

    itr = view.begin();
    itr += 2;
    CHECK(itr.value().extract_wrapped_value().to_int() == 3);
    itr += 1;
    itr -= 3;
    CHECK(itr.value().extract_wrapped_value().to_int() == 1);

    itr = view.begin();
    itr = itr + 3;
    CHECK(itr.value().extract_wrapped_value().to_int() == 4);
    itr = itr - 3;
    CHECK(itr.value().extract_wrapped_value().to_int() == 1);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_associative_view::find", "[variant_associative_view]")
{
    auto map = std::map<int, std::string>{ { 1, "one" }, { 2, "two" }, { 3, "three" } };
    variant var = map;
    variant_associative_view view = var.create_associative_view();

    REQUIRE(view.is_valid() == true);

    auto itr = view.find(1);

    CHECK(itr != view.end());
    CHECK(itr.key().extract_wrapped_value() == 1);
    CHECK(itr.value().extract_wrapped_value() == "one");


    SECTION("invalid test")
    {
        auto itr = view.find(4);
        CHECK(itr == view.end());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
