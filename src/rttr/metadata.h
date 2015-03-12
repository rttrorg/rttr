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

#ifndef RTTR_METADATA_H_
#define RTTR_METADATA_H_

#include "rttr/base/core_prerequisites.h"
#include "rttr/variant.h"

#include <algorithm>
#include <string>
#include <utility>

namespace rttr
{

/*!
 * This class is used to add custom meta data to the binding of a type.
 *
 *
 * \see method, property, enumeration, constructor and destructor
 */
class RTTR_API metadata
{
    public:
        metadata(const metadata& other) : m_key(other.m_key), m_value(other.m_value) {}
        metadata(std::string key, variant value) : m_key(std::move(key)), m_value(std::move(value)) {}
        metadata(int key, variant value) : m_key(std::move(key)), m_value(std::move(value)) {}
        metadata(metadata&& data) : m_key(std::move(data.m_key)), m_value(std::move(data.m_value)) { data.m_key = variant(); data.m_value = variant(); }
        metadata& operator=(metadata other) { std::swap(m_key, other.m_key); std::swap(m_value, other.m_value); return *this; }

        variant get_key() const      { return m_key; }
        variant get_value() const    { return m_value; }

    private:
        variant m_key;
        variant m_value;
};

} // end namespace rttr

#endif // RTTR_METADATA_H_
