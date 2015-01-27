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

#ifndef __RTTR_INSTANCE_H__
#define __RTTR_INSTANCE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/misc_type_traits.h"
#include "rttr/type.h"
#include "rttr/variant.h"

namespace rttr
{
namespace detail
{

class argument;
/*!
 * This class is used for forwarding the instance of an object to the function calls.
 *
 * \remark You should never explicit instantiate this class by yourself.
 */
class instance
{
public:
    instance() : _data(nullptr), _type(impl::get_invalid_type()) {}

    instance(variant& var) 
    :   _data(var.get_raw_ptr()),
        _type(var.get_raw_type())
    {
    }

    instance(const instance& other) 
    :   _data(other._data),
        _type(other._type)
    {
    }

    instance(instance&& other) 
    :   _data(other._data),
        _type(other._type)
    {
    }

    template<typename T>
    instance(const T& data, typename std::enable_if<!std::is_same<instance, T>::value >::type* = 0) 
    :   _data(detail::get_void_ptr(data)),
        _type(rttr::type::get<typename raw_type<T>::type>())
    {
        static_assert(!std::is_same<argument, T>::value, "Don't use the instance class for forwarding an argument!");
    }

    template<typename T>
    instance(T& data, typename std::enable_if<!std::is_same<instance, T>::value >::type* = 0) 
    :   _data(detail::get_void_ptr(data)),
        _type(rttr::type::get<typename raw_type<T>::type>())
    {
        static_assert(!std::is_same<argument, T>::value, "Don't use the instance class for forwarding an argument!");
    }

    template<typename TargetType>
    TargetType* try_convert() const
    {
        return (static_cast<TargetType*>(type::apply_offset(const_cast<instance*>(this)->_data, _type, type::get<TargetType>())));
    }

    bool is_valid() const { return (_data != nullptr); }
    operator bool() const { return (_data != nullptr); }

    type get_type() const { return _type; }

private:
    instance& operator=(const instance& other);

private:
    void*               _data;
    const rttr::type    _type;
};

} // end namespace detail

/*!
 * \brief Returns a dummy instance object.
 *         Use this function when you have a static \ref method or \ref property which you need to invoke.
 *
 * \return An instance object.
 */
RTTR_INLINE static detail::instance empty_instance() { return detail::instance(); }
} // end namespace rttr

#endif // __RTTR_INSTANCE_H__
