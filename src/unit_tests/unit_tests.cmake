####################################################################################
#                                                                                  #
#  Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     #
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

set(HEADER_FILES test_classes.h
                 enumeration/test_enumeration_reflection.h
                 method/test_method_reflection.h
                 property/test_property_reflection.h
                 type/test_type.h
                 version.rc.in
                 )
                
set(SOURCE_FILES main.cpp
                 misc/test_misc.cpp
                 property/test_property_reflection.cpp
                 property/property_access_level_test.cpp
                 type/test_type.cpp
                 constructor/constructor_access_level_test.cpp
                 constructor/constructor_default_arg_test.cpp
                 constructor/constructor_param_info_test.cpp
                 constructor/constructor_retrieve_test.cpp
                 constructor/constructor_invoke_test.cpp
                 constructor/constructor_misc_test.cpp
                 enumeration/test_enumeration_reflection.cpp
                 method/method_access_level_test.cpp
                 method/test_method_reflection.cpp
                 method/method_default_arg_test.cpp
                 method/method_param_info_test.cpp
                 variant/variant_assign_test.cpp
                 variant/variant_conv_test.cpp
                 variant/variant_conv_to_bool_test.cpp
                 variant/variant_ctor_test.cpp
                 variant/variant_cmp_equal_test.cpp
                 variant/variant_cmp_less_test.cpp
                 variant/variant_enum_test.cpp
                 variant/variant_misc_test.cpp
                 variant_array_view/variant_array_view_test.cpp
                 )
