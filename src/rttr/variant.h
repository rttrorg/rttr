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

#ifndef __RTTR_VARIANT_H__
#define __RTTR_VARIANT_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/type.h"

#include <type_traits>
#include <cstddef>
#include <algorithm>

namespace rttr
{

class variant_array;

namespace detail
{
    RTTR_LOCAL variant create_void_variant();
    class argument;
    class instance;
    class array_container_base;
}

/*!
 * The variant class allows to store data of any type and convert between these types transparently.
 * 
 * This class serves as container for any given single type. It can hold one value at a time
 * (using containers you can hold multiple types e.g. `std::vector<int>`). Remark that the content is copied
 * into the variant class. Even raw arrays (e.g. `int[10]`) are copied.
 *
 * In order to use this class with your custom `Type`, you have to register it with \ref #RTTR_DECLARE_TYPE(Type).
 * This class is mainly used for returning values from calls. See et.al. \ref property::get_value() or \ref method::invoke().
 *
 *
 * Copying and Assignment
 * ----------------------
 * A \ref variant object is can be copied and assigned, however each copy will perform a copy of the contained value.
 *
 * Typical Usage
 * ----------------------
 * 
\code{.cpp}
    variant var;
    var = 23;                               // copy integer
    int x = var.to_int();                   // x = 23

    var = std::string("Hello World");       // variant contains now a std::string
    var = "Hello World";                    // variant contains now a char[12] array
    int y = var.to_int();                   // y = 0, because invalid conversion
    std::string text = var.to_string();     // text = "Hello World", char array to string converted

    var = "42";                             // contains now char[3] array
    std::cout << var.to_int();              // convert char array to integer and prints "42"

    int my_array[100];
    var = my_array;                         // copies the content of my_array into var
    auto& arr = var.get_value<int[100]>();  // extracts the content of var by reference
\endcode
 *
 * 
 * It's of course also possible to store own custom types in a variant,
 * take a look at following code:
 *
\code{.cpp}
    struct custom_type
    {
        //...
    };

    variant var = custom_type(...);
    bool b = var.is_type<custom_type>();                // b = true
    custom_type& value = var.get_value<custom_type>();  // extracts the value by reference
\endcode
 *
 * The variant class allows to convert also custom types,
 * therefore you have to register a convert function:
 *
\code{.cpp}
    std::string converter_func(const custom_type& value, bool& ok)
    {
        ok = true;
        // convert value to std::string
        return std::string(...);
    }
    
    //...
    variant var = custom_type(...);
    var.can_convert<std::string>();     // return false
    // will register a convert from custom_type to std::string
    type::register_converter_func(converter_func);

    var.can_convert<std::string>();     // return true
    var.to_string();                    // converts from custom_type to std::string
\endcode
 *
 * \see variant_array
 */
class RTTR_API variant
{
    public:
        /*!
         * \brief Constructs an invalid variant. That means a valid which contains no data.
         *  
         * \see is_valid()
         */
        variant();
       
        /*!
         * \brief Constructs a new variant with the given value \p param of type \p T.
         *        The value will be copied into the variant.
         */
        template<typename T>
        variant(const T& param);

        /*!
         * \brief Constructs a new variant with the new value \p param.
         *        The value will be moved into the variant.
         */
        template<typename T>
        variant(T&& param 
#ifndef DOXYGEN
                , typename std::enable_if<!std::is_same<variant&, T>::value >::type* = 0
                , typename std::enable_if<!std::is_const<T>::value >::type* = 0
#endif
                );

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
        ~variant();

        /*!
         * \brief Swaps the content of this variant with the \p other variant.
         */
        void swap(variant& other);

        /*!
         * Assigns the value of the \p other object to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        template<typename T>
        variant& operator=(T&& other);
       
        /*!
         * Assigns the value of the \a other variant to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        variant& operator=(variant&& other);

        /*!
         * Assigns the value of the \a other variant to this variant.
         *
         * \return A reference to the variant with the new data.
         */
        variant& operator=(const variant& other);
        
