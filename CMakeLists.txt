cmake_minimum_required(VERSION 3.14)
project(my_search_util)

# Find Google Test
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# Add the project root directory to the include path
include_directories(${CMAKE_SOURCE_DIR})

add_executable(my_search_util src/main.cpp src/search_util.cpp)

# Optional: Add tests
enable_testing()
add_subdirectory(tests)