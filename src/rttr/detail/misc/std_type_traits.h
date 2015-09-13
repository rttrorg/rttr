/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
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

#ifndef RTTR_STD_TYPE_TRAITS_H_
#define RTTR_STD_TYPE_TRAITS_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <type_traits>

namespace rttr
{

namespace detail
{
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
// workaround because msvc can't handle
// T (*const)(U)
//
// Does not bind to a <T*> or <T*const> partial specialization with VC10 and earlier

template <class T> 
struct remove_pointer_imp
{
   typedef T type;
};

template <class T> 
struct remove_pointer_imp<T*>
{
   typedef T type;
};

template <class T, bool b> 
struct remove_pointer_imp3
{
   typedef typename remove_pointer_imp<typename std::remove_cv<T>::type>::type type;
};

template <class T> 
struct remove_pointer_imp3<T, false>
{
   typedef T type;
};

template <class T> 
struct remove_pointer_imp2
{
   typedef typename remove_pointer_imp3<T, std::is_pointer<T>::value>::type type;
};

template< typename T > 
struct remove_pointer
{ 
    typedef typename remove_pointer_imp2<T>::type type;
};
#   else
#   error "Check new MSVC Compiler!"
#   endif
#else

template<typename T>
using remove_pointer = std::remove_pointer<T>;

#endif

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

 #if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800  

template <typename T, size_t... Bounds> 
struct recombobulator;

template <typename T> 
struct recombobulator<T>
{
    using type = T;
};

template <typename T, size_t N, size_t... Bounds> 
struct recombobulator<T, N, Bounds...> : recombobulator<T[N], Bounds...> 
{ 
};

template <typename T> 
struct remove_const_impl
{
    using type = T;
};

template <typename T> 
struct remove_const_impl<const T> 
{
    using type = T;
};

template <typename T, size_t... Bounds> 
struct discombobulator_const : recombobulator<typename remove_const_impl<T>::type, Bounds...>
{
};

template <typename T, size_t N, size_t... Bounds>
struct discombobulator_const<T[N], Bounds...> : discombobulator_const<T, N, Bounds...>
{
};

template <typename T> 
struct remove_const : discombobulator_const<T> 
{
};

template <typename T> 
struct remove_const<T[]> 
{
    using U = typename remove_const<T>::type;
    using type = U[];
};

#   else
#   error "Check new MSVC Compiler!"
#   endif
#else

template<typename T>
using remove_const = std::remove_const<T>;

#endif

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800  

template <typename T> 
struct remove_volatile_impl
{
    using type = T;
};

template <typename T> 
struct remove_volatile_impl<volatile T> 
{
    using type = T;
};

template <typename T, size_t... Bounds> 
struct discombobulator_volatile : recombobulator<typename remove_volatile_impl<T>::type, Bounds...>
{
};

template <typename T, size_t N, size_t... Bounds>
struct discombobulator_volatile<T[N], Bounds...> : discombobulator_volatile<T, N, Bounds...>
{
};

template <typename T> 
struct remove_volatile : discombobulator_volatile<T> 
{
};

template <typename T> 
struct remove_volatile<T[]> 
{
    using U = typename remove_volatile<T>::type;
    using type = U[];
};

#   else
#   error "Check new MSVC Compiler!"
#   endif
#else

template<typename T>
using remove_volatile = std::remove_volatile<T>;

#endif

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800  

template<class T>
struct remove_cv
{
    typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};

#   else
#   error "Check new MSVC Compiler!"
#   endif
#else

template<typename T>
using remove_cv = std::remove_cv<T>;

#endif

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// shortcut for std::conditional (remark std::conditional_t is only valid for C++14

template< bool B, class T, class F >
using conditional_t = typename std::conditional<B,T,F>::type;
   
} // end namespace detail
} // end namespace rttr

#endif // RTTR_MISC_TYPE_TRAITS_H_
