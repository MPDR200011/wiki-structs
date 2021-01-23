cmake_minimum_required(VERSION 3.12)

project(wiki-structs)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Catch2 REQUIRED)

set(TEST_SRC
    test/main.cpp
    test/test_binary_heap.cpp
    test/test_fibonacci_heap.cpp
)

add_executable(tests ${TEST_SRC})

target_include_directories(tests PRIVATE src)

target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)
