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

#ifndef __RTTR_BENCH_UTILS_H__
#define __RTTR_BENCH_UTILS_H__

#include <string>
#include <sstream>

/* Convert double to string with specified number of places after the decimal. */
std::string prd(const double x, const int decDigits)
{
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(decDigits); // set # places after decimal
    ss << x;
    return ss.str();
}

std::string prd(const double x, const int decDigits, const int width)
{
    std::stringstream ss;
    ss << std::fixed << std::right;
    ss.fill(' ');        // fill space around displayed #
    ss.width(width);     // set  width around displayed #
    ss.precision(decDigits); // set # places after decimal
    ss << x;
    return ss.str();
}

/*! Center-aligns string within a field of width w. Pads with blank spaces
    to enforce alignment. */
std::string center(const std::string& s, const std::size_t w)
{
    std::stringstream ss, spaces;
    const std::size_t padding = (s.size() <= w) ? w - s.size() : s.size();   // count excess room to pad
    for(std::size_t i = 0; i < padding / 2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

/* Right-aligns string within a field of width w. Pads with blank spaces
   to enforce alignment. */
std::string right(const std::string& s, const std::size_t w) 
{
    std::stringstream ss, spaces;
    const std::size_t padding = (s.size() <= w) ? w - s.size() : s.size();   // count excess room to pad
    for(std::size_t i=0; i<padding; ++i)
        spaces << " ";
    ss << spaces.str() << s;                    // format with padding
    return ss.str();
}

/*! Left-aligns string within a field of width w. Pads with blank spaces
    to enforce alignment. */
std::string left(const std::string& s, const std::size_t w) 
{
    std::stringstream ss, spaces;
    const std::size_t padding = (s.size() <= w) ? w - s.size() : s.size();   // count excess room to pad
    for(std::size_t i=0; i<padding; ++i)
        spaces << " ";
    ss << s << spaces.str();                    // format with padding
    return ss.str();
}


#endif // __RTTR_BENCH_UTILS_H__
