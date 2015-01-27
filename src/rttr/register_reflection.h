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

#ifndef __RTTR_REGISTER_REFLECTION_H__
#define __RTTR_REGISTER_REFLECTION_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/metadata.h"
#include "rttr/policy.h"

#include <string>
#include <vector>

namespace rttr
{

/*!
 * \brief This class is used to register the constructors, properties, methods and enumeration
 *        for a certain class \p ClassType.
 *
 * Following example show the typical usage:
 *
 * First the class declaration.
\code{.cpp}
#include <rttr/type>
struct Mesh
{
  Mesh();
  Mesh(const std::string& name)
  Vector3d getPosition() const;
  void setPosition(const Vector3d& pos);
  enum E_TransformSpace
  {
    TS_LOCAL,
    TS_PARENT,
    TS_WORLD
  };
  
  void setDirection(const Vector3 &vec, E_TransformSpace ts = TS_LOCAL);
  
  private:
    Vector3d _pos;
};

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(Mesh) // to register the type
\endcode
*
* The in a cpp file, register the constructors, properties and methods of the class \p Mesh.
*
\code{.cpp}
#include "Mesh.cpp"
#include <rttr/reflect>
using namespace rttr;

// register the class Mesh before main is called
RTTR_REFLECT
{
  class_<Mesh>()
    .constructor<>()
    .constructor<const string&>()
    .enumeration<E_TransformSpace>({{"TS_LOCAL", TS_LOCAL},
                                    {"TS_PARENT", TS_PARENT},
                                    {"TS_WORLD", TS_WORLD}})
    .property("pos", &Mesh::getPosition, &Mesh::setPosition)
    .method("setDirection", &Mesh::setDirection);
}
\endcode
 *
 * Use it then for example like this:
 \code{.cpp}
int main()
{
  Mesh obj;
  const type mesh_type = type::get(obj);
  std::cout << mesh_type.get_name() << std::endl;     // prints "Mesh"
  
  // sets/gets a property
  property pos = mesh_type.get_property("pos");
  pos.set_value(obj, Vector3d(1,2,3));                // here we set the value
  variant var_pos = pos.get_value(obj);               // here we get the value
  
  Vector3d& vec = var_post.get_value<Vector3d>();     // vec == Vector3d(1,2,3)
  
  // invoke a method
  method meth_dir = mesh_type.get_method("setDirection");
  meth_dir.invoke(obj, Vector3d(1,2,3), Mesh::TS_WORLD);
  
  // retrieve all properties
  for (const auto& prop : mesh_type.get_properties())
  {
      std::cout << prop.get_name(); // prints all property names of the type Mesh
  }
  
  for (const auto& meth : mesh_type.get_methods())
  {
      std::cout << meth.get_name(); //  prints all method names of the type Mesh
  }
  
  return 0;
}
 \endcode
 *
 */
template<typename ClassType>
class class_
{
    public:
        class_(std::vector< rttr::metadata > data = std::vector< rttr::metadata >());
        ~class_();

