cmake_minimum_required(VERSION 3.14)

project(libserdes LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERDES_LIB_DIR include)

file(GLOB_RECURSE SERDES_LIB_SRC RELATIVE ${PROJECT_SOURCE_DIR} "${SERDES_LIB_DIR}/*.cc" "${SERDES_LIB_DIR}/*.h" "${SERDES_LIB_DIR}/*.?pp")

add_executable(serdes
  main.cpp
  ${SERDES_LIB_SRC}
)

