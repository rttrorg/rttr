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

#ifndef __RTTR_ACCESSOR_TYPE_H__
#define __RTTR_ACCESSOR_TYPE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/function_traits.h"
#include "rttr/detail/misc_type_traits.h"

#include <type_traits>

namespace rttr
{
namespace detail
{
    /////////////////////////////////////////////////////////////////////////////////////

    struct member_func_ptr
    {
        typedef member_func_ptr type;
    };

    struct function_ptr
    {
        typedef function_ptr type;
    };

    struct member_object_ptr
    {
        typedef member_object_ptr type;
    };

    struct object_ptr
    {
        typedef object_ptr type;
    };

    template<typename T>
    struct property_type : std::conditional<std::is_member_function_pointer<T>::value,
                                            member_func_ptr,
                                            typename std::conditional<std::is_member_object_pointer<T>::value,
                                                                     member_object_ptr,
                                                                     typename std::conditional<is_function_ptr<T>::value || is_std_function<T>::value,
                                                                                               function_ptr, 
                                                                                               typename std::conditional<std::is_pointer<T>::value,
                                                                                                                         object_ptr,
                                                                                                                         void
                                                                                                                        >::type                        
                                                                                                >::type
                                                                    >::type
                                            >::type
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    
    struct void_member_func
    {
        typedef void_member_func type;
    };

    struct return_member_func
    {
        typedef return_member_func type;
    };

    struct void_func
    {
        typedef void_func type;
    };

    struct return_func
    {
        typedef return_func type;
    };
 
    template<typename T>
    struct method_type : std::conditional<std::is_member_function_pointer<T>::value,
                                          typename std::conditional<is_void_func<T>::value,
                                                                    void_member_func,
                                                                    return_member_func
                                                                   >::type,
                                          typename std::conditional<is_function_ptr<T>::value || is_std_function<T>::value,
                                                                    typename std::conditional<is_void_func<T>::value,
                                                                                              void_func,
                                                                                              return_func
                                                                                              >::type,
                                                                    void
                                                                   >::type
                                         >::type
    {
    };

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_ACCESSOR_TYPE_H__
