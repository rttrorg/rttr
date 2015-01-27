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

#ifndef __RTTR_ARRAY_CONTAINER_H__
#define __RTTR_ARRAY_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/array_container_base.h"
#include "rttr/detail/utility.h"
#include "rttr/detail/array_accessor.h"
#include "rttr/detail/array_mapper.h"

#include <type_traits>
#include <cstddef>

namespace rttr
{

namespace detail
{
class argument;
template<typename T, typename Enable = void>
class array_container : public array_container_base
{
    using ArrayType = typename detail::raw_type<T>::type;
    public:
        array_container(const T& arg)
        :   _value(arg)
        {

        }

        array_container(T&& arg)
        :   _value(std::move(arg))
        {

        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<ArrayType>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<ArrayType>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<ArrayType>::get_ranke_type(index); }
        type get_type()                       const { return type::get<T>(); }
        bool is_raw_array()                   const { return std::is_array<ArrayType>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<ArrayType>::get_size(_value);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_size(_value, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_size(_value, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_size(_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return array_accessor<ArrayType>::set_size(_value, new_size);
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return array_accessor<ArrayType>::set_size(_value, new_size, index_1);
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::set_size(_value, new_size, index_1, index_2);
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::set_size(_value, new_size, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return array_accessor<ArrayType>::set_value(_value, arg);
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return array_accessor<ArrayType>::set_value(_value, arg, index_1);
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::set_value(_value, arg, index_1, index_2);
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::set_value(_value, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::set_value(_value, arg, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_value(_value, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_value(_value, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<ArrayType>::get_value(_value, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_value(_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return array_accessor<ArrayType>::insert_value(_value, arg, index_1);
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::insert_value(_value, arg, index_1, index_2);
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::insert_value(_value, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::insert_value(_value, arg, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return array_accessor<ArrayType>::remove_value(_value, index_1);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::remove_value(_value, index_1, index_2);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::remove_value(_value, index_1, index_2, index_3);
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::remove_value(_value, index_list);
        }

        array_container_base* clone() const { return new array_container<T>(_value); }

        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value))); }

    private:
        T _value;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// T*

template<typename T>
class array_container<T*> : public array_container_base
{
    using ArrayType = typename detail::raw_type<T>::type;
    public:
        array_container(T* arg)
        :   _value(static_cast<ArrayType*>(get_void_ptr(arg)))
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<ArrayType>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<ArrayType>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<ArrayType>::get_ranke_type(index); }
        type get_type()                       const { return type::get<T*>(); }
        bool is_raw_array()                   const { return std::is_array<ArrayType>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<ArrayType>::get_size(*_value);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return array_accessor<ArrayType>::set_size(*_value, new_size);
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return array_accessor<ArrayType>::set_size(*_value, new_size, index_1);
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::set_size(*_value, new_size, index_1, index_2);
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::set_size(*_value, new_size, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return array_accessor<ArrayType>::set_value(*_value, arg);
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return array_accessor<ArrayType>::set_value(*_value, arg, index_1);
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::set_value(*_value, arg, index_1, index_2);
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::set_value(*_value, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::set_value(*_value, arg, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return array_accessor<ArrayType>::insert_value(*_value, arg, index_1);
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::insert_value(*_value, arg, index_1, index_2);
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::insert_value(*_value, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::insert_value(*_value, arg, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return array_accessor<ArrayType>::remove_value(*_value, index_1);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<ArrayType>::remove_value(*_value, index_1, index_2);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<ArrayType>::remove_value(*_value, index_1, index_2, index_3);
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return array_accessor<ArrayType>::remove_value(*_value, index_list);
        }

        array_container_base* clone() const { return new array_container<ArrayType*>(_value); }

        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value))); }

    private:
        ArrayType* _value;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// const T*

template<typename T>
class array_container<const T*> : public array_container_base
{
    using ArrayType = typename detail::raw_type<T>::type;
    public:
        array_container(const T* arg)
        :   _value(static_cast<const T*>(get_void_ptr(arg)))
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<ArrayType>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<ArrayType>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<ArrayType>::get_ranke_type(index); }
        type get_type()                       const { return type::get<const T*>(); }
        bool is_raw_array()                   const { return std::is_array<ArrayType>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<ArrayType>::get_size(*_value);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_size(*_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return false;
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return false;
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return false;
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return false;
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return false;
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return false;
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return false;
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return false;
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return false;
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<ArrayType>::get_value(*_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return false;
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return false;
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return false;
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return false;
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return false;
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return false;
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return false;
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return false;
        }

        array_container_base* clone() const { return new array_container<const T*>(_value); }
        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value))); }

    private:
        const T* _value;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class array_container<T, typename std::enable_if<std::is_array<T>::value>::type> : public array_container_base
{
    using ArrayType = typename detail::raw_type<T>::type;
    public:
        array_container(const T& arg)
        {
            #if RTTR_COMPILER == RTTR_COMPILER_MSVC
                detail::copy_array(const_cast<std::remove_const<T>::type&>(arg), _value);
            #else
                detail::copy_array(arg, _value);
            #endif
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool is_dynamic()                     const { return array_mapper<T>::is_dynamic(); }
        std::size_t get_rank()                const { return rank<T>::value; }
        type get_rank_type(std::size_t index) const { return array_accessor<T>::get_ranke_type(index); }
        type get_type()                       const { return type::get<T>(); }
        bool is_raw_array()                   const { return std::is_array<T>::value; }

        /////////////////////////////////////////////////////////////////////////////////////////

        std::size_t get_size() const
        {
            return array_accessor<T>::get_size(_value);
        }
        std::size_t get_size(std::size_t index_1) const
        {
            return array_accessor<T>::get_size(_value, index_1);
        }
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<T>::get_size(_value, index_1, index_2);
        }
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<T>::get_size(_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_size(std::size_t new_size)
        {
            return array_accessor<T>::set_size(_value, new_size);
        }
        bool set_size(std::size_t new_size, std::size_t index_1)
        {
            return array_accessor<T>::set_size(_value, new_size, index_1);
        }
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<T>::set_size(_value, new_size, index_1, index_2);
        }
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<T>::set_size(_value, new_size, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool set_value(argument& arg)
        {
            return array_accessor<T>::set_value(_value, arg);
        }

        bool set_value(argument& arg, std::size_t index_1)
        {
            return array_accessor<T>::set_value(_value, arg, index_1);
        }
        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<T>::set_value(_value, arg, index_1, index_2);
        }

        bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<T>::set_value(_value, arg, index_1, index_2, index_3);
        }

        bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<T>::set_value(_value, arg, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        variant get_value(std::size_t index_1) const
        {
            return array_accessor<T>::get_value(_value, index_1);
        }

        variant get_value(std::size_t index_1, std::size_t index_2) const
        {
            return array_accessor<T>::get_value(_value, index_1, index_2);
        }

        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
        {
            return array_accessor<T>::get_value(_value, index_1, index_2, index_3);
        }

        variant get_value_variadic(const std::vector<std::size_t>& index_list) const
        {
            return array_accessor<T>::get_value(_value, index_list);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool insert_value(detail::argument& arg, std::size_t index_1)
        {
            return array_accessor<T>::insert_value(_value, arg, index_1);
        }

        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<T>::insert_value(_value, arg, index_1, index_2);
        }
        
        bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<T>::insert_value(_value, arg, index_1, index_2, index_3);
        }

        bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list)
        {
            return array_accessor<T>::insert_value(_value, arg, index_list);
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////

        bool remove_value(std::size_t index_1)
        {
            return array_accessor<T>::remove_value(_value, index_1);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2)
        {
            return array_accessor<T>::remove_value(_value, index_1, index_2);
        }

        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
        {
            return array_accessor<T>::remove_value(_value, index_1, index_2, index_3);
        }
        bool remove_value_variadic(const std::vector<std::size_t>& index_list)
        {
            return array_accessor<T>::remove_value(_value, index_list);
        }

        array_container_base* clone() const { return new array_container<T>(_value); }
        void* get_ptr() const { return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value))); }

    private:
        T _value;
        /////////////////////////////////////////////////////////////////////////////////////////
};

template<typename T, typename F>
array_container_base* create_array_container_impl(const F& value, typename std::enable_if<is_array<T>::value>::type* = 0)
{
    using ArrayType = typename std::remove_cv<typename std::remove_reference<F>::type>::type;
    return new array_container<ArrayType>(value);
}

template<typename T, typename F>
array_container_base* create_array_container_impl(const F& value, typename std::enable_if<!is_array<T>::value>::type* = 0)
{
    return nullptr;
}

template<typename T, typename F>
array_container_base* create_array_container_moved_impl(F&& value, typename std::enable_if<is_array<T>::value>::type* = 0)
{
    using ArrayType = typename std::remove_cv<typename std::remove_reference<F>::type>::type;
    return new array_container<ArrayType>(std::move(value));
}

template<typename T, typename F>
array_container_base* create_array_container_moved_impl(F&& value, typename std::enable_if<!is_array<T>::value>::type* = 0)
{
    return nullptr;
}

template<typename T>
array_container_base* create_array_container(const T& value)
{
    return create_array_container_impl<typename detail::raw_type<T>::type, T>(value);
}

template<typename T>
array_container_base* create_array_container_moved(T&& value)
{
    return create_array_container_moved_impl<typename detail::raw_type<T>::type, T>(std::move(value));
}

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_ARRAY_CONTAINER_H__
