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

#include "test_classes.h"

#include <iostream>

extern void start_rttr_cast_benchmark();

int main(int argc, char** argv)
{
    start_rttr_cast_benchmark();
    return 0;
}













































































































#if 0

void string_func(std::string& text)
{
    std::cout << text << std::endl;
    text += " world";
}

void func(variant2& var)
{
    if (var.is_type<std::string&>())
        string_func(var.get_value<std::string&>());
}

using namespace rttr;

struct Test
{
    Test() : value(23)
    {

    }
    Test(const Test& other)
    {
        std::cout << "copy ctor" << std::endl;
    }

    Test(Test&& other)
    {
        std::cout << "move ctor" << std::endl;
    }

    int value;
};

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(Test)
RTTR_DEFINE_STANDARD_TYPE_VARIANTS(Test)

RTTR_DECLARE_TYPE(char[6])
RTTR_DEFINE_TYPE(char[6])
//RTTR_DEFINE_STANDARD_TYPE_VARIANTS(char[6])















class DataContainer
{
public:
    DataContainer() : m_data(nullptr), m_typeInfo(rttr::type::get<void>()) {}

    DataContainer(const DataContainer& other) : m_data(other.m_data), m_typeInfo(other.m_typeInfo) {}
    DataContainer& operator=(const DataContainer& other)
    {
        m_data = other.m_data;
        m_typeInfo = other.m_typeInfo;
        return *this;
    }

    template<typename T>
    DataContainer(T&& value) 
    :   m_data(reinterpret_cast<void*>(const_cast< typename std::add_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type >(std::addressof(value)))),
        m_typeInfo(rttr::type::get<typename std::decay<T>::type>())
    {

    }

    template<typename T>
    bool is_type() const { return rttr::type::get<typename std::decay<T>::type>() == m_typeInfo; }

    template<typename T>
    T getType(typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr) const
    {
        T value = reinterpret_cast<T>(m_data);
        return (value);
    }

    template<typename T>
    T getType(typename std::enable_if<!std::is_pointer<T>::value >::type* = nullptr) const
    {
        typedef typename std::remove_reference<T>::type PtrType;
        PtrType* value = reinterpret_cast<PtrType*>(m_data);
        return (*value);
    }

private:
    void* m_data;
     rttr::type m_typeInfo;
};

template<typename T>
struct Hello
{

};

template<>
struct Hello<char>
{};

template<>
struct Hello<char[6]>
{};

void foofunc(char hello[6])
{

}


void myRealFunc(std::string& name)
{
    std::cout << name << std::endl;
    name += "additional";
}

void myRealPointerFunc(Test* arg)
{
    

    arg->value = 12;
}

void callFunc(const DataContainer& arg)
{
    if (arg.is_type<std::string&>())
        myRealFunc(arg.getType<std::string&>());
}

void callPointerFunc(const DataContainer& arg)
{
    if (arg.is_type<Test*>())
        myRealPointerFunc(arg.getType<Test*>());
}

std::string getValue(int foo)
{
    if (foo == 12)
        return std::string("Hello");
    else
        return std::string("NO");
}
/*
DataContainer getter(const DataContainer& arg)
{
    if (arg.isOfType<int>())
        return getValue(arg.getType<int>());
    return
        DataContainer();
}
*/


TEST_CASE("ReflectionTest", "[Basic]")
{
    int foo = 12;
#if 0
    DataContainer er;
    {
        std::string t = "HELLO WOLRD";
        DataContainer foo(std::move(t));
    }

    callFunc(std::string("hello"));
#endif
    //REQUIRE(Factorial(1) == 1);
    //REQUIRE(Factorial(2) == 2);
    //REQUIRE(Factorial(3) == 6);
    //REQUIRE(Factorial(10) == 3628800);
}

#endif


