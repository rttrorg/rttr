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
    m_orig_names.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_custom_names.reserve(RTTR_DEFAULT_TYPE_COUNT);

    m_base_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_derived_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_conversion_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
    m_get_derived_info_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);

    m_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_array_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_variant_create_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);

    m_type_size.reserve(RTTR_DEFAULT_TYPE_COUNT);
    m_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);

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
    m_orig_names.emplace_back("!invalid_type!");
    m_custom_names.emplace_back(m_orig_names[0]);

    m_base_class_list.emplace_back(type(0, nullptr));
    m_derived_class_list.emplace_back(type(0, nullptr));
    m_conversion_list.emplace_back(nullptr);
    m_get_derived_info_func_list.push_back(nullptr);

    m_raw_type_list.emplace_back(0);
    m_wrapped_type_list.emplace_back(0);
    m_array_raw_type_list.emplace_back(0);
    m_variant_create_func_list.push_back(nullptr);

    m_type_size.push_back(0);
    m_type_list.emplace_back(type(0, nullptr));

    m_is_class_list.push_back(false);
    m_is_enum_list.push_back(false);
    m_is_array_list.push_back(false);
    m_is_pointer_list.push_back(false);
    m_is_arithmetic_list.push_back(false);
    m_is_function_pointer_list.push_back(false);
    m_is_member_object_pointer_list.push_back(false);
    m_is_member_function_pointer_list.push_back(false);
    m_pointer_dim_list.push_back(0);

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

