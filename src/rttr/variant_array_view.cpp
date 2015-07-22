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

#include "rttr/variant_array_view.h"

#include "rttr/detail/variant_array_view/variant_array_view_policy.h"

#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

#include <tuple>

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::is_valid() const
{
    return m_data.m_policy(detail::variant_array_policy_operation::IS_VALID, m_data.m_address, detail::argument_wrapper());
}

/////////////////////////////////////////////////////////////////////////////////////////

variant_array_view::operator bool() const
{
    return m_data.m_policy(detail::variant_array_policy_operation::IS_VALID, m_data.m_address, detail::argument_wrapper());
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::is_dynamic() const
{
    return m_data.m_policy(detail::variant_array_policy_operation::IS_DYNAMIC, m_data.m_address, detail::argument_wrapper());
}

/////////////////////////////////////////////////////////////////////////////////////////

size_t variant_array_view::get_rank() const
{
    std::size_t result = 0;
    m_data.m_policy(detail::variant_array_policy_operation::GET_RANK, m_data.m_address, result);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
 
type variant_array_view::get_rank_type(std::size_t index) const
{
    std::tuple<std::size_t, type> param = std::make_tuple(index, detail::get_invalid_type());
    m_data.m_policy(detail::variant_array_policy_operation::GET_RANK_TYPE, m_data.m_address, param);

    return std::get<1>(param);

}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_array_view::get_type() const
{
    type result = detail::get_invalid_type();
    m_data.m_policy(detail::variant_array_policy_operation::GET_TYPE, m_data.m_address, result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::is_raw_array() const
{
    return m_data.m_policy(detail::variant_array_policy_operation::IS_RAW_ARRAY, m_data.m_address, detail::argument_wrapper());
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size() const
{
    std::size_t result = 0;
    m_data.m_policy(detail::variant_array_policy_operation::GET_SIZE, m_data.m_address, result);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size(std::size_t index_1) const
{
    m_data.m_policy(detail::variant_array_policy_operation::GET_SIZE_1, m_data.m_address, index_1);
    return index_1;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size(std::size_t index_1, std::size_t index_2) const
{
    std::tuple<std::size_t, std::size_t> param = std::make_tuple(index_1, index_2);
    m_data.m_policy(detail::variant_array_policy_operation::GET_SIZE_1, m_data.m_address, param);

    return std::get<1>(param);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array_view::get_size_variadic(const std::vector<std::size_t>& index_list) const
{
    std::tuple<const std::vector<std::size_t>&, std::size_t> param = std::make_tuple(index_list, 0);
    m_data.m_policy(detail::variant_array_policy_operation::GET_SIZE_1, m_data.m_address, param);

    return std::get<1>(param);
}
        
/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size)
{
    return m_data.m_policy(detail::variant_array_policy_operation::SET_SIZE, m_data.m_address, new_size);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size, std::size_t index_1)
{
    std::tuple<std::size_t&, std::size_t&> param = std::tie(new_size, index_1);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_SIZE_1, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
{
    std::tuple<std::size_t&, std::size_t&, std::size_t&> param = std::tie(new_size, index_1, index_2);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_SIZE_2, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
{
    std::tuple<const std::vector<std::size_t>&, std::size_t&> param = std::tie(index_list, new_size);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_SIZE_VARIADIC, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(detail::argument arg)
{
    return m_data.m_policy(detail::variant_array_policy_operation::SET_VALUE, m_data.m_address, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&> param = std::tie(arg, index_1);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_VALUE_1, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, std::size_t index_2, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&, std::size_t&> param = std::tie(arg, index_1, index_2);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_VALUE_2, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&, std::size_t&, std::size_t&> param = std::tie(arg, index_1, index_2, index_3);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_VALUE_3, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::set_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg)
{
    std::tuple<detail::argument&, const std::vector<std::size_t>&> param = std::tie(arg, index_list);
    return m_data.m_policy(detail::variant_array_policy_operation::SET_VALUE_VARIADIC, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1) const
{
    std::tuple<variant, std::size_t> param = std::make_tuple(variant(), index_1);
    m_data.m_policy(detail::variant_array_policy_operation::GET_VALUE, m_data.m_address, param);
    return std::move(std::get<0>(param));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1, std::size_t index_2) const
{
    std::tuple<variant, std::size_t, std::size_t> param = std::make_tuple(variant(), index_1, index_2);
    m_data.m_policy(detail::variant_array_policy_operation::GET_VALUE_1, m_data.m_address, param);
    return std::move(std::get<0>(param));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
{
    std::tuple<variant, std::size_t, std::size_t, std::size_t> param = std::make_tuple(variant(), index_1, index_2, index_3);
    m_data.m_policy(detail::variant_array_policy_operation::GET_VALUE_1, m_data.m_address, param);
    return std::move(std::get<0>(param));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array_view::get_value_variadic(const std::vector<std::size_t>& index_list) const
{
    std::tuple<variant, const std::vector<std::size_t>&> param = std::make_tuple(variant(), index_list);
    m_data.m_policy(detail::variant_array_policy_operation::GET_VALUE_1, m_data.m_address, param);
    return std::move(std::get<0>(param));
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&> param = std::tie(arg, index_1);
    return m_data.m_policy(detail::variant_array_policy_operation::INSERT_VALUE, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, std::size_t index_2, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&, std::size_t&> param = std::tie(arg, index_1, index_2);
    return m_data.m_policy(detail::variant_array_policy_operation::INSERT_VALUE_1, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg)
{
    std::tuple<detail::argument&, std::size_t&, std::size_t&, std::size_t&> param = std::tie(arg, index_1, index_2, index_3);
    return m_data.m_policy(detail::variant_array_policy_operation::INSERT_VALUE_2, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::insert_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg)
{
    std::tuple<detail::argument&, const std::vector<std::size_t>&> param = std::tie(arg, index_list);
    return m_data.m_policy(detail::variant_array_policy_operation::INSERT_VALUE_VARIADIC, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1)
{
    return m_data.m_policy(detail::variant_array_policy_operation::REMOVE_VALUE, m_data.m_address, index_1);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1, std::size_t index_2)
{
    std::tuple<std::size_t&, std::size_t&> param = std::tie(index_1, index_2);
    return m_data.m_policy(detail::variant_array_policy_operation::REMOVE_VALUE_1, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
{
    std::tuple<std::size_t&, std::size_t&, std::size_t&> param = std::tie(index_1, index_2, index_3);
    return m_data.m_policy(detail::variant_array_policy_operation::REMOVE_VALUE_2, m_data.m_address, param);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array_view::remove_value_variadic(const std::vector<std::size_t>& index_list)
{
    return m_data.m_policy(detail::variant_array_policy_operation::REMOVE_VALUE_VARIADIC, m_data.m_address, index_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
