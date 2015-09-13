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

#ifndef RTTR_REGISTRATION_H_
#define RTTR_REGISTRATION_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/policy.h"
#include "rttr/access_level.h"
#include "rttr/detail/registration/bind_types.h"
#include "rttr/detail/registration/registration_executer.h"

namespace rttr
{
namespace detail
{
    class meta_data;
    template<typename Enum_Type>
    class enum_data;
}

/*!
 * The \ref registration class is the entry point for the manual registration of reflection information
 * to the type system. It is possible to register *constructors*, *properties*, *methods* and *enumerations*.
 *
 * See following example for a typical usage:
 *
 * Put the \ref RTTR_ENABLE() macro inside the class declaration, when you will inherit from this class, 
 * otherwise you can omit the macro.
 * \code{.cpp}
 *  #include <rttr/type>
 *  struct Mesh
 *  {
 *      Mesh();
 *      Mesh(const std::string& name)
 *      Vector3d getPosition() const;
 *      void setPosition(const Vector3d& pos);
 *      enum E_TransformSpace
 *      {
 *          TS_LOCAL,
 *          TS_PARENT,
 *          TS_WORLD
 *      };
 *    
 *      void setDirection(const Vector3 &vec, E_TransformSpace ts = TS_LOCAL);
 *      RTTR_ENABLE()
 *  private:
 *      Vector3d _pos;
 *  };
 * \endcode
 *
 * Then in a cpp file, register the constructors, properties and methods of the class \p Mesh.
 *
 * \code{.cpp}
 *  #include "Mesh.cpp"
 *  #include <rttr/registration>
 *  using namespace rttr;
 *  
 *  // register the class Mesh before main is called
 *  RTTR_REGISTRATION
 *  {
 *    using namespace rttr;
 *    registration::class_<Mesh>("Mesh")
 *      .constructor<>()
 *      .constructor<const string&>()
 *      .enumeration<E_TransformSpace>("E_TransformSpace")
 *      (
 *          value("TS_LOCAL", TS_LOCAL),
 *          value("TS_PARENT", TS_PARENT),
 *          value("TS_WORLD", TS_WORLD),
 *
 *          meta_data("GUI_DESCR", "This enum describes the transformation.")
 *      )
 *      .property("pos", &Mesh::getPosition, &Mesh::setPosition)
 *      (
 *          meta_data("GUI_LABEL", "Position."),
 *          meta_data("GUI_DESCR", "The position of the mesh."),
 *      )
 *      .method("setDirection", &Mesh::setDirection);
 *  }
 * \endcode
 *
 * \remark See the usage of `()` operator to add additional \ref rttr::meta_data(variant, variant) "meta data", \ref policy "policies" or
 *         \ref rttr::value "enum values".
 *
 */
class RTTR_API registration
{
public:
    template<typename...T>
    class bind;

    /*!
     * The \ref class_ is used to register classes to RTTR.
     */
    template<typename Class_Type>
    class class_
    {
        public:
            /*!
             * \brief Construct a class_ object with the given name \p name.
             *
             * \param name The name of the class as string literal. Can be retrieved later via type::get_name().
             *
             */
            class_(const char* name);
            ~class_();


