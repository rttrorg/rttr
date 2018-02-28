project(test_package)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#test find package method of finding RTTR
find_package(rttr)

add_executable(${PROJECT_NAME} test_package.cpp)

if(TARGET RTTR::Core)
  message(STATUS "found shared RTTR build")
  target_link_libraries(${PROJECT_NAME} RTTR::Core)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DRTTR_SHARED)
endif()
if(TARGET RTTR::Core_Lib)
  message(STATUS "found static RTTR build")
  target_link_libraries(${PROJECT_NAME} RTTR::Core_Lib)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DRTTR_STATIC)
endif()


add_executable(${PROJECT_NAME}_2 test_package.cpp)
target_link_libraries(${PROJECT_NAME}_2 ${CONAN_LIBS})
