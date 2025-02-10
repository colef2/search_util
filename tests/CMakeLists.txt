include(GoogleTest)

add_executable(test_boyer_moore test_boyer_moore.cpp ../src/boyer_moore.cpp)
target_link_libraries(test_boyer_moore GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_boyer_moore
    PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(${ALL_TESTS} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
