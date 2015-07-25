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

#ifndef RTTR_VARIANT_ARRAY_H_
#define RTTR_VARIANT_ARRAY_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/misc_type_traits.h"
#include "rttr/detail/variant_array_view/variant_array_view_data.h"
#include "rttr/variant.h"

#include <cstddef>
#include <vector>

namespace rttr
{
    class type;
    class variant_array_view;

namespace detail
{
    class instance;
    class argument;
    enum class variant_array_policy_operation : uint8_t;
    struct argument_wrapper;

    typedef bool (*variant_array_policy_func)(variant_array_policy_operation, void* const &, argument_wrapper);
}

/*!
 * The \ref variant_array_view describes a class that refers to an array inside a \ref variant.
 * With an instance of that class you can set and get values of an array with any dimension level.
 *
 * A \ref variant_array_view can be created directly from a \ref variant with \ref variant::create_array_view().
 *
 * Meta Information
 * ----------------
 * An array is defined by its \ref variant_array_view::get_rank() "rank", it's \ref variant_array_view::get_size "size" and 
 * whether he is \ref variant_array_view::is_dynamic() "dynamic" or not.
 *
 * The rank of an array describes the number of dimensions. E.g. `int[10]` has a rank of `1`. `int[2][10]` has an rank of `2` and so on.
 * For retrieving the size of an array use \ref get_size().
 * With this function it is also possible to determine the size of the array relative to its rank level and it's index.
 * Take a look at following example:
 * \code{.cpp}
 *      variant var = std::vector<std::vector<int>>(10, std::vector<int>(20, 0));
 *      variant_array_view array = var.create_array_view();
 *      std::cout << array.get_size()  << std::endl; // prints "10"
 *      std::cout << array.get_size(0) << std::endl; // prints "20"
 *      std::cout << array.get_size(1) << std::endl; // prints "20"
 *      // INVALID call, max index is 9
 *      std::cout << array.get_size(10) << std::endl; // undefined behavior
 * \endcode
 *
 * When the given array type is \ref variant_array_view::is_dynamic() "dynamic" you can change the size of the array,
 * therefore \ref variant_array_view::set_size "set_size()" should be used.
 * A value of an array can be accessed with \ref variant_array_view::get_value "get_value()" or set with 
 * \ref variant_array_view::set_value "set_value". These function expect an index for up to rank level 3.
 * The array class has here one interesting feature, you can set and get the value of an array up to its rank count. e.g:
 * \code{.cpp}
 *      int obj[2][10];
 *      int sub_obj[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 *      array.set_value(0, sub_obj); // set the content of the obj[0] to zeros
 *      array.set_value(0, 1, 23);   // equivalent to call obj[0][1] == 23
 * \endcode
 *
 * When you have arrays bigger then this rank, use the counterpart functions: 
 * \ref variant_array_view::get_value_variadic "get_value_variadic" and \ref variant_array_view::set_value_variadic "set_value_variadic"
 * which expects a list of indices. When the array is dynamic it is also possible to
 * \ref variant_array_view::insert_value "insert" or \ref variant_array_view::remove_value "remove" values.
 *
 * RTTR recognize whether a type is an array or not with the help of an `array_mapper` class template.
 * This class does the mapping for the standard access function
 * defined in the \ref array class. At the moment there exist specializations for following types: 
 * `std::array<T, N>`, `std::vector<T>`, `std::list<T>` and raw-arrays `T[N]`.
 *
 * When you need to bind an own custom array type, then you have the implement following functions in the class `array_mapper`.
 *
 * \code{.cpp}
 *      namespace rttr
 *      {
 *      namespace detail
 *      {
 *      template <typename T>
 *      struct array_mapper< my_fancy_array<T> >
 *      {
 *        using raw_type = typename array_mapper<T>::raw_type;
 *        using sub_type = T;
 *      
 *        static bool is_dynamic()
 *        {
 *            ...
 *        }
 *        static std::size_t get_size(const my_fancy_array<T>&)
 *        {
 *            ...
 *        }
 *        static bool set_size(my_fancy_array<T>&)
 *        {
 *            ...
 *        }
 *        static const T& get_value(const my_fancy_array<T>& arr, std::size_t index)
 *        {
 *            ...
 *        }
 *        
 *        static T& get_value(my_fancy_array<T>& arr, std::size_t index)
 *        {
 *            ...
 *        }
 *        static bool insert_value(my_fancy_array<T>&, std::size_t, const T&)
 *        {
 *            ...
 *        }
 *        static bool remove_value(my_fancy_array<T>&, std::size_t)
 *        {
 *          ...
 *        }
 *      };
 *      } // end namespace detail
 *      } // end namespace rttr
 * \endcode
 *
 * Remark the namespaces rttr::detail, otherwise your specialization will not be recognized.
 *
 * Copying and Assignment
 * ----------------------
 * A \ref variant_array_view object is can be copied and assigned, however each copy will perform a copy of the contained value.
 *
 * Typical Usage
 * ----------------------
 * 
 * \code{.cpp}
 *      int obj[2][10];
 *      variant var = obj;
 *      if (var.is_array())
 *      {
 *        variant_array_view array = var.to_array();
 *        for (std::size_t index_1 = 0; index_1 < array.get_size(); ++index_1)
 *        {
 *          for (std::size_t index_2 = 0; index_2 < array.get_size(index_1); ++index_2)
 *          {
 *            array.set_value(index_1, index_2, 0);
 *          }
 *        }
 *      
 *        // it is also possible to set the sub array in one step
 *        for (std::size_t index_1 = 0; index_1 < array.get_size(); ++index_1)
 *        {
 *          int zeros[10] = {0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0};
 *          array.set_value(index_1, zeros);
 *        }
 *      }
 * \endcode
 *
 * \see variant
 */
class RTTR_API variant_array_view
{
    public:
        /*!
         * \brief Constructs an invalid variant_array_view object.
         *
         * \see is_valid()
         */
        variant_array_view();

