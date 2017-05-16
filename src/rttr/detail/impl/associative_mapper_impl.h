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
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename ConstType, typename Tp = conditional_t<std::is_const<ConstType>::value,
                                                                     typename T::const_iterator,
                                                                     typename T::iterator>>
struct associative_container_wrapper : iterator_wrapper_associative_container<Tp>
{
    using key_t = typename T::key_type;
    using value_t = typename associative_container_value_t<T>::type;
    using base_class = associative_container_mapper<T, ConstType>;
    using container_t = ConstType;

    static ConstType& get_container(void* container)
    {
        return *reinterpret_cast<ConstType*>(container);
    }

    static void begin_impl(void* container, iterator_data& itr)
    {
        base_class::create(itr, base_class::begin(get_container(container)));
    }

    static void end_impl(void* container, iterator_data& itr)
    {
        base_class::create(itr, base_class::end(get_container(container)));
    }

    static std::size_t get_size_impl(void* container)
    {
        return base_class::get_size(get_container(container));
    }

    static void find_impl(void* container, iterator_data& itr, argument& key)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
            base_class::create(itr, base_class::find(get_container(container), key.get_value<key_t>()));
        else
            end_impl(container, itr);
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename T1>
    static enable_if_t<!std::is_const<T1>::value, void> clear_impl_2(void* container)
    {
        base_class::clear(get_container(container));
    }

    template<typename T1>
    static enable_if_t<std::is_const<T1>::value, void> clear_impl_2(void* container)
    {
    }

    static void clear_impl(void* container)
    {
        clear_impl_2<ConstType>(container);
    }

    /////////////////////////////////////////////////////////////////////////

