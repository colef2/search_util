#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H

#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

class BoyerMoore {
public:
    BoyerMoore(const std::string& query, const std::string& path, bool case_insensitive = false);
    void search();

private:
    std::string query;
    std::string path;
    bool case_insensitive;
    std::regex regex_pattern;
    std::boyer_moore_searcher<const char*> bm_searcher;
    static const std::unordered_set<std::string> blacklisted_extensions;

    bool should_skip_file(const fs::path& file_path) const;
    bool is_ascii_file(const fs::path& file_path);
    std::string normalize_path(const fs::path& path);
    std::string search_file(const fs::path& file_path);
    void search_directory(const fs::path& dir_path);
    std::string to_lower(const std::string& s);
};

#endif // BOYER_MOORE_H
