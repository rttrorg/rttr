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

#ifndef RTTR_ASSOCIATIVE_MAPPER_H_
#define RTTR_ASSOCIATIVE_MAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"

namespace rttr
{

/*!
 * The \ref associative_container_mapper class is a class template to access an associative container via one common interface.
 *
 * This class will be only used internally by RTTR via the variant_associative_view class to get access to elements of an associative container.
 * In order to use your own custom associative container type, you have to provide a specialization of this class.
 *
 * Out of the box, RTTR has specialization for following associative container types:
 * - \p `std::set<Key>`
 * - \p `std::map<Key, T>`
 * - \p `std::multiset<Key>`
 * - \p `std::multimap<Key, T>`
 * - \p `std::unordered_set<Key>`
 * - \p `std::unordered_map<Key, T>`
 * - \p `std::unordered_multiset<Key>`
 * - \p `std::unordered_multimap<Key, T>`
 *
 * Custom associative container
 * -----------------------------
 * 1. By default it should be enough to just provide a class specialization of your type for the class associative_container_mapper
 *    and derive from `detail::associative_container_base`.
 *    Because when your type follows the interface of the STL associative containers
 *    <a target="_blank" href=https://en.wikipedia.org/wiki/Associative_containers>associative container</a>
 *    it should be enough, you don't need to map anything. For example:
 *    \code{.cpp}
 *    template<typename K, typename T, typename...Args>
 *    struct associative_container_mapper<my_custom_container<K, T>, Args...> : detail::associative_container_base<my_custom_container<K, T>, Args...>
 *    {
 *    };
 *    \endcode
 *    There is one exception for `set` like types, which store only the key, you should derive from `detail::associative_container_key_base`
 *    \code{.cpp}
 *    template<typename K, typename...Args>
 *    struct associative_container_mapper<my_custom_key_container<K>, Args...> : detail::associative_container_key_base<my_custom_key_container<K>, Args...>
 *    {
 *    };
 *    \endcode
 *
 * 2. When your interface is completly different then the STL interface, you have to implement several member functions:
 *    1.  `static const Itr& get_iterator(const detail::iterator_data& data);`
 *    2.  `static Itr& get_iterator(detail::iterator_data& data);`
 *    3.  `static void create(detail::iterator_data& itr_tgt, const detail::iterator& itr_src);`
 *    4.  `static void create(detail::iterator_data& itr_tgt, const detail::iterator_data& itr_src);`
 *    5.  `static void destroy(detail::iterator_data& itr);`
 *    6.  `static void advance(detail::iterator_data& itr, std::ptrdiff_t idx);`
 *    7.  `static bool equal(const detail::iterator_data& lhs_itr, const detail::iterator_data& rhs_itr) RTTR_NOEXCEPT;`
 *    8.  `static variant get_key(const detail::iterator_data& itr);`
 *    9.  `static variant get_value(const detail::iterator_data& itr);`
 *    10. `static void find(void* container, detail::iterator_data& itr, argument& key);`
 *    11. `static void equal_range(void* container, argument& key, detail::iterator_data& itr_begin, detail::iterator_data& itr_end);`
 *    12. `static std::size_t erase(void* container, argument& key);`
 *    13. `static void clear(void* container);`
 *    14. `static bool insert_key(void* container, argument& key, detail::iterator_data& itr);`
 *    15. `static bool insert_key_value(void* container, argument& key, argument& value, detail::iterator_data& itr);`
 *
 * The iterator itself is stored in the detail::iterator_data. You are free to use placement new (when the iterator has the same size as double)
 * or allocate on the iterator on the heap and store the pointer inside the detail::iterator_data.
 *
 */
template<typename...T>
struct associative_container_mapper
{
#ifndef DOXYGEN
    using is_valid = std::false_type;
#else
    static const Itr& get_iterator(const detail::iterator_data& data)
    {
    }

    static Itr& get_iterator(detail::iterator_data& data)
    {
    }

    static void create(detail::iterator_data& itr_tgt, const detail::iterator& itr_src)
    {
    }

    static void create(detail::iterator_data& itr_tgt, const detail::iterator_data& itr_src)
    {
    }

    static void destroy(detail::iterator_data& itr)
    {
    }

    static void advance(detail::iterator_data& itr, std::ptrdiff_t idx)
    {
    }

    static bool equal(const detail::iterator_data& lhs_itr, const detail::iterator_data& rhs_itr) RTTR_NOEXCEPT
    {
    }

    static variant get_key(const detail::iterator_data& itr)
    {
    }

    static variant get_value(const detail::iterator_data& itr)
    {
    }

    static void find(void* container, detail::iterator_data& itr, argument& key)
    {
    }

    static void equal_range(void* container, argument& key,
                            detail::iterator_data& itr_begin, detail::iterator_data& itr_end)
    {
    }

    static std::size_t erase(void* container, argument& key)
    {
    }

    static void clear(void* container)
    {
    }

    static bool insert_key(void* container, argument& key, detail::iterator_data& itr)
    {
    }

    static bool insert_key_value(void* container, argument& key, argument& value, detail::iterator_data& itr)
    {
    }
#endif
};

} // end namespace rttr

#include "rttr/detail/impl/associative_mapper_impl.h"

#endif // RTTR_ASSOCIATIVE_MAPPER_H_
