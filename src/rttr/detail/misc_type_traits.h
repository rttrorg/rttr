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

#ifndef __RTTR_MISC_TYPE_TRAITS_H__
#define __RTTR_MISC_TYPE_TRAITS_H__

#include "rttr/base/core_prerequisites.h"

#include "rttr/detail/array_mapper.h"

#include <type_traits>

namespace rttr
{

class type;

namespace detail
{
    struct derived_info;

    template<class T>
    struct raw_type { typedef T type; };

    template<class T> struct raw_type<const T>      { typedef typename raw_type<T>::type type; };

    template<class T> struct raw_type<T*>           { typedef typename raw_type<T>::type type; };
    template<class T> struct raw_type<T* const>     { typedef typename raw_type<T>::type type; };
    template<class T> struct raw_type<T* volatile>  { typedef typename raw_type<T>::type type; };

    template<class T> struct raw_type<T&>           { typedef typename raw_type<T>::type type; };
    template<class T, std::size_t N> 
    struct raw_type<const T[N]>                     { typedef typename raw_type<T[N]>::type type; };


    /////////////////////////////////////////////////////////////////////////////////////////

    template <bool... b> struct static_all_of;
    //specialization for type true, go continue recurse if the first argument is true
    template <bool... tail> 
    struct static_all_of<true, tail...> : static_all_of<tail...> {};
    // end recursion if first argument is false
    template <bool... tail> 
    struct static_all_of<false, tail...> : std::false_type {};

    // finish when no argument are left
    template <> struct static_all_of<> : std::true_type {};

    // use it like e.g.:
    // static_all_of<std::is_class<ClassType>::value...>::value

    /////////////////////////////////////////////////////////////////////////////////////////

    template <bool... b> struct static_any_of;

    template <bool... tail> 
    struct static_any_of<true, tail...> : std::true_type {};

    template <bool... tail> 
    struct static_any_of<false, tail...> : static_any_of<tail...> {};

    // finish when no argument are left
    template <> struct static_any_of<> : std::false_type {};

    /////////////////////////////////////////////////////////////////////////////////////////
    /*! 
     * Determine if the given type \a T has the method
     * 'type get_type() const' declared.
     */
    template <typename T>
    class has_get_type_func_impl
    {
        typedef char YesType[1];
        typedef char NoType[2];

        template <typename U, rttr::type (U::*)() const> 
        class check { };

        template <typename C>
        static YesType& f(check<C, &C::get_type>*);

        template <typename C>
        static NoType& f(...);

    public:
        static const bool value = (sizeof(f<typename raw_type<T>::type>(0)) == sizeof(YesType));
    };

    /*!
     * If T has a member function 'type get_type() const;' then inherits from true_type, otherwise inherits from false_type. 
     */
    template<class T, typename Enable = void>
    struct has_get_type_func : std::false_type
    {};

    template<class T>
    struct has_get_type_func<T, typename std::enable_if<has_get_type_func_impl<T>::value>::type > : std::true_type
    {};

    /////////////////////////////////////////////////////////////////////////////////
    
    /*! 
     * Determine if the given type \a T has the method
     * 'type get_type() const' declared.
     */
    template <typename T>
    class has_get_ptr_func_impl
    {
        typedef char YesType[1];
        typedef char NoType[2];

        template <typename U, void* (U::*)()> 
        class check { };

        template <typename C>
        static YesType& f(check<C, &C::get_ptr>*);

        template <typename C>
        static NoType& f(...);

    public:
        static const bool value = (sizeof(f<typename raw_type<T>::type>(0)) == sizeof(YesType));
    };

    /*!
     * If T has a member function 'type get_type() const;' then inherits from true_type, otherwise inherits from false_type. 
     */
    template<class T, typename Enable = void>
    struct has_get_ptr_func : std::false_type
    {};

    template<class T>
    struct has_get_ptr_func<T, typename std::enable_if<has_get_ptr_func_impl<T>::value>::type > : std::true_type
    {};

    /////////////////////////////////////////////////////////////////////////////////
    
    /*! 
     * Determine if the given type \a T has the method
     * 'type get_type() const' declared.
     */
    template <typename T>
    class has_get_derived_info_func_impl
    {
        typedef char YesType[1];
        typedef char NoType[2];

        template <typename U, derived_info (U::*)()> 
        class check { };

        template <typename C>
        static YesType& f(check<C, &C::get_derived_info>*);

        template <typename C>
        static NoType& f(...);

    public:
        static const bool value = (sizeof(f<typename raw_type<T>::type>(0)) == sizeof(YesType));
    };

    /*!
     * If T has a member function 'type get_type() const;' then inherits from true_type, otherwise inherits from false_type. 
     */
    template<class T, typename Enable = void>
    struct has_get_derived_info_func : std::false_type
    {};

    template<class T>
    struct has_get_derived_info_func<T, typename std::enable_if<has_get_derived_info_func_impl<T>::value>::type > : std::true_type
    {};

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct get_ptr_impl
    {
        static RTTR_INLINE void* get(T& data)
        {
            return const_cast<void*>(reinterpret_cast<const void*>(&data));
        }
    };

