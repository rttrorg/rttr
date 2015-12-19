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

#ifndef RTTR_TYPE_REGISTER_H_
#define RTTR_TYPE_REGISTER_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <memory>
#include <string>
#include <vector>

namespace rttr
{
class variant;
class type;
class argument;

template<typename T>
class class_;

namespace detail
{

class constructor_wrapper_base;
class destructor_wrapper_base;
class enumeration_wrapper_base;
class method_wrapper_base;
class property_wrapper_base;

struct type_converter_base;
struct type_comparator_base;
struct base_class_info;
struct derived_info;

using variant_create_func   = variant(*)(const argument&);
using get_derived_func      = derived_info(*)(void*);

template<typename T, typename Enable>
struct type_getter;

class metadata;

/*!
 * This class contains all functions to register properties, methods etc.. for a specific type.
 * This is a static pimpl, it will just forward the data to the \ref type_database class.
 */
class RTTR_API type_register
{
public:
    // no copy
    type_register(const type_register&) = delete;
    // no assign
    type_register& operator=(const type_register&) = delete;

    static void property(const type& t, std::unique_ptr<property_wrapper_base> prop);

    static void method(const type& t, std::unique_ptr<method_wrapper_base> meth);

    static void constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor);

    static void destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor);

    static void enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_data);

    static void custom_name(const type& t, std::string );

    static void metadata( const type& t, std::vector<metadata> data);

    static void converter(const type& t, std::unique_ptr<type_converter_base> converter);

    static void comparator(const type& t, type_comparator_base* comparator);

    /*!
     * \brief Register the type info for the given name
     *
     * \remark When a type with the given name is already registered,
     *         then the type for the already registered type will be returned.
     *
     * \return A valid type object.
     */
    static uint16_t type_reg(const char* name, 
                             const type& raw_type,
                             const type& wrapped_type,
                             const type& array_raw_type,
                             std::vector<base_class_info> base_classes, 
                             get_derived_func derived_func_ptr,
                             variant_create_func var_func_ptr,
                             std::size_t type_size,
                             bool is_class,
                             bool is_enum,
                             bool is_array,
                             bool is_pointer,
                             bool is_arithmetic,
                             bool is_function_pointer,
                             bool is_member_object_pointer,
                             bool is_member_function_pointer,
                             std::size_t pointer_dimension);

private:

    friend class type;
    template<typename T>
    friend class class_;

    template<typename T, typename Enable>
    friend struct detail::type_getter;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_REGISTER_H_
