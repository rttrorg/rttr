/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_UTILITY_H_
#define RTTR_UTILITY_H_

#include "rttr/detail/base/core_prerequisites.h"

#include "rttr/detail/misc/std_type_traits.h"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <algorithm>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// This will add the c++14 integer sequence to c++11

    template <class T, T... I>
    struct integer_sequence
    {
        template <T N> using append = integer_sequence<T, I..., N>;
        static std::size_t size() { return sizeof...(I); }
        using next = append<sizeof...(I)>;
        using type = T;
    };

    template <class T, T Index, std::size_t N>
    struct sequence_generator
    {
        using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
    };

    template <class T, T Index>
    struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

    template <std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;

#if RTTR_COMPILER == RTTR_COMPILER_MSVC && RTTR_COMP_VER <= 1800
    // workaround for a compiler bug of nested aliases (#1085630)
    template <class T, T N>
    struct make_integer_sequence_impl
    {
        using type = typename sequence_generator<T, N, N>::type;
    };

    template <class T, T N>
    struct make_index_sequence_impl
    {
        using type = typename make_integer_sequence_impl<T, N>::type;
    };

    template <class T, T N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

    template <std::size_t N>
    using make_index_sequence = typename make_integer_sequence_impl<std::size_t, N>::type;
#else
    template <class T, T N>
    using make_integer_sequence = typename sequence_generator<T, N, N>::type;

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;
#endif

    template<class... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// will remove all numbers of the given index sequence till the given \p Index.
// typename erase_sequence_till<index_sequence<0, 1, 2, 3, 4>, 3>::type =>   index_sequence<3, 4>

template<typename Index_Sequence, std::size_t Index>
struct erase_sequence_till;

template<std::size_t Index, std::size_t Idx, std::size_t... I>
struct erase_sequence_till<index_sequence<Idx, I...>, Index>
{
    using type = conditional_t< Idx == Index,
                                index_sequence<Idx, I...>,
                                typename erase_sequence_till<index_sequence<I...>, Index>::type>;
};

