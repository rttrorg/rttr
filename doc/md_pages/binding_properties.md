Properties {#binding_properties_page}
==========

For binding a property to RTTR you can use following functions: 
@ref rttr::property_() "property_()" and @ref rttr::property_readonly_() "property_readonly_()".

They have following synopsis:
~~~~{.cpp}
  template<typename A>
  void rttr::property_( const std::string & name, A accessor, std::vector< rttr::metadata > data );
  
  template<typename A>
  void rttr::property_readonly_( const std::string & name, A accessor, std::vector< rttr::metadata > data );
~~~~
- `name` is the name of the property
- `A` is the pointer to the property
- `data` contains metadata for this property; this is an optional parameter

It is also possible to use function pointers for the property as getter and setter functions.
Therefore the @ref rttr::property_( const std::string &, A1, A2 , std::vector< rttr::metadata >) "property_()" function is overloaded.

It has following synopsis:
~~~~{.cpp}
  template<typename A1, typename A2>
  void rttr::property_( const std::string & name, A1 getter, A2 setter, std::vector< rttr::metadata > data );
~~~~
- `name` is the name of the property
- `A1` is the function pointer to the getter and A2 is the function pointer to the setter of the property
- `data` contains metadata for this property; this is an optional parameter

The following example shows how to use these register functions:
~~~~{.cpp}
#include <rttr/reflect>

static const double pi = 3.14259;
static std::string global_text;
void set_text(const std::string& text) { global_text = text; }
const std::string& get_text() { return global_text; }

RTTR_REGISTER
{
  property_readonly_("PI", &pi);
  property_("global_text", &get_text, &set_text);
}
~~~~

There can be not two global properties with the same name. The later registered property with an already existing name will be discarded.

Invoke properties
-----------------
For setting and getting a property you have two options like with methods:
- calling @ref rttr::type::set_property_value(const std::string&, detail::argument) "type::set_property_value()" and @ref rttr::type::get_property_value(const std::string&) "type::get_property_value()" from the @ref rttr::type "type" class or
- retrieving a @ref rttr::property "property" object from @ref rttr::type::get_global_property(const std::string &) "type::get_global_property()" and then calling @ref rttr::property::set_value() "property::set_value()" and @ref rttr::property::get_value() "property::get_value()"

~~~~{.cpp}
int main()
{
  // option 1, via type
   variant value = type::get_property_value("pi");
   if (value && value.is_type<double>())
      std::cout << value.get_value<double>() << std::endl; // outputs: "3.14259"
      
  // option 2, via property class
   property prop = type::get_property_value("pi");
   if (prop)
   {
      value = prop.get_value();
      if (value.is_valid() && value.is_type<double>())
        std::cout << value.get_value<double>() << std::endl; // outputs: "3.14259"
   }
}
~~~~

The static @ref rttr::type::set_property_value(const std::string&, detail::argument) "type::set_property_value()" function
calls directly a global property with the given name. This function has a bool value as return value, indicating whether the 
property was invoked or not. For retrieving a property value use the static function 
@ref rttr::type::get_property_value(const std::string&) "type::get_property_value()". 
The returned @ref rttr::variant "variant" object contains the property value and also indicates whether the 
call to retrieve the property was successful or not. When the variant is @ref rttr::variant::is_valid() "not valid" 
then the call could not be done.

Another option is to retrieve a handle to the property via @ref rttr::type::get_global_property(const std::string &) "type::get_global_property()".
This is the preferred option, because then you directly set/get the value without searching every time for the property.
The property object is very lightweight and can be simply copied around different locations. The object stays valid till end of the `main()` function.

<hr>

<div type="button" class="btn btn-default">[previous](@ref binding_methods_page "Binding Methods")</div><div class="btn btn-default">[next](@ref binding_enums_page "Binding Enums")</div>
