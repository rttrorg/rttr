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

#include <rttr/registration>
#include <catch/catch.hpp>

using namespace rttr;

struct ctor_default_arg_test
{
    ctor_default_arg_test(int value1, bool value2, const std::string& text)
    { 
    }

    ctor_default_arg_test(int value1)
    { 
    }

    static std::shared_ptr<ctor_default_arg_test> create_object(int value1, bool value2, const std::string& value3) 
    { 
        if (value1 == 23 && value2 == true && value3 == std::string("default text"))
            return std::make_shared<ctor_default_arg_test>(value1, value2, value3); 
        else
            return std::shared_ptr<ctor_default_arg_test>();
    }
};

RTTR_REGISTRATION
{
    registration::class_<ctor_default_arg_test>("ctor_default_arg_test")
        .constructor<int, bool, const std::string&>()
        (
            default_arguments(true, std::string("default text")),
            policy::ctor::as_std_shared_ptr
        )
        .constructor(&ctor_default_arg_test::create_object)
        (
            default_arguments(true, std::string("default text"))
        )
        .constructor<int>()
        (
            default_arguments(23),
            policy::ctor::as_std_shared_ptr
        );
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - default argument test (real ctor)", "[constructor]") 
{
    type t = type::get<ctor_default_arg_test>();
    REQUIRE(t.is_valid() == true);

    auto ctor_list = t.get_constructors();
    constructor ctor = ctor_list[0];
    using create_type = std::shared_ptr<ctor_default_arg_test>;
    // too less arguments for invoke
    variant var = ctor.invoke();
    CHECK(var.is_valid() == false);

    // too much arguments
    var = ctor.invoke(23, true, std::string("default text"), 43.0);
    CHECK(var.is_valid() == false);

    // using 2 default arguments
    var = ctor.invoke(23);
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using 1 default arguments
    var = ctor.invoke(23, true);
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using NO default arguments
    var = ctor.invoke(23, true, std::string("default text"));
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - default argument test (func ctor)", "[constructor]") 
{
    type t = type::get<ctor_default_arg_test>();
    REQUIRE(t.is_valid() == true);

    auto ctor_list = t.get_constructors();
    constructor ctor = ctor_list[1];
    using create_type = std::shared_ptr<ctor_default_arg_test>;
    // too less arguments for invoke
    variant var = ctor.invoke();
    CHECK(var.is_valid() == false);

    // too much arguments
    var = ctor.invoke(23, true, std::string("default text"), 43.0);
    CHECK(var.is_valid() == false);

    // using 2 default arguments
    var = ctor.invoke(23);
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using 1 default arguments
    var = ctor.invoke(23, true);
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using NO default arguments
    var = ctor.invoke(23, true, std::string("default text"));
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - default argument test (invoke variadic; real ctor)", "[constructor]") 
{
    type t = type::get<ctor_default_arg_test>();
    REQUIRE(t.is_valid() == true);

    auto ctor_list = t.get_constructors();
    constructor ctor = ctor_list[0];
    using create_type = std::shared_ptr<ctor_default_arg_test>;
    // too less arguments for invoke
    variant var = ctor.invoke();
    CHECK(var.is_valid() == false);

    // too much arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    int value1 = 23;
    bool value2 = true;
    std::string value3 = ("default text");
    var = ctor.invoke_variadic({value1, value2, value3, 43.0});
#else
    var = ctor.invoke_variadic({23, true, std::string("default text"), 43.0});
#endif
    CHECK(var.is_valid() == false);

    // using 2 default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1});
#else
    var = ctor.invoke_variadic({23});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using 1 default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1, value2});
#else
    var = ctor.invoke_variadic({23, true});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using NO default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1, value2, value3});
#else
    var = ctor.invoke_variadic({23, true, std::string("default text")});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - default argument test (invoke variadic; func ctor)", "[constructor]") 
{
    type t = type::get<ctor_default_arg_test>();
    REQUIRE(t.is_valid() == true);

    auto ctor_list = t.get_constructors();
    constructor ctor = ctor_list[1];
    using create_type = std::shared_ptr<ctor_default_arg_test>;
    // too less arguments for invoke
    variant var = ctor.invoke();
    CHECK(var.is_valid() == false);

    // too much arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    int value1 = 23;
    bool value2 = true;
    std::string value3 = ("default text");
    var = ctor.invoke_variadic({value1, value2, value3, 43.0});
#else
    var = ctor.invoke_variadic({23, true, std::string("default text"), 43.0});
#endif
    CHECK(var.is_valid() == false);

    // using 2 default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1});
#else
    var = ctor.invoke_variadic({23});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using 1 default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1, value2});
#else
    var = ctor.invoke_variadic({23, true});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);

    // using NO default arguments
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    var = ctor.invoke_variadic({value1, value2, value3});
#else
    var = ctor.invoke_variadic({23, true, std::string("default text")});
#endif
    REQUIRE(var.is_type<create_type>() == true);
    CHECK(var.get_value<create_type>().get() != nullptr);
}

////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("constructor - default argument via type (real ctor)", "[constructor]") 
{
    type t = type::get<ctor_default_arg_test>();
    REQUIRE(t.is_valid() == true);

    CHECK(t.create().is_valid()     == true);
    CHECK(t.create({23}).is_valid() == true); // this will not invoke the same ctor like above, the first registered
    CHECK(t.create({23, false}).is_valid() == true);
    CHECK(t.create({23, false, std::string("text")}).is_valid() == true);
    // too much arguments
    CHECK(t.create({23, false, std::string("text"), 42}).is_valid() == false);
}

////////////////////////////////////////////////////////////////////////////////////////