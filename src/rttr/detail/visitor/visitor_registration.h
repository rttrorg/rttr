/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_VISITOR_REGISTRATION_H_
#define RTTR_VISITOR_REGISTRATION_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/type_list.h"

#include <type_traits>

namespace rttr
{
namespace detail
{
    /* This is a slightly modified version (>= VS2015 compatible) of the code from Raul Ramos. */

    /*
    MIT License

    Copyright (c) 2018 Raul Ramos
    */
    // basic operations
    template<typename T, typename TL>
    struct push_back;

    template<typename T, typename...TS>
    struct push_back<T, type_list<TS...>> {
        using type = type_list<TS..., T>;
    };

    template<typename T, typename Enable = void>
    struct is_class_complete_impl : std::false_type {};

    template<typename T>
    struct is_class_complete_impl<T, enable_if_t<std::is_object<T>::value &&
                                                 !std::is_pointer<T>::value &&
                                                 (sizeof(T) > 0)
                                                 >
                                 > : std::true_type {};

    template<typename T>
    using is_class_complete = std::integral_constant<bool, is_class_complete_impl<T>::value>;
} // end namespace detail
} // end namespace rttr

#define DECLARE_TL(_name) DECLARE_TL_IMPL(_name, __COUNTER__)
#define DECLARE_TL_IMPL(_name, _start)\
    /* Declare the type list meta-variable and initialize with an empty list */\
    template<size_t IDX>\
    struct _name##_history;\
    \
    template<> struct _name##_history<_start> {\
        using type = rttr::type_list<>;\
    };\
    \
    /* Check if the entry at "IDX" exists */\
    template <size_t IDX>\
    using _name##_is_defined = rttr::detail::is_class_complete<_name##_history<IDX>>;\
    \
    \
    /* Read from an index IDX */\
    template<size_t IDX, bool = std::is_same<std::true_type, _name##_is_defined<IDX>>::value>\
    struct _name##_read;\
    \
    template<size_t IDX>\
    struct _name##_read<IDX, true> {\
        using type = typename _name##_history<IDX>::type;\
    };\
    \
    template<size_t IDX>\
    struct _name##_read<IDX, false> {\
        using type = typename std::conditional< \
            (IDX > _start),                     /* Should we stop searching? */\
            typename _name##_read<IDX-1>::type, /* No */\
            rttr::type_list<>                     /* Yes => failed => empty type_list */\
        >::type;\
    }

#define READ_TL(_name) typename _name##_read<__COUNTER__ - 1>::type
#define ADD_TL_IMPL(_name, _class, _idx)\
    /* Define the current type_list at index _idx */\
    template<>\
    struct _name##_history<_idx> {\
        using previous = typename _name##_read<_idx - 1>::type;\
        using type = typename rttr::detail::push_back<_class, previous>::type;\
    }
#define ADD_TL(_name, _class) ADD_TL_IMPL(_name, _class, __COUNTER__)

DECLARE_TL(rttr_visitor_list);

#define RTTR_REGISTER_VISITOR(_class) ADD_TL(rttr_visitor_list, _class)

#endif // RTTR_VISITOR_REGISTRATION_H_
