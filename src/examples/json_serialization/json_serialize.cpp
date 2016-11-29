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

#include <cstdio>
#include <string>
#include <vector>
#include <array>

#include <iostream>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rttr/type>

using namespace rapidjson;
using namespace rttr;


namespace
{

/////////////////////////////////////////////////////////////////////////////////////////

void to_json_recursively(const instance& obj, PrettyWriter<StringBuffer>& writer);
void fromjson_recursively(instance obj, Value& json_object);

/////////////////////////////////////////////////////////////////////////////////////////

void write_atomic_types_to_json(const type& t, const variant& var, PrettyWriter<StringBuffer>& writer)
{
    if (t.is_arithmetic())
    {
        if (t == type::get<bool>())
            writer.Bool(var.to_bool());
        else if (t == type::get<char>())
            writer.Bool(var.to_bool());
        else if (t == type::get<int8_t>())
            writer.Int(var.to_int8());
        else if (t == type::get<int16_t>())
            writer.Int(var.to_int16());
        else if (t == type::get<int32_t>())
            writer.Int(var.to_int32());
        else if (t == type::get<int64_t>())
            writer.Int64(var.to_int64());
        else if (t == type::get<uint8_t>())
            writer.Uint(var.to_uint8());
        else if (t == type::get<uint16_t>())
            writer.Uint(var.to_uint16());
        else if (t == type::get<uint32_t>())
            writer.Uint(var.to_uint32());
        else if (t == type::get<uint64_t>())
            writer.Uint64(var.to_uint64());
        else if (t == type::get<float>())
            writer.Double(var.to_double());
        else if (t == type::get<double>())
            writer.Double(var.to_double());
    }
    else if (t.is_enumeration())
    {
        bool ok = false;
        auto result = var.to_string(&ok);
        if (ok)
        {
            writer.String(var.to_string());
            return;
        }
        ok = false;
        auto value = var.to_uint64(&ok);
        if (ok)
            writer.Uint64(value);
        else
            writer.Null();
    }
    else if (t == type::get<std::string>())
    {
        writer.String(var.to_string());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

static void write_array(const variant_array_view& a, PrettyWriter<StringBuffer>& writer)
{
    writer.StartArray();
    for (int i = 0; i < a.get_size(); ++i)
    {
        variant var = a.get_value_as_ref(i);
        if (var.is_array())
        {
            write_array(var.create_array_view(), writer);
        }
        else
        {
            variant wrapped_var = var.extract_wrapped_value();
            type value_type = wrapped_var.get_type();
            if (value_type.is_arithmetic() || value_type == type::get<std::string>() || value_type.is_enumeration())
            {
                write_atomic_types_to_json(value_type, wrapped_var, writer);
            }
            else // object
            {
                to_json_recursively(wrapped_var, writer);
            }
        }
    }
    writer.EndArray();
}

/////////////////////////////////////////////////////////////////////////////////////////

void to_json_recursively(const instance& obj2, PrettyWriter<StringBuffer>& writer)
{
    writer.StartObject();
    instance obj = obj2.get_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

    auto prop_list = obj.get_derived_type().get_properties();
    for (auto prop : prop_list)
    {
        if (prop.get_metadata("NO_SERIALIZE"))
            continue;

        variant prop_value = prop.get_value(obj);
        if (!prop_value)
            continue; // cannot serialize, because we cannot retrieve the value

        const auto name = prop.get_name();
        writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
        type value_type = prop_value.get_type();
        if (value_type.is_arithmetic() || value_type == type::get<std::string>() || value_type.is_enumeration())
        {
            write_atomic_types_to_json(value_type, prop_value, writer);
        }
        else if (value_type.is_array())
        {
            write_array(prop_value.create_array_view(), writer);
        }
        else
        {
            auto child_props = value_type.get_properties();
            if (!child_props.empty())
            {
                to_json_recursively(prop_value, writer);
            }
            else
            {
                bool ok = false;
                auto text = prop_value.to_string(&ok);
                if (!ok)
                    std::cerr << "cannot serialize property: " << prop.get_name() << std::endl;

                writer.String(text);
            }
        }
    }

    writer.EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant extract_basic_types(Value& json_value)
{
    switch(json_value.GetType())
    {
        case kStringType:
        {
            return std::string(json_value.GetString());
            break;
        }
        case kNullType:     break;
        case kFalseType:
        case kTrueType:
        {
            return json_value.GetBool();
            break;
        }
        case kNumberType:
        {
            if (json_value.IsInt())
                return json_value.GetInt();
            else if (json_value.IsDouble())
                return json_value.GetDouble();
            else if (json_value.IsUint())
                return json_value.GetUint();
            else if (json_value.IsInt64())
                return json_value.GetInt64();
            else if (json_value.IsUint64())
                return json_value.GetUint64();
            break;
        }
    }

    return variant();
}


/////////////////////////////////////////////////////////////////////////////////////////

void write_array_recursively(variant_array_view& var_array, Value& json_array_value)
{
    var_array.set_size(json_array_value.Size());
    for (SizeType i = 0; i < json_array_value.Size(); ++i)
    {
        auto& json_index_value = json_array_value[i];
        if (json_index_value.IsArray())
        {
            auto sub_array_view = var_array.get_value_as_ref(i).create_array_view();
            write_array_recursively(sub_array_view, json_index_value);
        }
        else if (json_index_value.IsObject())
        {
            variant var_tmp = var_array.get_value_as_ref(i);
            variant wrapped_var = var_tmp.extract_wrapped_value();
            fromjson_recursively(wrapped_var, json_index_value);
            var_array.set_value(i, wrapped_var);
        }
        else
        {
            const type array_type = var_array.get_rank_type(i);
            variant extracted_value = extract_basic_types(json_index_value);
            if (extracted_value.convert(array_type))
                var_array.set_value(i, extracted_value);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void fromjson_recursively(instance obj2, Value& json_object)
{
    instance obj = obj2.get_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
    const auto prop_list = obj.get_derived_type().get_properties();

    for (auto prop : prop_list)
    {

        Value::MemberIterator ret = json_object.FindMember(prop.get_name().data());
        if (ret == json_object.MemberEnd())
            continue;
        const type value_t = prop.get_type();

        auto& json_value = ret->value;
        switch(json_value.GetType())
        {
            case kArrayType:
            {
                variant var = prop.get_value(obj);
                auto array_view = var.create_array_view();
                write_array_recursively(array_view, json_value);

                prop.set_value(obj, var);
                break;
            }
            case kObjectType:
            {
                variant var = prop.get_value(obj);
                fromjson_recursively(var, json_value);
                prop.set_value(obj, var);
                break;
            }
            default:
            {
                variant extracted_value = extract_basic_types(json_value);
                if (extracted_value.convert(value_t)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
                    prop.set_value(obj, extracted_value);
            }
        }
    }
}

} // end namespace anonymous

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

namespace io
{

/////////////////////////////////////////////////////////////////////////////////////////

std::string to_json(rttr::instance obj)
{
    if (!obj.is_valid())
        return std::string();

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    to_json_recursively(obj, writer);

    return sb.GetString();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool from_json(const std::string& json, rttr::instance obj)
{
    Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    if (document.Parse(json.c_str()).HasParseError())
        return 1;

    fromjson_recursively(obj, document);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace io
