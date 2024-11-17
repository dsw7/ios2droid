#include "help.hpp"
#include "inspect.hpp"
#include "rename_files.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

void process_input(const std::optional<std::string> &option)
{
    if (not option.has_value()) {
        rename_files(true);
        return;
    }

    std::string_view file_or_opt = option.value();

    if (file_or_opt == "--help") {
        print_summary();
    } else if (file_or_opt == "-h") {
        print_summary();
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
