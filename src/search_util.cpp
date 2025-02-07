#include "search_util.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <future>

SearchUtil::SearchUtil(const std::string& query, const std::string& path)
    : query(query), path(path), regex_pattern(query), bm_searcher(query.data(), query.data() + query.length()) {}

bool SearchUtil::is_ascii_file(const fs::path& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    char ch;
    while (file.get(ch)) {
        if (static_cast<unsigned char>(ch) > 127) {
            return false;
        }
    }
    return true;
}

void SearchUtil::search_file(const fs::path& file_path) {
    if (!is_ascii_file(file_path)) {
        return;
    }

    std::ifstream file(file_path);
    std::string line;
    int line_number = 1;

    while (std::getline(file, line)) {
        if (std::regex_search(line, regex_pattern)) {
            std::cout << file_path << ":" << line_number << ": " << line << std::endl;
        }
        line_number++;
    }
}

void SearchUtil::search_directory(const fs::path& dir_path) {
    std::vector<std::future<void>> futures;

    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        if (entry.is_regular_file()) {
            futures.push_back(std::async(std::launch::async, &SearchUtil::search_file, this, entry.path()));
        }
    }

    for (auto& future : futures) {
        future.wait();
    }
}

void SearchUtil::search() {
    fs::path search_path(path);
    if (fs::is_directory(search_path)) {
        search_directory(search_path);
    } else if (fs::is_regular_file(search_path)) {
        search_file(search_path);
    } else {
        std::cerr << "Invalid path: " << path << std::endl;
    }
}