template<std::size_t Index>
struct erase_sequence_till<index_sequence<>, Index>
{
    using type = index_sequence<>;
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct remove_first_index_impl
{
    using type = index_sequence<>;
};

template<std::size_t First, std::size_t... I>
struct remove_first_index_impl<detail::index_sequence<First, I...>>
{
    using type = detail::index_sequence<I...>;
};

template<typename T>
using remove_first_index = typename remove_first_index_impl<T>::type;


/////////////////////////////////////////////////////////////////////////////////////////

template<typename, typename>
struct concat_index_sequence { };

template<std::size_t... Ts, std::size_t... Us>
struct concat_index_sequence<index_sequence<Ts...>, index_sequence<Us...>>
{
    using type = index_sequence<Ts..., Us...>;
};

/////////////////////////////////////////////////////////////////////////////////////////

template <class T>
struct remove_last_index_impl;

template <size_t Last>
struct remove_last_index_impl<index_sequence<Last>>
{
    using type = index_sequence<>;
};

template<std::size_t First, std::size_t... I>
struct remove_last_index_impl<index_sequence<First, I...>>
{
    using type = typename concat_index_sequence<
                                                 index_sequence<First>,
                                                 typename remove_last_index_impl<index_sequence<I...>>::type
                                                >::type;
};

template<typename T>
using remove_last_index = typename remove_last_index_impl<T>::type;



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static RTTR_FORCE_INLINE bool check_all_true() { return true; }

template<typename... BoolArgs>
static RTTR_INLINE bool check_all_true(bool arg1, BoolArgs... args) { return arg1 & check_all_true(args...); }

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// copy the content of any arbitrary array, use it like: 
// copy_array(in, out);
// works with every dimension

template<typename ElementType>
struct copy_array_impl
{
    void operator()(const ElementType &in, ElementType &out)
    {
        out = in;
    }
};

template<typename ElementType, std::size_t Count>
struct copy_array_impl<ElementType[Count]>
{
    void operator()(const ElementType (&in)[Count], ElementType (&out)[Count])
    {
        for(std::size_t i = 0; i < Count; ++i)
            copy_array_impl<ElementType>()(in[i], out[i]);
    }
};

template<typename ElementType, std::size_t Count>
auto copy_array(const ElementType (&in)[Count], ElementType (&out)[Count])
    -> ElementType (&)[Count]
{
    copy_array_impl<ElementType[Count]>()(in, out);
    return out;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// compare whether two arrays are the same or not
// the comparison will go down till element wise comparison

template<typename ElementType>
struct compare_array_equal_impl
{
    bool operator()(const ElementType &lhs, const ElementType &rhs)
    {
        return (lhs == rhs);
    }
};

template<typename ElementType, std::size_t Count>
struct compare_array_equal_impl<ElementType[Count]> 
{
    bool operator()(const ElementType (&lhs)[Count], const ElementType (&rhs)[Count])
    {
        for(std::size_t i = 0; i < Count; ++i)
        {
            if (!compare_array_equal_impl<ElementType>()(lhs[i], rhs[i]))
                return false;
        }

        return true;
    }
};

template<typename ElementType, std::size_t Count>
bool compare_array_equal(const ElementType (&lhs)[Count], const ElementType (&rhs)[Count])
{
    return compare_array_equal_impl<ElementType[Count]>()(lhs, rhs);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// compare whether array lhs is less than rhs
// the comparison will go down till element wise comparison

template<typename ElementType>
struct compare_array_less_impl
{
    int operator()(const ElementType &lhs, const ElementType &rhs)
    {
        return (lhs < rhs ? - 1 : ((rhs < lhs) ? 1 : 0));
    }
};
 
template<typename ElementType, std::size_t Count>
struct compare_array_less_impl<ElementType[Count]> 
{
    int operator()(const ElementType (&lhs)[Count], const ElementType (&rhs)[Count])
    {
    	int flag = 0;
        for(std::size_t i = 0; i < Count; ++i)
        {
            if ((flag = compare_array_less_impl<ElementType>()(lhs[i], rhs[i])) != 0)
                return flag;
        }
 
        return 0;
    }
};
 
template<typename ElementType, std::size_t Count>
bool compare_array_less(const ElementType (&lhs)[Count], const ElementType (&rhs)[Count])
{
    if (compare_array_less_impl<ElementType[Count]>()(lhs, rhs) == -1)
        return true;

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// make_unqiue implementation for C++11

template<class T> struct _Unique_if 
{
    using _Single_object = std::unique_ptr<T>;
};

template<class T> struct _Unique_if<T[]> 
{
    using _Unknown_bound = std::unique_ptr<T[]>;
};

template<class T, size_t N> struct _Unique_if<T[N]> 
{
    using _Known_bound = void;
};

template<class T, class... Args>
typename _Unique_if<T>::_Single_object
make_unique(Args&&... args) 
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
typename _Unique_if<T>::_Unknown_bound
make_unique(size_t n) 
{
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new U[n]());
}

template<class T, class... Args>
typename _Unique_if<T>::_Known_bound
make_unique(Args&&...) = delete;
        
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
RTTR_INLINE const T& as_const(T& obj) { return const_cast<T&>(obj); }

template< typename T >
RTTR_INLINE const T& as_const(const T& obj) { return obj; }

template<typename T>
RTTR_INLINE const T as_const(T&& obj) 
{
    static_assert(!std::is_const<T>::value, "The given obj is already const, moving a const RValue will result in a copy!");
    return std::forward<T>(obj); 
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE typename std::enable_if<std::is_pointer<T>::value, void*>::type as_void_ptr(const T& obj) 
{
    return const_cast<void*>(reinterpret_cast<const volatile void*>(obj));
}


template<typename T>
RTTR_FORCE_INLINE typename std::enable_if<!std::is_pointer<T>::value, void*>::type as_void_ptr(const T& obj) 
{
    return const_cast<void*>(reinterpret_cast<const volatile void*>(&obj));
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * Helper class to iterate in a ranged-based for loops backwards through a container.
 * use it like following: 
 * \code{.cpp}
 *   for(const auto& value: reverse(my_vector))
 *      std::cout << value << std::endl;
 * \endcode                
 */
template<class T>
class reverse_wrapper
{
    public:
        reverse_wrapper(T& container) : m_container(container) { }
        decltype(std::declval<T>().rbegin()) begin() { return m_container.rbegin(); }
        decltype(std::declval<T>().rend()) end() { return m_container.rend(); }

        decltype(std::declval<T>().crbegin()) begin() const { return m_container.crbegin(); }
        decltype(std::declval<T>().crend()) end() const { return m_container.crend(); }

    private:
        T& m_container;
};

template<class T>
class reverse_move_wrapper
{
    public:
        reverse_move_wrapper(T&& container) : m_container(std::move(container)) { }
        decltype(std::declval<T>().rbegin()) begin() { return m_container.rbegin(); }
        decltype(std::declval<T>().rend()) end() { return m_container.rend(); }
        decltype(std::declval<T>().crbegin()) begin() const { return m_container.crbegin(); }
        decltype(std::declval<T>().crend()) end() const { return m_container.crend(); }

    private:
        T m_container;
};


template<class T>
reverse_move_wrapper<T> reverse(T&& container) 
{
    return reverse_move_wrapper<T>(std::forward<T>(container));
}

template<class T>
const reverse_move_wrapper<const T> reverse(const T&& container) 
{
    return reverse_move_wrapper<const T>(std::forward<const T>(container));
}

template<class T>
reverse_wrapper<T> reverse(T& container) 
{
     return reverse_wrapper<T>(container);
}

template<class T>
const reverse_wrapper<const T> reverse(const T& container) 
{
     return reverse_wrapper<const T>(container);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
        
template<typename T>
using return_type_normal = add_pointer_t< remove_pointers_t<T> >;

template<typename T>
using raw_addressof_return_type = std::conditional< is_function_ptr< remove_pointers_except_one_t<T> >::value,
                                                    add_pointer_t< remove_pointers_except_one_t<T> >,
                                                    return_type_normal<T> >;


template<typename T>
using raw_addressof_return_type_t = typename raw_addressof_return_type<T>::type;

template<typename T, typename Enable = void>
struct raw_addressof_impl
{
    static RTTR_INLINE raw_addressof_return_type_t<T> get(T& data)
    {
        return std::addressof(data);
    }
};

template<typename T>
using is_raw_void_pointer = std::is_same<void*, add_pointer_t< raw_type_t<T> > >;

template<typename T>
using is_void_pointer = std::integral_constant<bool, std::is_pointer<T>::value && is_raw_void_pointer<T>::value && pointer_count<T>::value == 1>;

template<typename T>
struct raw_addressof_impl<T, enable_if_t<(std::is_pointer<T>::value && pointer_count<T>::value >= 1 && !is_void_pointer<T>::value) ||
                                         (pointer_count<T>::value == 1 && std::is_member_pointer<remove_pointer_t<T> >::value)
                                         > >
{
    static RTTR_INLINE raw_addressof_return_type_t<T> get(T& data)
    {
        return raw_addressof_impl< remove_pointer_t<T> >::get(*data);
    }
};

template<typename T>
struct raw_addressof_impl<T, enable_if_t<is_void_pointer<T>::value> >
{
    static RTTR_INLINE raw_addressof_return_type_t<T> get(T& data)
    {
        return data; // void pointer cannot be dereferenced to type "void"
    }
};

/*!
 * \brief This function will return from its raw type \p T
 *        its address as pointer.
 *
 * \see detail::raw_type
 *
 * \return The address of the raw type from the given object \p data as pointer.
 */
template<typename T>
static RTTR_INLINE raw_addressof_return_type_t<T> raw_addressof(T& data)
{
    return raw_addressof_impl<T>::get(data);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
 
/*!
 * The \ref move_wrapper class wraps a move-only type in a copyable object.
 *
 */
template<typename T>
struct move_wrapper
{
    move_wrapper(T&& value) : m_value(std::move(value)) { }

    move_wrapper(const move_wrapper& other) : m_value(std::move(other.m_value)) { }

    move_wrapper(move_wrapper&& other) : m_value(std::move(other.m_value)) { }

    move_wrapper& operator=(const move_wrapper& other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    move_wrapper& operator=(move_wrapper&& other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    mutable T m_value;
};

/*!
 * \brief This function creates an instance of a move_wrapper class for type \p T.
 *
 * \return An instance of move_wrapper<T>.
 */
template<typename T>
static RTTR_INLINE move_wrapper<T> make_rref(T&& value)
{
    return {std::move(value)};
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief When the given shared_ptr \p obj is empty, it will create a new default one,
 *        otherwise returns a copy.
 */
template<typename T>
static RTTR_INLINE std::shared_ptr<T> create_if_empty(const std::shared_ptr<T>& obj)
{
    return (obj.get() ? obj : std::make_shared<T>());
}

} // end namespace detail
} // end namespace rttr

#endif //RTTR_UTILITY_H_
