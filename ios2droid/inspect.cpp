#include "inspect.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <iostream>

void inspect_file(const std::string &file)
{
    std::filesystem::path filename = file;

    if (not std::filesystem::exists(filename))
    {
        std::cerr << fmt::format("File '{}' does not exist\n", file);
        return;
    }

    if (not std::filesystem::is_regular_file(filename))
    {
        std::cerr << fmt::format("File '{}' is not a regular file\n", file);
        return;
    }
}
