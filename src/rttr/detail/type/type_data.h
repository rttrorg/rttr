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

bool is_enum() RTTR_NOEXCEPT;

using is_enum_func = decltype(&is_enum);

struct type_data_funcs
{
    is_enum_func is_enum;
    class_data& (*get_class_data)();
};


/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct type_data
{
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

    static bool is_enum() RTTR_NOEXCEPT
    {
        return std::is_enum<T>();
    }

    static class_data& get_class_data() RTTR_NOEXCEPT
    {
        static std::unique_ptr<class_data> info = make_unique<class_data>();;
        return (*info.get());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

struct invalid_type_data
{
    static bool is_enum() RTTR_NOEXCEPT
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
    static const auto instance = type_data_funcs{ &type_data<T>::is_enum, &type_data<T>::get_class_data };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_INLINE const type_data_funcs& get_invalid_type_data() RTTR_NOEXCEPT
{
    static const auto instance = type_data_funcs{ &invalid_type_data::is_enum, &invalid_type_data::get_class_data };
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATA_H_
