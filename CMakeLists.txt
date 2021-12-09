cmake_minimum_required(VERSION 3.19)

project(libserdes LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SERDES_BUILD_TESTS "Build libserdes tests" ON)

set(SERDES_LIB_DIR include)
set(SERDES_TESTS_DIR tests)


file(GLOB_RECURSE SERDES_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${SERDES_LIB_DIR}/*.cc" "${SERDES_LIB_DIR}/*.h" "${SERDES_LIB_DIR}/*.?pp")


if(SERDES_BUILD_TESTS)

add_executable(test_serdes
  tests/test.cpp
  ${SERDES_LIB_SRC}
)

endif()

add_library(serdes INTERFACE)
target_sources(serdes INTERFACE serdes.hpp)
target_sources(serdes PRIVATE ${SERDES_LIB_SRC})
