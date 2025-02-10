#include "search_util.h"
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

    SearchUtil search_util(query, path, case_insensitive);
    search_util.search();

    return 0;
}
