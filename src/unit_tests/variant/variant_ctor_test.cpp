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
#include <iostream>
#include <rttr/type>

#include <type_traits>

#include <vector>
#include <map>
#include <string>

using namespace rttr;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor", "[variant]")
{
    SECTION("empty type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK((bool)var      == false);
        CHECK(var.get_type().is_valid() == false);

        variant var_2 = var;
        CHECK(var_2.is_valid() == false);
        CHECK((bool)var_2      == false);
        CHECK(var_2.get_type().is_valid() == false);

        variant var_3 = std::move(var_2);
        CHECK(var_3.is_valid() == false);
        CHECK((bool)var_3      == false);
        CHECK(var_3.get_type().is_valid() == false);
    }

    SECTION("insert arithmetic type")
    {
        variant var = 42;
        CHECK(var.is_valid() == true);
        CHECK((bool)var      == true);
        CHECK(var.get_type().is_valid() == true);
        CHECK(var.get_type() == type::get<int>());

        variant var_2 = var;
        CHECK(var_2.is_valid() == true);
        CHECK((bool)var_2      == true);
        CHECK(var_2.get_type().is_valid() == true);
        CHECK(var_2.get_type() == type::get<int>());

        variant var_3 = std::move(var_2);
        CHECK(var_2.is_valid() == false);
        CHECK(var_3.is_valid() == true);
        CHECK((bool)var_3      == true);
        CHECK(var_3.get_type().is_valid() == true);
        CHECK(var_3.get_type() == type::get<int>());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

struct simple_type
{
    simple_type() {}
    simple_type(const simple_type& other) 
    :   moved(other.moved),
        moved_from(other.moved_from)
    {
    }
    simple_type(simple_type&& other)
    :   moved(true) 
    { 
        other.moved_from = true; 
    }

    bool moved = false;
    bool moved_from = false;
};

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("move custom type", "[variant]")
{
    simple_type obj;
    variant var(obj);

    CHECK(obj.moved_from == false);
    
    variant var_2(std::move(obj));
    CHECK(obj.moved_from == true);
    CHECK(var_2.get_value<simple_type>().moved == true );
}

/////////////////////////////////////////////////////////////////////////////////////////

bool is_stored_internally(void* obj, const rttr::variant& var)
{
  std::uintptr_t a_addr = reinterpret_cast<std::uintptr_t>(&var);
  std::uintptr_t a_end = a_addr + sizeof(var);
  std::uintptr_t obj_addr = reinterpret_cast<std::uintptr_t>(obj);
  return (a_addr <= obj_addr) && (obj_addr < a_end);
}

/////////////////////////////////////////////////////////////////////////////////////////

struct BigObj 
{ 
    // two doubles, cannot be stored internally inside variant
    std::aligned_storage<sizeof(double[2]),8>::type m_data;
};


/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("check variant storage type", "[variant]")
{
    variant var = BigObj{};
    BigObj& obj = var.get_value<BigObj>();
    CHECK( !is_stored_internally(&obj, var) );

    var = 'D';
    char& ref_char = var.get_value<char>();
    CHECK( is_stored_internally(&ref_char, var) );

    var = 23;
    int& ref_int = var.get_value<int>();
    CHECK( is_stored_internally(&ref_int, var) );

    bool bool_array[4] = {true, false, true, false};
    var = bool_array;
    auto& ref_bool_array = var.get_value<bool[4]>();
    CHECK( is_stored_internally(&ref_bool_array, var) );
}

/////////////////////////////////////////////////////////////////////////////////////////

struct self_aware
{
    self_aware() { }
    ~self_aware() 
    { 
        CHECK(self == this); 
    }
    self_aware(const self_aware&) { }
    self_aware& operator=(const self_aware&) { return *this; }
    self_aware(self_aware&&) RTTR_NOEXCEPT_OR_NOTHROW { }
    self_aware& operator=(self_aware&&) RTTR_NOEXCEPT_OR_NOTHROW { return *this; }

    const self_aware* self = this;
};

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("copy non trivial type", "[variant]")
{
    static_assert(std::is_nothrow_move_constructible<self_aware>::value, "");

#if RTTR_COMPILER == RTTR_COMPILER_GNUC && RTTR_COMP_VER <= 5000
    static_assert(!std::has_trivial_copy_constructor<self_aware>::value, "");
#else
    static_assert(!std::is_trivially_copyable<self_aware>::value, "");
#endif

    SECTION("basic")
    {
        self_aware obj;
        variant var = obj;
    }

    SECTION("swap")
    {
        self_aware obj;
        variant a = obj;
        variant b = a;
        a.swap(b);
    }

    SECTION("move")
    {
        self_aware obj;
        variant a = obj;
        variant b = a;
        {
            variant tmp = std::move(a);
            a = std::move(b);
            b = std::move(tmp);
        }
    }
}