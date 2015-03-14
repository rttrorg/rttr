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

#include "rttr/type.h"

#include "rttr/constructor.h"
#include "rttr/property.h"
#include "rttr/destructor.h"
#include "rttr/enumeration.h"
#include "rttr/method.h"

#include "rttr/detail/constructor_container_base.h"
#include "rttr/detail/destructor_container_base.h"
#include "rttr/detail/enumeration_container_base.h"
#include "rttr/detail/method_container_base.h"
#include "rttr/detail/property_container.h"
#include "rttr/detail/array_container_base.h"
#include "rttr/rttr_enable.h"
#include "rttr/metadata.h"

#include "rttr/detail/reflection_database_p.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <thread>
#include <mutex>
#include <cstring>
#include <cctype>

using namespace std;

namespace rttr
{
static detail::reflection_database::NameToTag               *g_name_to_id                       = nullptr;
static const char*                                          *g_name_list                        = nullptr;
static type::type_id                                        *g_base_class_list                  = nullptr;
static type::type_id                                        *g_derived_class_list               = nullptr;
static detail::reflection_database::rttr_cast_func          *g_conversion_list                  = nullptr;
static detail::reflection_database::get_derived_info_func   *g_get_derived_info_func_list       = nullptr;
static detail::variant_create_func                          *g_variant_create_list              = nullptr;
static type::type_id                                        *g_raw_type_list                    = nullptr;
static bool                                                 *g_is_class_list                    = nullptr;
static bool                                                 *g_is_enum_list                     = nullptr;
static bool                                                 *g_is_array_list                    = nullptr;
static bool                                                 *g_is_pointer_list                  = nullptr;
static bool                                                 *g_is_primitive_list                = nullptr;
static bool                                                 *g_is_function_pointer_list         = nullptr;
static bool                                                 *g_is_member_object_pointer_list    = nullptr;
static bool                                                 *g_is_member_function_pointer_list  = nullptr;
static detail::metadata_container                           *g_metadata_list                    = nullptr;
static std::size_t                                          *g_pointer_dim_list                 = nullptr;
static unique_ptr<detail::constructor_container_base>       *g_ctor_list                        = nullptr;
static unique_ptr<detail::destructor_container_base>        *g_dtor_list                        = nullptr;
static unique_ptr<detail::enumeration_container_base>       *g_enumeration_list                 = nullptr;
static std::vector<std::unique_ptr<detail::type_converter_base>> *g_type_converter_list         = nullptr;
static unique_ptr<detail::reflection_database::class_data>  *g_class_data_list                  = nullptr;
static detail::reflection_database::property_map            *g_global_properties                = nullptr;
static detail::reflection_database::method_map              *g_global_methods                   = nullptr;

static detail::reflection_database::constructor_container   *g_constructor_list                 = nullptr;
static detail::reflection_database::destructor_container    *g_destructor_list                  = nullptr;
static detail::reflection_database::method_container        *g_method_list                      = nullptr;
static detail::reflection_database::property_container      *g_property_list                    = nullptr;

static detail::reflection_database::custom_name_map         *g_custom_name_map                  = nullptr;


static std::mutex                                           *g_register_type_mutex              = nullptr;
static std::mutex                                           *g_register_custom_name_mutex       = nullptr;

// because everything is initialized at static initialization time the call to
// register_type can be made from another translation unit before global statics
// like 'g_name_list' are initialized, therefore we have to initialize the data base singleton here explicit
static void init_globals()
{
    static bool initialized = false;
    if (initialized)
        return;

    detail::reflection_database& db = detail::reflection_database::instance();

    g_name_to_id                        = &db.name_to_id;
    g_name_list                         = db.name_list;
    g_base_class_list                   = db.base_class_list;
    g_derived_class_list                = db.derived_class_list;
    g_conversion_list                   = db.conversion_list;
    g_variant_create_list               = db.variant_create_func_list;
    g_get_derived_info_func_list        = db.get_derived_info_func_list;
    g_raw_type_list                     = db.raw_type_list;
    g_is_class_list                     = db.is_class_list;
    g_is_enum_list                      = db.is_enum_list;
    g_is_array_list                     = db.is_array_list;
    g_is_pointer_list                   = db.is_pointer_list;
    g_is_primitive_list                 = db.is_primitive_list;
    g_is_function_pointer_list          = db.is_function_pointer_list;
    g_is_member_object_pointer_list     = db.is_member_object_pointer_list;
    g_is_member_function_pointer_list   = db.is_member_function_pointer_list;
    g_metadata_list                     = db.meta_data_list;
    g_pointer_dim_list                  = db.pointer_dim_list;
    g_class_data_list                   = db.class_data_list;
    g_ctor_list                         = db.constructor_list;
    g_dtor_list                         = db.destructor_list;
    g_enumeration_list                  = db.enumeration_list;
    g_global_properties                 = &db.global_properties;
    g_global_methods                    = &db.global_methods;
    g_constructor_list                  = &db.m_constructor_list;
    g_destructor_list                   = &db.m_destructor_list;
    g_method_list                       = &db.m_method_list;
    g_property_list                     = &db.m_property_list;
    g_type_converter_list               = db.type_converter_list;
    g_custom_name_map                   = &db.m_custom_name_map;
    g_register_type_mutex               = &db.m_register_type_mutex;
    g_register_custom_name_mutex        = &db.m_register_custom_name_mutex;

    initialized = true;
}

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

void insert_space_after(std::string& text, const std::string& part)
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

void insert_space_before(std::string& text, const std::string& part)
{
   auto found_pos = text.find_last_of(part);

    if (found_pos == std::string::npos)
        return;

    if (found_pos == std::string::npos || found_pos > text.length())
        return;

    text.insert(found_pos, " ");
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string get_name_impl(type::type_id id)
{
    const auto db_string = g_name_list[id];
    const auto str_length = strlen(db_string);
    return std::string(db_string, str_length - detail::skip_size_at_end);
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string get_custom_name(type source_type, type raw_type)
{
    std::string source_name = get_name_impl(source_type.get_id());

    std::lock_guard<std::mutex> lock(*g_register_custom_name_mutex);
    auto& custom_name_map = *g_custom_name_map;
    auto orig_src_name  = source_name;
    auto raw_name       = get_name_impl(raw_type.get_id());
    auto ret = custom_name_map.find(raw_type);
    // We replace a custom registered name for a type for all derived types, e.g.
    // "std::basic_string<char>" => "std::string"
    // we want to use this also automatically for derived types like pointers, e.g.
    // "const std::basic_string<char>*" => "const std::string*"
    // therefore we have to replace the "raw_type" string
    if (ret != custom_name_map.end())
    {
        const std::string& custom_name = ret->second;
        remove_whitespaces(raw_name);
        remove_whitespaces(source_name);

        const auto start_pos    = source_name.find(raw_name);
        const auto end_pos      = start_pos + raw_name.length();
        if (start_pos == std::string::npos)
            return orig_src_name; // nothing was found...

        // remember the two parts before and after the found "raw_name"
        const auto start_part   = source_name.substr(0, start_pos);
        const auto end_part     = source_name.substr(end_pos, source_name.length());
        
        source_name.replace(start_pos, raw_name.length(), custom_name);

        if (is_space_after(orig_src_name, start_part))
            insert_space_after(source_name, start_part);

        if (is_space_before(orig_src_name, end_part))
            insert_space_before(source_name, end_part);
    }

    return source_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::get_name() const
{
    if (is_valid())
    {
        const type::type_id raw_id = g_raw_type_list[m_id];
        const type raw_type(raw_id);
        if (raw_type.is_class() || raw_type.is_enumeration())
            return get_custom_name(*this, raw_type);
        else
            return get_name_impl(m_id);
    }
    else
    {
        return std::string();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

type type::get_raw_type() const
{
    if (is_valid())
        return type(g_raw_type_list[m_id]);
    else
        return type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_derived_from(const type& other) const
{
    const type::type_id source_raw_id = g_raw_type_list[m_id];
    const type::type_id target_raw_id = g_raw_type_list[other.m_id];
    if (source_raw_id == target_raw_id)
        return true;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = g_base_class_list[row + i];
        if (currId == target_raw_id)
            return true;
        if (currId == 0) // invalid id
            return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void* type::apply_offset(void* ptr, const type& source_type, const type& target_type)
{
    type::type_id source_raw_id        = g_raw_type_list[source_type.m_id];
    const type::type_id target_raw_id  = g_raw_type_list[target_type.m_id];

    if (source_raw_id == target_raw_id || ptr == nullptr)
        return ptr;

    const detail::derived_info info = g_get_derived_info_func_list[source_raw_id](ptr);
    source_raw_id = g_raw_type_list[info.m_type.m_id];
    if (source_raw_id == target_raw_id)
        return info.m_ptr;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = g_base_class_list[row + i];
        if (currId == target_raw_id)
            return g_conversion_list[row + i](info.m_ptr);
        if (currId == 0) // invalid id
            return nullptr;
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create_variant(const detail::argument& data) const
{
    return g_variant_create_list[m_id](data);
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<type> type::get_base_classes() const
{
    const type::type_id raw_id = g_raw_type_list[m_id];
    vector<type> result;
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = g_base_class_list[row + i];
        if (currId != 0) // invalid id
            result.push_back(currId);
        else
            break;
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<type> type::get_derived_classes() const
{
    const type::type_id raw_id = g_raw_type_list[m_id];
    vector<type> result;
    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = g_derived_class_list[row + i];
        if (currId != 0) // invalid id
            result.push_back(currId);
        else
            break;
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_class() const
{
    return g_is_class_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_enumeration() const
{
    return g_is_enum_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_array() const
{
    return g_is_array_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_pointer() const
{
    return g_is_pointer_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_primitive() const
{
    return g_is_primitive_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_function_pointer() const
{
    return g_is_function_pointer_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_object_pointer() const
{
    return g_is_member_object_pointer_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::is_member_function_pointer() const
{
    return g_is_member_function_pointer_list[m_id];
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t type::get_pointer_dimension() const
{
    return g_pointer_dim_list[m_id];
}


/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type> type::get_types()
{
    std::vector<type> result;
    result.reserve(g_name_to_id->size());
    for (const auto& value : *g_name_to_id)
    {
        result.push_back(value.second);
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_metadata(int key) const
{
    return g_metadata_list[m_id].get_metadata(key);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_metadata(const std::string& key) const
{
    return g_metadata_list[m_id].get_metadata(key);
}

/////////////////////////////////////////////////////////////////////////////////////////

constructor type::get_constructor(const std::vector<type>& args) const
{
    if (is_class())
    {
        const auto classPtr = g_class_data_list[get_raw_type().get_id()].get();
        if (!classPtr)
            return constructor();
        
        for (const auto& ctor : classPtr->m_ctor_list)
        {
            if (detail::reflection_database::does_signature_match_arguments(ctor->get_parameter_types(), args))
                return constructor(ctor.get());
        }
    }
    else
    {
        const auto& ctor = g_ctor_list[get_raw_type().get_id()];
        if (ctor && detail::reflection_database::does_signature_match_arguments(ctor->get_parameter_types(), args))
            return constructor(ctor.get());
    }
    
    return constructor();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<constructor> type::get_constructors() const
{
    if (is_class())
    {
        const auto classPtr = g_class_data_list[get_raw_type().get_id()].get();
        if (!classPtr)
            return { constructor() };

        vector<constructor> result;
        result.reserve(classPtr->m_ctor_list.size());
        for (const auto& ctor : classPtr->m_ctor_list)
        {
            result.push_back(constructor(ctor.get()));
        }
        return result;
    }
    else
    {
        const auto ctor = g_ctor_list[g_raw_type_list[m_id]].get();
        if (ctor)
            return { constructor(ctor) };
    }

    return std::vector<constructor>();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create(vector<detail::argument> args) const
{
    if (is_class())
    {
        const auto classPtr = g_class_data_list[get_raw_type().get_id()].get();
        if (!classPtr)
            return variant();

        for (const auto& ctor : classPtr->m_ctor_list)
        {
            if (detail::reflection_database::does_signature_match_arguments(ctor->get_parameter_types(),
                detail::reflection_database::extract_types(args)))
            {
                return ctor->invoke_variadic(args);
            }
        }
    }
    else
    {
        const auto ctor = g_ctor_list[get_raw_type().get_id()].get();
        if (ctor)
            return ctor->invoke_variadic(args);
    }

    return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

destructor type::get_destructor() const
{
    return destructor(g_dtor_list[m_id].get());
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::destroy(variant& obj) const
{
    const auto dtor = g_dtor_list[m_id].get();
    if (dtor)
        dtor->invoke(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_property(const std::string& name) const
{
    if (const auto& classPtr = g_class_data_list[get_raw_type().get_id()].get())
    {
        if (const auto& ret = detail::reflection_database::find_property(name, classPtr->m_property_map))
            return property(ret);
    }
    
    // search in the base classes, but not recursively, otherwise we would search twice
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_property(name, classPtr->m_property_map))
                return property(ret);
        }
    }

    return property();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(const std::string& name, detail::instance obj) const
{
    const auto prop = get_property(name);
    return prop.get_value(obj);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::get_property_value(const std::string& name)
{
    const auto prop = get_global_property(name);
    return prop.get_value(empty_instance());
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(const std::string& name, detail::instance obj, detail::argument arg) const
{
    const auto prop = get_property(name);
    return prop.set_value(obj, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool type::set_property_value(const std::string& name, detail::argument arg)
{
    const auto prop = get_global_property(name);
    return prop.set_value(empty_instance(), arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

namespace
{

void retrieve_sorted_properties(vector<detail::property_container_base*>& result,
                                const detail::reflection_database::property_map& prop_map)
{
    set<detail::reflection_database::property_container::size_type> sorted_properties;
    for (const auto& prop : prop_map)
    {
        sorted_properties.insert(prop.second);
    }
    for (const auto& prop_id : sorted_properties)
    {
        result.push_back((*g_property_list)[prop_id].get());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void retrieve_sorted_methods(vector<detail::method_container_base*>& result,
                             const detail::reflection_database::method_map& meth_map)
{
    set<detail::reflection_database::method_container::size_type> sorted_methods;
    for (const auto& meth : meth_map)
    {
        sorted_methods.insert(meth.second);
    }
    for (const auto& prop_id : sorted_methods)
    {
        result.push_back((*g_method_list)[prop_id].get());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace anonymous
      
/////////////////////////////////////////////////////////////////////////////////////////
  
vector<property> type::get_properties() const
{
    vector<property> result;
    vector<detail::property_container_base*> props;

    if (auto classPtr = g_class_data_list[get_raw_type().get_id()].get())
    {
        retrieve_sorted_properties(props, classPtr->m_property_map);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (auto classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
            retrieve_sorted_properties(props, classPtr->m_property_map);
    }

    for (const auto& prop : props)
    {
        result.push_back(property(prop));
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(const string& name) const
{
    if (const auto classPtr = g_class_data_list[get_raw_type().get_id()].get())
    {
        if (const auto& ret = detail::reflection_database::find_method(name, classPtr->m_method_map))
            return method(ret);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_method(name, classPtr->m_method_map))
                return method(ret);
        }
    }

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_method(const std::string& name, const std::vector<type>& params) const
{
    if (const auto classPtr = g_class_data_list[get_raw_type().get_id()].get())
    {
        if (const auto& ret = detail::reflection_database::find_method(name, params, classPtr->m_method_map))
            return method(ret);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_method(name, params, classPtr->m_method_map))
                return method(ret);
        }
    }

    return method();
}

/////////////////////////////////////////////////////////////////////////////////////////

vector<method> type::get_methods() const
{
    vector<method> result;
    vector<detail::method_container_base*> methods;

    if (const auto classPtr = g_class_data_list[get_raw_type().get_id()].get())
    {
        retrieve_sorted_methods(methods, classPtr->m_method_map);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
            retrieve_sorted_methods(methods, classPtr->m_method_map);
    }

    for (const auto& meth : methods)
    {
        result.push_back(method(meth));
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

property type::get_global_property(const std::string& name)
{
    return property(detail::reflection_database::find_property(name, *g_global_properties));
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(const std::string& name)
{
    return method(detail::reflection_database::find_method(name, *g_global_methods));
}

/////////////////////////////////////////////////////////////////////////////////////////

method type::get_global_method(const std::string& name, const std::vector<type>& params)
{
    return method(detail::reflection_database::find_method(name, params, *g_global_methods));
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<method> type::get_global_methods()
{
    vector<method> result;
    vector<detail::method_container_base*> methods;

    retrieve_sorted_methods(methods, *g_global_methods);
    
    for (const auto& meth : methods)
    {
        result.push_back(method(meth));
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<property> type::get_global_properties()
{
    vector<property> result;
    vector<detail::property_container_base*> properties;

    retrieve_sorted_properties(properties, *g_global_properties);
    
    for (const auto& prop : properties)
    {
        result.push_back(property(prop));
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration type::get_enumeration() const
{
    return enumeration(g_enumeration_list[get_raw_type().get_id()].get());
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(const std::string& name, detail::instance obj, std::vector<detail::argument> args) const
{
    const auto meth = get_method(name, detail::reflection_database::extract_types(args));
    return meth.invoke_variadic(obj, std::move(args));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(const std::string& name, std::vector<detail::argument> args)
{
    const auto meth = get_global_method(name, detail::reflection_database::extract_types(args));
    return meth.invoke_variadic(empty_instance(), std::move(args));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

type type::get_by_name(const char* name)
{
#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    const auto orig_type_name = name + std::string(">(void)");
#elif RTTR_COMPILER == RTTR_COMPILER_GNUC
    const auto orig_type_name = name + std::string("]");
#endif

   const auto itr = g_name_to_id->find(orig_type_name.c_str());
   if (itr != g_name_to_id->end())
       return type(itr->second);
   else
       return type();
   
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

type type::register_type(const char* name, 
                         const type& raw_type,
                         vector<detail::base_class_info> base_classes,
                         detail::derived_info(*get_derived_func)(void*),
                         detail::variant_create_func var_func_ptr,
                         bool is_class,
                         bool is_enum,
                         bool is_array,
                         bool is_pointer,
                         bool is_primitive,
                         bool is_function_pointer,
                         bool is_member_object_pointer,
                         bool is_member_function_pointer,
                         std::size_t pointer_dimension)
{
    init_globals();

    std::lock_guard<std::mutex> lock(*g_register_type_mutex);
    using namespace detail;

    
    reflection_database& db = reflection_database::instance();
    {
        const auto itr = g_name_to_id->find(name);
        if (itr != g_name_to_id->end())
            return type(itr->second);
    }

    const auto ret = g_name_to_id->emplace(name, ++db.type_id_counter);
    if (ret.second)
    {
        g_name_list[db.type_id_counter] = name;
        const type::type_id raw_id = ((raw_type.get_id() == 0) ? db.type_id_counter : raw_type.get_id());
        g_raw_type_list[db.type_id_counter] = raw_id;
        g_get_derived_info_func_list[raw_id]  = get_derived_func;
        g_variant_create_list[db.type_id_counter] = var_func_ptr;
        const std::size_t row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
        std::size_t index = 0;
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
                itr = vector<detail::base_class_info>::reverse_iterator(base_classes.erase((++itr).base()));
            }
        }
        
        for (const auto& type : base_classes)
        {
            g_base_class_list[row + index] = type.m_base_type.get_id();
            g_conversion_list[row + index] = type.m_rttr_cast_func;
            ++index;
        }

        for (const auto& type : base_classes)
        {
            const std::size_t row = RTTR_MAX_INHERIT_TYPES_COUNT * type.m_base_type.get_raw_type().get_id();
            for (std::size_t i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
            {
                if (g_derived_class_list[row + i] == 0)
                {
                    g_derived_class_list[row + i] = db.type_id_counter;
                    break;
                }
                    
            }
        }

        db.is_class_list                    [ret.first->second] = is_class;
        db.is_enum_list                     [ret.first->second] = is_enum;
        db.is_array_list                    [ret.first->second] = is_array;
        db.is_pointer_list                  [ret.first->second] = is_pointer;
        db.is_primitive_list                [ret.first->second] = is_primitive;
        db.is_function_pointer_list         [ret.first->second] = is_function_pointer;
        db.is_member_object_pointer_list    [ret.first->second] = is_member_object_pointer;
        db.is_member_function_pointer_list  [ret.first->second] = is_member_function_pointer;
        db.pointer_dim_list                 [ret.first->second] = pointer_dimension;
    } // else cannot happen!

    return type(ret.first->second);
}

/////////////////////////////////////////////////////////////////////////////////////////

detail::type_converter_base* type::get_type_converter(const type& target_type) const
{
    const auto& converter_list = g_type_converter_list[m_id];

    for (const auto& converter : converter_list)
    {
        if (converter.get()->m_target_type == target_type)
            return converter.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::register_type_converter(std::unique_ptr<detail::type_converter_base> converter) const
{
    std::lock_guard<std::mutex> lock(*g_register_type_mutex); // registration has to be synchronized

    const auto& converter_list = g_type_converter_list[m_id];
    for (const auto& conv : converter_list)
    {
        if (conv.get()->m_target_type == converter->m_target_type)
            return;
    }
    g_type_converter_list[m_id].push_back(move(converter));
    g_type_converter_list[m_id].shrink_to_fit();
}

/////////////////////////////////////////////////////////////////////////////////////////

namespace impl
{

void register_property(type curr_type, unique_ptr<detail::property_container_base> prop)
{
    if (curr_type.is_class())
    {
        const type::type_id raw_id = curr_type.get_raw_type().get_id();
        if (!g_class_data_list[raw_id])
            g_class_data_list[raw_id].reset(new detail::reflection_database::class_data);

        detail::reflection_database::register_property(move(prop), g_class_data_list[raw_id]->m_property_map);
    }
    else
    {
        detail::reflection_database::register_property(move(prop), detail::reflection_database::instance().global_properties);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_method(type curr_type, std::unique_ptr<detail::method_container_base> method)
{
    if (curr_type.is_class())
    {
        const type::type_id raw_id = curr_type.get_raw_type().get_id();
        if (!g_class_data_list[raw_id])
            g_class_data_list[raw_id].reset(new detail::reflection_database::class_data);

        detail::reflection_database::register_method(move(method), g_class_data_list[raw_id]->m_method_map);
    }
    else
    {
        detail::reflection_database::register_method(move(method), detail::reflection_database::instance().global_methods);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_constructor(type t, std::unique_ptr<detail::constructor_container_base> ctor)
{
    if (t.is_class())
    {
        const type::type_id raw_id = t.get_raw_type().get_id();
        if (!g_class_data_list[raw_id])
            g_class_data_list[raw_id].reset(new detail::reflection_database::class_data);

        g_class_data_list[raw_id]->add_constructor(move(ctor));
    }
    else if (t.is_primitive() || t.is_pointer())
    {
        g_ctor_list[t.get_raw_type().get_id()] = move(ctor);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_destructor(type t, std::unique_ptr<detail::destructor_container_base> dtor)
{
    g_dtor_list[t.get_raw_type().get_id()] = move(dtor);
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_enumeration(type t, std::unique_ptr<detail::enumeration_container_base> enum_item)
{
    g_enumeration_list[t.get_raw_type().get_id()] = move(enum_item);
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_custom_name(type t, std::string name)
{
    std::lock_guard<std::mutex> lock(*g_register_custom_name_mutex);

    auto& custom_name_map = *g_custom_name_map;

    if (custom_name_map.find(t) != custom_name_map.end())
        return;

    custom_name_map.emplace(t, name);
    
}

/////////////////////////////////////////////////////////////////////////////////////////

void register_metadata(type t, std::vector< rttr::metadata > metadata)
{
     for (auto& item : metadata)
    {
        auto key    = item.get_key();
        auto value  = item.get_value();
        if (key.is_type<int>())
            g_metadata_list[t.get_id()].set_metadata(key.get_value<int>(), std::move(value));
        else if (key.is_type<std::string>())
            g_metadata_list[t.get_id()].set_metadata(std::move(key.get_value<std::string>()), std::move(value));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace impl
} // end namespace rttr
