#include "utils.hpp"

#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void print_separator()
{
    static std::string separator = std::string(50, '-');
    std::cout << separator << '\n';
}

std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath)
{
    std::vector<unsigned char> buffer;

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open file '{}'", filepath.string()));
    }

    std::streamsize size_file = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size_file);

    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_file))
    {
        throw std::runtime_error(fmt::format("Failed to read file '{}'", filepath.string()));
    }

    return buffer;
}

namespace easyexif_error_codes
{

const int PARSE_EXIF_ERROR_NO_JPEG = 1982;
const int PARSE_EXIF_ERROR_NO_EXIF = 1983;
const int PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN = 1984;
const int PARSE_EXIF_ERROR_CORRUPT = 1985;

} // namespace easyexif_error_codes
