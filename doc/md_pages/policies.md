Policies {#binding_policies_page}
========

Sometimes it is necessary to adjust the default binding behaviour RTTR. Therefore policies were introduced.
At the moment only return value policies are implemented.
The default binding behaviour of RTTR is to return all values by copying the content in a variant.

Currently implemented policies:
- @ref rttr::policy::prop::bind_as_ptr "policy::prop::bind_as_ptr"
- @ref rttr::policy::meth::return_ref_as_ptr "policy::meth::return_ref_as_ptr"
- @ref rttr::policy::meth::discard_return "policy::meth::discard_return"

For easier usage, the policies are grouped to its corresponding items; e.g. all policies for methods
can be found under `policy::meth`; all policies for properties under `policy::prop` etc.

bind_as_ptr
-----------
The motivation for this policy is to avoid expensive copies when returning a property.
When you have primitive data types like integer or doubles you are good to go with the standard binding behaviour.
But when you have big arrays, it would be a waste to always copy the content when retrieving or setting the value, therefore this policy was introduced.

Example:
~~~~{.cpp}
struct Foo
{
  std::vector<int> vec;
};

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Foo>()
        .property("vec", &Foo::vec)
        (
            policy::prop::bind_as_ptr
        );
}

int main()
{
    Foo obj;
    property vec_prop = type::get<Foo>().get_property("vec");
    variant vec_value = prop.get_value(obj);
    std::cout << value.is_type<std::vector<int>*>(); // prints "true"
    
    // not really necessary, but remark that now a std::vector<int>* is expected
    prop.set_value(obj, vec_value);
}
~~~~

return_ref_as_ptr
-----------------
The motivation for this policy is the same like the @ref rttr::bind_property_as_ptr "bind_property_as_ptr" policy.
When you really need to get a reference to the return value of a method call you have to use this policy,
otherwise the returned reference will be copied into the variant.

Example:

~~~~{.cpp}
struct Foo
{
    std::string& get_text() { static text; return text; }
};

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Foo>()
        .method("get_text", &Foo::get_text)
        (
            policy::meth::return_ref_as_ptr
        );
}

int main()
{
    Foo obj;
    method text_meth  = type::get<Foo>().get_method("get_text");
    variant vec_value = text_meth.invoke(obj);
    std::cout << value.is_type<std::string*>(); // prints "true"
}
~~~~

discard_return
--------------
Sometimes it is necessary that the client caller should ignore the return value of a method call.
Therefore this policies was introduced.

Example:
~~~~{.cpp}
struct Foo
{
    int calculate_func() { return 42; }
};

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Foo>()
        .method("get_value", &Foo::calculate_func)
        (
            policy::meth::discard_return
        );
}

int main()
{
    Foo obj;
    method text_meth  = type::get<Foo>().get_method("calculate_func");
    variant vec_value = text_meth.invoke(obj);
    std::cout << value.is_type<void>(); // prints "true"
}
~~~~

A closing hint: you can of course build your own policies in that way, that you build wrappers around your properties or methods and 
then bind the wrapper instead of the original accessor.

<hr>

<div type="button" class="btn btn-default">[previous](@ref binding_metadata_page "Binding Metadata")</div><div class="btn btn-default">[Finished](@ref tutorial_page "Tutorial")</div>
