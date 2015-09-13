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

#ifndef RTTR_POLICY_H_
#define RTTR_POLICY_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/policies/ctor_policies.h"
#include "rttr/detail/policies/meth_policies.h"
#include "rttr/detail/policies/prop_policies.h"

namespace rttr
{

/*!
 * The \ref policy class contains all policies that can be used during the registration of reflection information.
 *
 * For easier usage, the policies are subdivided into following groups:
 * - for methods: \ref policy::meth
 * - for properties: \ref policy::prop
 * - for constructors: \ref policy::ctor
 *
 */
struct RTTR_API policy
{
    /*!
     * The \ref meth class groups all policies that can be used during registration of methods.
     *
     * \endcode
     */
    struct RTTR_API meth
    {
        /*!
         * This policy can be used when a method return a reference to an object
         * and the caller should be able to access this object via the returned variant.
         * Reference cannot be copied directly in a variant, therefore it is possible transform the reference
         * to a pointer.
         *
         * See following example code:
         * \code{.cpp}
         *
         * std::string& get_text() { static std:string text("hello world"); return text; }
         *
         * RTTR_REGISTER
         * {
         *   registration::method("get_text", &get_text)
         *   (
         *      policy::meth::return_ref_as_ptr
         *   );
         * }
         * int main()
         * {
         *   method meth = type::get_global_method("get_text");
         *   std::cout << meth.get_return_type().get_name();    // prints "std::string*"
         *   variant var = meth.invoke(empty_instance());
         *   std::cout << var.is_type<std::string*>();          // prints "true"
         *   return 0;  
         * }
         * \endcode
         */
        static const detail::return_as_ptr      return_ref_as_ptr;

       /*!
         * This policy should be used when the return value of a method should not be forwarded to the caller.
         * For the caller it looks like the method has no return value, the return type will be *void*.
         *
         * See following example code:
         * \code{.cpp}
         *
         * using namespace rttr;
         * int my_func() { return 42; }
         *
         * RTTR_REGISTER
         * {
         *   registration::method("my_func", &my_func)
         *   (
         *      policy::meth::discard_return
         *   );
         * }
         *
         * int main()
         * {
         *   method meth = type::get_global_method("my_func");
         *   std::cout << meth.get_return_type().get_name();    // prints "void"
         *   variant var = meth.invoke(empty_instance());
         *   std::cout << var.is_type<void>();                  // prints "true"
         *   return 0;  
         * }
         * \endcode
         */
        static const detail::discard_return     discard_return;
    };

    /*!
     * The \ref prop class groups all policies that can be used during registration of properties.
     *
     */
    struct RTTR_API prop
    {
        /*!
         * The \ref bind_as_ptr policy will bind a member object as *pointer* type.
         *
         * This can be useful when binding big data types, like arrays, to avoid copies during get/set of the property.
         *
         * See following example code:
         * \code{.cpp}
         * using namespace rttr;
         * struct Foo
         * {
         *   std::vector<int> vec;
         * };
         *
         * RTTR_REGISTER
         * {
         *     registration::class_<Foo>()
         *         .property("vec", &Foo::vec)
         *          (
         *              policy::prop::bind_as_ptr
         *          );
         * }
         *
         * int main()
         * {
         *   Foo obj;
         *   property vec_prop = type::get<Foo>().get_property("vec");
         *   variant vec_value = prop.get_value(obj);
         *   std::cout << value.is_type<std::vector<int>*>();       // prints "true"
         *   prop.set_value(obj, vec_value);                        // not really necessary, but remark that now a std::vector<int>* is expected
         *   return 0;
         * }
         * \endcode
         */
        static const detail::bind_as_ptr        bind_as_ptr;
    };

    /*!
     * The \ref ctor class groups all policies that can be used during registration of constructors.
     *
     */
    struct RTTR_API ctor
    {
        /*!
         * Add documentation here
         */
        
    };
};

} // end namespace rttr

#endif // RTTR_POLICY_H_
