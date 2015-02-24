Metadata {#binding_metadata_page}
========

Adding additional meta information to properties or methods can be very useful.
So for instance, it allows to add ToolTips or the information what kind of editor should be created in the GUI.
You can also tag certain properties to make only those available in a scripting engine which has a certain key set.

The metadata consists of a `key`, which can be a std::string or an integer and a `value` which is a \ref variant.

Please take a look at following example:

\code{.cpp}
RTTR_REGISTER
{
    property_("value", value, { metadata(SCRIPTABLE, false), 
                                metadata("Description", "This is a value.") });
}
\endcode
This will register a global property named `"value"` with two metadata informations.
Both keys use the integer as data role, because the enum value will be implicit converted to an integer.

And the following snippet shows, how to retrieve this information:
\code{.cpp}
int main()
{
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
