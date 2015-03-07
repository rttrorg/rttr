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

#ifndef RTTR_TYPE_H_
#define RTTR_TYPE_H_

#include "rttr/base/core_prerequisites.h"

#include <type_traits>
#include <vector>
#include <string>
#include <memory>

namespace rttr
{
class variant;
class constructor;
class destructor;
class method;
class property;
class enumeration;
class type;

template<typename TargetType, typename SourceType>
TargetType rttr_cast(SourceType object);

namespace detail
{
class instance;
class argument;
struct derived_info;
struct base_class_info;
struct type_converter_base;

typedef variant(*variant_create_func)(const argument&);
} // end namespace detail

namespace impl
{
template<typename T, typename Enable = void>
struct MetaTypeInfo;
static type get_invalid_type();
} // end namespace impl

/*!
 * This class holds the type information for any arbitrary object.
 * 
 * Every class or primitive data type can have an unique type object.
 * With the help of this object you can compare unknown types for equality at runtime or introspect the type
 * for its properties, methods, enumerations, constructors and destructor.
 * 
 * Retrieve %type
 * ------------------
 * A type object **cannot** be created. It is only possible to retrieve a type object via three static template member functions:
 *
 * type::get<T>()
 *
 * This function just expects one template argument. Use it to check against a known type.
 *
 * \code{.cpp}
 *   type::get<int>() == type::get<int>();  // yields to true
 *   type::get<int>() == type::get<bool>(); // yields to false
 * \endcode
 *
 * type::get(const char*)
 *
 * This function just expects the name of the type. This is useful when you know only the name of the type and cannot include the type itself into the source code.
 * The name of the type is the same like you have registered with \ref RTTR_DECLARE_TYPE but as string literal. When you have used a typedef then you need to provide this typedef also as string literal.
 *
 * \code{.cpp}
 *   type::get("int") == type::get("int");   // yields to true
 *   type::get("bool") == type::get("int");  // yields to false
 *   type::get("MyNameSpace::MyStruct") == type::get("MyNameSpace::MyStruct");  // yields to true
 * \endcode
 *
 * type::get<T>(T&& obj)
 *
 * This function takes a universal reference and returns from every given object the corresponding type object.
 *
 * \code{.cpp}
 *   int int_obj;
 *   int* int_obj_ptr = &int_obj;
 *   const int* c_int_obj_ptr = int_obj_ptr;
 *   type::get<int>() == type::get(int_obj);             // yields to true
 *   type::get<int*>() == type::get(int_obj_ptr);        // yields to true
 *   type::get<const int*>() == type::get(c_int_obj_ptr);// yields to true
 * \endcode
 *
 * When this function is called for a glvalue expression whose type is a polymorphic class type,
 * then the result refers to a \ref type object representing the type of the most derived object.
 *
 * \code{.cpp}
 *   struct Base {};
 *   struct Derived : Base {};
 *   Derived d;
 *   Base& base = d;
 *   type::get<Derived>() == type::get(base) // yields to true
 *   type::get<Base>() == type::get(base)    // yields to false
 *   
 *   // remark, when called with pointers:
 *   Base* base_ptr = &d;
 *   type::get<Derived>() == type::get(base_ptr);  // yields to false
 *   type::get<Base*>() == type::get(base_ptr);    // yields to true
 * \endcode
 *
 * \remark If the type of the expression is a cv-qualified type, the result of the rttr::type::get expression refers to a rttr::type object representing the cv-unqualified type.
 * 
 * \code{.cpp}
 *   class D { ... };
 *   D d1;
 *   const D d2;
 *   type::get(d1)  == type::get(d2);         // yields true
 *   type::get<D>() == type::get<const D>();  // yields true
 *   type::get<D>() == type::get(d2);         // yields true
 *   type::get<D>() == type::get<const D&>(); // yields true
 *   type::get<D>() == type::get<const D*>(); // yields false
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
        typedef uint16 type_id;

        /*!
         * \brief Assigns a type to another one.
         *
         */
        type(const type& other);

