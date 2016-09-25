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
:   m_type_id_counter(0)
{
    m_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_type_list.emplace_back(type(&get_invalid_type_data()));
    m_type_data_func_list.push_back(&get_invalid_type_data());
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

void type_database::register_custom_name(type& t, string_view custom_name)
{
    if (!t.is_valid())
        return;

    auto& type_data = *t.m_type_data;

    const auto& orig_name = type_data.name;
    m_custom_name_to_id.erase(orig_name);

    type_data.name = custom_name.to_string();

    m_custom_name_to_id.insert(std::make_pair(type_data.name, t));
    std::string raw_name = type::normalize_orig_name(t.m_type_data->type_name);
    const auto& t_name = type_data.name;

    auto tmp_type_list = m_custom_name_to_id.value_data();
    for (auto& tt : tmp_type_list)
    {
        if (tt.get_raw_array_type() == t)
        {
            const auto& orig_name_derived = tt.m_type_data->name;
            m_custom_name_to_id.erase(orig_name_derived);

            tt.m_type_data->name = derive_name(orig_name_derived, raw_name, t_name);
            m_custom_name_to_id.insert(std::make_pair(tt.m_type_data->name, tt));
        }
    }
}

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
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static void remove_whitespaces(std::string& text)
{
    text.erase(std::remove_if(text.begin(), text.end(), static_cast<int(*)(int)>(&std::isspace)), text.end());
}

/////////////////////////////////////////////////////////////////////////////////////////

static bool is_space_after(const std::string& text, const std::string& part)
{
    auto found_pos = text.find(part);

    if (found_pos == std::string::npos)
        return false;

    found_pos = found_pos + part.length();

    if (found_pos == std::string::npos || found_pos > text.length())
        return false;

    return std::isspace(text[found_pos]) ? true : false;
}

/////////////////////////////////////////////////////////////////////////////////////////

static bool is_space_before(const std::string& text, const std::string& part)
{
    auto found_pos = text.find_last_of(part);

    if (found_pos == std::string::npos)
        return false;

    found_pos = found_pos - 1;

    if (found_pos == std::string::npos || found_pos > text.length())
        return false;

    return std::isspace(text[found_pos]) ? true : false;
}

/////////////////////////////////////////////////////////////////////////////////////////

static void insert_space_after(std::string& text, const std::string& part)
{
    auto found_pos = text.find(part);

    if (found_pos == std::string::npos)
        return;

    found_pos = found_pos + part.length();

    if (found_pos == std::string::npos || found_pos > text.length())
        return;

    text.insert(found_pos, " ");
}

/////////////////////////////////////////////////////////////////////////////////////////

static void insert_space_before(std::string& text, const std::string& part)
{
   auto found_pos = text.find_last_of(part);

    if (found_pos == std::string::npos)
        return;

    if (found_pos == std::string::npos || found_pos > text.length())
        return;

    text.insert(found_pos, " ");
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type_database::derive_name(const std::string& src_name, const std::string& raw_name,
                                       const std::string& custom_name)
{
    auto tmp_src_name = src_name;
    auto tmp_raw_name = raw_name;

    // We replace a custom registered name for a type for all derived types, e.g.
    // "std::basic_string<char>" => "std::string"
    // we want to use this also automatically for derived types like pointers, e.g.
    // "const std::basic_string<char>*" => "const std::string*"
    // therefore we have to replace the "raw_type" string
    remove_whitespaces(tmp_raw_name);
    remove_whitespaces(tmp_src_name);

    const auto start_pos = tmp_src_name.find(tmp_raw_name);
    const auto end_pos = start_pos + tmp_raw_name.length();
    if (start_pos == std::string::npos)
        return src_name; // nothing was found...

    // remember the two parts before and after the found "raw_name"
    const auto start_part = tmp_src_name.substr(0, start_pos);
    const auto end_part = tmp_src_name.substr(end_pos, tmp_src_name.length());

    tmp_src_name.replace(start_pos, tmp_raw_name.length(), custom_name);

    if (is_space_after(src_name, start_part))
        insert_space_after(tmp_src_name, start_part);

    if (is_space_before(src_name, end_part))
        insert_space_before(tmp_src_name, end_part);

    return tmp_src_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type_database::derive_name(const type_data& array_raw_type, string_view name)
{
    if (!array_raw_type.is_valid())
        return type::normalize_orig_name(name); // this type is already the raw_type, so we have to forward just the current name

    const auto& custom_name = array_raw_type.name;
    std::string raw_name_orig = type::normalize_orig_name(array_raw_type.type_name);

    const std::string src_name_orig = type::normalize_orig_name(name);
    return derive_name(src_name_orig, raw_name_orig, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type_database::register_name(uint16_t& id, type_data& info)
{
    using namespace detail;

    auto ret = m_orig_name_to_id.find(info.type_name);
    if (ret != m_orig_name_to_id.end())
    {
        id = (*ret).get_id();
        return true;
    }

    ++m_type_id_counter;

    m_orig_name_to_id.insert(std::make_pair(info.type_name, type(&info)));
    info.name = derive_name(*info.array_raw_type, info.type_name);
    m_custom_name_to_id.insert(std::make_pair(info.name, type(&info)));

    id = m_type_id_counter;
    info.type_index = id;
    m_type_list.emplace_back(type(&info));

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_base_class_info(type_data& info)
{
    auto base_classes = info.get_base_types();

    // remove double entries; can only be happen for virtual inheritance case
    set<type> double_entries;
    for (auto itr = base_classes.rbegin(); itr != base_classes.rend();)
    {
        if (double_entries.find(itr->m_base_type) == double_entries.end())
        {
            double_entries.insert(itr->m_base_type);
            ++itr;
        }
        else
        {
            itr = vector<base_class_info>::reverse_iterator(base_classes.erase((++itr).base()));
        }
    }

    // sort the base classes after it registration index, that means the root class is always the first in the list,
    // followed by its derived classes, here it depends on the order of RTTR_ENABLE(CLASS)
    std::sort(base_classes.begin(), base_classes.end(), [](const base_class_info& left, const base_class_info& right)
                                                          { return left.m_base_type.get_id() < right.m_base_type.get_id(); });

    auto& class_data = info.get_class_data();
    for (const auto& t : base_classes)
    {
        class_data.m_base_types.push_back(t.m_base_type);
        class_data.m_conversion_list.push_back(t.m_rttr_cast_func);

        auto r_type = t.m_base_type.get_raw_type();
        r_type.m_type_data->get_class_data().m_derived_types.push_back(type(&info));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type_data*>& type_database::get_type_data_func()
{
    return m_type_data_func_list;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

type type_database::register_type(type_data& info) RTTR_NOEXCEPT
{
    // register the base types
    info.get_base_types();

    //std::lock_guard<std::mutex> lock(*g_register_type_mutex);
    using namespace detail;
    uint16_t id = 0;
    const bool isAlreadyRegistered = register_name(id, info);
    if (isAlreadyRegistered)
        return type(m_type_data_func_list[id]);

    info.raw_type_data  = !info.raw_type_data->is_valid() ? &info : info.raw_type_data;
    info.array_raw_type = !info.array_raw_type->is_valid() ? &info : info.array_raw_type;

    m_type_data_func_list.push_back(&info);

    // has to be done as last step
    register_base_class_info(info);

    return type(m_type_data_func_list[id]);
}

/////////////////////////////////////////////////////////////////////////////////////////

type type_database::get_by_name(string_view name) const
{
    auto ret = m_custom_name_to_id.find(name);
    if (ret != m_custom_name_to_id.end())
        return (*ret);

    return get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
