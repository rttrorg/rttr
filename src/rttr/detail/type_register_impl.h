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

#ifndef RTTR_TYPE_REGISTER_IMPL_H_
#define RTTR_TYPE_REGISTER_IMPL_H_

#include <memory>
#include <string>
#include <vector>

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type_register::store_metadata(T& obj, std::vector<rttr::metadata> data)
{
    for (auto& item : data)
    {
        auto key    = item.get_key();
        auto value  = item.get_value();
        if (key.is_type<int>())
            obj->set_metadata(key.get_value<int>(), std::move(value));
        else if (key.is_type<std::string>())
            obj->set_metadata(std::move(key.get_value<std::string>()), std::move(value));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename... Args>
void type_register::constructor(std::vector< rttr::metadata > data)
{
    using namespace std;
    const type type = type::get<ClassType>();
    auto ctor = detail::make_unique<constructor_container<ClassType, Args...>>();
    // register the type with the following call:
    ctor->get_instanciated_type();
    ctor->get_parameter_types();

    store_metadata(ctor, std::move(data));
    constructor(type, move(ctor));
    destructor(type, detail::make_unique<destructor_container<ClassType>>());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename A, typename Policy>
void type_register::property(const std::string& name, A accessor, std::vector<rttr::metadata> data, const Policy&)
{
    using namespace std;
    using getter_policy = typename get_getter_policy<Policy>::type;
    using setter_policy = typename get_setter_policy<Policy>::type;
    using acc_type      = typename property_type<A>::type;

    auto prop = detail::make_unique<property_container<acc_type, A, void, getter_policy, setter_policy>>(name, type::get<ClassType>(), accessor);
    // register the type with the following call:
    prop->get_type();
    store_metadata(prop, std::move(data));
    property(type::get<ClassType>(), move(prop));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename A1, typename A2, typename Policy>
void type_register::property(const std::string& name, A1 getter, A2 setter, std::vector<rttr::metadata> data, const Policy&)
{
    using namespace std;
    using getter_policy = typename get_getter_policy<Policy>::type;
    using setter_policy = typename get_setter_policy<Policy>::type;
    using acc_type      = typename property_type<A1>::type;

    auto prop = detail::make_unique<property_container<acc_type, A1, A2, getter_policy, setter_policy>>(name, type::get<ClassType>(), getter, setter);
    // register the type with the following call:
    prop->get_type();
    store_metadata(prop, std::move(data));
    property(type::get<ClassType>(), move(prop));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename A, typename Policy>
void type_register::property_readonly(const std::string& name, A accessor, std::vector<rttr::metadata> data, const Policy&)
{
    using namespace std;
    using getter_policy = typename get_getter_policy<Policy>::type;
    using setter_policy = read_only;
    using acc_type      = typename property_type<A>::type;

    auto prop = detail::make_unique<property_container<acc_type, A, void, getter_policy, setter_policy>>(name, type::get<ClassType>(), accessor);
    // register the type with the following call:
    prop->get_type();
    store_metadata(prop, std::move(data));
    property(type::get<ClassType>(), move(prop));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename F, typename Policy>
void type_register::method(const std::string& name, F function, std::vector< rttr::metadata > data, const Policy&)
{
    using namespace std;
    using method_policy = typename get_method_policy<Policy>::type;

    auto meth = detail::make_unique<method_container<F, method_policy>>(name, type::get<ClassType>(), function);
    // register the underlying type with the following call:
    meth->get_return_type();
    meth->get_parameter_types();
    store_metadata(meth, std::move(data));
    method(type::get<ClassType>(), move(meth));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename EnumType>
void type_register::enumeration(std::string name, std::vector< std::pair< std::string, EnumType> > enum_data, std::vector<rttr::metadata> data)
{
    using namespace std;
    static_assert(is_enum<EnumType>::value, "No enum type provided, please call this method with an enum type!");

    type enum_type = type::get<EnumType>();
    if (!name.empty())
        custom_name(enum_type, std::move(name));

    type declar_type = get_invalid_type();
    if (!std::is_same<ClassType, void>::value)
        declar_type = type::get<ClassType>();

    auto enum_item = detail::make_unique<enumeration_container<EnumType>>(declar_type, move(enum_data));
    // register the underlying type with the following call:
    enum_item->get_type();

    store_metadata(enum_item, std::move(data));
    enumeration(enum_type, move(enum_item));
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_REGISTER_H_
