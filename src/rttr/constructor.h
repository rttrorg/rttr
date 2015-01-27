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
#ifndef __RTTR_CONSTRUCTOR_H__
#define __RTTR_CONSTRUCTOR_H__

#include "rttr/base/core_prerequisites.h"

#include <initializer_list>
#include <string>
#include <vector>

namespace rttr
{

class variant;
class type;

namespace detail
{
class constructor_container_base;
class argument;
}

/*!
 * The \ref constructor class provides several meta information about a constructor and can be invoked.
 *
 * A instance of a constructor class can only be obtained from the \ref type class.
 * See \ref type::get_constructor() and \ref type::get_constructors().
 *
 * For registration a constructor, nested inside a class, see \ref class_::constructor() and for global constructors see \ref constructor_.
 *
 * Meta Information
 * ----------------
 * A \ref constructor has a signature (\ref get_signature()) and instantiate exactly one type (\ref get_instanciated_type()).
 * With \ref get_parameter_types() you retrieve all type objects of the parameters for this constructor.
 * When the \ref constructor was declared inside a class, then \ref get_declaring_type() can be used to obtain the type of this class.
 *
 * The method can be invoked with \ref invoke(); When its not a \ref is_static "static method" you have to provide a class instance to invoke the method.
 * This instance can be the raw type on the stack; the current hierarchy level doesn't matter. It can be also a raw pointer to the object or
 * a \ref variant which contains the instance, again as pointer or stack object.
 *
 * Another way to invoke a method is to use the \ref type class through \ref type::create().
 *
 * Copying and Assignment
 * ----------------------
 * A \ref constructor object is lightweight and can be copied by value. However, each copy will refer to the same underlying constructor.
 *
 * Typical Usage
 * ----------------------
 * 
\code{.cpp}
    constructor string_ctor == type::get("std::string").get_constructor({type::get("const char*")});
    
    variant my_string = string_ctor.invoke({"Hello World"}); // returns an ptr to the object on the heap
    
    std::cout << my_string.get_value<std::string*>()->c_str() << std::endl; // prints 'Hello World'
    
    // don't forget to destroy the instance
    type::get("std::string").get_destructor().invoke(my_string);
    
    my_string.is_valid(); // yield to false
\endcode
 *
 * \see method, property, enumeration, destructor and type
 */
class RTTR_API constructor
{
    public:
       /*!
         * \brief Returns true if this constructor is valid, otherwise false.
         *
         * \return True if this constructor is valid, otherwise false.
         */
        bool is_valid() const;

        /*!
         * \brief Convenience function to check if this constructor is valid or not.
         *
         * \return True if this constructor is valid, otherwise false.
         */
        operator bool() const;

        /*!
         * Returns the type object of the instantiated type.
         *
         * \return The instantiated type.
         */
        type get_instanciated_type() const;

        /*!
         * \brief Returns the \ref type of the class or struct that declares this \ref constructor.
         *
         * \remark When this constructor does not belong to a class (i.e. is a primitive type) it will return an invalid type object.
         *         When this constructor is not valid, this function will return an invalid type object (see \ref type::is_valid).
         *
         * \return \ref type "Type" of the declaring class/struct for this enumeration.
         */
        type get_declaring_type() const;

        /*!
         * \brief Returns the signature of this constructor as readable string.
         *
         * \return The signature as readable string.
         */
        std::string get_signature() const;
        
        /*!
         * \brief Returns a list of type objects representing the number, order
         *        and type of the parameters for this constructor.
         *
         * \return A list representing the parameters of this constructor.
         */
        std::vector<type> get_parameter_types() const;

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
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke() const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1, detail::argument arg2) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1, detail::argument arg2, detail::argument arg3) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4,
                       detail::argument arg5) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *
         * \remark Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke(detail::argument arg1, detail::argument arg2, detail::argument arg3, detail::argument arg4,
                       detail::argument arg5, detail::argument arg6) const;

        /*!
         * \brief Invokes the constructor of type returned by \ref get_instanciated_type().
         *        The instance will always be created on the heap and will be returned as variant object.
         *         Use this method when you need to instantiate a constructor with more then 6 parameters.
         *
         * \remark Using this invoke function is slower, then specifying the arguments directly.
         *         Returns an invalid \ref variant object (see \ref variant::is_valid), when the arguments does 
         *         not match the parameters of the underlying constructor.
         *
         * \return An instance of the type \ref get_instanciated_type().
         */
        variant invoke_variadic(std::vector<detail::argument> args) const;

        /*!
         * \brief Returns true if this constructor is the same like the \p other.
         *
         * \return True if both constructors are equal, otherwise false.
         */
        bool operator==(const constructor& other) const;

        /*!
         * Returns true if this constructor is the not the same like the \p other.
         *
         * \return True if both constructors are different, otherwise false.
         */
        bool operator!=(const constructor& other) const;

    private:
        friend class type; // to prevent creation of this class
        constructor(const detail::constructor_container_base* container = nullptr);
    private:
        const detail::constructor_container_base* _container;
};

} // end namespace rttr

#endif // __RTTR_CONSTRUCTOR_H__