template<typename T>
static array_range<T> get_items_for_type(const type& t,
                                         const std::unordered_map<type, std::vector<T>>& class_map)
{
    const auto ret = class_map.find(t);
    if (ret != class_map.end())
    {
        auto& vec = ret->second;
        return array_range<T>(vec.data(), vec.size(),
                              default_predicate<T>([t](const T& item) { return (item.get_declaring_type() == t); }) );
    }

    return array_range<T>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static void update_class_list(const type& t,
                              std::unordered_map<type, std::vector<T>>& class_map)
{
    // update type "t" with all items from the base classes
    auto& all_items_list = class_map[t];
    auto item_range = get_items_for_type<T>(t, class_map);
    std::vector<T> item_vec(item_range.begin(), item_range.end());
    all_items_list.reserve(all_items_list.size() + 1);
    all_items_list.clear(); // this will not reduce the capacity, i.e. new memory allocation may not necessary
    for (const auto& base_type : t.get_base_classes())
    {
        auto base_item_range = get_items_for_type<T>(base_type, class_map);
        if (base_item_range.empty())
            continue;

        all_items_list.reserve(all_items_list.size() + base_item_range.size());
        all_items_list.insert(all_items_list.end(), base_item_range.begin(), base_item_range.end());
    }

    // insert own class items
    all_items_list.reserve(all_items_list.size() + item_vec.size());
    all_items_list.insert(all_items_list.end(), item_vec.begin(), item_vec.end());

    // update derived types
    for (const auto& derived_type : t.get_derived_classes())
        update_class_list<T>(derived_type, class_map);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static T get_class_item(const type& t, string_view name,
                        const std::unordered_map<type, std::vector<T>>& item_map)
{
    const auto ret = item_map.find(t);
    if (ret != item_map.end())
    {
        const auto& vec = ret->second;
        auto ret = std::find_if(vec.cbegin(), vec.cend(),
        [name](const T& item)
        {
            return (item.get_name() == name);
        });
        if (ret != vec.cend())
            return *ret;
    }

    return detail::create_invalid_item<T>();
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_property(const type& t, unique_ptr<property_wrapper_base> prop)
{
    if (!t.is_valid())
        return;

    const auto name = prop->get_name();

    if (t.is_class())
    {
        if (get_type_property(t, name))
            return;

        m_class_property_map[t].emplace_back(detail::create_item<property>(prop.get()));
        m_property_list.push_back(std::move(prop));
        update_class_list<property>(t, m_class_property_map);
    }
    else
    {
        if (get_global_property(name))
            return;

        property p = detail::create_item<property>(prop.get());
        m_global_properties.insert(std::move(name), std::move(p));
        m_property_list.push_back(std::move(prop));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

property type_database::get_class_property(const type& t, string_view name) const
{
    return get_class_item<property>(t, name, m_class_property_map);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type_database::get_type_property(const type& t, string_view name) const
{
    for (const auto& prop : get_items_for_type<property>(t, m_class_property_map))
    {
        if (prop.get_name() == name)
            return prop;
    }

    return create_invalid_item<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type_database::get_class_properties(const type& t) const
{
    const auto ret = m_class_property_map.find(t);
    if (ret != m_class_property_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
        {
            return array_range<property>(vec.data(), vec.size(),
                                         default_predicate<property>([](const property& prop)
                                         {
                                             return (prop.get_access_level() == access_levels::public_access);
                                         }) );
        }
    }

    return array_range<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

static bool is_valid_filter_item(filter_items filter)
{
    if ((filter.test_flag(filter_item::public_access) ||
         filter.test_flag(filter_item::non_public_access)) &&
        (filter.test_flag(filter_item::instance_item) ||
        filter.test_flag(filter_item::static_item)))
    {
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE bool filter_member_item(const T& item, const type& t, filter_items filter)
{
    bool result = true;

    if (filter.test_flag(filter_item::public_access) && filter.test_flag(filter_item::non_public_access))
    {
        result &= true;
    }
    else if (filter.test_flag(filter_item::public_access))
    {
        result &= (item.get_access_level() == access_levels::public_access);
    }
    else if (filter.test_flag(filter_item::non_public_access))
    {
        const auto access_level = item.get_access_level();
        result &= (access_level == access_levels::private_access || access_level == access_levels::protected_access);
    }

    if (filter.test_flag(filter_item::instance_item) && filter.test_flag(filter_item::static_item))
        result &= true;
    else if (filter.test_flag(filter_item::instance_item) && !filter.test_flag(filter_item::static_item))
        result &= !item.is_static();
    else if (!filter.test_flag(filter_item::instance_item) && filter.test_flag(filter_item::static_item))
        result &= item.is_static();

    if (filter.test_flag(filter_item::declared_only))
        result &= (item.get_declaring_type() == t);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE default_predicate<T> get_filter_predicate(const type& t, filter_items filter)
{
    if (!is_valid_filter_item(filter))
    {
        return {[](const T&){ return false; }};
    }
    else
    {
        return {[filter, t](const T& item)
        {
            return filter_member_item<T>(item, t, filter);
        }};
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type_database::get_class_properties(const type& t, filter_items filter) const
{
    const auto ret = m_class_property_map.find(t);
    if (ret != m_class_property_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
            return array_range<property>(vec.data(), vec.size(), get_filter_predicate<property>(t, filter));
    }

    return array_range<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

property type_database::get_global_property(string_view name) const
{
    const auto ret = m_global_properties.find(name);
    if (ret != m_global_properties.end())
        return *ret;

    return detail::create_invalid_item<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type_database::get_global_properties()
{
    auto& vec = m_global_properties.value_data();
    return array_range<property>(vec.data(), vec.size());
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<type> convert_param_list(const array_range<parameter_info>& param_list)
{
    std::vector<type> result;
    result.reserve(param_list.size());

    for (const auto& item : param_list)
        result.emplace_back(item.get_type());

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_method(const type& t, std::unique_ptr<method_wrapper_base> meth)
{
    if (!t.is_valid())
        return;

    const auto name = meth->get_name();
    if (t.is_class())
    {
        if (get_type_method(t, name, convert_param_list(meth->get_parameter_infos())))
            return;

        m_class_method_map[t].emplace_back(detail::create_item<method>(meth.get()));
        m_method_list.push_back(std::move(meth));
        update_class_list<method>(t, m_class_method_map);
    }
    else
    {
        if (get_global_method(name, convert_param_list(meth->get_parameter_infos())))
            return;

        method m = detail::create_item<method>(meth.get());
        m_global_methods.insert(std::move(name), std::move(m));
        m_method_list.push_back(std::move(meth));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_type_method(const type& t, string_view name) const
{
    for (const auto& meth : get_items_for_type<method>(t, m_class_method_map))
    {
        if (meth.get_name() == name)
        {
            return meth;
        }
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_class_method(const type& t, string_view name) const
{
    return get_class_item<method>(t, name, m_class_method_map);
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_type_method(const type& t, string_view name,
                                      const std::vector<type>& type_list) const
{
    for (const auto& meth : get_items_for_type<method>(t, m_class_method_map))
    {
        if (meth.get_name() == name &&
            detail::compare_with_type_list::compare(meth.get_parameter_infos(), type_list))
        {
            return meth;
        }
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_class_method(const type& t, string_view name,
                                       const std::vector<type>& type_list) const
{
    const auto ret = m_class_method_map.find(t);
    if (ret != m_class_method_map.end())
    {
        for (const auto& meth : ret->second)
        {
            if ( meth.get_name() == name &&
                 detail::compare_with_type_list::compare(meth.get_parameter_infos(), type_list))
            {
                return meth;
            }
        }
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_class_method(const type& t, string_view name,
                                       const std::vector<argument>& arg_list) const
{
    const auto ret = m_class_method_map.find(t);
    if (ret != m_class_method_map.end())
    {
        for (const auto& meth : ret->second)
        {
            if ( meth.get_name() == name &&
                 detail::compare_with_arg_list::compare(meth.get_parameter_infos(), arg_list))
            {
                return meth;
            }
        }
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type_database::get_class_methods(const type& t) const
{
    const auto ret = m_class_method_map.find(t);
    if (ret != m_class_method_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
        {
            return array_range<method>(vec.data(), vec.size(),
                                       default_predicate<method>([](const method& meth)
                                       {
                                           return (meth.get_access_level() == access_levels::public_access);
                                       }) );
        }
    }

    return array_range<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type_database::get_class_methods(const type& t, filter_items filter) const
{
    const auto ret = m_class_method_map.find(t);
    if (ret != m_class_method_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
            return array_range<method>(vec.data(), vec.size(), get_filter_predicate<method>(t, filter));
    }

    return array_range<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_global_method(string_view name) const
{
    const auto ret = m_global_methods.find(name);
    if (ret != m_global_methods.end())
        return *ret;

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_global_method(string_view name, const std::vector<type>& type_list) const
{
    auto itr = m_global_methods.find(name);
    while (itr != m_global_methods.end())
    {
        const auto& meth = *itr;
        if (meth.get_name() != name)
            break;

        if (detail::compare_with_type_list::compare(meth.get_parameter_infos(), type_list))
            return meth;

        ++itr;
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_database::get_global_method(string_view name, const std::vector<argument>& arg_list) const
{
    auto itr = m_global_methods.find(name);
    while (itr != m_global_methods.end())
    {
        const auto& meth = *itr;
        if (meth.get_name() != name)
            break;

        if (detail::compare_with_arg_list::compare(meth.get_parameter_infos(), arg_list))
            return meth;

        ++itr;
    }

    return detail::create_invalid_item<method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type_database::get_global_methods()
{
    auto& vec = m_global_methods.value_data();
    return array_range<method>(vec.data(), vec.size());
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
    if (!t.is_valid())
        return;

    // TO DO you cannot create constructor with the same argument type
    m_type_ctor_map[t].emplace_back(detail::create_item<constructor>(ctor.get()));
    m_constructor_list.push_back(std::move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////

constructor type_database::get_constructor(const type& t) const
{
    auto ret = m_type_ctor_map.find(t);
    if (ret != m_type_ctor_map.cend())
    {
        const auto& vec = ret->second;
        if (!vec.empty())
            return vec.front();
    }

    return create_invalid_item<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type_database::get_constructor(const type& t, const std::vector<type>& arg_type_list) const
{
    auto ret = m_type_ctor_map.find(t);
    if (ret != m_type_ctor_map.cend())
    {
        for (const auto& ctor : ret->second)
        {
            if (detail::compare_with_type_list::compare(ctor.get_parameter_infos(), arg_type_list))
                return ctor;
        }
    }

    return create_invalid_item<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type_database::get_constructor(const type& t, const std::vector<argument>& arg_list) const
{
    auto ret = m_type_ctor_map.find(t);
    if (ret != m_type_ctor_map.cend())
    {
        for (const auto& ctor : ret->second)
        {
            if (detail::compare_with_arg_list::compare(ctor.get_parameter_infos(), arg_list))
                return ctor;
        }
    }

    return create_invalid_item<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type_database::get_constructors(const type& t)
{
    const auto ret = m_type_ctor_map.find(t);
    if (ret != m_type_ctor_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
            return array_range<constructor>(vec.data(), vec.size(),
                                            default_predicate<constructor>([](const constructor& ctor)
                                            {
                                                return (ctor.get_access_level() == access_levels::public_access);
                                            }) );
    }

    return array_range<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
RTTR_FORCE_INLINE default_predicate<constructor> get_filter_predicate(const type& t, filter_items filter)
{
    if (!is_valid_filter_item(filter))
    {
        return {[](const constructor&){ return false; }};
    }
    else
    {
        return {[filter](const constructor& item)
        {
            bool result = true;

            if (filter.test_flag(filter_item::public_access) && filter.test_flag(filter_item::non_public_access))
            {
                result &= true;
            }
            else if (filter.test_flag(filter_item::public_access))
            {
                result &= (item.get_access_level() == access_levels::public_access);
            }
            else if (filter.test_flag(filter_item::non_public_access))
            {
                const auto access_level = item.get_access_level();
                result &= (access_level == access_levels::private_access || access_level == access_levels::protected_access);
            }

            return result;
        }};
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type_database::get_constructors(const type& t, filter_items filter) const
{
    const auto ret = m_type_ctor_map.find(t);
    if (ret != m_type_ctor_map.end())
    {
        auto& vec = ret->second;
        if (!vec.empty())
            return array_range<constructor>(vec.data(), vec.size(), get_filter_predicate<constructor>(t, filter));
    }

    return array_range<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_database::register_destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor)
{
    auto d = detail::create_item<destructor>(dtor.get());
    const auto ret = m_type_dtor_map.insert(std::make_pair(t, d));
    if (ret.second)
        m_destructor_list.push_back(std::move(dtor));
}

/////////////////////////////////////////////////////////////////////////////////////////

destructor type_database::get_destructor(const type& t) const
{
    auto ret = m_type_dtor_map.find(t);
    if (ret != m_type_dtor_map.end())
        return ret->second;
    else
        return create_invalid_item<destructor>();
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

void type_database::register_custom_name(const type& t, string_view custom_name)
{
    if (!t.is_valid())
        return;

    // TO DO normalize names
    const auto& orig_name = m_custom_names[t.get_id()];
    m_custom_name_to_id.erase(orig_name);

    m_custom_names[t.get_id()] = custom_name.to_string();
    m_custom_name_to_id.insert(std::make_pair(m_custom_names[t.get_id()], t.get_id()));
    std::string raw_name = type::normalize_orig_name(m_orig_names[t.get_id()]);
    const auto& t_name = m_custom_names[t.get_id()];

    auto tmp_id_list = m_custom_name_to_id.value_data();
    for (const auto& id : tmp_id_list)
    {
        if (m_array_raw_type_list[id] == t.get_id())
        {
            const auto& orig_name_derived = m_custom_names[id];
            m_custom_name_to_id.erase(orig_name_derived);

            m_custom_names[id] = derive_name(m_custom_names[id], raw_name, t_name);
            m_custom_name_to_id.insert(std::make_pair(m_custom_names[id], id));
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

std::string type_database::derive_name(const type& array_raw_type, string_view name)
{
    if (!array_raw_type.is_valid())
        return type::normalize_orig_name(name); // this type is already the raw_type, so we have to forward just the current name

    type::type_id raw_id = array_raw_type.get_id();
    const auto& custom_name = m_custom_names[raw_id];
    std::string raw_name_orig = type::normalize_orig_name(m_orig_names[raw_id]);

    const std::string src_name_orig = type::normalize_orig_name(name);
    return derive_name(src_name_orig, raw_name_orig, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type_database::register_name(string_view name, const type& array_raw_type, uint16_t& id,
                                  const type_data_funcs& info)
{
    using namespace detail;

    auto ret = m_orig_name_to_id.find(name);
    if (ret != m_orig_name_to_id.end())
    {
        id = *ret;
        return true;
    }

    m_orig_name_to_id.insert(std::make_pair(name, ++m_type_id_counter));
    m_orig_names.push_back(name);

    auto custom_name = derive_name(array_raw_type, name);
    m_custom_names.emplace_back(std::move(custom_name));
    m_custom_name_to_id.insert(std::make_pair(m_custom_names.back(), m_type_id_counter));

    id = m_type_id_counter;
    m_type_list.emplace_back(type(id, &info));

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
    m_base_class_list.resize(std::max(m_base_class_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT), detail::get_invalid_type());
    m_conversion_list.resize(std::max(m_conversion_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT));
    std::size_t index = 0;
    // here we store for a type X all its base classes
    for (const auto& type : base_classes)
    {
        m_base_class_list[row + index] = type.m_base_type;
        m_conversion_list[row + index]  = type.m_rttr_cast_func;
        ++index;
    }

    m_derived_class_list.resize(std::max(m_derived_class_list.size(), row + RTTR_MAX_INHERIT_TYPES_COUNT), detail::get_invalid_type());
    const auto id = src_type.get_id();
    // here we store for a type Y all its derived classes
    for (const auto& type : base_classes)
    {
        const std::size_t row = RTTR_MAX_INHERIT_TYPES_COUNT * type.m_base_type.get_raw_type().get_id();
        for (std::size_t i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
        {
            if (m_derived_class_list[row + i].get_id() == type::m_invalid_id)
            {
                m_derived_class_list[row + i] = src_type;
                break;
            }

        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<const type_data_funcs*>& type_database::get_type_data_func()
{
    return m_type_data_func_list;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

type type_database::register_type(string_view name,
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
                                  std::size_t pointer_dimension,
                                  const type_data_funcs& info) RTTR_NOEXCEPT
{
    type::init_globals();

    //std::lock_guard<std::mutex> lock(*g_register_type_mutex);

    using namespace detail;
    uint16_t id = 0;
    const bool isAlreadyRegistered = register_name(name, array_raw_type, id, info);
    if (isAlreadyRegistered)
        return type(id, m_type_data_func_list[id]);

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
    m_type_data_func_list.push_back(&info);

    // has to be done as last step
    register_base_class_info(type(id, nullptr), type(raw_id, nullptr), std::move(base_classes));

    return type(id, m_type_data_func_list[id]);
}

/////////////////////////////////////////////////////////////////////////////////////////

uint16_t type_database::get_by_name(string_view name) const
{
    auto ret = m_custom_name_to_id.find(name);
    if (ret != m_custom_name_to_id.end())
        return *ret;

    return type::m_invalid_id;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
