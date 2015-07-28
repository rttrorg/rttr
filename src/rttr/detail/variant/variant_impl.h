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

#ifndef RTTR_VARIANT_IMPL_H_
#define RTTR_VARIANT_IMPL_H_

#include "rttr/type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/type/type_converter.h"
#include "rttr/detail/data_address_container.h"
#include "rttr/detail/variant/variant_data_policy.h"
#include "rttr/variant_array_view.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant()
:   m_variant_policy(&detail::variant_data_policy_empty::invoke)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
RTTR_INLINE variant::variant(T&& val)
:   m_variant_policy(&detail::variant_policy<Tp>::invoke)
{
    static_assert(std::is_copy_constructible<Tp>::value || std::is_array<Tp>::value, 
                  "The given value is not copy constructible, try to add a copy constructor to the class.");

    detail::variant_policy<Tp>::create(std::forward<T>(val), m_variant_data);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::~variant()
{
   m_variant_policy(detail::variant_policy_operation::DESTROY, m_variant_data, detail::argument_wrapper());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp>
RTTR_INLINE variant& variant::operator=(T&& other)
{
    *this = variant(std::forward<T>(other));
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE T& variant::get_value()
{
    void* value;
    m_variant_policy(detail::variant_policy_operation::GET_VALUE, m_variant_data, value);
    typedef typename detail::remove_cv<T>::type nonRef;
    return *reinterpret_cast<nonRef*>(value);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_ptr() const
{
    void* value;
    m_variant_policy(detail::variant_policy_operation::GET_PTR, m_variant_data, value);
    return value;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type variant::get_raw_type() const
{
    type result(type::m_invalid_id);
    m_variant_policy(detail::variant_policy_operation::GET_RAW_TYPE, m_variant_data, result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_raw_ptr() const
{
    void* result;
    m_variant_policy(detail::variant_policy_operation::GET_RAW_PTR, m_variant_data, result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE detail::data_address_container variant::get_data_address_container() const
{
    detail::data_address_container result{type(type::m_invalid_id), type(type::m_invalid_id), nullptr, nullptr};
    m_variant_policy(detail::variant_policy_operation::GET_ADDRESS_CONTAINER, m_variant_data, result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool variant::is_type() const
{
    type src_type(type::m_invalid_id);
    m_variant_policy(detail::variant_policy_operation::GET_TYPE, m_variant_data, src_type);
    return (type::get<T>() == src_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool variant::can_convert() const
{
    return can_convert(type::get<T>());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool variant::convert(T& value) const
{
    bool ok = false;

    const type source_type = get_type();
    const type target_type = type::get<T>();
    if (target_type == source_type)
    {
        value = const_cast<variant&>(*this).get_value<T>();
    }
    else if (const auto& converter = source_type.get_type_converter(target_type))
    {
        detail::type_converter_target<T>* target_converter = static_cast<detail::type_converter_target<T>*>(converter);
        value = target_converter->convert(get_ptr(), ok);
    }
    else if (detail::pointer_count<T>::value == 1 && source_type.is_pointer())
    {
        if (void * ptr = type::apply_offset(get_raw_ptr(), source_type, target_type))
        {
            ok = true;
            value = reinterpret_cast<T>(ptr);
        }
    }

    return ok;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE T variant::convert(bool* ok) const
{
    static_assert(std::is_default_constructible<T>::value, "The given type T has no default constructor."
                                                           "You can only convert to a type, with a default constructor.");
    T result;
    const bool coud_convert = convert<T>(result);
    if (ok)
        *ok = coud_convert;

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_VARIANT_IMPL_H_
