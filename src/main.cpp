#include "search_util.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <query> <path>" << std::endl;
        return 1;
    }

    std::string query = argv[1];
    std::string path = argv[2];

    SearchUtil search_util(query, path);
    search_util.search();

    return 0;
}