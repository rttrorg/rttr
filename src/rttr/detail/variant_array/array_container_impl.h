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

#ifndef RTTR_ARRAY_CONTAINER_IMPL_H_
#define RTTR_ARRAY_CONTAINER_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/variant_array/array_container_base.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/array/array_accessor.h"
#include "rttr/detail/array/array_mapper.h"
#include "rttr/wrapper_mapper.h"

#include <type_traits>
#include <cstddef>

namespace rttr
{

namespace detail
{
class argument;

template<typename T>
class array_container : public array_container_base
{
    using Array_Type = typename detail::raw_type<T>::type;
    public:

        template<typename U = T>
        array_container(const T& arg, typename std::enable_if<!std::is_array<U>::value>::type* = nullptr )
        :   m_value(arg)
        {
        }

        template<typename U = T>
        array_container(const T& arg, typename std::enable_if<std::is_array<U>::value>::type* = nullptr )
        {
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
            copy_array(const_cast<typename detail::remove_const<T>::type&>(arg), m_value);
#   else
            #error "Check new MSVC Compiler!"
#   endif
#else
            copy_array(arg, m_value);
#endif
        }

        array_container(T&& arg)
        :   m_value(std::forward<T>(arg))
        {

        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<Array_Type>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<Array_Type>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<Array_Type>::get_ranke_type(index); }
        type get_type()                       const { return type::get<T>(); }
        bool is_raw_array()                   const { return std::is_array<Array_Type>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<Array_Type>::get_size(m_value);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<Array_Type>::get_size(m_value, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<Array_Type>::get_size(m_value, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<Array_Type>::get_size(m_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return array_accessor<Array_Type>::set_size(m_value, new_size);
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return array_accessor<Array_Type>::set_size(m_value, new_size, index_1);
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::set_size(m_value, new_size, index_1, index_2);
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::set_size(m_value, new_size, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return array_accessor<Array_Type>::set_value(m_value, arg);
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return array_accessor<Array_Type>::set_value(m_value, arg, index_1);
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::set_value(m_value, arg, index_1, index_2);
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::set_value(m_value, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::set_value(m_value, arg, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<Array_Type>::get_value(m_value, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<Array_Type>::get_value(m_value, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<Array_Type>::get_value(m_value, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<Array_Type>::get_value(m_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return array_accessor<Array_Type>::insert_value(m_value, arg, index_1);
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::insert_value(m_value, arg, index_1, index_2);
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::insert_value(m_value, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::insert_value(m_value, arg, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return array_accessor<Array_Type>::remove_value(m_value, index_1);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::remove_value(m_value, index_1, index_2);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::remove_value(m_value, index_1, index_2, index_3);
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::remove_value(m_value, index_list);
        }

        array_container_base* clone() const { return new array_container<T>(m_value); }

        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(m_value))); }

    private:
        T m_value;
};

/////////////////////////////////////////////////////////////////////////////////////////
// for wrappers or pointer types

template<typename Data_Type, typename Address_Type>
class array_container_with_address : public array_container_base
{
    using Array_Type = typename detail::raw_type<Address_Type>::type;
    public:
        array_container_with_address(const Data_Type& arg, const Address_Type& address)
        :   m_value(arg), m_address_data(address)
        {
            
        }

        array_container_with_address(Data_Type&& arg, const Address_Type& address)
        :   m_value(std::forward<Data_Type>(arg)), m_address_data(address)
        {

        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<Array_Type>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<Array_Type>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<Array_Type>::get_ranke_type(index); }
        type get_type()                       const { return type::get<Data_Type>(); }
        bool is_raw_array()                   const { return std::is_array<Array_Type>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<Array_Type>::get_size(*m_address_data);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<Array_Type>::get_size(*m_address_data, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<Array_Type>::get_size(*m_address_data, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<Array_Type>::get_size(*m_address_data, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return array_accessor<Array_Type>::set_size(*m_address_data, new_size);
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return array_accessor<Array_Type>::set_size(*m_address_data, new_size, index_1);
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::set_size(*m_address_data, new_size, index_1, index_2);
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::set_size(*m_address_data, new_size, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg);
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1);
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1, index_2);
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<Array_Type>::get_value(*m_address_data, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<Array_Type>::get_value(*m_address_data, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<Array_Type>::get_value(*m_address_data, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<Array_Type>::get_value(*m_address_data, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1);
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1, index_2);
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return array_accessor<Array_Type>::remove_value(*m_address_data, index_1);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<Array_Type>::remove_value(*m_address_data, index_1, index_2);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<Array_Type>::remove_value(*m_address_data, index_1, index_2, index_3);
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return array_accessor<Array_Type>::remove_value(*m_address_data, index_list);
        }

        array_container_base* clone() const { return new array_container_with_address<Data_Type, Address_Type>(m_value, m_address_data); }

        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(m_address_data))); }
        

    private:
        Data_Type m_value;
        Address_Type m_address_data;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!can_create_array_container<T>::value,
                        array_container_base*>::type 
create_array_container_impl(T&& value)
{
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<is_array<T>::value, 
                        array_container_base*>::type 
create_array_container_impl(T&& value)
{
    using raw_type = typename remove_cv<typename std::remove_reference<T>::type>::type;
    return new array_container<raw_type>(std::forward<T>(value));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<is_wrapper_array_type<T>::value && 
                        !is_pointer_array_type<T>::value, 
                        array_container_base*>::type 
create_array_container_impl(T&& value)
{
    using adress_type = decltype(wrapped_raw_addressof(value));
    adress_type address = wrapped_raw_addressof(value);
    using data_type = typename detail::remove_cv<typename std::remove_reference<T>::type>::type;
    return new array_container_with_address<data_type, adress_type>(std::forward<T>(value), address);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!is_wrapper_array_type<T>::value && 
                         is_pointer_array_type<T>::value,
                         array_container_base*>::type 
create_array_container_impl(T&& value)
{
   using adress_type = decltype(raw_addressof(value));
   adress_type address = raw_addressof(value);
   using data_type = typename detail::remove_cv<typename std::remove_reference<T>::type>::type;
   return new array_container_with_address<data_type, adress_type>(std::forward<T>(value), address);
}

/////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
array_container_base* create_array_container(T&& value)
{
    return create_array_container_impl(std::forward<T>(value));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ARRAY_CONTAINER_IMPL_H_
