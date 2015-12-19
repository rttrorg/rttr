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

#ifndef RTTR_ENUMERATION_WRAPPER_H_
#define RTTR_ENUMERATION_WRAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/enumeration/enumeration_wrapper_base.h"
#include "rttr/detail/enumeration/enum_data.h"
#include "rttr/argument.h"
#include "rttr/variant.h"

#include <utility>
#include <type_traits>

namespace rttr
{
namespace detail
{

template<typename Enum_Type, std::size_t N, std::size_t Metadata_Count>
class enumeration_wrapper : public enumeration_wrapper_base, public metadata_handler<Metadata_Count>
{
    public:
        enumeration_wrapper(std::array< enum_data<Enum_Type>, N > data, 
                            std::array<metadata, Metadata_Count> metadata_list)
        :   metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_enum_data_list(std::move(data))
        {
            static_assert(std::is_enum<Enum_Type>::value, "No enum type provided, please create an instance of this class only for enum types!");
        }

        type get_type() const { return type::get<Enum_Type>(); }
        type get_underlying_type() const { return type::get<typename std::underlying_type<Enum_Type>::type>(); }

        std::vector<std::string> get_names() const
        {
            std::vector<std::string> result;
            for (const auto& item : m_enum_data_list)
                result.push_back(item.get_name());

            return result;
        }

        std::vector<variant> get_values() const
        {
            std::vector<variant> result;
            for (const auto& item : m_enum_data_list)
                result.push_back(item.get_value());

            return result;
        }

        std::string value_to_name(argument& value) const
        {
            if (!value.is_type<Enum_Type>() &&
                !value.is_type<typename std::underlying_type<Enum_Type>::type>())
            {
                return std::string();
            }

            const Enum_Type enum_value = value.get_value<Enum_Type>();
            for (const auto& item : m_enum_data_list)
            {
                if (item.get_value() == enum_value)
                    return item.get_name();
            }
            return std::string();
        }

        variant name_to_value(const std::string& name) const
        {
            if (name.empty())
                return variant();

            for (const auto& item : m_enum_data_list)
            {
                if (item.get_name() == name)
                    return item.get_value();
            }
            return variant();
        }

        void add_metadata(detail::metadata data) const { }
        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

    private:
        std::array< enum_data<Enum_Type>, N > m_enum_data_list;
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_ENUMERATION_WRAPPER_H_
