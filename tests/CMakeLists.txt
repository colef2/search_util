add_executable(test_search_util test_search_util.cpp ../src/search_util.cpp)
target_link_libraries(test_search_util gtest gtest_main)

# Add the test to CTest
include(CTest)

add_test(NAME test_search_util COMMAND test_search_util)