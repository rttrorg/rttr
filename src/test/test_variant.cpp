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

#include "catch.hpp"

#include <rttr/reflect>

#include <vector>
#include <map>
#include <string>

using namespace rttr;
using namespace std;

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y)
{
    return std::abs(x - y) <= std::numeric_limits<T>::epsilon();
}


struct point
{
    point(int x, int y) : _x(x), _y(y) {}
    int _x;
    int _y;
};

RTTR_DECLARE_TYPE(point)

struct vector2d
{
    vector2d(int x, int y) : _x(x), _y(y) {}
    int _x;
    int _y;
};

RTTR_DECLARE_TYPE(vector2d)


static std::string convert_to_string(const point& p, bool& ok)
{
    ok = true;
    return std::to_string(p._x) + ", " + std::to_string(p._y);
}

static vector2d convert_to_vector(const point& p, bool& ok)
{
    ok = true;
    return vector2d(p._x, p._y);
}

struct base
{
    int dummy;
    RTTR_ENABLE()
};
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(base)

struct derived : virtual base
{
    double dummy2;
    RTTR_ENABLE(base)
};

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(derived)
RTTR_DECLARE_TYPE(derived**)

RTTR_REGISTER
{
    type::register_converter_func(convert_to_string);
    type::register_converter_func(convert_to_vector);
}

RTTR_DECLARE_TYPE(int[10])
RTTR_DECLARE_TYPE(int[2][10])
RTTR_DECLARE_TYPE(int(*)[2][10])
RTTR_DECLARE_TYPE(void**)

