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

#ifndef RTTR_ARRAY_WRAPPER_BASE_H_
#define RTTR_ARRAY_WRAPPER_BASE_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <vector>
#include <cstddef>

namespace rttr
{
class type;
class variant;
class instance;
class argument;

namespace detail
{

class RTTR_API array_wrapper_base
{
    public:
        array_wrapper_base() {}
        virtual ~array_wrapper_base() {};
        virtual bool        is_valid() const    { return false; }
        virtual bool        is_dynamic() const  { return false; }
        virtual std::size_t get_rank() const    { return 0; }
        virtual type        get_rank_type(std::size_t index) const { return get_invalid_type(); }
        virtual type        get_type() const    { return get_invalid_type(); }
        virtual bool        is_raw_array() const { return false; }

        virtual std::size_t get_size() const    { return 0; }
        virtual std::size_t get_size(std::size_t index_1) const { return 0; }
        virtual std::size_t get_size(std::size_t index_1, std::size_t index_2) const { return 0; }
        virtual std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const { return 0; }

        virtual bool set_size(std::size_t new_size) { return false; }
        virtual bool set_size(std::size_t new_size, std::size_t index_1) { return false; }
        virtual bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2) { return false; }
        virtual bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list) { return false; }

        virtual bool set_value(argument& arg) { return false; }
        virtual bool set_value(std::size_t index_1, argument& arg) { return false; }
        virtual bool set_value(std::size_t index_1, std::size_t index_2, argument& arg) { return false; }
        virtual bool set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg) { return false; }
        virtual bool set_value_variadic(const std::vector<std::size_t>& index_list, argument& arg) { return false; }

        virtual variant get_value(std::size_t index_1) const { return variant(); }
        virtual variant get_value(std::size_t index_1, std::size_t index_2) const { return variant(); }
        virtual variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const { return variant(); }
        virtual variant get_value_variadic(const std::vector<std::size_t>& index_list) const { return variant(); }

        virtual variant get_value_as_ref(std::size_t index_1) const { return variant(); }

        virtual bool insert_value(std::size_t index_1, argument& arg) { return false; }
        virtual bool insert_value(std::size_t index_1, std::size_t index_2, argument& arg) { return false; }
        virtual bool insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument& arg) { return false; }
        virtual bool insert_value_variadic(const std::vector<std::size_t>& index_list, argument& arg) { return false; }

        virtual bool remove_value(std::size_t index_1) { return false; }
        virtual bool remove_value(std::size_t index_1, std::size_t index_2) { return false; }
        virtual bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) { return false; }
        virtual bool remove_value_variadic(const std::vector<std::size_t>& index_list) { return false; }

        virtual std::unique_ptr<array_wrapper_base> clone() const { return detail::make_unique<array_wrapper_base>(); }
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ARRAY_WRAPPER_BASE_H_
