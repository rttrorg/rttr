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

#ifndef RTTR_VARIANT_ASSOCIATIVE_VIEW_H_
#define RTTR_VARIANT_ASSOCIATIVE_VIEW_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/variant.h"
#include "rttr/detail/variant_associative_view/variant_associative_view_p.h"

#include <cstddef>
#include <vector>
#include <memory>

namespace rttr
{
    class type;
    class instance;
    class argument;

/*!
 * The \ref variant_associative_view describes a class that refers to an
 * <a target="_blank" href=https://en.wikipedia.org/wiki/Associative_containers>associative container</a> (e.g: `std::map`)
 * inside a \ref variant.
 * With an instance of that class you can set/get values of such container,
 * without having access to the type declaration of the type or it's elements.
 *
 * A \ref variant_associative_view can be created directly from a \ref variant with its member function \ref variant::create_associative_view() "create_associative_view()".
 * \remark The instance of an variant_associative_view is always valid till the referenced \ref variant is valid, otherwise accessing a variant_associative_view
 *         is undefined behaviour.
 *
 * Meta Information
 * ----------------
 *
 *
 *
 * Copying and Assignment
 * ----------------------
 * A \ref variant_associative_view object can be copied and assigned,
 * however each copy will reference the address of same underlying \ref variant value.
 *
 * Typical Usage
 * ----------------------
 *
 * \code{.cpp}
 *
 * \endcode
 *
 * \see variant
 */
class RTTR_API variant_associative_view
{
    public:
        class const_iterator;

        /*!
         * \brief Constructs an invalid variant_map_view object.
         *
         * \see is_valid()
         */
        variant_associative_view();

        /*!
         * \brief Constructs a copy of the given variant_associative_view \p other.
         */
        variant_associative_view(const variant_associative_view& other);

        /*!
         * \brief Constructs a new variant_associative_view via move constructor.
         */
        variant_associative_view(variant_associative_view&& other) RTTR_NOEXCEPT;

        /*!
         * \brief Destroys the variant_associative_view.
         *
         * \remark The underlying data is not destroyed.
         */
        ~variant_associative_view() RTTR_NOEXCEPT;

        /*!
         * \brief Assigns the value of the \a other variant_associative_view to this variant_associative_view.
         *
         * \return A reference to the variant_associative_view with the new data.
         */
        variant_associative_view& operator=(const variant_associative_view& other) RTTR_NOEXCEPT;

        /*!
         * \brief Returns true if this variant_associative_view is valid, that means the object is holding some data.
         *        When the variant_associative_view doesn't hold any data it will return false.
         *
         * \return True if this array is valid, otherwise false.
         */
        bool is_valid() const RTTR_NOEXCEPT;

        /*!
         * \brief Convenience function to check if this \ref variant_associative_view is valid or not.
         *
         * \see is_valid()
         *
         * \return True if this \ref variant_associative_view is valid, otherwise false.
         */
        explicit operator bool() const RTTR_NOEXCEPT;

        /*!
         * \brief Swaps this variant_associative_view with the \a other variant_associative_view.
         */
        void swap(variant_associative_view& other) RTTR_NOEXCEPT;


       /*!
         * \brief Returns the \ref type object of this associative container.
         *
         * \remark When the view is not valid, this function will return an invalid type object.
         *
         * \return \ref type "Type" of the associative container.
         */
        type get_type() const RTTR_NOEXCEPT;

        /*!
         * \brief Returns the number of elements in the associative container.
         *
         * \return The number of elements in the associative container.
         */
        std::size_t get_size() const RTTR_NOEXCEPT;

        /*!
         * \brief Finds an element with specific key \p key .
         *
         * \return The element with key equivalent to \p key. If no element is found an invalid variant is returned.
         */
        variant find(argument key);

        /*!
         * \brief Removes the element (if one exists) with the key equivalent to \p key.
         *
         * \return The number of elements removed.
         */
        std::size_t erase(argument key);

        /*!
         * \brief Returns a range containing all elements with the given \p key in the container.
         *
         * \return std::pair containing a pair of iterators defining the wanted range:
         *         the first pointing to the first element that is not less than \p key and
         *         the second pointing to the first element greater than \p key.
         */
        std::pair<const_iterator, const_iterator> equal_range(argument key);

        /*!
         * \brief Returns an iterator to the first element of the container.
         *
         * \see end()
         *
         * \return Iterator to the first element .
         */
        const_iterator begin();

        /*!
         * \brief Returns an iterator to the element following the last element of the container.
         *
         * \see begin()
         *
         * \return Iterator to the element following the last element.
         */
        const_iterator end();

        class RTTR_API const_iterator
        {
            public:
                using self_type = const_iterator;
                using value_type = variant;

                ~const_iterator();
                const_iterator(const const_iterator &other);
                const_iterator& operator=(const_iterator other);

                const std::pair<variant, variant> operator*() const;

                const_iterator &operator++();
                const_iterator operator++(int);
                const_iterator &operator--();
                const_iterator operator--(int);
                const_iterator &operator+=(int i);
                const_iterator &operator-=(int i);
                const_iterator operator+(int i) const;
                const_iterator operator-(int i) const;

                bool operator==(const const_iterator &self_type) const;
                bool operator!=(const const_iterator &self_type) const;
            private:
                const_iterator(const detail::variant_associative_view_private* view) RTTR_NOEXCEPT;
                void swap(const_iterator& other);

                friend class variant_associative_view;

                const detail::variant_associative_view_private* m_view;
                detail::iterator_data m_itr;
        };
    private:
        friend class variant;
        friend class argument;

        detail::variant_associative_view_private m_view;
};

} // end namespace rttr

#endif // RTTR_VARIANT_ASSOCIATIVE_VIEW_H_