            /*!
             * \brief The bracket operator can be used to add additional meta data to the class type.
             *
             */
            template<typename...Args>
            class_<Class_Type>& operator()(Args&&...args);

            
            /*!
             * \brief Register a constructor for this class type with or without arguments.
             *
             * \param level  The access level of the constructor.
             *
             * \see rttr::type::get_constructor, rttr::type::create, access_level
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename... Args, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_level_list>::value>::type>
            bind<detail::ctor, Class_Type, Args...> constructor(acc_level level = acc_level());
            
            /*!
             * \brief Register a constructor for this class type which uses a function \p F.
             *
             * \param func   A function which creates an instance of \p Class_Type;
             *               This can be a pointer to a function or a std::function.
             * \param level  The access level of the constructor.
             *
             * \see rttr::type::get_constructor, rttr::type::create
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename F, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<!detail::contains<F, detail::access_level_list>::value>::type>
            bind<detail::ctor_func, Class_Type, F> constructor(F func, acc_level level = acc_level());


            /*!
             * \brief Register a property to this class.
             *
             * \param name  The name of the property.
             * \param acc   The accessor to the property; this can be a pointer to a member or a pointer to a variable.
             * \param level  The access level of the property.
             *
             * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename A, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_level_list>::value>::type>
            bind<detail::prop, Class_Type, A> property(const char* name, A acc, acc_level level = acc_level());

            /*!
             * \brief Register a read only property to this class.
             *
             * \param name  The name of the property.
             * \param acc   The accessor to the property; this can be a pointer to a member, a pointer to a variable,
             *              a pointer to a member function, a pointer to a function or a std::function.
             * \param level  The access level of the read only property.
             *
             * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename A, typename acc_level = detail::public_access, typename Tp = typename std::enable_if<detail::contains<acc_level, detail::access_level_list>::value>::type>
            bind<detail::prop_readonly, Class_Type, A> property_readonly(const char* name, A acc, acc_level level = acc_level());

            /*!
             * \brief Register a property to this class.
             *
             * \param name   The name of the property.
             * \param getter The getter accessor to the property; this can be a pointer to a member function,
             *               a pointer to a function or a std::function.
             * \param setter The setter accessor to the property; this can be a pointer to a member function,
             *               a pointer to a function or a std::function.
             * \param level  The access level of the property.
             *
             * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename A1, typename A2, typename Tp = typename std::enable_if<!detail::contains<A2, detail::access_level_list>::value>::type, typename acc_level = detail::public_access>
            bind<detail::prop, Class_Type, A1, A2> property(const char* name, A1 getter, A2 setter, acc_level level = acc_level());

            
            /*!
             * \brief Register a method to this class.
             *
             * \param name      The name of the method.
             * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
             * \param level     The access level of the method.
             *
             * \remark The method name does not have to be unique for this class.
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename F, typename acc_level = detail::public_access>
            bind<detail::meth, Class_Type, F> method(const char* name, F f, acc_level level = acc_level());

            
            /*!
             * \brief Register a nested enumeration of type \p Enum_Type
             *
             * \param name      The name of the enumeration.
             *
             * \return A \ref bind object, in order to chain more calls.
             */
            template<typename Enum_Type>
            bind<detail::enum_, Class_Type, Enum_Type> enumeration(const char* name);
        private:
            class_(const std::shared_ptr<detail::registration_executer>& reg_exec);
            class_(const class_& other);
            class_& operator=(const class_& other);
        private:
            std::shared_ptr<detail::registration_executer> m_reg_exec;
            template<typename...T>
            friend class bind;
    };

    /*!
     * \brief Register a global property with read write access.
     *
     * \param name  The name of the property.
     * \param acc   The accessor to the property; a pointer to a variable.
     *
     * \remark The name of the property has to be unique, otherwise it will not be registered.
     *
     * \return A \ref bind object, in order to chain more calls.
     */
    template<typename A>
    static bind<detail::prop, void, A> property(const char* name, A acc);

    /*!
     * \brief Register a global read only property.
     *
     * \param name  The name of the property.
     * \param acc   The accessor to the property; this can be a pointer to a variable,
     *              a pointer to a function or a std::function.
     *
     * \remark The name of the property has to be unique, otherwise it will not be registered.
     *
     * \return A \ref bind object, in order to chain more calls.
     */
    template<typename A>
    static bind<detail::prop_readonly, void, A> property_readonly(const char* name, A acc);

    /*!
     * \brief Register a property to this class.
     *
     * \param name   The name of the property.
     * \param getter The getter accessor to the property; this can be a pointer to a function or a std::function.
     * \param setter The setter accessor to the property; this can be a pointer to a function or a std::function.
     *
     * \remark The name of the property has to be unique, otherwise it will not be registered.
     *
     * \return A \ref bind object, in order to chain more calls.
     */
    template<typename A1, typename A2>
    static bind<detail::prop, void, A1, A2> property(const char* name, A1 getter, A2 setter);

    /*!
     * \brief Register a method to this class.
     *
     * \param name      The name of the method.
     * \param function  The function accessor to this method; this can be a member function, a function or a std::function.
     *
     * \remark The method name does *not* have to be unique.
     *
     * \return A \ref bind object, in order to chain more calls.
     */
    template<typename F>
    static bind<detail::meth, void, F> method(const char* name, F f);

