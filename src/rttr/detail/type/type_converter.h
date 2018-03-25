/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
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

#ifndef RTTR_TYPE_CONVERTER_H_
#define RTTR_TYPE_CONVERTER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/variant.h"
#include "rttr/detail/type/type_register.h"

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

struct type_converter_base
{
    type_converter_base(const type& target_type) : m_target_type(target_type) {}
    virtual variant to_variant(void* data, bool& ok) const = 0;
    virtual type get_source_type() const = 0;
    virtual ~type_converter_base() {};

    type m_target_type;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename TargetType>
struct type_converter_target : type_converter_base
{
    type_converter_target(const type& target_type) : type_converter_base(target_type) {}
    variant to_variant(void* data, bool& ok) const override { return convert(data, ok); }
    virtual TargetType convert(void* data, bool& ok) const = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename TargetType, typename SourceType, typename F>
struct type_converter : type_converter_target<TargetType>
{
    type_converter(const F& acc) : type_converter_target<TargetType>(type::get<TargetType>()), m_acc(acc) { }
    virtual type get_source_type() const override { return type::get<SourceType>(); }
    TargetType convert(void* data, bool& ok) const override
    {
        SourceType* obj = static_cast<SourceType*>(data);
        return m_acc(*obj, ok);
    }

    F m_acc;
};

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_CONVERTER_H_
