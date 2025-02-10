#include "boyer_moore.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <thread>

// If this were a security-focused application, would not have a blacklist or would handle differently.
const std::unordered_set<std::string> BoyerMoore::blacklisted_extensions = {
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

BoyerMoore::BoyerMoore(const std::string& query, const std::string& path, bool case_insensitive)
    : query(query), path(path), case_insensitive(case_insensitive),
      regex_pattern(case_insensitive ? std::regex(query, std::regex::icase) : std::regex(query)),
      bm_searcher(query.data(), query.data() + query.length())
{
    if (case_insensitive) {
        std::transform(this->query.begin(), this->query.end(), this->query.begin(), ::tolower);
    }
}

bool BoyerMoore::should_skip_file(const fs::path& file_path) const {
    std::string extension = file_path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return blacklisted_extensions.find(extension) != blacklisted_extensions.end();
}

bool BoyerMoore::is_ascii_file(const fs::path& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    char ch;
    while (file.get(ch)) {
        if (static_cast<unsigned char>(ch) > 127) {
            return false;
        }
    }
    return true;
}

std::string BoyerMoore::normalize_path(const fs::path& path) {
    std::string normalized = path.string();
    std::replace(normalized.begin(), normalized.end(), '\\', '/'); // Replace backslashes with forward slashes
    return normalized;
}

std::string BoyerMoore::search_file(const fs::path& file_path) {
    if (should_skip_file(file_path) || !is_ascii_file(file_path)) {
        return "";
    }

    std::ifstream file(file_path);
    if (!file) {
        //std::cerr << "Error opening file: " << normalize_path(file_path) << std::endl;
        return "";
    }

    std::string line;
    std::string file_path_str = normalize_path(file_path.string());
    std::stringstream results;

    while (std::getline(file, line)) {
        std::string search_line = case_insensitive ? to_lower(line) : line;
        if (std::regex_search(search_line, regex_pattern)) {
            results << "\"" << file_path_str << "\": " << line << "\n";
        }
    }

    return results.str();
}

std::string BoyerMoore::to_lower(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

void BoyerMoore::search_directory(const fs::path& dir_path) {
    std::vector<std::future<std::string>> futures;
    std::stack<fs::path> directories_to_process; // Stack for manual recursion
    directories_to_process.push(dir_path);

    while (!directories_to_process.empty()) {
        fs::path current_dir = directories_to_process.top();
        directories_to_process.pop();

        try {
            for (const auto& entry : fs::directory_iterator(current_dir, fs::directory_options::skip_permission_denied)) {
                try {
                    if (entry.is_directory()) {
                        // Add subdirectory to stack for later processing
                        directories_to_process.push(entry.path());
                    } else if (entry.is_regular_file()) {
                        // Process file asynchronously
                        futures.push_back(std::async(std::launch::async, &BoyerMoore::search_file, this, entry.path()));
                    }
                } catch (const fs::filesystem_error&) {
                    /*std::cerr << "Error accessing entry: " << entry.path().string()
                              << " - " << ex.what() << std::endl;*/
                }
            }
        } catch (const fs::filesystem_error&) {
            /*std::cerr << "Error accessing directory: " << current_dir.string()
                      << " - " << e.what() << std::endl;*/
        }
    }

    // Process all futures to ensure results are printed
    for (auto& future : futures) {
        try {
            std::string result = future.get();
            if (!result.empty()) {
                std::cout << result;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in async task: " << e.what() << std::endl;
        }
    }
}

void BoyerMoore::search() {
    try {
        fs::path search_path(path);
        if (fs::is_directory(search_path)) {
            search_directory(search_path);
        } else if (fs::is_regular_file(search_path)) {
            // Directly print the result of searching a single file
            std::cout << search_file(search_path);
        } else {
            std::cout << "Invalid path: " << path << "\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Caught filesystem_error during search: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Caught exception during search: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception during search." << std::endl;
    }
}


