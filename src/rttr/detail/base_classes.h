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

#ifndef __RTTR_BASE_CLASSES_H__
#define __RTTR_BASE_CLASSES_H__


namespace rttr
{

namespace detail
{
    
struct base_class_info
{
    base_class_info(type t,void*(*rttr_cast_func)(void*))
    : _base_type(t), _rttr_cast_func(rttr_cast_func)
    {}
    type            _base_type;
    void*           (*_rttr_cast_func)(void*);
};

/*!
 * This trait checks if a given type T has a typedef named \a base_class_list.
 * has_base_class_list_impl::value is true, when it has this type, otherwise false.
 */
template <typename T>
class has_base_class_list_impl
{
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename C>
    static YesType& test(typename C::base_class_list*);

    template <typename>
    static NoType& test(...);

public:
    static const bool value = (sizeof(YesType) == sizeof(test<T>(0)));
};

/*!
 * If T has a typedef called \a 'base_class_list' then inherits from true_type, otherwise inherits from false_type. 
 */
template<typename T, typename = void>
struct has_base_class_list : std::integral_constant<bool, false> 
{};

template<typename T>
struct has_base_class_list<T, typename std::enable_if<has_base_class_list_impl<T>::value>::type > : std::integral_constant<bool, true>
{};

typedef std::vector<detail::base_class_info> info_container;

/*!
 * This class fills from a given type_list the corresponding type objects into a std::vector.
 */
template<typename DerivedClass, typename... T> 
struct type_from_base_classes;

template<typename DerivedClass>
struct type_from_base_classes<DerivedClass> 
{
    RTTR_INLINE static void fill(info_container&) 
    { 
    }
};

/*!
 * \brief This is the trick to make the replacement of `dynamic_cast` possible.
 *        For every \p DerivedType is a function pointer stored, which performs a
 *        static_cast operation to it's BaseType.
 *        The given \p ptr is a void* ptr and has to be of type \p DerivedType,
 *        otherwise we would get undefined behavior.
 *        Therefore the \ref get_derived_info() function is used. It will return this information.
 */
template<typename DerivedType, typename BaseType>
static void* rttr_cast_impl(void* ptr)
{
    return static_cast<void*>(static_cast<BaseType*>(static_cast<DerivedType*>(ptr)));
}

template<typename DerivedClass, typename BaseClass, typename... U> 
struct type_from_base_classes<DerivedClass, BaseClass, U...>
{
    RTTR_INLINE static void fill(info_container& vec)
    {
        static_assert(has_base_class_list<BaseClass>::value, "The parent class has no base class list defined - please use the macro RTTR_ENABLE");
        vec.emplace_back(type::get<BaseClass>(), &rttr_cast_impl<DerivedClass, BaseClass>);
        // retrieve also the types of all base classes of the base classes; you will get an compile error here, 
        // when the base class has not defined the 'base_class_list' typedef 
        type_from_base_classes<DerivedClass, typename BaseClass::base_class_list>::fill(vec);
        // continue with the rest
        type_from_base_classes<DerivedClass, U...>::fill(vec);
    }
};

template<typename DerivedClass, class... BaseClassList, template<class...> class ClassContainer>
struct type_from_base_classes<DerivedClass, ClassContainer<BaseClassList...>> : type_from_base_classes<DerivedClass, BaseClassList...> { };

/*!
 * This helper trait returns a vector with type object of all base classes.
 * When there is no type_list defined or the class has no base class, an empty vector is returned.
 */
template<typename ClassType>
struct base_classes
{
    private:
        // extract the info
        RTTR_INLINE static void get_types_impl(info_container& vec, std::true_type)
        {
            type_from_base_classes<ClassType, typename ClassType::base_class_list>::fill(vec);
        }

        // no type list defined
        RTTR_INLINE static void get_types_impl(info_container&, std::false_type)
        {
        }
    public:
        RTTR_INLINE static info_container get_types()
        {
            info_container result;
            get_types_impl(result, typename has_base_class_list<ClassType>::type());
            return result;
        }
};

} // end namespace detail

} // end namespace rttr

#endif // __RTTR_BASE_CLASSES_H__
