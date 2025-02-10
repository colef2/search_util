#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class IndexSearch {
public:
    void build_index(const fs::path& directory);
    std::vector<std::string> query(const std::string& term) const;

private:
    std::unordered_map<std::string, std::vector<std::string>> index;
    void index_file(const fs::path& file_path);
};
