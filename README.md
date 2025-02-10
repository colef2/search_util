# Search Utility

A Grep-like utility implemented in C++20 for recursively searching files within a directory.

## Features

- Recursive directory search
- Parallel processing using all available CPU cores
- Efficient handling of large directories and files
- Exclusion of non-ASCII content
- Case-sensitive and case-insensitive search options
- Blacklist for specific file extensions

## Prerequisites

- CMake (version 3.14 or higher)
- C++20 compatible compiler

## Building the Project (Windows)

1. Install CMake
2. Verify installation with cmake --version. Add CMake to PATH if needed
3. Navigate to the build directory: cd search_util/build
4. Generate build files: cmake ..
5. Build the project: cmake --build . --config Release
6. Run the test suite: ctest (from the build directory)

# Use Case
1. Case sensitive: "./Build/Release/my_search_util "search" src/"
2. Case sensitive: "./Build/Release/my_search_util -i "SEARCH" src/"

# Overview
This project implements a Grep-like utility in C++ that recursively searches for a given query in all files within a directory. Key features include:
- C++20 standard compliance
- Utilization of all available CPU cores for parallel processing
- Efficient handling of large directories and files
- Exclusion of files with non-ASCII content
- Blacklist for specific file extensions (configurable in boyer_moore.cpp)

## Implementation Details

- Uses Boyer-Moore algorithm for efficient string searching
- Supports both file and directory paths as input
- Handles invalid paths with appropriate error messages

## Rationale for Using Boyer-Moore

1. **Regex Flexibility**: Adaptable for complex pattern matching in code.
2. **Partial Word Matching**: Useful for finding variable or function names.
3. **Low Overhead**: Suitable for frequently changing codebases.
4. **Memory Efficiency**: Performs well with large code samples.
5. **Adaptable to Various Search Patterns**: Efficient for both short and long queries.
6. **Simple Implementation**: Easy to maintain and modify.

While indexed search methods offer better performance for repeated queries on static data, Boyer-Moore provides the flexibility and low overhead needed for dynamic code repositories.
A hybrid approach combining indexed search for frequent queries and Boyer-Moore for dynamic data was considered. However, due to the project's time recommendation (1-3 hours), a more straightforward Boyer-Moore implementation was chosen.
