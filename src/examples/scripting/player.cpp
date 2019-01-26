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

#include "player.h"

#include "chaiscript_visitor.h" // important!! place before '#include <rttr/registration>'
#include <rttr/registration>

using namespace rttr;

player::player()
:   player(3, 100)
{

}

player::player(int ammo)
:   player(ammo, 100)
{

}

player::player(int ammo, int hitpoints)
:   bullets(ammo), hp(hitpoints)
{

}

void player::boost()
{
    speed += 10;
}

bool player::shoot()
{
    if (bullets < 1)
        return false;
    --bullets;

    std::cout << "shoot";
    return true;
}

void player::set_hp(int value)
{
    hp = value;
}

int player::get_hp() const
{
    return hp;
}

player player::create_player(int ammo)
{
    return player(ammo);
}

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    rttr::registration::class_<player>("player")
        .constructor<>()
        .constructor<int>()
        .constructor<int, int>()
        .property("hp", &player::get_hp, &player::set_hp)
        .property("speed", &player::speed)
        .property_readonly("bullets", &player::bullets)
        .method("shoot", &player::shoot)
        .method("boost", &player::boost)
        ;
}

