/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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

#include "rttr/detail/reflection_database_p.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <thread>
#include <mutex>

using namespace std;

namespace rttr
{
static detail::reflection_database::NameToTag               *g_name_to_id                   = nullptr;
static string                                               *g_name_list                    = nullptr;
static type::type_id                                        *g_base_class_list              = nullptr;
static type::type_id                                        *g_derived_class_list           = nullptr;
static detail::reflection_database::rttr_cast_func          *g_conversion_list              = nullptr;
static detail::reflection_database::get_derived_info_func   *g_get_derived_info_func_list   = nullptr;
static detail::reflection_database::variant_create_func     *g_variant_create_list          = nullptr;
static type::type_id                                        *g_raw_type_list                = nullptr;
static bool                                                 *g_is_class_list                = nullptr;
static bool                                                 *g_is_enum_list                 = nullptr;
static bool                                                 *g_is_array_list                = nullptr;
static bool                                                 *g_is_pointer_list              = nullptr;
static bool                                                 *g_is_primitive_list            = nullptr;
static unique_ptr<detail::constructor_container_base>       *g_ctor_list                    = nullptr;
static unique_ptr<detail::destructor_container_base>        *g_dtor_list                    = nullptr;
static unique_ptr<detail::enumeration_container_base>       *g_enumeration_list             = nullptr;
static std::vector<std::unique_ptr<detail::type_converter_base>> *g_type_converter_list          = nullptr;
static unique_ptr<detail::reflection_database::class_data>  *g_class_data_list              = nullptr;
static detail::reflection_database::property_map            *g_global_properties            = nullptr;
static detail::reflection_database::method_map              *g_global_methods               = nullptr;

static detail::reflection_database::constructor_container   *g_constructor_list             = nullptr;
static detail::reflection_database::destructor_container    *g_destructor_list              = nullptr;
static detail::reflection_database::method_container        *g_method_list                  = nullptr;
static detail::reflection_database::property_container      *g_property_list                = nullptr;


static std::mutex register_type_mutex;

// because everything is initialized at static initialization time the call to
// register_type can be made from another translation unit before global statics
// like 'g_name_list' are initialized, therefore we make a small singleton called type_data
//
// Before any other static function of type is called, a call to register has to be made first,
// otherwise a compile time error occurs.
static void init_globals()
{
    static bool initialized = false;
    if (initialized)
        return;

    detail::reflection_database& db = detail::reflection_database::instance();

    g_name_to_id                = &db.name_to_id;
    g_name_list                 = db.name_list;
    g_base_class_list           = db.base_class_list;
    g_derived_class_list        = db.derived_class_list;
    g_conversion_list           = db.conversion_list;
    g_variant_create_list       = db.variant_create_func_list;
    g_get_derived_info_func_list= db.get_derived_info_func_list;
    g_raw_type_list             = db.raw_type_list;
    g_is_class_list             = db.is_class_list;
    g_is_enum_list              = db.is_enum_list;
    g_is_array_list             = db.is_array_list;
    g_is_pointer_list           = db.is_pointer_list;
    g_is_primitive_list         = db.is_primitive_list;
    g_class_data_list           = db.class_data_list;
    g_ctor_list                 = db.constructor_list;
    g_dtor_list                 = db.destructor_list;
    g_enumeration_list          = db.enumeration_list;
    g_global_properties         = &db.global_properties;
    g_global_methods            = &db.global_methods;
    g_constructor_list          = &db._constructor_list;
    g_destructor_list           = &db._destructor_list;
    g_method_list               = &db._method_list;
    g_property_list             = &db._property_list;
    g_type_converter_list       = db.type_converter_list;

    initialized = true;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string type::get_name() const
{
    if (is_valid())
        return g_name_list[m_id];
    else
        return std::string();
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
    source_raw_id = g_raw_type_list[info._type.m_id];
    if (source_raw_id == target_raw_id)
        return info._ptr;

    const int row = RTTR_MAX_INHERIT_TYPES_COUNT * source_raw_id;
    for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
    {
        const type::type_id currId = g_base_class_list[row + i];
        if (currId == target_raw_id)
            return g_conversion_list[row + i](info._ptr);
        if (currId == 0) // invalid id
            return nullptr;
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::create_from_ptr(void* ptr) const
{
    return g_variant_create_list[m_id](ptr);
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

constructor type::get_constructor(const std::vector<type>& args) const
{
    if (is_class())
    {
        const auto classPtr = g_class_data_list[get_raw_type().get_id()].get();
        if (!classPtr)
            return constructor();
        
        for (const auto& ctor : classPtr->_ctorList)
        {
            if (detail::reflection_database::does_signature_match_arguments(ctor->get_parameter_types(), args))
                return constructor(ctor.get());
        }
    }
    else
    {
        const auto& ctor = g_ctor_list[get_raw_type().get_id()];
        if (detail::reflection_database::does_signature_match_arguments(ctor->get_parameter_types(), args))
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
        result.reserve(classPtr->_ctorList.size());
        for (const auto& ctor : classPtr->_ctorList)
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

        for (const auto& ctor : classPtr->_ctorList)
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
        if (const auto& ret = detail::reflection_database::find_property(name, classPtr->_property_map))
            return property(ret);
    }
    
    // search in the base classes, but not recursively, otherwise we would search twice
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_property(name, classPtr->_property_map))
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
        retrieve_sorted_properties(props, classPtr->_property_map);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (auto classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
            retrieve_sorted_properties(props, classPtr->_property_map);
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
        if (const auto& ret = detail::reflection_database::find_method(name, classPtr->_method_map))
            return method(ret);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_method(name, classPtr->_method_map))
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
        if (const auto& ret = detail::reflection_database::find_method(name, params, classPtr->_method_map))
            return method(ret);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto& classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
        {
            if (const auto& ret = detail::reflection_database::find_method(name, params, classPtr->_method_map))
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
        retrieve_sorted_methods(methods, classPtr->_method_map);
    }
    
    // search in the base classes, but not recursively
    for (const auto& type : get_base_classes())
    {
        if (const auto classPtr = g_class_data_list[type.get_raw_type().get_id()].get())
            retrieve_sorted_methods(methods, classPtr->_method_map);
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
    return meth.invoke_variadic(obj, args);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant type::invoke(const std::string& name, std::vector<detail::argument> args)
{
    const auto meth = get_global_method(name, detail::reflection_database::extract_types(args));
    return meth.invoke_variadic(empty_instance(), args);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

type type::get(const char* name)
{
   const auto itr = g_name_to_id->find(name);
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
                         variant(*variant_create_func)(void*),
                         bool is_class,
                         bool is_enum,
                         bool is_array,
                         bool is_pointer,
                         bool is_primitive)
{
    std::lock_guard<std::mutex> lock(register_type_mutex);

    using namespace detail;
    init_globals();
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
        g_variant_create_list[db.type_id_counter] = variant_create_func;
        const int row = RTTR_MAX_INHERIT_TYPES_COUNT * raw_id;
        int index = 0;
        // remove double entries; can only be happen for virtual inheritance case
        set<type> double_entries;
        for (auto itr = base_classes.rbegin(); itr != base_classes.rend();)
        {
            if (double_entries.find(itr->_base_type) == double_entries.end())
            {
                double_entries.insert(itr->_base_type);
                ++itr;
            }
            else
            {
                itr = vector<detail::base_class_info>::reverse_iterator(base_classes.erase((++itr).base()));
            }
        }
        
        for (const auto& type : base_classes)
        {
            g_base_class_list[row + index] = type._base_type.get_id();
            g_conversion_list[row + index] = type._rttr_cast_func;
            ++index;
        }

        for (const auto& type : base_classes)
        {
            const int row = RTTR_MAX_INHERIT_TYPES_COUNT * type._base_type.get_raw_type().get_id();
            for (int i = 0; i < RTTR_MAX_INHERIT_TYPES_COUNT; ++i)
            {
                if (g_derived_class_list[row + i] == 0)
                {
                    g_derived_class_list[row + i] = db.type_id_counter;
                    break;
                }
                    
            }
        }

        db.is_class_list    [ret.first->second] = is_class;
        db.is_enum_list     [ret.first->second] = is_enum;
        db.is_array_list    [ret.first->second] = is_array;
        db.is_pointer_list  [ret.first->second] = is_pointer;
        db.is_primitive_list[ret.first->second] = is_primitive;
    } // else cannot happen!

    return type(ret.first->second);
}

/////////////////////////////////////////////////////////////////////////////////////////

detail::type_converter_base* type::get_type_converter(const type& target_type) const
{
    const auto& converter_list = g_type_converter_list[m_id];

    for (const auto& converter : converter_list)
    {
        if (converter.get()->_target_type == target_type)
            return converter.get();
    }

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type::register_type_converter(std::unique_ptr<detail::type_converter_base> converter) const
{
    const auto& converter_list = g_type_converter_list[m_id];
    for (const auto& conv : converter_list)
    {
        if (conv.get()->_target_type == converter->_target_type)
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

        detail::reflection_database::register_property(move(prop), g_class_data_list[raw_id]->_property_map);
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

        detail::reflection_database::register_method(move(method), g_class_data_list[raw_id]->_method_map);
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

} // end namespace impl
} // end namespace rttr
