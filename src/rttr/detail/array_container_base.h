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

#ifndef __RTTR_ARRAY_CONTAINER_BASE_H__
#define __RTTR_ARRAY_CONTAINER_BASE_H__

#include "rttr/base/core_prerequisites.h"

#include <vector>
#include <cstddef>

namespace rttr
{
class type;
class variant;

namespace detail
{

class instance;
class argument;

class RTTR_API array_container_base
{
    public:
        virtual ~array_container_base() {};
        virtual bool        is_dynamic() const = 0;
        virtual std::size_t get_rank() const = 0;
        virtual type        get_rank_type(std::size_t index) const = 0;
        virtual type        get_type() const = 0;
        virtual bool        is_raw_array() const = 0;
      
        virtual std::size_t get_size() const = 0;
        virtual std::size_t get_size(std::size_t index_1) const = 0;
        virtual std::size_t get_size(std::size_t index_1, std::size_t index_2) const = 0;
        virtual std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const = 0;

        virtual bool set_size(std::size_t new_size) = 0;
        virtual bool set_size(std::size_t new_size, std::size_t index_1) = 0;
        virtual bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2) = 0;
        virtual bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list) = 0;
        
        virtual bool set_value(argument& arg) = 0;
        virtual bool set_value(argument& arg, std::size_t index_1) = 0;
        virtual bool set_value(argument& arg, std::size_t index_1, std::size_t index_2) = 0;
        virtual bool set_value(argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3) = 0;
        virtual bool set_value_variadic(argument& arg, const std::vector<std::size_t>& index_list) = 0;

        virtual variant get_value(std::size_t index_1) const = 0;
        virtual variant get_value(std::size_t index_1, std::size_t index_2) const = 0;
        virtual variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const = 0;
        virtual variant get_value_variadic(const std::vector<std::size_t>& index_list) const = 0;

        virtual bool insert_value(detail::argument& arg, std::size_t index_1) = 0;
        virtual bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2) = 0;
        virtual bool insert_value(detail::argument& arg, std::size_t index_1, std::size_t index_2, std::size_t index_3) = 0;
        virtual bool insert_value_variadic(detail::argument& arg, const std::vector<std::size_t>& index_list) = 0;

        virtual bool remove_value(std::size_t index_1) = 0;
        virtual bool remove_value(std::size_t index_1, std::size_t index_2) = 0;
        virtual bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) = 0;
        virtual bool remove_value_variadic(const std::vector<std::size_t>& index_list) = 0;

        virtual array_container_base* clone() const = 0;
        virtual void* get_ptr() const = 0;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_ARRAY_CONTAINER_BASE_H__
