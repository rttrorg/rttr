Variant {#binding_variant_page}
=======

The variant class acts as return value container for properties and methods. This class allows to store data of any type and convert between these types transparently.
It can hold one value at a time (using containers you can hold multiple types e.g. `std::vector<int>`). 
Remark that the content is copied into the variant class. Even raw arrays (e.g. `int[10]`) are copied.
When you would like to avoid copies, use pointer types or wrap your type in a `std::reference_wrapper<T>`
A typical usage is the following example:

~~~~{.cpp}
    variant var;
    var = 23;                                   // copy integer
    int x = var.to_int();                       // x = 23
    
    var = std:.string("Hello World");           // variant contains now a std::string
    var = "Hello World";                        // variant contains now a char[12] array
    int y = var.to_int();                       // y = 0, because invalid conversion
    std::string text = var.to_string();         // text = "Hello World", char array to string converted
    
    var = "42";                                 // contains now char[3] array
    std::cout << var.to_int();                  // convert char array to integer and prints "42"
    
    int my_array[100];    
    var = my_array;                             // copies the content of my_array into var
    auto& arr = var.get_value<int[100]>();      // extracts the content of var by reference
    
    bool ret = var.can_convert<variant_array>();// return true
    variant_array array = var.to_array();       // converts to variant_array, a helper class to get access to array values and other meta infos about array
    std::size_t size = array.get_size();        // size = 100
    array.set_value(0, 42);                     // set the first value to 42
    std::cout << array.get_value(0);            // prints 42
~~~~

<hr>

<div type="button" class="btn btn-default">[previous](@ref binding_enums_page "Binding Enums")</div><div class="btn btn-default">[next](@ref binding_classes_page "Binding Classes")</div>
