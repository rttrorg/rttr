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

#include <cstdint>

enum class enum_bool : bool
{
    ON = true,
    OFF = false
};

enum class enum_int8 : int8_t
{
    VALUE_1 = 0,
    VALUE_2,
    VALUE_3,
    VALUE_4
};

enum class enum_int : int
{
    VALUE_1 = 1000,
    VALUE_2 = 2000,
    VALUE_3 = 4000,
    VALUE_4 = 8000,
    VALUE_X = 100
};

using namespace rttr;

RTTR_REGISTRATION
{
    rttr::registration::enumeration<enum_bool>("enum_bool")
    (
        value("ON", enum_bool::ON),
        value("OFF", enum_bool::OFF)
    )
    .enumeration<enum_int8>("enum_int8")
    (
        value("VALUE_1", enum_int8::VALUE_1),
        value("VALUE_2", enum_int8::VALUE_2),
        value("VALUE_3", enum_int8::VALUE_3),
        value("VALUE_4", enum_int8::VALUE_4)
    )
    .enumeration<enum_int>("enum_int")
    (
        value("VALUE_1", enum_int::VALUE_1),
        value("VALUE_2", enum_int::VALUE_2),
        value("VALUE_3", enum_int::VALUE_3),
        value("VALUE_4", enum_int::VALUE_4),
        value("VALUE_X", enum_int::VALUE_X)
    )
    ;
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_bool() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_bool()   == true);
        CHECK(variant(enum_bool::OFF).to_bool()  == false);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_bool() == false);
        CHECK(variant(enum_int8::VALUE_2).to_bool() == true);
        CHECK(variant(enum_int8::VALUE_3).to_bool() == false); // conversion failed, value bigger than 1
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_int()   == 1);
        CHECK(variant(enum_bool::OFF).to_int()  == 0);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_int() == 0);
        CHECK(variant(enum_int8::VALUE_2).to_int() == 1);
        CHECK(variant(enum_int8::VALUE_3).to_int() == 2);
        CHECK(variant(enum_int8::VALUE_4).to_int() == 3);
    }

    SECTION("enum_int")
    {
        CHECK(variant(enum_int::VALUE_1).to_int() == 1000);
        CHECK(variant(enum_int::VALUE_2).to_int() == 2000);
        CHECK(variant(enum_int::VALUE_3).to_int() == 4000);
        CHECK(variant(enum_int::VALUE_4).to_int() == 8000);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_float() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_float()   == 1.0f);
        CHECK(variant(enum_bool::OFF).to_float()  == 0.0f);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_float() == 0.0f);
        CHECK(variant(enum_int8::VALUE_2).to_float() == 1.0f);
        CHECK(variant(enum_int8::VALUE_3).to_float() == 2.0f);
        CHECK(variant(enum_int8::VALUE_4).to_float() == 3.0f);
    }

    SECTION("enum_int")
    {
        CHECK(variant(enum_int::VALUE_1).to_float() == 1000.0f);
        CHECK(variant(enum_int::VALUE_2).to_float() == 2000.0f);
        CHECK(variant(enum_int::VALUE_3).to_float() == 4000.0f);
        CHECK(variant(enum_int::VALUE_4).to_float() == 8000.0f);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_double() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_double()   == 1.0);
        CHECK(variant(enum_bool::OFF).to_double()  == 0.0);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_double() == 0.0);
        CHECK(variant(enum_int8::VALUE_2).to_double() == 1.0);
        CHECK(variant(enum_int8::VALUE_3).to_double() == 2.0);
        CHECK(variant(enum_int8::VALUE_4).to_double() == 3.0);
    }

    SECTION("enum_int")
    {
        CHECK(variant(enum_int::VALUE_1).to_double() == 1000.0);
        CHECK(variant(enum_int::VALUE_2).to_double() == 2000.0);
        CHECK(variant(enum_int::VALUE_3).to_double() == 4000.0);
        CHECK(variant(enum_int::VALUE_4).to_double() == 8000.0);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int8() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_int8()   == 1);
        CHECK(variant(enum_bool::OFF).to_int8()  == 0);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_int8() == 0);
        CHECK(variant(enum_int8::VALUE_2).to_int8() == 1);
        CHECK(variant(enum_int8::VALUE_3).to_int8() == 2);
        CHECK(variant(enum_int8::VALUE_4).to_int8() == 3);
    }

    SECTION("enum_int")
    {
        // value too big
        bool ok = false;
        CHECK(variant(enum_int::VALUE_1).to_int8(&ok) == 0);
        CHECK(ok == false);

        // value fits in
        ok = false;
        CHECK(variant(enum_int::VALUE_X).to_int8(&ok) == 100);
        CHECK(ok == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_int16() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_int16()   == 1);
        CHECK(variant(enum_bool::OFF).to_int16()  == 0);
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_int16() == 0);
        CHECK(variant(enum_int8::VALUE_2).to_int16() == 1);
        CHECK(variant(enum_int8::VALUE_3).to_int16() == 2);
        CHECK(variant(enum_int8::VALUE_4).to_int16() == 3);
    }

    SECTION("enum_int")
    {
        bool ok = false;
        CHECK(variant(enum_int::VALUE_1).to_int16(&ok) == 1000);
        CHECK(ok == true);

        ok = false;
        CHECK(variant(enum_int::VALUE_2).to_int16(&ok) == 2000);
        CHECK(ok == true);

        ok = false;
        CHECK(variant(enum_int::VALUE_3).to_int16(&ok) == 4000);
        CHECK(ok == true);

        ok = false;
        CHECK(variant(enum_int::VALUE_4).to_int16(&ok) == 8000);
        CHECK(ok == true);

        ok = false;
        CHECK(variant(enum_int::VALUE_X).to_int16(&ok) == 100);
        CHECK(ok == true);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("variant::to_string() - from enum", "[variant]")
{
    SECTION("enum_bool")
    {
        CHECK(variant(enum_bool::ON).to_string()   == std::string("ON"));
        CHECK(variant(enum_bool::OFF).to_string()  == std::string("OFF"));
    }

    SECTION("enum_int8")
    {
        CHECK(variant(enum_int8::VALUE_1).to_string() == std::string("VALUE_1"));
        CHECK(variant(enum_int8::VALUE_2).to_string() == std::string("VALUE_2"));
        CHECK(variant(enum_int8::VALUE_3).to_string() == std::string("VALUE_3"));
        CHECK(variant(enum_int8::VALUE_4).to_string() == std::string("VALUE_4"));
    }

    SECTION("enum_int")
    {
        CHECK(variant(enum_int::VALUE_1).to_string() == std::string("VALUE_1"));
        CHECK(variant(enum_int::VALUE_2).to_string() == std::string("VALUE_2"));
        CHECK(variant(enum_int::VALUE_3).to_string() == std::string("VALUE_3"));
        CHECK(variant(enum_int::VALUE_4).to_string() == std::string("VALUE_4"));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
