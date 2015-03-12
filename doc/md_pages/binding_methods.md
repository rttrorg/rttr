Binding Methods {#binding_methods_page}
===============

As already mentioned to bind global methods to RTTR use the function \ref rttr::method_().
It has following synopsis:
~~~~{.cpp}
    template<typename F>
    void rttr::method_( const std::string & name, F function, std::vector< rttr::metadata > data);
~~~~
- `name` is the name of the function
- `F` is the function pointer or std::function your want to register
- `data` contains metadata for this function; this is an optional parameter

For example when you want to register the byte string to integer conversion function: `int atoi (const char * str);`

Do it in the following way:
~~~~{.cpp}
RTTR_REGISTER
{
    method_("atoi", &atoi);
}
~~~~

Overloaded Methods
------------------
When you want to register a function which is overloaded (same name, different signature),
you should use the helper function: @ref rttr::select_overload<Signature>(Signature*) "select_overload<T>(T*)"

This will avoid the hassle with the function pointer signature.
For example the two functions: `float sin (float x);` and `double sin (double x);` can be registered like following:

~~~~{.cpp}
#include <rttr/register>
#include <cmath.h>

RTTR_REGISTER
{
    method_("sin", select_overload<float(float)>(&sin));
    method_("sin", select_overload<double(double)>(&sin));
}
~~~~

Invoke of methods
-----------------
Invoking a method with RTTR can be done in two ways:
- calling @ref rttr::type::invoke(const std::string&, std::vector< detail::argument >) "type::invoke()" from the [type](@ref rttr::type) class.
- retrieving first a @ref rttr::method "method" object from @ref rttr::type::get_global_method(const std::string &, const std::vector< type >&) "type::get_global_method()" and then calling invoke

The first option needs less typing, but it is slower when you need to invoke the function several times.
For the second option you need more code to write, but it invokes the method faster.

The following example demonstrates the possibilities to invoke a method:
~~~~{.cpp}
int main()
{
    // let asume we have registered the pow function: double pow( double base, double exp );
    // option 1
    variant return_value = type::invoke("pow", {9.0, 2.0});
    if (return_value.is_valid() && return_value.is_type<double>())
        std::cout << return_value.get_value<double>() << std::endl; // 81
    
    // option 2
    method meth = type::get_global_method("pow");
    if (meth) // check if the function was found
    {
        return_value = meth.invoke(empty_instance(), 9.0, 3.0);
        if (return_value.is_valid() && return_value.is_type<double>())
            std::cout << return_value.get_value<double>() << std::endl; // 729
        
    }
}
~~~~

The @ref rttr::type::invoke(const std::string&, std::vector< detail::argument >) "type::invoke()" function will internally try 
to find a function based on the given name and the given types of the arguments. 
So finding the correct function when overloaded function are registered is automatically done.
Notice that you have to provide the arguments as a vector pack. Therefore an initializer list is quite handy to reduce typing.
The argument must match 100%, there is at the moment no conversion done. That means, when an integer argument is needed and you forward a double
value the function will **not** be called. The arguments will not be copied, instead they will be wrapped in an internal class and forwarded to the 
underlying function pointer. This class is called `detail::argument` do **not** create this class on your own. The class will implicit wrap your argument value.

The return value of @ref rttr::type::invoke(const std::string&, std::vector< detail::argument >) "type::invoke()" is a @ref rttr::variant "variant" object.
It indicates whether the function was called or not. RTTR does **not** use the exception mechanism of C++, therefore you have to check the return values when you are interested
in a successful call. The @ref rttr::variant object @ref rttr::variant::is_valid "is valid" when it was successfully called. 
When the function has a return value, then this value is also contained in the variant object.

When you prefer to hold a handle to the function use the getter @ref rttr::type::get_global_method(const std::string &, const std::vector< type >&) "type::get_global_method()"
to retrieve a @ref rttr::method "method" object. This has the advantage that you do not need to search for the function every time you want to invoke it.
Additionally this class provides @ref rttr::method::invoke "functions" to invoke a function without the need to create a vector of arguments.
The method object is lightweight and can be simply copied around different locations. The object stays valid till end of the `main()` function.

<hr>

<div class="btn btn-default">[previous](@ref binding_hello_world_page "Binding Hello World")</div><div class="btn btn-default">[next](@ref binding_properties_page "Binding Properties")</div>
