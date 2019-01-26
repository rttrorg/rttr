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

#ifndef RTTR_VISITOR_IMPL_H_
#define RTTR_VISITOR_IMPL_H_


namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename...Base_Classes>
void visitor::visit_type_begin(const type_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename...Base_Classes>
void visitor::visit_type_end(const type_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename...Ctor_Args>
void visitor::visit_constructor(const constructor_info<T>& info)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_constructor_function(const constructor_function_info<T>& info)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_method(const method_info<T>& info)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_global_method(const method_info<T>& info)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_property(const property_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_getter_setter_property(const property_getter_setter_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_global_property(const property_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_global_getter_setter_property(const property_getter_setter_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_readonly_property(const property_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void visitor::visit_global_readonly_property(const property_info<T>& info)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Base_Classes>
static visitor::type_info<type_list<T, type_list<Base_Classes...>>> make_type_visitor_info(const type& type_info)
{
    return visitor::type_info<type_list<T, type_list<Base_Classes...>>>{type_info};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Policy, typename Acc>
static visitor::constructor_function_info<type_list<T, Policy, Acc>> make_ctor_info_func(const constructor& ctor, const Acc& acc)
{
    return visitor::constructor_function_info<type_list<T, Policy, Acc>>{ctor, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Policy, typename...Args>
static visitor::constructor_info<type_list<T, Policy, type_list<Args...>>> make_ctor_info(const constructor& ctor)
{
    return visitor::constructor_info<type_list<T, Policy, type_list<Args...>>>{ctor};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Policy, typename Acc>
static visitor::method_info<type_list<T, Policy, Acc>> make_method_info(const method& meth, const Acc& acc)
{
    return visitor::method_info<type_list<T, Policy, Acc>>{meth, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Policy, typename Acc>
static visitor::property_info<type_list<T, Policy, Acc>> make_property_info(const property& prop, const Acc& acc)
{
    return visitor::property_info<type_list<T, Policy, Acc>>{prop, acc};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename Policy, typename Getter, typename Setter>
static visitor::property_getter_setter_info<type_list<T, Policy, Getter, Setter>>
make_property_getter_setter_info(const property& prop,
                                 const Getter& getter,
                                 const Setter& setter)
{
    return visitor::property_getter_setter_info<type_list<T, Policy, Getter, Setter>>{prop, getter, setter};
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct visitor_list_wrapper
{
    using visitor_list = type_list<>;
};

struct visitor_defined {};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VISITOR_IMPL_H_
