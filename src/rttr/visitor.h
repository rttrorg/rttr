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

#ifndef RTTR_VISITOR_H_
#define RTTR_VISITOR_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/rttr_enable.h"
#include "rttr/type_list.h"
#include "rttr/detail/visitor/visitor_registration.h"

#include <type_traits>
#include <functional>
#include <tuple>

namespace rttr
{

class property;
class method;
class constructor;

/*!
 * \brief The class \ref visitor, is used for visiting your registered accessors of a type at compile time.
 *
 * Sometimes it might be necessary to get access to the underlying
 * registered accessor (e.g. function pointers, object pointers, etc...) of a type.
 * For example in order to bind a type to a scripting library, which only allows to register concrete types.
 * The generic approach of using a visitor will solve this problem. Furthermore, because it is template based,
 * you can write one visitor to iterate over all your types in a generic way and have access to the static type information.
 *
 * Own visitor
 * -----------
 * In order to implement your own visitor class, you have to derive from the class \ref visitor and reimplement
 * the following visitor function templates:
 * 1. `void visitor::visit_type_begin(const type_info<T>& info)`
 * 2. `void visitor::visit_type_end(const type_info<T>& info);`
 * 3. `void visitor::visit_constructor(const constructor_info<T>& info);`
 * 4. `void visitor::visit_constructor_function(const constructor_function_info<T>& info);`
 * 5. `void visitor::visit_method(const method_info<T>& info);`
 * 6. `void visitor::visit_global_method(const method_info<T>& info);`
 *
 * and add the macro `RTTR_ENABLE(visitor);` in the class body.
 *
 * Using a visitor
 * ---------------
 * \code{.cpp}
 *
 * #include "print_visitor.h"  // IMPORTANT: You have to include your visitor always before the include of the `<rttr/registration>`
 * #include <rttr/registration>
 *
 * RTTR_REGISTRATION
 * {
 *     rttr::registration::class_<cat>("cat")
 *                           .method("meow", &cat::meow);
 *
 * }
 * int main()
 * {
 *    print_visitor vi;
 *
 *    type t = type:get_by_name("cat");
 *    vi.visit(t);
 * }
 * \endcode
 *
 * It is very important that the include of the visitor is done before the include of `<rttr/registration>`,
 * otherwise the visitor will not be found by rttr. The reason for that is, the visit functions will be created at compile
 * time, so the declaration has to be before the instantiation.
 * Because of limitations of the c++ preprocessor, it is not possible to include the visitor or the `<rttr/registration>` header in
 * a precompiled header and use the \ref visitor functionality. All custom visitors will not be found.
 *
 */
class RTTR_API visitor
{
public:
    /*!
     * \brief The constructor of the visitor class.
     *
     */
    visitor();

     /*!
     * \brief The destructor of the visitor class.
     *
     */
    virtual ~visitor();

    /*!
     * \brief Calling this function will indirectly call the visit functions
     *        for all registered types members (constructors, methods, properties, etc...)
     *        This includes also registered base classes.
     */
    void visit(type t);

    /*!
     * \brief Calling this function will indirectly call the function \ref visit_method()
     *        for the underlying registered type.
     */
    void visit(method meth);

    /*!
     * \brief Calling this function will indirectly call the function \ref visit_constructor()
     *        or \ref visit_constructor_function() for the underlying registered type.
     */
    void visit(constructor ctor);

    /*!
     * \brief Calling this function will indirectly call one of the functions:
     *        - \ref visit_property()
     *        - \ref visit_readonly_property()
     *        - \ref visit_global_property()
     *        - \ref visit_global_readonly_property()
     *         for the underlying registered type.
     */
    void visit(property prop);

#ifdef DOXYGEN
    /*!
     * \brief The \ref type_info class is used to forward all information during registration of a \ref rttr::registration::class_ "class".
     * The information will be forwarded when the methods: \ref visit_type_begin() and \ref visit_type_end() are implemented.
     */
    template<typename T>
    struct type_info
    {
        using declaring_type    = T;        //!< The compile time type information,
                                            //!< corresponds to template parameter used in: `rttr::type::get<T>()`, or `rttr::registration::class_<T>("...")`

