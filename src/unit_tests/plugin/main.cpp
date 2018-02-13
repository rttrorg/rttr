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

struct point2D { int x; int y; };

class test_plugin_class
{
public:
    test_plugin_class()
    {}

    int some_method()
    {
        return 42;
    }


    int value;
    std::vector<point2D> point_list = { {1, 2}, {3, 4}, {5, 6} };

};


RTTR_REGISTRATION
{
    rttr::registration::class_<test_plugin_class>("test_plugin_class")
            .constructor<>()
             (
                 rttr::policy::ctor::as_object
             )
            .property("value", &test_plugin_class::value)
            .property("point_list", &test_plugin_class::point_list)
             (
                 rttr::policy::prop::as_reference_wrapper
             )
            .method("some_method", &test_plugin_class::some_method)
            ;

}

