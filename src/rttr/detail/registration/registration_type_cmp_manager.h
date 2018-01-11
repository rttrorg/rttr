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

#ifndef RTTR_REGISTRATION_CMP_MANAGER_H_
#define RTTR_REGISTRATION_CMP_MANAGER_H_

#include "rttr/detail/base/core_prerequisites.h"

#include "rttr/detail/type/type_register.h"
#include "rttr/detail/type/type_comparator.h"
#include "rttr/detail/type/type_converter.h"

#include <vector>
#include <memory>

namespace rttr
{
namespace detail
{

/*!
 * This class saves the registration of all possible type cmp items  (used inside variant)
 * per module (*.DLL, *.so, ...) and will undo the registration
 * when the instance is destroyed.
 */
class RTTR_LOCAL registration_type_cmp_manager
{
    public:
        registration_type_cmp_manager() = default;
        ~registration_type_cmp_manager()
        {
            for (auto& item : m_type_converters)
                type_register::unregister_converter(item.get());
            for (auto& item : m_type_equal_cmps)
                type_register::unregister_equal_comparator(item.get());
            for (auto& item : m_type_less_than_cmps)
                type_register::unregister_less_than_comparator(item.get());
        }

        void add_item(std::unique_ptr<type_converter_base> conv)
        {
            type_register::register_converter(conv.get());
            m_type_converters.push_back(std::move(conv));
        }

        void add_equal_cmp(std::unique_ptr<type_comparator_base> cmp)
        {
            type_register::register_equal_comparator(cmp.get());
            m_type_equal_cmps.push_back(std::move(cmp));
        }

        void add_less_than_cmp(std::unique_ptr<type_comparator_base> cmp)
        {
            type_register::register_less_than_comparator(cmp.get());
            m_type_less_than_cmps.push_back(std::move(cmp));
        }

        // no copy, no assign
        registration_type_cmp_manager(const registration_type_cmp_manager&) = delete;
        registration_type_cmp_manager& operator=(const registration_type_cmp_manager&) = delete;

    private:
        std::vector<std::unique_ptr<type_converter_base>>       m_type_converters;
        std::vector<std::unique_ptr<type_comparator_base>>      m_type_equal_cmps;
        std::vector<std::unique_ptr<type_comparator_base>>      m_type_less_than_cmps;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
registration_type_cmp_manager& get_registration_type_cmp_manager() RTTR_NOEXCEPT
{
    static registration_type_cmp_manager obj;
    return obj;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_REGISTRATION_CMP_MANAGER_H_
