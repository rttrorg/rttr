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

#ifndef __RTTR_CONSTRUCTOR_CONTAINER_BASE_H__
#define __RTTR_CONSTRUCTOR_CONTAINER_BASE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/type.h"
#include "rttr/detail/metadata_container.h"

#include <string>
#include <vector>

namespace rttr
{

namespace detail
{
class argument;

/*!
 * Abstract class for a method.
 * 
 * This is the base class for all methods.
 * You can invoke the method.
 */
class RTTR_API constructor_container_base : public metadata_container
{
    public:
        constructor_container_base(type decalaring_type);
        constructor_container_base();

        virtual ~constructor_container_base();
        type get_declaring_type() const;
        std::string get_signature() const;
        virtual type get_instanciated_type() const = 0;
        virtual std::vector<type> get_parameter_types() const = 0;

        virtual std::vector<bool> get_is_reference() const;
        virtual std::vector<bool> get_is_const() const;
        
        virtual variant invoke() const;
        virtual variant invoke(detail::argument& arg1) const;
        virtual variant invoke(detail::argument& arg1, detail::argument& arg2) const;
        virtual variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3) const;
        virtual variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4) const;
        virtual variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4,
                               detail::argument& arg5) const;
        virtual variant invoke(detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4,
                               detail::argument& arg5, detail::argument& arg6) const;
        
        virtual variant invoke_variadic(std::vector<detail::argument>& args) const = 0;
    private:
        const type _decalaring_type;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_CONSTRUCTOR_CONTAINER_BASE_H__