# Change Log
All notable changes to this project will be documented in this file. This project adheres to [Semantic Versioning](http://semver.org/).

## [v0.9.6](https://github.com/rttrorg/rttr/releases/tag/v0.9.6) (2017-07-04)
[Full Changelog](https://github.com/rttrorg/rttr/compare/init_commit...v0.9.6)
- [FEATURE] Added `variant_associative_view` class #57
- [FEATURE] Added `variant_sequential_view` #46
- [FEATURE] Added `string_view` class
- [FEATURE] Added possibility to convert raw nullptr pointers in variant class to base types #59
- [FEATURE] Return the template parameters of a type #58
- [FEATURE] Added support for register base class property #29
- [FEATURE] Add implicit conversion from a wrapped value inside a variant #48
- [FEATURE] Support for VS2017 #45
- [FEATURE] Custom registered type name should be used in class templates as well #40
- [FEATURE] Add support for new property policy: 'as_reference_wrapper' #20
- [FEATURE] Add posibility to filter to class items, while retrieving, 
            e.g: retrieve private properties: `get_properties(filter_item::instance_item | filter_item::non_public_access)`
- [FEATURE] Added JSON serialization example
- [FEATURE] Added CI systems: Travis and Appveyor
- [FEATURE] Added code coverage tool: "CoverAlls"
- [FEATURE] Added `variant::get_wrapped_value()` & `variant::extract_wrapped_value()` functions

- [IMPROVEMENT] Return ranges instead of vectors #6
- [IMPROVEMENT] Remove branching when calling a wrapper method #19
- [IMPROVEMENT] Add 'noexcept' eveywhere where reasonable #17
- [IMPROVEMENT] Add 'constexpr' eveywhere where reasonable #16
- [IMPROVEMENT] Added own make_unique impl, not supported in C++11
- [IMPROVEMENT] Updated to catch: 1.8.2
- [IMPROVEMENT] Increased test coverage: 91%
- [IMPROVEMENT] Several docu improvments

- [BUGFIX] Added support for variant properties #47
- [BUGFIX] Polymorphic assignment of raw pointer properties does not work  #56
- [BUGFIX] Comparison of raw pointer properties does not work #55
- [BUGFIX] fix wrong enum conversion in variant function `convert(bool* ok)` #53
- [BUGFIX] Fixed not correct handling of std::ref(int*) type in variant #37
- [BUGFIX] Custom installation directory via CMAKE_INSTALL_PREFIX #34
- [BUGFIX] Replaced default constructor binding from `as_object` to `as_std_shared_ptr` #14
- [BUGFIX] Fixed missing invoke of dtor when type is pointer type #14

- [DEPRECATED]: `variant_array_view` class

## [v0.9.5](https://github.com/rttrorg/rttr/releases/tag/v0.9.5) (2015-12-23)
[Full Changelog](https://github.com/rttrorg/rttr/compare/init_commit...v0.9.5)

- the macro RTTR_DECLARE_TYPE was removed, so it is not necessary anymore to declare 
  a type before using it (RTTR does still not require a compiler with rtti)
- completely adjusted the registration API to be more future proof
- added the possibility to register the access specifier of your class member
- added the possibility to register free functions for constructors
- the `variant` class has get comparison operators `==` and `<`
- the `variannt_array` class is replaced by `variant_array_view` class
- added the possibility to register default values to methods and constructors
- added the possibilities to register names of method parameters
- new policies for constructors were added
- it's now possible to invoke methods while the instance is in a wrapped type, like std::shared_ptr<T>
- a lot of refactoring internally, almost every class was internally refactored
- added several benchmarks to measure the performance impact from certain 
  API calls of the library
- adjusted documentation style to use cleaner and modern HTML5 style
- rewritten the tutorial
- switched from mercurial to git and uploaded the source code to github
- made own website: www.rttr.org