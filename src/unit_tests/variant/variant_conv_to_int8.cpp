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

TEST_CASE("variant::to_int8() - from empty", "[variant]")
{
    variant var;
    bool ok = false;
    CHECK(var.to_int8(&ok) == 0);
    CHECK(ok == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from bool", "[variant]")
{
    variant var = true;
    REQUIRE(var.is_valid() == true);
    REQUIRE(var.can_convert<int8_t>() == true);

    // true case
    bool ok = false;
    CHECK(var.to_int8(&ok) == 1);
    CHECK(ok == true);

    CHECK(var.convert<int8_t>(&ok) == 1);
    CHECK(ok == true);
    CHECK(var.convert(type::get<int8_t>()) == true);
    CHECK(var.get_value<int8_t>() == 1);

    // false case
    var = false;
    CHECK(var.to_int8(&ok) == 0);
    CHECK(ok == true);

    CHECK(var.convert<int8_t>(&ok) == 0);
    CHECK(ok == true);
    CHECK(var.convert(type::get<int8_t>()) == true);
    CHECK(var.get_value<int8_t>() == 0);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from std::string", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = std::string("23");
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 23);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == 23);
    }

    SECTION("valid conversion negative")
    {
        variant var = std::string("-12");
        CHECK(var.to_int8() == -12);
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = std::string("512");
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = std::string("-512");
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("invalid conversion")
    {
        variant var = std::string("text 34 and text");
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);

        var = std::string("34 and text");
        ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from int", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = 50;
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == 50);
    }

    SECTION("valid conversion negative")
    {
        variant var = -60;
        CHECK(var.to_int8() == -60);
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = 512;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = -512;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from float", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = 1.5f;
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 1);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == 1);
    }

    SECTION("valid conversion negative")
    {
        variant var = -21.9f;
        CHECK(var.to_int8() == -21);
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = 512.2f;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = -512.2f;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from double", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = 1.5;
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 1);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == 1);
    }

    SECTION("valid conversion negative")
    {
        variant var = -21.9;
        CHECK(var.to_int8() == -21);
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = 512.2;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = -512.2;
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from int8_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = int8_t(50);
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == 50);
    }

    SECTION("valid conversion negative")
    {
        variant var = int8_t(-60);
        CHECK(var.to_int8() == int8_t(-60));
        CHECK(var.convert(type::get<int8_t>()) == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from int16_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = int16_t(50);
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == int16_t(50));
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == int16_t(50));
    }

    SECTION("valid conversion negative")
    {
        variant var = int16_t(-60);
        CHECK(var.to_int8() == int16_t(-60));
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = int16_t(512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = int16_t(-512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from int32_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = int32_t(50);
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == int32_t(50));
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == int32_t(50));
    }

    SECTION("valid conversion negative")
    {
        variant var = int32_t(-60);
        CHECK(var.to_int8() == int32_t(-60));
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = int32_t(512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = int32_t(-512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from int64_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = int64_t(50);
        REQUIRE(var.can_convert<int8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == int64_t(50));
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<int8_t>()) == true);
        CHECK(var.get_value<int8_t>() == int64_t(50));
    }

    SECTION("valid conversion negative")
    {
        variant var = int64_t(-60);
        CHECK(var.to_int8() == int64_t(-60));
        CHECK(var.convert(type::get<int8_t>()) == true);
    }

    SECTION("too big")
    {
        variant var = int64_t(512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }

    SECTION("too small")
    {
        variant var = int64_t(-512);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from uint8_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = uint8_t(50);
        REQUIRE(var.can_convert<uint8_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<uint8_t>()) == true);
        CHECK(var.get_value<uint8_t>() == 50);
    }

    SECTION("too big")
    {
        variant var = uint8_t(200);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from uint16_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = uint16_t(50);
        REQUIRE(var.can_convert<uint16_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<uint16_t>()) == true);
        CHECK(var.get_value<uint16_t>() == 50);
    }

    SECTION("too big")
    {
        auto e = std::numeric_limits<int8_t>::max();
        variant var = uint16_t(1000);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from uint32_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = uint32_t(50);
        REQUIRE(var.can_convert<uint32_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<uint32_t>()) == true);
        CHECK(var.get_value<uint32_t>() == 50);
    }

    SECTION("too big")
    {
        auto e = std::numeric_limits<int8_t>::max();
        variant var = uint32_t(1000);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from uint64_t", "[variant]")
{
    SECTION("valid conversion positive")
    {
        variant var = uint64_t(50);
        REQUIRE(var.can_convert<uint64_t>() == true);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 50);
        CHECK(ok == true);

        CHECK(ok == true);
        CHECK(var.convert(type::get<uint64_t>()) == true);
        CHECK(var.get_value<uint64_t>() == 50);
    }

    SECTION("too big")
    {
        auto e = std::numeric_limits<int8_t>::max();
        variant var = uint64_t(1000);
        bool ok = false;
        CHECK(var.to_int8(&ok) == 0);
        CHECK(ok == false);
        CHECK(var.convert(type::get<int8_t>()) == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
