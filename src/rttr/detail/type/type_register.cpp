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

#include "rttr/detail/type/type_register.h"

#include "rttr/detail/type/type_register_p.h"
#include "rttr/detail/type/type_database_p.h"

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

#include "rttr/detail/filter/filter_item_funcs.h"

using namespace std;

namespace rttr
{
namespace detail
{

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
    type_database::instance().register_enumeration(t, move(enum_item));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::custom_name(type& t, string_view custom_name)
{
    type_database::instance().register_custom_name(t, custom_name);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::metadata(const type& t, std::vector< ::rttr::detail::metadata > data)
{
    type_database::instance().register_metadata(t, move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::converter(const type& t, std::unique_ptr<type_converter_base> converter)
{
     type_database::instance().register_converter(t, move(converter));
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register::comparator(const type& t, type_comparator_base* comparator)
{
    type_database::instance().register_comparator(t, comparator);
}

/////////////////////////////////////////////////////////////////////////////////////////

type type_register::type_reg(type_data& info) RTTR_NOEXCEPT
{
    return type_database::instance().register_type(info);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// Here comes the private implementation of the registration class 'type_register_private'

void type_register_private::constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor)
{
    static std::vector<std::unique_ptr<constructor_wrapper_base> > constructor_list;
    if (!t.is_valid())
        return;

    auto& class_data = t.m_type_data->get_class_data();
    class_data.m_ctors.emplace_back(create_item<::rttr::constructor>(ctor.get()));
    constructor_list.push_back(std::move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor)
{
    static std::vector<std::unique_ptr<destructor_wrapper_base> > destructor_list;

    auto& dtor_type = t.m_type_data->get_class_data().m_dtor;
    if (!dtor_type) // when no dtor is set at the moment
    {
        auto d = create_item<::rttr::destructor>(dtor.get());
        dtor_type = d;
        destructor_list.push_back(std::move(dtor));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

flat_multimap<string_view, ::rttr::property>& type_register_private::get_global_property_storage()
{
    static flat_multimap<string_view, ::rttr::property> props;
    return props;
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<std::unique_ptr<detail::property_wrapper_base>>& get_property_storage()
{
    static std::vector<std::unique_ptr<detail::property_wrapper_base>> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::property(const type& t, std::unique_ptr<property_wrapper_base> prop)
{
    if (!t.is_valid())
        return;

    const auto name = prop->get_name();

    if (t.is_class())
    {
        auto& property_list = t.m_type_data->get_class_data().m_properties;

        if (get_type_property(t, name))
            return;

        property_list.emplace_back(detail::create_item<::rttr::property>(prop.get()));
        get_property_storage().push_back(std::move(prop));
        update_class_list(t, &detail::class_data::m_properties);
    }
    else
    {
        if (t.get_global_property(name))
            return;

        auto p = detail::create_item<::rttr::property>(prop.get());
        get_global_property_storage().insert(std::move(name), std::move(p));
        get_property_storage().push_back(std::move(prop));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

flat_multimap<string_view, ::rttr::method>& type_register_private::get_global_method_storage()
{
    static flat_multimap<string_view, ::rttr::method> meths;
    return meths;
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::vector<std::unique_ptr<method_wrapper_base>>& get_method_storage()
{
    static std::vector<std::unique_ptr<method_wrapper_base>> container;
    return container;
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_register_private::method(const type& t, std::unique_ptr<method_wrapper_base> meth)
{
    if (!t.is_valid())
        return;

    const auto name = meth->get_name();
    if (t.is_class())
    {
        if (get_type_method(t, name, convert_param_list(meth->get_parameter_infos())))
            return;

        auto& method_list = t.m_type_data->get_class_data().m_methods;
        method_list.emplace_back(create_item<::rttr::method>(meth.get()));
        get_method_storage().push_back(std::move(meth));
        update_class_list(t, &class_data::m_methods);
    }
    else
    {
        if (t.get_global_method(name, convert_param_list(meth->get_parameter_infos())))
            return;

        auto m = create_item<::rttr::method>(meth.get());
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

} // end namespace detail
} // end
