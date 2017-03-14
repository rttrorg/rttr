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

#ifndef RTTR_WRAPPER_MAPPER_IMPL_H_
#define RTTR_WRAPPER_MAPPER_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/std_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include <type_traits>

#include <memory>

namespace rttr
{

class type;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::shared_ptr<T>>
{
    using wrapped_type = decltype(std::shared_ptr<T>().get());
    using type = std::shared_ptr<T>;

    static RTTR_INLINE wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    static RTTR_INLINE type create(const wrapped_type& t)
    {
        return type(t);
    }
};

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::reference_wrapper<T>>
{
    using wrapped_type  = decltype(std::declval<std::reference_wrapper<T>>().get());
    using type          = std::reference_wrapper<T>;

    static RTTR_INLINE wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    static RTTR_INLINE type create(const wrapped_type& t)
    {
        return type(t);
    }
};

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::unique_ptr<T>>
{
    using wrapped_type  = decltype(std::declval<std::unique_ptr<T>>().get());
    using type          = std::unique_ptr<T>;

    static RTTR_INLINE wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    static RTTR_INLINE type create(const wrapped_type& t)
    {
        return type(t);
    }
};

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::weak_ptr<T>>
{
    using wrapped_type  = decltype(std::declval<std::weak_ptr<T>>().lock().get());
    using type          = std::weak_ptr<T>;

    static RTTR_INLINE wrapped_type get(const type& obj)
    {
        return obj.lock().get();
    }

    static RTTR_INLINE type create(const wrapped_type& t)
    {
        return type(); // empty type, weak pointer can only be created by a referencing shared_ptr
    }
};

namespace detail
{

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using wrapper_mapper_t = typename wrapper_mapper<typename remove_cv<typename std::remove_reference<T>::type
                                                                   >::type>::wrapped_type;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using is_wrapper = std::integral_constant<bool, !std::is_same<invalid_wrapper_type, wrapper_mapper_t<T>>::value >;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using wrapper_address_return_type_t = conditional_t<is_wrapper<T>::value,
                                                    raw_addressof_return_type_t< wrapper_mapper_t< T > >,
                                                    raw_addressof_return_type_t<T>>;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<is_wrapper<T>::value, raw_addressof_return_type_t< wrapper_mapper_t<T>> >::type wrapped_raw_addressof(T& obj)
{
    using raw_wrapper_type = remove_cv_t<remove_reference_t<T>>;
    wrapper_mapper_t<T> value = wrapper_mapper<raw_wrapper_type>::get(obj);
    return raw_addressof(value);
}

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!is_wrapper<T>::value, raw_addressof_return_type_t<T>>::type wrapped_raw_addressof(T& obj)
{
    return raw_addressof(obj);
}

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_WRAPPER_MAPPER_IMPL_H_
