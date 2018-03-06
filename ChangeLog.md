# Change Log
All notable changes to this project will be documented in this file. This project adheres to [Semantic Versioning](http://semver.org/).

## [v0.9.6](https://github.com/rttrorg/rttr/releases/tag/v0.9.6) (2018-03-15)
[Full Changelog](https://github.com/rttrorg/rttr/compare/v0.9.5...v0.9.6)
- [FEATURE] Added `variant_associative_view` class #57
- [FEATURE] Added `variant_sequential_view` #46
- [FEATURE] Added `string_view` class
- [FEATURE] Added `library` class in order to load plugins at runtime #116
- [FEATURE] Added `variant_cast` function in order to support move data from `variant` #108
- [FEATURE] Added possibility to convert raw nullptr pointers in variant class to base types #59
- [FEATURE] Return the template parameters of a type #58
- [FEATURE] Added support for register base class properties #29
- [FEATURE] Add implicit conversion from a wrapped value inside a variant #48
- [FEATURE] Support for VS2017 #45
- [FEATURE] Added support for C++17 compilers #100, #105
- [FEATURE] Custom registered type name should be used in class templates as well #40
- [FEATURE] Added new comparators(>, <=, >=) for `variant` class #61
- [FEATURE] Add support for new property policy: 'as_reference_wrapper' #20
- [FEATURE] Add possibility to filter to class items, while retrieving, 
            e.g: retrieve private properties: `get_properties(filter_item::instance_item | filter_item::non_public_access)`
- [FEATURE] Added JSON serialization example
- [FEATURE] Added loading plugins example #116
- [FEATURE] Added CI systems: Travis and Appveyor
- [FEATURE] Added static code analysis tool Codacy (cppcheck underlying)
- [FEATURE] Added code coverage tool: "CoverAlls"
- [FEATURE] Added `variant::get_wrapped_value()` & `variant::extract_wrapped_value()` functions

- [IMPROVEMENT] Return ranges instead of vectors #6
- [IMPROVEMENT] Remove branching when calling a wrapper method #19
- [IMPROVEMENT] Add 'noexcept' everywhere where reasonable #17
- [IMPROVEMENT] Add 'constexpr' everywhere where reasonable #16
- [IMPROVEMENT] Added own make_unique impl, not supported in C++11
- [IMPROVEMENT] Upgraded catch to version: 1.12.0 #120
- [IMPROVEMENT] Increased test coverage: 92%
- [IMPROVEMENT] Several docu improvements

- [BUGFIX] class items inheritance of items, was not working when derived class had no items at all #89
- [BUGFIX] Raw arrays by value, were not recognized by `variant_sequential_view` #125
- [BUGFIX] Explicit unloading of libraries which contain registered RTTR types, lead to crash #113
- [BUGFIX] Fix install handling for Mac/Linux #101, #102
- [BUGFIX] Added support for variant properties #47
- [BUGFIX] Typo in method name: rttr::constructor::get_instanciated_type() #83
- [BUGFIX] Polymorphic assignment of raw pointer properties does not work  #56
- [BUGFIX] Comparison of raw pointer properties does not work #55
- [BUGFIX] fix wrong enum conversion in variant function `convert(bool* ok)` #53
- [BUGFIX] Fixed not correct handling of std::ref(int*) type in variant #37
- [BUGFIX] Custom installation directory via CMAKE_INSTALL_PREFIX #34
- [BUGFIX] Replaced default constructor binding from `as_object` to `as_std_shared_ptr` #14
- [BUGFIX] Fixed missing invoke of dtor when type is pointer type #14
- [BUGFIX] Do not search for boost, when not needed #93

- [REMOVED]: `variant_array_view` class

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
