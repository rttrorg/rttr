Enums {#binding_enums_page}
=====

RTTR allows also to bind enumerated constants (enums). Therefore use the function @ref rttr::registration::enumeration() "registration::enumeration()".

It has following synopsis:

~~~~{.cpp}
  template<typename Enum_Type>
  registration rttr::registration::enumeration( const char* name );
~~~~

- `name` the declared name of this enum

~~~~{.cpp}
#include <rttr/register>
using namespace rttr;

enum E_Alignment
{
  AlignLeft       = 0x0001,
  AlignRight      = 0x0002,
  AlignHCenter    = 0x0004,
  AlignJustify    = 0x0008
};

RTTR_REGISTER
{
  registration::enumeration<E_Alignment>("E_Alignment")
  (
      value("AlignLeft",    enum_test::E_Alignment::AlignLeft),
      value("AlignRight",   enum_test::E_Alignment::AlignRight),
      value("AlignHCenter", enum_test::E_Alignment::AlignHCenter),
      value("AlignJustify", enum_test::E_Alignment::AlignJustify)
  )
}
~~~~

The *name* is a `const char*` and the *value* is the enum value.
The class @ref rttr::enumeration "enumeration" contains several meta information about an enum with conversion 
functions between the value representation and its literal representation.

How to use the enumeration class shows following example:
~~~~{.cpp}
  type enum_type = type::get("E_Alignment");
  if (enum_type && enum_type.is_enumeration())
  {
    enumeration enum_align = enum_type.get_enumeration();
    std::string key = enum_align.value_to_key(MyStruct::AlignHCenter);
    std::cout << key; // prints "AlignHCenter"
    
    variant var = enum_align.key_to_value(key);
  }
  std::cout << var.get_value<MyStruct::E_Alignment>(); // prints "4";
~~~~

<hr>

<div type="button" class="btn btn-default">[previous](@ref binding_properties_page "Binding Properties")</div><div class="btn btn-default">[next](@ref binding_variant_page "Binding Variant")</div>