        using base_classes      = type_list<Base_Classes...>; //!< A list of base classes from type \p T
        const type&             type_item;  //!< The type object
    };

    /*!
     * \brief The \ref constructor_info class is used to forward all information during registration of a constructor.
     * The information will be forwarded when the methods: \ref visit_constructor() are implemented.
     */
    template<typename T>
    struct constructor_info
    {
        using declaring_type    = T;        //!< The type \p T where this constructor is declared to.
        using policy            = Policy;   //!< The used policy during registration
        const constructor&      ctor_item;  //!< The constructor object
    };

    /*!
     * \brief The \ref constructor_function_info class is used to forward all information during registration of a \ref constructor function.
     * The information will be forwarded when the methods: \ref visit_constructor_function() are implemented.
     */
    template<typename T>
    struct constructor_function_info
    {
        using declaring_type = T;           //!< The type \p T where this constructor is declared to.
        using policy         = Policy;      //!< The used policy during registration
        const constructor    ctor_item;     //!< The constructor object
        Acc                  function_ptr;  //!< The function pointer to create object T
    };

    /*!
     * \brief The \ref method_info class is used to forward all information during registration of a \ref method.
     * The information will be forwarded when the methods: \ref visit_method() are implemented.
     */
    template<typename T>
    struct method_info
    {
        using declaring_type    = T;            //!< The type \p T where this method is declared to.
        using policy            = Policy;       //!< The used policy during registration
        const method&           method_item;    //!< The method object
        Acc                     function_ptr;   //!< The function pointer of the method (can be a member- or free function)
    };

    /*!
     * \brief The \ref property_info class is used to forward all information during registration of a \ref property.
     * The information will be forwarded when the methods: \ref visit_property() are implemented.
     */
    template<typename T>
    struct property_info
    {
        using declaring_type = T;
        using policy         = Policy;
        const property       property_item;
        Acc                  property_accessor;
    };

    /*!
     * \brief The \ref method_info class is used to forward all information during registration of a \ref property.
     * The information will be forwarded when the methods: \ref visit_property() are implemented.
     */
    template<typename T>
    struct property_getter_setter_info
    {
        using declaring_type = T;
        using policy         = Policy;
        const property       property_item;
        Getter               property_getter;
        Setter               property_setter;
    };
#else
    template<typename T>
    struct type_info;

    template<typename T, typename...Base_Classes>
    struct type_info<type_list<T, type_list<Base_Classes...>>>
    {
        using declaring_type    = T;
        using base_classes      = type_list<Base_Classes...>;
        const type&             type_item;
    };

    template<typename T>
    struct constructor_info;

    template<typename T, typename Policy, typename...Ctor_args>
    struct constructor_info<type_list<T, Policy, type_list<Ctor_args...>>>
    {
        using declaring_type    = T;
        using ctor_args         = type_list<Ctor_args...>;
        using policy            = Policy;
        const constructor&      ctor_item;
    };

    template<typename T>
    struct constructor_function_info;

    template<typename T, typename Policy, typename Acc>
    struct constructor_function_info<type_list<T, Policy, Acc>>
    {
        using declaring_type = T;
        using policy         = Policy;
        const constructor    ctor_item;
        Acc                  function_ptr;
    };

    template<typename T>
    struct method_info;

    template<typename T, typename Policy, typename Acc>
    struct method_info<type_list<T, Policy, Acc>>
    {
        using declaring_type = T;
        using policy         = Policy;
        const method         method_item;
        Acc                  function_ptr;
    };

    template<typename T>
    struct property_info;

    template<typename T, typename Policy, typename Acc>
    struct property_info<type_list<T, Policy, Acc>>
    {
        using declaring_type = T;
        using policy         = Policy;
        const property       property_item;
        Acc                  property_accessor;
    };

    template<typename T>
    struct property_getter_setter_info;