    template<typename T>
    struct get_ptr_impl<T*>
    {
        static RTTR_INLINE void* get(T* data)
        {
            return get_ptr_impl<T>::get(*data);
        }
    };

    template<>
    struct get_ptr_impl<void*>
    {
        static RTTR_INLINE void* get(void* data)
        {
            return data;
        }
    };

    template<>
    struct get_ptr_impl<const void*>
    {
        static RTTR_INLINE void* get(const void* data)
        {
            return const_cast<void*>(data);
        }
    };

    template<typename T>
    static RTTR_INLINE void* get_void_ptr(T* data)
    {
        return get_ptr_impl<T*>::get(data);
    }

    template<typename T>
    static RTTR_INLINE void* get_void_ptr(T& data)
    {
        return get_ptr_impl<T>::get(data);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    template<typename T, typename... Types>
    struct contains : static_any_of<std::is_same<T, Types>::value...>
    {
    };

    template<typename T, typename... Types, template<class...> class TContainer>
    struct contains<T, TContainer<Types...>> : contains<T, Types...>
    {
    };

    /////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_array_impl
    {
        typedef char YesType[1];
        typedef char NoType[2];

        template <typename U> static NoType& check(typename U::no_array_type*);
        template <typename U> static YesType& check(...);


        static const bool value = (sizeof(check<array_mapper<T> >(0)) == sizeof(YesType));
    };

    template<class T>
    struct is_array : std::conditional<is_array_impl<T>::value,
                                       std::true_type,
                                       std::false_type>::type
    {};

    /////////////////////////////////////////////////////////////////////////////////////
    // rank_type<T, size_t>::type
    //
    // rank_type<int[2][10][4], 0>::type => int[2][10][4]
    // rank_type<int[2][10][4], 1>::type => int[10][4]
    // rank_type<int[2][10][4], 2>::type => int[4]
    // works of course with all other class, which has an array_mapper specialization

    template <typename... T>
    struct concat_array_types;


    template <template <typename ...> class List, typename ...Types, typename T>
    struct concat_array_types<List<Types...>, T, std::true_type>
    {
        using type = List<Types...>;
    };

    template <template <typename... > class List, typename... Types, typename T>
    struct concat_array_types<List<Types...>, T, std::false_type>
    {
        using sub_type = typename array_mapper<T>::sub_type;
        using type = typename concat_array_types< List<Types..., T>, sub_type, typename std::is_same<T, sub_type>::type>::type;
    };
 
    template<typename T>
    struct array_rank_type_list
    {
        using sub_type = typename array_mapper<T>::sub_type;
        using types = typename concat_array_types< std::tuple<>, T, typename std::is_same<T, sub_type>::type>::type;
    };

    template<typename T, size_t N>
    struct rank_type
    {
        using type = typename std::tuple_element<N, typename array_rank_type_list<T>::types>::type;
    };
    
    /////////////////////////////////////////////////////////////////////////////////////
    // rank<T>::value
    //
    // rank<int[2][10][4]>::value => 3
    // rank<std::vector<std::vector<int>>>::value => 2
     template <typename... T>
     struct rank_impl
     {
         using type = typename std::integral_constant<std::size_t, 0>::type;
     };

     template <template <typename... > class List, typename... Types>
     struct rank_impl<List<Types...>>
     {
         using type = typename std::integral_constant<std::size_t, sizeof...(Types) - 1>::type;
     };

    template<typename T>
    using rank = typename rank_impl< typename detail::array_rank_type_list<T>::types >::type;

    /////////////////////////////////////////////////////////////////////////////////////
    // pointer_count<T>::value Returns the number of pointers for a type
    // e.g. pointer_count<char**>::value => 2
    //      pointer_count<char*>::value  => 1
    //      pointer_count<char>::value   => 0
    template<typename T>
    struct pointer_count_impl
    {
        static const std::size_t size = 0;
    };


    template<typename T>
    struct pointer_count_impl<T*>
    {
        static const std::size_t size = pointer_count_impl<T>::size + 1;
    };

    //template<typename T>
    //using pointer_count = std::integral_constant<std::size_t, pointer_count_impl<T>::size>;
    template<typename T>
    struct pointer_count : std::integral_constant<std::size_t, pointer_count_impl<T>::size>
    {};

    /////////////////////////////////////////////////////////////////////////////////////
    // is_char_array<T>::value Returns true if the given typ is a char array
    // e.g. is_char_array<char[10]>::value => true
    //      is_char_array<int[10]>::value => false
    //      is_char_array<char>::value => false
    template<typename T>
    struct char_array_impl : std::false_type
    {
    };


    template<std::size_t N>
    struct char_array_impl<char[N]> : std::true_type
    {
    };

    template<typename T>
    using is_char_array = char_array_impl<T>;

    template<typename T>
    struct is_one_dim_char_array : std::integral_constant<bool, is_char_array<T>::value && (std::rank<T>::value == 1)>
    {};

    template<typename T>
    struct is_array_and_not_one_dim_char_array : std::integral_constant<bool, std::is_array<T>::value && !is_one_dim_char_array<T>::value>
    {
        
    };

    /////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail

} // end namespace rttr

#endif // __RTTR_MISC_TYPE_TRAITS_H__
