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

#ifndef RTTR_METHOD_WRAPPER_H_
#define RTTR_METHOD_WRAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/method/method_wrapper_base.h"
#include "rttr/detail/misc/function_traits.h"
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/detail/type/accessor_type.h"
#include "rttr/detail/method/method_accessor.h"
#include "rttr/variant.h"


#include <functional>
#include <string>

namespace rttr
{
namespace detail
{

template<typename F, typename Policy>
class method_wrapper : public method_wrapper_base
{
    public:
        method_wrapper(std::string name, type declaring_type, F func_acc)
        : m_func_acc(func_acc)
        { }

        bool is_static()                        const   { return method_accessor<F, Policy>::is_static();           }
        type get_return_type()                  const   { return method_accessor<F, Policy>::get_return_type();     }
        std::vector<bool> get_is_reference()    const   { return method_accessor<F, Policy>::get_is_reference();    }
        std::vector<bool> get_is_const()        const   { return method_accessor<F, Policy>::get_is_const();        }
        std::vector<type> get_parameter_types() const   { return method_accessor<F, Policy>::get_parameter_types(); }

        variant invoke(instance& object) const
        {
           return method_accessor<F, Policy>::invoke(m_func_acc, object);
        }

        variant invoke(instance& object, argument& arg1) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1);
        }
        variant invoke(instance& object, argument& arg1, argument& arg2) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1, arg2);
        }
        variant invoke(instance& object, argument& arg1, argument& arg2, argument& arg3) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1, arg2, arg3);
        }
        variant invoke(instance& object, argument& arg1, argument& arg2, argument& arg3, argument& arg4) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1, arg2, arg3, arg4);
        }
        variant invoke(instance& object, argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1, arg2, arg3, arg4, arg5);
        }
        variant invoke(instance& object, argument& arg1, argument& arg2, argument& arg3, argument& arg4, argument& arg5, argument& arg6) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        variant invoke_variadic(instance& object, std::vector<argument>& args) const
        {
            return method_accessor<F, Policy>::invoke(m_func_acc, object, args);
        }

    private:
        F  m_func_acc;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_METHOD_WRAPPER_H_
