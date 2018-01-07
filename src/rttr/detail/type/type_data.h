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

#ifndef RTTR_TYPE_DATA_H_
#define RTTR_TYPE_DATA_H_

#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/function_traits.h"
#include "rttr/detail/misc/template_type_trait.h"
#include "rttr/detail/type/base_classes.h"
#include "rttr/detail/type/get_derived_info_func.h"
#include "rttr/detail/type/get_create_variant_func.h"
#include "rttr/detail/type/type_register.h"
#include "rttr/detail/type/type_name.h"
#include "rttr/detail/misc/utility.h"
#include "rttr/destructor.h"
#include "rttr/method.h"
#include "rttr/property.h"
#include "rttr/constructor.h"
#include "rttr/destructor.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/property/property_wrapper_base.h"
#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/detail/destructor/destructor_wrapper_base.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/metadata/metadata.h"


#include <type_traits>
#include <bitset>


namespace rttr
{
namespace detail
{
struct type_data;

template<typename T>
RTTR_INLINE type_data& get_type_data() RTTR_NOEXCEPT;
RTTR_INLINE type_data& get_invalid_type_data() RTTR_NOEXCEPT;

static type get_invalid_type() RTTR_NOEXCEPT;

using rttr_cast_func        = void*(*)(void*);
using get_derived_info_func = derived_info(*)(void*);

/////////////////////////////////////////////////////////////////////////////////////////

struct class_data
{
    class_data(get_derived_info_func func, std::vector<type> nested_types)
    :   m_derived_info_func(func),
        m_nested_types(nested_types),
        m_dtor(create_invalid_item<destructor>())
    {}

    get_derived_info_func       m_derived_info_func;
    std::vector<type>           m_base_types;
    std::vector<type>           m_derived_types;
    std::vector<rttr_cast_func> m_conversion_list;
    std::vector<property>       m_properties;
    std::vector<method>         m_methods;
    std::vector<constructor>    m_ctors;
    std::vector<type>           m_nested_types;
    destructor                  m_dtor;
    std::vector<std::unique_ptr<method_wrapper_base>>       m_method_storage;
    std::vector<std::unique_ptr<property_wrapper_base>>     m_property_storage;
    std::vector<std::unique_ptr<constructor_wrapper_base>>  m_constructor_storage;
    std::unique_ptr<destructor_wrapper_base>                m_dtor_storage;
};

enum class type_trait_infos : std::size_t
{
    is_class = 0,
    is_enum,
    is_array,
    is_pointer,
    is_arithmetic,
    is_function_pointer,
    is_member_object_pointer,
    is_member_function_pointer,
    is_associative_container,
    is_sequential_container,
    is_template_instantiation,

    TYPE_TRAIT_COUNT
};

 using type_traits = std::bitset<static_cast<std::size_t>(type_trait_infos::TYPE_TRAIT_COUNT)>;

/////////////////////////////////////////////////////////////////////////////////////////

namespace impl
{

using create_variant_func  = decltype(&create_invalid_variant_policy::create_variant);
using get_base_types_func  = decltype(&base_classes<int>::get_types);
using create_wrapper_func  = void(*)(const argument& arg, variant& var);
using get_enumeration_func = std::unique_ptr<enumeration_wrapper_base>&(*)(void);
using get_metadata_func    = std::unique_ptr<std::vector<metadata>>&(*)(void);

} // end namespace impl

/////////////////////////////////////////////////////////////////////////////////////////

struct type_data
{
    type_data* raw_type_data;
    type_data* wrapped_type;
    type_data* array_raw_type;

    ~type_data()
    {
        type_register::type_unreg(*this);
    }

    std::string name;
    string_view type_name;

    std::size_t get_sizeof;
    std::size_t get_pointer_dimension;

    impl::create_variant_func create_variant;
    impl::get_base_types_func get_base_types; // FIXME: this info should not be stored, its just temporarily,
                                              // thats why we store it as function pointer

    impl::get_enumeration_func get_enumeration;
    impl::get_metadata_func    get_metadata;
    impl::create_wrapper_func create_wrapper;
    class_data& (*get_class_data)();

    uint16_t type_index;
    static const uint16_t m_invalid_id = 0;
    RTTR_FORCE_INLINE bool is_valid() const RTTR_NOEXCEPT { return (type_index != m_invalid_id); }
    RTTR_FORCE_INLINE bool type_trait_value(type_trait_infos type_trait) const RTTR_NOEXCEPT { return m_type_traits.test(static_cast<std::size_t>(type_trait)); }


