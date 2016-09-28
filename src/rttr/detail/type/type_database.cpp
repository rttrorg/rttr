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

#include "rttr/detail/type/type_database_p.h"

#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/destructor/destructor_wrapper_base.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/parameter_info/parameter_infos_compare.h"
#include "rttr/detail/type/type_data.h"
#include "rttr/detail/filter/filter_item_funcs.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include <cctype>
#include <cstring>

using namespace std;


namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

type_database::type_database()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

type_database::~type_database()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

type_database& type_database::instance()
{
    static type_database obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE T* type_database::get_item_by_type(const type& t, const std::vector<data_container<T>>& vec)
{
    using vec_value_type = data_container<T>;
    const auto id = t.get_id();
    auto itr = std::lower_bound(vec.cbegin(), vec.cend(), id, typename vec_value_type::order_by_id());
    for (; itr != vec.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_id == id)
            return item.m_data.get();
        else
            break;
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE void type_database::register_item_type(const type& t, std::unique_ptr<T> new_item,
                                                   std::vector<data_container<T>>& vec)
{
    if (!t.is_valid())
        return;

    using data_type = data_container<T>;
    vec.push_back({t.get_id(), std::move(new_item)});
    std::stable_sort(vec.begin(), vec.end(), typename data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_item)
{
    register_item_type(t, std::move(enum_item), m_enumeration_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration type_database::get_enumeration(const type& t) const
{
    if (auto item = get_item_by_type(t, m_enumeration_list))
        return create_item<enumeration>(item);
    else
        return create_invalid_item<enumeration>();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_metadata(const type& t, std::vector<metadata> data)
{
    if (!t.is_valid() || data.empty())
        return;

    auto meta_vec = get_metadata_list(t);

    if (!meta_vec)
    {
        auto new_meta_vec = detail::make_unique<std::vector<metadata>>(data.cbegin(), data.cend());
        meta_vec = new_meta_vec.get();
        register_item_type(t, std::move(new_meta_vec), m_metadata_type_list);
    }

    auto meta_vec_ref = *meta_vec;

    // when we insert new items, we want to check first whether a item with same key exist => ignore this data
    for (auto& new_item : data)
    {
        if (get_metadata(new_item,  meta_vec_ref).is_valid() == false)
            meta_vec_ref.emplace_back(std::move(new_item));
    }

    std::sort(meta_vec_ref.begin(), meta_vec_ref.end(), metadata::order_by_key());
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type_database::get_metadata(const type& t, const variant& key) const
{
    auto meta_vec = get_metadata_list(t);
    return (meta_vec ? get_metadata(key, *meta_vec) : variant());
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type_database::get_metadata(const variant& key, const std::vector<metadata>& data) const
{
    auto itr = std::lower_bound(data.cbegin(), data.cend(), key, metadata::order_by_key());
    if (itr != data.cend())
    {
        auto& item = *itr;
        if (item.get_key() == key)
            return item.get_value();
    }

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<metadata>* type_database::get_metadata_list(const type& t) const
{
    return get_item_by_type(t, m_metadata_type_list);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_converter(const type& t, std::unique_ptr<type_converter_base> converter)
{
    if (!t.is_valid())
        return;

    if (get_converter(t, converter->m_target_type))
        return;

    using vec_value_type = data_container<type_converter_base>;
    m_type_converter_list.push_back({t.get_id(), std::move(converter)});
    std::stable_sort(m_type_converter_list.begin(), m_type_converter_list.end(), vec_value_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////

const type_converter_base* type_database::get_converter(const type& source_type, const type& target_type) const
{
    const auto src_id = source_type.get_id();
    const auto target_id = target_type.get_id();
    using vec_value_type = data_container<type_converter_base>;
    auto itr = std::lower_bound(m_type_converter_list.cbegin(), m_type_converter_list.cend(),
                                src_id, vec_value_type::order_by_id());
    for (; itr != m_type_converter_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_id != src_id)
            break; // type not found

        if (item.m_data->m_target_type.get_id() == target_id)
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_comparator(const type& t, const type_comparator_base* comparator)
{
    if (!t.is_valid())
        return;

    using data_type = data_container<const type_comparator_base*>;
    m_type_comparator_list.push_back({t.get_id(), comparator});
    std::stable_sort(m_type_comparator_list.begin(), m_type_comparator_list.end(),
                     data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base* type_database::get_comparator(const type& t) const
{
    using vec_value_type = data_container<const type_comparator_base*>;
    const auto id = t.get_id();
    auto itr = std::lower_bound(m_type_comparator_list.cbegin(), m_type_comparator_list.cend(), id,
                                vec_value_type::order_by_id());
    if (itr != m_type_comparator_list.cend() && itr->m_id == id)
        return itr->m_data;
    else
        return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
