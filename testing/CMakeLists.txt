cmake_minimum_required(VERSION 3.29)
project(Testing CXX)

set(CMAKE_CXX_STANDARD 17)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE encryption GTest::GTest GTest::Main)
target_include_directories(tests PUBLIC include)

gtest_discover_tests(tests)
