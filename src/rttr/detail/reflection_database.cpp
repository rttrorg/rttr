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

#include "rttr/detail/reflection_database_p.h"

#include "rttr/detail/constructor_container_base.h"
#include "rttr/detail/destructor_container_base.h"
#include "rttr/detail/enumeration_container_base.h"
#include "rttr/detail/method_container_base.h"
#include "rttr/detail/property_container.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <algorithm>

using namespace std;

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

bool reflection_database::does_signature_match_arguments(const vector<type>& param_list, const vector<type>& args)
{
    if (param_list.size() == args.size())
    {
        int index = 0;
        for (const auto& arg : args)
        {
            if (arg != param_list[index])
            {
                return false;
            }
            ++index;
        }
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<type> reflection_database::extract_types(const vector<detail::argument>& args)
{
    std::vector<type> result;
    for (const auto& arg : args)
    {
        result.push_back(arg.get_type());
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool reflection_database::is_method_already_registered(const unique_ptr<detail::method_container_base>& method, const method_map& method_container)
{
    const auto ret = method_container.equal_range(method->get_name());
    if (ret.first != method_container.end())
    {
        detail::method_container_base* meth = method.get();
        auto found_meth = find_if(ret.first, ret.second, [meth](const method_map::value_type& item)
        {
            auto curr_meth = reflection_database::instance()._method_list[item.second].get();
            if (does_signature_match_arguments(curr_meth->get_parameter_types(),
                                               meth->get_parameter_types()))
            {
                return true;
            }
            return false;
        });

        if (found_meth != ret.second) // method with same signature found
            return true;
    }

   return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void reflection_database::register_property(unique_ptr<detail::property_container_base> prop, property_map& property_container)
{
    const auto prop_name = prop->get_name();
    if (property_container.find(prop_name) != property_container.end())
        return;

    reflection_database::instance()._property_list.push_back(move(prop));
    property_container.emplace(prop_name, reflection_database::instance()._property_list.size() - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////

// methods that have the same name and signature will be ignored
void reflection_database::register_method(unique_ptr<detail::method_container_base> method, method_map& method_container)
{
    if (is_method_already_registered(method, method_container))
        return; //ignore this method, it's already registered

    auto meth_name = method->get_name();
    reflection_database::instance()._method_list.push_back(move(method));
    method_container.emplace(meth_name, reflection_database::instance()._method_list.size() - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////

detail::property_container_base* reflection_database::find_property(const std::string& name, property_map& property_container)
{
    const auto ret = property_container.find(name);
    if (ret != property_container.end())
        return reflection_database::instance()._property_list[ret->second].get();
    else
        return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief This function searches in \p method_container for a method with name \p name.
 *        When it founds a method it will return a valid pointer, otherwise a nullptr.
 */
detail::method_container_base* reflection_database::find_method(const std::string& name, const method_map& method_container)
{
    const auto ret = method_container.find(name);
    if (ret != method_container.end())
        return reflection_database::instance()._method_list[ret->second].get();
    else
        return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief This function searches in \p method_container for a method with name \p name and with a signature listed in \p args.
 *        When it founds a method it will return a valid pointer, otherwise a nullptr.
 */
detail::method_container_base* reflection_database::find_method(const std::string& name, const std::vector<type>& params, 
                                                                const method_map& method_container)
{
    const auto range = method_container.equal_range(name);
    auto ret = find_if(range.first, range.second, 
                [params](const method_map::value_type& item)
                {
                    const auto meth = reflection_database::instance()._method_list[item.second].get();
                    if (reflection_database::does_signature_match_arguments(meth->get_parameter_types(), params))
                        return true;
                    else
                        return false;
                } );

    if (ret != method_container.end())
        return reflection_database::instance()._method_list[ret->second].get();
    else
        return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////

void reflection_database::class_data::add_constructor(unique_ptr<detail::constructor_container_base> ctor)
{
    for (const auto& curr_ctor : _ctorList)
    {
        if (reflection_database::does_signature_match_arguments(curr_ctor.get()->get_parameter_types(),
                                                                move(ctor.get()->get_parameter_types())))
        {
            return; // don't worry, ctor will destroy itself
        }
    }
    _ctorList.push_back(move(ctor));
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr
