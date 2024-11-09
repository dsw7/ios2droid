#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "exif.h"

void print_build_information()
{
    std::cout << "Project version: " << PROJECT_VERSION << '\n';
    std::cout << "Binary compiled on: " << BUILD_DATE << '\n';
}

bool parse_exif_data(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filepath << '\n';
        return false;
    }

    std::streamsize size_file = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer;
    buffer.resize(size_file);

    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_file))
    {
        std::cerr << "Failed to read file: " << filepath << '\n';
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

void rename_file(const std::filesystem::path &filepath)
{
    static std::regex pattern("^IMG_\\d+$");

    if (!std::regex_match(filepath.stem().string(), pattern))
    {
        return;
    }

    std::cout << filepath << '\n';
    parse_exif_data(filepath);
}

void process_directory()
{
    const std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Will rename files in directory: " << cwd << '\n';

    if (std::filesystem::is_empty(cwd))
    {
        std::cout << "Directory is empty!\n";
        return;
    }

    for (auto const &target : std::filesystem::directory_iterator{cwd})
    {
        rename_file(target.path());
    }
}

int main()
{
    print_build_information();
    process_directory();
    return 0;
}
