####################################################################################
#                                                                                  #
#  Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             #
#                                                                                  #
#  This file is part of RTTR (Run Time Type Reflection)                            #
#  License: MIT License                                                            #
#                                                                                  #
#  Permission is hereby granted, free of charge, to any person obtaining           #
#  a copy of this software and associated documentation files (the "Software"),    #
#  to deal in the Software without restriction, including without limitation       #
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,        #
#  and/or sell copies of the Software, and to permit persons to whom the           #
#  Software is furnished to do so, subject to the following conditions:            #
#                                                                                  #
#  The above copyright notice and this permission notice shall be included in      #
#  all copies or substantial portions of the Software.                             #
#                                                                                  #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#  SOFTWARE.                                                                       #
#                                                                                  #
####################################################################################

set(SOURCE_FILES constructor.cpp
                 destructor.cpp
                 enumeration.cpp
                 method.cpp
                 property.cpp
                 policy.cpp
                 type.cpp
                 variant.cpp
                 variant_array.cpp
                 detail/constructor_container_base.cpp
                 detail/destructor_container_base.cpp
                 detail/enumeration_container_base.cpp
                 detail/method_container_base.cpp
                 detail/property_container_base.cpp
                 detail/metadata_container.cpp
                 detail/standard_types.cpp
                 detail/reflection_database.cpp
                 detail/standard_types_char.cpp
                 detail/std_conversion_functions.cpp
                 )

set(HEADER_FILES constructor.h
                 destructor.h
                 enumeration.h
                 metadata.h
                 method.h
                 property.h
                 rttr_cast.h
                 rttr_enable.h
                 type.h
                 variant.h
                 variant_array.h
                 register_reflection.h
                 policy.h
                 type
                 reflect
                 base/core_prerequisites.h
                 base/version.h.in
                 impl/register_reflection_impl.h
                 impl/rttr_cast_impl.h
                 impl/type_impl.h
                 impl/variant_impl.h
                 impl/variant_default_types_impl.h
                 impl/variant_array_impl.h
                 detail/argument.h
                 detail/array_mapper.h
                 detail/array_container.h
                 detail/array_container_base.h
                 detail/array_accessor.h
                 detail/accessor_type.h
                 detail/instance.h
                 detail/constructor_container.h
                 detail/constructor_container_base.h
                 detail/destructor_container.h
                 detail/destructor_container_base.h
                 detail/method_accessor.h
                 detail/method_container_base.h
                 detail/method_container.h
                 detail/metadata_container.h
                 detail/standard_types.h
                 detail/function_traits.h
                 detail/enumeration_container_base.h
                 detail/enumeration_container.h
                 detail/property_accessor.h
                 detail/property_container_base.h
                 detail/property_container.h
                 detail/property_container_member_func.h
                 detail/property_container_func.h
                 detail/property_container_member_object.h
                 detail/property_container_object.h
                 detail/misc_type_traits.h
                 detail/utility.h
                 detail/reflection_database_p.h
                 detail/std_conversion_functions.h
                 detail/type_converter.h
                )