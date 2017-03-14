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

project(bench_method)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

generateLibraryVersionVariables(${RTTR_VERSION_MAJOR} ${RTTR_VERSION_MINOR} ${RTTR_VERSION_PATCH}
                                "Benchmark method" "Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>" "MIT License")

loadFolder("bench_method" HPP_FILES SRC_FILES)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

if (USE_PCH)
  activate_precompiled_headers("pch.h" SRC_FILES)
endif()

add_executable(bench_method ${SRC_FILES} ${HPP_FILES})
if (MSVC)
  target_link_libraries(bench_method RTTR::Core ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY})
else()
  target_link_libraries(bench_method RTTR::Core ${CMAKE_THREAD_LIBS_INIT})
endif()

target_include_directories(bench_method PUBLIC ${NONIUS_DIR} ${Boost_INCLUDE_DIR})
target_compile_definitions(bench_method PUBLIC BOOST_ALL_NO_LIB)
if(MSVC)
    set_target_properties(bench_method PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
endif()

set_target_properties(bench_method PROPERTIES DEBUG_POSTFIX _d
                                              FOLDER "Benchmarks")
add_dependencies(bench_method RTTR::Core)

install(TARGETS bench_method
        RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR})

###############################################################################

if (BUILD_STATIC)
    add_executable(bench_method_lib ${SRC_FILES} ${HPP_FILES})
    if (MSVC)
      target_link_libraries(bench_method_lib RTTR::Core_Lib ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY} )
    else()
      target_link_libraries(bench_method_lib RTTR::Core_Lib ${CMAKE_THREAD_LIBS_INIT})
    endif()

    target_include_directories(bench_method_lib PUBLIC ${NONIUS_DIR} ${Boost_INCLUDE_DIR})
    target_compile_definitions(bench_method_lib PUBLIC BOOST_ALL_NO_LIB)
    if(MSVC)
        set_target_properties(bench_method_lib PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
    endif()

    set_target_properties(bench_method_lib PROPERTIES DEBUG_POSTFIX _d
                                                      FOLDER "Benchmarks")

    add_dependencies(bench_method_lib RTTR::Core_Lib)

    install(TARGETS bench_method_lib
            RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR})
endif()

###############################################################################

if (BUILD_WITH_STATIC_RUNTIME_LIBS)
    add_executable(bench_method_s ${SRC_FILES} ${HPP_FILES})
    if (MSVC)
      target_link_libraries(bench_method_s RTTR::Core_STL ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY} )
    else()
      target_link_libraries(bench_method_s RTTR::Core_STL ${CMAKE_THREAD_LIBS_INIT})
    endif()

    target_include_directories(bench_method_s PUBLIC ${NONIUS_DIR} ${Boost_INCLUDE_DIR})
    target_compile_definitions(bench_method_s PUBLIC BOOST_ALL_NO_LIB)
    if(MSVC)
        set_target_properties(bench_method_s PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
    endif()

    set_target_properties(bench_method_s PROPERTIES DEBUG_POSTFIX _d
                                                    FOLDER "Benchmarks")

    add_dependencies(bench_method_s RTTR::Core_STL)

    if (MSVC)
        target_compile_options(bench_method_s PUBLIC "/MT$<$<CONFIG:Debug>:d>")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(bench_method_s PROPERTIES LINK_FLAGS ${GNU_STATIC_LINKER_FLAGS})
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_target_properties(bench_method_s PROPERTIES LINK_FLAGS ${CLANG_STATIC_LINKER_FLAGS})
    else()
        message(SEND_ERROR "Do now know how to statically link against the standard library with this compiler.")
    endif()

    install(TARGETS bench_method_s
            RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR})

    if (BUILD_STATIC)
        add_executable(bench_method_lib_s ${SRC_FILES} ${HPP_FILES})
        if (MSVC)
          target_link_libraries(bench_method_lib_s RTTR::Core_Lib_STL ${Boost_CHRONO_LIBRARY} ${Boost_SYSTEM_LIBRARY} )
        else()
          target_link_libraries(bench_method_lib_s RTTR::Core_Lib_STL ${CMAKE_THREAD_LIBS_INIT})
        endif()

        target_include_directories(bench_method_lib_s PUBLIC ${NONIUS_DIR} ${Boost_INCLUDE_DIR})
        target_compile_definitions(bench_method_lib_s PUBLIC BOOST_ALL_NO_LIB)
        if(MSVC)
            set_target_properties(bench_method_lib_s PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
        endif()

        set_target_properties(bench_method_lib_s PROPERTIES DEBUG_POSTFIX _d
                                                            FOLDER "Benchmarks")

        add_dependencies(bench_method_lib_s RTTR::Core_Lib_STL)

        if (MSVC)
            target_compile_options(bench_method_lib_s PUBLIC "/MT$<$<CONFIG:Debug>:d>")
        elseif(CMAKE_COMPILER_IS_GNUCXX)
            set_target_properties(bench_method_lib_s PROPERTIES LINK_FLAGS ${GNU_STATIC_LINKER_FLAGS})
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set_target_properties(bench_method_lib_s PROPERTIES LINK_FLAGS ${CLANG_STATIC_LINKER_FLAGS})
        else()
            message(SEND_ERROR "Do now know how to statically link against the standard library with this compiler.")
        endif()

        install(TARGETS bench_method_lib_s
                RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR})
    endif()
endif()

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")
