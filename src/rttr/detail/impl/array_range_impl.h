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

#ifndef RTTR_ARRAY_RANGE_IMPL_H_
#define RTTR_ARRAY_RANGE_IMPL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/property.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE array_range<T, Predicate>::array_range(bounds_type begin, size_type size, const Predicate& pred)
:   m_begin(begin),
    m_end(begin + size),
    m_pred(pred)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE array_range<T, Predicate>::array_range()
:   m_begin(nullptr),
    m_end(nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::iterator array_range<T, Predicate>::begin()
{
    iterator itr(m_begin, this);
    if (m_pred(*itr))
        return itr;

    next(itr);

    return itr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::iterator array_range<T, Predicate>::end()
{
    return {m_end, this};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_iterator array_range<T, Predicate>::begin() const
{
    const_iterator itr(m_begin, this);
    if (m_pred(*itr))
        return itr;

    next(itr);

    return itr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_iterator array_range<T, Predicate>::end() const
{
    return {m_end, this};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_iterator array_range<T, Predicate>::cbegin() const
{
    const_iterator itr(m_begin, this);
    if (m_pred(*itr))
        return itr;

    next(itr);

    return itr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_iterator array_range<T, Predicate>::cend() const
{
    return {m_end, this};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::reverse_iterator array_range<T, Predicate>::rbegin()
{
    if (m_begin == m_end)
    {
        return {nullptr, this};
    }
    else
    {
        reverse_iterator itr(m_end - 1, this);
        if (m_pred(*itr))
            return itr;

        prev(itr);

        return itr;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::reverse_iterator array_range<T, Predicate>::rend()
{
    return (m_begin == m_end ? reverse_iterator{nullptr, this} : reverse_iterator{m_begin - 1, this});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_reverse_iterator array_range<T, Predicate>::rbegin() const
{
    if (m_begin == m_end)
    {
        return {nullptr, this};
    }
    else
    {
        const_reverse_iterator itr(m_end - 1, this);
        if (m_pred(*itr))
            return itr;

        prev(itr);

        return itr;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_reverse_iterator array_range<T, Predicate>::rend() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr, this} : const_reverse_iterator{m_begin - 1, this});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_reverse_iterator array_range<T, Predicate>::crbegin() const
{
    if (m_begin == m_end)
    {
        return {nullptr, this};
    }
    else
    {
        const_reverse_iterator itr(m_end - 1, this);
        if (m_pred(*itr))
            return itr;

        prev(itr);

        return itr;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE typename array_range<T, Predicate>::const_reverse_iterator array_range<T, Predicate>::crend() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr, this} : const_reverse_iterator{m_begin - 1, this});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE size_t array_range<T, Predicate>::size() const
{
    std::size_t result = 0;
    const_iterator itr{m_begin, this};
    while(itr != cend())
    {
        ++itr;
        ++result;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
RTTR_INLINE bool array_range<T, Predicate>::empty() const
{
    return (m_begin == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE void array_range<T, Predicate>::next(array_iterator<DataType>& itr) const
{
    ++itr.m_ptr;
    if (itr.m_ptr != m_end && !m_pred(*itr.m_ptr))
        next(itr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE void array_range<T, Predicate>::prev(array_reverse_iterator<DataType>& itr) const
{
    --itr.m_ptr;

    if (itr.m_ptr != (m_begin - 1) && !m_pred(*itr.m_ptr))
        prev(itr);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE
array_range<T, Predicate>::array_iterator_base<DataType>::array_iterator_base()
:   m_ptr(nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE
array_range<T, Predicate>::array_iterator_base<DataType>::array_iterator_base(typename array_iterator_base<DataType>::pointer ptr,
                                                                              const array_range<T, Predicate>* const range)
:   m_ptr(ptr),
    m_range(range)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_iterator_base<DataType>::reference
array_range<T, Predicate>::array_iterator_base<DataType>::operator*()
{
    return *m_ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_iterator_base<DataType>::pointer
array_range<T, Predicate>::array_iterator_base<DataType>::operator->()
{
    return m_ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE bool array_range<T, Predicate>::array_iterator_base<DataType>::operator==(const self_type& rhs) const
{
    return (m_ptr == rhs.m_ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE bool array_range<T, Predicate>::array_iterator_base<DataType>::operator!=(const self_type& rhs) const
{
    return (m_ptr != rhs.m_ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_iterator_base<DataType>::self_type&
array_range<T, Predicate>::array_iterator_base<DataType>::operator=(const self_type& other)
{
    m_ptr = other.m_ptr;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_iterator<DataType>::array_iterator()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_iterator<DataType>::array_iterator(const array_iterator<DataType>& other)
:   array_iterator_base<DataType>(other.m_ptr, other.m_range)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_iterator<DataType>::array_iterator(typename array_iterator_base<DataType>::pointer ptr,
                                                                                const array_range<T, Predicate>* const range)
:   array_iterator_base<DataType>(ptr, range)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_iterator<DataType>::self_type&
array_range<T, Predicate>::array_iterator<DataType>::operator++()
{
    this->m_range->next(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_iterator<DataType>::self_type
array_range<T, Predicate>::array_iterator<DataType>::operator++(int index)
{
    auto old_itr = *this;
    this->m_range->next(*this);
    return old_itr;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_reverse_iterator<DataType>::array_reverse_iterator()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_reverse_iterator<DataType>::array_reverse_iterator(const array_reverse_iterator<DataType>& other)
:   array_iterator_base<DataType>(other.m_ptr, other.m_range)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE array_range<T, Predicate>::array_reverse_iterator<DataType>::array_reverse_iterator(typename array_iterator_base<DataType>::pointer ptr,
                                                                                                const array_range<T, Predicate>* const range)
:   array_iterator_base<DataType>(ptr, range)
{
}

////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_reverse_iterator<DataType>::self_type&
array_range<T, Predicate>::array_reverse_iterator<DataType>::operator++()
{
    this->m_range->prev(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Predicate>
template<typename DataType>
RTTR_INLINE typename array_range<T, Predicate>::template array_reverse_iterator<DataType>::self_type
array_range<T, Predicate>::array_reverse_iterator<DataType>::operator++(int index)
{
    auto old_itr = *this;
    this->m_range->prev(*this);
    return old_itr;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE array_range<T, detail::no_predicate>::array_range(bounds_type begin, size_type size, const detail::no_predicate&)
:   m_begin(begin),
    m_end(begin + size)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE array_range<T, detail::no_predicate>::array_range()
:   m_begin(nullptr),
    m_end(nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::iterator array_range<T, detail::no_predicate>::begin()
{
    return {m_begin};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::iterator array_range<T, detail::no_predicate>::end()
{
    return {m_end};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_iterator array_range<T, detail::no_predicate>::begin() const
{
    return {m_begin};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_iterator array_range<T, detail::no_predicate>::end() const
{
    return {m_end};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_iterator array_range<T, detail::no_predicate>::cbegin() const
{
    return {m_begin};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_iterator array_range<T, detail::no_predicate>::cend() const
{
    return {m_end};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::reverse_iterator array_range<T, detail::no_predicate>::rbegin()
{
    return (m_begin == m_end ? reverse_iterator{nullptr} : reverse_iterator{m_end - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::reverse_iterator array_range<T, detail::no_predicate>::rend()
{
    return (m_begin == m_end ? reverse_iterator{nullptr} : reverse_iterator{m_begin - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_reverse_iterator array_range<T, detail::no_predicate>::rbegin() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr} : const_reverse_iterator{m_end - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_reverse_iterator array_range<T, detail::no_predicate>::rend() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr} : const_reverse_iterator{m_begin - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_reverse_iterator array_range<T, detail::no_predicate>::crbegin() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr} : const_reverse_iterator{m_end - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE typename array_range<T, detail::no_predicate>::const_reverse_iterator array_range<T, detail::no_predicate>::crend() const
{
    return (m_begin == m_end ? const_reverse_iterator{nullptr} : const_reverse_iterator{m_begin - 1});
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE size_t array_range<T, detail::no_predicate>::size() const
{
    return (m_end - m_begin);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_INLINE bool array_range<T, detail::no_predicate>::empty() const
{
    return (m_begin == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE
array_range<T, detail::no_predicate>::array_iterator_base<DataType>::array_iterator_base()
:   m_ptr(nullptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE
array_range<T, detail::no_predicate>::array_iterator_base<DataType>::array_iterator_base(typename array_iterator_base<DataType>::pointer ptr)
:   m_ptr(ptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_iterator_base<DataType>::reference
array_range<T, detail::no_predicate>::array_iterator_base<DataType>::operator*()
{
    return *m_ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_iterator_base<DataType>::pointer
array_range<T, detail::no_predicate>::array_iterator_base<DataType>::operator->()
{
    return m_ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE bool array_range<T, detail::no_predicate>::array_iterator_base<DataType>::operator==(const self_type& rhs) const
{
    return (m_ptr == rhs.m_ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE bool array_range<T, detail::no_predicate>::array_iterator_base<DataType>::operator!=(const self_type& rhs) const
{
    return (m_ptr != rhs.m_ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_iterator_base<DataType>::self_type&
array_range<T, detail::no_predicate>::array_iterator_base<DataType>::operator=(const self_type& other)
{
    m_ptr = other.m_ptr;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_iterator<DataType>::array_iterator()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_iterator<DataType>::array_iterator(const array_iterator<DataType>& other)
:   array_iterator_base<DataType>(other.m_ptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_iterator<DataType>::array_iterator(typename array_iterator_base<DataType>::pointer ptr)
:   array_iterator_base<DataType>(ptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_iterator<DataType>::self_type&
array_range<T, detail::no_predicate>::array_iterator<DataType>::operator++()
{
    ++this->m_ptr;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_iterator<DataType>::self_type
array_range<T, detail::no_predicate>::array_iterator<DataType>::operator++(int index)
{
    auto old_itr = *this;
    ++this->m_ptr;
    return old_itr;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_reverse_iterator<DataType>::array_reverse_iterator()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_reverse_iterator<DataType>::array_reverse_iterator(const array_reverse_iterator<DataType>& other)
:   array_iterator_base<DataType>(other.m_ptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE array_range<T, detail::no_predicate>::array_reverse_iterator<DataType>::array_reverse_iterator(typename array_iterator_base<DataType>::pointer ptr)
:   array_iterator_base<DataType>(ptr)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_reverse_iterator<DataType>::self_type&
array_range<T, detail::no_predicate>::array_reverse_iterator<DataType>::operator++()
{
    --this->m_ptr;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename DataType>
RTTR_INLINE typename array_range<T, detail::no_predicate>::template array_reverse_iterator<DataType>::self_type
array_range<T, detail::no_predicate>::array_reverse_iterator<DataType>::operator++(int index)
{
    auto old_itr = *this;
    --this->m_ptr;
    return old_itr;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr


#endif // RTTR_ITEM_RANGE_IMPL_H_
