# Include GoogleTest module
include(GoogleTest)

# Define test executable
add_executable(test_boyer_moore
    test_boyer_moore.cpp
    ../src/boyer_moore.cpp
)

# Link against GoogleTest libraries
target_link_libraries(test_boyer_moore
    PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Discover and add tests
gtest_discover_tests(test_boyer_moore
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
