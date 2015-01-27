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

#include "rttr/detail/misc_type_traits.h"
#include "rttr/detail/utility.h"
#include "rttr/detail/type_converter.h"
#include "rttr/detail/std_conversion_functions.h"
#include "rttr/detail/array_container.h"

namespace rttr
{

namespace detail
{
template<typename T>
detail::array_container_base* create_array_container(const T& value);
} // end namespace detail;

RTTR_INLINE variant::variant()
:   _holder(0)
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant(const variant& other)
:   _holder(other._holder ? other._holder->clone() : nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant(variant&& other)
:   _holder(other._holder)
{
    other._holder = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::~variant()
{
    delete _holder;
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    #if RTTR_COMP_VER <= 1800
        _holder = nullptr;
    #else
        #error "Please check if this lead to still to a crash."
    #endif
#endif
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void variant::swap(variant& other)
{
    std::swap(_holder, other._holder);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE variant& variant::operator=(T&& other)
{
    variant(static_cast<T&&>(other)).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant& variant::operator =(const variant& other)
{
    variant(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant& variant::operator=(variant&& other)
{
    other.swap(*this);
    variant().swap(other);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type variant::get_type() const
{
    return (_holder ? _holder->get_type() : impl::get_invalid_type());
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_ptr() const
{
    return (_holder ? _holder->get_ptr() : nullptr);
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type variant::get_raw_type() const
{
    return (_holder ? _holder->get_raw_type() : impl::get_invalid_type());
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE void* variant::get_raw_ptr() const
{
    return (_holder ? _holder->get_raw_ptr() : nullptr);
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool variant::is_valid() const
{
    return (_holder ? true : false); 
}

/////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE variant::variant_container_base::~variant_container_base()
{

}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant(const T& param)
:   _holder(new variant_container<typename std::remove_cv<typename std::remove_reference<T>::type>::type>(param))
{
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant(T&& param,
                typename std::enable_if<!std::is_same<variant&, T>::value >::type*,
                typename std::enable_if<!std::is_const<T>::value >::type*
                )
:   _holder(new variant_container<typename std::remove_cv<typename std::remove_reference<T>::type>::type>(static_cast<T&&>(param)))
{
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::is_type() const
{
    if (!is_valid())
        return false;
    else
        return (type::get<T>() == _holder->get_type());
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::can_convert() const
{
    if (!is_valid())
        return false;

    return _holder->can_convert(type::get<T>());
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
T& variant::get_value() const
{
    typedef typename std::remove_cv<T>::type nonRef;
    return static_cast<variant_container<nonRef> *>(_holder)->_value;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
T variant::convert(bool* ok) const
{
    const type& source_type = _holder->get_type();
    const type& target_type = type::get<T>();
    if (target_type == source_type)
    {
        if (ok)
            *ok = true;
        typedef typename std::remove_cv<T>::type nonRef;
        return static_cast<variant_container<nonRef> *>(_holder)->_value;
    }
    else
    {
        const auto& converter = source_type.get_type_converter(target_type);
        void* data = _holder->get_ptr();
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

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename SourceType>
T variant::get_value_with_default_value(const SourceType& source, T default_value, bool* ok)
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

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
variant::variant_container<T, Enable>::variant_container(const T& arg)
:   _value(arg)
{

}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
variant::variant_container<T, Enable>::variant_container(T&& arg)
:   _value(std::move(arg))
{

}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
variant::variant_container_base* variant::variant_container<T, Enable>::clone() const
{
    return (new variant_container<T>(_value));
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
type variant::variant_container<T, Enable>::get_type() const
{
    return type::get<T>();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>           
void* variant::variant_container<T, Enable>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
type variant::variant_container<T, Enable>::get_raw_type() const
{
    return type::get<typename detail::raw_type<T>::type>();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>            
void* variant::variant_container<T, Enable>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
bool variant::variant_container<T, Enable>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<T>();
    if (source_type == target_type)
        return true;
    
    if (source_type.get_type_converter(target_type))
        return true;
   
    if (detail::pointer_count<T>::value == 1 && target_type.is_pointer())
    {
        if (void * ptr = type::apply_offset(get_raw_ptr(), source_type, target_type))
            return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
bool variant::variant_container<T, Enable>::is_array() const
{
    return detail::is_array<typename detail::raw_type<T>::type>::value;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
detail::array_container_base* variant::variant_container<T, Enable>::to_array() const
{
    return detail::create_array_container(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
std::string variant::variant_container<T, Enable>::to_string(bool* ok) const
{
    return variant::get_value_with_default_value<std::string, T>(_value, std::string(), ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
int variant::variant_container<T, Enable>::to_int(bool* ok) const
{
    return variant::get_value_with_default_value<int, T>(_value, 0, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
bool variant::variant_container<T, Enable>::to_bool(bool* ok) const
{
    return variant::get_value_with_default_value<bool, T>(_value, false, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
float variant::variant_container<T, Enable>::to_float(bool* ok) const
{
    return variant::get_value_with_default_value<float, T>(_value, 0.0f, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable>
double variant::variant_container<T, Enable>::to_double(bool* ok) const
{
    return variant::get_value_with_default_value<double, T>(_value, 0.0, ok);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::variant_container(const T& arg)
{
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    detail::copy_array(const_cast<std::remove_const<T>::type&>(arg), _value);
#else
    detail::copy_array(arg, _value);
#endif
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
variant::variant_container_base* variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::clone() const
{
    return (new variant_container<T>(_value));
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
type variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::get_type() const
{
    return type::get<T>();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>           
void* variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
type variant::variant_container<T,  typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::get_raw_type() const
{
    return type::get<typename detail::raw_type<T>::type>();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>            
void* variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::is_array() const
{
    return detail::is_array<typename detail::raw_type<T>::type>::value;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
detail::array_container_base* variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_array() const
{
    return detail::create_array_container(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<T>();
    if (source_type == target_type)
        return true;
    
    if (source_type.get_type_converter(target_type))
        return true;
    
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::string variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_string(bool* ok) const
{
    return variant::get_value_with_default_value<std::string, T>(_value, std::string(), ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
int variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_int(bool* ok) const
{
    return variant::get_value_with_default_value<int, T>(_value, 0, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_bool(bool* ok) const
{
    return variant::get_value_with_default_value<bool, T>(_value, false, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
float variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_float(bool* ok) const
{
    return variant::get_value_with_default_value<float, T>(_value, 0.0f, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
double variant::variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value >::type>::to_double(bool* ok) const
{
    return variant::get_value_with_default_value<double, T>(_value, 0.0, ok);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// one dimensional array


template<std::size_t N>
variant::variant_container<char[N]>::variant_container(const char (&arg)[N])
{
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    detail::copy_array(const_cast<std::remove_const<char[N]>::type&>(arg), _value);
#else
    detail::copy_array(arg, _value);
#endif
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
variant::variant_container_base* variant::variant_container<char[N]>::clone() const
{
    return (new variant_container<char[N]>(_value));
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
type variant::variant_container<char[N]>::get_type() const
{
    return type::get<char[N]>();
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>           
void* variant::variant_container<char[N]>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
type variant::variant_container<char[N]>::get_raw_type() const
{
    return type::get<typename detail::raw_type<char[N]>::type>();
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>            
void* variant::variant_container<char[N]>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>  
bool variant::variant_container<char[N]>::is_array() const
{
    return detail::is_array<typename detail::raw_type<char[N]>::type>::value;
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>  
detail::array_container_base* variant::variant_container<char[N]>::to_array() const
{
    return detail::create_array_container<char[N]>(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
bool variant::variant_container<char[N]>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<char[N]>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<std::string>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
std::string variant::variant_container<char[N]>::to_string(bool* ok) const
{
    if (ok)
        *ok = true;
    return std::string(_value);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
int variant::variant_container<char[N]>::to_int(bool* ok) const
{
    return detail::char_to_int(_value, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
bool variant::variant_container<char[N]>::to_bool(bool* ok) const
{
    return detail::char_to_bool(_value, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
float variant::variant_container<char[N]>::to_float(bool* ok) const
{
    return detail::char_to_float(_value, ok);
}

/////////////////////////////////////////////////////////////////////////////////

template<std::size_t N>
double variant::variant_container<char[N]>::to_double(bool* ok) const
{
    return detail::char_to_double(_value, ok);
}

/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
