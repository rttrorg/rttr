JSON Serialization
==================

This example demonstrate the usage of RTTR in order to serialize and deserialize an object with JSON.

```cpp

RTTR_REGISTRATION
{
    rttr::registration::class_<shape>("shape")
        .property("visible", &shape::get_visible, &shape::set_visible)
        .property("color", &shape::color_)
        .property("name", &shape::name)
        .property("position", &shape::position);

    rttr::registration::class_<circle>("circle")
        .property("radius", &circle::radius)
        .property("points", &circle::points)
        .property("no_serialize", &circle::no_serialize)
        (
            metadata("NO_SERIALIZE", true)
        )
        ;

    rttr::registration::class_<point2d>("point2d")
        .property("x", &point2d::x)
        .property("y", &point2d::y)
        ;


    rttr::registration::enumeration<color>("color")
        (
            value("red", color::red),
            value("blue", color::blue),
            value("green", color::green)
        );
}
```
