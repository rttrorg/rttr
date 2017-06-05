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

#ifndef RTTR_SEQUENTIAL_MAPPER_IMPL_H_
#define RTTR_SEQUENTIAL_MAPPER_IMPL_H_

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
                                                                     typename sequential_container_mapper<T>::const_itr_t,
                                                                     typename sequential_container_mapper<T>::itr_t>>
struct sequential_container_mapper_wrapper : iterator_wrapper_base<Tp>
{
    using base_class    = sequential_container_mapper<T>;
    using value_t       = typename base_class::value_t;
    using itr_t         = typename base_class::itr_t;
    using const_itr_t   = typename base_class::const_itr_t;
    using itr_wrapper   = iterator_wrapper_base<Tp>;

    static ConstType& get_container(void* container)
    {
        return *reinterpret_cast<ConstType*>(container);
    }

    template<typename..., typename ReturnType = decltype(base_class::get_data(std::declval<itr_t>())), enable_if_t<std::is_reference<ReturnType>::value, int> = 0>
    static variant get_data(const iterator_data& itr)
    {
        auto& it = itr_wrapper::get_iterator(itr);
        return variant(std::ref(base_class::get_data(it)));
    }

    template<typename..., typename ReturnType = decltype(base_class::get_data(std::declval<itr_t>())), enable_if_t<!std::is_reference<ReturnType>::value, int> = 0>
    static variant get_data(const iterator_data& itr)
    {
        auto& it = itr_wrapper::get_iterator(itr);
        return variant(base_class::get_data(it));
    }

    static void begin(void* container, iterator_data& itr)
    {
        itr_wrapper::create(itr, base_class::begin(get_container(container)));
    }

    static void end(void* container, iterator_data& itr)
    {
        itr_wrapper::create(itr, base_class::end(get_container(container)));
    }

    static bool is_empty(void* container)
    {
        return base_class::is_empty(get_container(container));
    }

    static std::size_t get_size(void* container)
    {
        return base_class::get_size(get_container(container));
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename..., typename C = ConstType, enable_if_t<!std::is_const<C>::value, int> = 0>
    static void clear(void* container)
    {
        base_class::clear(get_container(container));
    }

    template<typename..., typename C = ConstType, enable_if_t<std::is_const<C>::value, int> = 0>
    static void clear(void* container)
    {
        // cannot clear a const container...
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename..., typename C = ConstType, enable_if_t<!std::is_const<C>::value, int> = 0>
    static void erase(void* container, const iterator_data& itr_pos, iterator_data& itr)
    {
        const auto ret = base_class::erase(get_container(container), itr_wrapper::get_iterator(itr_pos));
        itr_wrapper::create(itr, ret);
    }

    template<typename..., typename C = ConstType, enable_if_t<std::is_const<C>::value, int> = 0>
    static void erase(void* container, const iterator_data& itr_pos, iterator_data& itr)
    {
        itr_wrapper::create(itr, base_class::end(get_container(container)));
        return;
    }

    /////////////////////////////////////////////////////////////////////////

    template<typename..., typename V = value_t, enable_if_t<std::is_void<V>::value && !std::is_const<ConstType>::value, int> = 0>
    static bool insert(void* container, argument& value, const iterator_data& itr_pos, iterator_data& itr)
    {
        if (value.get_type() == ::rttr::type::get<value_t>())
        {
            auto ret = base_class::insert(get_container(container), value.get_value<value_t>(), itr_wrapper::get_iterator(itr_pos));
            itr_wrapper::create(itr, ret.first);
            return ret.second;
        }
        else
        {
            end(container, itr);
            return false;
        }
    }

    template<typename..., typename V = value_t, enable_if_t<!std::is_void<V>::value || std::is_const<ConstType>::value, int> = 0>
    static bool insert(void* container, argument& value, const iterator_data& itr_pos, iterator_data& itr)
    {
        end(container, itr);
        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct sequential_container_base
{
    using container_t   = T;
    using value_t       = typename T::value_type;
    using itr_t         = typename T::iterator;
    using const_itr_t   = typename T::const_iterator;

    static value_t& get_data(const itr_t& itr)
    {
        return *itr;
    }

    static const value_t& get_data(const const_itr_t& itr)
    {
        return *itr;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t begin(container_t& container)
    {
        return container.begin();
    }

    static const_itr_t begin(const container_t& container)
    {
        return container.begin();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t end(container_t& container)
    {
        return container.end();
    }

    static const_itr_t end(const container_t& container)
    {
        return container.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static void clear(container_t& container)
    {
        container.clear();
    }

    static bool is_empty(const container_t& container)
    {
        return container.empty();
    }

    static std::size_t get_size(const container_t& container)
    {
        return container.size();
    }

    static itr_t erase(container_t& container, const itr_t& itr)
    {
        return container.erase(itr);
    }

    static itr_t erase(container_t& container, const const_itr_t& itr)
    {
        return container.erase(itr);
    }

    static itr_t insert(container_t& container, const value_t& value, const itr_t& itr_pos)
    {
        return container.insert(itr_pos, value);
    }

    static itr_t insert(container_t& container, const value_t& value, const const_itr_t& itr_pos)
    {
        return container.insert(itr_pos, value);
    }
};



} // end namespace detail

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct sequential_container_mapper<std::vector<T>> : detail::sequential_container_base<std::vector<T>> {};

//////////////////////////////////////////////////////////////////////////////////////

template<>
struct sequential_container_mapper<std::vector<bool>>
{
    using container_t   = std::vector<bool>;
    using value_t       = typename std::vector<bool>::value_type;
    using itr_t         = typename std::vector<bool>::iterator;
    using const_itr_t   = typename std::vector<bool>::const_iterator;

    static value_t get_data(const itr_t& itr)
    {
        return *itr;
    }

    static value_t get_data(const const_itr_t& itr)
    {
        return *itr;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t begin(container_t& container)
    {
        return container.begin();
    }

    static const_itr_t begin(const container_t& container)
    {
        return container.begin();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t end(container_t& container)
    {
        return container.end();
    }

    static const_itr_t end(const container_t& container)
    {
        return container.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static void clear(container_t& container)
    {
        container.clear();
    }

    static bool is_empty(const container_t& container)
    {
        return container.empty();
    }

    static std::size_t get_size(const container_t& container)
    {
        return container.size();
    }

    static itr_t erase(container_t& container, const itr_t& itr)
    {
        return container.erase(itr);
    }

    static itr_t erase(container_t& container, const const_itr_t& itr)
    {
        return container.erase(itr);
    }

    static itr_t insert(container_t& container, const value_t& value, const itr_t& itr_pos)
    {
        return container.insert(itr_pos, value);
    }

    static itr_t insert(container_t& container, const value_t& value, const const_itr_t& itr_pos)
    {
        return container.insert(itr_pos, value);
    }
};

//////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

struct sequential_container_empty
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

    static variant get_data(const iterator_data& itr)
    {
        return variant();
    }

    static void begin(void* container, iterator_data& itr)
    {
    }

    static bool is_empty(void* container)
    {
        return true;
    }

    static std::size_t get_size(void* container)
    {
        return 0;
    }

    static void erase(void* container, const iterator_data& itr_pos, iterator_data& itr)
    {
    }

    static void clear(void* container)
    {
    }

    static bool insert(void* container, argument& value, const iterator_data& itr, iterator_data& pos)
    {
        return false;
    }
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_SEQUENTIAL_MAPPER_IMPL_H_
