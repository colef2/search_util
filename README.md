#Instructions (for Windows, can differ slightly in another env)
1. Install CMake
2. Verify Installation was successful with "cmake --version". Add CMake to PATH if needed
3. Build Project by navigating to "search_util/build" folder and running "cmake .."
4. Then running "cmake --build . --config Release" to Compile the project


#Use Case
1. ./Build/Debug/my_search_util "search" src/
2. ./Build/Debug/my_search_util -i "SEARCH" src/

#Overview
-The goal of this project is to implement a Grep-like utility in C++ that recursively searches for a given query in all files within a directory. The utility will:
-Use the C++20 standard.
-Utilize all available CPU cores for parallel processing.
-Handle large directories and files efficiently.
-Exclude files with non-ASCII content.