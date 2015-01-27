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

#ifndef __RTTR_PROPERTY_CONTAINER_BASE_H__
#define __RTTR_PROPERTY_CONTAINER_BASE_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/metadata_container.h"
#include "rttr/type.h"

#include <string>

namespace rttr
{
  
namespace detail
{
class instance;
class argument;
/*!
 * Abstract class for an instance of a Property.
 * 
 * This is the base class for all properties of the system.
 * It provide the basic mechanism for getting all meta data of a property,
 * but it also define a general interface to set/get properties via string: toString and fromString.
 */
class RTTR_API property_container_base : public metadata_container
{
    public:
        property_container_base(const std::string& name, const type decalaring_type);

        virtual ~property_container_base();

        // returns the name of this property.
        std::string get_name() const;

        //! Returns true whether this is a constant property, otherwise false.
        virtual bool is_readonly() const = 0;

        //! Returns true whether this is a static property, otherwise false.
        virtual bool is_static() const = 0;
    
        //! Returns the type of the underlying property.
        virtual type get_type() const = 0;

        //! Returns the class that declares this property.
        type get_declaring_type() const;

        //! Returns true when the underlying property is an array type.
        virtual bool is_array() const = 0;

        //! Sets this property of the given instance \p instance to the value of the argument \p argument.
        virtual bool set_value(detail::instance& object, detail::argument& arg) const = 0;

        //! Returns the value of this property from the given instance \p instance.
        virtual variant get_value(detail::instance& object) const = 0;
    private:
        const std::string   _name;
        const type          _decalaring_type;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_PROPERTY_CONTAINER_BASE_H__
