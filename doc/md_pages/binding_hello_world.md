Binding Types - Hello World  {#binding_hello_world_page}
===========================
Let's start with the traditional hello world example.

~~~~{.cpp}
#include <rttr/reflect>

static void f() { std::cout << "Hello World" << std::endl; }
using namespace rttr;

RTTR_REGISTER
{
    method_("f", &f);
}
  
int main()
{
    type::invoke("f", {});
}
// outputs: "Hello World"
~~~~

When you need to reflect a property, or like in this case a free function, you need to include first `#include <rttr/reflect>`. 
This will include everything necessary for creating the reflection information. 
The macro @ref RTTR_REGISTER must be placed outside of any function or class, just place directly into in your cpp file.
This macro executes the register process before *main* is called, 
that has the advantage that this reflection information is directly available when main is called.
Remark that this macro can only be placed one-time in a source file, otherwise you will get an compile error.

The shortest way to invoke the function `f()` is to call @ref rttr::type::invoke(const std::string&, std::vector< detail::argument >) "type::invoke()".
It requires the exact name of the free function and a vector of arguments. 
You can check whether the call was successful with checking the return value. 
When the returned @ref rttr::variant "variant" is @ref rttr::variant::is_valid() "valid" the call was successful, otherwise not.

<hr>

<div class="btn btn-default" disabled="true">previous</div><div class="btn btn-default">[next](@ref binding_methods_page "Binding Methods")</div>
