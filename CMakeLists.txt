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

####################################################################################
# Welcome to the CMake build system for RTTR( Run Time Type Reflection).           #
# This is the main file where the general build environment is set-up and the      #
# the build configuration options are initialized.                                 #
####################################################################################

cmake_minimum_required (VERSION 3.0)

project (rttr LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# our little cmake helper functions
include(utility)

# set up option variable for cmake

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(RTTR_DEFAULT_BUILD_DYNAMIC ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_UNIT_TESTS ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_STATIC OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WITH_STATIC_RUNTIME_LIBS OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WITH_RTTI ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_BENCHMARKS OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_EXAMPLES ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_DOCUMENTATION ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_INSTALLER ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_PACKAGE ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_USE_PCH ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_CUSTOM_DOXYGEN_STYLE ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WEBSITE_DOCU OFF CACHE INTERNAL "")
elseif()
  set(RTTR_DEFAULT_BUILD_DYNAMIC ON CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_STATIC OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WITH_STATIC_RUNTIME_LIBS OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WITH_RTTI OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_BENCHMARKS OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_EXAMPLES OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_DOCUMENTATION OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_INSTALLER OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_PACKAGE OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_USE_PCH OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_CUSTOM_DOXYGEN_STYLE OFF CACHE INTERNAL "")
  set(RTTR_DEFAULT_BUILD_WEBSITE_DOCU OFF CACHE INTERNAL "")
endif()

option(RTTR_BUILD_DYNAMIC "Build the dynamic/shared version of RTTR library" ${RTTR_DEFAULT_BUILD_DYNAMIC})
option(RTTR_BUILD_UNIT_TESTS "Build the unit tests of RTTR" ${RTTR_DEFAULT_BUILD_UNIT_TESTS})
option(RTTR_BUILD_STATIC "Build RTTR as static library" ${RTTR_DEFAULT_BUILD_STATIC})
option(RTTR_BUILD_WITH_STATIC_RUNTIME_LIBS "Link against the static runtime libraries" ${RTTR_DEFAULT_BUILD_WITH_STATIC_RUNTIME_LIBS})
option(RTTR_BUILD_WITH_RTTI "Enable build with C++ runtime type information for compilation" ${RTTR_DEFAULT_BUILD_WITH_RTTI})
option(RTTR_BUILD_BENCHMARKS "Enable this to build the benchmarks" ${RTTR_DEFAULT_BUILD_BENCHMARKS})
option(RTTR_BUILD_EXAMPLES "Enable this to build the examples" ${RTTR_DEFAULT_BUILD_EXAMPLES})
option(RTTR_BUILD_DOCUMENTATION "Enable this to build the documentation" ${RTTR_DEFAULT_BUILD_DOCUMENTATION})
option(RTTR_BUILD_INSTALLER "Enable this to build the installer" ${RTTR_DEFAULT_BUILD_INSTALLER})
option(RTTR_BUILD_PACKAGE "Enable this to build the installer" ${RTTR_DEFAULT_BUILD_PACKAGE})
option(RTTR_USE_PCH "Use precompiled header files for compilation" ${RTTR_DEFAULT_USE_PCH})
option(RTTR_CUSTOM_DOXYGEN_STYLE "Enable this option to use a custom doxygen style for HTML documentation; Otherwise the default will be used" ${RTTR_DEFAULT_CUSTOM_DOXYGEN_STYLE})
option(RTTR_BUILD_WEBSITE_DOCU "Enable this option to create the special docu for the website" ${RTTR_DEFAULT_BUILD_WEBSITE_DOCU})

# one precompiled headers cannot be used for multiple ninja targets
# thats why we have to disable this option, when RTTR_BUILD_STATIC or
# RTTR_BUILD_WITH_STATIC_RUNTIME_LIBS is ON (every target will create the same PCH.pch file)
# to get it working, we need the feature to enable different source properties
# for different targets
if (RTTR_USE_PCH)
  if (RTTR_BUILD_STATIC OR RTTR_BUILD_WITH_STATIC_RUNTIME_LIBS)
    set(RTTR_USE_PCH FALSE)
  endif()
endif()

include(config)
include(3rd_party_libs)

if (RTTR_BUILD_PACKAGE)
    include(installer)
endif()

# here we add our source code
add_subdirectory(src)
# and the documentation
if (RTTR_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()
