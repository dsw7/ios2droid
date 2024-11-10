#include "parse_exif.hpp"

#include <filesystem>
#include <iostream>
#include <regex>
#include <string>

void print_build_information()
{
    std::cout << "Project version: " << PROJECT_VERSION << '\n';
    std::cout << "Binary compiled on: " << BUILD_DATE << '\n';
}

void rename_file(const std::filesystem::path &filepath)
{
    static std::regex pattern("^IMG_\\d+$");

    if (!std::regex_match(filepath.stem().string(), pattern))
    {
        return;
    }

    std::cout << filepath << '\n';
    parse_date_taken_from_exif(filepath);
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
