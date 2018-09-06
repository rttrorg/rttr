/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_MY_VISITOR_H_
#define RTTR_MY_VISITOR_H_

#include <rttr/visitor.h>
#include <catch/catch.hpp>

struct visitor_test_class;
struct visitor_test_class_base;

class my_visitor : public rttr::visitor
{
public:
    my_visitor()
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename Derived>
    void iterate_base_classes()
    {
    }

    template<typename Derived, typename Base_Class, typename...Base_Classes>
    void iterate_base_classes()
    {
        if (rttr::type::get<Derived>().get_name() == "visitor_test_class")
        {
            CHECK((std::is_same<Derived, visitor_test_class>::value == true));
            CHECK((std::is_same<Base_Class, visitor_test_class_base>::value == true));
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, typename...Base_Classes>
    void visit_type_begin(const type_info<T>& info)
    {
        using declaring_type_t = typename type_info<T>::declaring_type;
        if (info.type_item.get_name() == "visitor_test_class_base")
        {
            CHECK((std::is_same<declaring_type_t, visitor_test_class_base>::value == true));
        }
        else if (info.type_item.get_name() == "visitor_test_class")
        {
            CHECK((std::is_same<declaring_type_t, visitor_test_class>::value == true));
        }
        iterate_base_classes<declaring_type_t, Base_Classes...>();

        visited_types.push_back(info.type_item);
    }

    template<typename T, typename...Base_Classes>
    void visit_type_end(const type_info<T>& info)
    {
        using declaring_type_t = typename type_info<T>::declaring_type;

        if (info.type_item.get_name() == "visitor_test_class_base")
        {
            CHECK((std::is_same<declaring_type_t, visitor_test_class_base>::value == true));
        }
        else if (info.type_item.get_name() == "visitor_test_class")
        {
            CHECK((std::is_same<declaring_type_t, visitor_test_class>::value == true));
        }

        iterate_base_classes<declaring_type_t, Base_Classes...>();

        visited_types.push_back(info.type_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, typename...Ctor_Args>
    void visit_constructor(const constructor_info<T>& info)
    {
        visited_ctors.push_back(info.ctor_item);
    }

    template<typename T, typename...Ctor_Args>
    void visit_constructor_function(const constructor_function_info<T>& info)
    {
        visited_ctors.push_back(info.ctor_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_global_method(const method_info<T>& info)
    {
        visited_meths.push_back(info.method_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_method(const method_info<T>& info)
    {
        visited_meths.push_back(info.method_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_property(const property_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    template<typename T>
    void visit_getter_setter_property(const property_getter_setter_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_global_property(const property_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    template<typename T>
    void visit_global_getter_setter_property(const property_getter_setter_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_readonly_property(const property_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    template<typename T>
    void visit_global_readonly_property(const property_info<T>& info)
    {
        visited_props.push_back(info.property_item);
    }

    /////////////////////////////////////////////////////////////////////////////////////

public:

    std::vector<rttr::type>             visited_types;
    std::vector<rttr::constructor>      visited_ctors;
    std::vector<rttr::method>           visited_meths;
    std::vector<rttr::property>         visited_props;

    /////////////////////////////////////////////////////////////////////////////////////

    RTTR_ENABLE(visitor)
};


RTTR_REGISTER_VISITOR(my_visitor);

#endif  // RTTR_MY_VISITOR_H_
