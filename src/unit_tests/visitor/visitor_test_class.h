/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_VISITOR_TEST_CLASS_H_
#define RTTR_VISITOR_TEST_CLASS_H_

#include <rttr/type>

struct visitor_test_class_base
{
    visitor_test_class_base() {};
    virtual ~visitor_test_class_base(){}

    void base_method() {}

    int base_property;

    RTTR_ENABLE()
};

struct visitor_test_class : visitor_test_class_base
{
    visitor_test_class() {};
    visitor_test_class(int) {};


    static visitor_test_class create_object(int value) { return visitor_test_class(value); };

    void some_method(int) {}

    bool derived_property;
    const float readonly_property = 3;

    void set_value(int value) { m_value = value; }
    int get_value() const { return m_value; }

    RTTR_ENABLE(visitor_test_class_base)

    private:
        int m_value;
};

static std::string some_global_method() { return "42"; }

#endif // RTTR_VISITOR_TEST_CLASS_H_
