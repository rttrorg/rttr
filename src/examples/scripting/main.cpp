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

#include <iostream>

#include <rttr/type>
#include "chaiscript_visitor.h"

using namespace rttr;

int main(int argc, char** argv)
{
    chaiscript::ChaiScript chai;
    chaiscript_visitor chai_vi(chai);

    // here we bind at runtime, with no compile time information, the class "player"
    // to 'chaiscript'
    chai_vi.visit(type::get_by_name("player"));

    std::string script = R"(
                            var obj = player(12)
                            obj.shoot()
                            obj.speed = 23
                            obj.set_hp(12);
                            print("")
                            print("speed: ${obj.speed}")
                            print("bullets: ${obj.bullets}")
                            print("hp: ${obj.get_hp()}")
                            )";
    try
    {
        chai.eval(script);
    }
    catch(const chaiscript::exception::bad_boxed_cast& e)
    {
        auto f = e.what();
        std::cerr << f;
    }
    catch (const chaiscript::exception::eval_error &e)
    {
        auto f = e.reason;
        std::cerr << f;
    }
    std::cout << "\n";
    return 0;
}

