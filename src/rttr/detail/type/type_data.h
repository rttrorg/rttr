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
struct type_data_funcs;

template<typename T>
RTTR_INLINE const type_data_funcs& get_type_data() RTTR_NOEXCEPT;
RTTR_INLINE const type_data_funcs& get_invalid_type_data() RTTR_NOEXCEPT;

using rttr_cast_func        = void*(*)(void*);
using get_derived_info_func = derived_info(*)(void*);


/////////////////////////////////////////////////////////////////////////////////////////

struct class_data
{
    class_data() : m_dtor(create_invalid_item<destructor>()) {}

    std::vector<type>           m_base_types;
    std::vector<type>           m_derived_types;
    get_derived_info_func       m_derived_info_func;
    std::vector<rttr_cast_func> m_conversion_list;
    std::vector<property>       m_props;
    std::vector<method>         m_meth;
    std::vector<constructor>    m_ctor;
    destructor                  m_dtor;
};

/////////////////////////////////////////////////////////////////////////////////////////
// workaround for a c++ bug, function ptr declaration for noexcept is not supported
// fixed in c++17 (only clang honor this)
namespace impl
{

const type_data_funcs& get_raw_type() RTTR_NOEXCEPT;
const type_data_funcs& get_wrapped_type() RTTR_NOEXCEPT;
const type_data_funcs& get_array_raw_type() RTTR_NOEXCEPT;
std::string& get_name() RTTR_NOEXCEPT;
string_view get_type_name_impl() RTTR_NOEXCEPT;
std::size_t get_sizeof() RTTR_NOEXCEPT;
std::size_t get_pointer_dimension() RTTR_NOEXCEPT;

bool is_class() RTTR_NOEXCEPT;
bool is_enum() RTTR_NOEXCEPT;
bool is_array() RTTR_NOEXCEPT;
bool is_pointer() RTTR_NOEXCEPT;
bool is_arithmetic() RTTR_NOEXCEPT;
bool is_function_pointer() RTTR_NOEXCEPT;
bool is_member_object_pointer() RTTR_NOEXCEPT;
bool is_member_function_pointer() RTTR_NOEXCEPT;


using get_raw_type_func = decltype(&get_raw_type);
using get_wrapped_type_func = decltype(&get_wrapped_type);
using get_array_raw_type_func = decltype(&get_array_raw_type);
using get_name_func = decltype(&get_name);
using get_type_name_func = decltype(&get_type_name_impl);

using get_sizeof_func = decltype(&get_sizeof);
using get_pointer_dimension_func = decltype(&get_pointer_dimension);

using is_class_func = decltype(&is_class);
using is_enum_func = decltype(&is_enum);
using is_array_func = decltype(&is_array);
using is_pointer_func = decltype(&is_pointer);
using is_arithmetic_func = decltype(&is_arithmetic);
using is_function_pointer_func = decltype(&is_function_pointer);
using is_member_object_pointer_func = decltype(&is_member_object_pointer);
using is_member_function_pointer_func = decltype(&is_member_function_pointer);

} // end namespace impl

/////////////////////////////////////////////////////////////////////////////////////////

struct type_data_funcs
{
    impl::get_raw_type_func get_raw_type;
    impl::get_wrapped_type_func get_wrapped_type;
    impl::get_array_raw_type_func get_array_raw_type;

    impl::get_name_func get_name;
    impl::get_type_name_func get_type_name;

    impl::get_sizeof_func get_sizeof;
    impl::get_pointer_dimension_func get_pointer_dimension;

    impl::is_class_func is_class;
    impl::is_enum_func is_enum;
    impl::is_array_func is_array;
    impl::is_pointer_func is_pointer;
    impl::is_arithmetic_func is_arithmetic;
    impl::is_function_pointer_func is_function_pointer;
    impl::is_member_object_pointer_func is_member_object_pointer;
    impl::is_member_function_pointer_func is_member_function_pointer;

