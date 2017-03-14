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

#include <rttr/registration>

#include <vector>
#include <map>
#include <list>
#include <set>

// explicit instantiation of std::string needed, otherwise we get a linker error with clang on osx
// thats a bug in libc++, because of interaction with __attribute__ ((__visibility__("hidden"), __always_inline__)) in std::string
template class std::basic_string<char>;

RTTR_REGISTRATION
{
    using namespace rttr;

    type::get<std::nullptr_t>();

    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(void)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(rttr::type)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(bool)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(signed char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(wchar_t)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(short int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned short int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned long long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(float)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(double)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long double)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(std::string)
    type::get<std::vector<bool>>();
    type::get<std::vector<int>>();
    type::get<std::vector<float>>();
    type::get<std::vector<double>>();

    registration::class_<std::string>("std::string")
                .constructor<>()
                .constructor<const std::string&>()
                .constructor<const std::string&, unsigned int, unsigned int>()
                .constructor<const char*>()
                .constructor<const char*, unsigned int>()
                .constructor<unsigned int, char>()
                .method("length",       &std::string::length)
                .method("size",         &std::string::size)
                .method("empty",        &std::string::empty)
#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_ARCH_TYPE == RTTR_ARCH_32
                .method("at",           static_cast<char&(std::string::*)(std::size_t)>(&std::string::at))
                .method("at",           static_cast<const char&(std::string::*)(std::size_t) const>(&std::string::at))
                .method("operator[]",   static_cast<char&(std::string::*)(std::size_t)>(&std::string::operator[]))
                .method("operator[]",   static_cast<const char&(std::string::*)(std::size_t) const>(&std::string::operator[]))
#else
                .method("at",           rttr::select_const(&std::string::at))
                .method("at",           rttr::select_non_const(&std::string::at))
                .method("operator[]",   rttr::select_overload<char&(size_t)>(&std::string::operator[]))
                .method("operator[]",   rttr::select_non_const(&std::string::operator[]))
#endif
                .method("data",         &std::string::data)
                .method("c_str",        &std::string::c_str);
}