        /*!
         * \brief Constructs a copy of the given variant_array_view \p other.
         */
        variant_array_view(const variant_array_view& other);

        /*!
         * \brief Destroys the variant_array_view and the contained data.
         */
        ~variant_array_view();

        /*!
         * \brief Assigns the value of the \a other variant_array_view to this variant_array_view.
         *
         * \return A reference to the variant_array_view with the new data.
         */
        variant_array_view& operator=(const variant_array_view& other);

        /*!
         * \brief Returns true if this variant_array_view is valid, that means the object is holding some data.
         *        When the variant_array_view doesn't hold any data it will return false.
         *
         * \return True if this array is valid, otherwise false.
         */
        bool is_valid() const;

        /*!
         * \brief Convenience function to check if this \ref variant_array_view is valid or not.
         *
         * \see is_valid()
         *
         * \return True if this \ref variant_array_view is valid, otherwise false.
         */
        explicit operator bool() const;

        /*!
         * \brief Swaps this variant_array_view with the \a other variant_array_view.
         */
        void swap(variant_array_view& other);
       

        /*!
         * \brief Returns true if this array is dynamic, that means the size can be changed; otherwise false.
         *
         * \see get_size()
         *
         * \return A boolean flag which indicates whether this array is dynamic or not.
         */
        bool is_dynamic() const;

        /*!
         * \brief Gets the rank (number of dimensions) of the array. 
         *        For example, int[10] returns 1; int[2][10] returns 2, and so on.
         *
         * \return Returns the rank of the array.
         */
        std::size_t get_rank() const;

        /*!
         * \brief Gets the type of the given rank index.
         *        For example, an array of type int[2][10] returns for get_rank_type(0) => int[2][10]
         *        get_rank_type(1) => int[10]; get_rank_type(2) => int
         *
         * \return The rank type at the given dimension \p index.
         */
        type get_rank_type(std::size_t index) const;
        
       /*!
         * \brief Returns the \ref type object of this array.
         *
         * \remark When the array is not valid, this function will return an invalid type object.
         *
         * \return \ref type "Type" of the array.
         */
        type get_type() const;

        /*!
         * \brief Returns the size of the first dimension from the array.
         *
         * \return The size of the array.
         */
        std::size_t get_size() const;

        /*!
         * \brief Returns the size of the array at the second dimension 
         *        at index \p index_1.
         *
         * \return The size of the array.
         */
        std::size_t get_size(std::size_t index_1) const;

        /*!
         * \brief Returns the size of the array at the third dimension at index \p index_2,
         *        relative to the first dimension at index \p index_1.
         *
         * \return The size of the array.
         */
        std::size_t get_size(std::size_t index_1, std::size_t index_2) const;

        /*!
         * \brief Returns the size from of the array at the specified indices in list \p index_list.
         *        The index count specify the array dimension.
         *
         * \return The size of the array.
         */
        std::size_t get_size_variadic(const std::vector<std::size_t>& index_list) const;
        
        /*!
         * \brief Sets the size of the array at the first dimension to \p new_size.
         *
         * \return True, when the size of the array could be changed, otherwise false.
         */
        bool set_size(std::size_t new_size);

        /*!
         * \brief Sets the size of the array at the second dimension
         *        relative to the first dimension at index \p index_1 to \p new_size.
         *
         * \return True, when the size of the array could be changed, otherwise false.
         */
        bool set_size(std::size_t new_size, std::size_t index_1);

        /*!
         * \brief Sets the size of the array at the third dimension
         *        relative to the first dimension at index \p index_1
         *        and the second dimension at index \p index_2 to \p new_size.
         *
         * \return True, when the size of the array could be changed, otherwise false.
         */
        bool set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2);

