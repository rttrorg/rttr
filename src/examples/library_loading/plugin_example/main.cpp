/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
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

#include <cstdio>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <iostream>

#include <rttr/registration>

class MyPlugin
{
public:
    MyPlugin()
    {}

    int some_method()
    {
        return 42;
    }

    void some_foo()
    {
        std::cout << "int";
    }
    int value;
    bool flag;

};

class MyPlugin2
{
public:
    MyPlugin2()
    {}

    int some_method()
    {
        return 42;
    }

    void some_foo()
    {
        std::cout << "int";
    }
    int value;
    bool flag;

    RTTR_ENABLE()

};

RTTR_REGISTRATION
{
   rttr::registration::class_<MyPlugin>("MyPlugin")
   .constructor<>()
   .property("value", &MyPlugin::value)
    .property("flag", &MyPlugin::flag)
    .method("some_method", &MyPlugin::some_method)
    //.method("some_foo", &MyPlugin::some_foo)
    //.method("some_foo3", &MyPlugin::some_foo)
    ;
#if 0

    MyPlugin foo;
    rttr::type::get<MyPlugin>();

    rttr::type::get<MyPlugin*>();

    rttr::type::get<MyPlugin2>();
    rttr::type::get<MyPlugin2*>();
    rttr::type::get<MyPlugin2**>();
    rttr::type::get<MyPlugin2***>();

   //new rttr::detail::property_wrapper<rttr::detail::member_object_ptr, int(MyPlugin::*), void, rttr::access_levels::public_access, rttr::detail::return_as_copy, rttr::detail::set_value, 0>("foo", rttr::type::get<MyPlugin>(), &MyPlugin::value, {});
   rttr::variant var = 2;
  // rttr::variant var5 = 5;
  // rttr::variant varf5 = std::string("asds");
#endif
}

