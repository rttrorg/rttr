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

#ifndef RTTR_REGISTRATION_IMPL_H_
#define RTTR_REGISTRATION_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/argument_extractor.h"
#include "rttr/detail/constructor/constructor_wrapper.h"
#include "rttr/detail/destructor/destructor_wrapper.h"
#include "rttr/detail/enumeration/enumeration_wrapper.h"
#include "rttr/detail/method/method_wrapper.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/type/accessor_type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/type/type_register.h"
#include "rttr/detail/registration/bind_impl.h"
#include "rttr/policy.h"

#include <string>
#include <vector>

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

static RTTR_INLINE detail::meta_data meta_data(variant key, variant value)
{
    return detail::meta_data{std::move(key), std::move(value)}; 
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Enum_Type>
static RTTR_INLINE detail::enum_data<Enum_Type> value(const char* name, Enum_Type value)
{
    return detail::enum_data<Enum_Type>(name, value);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
registration::class_<Class_Type>::class_(const char* name)
{
    auto t = type::get<Class_Type>();

    if (name != nullptr)
        detail::type_register::custom_name(t, name);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
registration::class_<Class_Type>::class_(const std::shared_ptr<detail::registration_executer>& reg_exec)
:   m_reg_exec(reg_exec)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
registration::class_<Class_Type>::~class_()
{
    // make sure that all base classes are registered
    detail::base_classes<Class_Type>::get_types();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename...Args>
registration::class_<Class_Type>& registration::class_<Class_Type>::operator()(Args&&...args)
{
    auto data = detail::get_meta_data(std::forward<Args>(args)...);
    detail::type_register::meta_data(type::get<Class_Type>(), std::move(data));
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename... Args>
registration::bind<detail::ctor, Class_Type, Args...> registration::class_<Class_Type>::constructor()
{
    return {create_if_empty(m_reg_exec)};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename F>
registration::bind<detail::ctor_func, Class_Type, F> registration::class_<Class_Type>::constructor(F func)
{
    static_assert(std::is_same<detail::return_func, typename detail::method_type<F>::type>::value, "For creating this 'class type', please provide a function pointer or std::function with a return value.");
    return {create_if_empty(m_reg_exec), func};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename A, typename acc_level, typename Tp>
registration::bind<detail::prop, Class_Type, A> registration::class_<Class_Type>::property(const char* name, A acc, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {create_if_empty(m_reg_exec), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename A, typename acc_level, typename Tp>
registration::bind<detail::prop_readonly, Class_Type, A> registration::class_<Class_Type>::property_readonly(const char* name, A acc, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {create_if_empty(m_reg_exec), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename A1, typename A2,  typename Tp, typename acc_level>
registration::bind<detail::prop, Class_Type, A1, A2> registration::class_<Class_Type>::property(const char* name, A1 getter, A2 setter, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {create_if_empty(m_reg_exec), name, getter, setter};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename F, typename acc_level>
registration::bind<detail::meth, Class_Type, F> registration::class_<Class_Type>::method(const char* name, F f, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {create_if_empty(m_reg_exec), name , f};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type>
template<typename Enum_Type>
registration::bind<detail::enum_, Class_Type, Enum_Type> registration::class_<Class_Type>::enumeration(const char* name)
{
    static_assert(std::is_enum<Enum_Type>::value, "No enum type provided, please call this method with an enum type!");
    return {create_if_empty(m_reg_exec), name};
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename acc_level, typename Tp>
registration::bind<detail::prop, void, A> registration::property(const char* name, A acc, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {std::make_shared<detail::registration_executer>(), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename acc_level, typename Tp>
registration::bind<detail::prop_readonly, void, A> registration::property_readonly(const char* name, A acc, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {std::make_shared<detail::registration_executer>(), name, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2, typename Tp, typename acc_level>
registration::bind<detail::prop, void, A1, A2> registration::property(const char* name, A1 getter, A2 setter, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {std::make_shared<detail::registration_executer>(), name, getter, setter};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename acc_level>
registration::bind<detail::meth, void, F> registration::method(const char* name, F f, acc_level level)
{
    static_assert(detail::contains<acc_level, detail::access_level_list>::value, "The given type of 'level' is not a type of 'rttr::access_level.'");
    return {std::make_shared<detail::registration_executer>(), name, f};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Enum_Type>
registration::bind<detail::enum_, void, Enum_Type> registration::enumeration(const char* name)
{
    static_assert(std::is_enum<Enum_Type>::value, "No enum type provided, please call this method with an enum type!");
    return {std::make_shared<detail::registration_executer>(), name};
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#define RTTR_REGISTER                                                   \
static void rttr__auto_register_reflection_function__();                \
namespace                                                               \
{                                                                       \
    struct rttr__auto__register__                                       \
    {                                                                   \
        rttr__auto__register__()                                        \
        {                                                               \
            rttr__auto_register_reflection_function__();                \
        }                                                               \
    };                                                                  \
}                                                                       \
static const rttr__auto__register__ RTTR_CAT(auto_register__,__LINE__); \
static void rttr__auto_register_reflection_function__()


#endif // RTTR_REGISTRATION_IMPL_H_
