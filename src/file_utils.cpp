#include "file_utils.h"

#include <fstream>
#include <sstream>

std::string readTextFile(const std::string& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        return "";
    }

    std::ostringstream content;
    content << input.rdbuf();
    return content.str();
}
