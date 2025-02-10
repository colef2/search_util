#include <gtest/gtest.h>
#include "src/boyer_moore.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

// Test fixture for shared setup and teardown
class BoyerMooreTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(BoyerMooreTestFixture, BasicSearch) {
    std::string query = "hello";
    std::string path = "test_files/basic.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n");
}

TEST_F(BoyerMooreTestFixture, MultipleMatchesInFile) {
    std::string query = "test";
    std::string path = "test_files/multiple_matches.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output,
              "\"test_files/multiple_matches.txt\": This is a test line.\n"
              "\"test_files/multiple_matches.txt\": Another test line.\n"
              "\"test_files/multiple_matches.txt\": Yet another test line.\n");
}

TEST_F(BoyerMooreTestFixture, CaseSensitiveSearch) {
    std::string query = "Hello";
    std::string path = "test_files/basic.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(BoyerMooreTestFixture, CaseInsensitiveSearch) {
    std::string query = "HELLO";
    std::string path = "test_files/basic.txt";
    BoyerMoore boyer_moore(query, path, true);  // Assuming BoyerMoore constructor takes a bool for case insensitivity

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n");
}

TEST_F(BoyerMooreTestFixture, EmptyQuery) {
    std::string query = "";
    std::string path = "test_files/basic.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/basic.txt\": hello world\n\"test_files/basic.txt\": this is a test\n");
}

TEST_F(BoyerMooreTestFixture, EmptyFile) {
    std::string query = "test";
    std::string path = "test_files/empty.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(BoyerMooreTestFixture, NoMatches) {
    std::string query = "nonexistent";
    std::string path = "test_files/basic.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(BoyerMooreTestFixture, FileWithoutExtension) {
    std::string query = "test";
    std::string path = "test_files/file_without_extension";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/file_without_extension\": This is a test content.\n");
}

TEST_F(BoyerMooreTestFixture, SpecialCharacters) {
    std::string query = "#pecial";
    std::string path = "test_files/special.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    std::cerr << "Actual output: " << output << std::endl;
    EXPECT_EQ(output, "\"test_files/special.txt\": line with #pecial ch@r@cters\n");
}

TEST_F(BoyerMooreTestFixture, VeryLongLines) {
    std::string query = "a";
    std::string path = "test_files/long_line.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/long_line.txt\": " + std::string(10000, 'a') + "\n");
}

TEST_F(BoyerMooreTestFixture, BinaryFile) {
    std::string query = "\x00\x01";
    std::string path = "test_files/binary.bin";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(BoyerMooreTestFixture, LargeFile) {
    std::string query = "line to find";
    std::string path = "test_files/large_file.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/large_file.txt\": line to find\n");
}

TEST_F(BoyerMooreTestFixture, DeepDirectory) {
    std::string query = "test";
    std::string path = "test_files/deep_directory";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\"test_files/deep_directory/sub1/sub2/sub3/file.txt\": test content\n");
}

TEST_F(BoyerMooreTestFixture, MixedAsciiAndNonAsciiFile) {
    std::string query = "test";
    std::string path = "test_files/mixed_ascii_nonascii.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty()); // Output should be empty for mixed ASCII and non-ASCII files
}

TEST_F(BoyerMooreTestFixture, InvalidFilePath) {
    std::string query = "test";
    std::string path = "nonexistent_file.txt";
    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Invalid path: nonexistent_file.txt\n");
}

TEST_F(BoyerMooreTestFixture, NonAsciiFile) {
    std::string query = "test";
    std::string path = "test_files/non_ascii.txt";

    BoyerMoore boyer_moore(query, path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty());
}

TEST_F(BoyerMooreTestFixture, DirectoryWithNoMatchingFiles) {
    std::string query = "notfound";
    std::string dir_path = "test_files/no_matches_dir";

    // Create the directory and files without the query
    fs::create_directories(dir_path);

    std::ofstream file1(dir_path + "/file1.txt");
    file1 << "This is some random text.";
    file1.close();

    std::ofstream file2(dir_path + "/file2.txt");
    file2 << "Another unrelated line.";
    file2.close();

    BoyerMoore boyer_moore(query, dir_path);

    testing::internal::CaptureStdout();
    boyer_moore.search();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "");
}
