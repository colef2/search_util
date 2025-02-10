#include "hybrid_search.h"
#include <iostream>

HybridSearch::HybridSearch() : use_indexing(false), is_index_built(false) {}

void HybridSearch::build_index(const fs::path& directory) {
    index_search.build_index(directory);
    is_index_built = true;
    use_indexing = true;
}

void HybridSearch::search(const std::string& query, const fs::path& path) {
    if (use_indexing && is_index_built) {
        auto results = index_search.query(query);
        for (const auto& result : results) {
            std::cout << result << ": " << query << std::endl;
        }
    } else {
        BoyerMoore boyer_moore(query, path.string());
        boyer_moore.search();
    }
}
