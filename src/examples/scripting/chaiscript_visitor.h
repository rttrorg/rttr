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

#ifndef CHAISCRIPT_VISITOR_H_
#define CHAISCRIPT_VISITOR_H_

#include <rttr/visitor.h>

#include <chaiscript/dispatchkit/function_call.hpp>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

class chaiscript_visitor : public rttr::visitor
{
public:
    chaiscript_visitor(chaiscript::ChaiScript& chai)
    :   m_chai(chai)
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
        m_chai.add(chaiscript::base_class<Base_Class, Derived>());
        iterate_base_classes<Derived, Base_Classes...>();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, typename...Base_Classes>
    void visit_type_begin(const type_info<T>& info)
    {
        using declaring_type_t = typename type_info<T>::declaring_type;
        iterate_base_classes<declaring_type_t, Base_Classes...>();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, typename...Ctor_Args>
    void visit_constructor(const constructor_info<T>& info)
    {
        using declaring_type_t = typename constructor_info<T>::declaring_type;
        m_chai.add(chaiscript::constructor<declaring_type_t(Ctor_Args...)>(), get_type_name<declaring_type_t>());
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_global_method(const method_info<T>& info)
    {
        m_chai.add(chaiscript::fun(info.function_ptr), info.method_item.get_name().to_string());
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_method(const method_info<T>& info)
    {
        m_chai.add(chaiscript::fun(info.function_ptr), info.method_item.get_name().to_string());
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void visit_property(const property_info<T>& info)
    {
        m_chai.add(chaiscript::fun(info.property_accessor), info.property_item.get_name().to_string());
    }

    template<typename T>
    void visit_getter_setter_property(const property_getter_setter_info<T>& info)
    {
        m_chai.add(chaiscript::fun(info.property_getter), std::string("get_") + info.property_item.get_name().to_string());
        m_chai.add(chaiscript::fun(info.property_setter), std::string("set_") + info.property_item.get_name().to_string());
    }

    template<typename T>
    void visit_readonly_property(const property_info<T>& info)
    {
        m_chai.add(chaiscript::fun(info.property_accessor), info.property_item.get_name().to_string());
    }

    /////////////////////////////////////////////////////////////////////////////////////

private:
    template<typename T>
    static std::string get_type_name()
    {
        return rttr::type::template get<T>().get_name().to_string();
    }

private:
    chaiscript::ChaiScript& m_chai;

    RTTR_ENABLE(visitor) // Important!! Otherwise the object instance cannot be casted from "visitor" to "chai_script_binding_visitor"
};


RTTR_REGISTER_VISITOR(chaiscript_visitor); // Important!!
                                           // In order to make the visitor available during registration

#endif  // CHAISCRIPT_VISITOR_H_
