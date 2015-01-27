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

#ifndef __RTTR_TYPE_IMPL_H__
#define __RTTR_TYPE_IMPL_H__

#include <type_traits>
#include "rttr/detail/misc_type_traits.h"
#include "rttr/detail/function_traits.h"

namespace rttr
{

namespace detail
{
    template<typename TargetType, typename SourceType, typename F>
    struct type_converter;
}

RTTR_INLINE type::type()
:   m_id(0)
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

namespace detail
{
class constructor_container_base;
class destructor_container_base;
class enumeration_container_base;
class method_container_base;
class property_container_base;
}

namespace impl
{

RTTR_API void register_property(type, std::unique_ptr<detail::property_container_base>);
RTTR_API void register_method(type, std::unique_ptr<detail::method_container_base>);
RTTR_API void register_constructor(type, std::unique_ptr<detail::constructor_container_base>);
RTTR_API void register_destructor(type, std::unique_ptr<detail::destructor_container_base>);
RTTR_API void register_enumeration(type t, std::unique_ptr<detail::enumeration_container_base>);

static type get_invalid_type() { return type(); }

template <typename T>
struct MetaTypeInfo
{
    enum { Defined = 0 };
    static type get_type()
    {
        // when you get this error, you have to declare first the type with this macro
        static_assert(sizeof(T) != sizeof(T), "The given type T is not registered to the type system; please register with RTTR_DECLARE_TYPE.");
        return get_invalid_type();
    }
};


template<typename T>
struct auto_register_type;
/////////////////////////////////////////////////////////////////////////////////

template<typename T>
static RTTR_INLINE type get_type_from_instance(const T*)
{
    return impl::MetaTypeInfo<T>::get_type();
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = std::is_same<T, typename detail::raw_type<T>::type >::value>
struct raw_type_info
{
    static RTTR_INLINE type get_type() { return get_invalid_type(); } // we have to return an empty type, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct raw_type_info<T, false>
{
    static RTTR_INLINE type get_type() { return MetaTypeInfo<typename detail::raw_type<T>::type>::get_type(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool>
struct type_from_instance;

//! Specialization for retrieving the type from the instance directly
template<typename T>
struct type_from_instance<T, false> // the typeInfo function is not available
{
    static RTTR_INLINE type get(T&&)
    {
        return impl::MetaTypeInfo<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
    }
};

//! Specialization for retrieving the type from the instance directly
template<typename T>
struct type_from_instance<T, true>
{
    static RTTR_INLINE type get(T&& object)
    {
        return object.get_type();
    }
};

} // end namespace impl

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE type type::get()
{
    return impl::MetaTypeInfo<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
RTTR_INLINE type type::get(T&& object)
{
    using remove_ref = typename std::remove_reference<T>::type;
    return impl::type_from_instance<T, detail::has_get_type_func<T>::value && !std::is_pointer<remove_ref>::value>::get(std::forward<T>(object));
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
    using target_type_orig = typename detail::function_traits<F>::return_type;
    using target_type = typename std::remove_cv<typename std::remove_reference<target_type_orig>::type>::type;
    
    const std::size_t arg_count = detail::function_traits<F>::arg_count;
    
    static_assert(arg_count == 2, "Invalid argument count! The converter function signature must be: <target_type(source_type, bool&)>");
    static_assert(!std::is_same<void, target_type>::value, "Return type cannot be void!");
    static_assert(std::is_same<bool&, typename detail::param_types<F, 1>::type>::value, "Second argument type must be a bool reference(bool&).");
    
    using source_type_orig = typename detail::param_types<F, 0>::type;
    using source_type = typename std::remove_cv<typename std::remove_reference<source_type_orig>::type>::type;

    std::unique_ptr<detail::type_converter_base> converter(new detail::type_converter<target_type, source_type, F>(func));
    type source_t = type::get<source_type>();
    source_t.register_type_converter(std::move(converter));
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

static void _rttr_auto_register_reflection_function();
#define RTTR_REGISTER_FRIEND friend void _rttr_auto_register_reflection_function();

#define RTTR_CAT_IMPL(a,b) a##b
#define RTTR_CAT(a,b) RTTR_CAT_IMPL(a,b)

#define RTTR_DECLARE_TYPE(...)                                                                                          \
namespace rttr                                                                                                          \
{                                                                                                                       \
    namespace impl                                                                                                      \
    {                                                                                                                   \
        template<>                                                                                                      \
        struct MetaTypeInfo< __VA_ARGS__ >                                                                              \
        {                                                                                                               \
            enum { Defined = 1 };                                                                                       \
            static RTTR_INLINE rttr::type get_type()                                                                    \
            {                                                                                                           \
                static const type val = rttr::type::register_type(#__VA_ARGS__,                                         \
                                                                  raw_type_info<__VA_ARGS__>::get_type(),               \
                                                                  std::move(base_classes<__VA_ARGS__>::get_types()),    \
                                                                  get_most_derived_info_func<__VA_ARGS__>(),            \
                                                                  get_create_variant_func<__VA_ARGS__>(),               \
                                                                  std::is_class<__VA_ARGS__>::value,                    \
                                                                  std::is_enum<__VA_ARGS__>::value,                     \
                                                                  ::rttr::detail::is_array<__VA_ARGS__>::value,         \
                                                                  std::is_pointer<__VA_ARGS__>::value,                  \
                                                                  std::is_arithmetic<__VA_ARGS__>::value);              \
                return val;                                                                                             \
            }                                                                                                           \
        };                                                                                                              \
    }                                                                                                                   \
} // end namespace rttr

#define RTTR_DEFINE_TYPE(...)                                                                                           \
namespace rttr                                                                                                          \
{                                                                                                                       \
    namespace impl                                                                                                      \
    {                                                                                                                   \
        template<>                                                                                                      \
        struct auto_register_type<__VA_ARGS__>                                                                          \
        {                                                                                                               \
            auto_register_type()                                                                                        \
            {                                                                                                           \
                MetaTypeInfo<__VA_ARGS__>::get_type();                                                                  \
            }                                                                                                           \
        };                                                                                                              \
    }                                                                                                                   \
}                                                                                                                       \
static const rttr::impl::auto_register_type<__VA_ARGS__> RTTR_CAT(autoRegisterType,__COUNTER__);

#define RTTR_DECLARE_STANDARD_TYPE_VARIANTS(T) RTTR_DECLARE_TYPE(T)                                                     \
                                               RTTR_DECLARE_TYPE(T*)                                                    \
                                               RTTR_DECLARE_TYPE(const T*)

#define RTTR_DEFINE_STANDARD_TYPE_VARIANTS(T) RTTR_DEFINE_TYPE(T)                                                       \
                                              RTTR_DEFINE_TYPE(T*)                                                      \
                                              RTTR_DEFINE_TYPE(const T*)


#endif // __RTTR_TYPE_IMPL_H__
