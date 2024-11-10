#include "utils.hpp"

#include <fstream>
#include <iostream>

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
