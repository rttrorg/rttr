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

#ifndef RTTR_VARIANT_DATA_POLICY_H_
#define RTTR_VARIANT_DATA_POLICY_H_

#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/variant/variant_data.h"
#include "rttr/detail/misc/argument_wrapper.h"
#include "rttr/detail/variant_array/variant_array_creator.h"

#include <cstdint>

namespace rttr
{
namespace detail
{

template<typename T>
struct variant_data_policy_big;
template<typename T>
struct variant_data_policy_small;
template<typename T>
struct variant_data_policy_array_small;
template<typename T>
struct variant_data_policy_array_big;

struct void_variant_type;
struct variant_data_policy_void;

template<typename T>
struct variant_data_policy_string;

template<typename T, typename Converter_Class>
struct variant_data_policy_arithmetic;

template<typename T>
struct variant_data_converter;

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, bool Can_Place = (sizeof(T) <= sizeof(variant_data)) && 
                                      (std::alignment_of<T>::value <= std::alignment_of<variant_data>::value)>
using can_place_in_variant = std::integral_constant<bool, Can_Place>;

/*!
 * This template alias will determine the manager class for the given type \p T which is placed inside the variant.
 *
 * Small types will be placed inside the variant without explicit allocation of memory.
 * This will improve the performance a lot.
 *
 * \remark Raw char arrays with rank equal to one, will be copied to a std::string.
 *
 * \return The manager class for the type T.
 */
template<typename T>
using variant_policy = conditional_t<std::is_same<T, void_variant_type>::value,
                                     variant_data_policy_void,
                                     conditional_t<std::is_same<T, std::string>::value || is_one_dim_char_array<T>::value,
                                                   variant_data_policy_string<variant_data_converter<std::string>>,
                                                   conditional_t<can_place_in_variant<T>::value,
                                                                 conditional_t<std::is_arithmetic<T>::value,
                                                                               variant_data_policy_arithmetic<T, variant_data_converter<T>>,
                                                                               conditional_t<std::is_array<T>::value,
                                                                                             variant_data_policy_array_small<T>,
                                                                                             variant_data_policy_small<T>
                                                                                            >
                                                                              >,
                                                                  conditional_t<std::is_array<T>::value,
                                                                                variant_data_policy_array_big<T>,
                                                                                variant_data_policy_big<T>
                                                                               >
                                                                >
                                                  >
                                    >;

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief This enum is used in the invoke function to delegate
 *        the execution of certain operations.
 *
 */
enum class variant_policy_operation : uint8_t
{
    DESTROY,
    CLONE,
    GET_VALUE,
    GET_TYPE,
    GET_PTR,
    GET_RAW_TYPE,
    GET_RAW_PTR,
    GET_ADDRESS_CONTAINER,
    IS_ARRAY,
    TO_ARRAY,
    IS_VALID,
    CONVERT
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used for small custom data types.
 *
 * These are types which will fit into the storage of \ref variant_data.
 * The data will be placed with "placement new" inside the variant.
 * That also means the destructor will be called explicitly.
 */
template<typename T>
struct variant_data_policy_small
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        #define EXTRACT_DATA(src) reinterpret_cast<T&>(const_cast<variant_data&>(src))

        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            {
                EXTRACT_DATA(src_data).~T();
                break;
            }
            case variant_policy_operation::CLONE:
            {
                variant_data& dest = arg.get_value<variant_data>();
                new (&dest) T(reinterpret_cast<const T&>(src_data)); 
                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<T>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<typename raw_type<T>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();

                data.m_type                         = type::get< raw_addressof_return_type_t<T> >();
                data.m_wrapped_type                 = type::get< wrapper_address_return_type_t<T> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return ::rttr::detail::is_array<typename raw_type<T>::type>::value;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<variant_array_data&>() = create_variant_array(EXTRACT_DATA(src_data));
                //auto& data = *static_cast<std::tuple<void*, >*>(arg->m_data);
                //arg->m_data = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            default: return false;
        }

