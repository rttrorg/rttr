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

project(rttr)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

loadFolder("rttr" HPP_FILES SRC_FILES)

if (CREATE_UNITY_BUILD)
  generateUnityFile(UnityBuild SRC_FILES)
endif()

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

if (MSVC)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/version.rc
                 @ONLY)
  add_library(rttr SHARED ${UnityBuild} ${SRC_FILES} ${HPP_FILES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
else()
  add_library(rttr SHARED ${UnityBuild} ${SRC_FILES} ${HPP_FILES})
endif()
  
install(TARGETS rttr EXPORT rttr_lib
        RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR} 
        LIBRARY DESTINATION ${RTTR_BIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${RTTR_LIB_INSTALL_DIR})

# install the shared object also into the lib directory
install(TARGETS rttr
        RUNTIME DESTINATION ${RTTR_LIB_INSTALL_DIR}
        LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR})
        
install(EXPORT rttr_lib
        DESTINATION cmake
        FILE rttr-config.cmake)
        
target_include_directories(rttr PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include>
)

#set_target_properties(rttr PROPERTIES OUTPUT_NAME foo)
target_compile_definitions(rttr PRIVATE RTTR_DLL_EXPORTS)
target_compile_definitions(rttr PUBLIC RTTR_DLL )

set_target_properties(rttr PROPERTIES VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION})

if (BUILD_STATIC)
  add_library(rttr-static STATIC ${UnityBuild} ${SRC_FILES} ${HPP_FILES})
  
  install(TARGETS rttr-static EXPORT rttr_lib
          ARCHIVE DESTINATION ${RTTR_LIB_INSTALL_DIR}
          LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR})
        
  target_include_directories(rttr-static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include>)
  
  set_target_properties(rttr-static PROPERTIES VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION})
endif()

if (BUILD_WITH_STATIC_RUNTIME_LIBS)
  if (MSVC)
    add_library(rttr-stdstatic SHARED ${UnityBuild} ${SRC_FILES} ${HPP_FILES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
  else()
    add_library(rttr-stdstatic SHARED ${UnityBuild} ${SRC_FILES} ${HPP_FILES})
  endif()

  add_library(rttr-static-stdstatic STATIC ${UnityBuild} ${SRC_FILES} ${HPP_FILES})
  
  install(TARGETS rttr-stdstatic EXPORT rttr_lib
          RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR}
          ARCHIVE DESTINATION ${RTTR_LIB_INSTALL_DIR}
          LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR})
          
  install(TARGETS rttr-stdstatic
          RUNTIME DESTINATION ${RTTR_LIB_INSTALL_DIR}
          LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR})
          
  install(TARGETS rttr-static-stdstatic EXPORT rttr_lib
          ARCHIVE DESTINATION ${RTTR_LIB_INSTALL_DIR}
          LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR})
        
  target_include_directories(rttr-stdstatic PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include>)
  
  target_include_directories(rttr-static-stdstatic PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include>)
  
  set_target_properties(rttr-stdstatic PROPERTIES VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION})
  set_target_properties(rttr-static-stdstatic PROPERTIES VERSION ${RTTR_VERSION} SOVERSION ${RTTR_VERSION})

  if (MSVC)
    target_compile_options(rttr-stdstatic PUBLIC "/MT$<$<CONFIG:Debug>:d>")
    target_compile_options(rttr-static-stdstatic PUBLIC "/MT$<$<CONFIG:Debug>:d>")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(rttr-stdstatic PROPERTIES LINK_FLAGS "-static-libstdc++")
    set_target_properties(rttr-static-stdstatic PROPERTIES LINK_FLAGS "-static-libstdc++")
  else()
    message(SEND_ERROR "Do now know how to statically link against the standard library with this compiler.")
  endif()
endif()

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")