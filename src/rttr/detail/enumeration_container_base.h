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

#ifndef __RTTR_ENUMERATION_CONTAINER_BASE_H__
#define __RTTR_ENUMERATION_CONTAINER_BASE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/type.h"
#include "rttr/detail/metadata_container.h"

#include <string>
#include <vector>
#include <initializer_list>

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
class RTTR_API enumeration_container_base : public metadata_container
{
    public:
        enumeration_container_base(const type declaring_type);
        virtual ~enumeration_container_base();

        virtual type get_underlying_type() const = 0;
        
        virtual type get_type() const = 0;

        virtual std::vector<std::string> get_keys() const = 0;

        virtual std::vector<variant> get_values() const = 0;

        virtual std::string value_to_key(detail::argument& value) const = 0;

        virtual variant key_to_value(const std::string& key) const = 0;

        // Returns the class that declares this property.
        type get_declaring_type() const;

    private:
        const type  _declaring_type;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_ENUMERATION_CONTAINER_BASE_H__
