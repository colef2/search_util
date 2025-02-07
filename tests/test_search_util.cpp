#include <gtest/gtest.h>
#include "search_util.h" // Include your search utility header
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Test fixture for shared setup and teardown
class SearchTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test directory and files
        fs::create_directory("test_files");

        // Create a basic test file
        std::ofstream basic_file("test_files/basic.txt");
        basic_file << "hello world\n";
        basic_file << "this is a test\n";
        basic_file.close();

        // Create a large test file
        std::ofstream large_file("test_files/large_file.txt");
        for (int i = 0; i < 1000000; ++i) {
            large_file << "line " << i << "\n";
        }
        large_file.close();

        // Create a file with special characters
        std::ofstream special_file("test_files/special.txt");
        special_file << "line with $pecial ch@r@cters\n";
        special_file.close();

        // Create a file with very long lines
        std::ofstream long_line_file("test_files/long_line.txt");
        std::string long_line(10000, 'a'); // A line with 10,000 'a's
        long_line_file << long_line << "\n";
        long_line_file.close();

        // Create a binary file
        std::ofstream binary_file("test_files/binary.bin", std::ios::binary);
        binary_file << "\x00\x01\x02\x03";
        binary_file.close();
    }

    void TearDown() override {
        // Clean up the test directory
        fs::remove_all("test_files");
    }
};

// Basic functionality tests
TEST_F(SearchTestFixture, BasicSearch) {
    std::string query = "hello";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("hello"), std::string::npos);
}

TEST_F(SearchTestFixture, CaseSensitiveSearch) {
    std::string query = "Hello";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("hello"), std::string::npos); // Should not find "hello"
}

TEST_F(SearchTestFixture, CaseInsensitiveSearch) {
    std::string query = "HELLO";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("hello"), std::string::npos); // Should find "hello"
}

// Edge case tests
TEST_F(SearchTestFixture, EmptyQuery) {
    std::string query = "";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(SearchTestFixture, EmptyFile) {
    std::string query = "test";
    std::string path = "test_files/empty.txt";
    std::ofstream empty_file(path); // Create an empty file
    empty_file.close();

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(SearchTestFixture, NoMatches) {
    std::string query = "nonexistent";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(SearchTestFixture, SpecialCharacters) {
    std::string query = "$pecial";
    std::string path = "test_files/special.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("$pecial"), std::string::npos);
}

TEST_F(SearchTestFixture, VeryLongLines) {
    std::string query = "a";
    std::string path = "test_files/long_line.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("a"), std::string::npos);
}

TEST_F(SearchTestFixture, BinaryFile) {
    std::string query = "\x00\x01";
    std::string path = "test_files/binary.bin";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty()); // Binary files should be skipped
}

// Stress tests
TEST_F(SearchTestFixture, LargeFile) {
    std::string query = "line";
    std::string path = "test_files/large_file.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("line"), std::string::npos);
}

TEST_F(SearchTestFixture, DeepDirectory) {
    std::string query = "test";
    std::string path = "test_files/deep_directory";
    fs::create_directories(path + "/sub1/sub2/sub3");
    std::ofstream file(path + "/sub1/sub2/sub3/file.txt");
    file << "test\n";
    file.close();

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("test"), std::string::npos);
}

// Error handling tests
TEST_F(SearchTestFixture, InvalidFilePath) {
    std::string query = "test";
    std::string path = "nonexistent_file.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Invalid path"), std::string::npos);
}

TEST_F(SearchTestFixture, NonAsciiFile) {
    std::string query = "test";
    std::string path = "test_files/non_ascii.txt";
    std::ofstream non_ascii_file(path);
    non_ascii_file << "こんにちは\n"; // Non-ASCII characters
    non_ascii_file.close();

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty()); // Non-ASCII files should be skipped
}