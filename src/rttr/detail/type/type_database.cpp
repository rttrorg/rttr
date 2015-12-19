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
    m_orig_names.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_custom_names.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_orig_name_to_id.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_custom_name_to_id.reserve(RTTR_DEFAULT_TYPE_COUNT);
    
    m_base_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_derived_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_conversion_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_get_derived_info_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    
    m_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_array_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_variant_create_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    
    m_type_size.reserve(RTTR_DEFAULT_TYPE_COUNT);

    m_is_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_enum_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_array_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_arithmetic_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_function_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_member_object_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_is_member_function_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_pointer_dim_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    
    
    // The following inserts are done, because we use the type_id directly
    // as index for the vector to access the following type information
    // type_id 0 is the invalid type, therfore we have to fill some dummy data
    m_orig_names.push_back("!invalid_type!");
    m_custom_names.push_back(m_orig_names[0]);
    
    m_base_class_list.push_back(0);
    m_derived_class_list.push_back(0);
    m_conversion_list.push_back(0);
    m_get_derived_info_func_list.push_back(nullptr);
    
    m_raw_type_list.push_back(0);
    m_wrapped_type_list.push_back(0);
    m_array_raw_type_list.push_back(0);
    m_variant_create_func_list.push_back(nullptr);
    
    m_type_size.push_back(0);

    m_is_class_list.push_back(false);
    m_is_enum_list.push_back(false);
    m_is_array_list.push_back(false);
    m_is_pointer_list.push_back(false);
    m_is_arithmetic_list.push_back(false);
    m_is_function_pointer_list.push_back(false);
    m_is_member_object_pointer_list.push_back(false);
    m_is_member_function_pointer_list.push_back(false);
    m_pointer_dim_list.push_back(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

type_database& type_database::instance()
{
    static type_database obj; 
    return obj; 
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_property(const type& t, unique_ptr<property_wrapper_base> prop)
{
    if (!t.is_valid())
        return;

    const auto name = prop->get_name();
    if (t.is_class())
    {
        if (get_class_property(t, name))
            return;
    }
    else
    {
        if (get_global_property(name))
            return;
    }
    const auto name_hash = generate_hash(name);
    if (t.is_class())
    {
        m_class_property_list.push_back({t.get_id(), get_class_property_count(t), name_hash, move(prop)});
        std::sort(m_class_property_list.begin(), m_class_property_list.end(), class_member<property_wrapper_base>::order());
    }
    else
    {
        m_global_property_list.push_back({name_hash, move(prop)});
        std::sort(m_global_property_list.begin(), m_global_property_list.end(), global_member<property_wrapper_base>::order());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

const property_wrapper_base* type_database::get_class_property(const type& t, const char* name) const
{
    using vec_value_type = class_member<property_wrapper_base>;
    const auto name_hash = generate_hash(name);
    const auto raw_type = t.get_raw_type();
    auto itr = std::lower_bound(m_class_property_list.cbegin(), m_class_property_list.cend(), 
                                vec_value_type{raw_type.get_id(), name_hash},
                                vec_value_type::order());
    for (; itr != m_class_property_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_class_id != raw_type.get_id())
            break;

        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) == 0)
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const property_wrapper_base*> type_database::get_all_class_properties(const type& t) const
{
    using member_type = property_wrapper_base;
    using vec_value_type = class_member<member_type>;
    
    const auto raw_type = t.get_raw_type();
    auto itr = std::lower_bound(m_class_property_list.cbegin(), m_class_property_list.cend(), 
                                vec_value_type{raw_type.get_id()}, vec_value_type::order());

    using sort_item = std::pair<uint16_t, member_type*>;
    std::vector<sort_item> sorted_vec;
    for (; itr != m_class_property_list.cend(); ++itr)
    {
        const auto& item = *itr;
        if (item.m_class_id != raw_type.get_id())
            break;

        sorted_vec.push_back({item.m_register_index, item.m_data.get()});
    }

    std::sort(sorted_vec.begin(), sorted_vec.end(), [](const sort_item& left, const sort_item& right)
                                                    { return left.first < right.first; });

    std::vector<const member_type*> result;
    result.reserve(sorted_vec.size());
    for (const auto& item : sorted_vec)
        result.push_back(item.second);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_class_property_count(const type& t) const
{
    return static_cast<uint16_t>(get_all_class_properties(t).size());
}


/////////////////////////////////////////////////////////////////////////////////////////

const property_wrapper_base* type_database::get_global_property(const char* name) const
{
    using vec_value_type = global_member<property_wrapper_base>;
    const auto name_hash = generate_hash(name);
    auto itr = std::lower_bound(m_global_property_list.cbegin(), m_global_property_list.cend(), 
                                vec_value_type{name_hash}, vec_value_type::order());
    for (; itr != m_global_property_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) == 0)
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const property_wrapper_base*> type_database::get_all_global_properties() const
{
    std::vector<const property_wrapper_base*> result;
    result.reserve(m_global_property_list.size());

    for (const auto& item : as_const(m_global_property_list))
        result.push_back(item.m_data.get());

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_global_property_count(const type& t) const
{
    return static_cast<uint16_t>(m_global_property_list.size());
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<type> convert_param_list(const vector<parameter_info>& param_list)
{
    std::vector<type> result;
    result.reserve(param_list.size());

    for (const auto& item : param_list)
        result.emplace_back(item.get_type());

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_method(const type& t, std::unique_ptr<method_wrapper_base> method)
{
    if (!t.is_valid())
        return;

    const auto name = method->get_name();
    if (t.is_class())
    {
        if (get_class_method(t, name, convert_param_list(method->get_parameter_infos())))
            return;
    }
    else
    {
        if (get_global_method(name, convert_param_list(method->get_parameter_infos())))
            return;
    }
    const auto name_hash = generate_hash(name);
    if (t.is_class())
    {
        auto foo = get_class_method_count(t);
        m_class_method_list.push_back({t.get_id(), get_class_method_count(t), name_hash, move(method)});
        std::sort(m_class_method_list.begin(), m_class_method_list.end(), class_member<method_wrapper_base>::order());
    }
    else
    {
        m_global_method_list.push_back({name_hash, move(method)});
        std::sort(m_global_method_list.begin(), m_global_method_list.end(), global_member<method_wrapper_base>::order());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_class_method(const type& t, const char* name) const
{
    using vec_value_type = class_member<method_wrapper_base>;
    const auto name_hash = generate_hash(name);
    const auto raw_type = t.get_raw_type();
    auto itr = std::lower_bound(m_class_method_list.cbegin(), m_class_method_list.cend(), 
                                vec_value_type{raw_type.get_id(), name_hash},
                                vec_value_type::order());
    for (; itr != m_class_method_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_class_id != raw_type.get_id())
            break;

        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) == 0)
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

struct compare_exact
{
    static RTTR_INLINE bool compare(const vector<parameter_info>& param_list, const vector<type>& arg_types)
    {
        const auto param_count = param_list.size();
        if (param_count != arg_types.size())
            return false;

        for (std::size_t index = 0; index < param_count; ++index)
        {
            if ((param_list[index].get_type() != arg_types[index]))
                return false;
        }

        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

struct compare_with_defaults
{
    static RTTR_INLINE bool compare(const vector<parameter_info>& param_list, const vector<argument>& args)
    {
        const auto param_count = param_list.size();
        const auto arg_count = args.size();
        if (arg_count > param_count)
            return false;

        std::size_t index = 0;
        for (; index < arg_count; ++index)
        {
            if ((param_list[index].get_type() != args[index].get_type()))
                return false;
        }

        // when there are still some parameter left, check if they are default values or not
        for (;index < param_count; ++index)
        {
            if (!param_list[index].has_default_value())
                return false;
        }

        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Container, typename Compare_Type>
const method_wrapper_base* type_database::get_class_method(const type& t, const char* name, const Container& container) const
{
    using vec_value_type = class_member<method_wrapper_base>;
    const auto name_hash = generate_hash(name);
    const auto raw_type = t.get_raw_type();
    auto itr = std::lower_bound(m_class_method_list.cbegin(), m_class_method_list.cend(), 
                                vec_value_type{raw_type.get_id(), name_hash},
                                vec_value_type::order());
    for (; itr != m_class_method_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_class_id != raw_type.get_id())
            break;

        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) != 0)
            continue;

        if (Compare_Type::compare(item.m_data->get_parameter_infos(), container))
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_class_method(const type& t, const char* name, const std::vector<type>& param_type_list) const
{
    return get_class_method<std::vector<type>, compare_exact>(t, name, param_type_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_class_method(const type& t, const char* name, 
                                                           const std::vector<argument>& arg_list) const
{
    return get_class_method<std::vector<argument>, compare_with_defaults>(t, name, arg_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const method_wrapper_base*> type_database::get_all_class_methods(const type& t) const
{
    using member_type = method_wrapper_base;
    using vec_value_type = class_member<member_type>;

    const auto raw_type = t.get_raw_type();
    auto itr = std::lower_bound(m_class_method_list.cbegin(), m_class_method_list.cend(), 
                                vec_value_type{raw_type.get_id()}, vec_value_type::order());

    using sort_item = std::pair<uint16_t, member_type*>;
    std::vector<sort_item> sorted_vec;
    for (; itr != m_class_method_list.cend(); ++itr)
    {
        const auto& item = *itr;
        if (item.m_class_id != raw_type.get_id())
            break;

        sorted_vec.push_back({item.m_register_index, item.m_data.get()});
    }

    std::sort(sorted_vec.begin(), sorted_vec.end(), [](const sort_item& left, const sort_item& right)
                                                    { return left.first < right.first; });
    
    std::vector<const member_type*> result;
    result.reserve(sorted_vec.size());
    for (const auto& item : sorted_vec)
        result.push_back(item.second);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_class_method_count(const type& t) const
{
    return static_cast<uint16_t>(get_all_class_methods(t).size());
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_global_method(const char* name) const
{
    using member_type = method_wrapper_base;
    using vec_value_type = global_member<member_type>;
    const auto name_hash = generate_hash(name);
    auto itr = std::lower_bound(m_global_method_list.cbegin(), m_global_method_list.cend(), 
                                vec_value_type{name_hash}, vec_value_type::order());
    for (; itr != m_global_method_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) == 0)
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Container, typename Compare_Type>
const method_wrapper_base* type_database::get_global_method(const char* name, const Container& container) const
{
    using member_type = method_wrapper_base;
    using vec_value_type = global_member<member_type>;
    const auto name_hash = generate_hash(name);
    auto itr = std::lower_bound(m_global_method_list.cbegin(), m_global_method_list.cend(), 
                                vec_value_type{name_hash}, vec_value_type::order());
    for (; itr != m_global_method_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_name_hash != name_hash)
            break;

        if (std::strcmp(item.m_data->get_name(), name) != 0)
            continue;

        if (Compare_Type::compare(item.m_data->get_parameter_infos(), container))
            return item.m_data.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_global_method(const char* name, const std::vector<type>& arg_type_list) const
{
    return get_global_method<std::vector<type>, compare_exact>(name, arg_type_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

const method_wrapper_base* type_database::get_global_method(const char* name, const std::vector<argument>& arg_list) const
{
    return get_global_method<std::vector<argument>, compare_with_defaults>(name, arg_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const method_wrapper_base*> type_database::get_all_global_methods() const
{
    std::vector<const method_wrapper_base*> result;
    result.reserve(m_global_method_list.size());

    for (const auto& item : as_const(m_global_method_list))
        result.push_back(item.m_data.get());

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_global_method_count(const type& t) const
{
    return static_cast<uint16_t>(m_global_method_list.size());
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE T* type_database::get_item_by_type(const type& t, const std::vector<type_data<T>>& vec)
{
    using vec_value_type = type_data<T>;
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
                                                   std::vector<type_data<T>>& vec)
{
    if (!t.is_valid())
        return;

    using data_type = type_data<T>;
    vec.push_back({t.get_id(), std::move(new_item)});
    std::stable_sort(vec.begin(), vec.end(), typename data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////

void type_database::register_constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor)
{
    // TO DO you cannot create constructor with the same argument type
    register_item_type(t, std::move(ctor), m_constructor_list);
}

/////////////////////////////////////////////////////////////////////////////////////

const constructor_wrapper_base* type_database::get_constructor(const type& t) const
{
    return get_item_by_type(t, m_constructor_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Container, typename Comparer_Type>
const constructor_wrapper_base* type_database::get_constructor(const type& t, const Container& container) const
{
    using vec_value_type = type_data<constructor_wrapper_base>;
    const auto id = t.get_id();
    auto itr = std::lower_bound(m_constructor_list.cbegin(), m_constructor_list.cend(), 
                                id, vec_value_type::order_by_id());
    for (; itr != m_constructor_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_id != id)
            break;

        if (Comparer_Type::compare(item.m_data->get_parameter_infos(), container))
            return item.m_data.get(); 
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

const constructor_wrapper_base* type_database::get_constructor(const type& t, const std::vector<type>& arg_type_list) const
{
    return get_constructor<std::vector<type>, compare_exact>(t, arg_type_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

const constructor_wrapper_base* type_database::get_constructor(const type& t, const std::vector<argument>& arg_list) const
{
    return get_constructor<std::vector<argument>, compare_with_defaults>(t, arg_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const constructor_wrapper_base*> type_database::get_constructors(const type& t) const
{
    using vec_value_type = type_data<constructor_wrapper_base>;
    const auto id = t.get_id();
    auto itr = std::lower_bound(m_constructor_list.cbegin(), m_constructor_list.cend(), 
                                id, vec_value_type::order_by_id());

    std::vector<const constructor_wrapper_base*> result;
    for (; itr != m_constructor_list.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_id != id)
            break;

        result.push_back(item.m_data.get());
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


void type_database::register_destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor)
{
    register_item_type(t, std::move(dtor), m_destructor_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

const destructor_wrapper_base* type_database::get_destructor(const type& t) const
{
    return get_item_by_type(t, m_destructor_list);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_item)
{
    register_item_type(t, std::move(enum_item), m_enumeration_list);
}

/////////////////////////////////////////////////////////////////////////////////////////

const enumeration_wrapper_base* type_database::get_enumeration(const type& t) const
{
    return get_item_by_type(t, m_enumeration_list);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_custom_name(const type& t, std::string custom_name)
{
    if (!t.is_valid())
        return;

    // TO DO normalize names
    m_custom_names[t.get_id()] = std::move(custom_name);
    const auto& custom_ref = m_custom_names[t.get_id()];
    std::string raw_name = type::normalize_orig_name(m_orig_names[t.get_id()]);
    for (auto& name_to_id : m_custom_name_to_id)
    {
        const auto& id = name_to_id.m_id;
        if (m_array_raw_type_list[id] == t.get_id())
        {
            m_custom_names[id] = derive_name(m_custom_names[id], raw_name, custom_ref);
            name_to_id.m_hash_value = generate_hash(m_custom_names[id]);
        }
    }
    
    std::sort(m_custom_name_to_id.begin(), m_custom_name_to_id.end(), name_to_id::order_by_name());
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
    
    using vec_value_type = type_data<type_converter_base>;
    m_type_converter_list.push_back({t.get_id(), std::move(converter)});
    std::stable_sort(m_type_converter_list.begin(), m_type_converter_list.end(), vec_value_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////

const type_converter_base* type_database::get_converter(const type& source_type, const type& target_type) const
{
    const auto src_id = source_type.get_id();
    const auto target_id = target_type.get_id();
    using vec_value_type = type_data<type_converter_base>;
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

    using data_type = type_data<const type_comparator_base*>;
    m_type_comparator_list.push_back({t.get_id(), comparator});
    std::stable_sort(m_type_comparator_list.begin(), m_type_comparator_list.end(), 
                     data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base* type_database::get_comparator(const type& t) const
{
    using vec_value_type = type_data<const type_comparator_base*>;
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

std::string type_database::derive_name(const type& array_raw_type, const char* name)
{
    if (!array_raw_type.is_valid())
        return type::normalize_orig_name(name); // this type is already the raw_type, so we have to forward just the current name

    type::type_id raw_id = array_raw_type.get_id();
    const auto custom_name = m_custom_names[raw_id];
    std::string raw_name_orig = type::normalize_orig_name(m_orig_names[raw_id]);

    const std::string src_name_orig = type::normalize_orig_name(name);
    return derive_name(src_name_orig, raw_name_orig, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type_database::register_name(const char* name, const type& array_raw_type, uint16_t& id)
{
    using namespace detail;

    const auto hash_value = generate_hash(name);
    // check if the name was already registered, then return with the already stored id
    auto itr = std::lower_bound(m_orig_name_to_id.cbegin(), m_orig_name_to_id.cend(), hash_value, name_to_id::order_by_name());
    for (; itr != m_orig_name_to_id.end(); ++itr)
    {
        auto& item = *itr;
        if (item.m_hash_value != hash_value)
            break;

        if (std::strcmp(m_orig_names[item.m_id], name) == 0)
        {
            id = static_cast<type::type_id>(item.m_id);
            return true;
        }
    }

    m_orig_name_to_id.emplace_back(name_to_id{++m_type_id_counter, hash_value});
    std::sort( m_orig_name_to_id.begin(), m_orig_name_to_id.end(), name_to_id::order_by_name());
    m_orig_names.push_back(name);

    auto custom_name = derive_name(array_raw_type, name);
    // TO DO hash normalized name (i.e. without spaces)
    m_custom_name_to_id.emplace_back(name_to_id{m_type_id_counter, generate_hash(custom_name)});
    std::sort(m_custom_name_to_id.begin(), m_custom_name_to_id.end(), name_to_id::order_by_name());
    m_custom_names.emplace_back(std::move(custom_name));

    id = m_type_id_counter;
    
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_base_class_info(const type& src_type, const type& raw_type, vector<base_class_info> base_classes)
{
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
    const std::size_t row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_type.get_id();
    m_base_class_list.resize(std::max(m_base_class_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT));
    m_conversion_list.resize(std::max(m_conversion_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT));
    std::size_t index = 0;
    // here we store for a type X all its base classes
    for (const auto& type : base_classes)
    {
        m_base_class_list[row + index] = type.m_base_type.get_id();
        m_conversion_list[row + index] = type.m_rttr_cast_func;
        ++index;
    }
    
    m_derived_class_list.resize(std::max(m_derived_class_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT));
    const auto id = src_type.get_id();
    // here we store for a type Y all its derived classes
    for (const auto& type : base_classes)
    {
        const std::size_t row = RTTR_MAX_INHERIT_TYPES_COUNT * type.m_base_type.get_raw_type().get_id();
        for (std::size_t i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
        {
            if (m_derived_class_list[row + i] == type::m_invalid_id)
            {
                m_derived_class_list[row + i] = id;
                break;
            }
                
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::register_type(const char* name, 
                                      const type& raw_type,
                                      const type& wrapped_type,
                                      const type& array_raw_type,
                                      vector<base_class_info> base_classes,
                                      get_derived_func derived_func_ptr,
                                      variant_create_func var_func_ptr,
                                      std::size_t type_size,
                                      bool is_class,
                                      bool is_enum,
                                      bool is_array,
                                      bool is_pointer,
                                      bool is_arithmetic,
                                      bool is_function_pointer,
                                      bool is_member_object_pointer,
                                      bool is_member_function_pointer,
                                      std::size_t pointer_dimension)
{
    type::init_globals();

    //std::lock_guard<std::mutex> lock(*g_register_type_mutex);

    using namespace detail;
    uint16_t id = 0;
    const bool isAlreadyRegistered = register_name(name, array_raw_type, id);
    if (isAlreadyRegistered)
        return id;

    // to do check: why return an invalid type anyway?
    const type::type_id raw_id = ((raw_type.get_id() == 0) ? id : raw_type.get_id());
    m_raw_type_list.push_back(raw_id);
    m_wrapped_type_list.push_back(wrapped_type.get_id());

    m_array_raw_type_list.push_back(array_raw_type.get_id() == 0 ? id : array_raw_type.get_id());
    m_get_derived_info_func_list.resize(std::max(m_get_derived_info_func_list.size(), static_cast<std::size_t>(raw_id + 1)));
    m_get_derived_info_func_list[raw_id]  = derived_func_ptr;
    m_variant_create_func_list.push_back(var_func_ptr);

    m_type_size.push_back(type_size);

    m_is_class_list.push_back(is_class);
    m_is_enum_list.push_back(is_enum);
    m_is_array_list.push_back(is_array);
    m_is_pointer_list.push_back(is_pointer);
    m_is_arithmetic_list.push_back(is_arithmetic);
    m_is_function_pointer_list.push_back(is_function_pointer);
    m_is_member_object_pointer_list.push_back(is_member_object_pointer);
    m_is_member_function_pointer_list.push_back(is_member_function_pointer);
    m_pointer_dim_list.push_back(pointer_dimension);

    // has to be done as last step
    register_base_class_info(id, type(raw_id), std::move(base_classes));

    return id;
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_by_name(const char* name) const
{
    // TO DO normalize name
    const auto hash_value = generate_hash(name);
    auto itr = std::lower_bound(m_custom_name_to_id.cbegin(), m_custom_name_to_id.cend(), hash_value, 
                                name_to_id::order_by_name());
    for (; itr != m_custom_name_to_id.cend(); ++itr)
    {
        auto& item = *itr;

        if (item.m_hash_value != hash_value)
            break;

        if (m_custom_names[item.m_id].compare(name) == 0)
            return item.m_id;
    }

    // TO DO not 100% fine, should use type::type_id or not as return value?
    return type::m_invalid_id;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
