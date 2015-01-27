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

#include "rttr/type.h"
#include "rttr/detail/misc_type_traits.h"

#include <type_traits>

namespace rttr
{

template<typename TargetType, typename SourceType>
RTTR_INLINE TargetType rttr_cast(SourceType object)
{
    static_assert(detail::pointer_count<TargetType>::value == 1, "Return type must be a pointer");
    static_assert(detail::pointer_count<TargetType>::value == 1, "Argument type must be a pointer");
    static_assert(rttr::detail::has_get_type_func<SourceType>::value, "Class has not type defined - please use the macro RTTR_ENABLE.");

    typedef typename std::remove_pointer<TargetType>::type ReturnType;
    typedef typename std::remove_pointer<SourceType>::type ArgType;
    static_assert( (std::is_const<ArgType>::value && std::is_const<ReturnType>::value) ||
                   (!std::is_const<ArgType>::value && std::is_const<ReturnType>::value) ||
                   (!std::is_const<ArgType>::value && !std::is_const<ReturnType>::value), "Return type must have const qualifier");
   
    return static_cast<TargetType>(type::apply_offset(object->get_ptr(), object->get_type(), type::get<TargetType>()));
}

}
