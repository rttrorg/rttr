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

#ifndef RTTR_ASSOCIATIVE_MAPPER_IMPL_H_
#define RTTR_ASSOCIATIVE_MAPPER_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/iterator_wrapper.h"

#include "rttr/variant.h"
#include <type_traits>

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value, void>
associative_container_base_clear(void* container)
{
    reinterpret_cast<T*>(container)->clear();
}

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value, void>
associative_container_base_clear(void* container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value, std::size_t>
associative_container_base_erase(void* container, argument& key)
{
    using key_t = typename T::key_type;
    if (key.get_type() == ::rttr::type::get<key_t>())
    {
        return reinterpret_cast<T*>(container)->erase(key.get_value<key_t>());
    }
    else
    {
        return 0;
    }
}

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value, std::size_t>
associative_container_base_erase(void* container, argument& key)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
using is_key_value_container = std::integral_constant<bool, !std::is_same<typename T::key_type, typename T::value_type>::value>;

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && is_key_value_container<T>::value,
                                      std::pair<typename T::iterator, bool>>
associative_container_base_insert_key(void* container, argument& key)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value && is_key_value_container<T>::value,
                                     std::pair<typename T::const_iterator, bool>>
associative_container_base_insert_key(void* container, argument& key)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp = decltype(remove_const_t<T>().insert(std::declval<typename T::value_type>()))>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && !is_key_value_container<T>::value && std::is_same<Tp, typename T::iterator>::value,
                                     std::pair<typename T::iterator, bool>>
associative_container_base_insert_key(void* container, argument& key)
{
    using key_type_t = typename T::key_type;
    if (key.get_type() == ::rttr::type::get<key_type_t>())
    {
        return std::make_pair(reinterpret_cast<T*>(container)->insert(key.get_value<key_type_t>()), true);
    }
    else
    {
        return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp = decltype(remove_const_t<T>().insert(std::declval<typename T::value_type>()))>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && !is_key_value_container<T>::value && !std::is_same<Tp, typename T::iterator>::value,
                                     std::pair<typename T::iterator, bool>>
associative_container_base_insert_key(void* container, argument& key)
{
    using key_type_t = typename T::key_type;
    if (key.get_type() == ::rttr::type::get<key_type_t>())
    {
        return reinterpret_cast<T*>(container)->insert(key.get_value<key_type_t>());
    }
    else
    {
        return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value && !is_key_value_container<T>::value,
                                     std::pair<typename T::const_iterator, bool>>
associative_container_base_insert_key(void* container, argument& key)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Tp = decltype(remove_const_t<T>().insert(std::declval<typename T::value_type>()))>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && is_key_value_container<T>::value && std::is_same<Tp, typename T::iterator>::value,
                                      std::pair<typename T::iterator, bool>>
associative_container_base_insert_key_value(void* container, argument& key, argument& value)
{
    using key_t = typename T::key_type;
    using value_t = typename T::mapped_type;
    if (key.get_type() == ::rttr::type::get<key_t>() &&
        value.get_type() == ::rttr::type::get<value_t>())
    {
        return std::make_pair(reinterpret_cast<T*>(container)->insert(std::make_pair(key.get_value<key_t>(), value.get_value<value_t>())), true);
    }
    else
    {
        return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
    }
}

template<typename T, typename Tp = decltype(remove_const_t<T>().insert(std::declval<typename T::value_type>()))>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && is_key_value_container<T>::value && !std::is_same<Tp, typename T::iterator>::value,
                                      std::pair<typename T::iterator, bool>>
associative_container_base_insert_key_value(void* container, argument& key, argument& value)
{
    using key_t = typename T::key_type;
    using value_t = typename T::mapped_type;
    if (key.get_type() == ::rttr::type::get<key_t>() &&
        value.get_type() == ::rttr::type::get<value_t>())
    {
        return reinterpret_cast<T*>(container)->insert(std::make_pair(key.get_value<key_t>(), value.get_value<value_t>()));
    }
    else
    {
        return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value && is_key_value_container<T>::value,
                                     std::pair<typename T::const_iterator, bool>>
associative_container_base_insert_key_value(void* container, argument& key, argument& value)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<!std::is_const<T>::value && !is_key_value_container<T>::value,
                                      std::pair<typename T::iterator, bool>>
associative_container_base_insert_key_value(void* container, argument& key, argument& value)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE static enable_if_t<std::is_const<T>::value && !is_key_value_container<T>::value,
                                     std::pair<typename T::const_iterator, bool>>
associative_container_base_insert_key_value(void* container, argument& key, argument& value)
{
    return std::make_pair(reinterpret_cast<T*>(container)->end(), false);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename ConstType, typename Tp = conditional_t<std::is_const<ConstType>::value,
                                                                     typename T::const_iterator,
                                                                     typename T::iterator>>
struct associative_container_base : iterator_wrapper_associative_container<Tp>
{
    using key_t = typename T::key_type;
    using value_t = typename associative_container_value_t<T>::type;

    static void begin(void* container, iterator_data& itr)
    {
        associative_container_mapper<T, ConstType>::create(itr, reinterpret_cast<ConstType*>(container)->begin());
    }

    static void end(void* container, iterator_data& itr)
    {
        associative_container_mapper<T, ConstType>::create(itr, reinterpret_cast<ConstType*>(container)->end());
    }

    static std::size_t get_size(void* container)
    {
        return (reinterpret_cast<ConstType*>(container)->size());
    }

    static void find(void* container, iterator_data& itr, argument& key)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
            associative_container_mapper<T, ConstType>::create(itr, reinterpret_cast<ConstType*>(container)->find(key.get_value<key_t>()));
        else
            end(container, itr);
    }

    static void equal_range(void* container, argument& key,
                            iterator_data& itr_begin, iterator_data& itr_end)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
        {
            auto ret = reinterpret_cast<ConstType*>(container)->equal_range(key.get_value<key_t>());
            associative_container_mapper<T, ConstType>::create(itr_begin, ret.first);
            associative_container_mapper<T, ConstType>::create(itr_end, ret.second);
        }
        else
        {
            end(container, itr_begin);
            end(container, itr_end);
        }
    }

    static std::size_t erase(void* container, argument& key)
    {
        return associative_container_base_erase<ConstType>(container, key);
    }

    static void clear(void* container)
    {
        associative_container_base_clear<ConstType>(container);
    }

    static bool insert_key(void* container, argument& key, iterator_data& itr)
    {
        auto ret = associative_container_base_insert_key<ConstType>(container, key);
        associative_container_mapper<T, ConstType>::create(itr, ret.first);
        return ret.second;
    }

    static bool insert_key_value(void* container, argument& key, argument& value, iterator_data& itr)
    {
        auto ret = associative_container_base_insert_key_value<ConstType>(container, key, value);
        associative_container_mapper<T, ConstType>::create(itr, ret.first);
        return ret.second;
    }
};

//////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename...Args>
struct associative_container_key_base : associative_container_base<T, Args...>
{
    static variant get_key(const iterator_data& itr)
    {
        auto& it = associative_container_base<T,  Args...>::get_iterator(itr);
        return variant(std::ref(*it));
    }

    static variant get_value(const iterator_data& itr)
    {
        return variant();
    }
};

} // end namespace detail

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::set<K>, Args...> : detail::associative_container_key_base<std::set<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::multiset<K>, Args...> : detail::associative_container_key_base<std::multiset<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::map<K, T>, Args...> : detail::associative_container_base<std::map<K, T>, Args...> { };

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::multimap<K, T>, Args...> : detail::associative_container_base<std::multimap<K, T>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::unordered_set<K>, Args...> : detail::associative_container_key_base<std::unordered_set<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::unordered_map<K, T>, Args...> : detail::associative_container_base<std::unordered_map<K, T>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::unordered_multiset<K>, Args...> : detail::associative_container_key_base<std::unordered_multiset<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::unordered_multimap<K, T>, Args...> : detail::associative_container_base<std::unordered_multimap<K, T>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

struct associative_container_empty
{
    static void create(iterator_data& itr_tgt, const iterator_data& src)
    {
    }

    static void advance(iterator_data& itr, std::ptrdiff_t idx)
    {
    }

    static void destroy(iterator_data& itr)
    {
    }

    static bool equal(const iterator_data& lhs_itr, const iterator_data& rhs_itr) RTTR_NOEXCEPT
    {
        return true;
    }

    static variant get_key(const iterator_data& itr)
    {
        return variant();
    }

    static variant get_value(const iterator_data& itr)
    {
        return variant();
    }

    static void begin(void* container, iterator_data& itr)
    {

    }

    static std::size_t get_size(void* container)
    {
        return 0;
    }

    static void find(void* container, iterator_data& itr, argument& arg)
    {
    }

    static std::size_t erase(void* container, argument& arg)
    {
        return 0;
    }

    static void clear(void* container)
    {
    }

    static void equal_range(void* container, argument& key,
                            iterator_data& itr_begin, iterator_data& itr_end)
    {

    }

    static bool insert_key(void* container, argument& key, iterator_data& itr)
    {
        return false;
    }

    static bool insert_key_value(void* container, argument& key, argument& value, iterator_data& itr)
    {
        return false;
    }
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ASSOCIATIVE_MAPPER_H_
