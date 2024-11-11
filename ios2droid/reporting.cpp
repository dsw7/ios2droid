#include "reporting.hpp"

#include <cstdlib>
#include <errno.h>
#include <fmt/core.h>
#include <iostream>
#include <optional>
#include <string.h>
#include <string>

namespace
{

const char *TARGET = "CURRENT_FILE";

std::optional<const std::string> get_target()
{
    const char *target = std::getenv(TARGET);

    if (target == nullptr)
    {
        return std::nullopt;
    }

    return std::string(target);
}

} // namespace

namespace reporting
{

void set_target(const std::string &filename)
{
    if (setenv(TARGET, filename.c_str(), 1) != 0)
    {
        std::cerr << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void unset_target()
{
    if (unsetenv(TARGET) != 0)
    {
        std::cerr << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void print_to_stdout(const std::string &message)
{
    std::cout << fmt::format("[{}] {}\n", get_target().value_or("Unset"), message);
}

} // namespace reporting
