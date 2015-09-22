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

#ifndef RTTR_ARGUMENT_H_
#define RTTR_ARGUMENT_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"

#include <type_traits>
#include <utility>

namespace rttr
{
class type;
class variant;
class variant_array;
class instance;


/*!
 * This class is used for forwarding the arguments to the function calls.
 *
 * \remark You should never explicit instantiate this class by yourself.
 */
class RTTR_API argument
{
public:
    RTTR_INLINE argument();

    RTTR_INLINE argument(argument&& arg);
    RTTR_INLINE argument(const argument& other);
    RTTR_INLINE argument(variant& var);
    RTTR_INLINE argument(const variant& var);

    template<typename T>
    RTTR_INLINE argument(const T& data, typename std::enable_if<!std::is_same<argument, T>::value && !std::is_same<variant_array_view, T>::value >::type* = nullptr);
    template<typename T>
    RTTR_INLINE argument(T& data, typename std::enable_if<!std::is_same<argument, T>::value && !std::is_same<variant_array_view, T>::value>::type* = nullptr);

    RTTR_INLINE argument& operator=(const argument& other);

    template<typename T>
    bool is_type() const;
    RTTR_INLINE type get_type() const;
    RTTR_INLINE void* get_ptr() const;

    template<typename T>
    RTTR_INLINE typename std::enable_if<!std::is_rvalue_reference<T>::value, T>::type& get_value() const;
    template<typename T>
    RTTR_INLINE typename std::enable_if<std::is_rvalue_reference<T>::value, typename std::remove_reference<T>::type>::type&& get_value() const;

private:
    const void*         m_data;
    const rttr::type    m_type;
};

} // end namespace rttr

#include "rttr/detail/impl/argument_impl.h"

#endif // RTTR_ARGUMENT_H_
