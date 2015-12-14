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

#ifndef RTTR_ARRAY_MAPPER_H_
#define RTTR_ARRAY_MAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <cstddef>
#include <vector>
#include <list>
#include <array>

namespace rttr
{
namespace detail
{
struct invalid_array_type {};
}

//////////////////////////////////////////////////////////////////////////////////////

/*!
 * The \ref array_mapper class is a class template to access different array types via one common interface.
 *
 * This class will be only used **internally** by RTTR via the \ref variant_array_view class to get access to elements
 * of an array. In order to use your own custom array type, you have to provide a specialization of this class.
 *
 * Out of the box, RTTR has specialization for following array types:
 * - \p `T[N]`, raw arrays
 * - \p `std::vector<T>` 
 * - \p `std::array<T, N>`
 * - \p `std::list<T>`
 *
 * Custom array types
 * --------------------
 * For a specialization of the class \ref rttr::array_mapper<T> "array_mapper<T>" you have to provide two nested alias templates:
 * 1. `using raw_type = typename array_mapper<T>::raw_type;`
 * 2. `using sub_type = T;`
 *
 * and following member functions:
 * 1. `static bool is_dynamic();`
 * 2. `static bool set_size(array_type& arr, std::size_t new_size);`
 * 3. `static const T& get_value(const array_type& arr, std::size_t index);`
 * 4. `static T& get_value(array_type& arr, std::size_t index);`
 * 5. `static bool insert_value(array_type& arr, const T& value, std::size_t index);`
 * 6. `static bool remove_value(array_type& arr, std::size_t index);`
 * 
 *
 * Following code example for `std::vector<T>` illustrates how to add a specialization:
 *
 * \code{.cpp}
 *  namespace rttr
 *  {
 *  template <typename T>
 *  struct array_mapper<std::vector<T> >
 *  {
 *    using raw_type = typename array_mapper<T>::raw_type;
 *    using sub_type = T;
 *    
 *    static bool is_dynamic()
 *    {
 *        return true;
 *    }
 *    
 *    static std::size_t get_size(const std::vector<T>& arr)
 *    {
 *        return arr.size();
 *    }
 *    
 *    static bool set_size(std::vector<T>& arr, std::size_t new_size)
 *    {
 *        arr.resize(new_size);
 *        return true;
 *    }
 *    
 *    static const T& get_value(const std::vector<T>& arr, std::size_t index)
 *    {
 *        return arr[index];
 *    }
 *    
 *    static T& get_value(std::vector<T>& arr, std::size_t index)
 *    {
 *        return arr[index];
 *    }
 *    
 *    static bool insert_value(std::vector<T>& arr, const T& value, std::size_t index)
 *    {
 *        arr.insert(arr.begin() + index, value);
 *        return true;
 *    }
 *    
 *    static bool remove_value(std::vector<T>& arr, std::size_t index)
 *    {
 *        arr.erase(arr.begin() + index);
 *        return true;
 *    }
 *  };
 *  } // end namespace rttr
 * \endcode
 *
 * \remark
 * Make sure you put your specialization inside the namespace `rttr`. 
 * The best place for this code, is below the declaration of your custom array type.
 * When this is not possible, include your specialization code before registering your types to RTTR.
 *
 */
template <typename T>
struct array_mapper
{
#ifndef DOXYGEN
    using no_array_type = detail::invalid_array_type;
    using raw_type = T;
    using sub_type = T;
#else
    using array_type = T;
    using raw_type = typename array_mapper<T>::raw_type;
    using sub_type = T;
    
    static bool is_dynamic()
    {
    }

    static std::size_t get_size(const array_type&)
    {
    }

    static bool set_size(array_type& arr, std::size_t new_size)
    {
    }

    static const T& get_value(const array_type& arr, std::size_t index)
    {
    }
    
    static T& get_value(array_type& arr, std::size_t index)
    {
    }

    static bool insert_value(array_type&, const T& value, std::size_t,)
    {
    }

    static bool remove_value(array_type&, std::size_t)
    {
    }
#endif
};

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#include "rttr/detail/array/array_mapper_impl.h"

#endif // RTTR_ARRAY_MAPPER_H_
