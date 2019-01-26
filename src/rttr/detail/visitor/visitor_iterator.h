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

#ifndef RTTR_VISITOR_ITAERATOR_H_
#define RTTR_VISITOR_ITAERATOR_H_

#include "rttr/visitor.h"

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

 template<typename... T>
struct RTTR_LOCAL visitor_iterator;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Args>
struct RTTR_LOCAL visitor_iterator<T, Args...>
{
    template<typename Invoker>
    static void visit(visitor& visitor, const Invoker& invoker)
    {
        if (auto obj = rttr_cast<T*>(&visitor))
        {
            invoker. template call<T>(*obj);
        }
        else
        {
            visitor_iterator<Args...>::visit(visitor, invoker);
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<template <class...> class List, typename ...Args>
struct visitor_iterator<List<Args...>> : visitor_iterator<Args...> { };

template<>
struct RTTR_LOCAL visitor_iterator<>
{
    template<typename Invoker>
    static void visit(visitor& visitor, const Invoker& invoker)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr


#endif // RTTR_VISITOR_ITAERATOR_H_
