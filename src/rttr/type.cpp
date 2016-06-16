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

#define RTTR_DECL_DB_TYPE(member, variable) static decltype(std::declval<detail::type_database>().member) * variable = nullptr;

RTTR_DECL_DB_TYPE(m_orig_names, g_orig_names)
RTTR_DECL_DB_TYPE(m_custom_names, g_custom_names)

RTTR_DECL_DB_TYPE(m_orig_name_to_id, g_orig_name_to_id)
RTTR_DECL_DB_TYPE(m_custom_name_to_id, g_custom_name_to_id)


RTTR_DECL_DB_TYPE(m_base_class_list, g_base_class_list)
RTTR_DECL_DB_TYPE(m_derived_class_list, g_derived_class_list)
RTTR_DECL_DB_TYPE(m_get_derived_info_func_list, g_get_derived_info_func_list)
RTTR_DECL_DB_TYPE(m_conversion_list, g_conversion_list)

RTTR_DECL_DB_TYPE(m_raw_type_list, g_raw_type_list)
RTTR_DECL_DB_TYPE(m_wrapped_type_list, g_wrapped_type_list)
RTTR_DECL_DB_TYPE(m_array_raw_type_list, g_array_raw_type_list)
RTTR_DECL_DB_TYPE(m_variant_create_func_list, g_variant_create_func_list)

RTTR_DECL_DB_TYPE(m_type_size, g_type_size)
RTTR_DECL_DB_TYPE(m_type_list, g_type_list)

RTTR_DECL_DB_TYPE(m_is_class_list, g_is_class_list)
RTTR_DECL_DB_TYPE(m_is_enum_list, g_is_enum_list)
RTTR_DECL_DB_TYPE(m_is_array_list, g_is_array_list)
RTTR_DECL_DB_TYPE(m_is_pointer_list, g_is_pointer_list)
RTTR_DECL_DB_TYPE(m_is_arithmetic_list, g_is_arithmetic_list)
RTTR_DECL_DB_TYPE(m_is_function_pointer_list, g_is_function_pointer_list)
RTTR_DECL_DB_TYPE(m_is_member_object_pointer_list, g_is_member_object_pointer_list)
RTTR_DECL_DB_TYPE(m_is_member_function_pointer_list, g_is_member_function_pointer_list)
RTTR_DECL_DB_TYPE(m_pointer_dim_list, g_pointer_dim_list)

// because everything is initialized at static initialization time the call to
// register_type can be made from another translation unit before global statics
// like 'g_name_list' are initialized, therefore we have to initialize the data base singleton here explicit
void type::init_globals()
{
    static bool initialized = false;
    if (initialized)
        return;

    auto& db = detail::type_database::instance();
    #define RTTR_SET_DB_TYPE(member, variable) variable = &db.member;

    RTTR_SET_DB_TYPE(m_orig_names, g_orig_names)
    RTTR_SET_DB_TYPE(m_custom_names, g_custom_names)

    RTTR_SET_DB_TYPE(m_orig_name_to_id, g_orig_name_to_id)
    RTTR_SET_DB_TYPE(m_custom_name_to_id, g_custom_name_to_id)

    RTTR_SET_DB_TYPE(m_base_class_list, g_base_class_list)
    RTTR_SET_DB_TYPE(m_derived_class_list, g_derived_class_list)
    RTTR_SET_DB_TYPE(m_get_derived_info_func_list, g_get_derived_info_func_list)
    RTTR_SET_DB_TYPE(m_conversion_list, g_conversion_list)

    RTTR_SET_DB_TYPE(m_raw_type_list, g_raw_type_list)
    RTTR_SET_DB_TYPE(m_wrapped_type_list, g_wrapped_type_list)
    RTTR_SET_DB_TYPE(m_array_raw_type_list, g_array_raw_type_list)
    RTTR_SET_DB_TYPE(m_variant_create_func_list, g_variant_create_func_list)

    RTTR_SET_DB_TYPE(m_type_size, g_type_size)
    RTTR_SET_DB_TYPE(m_type_list, g_type_list)

    RTTR_SET_DB_TYPE(m_is_class_list, g_is_class_list)
    RTTR_SET_DB_TYPE(m_is_enum_list, g_is_enum_list)
    RTTR_SET_DB_TYPE(m_is_array_list, g_is_array_list)
    RTTR_SET_DB_TYPE(m_is_pointer_list, g_is_pointer_list)
    RTTR_SET_DB_TYPE(m_is_arithmetic_list, g_is_arithmetic_list)
    RTTR_SET_DB_TYPE(m_is_function_pointer_list, g_is_function_pointer_list)
    RTTR_SET_DB_TYPE(m_is_member_object_pointer_list, g_is_member_object_pointer_list)
    RTTR_SET_DB_TYPE(m_is_member_function_pointer_list, g_is_member_function_pointer_list)
    RTTR_SET_DB_TYPE(m_pointer_dim_list, g_pointer_dim_list)

    initialized = true;
}

/////////////////////////////////////////////////////////////////////////////////////////

