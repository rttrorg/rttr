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

#ifndef RTTR_GET_CREATE_VARIANT_FUNC_H_
#define RTTR_GET_CREATE_VARIANT_FUNC_H_

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/detail/misc_type_traits.h"
#include "rttr/detail/argument.h"
#include <type_traits>

namespace rttr
{

namespace detail
{

/*!
 * \brief The following code is used for the function bool variant::convert(const type& target_type).
 *
 * With the create_variant function it is possible to perform a rttr_cast internally in variant.
 * So basically a conversion of source_type* to target_type*.
 *
 * Template arguments cannot be forwarded at runtime to some derived or base classes.
 */

template<typename T>
variant create_invalid_variant(const argument& data)
{
    return variant();
}

template<typename T>
variant create_variant(const argument& data)
{
    return data.get_value<T>();
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

typedef variant(*variant_create_func)(const argument&);

template<typename Source_Type, typename Enable = void>
struct variant_creater
{
    static variant_create_func create()
    {
        return create_variant<Source_Type>;
    }
};

template<typename Source_Type>
struct variant_creater<Source_Type, typename std::enable_if<std::is_function<Source_Type>::value>::type>
{
    static variant_create_func create()
    {
        return create_invalid_variant<Source_Type>;
    }
};

} // end namespace detail

} // end namespace rttr

#endif // RTTR_GET_CREATE_VARIANT_FUNC_H_