        return true;
        #undef EXTRACT_DATA
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {
        new (&dest) T(std::forward<U>(value)); 
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used for custom types that size does NOT fit into variant_data.
 * Then the type will be allocated on the heap and a pointer to the data is stored in variant_data.
 */
template<typename T>
struct variant_data_policy_big
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        #define EXTRACT_DATA(src) *reinterpret_cast<T*&>(const_cast<variant_data&>(src))

        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            {
                delete reinterpret_cast<T*&>(const_cast<variant_data&>(src_data));
                reinterpret_cast<T*&>(const_cast<variant_data&>(src_data)) = nullptr;
                break;
            }
            case variant_policy_operation::CLONE:
            {
                variant_data& dest = arg.get_value<variant_data>();
                reinterpret_cast<T*&>(dest) = new T(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<T>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<typename raw_type<T>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();

                data.m_type                         = type::get< raw_addressof_return_type_t<T> >();
                data.m_wrapped_type                 = type::get< wrapper_address_return_type_t<T> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return ::rttr::detail::is_array<typename raw_type<T>::type>::value;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<variant_array_data&>() = create_variant_array(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            default: return false;
        }

        return true;
        #undef EXTRACT_DATA
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {
        reinterpret_cast<T*&>(dest) = new T(std::forward<U>(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used for small raw array types, which fit in \p variant_data.
 *
 * The array data will be copied into \p variant_data.
 */
template<typename T>
struct variant_data_policy_array_small
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        #define EXTRACT_DATA(src) reinterpret_cast<T&>(const_cast<variant_data&>(src))
        switch (op)
        {
            case variant_policy_operation::DESTROY:
            {
                break;
            }
            case variant_policy_operation::CLONE:
            {
                variant_data& dest = arg.get_value<variant_data>();
    #if RTTR_COMPILER == RTTR_COMPILER_MSVC
    #   if RTTR_COMP_VER <= 1800
                copy_array(const_cast<typename remove_const<T>::type&>(reinterpret_cast<const T&>(src_data)),
                           reinterpret_cast<T&>(dest));
    #   else
            #error "Check new MSVC Compiler!"
    #   endif
    #else
                copy_array(reinterpret_cast<const T&>(src_data),
                           reinterpret_cast<T&>(dest));
    #endif

            break;
           }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<T>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<typename raw_type<T>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();
        
                data.m_type                         = type::get< raw_addressof_return_type_t<T> >();
                data.m_wrapped_type                 = type::get< wrapper_address_return_type_t<T> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return true;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<variant_array_data&>() = create_variant_array(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            default: return false;
        }
    
        return true;
        #undef EXTRACT_DATA
    }


    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {

#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
        copy_array(const_cast<typename remove_const<T>::type&>(value),
                   reinterpret_cast<T&>(dest));
#   else
        #error "Check new MSVC Compiler!"
#   endif
#else
        copy_array(value, reinterpret_cast<T&>(dest));
#endif

    }
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * This policy is used for raw array types of arbitrary rank, which fit NOT into \p variant_data.
 *
 * A copy of the given array value will be allocated on the heap. The pointer to the array is stored in \p variant_data.
 */
template<typename T>
struct variant_data_policy_array_big
{
    static bool invoke(variant_policy_operation op, const variant_data& src_data, argument_wrapper arg)
    {
        using array_dest_type = decltype(new T);
        #define EXTRACT_DATA(src) reinterpret_cast<T&>(*reinterpret_cast<array_dest_type&>(const_cast<variant_data&>(src)))

        switch (op)
        {
            case variant_policy_operation::DESTROY: 
            {
                delete [] reinterpret_cast<array_dest_type&>(const_cast<variant_data&>(src_data));
                reinterpret_cast<array_dest_type&>(const_cast<variant_data&>(src_data)) = nullptr;
                break;
            }
            case variant_policy_operation::CLONE:
            {
                variant_data& dest = arg.get_value<variant_data>();
                reinterpret_cast<array_dest_type&>(dest) = new T;

    #if RTTR_COMPILER == RTTR_COMPILER_MSVC
    #   if RTTR_COMP_VER <= 1800
                copy_array(const_cast<typename remove_const<T>::type&>(EXTRACT_DATA(src_data)),
                           reinterpret_cast<T&>(*reinterpret_cast<array_dest_type&>(dest)));
    #   else
                #error "Check new MSVC Compiler!"
    #   endif
    #else
                copy_array(reinterpret_cast<const T&>(*reinterpret_cast<const array_dest_type&>(src_data)),
                           reinterpret_cast<T&>(*reinterpret_cast<array_dest_type&>(dest)));
    #endif

                break;
            }
            case variant_policy_operation::GET_VALUE:
            {
                arg.get_value<void*>() = &EXTRACT_DATA(src_data);
                break;
            }
            case variant_policy_operation::GET_TYPE:
            {
                arg.get_value<type>() = type::get<T>();
                break;
            }
            case variant_policy_operation::GET_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(std::addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_RAW_TYPE:
            {
                arg.get_value<type>() = type::get<typename raw_type<T>::type>();
                break;
            }
            case variant_policy_operation::GET_RAW_PTR:
            {
                arg.get_value<void*>() = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::GET_ADDRESS_CONTAINER:
            {
                data_address_container& data        = arg.get_value<data_address_container>();

                data.m_type                         = type::get< raw_addressof_return_type_t<T> >();
                data.m_wrapped_type                 = type::get< wrapper_address_return_type_t<T> >();
                data.m_data_address                 = as_void_ptr(raw_addressof(EXTRACT_DATA(src_data)));
                data.m_data_address_wrapped_type    = as_void_ptr(wrapped_raw_addressof(EXTRACT_DATA(src_data)));
                break;
            }
            case variant_policy_operation::IS_ARRAY:
            {
                return true;
            }
            case variant_policy_operation::TO_ARRAY:
            {
                arg.get_value<variant_array_data&>() = create_variant_array(EXTRACT_DATA(src_data));
                break;
            }
            case variant_policy_operation::IS_VALID:
            {
                return true;
            }
            default: return false;
        }

        return true;
        #undef EXTRACT_DATA
    }

    template<typename U>
    static RTTR_INLINE void create(U&& value, variant_data& dest)
    {
        using array_dest_type = decltype(new T);
        reinterpret_cast<array_dest_type&>(dest) = new T;

#if RTTR_COMPILER == RTTR_COMPILER_MSVC
#   if RTTR_COMP_VER <= 1800
        copy_array(const_cast<typename remove_const<T>::type&>(value),
                   reinterpret_cast<T&>(*reinterpret_cast<array_dest_type&>(dest)));
#   else
        #error "Check new MSVC Compiler!"
#   endif
#else
        copy_array(value, reinterpret_cast<T&>(*reinterpret_cast<array_dest_type&>(dest)));
#endif
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_DATA_POLICY_H_
