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

#ifndef __RTTR_ENUMERATION_H__
#define __RTTR_ENUMERATION_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/type.h"

#include <memory>
#include <string>

namespace rttr
{
class type;
class Variant;

namespace detail
{
class enumeration_container_base;
class argument;
}
/*!
 * The \ref enumeration class provides several meta information about an enum.
 * 
 * A instance of an enumeration class can only be obtained from the \ref type class.
 * See \ref type::get_enum() and \ref type::get_enumerations().
 *
 * For registration an enum, nested inside a class, see \ref class_::enumeration() and for global enums see \ref register_global::enumeration.
 *
 * Meta Information
 * ----------------
 * An \ref enumeration is described by it's name (\ref get_name()), it's keys (\ref get_keys()) and it's corresponding values (\ref get_values()).
 * The key is represented as std::string and the values are stored as the underlying enum value.
 * When the \ref enumeration was declared inside a class, then \ref get_declaring_type() can be used to obtain the type of this class.
 *
 * The conversion functions \ref key_to_value(), \ref value_to_key() allow conversion between the value representation of an enumeration and its literal representation.
 *
 * Copying and Assignment
 * ----------------------
 * A \ref enumeration object is lightweight and can be copied by value. However, each copy will refer to the same underlying enumeration.
 *
 * Typical Usage
 * ----------------------
 * 
\code{.cpp}
  using namespace rttr;
  struct MyStruct 
  {
     enum E_Alignment
     {
        AlignLeft       = 0x0001,
        AlignRight      = 0x0002,
        AlignHCenter    = 0x0004,
        AlignJustify    = 0x0008
     };
  };
  enumeration enum_align = type::get("MyStruct").get_enumeration("E_Alignment");
  if (enum_align)
  {
     MyStruct::E_Alignment enum_value = MyStruct::AlignLeft;
     std::string key = enum_align.value_to_key(enum_value);
     std::cout << key; // prints "AlignLeft"
    
     variant var = enum_align.key_to_value("AlignJustify");
     std::cout << var.get_value<MyStruct::E_Alignment>(); // prints "8";
  }
\endcode
 *
 * \see method, property, constructor and type
 */
class RTTR_API enumeration
{
    public:
        /*!
         * \brief Returns true if this \ref enumeration is valid, otherwise false.
         *
         * \return True if this \ref enumeration is valid, otherwise false.
         */
        bool is_valid() const;

        /*!
         * \brief Convenience function to check if this \ref enumeration is valid or not.
         *
         * \return True if this \ref enumeration is valid, otherwise false.
         */
        operator bool() const;

        /*!
         * \brief Returns the name of this \ref enumeration.
         *
         * \return Name of the \ref enumeration.
         */
        std::string get_name() const;

        /*!
         * \brief Returns the underlying type (int, unsigned int, etc.) of this \ref enumeration.
         *
         * \return Data type of the \ref enumeration.
         */
        type get_underlying_type() const;

        /*!
         * \brief Returns the type object of this \ref enumeration.
         *
         * \return Data type of the \ref enumeration.
         */
        type get_type() const;

        /*!
         * \brief Returns the \ref type of the class or struct that declares this \ref enumeration.
         *
         * \remark When this enumeration does not belong to a class (i.e. is a global enumeration) it will return an invalid type object.
         *         When this enumeration is not valid, this function will return an invalid type object (see \ref type::is_valid).
         *
         * \return \ref type "Type" of the declaring class/struct for this enumeration.
         */
        type get_declaring_type() const;

        /*!
         * \brief Returns the metadata for the given key \p key.
         *
         * \remark When no metadata is registered with the given \p key, 
         *         an invalid \ref variant object is returned (see \ref variant::is_valid).
         *
         * \return A variant object, containing arbitrary data.
         */
        variant get_metadata(int key) const;

        /*!
         * \brief Returns the metadata for the given key \p key.
         *
         * \remark When no metadata is registered with the given \p key, 
         *         an invalid \ref variant object is returned (see \ref variant::is_valid).
         *
         * \return A variant object, containing arbitrary data.
         */
        variant get_metadata(const std::string& key) const;

        /*!
         * \brief Returns all enum keys registered for this enumeration.
         *
         * \remark When the enumeration is invalid then an empty vector is returned.
         *
         * \return A vector of enumeration keys.
         */
        std::vector<std::string> get_keys() const;


         /*!
         * \brief Returns all enum values registered for this enumeration.
         *
         * \remark When the enumeration is invalid then an empty vector is returned.
         *
         * \return A vector of enumeration values.
         */
        std::vector<variant> get_values() const;

        /*!
         * \brief Returns the string that is used as the name of the given enumeration \p value,
         *        or an empty string if the \p value is not defined.
         *
         * \return A std::string object, containing the key for the given value.
         */
        std::string value_to_key(detail::argument value) const;
        
        /*!
         * \brief Returns the value of the given enumeration key, or an empty variant if the key is not defined.
         *
         * \return A variant object, containing the value for the given key.
         */
        variant key_to_value(const std::string& key) const;

        /*!
         * \brief Returns true if this enumeration is the same like the \p other.
         *
         * \return True if both enumerations are equal, otherwise false.
         */
        bool operator==(const enumeration& other) const;

        /*!
         * Returns true if this enumeration is the not the same like the \p other.
         *
         * \return True if both enumerations are different, otherwise false.
         */
        bool operator!=(const enumeration& other) const;

    private:
        friend class type; // to prevent creation of this class
        //! Constructs a valid MetaProperty from a PropertyContainerBase.
        enumeration(const detail::enumeration_container_base* container = nullptr);
    private:
        const detail::enumeration_container_base* _container;
};

} // end namespace rttr

#endif // __RTTR_ENUMERATION_H__
