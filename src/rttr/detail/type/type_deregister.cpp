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

#include "rttr/detail/type/type_deregister.h"

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

template<typename T, typename I>
static void remove_wrapper_item(T& container, I& item)
{
     container.erase(std::remove_if(container.begin(), container.end(),
                     [&item](std::unique_ptr<class_item_to_wrapper_t<raw_type_t<I>>>& item_)
                            { return (create_item<raw_type_t<I>>(item_.get()) == item); }),
                     container.end());
}

template<typename T, typename I>
static void remove_item(T& container, I& item)
{
     container.erase(std::remove_if(container.begin(), container.end(),
                     [&item](I& item_)
                            { return (item_== item); }),
                     container.end());
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::item(const type& t) RTTR_NOEXCEPT
{
    auto& type_data_container = type_register_private::get_type_data_storage();

    auto& info = *t.m_type_data;
    type_data_container.erase(std::remove_if(type_data_container.begin(), type_data_container.end(),
                                             [info](type_data* data_info) { return (data_info == &info); }),
                              type_data_container.end());

    auto& orig_name_to_id = type_register_private::get_orig_name_to_id();
    orig_name_to_id.erase(info.type_name);

    if (t.is_class())
    {
        info.get_class_data().m_methods.clear();
        info.get_class_data().m_method_storage.clear();

        info.get_class_data().m_properties.clear();
        info.get_class_data().m_property_storage.clear();

        info.get_class_data().m_constructor_storage.clear();
        info.get_class_data().m_dtor_storage.reset();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::item(const property& prop) RTTR_NOEXCEPT
{

}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::item(const method& meth) RTTR_NOEXCEPT
{
    auto& class_data = meth.get_declaring_type().m_type_data->get_class_data();

    auto& methods = class_data.m_methods;
    methods.erase(std::remove_if(methods.begin(), methods.end(),
                 [&meth](const method& item) { return (item == meth); }),
                 methods.end());

    auto& method_wrappers = class_data.m_method_storage;
    remove_wrapper_item(method_wrappers, meth);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::global_method(const method& meth) RTTR_NOEXCEPT
{
    auto& g_methods = type_register_private::get_global_method_storage();
    g_methods.erase(meth.get_name());
    remove_wrapper_item(type_register_private::get_method_storage(), meth);
    remove_item(type_register_private::get_global_methods(), meth);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::global_property(const property& prop) RTTR_NOEXCEPT
{
    auto& g_props = type_register_private::get_global_property_storage();
    g_props.erase(prop.get_name());

    remove_wrapper_item(type_register_private::get_property_storage(), prop);
    remove_item(type_register_private::get_global_properties(), prop);
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::item(const constructor& ctor) RTTR_NOEXCEPT
{
}

/////////////////////////////////////////////////////////////////////////////////////////

void type_deregister::item(const enumeration& e) RTTR_NOEXCEPT
{

}

/////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
