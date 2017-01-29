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

#ifndef RTTR_VARIANT_ASSOCIATIVE_VIEW_P_H_
#define RTTR_VARIANT_ASSOCIATIVE_VIEW_P_H_

#include "rttr/variant.h"
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/associative_mapper.h"

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

class RTTR_LOCAL variant_associative_view_private
{
    public:
        variant_associative_view_private() RTTR_NOEXCEPT
        :   m_type(get_invalid_type()),
            m_container(nullptr),
            m_get_size_func(associative_container_empty::get_size),
            m_begin_func(associative_container_empty::begin),
            m_end_func(associative_container_empty::begin),
            m_equal_func(associative_container_empty::equal),
            m_create_func(associative_container_empty::create),
            m_delete_func(associative_container_empty::destroy),
            m_get_key_func(associative_container_empty::get_key),
            m_get_value_func(associative_container_empty::get_value),
            m_advance_func(associative_container_empty::advance)
        {
        }

        template<typename T, typename RawType = raw_type_t<T>, typename ConstType = remove_pointer_t<T>>
        variant_associative_view_private(const T& container) RTTR_NOEXCEPT
        :   m_type(type::get<RawType>()),
            m_container(as_void_ptr(container)),
            m_get_size_func(associative_container_mapper<RawType, ConstType>::get_size),
            m_begin_func(associative_container_mapper<RawType, ConstType>::begin),
            m_end_func(associative_container_mapper<RawType, ConstType>::end),
            m_equal_func(associative_container_mapper<RawType, ConstType>::equal),
            m_create_func(associative_container_mapper<RawType, ConstType>::create),
            m_delete_func(associative_container_mapper<RawType, ConstType>::destroy),
            m_get_key_func(associative_container_mapper<RawType, ConstType>::get_key),
            m_get_value_func(associative_container_mapper<RawType, ConstType>::get_value),
            m_advance_func(associative_container_mapper<RawType, ConstType>::advance)
        {

        }

        RTTR_INLINE variant_associative_view_private(const variant_associative_view_private& other) = default;

        RTTR_INLINE ~variant_associative_view_private()
        {

        }

        RTTR_INLINE type get_type() const RTTR_NOEXCEPT
        {
            return m_type;
        }

        RTTR_INLINE void copy(iterator_data& itr_tgt, const iterator_data& itr_src) const
        {
            m_create_func(itr_tgt, itr_src);
        }

        RTTR_INLINE void destroy(iterator_data& itr) const
        {
            m_delete_func(itr);
        }

        RTTR_INLINE void begin(iterator_data& itr) const
        {
            m_begin_func(m_container, itr);
        }

        RTTR_INLINE void end(iterator_data& itr) const
        {
            m_end_func(m_container, itr);
        }

        RTTR_INLINE std::size_t get_size() const RTTR_NOEXCEPT
        {
            return m_get_size_func(m_container);
        }

        RTTR_INLINE bool equal(const iterator_data& lhs_itr, const iterator_data& rhs_itr) const RTTR_NOEXCEPT
        {
            return m_equal_func(lhs_itr, rhs_itr);
        }

        RTTR_INLINE std::pair<variant, variant> get_value(const iterator_data& itr) const
        {
            return {m_get_key_func(itr), m_get_value_func(itr)};
        }

        RTTR_INLINE void advance(iterator_data& itr, std::size_t index) const
        {
            m_advance_func(itr, index);
        }

    private:
        void eequal_cmp_dummy_func(const iterator_data& lhs_itr, const iterator_data& rhs_itr) RTTR_NOEXCEPT;
        using equality_func = decltype(&eequal_cmp_dummy_func); // workaround because of 'noexcept' can only appear on function declaration

        using find_func         = std::size_t(*)(void *container, const void *p, void **itr);
        using get_size_func     = std::size_t(*)(void* container);
        using begin_func        = void(*)(void* container, iterator_data& itr);
        using end_func          = void(*)(void* container, iterator_data& itr);
        using advance_func      = void(*)(iterator_data& itr, std::size_t index);
        using create_func       = void(*)(iterator_data& itr_tgt, const iterator_data& itr_src);
        using delete_func       = void(*)(iterator_data& itr);
        using get_key_func      = variant (*)(const iterator_data& itr);
        using get_value_func    = variant (*)(const iterator_data& itr);

        type            m_type;
        void*           m_container;
        get_size_func   m_get_size_func;
        begin_func      m_begin_func;
        end_func        m_end_func;
        equality_func   m_equal_func;
        create_func     m_create_func;
        delete_func     m_delete_func;
        get_key_func    m_get_key_func;
        get_value_func  m_get_value_func;
        advance_func    m_advance_func;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_ASSOCIATIVE_VIEW_P_H_
