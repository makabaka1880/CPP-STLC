cmake_minimum_required(VERSION 3.28)
project(lambda CXX)

set(CMAKE_CXX_STANDARD 26)

# Use FetchContent to download and build Google Test
include(FetchContent)
FetchContent_Declare(
        gtest
        URL https://github.com/google/googletest/archive/v1.14.0.zip
)
# This will download, configure, and build gtest and make it available.
# It also includes the GoogleTest module, which defines gtest_discover_tests.
FetchContent_MakeAvailable(gtest)
include(GoogleTest)
# Enable testing
enable_testing()

# Create a library from your source files
add_library(lambda_lib
        terms/Terms.cpp
        terms/Variable.cpp
        terms/Application.cpp
        terms/Abstraction.cpp
        exceptions/Exceptions.cpp
)

target_include_directories(lambda_lib PUBLIC
        .
        terms
        exceptions
)

# Main executable
add_executable(lambda main.cpp)
target_link_libraries(lambda lambda_lib)

# Test executable
add_executable(lambda_tests
        tests/test_variable.cpp
        tests/test_application.cpp
        tests/test_abstraction.cpp
)

# Link the test executable to your project library and the gtest libraries
target_link_libraries(lambda_tests
        lambda_lib
        gtest_main
)

# This command must be called AFTER gtest has been made available
gtest_discover_tests(lambda_tests)