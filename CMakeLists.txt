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

    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_executable(test_serdes
                   tests/test.cpp
                   serdes.hpp
                   ${SERDES_LIB_SRC}
                   )

    target_link_libraries(test_serdes gtest_main)
    target_include_directories(test_serdes PUBLIC ${SERDES_LIB_DIR})
    include(GoogleTest)
    gtest_discover_tests(test_serdes)
endif()

add_library(serdes INTERFACE)
target_sources(serdes INTERFACE serdes.hpp)
target_sources(serdes PRIVATE ${SERDES_LIB_SRC})
