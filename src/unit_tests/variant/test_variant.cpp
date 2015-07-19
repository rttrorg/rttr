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

namespace
{
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y)
{
    return std::abs(x - y) <= std::numeric_limits<T>::epsilon();
}

struct point
{
    point(int x, int y) : _x(x), _y(y) {}

    point(const point& other) : _x(other._x), _y(other._y) { }

    point(point&& other) : _x(other._x), _y(other._y) { other._x = 0; other._y = 0; }

    bool operator ==(const point& other) { return (_x == other._x && _y == other._y); }
    int _x;
    int _y;
};

struct vector2d
{
    vector2d(int x, int y) : _x(x), _y(y) {}
    int _x;
    int _y;
};

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

struct derived : virtual base
{
    double dummy2;
    RTTR_ENABLE(base)
};

struct other_derived : virtual base
{
    double dummy3;
    RTTR_ENABLE(base)
};

} // end anonymous namespace

RTTR_REGISTER
{
    type::register_converter_func(convert_to_string);
    type::register_converter_func(convert_to_vector);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant copy test", "[variant]")
{
    SECTION("primitive type")
    {
        int value = 23;
        variant var = value;
        variant var_cpy = var;
        REQUIRE(var_cpy.is_valid() == true);
        REQUIRE(var_cpy.is_type<int>() == true);
        CHECK(var_cpy.get_value<int>() == value);
    }

    SECTION("small custom type")
    {
        point value = point(23, 42);
        variant var = value;
        variant var_cpy = var;
        REQUIRE(var_cpy.is_valid() == true);
        REQUIRE(var_cpy.is_type<point>() == true);
        CHECK(var_cpy.get_value<point>() == value);
    }

    SECTION("big custom type")
    {
        std::string text = std::string("hello world");
        variant var = text;
        variant var_cpy = var;
        REQUIRE(var_cpy.is_valid() == true);
        REQUIRE(var_cpy.is_type<std::string>() == true);
        CHECK(var_cpy.get_value<std::string>() == text);
    }

    SECTION("variant_array")
    {
        variant_array array = std::array<int, 100>();
        REQUIRE(array.get_size() == 100);

        variant var = array;
        CHECK((var.get_type() == type::get<std::array<int, 100>>()));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant assignment test", "[variant]")
{
    SECTION("primitive type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK(var.get_type().is_valid() == false);

        var = 23;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<int>() == true);
        CHECK(var.get_value<int>() == 23);
    }

    SECTION("small custom type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK(var.get_type().is_valid() == false);

        point value = point(23, 42);
        var = value;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<point>() == true);
        CHECK(var.get_value<point>() == value);
    }

    SECTION("big custom type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK(var.get_type().is_valid() == false);

        std::string text = std::string("hello world");
        var = text;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<std::string>() == true);
        CHECK(var.get_value<std::string>() == text);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant move test", "[variant]")
{
    SECTION("move ctor - small custom type")
    {
        point value = point(23, 42);

        variant var = std::move(value);
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<point>() == true);
        CHECK(value == point(0, 0)); // when empty, it was moved
    }

    SECTION("move ctor - big custom type")
    {
        string text("hello world");

        variant var = std::move(text);
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<string>() == true);
        CHECK(text.empty() == true);
    }

    SECTION("move assignment - small custom type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK(var.get_type().is_valid() == false);

        point value = point(23, 42);
        var = std::move(value);
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<point>() == true);
        CHECK(value == point(0, 0)); // when empty, it was moved
    }

    SECTION("move assignment - big custom type")
    {
        variant var;
        CHECK(var.is_valid() == false);
        CHECK(var.get_type().is_valid() == false);

        string text("hello world");
        var = std::move(text);
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<string>() == true);
        CHECK(text.empty() == true);
    }

    SECTION("rvalue move")
    {
        variant var = std::vector<int>(50, 0);
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.is_type<std::vector<int>>() == true);
    }

    SECTION("variant_array")
    {
        variant_array array = std::array<int, 100>();
        REQUIRE(array.get_size() == 100);
        
        variant var = std::move(array);
        CHECK(array.is_valid() == false);
        CHECK((var.get_type() == type::get<std::array<int, 100>>()));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant swap test", "[variant]")
{
    string text("hello world");
    point point_value = point(23, 42);

    variant var1 = text;
    variant var2 = point_value;

    var1.swap(var2);

    REQUIRE(var1.is_valid() == true);
    REQUIRE(var1.is_type<point>() == true);
    CHECK(var1.get_value<point>() == point_value);

    REQUIRE(var2.is_valid() == true);
    REQUIRE(var2.is_type<string>() == true);
    CHECK(var2.get_value<std::string>() == text);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to bool", "[variant]")
{
    SECTION("bool to bool")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);

        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);

        var = false;
        CHECK(var.to_bool() == false);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == false);
    }

    SECTION("char to bool")
    {
        variant var = "true";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);
        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);

        var = "fdsfsdf";
        CHECK(var.to_bool() == true);
        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);
    
        var = "false";
        CHECK(var.to_bool() == false);
        var = "false   ";
        CHECK(var.to_bool() == false);
        var = "   false   ";
        CHECK(var.to_bool() == false);
        var = "   FALSE   ";
        CHECK(var.to_bool() == false);
        var = " \n  FALSE\n";
        CHECK(var.to_bool() == false);

        CHECK(var.convert<bool>() == false);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == false);
    }

    SECTION("std::string to bool")
    {
        variant var = std::string("true");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);

        var = std::string("fdsfsdf");
        CHECK(var.to_bool() == true);
    
        var = std::string("false");
        CHECK(var.to_bool() == false);
        var = std::string("false   ");
        CHECK(var.to_bool() == false);
        var = std::string("   false   ");
        CHECK(var.to_bool() == false);
        var = std::string("   FALSE   ");
        CHECK(var.to_bool() == false);
        var = std::string(" \n  FALSE\n");
        CHECK(var.to_bool() == false);
    }
    
    SECTION("int to bool")
    {
        variant var = 1;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);
        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);

        var = 0;
        CHECK(var.to_bool() == false);
        CHECK(var.convert<bool>() == false);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == false);
    }

    SECTION("float to bool")
    {
        variant var = 1.0f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);
        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);

        var = 0.0f;
        CHECK(var.to_bool() == false);

        var = 1.17149435e-38f;
        CHECK(var.to_bool() == false);
        var = 1.17149435e-37f;
        CHECK(var.to_bool() == true);
        CHECK(var.convert<bool>() == true);
        CHECK(var.convert(type::get<bool>()) == true);
        CHECK(var.get_value<bool>() == true);
    }

    SECTION("double to bool")
    {
        variant var = 1.0;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<bool>() == true);

        CHECK(var.to_bool() == true);

        var = 0.0;
        CHECK(var.to_bool() == false);

        var = 0.5;
        CHECK(var.to_bool() == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to int", "[variant]")
{
    SECTION("int to int")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);

        CHECK(var.to_int() == 12);
        CHECK(var.convert<int>() == 12);
        CHECK(var.convert(type::get<int>()) == true);
        CHECK(var.get_value<int>() == 12);

        var = -23;
        CHECK(var.to_int() == -23);
        CHECK(var.convert<int>() == -23);
        CHECK(var.convert(type::get<int>()) == true);
        CHECK(var.get_value<int>() == -23);
    }

    SECTION("char to int")
    {
        variant var = "23";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);
        CHECK(var.can_convert(type::get<int>()) == true);

        CHECK(var.to_int() == 23);
        CHECK(var.convert<int>() == 23);
        CHECK(var.convert(type::get<int>()) == true);
        CHECK(var.get_value<int>() == 23);

        var = "-12";
        CHECK(var.to_int() == -12);
    
        var = "text 34 and text";
        bool ok = false;
        CHECK(var.to_int(&ok) == 0);
        CHECK(ok == false);

        var = "34 and text";
        ok = false;
        CHECK(var.to_int(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert<int>() == 0);
        CHECK(var.convert(type::get<int>()) == false);
    }

    SECTION("std::string to int")
    {
        variant var = std::string("23");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);

        CHECK(var.to_int() == 23);
        CHECK(var.convert<int>() == 23);
        CHECK(var.convert(type::get<int>()) == true);
        CHECK(var.get_value<int>() == 23);

        var = std::string("-12");
        CHECK(var.to_int() == -12);
    
        var = std::string("text 34 and text");
        bool ok = false;
        CHECK(var.to_int(&ok) == 0);
        CHECK(ok == false);

        var = std::string("34 and text");
        ok = false;
        CHECK(var.to_int(&ok) == 0);
        CHECK(ok == false);
    }
    
    SECTION("bool to int")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);

        CHECK(var.to_int() == 1);

        var = false;
        CHECK(var.to_int() == 0);
    }

    SECTION("float to int")
    {
        variant var = 1.5f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);

        CHECK(var.to_int() == 1);

        var = 3.1423f;
        CHECK(var.to_int() == 3);

        var = 0.0f;
        CHECK(var.to_int() == 0);
    }

    SECTION("double to int")
    {
        variant var = 1.5;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<int>() == true);

        CHECK(var.to_int() == 1);

        var = 3.1423f;
        CHECK(var.to_int() == 3);

        var = 0.0;
        CHECK(var.to_int() == 0);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to std::string", "[variant]")
{
    SECTION("int to std::string")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "12");
        CHECK(var.convert<std::string>() == "12");
        CHECK(var.convert(type::get<std::string>()) == true);
        CHECK(var.get_value<std::string>() == "12");

        var = -23;
        CHECK(var.to_string() == "-23");
    }

    SECTION("char to std::string")
    {
        variant var = "text";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "text");

        var = "text 42";
        CHECK(var.to_string() == "text 42");
    }

    SECTION("std::string to std::string")
    {
        variant var = std::string("23");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "23");

        var = std::string("-12");
        CHECK(var.to_string() == "-12");
    }
    
    SECTION("bool to std::string")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "true");

        var = false;
        CHECK(var.to_string() == "false");
    }

    SECTION("float to std::string")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "1.567");

        var = 3.12345678f;
        CHECK(var.to_string() == "3.12346");

        var = 0.0f;
        CHECK(var.to_string() == "0");
    }

    SECTION("double to std::string")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<std::string>() == true);

        CHECK(var.to_string() == "1.567");

        var = 3.12345678;
        CHECK(var.to_string() == "3.12345678");

        var = 0.0;
        CHECK(var.to_string() == "0");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to float", "[variant]")
{
    SECTION("int to float")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(var.to_float() == 12.0f);
        CHECK(var.convert<float>() == 12.0f);
        CHECK(var.convert(type::get<float>()) == true);
        CHECK(var.get_value<float>() == 12.0f);

        var = -23;
        CHECK(var.to_float() == -23.0f);
    }

    SECTION("char to float")
    {
        variant var = "23.0";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(var.to_float() == 23.0f);
        CHECK(var.convert<float>() == 23.0f);
        CHECK(var.convert(type::get<float>()) == true);
        CHECK(var.get_value<float>() == 23.0f);

        var = "text 42";
        bool ok = false;
        CHECK(var.to_float(&ok) == 0);
        CHECK(ok == false);

        var = "1.23456";
        ok = false;
        CHECK(var.to_float(&ok) == 1.23456f);
        CHECK(ok == true);

        var = "1.23456 Text";
        ok = false;
        CHECK(var.to_float(&ok) == 0);
        CHECK(ok == false);

    }

    SECTION("std::string to float")
    {
        variant var = std::string("23.0");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(var.to_float() == 23.0f);

        var = std::string("text 42");
        bool ok = false;
        CHECK(var.to_float(&ok) == 0);
        CHECK(ok == false);

        var = std::string("1.23456");
        ok = false;
        CHECK(var.to_float(&ok) == 1.23456f);
        CHECK(ok == true);

        var = std::string("1.23456 Text");
        ok = false;
        CHECK(var.to_float(&ok) == 0);
        CHECK(ok == false);
    }
    
    SECTION("bool to float")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(var.to_float() == 1.0f);

        var = false;
        CHECK(var.to_float() == 0.0f);
    }

    SECTION("float to float")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(almost_equal(var.to_float(), 1.567f) == true);

        var = 3.12345678f;
        CHECK(almost_equal(var.to_float(), 3.1234567f) == true);

        var = 0.0f;
        CHECK(almost_equal(var.to_float(), 0.0f) == true);
    }

    SECTION("double to float")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<float>() == true);

        CHECK(almost_equal(var.to_float(), 1.567f) == true);

        var = 3.12345678;
        CHECK(almost_equal(var.to_float(), 3.1234567f) == true);

        var = 0.0;
        CHECK(almost_equal(var.to_float(), 0.0f) == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant conversion - to double", "[variant]")
{
    SECTION("int to double")
    {
        variant var = 12;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        CHECK(var.to_double() == 12.0);
        CHECK(var.convert<double>() == 12.0);
        CHECK(var.convert(type::get<double>()) == true);
        CHECK(var.get_value<double>() == 12.0);

        var = -23;
        CHECK(var.to_double() == -23.0);
    }

    SECTION("char to double")
    {
        variant var = "23.0";
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        CHECK(var.to_double() == 23.0);

        var = "text 42";
        bool ok = false;
        CHECK(var.to_double(&ok) == 0);
        CHECK(ok == false);

        var = "1.23456";
        ok = false;
        CHECK(var.to_double(&ok) == 1.23456);
        CHECK(ok == true);

        var = "1.23456 Text";
        ok = false;
        CHECK(var.to_double(&ok) == 0.0);
        CHECK(ok == false);
    }

    SECTION("std::string to double")
    {
        variant var = std::string("23.0");
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        REQUIRE(var.to_double() == 23.0);

        var = std::string("text 42");
        bool ok = false;
        CHECK(var.to_double(&ok) == 0.0);
        CHECK(ok == false);

        var = std::string("1.23456");
        ok = false;
        CHECK(var.to_double(&ok) == 1.23456);
        CHECK(ok == true);

        var = std::string("1.23456 Text");
        ok = false;
        CHECK(var.to_double(&ok) == 0.0);
        CHECK(ok == false);
    }
    
    SECTION("bool to double")
    {
        variant var = true;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        CHECK(var.to_double() == 1.0);

        var = false;
        CHECK(var.to_double() == 0.0);
    }

    SECTION("float to double")
    {
        variant var = 1.567f;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        CHECK(almost_equal(var.to_double(), 1.5670000314712524) == true);

        var = 3.123456f;
        CHECK(almost_equal(var.to_double(), 3.1234560012817383) == true);

        var = 0.0f;
        CHECK(almost_equal(var.to_double(), 0.0) == true);
    }

    SECTION("double to double")
    {
        variant var = 1.567;
        REQUIRE(var.is_valid() == true);
        REQUIRE(var.can_convert<double>() == true);

        CHECK(almost_equal(var.to_double(), 1.567) == true);

        var = 3.12345678;
        CHECK(almost_equal(var.to_double(), 3.12345678) == true);

        var = 0.0;
        CHECK(almost_equal(var.to_double(), 0.0) == true);
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

    CHECK(var.convert(type::get<std::string>())   == true);
    REQUIRE(var.is_type<std::string>()              == true);
    CHECK(var.get_value<std::string>()            == "12, 34");

    // convert to other custom type
    var = point(12, 34);
    bool ret = var.convert(type::get<vector2d>());
    CHECK(ret == true);
    CHECK(var.is_type<vector2d>() == true);
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
    REQUIRE(var.can_convert(type::get<derived**>()) == false);
    REQUIRE(var.can_convert(type::get<other_derived*>())  == false);
        
    bool could_convert = var.convert(type::get<derived**>());
    CHECK(could_convert == false);

    could_convert = var.convert(type::get<other_derived*>());
    CHECK(could_convert == false);

    could_convert = var.convert(type::get<derived*>());
    CHECK(could_convert == true);
    CHECK(var.is_type<derived*>() == true);
    CHECK(var.get_value<derived*>() == d);
    CHECK(var.convert<base*>() == b);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant test - array", "[variant]")
{
   SECTION("test raw array")
    {
        int obj[2][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        variant_array int_array = &obj;
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

        variant_array vec_array = var;
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
        variant_array vec_array = var;
        // pointer to const array cannot be changed
        REQUIRE(vec_array.set_size(0)   == false);
    }

    SECTION("check if vector<bool>() is supported in variant")
    {
        auto orig_vec = std::vector<bool>({true, false, false});
        variant var = &orig_vec;
        REQUIRE(*var.get_value<std::vector<bool>*>() == orig_vec);

        variant_array var_arr = var;
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
        var_arr = var;
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
        variant_array vec_array = var;
        REQUIRE(vec_array.is_valid()    == true);
       
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);
        CHECK(vec.size()                == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    }

    SECTION("check moved array type")
    {
        std::vector<int> vec(50, 0);
        variant_array vec_array = std::move(vec);
        REQUIRE(vec_array.is_valid()    == true);
        REQUIRE(vec.size()              == 0);
       
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    }

    SECTION("check std::shared_ptr wrapper array type")
    {
        variant_array vec_array;
        std::weak_ptr<std::vector<int>> weak_ptr_obj;
        // we want to check if the shared_ptr is internally hold by the variant_array (makes a copy)
        {
            std::shared_ptr<std::vector<int>> obj = std::make_shared<std::vector<int>>(50, 0);
            weak_ptr_obj = obj;
            vec_array = obj;
            REQUIRE(vec_array.is_valid()    == true);
            CHECK(vec_array.get_size()      == 50);
            // obj will be destroyed, but not the ptr
        }
        REQUIRE(weak_ptr_obj.expired()  == false);
        CHECK(vec_array.is_valid()    == true);
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        CHECK(ret.is_type<int>()      == true);
        CHECK(ret.get_value<int>()    == 23);
    }

    SECTION("check moved wrapper array type")
    {
        variant_array vec_array = std::make_shared<std::vector<int>>(50, 0);
        REQUIRE(vec_array.is_valid()    == true);
        CHECK(vec_array.get_size()      == 50);
       
        vec_array.set_size(70);
        CHECK(vec_array.get_size()      == 70);

        vec_array.set_value(50, 23);
        variant ret = vec_array.get_value(50);
        REQUIRE(ret.is_type<int>()      == true);
        REQUIRE(ret.get_value<int>()    == 23);
    }

    SECTION("copy variant into variant_array")
    {
        variant var = std::vector<int>(100, 0);
        variant_array array = var;

        CHECK(var.is_valid() == true);
        CHECK(array.get_size() == 100);
        CHECK((array.get_type() == type::get<std::vector<int>>()));
    }

    SECTION("move variant into variant_array")
    {
        variant var = std::vector<int>(100, 0);
        variant_array array = std::move(var);

        CHECK(var.is_valid() == false);
        CHECK(array.get_size() == 100);
        CHECK((array.get_type() == type::get<std::vector<int>>()));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant_array test - array", "[variant]")
{
    variant_array array = std::vector<int>(50, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////