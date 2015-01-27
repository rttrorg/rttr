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
#include "catch.hpp"

#include <rttr/type>

#include <vector>
#include <map>
#include <string>

using namespace rttr;

typedef std::map<int, std::string> IntToStringMap;
typedef std::map<int, int> IntToIntMap;

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(IntToStringMap)

RTTR_DECLARE_STANDARD_TYPE_VARIANTS(IntToIntMap)

RTTR_DECLARE_TYPE(int***)

RTTR_DECLARE_TYPE(int[10])

enum E_Alignment
{
    LEFT,
    RIGHT,
    BOTTOM,
    DOWN
};

RTTR_DECLARE_TYPE(E_Alignment)

TEST_CASE("TypeInfoTests - BasicTests", "[type]")
{
    using namespace rttr;

    SECTION("simple basic check")
    {
        int intVar = 23;
        const type intTypeInfo = type::get(intVar);
        CHECK(intTypeInfo.get_name() == "int");
        CHECK(intTypeInfo == type::get<int>());

        bool boolVar = true;
        const type boolTypeInfo = type::get(boolVar);
        CHECK(boolTypeInfo.get_name() == "bool");
        CHECK(boolTypeInfo == type::get<bool>());

        CHECK(boolTypeInfo != intTypeInfo);

        int ***intPtr = NULL;
        CHECK(type::get<int***>() == type::get(intPtr));

    }

    SECTION("check pointer types")
    {
        int intVar = 23;
        int* intPtrVar = &intVar;
        const type intPtrTypeInfo = type::get(intPtrVar);
        CHECK(intPtrTypeInfo.get_name() == "int*");
        CHECK(intPtrTypeInfo == type::get<int*>());

        bool boolVar = true;
        bool* boolPtrVar = &boolVar;
        const type boolPtrTypeInfo = type::get(boolPtrVar);
        CHECK(boolPtrTypeInfo.get_name() == "bool*");
        CHECK(boolPtrTypeInfo == type::get<bool*>());

        CHECK(boolPtrTypeInfo != intPtrTypeInfo);
    }

    SECTION("check that cv was removed")
    {
        int intVar = 42;
        const int constIntVar = 42;
        CHECK(type::get(intVar) == type::get(constIntVar));
        CHECK(type::get<int>() == type::get(constIntVar));
        CHECK(type::get<int>() == type::get<const int>());
        CHECK(type::get<int>() == type::get<const int &>());

        CHECK(type::get<int*>() == type::get(&intVar));
        CHECK(type::get<int*>() == type::get<int *const>());
        CHECK(type::get<const int*>() == type::get(&constIntVar));
        CHECK(type::get<const int*>() == type::get<const int *const>());

        const int& intConstRef = intVar;
        CHECK(type::get<int>() == type::get(intConstRef));
        int*** ptr = NULL;
        CHECK(type::get<int***>() == type::get(ptr));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - ComplexerTypes", "[type]")
{
    std::vector<int> myList;
    std::vector<int> myList2;
    CHECK(type::get<std::vector<int> >() == type::get(myList));
    CHECK(type::get(myList) == type::get(myList2));

    IntToStringMap myMap;
    std::map<int, std::string> myMap2;

    CHECK((type::get<std::map<int, std::string> >() == type::get<IntToStringMap>()));
    CHECK((type::get<std::map<int, std::string> >() == type::get(myMap)));
    CHECK((type::get<IntToStringMap>() == type::get(myMap)));
    CHECK((type::get(myMap) == type::get(myMap2)));

    CHECK((type::get(myMap) != type::get<std::map<int, int> >()));
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - SingleClassInheritance", "[type]")
{

    {
        ClassSingle6A instance6A;
        ClassSingleBase& baseSingle = instance6A;

        // down cast cast
        CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) != NULL);
        CHECK(rttr_cast<ClassSingle3A*>(&baseSingle) != NULL);
        CHECK(rttr_cast<ClassSingle6B*>(&baseSingle) == NULL);

        // up cast cast
        CHECK(rttr_cast<ClassSingleBase*>(&instance6A) != NULL);
        CHECK(rttr_cast<ClassSingle3A*>(&instance6A) != NULL);
        CHECK(rttr_cast<ClassSingle1A*>(&instance6A) != NULL);
    }

    {
        ClassSingle6E instance6E;
        ClassSingleBase& baseSingle = instance6E;

        // down cast cast
        CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) == NULL);
        CHECK(rttr_cast<ClassSingle3E*>(&baseSingle) != NULL);
        CHECK(rttr_cast<ClassSingle6E*>(&baseSingle) != NULL);

        // up cast cast
        CHECK(rttr_cast<ClassSingleBase*>(&instance6E) != NULL);
        CHECK(rttr_cast<ClassSingle3E*>(&instance6E) != NULL);
        CHECK(rttr_cast<ClassSingle1E*>(&instance6E) != NULL);
        CHECK(rttr_cast<ClassSingle6E*>(&instance6E) != NULL);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - MultipleClassInheritance", "[type]")
{
    {
        FinalClass final;
        ClassMultipleBaseA& baseMultiA = final;
        ClassMultipleBaseB& baseMultiB = final;
        ClassMultipleBaseC& baseMultiC = final;
        ClassMultipleBaseD& baseMultiD = final;
        ClassMultipleBaseE& baseMultiE = final;

        // down cast cast
        CHECK(rttr_cast<FinalClass*>(&baseMultiA) != NULL);
        CHECK(rttr_cast<FinalClass*>(&baseMultiB) != NULL);
        CHECK(rttr_cast<FinalClass*>(&baseMultiC) != NULL);
        CHECK(rttr_cast<FinalClass*>(&baseMultiD) != NULL);
        CHECK(rttr_cast<FinalClass*>(&baseMultiE) != NULL);

        // up cast cast
        CHECK(rttr_cast<ClassMultipleBaseA*>(&final) != NULL);
        CHECK(rttr_cast<ClassMultipleBaseB*>(&final) != NULL);
        CHECK(rttr_cast<ClassMultipleBaseC*>(&final) != NULL);
        CHECK(rttr_cast<ClassMultipleBaseD*>(&final) != NULL);
        CHECK(rttr_cast<ClassMultipleBaseE*>(&final) != NULL);

        // down cast cast to the middle
        CHECK(rttr_cast<ClassMultiple3A*>(&baseMultiA) != NULL);
        CHECK(rttr_cast<ClassMultiple3B*>(&baseMultiB) != NULL);
        CHECK(rttr_cast<ClassMultiple3C*>(&baseMultiC) != NULL);
        CHECK(rttr_cast<ClassMultiple3D*>(&baseMultiD) != NULL);
        CHECK(rttr_cast<ClassMultiple3E*>(&baseMultiE) != NULL);
    }

    {
        FinalClass final;
        ClassMultiple6A classMulti6A;
        ClassMultiple3B classMulti3B;
        ClassMultiple1E classMulti1E;

        // check for invalid casts
        CHECK(rttr_cast<FinalClass*>(&classMulti6A) == NULL);

        CHECK(rttr_cast<ClassMultiple4B*>(&classMulti3B) == NULL);

        CHECK(rttr_cast<ClassMultiple2E*>(&classMulti1E) == NULL);
        CHECK(rttr_cast<ClassMultiple6E*>(&classMulti1E) == NULL);

        // check for valid casts
        CHECK(rttr_cast<ClassMultiple5A*>(&classMulti6A) != NULL);
        CHECK(rttr_cast<ClassMultipleBaseA*>(&classMulti6A) != NULL);

        CHECK(rttr_cast<ClassMultiple2B*>(&classMulti3B) != NULL);
        CHECK(rttr_cast<ClassMultiple1B*>(&classMulti3B) != NULL);

        CHECK(rttr_cast<ClassMultipleBaseE*>(&classMulti1E) != NULL);
        CHECK(rttr_cast<ClassMultiple1E*>(&classMulti1E) != NULL);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Virtual Inheritance", "[type]")
{
    DiamondBottom diamond;

    DiamondTop* base    = &diamond;
    DiamondLeft* left   = &diamond;
    DiamondRight* right = &diamond;
 
    CHECK(rttr_cast<DiamondBottom*>(base)   == &diamond);   // top to bottom cast
    CHECK(rttr_cast<DiamondTop*>(&diamond)  == base);       // bottom to top
    CHECK(rttr_cast<DiamondLeft*>(base)     == left);       // base to one level up - left class
    CHECK(rttr_cast<DiamondRight*>(base)    == right);      // base to one level up - right class
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - TypeId/ClassInheritance", "[type]")
{
    ClassSingle6A instance6A;
    ClassSingleBase& baseSingle     = instance6A;
    ClassSingleBase* baseSinglePtr  = &instance6A;

    CHECK(type::get<ClassSingleBase*>() == type::get(baseSinglePtr));

    CHECK(type::get<ClassSingle6A>() == type::get(baseSingle));
    CHECK(type::get<ClassSingleBase*>() == type::get(&baseSingle));

    ClassSingle3A instance3A;
    CHECK(type::get<ClassSingle3A>() == type::get(instance3A));
    CHECK(type::get<ClassSingle6A>() != type::get(instance3A));
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Check is_primitive", "[type]")
{
    CHECK(type::get<int>().is_primitive()               == true);
    CHECK(type::get<float>().is_primitive()             == true);
    CHECK(type::get<double>().is_primitive()            == true);
    CHECK(type::get<char>().is_primitive()              == true);
    CHECK(type::get<bool>().is_primitive()              == true);

    CHECK(type::get<std::string>().is_primitive()       == false);
    CHECK(type::get<ClassSingle6A>().is_primitive()     == false);
    CHECK(type::get<ClassMultiple2B>().is_primitive()   == false);
    CHECK(type::get<FinalClass>().is_primitive()        == false);
    CHECK(type::get<E_Alignment>().is_primitive()       == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Check is_class", "[type]")
{

    CHECK(type::get<std::string>().is_class()     == true);
    CHECK(type::get<ClassSingle6A>().is_class()   == true);
    CHECK(type::get<ClassMultiple2B>().is_class() == true);
    CHECK(type::get<FinalClass>().is_class()      == true);

    CHECK(type::get<int>().is_class()            == false);
    CHECK(type::get<float>().is_class()          == false);
    CHECK(type::get<double>().is_class()         == false);
    CHECK(type::get<char>().is_class()           == false);
    CHECK(type::get<bool>().is_class()           == false);
    CHECK(type::get<E_Alignment>().is_class()    == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Check is_enum", "[type]")
{
    CHECK(type::get<E_Alignment>().is_enumeration()     == true);

    CHECK(type::get<int>().is_enumeration()             == false);
    CHECK(type::get<float>().is_enumeration()           == false);
    CHECK(type::get<double>().is_enumeration()          == false);
    CHECK(type::get<char>().is_enumeration()            == false);
    CHECK(type::get<bool>().is_enumeration()            == false);
    CHECK(type::get<ClassSingle6A>().is_enumeration()   == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Check is_pointer", "[type]")
{
    CHECK(type::get<int*>().is_pointer()             == true);
    CHECK(type::get<ClassSingle6A*>().is_pointer()   == true);
    CHECK(type::get<int***>().is_pointer()           == true);
    CHECK(type::get<char*>().is_pointer()            == true);

    CHECK(type::get<int>().is_pointer()              == false);
    CHECK(type::get<float>().is_pointer()            == false);
    CHECK(type::get<double>().is_pointer()           == false);
    CHECK(type::get<char>().is_pointer()             == false);
    CHECK(type::get<bool>().is_pointer()             == false);
    CHECK(type::get<ClassSingle6A>().is_pointer()    == false);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TypeInfoTests - Check is_array", "[type]")
{
    CHECK(type::get<int[10]>().is_array()        == true);
    CHECK(type::get<char[10]>().is_array()       == true);

    CHECK(type::get<int>().is_array()            == false);
    CHECK(type::get<float>().is_array()          == false);
    CHECK(type::get<int*>().is_array()           == false);
    CHECK(type::get<float*>().is_array()         == false);
    CHECK(type::get<double>().is_array()         == false);
    CHECK(type::get<char>().is_array()           == false);
    CHECK(type::get<bool>().is_array()           == false);
    CHECK(type::get<ClassSingle6A>().is_array()  == false);
}

/////////////////////////////////////////////////////////////////////////////////////////
