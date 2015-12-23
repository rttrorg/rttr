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

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type()
:   m_id(m_invalid_id)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type(type::type_id id)
:   m_id(id)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type(const type& other)
:   m_id(other.m_id)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type& type::operator=(const type& other)
{
    m_id = other.m_id;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator<(const type& other) const
{
    return (m_id < other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator>(const type& other) const
{
    return (m_id > other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator>=(const type& other) const
{
    return (m_id >= other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator<=(const type& other) const
{
    return (m_id <= other.m_id);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator==(const type& other) const
{
    return (m_id == other.m_id); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::operator!=(const type& other) const
{
    return (m_id != other.m_id); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::type_id type::get_id() const 
{ 
    return m_id;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE bool type::is_valid() const
{
    return (m_id != 0); 
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type::operator bool() const
{
    return (m_id != 0); 
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(begin_skip, end_skip)      \
namespace detail                                                            \
{                                                                           \
    RTTR_STATIC_CONSTEXPR std::size_t skip_size_at_begin = begin_skip;      \
    RTTR_STATIC_CONSTEXPR std::size_t skip_size_at_end   = end_skip;        \
}

#if RTTR_COMPILER == RTTR_COMPILER_MSVC
    // sizeof("const char *__cdecl rttr::detail::f<"), sizeof(">(void)")
    RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(36, 7)
#elif RTTR_COMPILER == RTTR_COMPILER_GNUC
    // sizeof("const char* rttr::detail::f() [with T = "), sizeof("]")
    RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(40, 1)
#elif RTTR_COMPILER == RTTR_COMPILER_CLANG
    // sizeof("const char* rttr::detail::f() [T = "), sizeof("]")
    RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(35, 1)
#else
#   error "This compiler does not supported extracting a function signature via preprocessor!"
#endif

namespace detail
{

RTTR_INLINE static type get_invalid_type() { return type(); }

/////////////////////////////////////////////////////////////////////////////////

template <std::size_t N>
RTTR_INLINE static const char* extract_type_signature(const char (&signature)[N])
{
//    static_assert(N > skip_size_at_begin + skip_size_at_end, "RTTR is misconfigured for your compiler.")
    return &signature[rttr::detail::skip_size_at_begin];
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE static const char* f()
{
    return extract_type_signature(
    #if RTTR_COMPILER == RTTR_COMPILER_MSVC
                                                            __FUNCSIG__
    #elif RTTR_COMPILER == RTTR_COMPILER_GNUC
                                                            __PRETTY_FUNCTION__
    #elif RTTR_COMPILER == RTTR_COMPILER_CLANG
                                                            __PRETTY_FUNCTION__
    #else
        #error "Don't know how the extract type signatur for this compiler! Abort! Abort!" 
    #endif
                                   );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = std::is_same<T, typename raw_type<T>::type >::value>
struct raw_type_info
{
    static RTTR_INLINE type get_type() { return get_invalid_type(); } // we have to return an empty type, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct raw_type_info<T, false>
{
    static RTTR_INLINE type get_type() { return type::get<typename raw_type<T>::type>(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = is_wrapper<T>::value>
struct wrapper_type_info
{
    static RTTR_INLINE type get_type() { return type::get<typename wrapper_mapper<T>::wrapped_type>(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_type_info<T, false>
{
    static RTTR_INLINE type get_type() { return get_invalid_type(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = std::is_same<T, typename raw_array_type<T>::type >::value>
struct array_raw_type
{
    static RTTR_INLINE type get_type() { return get_invalid_type(); } // we have to return an empty type, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct array_raw_type<T, false>
{
    static RTTR_INLINE type get_type() { return type::get<typename raw_array_type<T>::type>(); }
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Enable>
struct type_getter
{
    static type get_type()
    {
        // when you get an error here, then the type was not completely defined 
        // (a forward declaration is not enough because base_classes will not be found)
        using type_must_be_complete = char[ sizeof(T) ? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        static const type val = type_register::type_reg(f<T>(),
                                                        raw_type_info<T>::get_type(),
                                                        wrapper_type_info<T>::get_type(),
                                                        array_raw_type<T>::get_type(),
                                                        std::move(base_classes<T>::get_types()),
                                                        get_most_derived_info_func<T>(),
                                                        &create_variant_func<T>::create_variant,
                                                        sizeof(T),
                                                        std::is_class<T>::value,
                                                        std::is_enum<T>::value,
                                                        is_array<T>::value,
                                                        std::is_pointer<T>::value,
                                                        std::is_arithmetic<T>::value,
                                                        is_function_ptr<T>::value,
                                                        std::is_member_object_pointer<T>::value,
                                                        std::is_member_function_pointer<T>::value,
                                                        pointer_count<T>::value);
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
    static type get_type()
    {
        static const type val = type_register::type_reg(f<void>(),
                                                        raw_type_info<void>::get_type(),
                                                        wrapper_type_info<void>::get_type(),
                                                        array_raw_type<void>::get_type(),
                                                        std::vector<base_class_info>(),
                                                        get_most_derived_info_func<void>(),
                                                        nullptr,
                                                        0,
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        false);
        return val;
    }
};

/////////////////////////////////////////////////////////////////////////////////

/*!
 * Explicit specializations for function types;
 * because we cannot implement the check whether a type is completly defined for functions
 */
template <typename T>
struct type_getter<T, typename std::enable_if<std::is_function<T>::value>::type>
{
    static type get_type()
    {
        static const type val = type_register::type_reg(f<T>(),
                                                        raw_type_info<T>::get_type(),
                                                        wrapper_type_info<T>::get_type(),
                                                        array_raw_type<T>::get_type(),
                                                        std::vector<detail::base_class_info>(),
                                                        get_most_derived_info_func<T>(),
                                                        &create_variant_func<T>::create_variant,
                                                        0,
                                                        std::is_class<T>::value,
                                                        std::is_enum<T>::value,
                                                        is_array<T>::value,
                                                        std::is_pointer<T>::value,
                                                        std::is_arithmetic<T>::value,
                                                        is_function_ptr<T>::value,
                                                        std::is_member_object_pointer<T>::value,
                                                        std::is_member_function_pointer<T>::value,
                                                        pointer_count<T>::value);
        return val;
    }
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE type get_type_from_instance(const T*)
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
    static RTTR_INLINE type get(T&&)
    {
        return detail::type_getter<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

//! Specialization for retrieving the type from the instance directly
template<typename T>
struct type_from_instance<T, true>
{
    static RTTR_INLINE type get(T&& object)
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
RTTR_INLINE type type::get()
{
    return detail::type_getter<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE type type::get(T&& object)
{
    using remove_ref = typename std::remove_reference<T>::type;
    return detail::type_from_instance<T, detail::has_get_type_func<T>::value && !std::is_pointer<remove_ref>::value>::get(std::forward<T>(object));
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool type::is_derived_from() const
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
    static_assert(detail::is_class_template<T>::value, "No template type provided! Please use this function only with template types, e.g. My_Custom_Type<T>.");
    static detail::type_comparator<T> cmp;
    detail::type_register::comparator(type::get<T>(), &cmp);
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

#define RTTR_CAT_IMPL(a,b) a##b
#define RTTR_CAT(a,b) RTTR_CAT_IMPL(a,b)

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