    /*!
     * \brief Register a global enumeration of type \p Enum_Type
     *
     * \param name The name of the enumeration.
     *
     * \remark The name of the enumeration has to be unique, otherwise it will not be registered.
     *
     * \return A \ref bind object, in order to chain more calls.
     */
    template<typename Enum_Type>
    static bind<detail::enum_, void, Enum_Type> enumeration(const char* name);

private:
    registration() {}
    registration(const std::shared_ptr<detail::registration_executer>& reg_exec) : m_reg_exec(reg_exec) { }
    registration(const registration& other);
    registration& operator=(const registration& other);

private:
    std::shared_ptr<detail::registration_executer> m_reg_exec;
    template<typename...T>
    friend class bind;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief This is a helper function to register overloaded functions.
 *
 * Use it like following:
 * \code{.cpp}
 *
 * #include <rttr/registration>
 * #include <cmath>
 * using namespace rttr;
 * 
 * RTTR_REGISTRATION
 * {
 *      registration::method("pow", select_overload<float(float, float)>(&pow));
 * }
 * \endcode
 *
 * \remark The method **cannot** be used with *MSVC x86* compiler, because of the different calling convention for global- and member-functions.
 *
 */
template<typename Signature>
Signature* select_overload(Signature* func)
{
  return func;
}


/*!
 * \brief This is a helper function to register overloaded member functions.
 *
 * Use it like following:
 * \code{.cpp}
 *
 * #include <rttr/registration>
 * #include <cmath>
 * using namespace rttr;
 *
 * struct Foo
 * {
 *   void func(int);
 *   void func(int, int);
 * };
 * 
 * RTTR_REGISTRATION
 * {
 *      registration::class_<foo>("foo")
 *          .method("func", select_overload<void(int)>(&foo::func))
 *          .method("func", select_overload<void(int, int)>(&foo::func));
 * }
 * \endcode
 *
 * \remark The method **cannot** be used with *MSVC x86* compiler, because of the different calling convention for global- and member-functions.
 *
 */
template<typename Signature, typename ClassType>
auto select_overload(Signature (ClassType::*func)) -> decltype(func)
{
    return func;
}


/*!
 * \brief This is a helper function to register overloaded const member functions.
 *
 * Use it like following:
 * \code{.cpp}
 *
 * #include <rttr/registration>
 * using namespace rttr;
 * struct Foo
 * {
 *   void func();
 *   void func() const;
 * };
 * 
 * RTTR_REGISTRATION
 * {
 *      registration::class<Foo>("Foo")
 *         .method("func", select_non_const(&Foo::func))
 *         .method("func", select_const(&Foo::func));
 * }
 * \endcode
 *
 * \remark The method **cannot** be used with *MSVC x86* compiler, because of the different calling convention for global- and member-functions.
 *
 */
template<typename ClassType, typename ReturnType, typename... Args>
auto select_const(ReturnType (ClassType::*func)(Args...) const) -> decltype(func)
{
    return func;
}

/*!
 * \brief This is a helper function to register overloaded const member functions.
 *
 * Use it like following:
 * \code{.cpp}
 *
 * #include <rttr/registration>
 * using namespace rttr;
 * struct Foo
 * {
 *   void func();
 *   void func() const;
 * };
 * 
 * RTTR_REGISTRATION
 * {
 *      registration::class<Foo>("Foo")
 *          .method("func", select_non_const(&Foo::func))
 *          .method("func", select_const(&Foo::func));
 * }
 * \endcode
 *
 * \remark The method **cannot** be used with *MSVC x86* compiler, because of the different calling convention for global- and member-functions.
 *
 */
template<typename ClassType, typename ReturnType, typename... Args>
auto select_non_const(ReturnType(ClassType::*func)(Args...)) -> decltype(func)
{
    return func;
}

/*!
 * The \ref meta_data function can be used to add additional meta data information during the 
 * registration process of reflection information. Use it in the `()` operator of the returned 
 * \ref bind object.
 */
static RTTR_INLINE detail::meta_data meta_data(variant key, variant value);

/*!
 * The \ref value function should be used to add a mapping from enum `name` to `value`
 * during the registration process of reflection information. Use it in the `()` operator of the returned 
 * \ref bind object.
 *
 * \see \ref registration::enumeration
 */
template<typename Enum_Type>
static detail::enum_data<Enum_Type> value(const char* name, Enum_Type value);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#ifdef DOXYGEN

/*!
 * \brief Use this macro to automatically register your reflection information to RTTR before `main` is called.
 *
 * Use it in following way:
 * \code{.cpp}
 * RTTR_REGISTRATION
 * {
 *      rttr::registration::method("foo", &foo);
 * }
 * \endcode
 *
 * Just place the macro in global scope in a cpp file. 
 *
 * \remark It is not possible to place the macro multiple times in one cpp file.
 *
 */
#define RTTR_REGISTRATION

/*!
 * \brief Place this macro inside a class, when you need to reflect properties or methods 
 *        which are declared in private scope of the class.
 *
\code{.cpp}
class Foo
{
    private:
        int m_value;

    RTTR_REGISTRATION_FRIEND
};
\endcode

\code{.cpp}
RTTR_REGISTRATION
{
    rttr::registration::class_<Foo>()
        .property("value", &Foo::m_value);
}
\endcode
 */
#define RTTR_REGISTRATION_FRIEND

/*!
 * The \ref bind class is used to chain registration calls.
 *
 * The class should not be instantiate directly, because it will be created automatically,
 * when a *constructor*, *property*, *method* or *enumeration* is registered.
 * 
 * Additionally, the class has implemented the bracket operator `()` as variadic function tenplate.
 * Use it to forward meta_data or policies to the previous registered item.
 *
 * \remark Do not instantiate this class directly.
*/
template<typename...T>
class registration::bind : public base_class
{
    public:
        /*!
         * \brief The bracket operator can be used to add additional meta data or policies.
         */
        template<typename... Args>
        base_class operator()(Args&&... arg);
};

#endif

} // end namespace rttr

#include "rttr/detail/registration/registration_impl.h"

#endif // RTTR_REGISTRATION_H_
