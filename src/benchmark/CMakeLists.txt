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

project(benchmark)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
                    
add_definitions(-DRTTR_DLL)

set(SOURCE_FILES main.cpp
                 benchmark_rttr_cast.cpp
    )

set(HEADER_FILES test_classes.h
                 benchmark_utils.h
    )

if (USE_PCH)
  activate_precompiled_headers("pch.h" SOURCE_FILES)
endif()
  
add_executable(benchmark ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(benchmark rttr)
add_dependencies(benchmark rttr)
get_target_property(UnitTest_Exec unit_tests LOCATION)

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")