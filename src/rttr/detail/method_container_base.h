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

#ifndef __RTTR_METHOD_CONTAINER_BASE_H__
#define __RTTR_METHOD_CONTAINER_BASE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/detail/metadata_container.h"

#include <string>
#include <vector>

namespace rttr
{
class type;

namespace detail
{
class argument;
class instance;

/*!
 * Abstract class for a method.
 * 
 * This is the base class for all methods.
 * You can invoke the method via method_container_base::invoke.
 */
class RTTR_API method_container_base : public metadata_container
{
    public:
        method_container_base(const std::string& name, const type decalaring_type);

        virtual ~method_container_base();

        std::string get_name() const;
        type get_declaring_type() const;
        std::string get_signature() const;

        virtual type get_return_type() const = 0;
        virtual bool is_static() const = 0;
        virtual std::vector<type> get_parameter_types() const = 0;
        virtual std::vector<bool> get_is_reference() const = 0;
        virtual std::vector<bool> get_is_const() const = 0;

        virtual variant invoke(detail::instance& object) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3,
                               detail::argument& arg4) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3,
                               detail::argument& arg4, detail::argument& arg5) const = 0;
        virtual variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3,
                               detail::argument& arg4, detail::argument& arg5, detail::argument& arg6) const = 0;

        virtual variant invoke_variadic(detail::instance& object, std::vector<detail::argument>& args) const = 0;

    private:
        const std::string   _name;
        const type          _decalaring_type;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_METHOD_CONTAINER_BASE_H__
