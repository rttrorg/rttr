/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
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

#ifndef RTTR_ACCESS_LEVEL_H_
#define RTTR_ACCESS_LEVEL_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"

namespace rttr
{
namespace detail
{
    struct public_access    {};
    struct protected_access {};
    struct private_access   {};

    using access_level_list = type_list<public_access, protected_access, private_access>;
}

/*!
 * The \ref access_level class contains three static members, which can be used to specify
 * the access control for \ref method "methods" or \ref property "properties"
 *
 * \code{.cpp}
 *
 *  registration::class<foo>("foo")
 *      .method("test", &foo::func_1, access_level::public_access);
 *
 * \endcode
 */
struct RTTR_API access_level
{
    /*!
     * Declares the visibility of class member as *public*.
     */
    static const detail::public_access      public_access;

    /*!
     * Declares the visibility of class member as *protected*.
     */
    static const detail::protected_access   protected_access;

    /*!
     * Declares the visibility of class member as *private*.
     */
    static const detail::private_access     private_access;
};

} // end namespace rttr

#endif // RTTR_ACCESS_LEVEL_H_
