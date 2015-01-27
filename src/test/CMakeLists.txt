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

project(unit-tests)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

set(SOURCE_FILES main.cpp
                 test_type.cpp
                 test_variant.cpp
                 test_constructor_reflection.cpp
                 test_method_reflection.cpp
                 test_property_reflection.cpp
                 test_enumeration_reflection.cpp
    )

set(HEADER_FILES test_classes.h
                 test_type.h
                 test_constructor_reflection.h
                 test_method_reflection.h
                 test_property_reflection.h
                 test_enumeration_reflection.h
                 catch.hpp)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SOURCE_FILES)
endif()

add_executable(unit-tests ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(unit-tests rttr)
add_dependencies(unit-tests rttr)

get_target_property(UnitTest_Exec unit-tests LOCATION)
add_custom_target(run-tests ALL
                  COMMAND "${UnitTest_Exec}"
                  DEPENDS unit-tests
                  COMMENT "Running unit-tests")


message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")