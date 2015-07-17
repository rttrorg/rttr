####################################################################################
#                                                                                  #
#  Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       #
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

set(HEADER_FILES constructor.h
                 destructor.h
                 enumeration.h
                 metadata.h
                 method.h
                 policy.h
                 property.h
                 register
                 register_reflection.h
                 rttr_cast.h
                 rttr_enable.h
                 type
                 type.h
                 variant.h
                 variant_array.h
                 wrapper_mapper.h
                 detail/argument.h
                 detail/argument_impl.h
                 detail/data_address_container.h
                 detail/instance.h
                 detail/instance_impl.h
                 detail/std_conversion_functions.h
                 detail/register_reflection_impl.h
                 detail/rttr_cast_impl.h
                 detail/wrapper_mapper_impl.h
                 detail/array/array_accessor.h
                 detail/array/array_accessor_impl.h
                 detail/array/array_mapper.h
                 detail/base/core_prerequisites.h
                 detail/base/version.h.in
                 detail/base/version.rc.in
                 detail/constructor/constructor_container.h
                 detail/constructor/constructor_container_base.h
                 detail/destructor/destructor_container.h
                 detail/destructor/destructor_container_base.h
                 detail/enumeration/enumeration_container.h
                 detail/enumeration/enumeration_container_base.h
                 detail/metadata/metadata_container.h
                 detail/method/method_accessor.h
                 detail/method/method_container.h
                 detail/method/method_container_base.h
                 detail/misc/argument_wrapper.h
                 detail/misc/function_traits.h
                 detail/misc/misc_type_traits.h
                 detail/misc/number_conversion.h
                 detail/misc/std_type_traits.h
                 detail/misc/utility.h
                 detail/property/property_accessor.h
                 detail/property/property_container.h
                 detail/property/property_container_base.h
                 detail/property/property_container_func.h
                 detail/property/property_container_member_func.h
                 detail/property/property_container_member_object.h
                 detail/property/property_container_object.h
                 detail/type/accessor_type.h
                 detail/type/base_classes.h
                 detail/type/get_create_variant_func.h
                 detail/type/get_derived_info_func.h
                 detail/type/type_converter.h
                 detail/type/type_database_p.h
                 detail/type/type_register.h
                 detail/type/type_impl.h
                 detail/variant/variant_data.h
                 detail/variant/variant_data_converter.h
                 detail/variant/variant_data_policy.h
                 detail/variant/variant_data_policy_arithmetic.h
                 detail/variant/variant_data_policy_empty.h
                 detail/variant/variant_data_policy_void.h
                 detail/variant/variant_data_policy_string.h
                 detail/variant/variant_impl.h
                 detail/variant_array/variant_array_impl.h
                 detail/variant_array/variant_array_creator.h
                 detail/variant_array/variant_array_creator_impl.h
                 detail/variant_array/variant_array_data.h
                 detail/variant_array/variant_array_policy.h
                 detail/variant_array/variant_array_policy_empty.h
                )
                
set(SOURCE_FILES constructor.cpp
                 destructor.cpp
                 enumeration.cpp
                 method.cpp
                 policy.cpp
                 property.cpp
                 type.cpp
                 variant.cpp
                 variant_array.cpp
                 detail/argument.cpp
                 detail/standard_types.cpp
                 detail/std_conversion_functions.cpp
                 detail/constructor/constructor_container_base.cpp
                 detail/destructor/destructor_container_base.cpp
                 detail/enumeration/enumeration_container_base.cpp
                 detail/metadata/metadata_container.cpp
                 detail/method/method_container_base.cpp
                 detail/property/property_container_base.cpp
                 detail/type/type_database.cpp
                 detail/type/type_register.cpp
                 )
