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

#ifndef __RTTR_CONSTRUCTOR_CONTAINER_H__
#define __RTTR_CONSTRUCTOR_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/constructor_container_base.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/utility.h"
#include "rttr/variant.h"

#include <vector>
#include <utility>
#include <type_traits>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename ClassType, typename... Args>
class constructor_container : public constructor_container_base
{
    public:
        constructor_container() : constructor_container_base(type::get<typename raw_type<ClassType>::type>()) {}
        RTTR_INLINE std::vector<type> get_parameter_types_impl(std::false_type) const { return {}; }
        RTTR_INLINE std::vector<type> get_parameter_types_impl(std::true_type) const { return { type::get<Args>()...}; }
        std::vector<type> get_parameter_types() const { return get_parameter_types_impl(std::integral_constant<bool, sizeof...(Args) != 0>()); }
       
        type get_instanciated_type() const  { return type::get<ClassType*>(); }
        
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
                return (new ClassType(args. template get_value<Args>()...));
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

        variant invoke(detail::argument& arg1) const
        {
            return invoke_impl(std::integral_constant<bool, 1 == sizeof...(Args)>(), arg1);
        }
        variant invoke(detail::argument& arg1, detail::argument& arg2) const
        {
            return invoke_impl(std::integral_constant<bool, 2 == sizeof...(Args)>(), arg1, arg2);
        }
        variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3) const
        {
            return invoke_impl(std::integral_constant<bool, 3 == sizeof...(Args)>(), arg1, arg2, arg3);
        }
        variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4) const
        {
            return invoke_impl(std::integral_constant<bool, 4 == sizeof...(Args)>(), arg1, arg2, arg3, arg4);
        }
        variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4, detail::argument& arg5) const
        {
            return invoke_impl(std::integral_constant<bool, 5 == sizeof...(Args)>(), arg1, arg2, arg3, arg4, arg5);
        }
        variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4, detail::argument& arg5, detail::argument& arg6) const
        {
            return invoke_impl(std::integral_constant<bool, 6 == sizeof...(Args)>(), arg1, arg2, arg3, arg4, arg5, arg6);
        }

        template<std::size_t ...I>
        RTTR_INLINE variant invoke_variadic_impl(std::vector<detail::argument>& args, index_sequence<I...>) const
        {
            if (args.size() == sizeof...(I))
                return invoke_variadic_extracted(args[I]...);
            else
                return variant();
        }

        variant invoke_variadic(std::vector<detail::argument>& args) const
        {
            return invoke_variadic_impl(args, make_index_sequence<sizeof...(Args)>());
        }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_CONSTRUCTOR_CONTAINER_H__
