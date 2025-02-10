#include <gtest/gtest.h>
#include "src/search_util.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

// Helper function to create a test file with given content
void createTestFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

// Helper function to read file contents
std::string readFileContents(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Test fixture for shared setup and teardown
class SearchTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(SearchTestFixture, BasicSearch) {
    std::string query = "hello";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n");
}

TEST_F(SearchTestFixture, CaseSensitiveSearch) {
    std::string query = "Hello";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(SearchTestFixture, CaseInsensitiveSearch) {
    std::string query = "HELLO";
    std::string path = "test_files/basic.txt";
    SearchUtil search_util(query, path, true);  // Assuming SearchUtil constructor takes a bool for case insensitivity

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n");
}

TEST_F(SearchTestFixture, EmptyQuery) {
    std::string query = "";
    std::string path = "test_files/basic.txt";
    createTestFile(path, "hello world\nthis is a test\n");
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n\"test_files/basic.txt\": this is a test\n");
}

TEST_F(SearchTestFixture, EmptyFile) {
    std::string query = "test";
    std::string path = "test_files/empty.txt";
    createTestFile(path, "");
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
    std::string query = "#pecial";
    std::string path = "test_files/special.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    std::cerr << "Actual output: " << output << std::endl;
    EXPECT_EQ(output, "\"test_files/special.txt\": line with #pecial ch@r@cters\n");
}


TEST_F(SearchTestFixture, VeryLongLines) {
    std::string query = "a";
    std::string path = "test_files/long_line.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/long_line.txt\": " + std::string(10000, 'a') + "\n");
}

TEST_F(SearchTestFixture, BinaryFile) {
    std::string query = "\x00\x01";
    std::string path = "test_files/binary.bin";

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(SearchTestFixture, LargeFile) {
    std::string query = "line to find";
    std::string path = "test_files/large_file.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/large_file.txt\": line to find\n");
}

TEST_F(SearchTestFixture, DeepDirectory) {
    std::string query = "test";
    std::string path = "test_files/deep_directory";
    fs::create_directories(path + "/sub1/sub2/sub3");
    createTestFile(path + "/sub1/sub2/sub3/file.txt", "test content\n");

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/deep_directory/sub1/sub2/sub3/file.txt\": test content\n");
}

TEST_F(SearchTestFixture, InvalidFilePath) {
    std::string query = "test";
    std::string path = "nonexistent_file.txt";
    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Invalid path: nonexistent_file.txt\n");
}

TEST_F(SearchTestFixture, NonAsciiFile) {
    std::string query = "test";
    std::string path = "test_files/non_ascii.txt";
    createTestFile(path, "こんにちは\n");

    SearchUtil search_util(query, path);

    testing::internal::CaptureStdout();
    search_util.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}