string_view type::get_name() const RTTR_NOEXCEPT
{
    if (is_valid())
        return (*g_custom_names)[m_id];

    return string_view();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::get_full_name() const
{
    if (is_valid())
       return normalize_orig_name((*g_orig_names)[m_id]);

    return std::string();
}

/////////////////////////////////////////////////////////////////////////////////////////

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

} // end anonymous namespace

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::normalize_orig_name(string_view name)
{
    std::string normalized_name = name.to_string();

    move_pointer_and_ref_to_type(normalized_name);
    return normalized_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_raw_type() const RTTR_NOEXCEPT
{
    if (is_valid())
        return type((*g_raw_type_list)[m_id], nullptr);
    else
        return type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_wrapped_type() const RTTR_NOEXCEPT
{
    if (is_valid())
        return type((*g_wrapped_type_list)[m_id], nullptr);
    else
        return type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_derived_from(const type& other) const RTTR_NOEXCEPT
{
    const type::type_id source_raw_id = (*g_raw_type_list)[m_id];
    const type::type_id target_raw_id = (*g_raw_type_list)[other.m_id];
    if (source_raw_id == target_raw_id)
        return true;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_base_class_list)[row + i].get_id();
        if (currId == target_raw_id)
            return true;
        if (currId == type::m_invalid_id)
            return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void* type::apply_offset(void* ptr, const type& source_type, const type& target_type) RTTR_NOEXCEPT
{
    type::type_id source_raw_id        = (*g_raw_type_list)[source_type.m_id];
    const type::type_id target_raw_id  = (*g_raw_type_list)[target_type.m_id];

    if (source_raw_id == target_raw_id || ptr == nullptr)
        return ptr;

    const detail::derived_info info = (*g_get_derived_info_func_list)[source_raw_id](ptr);
    source_raw_id = (*g_raw_type_list)[info.m_type.m_id];
    if (source_raw_id == target_raw_id)
        return info.m_ptr;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_base_class_list)[row + i].get_id();
        if (currId == target_raw_id)
            return (*g_conversion_list)[row + i](info.m_ptr);
        if (currId == type::m_invalid_id)
            return nullptr;
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_derived_type(void* ptr, const type& source_type) RTTR_NOEXCEPT
{
    if (ptr == nullptr)
        return type();

    type::type_id source_raw_id     = (*g_raw_type_list)[source_type.m_id];
    const detail::derived_info info = (*g_get_derived_info_func_list)[source_raw_id](ptr);
    return info.m_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create_variant(const argument& data) const
{
    return (*g_variant_create_func_list)[m_id](data);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_base_classes() const RTTR_NOEXCEPT
{
    std::size_t end_index = 0;
    const type::type_id raw_id = (*g_raw_type_list)[m_id];
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;

    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_base_class_list)[row + i].get_id();
        if (currId == type::m_invalid_id)
        {
            end_index = i;
            break;
        }
    }

    return array_range<type>(&(*g_base_class_list)[row], end_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_derived_classes() const RTTR_NOEXCEPT
{
    const type::type_id raw_id = (*g_raw_type_list)[m_id];
    std::size_t end_index = 0;
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_derived_class_list)[row + i].get_id();
        if (currId == type::m_invalid_id)
        {
            end_index = i;
            break;
        }
    }

    return array_range<type>(&(*g_derived_class_list)[row], end_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t type::get_sizeof() const RTTR_NOEXCEPT
{
    return (*g_type_size)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_class() const RTTR_NOEXCEPT
{
    return (*g_is_class_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_wrapper() const RTTR_NOEXCEPT
{
    return ((*g_wrapped_type_list)[m_id] != type::m_invalid_id);
}


/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_array() const RTTR_NOEXCEPT
{
    return (*g_is_array_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_pointer() const RTTR_NOEXCEPT
{
    return (*g_is_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_arithmetic() const RTTR_NOEXCEPT
{
    return (*g_is_arithmetic_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_function_pointer() const RTTR_NOEXCEPT
{
    return (*g_is_function_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_object_pointer() const RTTR_NOEXCEPT
{
    return (*g_is_member_object_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_function_pointer() const RTTR_NOEXCEPT
{
    return (*g_is_member_function_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t type::get_pointer_dimension() const RTTR_NOEXCEPT
{
    return (*g_pointer_dim_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_raw_array_type() const RTTR_NOEXCEPT
{
    return type((*g_array_raw_type_list)[m_id], nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<type> type::get_types() RTTR_NOEXCEPT
{
    return array_range<type>(&(*g_type_list)[1], g_type_list->size() - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_metadata(const variant& key) const
{
    return detail::type_database::instance().get_metadata(*this, key);
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type::get_constructor(const std::vector<type>& args) const RTTR_NOEXCEPT
{
    return constructor(detail::type_database::instance().get_constructor(*this, args));
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type::get_constructors() const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_constructors(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<constructor> type::get_constructors(filter_items filter) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_constructors(*this, filter);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create(vector<argument> args) const
{
    auto ctor = detail::type_database::instance().get_constructor(*this, args);
    return ctor.invoke_variadic(std::move(args));
}

/////////////////////////////////////////////////////////////////////////////////////////

destructor type::get_destructor() const RTTR_NOEXCEPT
{
    return destructor(detail::type_database::instance().get_destructor(get_raw_type()));
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::destroy(variant& obj) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_destructor(get_raw_type()).invoke(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_property(string_view name) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_property(get_raw_type(), name);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(string_view name, instance obj) const
{
    const auto prop = get_property(name);
    return prop.get_value(obj);
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
    return detail::type_database::instance().get_class_properties(get_raw_type());
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<property> type::get_properties(filter_items filter) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_class_properties(get_raw_type(), filter);
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
    return property(detail::type_database::instance().get_global_property(name));
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
    return detail::type_database::instance().get_global_properties();
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
    return type(detail::type_database::instance().get_by_name(name), nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

const detail::type_converter_base* type::get_type_converter(const type& target_type) const RTTR_NOEXCEPT
{
    return detail::type_database::instance().get_converter(*this, target_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
