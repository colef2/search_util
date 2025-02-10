cmake_minimum_required(VERSION 3.14)
project(my_search_util)

set(CMAKE_CXX_STANDARD 20)

# Force Google Test to use the same runtime library as the rest of the project
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Add the project root directory to the include path
include_directories(${CMAKE_SOURCE_DIR})

add_executable(my_search_util
    src/main.cpp
    src/boyer_moore.cpp
)

enable_testing()
add_subdirectory(tests)