        /*!
         * \brief Sets the size of the array at the specified indices in list \p index_list.
         *        The index count specify the array dimension.
         *
         * \return True, when the size of the array could be changed, otherwise false.
         */
        bool set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list);

        /*!
         * \brief Copies the content of the the array \p arg into the underlying array.
         *
         * \return True if the value could be set, otherwise false.
         */
        bool set_value(detail::argument arg);
        
        /*!
         * \brief Set the content of the the argument \p arg into the in the first dimension 
         *        of the array at index \p index_1.
         *
         * \return True if the value could be set, otherwise false.
         */
        bool set_value(std::size_t index_1, detail::argument arg);

        /*!
         * \brief Set the content of the the argument \p arg into the in the second dimension at \p index_2
         *        of the array relative to the first dimension at index_1.
         *
         * \return True if the value could be set, otherwise false.
         */
        bool set_value(std::size_t index_1, std::size_t index_2, detail::argument arg);

        /*!
         * \brief Set the content of the the argument \p arg into the in the third dimension at \p index_3
         *        of the array relative to the first dimension at \p index_1 and second dimension at \p index_2.
         *
         * \return True if the value could be set, otherwise false.
         */
        bool set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg);

        /*!
         * \brief Set the content of the the argument \p arg into the array in the n-th dimension given in the list \p index_list.
         *
         * \remark Use this function when you want to set a value into a dimension which is bigger then three. 
         *         Otherwise use the corresponding functions of \ref set_value() .
         *
         * \return True if the value could be set, otherwise false.
         */
        bool set_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg);


        /*!
         * \brief Returns the value of the array in the first dimension at index \p index_1.
         *
         * \return The value of the given array at the specified indices.
         */
        variant get_value(std::size_t index_1) const;

        /*!
         * \brief Returns the value of the array in the second dimension at index \p index_2,
         *        relative to the first dimension at index \p index_1
         *
         * \return The value of the given array at the specified indices.
         */
        variant get_value(std::size_t index_1, std::size_t index_2) const;

        /*!
         * \brief Returns the value of the array in the third dimension at index \p index_3,
         *        relative to the first dimension at index \p index_1 and second dimension at index \p index_2.
         *
         * \return The value of the given array at the specified indices.
         */
        variant get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const;

        /*!
         * \brief Returns the value of the array relative to to indices given in the list \p index_list.
         *
         * \return The value of the given array at the specified indices.
         */
        variant get_value_variadic(const std::vector<std::size_t>& index_list) const;

        /*!
         * \brief Inserts the given argument \p arg into the array, in the first dimension at index \p index_1.
         *
         * \remark This operation is only possible when the array is \ref is_dynamic() "dynamic".
         *
         * \return True if \p arg could be inserted, otherwise false.
         */
        bool insert_value(std::size_t index_1, detail::argument arg);

        /*!
         * \brief Inserts the given argument \p arg into the array, in the second dimension at index \p index_2,
         *        relative to the first dimension at index \p index_1
         *
         * \remark This operation is only possible when the array is \ref is_dynamic() "dynamic".
         *
         * \return True if \p arg could be inserted, otherwise false.
         */
        bool insert_value(std::size_t index_1, std::size_t index_2, detail::argument arg);

        /*!
         * \brief Inserts the given argument \p arg into the array, in the third dimension at index \p index_3,
         *        relative to the first dimension at index \p index_1 and the second dimension at index \p index_2.
         *
         * \remark This operation is only possible when the array is \ref is_dynamic() "dynamic".
         *
         * \return True if \p arg could be inserted, otherwise false.
         */
        bool insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg);

        /*!
         * \brief Inserts the given argument \p arg into the array, relative to to indices given in the list \p index_list.
         *
         * \remark This operation is only possible when the array is \ref is_dynamic() "dynamic".
         *
         * \return True if \p arg could be inserted, otherwise false.
         */
        bool insert_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg);

        /*!
         * \brief Removes the value at index \p index_1 in the first dimension of the array.
         *
         * \return True if the value could be removed, otherwise false.
         */
        bool remove_value(std::size_t index_1);

        /*!
         * \brief Removes the value at index \p index_2 in the second dimension of the array
         *        relative to the first dimension at index \p index_1.
         *
         * \return True if the value could be removed, otherwise false.
         */
        bool remove_value(std::size_t index_1, std::size_t index_2);

        /*!
         * \brief Removes the value at index \p index_3 in the third dimension of the array.
         *        relative to the first dimension at index \p index_1 and the second dimension at index \p index_2.
         *
         * \return True if the value could be removed, otherwise false.
         */
        bool remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3);

        /*!
         * \brief Removes the value at index \p index_1 in the first dimension of the array.
         *
         * \return True if the value could be removed, otherwise false.
         */
        bool remove_value_variadic(const std::vector<std::size_t>& index_list);

    private:
        friend class variant;
        friend class detail::argument;

        detail::variant_array_view_data          m_data;
};

} // end namespace rttr

#include "rttr/detail/variant_array_view/variant_array_view_impl.h"

#endif // RTTR_VARIANT_ARRAY_H_