    static void equal_range_impl(void* container, argument& key,
                                 iterator_data& itr_begin, iterator_data& itr_end)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
        {
            auto ret = base_class::equal_range(get_container(container), key.get_value<key_t>());
            base_class::create(itr_begin, ret.first);
            base_class::create(itr_end, ret.second);
        }
        else
        {
            end_impl(container, itr_begin);
            end_impl(container, itr_end);
        }
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename T1>
    static enable_if_t<!std::is_const<T1>::value, std::size_t> erase_impl_2(void* container, argument& key)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
        {
            return base_class::erase(get_container(container), key.get_value<key_t>());
        }
        else
        {
            return 0;
        }
    }

    template<typename T1>
    static enable_if_t<std::is_const<T1>::value, std::size_t> erase_impl_2(void* container, argument& key)
    {
        return 0;
    }

    static std::size_t erase_impl(void* container, argument& key)
    {
        return erase_impl_2<ConstType>(container, key);
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename T1>
    static enable_if_t<!std::is_const<T1>::value, bool> insert_key_impl_2(void* container, argument& key, iterator_data& itr)
    {
        if (key.get_type() == ::rttr::type::get<key_t>())
        {
            auto ret = base_class::insert_key(get_container(container), key.get_value<key_t>());
            base_class::create(itr, ret.first);
            return ret.second;
        }
        else
        {
            end_impl(container, itr);
            return false;
        }
    }

    template<typename T1>
    static enable_if_t<std::is_const<T1>::value, bool> insert_key_impl_2(void* container, argument& key, iterator_data& itr)
    {
        end_impl(container, itr);
        return false;
    }

    static bool insert_key_impl(void* container, argument& key, iterator_data& itr)
    {
        return insert_key_impl_2<ConstType>(container, key, itr);
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename T1>
    static enable_if_t<!std::is_const<T1>::value, bool> insert_key_value_impl_2(void* container, argument& key, argument& value, iterator_data& itr)
    {
        if (key.get_type() == ::rttr::type::get<key_t>() &&
            value.get_type() == ::rttr::type::get<value_t>())
        {
            auto ret = base_class::insert_key_value(get_container(container), key.get_value<key_t>(), value.get_value<value_t>());
            base_class::create(itr, ret.first);
            return ret.second;
        }
        else
        {
            end_impl(container, itr);
            return false;
        }
    }

    template<typename T1>
    static enable_if_t<std::is_const<T1>::value, bool> insert_key_value_impl_2(void* container, argument& key, argument& value, iterator_data& itr)
    {
        end_impl(container, itr);
        return false;
    }

    static bool insert_key_value_impl(void* container, argument& key, argument& value, iterator_data& itr)
    {
        return insert_key_value_impl_2<ConstType>(container, key, value, itr);
    }

};

//////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename ConstType>
struct associative_container_base : associative_container_wrapper<T, ConstType>
{
    using key_t = typename T::key_type;
    using value_t = typename associative_container_value_t<T>::type;
    using base_class = associative_container_mapper<T, ConstType>;
    using itr_t = typename associative_container_wrapper<T, ConstType>::iterator;

    static itr_t begin(ConstType& container)
    {
        return container.begin();
    }

    static itr_t end(ConstType& container)
    {
        return container.end();
    }

    static std::size_t get_size(ConstType& container)
    {
        return container.size();
    }

    static itr_t find(ConstType& container, key_t& key)
    {
        return container.find(key);
    }

    static void clear(ConstType& container)
    {
        container.clear();
    }

    static std::pair<itr_t, itr_t> equal_range(ConstType& container, key_t& key)
    {
        return container.equal_range(key);
    }

    static std::size_t erase(ConstType& container, key_t& key)
    {
        return container.erase(key);
    }

    static std::pair<itr_t, bool> insert_key(ConstType& container, key_t& key)
    {
        return {container.end(), false};
    }

    static std::pair<itr_t, bool> insert_key_value(ConstType& container, key_t& key, value_t& value)
    {
        return container.insert(std::make_pair(key, value));
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

    using key_t = typename T::key_type;
    using itr_t = typename associative_container_wrapper<T, Args...>::iterator;
    using container_t = typename associative_container_wrapper<T, Args...>::container_t;

    static std::pair<itr_t, bool> insert_key(container_t& container, key_t& key)
    {
        return container.insert(key);
    }

    static std::pair<itr_t, bool> insert_key_value(container_t& container, key_t& key, value_t& value)
    {
        return {container.end(), false};
    }
};

template<typename T, typename...Args>
struct associative_container_base_multi : associative_container_base<T, Args...>
{
    using key_t = typename T::key_type;
    using itr_t = typename associative_container_wrapper<T, Args...>::iterator;
    using container_t = typename associative_container_wrapper<T, Args...>::container_t;

    static std::pair<itr_t, bool> insert_key_value(container_t& container, key_t& key, value_t& value)
    {
        return {container.insert(std::make_pair(key, value)), true};
    }
};

template<typename T, typename...Args>
struct associative_container_key_base_multi : associative_container_key_base<T, Args...>
{
    using key_t = typename T::key_type;
    using itr_t = typename associative_container_wrapper<T, Args...>::iterator;
    using container_t = typename associative_container_wrapper<T, Args...>::container_t;

    static std::pair<itr_t, bool> insert_key(container_t& container, key_t& key)
    {
        return {container.insert(key), true};
    }
};

} // end namespace detail

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::set<K>, Args...> : detail::associative_container_key_base<std::set<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::multiset<K>, Args...> : detail::associative_container_key_base_multi<std::multiset<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::map<K, T>, Args...> : detail::associative_container_base<std::map<K, T>, Args...> { };

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::multimap<K, T>, Args...> : detail::associative_container_base_multi<std::multimap<K, T>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::unordered_set<K>, Args...> : detail::associative_container_key_base<std::unordered_set<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::unordered_map<K, T>, Args...> : detail::associative_container_base<std::unordered_map<K, T>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename...Args>
struct associative_container_mapper<std::unordered_multiset<K>, Args...> : detail::associative_container_key_base_multi<std::unordered_multiset<K>,  Args...> {};

//////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, typename...Args>
struct associative_container_mapper<std::unordered_multimap<K, T>, Args...> : detail::associative_container_base_multi<std::unordered_multimap<K, T>,  Args...> {};

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