TEST_CASE("variant test - BasicTests", "[variant]")
{
    using namespace rttr;

    SECTION("simple basic check")
    {
        variant var = string("hello world");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<string>() == true);
        const int value = 12;
        variant foo(&value);
        const int* result = foo.get_value<const int*>();
        REQUIRE(result == &value);

        var = 42;
        REQUIRE(var.is_type<int>() == true);
        
        var = "hello char array";
        REQUIRE(var.is_type<char[17]>() == true);

        var = 42.0;
        REQUIRE(var.is_type<double>() == true);
    }

    SECTION("extracting tests")
    {
        variant var = string("hello world");
        REQUIRE(var.get_value<string>() == string("hello world"));

        var = 42;
        REQUIRE(var.get_value<int>() == 42);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to bool", "[variant]")
{
    SECTION("bool to bool")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);
        
        REQUIRE(var.convert<bool>() == true);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == true);

        var = false;
        REQUIRE(var.to_bool() == false);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == false);
    }

    SECTION("char to bool")
    {
        variant var = "true";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);
        REQUIRE(var.convert<bool>() == true);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == true);

        var = "fdsfsdf";
        REQUIRE(var.to_bool() == true);
        REQUIRE(var.convert<bool>() == true);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == true);
    
        var = "false";
        REQUIRE(var.to_bool() == false);
        var = "false   ";
        REQUIRE(var.to_bool() == false);
        var = "   false   ";
        REQUIRE(var.to_bool() == false);
        var = "   FALSE   ";
        REQUIRE(var.to_bool() == false);
        var = " \n  FALSE\n";
        REQUIRE(var.to_bool() == false);

        REQUIRE(var.convert<bool>() == false);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == false);
    }

    SECTION("std::string to bool")
    {
        variant var = std::string("true");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);

        var = std::string("fdsfsdf");
        REQUIRE(var.to_bool() == true);
    
        var = std::string("false");
        REQUIRE(var.to_bool() == false);
        var = std::string("false   ");
        REQUIRE(var.to_bool() == false);
        var = std::string("   false   ");
        REQUIRE(var.to_bool() == false);
        var = std::string("   FALSE   ");
        REQUIRE(var.to_bool() == false);
        var = std::string(" \n  FALSE\n");
        REQUIRE(var.to_bool() == false);
    }
    
    SECTION("int to bool")
    {
        variant var = 1;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);
        REQUIRE(var.convert<bool>() == true);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == true);

        var = 0;
        REQUIRE(var.to_bool() == false);
        REQUIRE(var.convert<bool>() == false);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == false);
    }

    SECTION("float to bool")
    {
        variant var = 1.0f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);
        REQUIRE(var.convert<bool>() == true);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == true);

        var = 0.0f;
        REQUIRE(var.to_bool() == false);

        var = 1.0f / 10000000.0f;
        REQUIRE(var.to_bool() == false);
        REQUIRE(var.convert<bool>() == false);
        REQUIRE(var.convert(type::get<bool>()) == true);
        REQUIRE(var.get_value<bool>() == false);
    }

    SECTION("double to bool")
    {
        variant var = 1.0;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_bool() == true);

        var = 0.0;
        REQUIRE(var.to_bool() == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to int", "[variant]")
{
    SECTION("int to int")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 12);
        REQUIRE(var.convert<int>() == 12);
        REQUIRE(var.convert(type::get<int>()) == true);
        REQUIRE(var.get_value<int>() == 12);

        var = -23;
        REQUIRE(var.to_int() == -23);
        REQUIRE(var.convert<int>() == -23);
        REQUIRE(var.convert(type::get<int>()) == true);
        REQUIRE(var.get_value<int>() == -23);
    }

    SECTION("char to int")
    {
        variant var = "23";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 23);
        REQUIRE(var.convert<int>() == 23);
        REQUIRE(var.convert(type::get<int>()) == true);
        REQUIRE(var.get_value<int>() == 23);

        var = "-12";
        REQUIRE(var.to_int() == -12);
    
        var = "text 34 and text";
        bool ok = false;
        REQUIRE(var.to_int(&ok) == 0);
        REQUIRE(ok == false);

        var = "34 and text";
        ok = false;
        REQUIRE(var.to_int(&ok) == 0);
        REQUIRE(ok == false);
        REQUIRE(var.convert<int>() == 0);
        REQUIRE(var.convert(type::get<int>()) == false);
    }

    SECTION("std::string to int")
    {
        variant var = std::string("23");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 23);
        REQUIRE(var.convert<int>() == 23);
        REQUIRE(var.convert(type::get<int>()) == true);
        REQUIRE(var.get_value<int>() == 23);

        var = std::string("-12");
        REQUIRE(var.to_int() == -12);
    
        var = std::string("text 34 and text");
        bool ok = false;
        REQUIRE(var.to_int(&ok) == 0);
        REQUIRE(ok == false);

        var = std::string("34 and text");
        ok = false;
        REQUIRE(var.to_int(&ok) == 0);
        REQUIRE(ok == false);
    }
    
    SECTION("bool to int")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 1);

        var = false;
        REQUIRE(var.to_int() == 0);
    }

    SECTION("float to int")
    {
        variant var = 1.5f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 1);

        var = 3.1423f;
        REQUIRE(var.to_int() == 3);

        var = 0.0f;
        REQUIRE(var.to_int() == 0);
    }

    SECTION("double to int")
    {
        variant var = 1.5;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_int() == 1);

        var = 3.1423f;
        REQUIRE(var.to_int() == 3);

        var = 0.0;
        REQUIRE(var.to_int() == 0);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to std::string", "[variant]")
{
    SECTION("int to std::string")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "12");
        REQUIRE(var.convert<std::string>() == "12");
        REQUIRE(var.convert(type::get<std::string>()) == true);
        REQUIRE(var.get_value<std::string>() == "12");

        var = -23;
        REQUIRE(var.to_string() == "-23");
    }

    SECTION("char to std::string")
    {
        variant var = "text";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "text");

        var = "text 42";
        REQUIRE(var.to_string() == "text 42");
    }

    SECTION("std::string to std::string")
    {
        variant var = std::string("23");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "23");

        var = std::string("-12");
        REQUIRE(var.to_string() == "-12");
    }
    
    SECTION("bool to std::string")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "true");

        var = false;
        REQUIRE(var.to_string() == "false");
    }

    SECTION("float to std::string")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "1.567");

        var = 3.12345678f;
        REQUIRE(var.to_string() == "3.12346");

        var = 0.0f;
        REQUIRE(var.to_string() == "0");
    }

    SECTION("double to std::string")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_string() == "1.567");

        var = 3.12345678;
        REQUIRE(var.to_string() == "3.12345678");

        var = 0.0;
        REQUIRE(var.to_string() == "0");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to float", "[variant]")
{
    SECTION("int to float")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_float() == 12.0f);
        REQUIRE(var.convert<float>() == 12.0f);
        REQUIRE(var.convert(type::get<float>()) == true);
        REQUIRE(var.get_value<float>() == 12.0f);

        var = -23;
        REQUIRE(var.to_float() == -23.0f);
    }

    SECTION("char to float")
    {
        variant var = "23.0";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_float() == 23.0f);
        REQUIRE(var.convert<float>() == 23.0f);
        REQUIRE(var.convert(type::get<float>()) == true);
        REQUIRE(var.get_value<float>() == 23.0f);

        var = "text 42";
        bool ok = false;
        REQUIRE(var.to_float(&ok) == 0);
        REQUIRE(ok == false);

        var = "1.23456";
        ok = false;
        REQUIRE(var.to_float(&ok) == 1.23456f);
        REQUIRE(ok == true);

        var = "1.23456 Text";
        ok = false;
        REQUIRE(var.to_float(&ok) == 0);
        REQUIRE(ok == false);

    }

    SECTION("std::string to float")
    {
        variant var = std::string("23.0");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_float() == 23.0f);

        var = std::string("text 42");
        bool ok = false;
        REQUIRE(var.to_float(&ok) == 0);
        REQUIRE(ok == false);

        var = std::string("1.23456");
        ok = false;
        REQUIRE(var.to_float(&ok) == 1.23456f);
        REQUIRE(ok == true);

        var = std::string("1.23456 Text");
        ok = false;
        REQUIRE(var.to_float(&ok) == 0);
        REQUIRE(ok == false);
    }
    
    SECTION("bool to float")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_float() == 1.0f);

        var = false;
        REQUIRE(var.to_float() == 0.0f);
    }

    SECTION("float to float")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(almost_equal(var.to_float(), 1.567f) == true);

        var = 3.12345678f;
        REQUIRE(almost_equal(var.to_float(), 3.1234567f) == true);

        var = 0.0f;
        REQUIRE(almost_equal(var.to_float(), 0.0f) == true);
    }

    SECTION("double to float")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(almost_equal(var.to_float(), 1.567f) == true);

        var = 3.12345678;
        REQUIRE(almost_equal(var.to_float(), 3.1234567f) == true);

        var = 0.0;
        REQUIRE(almost_equal(var.to_float(), 0.0f) == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to double", "[variant]")
{
    SECTION("int to double")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_double() == 12.0);
        REQUIRE(var.convert<double>() == 12.0);
        REQUIRE(var.convert(type::get<double>()) == true);
        REQUIRE(var.get_value<double>() == 12.0);

        var = -23;
        REQUIRE(var.to_double() == -23.0);
    }

    SECTION("char to double")
    {
        variant var = "23.0";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_double() == 23.0);

        var = "text 42";
        bool ok = false;
        REQUIRE(var.to_double(&ok) == 0);
        REQUIRE(ok == false);

        var = "1.23456";
        ok = false;
        REQUIRE(var.to_double(&ok) == 1.23456);
        REQUIRE(ok == true);

        var = "1.23456 Text";
        ok = false;
        REQUIRE(var.to_double(&ok) == 0.0);
        REQUIRE(ok == false);
    }

    SECTION("std::string to double")
    {
        variant var = std::string("23.0");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_double() == 23.0);

        var = std::string("text 42");
        bool ok = false;
        REQUIRE(var.to_double(&ok) == 0.0);
        REQUIRE(ok == false);

        var = std::string("1.23456");
        ok = false;
        REQUIRE(var.to_double(&ok) == 1.23456);
        REQUIRE(ok == true);

        var = std::string("1.23456 Text");
        ok = false;
        REQUIRE(var.to_double(&ok) == 0.0);
        REQUIRE(ok == false);
    }
    
    SECTION("bool to double")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.to_double() == 1.0);

        var = false;
        REQUIRE(var.to_double() == 0.0);
    }

    SECTION("float to double")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(almost_equal(var.to_double(), 1.5670000314712524) == true);

        var = 3.123456f;
        REQUIRE(almost_equal(var.to_double(), 3.1234560012817383) == true);

        var = 0.0f;
        REQUIRE(almost_equal(var.to_double(), 0.0) == true);
    }

    SECTION("double to double")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(almost_equal(var.to_double(), 1.567) == true);

        var = 3.12345678;
        REQUIRE(almost_equal(var.to_double(), 3.12345678) == true);

        var = 0.0;
        REQUIRE(almost_equal(var.to_double(), 0.0) == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant test - convert custom types", "[variant]")
{
    variant var = point(12, 34);
    REQUIRE(var.is_type<point>() == true);
    bool ok = false;
    std::string text = var.to_string(&ok);
    REQUIRE(ok == true);

    REQUIRE(var.can_convert<std::string>() == true);
    REQUIRE(var.can_convert<vector2d>() == true);

    REQUIRE(var.convert(type::get<std::string>())   == true);
    REQUIRE(var.is_type<std::string>()              == true);
    REQUIRE(var.get_value<std::string>()            == "12, 34");

    // convert to other custom type
    var = point(12, 34);
    bool ret = var.convert(type::get<vector2d>());
    REQUIRE(ret == true);
    REQUIRE(var.is_type<vector2d>() == true);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant test - convert internal", "[variant]")
{
    variant var = 23;
    bool ret = var.convert(type::get<std::string>());
    REQUIRE(ret == true);
    REQUIRE(var.is_type<std::string>() == true);

    derived* d = new derived;
    base* b = d;
    var = b;
    REQUIRE(var.can_convert(type::get<derived>())   == false);
    REQUIRE(var.can_convert(type::get<derived*>())  == true);
    
    bool could_convert = var.convert(type::get<derived**>());
    REQUIRE(could_convert == false);

    could_convert = var.convert(type::get<derived*>());
    REQUIRE(could_convert == true);
    REQUIRE(var.is_type<derived*>() == true);
    REQUIRE(var.get_value<derived*>() == d);
    REQUIRE(var.convert<base*>() == b);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant test - array", "[variant]")
{
   SECTION("test raw array")
    {
        int obj[2][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        auto foo = &obj;
        variant_array int_array = foo;
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
        std::vector<int> vec(50, 0);
        variant var = &vec;
        variant_array vec_array = var.to_array();
        REQUIRE(vec_array.is_valid()    == true);
        REQUIRE(vec_array.is_dynamic()  == true);
        REQUIRE(vec_array.get_rank()        == 1);
        REQUIRE(vec_array.get_rank_type(0)  == type::get(vec));
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
        variant_array vec_array = var.to_array();
        // pointer to const array cannot be changed
        REQUIRE(vec_array.set_size(0)   == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_array test - array", "[variant]")
{
    variant_array array = std::vector<int>(50, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////