/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_ARRAY_MAPPER_IMPL_H_
#define RTTR_ARRAY_MAPPER_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <cstddef>
#include <vector>
#include <list>
#include <array>

namespace rttr
{

//////////////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct array_mapper<T[N]>
{
    using raw_type = typename array_mapper<T>::raw_type;
    using sub_type = T;

    static bool is_dynamic() RTTR_NOEXCEPT
    {
        return false;
    }

    static std::size_t get_size(const T (&)[N]) RTTR_NOEXCEPT
    {
        return N;
    }

    static bool set_size(T (&)[N], std::size_t)
    {
        return false;
    }

    static const T& get_value(const T (& arr)[N], std::size_t index)
    {
        return arr[index];
    }

    static T& get_value(T (& arr)[N], std::size_t index)
    {
        return arr[index];
    }

    static bool insert_value(T (&)[N], const T&, std::size_t)
    {
      return false;
    }

    static bool remove_value(T (&)[N], std::size_t)
    {
      return false;
    }

};

//////////////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct array_mapper<std::array<T, N> >
{
    using raw_type = typename array_mapper<T>::raw_type;
    using sub_type = T;

    static bool is_dynamic() RTTR_NOEXCEPT
    {
        return false;
    }

    static std::size_t get_size(const std::array<T, N>&) RTTR_NOEXCEPT
    {
        return N;
    }

    static bool set_size(std::array<T, N>&, std::size_t)
    {
        return false;
    }

    static const T& get_value(const std::array<T, N>& arr, std::size_t index)
    {
        return arr[index];
    }

    static T& get_value(std::array<T, N>& arr, std::size_t index)
    {
        return arr[index];
    }

    static bool insert_value(std::array<T, N>&, const T&, std::size_t)
    {
      return false;
    }

    static bool remove_value(std::array<T, N>&, std::size_t)
    {
      return false;
    }
};

//////////////////////////////////////////////////////////////////////////////////////

 template <typename T>
 struct array_mapper<std::vector<T> >
 {
    using raw_type = typename array_mapper<T>::raw_type;
    using sub_type = T;

    static bool is_dynamic() RTTR_NOEXCEPT
    {
        return true;
    }

    static std::size_t get_size(const std::vector<T>& arr) RTTR_NOEXCEPT
    {
        return arr.size();
    }

    static bool set_size(std::vector<T>& arr, std::size_t new_size)
    {
        arr.resize(new_size);
        return true;
    }

    static const T& get_value(const std::vector<T>& arr, std::size_t index)
    {
        return arr[index];
    }

    static T& get_value(std::vector<T>& arr, std::size_t index)
    {
        return arr[index];
    }

    static bool insert_value(std::vector<T>& arr, const T& value, std::size_t index)
    {
        arr.insert(arr.begin() + index, value);
        return true;
    }

    static bool remove_value(std::vector<T>& arr, std::size_t index)
    {
        arr.erase(arr.begin() + index);
        return true;
    }
};

 //////////////////////////////////////////////////////////////////////////////////////
 // specialization for std::vector<bool>, because this is not really a vector

 template <>
 struct array_mapper<std::vector<bool> >
 {
    using raw_type          = array_mapper<bool>::raw_type;
    using sub_type          = bool;

    static bool is_dynamic() RTTR_NOEXCEPT
    {
        return true;
    }

    static std::size_t get_size(const std::vector<bool>& arr) RTTR_NOEXCEPT
    {
        return arr.size();
    }

    static bool set_size(std::vector<bool>& arr, std::size_t new_size)
    {
        arr.resize(new_size);
        return true;
    }

    static std::vector<bool>::const_reference get_value(const std::vector<bool>& arr, std::size_t index)
    {
        return arr[index];
    }

    static std::vector<bool>::reference get_value(std::vector<bool>& arr, std::size_t index)
    {
        return arr[index];
    }

    static bool insert_value(std::vector<bool>& arr, const bool& value, std::size_t index)
    {
        arr.insert(arr.begin() + index, value);
        return true;
    }

    static bool remove_value(std::vector<bool>& arr, std::size_t index)
    {
        arr.erase(arr.begin() + index);
        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////////////

 template <typename T>
 struct array_mapper<std::list<T> >
 {
    using raw_type = typename array_mapper<T>::raw_type;
    using sub_type = T;

    static bool is_dynamic() RTTR_NOEXCEPT
    {
        return true;
    }

    static std::size_t get_size(const std::list<T>& arr) RTTR_NOEXCEPT
    {
        return arr.size();
    }

     static bool set_size(std::list<T>& arr, std::size_t new_size)
    {
        arr.resize(new_size);
        return true;
    }

    static const T& get_value(const std::list<T>& arr, std::size_t index)
    {
        typename std::list<T>::const_iterator it = arr.begin();
        std::advance(it, index);
        return *it;
    }

    static T& get_value(std::list<T>& arr, std::size_t index)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, index);
        return *it;
    }

    static bool insert_value(std::list<T>& arr, const T& value, std::size_t index)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, index);
        arr.insert(it, value);
        return true;
    }

    static bool remove_value(std::list<T>& arr, std::size_t index)
    {
        typename std::list<T>::iterator it = arr.begin();
        std::advance(it, index);
        arr.erase(it);
        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#endif // RTTR_ARRAY_MAPPER_IMPL_H_
