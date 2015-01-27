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

#ifndef __RTTR_PROPERTY_CONTAINER_H__
#define __RTTR_PROPERTY_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/function_traits.h"
#include "rttr/detail/property_container_base.h"
#include "rttr/detail/instance.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/accessor_type.h"
#include "rttr/policy.h"
#include "rttr/detail/array_mapper.h"
#include "rttr/detail/utility.h"
#include "rttr/detail/property_accessor.h"

#include <functional>

namespace rttr
{
namespace detail
{

template<typename Accessor_Type, typename Getter, typename Setter, typename Get_Policy, typename Set_Policy>
class property_container;

#include "rttr/detail/property_container_member_func.h"
#include "rttr/detail/property_container_func.h"
#include "rttr/detail/property_container_member_object.h"
#include "rttr/detail/property_container_object.h"

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_PROPERTY_CONTAINER_H__
