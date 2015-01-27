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
#include "rttr/type.h"
#include "rttr/variant.h"
#include "rttr/variant_array.h"

#include <type_traits>
#include <utility>

namespace rttr
{
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
    argument() : _data(nullptr), _type(impl::get_invalid_type()) {}
    argument(const argument& other) : _data(other._data), _type(other._type) {}
    argument(variant& var) : _data(var.get_ptr()), _type(var.get_type()) {}
    argument(const variant& var) : _data(var.get_ptr()), _type(var.get_type()) {}
    argument(variant_array& var) : _data(var.get_ptr()), _type(var.get_type()) {}
    argument(const variant_array& var) : _data(var.get_ptr()), _type(var.get_type()) {}

    template<typename T>
    argument(const T& data, typename std::enable_if<!std::is_same<argument, T>::value >::type* = 0) 
    :   _data(reinterpret_cast<const void*>(std::addressof(data))),
        _type(rttr::type::get<T>())
    {
        static_assert(!std::is_same<instance, T>::value, "Don't use the argument class for forwarding an instance!");
    }

    template<typename T>
    argument(T& data, typename std::enable_if<!std::is_same<argument, T>::value >::type* = 0) 
    :   _data(reinterpret_cast<const void*>(std::addressof(data))),
        _type(rttr::type::get<T>())
    {
        static_assert(!std::is_same<instance, T>::value, "Don't use the argument class for forwarding an instance!");
    }

    template<typename T>
    bool is_type()  const { return rttr::type::get<T>() == _type; }
    type get_type() const { return _type; }
    void* get_ptr() const { return const_cast<void *>(_data); }

    template<typename T>
    T& get_value() const
    {
        using raw_type = typename std::remove_reference<T>::type;
        return (*reinterpret_cast<raw_type*>(const_cast<void *>(_data)));
    }

    argument& operator=(const argument& other)
    {
        _data = other._data;
        const_cast<rttr::type&>(_type) = other._type;
        return *this;
    }

private:
    const void*         _data;
    const rttr::type    _type;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_ARGUMENT_H__