        /*!
         * \brief Register a constructor for this class type with with or without arguments.
         *
         * \see rttr::type::get_constructor, rttr::type::create
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename... Args>
        class_& constructor(std::vector< rttr::metadata > data = std::vector< rttr::metadata >());


        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name  The name of the property.
         * \param acc   The accessor to the property; this can be a pointer to a member or a pointer to a variable.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A>
        class_& property(const std::string& name, A acc);

        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name  The name of the property.
         * \param acc   The accessor to the property; this can be a pointer to a member or a pointer to a variable.
         * \param data  Additional meta data.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A>
        class_& property(const std::string& name, A acc, std::vector< rttr::metadata > data);

        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param acc    The accessor to the property; this can be a pointer to a member or a pointer to a variable.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A, typename Policy>
        class_& property(const std::string& name, A acc, const Policy& policy,
#ifndef DOXYGEN
                         typename std::enable_if<detail::contains<Policy, detail::policy_list>::value>::type* = nullptr
#endif
                );


        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param acc    The accessor to the property; this can be a pointer to a member or a pointer to a variable.
         * \param data   Additional meta data.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A, typename Policy>
        class_& property(const std::string& name, A acc, std::vector< rttr::metadata > data, const Policy& policy);

        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param getter The getter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param setter The setter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A1, typename A2>
        class_& property(const std::string& name, A1 getter, A2 setter,
#ifndef DOXYGEN
                         typename std::enable_if<!detail::contains<A2, detail::policy_list>::value>::type* = nullptr
#endif
);
   

        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param getter The getter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param setter The setter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param data   Additional meta data.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A1, typename A2>
        class_& property(const std::string& name, A1 getter, A2 setter, std::vector< rttr::metadata > data);

        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param getter The getter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param setter The setter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A1, typename A2, typename Policy>
        class_& property(const std::string& name, A1 getter, A2 setter, const Policy& policy);
       
        /*!
         * \brief Register a property with read and write access to this class.
         *
         * \param name   The name of the property.
         * \param getter The getter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param setter The setter accessor to the property; this can be a pointer to a member function,
         *               a pointer to a function or a std::function.
         * \param data   Additional meta data.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A1, typename A2, typename Policy>
        class_& property(const std::string& name, A1 getter, A2 setter, std::vector< rttr::metadata > data, const Policy& policy);
        


        /*!
         * \brief Register a property with read only access to this class.
         *
         * \param name  The name of the property.
         * \param acc   The accessor to the property; this can be a pointer to a member, a pointer to a variable,
         *              a pointer to a member function, a pointer to a function or a std::function.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A>
        class_& property_readonly(const std::string& name, A accessor);

        /*!
         * \brief Register a property with read only access to this class.
         *
         * \param name  The name of the property.
         * \param acc   The accessor to the property; this can be a pointer to a member, a pointer to a variable,
         *              a pointer to a member function, a pointer to a function or a std::function.
         * \param data  Additional meta data.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A>
        class_& property_readonly(const std::string& name, A acc, std::vector< rttr::metadata > data);

        /*!
         * \brief Register a property with read only access to this class.
         *
         * \param name   The name of the property.
         * \param acc    The accessor to the property; this can be a pointer to a member, a pointer to a variable,
         *               a pointer to a member function, a pointer to a function or a std::function.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A, typename Policy>
        class_& property_readonly(const std::string& name, A acc, const Policy& policy);

        /*!
         * \brief Register a property with read only access to this class.
         *
         * \param name   The name of the property.
         * \param acc    The accessor to the property; this can be a pointer to a member, a pointer to a variable,
         *               a pointer to a member function, a pointer to a function or a std::function.
         * \param data   Additional meta data.
         * \param policy The policies parameter describes how the property should be binded to the type system.
         *
         * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename A, typename Policy>
        class_& property_readonly(const std::string& name, A acc, std::vector< rttr::metadata > data, const Policy& policy);

        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////

        /*!
         * \brief Register a method to this class.
         *
         * \param name      The name of the method.
         * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
         *
         * \remark The method name does not have to be unique for this class.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename F>
        class_& method(const std::string& name, F function);

        /*!
         * \brief Register a method to this class.
         *
         * \param name      The name of the method.
         * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
         * \param data      Additional meta data.
         *
         * \remark The method name does not have to be unique for this class.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename F>
        class_& method(const std::string& name, F function, std::vector< rttr::metadata > data);

        /*!
         * \brief Register a method to this class.
         *
         * \param name      The name of the method.
         * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
         * \param policy    The policies parameter describes how the method should be binded to the type system.
         *
         * \remark The method name does not have to be unique for this class.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename F, typename Policy>
        class_& method(const std::string& name, F function, const Policy& policy);

        /*!
         * \brief Register a method to this class.
         *
         * \param name      The name of the method.
         * \param function  The function accessor to this method; this can be a member function, a function or an std::function.
         * \param data      Additional meta data.
         * \param policy    The policies parameter describes how the method should be binded to the type system.
         *
         * \remark The method name does not have to be unique for this class.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename F, typename Policy>
        class_& method(const std::string& name, F function, std::vector< rttr::metadata > data, const Policy& policy);

        /*!
         * \brief Register a nested enumeration of type \p EnumType
         *
         * \param enum_data The name of the property.
         * \param data Additional meta data.
         *
         * \remark Before using this make sure you have registered the type with \ref #RTTR_DECLARE_TYPE.
         *
         * \return Reference to this, in order to chain other calls.
         */
        template<typename EnumType>
        class_& enumeration(std::vector< std::pair< std::string, EnumType> > enum_data,
                            std::vector< rttr::metadata > data = std::vector< rttr::metadata >());

