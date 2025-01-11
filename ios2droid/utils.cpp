#include "utils.hpp"

#include <fmt/core.h>
#include <fstream>
#include <stdexcept>
#include <string>

void print_separator()
{
    static std::string separator = std::string(50, '-');
    fmt::print("{}\n", separator);
}

std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        const std::string errmsg = fmt::format("Failed to open file '{}'", filepath.string());
        throw std::runtime_error(errmsg);
    }

    std::streamsize size_file = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer;
    buffer.resize(size_file);

    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_file)) {
        const std::string errmsg = fmt::format("Failed to read file '{}'", filepath.string());
        throw std::runtime_error(errmsg);
    }

    return buffer;
}
