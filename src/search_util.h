#ifndef SEARCH_UTIL_H
#define SEARCH_UTIL_H

#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

class SearchUtil {
public:
    SearchUtil(const std::string& query, const std::string& path);
    void search();

private:
    std::string query;
    std::string path;
    std::regex regex_pattern;
    std::boyer_moore_searcher<const char*> bm_searcher;

    bool is_ascii_file(const fs::path& file_path);
    void search_file(const fs::path& file_path);
    void search_directory(const fs::path& dir_path);
};

#endif // SEARCH_UTIL_H