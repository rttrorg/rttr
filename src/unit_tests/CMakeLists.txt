####################################################################################
#                                                                                  #
#  Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           #
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

add_subdirectory(base_library)

set(HPP_FILES "")
set(SRC_FILES "")

cmake_policy(SET CMP0025 NEW)
project(unit_tests LANGUAGES CXX)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

generateLibraryVersionVariables(${RTTR_VERSION_MAJOR} ${RTTR_VERSION_MINOR} ${RTTR_VERSION_PATCH}
                                "RTTR unit_tests" "Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>" "MIT License")

loadFolder("unit_tests" HPP_FILES SRC_FILES)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_executable(unit_tests ${SRC_FILES} ${HPP_FILES})
target_link_libraries(unit_tests RTTR::Core unit_test_base Catch2::Catch)
add_dependencies(unit_tests RTTR::Core)
set_target_properties(unit_tests PROPERTIES DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                                            FOLDER "Testing"
                                            INSTALL_RPATH "${RTTR_EXECUTABLE_INSTALL_RPATH}"
                                            CXX_STANDARD ${MAX_CXX_STANDARD})

set_compiler_warnings(unit_tests)

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message("XCODE_VERSION: ${XCODE_VERSION}")

if ((CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0.1.10010046")
    OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6.0.1")
    OR (XCODE AND XCODE_VERSION VERSION_GREATER "10.1"))
        target_compile_options(unit_tests PRIVATE -Wno-error=self-assign-overloaded)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9.1.0")
    target_compile_options(unit_tests PRIVATE -Wno-error=init-list-lifetime)
endif()

if (MSVC)
    target_compile_options(unit_tests PRIVATE /bigobj) 
endif()
                                            
# run tests
add_custom_target(run_tests ALL
                  COMMAND "$<TARGET_FILE:unit_tests>"
                  DEPENDS unit_tests
                  COMMENT "Running unit_tests")
                  
set_target_properties(run_tests PROPERTIES 
                                FOLDER "Testing")

add_subdirectory(plugin)

add_dependencies(unit_tests unit_test_plugin)

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")
