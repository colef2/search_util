#pragma once

#include "boyer_moore.h"
#include "index_search.h"
#include <filesystem>

namespace fs = std::filesystem;

class HybridSearch {
public:
    HybridSearch();
    void build_index(const fs::path& directory);
    void search(const std::string& query, const fs::path& path);

private:
    IndexSearch index_search;
    bool use_indexing;
    bool is_index_built;
};
