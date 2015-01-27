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

#include "rttr/variant_array.h"

#include "rttr/detail/argument.h"
#include "rttr/detail/instance.h"

using namespace std;

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////////////

variant_array::variant_array(detail::array_container_base* container)
:   _container(container)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::is_valid() const
{
    return (_container ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::is_dynamic() const
{
    if (is_valid())
        return _container->is_dynamic();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

size_t variant_array::get_rank() const
{
    if (is_valid())
        return _container->get_rank();
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
 
type variant_array::get_rank_type(std::size_t index) const
{
    if (is_valid())
        return _container->get_rank_type(index);
    else
        return impl::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type variant_array::get_type() const
{
    if (is_valid())
        return _container->get_type();
    else
        return impl::get_invalid_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::is_raw_array() const
{
    if (is_valid())
        return _container->is_raw_array();
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array::get_size() const
{
    if (is_valid())
        return _container->get_size();
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array::get_size(std::size_t index_1) const
{
    if (is_valid())
        return _container->get_size(index_1);
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array::get_size(std::size_t index_1, std::size_t index_2) const
{
    if (is_valid())
        return _container->get_size(index_1, index_2);
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::size_t variant_array::get_size_variadic(const std::vector<std::size_t>& index_list) const
{
    if (is_valid())
        return _container->get_size_variadic(index_list);
    else
        return 0;
}
        
/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_size(std::size_t new_size)
{
    if (is_valid())
        return _container->set_size(new_size);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_size(std::size_t new_size, std::size_t index_1)
{
    if (is_valid())
        return _container->set_size(new_size, index_1);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_size(std::size_t new_size, std::size_t index_1, std::size_t index_2)
{
    if (is_valid())
        return _container->set_size(new_size, index_1, index_2);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_size_variadic(std::size_t new_size, const std::vector<std::size_t>& index_list)
{
    if (is_valid())
        return _container->set_size_variadic(new_size, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_value(detail::argument arg)
{
    if (is_valid())
        return _container->set_value(arg);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_value(std::size_t index_1, detail::argument arg)
{
    if (is_valid())
        return _container->set_value(arg, index_1);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_value(std::size_t index_1, std::size_t index_2, detail::argument arg)
{
    if (is_valid())
        return _container->set_value(arg, index_1, index_2);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg)
{
    if (is_valid())
        return _container->set_value( arg, index_1, index_2, index_3);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::set_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg)
{
    if (is_valid())
        return _container->set_value_variadic(arg, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array::get_value(std::size_t index_1) const
{
    if (is_valid())
        return _container->get_value(index_1);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array::get_value(std::size_t index_1, std::size_t index_2) const
{
    if (is_valid())
        return _container->get_value(index_1, index_2);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array::get_value(std::size_t index_1, std::size_t index_2, std::size_t index_3) const
{
    if (is_valid())
        return _container->get_value(index_1, index_2, index_3);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant variant_array::get_value_variadic(const std::vector<std::size_t>& index_list) const
{
    if (is_valid())
        return _container->get_value_variadic(index_list);
    else
        return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::insert_value(std::size_t index_1, detail::argument arg)
{
    if (is_valid())
        return _container->insert_value(arg, index_1);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::insert_value(std::size_t index_1, std::size_t index_2, detail::argument arg)
{
    if (is_valid())
        return _container->insert_value(arg, index_1, index_2);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::insert_value(std::size_t index_1, std::size_t index_2, std::size_t index_3, detail::argument arg)
{
    if (is_valid())
        return _container->insert_value(arg, index_1, index_2, index_3);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::insert_value_variadic(const std::vector<std::size_t>& index_list, detail::argument arg)
{
    if (is_valid())
        return _container->insert_value_variadic(arg, index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::remove_value(std::size_t index_1)
{
    if (is_valid())
        return _container->remove_value(index_1);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::remove_value(std::size_t index_1, std::size_t index_2)
{
    if (is_valid())
        return _container->remove_value(index_1, index_2);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::remove_value(std::size_t index_1, std::size_t index_2, std::size_t index_3)
{
    if (is_valid())
        return _container->remove_value(index_1, index_2, index_3);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool variant_array::remove_value_variadic(const std::vector<std::size_t>& index_list)
{
    if (is_valid())
        return _container->remove_value_variadic(index_list);
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void* variant_array::get_ptr() const
{
    if (is_valid())
        return _container->get_ptr();
    else
        return nullptr;
}

} // end namespace rttr
