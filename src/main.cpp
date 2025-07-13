#include "inspect.hpp"
#include "rename_files.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

void print_help_messages()
{
    std::cout << "-- iOS2Droid | v" << PROJECT_VERSION << '\n';
    std::cout << "-- Site: https://github.com/dsw7/ios2droid\n";

    const std::string messages = R"(
Rename iOS originating photos:
  IMG_XXXX.JPG -> YYYYMMDD_HHMMSS.jpg

Program also offers a means of inspecting the EXIF metadata
associated with a photo.

Usage:
  ios2droid [OPTION]
  ios2droid FILE

Options:
  -h, --help    Show this help message and exit
  -r, --rename  Rename files of iOS origin

Arguments:
  FILE          The file to inspect (optional)

Examples:
  1. Run a dry run. Will not actually rename files:
    $ ios2droid (no arguments)
  2. Rename files in current working directory if satisfied with dry run:
    $ ios2droid -r
  3. To inspect EXIF metadata associated with a file:
    $ ios2droid FILE
)";

    std::cout << messages << '\n';
}

void process_input(const std::optional<std::string> &option)
{
    if (not option.has_value()) {
        rename_files(true);
        return;
    }

    const std::string file_or_opt = option.value();

    if (file_or_opt == "--help") {
        print_help_messages();
    } else if (file_or_opt == "-h") {
        print_help_messages();
    } else if (file_or_opt == "--rename") {
        rename_files(false);
    } else if (file_or_opt == "-r") {
        rename_files(false);
    } else {
        inspect_file(file_or_opt);
    }
}

int main(int argc, char **argv)
{
    std::optional<std::string> option = std::nullopt;

    if (argc > 1) {
        option = argv[1];
    }

    try {
        process_input(option);
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
