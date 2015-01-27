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

struct property_test
{
    property_test() : _p1(0), _p2(12), _p3(true), _p4(23) 
    {
        _array.resize(1000); 
        for (int i = 0; i < 100; ++i)
            _other_array[i] = i;
    }
    virtual ~property_test() {}

    const std::string& get_p7() const { return _p7; }
    void set_p7(const std::string& text) { _p7 = text; }
    int get_prop() const volatile { return 22; }
   
    int                 _p1;
    short int           _p2;
    bool                _p3;
    const double        _p4;
    static int          _p5;
    static const int    _p6;
    std::string         _p7;
    double*             _p8;
    std::vector<int>    _array;
    int                 _other_array[100];

    RTTR_REGISTER_FRIEND;
};

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(property_test)
RTTR_DECLARE_TYPE(int[100])
RTTR_DECLARE_TYPE(int(*)[100])
RTTR_DECLARE_TYPE(const int(*)[100])


/////////////////////////////////////////////////////////////////////////////////////////
// test derived properties

namespace ns_property
{
struct top
{
    virtual ~top() {}
    top() : _p1(12){}
    int _p1;
    RTTR_ENABLE()
};

/////////////////////////////////////////////////////////////////////////////////////////

struct left : virtual top
{

    left() : _p2(true){}
    bool _p2;

    RTTR_ENABLE(top)
};

/////////////////////////////////////////////////////////////////////////////////////////

struct right : virtual top
{

    right() : _p3(true){}
    bool _p3;

    RTTR_ENABLE(top)
};

/////////////////////////////////////////////////////////////////////////////////////////

struct right_2
{
    virtual ~right_2() {}
    right_2() : _p4(true){}
    bool _p4;
    RTTR_ENABLE()
};

/////////////////////////////////////////////////////////////////////////////////////////

struct bottom : left, right, right_2
{
    bottom() : _p5(23.0){}
    double _p5;
    RTTR_ENABLE(left, right, right_2)
};

}

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(ns_property::top)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(ns_property::left)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(ns_property::right)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(ns_property::right_2)
RTTR_DECLARE_STANDARD_TYPE_VARIANTS(ns_property::bottom)

/////////////////////////////////////////////////////////////////////////////////////////
