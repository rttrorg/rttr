Metadata {#binding_metadata_page}
========

Adding additional meta information to properties or methods can be very useful.
So for instance, it allows to add ToolTips or the information what kind of editor should be created in the GUI.
You can also tag certain properties to make only those available in a scripting engine which has a certain key set.

The metadata consists of a `key` and a `value`, both objects are forwarded to @ref rttr::variant "variants".
So the only requirement for metadata is that it has to be copyable.

Please take a look at following example:

\code{.cpp}
RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::property("value", value)
    (    
        meta_data(SCRIPTABLE, false), 
        meta_data("Description", "This is a value.")
    );
}
\endcode
In order to add metadata to a registered item you have to use the `()` operator of the returned @ref rttr::registration::bind "bind" object.
Then you call for every metadata item you want to add, the function @ref rttr::meta_data(variant, variant) "meta_data()".

It has following synopsis:
~~~~{.cpp}
  rttr::detail::meta_data rttr::meta_data( variant key, variant value );
~~~~

This will register a global property named `"value"` with two metadata informations.
The first use an enum type as key, the second a string.

And the following snippet shows, how to retrieve this information:
\code{.cpp}
int main()
{
    using namespace rttr;

    property prop = type::get_global_property("value");
    variant value = prop.get_metadata(SCRIPTABLE);
    std::cout << value.get_value<bool>(); // prints "false"
    
    value = prop.get_metadata("Description");
    std::cout << value.get_value<int>(); // prints "This is a value."
}
\endcode

Every \ref property, \ref method, \ref enumeration and \ref constructor can have metadata.

<hr>

<div type="button" class="btn btn-default">[previous](@ref binding_classes_page "Binding Classes")</div><div class="btn btn-default">[next](@ref binding_policies_page "Binding Policies")</div>
