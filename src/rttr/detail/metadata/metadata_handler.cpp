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

#include "rttr/detail/metadata/metadata_handler.h"
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

metadata_handler::metadata_handler()
:   m_index(get_global_index())
{
}

/////////////////////////////////////////////////////////////////////////////////////////

metadata_handler::~metadata_handler()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

void metadata_handler::add_metadata(detail::metadata data) const 
{
    type_database::instance().add_metadata(m_index, std::move(data));
}

/////////////////////////////////////////////////////////////////////////////////////////

variant metadata_handler::get_metadata(const variant& key) const
{
    return type_database::instance().get_metadata(m_index, key);
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
