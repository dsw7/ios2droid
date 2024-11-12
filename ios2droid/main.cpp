#include "help.hpp"
#include "inspect.hpp"
#include "rename_files.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

void process_input(const std::optional<std::string> &option)
{
    if (not option.has_value())
    {
        rename_files();
        return;
    }

    if (option.value().compare("--help") == 0 or option.value().compare("-h") == 0)
    {
        print_summary();
        return;
    }

    inspect_file(option.value());
}

int main(int argc, char **argv)
{
    std::optional<std::string> option = std::nullopt;

    if (argc > 1)
    {
        option = argv[1];
    }

    try
    {
        process_input(option);
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