    private:
        class_(const class_& other);
        class_& operator=(const class_& other);
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief Register a constructor for a type T.
 *
 * \param data  Additional meta data.
 *
 * \see rttr::type::get_constructor, rttr::type::create
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename T>
void constructor_(std::vector< rttr::metadata > data = std::vector< rttr::metadata >());

/*!
 * \brief Register a property with read and write access.
 *
 * \param name  The name of the property.
 * \param acc   The accessor to the property; a pointer to a variable.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A>
void property_(const std::string& name, A acc);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name  The name of the property.
 * \param acc   The accessor to the property; a pointer to a variable.
 * \param data  Additional meta data.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A>
void property_(const std::string& name, A acc, std::vector< rttr::metadata > data);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name   The name of the property.
 * \param acc    The accessor to the property; a pointer to a variable.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A, typename Policy>
void property_(const std::string& name, A acc, const Policy& policy,
#ifndef DOXYGEN
               typename std::enable_if<detail::contains<Policy, detail::policy_list>::value>::type* = nullptr
#endif
);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name   The name of the property.
 * \param acc    The accessor to the property; a pointer to a variable.
 * \param data  Additional meta data.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A, typename Policy>
void property_(const std::string& name, A acc, std::vector< rttr::metadata > data, const Policy& policy);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name    The name of the property.
 * \param getter  The getter accessor to the property; this can be a pointer to a function or a std::function.
 * \param setter  The setter accessor to the property; this can be a pointer to a function or a std::function.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A1, typename A2>
void property_(const std::string& name, A1 getter, A2 setter, 
#ifndef DOXYGEN
               typename std::enable_if<!detail::contains<A2, detail::policy_list>::value>::type* = nullptr
#endif
              );

/*!
 * \brief Register a property with read and write access.
 *
 * \param name    The name of the property.
 * \param getter  The getter accessor to the property; this can be a pointer to a function or a std::function.
 * \param setter  The setter accessor to the property; this can be a pointer to a function or a std::function.
 * \param data    Additional meta data.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A1, typename A2>
void property_(const std::string& name, A1 getter, A2 setter, std::vector< rttr::metadata > data);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name    The name of the property.
 * \param getter  The getter accessor to the property; this can be a pointer to a function or a std::function.
 * \param setter  The setter accessor to the property; this can be a pointer to a function or a std::function.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A1, typename A2, typename Policy>
void property_(const std::string& name, A1 getter, A2 setter, const Policy& policy);

/*!
 * \brief Register a property with read and write access.
 *
 * \param name    The name of the property.
 * \param getter  The getter accessor to the property; this can be a pointer to a function or a std::function.
 * \param setter  The setter accessor to the property; this can be a pointer to a function or a std::function.
 * \param data    Additional meta data.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for this class, otherwise it will not be registered.
 */
template<typename A1, typename A2, typename Policy>
void property_(const std::string& name, A1 getter, A2 setter,
#ifndef DOXYGEN
               std::vector< rttr::metadata > data,  const Policy& policy
#endif
);

/*!
 * \brief Register a property with read only access.
 *
 * \param name  The name of the property.
 * \param acc   The accessor to the property; this can be a pointer to a variable,
 *              a pointer to a function or a std::function.
 * \param data  Additional meta data.
 *
 * \remark The name of the property has to be unique for a namespace, otherwise it will not be registered.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename A>
void property_readonly_(const std::string& name, A acc);

/*!
 * \brief Register a property with read only access.
 *
 * \param name  The name of the property.
 * \param acc   The accessor to the property; this can be a pointer to a variable,
 *              a pointer to a function or a std::function.
 * \param data  Additional meta data.
 *
 * \remark The name of the property has to be unique for a namespace, otherwise it will not be registered.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename A>
void property_readonly_(const std::string& name, A acc, std::vector< rttr::metadata > data);

/*!
 * \brief Register a property with read only access.
 *
 * \param name   The name of the property.
 * \param acc    The accessor to the property; this can be a pointer to a variable,
 *               a pointer to a function or a std::function.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for a namespace, otherwise it will not be registered.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename A, typename Policy>
void property_readonly_(const std::string& name, A acc, const Policy& policy);

/*!
 * \brief Register a property with read only access.
 *
 * \param name   The name of the property.
 * \param acc    The accessor to the property; this can be a pointer to a variable,
 *               a pointer to a function or a std::function.
 * \param data   Additional meta data.
 * \param policy The policies parameter describes how the property should be binded to the type system.
 *
 * \remark The name of the property has to be unique for a namespace, otherwise it will not be registered.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename A, typename Policy>
void property_readonly_(const std::string& name, A acc, std::vector< rttr::metadata > data, const Policy& policy);

/*!
 * \brief Register a free function.
 *
 * \param name      The name of the method.
 * \param function  The function accessor to this method; this can be a member function, a function or a std::function.
 *
 * \remark The method name does not have to be unique for this class.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename F>
void method_(const std::string& name, F function);

/*!
 * \brief Register a free function.
 *
 * \param name      The name of the method.
 * \param function  The function accessor to this method; this can be a member function, a function or a std::function.
 * \param data      Additional meta data.
 *
 * \remark The method name does not have to be unique for this class.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename F>
void method_(const std::string& name, F function, std::vector< rttr::metadata > data);

/*!
 * \brief Register a free function.
 *
 * \param name      The name of the method.
 * \param function  The function accessor to this method; this can be a member function, a function or a std::function.
 * \param policy    The policies parameter describes how the method should be binded to the type system.
 *
 * \remark The method name does not have to be unique for this class.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename F, typename Policy>
void method_(const std::string& name, F function, const Policy& policy);

/*!
 * \brief Register a free function.
 *
 * \param name      The name of the method.
 * \param function  The function accessor to this method; this can be a member function, a function or a std::function.
 * \param data      Additional meta data.
 * \param policy    The policies parameter describes how the method should be binded to the type system.
 *
 * \remark The method name does not have to be unique for this class.
 *
 * \return Reference to this, in order to chain other calls.
 */
template<typename F, typename Policy>
void method_(const std::string& name, F function, std::vector< rttr::metadata > data, const Policy& policy);

/*!
 * \brief Register an enumeration of type \p EnumType
 *
 * \param enum_data The name of the property.
 * \param data      Additional meta data.
 *
 * \remark Before using this make sure you have registered the type with \ref #RTTR_DECLARE_TYPE.
 */
template<typename EnumType>
void enumeration_(std::vector< std::pair< std::string, EnumType> > enum_data,
                  std::vector< rttr::metadata > data = std::vector< rttr::metadata >());



#ifdef DOXYGEN
/*!
 * \brief Use this macro to automatically register your reflection information to RTTR before `main` is called.
 *
 * Use it in following way:
\code{.cpp}
RTTR_REGISTER
{
  rttr::method_("foo", &foo);
}
\endcode
 *
 * Just place the macro in global scope in a cpp file. 
 *
 * \remark It is not possible to place the macro multiple times in one cpp file.
 *
 */
#define RTTR_REGISTER

/*!
 * \brief Place this macro inside a class, when you need to reflect properties or methods 
 *        which are declared in private scope of the class.
 *
\code{.cpp}
class Foo
{
    private:
        int m_value;

    RTTR_REGISTER_FRIEND
};
\endcode

\code{.cpp}
RTTR_REGISTER
{
  class_<Foo>()
    .property("value", &Foo::m_value);
}
\endcode
 */
#define RTTR_REGISTER_FRIEND
#endif

} // end namespace rttr

#include "rttr/impl/register_reflection_impl.h"

#endif //__RTTR_REGISTER_REFLECTION_H__
