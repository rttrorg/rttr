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

#ifndef __RTTR_METHOD_CONTAINER_H__
#define __RTTR_METHOD_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/method_container_base.h"
#include "rttr/detail/function_traits.h"
#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"
#include "rttr/detail/accessor_type.h"
#include "rttr/detail/method_accessor.h"
#include "rttr/variant.h"


#include <functional>
#include <string>

namespace rttr
{
namespace detail
{

template<typename F, typename Policy>
class method_container : public method_container_base
{
    public:
        method_container(const std::string& name, const type declaring_type, F func_acc)
        :   method_container_base(name, declaring_type),
            _func_acc(func_acc)
        { }

        bool is_static()                        const   { return method_accessor<F, Policy>::is_static();           }
        type get_return_type()                  const   { return method_accessor<F, Policy>::get_return_type();     }
        std::vector<bool> get_is_reference()    const   { return method_accessor<F, Policy>::get_is_reference();    }
        std::vector<bool> get_is_const()        const   { return method_accessor<F, Policy>::get_is_const();        }
        std::vector<type> get_parameter_types() const   { return method_accessor<F, Policy>::get_parameter_types(); }

        variant invoke(detail::instance& object) const
        {
           return method_accessor<F, Policy>::invoke(_func_acc, object);
        }

        variant invoke(detail::instance& object, detail::argument& arg1) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1);
        }
        variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1, arg2);
        }
        variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1, arg2, arg3);
        }
        variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1, arg2, arg3, arg4);
        }
        variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4, detail::argument& arg5) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1, arg2, arg3, arg4, arg5);
        }
        variant invoke(detail::instance& object, detail::argument& arg1, detail::argument& arg2, detail::argument& arg3, detail::argument& arg4, detail::argument& arg5, detail::argument& arg6) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, arg1, arg2, arg3, arg4, arg5, arg6);
        }

        variant invoke_variadic(detail::instance& object, std::vector<detail::argument>& args) const
        {
            return method_accessor<F, Policy>::invoke(_func_acc, object, args);
        }

    private:
        F  _func_acc;
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_METHOD_CONTAINER_H__