        /*!
         * Returns true if this variant data is of the given template type \a T.
         * 
         * \return True if variant is the same like \a T, otherwise false.
         */
        template<typename T>
        bool is_type() const;

        /*!
         * \brief Returns the type object of underlying data type.
         *
         * \remark When the variant has not stored any data, then an invalid type object is returned.
         * 
         * \return type of the underlying data type.
         */
        type get_type() const;

        /*!
         * Returns true if this variant is valid, that means the variant is holding some data.
         *
         * When the variant doesn't hold any data it will return false.
         * 
         * \return True if this variant is valid, otherwise false.
         */
        bool is_valid() const;

        /*!
         * \brief Returns true if the contained value can be converted to the given type \p T.
         *        Otherwise false.
         * 
         * \return True if this variant can be converted to \p T; otherwise false.
         */
        template<typename T>
        bool can_convert() const;

        /*!
         * \brief Returns true if the contained value can be converted to the given type \p target_type;
         *        otherwise false.
         * 
         * \return True if this variant can be converted to \p target_type; otherwise false.
         */
        bool can_convert(const type& target_type) const;

        /*!
         * \brief Converts the containing variant internally to the given type \p target_type.
         *        When the conversion was successfully the function will return true. 
         *        When the conversion fails, then the containing variant value stays the same and the function will return false.
         *
         *        A variant containing a pointer to a custom type will also convert and return true
         *        for this function if a \ref rttr_cast to the type described by \p target_type would succeed.
         *
         * \see can_convert()
         * 
         * \return True if this variant can be converted to \p target_type; otherwise false.
         */
        bool convert(const type& target_type);

        /*!
         * \brief Returns a reference to the containing value as type \p T.
         *
         * \remark Only call this method when it is possible to return the containing value as the given type \p T.
         *         Use therefore the method is_type().
         *         Otherwise the call leads to undefined behaviour.
         *         Make sure you don't clean this variant, when you still hold a reference to the containing value.
         *
         * \see is_type()
         *
         * \return A reference to the stored value.
         */
        template<typename T>
        T& get_value() const;


        /*!
         * \brief Converts the containing value to type \p T and returns the value.
         *        If \a ok is non-null: \a *ok is set to true if the value could be converted to an \p T; otherwise \a *ok is set to false.
         *
         *
         * \remark Only call this method when it is possible to return the containing value as type \p T.
         *         Use therefore the method can_convert().
         *         Otherwise the call leads to undefined behaviour.
         *
         * \see can_convert()
         *
         * \return The converted value as type \p T.
         */
         template<typename T>
         T convert(bool* ok = nullptr) const;

        /*!
         * \brief Returns the containing variant as \p int value when the type is an integer,
         *        or when a conversion function for the underlying type to \p int was registered;
         *        otherwise returns 0.
         *
         *        If \a ok is non-null: \a *ok is set to true if the value could be converted to an \p int; otherwise \a *ok is set to false.
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
         * \brief Creates a valid variant_array object from the underlying value when the containing type 
         *        is an \ref type::is_array() "array" or it contains a pointer to an array type.
         *
         * A typical example is the following:
         *
         * \code{.cpp}
         *   int obj_array[100];
         *   variant var = obj_array;              // copies the content of obj_array into var
         *   variant_array array = var.to_array(); // Copies the content of var to a variant_array object
         *   auto x = array.get_size();            // set x to 100
         *   array.set_value(0, 42);               // set the first index to the value 42
         * \endcode
         *
         * \see can_convert(), convert()
         *
         * \return A variant_array object.
         */
        variant_array to_array() const;


    private:
        template<typename T, typename SourceType>
        static T get_value_with_default_value(const SourceType& source, T default_value, bool* ok);

        /*!
         * \brief Returns a pointer to the underlying data
         *
         * \remark You do not have to use this method directly.
         *
         * \return
         */
        void* get_ptr() const;

        /*!
         * \brief Returns a pointer to the underlying data
         *
         * \remark You do not have to use this method directly.
         *
         * \return
         */
        void* get_raw_ptr() const;

        /*!
         * \brief Returns a pointer to the underlying data
         *
         * \remark You do not have to use this method directly.
         *
         * \return
         */
        type get_raw_type() const;