    class_data& (*get_class_data)();
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct type_data
{
    static const type_data_funcs& get_raw_type() RTTR_NOEXCEPT
    {
        return get_type_data<raw_type_t<T>>();
    }

    static const type_data_funcs& get_wrapped_type() RTTR_NOEXCEPT
    {
        return get_type_data<wrapper_mapper_t<T>>();
    }

    static const type_data_funcs& get_array_raw_type() RTTR_NOEXCEPT
    {
        return get_type_data<raw_array_type_t<T>>();
    }

    static std::string& get_name() RTTR_NOEXCEPT
    {
        static std::string name(get_type_name());
        return name;
    }

    static string_view get_type_name() RTTR_NOEXCEPT
    {
        static const string_view name = ::rttr::detail::get_type_name<T>();
        return name;
    }

    ///////////////////////////////////////////////////////////////////////////

    static std::size_t get_sizeof() RTTR_NOEXCEPT
    {
        return sizeof(T);
    }

    static std::size_t get_pointer_dimension() RTTR_NOEXCEPT
    {
        return pointer_count<T>::value;
    }

    ///////////////////////////////////////////////////////////////////////////

    static bool is_class() RTTR_NOEXCEPT
    {
        return std::is_class<T>();
    }

    static bool is_enum() RTTR_NOEXCEPT
    {
        return std::is_enum<T>();
    }

    static bool is_array() RTTR_NOEXCEPT
    {
        return ::rttr::detail::is_array<T>();
    }

    static bool is_pointer() RTTR_NOEXCEPT
    {
        return std::is_pointer<T>();
    }

    static bool is_arithmetic() RTTR_NOEXCEPT
    {
        return std::is_arithmetic<T>();
    }

    static bool is_function_pointer() RTTR_NOEXCEPT
    {
        return is_function_ptr<T>();
    }

    static bool is_member_object_pointer() RTTR_NOEXCEPT
    {
        return std::is_member_object_pointer<T>();
    }

    static bool is_member_function_pointer() RTTR_NOEXCEPT
    {
        return std::is_member_function_pointer<T>();
    }

    ///////////////////////////////////////////////////////////////////////////

    static class_data& get_class_data() RTTR_NOEXCEPT
    {
        static std::unique_ptr<class_data> info = make_unique<class_data>();;
        return (*info.get());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

struct invalid_type_data
{
    static const type_data_funcs& get_raw_type() RTTR_NOEXCEPT
    {
        return get_invalid_type_data();
    }

    static const type_data_funcs& get_wrapped_type() RTTR_NOEXCEPT
    {
        return get_invalid_type_data();
    }

    static const type_data_funcs& get_array_raw_type() RTTR_NOEXCEPT
    {
        return get_invalid_type_data();
    }

    static std::string& get_name() RTTR_NOEXCEPT
    {
        static std::string name;
        return name;
    }

    static string_view get_type_name() RTTR_NOEXCEPT
    {
        static const string_view name = get_name();
        return name;
    }

    ///////////////////////////////////////////////////////////////////////////

    static std::size_t get_sizeof() RTTR_NOEXCEPT
    {
        return 0;
    }

    static std::size_t get_pointer_dimension() RTTR_NOEXCEPT
    {
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////

    static bool is_class() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_enum() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_array() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_pointer() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_arithmetic() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_function_pointer() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_member_object_pointer() RTTR_NOEXCEPT
    {
        return false;
    }

    static bool is_member_function_pointer() RTTR_NOEXCEPT
    {
        return false;
    }

    static class_data& get_class_data() RTTR_NOEXCEPT
    {
        static std::unique_ptr<class_data> info = detail::make_unique<class_data>();;
        return (*info.get());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE const type_data_funcs& get_type_data() RTTR_NOEXCEPT
{
    static const auto instance = type_data_funcs{ &type_data<T>::get_raw_type, &type_data<T>::get_wrapped_type,
                                                  &type_data<T>::get_array_raw_type,
                                                  &type_data<T>::get_name, &type_data<T>::get_type_name,
                                                  &conditional_t<std::is_same<T, void>::value || std::is_function<T>::value,
                                                                 invalid_type_data,
                                                                 type_data<T>>::get_sizeof,
                                                  &type_data<T>::get_pointer_dimension,
                                                  &type_data<T>::is_class,
                                                  &type_data<T>::is_enum,
                                                  &type_data<T>::is_array,
                                                  &type_data<T>::is_pointer,
                                                  &type_data<T>::is_arithmetic,
                                                  &type_data<T>::is_function_pointer,
                                                  &type_data<T>::is_member_object_pointer,
                                                  &type_data<T>::is_member_function_pointer,
                                                  &type_data<T>::get_class_data };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE const type_data_funcs& get_invalid_type_data() RTTR_NOEXCEPT
{
    static const auto instance = type_data_funcs{ &invalid_type_data::get_raw_type, &invalid_type_data::get_wrapped_type,
                                                  &invalid_type_data::get_array_raw_type,
                                                  &invalid_type_data::get_name, &invalid_type_data::get_type_name,
                                                  &invalid_type_data::get_sizeof, &invalid_type_data::get_pointer_dimension,
                                                  &invalid_type_data::is_class,
                                                  &invalid_type_data::is_enum,
                                                  &invalid_type_data::is_array,
                                                  &invalid_type_data::is_pointer,
                                                  &invalid_type_data::is_arithmetic,
                                                  &invalid_type_data::is_function_pointer,
                                                  &invalid_type_data::is_member_object_pointer,
                                                  &invalid_type_data::is_member_function_pointer,
                                                  &invalid_type_data::get_class_data };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATA_H_
