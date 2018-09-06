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

#include "rttr/visitor.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

visitor::visitor()
{

}

/////////////////////////////////////////////////////////////////////////////////////////

visitor::~visitor()
{

}

/////////////////////////////////////////////////////////////////////////////////////////

void visitor::visit(type t)
{
    // first we visit all base classes
    for (auto& t_ : t.get_base_classes())
        visit_impl(t_);

    // as last step, the current type itself
    visit_impl(t);
}

/////////////////////////////////////////////////////////////////////////////////////////

void visitor::visit_impl(const type& t)
{
    t.visit(*this, detail::type_of_visit::begin_visit_type);
    const auto filter = filter_item::instance_item | filter_item::static_item |
                        filter_item::public_access | filter_item::non_public_access |
                        filter_item::declared_only;

    for (auto ctor : t.get_constructors(filter))
    {
        ctor.visit(*this);
    }

    for (auto prop : t.get_properties(filter))
    {
        prop.visit(*this);
    }

    for (auto meth : t.get_methods(filter))
    {
        meth.visit(*this);
    }

    t.visit(*this, detail::type_of_visit::end_visit_type);
}

/////////////////////////////////////////////////////////////////////////////////////////

void visitor::visit(method meth)
{
    meth.visit(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

void visitor::visit(constructor ctor)
{
    ctor.visit(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

void visitor::visit(property prop)
{
    prop.visit(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
