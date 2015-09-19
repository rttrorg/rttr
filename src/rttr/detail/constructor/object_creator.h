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

#ifndef RTTR_OBJECT_CREATOR_H_
#define RTTR_OBJECT_CREATOR_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/policies/ctor_policies.h"

#include <memory>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Policy>
struct object_creator;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
struct object_creator<Class_Type, as_raw_pointer>
{
    template<typename... Args>
    static RTTR_INLINE Class_Type* create(Args&&... args)
    {
        return (new Class_Type(std::forward<Args>(args)...));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
struct object_creator<Class_Type, as_object>
{
    template<typename... Args>
    static RTTR_INLINE Class_Type create(Args&&... args)
    {
        return Class_Type(std::forward<Args>(args)...);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
struct object_creator<Class_Type, as_std_shared_ptr>
{
    template<typename... Args>
    static RTTR_INLINE std::shared_ptr<Class_Type> create(Args&&... args)
    {
        return (std::make_shared<Class_Type>(std::forward<Args>(args)...));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_OBJECT_CREATOR_H_
