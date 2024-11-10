#include "help.hpp"
#include "parse_exif.hpp"

#include <filesystem>
#include <iostream>
#include <regex>
#include <string>

void rename_file(const std::filesystem::path &filepath)
{
    static std::regex pattern("^IMG_\\d+$");

    if (!std::regex_match(filepath.stem().string(), pattern))
    {
        return;
    }

    std::cout << filepath << '\n';
    parse_all_data_from_exif(filepath);
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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        process_directory();
        return 0;
    }

    std::string arg_1 = argv[1];

    if (arg_1.compare("--help") == 0 or arg_1.compare("-h") == 0)
    {
        print_summary();
    }

    return 0;
}
