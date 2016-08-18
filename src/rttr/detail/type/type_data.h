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

#ifndef RTTR_TYPE_DATA_H_
#define RTTR_TYPE_DATA_H_

#include <type_traits>
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/function_traits.h"
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


namespace rttr
{
namespace detail
{
struct type_data;

template<typename T>
RTTR_INLINE type_data& get_type_data() RTTR_NOEXCEPT;
RTTR_INLINE type_data& get_invalid_type_data() RTTR_NOEXCEPT;

using rttr_cast_func        = void*(*)(void*);
using get_derived_info_func = derived_info(*)(void*);


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

struct class_data
{
    class_data(get_derived_info_func func) : m_derived_info_func(func),
                                             m_dtor(create_invalid_item<destructor>())
    {}

    get_derived_info_func       m_derived_info_func;
    std::vector<type>           m_base_types;
    std::vector<type>           m_derived_types;
    std::vector<rttr_cast_func> m_conversion_list;
    std::vector<property>       m_properties;
    std::vector<method>         m_methods;
    std::vector<constructor>    m_ctors;
    destructor                  m_dtor;
};

/////////////////////////////////////////////////////////////////////////////////////////
// workaround for a c++ bug, function ptr declaration for noexcept is not supported
// fixed in c++17 (only clang honor this)
namespace impl
{

using create_variant_func = decltype(&create_invalid_variant_policy::create_variant);
using get_base_types_func = decltype(&base_classes<int>::get_types);

} // end namespace impl

/////////////////////////////////////////////////////////////////////////////////////////

struct type_data
{
    type_data* raw_type_data;
    type_data* wrapped_type;
    type_data* array_raw_type;

    std::string name;
    string_view type_name;

    std::size_t get_sizeof;
    std::size_t get_pointer_dimension;

    bool is_class;
    bool is_enum;
    bool is_array;
    bool is_pointer;
    bool is_arithmetic;
    bool is_function_pointer;
    bool is_member_object_pointer;
    bool is_member_function_pointer;

    impl::create_variant_func create_variant;
    impl::get_base_types_func get_base_types;


    class_data& (*get_class_data)();

    uint16_t type_index;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static class_data& get_type_class_data() RTTR_NOEXCEPT
{
    static std::unique_ptr<class_data> info = make_unique<class_data>(get_most_derived_info_func<T>());
    return (*info.get());
}

/////////////////////////////////////////////////////////////////////////////////////////

static class_data& get_invalid_type_class_data() RTTR_NOEXCEPT
{
    static std::unique_ptr<class_data> info = detail::make_unique<class_data>(nullptr);
    return (*info.get());
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Enable = void>
struct get_size_of
{
    RTTR_INLINE static std::size_t value() RTTR_CONSTEXPR
    {
        return sizeof(T);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct get_size_of<T, enable_if_t<std::is_same<T, void>::value || std::is_function<T>::value>>
{
    RTTR_INLINE static std::size_t value() RTTR_CONSTEXPR
    {
        return 0;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
type_data& get_type_data() RTTR_NOEXCEPT
{
    static auto instance = type_data{ &get_type_data<raw_type_t<T>>(), wrapper_type_info<T>::get_type().m_type_data,
                                      &get_type_data<raw_array_type_t<T>>(),

                                      ::rttr::detail::get_type_name<T>(), ::rttr::detail::get_type_name<T>(),

                                      get_size_of<T>::value(),
                                      pointer_count<T>::value,

                                      std::is_class<T>(),
                                      std::is_enum<T>(),
                                      ::rttr::detail::is_array<T>(),
                                      std::is_pointer<T>(),
                                      std::is_arithmetic<T>(),
                                      is_function_ptr<T>(),
                                      std::is_member_object_pointer<T>(),
                                      std::is_member_function_pointer<T>(),

                                      &create_variant_func<T>::create_variant,
                                      &base_classes<T>::get_types,
                                      &get_type_class_data<T>,
                                      0 };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE type_data& get_invalid_type_data() RTTR_NOEXCEPT
{
    static auto instance = type_data{ &get_invalid_type_data(), &get_invalid_type_data(),
                                      &get_invalid_type_data(),
                                      std::string(""), string_view(),
                                      0, 0,

                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,

                                      &create_invalid_variant_policy::create_variant,
                                      &base_classes<void>::get_types,
                                      &get_invalid_type_class_data,
                                      0};
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATA_H_
