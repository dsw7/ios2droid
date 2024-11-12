#include "help.hpp"

#include <iostream>

namespace
{

void print_build_information()
{
    std::cout << "iOS2Droid version: " << PROJECT_VERSION << '\n';
    std::cout << "Compiled on: " << BUILD_DATE << '\n';
}

void print_help_messages()
{
    std::cout << "Usage: ios2droid [OPTIONS] [FILE]\n\n"
                 "Options:\n"
                 "  -h, --help     Show this help message and exit\n"
                 "  -r, --rename   Rename files of iOS origin\n\n"
                 "Arguments:\n"
                 "  FILE           The file to inspect (optional)\n\n"
                 "Description:\n"
                 "  - Run `ios2droid -h` or `ios2droid --help` to display this help message.\n"
                 "  - Run `ios2droid FILE` to inspect EXIF metadata associated with a file.\n"
                 "  - Run `ios2droid` with no arguments rename all files in current directory.\n";
}

} // namespace

void print_summary()
{
    print_build_information();
    print_help_messages();
}
