####################################################################################
#                                                                                  #
#  Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           #
#                                                                                  #
#  This file is part of the examples of RTTR (Run Time Type Reflection)            #
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

project(unit_test_base LANGUAGES CXX)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

set(HPP_FILES "")
set(SRC_FILES "")

generateLibraryVersionVariables(${RTTR_VERSION_MAJOR} ${RTTR_VERSION_MINOR} ${RTTR_VERSION_PATCH}
                                "RTTR Unit-Tests: unit_test_base" "Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>" "MIT License")

loadFolder("files" HPP_FILES SRC_FILES)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_library(unit_test_base SHARED ${SRC_FILES} ${HPP_FILES})

target_compile_definitions(unit_test_base PRIVATE RTTR_DLL_EXPORTS)
target_compile_definitions(unit_test_base PUBLIC RTTR_DLL)

target_link_libraries(unit_test_base RTTR::Core)
set_target_properties(unit_test_base PROPERTIES 
                                     DEBUG_POSTFIX ${RTTR_DEBUG_POSTFIX}
                                     FOLDER "Testing")

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")