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

#ifndef RTTR_TYPE_H_
#define RTTR_TYPE_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <type_traits>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

namespace rttr
{
class variant;
class constructor;
class destructor;
class method;
class property;
class enumeration;
class type;
class instance;
class argument;

template<typename Target_Type, typename Source_Type>
Target_Type rttr_cast(Source_Type object);

namespace detail
{
struct derived_info;
struct base_class_info;
struct type_converter_base;
class type_register;
class type_database;

template<typename T, typename Enable = void>
struct type_getter;

static type get_invalid_type();
} // end namespace detail

/*!
 * The \ref type class holds the type information for any arbitrary object.
 * 
 * Every class or primitive data type can have an unique type object.
 * With the help of this object you can compare unknown types for equality at runtime or introspect the type
 * for its \ref property "properties", \ref method "methods", \ref enumeration "enumerations", 
 * \ref constructor "constructors" and \ref destructor "destructor".
 * 
 * Retrieve %type
 * ------------------
 * A type object **cannot** be created. It is only possible to retrieve a type object via three static template member functions:
 *
 * ### type::get<T>() ###
 *
 * This function just expects one template argument. Use it to check against a known type.
 *
 * \code{.cpp}
 *      type::get<int>() == type::get<int>();  // yields to true
 *      type::get<int>() == type::get<bool>(); // yields to false
 * \endcode
 *
 * ### type::get_by_name(const char*) ###
 *
 * This function just expects the name of the type. This is useful when you know only the name of the type and cannot include the type itself into the source code.
 *
 * \code{.cpp}
 *      type::get_by_name("int")  == type::get<int>(); // yields to true
 *      type::get_by_name("bool") == type::get<int>(); // yields to false
 *      type::get_by_name("MyNameSpace::MyStruct") == type::get<MyNameSpace::MyStruct>();  // yields to true
 * \endcode
 *
 * \remark Before using the function \ref type::get_by_name(), you have to use one time the function via \ref type::get<T>(), otherwise the type is not registered in the type system.
 *
 * ### type::get<T>(T&& obj) ###
 *
 * This function takes a universal reference and returns from every given object the corresponding type object.
 *
 * \code{.cpp}
 *      int int_obj;
 *      int* int_obj_ptr = &int_obj;
 *      const int* c_int_obj_ptr = int_obj_ptr;
 *      
 *      type::get<int>()         == type::get(int_obj);        // yields to true
 *      type::get<int*>()        == type::get(int_obj_ptr);    // yields to true
 *      type::get<const int*>()  == type::get(c_int_obj_ptr);  // yields to true
 * \endcode
 *
 * When this function is called for a glvalue expression whose type is a polymorphic class type,
 * then the result refers to a \ref type object representing the type of the most derived object.
 *
 * \code{.cpp}
 *      struct Base { RTTR_ENABLE() };
 *      struct Derived : Base { RTTR_ENABLE(Base) };
 *      //...
 *      Derived d;
 *      Base& base = d;
 *      type::get<Derived>()   == type::get(base);      // yields to true
 *      type::get<Base>()      == type::get(base);      // yields to false
 *      
 *      // remark, when called with pointers:
 *      Base* base_ptr = &d;
 *      type::get<Derived>()   == type::get(base_ptr);  // yields to false
 *      type::get<Base*>()     == type::get(base_ptr);  // yields to true
 * \endcode
 *
 * \remark If the type of the expression is a cv-qualified type, the result of the rttr::type::get expression refers to a rttr::type object representing the cv-unqualified type.
 * 
 * \code{.cpp}
 *      class D { ... };
 *      D d1;
 *      const D d2;
 *      type::get(d1)  == type::get(d2);         // yields true
 *      type::get<D>() == type::get<const D>();  // yields true
 *      type::get<D>() == type::get(d2);         // yields true
 *      type::get<D>() == type::get<const D&>(); // yields true
 *      type::get<D>() == type::get<const D*>(); // yields false
 * \endcode
 * Any `top level` cv-qualifier of the given type `T` will be removed.
 *
 *
 * Copying and Assignment
 * ----------------------
 * A \ref type object is lightweight and can be copied by value. However, each copy will refer to the same underlying type.
 *
 */
class RTTR_API type
{
    public:
        typedef uint16_t type_id;

