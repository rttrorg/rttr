/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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

#ifndef __RTTR_DESTRUCTORCONTAINERBASE_H__
#define __RTTR_DESTRUCTORCONTAINERBASE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/type.h"

#include <string>
#include <initializer_list>

namespace rttr
{

namespace detail
{
/*!
 * Abstract class for a method.
 * 
 * This is the base class for all methods.
 * You can invoke the method.
 */
class RTTR_API destructor_container_base
{
    public:
        virtual ~destructor_container_base();

        virtual type get_destructed_type() const = 0;
        virtual void invoke(variant& obj) const = 0;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_DESTRUCTORCONTAINERBASE_H__
