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

#ifndef RTTR_META_DATA_HANDLER_H_
#define RTTR_META_DATA_HANDLER_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/meta_data/meta_data.h"
#include "rttr/variant.h"

namespace rttr
{
namespace detail
{

/*!
 * This class holds an index to possible meta data.
 * This can be also a name or the declaring type of a property or method.
 */
class RTTR_API meta_data_handler
{
    public:
        meta_data_handler();
        virtual ~meta_data_handler();

        void add_meta_data(detail::meta_data data) const;
        variant get_meta_data(const variant& key) const;

        RTTR_INLINE uint32 get_meta_index() const { return m_index; }

    private:
        uint32 m_index;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_META_DATA_HANDLER_H_
