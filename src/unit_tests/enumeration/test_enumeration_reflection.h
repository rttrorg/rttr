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

#ifndef TEST_ENUMERATION_REFLECTION_H_
#define TEST_ENUMERATION_REFLECTION_H_

#include <rttr/type>


struct enum_test
{
    enum E_Alignment
    {
        AlignLeft       = 0x0001,
        AlignRight      = 0x0002,
        AlignHCenter    = 0x0004,
        AlignJustify    = 0x0008
    };

    enum class E_Orientation
    {
        Horizontal      = 0,
        Vertical        = 1
    };

    enum_test() : _alignment(E_Alignment::AlignLeft), _orientation(E_Orientation::Vertical)
    {}

    enum_test(E_Alignment align, E_Orientation orient) : _alignment(align), _orientation(orient)
    {}


    E_Alignment     _alignment;
    E_Orientation   _orientation;
};

enum E_DayOfWeek
{
    Monday      = 0,
    Tuesday     = 1,
    Wednesday   = 2,
    Thursday    = 3,
    Friday      = 4,
    Saturday    = 5,
    Sunday      = 6
};

#endif // TEST_ENUMERATION_REFLECTION_H_
