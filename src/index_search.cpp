#include "index_search.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void IndexSearch::build_index(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            index_file(entry.path());
        }
    }
}

void IndexSearch::index_file(const fs::path& file_path) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            index[word].push_back(file_path.string());
        }
    }
}

std::vector<std::string> IndexSearch::query(const std::string& term) const {
    auto it = index.find(term);
    if (it != index.end()) {
        return it->second;
    }
    return {};
}
