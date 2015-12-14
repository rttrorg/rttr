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

project(unit_tests)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

generateLibraryVersionVariables(${RTTR_VERSION_MAJOR} ${RTTR_VERSION_MINOR} ${RTTR_VERSION_PATCH} 
                                "RTTR unit_tests" "Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>" "MIT License")
                                
loadFolder("unit_tests" HPP_FILES SRC_FILES)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_executable(unit_tests ${SRC_FILES} ${HPP_FILES})
target_link_libraries(unit_tests rttr_core)
add_dependencies(unit_tests rttr_core)
target_include_directories(unit_tests PUBLIC ${RTTR_3RD_PARTY_DIR})
set_target_properties(unit_tests PROPERTIES DEBUG_POSTFIX _d)

# run tests
add_custom_target(run_tests ALL
                  COMMAND "$<TARGET_FILE:unit_tests>"
                  DEPENDS unit_tests
                  COMMENT "Running unit_tests")


message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")