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

#include "rttr/detail/type/type_register.h"

#include "rttr/detail/type/type_register_p.h"

#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/destructor/destructor_wrapper_base.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/parameter_info/parameter_infos_compare.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/metadata/metadata.h"
#include "rttr/constructor.h"
#include "rttr/destructor.h"
#include "rttr/property.h"
#include "rttr/method.h"
#include "rttr/detail/type/type_data.h"

#include "rttr/detail/filter/filter_item_funcs.h"
#include "rttr/detail/type/type_string_utils.h"

#include <set>

using namespace std;

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<type> convert_param_list(const array_range<parameter_info>& param_list);

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::property(const type& t, unique_ptr<property_wrapper_base> prop)
{
   type_register_private::property(t, move(prop));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::method(const type& t, std::unique_ptr<method_wrapper_base> method)
{
    type_register_private::method(t, move(method));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor)
{
    type_register_private::constructor(t, move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor)
{
    type_register_private::destructor(t, move(dtor));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_item)
{
    t.m_type_data->get_enumeration() = std::move(enum_item);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::custom_name(type& t, string_view custom_name)
{
    type_register_private::register_custom_name(t, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::metadata(const type& t, std::vector<::rttr::detail::metadata> data)
{
    auto& vec_to_insert = *t.m_type_data->get_metadata().get();

    // when we insert new items, we want to check first whether a item with same key exist => ignore this data
    for (auto& new_item : data)
    {
        if (type_register_private::get_metadata(new_item,  vec_to_insert).is_valid() == false)
            vec_to_insert.emplace_back(std::move(new_item));
    }

    std::sort(vec_to_insert.begin(), vec_to_insert.end(), metadata::order_by_key());
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::converter(const type& t, const type_converter_base* converter)
{
     type_register_private::converter(t, converter);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::comparator(const type& t, type_comparator_base* comparator)
{
    type_register_private::comparator(t, comparator);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::equal_comparator(const type& t, type_comparator_base* comparator)
{
    type_register_private::equal_comparator(t, comparator);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::less_than_comparator(const type& t, type_comparator_base* comparator)
{
    type_register_private::less_than_comparator(t, comparator);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::register_base_class(const type& derived_type, const base_class_info& base_info)
{
    auto& class_data = derived_type.m_type_data->get_class_data();
    auto itr = std::find_if(class_data.m_base_types.begin(), class_data.m_base_types.end(),
    [base_info](const type& t)
    {
        return (t == base_info.m_base_type);
    });

    if (itr != class_data.m_base_types.end()) // already registerd as base class => quit
        return;

    std::vector<std::pair<type, rttr_cast_func>> tmp_sort_vec;
    using sorted_pair = decltype(tmp_sort_vec)::value_type;
    if (class_data.m_base_types.size() != class_data.m_conversion_list.size())
        return; // error!!!

    int index = 0;
    for (const auto& item : class_data.m_base_types)
    {
        tmp_sort_vec.emplace_back(item, class_data.m_conversion_list[index]);
        ++index;
    }

    tmp_sort_vec.emplace_back(base_info.m_base_type, base_info.m_rttr_cast_func);
    std::sort(tmp_sort_vec.begin(), tmp_sort_vec.end(),
    [](const sorted_pair& left, const sorted_pair& right)
    { return left.first.get_id() < right.first.get_id(); });

    class_data.m_base_types.clear();
    class_data.m_conversion_list.clear();

    std::transform(tmp_sort_vec.begin(),
                   tmp_sort_vec.end(),
                   std::back_inserter(class_data.m_base_types),
                   [](const sorted_pair& item)-> type { return item.first; });

    std::transform(tmp_sort_vec.begin(),
                   tmp_sort_vec.end(),
                   std::back_inserter(class_data.m_conversion_list),
                   [](const sorted_pair& item)-> rttr_cast_func { return item.second; });

    auto r_type = base_info.m_base_type.get_raw_type();
    r_type.m_type_data->get_class_data().m_derived_types.push_back(type(derived_type.m_type_data));
}


/////////////////////////////////////////////////////////////////////////////////////////

std::deque<std::unique_ptr<method_wrapper_base>>& type_register_private::get_method_storage()
{
    static std::deque<std::unique_ptr<method_wrapper_base>> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

flat_multimap<string_view, ::rttr::method>& type_register_private::get_global_method_storage()
{
    static flat_multimap<string_view, ::rttr::method> meths;
    return meths;
}

/////////////////////////////////////////////////////////////////////////////////////////

flat_multimap<string_view, ::rttr::property>& type_register_private::get_global_property_storage()
{
    static flat_multimap<string_view, ::rttr::property> props;
    return props;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::deque<std::unique_ptr<property_wrapper_base>>& type_register_private::get_property_storage()
{
    static std::deque<std::unique_ptr<property_wrapper_base>> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<method>& type_register_private::get_global_methods()
{
    static std::vector<::rttr::method> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<property>& type_register_private::get_global_properties()
{
    static std::vector<::rttr::property> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

type type_register::type_reg(type_data& info) RTTR_NOEXCEPT
{
    return type_register_private::register_type(info);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// Here comes the implementation of the registration class 'type_register_private'

std::vector<type_data*>& type_register_private::get_type_data_storage()
{
    static std::vector<type_data*> obj = {&get_invalid_type_data()};
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type>& type_register_private::get_type_storage()
{
    static std::vector<type> obj = {type(&get_invalid_type_data())};
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

flat_map<string_view, type>& type_register_private::get_orig_name_to_id()
{
    static flat_map<string_view, type> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

flat_map<std::string, type, hash>& type_register_private::get_custom_name_to_id()
{
    static flat_map<std::string, type, hash> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type_register_private::data_container<const type_converter_base*>>& type_register_private::get_type_converter_list()
{
    static std::vector<data_container<const type_converter_base*>> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type_register_private::data_container<const type_comparator_base*>>& type_register_private::get_type_comparator_list()
{
    static std::vector<data_container<const type_comparator_base*>> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type_register_private::data_container<const type_comparator_base*>>& type_register_private::get_type_equal_comparator_list()
{
    static std::vector<data_container<const type_comparator_base*>> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type_register_private::data_container<const type_comparator_base*>>& type_register_private::get_type_less_comparator_list()
{
    static std::vector<data_container<const type_comparator_base*>> obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type_register_private::register_name(uint16_t& id, type_data& info)
{
    using namespace detail;

    auto& orig_name_to_id = get_orig_name_to_id();
    auto ret = orig_name_to_id.find(info.type_name);
    if (ret != orig_name_to_id.end())
    {
        id = (*ret).get_id();
        return true;
    }

    static type::type_id m_type_id_counter = 0;
    ++m_type_id_counter;

    orig_name_to_id.insert(std::make_pair(info.type_name, type(&info)));
    info.name = derive_name(type(&info));
    get_custom_name_to_id().insert(std::make_pair(info.name, type(&info)));

    id = m_type_id_counter;
    info.type_index = id;
    get_type_storage().emplace_back(type(&info));

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::register_base_class_info(type_data& info)
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

type type_register_private::register_type(type_data& info) RTTR_NOEXCEPT
{
    auto& type_data_container = get_type_data_storage();
    // register the base types
    info.get_base_types();

    using namespace detail;
    uint16_t id = 0;
    const bool isAlreadyRegistered = register_name(id, info);
    if (isAlreadyRegistered)
        return type(type_data_container[id]);

    info.raw_type_data  = !info.raw_type_data->is_valid() ? &info : info.raw_type_data;

    type_data_container.push_back(&info);

    // has to be done as last step
    register_base_class_info(info);

    update_custom_name(derive_template_instance_name(info), type(&info));

    // when a base class type has class items, but the derived one not,
    // we update the derived class item list
    const auto t = type(type_data_container[id]);
    update_class_list(t, &detail::class_data::m_properties);
    update_class_list(t, &detail::class_data::m_methods);

    return t;
}

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

static void move_pointer_and_ref_to_type(std::string& type_name)
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

static std::string normalize_orig_name(string_view name)
{
    std::string normalized_name = name.to_string();

    move_pointer_and_ref_to_type(normalized_name);
    return normalized_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string derive_name_impl(const std::string& src_name, const std::string& raw_name,
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

std::string type_register_private::derive_template_instance_name(type_data& info)
{
    auto& nested_types = info.get_class_data().m_nested_types;
    if (nested_types.empty()) // no template type
        return info.name;

    const auto has_custom_name = (info.name != info.type_name);
    if (has_custom_name)
        return info.name;

    const auto start_pos = info.name.find("<");
    const auto end_pos = info.name.rfind(">");

    if (start_pos == std::string::npos || end_pos == std::string::npos)
        return info.name;

    auto new_name = info.name.substr(0, start_pos);
    const auto end_part = info.name.substr(end_pos);
    auto index = nested_types.size();
    new_name += std::string("<");
    for (const auto& item : nested_types)
    {
        --index;
        const auto& custom_name = item.m_type_data->name;
        new_name += custom_name;
        if (index > 0)
            new_name += ",";
    }

    new_name += end_part;

    return new_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::update_custom_name(std::string new_name, const type& t)
{
    auto& type_name = t.m_type_data->name;

    if (new_name != type_name)
    {
        auto& custom_name_to_id = get_custom_name_to_id();
        custom_name_to_id.erase(type_name);

        type_name = std::move(new_name);
        custom_name_to_id.insert(std::make_pair(type_name, t));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type_register_private::derive_name(const type& t)
{
    if (t.is_array() && t.get_raw_array_type().is_valid())
    {
        auto src_name_orig  = normalize_orig_name(t.get_full_name());
        auto custom_name    = t.get_raw_array_type().get_name();
        auto raw_name_orig  = normalize_orig_name( t.get_raw_array_type().get_full_name());

        return derive_name_impl(src_name_orig, raw_name_orig, custom_name.to_string());
    }
    else if (t != t.get_raw_type())
    {
        auto src_name_orig  = normalize_orig_name(t.get_full_name());
        auto custom_name    = t.get_raw_type().get_name();
        auto raw_name_orig  = normalize_orig_name( t.get_raw_type().get_full_name());

        return derive_name_impl(src_name_orig, raw_name_orig, custom_name.to_string());
    }
    else
    {
        return normalize_orig_name(t.get_full_name());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::register_custom_name(type& t, string_view custom_name)
{
    if (!t.is_valid())
        return;

    update_custom_name(custom_name.to_string(), t);

    // we have to make a copy of the list, because we also perform an insertion with 'update_custom_name'
    auto tmp_type_list = get_custom_name_to_id().value_data();
    for (auto& tt : tmp_type_list)
    {
        if (tt == t || tt.get_raw_type() == tt)
            continue;

        auto new_name = derive_name(tt);
        update_custom_name(new_name, tt);
    }

    for (auto& tt : tmp_type_list)
    {
        if (tt == t || !tt.is_template_instantiation())
            continue;

        update_custom_name(derive_template_instance_name(*tt.m_type_data), tt);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor)
{
    auto& class_data = t.m_type_data->get_class_data();
    class_data.m_ctors.emplace_back(create_item<::rttr::constructor>(ctor.get()));
    class_data.m_constructor_storage.push_back(std::move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor)
{
    auto& class_data = t.m_type_data->get_class_data();

    auto& dtor_type = class_data.m_dtor;
    if (!dtor_type) // when no dtor is set at the moment
    {
        auto d = create_item<::rttr::destructor>(dtor.get());
        dtor_type = d;
        class_data.m_dtor_storage = std::move(dtor);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::property(const type& t, std::unique_ptr<property_wrapper_base> prop)
{
    const auto name = prop->get_name();

    if (t.is_class())
    {
        auto& property_list = t.m_type_data->get_class_data().m_properties;

        if (get_type_property(t, name))
            return;

        property_list.emplace_back(detail::create_item<::rttr::property>(prop.get()));
        t.m_type_data->get_class_data().m_property_storage.push_back(std::move(prop));
        update_class_list(t, &detail::class_data::m_properties);
    }
    else
    {
        if (t.get_global_property(name))
            return;

        auto p = detail::create_item<::rttr::property>(prop.get());
        get_global_properties().push_back(p);
        get_global_property_storage().insert(std::move(name), std::move(p));
        get_property_storage().push_back(std::move(prop));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::method(const type& t, std::unique_ptr<method_wrapper_base> meth)
{
    const auto name = meth->get_name();
    if (t.is_class())
    {
        if (get_type_method(t, name, convert_param_list(meth->get_parameter_infos())))
            return;

        auto& method_list = t.m_type_data->get_class_data().m_methods;
        method_list.emplace_back(create_item<::rttr::method>(meth.get()));
        t.m_type_data->get_class_data().m_method_storage.push_back(std::move(meth));
        update_class_list(t, &class_data::m_methods);
    }
    else
    {
        if (t.get_global_method(name, convert_param_list(meth->get_parameter_infos())))
            return;

        auto m = create_item<::rttr::method>(meth.get());
        get_global_methods().push_back(m);
        get_global_method_storage().insert(std::move(name), std::move(m));
        get_method_storage().push_back(std::move(meth));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// private stuff

static std::vector<type> convert_param_list(const array_range<parameter_info>& param_list)
{
    std::vector<type> result;
    result.reserve(param_list.size());

    for (const auto& item : param_list)
        result.emplace_back(item.get_type());

    return result;
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

property type_register_private::get_type_property(const type& t, string_view name)
{
    for (const auto& prop : get_items_for_type(t, t.m_type_data->get_class_data().m_properties))
    {
        if (prop.get_name() == name)
            return prop;
    }

    return create_invalid_item<::rttr::property>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_register_private::get_type_method(const type& t, string_view name)
{
    for (const auto& meth : get_items_for_type(t, t.m_type_data->get_class_data().m_methods))
    {
        if (meth.get_name() == name)
        {
            return meth;
        }
    }

    return create_invalid_item<::rttr::method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type_register_private::get_type_method(const type& t, string_view name,
                                              const std::vector<type>& type_list)
{
    for (const auto& meth : get_items_for_type(t, t.m_type_data->get_class_data().m_methods))
    {
        if (meth.get_name() == name &&
            compare_with_type_list::compare(meth.get_parameter_infos(), type_list))
        {
            return meth;
        }
    }

    return detail::create_invalid_item<::rttr::method>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type_register_private::update_class_list(const type& t, T item_ptr)
{
    // update type "t" with all items from the base classes
    auto& all_class_items = (t.m_type_data->get_class_data().*item_ptr);
    auto item_range = get_items_for_type(t, all_class_items);
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
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::converter(const type& t, const type_converter_base* converter)
{
    if (!t.is_valid())
        return;

    if (get_converter(t, converter->m_target_type))
        return;

    using vec_value_type = data_container<const type_converter_base*>;
    auto& container = get_type_converter_list();
    container.push_back({t.get_id(), converter});
    std::stable_sort(container.begin(), container.end(), vec_value_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Container, typename Item>
static void remove_item(Container& container, Item& item)
{
    using order = typename Container::value_type::order_by_id;
    auto itr = std::lower_bound(container.begin(), container.end(),
                                item, order());
    if (itr != container.end())
    {
        if ((*itr).m_data == item)
        {
            container.erase(itr);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::deregister_converter(const type_converter_base* converter)
{
    remove_item(get_type_converter_list(), converter);
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::comparator(const type& t, const type_comparator_base* comparator)
{
    if (!t.is_valid())
        return;

    using data_type = data_container<const type_comparator_base*>;
    auto& container = get_type_comparator_list();
    container.push_back({t.get_id(), comparator});
    std::stable_sort(container.begin(), container.end(),
                     data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::deregister_comparator(const type_comparator_base* converter)
{

}

/////////////////////////////////////////////////////////////////////////////////////

const type_converter_base* type_register_private::get_converter(const type& source_type, const type& target_type)
{
    const auto src_id = source_type.get_id();
    const auto target_id = target_type.get_id();
    using vec_value_type = data_container<const type_converter_base*>;
    auto& container = get_type_converter_list();
    auto itr = std::lower_bound(container.cbegin(), container.cend(),
                                src_id, vec_value_type::order_by_id());
    for (; itr != container.cend(); ++itr)
    {
        auto& item = *itr;
        if (item.m_id != src_id)
            break; // type not found

        if (item.m_data->m_target_type.get_id() == target_id)
            return item.m_data;
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base* type_register_private::get_comparator(const type& t)
{
    using vec_value_type = data_container<const type_comparator_base*>;
    const auto id = t.get_id();
    auto& container = get_type_comparator_list();
    auto itr = std::lower_bound(container.cbegin(), container.cend(), id,
                                vec_value_type::order_by_id());
    if (itr != container.cend() && itr->m_id == id)
        return itr->m_data;
    else
        return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base* type_register_private::get_equal_comparator(const type& t)
{
    return get_type_comparator_impl(t, get_type_equal_comparator_list());
}

/////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base* type_register_private::get_less_than_comparator(const type& t)
{
    return get_type_comparator_impl(t, get_type_less_comparator_list());
}

/////////////////////////////////////////////////////////////////////////////////////

const type_comparator_base*
type_register_private::get_type_comparator_impl(const type& t,
                                                const std::vector<type_register_private::data_container<const type_comparator_base*>>& comparator_list)
 {
    using vec_value_type = data_container<const type_comparator_base*>;
    const auto id = t.get_id();
    auto itr = std::lower_bound(comparator_list.cbegin(), comparator_list.cend(), id,
                                vec_value_type::order_by_id());
    if (itr != comparator_list.cend() && itr->m_id == id)
        return itr->m_data;
    else
        return nullptr;
 }

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::equal_comparator(const type& t, const type_comparator_base* comparator)
{
    register_comparator_impl(t, comparator, get_type_equal_comparator_list());
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::deregister_equal_comparator(const type_comparator_base* converter)
{
    remove_item(get_type_equal_comparator_list(), converter);
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::less_than_comparator(const type& t, const type_comparator_base* comparator)
{
    register_comparator_impl(t, comparator, get_type_less_comparator_list());
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::deregister_less_than_comparator(const type_comparator_base* converter)
{
    remove_item(get_type_equal_comparator_list(), converter);
}

/////////////////////////////////////////////////////////////////////////////////////

void type_register_private::register_comparator_impl(const type& t, const type_comparator_base* comparator,
                                                     std::vector<data_container<const type_comparator_base*>>& comparator_list)
{
    if (!t.is_valid())
        return;

    using data_type = data_container<const type_comparator_base*>;
    comparator_list.push_back({t.get_id(), comparator});
    std::stable_sort(comparator_list.begin(), comparator_list.end(),
                     data_type::order_by_id());
}

/////////////////////////////////////////////////////////////////////////////////////

variant type_register_private::get_metadata(const type& t, const variant& key)
{
    auto meta_vec = t.m_type_data->get_metadata().get();
    return (meta_vec ? get_metadata(key, *meta_vec) : variant());
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type_register_private::get_metadata(const variant& key, const std::vector<metadata>& data)
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

/////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end
