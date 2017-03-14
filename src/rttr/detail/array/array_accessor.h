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

#ifndef RTTR_ARRAY_ACCESSOR_H_
#define RTTR_ARRAY_ACCESSOR_H_

#include "rttr/detail/base/core_prerequisites.h"

namespace rttr
{
class type;
class variant;

namespace detail
{
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Array_Type>
struct array_accessor
{
    template<typename... Indices>
    static std::size_t get_size(const Array_Type& array, Indices... args);

    static std::size_t get_size(const Array_Type& array, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool set_size(const Array_Type&, std::size_t, Indices... indices);
    static bool set_size(const Array_Type&, std::size_t, const std::vector<std::size_t>&);

    template<typename... Indices>
    static bool set_size(Array_Type& array, std::size_t new_size, Indices... args);

    static bool set_size(Array_Type& array, std::size_t new_size, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static variant get_value(const Array_Type& array, Indices... indices);

    static variant get_value(const Array_Type& array, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    static variant get_value_as_ref(Array_Type& array, std::size_t index_1);
    static variant get_value_as_ref(const Array_Type& array, std::size_t index_1);

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool set_value(const Array_Type&, argument&, Indices... indices);
    static bool set_value(const Array_Type&, argument&, const std::vector<std::size_t>&);

    template<typename... Indices>
    static bool set_value(Array_Type& array, argument& arg, Indices... indices);

    static bool set_value(Array_Type& array, argument& arg, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool insert_value(const Array_Type&, argument&, Indices... indices);
    static bool insert_value(const Array_Type&, argument&, const std::vector<std::size_t>&);

    template<typename... Indices>
    static bool insert_value(Array_Type& array, argument& arg, Indices... indices);
    static bool insert_value(Array_Type& array, argument& arg, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    template<typename... Indices>
    static bool remove_value(const Array_Type&, Indices... indices);
    static bool remove_value(const Array_Type&, const std::vector<std::size_t>&);

    template<typename... Indices>
    static bool remove_value(Array_Type& array, Indices... indices);
    static bool remove_value(Array_Type& array, const std::vector<std::size_t>& index_list);

    /////////////////////////////////////////////////////////////////////////////////////////

    static type get_ranke_type(std::size_t index);

    /////////////////////////////////////////////////////////////////////////////////////////
};

} // end namespace detail
} // end namespace rttr

#include "rttr/detail/array/array_accessor_impl.h"

#endif // RTTR_ARRAY_ACCESSOR_H_
