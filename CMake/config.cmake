####################################################################################
#                                                                                  #
#  Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     #
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

# setup version numbers
set(RTTR_VERSION_MAJOR 0)
set(RTTR_VERSION_MINOR 9)
set(RTTR_VERSION_PATCH 6)
set(RTTR_VERSION ${RTTR_VERSION_MAJOR}.${RTTR_VERSION_MINOR}.${RTTR_VERSION_PATCH})
set(RTTR_VERSION_STR "${RTTR_VERSION_MAJOR}.${RTTR_VERSION_MINOR}.${RTTR_VERSION_PATCH}")
math(EXPR RTTR_VERSION_CALC "${RTTR_VERSION_MAJOR}*1000 + ${RTTR_VERSION_MINOR}*100 + ${RTTR_VERSION_PATCH}")
set(RTTR_PRODUCT_NAME "RTTR")
message("Project version: ${RTTR_VERSION_STR}")

# files
set(README_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(LICENSE_FILE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

# in order to group in visual studio the targets into solution filters
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#3rd part dependencies dirs
set(RTTR_3RD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rd_party")

is_vs_based_build(VS_BUILD)

set(CMAKE_DEBUG_POSTFIX CACHE STRING "_d")

# detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(RTTR_NATIVE_ARCH 64)
    message(STATUS "Architecture: x64")
else()
    set(RTTR_NATIVE_ARCH 32)
    message(STATUS "Architecture: x86")
endif()

enable_rtti(BUILD_WITH_RTTI)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Werror")
    message(STATUS "added flag -std=c++0x, -Wall, -Werror to g++")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
    message(STATUS "added flag -std=c++11, -Wall, -Werror to g++")
  endif()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.0.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
  endif()

  if(MINGW)
    set(GNU_STATIC_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
  else()
    set(GNU_STATIC_LINKER_FLAGS "-static-libgcc -static-libstdc++")
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
  message(STATUS "added flag -std=c++11, -Wall, -Werror to g++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
  message(WARNING "clang support is currently experimental")
  
  set(CLANG_STATIC_LINKER_FLAGS "-stdlib=libc++ -static")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /WX")
    replaceCompilerOption("/W3" "/W4")
    message(STATUS "added flag /bigobj, /W4 to MSVC compiler")
    message(STATUS "Treats all compiler warnings as errors.")
endif()

# RelWithDepInfo should have the same option like the Release build
# but of course with Debug informations
if(MSVC)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zi /DEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX )
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")
else()
  message(WARNING "Please adjust CMAKE_CXX_FLAGS_RELWITHDEBINFO flags for this compiler!")
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CMake/rttr-config-version.cmake"
  VERSION ${RTTR_VERSION_STR}
  COMPATIBILITY AnyNewerVersion
)

if (BUILD_INSTALLER)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CMake/rttr-config-version.cmake"
             DESTINATION ${CMAKE_INSTALL_DATADIR}/rttr/cmake
             COMPONENT Devel)
endif()
