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

#ifndef RTTR_TYPE_IMPL_H_
#define RTTR_TYPE_IMPL_H_

#include <type_traits>
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/function_traits.h"
#include "rttr/detail/type/base_classes.h"
#include "rttr/detail/type/get_derived_info_func.h"
#include "rttr/detail/type/get_create_variant_func.h"
#include "rttr/detail/type/type_register.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/wrapper_mapper.h"
#include "rttr/detail/type/type_comparator.h"
#include "rttr/detail/type/type_data.h"
#include "rttr/detail/type/type_name.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type() RTTR_NOEXCEPT
:    m_type_data(&detail::get_invalid_type_data())
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type(detail::type_data* data) RTTR_NOEXCEPT
:  m_type_data(data)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type(const type& other) RTTR_NOEXCEPT
:   m_type_data(other.m_type_data)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type& type::operator=(const type& other) RTTR_NOEXCEPT
{
    m_type_data = other.m_type_data;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator<(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index < other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator>(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index > other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator>=(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index >= other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator<=(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index <= other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator==(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index == other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator!=(const type& other) const RTTR_NOEXCEPT
{
    return (m_type_data->type_index != other.m_type_data->type_index);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type_id type::get_id() const RTTR_NOEXCEPT
{
    return m_type_data->type_index;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::is_valid() const RTTR_NOEXCEPT
{
    return m_type_data->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::operator bool() const RTTR_NOEXCEPT
{
    return m_type_data->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE type type::get_raw_type() const RTTR_NOEXCEPT
{
    return type(m_type_data->raw_type_data);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE type type::get_wrapped_type() const RTTR_NOEXCEPT
{
    return type(m_type_data->wrapped_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE type type::get_raw_array_type() const RTTR_NOEXCEPT
{
    return type(m_type_data->array_raw_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE string_view type::get_name() const RTTR_NOEXCEPT
{
    return m_type_data->name;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE string_view type::get_full_name() const RTTR_NOEXCEPT
{
    return m_type_data->type_name;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE std::size_t type::get_sizeof() const RTTR_NOEXCEPT
{
    return m_type_data->get_sizeof;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE std::size_t type::get_pointer_dimension() const RTTR_NOEXCEPT
{
    return m_type_data->get_pointer_dimension;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_class() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_class);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_enumeration() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_enum);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_array() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_array);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_associative_container() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_associative_container);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_pointer() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_pointer);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_arithmetic() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_arithmetic);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_function_pointer() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_function_pointer);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_member_object_pointer() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_member_object_pointer);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_member_function_pointer() const RTTR_NOEXCEPT
{
    return m_type_data->type_trait_value(detail::type_trait_infos::is_member_function_pointer);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE bool type::is_wrapper() const RTTR_NOEXCEPT
{
    return m_type_data->wrapped_type->is_valid();
}


/////////////////////////////////////////////////////////////////////////////////////////

RTTR_FORCE_INLINE variant type::create_variant(const argument& data) const
{
    return m_type_data->create_variant(data);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

RTTR_INLINE static type get_invalid_type() RTTR_NOEXCEPT { return type(); }

/////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Enable>
struct type_getter
{
    static type get_type() RTTR_NOEXCEPT
    {
        // when you get an error here, then the type was not completely defined
        // (a forward declaration is not enough because base_classes will not be found)
        using type_must_be_complete = char[ sizeof(T) ? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        static const type val = type_register::type_reg( get_type_data<T>() );
        return val;
    }
};

/////////////////////////////////////////////////////////////////////////////////

/*!
 * Explicit specializations for type void;
 * because we cannot implement the check whether a type is completely defined for type `void`
 */
template <>
struct type_getter<void>
{
    static type get_type() RTTR_NOEXCEPT
    {
        static const type val = type_register::type_reg( get_type_data<void>() );
        return val;
    }
};

/////////////////////////////////////////////////////////////////////////////////

/*!
 * Explicit specializations for function types;
 * because we cannot implement the check whether a type is completely defined for functions
 */
template <typename T>
struct type_getter<T, typename std::enable_if<std::is_function<T>::value>::type>
{
    static type get_type() RTTR_NOEXCEPT
    {
        static const type val = type_register::type_reg( get_type_data<T>() );
        return val;
    }
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE type get_type_from_instance(const T*) RTTR_NOEXCEPT
{
    return detail::type_getter<T>::get_type();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool>
struct type_from_instance;

//! Specialization for retrieving the type from the instance directly
template<typename T>
struct type_from_instance<T, false> // the typeInfo function is not available
{
    static RTTR_INLINE type get(T&&) RTTR_NOEXCEPT
    {
        return detail::type_getter<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

//! Specialization for retrieving the type from the instance directly
template<typename T>
struct type_from_instance<T, true>
{
    static RTTR_INLINE type get(T&& object) RTTR_NOEXCEPT
    {
        return object.get_type();
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename TargetType, typename SourceType, typename F>
struct type_converter;

} // end namespace detail

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE type type::get() RTTR_NOEXCEPT
{
    return detail::type_getter<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
RTTR_INLINE type type::get<detail::invalid_type>() RTTR_NOEXCEPT
{
    return detail::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE type type::get(T&& object) RTTR_NOEXCEPT
{
    using remove_ref = typename std::remove_reference<T>::type;
    return detail::type_from_instance<T, detail::has_get_type_func<T>::value && !std::is_pointer<remove_ref>::value>::get(std::forward<T>(object));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool type::is_derived_from() const RTTR_NOEXCEPT
{
    return is_derived_from(type::get<T>());
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename F>
RTTR_INLINE void type::register_converter_func(F func)
{
    using namespace detail;

    using target_type_orig = typename function_traits<F>::return_type;
    using target_type = remove_cv_t<remove_reference_t<target_type_orig>>;

    const std::size_t arg_count = function_traits<F>::arg_count;

    static_assert(arg_count == 2, "Invalid argument count! The converter function signature must be: <target_type(source_type, bool&)>");
    static_assert(!std::is_same<void, target_type>::value, "Return type cannot be void!");
    static_assert(std::is_same<bool&, typename param_types<F, 1>::type>::value, "Second argument type must be a bool reference(bool&).");

    using source_type_orig = param_types_t<F, 0>;
    using source_type = remove_cv_t<remove_reference_t<source_type_orig>>;

    auto converter = detail::make_unique<type_converter<target_type, source_type, F>>(func);
    type source_t = type::get<source_type>();
    type_register::converter(source_t, std::move(converter));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type::register_comparators()
{
    register_equal_comparator<T>();
    register_less_than_comparator<T>();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type::register_equal_comparator()
{
    static detail::type_equal_comparator<T> cmp;
    detail::type_register::equal_comparator(type::get<T>(), &cmp);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void type::register_less_than_comparator()
{
    static detail::type_less_than_comparator<T> cmp;
    detail::type_register::less_than_comparator(type::get<T>(), &cmp);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr


namespace std
{
    template <>
    class hash<rttr::type>
    {
    public:
        size_t operator()(const rttr::type& info) const
        {
            return hash<rttr::type::type_id>()(info.get_id());
        }
    };
} // end namespace std

#define RTTR_CAT_IMPL(a, b) a##b
#define RTTR_CAT(a, b) RTTR_CAT_IMPL(a, b)

static void rttr_auto_register_reflection_function_();
namespace rttr
{
namespace detail
{
template<typename Ctor_Type, typename Policy, typename Accessor, typename Arg_Indexer>
struct constructor_invoker;
}
}

#define RTTR_REGISTRATION_FRIEND friend void ::rttr_auto_register_reflection_function_();                               \
                                 template<typename Ctor_Type, typename Policy, typename Accessor, typename Arg_Indexer> \
                                 friend struct rttr::detail::constructor_invoker;


#define RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(T) rttr::type::get<T>();       \
                                                    rttr::type::get<T*>();      \
                                                    rttr::type::get<const T*>();
#endif // RTTR_TYPE_IMPL_H_
