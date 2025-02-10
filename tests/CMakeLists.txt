include(GoogleTest)

add_executable(test_search_util test_search_util.cpp ../src/search_util.cpp)
target_link_libraries(test_search_util GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_search_util
    PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(${ALL_TESTS} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
