#include "parse_exif.hpp"

#include "exif.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace
{

std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath)
{
    std::vector<unsigned char> buffer;

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filepath << '\n';
        return buffer;
    }

    std::streamsize size_file = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size_file);

    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_file))
    {
        std::cerr << "Failed to read file: " << filepath << '\n';
    }

    return buffer;
}

} // namespace

bool parse_date_taken_from_exif(const std::filesystem::path &filepath)
{
    std::vector<unsigned char> buffer = load_file_into_buffer(filepath);

    if (buffer.empty())
    {
        return false;
    }

    easyexif::EXIFInfo exif_info;

    int rv = exif_info.parseFrom(buffer.data(), buffer.size());

    if (rv != 0)
    {
        std::cerr << "Error parsing EXIF: code " << rv << '\n';
        return false;
    }

    std::cout << "Date Taken: " << exif_info.DateTimeOriginal << '\n';
    return true;
}
