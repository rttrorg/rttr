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

#include "test_classes.h"
#include "benchmark_utils.h"

#include <iostream>
#include <chrono>
#include <memory>

#define ITEM_COUNT 50000
#define RUN_COUNT 5
#define COLUMN_0_WIDTH 41
#define COLUMN_1_WIDTH 14
#define COLUMN_2_WIDTH 14
#define TABLE_WIDTH COLUMN_0_WIDTH + COLUMN_1_WIDTH + COLUMN_2_WIDTH + 8

using namespace rttr;
using namespace std;


struct BenchmarkResult
{
    BenchmarkResult(const string& title, chrono::high_resolution_clock::duration time) : _title(title), _timing(time) {}
    string                                  _title;
    chrono::high_resolution_clock::duration _timing;
};

void printStats(const vector<BenchmarkResult>& timingResults)
{
    cout << "-------------------------------------------------------\n";
    for (vector<BenchmarkResult>::const_iterator itr = timingResults.begin();
         itr != timingResults.end();
         ++itr)
    {
        if (!(*itr)._title.empty())
            cout << (*itr)._title << ": " << chrono::duration_cast<chrono::milliseconds>((*itr)._timing).count() << " ms\n";
    }
    cout << "-------------------------------------------------------\n";
}

void print_stats(const std::string& text, const chrono::high_resolution_clock::duration& time_1, 
                                          const chrono::high_resolution_clock::duration& time_2)
{
    std::cout << left(text, COLUMN_0_WIDTH) << " | "
              << center(std::to_string(chrono::duration_cast<chrono::milliseconds>(time_1).count()) + " ms", COLUMN_1_WIDTH) << " | "
              << center(std::to_string(chrono::duration_cast<chrono::milliseconds>(time_2).count()) + " ms", COLUMN_2_WIDTH) << " | "
              << std::endl;
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique_impl(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


void bench_single_inheritance_level_1()
{
    vector<unique_ptr<ClassSingleBase> > vec;
    vec.reserve(ITEM_COUNT * 5);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassSingle1A>());
        vec.push_back(make_unique_impl<ClassSingle1B>());
        vec.push_back(make_unique_impl<ClassSingle1C>());
        vec.push_back(make_unique_impl<ClassSingle1D>());
        vec.push_back(make_unique_impl<ClassSingle1E>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle1A* item = dynamic_cast<ClassSingle1A*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1B* item = dynamic_cast<ClassSingle1B*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1C* item = dynamic_cast<ClassSingle1C*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1D* item = dynamic_cast<ClassSingle1D*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1E* item = dynamic_cast<ClassSingle1E*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;
    
    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle1A* item = rttr_cast<ClassSingle1A*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1B* item = rttr_cast<ClassSingle1B*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1C* item = rttr_cast<ClassSingle1C*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1D* item = rttr_cast<ClassSingle1D*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle1E* item = rttr_cast<ClassSingle1E*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_single_inheritance_level_1" << std::endl;
    
    print_stats("downcasting to level 1", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_single_inheritance_level_3()
{
    vector<unique_ptr<ClassSingleBase> > vec;
    vec.reserve(ITEM_COUNT * 5);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassSingle3A>());
        vec.push_back(make_unique_impl<ClassSingle3B>());
        vec.push_back(make_unique_impl<ClassSingle3C>());
        vec.push_back(make_unique_impl<ClassSingle3D>());
        vec.push_back(make_unique_impl<ClassSingle3E>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle3A* item = dynamic_cast<ClassSingle3A*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3B* item = dynamic_cast<ClassSingle3B*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3C* item = dynamic_cast<ClassSingle3C*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3D* item = dynamic_cast<ClassSingle3D*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3E* item = dynamic_cast<ClassSingle3E*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;
    
    vector<void*> result;
    result.resize(vec.size());
    chrono::high_resolution_clock::duration elapsedTime_2;
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle3A* item = rttr_cast<ClassSingle3A*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3B* item = rttr_cast<ClassSingle3B*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3C* item = rttr_cast<ClassSingle3C*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3D* item = rttr_cast<ClassSingle3D*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle3E* item = rttr_cast<ClassSingle3E*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_single_inheritance_level_3" << std::endl;
    
    print_stats("downcasting to level 3", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_single_inheritance_level_6()
{
    vector<unique_ptr<ClassSingleBase> > vec;
    vec.reserve(ITEM_COUNT * 5);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassSingle6A>());
        vec.push_back(make_unique_impl<ClassSingle6B>());
        vec.push_back(make_unique_impl<ClassSingle6C>());
        vec.push_back(make_unique_impl<ClassSingle6D>());
        vec.push_back(make_unique_impl<ClassSingle6E>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle6A* item = dynamic_cast<ClassSingle6A*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6B* item = dynamic_cast<ClassSingle6B*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6C* item = dynamic_cast<ClassSingle6C*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6D* item = dynamic_cast<ClassSingle6D*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6E* item = dynamic_cast<ClassSingle6E*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;

    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassSingle6A* item = rttr_cast<ClassSingle6A*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6B* item = rttr_cast<ClassSingle6B*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6C* item = rttr_cast<ClassSingle6C*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6D* item = rttr_cast<ClassSingle6D*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassSingle6E* item = rttr_cast<ClassSingle6E*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_single_inheritance_level_6" << std::endl;
    
    print_stats("downcasting to level 6", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_virtual_inheritance_level_1()
{
    vector<unique_ptr<ClassDiamondTop> > vec;

    vec.reserve(ITEM_COUNT * 3);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassDiamondLeft1>());
        vec.push_back(make_unique_impl<ClassDiamondMiddle1>());
        vec.push_back(make_unique_impl<ClassDiamondRight1>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft1* item = dynamic_cast<ClassDiamondLeft1*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle1* item = dynamic_cast<ClassDiamondMiddle1*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight1* item = dynamic_cast<ClassDiamondRight1*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;
    
    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft1* item = rttr_cast<ClassDiamondLeft1*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle1* item = rttr_cast<ClassDiamondMiddle1*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight1* item = rttr_cast<ClassDiamondRight1*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_virtual_inheritance_level_1" << std::endl;
    
    print_stats("downcasting to level 1 - virtual inheri.", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_virtual_inheritance_level_3()
{
    vector<unique_ptr<ClassDiamondTop> > vec;

    vec.reserve(ITEM_COUNT * 3);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassDiamondLeft3>());
        vec.push_back(make_unique_impl<ClassDiamondMiddle3>());
        vec.push_back(make_unique_impl<ClassDiamondRight3>());
    }

   chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft3* item = dynamic_cast<ClassDiamondLeft3*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle3* item = dynamic_cast<ClassDiamondMiddle3*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight3* item = dynamic_cast<ClassDiamondRight3*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;
        
    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft3* item = rttr_cast<ClassDiamondLeft3*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle3* item = rttr_cast<ClassDiamondMiddle3*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight3* item = rttr_cast<ClassDiamondRight3*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_virtual_inheritance_level_3" << std::endl;
    
    print_stats("downcasting to level 3 - virtual inheri.", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_virtual_inheritance_level_6()
{
    vector<unique_ptr<ClassDiamondTop> > vec;

    vec.reserve(ITEM_COUNT * 3);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassDiamondLeft6>());
        vec.push_back(make_unique_impl<ClassDiamondMiddle6>());
        vec.push_back(make_unique_impl<ClassDiamondRight6>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft6* item = dynamic_cast<ClassDiamondLeft6*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle6* item = dynamic_cast<ClassDiamondMiddle6*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight6* item = dynamic_cast<ClassDiamondRight6*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;
    
    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassDiamondLeft6* item = rttr_cast<ClassDiamondLeft6*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondMiddle6* item = rttr_cast<ClassDiamondMiddle6*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            else if (ClassDiamondRight6* item = rttr_cast<ClassDiamondRight6*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
            
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_virtual_inheritance_level_6" << std::endl;
    
    print_stats("downcasting to level 6 - virtual inheri.", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_multiple_class_hierachy_end()
{
    vector<unique_ptr<ClassMultipleBaseA> > vec;

    vec.reserve(ITEM_COUNT * 7);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<FinalClass>());
        vec.push_back(make_unique_impl<ClassMultiple5A>());
        vec.push_back(make_unique_impl<FinalClass>());
        vec.push_back(make_unique_impl<ClassMultiple4A>());
        vec.push_back(make_unique_impl<FinalClass>());
        vec.push_back(make_unique_impl<ClassMultiple5A>());
        vec.push_back(make_unique_impl<FinalClass>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (FinalClass* item = dynamic_cast<FinalClass*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;


    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (int i = 0; i < ITEM_COUNT * 7; ++i)
        {
            if (FinalClass* item = rttr_cast<FinalClass*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_multiple_class_hierachy_end" << std::endl;
    
    print_stats("Cast from Base to Final - multi inheri.", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_cross_cast_1()
{
    vector<shared_ptr<ClassMultipleBaseA> > vec;

    vec.reserve(ITEM_COUNT * 2);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<FinalClass>());
        vec.push_back(make_unique_impl<ClassMultiple6A>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassMultiple1B* item = dynamic_cast<ClassMultiple1B*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;

    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassMultiple1B* item = rttr_cast<ClassMultiple1B*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_cross_cast_1" << std::endl;
    
    print_stats("crosscasting - 2 classes wide", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_cross_cast_2()
{
    vector<shared_ptr<ClassMultipleBaseA> > vec;

    vec.reserve(ITEM_COUNT * 2);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<FinalClass>());
        vec.push_back(make_unique_impl<ClassMultiple6A>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassMultiple1E* item = dynamic_cast<ClassMultiple1E*>(vec[i].get()))
            {
                result_reference[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;

    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (ClassMultiple1E* item = rttr_cast<ClassMultiple1E*>(vec[i].get()))
            {
                result[i] = reinterpret_cast<void*>(item);
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_cross_cast_2" << std::endl;
    
    print_stats("crosscasting - 4 classes wide", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void bench_type_get_vs_typeid()
{
    vector<unique_ptr<ClassSingleBase> > vec;

    vec.reserve(ITEM_COUNT * 5);
    for (int i = 0; i < ITEM_COUNT; ++i)
    {
        vec.push_back(make_unique_impl<ClassSingle6A>());
        vec.push_back(make_unique_impl<ClassSingle6B>());
        vec.push_back(make_unique_impl<ClassSingle6C>());
        vec.push_back(make_unique_impl<ClassSingle6D>());
        vec.push_back(make_unique_impl<ClassSingle6E>());
    }

    chrono::high_resolution_clock::duration elapsedTime_1;
    vector<void*> result_reference;
    result_reference.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (typeid(ClassSingle6A) == typeid(*vec[i]))
            {
                result_reference[i] = vec[i]->get_ptr();
            }
            else if (typeid(ClassSingle6B) == typeid(*vec[i]))
            {
                result_reference[i] = vec[i]->get_ptr();
            }
            else if (typeid(ClassSingle6C) == typeid(*vec[i]))
            {
                result_reference[i] = vec[i]->get_ptr();
            }
            else if (typeid(ClassSingle6D) == typeid(*vec[i]))
            {
                result_reference[i] = vec[i]->get_ptr();
            }
            else if (typeid(ClassSingle6E) == typeid(*vec[i]))
            {
                result_reference[i] = vec[i]->get_ptr();
            }
        }
        elapsedTime_1 += chrono::high_resolution_clock::now() - startTime;
    }
    elapsedTime_1 /= RUN_COUNT;

    chrono::high_resolution_clock::duration elapsedTime_2;
    vector<void*> result;
    result.resize(vec.size());
    for (int count = 0; count < RUN_COUNT; ++count)
    {
        const auto startTime = chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (type::get<ClassSingle6A>() == type::get(*vec[i]))
            {
                result[i] = vec[i]->get_ptr();
            }
            else if (type::get<ClassSingle6B>() == type::get(*vec[i]))
            {
                result[i] = vec[i]->get_ptr();
            }
            else if (type::get<ClassSingle6C>() == type::get(*vec[i]))
            {
                result[i] = vec[i]->get_ptr();
            }
            else if (type::get<ClassSingle6D>() == type::get(*vec[i]))
            {
                result[i] = vec[i]->get_ptr();
            }
            else if (type::get<ClassSingle6E>() == type::get(*vec[i]))
            {
                result[i] = vec[i]->get_ptr();
            }
        }
        elapsedTime_2 += chrono::high_resolution_clock::now() - startTime;
    }

    elapsedTime_2 /= RUN_COUNT;

    if (result != result_reference)
        std::cout << "ERROR - Invalid rttr_cast - bench_type_get_vs_typeid" << std::endl;
    
    print_stats("BaseClass to Level 6 [typeid]", elapsedTime_1, elapsedTime_2);
}

/////////////////////////////////////////////////////////////////////////////////////////

void start_rttr_cast_benchmark()
{
    std::cout << center("dynamic_cast vs. rttr_cast", TABLE_WIDTH) << std::endl;
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    std::cout << left("", COLUMN_0_WIDTH) << " | "
              << center("[dynamic_cast]", COLUMN_1_WIDTH)    << " | "
              << center("[rttr_cast]", COLUMN_2_WIDTH)       << " | "
              << "\n";

    std::cout << std::string(TABLE_WIDTH, '-') << "\n";

    bench_single_inheritance_level_1();
    bench_single_inheritance_level_3();
    bench_single_inheritance_level_6();
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    bench_virtual_inheritance_level_1();
    bench_virtual_inheritance_level_3();
    bench_virtual_inheritance_level_6();
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    bench_multiple_class_hierachy_end();
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    bench_cross_cast_1();
    bench_cross_cast_2();
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
    bench_type_get_vs_typeid();
    std::cout << std::string(TABLE_WIDTH, '-') << "\n";
}