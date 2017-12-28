/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_REGISTRATION_STATE_SAVER_H_
#define RTTR_REGISTRATION_STATE_SAVER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/array_range.h"
#include "rttr/detail/type/type_deregister.h"

#include <vector>

namespace rttr
{
namespace detail
{

/*!
 * This class saves the current state of the registration of all possible items
 * and will undo the registration when the instance is destroyed.
 */
class registration_state_saver
{
    private:
        /*!
         * This will perform a copy of the given \p range starting at index \p from_index.
         */
        template<typename T>
        std::vector<T> copy_range(const array_range<T>& range, typename array_range<T>::size_type from_index)
        {
            std::vector<T> result;
            const auto size = range.size();
            result.reserve(size - from_index);
            typename array_range<T>::size_type index = 0;
            for (auto& item : range)
            {
                if (index >= from_index)
                    result.push_back(item);

                ++index;
            }


            return result;
        }

    public:
        /*!
         * \brief Will perform the deregistration when the object goes out of scope.
         */
        ~registration_state_saver()
        {
            for (auto meth : m_global_properties)
                type_deregister::global_property(meth);

            for (auto meth : m_global_methods)
                type_deregister::global_method(meth);

            for (auto t : m_types)
                type_deregister::item(t);
        }

        /*!
         * Begin storing the current registration state.
         * The first method you should call.
         */
        void begin_save_state()
        {
            m_old_type_size     = type::get_types().size();
            m_old_methods_size  = type::get_global_methods().size();
            m_old_property_size = type::get_global_properties().size();
        }

        /*!
         * End storing the current registration state.
         * The last method you should call.
         */
        void end_save_state()
        {
            m_types             = copy_range(type::get_types(), m_old_type_size);
            m_global_methods    = copy_range(type::get_global_methods(), m_old_methods_size);
            m_global_properties = copy_range(type::get_global_properties(), m_old_property_size);

            m_types.shrink_to_fit();
            m_global_methods.shrink_to_fit();
            m_global_properties.shrink_to_fit();
        }

    private:
        std::size_t                 m_old_type_size;
        std::size_t                 m_old_methods_size;
        std::size_t                 m_old_property_size;
        std::vector<type>           m_types;
        std::vector<method>         m_global_methods;
        std::vector<property>       m_global_properties;
        std::vector<enumeration>    m_global_enums;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_REGISTRATION_STATE_SAVER_H_
