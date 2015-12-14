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

#include <rttr/registration>

using namespace rttr;

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from empty", "[variant]")
{
    variant var;
    CHECK(var.to_bool() == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from bool", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from char", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from std::string", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from int", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from float", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from double", "[variant]")
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

/////////////////////////////////////////////////////////////////////////////////////////
