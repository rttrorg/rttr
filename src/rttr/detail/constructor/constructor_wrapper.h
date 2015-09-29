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

#ifndef RTTR_CONSTRUCTOR_WRAPPER_H_
#define RTTR_CONSTRUCTOR_WRAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/type/accessor_type.h"
#include "rttr/argument.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/detail/misc/function_traits.h"
#include "rttr/variant.h"
#include "rttr/policy.h"
#include "rttr/detail/method/method_accessor.h"
#include "rttr/detail/constructor/object_creator.h"

#include <vector>
#include <utility>
#include <type_traits>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename Constructor_Type, typename Policy, typename... Args>
class constructor_wrapper;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename Policy, typename... Args>
class constructor_wrapper<ClassType, class_ctor, Policy, Args...> : public constructor_wrapper_base
{
    using instanciated_type = decltype(object_creator<ClassType, Policy>::create());
    public:
        constructor_wrapper() {}
        RTTR_INLINE std::vector<type> get_parameter_types_impl(std::false_type) const { return {}; }
        RTTR_INLINE std::vector<type> get_parameter_types_impl(std::true_type) const { return { type::get<Args>()...}; }
        std::vector<type> get_parameter_types() const { return get_parameter_types_impl(std::integral_constant<bool, sizeof...(Args) != 0>()); }
       
        type get_instanciated_type()    const { return type::get<instanciated_type>(); }
        type get_declaring_type()       const { return type::get<typename raw_type<ClassType>::type>(); }
        
        RTTR_INLINE std::vector<bool> get_is_reference_impl(std::true_type) const { return {std::is_reference<Args>::value...}; }
        RTTR_INLINE std::vector<bool> get_is_reference_impl(std::false_type) const { return {}; }
        
        RTTR_INLINE std::vector<bool> get_is_const_impl(std::true_type) const { return {std::is_const<typename std::remove_reference<Args>::type>::value...}; }
        RTTR_INLINE std::vector<bool> get_is_const_impl(std::false_type) const { return {}; }

        std::vector<bool> get_is_reference() const { return get_is_reference_impl(std::integral_constant<bool, sizeof...(Args) != 0>()); }
        std::vector<bool> get_is_const() const { return get_is_const_impl(std::integral_constant<bool, sizeof...(Args) != 0>()); }

        template<typename... TArgs>
        RTTR_INLINE variant invoke_variadic_extracted(TArgs&... args) const
        {
            if (check_all_true(args. template is_type<Args>()...))
                return variant(object_creator<ClassType, Policy>::create(args. template get_value<Args>()...) );
            else
                return variant();
        }

        template<typename... TArgs>
        RTTR_INLINE variant invoke_impl(std::true_type, TArgs&... args) const
        {
            return invoke_variadic_extracted(args...);
        }

        template<typename... TArgs>
        RTTR_INLINE variant invoke_impl(std::false_type, TArgs&... args) const
        {
            return variant();
        }

        variant invoke() const
        {
            return invoke_impl(std::integral_constant<bool, 0 == sizeof...(Args)>());
        }

        variant invoke(argument& arg1) const
        {
            return invoke_impl(std::integral_constant<bool, 1 == sizeof...(Args)>(), arg1);
        }
        variant invoke(argument& arg1, argument& arg2) const
        {
            return invoke_impl(std::integral_constant<bool, 2 == sizeof...(Args)>(), arg1, arg2);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3) const
        {
            return invoke_impl(std::integral_constant<bool, 3 == sizeof...(Args)>(), arg1, arg2, arg3);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4) const
        {
            return invoke_impl(std::integral_constant<bool, 4 == sizeof...(Args)>(), arg1, arg2, arg3, arg4);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5) const
        {
            return invoke_impl(std::integral_constant<bool, 5 == sizeof...(Args)>(), arg1, arg2, arg3, arg4, arg5);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5, argument& arg6) const
        {
            return invoke_impl(std::integral_constant<bool, 6 == sizeof...(Args)>(), arg1, arg2, arg3, arg4, arg5, arg6);
        }

        template<std::size_t ...I>
        RTTR_INLINE variant invoke_variadic_impl(std::vector<argument>& args, index_sequence<I...>) const
        {
            if (args.size() == sizeof...(I))
                return invoke_variadic_extracted(args[I]...);
            else
                return variant();
        }

        variant invoke_variadic(std::vector<argument>& args) const
        {
            return invoke_variadic_impl(args, make_index_sequence<sizeof...(Args)>());
        }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename Policy, typename F>
class constructor_wrapper<ClassType, return_func, Policy, F> : public constructor_wrapper_base
{
    using instanciated_type = typename function_traits<F>::return_type;

    public:
        constructor_wrapper(F creator_func) : m_creator_func(creator_func) {}

        type get_instanciated_type()            const { return type::get<instanciated_type>();                              }
        type get_declaring_type()               const { return type::get<typename raw_type<ClassType>::type>();     }
        std::vector<bool> get_is_reference()    const { return method_accessor<F, Policy>::get_is_reference();      }
        std::vector<bool> get_is_const()        const { return method_accessor<F, Policy>::get_is_const();          }
        std::vector<type> get_parameter_types() const { return method_accessor<F, Policy>::get_parameter_types();   }

        variant invoke() const
        {
           return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance());
        }
        variant invoke(argument& arg1) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1);
        }
        variant invoke(argument& arg1, argument& arg2) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1, arg2);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1, arg2, arg3);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1, arg2, arg3, arg4);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1, arg2, arg3, arg4, arg5);
        }
        variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5, argument& arg6) const
        {
            return method_accessor<F, Policy>::invoke(m_creator_func, empty_instance(), arg1, arg2, arg3, arg4, arg5, arg6);
        }
        variant invoke_variadic(std::vector<argument>& args) const
        {
            return method_accessor<F, Policy>::invoke_variadic(m_creator_func, empty_instance(), args);
        }
    private:
         F  m_creator_func;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_CONSTRUCTOR_WRAPPER_H_
