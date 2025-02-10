#include "search_util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <future>

const std::unordered_set<std::string> SearchUtil::blacklisted_extensions = {
    ".exe", ".dll", ".so", ".dylib", ".bin", ".app",
    ".o", ".obj", ".class",
    ".zip", ".tar", ".gz", ".bz2", ".7z", ".rar",
    ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".ico",
    ".mp3", ".mp4", ".avi", ".mov", ".flv", ".wav",
    ".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx",
    ".db", ".sqlite", ".mdb",
    ".iso", ".img", ".dmg",
    ".ttf", ".otf",
    ".dat", ".pkl"
};

SearchUtil::SearchUtil(const std::string& query, const std::string& path, bool case_insensitive)
    : query(query), path(path), case_insensitive(case_insensitive),
      regex_pattern(case_insensitive ? std::regex(query, std::regex::icase) : std::regex(query)),
      bm_searcher(query.data(), query.data() + query.length())
{
    if (case_insensitive) {
        std::transform(this->query.begin(), this->query.end(), this->query.begin(), ::tolower);
    }
}

bool SearchUtil::should_skip_file(const fs::path& file_path) const {
    std::string extension = file_path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return blacklisted_extensions.find(extension) != blacklisted_extensions.end();
}


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

std::string SearchUtil::search_file(const fs::path& file_path) {
    if (should_skip_file(file_path) || !is_ascii_file(file_path)) {
        return "";
    }

    std::ifstream file(file_path);
    std::string line;
    std::string file_path_str = file_path.string();
    std::stringstream results;

    while (std::getline(file, line)) {
        std::string search_line = case_insensitive ? to_lower(line) : line;
        if (std::regex_search(search_line, regex_pattern)) {
            results << "\"" << file_path_str << "\": " << line << "\n";
        }
    }

    return results.str();
}

std::string SearchUtil::to_lower(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

void SearchUtil::search_directory(const fs::path& dir_path) {
    std::vector<std::future<std::string>> futures;

    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        if (entry.is_regular_file()) {
            futures.push_back(std::async(std::launch::async, &SearchUtil::search_file, this, entry.path()));
        }
    }

    for (auto& future : futures) {
        std::string result = future.get();
        if (!result.empty()) {
            std::cout << result;
        }
    }
}

void SearchUtil::search() {
    fs::path search_path(path);
    if (fs::is_directory(search_path)) {
        search_directory(search_path);
    } else if (fs::is_regular_file(search_path)) {
        std::cout << search_file(search_path);
    } else {
        std::cerr << "Invalid path: " << path << std::endl;
    }
}