        /*!
         * \brief Assigns a type to another one.
         *
         */
        RTTR_INLINE type(const type& other);

        /*!
         * \brief Assigns a type to another one.
         *
         * \return A type object.
         */
        RTTR_INLINE type& operator=(const type& other);

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is less than the \a other.
         */
        RTTR_INLINE bool operator<(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is greater than the \a other.
         */
        RTTR_INLINE bool operator>(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is greater than or equal to \a other.
         */
        RTTR_INLINE bool operator>=(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is less than or equal to \a other.
         */
        RTTR_INLINE bool operator<=(const type& other) const;

        /*!
         * \brief Compares this type with the \a other type and returns true 
         *        if both describe the same type, otherwise returns false.
         *
         * \return True if both type are equal, otherwise false.
         */
        RTTR_INLINE bool operator==(const type& other) const;

        /*!
         * \brief Compares this type with the \a other type and returns true 
         *        if both describe different types, otherwise returns false.
         *
         * \return True if both type are \b not equal, otherwise false.
         */
        RTTR_INLINE bool operator!=(const type& other) const;

        /*!
         * \brief Returns the id of this type.
         *
         * \note This id is unique at process runtime,
         *       but the id can be changed every time the process is executed.
         *
         * \return The type id.
         */
        RTTR_INLINE type_id get_id() const;

        /*!
         * \brief Returns the unique and human-readable name of the type.
         *
         * \remark The content of this string is compiler depended.
         *
         * \return The type name.
         */
        std::string get_name() const;
        
        /*!
         * \brief Returns true if this type is valid, that means the type holds valid data to a type.
         * 
         * \return True if this type is valid, otherwise false.
         */
        RTTR_INLINE bool is_valid() const;

        /*!
         * \brief Convenience function to check if this \ref type is valid or not.
         *
         * \return True if this \ref type is valid, otherwise false.
         */
         RTTR_INLINE explicit operator bool() const;

        /*!
         * \brief Returns a type object which represent the raw type.
         *        A raw type, is a type type without any qualifiers (const and volatile) nor any pointer.
         *
         * \remark When the current type is already the raw type, it will return an copy from itself.
         *
         * \return The corresponding raw type object.
         */
        type get_raw_type() const;

        /*!
         * \brief Returns a type object which represent the wrapped type.
         *        A wrapper type is a class which encapsulate an instance of another type.
         *        This encapsulate type is also called *wrapped type*.
         *
         * See following example code:
         * \code{.cpp}
         *   type wrapped_type = type::get<std::shared_ptr<int>>().get_wrapped_type(); 
         *   wrapped_type == type::get<int*>(); // yields to true
         *
         *   wrapped_type = type::get<std::reference_wrapper<int>>().get_wrapped_type(); 
         *   wrapped_type == type::get<int>(); // yields to true
         * \endcode
         *
         * \remark When the current type is not a wrapper type, this function will return an \ref type::is_valid "invalid type".
         *
         * \see wrapper_mapper<T>
         *
         * \return The type object of the wrapped type.
         */
        type get_wrapped_type() const;

        /*!
         * \brief Returns a type object for the given template type \a T.
         *
         * \return type for the template type \a T.
         */
        template<typename T> 
        static type get();

        /*!
         * \brief Returns a type object for the given instance \a object.
         *
         * \remark If the type of the expression is a cv-qualified type, the result of the type::get() expression refers to a
         *         type object representing the cv-unqualified type.
         *         When type::get() is applied to a glvalue expression whose type is a polymorphic class type,
         *         the result refers to a type object representing the type of the most derived object.
         *
         * \return type for an \a object of type \a T.
         */
        template<typename T> 
        static type get(T&& object);

        /*!
         * \brief Returns the type object with the given name \p name.
         *
         * \remark The search for the type is case sensitive. White spaces will be ignored.
         *         The name of the type corresponds to the name which was used during \ref RTTR_REGISTRATION.
         *         Only after the registration process was executed, then the type can be retrieved with this function.
         *         Otherwise and invalid type will be returned.
         *
         * \return \ref type object with the name \p name.
         */
        static type get_by_name(const char* name);

        /*!
         * \brief Returns a list of all registered type objects.
         *
         * \remark The order of the type object is unspecified.
         *
         * \return A vector of type objects.
         */
        static std::vector<type> get_types();

        /*!
         * \brief Returns the size in bytes of the object representation of the current type (i.e. `sizeof(T)`).
         *
         * \return The size of the type in bytes.
         */
        std::size_t get_sizeof() const;

        /*!
         * \brief Returns true whether the given type is class; that is not an atomic type or a method.
         *
         * \return True if the type is a class, otherwise false.
         */
        bool is_class() const;

        /*!
         * \brief Returns true whether the given type represents an enumeration.
         *
         * \return True if the type is an enumeration, otherwise false.
         */
        bool is_enumeration() const;

        /*!
         * \brief Returns the enumerator if this type is an enum type; 
         *        otherwise the returned value is \ref enumeration::is_valid "not valid".
         *
         * \see is_enumeration()
         *
         * \return A enumeration object.
         */
        enumeration get_enumeration() const;

        /*!
         * \brief Returns true whether the given type represents a wrapper type.
         *        A wrapper type is a class which encapsulate an instance of another type.
         *        RTTR recognize automatically following wrapper types:
         *        - \p `std::shared_ptr<T>` 
         *        - \p `std::reference_wrapper<T>`
         *        - \p `std::weak_ptr<T>`
         *        - \p `std::unique_ptr<T>`
         *
         *        In order to work with custom wrapper types, its required to specialize the class \ref rttr::wrapper_mapper<T> "wrapper_mapper<T>"
         *        and implement a getter function to retrieve the encapsulate type.
         *
         * \see \ref rttr::wrapper_mapper<T> "wrapper_mapper<T>"
         *
         * \return True if the type is an wrapper, otherwise false.
         *
         */
        bool is_wrapper() const;

        /*!
         * \brief Returns true whether the given type represents an array.
         *
         * \return True if the type is an array, otherwise false.
         *
         * \see \ref array_mapper "array_mapper<T>"
         */
        bool is_array() const;

        /*!
         * \brief Returns true whether the given type represents a pointer.
         *        e.g. `int*`, or `bool*`
         *
         * \return True if the type is a pointer, otherwise false.
         */
        bool is_pointer() const;

        /*!
         * \brief Returns true whether the given type represents an arithmetic type.
         *        An arithmetic type is a integral type or a floating point type.
         *        e.g. `bool`, `int`, `float`, etc...
         *
         * \return True if the type is a arithmetic type, otherwise false.
         */
        bool is_arithmetic() const;

        /*!
         * \brief Returns true whether the given type represents a pointer to a function 
         *        e.g. `void (*)(void)`
         *
         * \return True if the type is a function pointer, otherwise false.
         */
        bool is_function_pointer() const;

        /*!
         * \brief Returns true whether the given type represents a pointer to a member object.
         *        e.g. `int (MyClass::*)`
         *
         * \return True if the type is a member object pointer, otherwise false.
         */
        bool is_member_object_pointer() const;

        /*!
         * \brief Returns true whether the given type represents a pointer to a member function.
         *        e.g. `void (MyClass::*)(void)`
         *
         * \return True if the type is a member function pointer type, otherwise false.
         */
        bool is_member_function_pointer() const;

        /*!
         * \brief Returns true if this type is derived from the given type \p other, otherwise false.
         *
         * \remark Make sure that the complete class hierarchy has the macro RTTR_ENABLE
         *         inside the class declaration, otherwise the returned information of this function
         *         is **not correct**.
         *
         * \return Returns true if this type is a derived type from \p other, otherwise false.
         */
        bool is_derived_from(const type& other) const;

        /*!
         * \brief Returns true if this type is derived from the given type \a T, otherwise false.
         *
         * \remark Make sure that the complete class hierarchy has the macro RTTR_ENABLE
         *         inside the class declaration, otherwise the returned information of this function
         *         is **not correct**.
         *
         * \return Returns true if this type is a derived type from \a T, otherwise false.
         */
        template<typename T>
        bool is_derived_from() const;

        /*!
         * \brief Returns a list of all base classes of this type.
         *
         * \remark Make sure that the complete class hierarchy has the macro RTTR_ENABLE
         *         inside the class declaration, otherwise the returned information of this function
         *         is **not correct**. The order of this list depends on the declaration order of classes
         *         inside RTTR_ENABLE. E.g. RTTR_ENABLE(A1, A2) => A1 will be for A2 in the list.
         *         Accordingly the root (or parent or base) class is always the first type in the list.
         *
         * \return A list of type objects.
         */
        std::vector<type> get_base_classes() const;

        /*!
         * \brief Returns a list of all derived classes of this type.
         *
         * \remark Make sure that the complete class hierarchy has the macro RTTR_ENABLE
         *         inside the class declaration, otherwise the returned information of this function
         *         is **not correct**. The order of this list depends on the declaration order of classes
         *         inside RTTR_ENABLE. E.g. RTTR_ENABLE(A1, A2) => A1 will be for A2 in the list.
         *
         * \return A list of type objects.
         */
        std::vector<type> get_derived_classes() const;

        /////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////

         /*!
         * \brief Returns the meta data for the given key \p key.
         *
         * \remark When no meta data is registered with the given \p key, 
         *         an invalid \ref variant object is returned (see \ref variant::is_valid).
         *
         * \return A variant object, containing arbitrary data.
         */
        variant get_metadata(const variant& key) const;

        /*!
         * \brief Returns a public constructor whose parameters match the types in the specified list.
         *
         * \remark When no parameter list is given, it will be searched for the default constructor.
         *
         * \return A valid constructor will be returned when the parameter matches the registered constructor; 
         *         otherwise an invalid constructor.
         */
        constructor get_constructor(const std::vector<type>& params = std::vector<type>() ) const;

        /*!
         * \brief Returns a list of all registered constructors for this type; the order is unspecified.
         * 
         * \return Returns a list of all registered constructors.
         */
        std::vector<constructor> get_constructors() const;

        /*!
         * \brief Creates an instance of the current type, with the given arguments \p args for the constructor.
         *
         * \remark When the argument types does not match the parameter list of the constructor then the he will not be invoked.
         *         Constructors with registered \ref default_arguments will be honored.
         * 
         * \return Returns an instance of the given type.
         */
        variant create(std::vector<argument> args = std::vector<argument>()) const;

        /*!
         * \brief Returns the corresponding destructor for this type.
         *
         * \remark When there is no constructor registered for this type, then also the destructor is not available.
         *        A destructor will always been automatically registered.
         *
         * \return Returns the destructor for this type.
         */
        destructor get_destructor() const;

        /*!
         * \brief Destroys the given object \p obj.
         *
         * \remark When the \p obj could be destroyed the given \p obj is invalid after calling this method;
         *         Otherwise it is still valid.
         */
        void destroy(variant& obj) const;


        /*!
         * \brief Returns a property with the name \p name.
         *
         * \remark When there exists no property with the name \p name, then an invalid property is returned.
         *
         * \return A property with name \p name.
         */
        property get_property(const char* name) const;

        /*!
         * \brief Returns a list of all registered properties for this type and
         *        all its base classes.
         *
         * \remark The properties are sorted after its order of registration.
         *
         * \return A vector with properties.
         */
        std::vector<property> get_properties() const;

        /*!
         * \brief Returns a global property with the name \p name.
         *
         * \remark When there exists no property with the name \p name, then an invalid property is returned.
         *
         * \return A property with name \p name.
         */
        static property get_global_property(const char* name);

        /*!
         * \brief Returns a list of all registered global properties.
         *
         * \remark The order of the properties in the vector is undefined.
         *         Client code should **not** depend on the order in which properties are returned, 
         *         because that order varies.
         *
         * \return A vector with properties.
         */
        static std::vector<property> get_global_properties();


        /*!
         * \brief Returns the property value of property named \p name from the instance \p obj.
         *
         * \remark When the given instance is empty, the value of global property will be tryed to returned.
         *
         * \return A variant containing the value of the property.
         */
        variant get_property_value(const char* name, instance obj) const;

        /*!
         * \brief Returns the property value of property named \p name.
         *
         * \return A variant containing the value of the property.
         */
        static variant get_property_value(const char* name);

        /*!
         * \brief This function will set the given value \p arg to a property named \p name to the instance \p obj.
         *
         * \remark When the given instance is empty, the value of a global property with name \p name will be tryed to set.
         *
         * \return A bool value, which is true, when the value could be set, otherwise false.
         */
        bool set_property_value(const char* name, instance obj, argument arg) const;

        /*!
         * \brief This function will set the given value \p arg to a property named \p name.
         *
         * \return A bool value, which is true, when the value could be set, otherwise false.
         */
        static bool set_property_value(const char* name, argument arg);


        /*!
         * \brief Returns a method with the name \p name.
         *
         * \remark When there exists no method with the name \p name, then an invalid method is returned.
         *
         * \return A method with name \p name.
         */
        method get_method(const char* name) const;

        /*!
         * \brief Returns a method with the name \p name which match the given parameter list \p params.
         *
         * \remark When there exists no method with the name \p name and matching parameter list \p params,
         *         then an invalid method is returned.
         *
         * \return A method with name \p name.
         */
        method get_method(const char* name, const std::vector<type>& params) const;
        
        /*!
         * \brief Returns a list of all registered methods for this type and
         *        all its base classes.
         *
         * \remark The methods are sorted after its order of registration.
         *
         * \return A vector with method objects
         */
        std::vector<method> get_methods() const;

        /*!
         * \brief Returns a global method with the name \p name.
         *
         * \remark When there exists no method with the name \p name, and invalid method is returned.
         *
         * \return A method with name \p name.
         */
        static method get_global_method(const char* name);

        /*!
         * \brief Returns a global method with the name \p name which match the given parameter list \p params.
         *
         * \remark When there exists no method with the name \p name and matching parameter list \p params,
         *         then an invalid method is returned.
         *
         * \return A method with name \p name and parameter signature \p params.
         */
        static method get_global_method(const char* name, const std::vector<type>& params);

        /*!
         * \brief Returns a list of all registered global methods.
         *
         * \remark The order of the methods in the vector is undefined.
         *         Client code should **not** depend on the order in which methods are returned, 
         *         because that order varies.
         *
         * \return A vector with methods.
         */
        static std::vector<method> get_global_methods();


        /*!
         * \brief Invokes the method represented by the current instance \p object.
         *
         * \remark When it's a static method you still need to provide an instance object,
         *         use therefore the default ctor of \ref instance::instance() "instance()",
         *         or as shortcut use simply `{}`.
         *         Methods with registered \ref default_arguments will be honored.
         *
         * \return A variant object containing the possible return value, 
         *         otherwise when it is a void function an empty but valid variant object.
         */
        variant invoke(const char* name, instance obj, std::vector<argument> args) const;

        /*!
         * \brief Invokes a global method named \p name with the specified argument \p args.
         *
         * \return A variant object containing the possible return value, 
         *         otherwise when it is a void function an empty but valid variant object.
         *         Methods with registered \ref default_arguments will be honored.
         */
        static variant invoke(const char* name, std::vector<argument> args);

        /*!
         * \brief Register a converter func `F`, which will be used internally by the 
         *        \ref variant class to convert between types.
         *
         *  This function converts a *source* type to a *target* type.
         *  The signature of this function has to be the following: `TargetType (SourceType, bool& ok)`
         *
         * See following example code:
         *  \code{.cpp}
         *   std::string conv_func(int value, bool& ok)
         *   {
         *      std::string result = std::to_string(value);
         *      ok = true;
         *      return result;
         *   }
         *   // register the conversion function
         *   type::register_converter_func(conv_func);
         *  \endcode
         *
         * \see variant::convert()
         */
        template<typename F>
        static void register_converter_func(F func);

        /*!
         * \brief Register comparison operators for template type \p T.
         *        This requires a valid `operator==` and `operator<` for type \p T.
         *
         * The registered comparison operators will be used in the \ref variant class.
         *
         * See following example code:
         *  \code{.cpp}
         *  // two variants, same content
         *   variant a = std::make_tuple<int, std::string>(12, std::string("some text"));
         *   variant b = std::make_tuple<int, std::string>(12, std::string("some text"));
         *
         *   a == b;                // yields to false
         *
         *   // register comparators for template type '<std::tuple<std::string, int>'
         *   type::register_comparators<std::tuple<std::string, int>>();
         *
         *   a == b;                // yields to true
         *  \endcode
         *
         * \see variant::operator==(), variant::operator<()
         */
        template<typename T>
        static void register_comparators();

    private:

        /*!
         * Constructs an empty and invalid type object.
         */
        RTTR_INLINE type();

        /*!
         * \brief Constructs a valid type object.
         *
         * \param id The unique id of the data type.
         */
        RTTR_INLINE type(type_id id);

        /*!
         * \brief This function try to convert the given pointer \p ptr from the type \p source_type
         *        to the target type \p target_type.
         *
         * \remark The returned pointer is always the raw type of \p target_type. You do not have to use this function by your own.
         *
         * \return Returns the converted pointer; when the conversion fails is a null pointer is returned.
         */
        static void* apply_offset(void* ptr, const type& source_type, const type& target_type);

        /*!
         * \brief When for the current type instance a converter function to type \p target_type was registered,
         *        then this function returns a valid pointer to a type_converter_base object.
         *        Otherwise this function returns a nullptr.
         *
         */
        const detail::type_converter_base* get_type_converter(const type& target_type) const;

        /*!
         * \brief Returns the level of indirection for this this type. A.k.a pointer count. 
         *        E.g. (`int` will return `0`; `int*` will return `1`; `int**` will return `2`; etc...)
         *
         * \return The pointer dimension.
         */
        std::size_t get_pointer_dimension() const;

        /*!
         * \brief Returns the raw type of an array
         *        E.g.  for type `int[100]` it will return the type object of `int`;
         *              `const int[10][25]` will return also `int`; etc...
         *
         * \return The raw array type.
         */
        type get_raw_array_type() const;

        /*!
         * \brief Returns the compiler depended name of the type.
         *
         * \return The full type name.
         */
        std::string get_full_name() const;

        /*!
         * \brief Returns a normalized string of the given compiler depended type name.
         *
         * \return A normalized string of the given original type name.
         */
        static std::string normalize_orig_name(const char* name);

        /*!
         * \brief Initialize all the global variables needed to retrieve the type informations.
         *
         */
        static void init_globals();


        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////

        //! Creates a variant from the given argument data.
        variant create_variant(const argument& data) const;

        friend class variant;
        template<typename Target_Type, typename Source_Type>
        friend Target_Type rttr_cast(Source_Type object);

        template<typename T, typename Enable>
        friend struct detail::type_getter;
        friend class instance;
        friend class detail::type_register;
        friend type detail::get_invalid_type();
        friend class detail::type_database;
        
    private:
        type_id  m_id;
        static const type_id m_invalid_id = 0;
};

} // end namespace rttr

#include "rttr/detail/type/type_impl.h"

#endif // RTTR_TYPE_H_
