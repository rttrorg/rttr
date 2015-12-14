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

#include "rttr/detail/meta_data/meta_data_handler.h"
#include "rttr/detail/type/type_database_p.h"

namespace rttr
{
namespace detail
{

static uint32 get_global_index()
{
    static uint32 item_count;
    return item_count++;
}

/////////////////////////////////////////////////////////////////////////////////////////

meta_data_handler::meta_data_handler()
:   m_index(get_global_index())
{
}

/////////////////////////////////////////////////////////////////////////////////////////

meta_data_handler::~meta_data_handler()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

void meta_data_handler::add_meta_data(detail::meta_data data) const 
{
    type_database::instance().add_meta_data(m_index, std::move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant meta_data_handler::get_meta_data(const variant& key) const
{
    return type_database::instance().get_meta_data(m_index, key);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
