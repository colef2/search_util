cmake_minimum_required(VERSION 3.14)
project(my_search_util)

set(CMAKE_CXX_STANDARD 20)

# Force Google Test to use the same runtime library as the rest of the project
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

# These two are to reduce compiler warnings for now
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

string(TIMESTAMP CURRENT_DATE "%Y-%m-%d")
if(CURRENT_DATE GREATER "2025-05-05")
  message(WARNING "Review error suppression lines added on 2025-02-08")
endif()


# Add Google Test manually instead of using find_package
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add the project root directory to the include path
include_directories(${CMAKE_SOURCE_DIR})

# Add all source files
add_executable(my_search_util
    src/main.cpp
    src/boyer_moore.cpp
    src/index_search.cpp
    src/hybrid_search.cpp
)

enable_testing()
add_subdirectory(tests)