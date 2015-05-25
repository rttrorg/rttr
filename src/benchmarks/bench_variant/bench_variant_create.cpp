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

#include <rttr/type>
#include <rttr/register>

#include <nonius/nonius.h++>
#include <nonius/html_group_reporter.h>


/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_string_ctor()
{
    return nonius::benchmark("std::string", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct(std::string("normal text"));
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_char_ctor()
{
    return nonius::benchmark("char", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct("normal text");
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_ctor()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct(42.0f);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_ctor()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct(42.0);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_ctor()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct(42);
        });
    });
}


/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_ctor()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        std::vector<nonius::storage_for<rttr::variant>> vec(meter.runs());
        meter.measure([&](int i ) 
        {
           vec[i].construct(true);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


nonius::benchmark bench_variant_string_dtor()
{
    return nonius::benchmark("std::string", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct(std::string("normal text"));

        meter.measure([&](int i ) 
        {
           vec[i].destruct();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_char_dtor()
{
    return nonius::benchmark("char", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct("normal text");

        meter.measure([&](int i ) 
        {
           vec[i].destruct();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_dtor()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct(42.0f);

        meter.measure([&](int i ) 
        {
           vec[i].destruct();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_dtor()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct(42.0);

        meter.measure([&](int i ) 
        {
           vec[i].destruct();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_dtor()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct(static_cast<int>(42));

        meter.measure([&](int i ) 
        {
            vec[i].destruct();
        });
    });
}


/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_dtor()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        std::vector<nonius::destructable_object<rttr::variant>> vec(meter.runs());
        for(auto&& item : vec)
            item.construct(true);

        meter.measure([&](int i ) 
        {
           vec[i].destruct();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void bench_variant_create()
{
    nonius::configuration cfg;
    cfg.title = "rttr::variant creation";
    
    nonius::html_group_reporter reporter;
    reporter.set_output_file("benchmark_variant.html");
    
    reporter.set_current_group_name("constructor", "The construction of a variant with automatic storage.");
    nonius::benchmark benchmarks_group_1[] = { bench_variant_string_ctor(),
                                               bench_variant_char_ctor(),
                                               bench_variant_double_ctor(),
                                               bench_variant_float_ctor(),
                                               bench_variant_int_ctor(),
                                               bench_variant_bool_ctor()};
    nonius::go(cfg, std::begin(benchmarks_group_1), std::end(benchmarks_group_1), reporter);

    reporter.set_current_group_name("destructor", "The destruction of a variant with automatic storage.");
    nonius::benchmark benchmarks_group_2[] = { bench_variant_string_dtor(),
                                               bench_variant_char_dtor(),
                                               bench_variant_double_dtor(),
                                               bench_variant_float_dtor(),
                                               bench_variant_int_dtor(),
                                               bench_variant_bool_dtor()};
    nonius::go(cfg, std::begin(benchmarks_group_2), std::end(benchmarks_group_2), reporter);

    reporter.generate_report();
}

/////////////////////////////////////////////////////////////////////////////////////////
