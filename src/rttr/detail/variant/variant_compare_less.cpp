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

#include "rttr/detail/variant/variant_compare_less.h"
#include "rttr/type.h"
#include "rttr/variant.h"

#include <type_traits>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_compare_less(const variant& lhs, const type& lhs_type, const variant& rhs, const type& rhs_type)
{
    if (lhs_type.is_arithmetic() && rhs_type.is_arithmetic())
    {
        if (is_floating_point(lhs_type) || is_floating_point(rhs_type))
            return (lhs.to_double() < rhs.to_double());
        else
            return (lhs.to_int64() < rhs.to_int64());
    }
    else
    {
        variant lhs_tmp;
        if (lhs.convert(rhs_type, lhs_tmp))
            return lhs_tmp.compare_less(rhs);
    
        if (!lhs.is_nullptr() && rhs.is_nullptr())
            return false;
    
        // as last try, do a string conversion
        bool ok1 = false;
        bool ok2 = false;
        auto ret = (lhs.to_string(&ok1) < rhs.to_string(&ok2));
        if (ok1 && ok2)
            return ret;
        else
            return (lhs_type < rhs_type);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
