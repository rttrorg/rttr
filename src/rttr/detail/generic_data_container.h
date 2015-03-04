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

#ifndef __RTTR_GENERIC_DATA_CONTAINER_H__
#define __RTTR_GENERIC_DATA_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"

namespace rttr
{

namespace detail
{

//! This class declaration is just used for declaring a member function typedef.
class void_helper_class
{
};
typedef void (*void_func_ptr)();
typedef void (void_helper_class::*void_mem_func_ptr)();
typedef char (void_helper_class::*void_mem_obj_ptr);

/*!
 * \brief The following code is used for the function bool variant::convert(const type& target_type).
 *
 * With the create_variant function it is possible to perform a rttr_cast internally in variant.
 * So basically a conversion of source_type* to target_type*.
 *
 * Template arguments cannot be forwarded at runtime to some derived or base classes, 
 * thats why we have to work with a base type, which is void*.
 */
 
union RTTR_API generic_data_container
{
    void*               m_obj_ptr;      //!< Stores a pointer to an object.
    void_mem_obj_ptr    m_mem_obj_ptr;  //!< Stores a pointer to an member object.
    void_func_ptr       m_func_ptr;     //!< Stores a pointer to a function.
    void_mem_func_ptr   m_mem_func_ptr; //!< Stores a pointer to a member function.
};

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_GENERIC_DATA_CONTAINER_H__