    template<typename T, typename Policy, typename Getter, typename Setter>
    struct property_getter_setter_info<type_list<T, Policy, Getter, Setter>>
    {
        using declaring_type = T;
        using policy         = Policy;
        const property       property_item;
        Getter               property_getter;
        Setter               property_setter;
    };
#endif

    /*!
     * \brief This function will be called when you visit a type via: \ref visit(type)
     *        It is the first function that will be invoked, when visiting a type.
     *        When the visiting type has base classes, it will be first invoked with he most basic type
     *        and last step with the current type.
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param Base_Classes... A list of base classes, ascending order
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * \remark You have to register your type via: \ref registration::class_
     *         otherwise the static type information for invoke cannot be retrieved.
     *         The signature has to match exactly the declaration here, otherwise it will not be invoked.
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see type::get_base_classes()
     */
    template<typename T, typename...Base_Classes>
    void visit_type_begin(const type_info<T>& info);

    /*!
     * \brief This function will be called when you visit a type via: \ref visit(type).
     *        It is the last function that will be invoked, when visiting a type.
     *        When the visiting type has base classes, it will be first invoked with he most basic type
     *        and last step with the current type.
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal type type, do not work with this parameter directly.
     * \param Base_Classes... A list of base classes, ascending order
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * \remark You have to register your type via: \ref registration::class_
     *         otherwise the static type information for invoke cannot be retrieved.
     *         The signature has to match exactly the declaration here, otherwise it will not be invoked.
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see type::get_base_classes()
     */
    template<typename T, typename...Base_Classes>
    void visit_type_end(const type_info<T>& info);

    /*!
     * \brief This function will be called when you visit a constructor via: \ref visit(type)
     *        or directly\ref visit(constructor).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param Ctor_Args The argument of the constructor
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(constructor)
     */
    template<typename T, typename...Ctor_Args>
    void visit_constructor(const constructor_info<T>& info);

    /*!
     * \brief This function will be called when you visit a constructor function via: \ref visit(type)
     *        or \ref visit(constructor).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(constructor)
     */
    template<typename T>
    void visit_constructor_function(const constructor_function_info<T>& info);

    /*!
     * \brief This function will be called when you visit a type method via: \ref visit(type) or \ref visit(method).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(method), type::get_method()
     */
    template<typename T>
    void visit_method(const method_info<T>& info);

    /*!
     * \brief This function will be called when you visit a global method via: \ref visit(method).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(method), type::get_global_method()
     */
    template<typename T>
    void visit_global_method(const method_info<T>& info);

    /*!
     * \brief This function will be called when you visit a property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_property()
     */
    template<typename T>
    void visit_property(const property_info<T>& info);

    /*!
     * \brief This function will be called when you visit a property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_property()
     */
    template<typename T>
    void visit_getter_setter_property(const property_getter_setter_info<T>& info);

    /*!
     * \brief This function will be called when you visit a global property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_global_property()
     */
    template<typename T>
    void visit_global_property(const property_info<T>& info);

    /*!
     * \brief This function will be called when you visit a global property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_global_property()
     */
    template<typename T>
    void visit_global_getter_setter_property(const property_getter_setter_info<T>& info);

    /*!
     * \brief This function will be called when you visit a read only property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_property()
     */
    template<typename T>
    void visit_readonly_property(const property_info<T>& info);

    /*!
     * \brief This function will be called when you visit a global read only property via: \ref visit(property).
     *        Reimplement this function, when you need the static compile time type information.
     *
     * \param T Internal template type, do not work with this parameter directly.
     * \param info This object will be provided by RTTR, use it's public members and the `using's`
     *
     * You normally don't call this function directly. However, make sure this function is declared public,
     * otherwise it cannot be invoked.
     *
     * \see visit(property), type::get_global_property()
     */
    template<typename T>
    void visit_global_readonly_property(const property_info<T>& info);

private:
    void visit_impl(const type& t);

private:
    RTTR_ENABLE();
};

} // end namespace rttr

#include "rttr/detail/visitor/visitor_impl.h"

#endif  // RTTR_VISITOR_H_
