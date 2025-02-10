#include "hybrid_search.h"
#include <iostream>
#include <string>

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [-i] <query> <path>" << std::endl;
    std::cerr << "  -i: Perform case-insensitive search" << std::endl;
}

int main(int argc, char* argv[]) {
    bool case_insensitive = false;
    int arg_offset = 0;

    if (argc < 3 || argc > 4) {
        print_usage(argv[0]);
        return 1;
    }

    if (std::string(argv[1]) == "-i") {
        if (argc != 4) {
            print_usage(argv[0]);
            return 1;
        }
        case_insensitive = true;
        arg_offset = 1;
    }

    std::string query = argv[1 + arg_offset];
    std::string path = argv[2 + arg_offset];

    HybridSearch hybrid_search;

    if (fs::is_directory(path)) {
        std::cout << "Building index..." << std::endl;
        hybrid_search.build_index(path);
    }

    //std::cout << "Searching for '" << query << "' in " << path << std::endl;
    hybrid_search.search(query, path);

    return 0;
}
