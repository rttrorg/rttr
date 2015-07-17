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

#include "rttr/variant.h"

#include "rttr/detail/variant/variant_data_policy.h"
#include "rttr/variant_array.h"
#include "rttr/detail/argument.h"

#include <algorithm>
#include <limits>
#include <string>
#include <set>

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

variant::variant(const variant& other)
:   m_variant_policy(other.m_variant_policy)
{
    m_variant_policy(detail::variant_policy_operation::CLONE, other.m_variant_data, m_variant_data);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant::variant(variant&& other)
:   m_variant_policy(other.m_variant_policy)
{
    m_variant_data = other.m_variant_data;
    other.m_variant_policy = &detail::variant_data_policy_empty::invoke;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant::variant(variant_array&& array)
:   m_variant_policy(&detail::variant_data_policy_empty::invoke)
{
    array.m_variant.swap(*this);
    array.m_data.m_policy = &detail::variant_array_policy_empty::invoke;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant::variant(const variant_array& array)
:   m_variant_policy(&detail::variant_data_policy_empty::invoke)
{
    variant(array.m_variant).swap(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

void variant::swap(variant& other)
{
    std::swap(m_variant_policy, other.m_variant_policy);
    std::swap(m_variant_data, other.m_variant_data);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant& variant::operator=(const variant& other)
{
    variant(other).swap(*this);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant& variant::operator=(variant&& other)
{
    other.swap(*this);
    variant().swap(other);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

void variant::clear()
{
    variant().swap(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant::is_valid() const
{
    return m_variant_policy(detail::variant_policy_operation::IS_VALID, m_variant_data, nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant::operator bool() const
{
    return m_variant_policy(detail::variant_policy_operation::IS_VALID, m_variant_data, nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant::get_type() const
{
    type src_type(type::m_invalid_id);
    m_variant_policy(detail::variant_policy_operation::GET_TYPE, m_variant_data, src_type);
    return src_type;
}

/////////////////////////////////////////////////////////////////////////////////////////

detail::variant_array_data variant::to_array() const
{
    detail::variant_array_data result;
    m_variant_policy(detail::variant_policy_operation::TO_ARRAY, m_variant_data, result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant::can_convert(const type& target_type) const
{
    if (!is_valid())
        return false;

    const type source_type = get_type();
    
    if (source_type == target_type)
        return true;

    if (source_type.get_pointer_dimension() == 1 && target_type.get_pointer_dimension() == 1)
    {
        if (void * ptr = type::apply_offset(get_raw_ptr(), source_type, target_type))
            return true;
    }

    if (source_type.get_type_converter(target_type))
        return true;

    const bool source_is_arithmetic = source_type.is_arithmetic();
    const bool target_is_arithmetic = target_type.is_arithmetic();
    const type string_type = type::get<std::string>();

    return ((source_is_arithmetic && target_is_arithmetic) ||
            (source_is_arithmetic && target_type == string_type) ||
            (source_type == string_type && target_is_arithmetic));
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant::to_bool() const
{
    return convert_to_basic_type<bool>(nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

int variant::to_int(bool *ok) const
{
    return convert_to_basic_type<int>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

std::string variant::to_string(bool *ok) const
{
    return convert_to_basic_type<std::string>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

float variant::to_float(bool* ok)
{
    return convert_to_basic_type<float>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

double variant::to_double(bool* ok)
{
    return convert_to_basic_type<double>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool variant::convert_to_basic_type(T& to) const
{
    return m_variant_policy(detail::variant_policy_operation::CONVERT, m_variant_data, detail::argument(to));
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant::convert(const type& target_type)
{
    if (!is_valid())
        return false;

    bool ok = false;

    variant new_var;
    const type source_type = get_type();
    const bool source_is_arithmetic = source_type.is_arithmetic();
    const bool target_is_arithmetic = target_type.is_arithmetic();
    const type string_type = type::get<std::string>();
    if (target_type == source_type)
    {
        return true; // the current variant is already the target type, we don't need to do anything
    }
    else if ((source_is_arithmetic && target_is_arithmetic) ||
            (source_is_arithmetic && target_type == string_type) ||
            (source_type == string_type && target_is_arithmetic))
    {
        if (target_type == type::get<bool>())
        {
            bool value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<char>())
        {
            char value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<int8>())
        {
            int8 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<int16>())
        {
            int16 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<int32>())
        {
            int32 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<int64>())
        {
            int64 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<uint8>())
        {
            uint8 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<uint16>())
        {
            uint16 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<uint32>())
        {
            uint32 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<uint64>())
        {
            uint64 value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<float>())
        {
            float value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == type::get<double>())
        {
            double value;
            if (convert_to_basic_type(value))
                new_var = value;
        }
        else if (target_type == string_type)
        {
            std::string value;
            if (convert_to_basic_type(value))
                new_var = std::move(value);
        }
        
        if (new_var.is_valid())
            ok = true;
    }
    else
    {
        if (const auto& converter = source_type.get_type_converter(target_type))
        {
            void* raw_ptr = get_ptr();
            new_var = converter->to_variant(raw_ptr, ok);
        }
        else if (source_type.is_pointer() &&
                 (source_type.get_pointer_dimension() == 1 && target_type.get_pointer_dimension() == 1))
        {
            void* raw_ptr = get_raw_ptr();
            if (void* casted_ptr = type::apply_offset(raw_ptr, source_type, target_type))
            {
                // although we forward a void* to create a variant,
                // it will create a variant for the specific class type
                new_var = target_type.create_variant(casted_ptr);
                if (new_var.is_valid())
                    ok = true;
            }
        }
    }

    if (ok)
        swap(new_var);

    return ok;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
RTTR_FORCE_INLINE T variant::convert_to_basic_type(bool* ok) const
{
    static_assert(std::is_default_constructible<T>::value, "The given type T has no default constructor."
                                                           "You can only convert to a type, with a default constructor.");
    bool tmp_ok = false;
    T result;
    const type source_type = get_type();
    const type target_type = type::get<T>();

    if (target_type == source_type)
    {
        result = const_cast<variant&>(*this).get_value<T>();
    }
    else if (convert_to_basic_type(result))
    {
        tmp_ok = true;
    }
    else if (const auto& converter = source_type.get_type_converter(target_type))
    {
        detail::type_converter_target<T>* target_converter = static_cast<detail::type_converter_target<T>*>(converter);
        void* raw_ptr = get_ptr();
        result = target_converter->convert(raw_ptr, tmp_ok);
    }
    
    if (ok)
        *ok = tmp_ok;

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
bool variant::convert<bool>(bool* ok) const
{
    return convert_to_basic_type<bool>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
int variant::convert<int>(bool* ok) const
{
    return convert_to_basic_type<int>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
float variant::convert<float>(bool* ok) const
{
    return convert_to_basic_type<float>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
double variant::convert<double>(bool* ok) const
{
    return convert_to_basic_type<double>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

template<>
std::string variant::convert<std::string>(bool* ok) const
{
    return convert_to_basic_type<std::string>(ok);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
