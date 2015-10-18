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

#ifndef RTTR_VARIANT_DATA_CONVERTER_H_
#define RTTR_VARIANT_DATA_CONVERTER_H_

#include "rttr/detail/conversion/std_conversion_functions.h"
#include "rttr/detail/conversion/number_conversion.h"
#include "rttr/detail/enumeration/enumeration_helper.h"

namespace rttr
{
class argument;

namespace detail
{

template<typename T>
struct convert_from;

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * The default converter manager class. 
 *
 * It will check at runtime the incoming argument type and will forward its value to the corresponding conversion function.
 * All basic fixed integer and floating-point (float, double) types are handled.
 *
 * \remark Custom types will not be handled here, 
 *         therefore a converter function has to be registered explicitly. See \ref type::register_converter_func.
 *         With this class we can avoid this step.
 */
template<typename T, typename Type_Converter = convert_from<T>>
struct default_type_converter
{
    static bool convert_to(const T& value, argument& arg)
    {
        const type target_type = arg.get_type();
        bool result = false;
        if (target_type == type::get<bool>())
            return Type_Converter::to(value, arg.get_value<bool>());
        else if (target_type == type::get<char>())
            return Type_Converter::to(value, arg.get_value<char>());
        else if (target_type == type::get<int8>())
            return Type_Converter::to(value, arg.get_value<int8>());
        else if (target_type == type::get<int16>())
            return Type_Converter::to(value, arg.get_value<int16>());
        else if (target_type == type::get<int32>())
            return Type_Converter::to(value, arg.get_value<int32>());
        else if (target_type == type::get<int64>())
            return Type_Converter::to(value, arg.get_value<int64>());
        else if (target_type == type::get<uint8>())
            return Type_Converter::to(value, arg.get_value<uint8>());
        else if (target_type == type::get<uint16>())
            return Type_Converter::to(value, arg.get_value<uint16>());
        else if (target_type == type::get<uint32>())
            return Type_Converter::to(value, arg.get_value<uint32>());
        else if (target_type == type::get<uint64>())
            return Type_Converter::to(value, arg.get_value<uint64>());
        else if (target_type == type::get<float>())
            return Type_Converter::to(value, arg.get_value<float>());
        else if (target_type == type::get<double>())
            return Type_Converter::to(value, arg.get_value<double>());
        else if (target_type == type::get<std::string>())
            return Type_Converter::to(value, arg.get_value<std::string>());
        else
            return false;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * The empty converter class, does nothing. The only purpose is to avoid a compile time error.
 * The conversion of custom types will be handled via registered conversion functions. See \ref type::register_converter_func.
 */
template<typename T>
struct empty_type_converter
{
    static RTTR_INLINE bool convert_to(const T& value, argument& arg)
    {
        return false;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * The default implementation of the converter class for all basic types.
 *
 * That are:
 *  - all integral fixed types: int8_t till uint64_t
 *  - all floating point types
 *  - std::string
 */
template<typename T>
struct convert_from
{
    static RTTR_INLINE bool to(const T& from, bool& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, char& to)
    {
        to = static_cast<char>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const T& from, int8& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, int16& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, int32& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, int64& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, uint8& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, uint16& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, uint32& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, uint64& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, float& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, double& to)
    {
        return false;
    }

    static RTTR_INLINE bool to(const T& from, std::string& to)
    {
        return false;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// here come the specialization for several atomic types

template<>
struct RTTR_API convert_from<bool>
{
    static RTTR_INLINE bool to(const bool& from, bool& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, char& to)
    {
        to = static_cast<char>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, int8& to)
    {
        to = static_cast<int8>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, int16& to)
    {
        to = static_cast<int16>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, int32& to)
    {
        to = static_cast<int32>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, int64& to)
    {
        to = static_cast<int64>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, uint8& to)
    {
        to = static_cast<uint8>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, uint16& to)
    {
        to = static_cast<uint16>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, uint32& to)
    {
        to = static_cast<uint32>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, uint64& to)
    {
        to = static_cast<uint64>(from ? 1 : 0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, float& to)
    {
        to = static_cast<float>(from ? 1.0f : 0.0f);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, double& to)
    {
        to = static_cast<double>(from ? 1.0 : 0.0);
        return true;
    }

    static RTTR_INLINE bool to(const bool& from, std::string& to)
    {
        to = (from ? "true" : "false");
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<char>
{
    static RTTR_INLINE bool to(const char& from, bool& to)
    {
        to = (from != '\0');
        return true;
    }

    static RTTR_INLINE bool to(const char& from, char& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const char& from, int8& to)
    {
        to = static_cast<int8>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, int16& to)
    {
        to = static_cast<int16>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, int32& to)
    {
        to = static_cast<int32>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, int64& to)
    {
        to = static_cast<int64>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, uint8& to)
    {
        to = static_cast<uint8>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, uint16& to)
    {
        to = static_cast<uint16>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, uint32& to)
    {
        to = static_cast<uint32>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, uint64& to)
    {
        to = static_cast<uint64>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const char& from, std::string& to)
    {
        to = std::string(1, from);
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<int8>
{
    static RTTR_INLINE bool to(const int8& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, char& to)
    {
        to = static_cast<char>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, int8& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, int16& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, int32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, int64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int8& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int8& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int8& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int8& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int8& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<int16>
{
    static RTTR_INLINE bool to(const int16& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, int16& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, int32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, int64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int16& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int16& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<int32>
{
    static RTTR_INLINE bool to(const int32& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const int32& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, int32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int32& from, int64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int32& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int32& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int32& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int32& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<int64>
{
    static RTTR_INLINE bool to(const int64& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const int64& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, int64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const int64& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const int64& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int64& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const int64& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<uint8>
{
    static RTTR_INLINE bool to(const uint8& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint8& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint8& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint8& from, int32& to)
    {
        to = static_cast<int32>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, int64& to)
    {
        to = static_cast<int64>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, uint8& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, uint16& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, uint32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, uint64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint8& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<uint16>
{
    static RTTR_INLINE bool to(const uint16& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint16& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint16& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint16& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint16& from, int64& to)
    {
        to = static_cast<int64>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint16& from, uint16& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, uint32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, uint64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint16& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<uint32>
{
    static RTTR_INLINE bool to(const uint32& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const uint32& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, int64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint32& from, uint32& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint32& from, uint64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint32& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint32& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint32& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<uint64>
{
    static RTTR_INLINE bool to(const uint64& from, bool& to)
    {
        to = (from != 0);
        return true;
    }

    static RTTR_INLINE bool to(const uint64& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, int64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const uint64& from, uint64& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const uint64& from, float& to)
    {
        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint64& from, double& to)
    {
        to = static_cast<double>(from);
        return true;
    }

    static RTTR_INLINE bool to(const uint64& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<float>
{
    static RTTR_INLINE bool to(const float& from, bool& to)
    {
        to = !(from <= std::numeric_limits<float>::min() && 
               from >= -1 * std::numeric_limits<float>::min());

        return true;
    }

    static RTTR_INLINE bool to(const float& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, int64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const float& from, float& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const float& from, double& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const float& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<double>
{
    static RTTR_INLINE bool to(const double& from, bool& to)
    {
        to = !(from <= std::numeric_limits<double>::min() && 
               from >= -1 * std::numeric_limits<double>::min());

        return true;
    }

    static RTTR_INLINE bool to(const double& from, char& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, int8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, int16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, int32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, int64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, uint8& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, uint16& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, uint32& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, uint64& to)
    {
        return convert_to(from, to);
    }

    static RTTR_INLINE bool to(const double& from, float& to)
    {
        RTTR_CONSTEXPR_OR_CONST double float_min = -1 * std::numeric_limits<float>::max();
        RTTR_CONSTEXPR_OR_CONST double float_max = std::numeric_limits<float>::max();
        if (from < float_min || from > float_max)
            return false;

        to = static_cast<float>(from);
        return true;
    }

    static RTTR_INLINE bool to(const double& from, double& to)
    {
        to = from;
        return true;
    }

    static RTTR_INLINE bool to(const double& from, std::string& to)
    {
        return convert_to(from, to);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

template<>
struct RTTR_API convert_from<std::string>
{
    static RTTR_INLINE bool to(const std::string& from, bool& to)
    {
        bool ok;
        to = string_to_bool(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, char& to)
    {
        const auto& val = from;
        if (val.empty())
            to ='\0';
        else
            to = val[0];
    
        return true;
    }

    static RTTR_INLINE bool to(const std::string& from, int8& to)
    {
        bool ok;
        int val = string_to_int(from, &ok);
        if (!ok)
            return false;

        return convert_to(val, to);
    }

    static RTTR_INLINE bool to(const std::string& from, int16& to)
    {
        bool ok;
        int val = string_to_int(from, &ok);
        if (!ok)
            return false;

        return convert_to(val, to);
    }

    static RTTR_INLINE bool to(const std::string& from, int32& to)
    {
        bool ok;
        to = string_to_int(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, int64& to)
    {
        bool ok;
        to = string_to_int(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, uint8& to)
    {
        bool ok;
        unsigned int val = string_to_int(from, &ok);
        if (!ok)
            return false;

        return convert_to(val, to);
    }

    static RTTR_INLINE bool to(const std::string& from, uint16& to)
    {
        bool ok;
        unsigned int val = string_to_int(from, &ok);
        if (!ok)
            return false;

        return convert_to(val, to);
    }

    static RTTR_INLINE bool to(const std::string& from, uint32& to)
    {
        bool ok;
        to = string_to_int(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, uint64& to)
    {
        bool ok;
        to = string_to_int(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, float& to)
    {
        bool ok;
        to = string_to_float(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, double& to)
    {
        bool ok;
        to = string_to_double(from, &ok);
        return ok;
    }

    static RTTR_INLINE bool to(const std::string& from, std::string& to)
    {
        to = from;
        return true;
    }

};

/////////////////////////////////////////////////////////////////////////////////////////

// MSVC generates following warning: 'warning C4800: 'const enum_bool' : forcing value to bool 'true' or 'false' (performance warning)'
// For unknown reason the MSVC compiler is too dump to recognize that I can safely convert an enumeration
// with underlying type bool, to type bool (thats no int to bool conversion!)
// so we disable the warning for enum conversions:
#if  RTTR_COMPILER == RTTR_COMPILER_MSVC

#pragma warning(push)
#pragma warning(disable:4800)

#endif

template<typename T>
struct convert_from_enum
{
    template<typename T_>
    using enum_type_t = typename std::underlying_type<T_>::type;

    static RTTR_INLINE enum_type_t<T> get_underlying_value(const T& from)
    {
        return static_cast<enum_type_t<T>>(from);
    }

    template<typename T1>
    static RTTR_INLINE
    enable_if_t<!std::is_same<bool, enum_type_t<T1> >::value, bool>
    to(const T1& from, bool& to)
    {
        const auto value = get_underlying_value(from);
        if (value == 0)
        {
            to = false;
            return true;
        }
        else if (value == 1)
        {
            to = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    template<typename T1>
    static RTTR_INLINE
    enable_if_t<std::is_same<bool, enum_type_t<T1> >::value, bool>
    to(const T1& from, bool& to)
    {
        // for unknown reason MSVC will here scream a warning 'C4800'...
        to = static_cast<bool>(from);
        return true;
    }
   
    static RTTR_INLINE bool to(const T& from, char& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, int8& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, int16& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, int32& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, int64& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, uint8& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, uint16& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, uint32& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, uint64& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, float& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, double& to)
    {
        return convert_from<enum_type_t<T>>::to(get_underlying_value(from), to);
    }

    static RTTR_INLINE bool to(const T& from, std::string& to)
    {
        to = get_enumeration_name(from);
        return (to.empty() != false);
    }

};

/////////////////////////////////////////////////////////////////////////////////////////

#if  RTTR_COMPILER == RTTR_COMPILER_MSVC
// restore warning level
#pragma warning(pop)

#endif

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_VARIANT_DATA_CONVERTER_H_
