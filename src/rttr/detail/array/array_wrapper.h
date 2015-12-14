/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_ARRAY_WRAPPER_H_
#define RTTR_ARRAY_WRAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/array/array_wrapper_base.h"
#include "rttr/detail/array/array_accessor.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/array_mapper.h"
#include "rttr/wrapper_mapper.h"

#include <type_traits>
#include <cstddef>

namespace rttr
{
class argument;

namespace detail
{

template<typename T, typename Array_Address = wrapper_address_return_type_t<T>>
class array_wrapper;


template<typename T, typename Array_Address>
class array_wrapper : public array_wrapper_base
{
    using Array_Type = typename detail::raw_type<Array_Address>::type;
    public:
        array_wrapper(const Array_Address& address)
        :   m_address_data(address)
        {
        }

        array_wrapper()
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////
        bool is_valid()                       const { return true; }
        bool is_dynamic()                     const { return array_mapper<Array_Type>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<Array_Type>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<Array_Type>::get_ranke_type(index); }
        type get_type()                       const { return type::get<Array_Type>(); }
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

        bool set_value(std::size_t index_1, argument& arg)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1);
        }
        bool set_value(std::size_t index_1, std::size_t index_2, argument& arg)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1, index_2);
        }

        bool set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg)
        {
            return array_accessor<Array_Type>::set_value(*m_address_data, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(const std::vector<std::size_t>& index_list, argument& arg)
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

        bool insert_value(std::size_t index_1, argument& arg)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1);
        }

        bool insert_value(std::size_t index_1, std::size_t index_2, argument& arg)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1, index_2);
        }
        
        bool insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg)
        {
            return array_accessor<Array_Type>::insert_value(*m_address_data, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(const std::vector<std::size_t>& index_list, argument& arg)
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

        std::unique_ptr<array_wrapper_base> clone() const 
        {
            return detail::make_unique<array_wrapper<T, Array_Address>>(m_address_data);
        }

    private:
        Array_Address m_address_data;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ARRAY_WRAPPER_H_
