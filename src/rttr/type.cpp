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

#include "rttr/type.h"

#include "rttr/constructor.h"
#include "rttr/property.h"
#include "rttr/destructor.h"
#include "rttr/enumeration.h"
#include "rttr/method.h"

#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/destructor/destructor_wrapper_base.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/rttr_enable.h"

#include "rttr/detail/type/type_database_p.h"
#include "rttr/detail/parameter_info/parameter_infos_compare.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <thread>
#include <mutex>
#include <cstring>
#include <cctype>
#include <utility>

using namespace std;

namespace rttr
{
namespace
{

/////////////////////////////////////////////////////////////////////////////////////////

bool rotate_char_when_whitespace_before(std::string& text, std::string::size_type& pos, char c)
{
    auto result = text.find(c, pos);
    if (result != std::string::npos && result > 0)
    {
        if (::isspace(text[result - 1]))
        {
            text[result - 1] = c;
            text[result] = ' ';
        }
        pos = result + 1;
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void move_pointer_and_ref_to_type(std::string& type_name)
{
    std::string::size_type pos = 0;
    while(pos < type_name.length())
    {
        if (!rotate_char_when_whitespace_before(type_name, pos, '*') &&
            !rotate_char_when_whitespace_before(type_name, pos, '&') &&
            !rotate_char_when_whitespace_before(type_name, pos, ')'))
        {
            pos = std::string::npos;
        }
    }

    const auto non_whitespace = type_name.find_last_not_of(' ');
    type_name.resize(non_whitespace + 1);
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
static detail::default_predicate<T> get_filter_predicate(const type& t, filter_items filter)
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

template<>
static detail::default_predicate<constructor> get_filter_predicate(const type& t, filter_items filter)
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

template<typename T>
static array_range<T> get_items_for_type(const type& t,
                                         const std::vector<T>& vec)
{
    return array_range<T>(vec.data(), vec.size(),
                          detail::default_predicate<T>([t](const T& item) { return (item.get_declaring_type() == t); }) );
}

/////////////////////////////////////////////////////////////////////////////////////////

static detail::flat_multimap<string_view, property>& get_global_property_storage()
{
    static detail::flat_multimap<string_view, property> props;
    return props;
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<std::unique_ptr<detail::property_wrapper_base>>& get_property_storage()
{
    static std::vector<std::unique_ptr<detail::property_wrapper_base>> container;
    return container;
}

} // end anonymous namespace

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

std::string type::normalize_orig_name(string_view name)
{
    std::string normalized_name = name.to_string();

    move_pointer_and_ref_to_type(normalized_name);
    return normalized_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_derived_from(const type& other) const RTTR_NOEXCEPT
{
    auto& src_raw_type = m_type_data->raw_type_data;
    auto& tgt_raw_type = other.m_type_data->raw_type_data;

    if (src_raw_type == tgt_raw_type)
        return true;

    for (auto& t : src_raw_type->get_class_data().m_base_types)
    {
        if (t.m_type_data == tgt_raw_type)
        {
            return true;
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void* type::apply_offset(void* ptr, const type& source_type, const type& target_type) RTTR_NOEXCEPT
{
    auto& src_raw_type = source_type.m_type_data->raw_type_data;
    auto& tgt_raw_type = target_type.m_type_data->raw_type_data;

    if (src_raw_type == tgt_raw_type || ptr == nullptr)
        return ptr;

    const detail::derived_info info = src_raw_type->get_class_data().m_derived_info_func(ptr);
    if (info.m_type.m_type_data->raw_type_data == tgt_raw_type)
        return info.m_ptr;

    auto& class_list = info.m_type.m_type_data->raw_type_data->get_class_data();
    int i = 0;
    for (auto& t : class_list.m_base_types)
    {
        if (t.m_type_data == tgt_raw_type)
        {
            return class_list.m_conversion_list[i](info.m_ptr);
        }
        ++i;
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_derived_type(void* ptr, const type& source_type) RTTR_NOEXCEPT
{
    if (ptr == nullptr)
        return type();

    auto& src_raw_type = source_type.m_type_data->raw_type_data;
    const detail::derived_info info = src_raw_type->get_class_data().m_derived_info_func(ptr);
    return info.m_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_base_classes() const RTTR_NOEXCEPT
{
    return array_range<type>(m_type_data->get_class_data().m_base_types.data(),
                             m_type_data->get_class_data().m_base_types.size());
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_derived_classes() const RTTR_NOEXCEPT
{
    return array_range<type>(m_type_data->get_class_data().m_derived_types.data(),
                             m_type_data->get_class_data().m_derived_types.size());
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_types() RTTR_NOEXCEPT
{
    auto& type_list = detail::type_database::instance().m_type_list;
    return array_range<type>(&type_list[1], type_list.size() - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_metadata(const variant& key) const
{
    return detail::type_database::instance().get_metadata(*this, key);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create(vector<argument> args) const
{
    auto& ctors = m_type_data->get_class_data().m_ctors;
    for (const auto& ctor : ctors)
    {
        if (detail::compare_with_arg_list::compare(ctor.get_parameter_infos(), args))
            return ctor.invoke_variadic(std::move(args));
    }

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::destroy(variant& obj) const RTTR_NOEXCEPT
{
    return get_destructor().invoke(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_property(string_view name) const RTTR_NOEXCEPT
{
    const auto& vec = get_raw_type().m_type_data->get_class_data().m_properties;
    auto ret = std::find_if(vec.cbegin(), vec.cend(),
                            [name](const property& item)
                            {
                                return (item.get_name() == name);
                            });
    if (ret != vec.cend())
        return *ret;

    return detail::create_invalid_item<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(string_view name, instance obj) const
{
    return get_property(name).get_value(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(string_view name)
{
    const auto prop = get_global_property(name);
    return prop.get_value(instance());
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(string_view name, instance obj, argument arg) const
{
    const auto prop = get_property(name);
    return prop.set_value(obj, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(string_view name, argument arg)
{
    const auto prop = get_global_property(name);
    return prop.set_value(instance(), arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type::get_properties() const RTTR_NOEXCEPT
{
    auto& vec = get_raw_type().m_type_data->get_class_data().m_properties;
    if (!vec.empty())
    {
        return array_range<property>(vec.data(), vec.size(),
                                     detail::default_predicate<property>([](const property& prop)
                                     {
                                         return (prop.get_access_level() == access_levels::public_access);
                                     }) );
    }

    return array_range<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type::get_properties(filter_items filter) const RTTR_NOEXCEPT
{
    auto& vec = get_raw_type().m_type_data->get_class_data().m_properties;
    if (!vec.empty())
        return array_range<property>(vec.data(), vec.size(), get_filter_predicate<property>(*this, filter));

    return array_range<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(string_view name) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_method(get_raw_type(), name);
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(string_view name, const std::vector<type>& params) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_method(get_raw_type(), name, params);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type::get_methods() const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_methods(get_raw_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type::get_methods(filter_items filter) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_methods(get_raw_type(), filter);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_global_property(string_view name) RTTR_NOEXCEPT
{
    auto& prop_list = get_global_property_storage();
    const auto ret = prop_list.find(name);
    if (ret != prop_list.end())
        return *ret;

    return detail::create_invalid_item<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(string_view name) RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_global_method(name);
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(string_view name, const std::vector<type>& params) RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_global_method(name, params);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<method> type::get_global_methods() RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_global_methods();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type::get_global_properties() RTTR_NOEXCEPT
{
    auto& vec = get_global_property_storage().value_data();
    return array_range<property>(vec.data(), vec.size());
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration type::get_enumeration() const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_enumeration(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(string_view name, instance obj, std::vector<argument> args) const
{
    if (auto meth = detail::type_database::instance().get_class_method(get_raw_type(), name, args))
        return meth.invoke_variadic(obj, args);

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(string_view name, std::vector<argument> args)
{
    const auto& db = detail::type_database::instance();
    return db.get_global_method(name, args).invoke_variadic(instance(), args);
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_by_name(string_view name) RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_by_name(name);
}

/////////////////////////////////////////////////////////////////////////////////////////

const detail::type_converter_base* type::get_type_converter(const type& target_type) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_converter(*this, target_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type::get_constructor(const std::vector<type>& args) const RTTR_NOEXCEPT
{
    auto& ctors = m_type_data->get_class_data().m_ctors;
    for (const auto& ctor : ctors)
    {
        if (detail::compare_with_type_list::compare(ctor.get_parameter_infos(), args))
            return ctor;
    }

    return detail::create_invalid_item<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type::get_constructors() const RTTR_NOEXCEPT
{
    auto& ctors = m_type_data->get_class_data().m_ctors;
    if (!ctors.empty())
    {
        return array_range<constructor>(ctors.data(), ctors.size(),
                                        detail::default_predicate<constructor>([](const constructor& ctor)
                                        {
                                            return (ctor.get_access_level() == access_levels::public_access);
                                        }) );
    }

    return array_range<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type::get_constructors(filter_items filter) const RTTR_NOEXCEPT
{
    auto& ctors = m_type_data->get_class_data().m_ctors;
    if (!ctors.empty())
        return array_range<constructor>(ctors.data(), ctors.size(), get_filter_predicate<constructor>(*this, filter));

    return array_range<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::register_constructor(const type& t, std::unique_ptr<detail::constructor_wrapper_base> ctor)
{
    static std::vector<std::unique_ptr<detail::constructor_wrapper_base> > constructor_list;
    if (!t.is_valid())
        return;

    auto& class_data = t.m_type_data->get_class_data();
    class_data.m_ctors.emplace_back(detail::create_item<constructor>(ctor.get()));
    constructor_list.push_back(std::move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

destructor type::get_destructor() const RTTR_NOEXCEPT
{
    return get_raw_type().m_type_data->get_class_data().m_dtor;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::register_destructor(const type& t, std::unique_ptr<detail::destructor_wrapper_base> dtor)
{
    static std::vector<std::unique_ptr<detail::destructor_wrapper_base> > destructor_list;

    auto& dtor_type = t.m_type_data->get_class_data().m_dtor;
    if (!dtor_type) // when no dtor is set at the moment
    {
        auto d = detail::create_item<destructor>(dtor.get());
        dtor_type = d;
        destructor_list.push_back(std::move(dtor));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_type_property(const type& t, string_view name)
{
    for (const auto& prop : get_items_for_type(t, t.m_type_data->get_class_data().m_properties))
    {
        if (prop.get_name() == name)
            return prop;
    }

    return detail::create_invalid_item<property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type::update_class_list(const type& t, T item_ptr)
{
    // update type "t" with all items from the base classes
    auto& all_class_items = (t.m_type_data->get_class_data().*item_ptr);
    auto item_range = get_items_for_type(t, t.m_type_data->get_class_data().*item_ptr);
    detail::remove_cv_ref_t<decltype(all_class_items)> item_vec(item_range.begin(), item_range.end());
    all_class_items.reserve(all_class_items.size() + 1);
    all_class_items.clear(); // this will not reduce the capacity, i.e. new memory allocation may not necessary
    for (const auto& base_type : t.get_base_classes())
    {
        auto base_properties = get_items_for_type(base_type, base_type.m_type_data->get_class_data().*item_ptr);
        if (base_properties.empty())
            continue;

        all_class_items.reserve(all_class_items.size() + base_properties.size());
        all_class_items.insert(all_class_items.end(), base_properties.begin(), base_properties.end());
    }

    // insert own class items
    all_class_items.reserve(all_class_items.size() + item_vec.size());
    all_class_items.insert(all_class_items.end(), item_vec.begin(), item_vec.end());

    // update derived types
    for (const auto& derived_type : t.get_derived_classes())
        update_class_list<T>(derived_type, item_ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::register_property(const type& t, std::unique_ptr<detail::property_wrapper_base> prop)
{
    if (!t.is_valid())
        return;

    const auto name = prop->get_name();

    if (t.is_class())
    {
        auto& property_list = t.m_type_data->get_class_data().m_properties;

        if (get_type_property(t, name))
            return;

        property_list.emplace_back(detail::create_item<property>(prop.get()));
        get_property_storage().push_back(std::move(prop));
        update_class_list(t, &detail::class_data::m_properties);
    }
    else
    {
        if (get_global_property(name))
            return;

        property p = detail::create_item<property>(prop.get());
        get_global_property_storage().insert(std::move(name), std::move(p));
        get_property_storage().push_back(std::move(prop));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
