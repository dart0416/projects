#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// ANSI color codes for text color
const std::string COLOR_WHITE = "\033[1;37m";    // White
const std::string COLOR_BLUE = "\033[1;34m";     // Blue
const std::string COLOR_CYAN = "\033[1;36m";     // Cyan
const std::string COLOR_RESET = "\033[0m";       // Reset to default
const std::string COLOR_RED = "\033[1;31m";      // Red

void search_directory(const fs::path& path, const std::string& search_string) {
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_directory()) {
            std::string filename = entry.path().filename().string();
            std::string directory_path = entry.path().parent_path().string();

            if (filename.find(search_string) != std::string::npos) {
                std::cout << COLOR_WHITE << directory_path << "/";
                std::cout << COLOR_BLUE << filename << COLOR_RESET << std::endl;
            }
        } else {
            std::string filename = entry.path().filename().string();
            std::string directory_path = entry.path().parent_path().string();

            if (filename.find(search_string) != std::string::npos) {
                std::cout << COLOR_WHITE << directory_path << "/";
                std::cout << COLOR_CYAN << filename << COLOR_RESET << std::endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << COLOR_RED << "Usage: " << argv[0] << " <directory> <search_string>" << COLOR_RESET << std::endl;
        return EXIT_FAILURE;
    }

    std::string path = argv[1];
    std::string search_string = argv[2];

    search_directory(path, search_string);

    return EXIT_SUCCESS;
}

