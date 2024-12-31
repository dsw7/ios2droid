#include "help.hpp"
#include "utils.hpp"

#include <iostream>

namespace {

void print_build_information()
{
    std::cout << "iOS2Droid version: " << PROJECT_VERSION << '\n';
    std::cout << "Compiled on: " << BUILD_DATE << '\n';
    print_separator();
}

void print_help_messages()
{
    std::cout
        << "Usage: ios2droid [-h | --help] [-r | --rename] [FILE]\n\n"
           "Options:\n"
           "  -h, --help     Show this help message and exit\n"
           "  -r, --rename   Rename files of iOS origin\n\n"
           "Arguments:\n"
           "  FILE           The file to inspect (optional)\n\n"
           "Examples:\n"
           "  - \033[2mios2droid FILE\033[0m\n"
           "     > To inspect EXIF metadata associated with a file\n"
           "  - \033[2mios2droid\033[0m (no arguments)\n"
           "     > Dry run - will print out which files would have been renamed in the current working directory\n"
           "  - \033[2mios2droid [-r | --rename]\033[0m\n"
           "     > Rename files in current working directory if satisfied with dry run\n\n";
}

} // namespace

void print_summary()
{
    print_build_information();
    print_help_messages();
}
