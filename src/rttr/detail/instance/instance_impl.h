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

#ifndef RTTR_INSTANCE_IMPL_H_
#define RTTR_INSTANCE_IMPL_H_

#include "rttr/variant.h"
#include "rttr/type.h"
#include "rttr/wrapper_mapper.h"
#include "rttr/detail/misc/misc_type_traits.h"

namespace rttr
{
namespace detail
{

RTTR_INLINE instance::instance() : m_data_container(data_address_container{get_invalid_type(), get_invalid_type(), nullptr, nullptr}) {}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE instance::instance(variant& var)
:   m_data_container(var.get_data_address_container())
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE instance::instance(const instance& other)
:   m_data_container(other.m_data_container)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE instance::instance(instance&& other)
:   m_data_container(other.m_data_container)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE instance::instance(const T& data, typename std::enable_if<!std::is_same<instance, T>::value >::type*) 
:   m_data_container(data_address_container{
                     rttr::type::get<typename raw_type<T>::type>(), rttr::type::get<detail::wrapper_address_return_type_t<T>>(),
                     detail::as_void_ptr(detail::raw_addressof(data)), detail::as_void_ptr(detail::wrapped_raw_addressof(data))})
{
    static_assert(!std::is_same<argument, T>::value, "Don't use the instance class for forwarding an argument!");
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE instance::instance(T& data, typename std::enable_if<!std::is_same<instance, T>::value >::type*) 
:   m_data_container(data_address_container{
                     rttr::type::get<typename raw_type<T>::type>(), rttr::type::get<detail::wrapper_address_return_type_t<T>>(),
                     detail::as_void_ptr(detail::raw_addressof(data)), detail::as_void_ptr(detail::wrapped_raw_addressof(data))})
{
    static_assert(!std::is_same<argument, T>::value, "Don't use the instance class for forwarding an argument!");
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename TargetType>
RTTR_INLINE TargetType* instance::try_convert() const
{
    TargetType* target = static_cast<TargetType*>(type::apply_offset(const_cast<instance*>(this)->m_data_container.m_data_address, m_data_container.m_type, type::get<TargetType>()));
    
    if (!target)
        return (static_cast<TargetType*>(type::apply_offset(const_cast<instance*>(this)->m_data_container.m_data_address_wrapped_type, m_data_container.m_wrapped_type, type::get<TargetType>())));
    
    return target;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool instance::is_valid() const { return (m_data_container.m_data_address != nullptr); }

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE instance::operator bool() const { return (m_data_container.m_data_address != nullptr); }

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type instance::get_type() const { return m_data_container.m_type; }

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_INSTANCE_IMPL_H_
