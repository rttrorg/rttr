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

project(bench-variant)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
                    
set(SOURCE_FILES main.cpp
                 bench_variant_conversion.cpp
                 bench_variant_create.cpp)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SOURCE_FILES)
endif()

add_executable(bench-variant ${SOURCE_FILES} ${HEADER_FILES})
if (MSVC)
  target_link_libraries(bench-variant rttr ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY} )
else()
  target_link_libraries(bench-variant rttr ${CMAKE_THREAD_LIBS_INIT})
endif()

target_include_directories(bench-variant PUBLIC ${RTTR_3RD_PARTY_DIR} ${Boost_INCLUDE_DIR})
target_compile_definitions(bench-variant PUBLIC BOOST_ALL_NO_LIB)
if(MSVC)
    set_target_properties(bench-variant PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
endif()

set_target_properties(bench-variant PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(bench-variant rttr)

install(TARGETS bench-variant
        RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR} 
        )
        
message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")