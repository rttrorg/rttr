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

find_package(Doxygen)
# These variables are defined in the doxygen file(@VAR@) as place holders
# an will be substituted by the following values
set(DOXYGEN_PROJECT_NUMBER    "${RTTR_VERSION_STR}")
set(DOXYGEN_PROJECT_NAME      "${PROJECT_NAME}")
set(DOXYGEN_INPUT_DIRECTORY   "${CMAKE_SOURCE_DIR}")
set(DOXYGEN_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/doc")
set(DOXYGEN_DOC_INSTALL_DIR   "doc")
set(DOXYGEN_CONFIG_FILE       "DoxyFile.cfg")
set(DOXYGEN_EXCLUDE           "")
set(DOXYGEN_STRIP_FROM_PATH   "${CMAKE_SOURCE_DIR}/src/rttr")
set(DOXYGEN_PREDEFINED        "RTTR_EXPORT= DOXYGEN")
set(DOXYGEN_EXCLUDE_SYMBOLS   "impl* \\
                               impl::* \\
                               detail* \\
                               detail:: \\
                               *MetaTypeInfo* \\
                               *method_container* \\
                               *property_container* \\
                               ")

set(DOXYGEN_EXCLUDE_PATTERNS  "*.cpp* \\
                              *_p.cpp* \\
                              *PCH.h* \\
                              *_impl.h* \\
                              *_p.h* \\
                              */src/test/* \\
                              */src/benchmark/*")

if(NOT DOXYGEN_FOUND)
  message(WARNING "Doxygen not found - Documentation will not be generated!")
  return()
endif()
  
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE})
  message(WARNING "Doxygen configuration file not found - Documentation will not be generated!")
  return()
endif()

# modify the doxygen file, i.e. substiture the variables with values
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} ${DOXYGEN_OUTPUT_DIRECTORY}/${DOXYGEN_CONFIG_FILE})
# and also the redirect short cut
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/index.html DESTINATION ${DOXYGEN_OUTPUT_DIRECTORY})

add_custom_target(Doc ALL 
                  COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUTPUT_DIRECTORY}/${DOXYGEN_CONFIG_FILE}"
                  WORKING_DIRECTORY "${DOXYGEN_OUTPUT_DIRECTORY}"
                  COMMENT "Building documentation...")
         
install(DIRECTORY "${DOXYGEN_OUTPUT_DIRECTORY}/html" 
        DESTINATION "${DOXYGEN_DOC_INSTALL_DIR}"
        PATTERN "*.*"
        PERMISSIONS OWNER_READ)
        
install(FILES "${DOXYGEN_OUTPUT_DIRECTORY}/index.html" 
        DESTINATION "${DOXYGEN_DOC_INSTALL_DIR}" 
        PERMISSIONS OWNER_READ)
