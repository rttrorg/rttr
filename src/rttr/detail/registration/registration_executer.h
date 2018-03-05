/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel@rttr.org>                                 *
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

#ifndef RTTR_REGISTRATION_EXECUTER_H_
#define RTTR_REGISTRATION_EXECUTER_H_

#include "rttr/detail/base/core_prerequisites.h"

#include <functional>
#include <vector>
#include <utility>

namespace rttr
{
namespace detail
{
/*!
 * The \ref registration_executer class will be used to delay the execution of a function call.
 *
 * The purpose of this class is to preserve the registration order during the manual register process.
 * For a certain key, which is ideally the *this* pointer, a function can be registered.
 * This function can be actually empty or not. For already registered keys the function can be overwritten.
 *
 * When an instance of a registration_executer object will be freed,
 * all the contained functions will be executed in the order of its registration.
 */
class RTTR_API registration_executer
{
public:
    registration_executer();
    ~registration_executer();

    void add_registration_func(const void* key, std::function<void(void)> func = std::function<void(void)>());

private:
    using item_type     = std::pair<const void*, std::function<void(void)>>;
    using register_list = std::vector<item_type>;
    register_list m_list;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_REGISTRATION_EXECUTER_H_
