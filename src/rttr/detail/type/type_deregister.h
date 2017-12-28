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

#ifndef RTTR_TYPE_DEREGISTER_H_
#define RTTR_TYPE_DEREGISTER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/string_view.h"


namespace rttr
{

class property;
class method;
class enumeration;
class constructor;

namespace detail
{

/*!
 * Dergisters previousely registered items.
 */
class RTTR_API type_deregister
{
    public:

        /*!
         * \brief unregister the given property \p prop.
         *
         */
        static void item(const type& t) RTTR_NOEXCEPT;

        /*!
         * \brief unregister the given property \p prop.
         *
         */
        static void item(const property& prop) RTTR_NOEXCEPT;

        /*!
         * \brief unregister the given method \p meth.
         *
         */
        static void item(const method& meth) RTTR_NOEXCEPT;

        /*!
         * \brief unregister the given global method \p meth.
         *
         */
        static void global_method(const method& meth) RTTR_NOEXCEPT;

        /*!
         * \brief unregister the given global property \p prop.
         *
         */
        static void global_property(const property& prop) RTTR_NOEXCEPT;

        /*!
         * \brief unregister the given constructor \p ctor.
         *
         */
        static void item(const constructor& ctor) RTTR_NOEXCEPT;


        /*!
         * \brief unregister the given enumeration \p e.
         *
         */
        static void item(const enumeration& enum_) RTTR_NOEXCEPT;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DEREGISTER_H_
