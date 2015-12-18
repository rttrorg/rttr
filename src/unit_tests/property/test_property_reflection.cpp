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

#include "unit_tests/property/test_property_reflection.h"

#include <rttr/registration>

using namespace rttr;
using namespace std;

#include <iostream>
#include <memory>
#include <functional>

#include <catch/catch.hpp>

/////////////////////////////////////////////////////////////////////////////////////////
// init static variables and some global functions added as properties to the test class

int get_propert_p9() { return 42; }
int property_test::_p5= 23;
const int property_test::_p6 = 12;
static float singleton_property = 3.1425f;
static std::string p11;

const std::string& my_p11_getter()
{
    return p11;
}

void my_p11_setter(const std::string& text)
{
    p11 = text;
}

enum E_MetaData
{
    SCRIPTABLE  = 0,
    TOOL_TIP    = 1,
    DESCRIPTION = 2
};

static void my_callback(int)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

static const double pi = 3.124256;
static std::string Global_Text;

std::string& get_global_text() { return Global_Text; }
static std::vector<int> global_array(1000, 42);
const int* ptr_type;
/////////////////////////////////////////////////////////////////////////////////////////

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<property_test>("property_test")
        .constructor<>() ( policy::ctor::as_raw_ptr )
        .property("p1",    &property_test::_p1)
        .property_readonly("p2",     &property_test::_p2)
        .property("p3",    &property_test::_p3)
        .property_readonly("p4",     &property_test::_p4)
        .property("p5",    &property_test::_p5)
        (
            metadata(E_MetaData::SCRIPTABLE, true), 
            metadata(E_MetaData::TOOL_TIP, "This is property 5.")
        )
        .property_readonly("p6",     &property_test::_p6)
        (
            metadata(E_MetaData::SCRIPTABLE, false)
        )
        .property("p7",    &property_test::get_p7, &property_test::set_p7)
        .property("p8",    &singleton_property)
        (
            metadata("Global_Tag", true)
        )
        .property_readonly("p9",     &get_propert_p9)
        .property_readonly("p10",    std::function<int()>([](){ return 45;}))
        .property("p11",   &my_p11_getter, &my_p11_setter)
        .property("p12",   std::function<const string&(void)>(&my_p11_getter), std::function<void(const std::string&)>(&my_p11_setter))
        .property("array_copy", &property_test::_array)
        .property("array", &property_test::_array)
        (
            policy::prop::bind_as_ptr
        )
        .property("raw_array", &property_test::_other_array)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p7_as_ptr_read_only", &property_test::get_p7)
        (
            policy::prop::bind_as_ptr
        )
        .property("p7_as_ptr", &property_test::get_p7, &property_test::set_p7)
        (
            policy::prop::bind_as_ptr
        )
        ;


    /////////////////////////////////////////

    registration::class_<ns_property::top>("ns_property::top")
        .property("p1", &ns_property::top::_p1);

    registration::class_<ns_property::left>("ns_property::left")
        .constructor<>()
        .property("p2", &ns_property::left::_p2);

    registration::class_<ns_property::right>("ns_property::right")
        .constructor<>()
        .property("p3", &ns_property::right::_p3)
        .property("p2", &p11); // double property

    registration::class_<ns_property::right_2>("ns_property::right_2")
        .constructor<>()
        .property("p4", &ns_property::right_2::_p4);

    registration::class_<ns_property::bottom>("ns_property::bottom")
        .constructor<>()
        .property("p5", &ns_property::bottom::_p5)
        .property("callback", &ns_property::bottom::get_function_cb, &ns_property::bottom::set_function_cb);

    /////////////////////////////////////////

    registration::property_readonly("PI", &pi)
        .property("Global_Text", &Global_Text)
        // test policies
        .property("p1_as_ptr", &global_array)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p2_as_ptr", &get_global_text)
        (
            policy::prop::bind_as_ptr
        )
        .property_readonly("p3_as_ptr", std::function<const string&(void)>(&my_p11_getter))
        (
            policy::prop::bind_as_ptr
        )
        .property("p4_as_ptr", std::function<const string&(void)>(&my_p11_getter), std::function<void(const std::string&)>(&my_p11_setter))
        (
            policy::prop::bind_as_ptr
        );
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test Properties", "[property]") 
{
    type prop_type = type::get<property_test>();
    REQUIRE(prop_type.is_valid() == true);
    
    REQUIRE(prop_type.get_property("p1").is_valid() == true);
    REQUIRE(prop_type.get_property("p2").is_valid() == true);
    REQUIRE(prop_type.get_property("p3").is_valid() == true);
    REQUIRE(prop_type.get_property("p4").is_valid() == true);
    REQUIRE(prop_type.get_property("p5").is_valid() == true);
    REQUIRE(prop_type.get_property("p6").is_valid() == true);
    REQUIRE(prop_type.get_property("p7").is_valid() == true);
    // invalid test
    REQUIRE(prop_type.get_property("p23").is_valid() == false);

    SECTION("Test properties - read/write")
    {
        variant prop_inst = prop_type.create({});
        REQUIRE(prop_inst.is_valid() == true);

        property p1 = prop_type.get_property("p1");
        REQUIRE(p1.is_readonly() == false);
        
        // access
        bool ret = p1.set_value(prop_inst, 2);
        REQUIRE(ret == true);
    
        variant val = p1.get_value(prop_inst);
        REQUIRE(val.is_type<int>() == true);

        REQUIRE(val.get_value<int>() == 2);

        ///////////////////////////////////////////
        // class bool property
        property p3 = prop_type.get_property("p3");
        REQUIRE(p3.is_readonly() == false);
        
        // access
        ret = p3.set_value(prop_inst, false);
        REQUIRE(ret == true);
    
        val = p3.get_value(prop_inst);
        REQUIRE(val.is_type<bool>() == true);
        REQUIRE(val.get_value<bool>() == false);

        ///////////////////////////////////////////
        // class static int property
        property p5 = prop_type.get_property("p5");
        REQUIRE(p5.is_readonly() == false);
        
        // access
        ret = p5.set_value(prop_inst, 1000);
        REQUIRE(ret == true);
    
        val = p5.get_value(prop_inst);
        REQUIRE(val.is_type<int>() == true);
        REQUIRE(val.get_value<int>() == 1000);


        ///////////////////////////////////////////
        // a global static property
        property p8 = prop_type.get_property("p8");
        REQUIRE(p8.is_readonly() == false);
        REQUIRE(p8.is_static() == true);
        
        // access
        ret = p8.set_value(prop_inst, 5.678f);
        REQUIRE(ret == true);
    
        val = p8.get_value(prop_inst);
        REQUIRE(val.is_type<float>() == true);
        REQUIRE(val.get_value<float>() == 5.678f);

        ///////////////////////////////////////////
        // a getter/setter member function property
        property p7 = prop_type.get_property("p7");
        REQUIRE(p7.is_readonly() == false);
        
        // access
        ret = p7.set_value(prop_inst, string("hello world"));
        REQUIRE(ret == true);
    
        val = p7.get_value(prop_inst);
        REQUIRE(val.is_type<std::string>() == true);
        REQUIRE(val.get_value<std::string>() == "hello world");

        ///////////////////////////////////////////
        // a getter/setter function property
        property p11 = prop_type.get_property("p11");
        REQUIRE(p11.is_readonly() == false);
        
        // access
        ret = p11.set_value(prop_inst, string("hello world"));
        REQUIRE(ret == true);
    
        val = p11.get_value(prop_inst);
        REQUIRE(val.is_type<std::string>() == true);
        REQUIRE(val.get_value<std::string>() == "hello world");

        ///////////////////////////////////////////
        // a getter/setter function property
        property p12 = prop_type.get_property("p12");
        REQUIRE(p12.is_readonly() == false);
        
        // access
        ret = p12.set_value(prop_inst, string("Another Text"));
        REQUIRE(ret == true);
    
        val = p12.get_value(prop_inst);
        REQUIRE(val.is_type<std::string>() == true);
        REQUIRE(val.get_value<std::string>() == "Another Text");

        // test property on stack
        variant prop_stack = property_test();
        p7.set_value(prop_stack, string("Hello World"));

        val = p7.get_value(prop_stack);
        REQUIRE(val.is_type<std::string>() == true);
        REQUIRE(val.get_value<std::string>() == "Hello World");

        prop_type.destroy(prop_inst);
        REQUIRE(prop_inst.is_valid() == false);
    }

    SECTION("test read only properties")
    {
        variant prop_inst = prop_type.create({});
        REQUIRE(prop_type.is_valid() == true);

        // self reflected const property
        property p2 = prop_type.get_property("p2");
        bool ret = p2.set_value(prop_inst, 45);
        REQUIRE(ret == false);
        REQUIRE(p2.is_readonly() == true);

        variant val = p2.get_value(prop_inst);
        REQUIRE(val.is_valid() == true);
        REQUIRE(val.is_type<short int>() == true);
        REQUIRE(val.get_value<short int>() == 12);

        ///////////////////////////////////////////
        // class const property
        property p4 = prop_type.get_property("p4");
        REQUIRE(p4.get_declaring_type() == prop_type);
        REQUIRE(p4.get_type() == type::get<double>());

        ret = p4.set_value(prop_inst, 12.0);
        REQUIRE(ret == false);
        REQUIRE(p4.is_readonly() == true);

        val = p4.get_value(prop_inst);
        REQUIRE(val.is_valid() == true);
        REQUIRE(val.is_type<double>() == true);
        REQUIRE(val.get_value<double>() == 23.0);

        ///////////////////////////////////////////
        // class static const property
        property p6 = prop_type.get_property("p6");
        ret = p6.set_value(prop_inst, 12.0);
        REQUIRE(ret == false);
        REQUIRE(p6.is_readonly() == true);

        val = p6.get_value(prop_inst);
        REQUIRE(val.is_valid() == true);
        REQUIRE(val.is_type<int>() == true);
        REQUIRE(val.get_value<int>() == 12);

        ///////////////////////////////////////////
        // global function property
        property p9 = prop_type.get_property("p9");
        ret = p9.set_value(prop_inst, 12.0);
        REQUIRE(ret == false);
        REQUIRE(p9.is_readonly() == true);

        val = p9.get_value(prop_inst);
        REQUIRE(val.is_valid() == true);
        REQUIRE(val.is_type<int>() == true);
        REQUIRE(val.get_value<int>() == 42);

        ///////////////////////////////////////////
        // std::function property
        property p10 = prop_type.get_property("p10");
        ret = p10.set_value(prop_inst, 12.0);
        REQUIRE(ret == false);
        REQUIRE(p10.is_readonly() == true);

        val = p10.get_value(prop_inst);
        REQUIRE(val.is_valid() == true);
        REQUIRE(val.is_type<int>() == true);
        REQUIRE(val.get_value<int>() == 45);

        prop_type.destroy(prop_inst);
        REQUIRE(prop_inst.is_valid() == false);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Invoke property via wrapper", "[property]") 
{
    SECTION("test property access via shared_ptr wrapper")
    {
        std::shared_ptr<property_test> obj = std::make_shared<property_test>();

        type obj_t = type::get(obj);
        REQUIRE(obj_t.is_wrapper() == true);

        type wrapper_t = obj_t.get_wrapped_type();
        REQUIRE(wrapper_t.is_wrapper() == false);
        REQUIRE(wrapper_t == type::get<property_test*>());
        property p1 = wrapper_t.get_property("p1");
        CHECK(p1.is_readonly() == false);
        // access
        bool ret = p1.set_value(obj, 2);
        CHECK(ret == true);
    
        variant val = p1.get_value(obj);
        CHECK(val.is_type<int>() == true);
    
        CHECK(val.get_value<int>() == 2);
        CHECK(obj.get()->_p1 == 2);
    }

    SECTION("test property access via variant with shared_ptr wrapper")
    {
        variant var = std::make_shared<property_test>();
        CHECK(var.get_type().is_wrapper() == true);
        CHECK(var.get_type() == type::get<shared_ptr<property_test>>());
        CHECK(var.get_type().get_wrapped_type() == type::get<property_test*>());

        type wrapper_t = var.get_type().get_wrapped_type();
        property p1 = wrapper_t.get_property("p1");
        CHECK(p1.is_readonly() == false);
        // access
        bool ret = p1.set_value(var, 2);
        CHECK(ret == true);
    
        variant val = p1.get_value(var);
        CHECK(val.is_type<int>() == true);
        CHECK(val.get_value<int>() == 2);
    }

    SECTION("test property access via reference_wrapper")
    {
        property_test instance;
        std::reference_wrapper<property_test> obj = std::ref(instance);

        type obj_t = type::get(obj);
        REQUIRE(obj_t.is_wrapper() == true);

        type wrapper_t = obj_t.get_wrapped_type();
        CHECK(wrapper_t.is_wrapper() == false);
        CHECK(wrapper_t == type::get<property_test>());

        property p1 = wrapper_t.get_property("p1");
        CHECK(p1.is_readonly() == false);
        // access
        bool ret = p1.set_value(obj, 2);
        CHECK(ret == true);
    
        variant val = p1.get_value(obj);
        CHECK(val.is_type<int>() == true);
        CHECK(val.get_value<int>() == 2);
        CHECK(obj.get()._p1 == 2);
        CHECK(instance._p1 == 2);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test property array access", "[property]") 
{
    SECTION("test raw array")
    {
        property_test obj;
        type bottom_type = type::get(obj);
        property prop = bottom_type.get_property("array_copy");
        REQUIRE(bool(prop) == true);
        variant var = prop.get_value(obj);
        REQUIRE(var.is_type<std::vector<int>>() == true);
        
        variant_array_view var_array = var.create_array_view();
        var_array.set_size(10);
        prop.set_value(obj, var);
        REQUIRE(obj._array.size() == 10);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test property policy", "[property]") 
{
    SECTION("global property - array - return value converted to pointer")
    {
        property p1 = type::get_global_property("p1_as_ptr");
        REQUIRE(p1.get_type() == type::get<std::vector<int>*>());
        REQUIRE(p1.is_readonly() == false);
        REQUIRE(p1.is_array() == true);

        variant ret = p1.get_value(instance());
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.is_type<std::vector<int>*>() == true);
        bool could_set_value = p1.set_value(instance(), ret);
        REQUIRE(could_set_value == true);

        std::vector<int>* vec = ret.get_value<std::vector<int>*>();
        REQUIRE(vec == &global_array);
    }

    SECTION("global property - read only function - return value converted to pointer")
    {
        property p2 = type::get_global_property("p2_as_ptr");
        REQUIRE(p2.get_type() == type::get<const std::string*>());
        REQUIRE(p2.is_readonly() == true);

        variant ret = p2.get_value(instance());
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.is_type<const std::string*>() == true);
        p2.set_value(instance(), ret);
    }

    SECTION("global property - read only std::function - return value converted to pointer")
    {
        property p3 = type::get_global_property("p3_as_ptr");
        REQUIRE(p3.get_type() == type::get<const std::string*>());
        REQUIRE(p3.is_readonly() == true);
        
        variant ret = p3.get_value(instance());
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.is_type<const std::string*>() == true);
    }


    SECTION("global property - read only std::function - return value converted to pointer")
    {
        property p3 = type::get_global_property("p3_as_ptr");
        REQUIRE(p3.get_type() == type::get<const std::string*>());
        REQUIRE(p3.is_readonly() == true);
        
        variant ret = p3.get_value(instance());
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.is_type<const std::string*>() == true);
    }

    SECTION("global property - setter/getter function - return value converted to pointer")
    {
        property p4 = type::get_global_property("p4_as_ptr");
        REQUIRE(p4.is_array() == false);
        REQUIRE(p4.get_type() == type::get<const std::string*>());
        REQUIRE(p4.is_readonly() == false);
        
        variant ret = p4.get_value(instance());
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.is_type<const std::string*>() == true);

        bool could_set_value = p4.set_value(instance(), ret);
        REQUIRE(could_set_value == true);
    }

    SECTION("class property - raw array")
    {
        property_test obj;
        type prop_type = type::get(obj);

  
        property array_prop = prop_type.get_property("raw_array");
        REQUIRE(array_prop.is_array() == true);
        variant array_obj = array_prop.get_value(obj); // the array is returned by pointer
        variant_array_view other_array = array_obj.create_array_view();
        REQUIRE(other_array.is_valid() == true);

        variant ret = other_array.get_value(23);
        REQUIRE(ret.is_type<int>() == true);
        REQUIRE(ret.get_value<int>() == 23);

        for (std::size_t i = 0; i < other_array.get_size(); ++i)
            other_array.set_value(i, static_cast<int>(i * 2));

        // did we really set the value?
        for (std::size_t i = 0; i < other_array.get_size(); ++i)
            REQUIRE(obj._other_array[i] == i * 2);

        bool wasSet = array_prop.set_value(obj, array_obj);
        REQUIRE(wasSet == true);
    }

    SECTION("class property - array")
    {
        property_test obj;
        type prop_type = type::get(obj);
        property array_prop = prop_type.get_property("array");
        REQUIRE(array_prop.is_array() == true);
        variant array_obj = array_prop.get_value(obj); // the array is returned by pointer
        variant_array_view vec_array = array_obj.create_array_view();
        REQUIRE(vec_array.is_valid() == true);
        REQUIRE(array_obj.is_type<std::vector<int>*>() == true);
        REQUIRE(array_obj.get_value<std::vector<int>*>() == &obj._array);
    }

    SECTION("class property - getter member function")
    {
        property_test obj;
        type prop_type = type::get(obj);
        property p7_as_ptr = prop_type.get_property("p7_as_ptr_read_only");
        REQUIRE(bool(p7_as_ptr) == true);
        REQUIRE(p7_as_ptr.is_readonly() == true);
        REQUIRE(p7_as_ptr.get_type() == type::get<const std::string*>());
    }
  
    SECTION("class property - getter/setter member function")
    {
        property_test obj;
        type prop_type = type::get(obj);
        property p7_as_ptr = prop_type.get_property("p7_as_ptr");
        REQUIRE(bool(p7_as_ptr) == true);
        REQUIRE(p7_as_ptr.is_readonly() == false);
        REQUIRE(p7_as_ptr.get_type() == type::get<const std::string*>());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test property inheritance", "[property]") 
{
    type t = type::get<ns_property::bottom>();
    auto props = t.get_properties();
    REQUIRE(props.size() == 7);

    CHECK(props[0].get_name() == "p1"); // top
    CHECK(props[1].get_name() == "p2"); // left
    CHECK(props[2].get_name() == "p3"); // right
    CHECK(props[3].get_name() == "p2"); // right
    CHECK(props[4].get_name() == "p4"); // right2
    CHECK(props[5].get_name() == "p5"); // bottom
    CHECK(props[6].get_name() == "callback"); // bottom

    CHECK(props[5] == props[5]);
    CHECK(props[5] != props[1]);

    ns_property::bottom instance;
    ns_property::top* top = &instance;
    // try access from top instance a property in the most derived class (bottom)
    variant ret = props[0].get_value(top);
    REQUIRE(ret.is_type<int>() == true);
    CHECK(ret.get_value<int>() == 12);
    // try to change the value
    props[0].set_value(top, 2000);
    CHECK(instance._p1 == 2000);
    
    // and now the other way around, from bottom a top property
    ret = props[5].get_value(&instance);
    REQUIRE(ret.is_type<double>() == true);
    CHECK(ret.get_value<double>() == 23.0);
    // try to change the value
    props[5].set_value(top, 42.0);
    CHECK(instance._p5 == 42.0);

    // check double declared property is from left class
    CHECK(props[1].get_declaring_type() == type::get<ns_property::left>());
    // the right class has still its property?
    CHECK(type::get<ns_property::right>().get_property("p2").is_valid() == true);

    property p1 = type::get<ns_property::bottom>().get_property("p1");
    CHECK(bool(p1) == true);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test get/set global property", "[property]")
{
    property pi_prop = type::get_global_property("PI");
    REQUIRE(bool(pi_prop) == true);
    CHECK(pi_prop.set_value(instance(), 3.2) == false);

    property global_text = type::get_global_property("Global_Text");
    REQUIRE(bool(global_text) == true);
    CHECK(global_text.set_value(instance(), std::string("Hello World")) == true);
    CHECK(global_text.get_value(instance()).get_value<std::string>() == "Hello World");

    auto list = type::get_global_properties();
    CHECK(list.size() >= 2);
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test property shortcuts to set/get property", "[property]")
{
    SECTION("test set property with instance")
    {
        property_test obj;
        variant ret = type::get(obj).get_property_value("p2", obj);
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.get_value<short int>() == 12);

        bool success = type::get(obj).set_property_value("p1", obj, 500);
        REQUIRE(success == true);
        REQUIRE(obj._p1 == 500);
    }

    SECTION("test set property with derived instance")
    {
        // derived obj
        ns_property::bottom bottom;
        ns_property::right_2& obj = bottom;

        variant ret = type::get(bottom).get_property_value("p5", obj);
        REQUIRE(ret.is_valid() == true);
        REQUIRE(ret.get_value<double>() == 23.0);

        bool success = type::get(bottom).set_property_value("p5", obj, 500.0);
        REQUIRE(success == true);
        REQUIRE(bottom._p5 == 500.0);

        auto cb = &my_callback;
        success = type::get(bottom).set_property_value("callback", obj, cb);
        REQUIRE(bottom.m_funcPtr == cb);
    }
    
    SECTION("test set global property")
    {
        bool success = type::set_property_value("Global_Text", string("Another Text"));
        REQUIRE(success == true);

        variant ret = type::get_property_value("Global_Text");
        REQUIRE(ret.get_value<std::string>() == string("Another Text"));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test get_base_classes", "[property]")
{
    ns_property::bottom b;
    const auto base_list = type::get(b).get_base_classes();
    REQUIRE(base_list.size() == 4);

    REQUIRE(base_list[0] == type::get<ns_property::top>());
    REQUIRE(base_list[1] == type::get<ns_property::left>());
    REQUIRE(base_list[2] == type::get<ns_property::right>());
    REQUIRE(base_list[3] == type::get<ns_property::right_2>());

    REQUIRE(type::get(b).is_derived_from(type::get<ns_property::top>()) == true); // dynamic
    REQUIRE(type::get(b).is_derived_from<ns_property::top>() == true); // static
}

/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Test property meta data", "[property]") 
{
    property p5 = type::get<property_test>().get_property("p5");
    variant value = p5.get_metadata(E_MetaData::SCRIPTABLE);
    REQUIRE(value.is_type<bool>() == true);
    REQUIRE(value.get_value<bool>() == true);
    
    value = p5.get_metadata(E_MetaData::TOOL_TIP);
    REQUIRE(value.is_type<std::string>() == true);
    REQUIRE(value.get_value<std::string>() == "This is property 5.");

    // no meta data
    property p4 = type::get<property_test>().get_property("p4");
    REQUIRE(p4.is_valid() == true);
    REQUIRE(p4.get_metadata(E_MetaData::SCRIPTABLE).is_valid() == false);

    // not scriptable property
    property p6 = type::get<property_test>().get_property("p6");
    REQUIRE(p6.is_valid() == true);
    REQUIRE(p6.get_metadata(E_MetaData::SCRIPTABLE).is_valid() == true);
    REQUIRE(p6.get_metadata(E_MetaData::SCRIPTABLE).get_value<bool>() == false);


    // string metdadata
    property p8 = type::get<property_test>().get_property("p8");
    REQUIRE(bool(p8) == true);

    value = p8.get_metadata("Global_Tag");
    REQUIRE(value.is_type<bool>() == true);
    REQUIRE(value.get_value<bool>() == true);

}

/////////////////////////////////////////////////////////////////////////////////////////
