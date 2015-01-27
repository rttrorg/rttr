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

#include "rttr/variant.h"
#include "rttr/detail/std_conversion_functions.h"
#include "rttr/variant_array.h"
#include <limits>

#include <string>

// needed for type::get<void>()
#include "rttr/rttr_enable.h"
#include "rttr/detail/array_container.h"

RTTR_DECLARE_TYPE(void)
RTTR_DECLARE_TYPE(int)
RTTR_DECLARE_TYPE(bool)
RTTR_DECLARE_TYPE(float)
RTTR_DECLARE_TYPE(double)
RTTR_DECLARE_TYPE(std::string)

namespace rttr
{
namespace detail
{
    variant create_void_variant()
    {
        variant var;
        var._holder = new variant::variant_container<void>();
        return var;
    }

    variant void_variant = create_void_variant();
} // end namespace impl

////////////////////////////////////////////////////////////////////////////////////////

int variant::to_int(bool *ok) const
{
    return convert<int>(ok);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::to_bool() const
{
    return convert<bool>();
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::to_string(bool *ok) const
{
    return convert<std::string>(ok);
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::to_float(bool* ok)
{
    return convert<float>(ok);
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::to_double(bool* ok)
{
    return convert<double>(ok);
}

/////////////////////////////////////////////////////////////////////////////////

variant_array variant::to_array() const
{
    if (_holder)
        return variant_array(_holder->to_array());
    else
        return variant_array();
}

/////////////////////////////////////////////////////////////////////////////////

bool variant::can_convert(const type& target_type) const
{
    if (_holder)
        return _holder->can_convert(target_type);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<> 
bool variant::can_convert<variant_array>() const
{
    if (_holder)
        return _holder->is_array();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////

bool variant::convert(const type& target_type)
{
    if (!_holder)
        return false;

    bool ok = false;
    variant new_var;
    const type& source_type = _holder->get_type();
    if (target_type == source_type)
    {
        return true;
    }
    else if (target_type == type::get<std::string>())
    {
        new_var = to_string(&ok);
    }
    else if (target_type == type::get<int>())
    {
        new_var = to_int(&ok);
    }
    else if (target_type == type::get<bool>())
    {
        new_var = to_bool();
        ok = true;
    }
    else if (target_type == type::get<float>())
    {
        new_var = to_float(&ok);
    }
    else if (target_type == type::get<double>())
    {
        new_var = to_double(&ok);
    }
    else
    {
        if (const auto& converter = source_type.get_type_converter(target_type))
        {
            void* data = _holder->get_ptr();
            new_var = converter->to_variant(data, ok);
        }
        else
        {
            void* data = _holder->get_raw_ptr();
            void* d_ptr = type::apply_offset(data, source_type, target_type);
            if (d_ptr)
            {
                new_var = target_type.create_from_ptr(d_ptr);
                if (new_var.is_valid())
                    ok = true;
            }
        }
        
    }

    if (ok)
        swap(new_var);

    return ok;
}

/////////////////////////////////////////////////////////////////////////////////

template<>
std::string variant::convert<std::string>(bool* ok) const
{
    if (_holder)
        return _holder->to_string(ok);
    else
    {
        if (ok)
            *ok = false;
        return std::string();
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<>
int variant::convert<int>(bool* ok) const
{
    if (_holder)
        return _holder->to_int(ok);
    else
    {
        if (ok)
            *ok = false;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<>
bool variant::convert<bool>(bool* ok) const
{
    if (_holder)
        return _holder->to_bool(ok);
    else
    {
        if (ok)
            *ok = false;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<>
float variant::convert<float>(bool* ok) const
{
    if (_holder)
        return _holder->to_float(ok);
    else
    {
        if (ok)
            *ok = false;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<>
double variant::convert<double>(bool* ok) const
{
    if (_holder)
        return _holder->to_double(ok);
    else
    {
        if (ok)
            *ok = false;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<>
variant_array variant::convert<variant_array>(bool* ok) const
{
    variant_array result;

    if (_holder)
        result = _holder->to_array();
    
    if (ok && result.is_valid())
        *ok = false;

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////

variant::variant_container<void>::variant_container()
{

}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// void

variant::variant_container_base* variant::variant_container<void>::clone() const
{
    return (new variant_container<void>());
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<void>::get_type() const
{
    return type::get<void>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<void>::get_ptr() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<void>::get_raw_type() const
{
    return type::get<void>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<void>::get_raw_ptr() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<void>::is_array() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<void>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<void>::can_convert(const type& target_type) const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<void>::to_string(bool* ok) const
{
    if (ok)
        *ok = false;

    return std::string();
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<void>::to_int(bool* ok) const
{
    if (ok)
        *ok = false;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<void>::to_bool(bool* ok) const
{
    if (ok)
        *ok = false;

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<void>::to_float(bool* ok) const
{
    if (ok)
        *ok = false;

    return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<void>::to_double(bool* ok) const
{
    if (ok)
        *ok = false;

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// std::string

variant::variant_container<std::string>::variant_container(const std::string& arg)
: _value(arg)
{

}

variant::variant_container<std::string>::variant_container(std::string&& arg)
: _value(std::move(arg))
{
}

variant::variant_container_base* variant::variant_container<std::string>::clone() const
{
    return (new variant_container<std::string>(_value));
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<std::string>::get_type() const
{
    return type::get<std::string>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<std::string>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<std::string>::get_raw_type() const
{
    return type::get<detail::raw_type<std::string>::type>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<std::string>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<std::string>::is_array() const
{
    return detail::is_array<detail::raw_type<std::string>::type>::value;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<std::string>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<std::string>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<std::string>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<int>())
        return true;
    else if (target_type == type::get<bool>())
        return true;
    else if (target_type == type::get<float>())
        return true;
    else if (target_type == type::get<double>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<std::string>::to_string(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<std::string>::to_int(bool* ok) const
{
    return detail::string_to_int(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<std::string>::to_bool(bool* ok) const
{
    return detail::string_to_bool(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<std::string>::to_float(bool* ok) const
{
    return detail::string_to_float(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<std::string>::to_double(bool* ok) const
{
    return detail::string_to_double(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// int

variant::variant_container<int>::variant_container(const int& arg)
: _value(arg)
{

}

variant::variant_container<int>::variant_container(int&& arg)
: _value(std::move(arg))
{
}

variant::variant_container_base* variant::variant_container<int>::clone() const
{
    return (new variant_container<int>(_value));
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<int>::get_type() const
{
    return type::get<int>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<int>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<int>::get_raw_type() const
{
    return type::get<detail::raw_type<int>::type>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<int>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<int>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<int>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<std::string>())
        return true;
    else if (target_type == type::get<bool>())
        return true;
    else if (target_type == type::get<float>())
        return true;
    else if (target_type == type::get<double>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<int>::is_array() const
{
    return detail::is_array<detail::raw_type<int>::type>::value;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<int>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<int>::to_string(bool* ok) const
{
    return detail::int_to_string(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<int>::to_int(bool* ok) const
{
    if (ok)
        *ok = true;
    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<int>::to_bool(bool* ok) const
{
    if (ok)
        *ok = true;

    return (_value != 0) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<int>::to_float(bool* ok) const
{
    if (ok)
        *ok = true;
    return static_cast<float>(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<int>::to_double(bool* ok) const
{
    if (ok)
        *ok = true;
    return static_cast<double>(_value);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// bool

variant::variant_container<bool>::variant_container(const bool& arg)
: _value(arg)
{

}

variant::variant_container<bool>::variant_container(bool&& arg)
: _value(std::move(arg))
{
}

variant::variant_container_base* variant::variant_container<bool>::clone() const
{
    return (new variant_container<bool>(_value));
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<bool>::get_type() const
{
    return type::get<bool>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<bool>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<bool>::get_raw_type() const
{
    return type::get<detail::raw_type<bool>::type>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<bool>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<bool>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<bool>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<std::string>())
        return true;
    else if (target_type == type::get<int>())
        return true;
    else if (target_type == type::get<float>())
        return true;
    else if (target_type == type::get<double>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<bool>::is_array() const
{
    return detail::is_array<detail::raw_type<bool>::type>::value;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<bool>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<bool>::to_string(bool* ok) const
{
    return (_value ? "true" : "false");
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<bool>::to_int(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<bool>::to_bool(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<bool>::to_float(bool* ok) const
{
    if (ok)
        *ok = true;
    
    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<bool>::to_double(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// float

variant::variant_container<float>::variant_container(const float& arg)
: _value(arg)
{

}

variant::variant_container<float>::variant_container(float&& arg)
: _value(std::move(arg))
{
}

variant::variant_container_base* variant::variant_container<float>::clone() const
{
    return (new variant_container<float>(_value));
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<float>::get_type() const
{
    return type::get<float>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<float>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<float>::get_raw_type() const
{
    return type::get<detail::raw_type<float>::type>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<float>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<float>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<float>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<std::string>())
        return true;
    else if (target_type == type::get<int>())
        return true;
    else if (target_type == type::get<bool>())
        return true;
    else if (target_type == type::get<double>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<float>::is_array() const
{
    return detail::is_array<detail::raw_type<float>::type>::value;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<float>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<float>::to_string(bool* ok) const
{
    return detail::float_to_string(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<float>::to_int(bool* ok) const
{
    if (ok)
        *ok = true;

    return static_cast<int>(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<float>::to_bool(bool* ok) const
{
    if (ok)
        *ok = true;

    return (std::abs(_value) <= std::numeric_limits<float>::epsilon()) ? false : true;
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<float>::to_float(bool* ok) const
{
    if (ok)
        *ok = true;
    
    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<float>::to_double(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// double

variant::variant_container<double>::variant_container(const double& arg)
: _value(arg)
{

}

variant::variant_container<double>::variant_container(double&& arg)
: _value(std::move(arg))
{
}

variant::variant_container_base* variant::variant_container<double>::clone() const
{
    return (new variant_container<double>(_value));
}

////////////////////////////////////////////////////////////////////////////////////////
    
type variant::variant_container<double>::get_type() const
{
    return type::get<double>();
}

////////////////////////////////////////////////////////////////////////////////////////
    
void* variant::variant_container<double>::get_ptr() const
{
    return const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_value)));
}

////////////////////////////////////////////////////////////////////////////////////////

type variant::variant_container<double>::get_raw_type() const
{
    return type::get<detail::raw_type<double>::type>();
}

////////////////////////////////////////////////////////////////////////////////////////

void* variant::variant_container<double>::get_raw_ptr() const
{
    return detail::get_void_ptr(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<double>::is_array() const
{
    return detail::is_array<detail::raw_type<double>::type>::value;
}

////////////////////////////////////////////////////////////////////////////////////////

detail::array_container_base* variant::variant_container<double>::to_array() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<double>::can_convert(const type& target_type) const
{
    const auto& source_type = type::get<double>();
    if (source_type == target_type)
        return true;
    else if (target_type == type::get<std::string>())
        return true;
    else if (target_type == type::get<int>())
        return true;
    else if (target_type == type::get<bool>())
        return true;
    else if (target_type == type::get<float>())
        return true;
    else if (source_type.get_type_converter(target_type))
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////////////

std::string variant::variant_container<double>::to_string(bool* ok) const
{
    return detail::double_to_string(_value, ok);
}

////////////////////////////////////////////////////////////////////////////////////////

int variant::variant_container<double>::to_int(bool* ok) const
{
    if (ok)
        *ok = true;

    return static_cast<int>(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

bool variant::variant_container<double>::to_bool(bool* ok) const
{
    if (ok)
        *ok = true;

    return (std::abs(_value) <= std::numeric_limits<double>::epsilon()) ? false : true;
}

////////////////////////////////////////////////////////////////////////////////////////

float variant::variant_container<double>::to_float(bool* ok) const
{
    if (ok)
        *ok = true;
    
    return static_cast<float>(_value);
}

////////////////////////////////////////////////////////////////////////////////////////

double variant::variant_container<double>::to_double(bool* ok) const
{
    if (ok)
        *ok = true;

    return _value;
}

} // end namespace rttr
