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

nonius::benchmark bench_variant_int_string_to_int()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        rttr::variant var = "23";
        meter.measure([&]()
        {
           return var.to_int();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_string_to_float()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        rttr::variant var = "23";
        meter.measure([&]()
        {
            return var.to_float();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_string_to_double()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        rttr::variant var = "23";
        meter.measure([&]()
        {
            return var.to_double();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_string_to_bool()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        rttr::variant var = "23";
        meter.measure([&]()
        {
            return var.to_bool();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


nonius::benchmark bench_variant_float_string_to_int()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0f";
        meter.measure([&]()
        {
           return var.to_int();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_string_to_float()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0f";
        meter.measure([&]()
        {
            return var.to_float();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_string_to_double()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0f";
        meter.measure([&]()
        {
            return var.to_double();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_string_to_bool()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0f";
        meter.measure([&]()
        {
            return var.to_bool();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


nonius::benchmark bench_variant_double_string_to_int()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0";
        meter.measure([&]()
        {
           return var.to_int();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_string_to_float()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0";
        meter.measure([&]()
        {
            return var.to_float();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_string_to_double()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0";
        meter.measure([&]()
        {
            return var.to_double();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_string_to_bool()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        rttr::variant var = "23.0";
        meter.measure([&]()
        {
            return var.to_bool();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


nonius::benchmark bench_variant_bool_string_to_int()
{
    return nonius::benchmark("int", [](nonius::chronometer meter)
    {
        rttr::variant var = "true";
        meter.measure([&]()
        {
           return var.to_int();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_string_to_float()
{
    return nonius::benchmark("float", [](nonius::chronometer meter)
    {
        rttr::variant var = "true";
        meter.measure([&]()
        {
            return var.to_float();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_string_to_double()
{
    return nonius::benchmark("double", [](nonius::chronometer meter)
    {
        rttr::variant var = "true";
        meter.measure([&]()
        {
            return var.to_double();
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_string_to_bool()
{
    return nonius::benchmark("bool", [](nonius::chronometer meter)
    {
        rttr::variant var = "true";
        meter.measure([&]()
        {
            return var.to_bool();
        });
    });
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#if 0
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

#endif

/////////////////////////////////////////////////////////////////////////////////////////

void bench_variant_conversion()
{
    nonius::configuration cfg;
    cfg.title = "rttr::variant conversion";
    
    nonius::html_group_reporter reporter;
    reporter.set_output_file("benchmark_variant_conversion.html");
    
    //////////////////////////////////

    reporter.set_current_group_name("int string to type");
    nonius::benchmark benchmarks_group_1[] = { bench_variant_int_string_to_int(),
                                               bench_variant_int_string_to_float(),
                                               bench_variant_int_string_to_double(),
                                               bench_variant_int_string_to_bool(),
                                               };
    nonius::go(cfg, std::begin(benchmarks_group_1), std::end(benchmarks_group_1), reporter);

    //////////////////////////////////

    reporter.set_current_group_name("float string to type");
    nonius::benchmark benchmarks_group_2[] = { bench_variant_float_string_to_int(),
                                               bench_variant_float_string_to_float(),
                                               bench_variant_float_string_to_double(),
                                               bench_variant_float_string_to_bool(),
                                               };
    nonius::go(cfg, std::begin(benchmarks_group_2), std::end(benchmarks_group_2), reporter);

    //////////////////////////////////

    reporter.set_current_group_name("double string to type");
    nonius::benchmark benchmarks_group_3[] = { bench_variant_double_string_to_int(),
                                               bench_variant_double_string_to_float(),
                                               bench_variant_double_string_to_double(),
                                               bench_variant_double_string_to_bool(),
                                               };
    nonius::go(cfg, std::begin(benchmarks_group_3), std::end(benchmarks_group_3), reporter);

    //////////////////////////////////

    reporter.set_current_group_name("bool string to type");
    nonius::benchmark benchmarks_group_4[] = { bench_variant_bool_string_to_int(),
                                               bench_variant_bool_string_to_float(),
                                               bench_variant_bool_string_to_double(),
                                               bench_variant_bool_string_to_bool(),
                                               };
    nonius::go(cfg, std::begin(benchmarks_group_4), std::end(benchmarks_group_4), reporter);

    reporter.generate_report();
}

/////////////////////////////////////////////////////////////////////////////////////////
