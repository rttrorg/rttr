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

#ifndef RTTR_TYPE_REGISTER_P_H_
#define RTTR_TYPE_REGISTER_P_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/flat_multimap.h"

#include "rttr/string_view.h"

#include <memory>
#include <string>
#include <vector>

namespace rttr
{
class type;
class property;
class method;

namespace detail
{

class constructor_wrapper_base;
class destructor_wrapper_base;
class property_wrapper_base;
class method_wrapper_base;
class enumeration_wrapper_base;

struct type_data;

class metadata;

/*!
 * This class contains all logic to register properties, methods etc.. for a specific type.
 * It is not part of the rttr API
 */
class RTTR_LOCAL type_register_private
{
public:

    static void constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor);
    static void destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor);
    static void property(const type& t, std::unique_ptr<property_wrapper_base> prop);
    static void method(const type& t, std::unique_ptr<method_wrapper_base> meth);

    static flat_multimap<string_view, ::rttr::property>& get_global_property_storage();
    static flat_multimap<string_view, ::rttr::method>& get_global_method_storage();

private:
    static ::rttr::property get_type_property(const type& t, string_view name);
    static ::rttr::method get_type_method(const type& t, string_view name);
    static ::rttr::method get_type_method(const type& t, string_view name,
                                          const std::vector<type>& type_list);

    template<typename T>
    static void update_class_list(const type& t, T item_ptr);
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_REGISTER_P_H_