        /*!
         * \brief Assigns a type to another one.
         *
         * \return A type object.
         */
        type& operator=(const type& other);

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is less than the \a other.
         */
        bool operator<(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is greater than the \a other.
         */
        bool operator>(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is greater than or equal to \a other.
         */
        bool operator>=(const type& other) const;

        /*!
         * \brief Comparison operator for sorting the type data according to some internal criterion.
         *
         * \return True if this type is less than or equal to \a other.
         */
        bool operator<=(const type& other) const;

        /*!
         * \brief Compares this type with the \a other type and returns true 
         *        if both describe the same type, otherwise returns false.
         *
         * \return True if both type are equal, otherwise false.
         */
        bool operator==(const type& other) const;

        /*!
         * \brief Compares this type with the \a other type and returns true 
         *        if both describe different types, otherwise returns false.
         *
         * \return True if both type are \b not equal, otherwise false.
         */
        bool operator!=(const type& other) const;

        /*!
         * \brief Returns the id of this type.
         *
         * \note This id is unique at process runtime,
         *       but the id can be changed every time the process is executed.
         *
         * \return The type id.
         */
        type_id get_id() const;

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
        bool is_valid() const;

        /*!
         * \brief Convenience function to check if this \ref type is valid or not.
         *
         * \return True if this \ref type is valid, otherwise false.
         */
        explicit operator bool() const;

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
         * \brief Returns the type object for the given \a name.
         *
         * \remark The search for the type is case sensitive. 
         *         The name itself correspond to the name registered with RTTR_DECLARE_TYPE.
         *
         * \return type for an \a object of name \a name.
         */
        static type get(const char* name);

        /*!
         * \brief Returns a list of all registered type objects.
         *
         * \remark The order of the type object is unspecified.
         *
         * \return A vector of type objects.
         */
        static std::vector<type> get_types();

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
         * \brief Returns true whether the given type represents an array.
         *
         * \return True if the type is an array, otherwise false.
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
         * \brief Returns true whether the given type represents an primitive type.
         *        e.g. `int`, `bool`, etc...
         *
         * \return True if the type is a primitive type, otherwise false.
         */
        bool is_primitive() const;

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
         *         is **not correct**.
         *
         * \return A list of type objects.
         */
        std::vector<type> get_base_classes() const;

        /*!
         * \brief Returns a list of all derived classes of this type.
         *
         * \remark Make sure that the complete class hierarchy has the macro RTTR_ENABLE
         *         inside the class declaration, otherwise the returned information of this function
         *         is **not correct**.
         *
         * \return A list of type objects.
         */
        std::vector<type> get_derived_classes() const;

        /////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////

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
         * \brief Creates an instance of the given type, with the given arguments for the constructor.
         *
         * \remark When the arguments does not match the parameter list of the constructor then he will not be invoked.
         * 
         * \return Returns an instance of the given type.
         */
        variant create(std::vector<detail::argument> args) const;

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
        property get_property(const std::string& name) const;

        /*!
         * \brief Returns a list of all registered properties for this type and
         *        all its base classes.
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
        static property get_global_property(const std::string& name);

        /*!
         * \brief Returns a list of all registered global properties.
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
        variant get_property_value(const std::string& name, detail::instance obj) const;

        /*!
         * \brief Returns the property value of property named \p name.
         *
         * \return A variant containing the value of the property.
         */
        static variant get_property_value(const std::string& name);

        /*!
         * \brief This function will set the given value \p arg to a property named \p name to the instance \p obj.
         *
         * \remark When the given instance is empty, the value of a global property with name \p name will be tryed to set.
         *
         * \return A bool value, which is true, when the value could be set, otherwise false.
         */
        bool set_property_value(const std::string& name, detail::instance obj, detail::argument arg) const;

        /*!
         * \brief This function will set the given value \p arg to a property named \p name.
         *
         * \return A bool value, which is true, when the value could be set, otherwise false.
         */
        static bool set_property_value(const std::string& name, detail::argument arg);


        /*!
         * \brief Returns a method with the name \p name.
         *
         * \remark When there exists no method with the name \p name, then an invalid method is returned.
         *
         * \return A method with name \p name.
         */
        method get_method(const std::string& name) const;

        /*!
         * \brief Returns a method with the name \p name which match the given parameter list \p params.
         *
         * \remark When there exists no method with the name \p name and matching parameter list \p params,
         *         then an invalid method is returned.
         *
         * \return A method with name \p name.
         */
        method get_method(const std::string& name, const std::vector<type>& params) const;
        
        /*!
         * \brief Returns a list of all registered methods for this type and
         *        all its base classes.
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
        static method get_global_method(const std::string& name);

        /*!
         * \brief Returns a global method with the name \p name which match the given parameter list \p params.
         *
         * \remark When there exists no method with the name \p name and matching parameter list \p params,
         *         then an invalid method is returned.
         *
         * \return A method with name \p name and parameter signature \p params.
         */
        static method get_global_method(const std::string& name, const std::vector<type>& params);

        /*!
         * \brief Returns a list of all registered global methods.
         *
         * \return A vector with methods.
         */
        static std::vector<method> get_global_methods();


        /*!
         * \brief Invokes the method represented by the current instance \p object.
         *
         * \remark When it's a static method you still need to provide an instance object, use therefore the function `empty_instance()`.
         *
         * \return A variant object containing the possible return value, 
         *         otherwise when it is a void function an empty but valid variant object.
         */
        variant invoke(const std::string& name, detail::instance obj, std::vector<detail::argument> args) const;

        /*!
         * \brief Invokes a global method named \p name with the specified argument \p args.
         *
         * \return A variant object containing the possible return value, 
         *         otherwise when it is a void function an empty but valid variant object.
         */
        static variant invoke(const std::string& name, std::vector<detail::argument> args);

        /*!
         * \brief Register a converter func `F`.
         *        This function converts a source Type to a target type.
         *        The signature of this function has to be the following: <TargetType (SourceType, bool& ok)>
         * e.g.:
            \code{.cpp}
             std::string conv_func(int value, bool& ok)
             {
                std::string result = std::to_string(value);
                ok = true;
                return result;
             }
             //...
             type::register_converter_func(conv_func);
            \endcode
         */
        template<typename F>
        static void register_converter_func(F func);

    private:

        /*!
         * Constructs an empty and invalid type object.
         */
        type();

        /*!
         * \brief Constructs a valid type object.
         *
         * \param id The unique id of the data type.
         */
        type(type_id id);

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
        detail::type_converter_base* get_type_converter(const type& target_type) const;

        /*!
         * \brief Returns the amount of pointers in the type. E.g. (`int` will return `0`; `int*` will return `1`; `int**` will return `2`; etc...)
         *
         * \return The pointer count.
         */
        std::size_t get_pointer_count() const;


        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////

        /*!
         * \brief Register the type info for the given name
         *
         * \remark When a type with the given name is already registered,
         *         then the type for the already registered type will be returned.
         *
         * \return A valid type object.
         */
        static type register_type(const char* name, 
                                  const type& raw_type,
                                  std::vector<detail::base_class_info> base_classes, 
                                  detail::derived_info(*get_derived_func)(void*),
                                  detail::variant_create_func var_func_ptr,
                                  bool is_class,
                                  bool is_enum,
                                  bool is_array,
                                  bool is_pointer,
                                  bool is_primitive,
                                  bool is_function_pointer,
                                  bool is_member_object_pointer,
                                  bool is_member_function_pointer,
                                  std::size_t get_pointer_count);

        void register_type_converter(std::unique_ptr<detail::type_converter_base> converter) const;

        //! Creates a variant from the given argument data.
        variant create_variant(const detail::argument& data) const;

        template<typename T, typename Enable>
        friend struct impl::MetaTypeInfo;

        template<typename T>
        friend class class_;

        friend type impl::get_invalid_type();
        friend class variant;

        friend class detail::instance;
        
        template<typename TargetType, typename SourceType>
        friend TargetType rttr_cast(SourceType object);

    private:
        type_id  m_id;
};

} // end namespace rttr

#include "rttr/impl/type_impl.h"

#endif // RTTR_TYPE_H_
