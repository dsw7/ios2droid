#include "help.hpp"
#include "inspect.hpp"
#include "rename_files.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

void process_input(const std::optional<std::string> &option)
{
    bool is_dry_run = true;

    if (not option.has_value()) {
        rename_files(is_dry_run);
        return;
    }

    std::string file_or_opt = option.value();

    if (file_or_opt.compare("--help") == 0 or file_or_opt.compare("-h") == 0) {
        print_summary();
        return;
    }

    if (file_or_opt.compare("--rename") == 0 or file_or_opt.compare("-r") == 0) {
        is_dry_run = false;
        rename_files(is_dry_run);
        return;
    }

    inspect_file(file_or_opt);
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
