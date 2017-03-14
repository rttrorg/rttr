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

#include "rttr/variant_array_view.h"

#include "rttr/argument.h"
#include "rttr/instance.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::is_valid() const RTTR_NOEXCEPT
{
    return m_array_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant_array_view::operator bool() const RTTR_NOEXCEPT
{
    return m_array_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::is_dynamic() const RTTR_NOEXCEPT
{
    return m_array_wrapper->is_dynamic();
}

/////////////////////////////////////////////////////////////////////////////////////////

size_t variant_array_view::get_rank() const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_rank();
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_array_view::get_rank_type(std::size_t index) const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_rank_type(index);
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_array_view::get_type() const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size() const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_size();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size(std::size_t index_1) const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_size(index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size(std::size_t index_1, std::size_t index_2) const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_size(index_1, index_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size_variadic(const std::vector<std::size_t>& index_list) const RTTR_NOEXCEPT
{
    return m_array_wrapper->get_size_variadic(index_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size)
{
    return m_array_wrapper->set_size(new_size);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size, std::size_t index_1)
{
    return m_array_wrapper->set_size(new_size, index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
{
    return m_array_wrapper->set_size(new_size, index_1, index_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
{
    return m_array_wrapper->set_size_variadic(new_size, index_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(argument arg)
{
    return m_array_wrapper->set_value(arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, argument arg)
{
    return m_array_wrapper->set_value(index_1, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, std::size_t index_2, argument arg)
{
    return m_array_wrapper->set_value(index_1, index_2, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument arg)
{
    return m_array_wrapper->set_value(index_1, index_2, index_3, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value_variadic(const std::vector<std::size_t>& index_list, argument arg)
{
    return m_array_wrapper->set_value_variadic(index_list, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1) const
{
    return m_array_wrapper->get_value(index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1, std::size_t index_2) const
{
    return m_array_wrapper->get_value(index_1, index_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
{
    return m_array_wrapper->get_value(index_1, index_2, index_3);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value_variadic(const std::vector<std::size_t>& index_list) const
{
    return m_array_wrapper->get_value_variadic(index_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value_as_ref(std::size_t index_1) const
{
    return m_array_wrapper->get_value_as_ref(index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, argument arg)
{
    return m_array_wrapper->insert_value(index_1, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, std::size_t index_2, argument arg)
{
    return m_array_wrapper->insert_value(index_1, index_2, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, argument arg)
{
    return m_array_wrapper->insert_value(index_1, index_2, index_3, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value_variadic(const std::vector<std::size_t>& index_list, argument arg)
{
    return m_array_wrapper->insert_value_variadic(index_list, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1)
{
    return m_array_wrapper->remove_value(index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1, std::size_t index_2)
{
    return m_array_wrapper->remove_value(index_1, index_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
{
    return m_array_wrapper->remove_value(index_1, index_2, index_3);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value_variadic(const std::vector<std::size_t>& index_list)
{
    return m_array_wrapper->remove_value_variadic(index_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