    type_traits m_type_traits;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static class_data& get_type_class_data() RTTR_NOEXCEPT
{
    static std::unique_ptr<class_data> info = detail::make_unique<class_data>(get_most_derived_info_func<T>(), template_type_trait<T>::get_template_arguments());
    return (*info.get());
}

/////////////////////////////////////////////////////////////////////////////////////////

static class_data& get_invalid_type_class_data() RTTR_NOEXCEPT
{
    static std::unique_ptr<class_data> info = detail::make_unique<class_data>(nullptr, std::vector<type>());
    return (*info.get());
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable = void>
struct get_size_of
{
    RTTR_INLINE RTTR_CONSTEXPR static std::size_t value()
    {
        return sizeof(T);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct get_size_of<T, enable_if_t<std::is_same<T, void>::value || std::is_function<T>::value>>
{
    RTTR_INLINE RTTR_CONSTEXPR static std::size_t value()
    {
        return 0;
    }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = std::is_same<T, typename raw_type<T>::type >::value>
struct raw_type_info
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return get_invalid_type(); } // we have to return an empty type, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct raw_type_info<T, false>
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return type::get<typename raw_type<T>::type>(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = std::is_array<T>::value>
struct array_raw_type
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return type::get<raw_array_type_t<T>>(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct array_raw_type<T, false>
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return get_invalid_type(); } // we have to return an empty type, so we can stop the recursion
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, bool = is_wrapper<T>::value>
struct wrapper_type_info
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return type::get<wrapper_mapper_t<T>>(); }
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_type_info<T, false>
{
    static RTTR_INLINE type get_type() RTTR_NOEXCEPT { return get_invalid_type(); }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename Wrapper, typename Wrapped_Type>
static void create_wrapper(const argument& arg, variant& var)
{
    if (arg.get_type() != type::get<Wrapped_Type>())
        return;

    auto& wrapped_type = arg.get_value<Wrapped_Type>();
    var = wrapper_mapper<Wrapper>::create(wrapped_type);
}

template<typename Wrapper, typename Tp = wrapper_mapper_t<Wrapper>>
static enable_if_t<is_wrapper<Wrapper>::value &&
                   ::rttr::detail::is_copy_constructible<Wrapper>::value &&
                   std::is_default_constructible<Wrapper>::value &&
                   has_create_wrapper_func<Wrapper>::value, impl::create_wrapper_func>
get_create_wrapper_func()
{
    return &create_wrapper<Wrapper, Tp>;
}


template<typename Wrapper, typename Tp = wrapper_mapper_t<Wrapper>>
static enable_if_t<!is_wrapper<Wrapper>::value ||
                   !::rttr::detail::is_copy_constructible<Wrapper>::value ||
                   !std::is_default_constructible<Wrapper>::value ||
                   !has_create_wrapper_func<Wrapper>::value, impl::create_wrapper_func>
get_create_wrapper_func()
{
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static std::unique_ptr<enumeration_wrapper_base>& get_enumeration_func_impl()
{
    static std::unique_ptr<enumeration_wrapper_base> obj = make_unique<enumeration_wrapper_base>();
    return obj;
}

template<typename T>
static enable_if_t<std::is_enum<T>::value, impl::get_enumeration_func>
get_enumeration_func()
{
    return &get_enumeration_func_impl<T>;
}

template<typename T>
static enable_if_t<!std::is_enum<T>::value, impl::get_enumeration_func>
get_enumeration_func()
{
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static std::unique_ptr<std::vector<metadata>>& get_metadata_func_impl()
{
    static std::unique_ptr<std::vector<metadata>> obj = make_unique<std::vector<metadata>>();
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

using type_trait_value = uint64_t;
#define TYPE_TRAIT_TO_BITSET_VALUE(trait) (static_cast<std::uint64_t>(std::trait<T>::value) << static_cast<std::size_t>(type_trait_infos::trait))
#define TYPE_TRAIT_TO_BITSET_VALUE_2(trait, enum_key) (static_cast<std::uint64_t>(trait<T>::value) << static_cast<std::size_t>(type_trait_infos::enum_key))

template<typename T>
type_data& get_type_data() RTTR_NOEXCEPT
{
    static type_data instance { raw_type_info<T>::get_type().m_type_data, wrapper_type_info<T>::get_type().m_type_data,
                                array_raw_type<T>::get_type().m_type_data,

                                ::rttr::detail::get_type_name<T>().to_string(), ::rttr::detail::get_type_name<T>(),

                                get_size_of<T>::value(),
                                pointer_count<T>::value,

                                &create_variant_func<T>::create_variant,
                                &base_classes<T>::get_types,
                                get_enumeration_func<T>(),
                                &get_metadata_func_impl<T>,
                                get_create_wrapper_func<T>(),

                                &get_type_class_data<T>,
                                0,
                                type_trait_value{ TYPE_TRAIT_TO_BITSET_VALUE(is_class) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE(is_enum) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::is_array, is_array) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE(is_pointer) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE(is_arithmetic) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE_2(is_function_ptr, is_function_pointer) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE(is_member_object_pointer) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE(is_member_function_pointer) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::is_associative_container, is_associative_container) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::is_sequential_container, is_sequential_container) |
                                                  TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::template_type_trait, is_template_instantiation)
                                                }
                              };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

static type_data& get_invalid_type_data_impl() RTTR_NOEXCEPT
{
    static type_data instance{ nullptr, nullptr,
                               nullptr,
                               std::string(""), string_view(),
                               0, 0,
                               &create_invalid_variant_policy::create_variant,
                               &base_classes<void>::get_types,
                               nullptr,
                               nullptr,
                               get_create_wrapper_func<void>(),
                               &get_invalid_type_class_data,
                               0,
                               type_trait_value{0}};

    instance.raw_type_data  = &instance;
    instance.wrapped_type   = &instance;
    instance.array_raw_type = &instance;

    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type_data& get_invalid_type_data() RTTR_NOEXCEPT
{
    static auto& instance = get_invalid_type_data_impl();
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATA_H_
