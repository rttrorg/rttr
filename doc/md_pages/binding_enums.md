Enums {#binding_enums_page}
=====

RTTR allows also to bind enumerated constants (enums). Therefore use the function @ref rttr::enumeration_() "enumeration_()".

It has following synopsis:

~~~~{.cpp}
  template<typename EnumType>
  void rttr::enumeration_( std::vector< std::pair< std::string, EnumType > > enum_data, std::vector< rttr::metadata > data );
~~~~

- `enum_data` contains a list of *key* to *value* pairs
- `data` contains metadata for this property; this is an optional parameter

~~~~{.cpp}
#include <rttr/register>

enum E_Alignment
{
  AlignLeft       = 0x0001,
  AlignRight      = 0x0002,
  AlignHCenter    = 0x0004,
  AlignJustify    = 0x0008
};

RTTR_REGISTER
{
  enumeration_<E_Alignment>({ {"AlignLeft",    E_Alignment::AlignLeft},
                              {"AlignRight",   E_Alignment::AlignRight},
                              {"AlignHCenter", E_Alignment::AlignHCenter},
                              {"AlignJustify", E_Alignment::AlignJustify}
                            });
}
~~~~

The *key* is a std::string and the *value* is the enum value.
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
