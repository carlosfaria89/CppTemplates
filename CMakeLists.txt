cmake_minimum_required(VERSION 3.15)
project(CppTemplates LANGUAGES CXX)

# Set CXX Standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check whether VCPKG is set up in your system
if(NOT DEFINED ENV{VCPKG_ROOT})
  message(FATAL_ERROR "ERROR: This project requires VCPKG.\n")
endif()

# Set VCPKG root directory
set(VCPKG_ROOT $ENV{VCPKG_ROOT})

# Include VCPKG toolchain
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Add library project
set(LIB_PROJECT ${CMAKE_PROJECT_NAME}_LIB)
add_library(${LIB_PROJECT} include/GenericPointSet.h src/GenericPointSet.cpp
        src/GenericPointSet.cpp)
target_include_directories(${LIB_PROJECT} PRIVATE include)

# Add executable project
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp
        src/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PROJECT})

# Setup GTest
enable_testing()
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Add test project
set(TEST_PROJECT ${CMAKE_PROJECT_NAME}_TEST)
add_executable(${TEST_PROJECT} test/test_main.cpp test/GenericPointSet.cpp
        test/GenericPointSet.cpp)
target_include_directories(${TEST_PROJECT} PRIVATE include)
target_link_libraries(${TEST_PROJECT} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${LIB_PROJECT})

# Automatically discover and add tests
gtest_discover_tests(${TEST_PROJECT})