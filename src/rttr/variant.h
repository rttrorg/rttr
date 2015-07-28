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

#ifndef RTTR_VARIANT_H_
#define RTTR_VARIANT_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/variant/variant_data.h"
#include "rttr/detail/variant_array_view/variant_array_view_data.h"
#include "rttr/detail/misc/argument_wrapper.h"

#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace rttr
{

class variant_array_view;
class type;
class variant;

namespace detail
{
    class argument;
    class instance;
    struct data_address_container;
    enum class variant_policy_operation : uint8_t;

    template<typename T, typename Decayed = decay_t<T>>
    using decay_variant_t = typename std::enable_if<!std::is_same<Decayed, variant>::value &&
                                                    !std::is_same<Decayed, variant_array_view>::value, Decayed>::type;

    typedef bool (*variant_policy_func)(variant_policy_operation, const variant_data&, argument_wrapper);
}

/*!
 * The \ref variant class allows to store data of any type and convert between these types transparently.
 * 
 * This class serves as container for any given single \ref get_type() "type". It can hold one value at a time
 * (using containers you can hold multiple types e.g. `std::vector<int>`). Remark that the content is copied
 * into the variant class. Even raw arrays (e.g. `int[10]`) are copied. However, the internal implementation of variant
 * has an optimization for storing small types, which avoid heap allocation.
 *
 * The main purpose of this class is to be the return value for \ref property::get_value() "property" and 
 * \ref method::invoke() "method" invokes or as container for storing meta data.
 *
 * Copying and Assignment
 * ----------------------
 * A \ref variant object can be copied and assigned, however each copy will perform a copy of the contained value.
 *
 * Typical Usage
 * -------------
 * 
 * \code{.cpp}
 *  variant var;
 *  var = 23;                               // copy integer
 *  int x = var.to_int();                   // x = 23
 *  
 *  var = "Hello World";                    // var contains now a std::string (implicit conversion of string literals to std::string)
 *  int y = var.to_int();                   // y = 0, because invalid conversion
 *  
 *  var = "42";                             // contains a std::string
 *  std::cout << var.to_int();              // convert std::string to integer and prints "42"
 *  
 *  int my_array[100];
 *  var = my_array;                         // copies the content of my_array into var
 *  auto& arr = var.get_value<int[100]>();  // extracts the content of var by reference
 * \endcode
 *
 * Extract Value
 * -------------
 * For extracting a value out of a variant you can use the \ref get_value() function.
 * This will return a *const reference* to the contained value.
 * However, you must instantiated this function with the exact type of the stored value,
 * otherwise undefined behaviour will occur.
 * Therefore you **should** check it's \ref get_type() "type" before extracting with \ref is_type<T>().
 *
 * See following example:
 * \code{.cpp}
 *  struct custom_type
 *  {
 *     //...
 *  };
 *  
 *  variant var = custom_type{};
 *  if (var.is_type<custom_type>())                             // yields to true
 *    const custom_type& value = var.get_value<custom_type>();  // extracts the value by reference
 * \endcode
 *
 * Conversion
 * ----------
 * The \ref variant class offers three possibilities to convert to a new type.
 * - \ref convert(const type& target_type) "convert(const type& target_type)" - convert the variant internally to a new type
 * - \ref variant::convert_to<T>(bool* ok) "convert_to\<T\>(bool *ok)" - convert the contained value to an internally default created value of type \p T
 * - \ref variant::convert_to<T>(T& value) "convert_to\<T\>(T& value)" - convert the contained value to a given \p value of type \p T
 *
 * See following example code:
 * \code{.cpp}
 *  variant var = 500;                          // var contains an int
 *  if(var.can_convert<std::string>())          // check whether conversion is possible
 *  {
 *    var.convert(type::get<std::string>());    // var contains now a std::string, with value => "23"
 *    var.is_type<std::string>();               // yields to 'true'
 *  }
 *
 *  bool ok = false;
 *  int int_value = var.convert<int>(&ok);      // int_value == 23, ok yields to 'true'
 *  // or
 *  uint8_t small_int = 0;
 *  bool result = var.convert<uint8_t>(small_int); // result == false, because small_int cannot hold the value '500'
 * 
 * \endcode
 *
 * \remark
 * It is possible that \ref can_convert() will return `true`, but \ref convert() will actually fail and return `false`.
 * The reason for this is \ref can_convert() will return the general ability of converting between types given suitable data; 
 * when no suitable data is given, the conversion cannot be performed.
 *
 * A good example is the conversion from `std::string` to `int`.
 * \code{.cpp}
 *  variant var = "Answer: 42";
 *  var.can_convert<int>();             // yields to 'true'
 *  int number;
 *  bool result = var.convert(number);  // yields to 'false', because the string contains non-numeric characters
 *
 * \endcode
 *
 * Hence, it is important to have both functions return true for a successful conversion.
 *
 * Custom Converter
 * ----------------
 * The variant class allows to convert from and to user-defined types, 
 * therefore you have to register a conversion function.
 *
 * See following example code:
 * \code{.cpp}
 *  std::string converter_func(const custom_type& value, bool& ok)
 *  {
 *      ok = true;
 *      // convert value to std::string
 *      return std::string(...);
 *  }
 *  
 *  //...
 *  variant var = custom_type(...);
 *  var.can_convert<std::string>();     // return 'false'
 *  // register the conversion function
 *  type::register_converter_func(converter_func);
 *  
 *  var.can_convert<std::string>();     // return 'true'
 *  var.to_string();                    // converts from 'custom_type' to 'std::string'
 * \endcode
 *
 * For more information see \ref type::register_converter_func()
 *
 * \see variant_array_view
 */
class RTTR_API variant
{
    public:
        /*!
         * \brief Constructs an invalid variant. That means a valid which contains no data.
         *  
         * \see is_valid()
         */
        RTTR_INLINE variant();

        /*!
         * \brief Constructs a new variant with the new value \p val.
         *        The value will be copied or moved into the variant.
         */
        template<typename T, typename Tp = detail::decay_variant_t<T>>
        variant(T&& val);

        /*!
         * \brief Constructs a new variant object from the given variant \p other.
         */
        variant(const variant& other);

        /*!
         * \brief Constructs a new variant via move constructor.
         */
        variant(variant&& other);

        /*!
         * \brief Destroys the variant and the contained object.
         */
        RTTR_INLINE ~variant();

        /*!
         * Assigns the value of the \p other object to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        template<typename T, typename Tp = detail::decay_variant_t<T>>
        variant& operator=(T&& other);

        /*!
         * \brief Assigns the value of the \a other variant to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        variant& operator=(variant&& other);

        /*!
         * \brief Assigns the value of the \a other variant to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        variant& operator=(const variant& other);

        /*!
         * \brief Compares this variant with \p other and returns `true` if they are equal; otherwise returns `false`.
         * 
         * The variant uses the equality operator of the containing \ref get_type() "type" to check for equality.
         * When \p other is not of the same type as the containing type, it will try to convert to it and do then the equality check.
         *
         * \see bool operator!=(const variant&) const;
         *
         * \return A boolean with value `true`, that indicates both variant's are equal, otherwise `false`.
         */
        bool operator==(const variant& other) const;

        /*!
         * \brief Compares this variant with \p other and returns `true` if they are **not** equal; otherwise returns `false`.
         *
         * \see bool operator==(const variant&) const;
         *
         * \return A boolean with value `true`, that indicates both variant's are **not** equal, otherwise `false`.
         */
        bool operator!=(const variant& other) const;

        /*!
         * \brief Compares this variant with \p other and returns `true` if this is less than \p other, otherwise returns `false`.
         * 
         * The variant uses the less than operator of the containing \ref get_type() "type".
         * When \p other is not of the same type as the containing type, it will try to convert to it and do then the less-than check.
         *
         * \see bool operator!=(const variant&) const;
         *
         * \return A boolean with value `true`, that indicates that this variant is less than the other, otherwise `false`.
         */
        bool operator<(const variant& other) const;
        bool operator>(const variant& other) const;
        
        bool operator<=(const variant& other) const;
        bool operator>=(const variant& other) const;

        /*!
         * \brief When the variant contains a value, then this function will clear the content.
         *
         * \remark After calling this function \ref is_valid() will return `false`.
         */
        void clear();

        /*!
         * \brief Swaps the content of this variant with \p other variant.
         */
        void swap(variant& other);
        
        /*!
         * \brief Returns `true` if the containing variant data is of the given template type `T`.
         * 
         * \return True if variant data is of type `T`, otherwise false.
         */
        template<typename T>
        bool is_type() const;

        /*!
         * \brief Returns the \ref type object of underlying data.
         *
         * \remark When the variant has not stored any data, then an invalid \ref type object is returned.
         * 
         * \return \ref type of the underlying data type.
         */
        type get_type() const;

        /*!
         * \brief Returns true if this variant is valid, that means the variant is holding some data.
         *
         * When the variant doesn't hold any data it will return false.
         *
         * \remark A variant can also hold `void` data, this is used to indicate that a method call,
         *         which has no return value, was successfully. In this case, there is no data actually stored,
         *         but this function will return true.
         * 
         * \return `True` if this variant is valid, otherwise `false`.
         */
        bool is_valid() const;

        /*!
         * \brief Convenience function to check if this \ref variant is valid or not.
         *
         * \see is_valid()
         *
         * \return True if this \ref variant is valid, otherwise false.
         */
        explicit operator bool() const;

        /*!
         * \brief When the \ref variant::get_type "type" or its \ref type::get_raw_type() "raw type" 
         *        or the \ref type::get_wrapped_type() "wrapped type" is an \ref type::is_array() "array",
         *        then this function will return true, otherwise false.
         * 
         * \return True if the containing value is an array; otherwise false.
         */
        bool is_array() const;

        /*!
         * \brief Returns a reference to the containing value as type \p T.
         *
         * \code{.cpp}
         *  struct custom_type
         *  {
         *     //...
         *  };
         *  
         *  variant var = custom_type{};
         *  if (var.is_type<custom_type>())                             // yields to true
         *    const custom_type& value = var.get_value<custom_type>();  // extracts the value by reference
         * \endcode
         *
         * \remark Only call this method when it is possible to return the containing value as the given type \p T.
         *         Use therefore the method \ref is_type().
         *         Otherwise the call leads to undefined behaviour.
         *         Also make sure you don't clean this variant, when you still hold a reference to the containing value.
         *
         * \see is_type()
         *
         * \return A reference to the stored value.
         */
        template<typename T>
        T& get_value();

        /*!
         * \brief Returns `true` if the contained value can be converted to the given type \p T.
         *        Otherwise `false`.
         * 
         * \return `True` if this variant can be converted to `T`; otherwise `false`.
         */
        template<typename T>
        bool can_convert() const;

        /*!
         * \brief Returns `true` if the contained value can be converted to the given type \p target_type;
         *        otherwise `false`.
         *
         * The returned value indicates that a conversion is in general possible.
         * However a conversion might still fail when doing the actual conversion with \ref convert().
         * An example is the conversion from a string to a number. 
         * When the string does not contain non-numeric characters, the conversion will not succeed.
         * 
         * \return `True` if this variant can be converted to \p target_type; otherwise `false`.
         */
        bool can_convert(const type& target_type) const;

        /*!
         * \brief Converts the containing variant internally to the given type \p target_type.
         *        When the conversion was successfully the function will return `true`. 
         *        When the conversion fails, then the containing variant value stays the same and the function will return `false`.
         *
         * A variant containing a pointer to a custom type will be also converted and return `true`
         * for this function, if a \ref rttr_cast to the type described by \p target_type would succeed.
         *
         * See therefore following example code:
         * \code{.cpp}
         *  struct base { virtual ~base(){} };
         *  struct derived : base { };
         *  derived d;
         *  variant var = static_cast<base*>(&d);           // var contains a '*base' ptr
         *  bool ret = var.convert(type::get<derived*>());  // yields to 'true'
         *  var.is_type<derived*>();                        // yields to 'true'
         * \endcode
         *
         * \see can_convert()
         * 
         * \return `True` if this variant can be converted to \p target_type; otherwise `false`.
         */
        bool convert(const type& target_type);

        /*!
         * \brief Converts the containing data to a *new value* of type \p T and return this *value*.
         *        If \p ok is non-null: \p *ok is set to `true` when the value was successfully converted to \p T; otherwise \p *ok is set to `false`.
         *        The type \p T must meet the requirement to be *default constructible*.
         *
         * \code{.cpp}
         *  variant val = 12;
         *  if (val.can_convert<float>())
         *  {
         *    bool ok = false;
         *    float result = val.convert<float>(&ok);
         *    if (ok)
         *    // ...
         *  }
         * \endcode
         *
         * \remark Before doing the conversion you should check whether it is in general possible to convert to type \p T
         *         with the function \ref can_convert(). When the conversion fails, a default constructed value of type \p T is returned.
         *
         * \see can_convert()
         *
         * \return The converted value as type \p T.
         */
        template<typename T>
        T convert(bool* ok = nullptr) const;

        /*!
         * \brief Converts the containing data to the given value \p value and returns a `bool` flag that indicated whether the conversion
         *        was successful or not.
         *
         * When you need to convert to a type which cannot be default constructed use this function.
         *
         * See following example code:
         * \code{.cpp}
         *  variant val = 12;
         *  if (val.can_convert<custom_type>())
         *  {
         *    custom_type obj(42); // non-default ctor type
         *    bool result = val.convert(obj);
         *    if (result)
         *    // ...
         *  }
         * \endcode
         *
         * \remark Before doing the conversion you should check whether it is in general possible to convert to type \p T
         *         with the function \ref can_convert()
         *
         * \see can_convert()
         *
         * \return `True` if the contained data could be converted to \p value; otherwise `false`.
         */
        template<typename T>
        bool convert(T& value) const;

        /*!
         * \brief Returns the containing variant as `int` value when the type is an integer,
         *        or when a conversion function for the underlying type to `int` was registered;
         *        otherwise returns `0`.
         *
         *        If \p ok is non-null: \p *ok is set to `true` if the value could be converted to an `int`; otherwise \p *ok is set to `false`.
         *
         * \see can_convert(), is_type()
         *
         * \return An integer value.
         */
        int to_int(bool *ok = nullptr) const;

        /*!
         * \brief Returns the containing variant as \p std::string when the type is an \p std::string,
         *        or when a conversion function for the underlying type to \p std::string was registered;
         *        otherwise returns an empty default constructed \p std::string object is returned.
         *
         *        If \a ok is non-null: \a *ok is set to true if the value could be converted to an \p std::string; otherwise \a *ok is set to false.
         *
         * \see can_convert(), is_type()
         *
         * \return An std::string value.
         */
        std::string to_string(bool *ok = nullptr) const;

        /*!
         * \brief Returns the variant as a \p bool if the variant has is_type() bool.
         *
         *        Returns true if the variant is of type \p int, \p bool, \p float, \p double and the value is non-zero.
         *        or if the variant has type `std::string` or `char[]` and its lower-case content is not one of the following:
         *        empty, \p "0" or \p "false"; otherwise returns false.
         *
         *        When the type is a custom type and a conversion function to \p bool was registered,
         *        then this call will try to convert the value to \p bool.
         *
         * \see can_convert(), is_type()
         *
         * \return A bool value.
         */
        bool to_bool() const;

        /*!
         * \brief Returns the containing variant as \p float value when the type is a \p float,
         *        or when a conversion function for the underlying type to \p float was registered;
         *        otherwise returns 0.
         *
         *        If \a ok is non-null: \a *ok is set to true if the value could be converted to an \p float; otherwise \a *ok is set to false.
         *
         * \see can_convert(), is_type()
         *
         * \return An float value.
         */
        float to_float(bool* ok = nullptr);

        /*!
         * \brief Returns the containing variant as \p double value when the type is a \p double,
         *        or when a conversion function for the underlying type to \p double was registered;
         *        otherwise returns 0.
         *
         *        If \a ok is non-null: \a *ok is set to true if the value could be converted to an \p double; otherwise \a *ok is set to false.
         *
         * \see can_convert(), is_type()
         *
         * \return An double value.
         */
        double to_double(bool* ok = nullptr);

        /*!
         * \brief Creates a \ref variant_array_view from the containing value,
         *        when the \ref variant::get_type "type" or its \ref type::get_raw_type() "raw type" 
         *        or the \ref type::get_wrapped_type() "wrapped type" is an \ref type::is_array() "array".
         *        Otherwise a default constructed variant_array_view will be returned.
         *        For shorten this check, use the function \ref is_array().
         *
         * A typical example is the following:
         *
         * \code{.cpp}
         *   int obj_array[100];
         *   variant var = obj_array;                            // copies the content of obj_array into var
         *   variant_array_view array = var.create_array_view(); // copies the content of var to a variant_array object
         *   auto x = array.get_size();                          // set x to 100
         *   array.set_value(0, 42);                             // set the first index to the value 42
         * \endcode
         *
         * \see can_convert(), convert()
         *
         * \remark This function will return an \ref variant_array_view::is_valid() "invalid" object, when the \ref variant::get_type "type" is no array.
         *
         * \return A variant_array_view object.
         */
        variant_array_view create_array_view() const;

    private:
        /////////////////////////////////////////////////////////////////////////////////


        /*!
         * \brief Returns a pointer to the underlying data
         *
         * \remark You do not have to use this method directly.
         *
         * \return
         */
        RTTR_INLINE void* get_ptr() const;

         /*!
         * \brief Returns the type object of the underlying data
         *
         * \remark You do not have to use this method directly.
         *
         * \return Type object.
         */
        RTTR_INLINE type get_raw_type() const;

        /*!
         * \brief Returns a pointer to the underlying data.
         *        This will return 
         *
         * \remark You do not have to use this method directly.
         *
         * \return Raw void pointer.
         */
        RTTR_INLINE void* get_raw_ptr() const;

        /*!
         * \brief Returns a pointer to the underlying object pointer wrapped in a smart_ptr.
         *
         * \remark You do not have to use this method directly.
         *
         * \return Type object of the wrapped pointer object.
         */
        RTTR_INLINE detail::data_address_container get_data_address_container() const;

        /*!
         * \brief Converts the containing data to the given target type \p T.
         *
         * \remark The containing data will not be removed,
         *         instead a copy with the new target type will be returned.
         *
         * \return The converted type T. When the conversion failed, the argument \p ok will be set to false.
         */
        template<typename T>
        T convert_to_basic_type(bool* ok) const;

        /*!
         * \brief Tries to convert the internal type to the given type \p to.
         *
         * \return True, when the conversion was successful, otherwise false.
         */
        template<typename T>
        bool convert_to_basic_type(T& to) const;

        /*!
         * \brief Compares the containing and the given variant \p other for equality.
         *
         * \return A boolean with value `true`, that indicates both variant's are equal, otherwise `false`.
         */
        bool compare_equal(const variant& other) const;

    private:
        friend detail::argument;
        friend detail::instance;

        detail::variant_data            m_data;
        detail::variant_policy_func     m_policy;
};

#ifndef DOXYGEN
    template<> RTTR_API std::string variant::convert<std::string>(bool* ok) const;
    template<> RTTR_API int variant::convert<int>(bool* ok) const;
    template<> RTTR_API bool variant::convert<bool>(bool* ok) const;
    template<> RTTR_API float variant::convert<float>(bool* ok) const;
    template<> RTTR_API double variant::convert<double>(bool* ok) const;
#endif

} // end namespace rttr

#include "rttr/detail/variant/variant_impl.h"

#endif // RTTR_VARIANT_H_
