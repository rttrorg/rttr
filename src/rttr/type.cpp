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

std::string type::get_name() const
{
    if (is_valid())
        return (*g_custom_names)[m_id];

    return std::string();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::get_full_name() const
{
    if (is_valid())
       return normalize_orig_name((*g_orig_names)[m_id]);

    return std::string();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::normalize_orig_name(const char* name)
{
    return std::string(name, strlen(name) - detail::skip_size_at_end);
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_raw_type() const
{
    if (is_valid())
        return type((*g_raw_type_list)[m_id]);
    else
        return type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_wrapped_type() const
{
    if (is_valid())
        return type((*g_wrapped_type_list)[m_id]);
    else
        return type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_derived_from(const type& other) const
{
    const type::type_id source_raw_id = (*g_raw_type_list)[m_id];
    const type::type_id target_raw_id = (*g_raw_type_list)[other.m_id];
    if (source_raw_id == target_raw_id)
        return true;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_base_class_list)[row + i];
        if (currId == target_raw_id)
            return true;
        if (currId == type::m_invalid_id)
            return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void* type::apply_offset(void* ptr, const type& source_type, const type& target_type)
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
        const type::type_id currId = (*g_base_class_list)[row + i];
        if (currId == target_raw_id)
            return (*g_conversion_list)[row + i](info.m_ptr);
        if (currId == type::m_invalid_id)
            return nullptr;
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create_variant(const argument& data) const
{
    return (*g_variant_create_func_list)[m_id](data);
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<type> type::get_base_classes() const
{
    const type::type_id raw_id = (*g_raw_type_list)[m_id];
    vector<type> result;
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_base_class_list)[row + i];
        if (currId != type::m_invalid_id)
            result.push_back(currId);
        else
            break;
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<type> type::get_derived_classes() const
{
    const type::type_id raw_id = (*g_raw_type_list)[m_id];
    vector<type> result;
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = (*g_derived_class_list)[row + i];
        if (currId != type::m_invalid_id)
            result.push_back(currId);
        else
            break;
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t type::get_sizeof() const
{
    return (*g_type_size)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_class() const
{
    return (*g_is_class_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_enumeration() const
{
    return (*g_is_enum_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_wrapper() const
{
    return ((*g_wrapped_type_list)[m_id] != type::m_invalid_id);
}


/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_array() const
{
    return (*g_is_array_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_pointer() const
{
    return (*g_is_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_arithmetic() const
{
    return (*g_is_arithmetic_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_function_pointer() const
{
    return (*g_is_function_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_object_pointer() const
{
    return (*g_is_member_object_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_function_pointer() const
{
    return (*g_is_member_function_pointer_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t type::get_pointer_dimension() const
{
    return (*g_pointer_dim_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_raw_array_type() const
{
    return (*g_array_raw_type_list)[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type> type::get_types()
{
    std::vector<type> result;
    result.reserve(g_orig_names->size() - 1); // we ignore the invalid type
    for (std::size_t id = 1; id < g_orig_names->size(); ++id)
    {
        result.push_back(type(static_cast<type::type_id>(id)));
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_metadata(const variant& key) const
{
    return detail::type_database::instance().get_metadata(*this, key);
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<type> extract_types(const vector<argument>& args)
{
    std::vector<type> result;
    result.reserve(args.size());
    for (const auto& arg : args)
        result.push_back(arg.get_type());
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type::get_constructor(const std::vector<type>& args) const
{
    return constructor(detail::type_database::instance().get_constructor(*this, args));
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<constructor> type::get_constructors() const
{
    const auto constructors = detail::type_database::instance().get_constructors(*this);
    vector<constructor> result;
    result.reserve(constructors.size());
    for (const auto& ctor : constructors)
        result.push_back(constructor(ctor));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create(vector<argument> args) const
{
    if (auto ctor = detail::type_database::instance().get_constructor(*this, args))
        return ctor->invoke_variadic(args);

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

destructor type::get_destructor() const
{
    return destructor(detail::type_database::instance().get_destructor(*this));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::destroy(variant& obj) const
{
    if (auto dtor = detail::type_database::instance().get_destructor(*this))
        dtor->invoke(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_property(const char* name) const
{
    const auto& obj = detail::type_database::instance();

    if (const auto prop = obj.get_class_property(get_raw_type(), name))
        return property(prop);
    
    for (const auto& type : get_base_classes())
    {
        if (const auto prop = obj.get_class_property(type.get_raw_type(), name))
            return property(prop);
    }

    return property();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(const char* name, instance obj) const
{
    const auto prop = get_property(name);
    return prop.get_value(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(const char* name)
{
    const auto prop = get_global_property(name);
    return prop.get_value(instance());
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(const char* name, instance obj, argument arg) const
{
    const auto prop = get_property(name);
    return prop.set_value(obj, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(const char* name, argument arg)
{
    const auto prop = get_global_property(name);
    return prop.set_value(instance(), arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<property> type::get_properties() const
{
    const auto& obj = detail::type_database::instance();
    vector<const detail::property_wrapper_base*> props;

    for (const auto& type :get_base_classes())
    {
       const auto property_list = obj.get_all_class_properties(type.get_raw_type());
       props.insert(props.end(), property_list.cbegin(), property_list.cend());
    }

    const auto vec = obj.get_all_class_properties(get_raw_type());
    props.insert(props.end(), vec.cbegin(), vec.cend());

    vector<property> result;
    result.reserve(props.size());
    for (const auto& prop : props)
        result.push_back(property(prop));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(const char* name) const
{
    const auto& obj = detail::type_database::instance();

    if (const auto meth = obj.get_class_method(get_raw_type(), name))
        return method(meth);
    
    for (const auto& type : detail::reverse(get_base_classes()))
    {
        if (const auto meth = obj.get_class_method(type.get_raw_type(), name))
            return method(meth);
    }

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(const char* name, const std::vector<type>& params) const
{
    const auto& obj = detail::type_database::instance();

    if (const auto meth = obj.get_class_method(get_raw_type(), name, params))
        return method(meth);
    
    for (const auto& type : detail::reverse(get_base_classes()))
    {
        if (const auto meth = obj.get_class_method(type.get_raw_type(), name, params))
            return method(meth);
    }

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<method> type::get_methods() const
{
    const auto& obj = detail::type_database::instance();
    vector<const detail::method_wrapper_base*> methods;

    for (const auto& type : get_base_classes())
    {
       const auto method_list = obj.get_all_class_methods(type.get_raw_type());
       methods.insert(methods.end(), method_list.cbegin(), method_list.cend());
    }

    const auto vec = obj.get_all_class_methods(get_raw_type());
    methods.insert(methods.end(), vec.cbegin(), vec.cend());

    vector<method> result;
    result.reserve(methods.size());
    for (const auto& meth : methods)
        result.push_back(method(meth));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_global_property(const char* name)
{
    return property(detail::type_database::instance().get_global_property(name));
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(const char* name)
{
    const auto& db = detail::type_database::instance();
    if (const auto meth = db.get_global_method(name))
        return method(meth);

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(const char* name, const std::vector<type>& params)
{
    const auto& db = detail::type_database::instance();
    if (const auto meth = db.get_global_method(name, params))
        return method(meth);

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<method> type::get_global_methods()
{
    const auto& db = detail::type_database::instance();
    const auto methods = db.get_all_global_methods();

    vector<method> result;
    result.reserve(methods.size());
    for (const auto& meth : methods)
        result.push_back(method(meth));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<property> type::get_global_properties()
{
    const auto property_list = detail::type_database::instance().get_all_global_properties();

    vector<property> result;
    result.reserve(property_list.size());
    for (const auto& prop : property_list)
        result.push_back(property(prop));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration type::get_enumeration() const
{
    return enumeration(detail::type_database::instance().get_enumeration(*this));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(const char* name, instance obj, std::vector<argument> args) const
{
    const auto& db = detail::type_database::instance();

    if (const auto meth = db.get_class_method(get_raw_type(), name, args))
        return meth->invoke_variadic(obj, args);
    
    for (const auto& type : detail::reverse(get_base_classes()))
    {
        if (const auto meth = db.get_class_method(type.get_raw_type(), name, args))
            return meth->invoke_variadic(obj, args);
    }

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(const char* name, std::vector<argument> args)
{
    const auto& db = detail::type_database::instance();
    if (const auto meth = db.get_global_method(name, args))
        return meth->invoke_variadic(instance(), args);

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_by_name(const char* name)
{
    return detail::type_database::instance().get_by_name(name);
}

/////////////////////////////////////////////////////////////////////////////////////////

const detail::type_converter_base* type::get_type_converter(const type& target_type) const
{
    return detail::type_database::instance().get_converter(*this, target_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
