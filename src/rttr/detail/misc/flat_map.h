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

#ifndef RTTR_FLAT_MAP_H_
#define RTTR_FLAT_MAP_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/misc/std_type_traits.h"

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

namespace rttr
{
namespace detail
{

template<typename T>
struct equal_types
{
    RTTR_CONSTEXPR bool operator()(const T& left, const T& right) const
    {
        return (left == right);
    }
};

template<>
struct equal_types<const char*>
{
    RTTR_CONSTEXPR bool operator()(const char *const left, const char *const right) const
    {
        return (std::strcmp(left, right) == 0);
    }
};


template<typename Key, typename Hash_Type = std::size_t>
struct key_data
{
    Key         m_key;
    Hash_Type   m_hash_value;

    struct order
    {
        RTTR_INLINE bool operator () (const key_data& left, const key_data& right)  const
        {
            return (left.m_hash_value < right.m_hash_value);
        }
        RTTR_INLINE bool operator () (const Hash_Type& left, const key_data& right) const
        {
            return (left < right.m_hash_value);
        }
        RTTR_INLINE bool operator () (const key_data& left, const Hash_Type& right) const
        {
            return (left.m_hash_value < right);
        }
    };
};

/*!
 * \brief The flat_map class implements a simple map based on std::vector instead of a binary tree.
 *
 */
template<typename Key, typename Value, class Hash = std::size_t, typename Compare = equal_types<Key>>
class flat_map
{
    public:
        using key_data_type = key_data<Key>;
        using value_type = std::pair<const Key, Value>;
        using iterator = typename std::vector<Value>::iterator;
        using const_iterator = typename std::vector<Value>::const_iterator;
        using const_iterator_key = typename std::vector<key_data_type>::const_iterator;
        using hash_type = std::size_t;


        flat_map() {}

    private:


    public:
        iterator end()
        {
            return m_value_list.end();
        }

        const_iterator end() const
        {
            return m_value_list.end();
        }

        const_iterator cend() const
        {
            return m_value_list.cend();
        }

        bool insert(value_type&& value)
        {
            return insert(std::move(value.first), std::move(value.second));
        }

        bool insert(const Key&& key, Value&& value)
        {
            if (find(key) == cend())
            {
                m_key_list.push_back(key_data_type{std::move(key), Hash()(key)});
                std::stable_sort(m_key_list.begin(), m_key_list.end(), typename key_data_type::order());

                auto itr_key = find_key(key);
                if (itr_key != m_key_list.cend())
                {
                    const auto index = std::distance(m_key_list.cbegin(), itr_key);
                    m_value_list.insert(m_value_list.begin() + index, value);
                }

                m_key_list.shrink_to_fit();
                m_value_list.shrink_to_fit();
                return true;
            }

            return false;
        }

        iterator find(const Key& key)
        {
            const auto itr = find_key(key);
            if (itr != m_key_list.end())
                return (m_value_list.begin() + std::distance(m_key_list.cbegin(), itr));
            else
                return (m_value_list.end());
        }

        const_iterator find(const Key& key) const
        {
            const auto itr = find_key(key);
            if (itr != m_key_list.end())
                return (m_value_list.cbegin() + std::distance(m_key_list.cbegin(), itr));
            else
                return (m_value_list.cend());
        }

        std::vector<Value>& value_data()
        {
            return m_value_list;
        }

    private:
        const_iterator_key find_key(const Key& key) const
        {
            const auto hash_value = Hash()(key);
            auto itr = std::lower_bound(m_key_list.cbegin(), m_key_list.cend(),
                                        hash_value,
                                        typename key_data_type::order());
            for (; itr != m_key_list.cend(); ++itr)
            {
                auto& item = *itr;
                if (item.m_hash_value != hash_value)
                    break;

                if (Compare()(item.m_key, key))
                    return itr;
            }
            return m_key_list.end();
        }

    private:
        std::vector<key_data_type>  m_key_list;
        std::vector<Value>          m_value_list;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_FLAT_MAP_H_
