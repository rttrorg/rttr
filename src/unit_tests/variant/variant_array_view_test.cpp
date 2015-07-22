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

#include <catch/catch.hpp>

#include <rttr/register>

#include <vector>
#include <map>
#include <string>

using namespace rttr;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant test - array", "[variant]")
{
   SECTION("test raw array")
    {
        int obj[2][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        variant var = &obj;
        variant_array_view int_array = var.create_array_view();
        REQUIRE(int_array.is_valid() == true);

        // check meta information
        REQUIRE(int_array.get_type()        == type::get(&obj));
        REQUIRE(int_array.get_rank()        == 2);
        REQUIRE(int_array.get_rank_type(0)  == type::get<int[2][10]>());
        REQUIRE(int_array.get_rank_type(1)  == type::get<int[10]>());
        REQUIRE(int_array.get_rank_type(2)  == type::get<int>());
        REQUIRE(int_array.get_rank_type(3).is_valid() == false);

        REQUIRE(int_array.is_dynamic()      == false);
        REQUIRE(int_array.get_size()        == 2);
        REQUIRE(int_array.get_size(0)       == 10);
        REQUIRE(int_array.get_size(1)       == 10);

        int sub_array_int[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (std::size_t i = 0; i < int_array.get_size(); ++i)
            int_array.set_value(i, sub_array_int);

        REQUIRE(obj[0][5] == 5);
        REQUIRE(obj[1][9] == 9);

        for (std::size_t i = 0; i < int_array.get_size(); ++i)
            for (std::size_t j = 0; j < int_array.get_size(i); ++j)
                int_array.set_value(i, j, -1);

        REQUIRE(obj[0][5] == -1);
        REQUIRE(obj[1][9] == -1);

        bool inserted = int_array.insert_value(0, sub_array_int);
        REQUIRE(inserted == false);
        
        bool removed = int_array.remove_value(0);
        REQUIRE(removed == false);
    }

    SECTION("test dynamic array")
    {
        variant var = std::vector<int>(50, 0);

        // check whether we can convert to array
        REQUIRE(var.get_type().is_array()                == true);

        variant_array_view vec_array = var.create_array_view();
        REQUIRE(vec_array.is_valid()    == true);
        REQUIRE(vec_array.is_dynamic()  == true);
        REQUIRE(vec_array.get_rank()        == 1);
        REQUIRE(vec_array.get_rank_type(0)  == type::get(std::vector<int>()));
        REQUIRE(vec_array.get_rank_type(1)  == type::get<int>());

        REQUIRE(vec_array.get_size()        == 50);
        REQUIRE(vec_array.get_size(0)       == 0); // invalid
        
        vec_array.set_size(70);
        REQUIRE(vec_array.get_size()        == 70);
        vec_array.set_size(70, 0);
        REQUIRE(vec_array.get_size()        == 70);
        REQUIRE(vec_array.get_size(0)       == 0);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    
        bool inserted = vec_array.insert_value(50, 42);
        REQUIRE(inserted == true);
        ret = vec_array.get_value(50);
        REQUIRE(ret.get_value<int>() == 42);

        ret = vec_array.get_value(51);
        REQUIRE(ret.get_value<int>() == 23);
        REQUIRE(vec_array.get_size() == 71);

        bool removed = vec_array.remove_value(50);
        REQUIRE(removed == true);
        REQUIRE(vec_array.get_size() == 70);
        ret = vec_array.get_value(50);
        REQUIRE(ret.get_value<int>() == 23);
    }

    SECTION("test const array")
    {
        const std::vector<int> vec(50, 0);
        variant var = &vec;
        variant_array_view vec_array = var.create_array_view();
        // pointer to const array cannot be changed
        REQUIRE(vec_array.set_size(0)   == false);
    }

    SECTION("check if vector<bool>() is supported in variant")
    {
        auto orig_vec = std::vector<bool>({true, false, false});
        variant var = &orig_vec;
        REQUIRE(*var.get_value<std::vector<bool>*>() == orig_vec);

        variant_array_view var_arr = var.create_array_view();
        var_arr.set_value(0, false);
        REQUIRE(orig_vec[0] == false);

        var_arr.set_size(100);
        REQUIRE(orig_vec.size() == 100);

        var_arr.set_size(50);
        REQUIRE(orig_vec.size() == 50);

        var_arr.insert_value(25, true);
        REQUIRE(orig_vec[25] == true);

        // check copied array
        var =  std::vector<bool>({true, false, false});
        var_arr = var.create_array_view();
        REQUIRE(var_arr.get_size() == 3);

        var_arr.set_value(0, false);
        CHECK(var_arr.get_value(0).get_value<bool>() == false);
        CHECK(var_arr.get_value(1).get_value<bool>() == false);
        CHECK(var_arr.get_value(2).get_value<bool>() == false);

        var_arr.set_value(1, true);
        var_arr.set_value(2, true);
        CHECK(var_arr.get_value(1).get_value<bool>() == true);
        CHECK(var_arr.get_value(2).get_value<bool>() == true);
    }

    SECTION("check array with wrapper type")
    {
        std::vector<int> vec(50, 0);

        std::reference_wrapper<decltype(vec)> ref_vec = std::ref(vec);
        variant var = ref_vec;
        variant_array_view vec_array = var.create_array_view();
        REQUIRE(vec_array.is_valid()    == true);
       
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);
        CHECK(vec.size()                == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    }

    SECTION("check moved wrapper array type")
    {
        variant var = std::make_shared<std::vector<int>>(50, 0);
        //if (var.can_create_array_view())
        variant_array_view vec_array = var.create_array_view();
        REQUIRE(vec_array.is_valid()    == true);
        CHECK(vec_array.get_size()      == 50);
       
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    }

    SECTION("copy variant into variant_array_view")
    {
        variant var = std::vector<int>(100, 0);
        variant_array_view array = var.create_array_view();

        CHECK(var.is_valid() == true);
        CHECK(array.get_size() == 100);
        CHECK((array.get_type() == type::get<std::vector<int>>()));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////