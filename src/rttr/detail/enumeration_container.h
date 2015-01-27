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

#ifndef __RTTR_ENUMERATION_CONTAINER_H__
#define __RTTR_ENUMERATION_CONTAINER_H__

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/enumeration_container_base.h"
#include "rttr/detail/argument.h"
#include "rttr/variant.h"

#include <utility>
#include <type_traits>

namespace rttr
{
namespace detail
{

template<typename EnumType>
class enumeration_container : public enumeration_container_base
{
    public:
        enumeration_container(const type declaring_type, std::vector< std::pair<std::string, EnumType> > data)
        :   enumeration_container_base(declaring_type),
            _keyToValue(move(data))
        {
            static_assert(std::is_enum<EnumType>::value, "No enum type provided, please create an instance of this class only for enum types!");
        }

        type get_type() const { return type::get<EnumType>(); }
        type get_underlying_type() const { return type::get<typename std::underlying_type<EnumType>::type>(); }

        std::vector<std::string> get_keys() const
        {
            std::vector<std::string> result;
            for (const auto& item : _keyToValue)
            {
                result.push_back(item.first);
            }
            return result;
        }

        std::vector<variant> get_values() const
        {
            std::vector<variant> result;
            for (const auto& item : _keyToValue)
            {
                result.push_back(item.second);
            }
            return result;
        }

        std::string value_to_key(detail::argument& value) const
        {
            if (!value.is_type<EnumType>() &&
                !value.is_type<typename std::underlying_type<EnumType>::type>())
            {
                return std::string();
            }

            EnumType enum_value = value.get_value<EnumType>();
            for (const auto& item : _keyToValue)
            {
                if (item.second == enum_value)
                    return item.first;
            }
            return std::string();
        }

        variant key_to_value(const std::string& key) const
        {
            if (key.empty())
                return variant();

            for (const auto& item : _keyToValue)
            {
                if (item.first == key)
                    return item.second;
            }
            return variant();
        }

    private:
        std::vector<std::pair<std::string, EnumType> > _keyToValue;
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // __RTTR_CONSTRUCTOR_CONTAINER_H__
