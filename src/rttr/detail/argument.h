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

#ifndef __RTTR_ARGUMENT_H__
#define __RTTR_ARGUMENT_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/misc_type_traits.h"

#include <type_traits>
#include <utility>

namespace rttr
{
class type;
class variant;
class variant_array;

namespace detail
{
class instance;

/*!
 * This class is used for forwarding the arguments to the function calls.
 *
 * \remark You should never explicit instantiate this class by yourself.
 */
class argument
{
public:
    argument();

    argument(argument&& arg);
    argument(const argument& other);
    argument(variant& var);
    argument(const variant& var);
    argument(variant_array& var);
    argument(const variant_array& var);

    template<typename T>
    argument(const T& data, typename std::enable_if<!std::is_same<argument, T>::value >::type* = 0);

    template<typename T>
    argument(T& data, typename std::enable_if<!std::is_same<argument, T>::value >::type* = 0);

    template<typename T>
    bool is_type() const;
    type get_type() const;
    void* get_ptr() const;

    template<typename T>
    T& get_value() const;

    argument& operator=(const argument& other);

private:
    const void*         _data;
    const rttr::type    _type;
};

} // end namespace detail
} // end namespace rttr

#include "rttr/detail/argument_impl.h"

#endif // __RTTR_ARGUMENT_H__
