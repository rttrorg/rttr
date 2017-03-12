# Change Log
All notable changes to this project will be documented in this file. This project adheres to [Semantic Versioning](http://semver.org/).

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