#include "boyer_moore.h"
#include <iostream>
#include <string>

// Prints the usage instructions for the program
void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [-i] <query> <path>\n";
    std::cerr << "  -i: Perform case-insensitive search\n";
}

int main(int argc, char* argv[]) {
    // Validate argument count
    if (argc < 3 || argc > 4) {
        print_usage(argv[0]);
        return 1;
    }

    // Parse arguments
    bool case_insensitive = false;
    int arg_offset = 0;

    if (std::string(argv[1]) == "-i") {
        if (argc != 4) { // Case-insensitive requires three additional arguments
            print_usage(argv[0]);
            return 1;
        }
        case_insensitive = true;
        arg_offset = 1;
    }

    const std::string query = argv[1 + arg_offset];
    const std::string path = argv[2 + arg_offset];

    // Perform the search
    try {
        BoyerMoore boyer_moore(query, path, case_insensitive);
        boyer_moore.search();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