        class variant_container_base
        {
            public:
                virtual ~variant_container_base();

                virtual type get_type() const = 0;
                
                virtual void* get_ptr() const = 0;

                virtual type get_raw_type() const = 0;
                
                virtual void* get_raw_ptr() const = 0;

                virtual bool is_array() const = 0;

                virtual detail::array_container_base* to_array() const = 0;

                virtual std::string to_string(bool* ok) const = 0;
                virtual int to_int(bool* ok) const = 0;
                virtual bool to_bool(bool* ok) const = 0;
                virtual float to_float(bool* ok) const = 0;
                virtual double to_double(bool* ok) const = 0;

                std::size_t to_size_t(bool* ok) const;

                virtual variant_container_base* clone() const = 0;

                virtual bool can_convert(const type& target_type) const = 0;
        };

        template<typename T, typename Enable = void>
        class variant_container : public variant_container_base
        {
            public:
                variant_container(const T& arg);

                variant_container(T&& arg);

                variant_container_base* clone() const;

                type get_type() const;

                void* get_ptr() const;

                type get_raw_type() const;
                
                void* get_raw_ptr() const;

                bool is_array() const;

                detail::array_container_base* to_array() const;
                
                bool can_convert(const type& target_type) const;

                std::string to_string(bool* ok) const;
                int to_int(bool* ok) const;
                bool to_bool(bool* ok) const;
                float to_float(bool* ok) const;
                double to_double(bool* ok) const;

                T _value; // the stored data

            private: // unimplemented
                variant_container & operator=(const variant_container &);
        };

        template<typename T>
        class variant_container<T, typename std::enable_if<detail::is_array_and_not_one_dim_char_array<T>::value>::type> : public variant_container_base
        {
            public:
                variant_container(const T& arg);

                variant_container_base* clone() const;

                type get_type() const;

                void* get_ptr() const;

                type get_raw_type() const;
                
                void* get_raw_ptr() const;

                bool can_convert(const type& target_type) const;

                bool is_array() const;

                detail::array_container_base* to_array() const;

                std::string to_string(bool* ok) const;
                int to_int(bool* ok) const;
                bool to_bool(bool* ok) const;
                float to_float(bool* ok) const;
                double to_double(bool* ok) const;

                T _value; // the stored data

            private: // unimplemented
                variant_container & operator=(const variant_container &);
        };

        template<std::size_t N>
        class variant_container<char[N]> : public variant_container_base
        {
            public:
                variant_container(const char (&arg)[N]);

                variant_container_base* clone() const;

                type get_type() const;

                void* get_ptr() const;

                type get_raw_type() const;
                
                void* get_raw_ptr() const;

                bool can_convert(const type& target_type) const;

                bool is_array() const;

                detail::array_container_base* to_array() const;

                std::string to_string(bool* ok) const;
                int to_int(bool* ok) const;
                bool to_bool(bool* ok) const;
                float to_float(bool* ok) const;
                double to_double(bool* ok) const;

                char _value[N]; // the stored data

            private: // unimplemented
                variant_container & operator=(const variant_container &);
        };

    private:
        friend variant detail::create_void_variant();
        friend detail::argument;
        friend detail::instance;
        variant_container_base*         _holder;
};

#ifndef DOXYGEN
template<> RTTR_API std::string variant::convert<std::string>(bool* ok) const;
template<> RTTR_API int variant::convert<int>(bool* ok) const;
template<> RTTR_API bool variant::convert<bool>(bool* ok) const;
template<> RTTR_API float variant::convert<float>(bool* ok) const;
template<> RTTR_API double variant::convert<double>(bool* ok) const;
template<> RTTR_API variant_array variant::convert<variant_array>(bool* ok) const;

template<> RTTR_API bool variant::can_convert<variant_array>() const;
#endif

namespace detail
{
    RTTR_API extern variant void_variant;
} // end namespace impl

} // end namespace rttr

#include "rttr/impl/variant_impl.h"
#include "rttr/impl/variant_default_types_impl.h"

#endif // __RTTR_VARIANT_H__
