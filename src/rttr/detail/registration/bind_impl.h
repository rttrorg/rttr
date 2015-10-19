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

#ifndef RTTR_BIND_IMPL_H_
#define RTTR_BIND_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/argument_extractor.h"
#include "rttr/detail/constructor/constructor_wrapper.h"
#include "rttr/detail/destructor/destructor_wrapper.h"
#include "rttr/detail/enumeration/enumeration_wrapper.h"
#include "rttr/detail/enumeration/enum_data.h"
#include "rttr/detail/meta_data/meta_data.h"
#include "rttr/detail/method/method_wrapper.h"
#include "rttr/detail/property/property_wrapper.h"
#include "rttr/detail/type/accessor_type.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/type/type_register.h"
#include "rttr/detail/default_arguments/default_arguments.h"
#include "rttr/policy.h"
#include "rttr/type.h"

#include <functional>
#include <string>
#include <vector>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename... Args>
static RTTR_INLINE std::vector<meta_data> get_meta_data(Args&&... arg)
{
    return forward_to_vector<meta_data>(std::forward<Args>(arg)...);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE void store_meta_data(T& obj, std::vector<meta_data> data)
{
    for (auto& item : data)
    {
        obj->add_meta_data(std::move(item));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Enum_Type, typename... Args>
static RTTR_INLINE auto get_enum_values(Args&&... arg) -> decltype(forward_to_array<enum_data<Enum_Type>>(std::forward<Args>(arg)...))
{
    return forward_to_array<enum_data<Enum_Type>>(std::forward<Args>(arg)...);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename...Ctor_Args>
class registration::bind<detail::ctor, Class_Type, Ctor_Args...> : public registration::class_<Class_Type>
{
    private:
        using default_create_policy = detail::as_raw_pointer;
    
        template<typename Policy, typename...TArgs>
        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base>
        create_constructor_wrapper(detail::default_args<TArgs...> def_args)
        {
            using namespace detail;
            return detail::make_unique<constructor_wrapper<Class_Type, class_ctor, Policy, default_args<TArgs...>, Ctor_Args...>>(std::move(def_args));
        }

        template<typename Policy>
        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base>
        create_constructor_wrapper(detail::default_args<> def_args)
        {
            using namespace detail;
            return detail::make_unique<constructor_wrapper<Class_Type, class_ctor, Policy, default_args<>, Ctor_Args...>>();
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec)
        :   registration::class_<Class_Type>(reg_exec), m_reg_exec(reg_exec)
        {
            m_reg_exec->add_registration_func(this);
        }
        ~bind()
        {
            using namespace detail;
            if (!m_ctor.get())
                m_ctor = detail::make_unique<detail::constructor_wrapper<Class_Type, class_ctor, default_create_policy, detail::default_args<>, Ctor_Args...>>();

            // register the type with the following call:
            m_ctor->get_instanciated_type();
            m_ctor->get_parameter_types();

            auto wrapper = detail::make_rref(std::move(m_ctor));
            auto reg_func = [wrapper]()
            {
                type_register::constructor(type::get<Class_Type>(), std::move(wrapper.m_value));
                type_register::destructor(type::get<Class_Type>(), detail::make_unique<destructor_wrapper<Class_Type>>());
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }

        template<typename... Args>
        registration::class_<Class_Type> operator()(Args&&... arg)
        {
            using namespace detail;

            using policy_types_found = typename find_types<constructor_policy_list, as_type_list_t<raw_type_t<Args>...>>::type;
            static_assert(!has_double_types<policy_types_found>::value, "There are multiple policies of the same type forwarded, that is not allowed!");

            using has_default_types = has_default_types<type_list<Ctor_Args...>, type_list<Args...>>;
            static_assert( (has_default_args<Args...>::value && has_default_types::value) || !has_default_args<Args...>::value, 
                           "The provided default arguments, cannot be used with the given constructor.");
            static_assert( (count_default_args<Args...>::value <= 1), 
                           "Too many default arguments provided, only one set of default arguments can be provided!");

            // when no policy was added, we need a default policy
            using policy_list = conditional_t< type_list_size<policy_types_found>::value == 0,
                                               default_create_policy,
                                               policy_types_found>;

            // at the moment we only supported one policy
            using first_prop_policy = typename std::tuple_element<0, as_std_tuple_t<policy_list>>::type;
            m_ctor = create_constructor_wrapper<first_prop_policy>(std::move(get_default_args<Ctor_Args...>(std::forward<Args>(arg)...)) );
            
            store_meta_data(m_ctor, get_meta_data(std::forward<Args>(arg)...));
            
            return registration::class_<Class_Type>(m_reg_exec);
        }

    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        std::unique_ptr<detail::constructor_wrapper_base> m_ctor;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename F>
class registration::bind<detail::ctor_func, Class_Type, F> : public registration::class_<Class_Type>
{
    private:
        template<typename...TArgs>
        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base>
        create_constructor_wrapper(F func, detail::default_args<TArgs...> def_args)
        {
            using namespace detail;
            return detail::make_unique<constructor_wrapper<Class_Type, return_func, default_invoke, default_args<TArgs...>, F>>(func, std::move(def_args));
        }

        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base>
        create_constructor_wrapper(F func, detail::default_args<> def_args)
        {
            using namespace detail;
            return detail::make_unique<constructor_wrapper<Class_Type, return_func, default_invoke, default_args<>, F>>(func);
        }

        // this indirection is necessary to delay the instantiation of the created constructor wrapper
        // so we can make use of static_assert and check whether the given accessor is correct or not
        // REMARK: this seems to be a MSVC problem...
        template<typename Acc_Func>
        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base> create_default_constructor(Acc_Func func)
        {
            using namespace detail;
            return detail::make_unique<constructor_wrapper<Class_Type, return_func, default_invoke, detail::default_args<>, F>>(func);
        }

        template<typename Acc_Func, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::constructor_wrapper_base> create_custom_constructor(Acc_Func func, Args&&...args)
        {
            using namespace detail;
            using has_default_types = has_default_types<type_list<Acc_Func>, type_list<Args...>>;
            static_assert( (has_default_args<Args...>::value && has_default_types::value) || !has_default_args<Args...>::value, 
                           "The provided default arguments, cannot be used with the given method accessor.");
            static_assert( (count_default_args<Args...>::value <= 1), 
                           "Too many default arguments provided, only one set of default arguments can be provided!");

            auto ctor = create_constructor_wrapper(func, std::move(get_default_args<Acc_Func>(std::forward<Args>(args)...)) );

            store_meta_data(ctor, get_meta_data(std::forward<Args>(args)...));
           
            return std::move(ctor);
        }
    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, F func)
        :   registration::class_<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_func(func)
        {
            m_reg_exec->add_registration_func(this);
        }

        ~bind()
        {
            using namespace detail;
            if (!m_ctor.get())
                m_ctor = create_default_constructor(m_func);

            m_ctor->get_instanciated_type();
            m_ctor->get_parameter_types();
            
            auto wrapper = detail::make_rref(std::move(m_ctor));
            auto reg_func = [wrapper]()
            {
                type_register::constructor(type::get<Class_Type>(), std::move(wrapper.m_value));
            };

            m_reg_exec->add_registration_func(this, reg_func);
        }

        template<typename... Args>
        registration::class_<Class_Type> operator()(Args&&... args)
        {
            m_ctor = create_custom_constructor(m_func, std::forward<Args>(args)...);
            return registration::class_<Class_Type>(m_reg_exec);
        }

    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        F m_func;
        std::unique_ptr<detail::constructor_wrapper_base> m_ctor;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using registration_derived_t = detail::conditional_t< std::is_same<T, void>::value,
                                                      registration,
                                                      registration::class_<T>
                                                     >;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename A>
class registration::bind<detail::prop, Class_Type, A> : public registration_derived_t<Class_Type>
{
    private:
        using default_getter_policy = detail::return_as_copy;
        using default_setter_policy = detail::set_value;

        template<typename Acc>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_default_property(Acc acc)
        {
            using namespace detail;
            using acc_type = typename property_type<Acc>::type;
            return detail::make_unique<property_wrapper<acc_type, A, void, default_getter_policy, default_setter_policy>>(acc);
        }

        template<typename Acc, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_custom_property(Acc acc, Args&&...args)
        {
            using namespace detail;
            using policy_types_found = typename find_types<property_policy_list, as_type_list_t<raw_type_t<Args>...>>::type;
            static_assert(!has_double_types<policy_types_found>::value, "There are multiple policies of the same type forwarded, that is not allowed!");
            // when no policy was added, we need a default policy
            using policy_list = conditional_t< type_list_size<policy_types_found>::value == 0,
                                               default_getter_policy,
                                               policy_types_found>;


            // at the moment we only supported one policy
            using first_prop_policy = typename std::tuple_element<0, as_std_tuple_t<policy_list>>::type;
            using getter_policy     = typename get_getter_policy<first_prop_policy>::type;
            using setter_policy     = typename get_setter_policy<first_prop_policy>::type;
            using acc_type          = typename property_type<Acc>::type;

            auto prop = detail::make_unique<property_wrapper<acc_type, Acc, void, getter_policy, setter_policy>>(acc);
            store_meta_data(prop, get_meta_data(std::forward<Args>(args)...));
            return std::move(prop);
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, const char* name, A acc) 
        :   registration_derived_t<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_name(name), m_acc(acc) 
        {
            m_reg_exec->add_registration_func(this);
        }

        ~bind()
        {
            using namespace detail;
            if (!m_prop.get())
                m_prop = create_default_property(m_acc);
        
            m_prop->set_name(m_name);
            m_prop->set_declaring_type(type::get<Class_Type>());
            // register the underlying type with the following call:
            m_prop->get_type();
        
            auto wrapper = detail::make_rref(std::move(m_prop));
            auto reg_func = [wrapper]()
            {
                type_register::property(rttr::type::get<Class_Type>(), std::move(wrapper.m_value));
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }


        template<typename... Args>
        registration_derived_t<Class_Type> operator()(Args&&... args)
        {
            m_prop = create_custom_property(m_acc, std::forward<Args>(args)...);
            return registration_derived_t<Class_Type>(m_reg_exec);
        }

    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        const char* m_name;
        A           m_acc;
        std::unique_ptr<detail::property_wrapper_base> m_prop;

};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename A1, typename A2>
class registration::bind<detail::prop, Class_Type, A1, A2> : public registration_derived_t<Class_Type>
{
    private:
        using default_getter_policy = detail::return_as_copy;
        using default_setter_policy = detail::set_value;

        template<typename Acc1, typename Acc2>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_default_property(Acc1 getter, Acc2 setter)
        {
            using namespace detail;
            using acc_type = typename property_type<A1>::type;
            return detail::make_unique<property_wrapper<acc_type, Acc1, Acc2, default_getter_policy, default_setter_policy>>(getter, setter);
        }

        template<typename Acc1, typename Acc2, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_custom_property(Acc1 getter, Acc2 setter, Args&&...args)
        {
            using namespace detail;

            using policy_types_found = typename find_types<property_policy_list, as_type_list_t<raw_type_t<Args>...>>::type;
            static_assert(!has_double_types<policy_types_found>::value, "There are multiple policies of the same type forwarded, that is not allowed!");
            // when no policy was added, we need a default policy
            using policy_list = conditional_t< type_list_size<policy_types_found>::value == 0,
                                               default_getter_policy,
                                               policy_types_found>;
            // at the moment we only supported one policy
            using first_prop_policy = typename std::tuple_element<0, as_std_tuple_t<policy_list>>::type;
            using getter_policy     = typename get_getter_policy<first_prop_policy>::type;
            using setter_policy     = typename get_setter_policy<first_prop_policy>::type;
            using acc_type          = typename property_type<A1>::type;
            auto prop = detail::make_unique<property_wrapper<acc_type, Acc1, Acc2, getter_policy, setter_policy>>(getter, setter);

            store_meta_data(prop, get_meta_data(std::forward<Args>(args)...));
            return std::move(prop);
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, const char* name, A1 getter, A2 setter)
        :   registration_derived_t<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_name(name), m_getter(getter), m_setter(setter) 
        {
            m_reg_exec->add_registration_func(this);
        }

        ~bind()
        {
            using namespace detail;
            if (!m_prop.get())
                m_prop = create_default_property(m_getter, m_setter);

            m_prop->set_name(m_name);
            m_prop->set_declaring_type(type::get<Class_Type>());
            // register the underlying type with the following call:
            m_prop->get_type();
            auto wrapper = detail::make_rref(std::move(m_prop));
            auto reg_func = [wrapper]()
            {
                type_register::property(rttr::type::get<Class_Type>(), std::move(wrapper.m_value));
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }
    
        template<typename... Args>
        registration_derived_t<Class_Type> operator()(Args&&... args)
        {
            m_prop = create_custom_property(m_getter, m_setter, std::forward<Args>(args)...);
            return registration_derived_t<Class_Type>(m_reg_exec);
        }

    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        const char* m_name;
        A1          m_getter;
        A2          m_setter;
        std::unique_ptr<detail::property_wrapper_base> m_prop;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename A>
class registration::bind<detail::prop_readonly, Class_Type, A> : public registration_derived_t<Class_Type>
{
    private:
        using default_getter_policy = detail::return_as_copy;
        using default_setter_policy = detail::read_only;

        template<typename Acc>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_default_property(Acc acc)
        {
            using namespace detail;
            using acc_type = typename property_type<Acc>::type;
            return detail::make_unique<property_wrapper<acc_type, A, void, default_getter_policy, default_setter_policy>>(acc);
        }

        template<typename Acc, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::property_wrapper_base> create_custom_property(Acc acc, Args&&...args)
        {
            using namespace detail;
            using policy_types_found = typename find_types<property_policy_list, as_type_list_t<raw_type_t<Args>...>>::type;
            static_assert(!has_double_types<policy_types_found>::value, "There are multiple policies of the same type forwarded, that is not allowed!");
            // when no policy was added, we need a default policy
            using policy_list = conditional_t< type_list_size<policy_types_found>::value == 0,
                                               default_getter_policy,
                                               policy_types_found>;

            // at the moment we only supported one policy
            using first_prop_policy = typename std::tuple_element<0, as_std_tuple_t<policy_list>>::type;
            using getter_policy     = typename get_getter_policy<first_prop_policy>::type;
            using acc_type          = typename property_type<Acc>::type;

            auto prop = detail::make_unique<property_wrapper<acc_type, Acc, void, getter_policy, default_setter_policy>>(acc);

            store_meta_data(prop, get_meta_data(std::forward<Args>(args)...));

            return std::move(prop);
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, const char* name, A acc)
        :   registration_derived_t<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_name(name), m_acc(acc)
        {
            m_reg_exec->add_registration_func(this);
        }

        ~bind()
        {
            using namespace detail;
            if (!m_prop.get())
                m_prop = create_default_property(m_acc);

            m_prop->set_name(m_name);
            m_prop->set_declaring_type(type::get<Class_Type>());
            // register the underlying type with the following call:
            m_prop->get_type();
            auto wrapper = detail::make_rref(std::move(m_prop));
            auto reg_func = [wrapper]()
            {
                type_register::property(rttr::type::get<Class_Type>(), std::move(wrapper.m_value));
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }
    
        template<typename... Args>
        registration_derived_t<Class_Type> operator()(Args&&... args)
        {
            m_prop = create_custom_property(m_acc, std::forward<Args>(args)...);
            return registration_derived_t<Class_Type>(m_reg_exec);
        }
    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        const char* m_name;
        A           m_acc;
        std::unique_ptr<detail::property_wrapper_base> m_prop;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename F>
class registration::bind<detail::meth, Class_Type, F> : public registration_derived_t<Class_Type>
{
    private:
        template<typename Acc_Func>
        static RTTR_INLINE std::unique_ptr<detail::method_wrapper_base> create_default_method(Acc_Func func)
        {
            using namespace detail;
            return detail::make_unique<method_wrapper<Acc_Func, default_invoke, default_args<>>>(func);
        }

        template<typename Acc_Func, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::method_wrapper_base> create_custom_method(Acc_Func func, Args&&...args)
        {
            using namespace detail;

            using policy_types_found = typename find_types<method_policy_list, as_type_list_t<raw_type_t<Args>...>>::type;
            static_assert(!has_double_types<policy_types_found>::value, "There are multiple policies of the same type forwarded, that is not allowed!");

            using has_default_types = has_default_types<type_list<Acc_Func>, type_list<Args...>>;
            static_assert( (has_default_args<Args...>::value && has_default_types::value) ||  !has_default_args<Args...>::value, 
                            "The provided default arguments, cannot be used with the given method accessor.");
            static_assert((count_default_args<Args...>::value <= 1), 
                          "Too many default arguments provided, only one set of default arguments can be provided!");
            
            // when no policy was added, we need a default policy
            using policy_list = conditional_t< type_list_size<policy_types_found>::value == 0,
                                               default_invoke,
                                               policy_types_found>;
            using policy = typename std::tuple_element<0, as_std_tuple_t<policy_list>>::type;

            auto meth = create_method_wrapper<policy>(func, std::move(get_default_args<Acc_Func>(std::forward<Args>(args)...)) );

            store_meta_data(meth, get_meta_data(std::forward<Args>(args)...));
            return std::move(meth);
        }

        template<typename Policy, typename...TArgs>
        static RTTR_INLINE std::unique_ptr<detail::method_wrapper_base>
        create_method_wrapper(F func, detail::default_args<TArgs...> def_args)
        {
            return detail::make_unique<detail::method_wrapper<F, Policy, detail::default_args<TArgs...>>>(func, std::move(def_args));
        }

        template<typename Policy>
        static RTTR_INLINE std::unique_ptr<detail::method_wrapper_base>
        create_method_wrapper(F func, detail::default_args<> def_args)
        {
            return detail::make_unique<detail::method_wrapper<F, Policy, detail::default_args<>>>(func);
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, const char* name, F f)
        :   registration_derived_t<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_name(name), m_func(f) 
        {
            m_reg_exec->add_registration_func(this);
        }

        ~bind()
        {
            using namespace detail;
            if (!m_meth.get())
                m_meth = create_default_method(m_func);

            m_meth->set_name(m_name);
            m_meth->set_declaring_type(type::get<Class_Type>());
            // register the underlying type with the following call:
            m_meth->get_return_type();
            m_meth->get_parameter_types();

            auto wrapper = detail::make_rref(std::move(m_meth));
            auto reg_func = [wrapper]()
            {
                type_register::method(type::get<Class_Type>(), std::move(wrapper.m_value));
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }

    public:
        template<typename... Args>
        registration_derived_t<Class_Type> operator()(Args&&... args)
        {
            m_meth = create_custom_method(m_func, std::forward<Args>(args)...);
            return registration_derived_t<Class_Type>(m_reg_exec);
        }

    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        const char* m_name;
        F           m_func;
        std::unique_ptr<detail::method_wrapper_base> m_meth;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Class_Type, typename Enum_Type>
class registration::bind<detail::enum_, Class_Type, Enum_Type> : public registration_derived_t<Class_Type>
{
    private:
        template<typename E_Type>
        static RTTR_INLINE std::unique_ptr<detail::enumeration_wrapper_base> create_default_enum()
        {
            using namespace detail;
            return detail::make_unique<enumeration_wrapper<E_Type, 0>>(get_enum_values<E_Type>());
        }

        template<typename E_Type, typename... Args>
        static RTTR_INLINE std::unique_ptr<detail::enumeration_wrapper_base> create_custom_enum(Args&&...args)
        {
            using namespace detail;
            static const std::size_t enum_count = count_type<enum_data<Enum_Type>, as_type_list_t<raw_type_t<Args>...>>::value;
            auto enum_wrapper = detail::make_unique<enumeration_wrapper<E_Type, enum_count>>(get_enum_values<E_Type>(std::forward<Args>(args)...));

            store_meta_data(enum_wrapper, get_meta_data(std::forward<Args>(args)...));
            return std::move(enum_wrapper);
        }

    public:
        bind(const std::shared_ptr<detail::registration_executer>& reg_exec, const char* name)
        :   registration_derived_t<Class_Type>(reg_exec), m_reg_exec(reg_exec), m_declared_type(detail::get_invalid_type())
        {
            using namespace detail;

            m_reg_exec->add_registration_func(this);
            type_register::custom_name(type::get<Enum_Type>(), name);

            if (!std::is_same<Class_Type, void>::value)
                m_declared_type = type::get<Class_Type>();
        }

        ~bind()
        {
            using namespace detail;
            if (!m_enum.get())
                m_enum = create_default_enum<Enum_Type>();

            m_enum->set_declaring_type(m_declared_type);
            // register the underlying type with the following call:
            m_enum->get_type();

            auto wrapper = detail::make_rref(std::move(m_enum));
            auto reg_func = [wrapper]()
            {
                type_register::enumeration(type::get<Enum_Type>(), std::move(wrapper.m_value));
            };
            m_reg_exec->add_registration_func(this, std::move(reg_func));
        }
    
        template<typename... Args>
        registration_derived_t<Class_Type> operator()(Args&&... arg)
        {
            m_enum = create_custom_enum<Enum_Type>(std::forward<Args>(arg)...);
            return registration_derived_t<Class_Type>(m_reg_exec);
        }
    private:
        std::shared_ptr<detail::registration_executer> m_reg_exec;
        type m_declared_type;
        std::unique_ptr<detail::enumeration_wrapper_base> m_enum;
};

} // end namespace rttr


#endif // RTTR_BIND_REFLECTION_IMPL_H_
