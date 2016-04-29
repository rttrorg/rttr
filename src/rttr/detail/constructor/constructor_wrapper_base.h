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

#ifndef RTTR_CONSTRUCTOR_WRAPPER_BASE_H_
#define RTTR_CONSTRUCTOR_WRAPPER_BASE_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/metadata/metadata_handler.h"
#include "rttr/parameter_info.h"
#include "rttr/access_levels.h"
#include "rttr/array_range.h"
#include "rttr/string_view.h"

#include <string>
#include <vector>

namespace rttr
{
class type;
class variant;
class argument;

namespace detail
{

/*!
 * Abstract class for a method.
 *
 * This is the base class for all methods.
 * You can invoke the method.
 */
class RTTR_API constructor_wrapper_base
{
    public:
        constructor_wrapper_base();

        virtual ~constructor_wrapper_base();
        virtual type get_declaring_type() const = 0;
        string_view get_signature() const;
        virtual access_levels get_access_level() const = 0;
        virtual type get_instanciated_type() const = 0;
        virtual std::vector<bool> get_is_reference() const = 0;
        virtual std::vector<bool> get_is_const() const = 0;
        virtual array_range<parameter_info> get_parameter_infos() const = 0;
        virtual variant get_metadata(const variant& key) const = 0;

        virtual variant invoke() const = 0;
        virtual variant invoke(argument& arg1) const = 0;
        virtual variant invoke(argument& arg1, argument& arg2) const = 0;
        virtual variant invoke(argument& arg1, argument& arg2, argument& arg3) const = 0;
        virtual variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4) const = 0;
        virtual variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4,
                               argument& arg5) const = 0;
        virtual variant invoke(argument& arg1, argument& arg2, argument& arg3, argument& arg4,
                               argument& arg5, argument& arg6) const = 0;

        virtual variant invoke_variadic(std::vector<argument>& args) const = 0;
    protected:
        void init();
    private:
        void create_signature_string();
    private:
        string_view m_signature_view;
        std::string m_signature;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_CONSTRUCTOR_WRAPPER_BASE_H_
