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

#ifndef RTTR_META_DATA_H_
#define RTTR_META_DATA_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/variant.h"

namespace rttr
{
namespace detail
{

/*!
 * This class holds meta data.
 * 
 */
class RTTR_API meta_data
{
    public:
        meta_data(variant key, variant value) : m_key(std::move(key)), m_value(std::move(value)) { }
        meta_data(const meta_data& other) : m_key(other.m_key), m_value(other.m_value) {}
        meta_data(meta_data&& other) : m_key(std::move(other.m_key)), m_value(std::move(other.m_value)) {}
        meta_data& operator=(const meta_data& other) { m_key = other.m_key; m_value = other.m_value; return *this; }

        variant get_key() const      { return m_key; }
        variant get_value() const    { return m_value; }

        struct order_by_key
        {
            RTTR_INLINE bool operator () ( const meta_data& _left, const meta_data& _right )  const
            {
                return _left.m_key < _right.m_key;
            }
            RTTR_INLINE bool operator () ( const variant& _left, const meta_data& _right ) const
            {
                return _left < _right.m_key;
            }
            RTTR_INLINE bool operator () ( const meta_data& _left, const variant& _right ) const
            {
                return _left.m_key < _right;
            }
        };

    private:
        variant m_key;
        variant m_value;
};

using meta_data_index = uint32;

} // end namespace detail
} // end namespace rttr

#endif // RTTR_META_DATA_H_
