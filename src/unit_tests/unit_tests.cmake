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

set(HEADER_FILES test_classes.h
                 constructor/test_constructor_reflection.h
                 enumeration/test_enumeration_reflection.h
                 method/test_method_reflection.h
                 property/test_property_reflection.h
                 type/test_type.h
                 version.rc.in
                 )
                
set(SOURCE_FILES main.cpp
                 misc/test_misc.cpp
                 property/test_property_reflection.cpp
                 type/test_type.cpp
                 constructor/test_constructor_reflection.cpp
                 enumeration/test_enumeration_reflection.cpp
                 method/test_method_reflection.cpp
                 variant/variant_array_test.cpp
                 variant/variant_assign_test.cpp
                 variant/variant_conv_test.cpp
                 variant/variant_ctor_test.cpp
                 variant/variant_misc_test.cpp
                 )
