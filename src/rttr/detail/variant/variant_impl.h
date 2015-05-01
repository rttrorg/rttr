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
#include "rttr/detail/std_conversion_functions.h"
#include "rttr/detail/array/array_container.h"
#include "rttr/detail/data_address_container.h"

namespace rttr
{

RTTR_INLINE variant::variant()
:   m_holder(0)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant(const variant& other)
:   m_holder(other.m_holder ? other.m_holder->clone() : nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant(variant&& other)
:   m_holder(other.m_holder)
{
    other.m_holder = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::~variant()
{
    delete m_holder;
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
        m_holder = nullptr;
#   else
#       error "Please check if this lead to still to a crash."
#   endif
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant::swap(variant& other)
{
    std::swap(m_holder, other.m_holder);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE variant& variant::operator=(T&& other)
{
    variant(static_cast<T&&>(other)).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant& variant::operator =(const variant& other)
{
    variant(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant& variant::operator=(variant&& other)
{
    other.swap(*this);
    variant().swap(other);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type variant::get_type() const
{
    return (m_holder ? m_holder->get_type() : detail::get_invalid_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_ptr() const
{
    return (m_holder ? m_holder->get_ptr() : nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type variant::get_raw_type() const
{
    return (m_holder ? m_holder->get_raw_type() : detail::get_invalid_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_raw_ptr() const
{
    return (m_holder ? m_holder->get_raw_ptr() : nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE detail::data_address_container variant::get_data_address_container() const
{
    return (m_holder ? m_holder->get_data_address_container() : detail::data_address_container{detail::get_invalid_type(), detail::get_invalid_type(), nullptr, nullptr});
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool variant::is_valid() const
{
    return (m_holder ? true : false); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant_container_base::~variant_container_base()
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant(const T& param)
:   m_holder(new variant_container<typename detail::remove_cv<typename std::remove_reference<T>::type>::type>(param))
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant(T&& param,
                typename std::enable_if<!std::is_same<variant&, T>::value >::type*,
                typename std::enable_if<!std::is_const<T>::value >::type*
                )
:   m_holder(new variant_container<typename detail::remove_cv<typename std::remove_reference<T>::type>::type>(std::forward<T>(param)))
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::is_type() const
{
    if (!is_valid())
        return false;
    else
        return (type::get<T>() == m_holder->get_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::can_convert() const
{
    return can_convert(type::get<T>());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T& variant::get_value()
{
    typedef typename detail::remove_cv<T>::type nonRef;
    return static_cast<variant_container<nonRef> *>(m_holder)->m_value;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T variant::convert(bool* ok) const
{
    const type& source_type = m_holder->get_type();
    const type& target_type = type::get<T>();
    if (target_type == source_type)
    {
        if (ok)
            *ok = true;
        typedef typename std::remove_cv<T>::type nonRef;
        return static_cast<variant_container<nonRef> *>(m_holder)->m_value;
    }
    else
    {
        const auto& converter = source_type.get_type_converter(target_type);
        void* data = m_holder->get_ptr();
        using t_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
        if (const auto& targert_converter = static_cast<detail::type_converter_target<t_type>*>(converter))
        {
            if (ok)
            {
                return targert_converter->convert(data, *ok);
            }
            else
            {
                bool ok_ref = false;
                return targert_converter->convert(data, ok_ref);
            }
        }

        if (detail::pointer_count<T>::value == 1 && source_type.is_pointer())
        {
            if (void * ptr = type::apply_offset(get_raw_ptr(), source_type, target_type))
            {
                if (ok)
                    *ok = true;
                return static_cast<T>(ptr);
            }
        }
        if (ok)
            *ok = false;
        
        // danger the conversion doesn't worked
        return static_cast<T>(nullptr);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename SourceType>
typename std::enable_if<!detail::is_one_dim_char_array<SourceType>::value, T>::type 
variant::get_value_with_default_value(const SourceType& source, T default_value, bool* ok)
{
    const type& source_type = type::get<SourceType>();
    const type& target_type = type::get<T>();
    if (const auto& converter = source_type.get_type_converter(target_type))
    {
        void* data = const_cast<void*>(reinterpret_cast<const void*>(std::addressof(source)));
        using t_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
        const auto& targert_converter = static_cast<detail::type_converter_target<t_type>*>(converter);
        if (ok)
        {
            return targert_converter->convert(data, *ok);
        }
        else
        {
            bool ok_ref = false;
            return targert_converter->convert(data, ok_ref);
        }
    }
    else
    {
        return default_value;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename SourceType>
typename std::enable_if<detail::is_one_dim_char_array<SourceType>::value, bool>::type 
variant::get_value_with_default_value(const SourceType& source, bool default_value, bool* ok)
{
    // TO DO provide dimension
    return detail::char_to_bool(source, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename SourceType>
typename std::enable_if<detail::is_one_dim_char_array<SourceType>::value, int>::type 
variant::get_value_with_default_value(const SourceType& source, int default_value, bool* ok)
{
    return detail::char_to_int(source, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename SourceType>
typename std::enable_if<detail::is_one_dim_char_array<SourceType>::value, std::string>::type 
variant::get_value_with_default_value(const SourceType& source, std::string default_value, bool* ok)
{
    if (ok)
        *ok = true;
    
    return std::string(source);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename SourceType>
typename std::enable_if<detail::is_one_dim_char_array<SourceType>::value, float>::type 
variant::get_value_with_default_value(const SourceType& source, float default_value, bool* ok)
{
    return detail::char_to_float(source, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename SourceType>
typename std::enable_if<detail::is_one_dim_char_array<SourceType>::value, double>::type 
variant::get_value_with_default_value(const SourceType& source, double default_value, bool* ok)
{
    return detail::char_to_double(source, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename U>
variant::variant_container<T>::variant_container(const T& arg, typename std::enable_if<!std::is_array<U>::value>::type*)
:   m_value(arg)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename U>
variant::variant_container<T>::variant_container(const T& arg, typename std::enable_if<std::is_array<U>::value>::type*)
{
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
        detail::copy_array(const_cast<typename detail::remove_const<T>::type&>(arg), m_value);
#   else
        #error "Check new MSVC Compiler!"
#   endif
#else
    detail::copy_array(arg, m_value);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant_container<T>::variant_container(T&& arg)
:   m_value(std::forward<T>(arg))
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant_container_base* variant::variant_container<T>::clone() const
{
    return (new variant_container<T>(m_value));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
type variant::variant_container<T>::get_type() const
{
    return type::get<T>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>           
void* variant::variant_container<T>::get_ptr() const
{
    return detail::as_void_ptr(std::addressof(m_value));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
type variant::variant_container<T>::get_raw_type() const
{
    return type::get<typename detail::raw_type<T>::type>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>            
void* variant::variant_container<T>::get_raw_ptr() const
{
    return detail::as_void_ptr(detail::raw_addressof(m_value));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
detail::data_address_container variant::variant_container<T>::get_data_address_container() const
{
    return detail::data_address_container{type::get< detail::raw_addressof_return_type_t<T> >(), 
                                          type::get< detail::wrapper_adress_return_type_t<T> >(),
                                          detail::as_void_ptr(detail::raw_addressof(m_value)),
                                          detail::as_void_ptr(detail::wrapped_raw_addressof(m_value))};
    
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<T>();
    if (source_type == target_type)
        return true;
    
    if (source_type.get_type_converter(target_type))
        return true;
   
    if (detail::pointer_count<T>::value == 1 && target_type.get_pointer_dimension() == 1)
    {
        if (void * ptr = type::apply_offset(get_raw_ptr(), source_type, target_type))
            return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T>::is_array() const
{
    return detail::is_array<typename detail::raw_type<T>::type>::value;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
detail::array_container_base* variant::variant_container<T>::to_array() const
{
    return detail::create_array_container(m_value);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::string variant::variant_container<T>::to_string(bool* ok) const
{
    return variant::get_value_with_default_value(m_value, std::string(), ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
int variant::variant_container<T>::to_int(bool* ok) const
{
    return variant::get_value_with_default_value(m_value, 0, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T>::to_bool(bool* ok) const
{
    return variant::get_value_with_default_value(m_value, false, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
float variant::variant_container<T>::to_float(bool* ok) const
{
    return variant::get_value_with_default_value(m_value, 0.0f, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
double variant::variant_container<T>::to_double(bool* ok) const
{
    return variant::get_value_with_default_value(m_value, 0.0, ok);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_VARIANT_IMPL_H_
