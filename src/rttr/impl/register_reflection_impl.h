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

#ifndef RTTR_REGISTER_REFLECTION_IMPL_H_
#define RTTR_REGISTER_REFLECTION_IMPL_H_

#include "rttr/detail/constructor_container.h"
#include "rttr/detail/destructor_container.h"
#include "rttr/detail/enumeration_container.h"
#include "rttr/detail/method_container.h"
#include "rttr/detail/property_container.h"
#include "rttr/detail/accessor_type.h"
#include "rttr/detail/misc_type_traits.h"
#include "rttr/detail/utility.h"
#include "rttr/detail/type_register.h"

#include <type_traits>
#include <memory>

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void constructor_(std::vector< rttr::metadata > data)
{
    detail::type_register::constructor<T>(std::move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A>
void property_(const std::string& name, A acc)
{
    using namespace std;
    static_assert(is_pointer<A>::value, "No valid property accessor provided!");
    detail::type_register::property<void, A>(name, acc, std::vector< rttr::metadata >(), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A>
void property_(const std::string& name, A acc, std::vector< rttr::metadata > data)
{
    using namespace std;
    static_assert(is_pointer<A>::value, "No valid property accessor provided!");
    detail::type_register::property<void, A>(name, acc, std::move(data), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Policy>
void property_(const std::string& name, A acc, 
               const Policy& policy, typename std::enable_if<detail::contains<Policy, detail::policy_list>::value>::type*)
{
    using namespace std;
    static_assert(is_pointer<A>::value, "No valid property accessor provided!");
    detail::type_register::property<void, A>(name, acc, std::vector< rttr::metadata >(), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Policy>
void property_(const std::string& name, A acc, std::vector< rttr::metadata > data,
               const Policy& policy)
{
    using namespace std;
    static_assert(is_pointer<A>::value, "No valid property accessor provided!");
    detail::type_register::property<void, A>(name, acc, std::move(data), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2>
void property_(const std::string& name, A1 getter, A2 setter, 
               typename std::enable_if<!detail::contains<A2, detail::policy_list>::value>::type*)
{
    using namespace std;
    static_assert(is_pointer<A1>::value || is_pointer<A2>::value ||
                  detail::is_function_ptr<A1>::value || detail::is_function_ptr<A2>::value ||
                  detail::is_std_function<A1>::value || detail::is_std_function<A2>::value, 
                  "No valid property accessor provided!");
    detail::type_register::property<void, A1, A2>(name, getter, setter, std::vector< rttr::metadata >(), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2>
void property_(const std::string& name, A1 getter, A2 setter, std::vector< rttr::metadata > data)
{
    using namespace std;
    static_assert(is_pointer<A1>::value || is_pointer<A2>::value ||
                  detail::is_function_ptr<A1>::value || detail::is_function_ptr<A2>::value ||
                  detail::is_std_function<A1>::value || detail::is_std_function<A2>::value, 
                  "No valid property accessor provided!");
    detail::type_register::property<void, A1, A2>(name, getter, setter, move(data), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2, typename Policy>
void property_(const std::string& name, A1 getter, A2 setter,
               const Policy& policy)
{
    using namespace std;
    static_assert(is_pointer<A1>::value || is_pointer<A2>::value ||
                  detail::is_function_ptr<A1>::value || detail::is_function_ptr<A2>::value ||
                  detail::is_std_function<A1>::value || detail::is_std_function<A2>::value, 
                  "No valid property accessor provided!");
    detail::type_register::property<void, A1, A2>(name, getter, setter, std::vector< rttr::metadata >(), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A1, typename A2, typename Policy>
void property_(const std::string& name, A1 getter, A2 setter, std::vector< rttr::metadata > data,
               const Policy& policy)
{
    using namespace std;
    static_assert(is_pointer<A1>::value || is_pointer<A2>::value ||
                  detail::is_function_ptr<A1>::value || detail::is_function_ptr<A2>::value ||
                  detail::is_std_function<A1>::value || detail::is_std_function<A2>::value, 
                  "No valid property accessor provided!");
    detail::type_register::property<void, A1, A2>(name, getter, setter, std::move(data), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A>
void property_readonly_(const std::string& name, A acc)
{
    using namespace std;
    static_assert(is_pointer<A>::value || detail::is_function_ptr<A>::value || detail::is_std_function<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property_readonly<void, A>(name, acc, std::vector< rttr::metadata >(), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A>
void property_readonly_(const std::string& name, A acc, std::vector< rttr::metadata > data)
{
    using namespace std;
    static_assert(is_pointer<A>::value || detail::is_function_ptr<A>::value || detail::is_std_function<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property_readonly<void, A>(name, acc, std::move(data), detail::default_property_policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Policy>
void property_readonly_(const std::string& name, A acc, const Policy& policy)
{
    using namespace std;
    static_assert(is_pointer<A>::value || detail::is_function_ptr<A>::value || detail::is_std_function<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property_readonly<void, A>(name, acc, std::vector< rttr::metadata >(), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename A, typename Policy>
void property_readonly_(const std::string& name, A acc, std::vector< rttr::metadata > data, const Policy& policy)
{
    using namespace std;
    static_assert(is_pointer<A>::value || detail::is_function_ptr<A>::value || detail::is_std_function<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property_readonly<void, A>(name, acc, std::move(data), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F>
void method_(const std::string& name, F function)
{
    detail::type_register::method<void>(name, function, std::vector< rttr::metadata >(), detail::default_invoke());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F>
void method_(const std::string& name, F function, std::vector< rttr::metadata > data)
{
    detail::type_register::method<void>(name, function, std::move(data), detail::default_invoke());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy>
void method_(const std::string& name, F function, const Policy& policy)
{
    detail::type_register::method<void>(name, function, std::vector< rttr::metadata >(), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F, typename Policy>
void method_(const std::string& name, F function, std::vector< rttr::metadata > data, const Policy& policy)
{
    detail::type_register::method<void>(name, function, std::move(data), policy);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename EnumType>
void enumeration_(std::string name, std::vector< std::pair< std::string, EnumType> > enum_data, std::vector<rttr::metadata> data)
{
    detail::type_register::enumeration<void, EnumType>(std::move(name), std::move(enum_data), std::move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
class_<ClassType>::class_(std::string name, std::vector< rttr::metadata > data)
{
    auto t = type::get<ClassType>();

    if (!name.empty())
        detail::type_register::custom_name(t, std::move(name));

    detail::type_register::metadata(t, std::move(data));

    static_assert(std::is_class<ClassType>::value, "Reflected type is not a class or struct.");
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
class_<ClassType>::~class_()
{
    // make sure that all base classes are registered
    detail::base_classes<ClassType>::get_types();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename... Args>
class_<ClassType>& class_<ClassType>::constructor(std::vector<rttr::metadata> data)
{
    detail::type_register::constructor<ClassType, Args...>(std::move(data));
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A acc)
{
    static_assert(std::is_member_object_pointer<A>::value ||
                  std::is_pointer<A>::value,
                  "No valid property accessor provided!");

    detail::type_register::property<ClassType, A>(name, acc, std::vector<rttr::metadata>(),
                                                  detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A acc, std::vector<rttr::metadata> data)
{
    static_assert(std::is_member_object_pointer<A>::value ||
                  std::is_pointer<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property<ClassType, A>(name, acc, std::move(data), 
                                                  detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A, typename Policy>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A acc, const Policy& policy, 
                                               typename std::enable_if<detail::contains<Policy, detail::policy_list>::value>::type*)
{
    static_assert(std::is_member_object_pointer<A>::value ||
                  std::is_pointer<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property<ClassType, A>(name, acc, std::vector<rttr::metadata>(), 
                                                  policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A, typename Policy>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A acc, std::vector<rttr::metadata> data,
                                               const Policy& policy)
{
    static_assert(std::is_member_object_pointer<A>::value ||
                  std::is_pointer<A>::value,
                  "No valid property accessor provided!");
    detail::type_register::property<ClassType, A>(name, acc, std::move(data),
                                                  policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A1, typename A2>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A1 getter, A2 setter,
                                               typename std::enable_if<!detail::contains<A2, detail::policy_list>::value>::type*)
{
    detail::type_register::property<ClassType, A1, A2>(name, getter, setter, std::vector<rttr::metadata>(),
                                                       detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A1, typename A2>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A1 getter, A2 setter, std::vector<rttr::metadata> data)
{
    detail::type_register::property<ClassType, A1, A2>(name, getter, setter, std::move(data),
                                                       detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A1, typename A2, typename Policy>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A1 getter, A2 setter, const Policy& policy)
{
    detail::type_register::property<ClassType, A1, A2>(name, getter, setter, std::vector<rttr::metadata>(), 
                                                       policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A1, typename A2, typename Policy>
class_<ClassType>& class_<ClassType>::property(const std::string& name, A1 getter, A2 setter, 
                                               std::vector<rttr::metadata> data, const Policy& policy)
{
    detail::type_register::property<ClassType, A1, A2>(name, getter, setter, std::move(data),
                                                       policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A>
class_<ClassType>& class_<ClassType>::property_readonly(const std::string& name, A acc)
{
    detail::type_register::property_readonly<ClassType, A>(name, acc, std::vector<rttr::metadata>(),
                                                           detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A>
class_<ClassType>& class_<ClassType>::property_readonly(const std::string& name, A acc, std::vector<rttr::metadata> data)
{
    detail::type_register::property_readonly<ClassType, A>(name, acc, std::move(data),
                                                           detail::default_property_policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A, typename Policy>
class_<ClassType>& class_<ClassType>::property_readonly(const std::string& name, A acc, const Policy& policy)
{
    detail::type_register::property_readonly<ClassType, A>(name, acc, std::vector<rttr::metadata>(),
                                                           policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename A, typename Policy>
class_<ClassType>& class_<ClassType>::property_readonly(const std::string& name, A acc, std::vector<rttr::metadata> data,
                                                        const Policy& policy)
{
    detail::type_register::property_readonly<ClassType, A>(name, acc, std::move(data),
                                                           policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename F>
class_<ClassType>& class_<ClassType>::method(const std::string& name, F function)
{
    detail::type_register::method<ClassType>(name, function, std::vector< rttr::metadata >(), detail::default_invoke());
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename F>
class_<ClassType>& class_<ClassType>::method(const std::string& name, F function, std::vector< rttr::metadata > data)
{
    detail::type_register::method<ClassType>(name, function, std::move(data), detail::default_invoke());
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename F, typename Policy>
class_<ClassType>& class_<ClassType>::method(const std::string& name, F function, const Policy& policy)
{
    detail::type_register::method<ClassType>(name, function, std::vector< rttr::metadata >(), policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename F, typename Policy>
class_<ClassType>& class_<ClassType>::method(const std::string& name, F function, std::vector< rttr::metadata > data, const Policy& policy)
{
    detail::type_register::method<ClassType>(name, function, std::move(data), policy);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
template<typename EnumType>
class_<ClassType>& class_<ClassType>::enumeration(std::string name, std::vector< std::pair< std::string, EnumType> > enum_data, std::vector<rttr::metadata> data)
{
    detail::type_register::enumeration<ClassType, EnumType>(std::move(name), std::move(enum_data), std::move(data));
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// register_global

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

#endif // RTTR_REGISTER_REFLECTION_IMPL_H_
