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

#include <rttr/type>
#include <string>

struct method_test
{
    virtual ~method_test() {}
    void method_1()                  { method_1_called = true; }                        // basic simple method
    void method_2() const volatile   { method_2_called = true; }                        // some more qualifiers
    void method_3(int value)         { method_3_called = true; method_3_value = value;} // call method with one parameter
    void method_4(std::string& text) { text = "Text Changed"; method_4_called = true; } // call method with parameter by reference
    int method_5(double* arg)        { method_5_called = true; *arg = 22.0; return 42; }// method with return value
    const std::string& method_6() const { method_6_called = true; return dummy_text; }  // method which return a value by reference
    static int method_7(double value){ method_7_called = true; return 23;}              // static method
    virtual void method_8()          { method_8_called = true; }                        // virtual method
    void method_9(int, int, int, int, bool, int, int, int, int, int) { method_9_called = true; } // method with many arguments
    void method_default_arg(int var = 23) { method_default_arg_called = true; }         // method with default argument
    void method_raw_array(int (&arr)[10]) {  method_raw_array_called = true;  }


    int method_5(int, double)       { method_5_overloaded_called = true; return 42; }   // overloaded method
    
    double dummy_data = 12;
    std::string dummy_text = "Hello World";
    int method_3_value = 0;

    bool method_1_called            = false;
    mutable bool method_2_called    = false;
    bool method_3_called            = false;
    bool method_4_called            = false;
    bool method_5_called            = false;
    mutable bool method_6_called    = false;
    static bool method_7_called;
    bool method_8_called            = false;
    bool method_9_called            = false;
    bool method_5_overloaded_called = false;
    bool method_default_arg_called  = false;
    bool method_raw_array_called    = false;

    RTTR_ENABLE()
};

RTTR_DECLARE_TYPE(int[10])
RTTR_DECLARE_TYPE(int[9])

struct method_test_derived : method_test
{

    virtual void method_8()         { method_8_derived_called = true; }
    void method_10(int value)       { method_10_derived_called = true;} // here we want to check if a base ptr can be converted to the middle

    bool method_8_derived_called    = false;
    bool method_10_derived_called   = false;

    RTTR_ENABLE(method_test)
};

struct method_test_right
{
    virtual ~method_test_right() {}
    void method_12()                { method_12_right_called = true;}

    bool method_12_right_called   = false;
    RTTR_ENABLE()
};

struct method_test_final : method_test_derived, method_test_right
{

    void method_13()                { method_13_final_called = true;}

    bool method_13_final_called    = false;

    RTTR_ENABLE(method_test_derived, method_test_right)
};

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(method_test)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(method_test_derived)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(method_test_right)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(method_test_final)
