#include "reporting.hpp"

#include <cstdlib>
#include <errno.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <optional>
#include <string.h>
#include <string>

namespace {

const char *TARGET = "CURRENT_FILE";

std::optional<const std::string> get_target()
{
    const char *target = std::getenv(TARGET);

    if (target == nullptr) {
        return std::nullopt;
    }

    return std::string(target);
}

} // namespace

namespace reporting {

void set_target(const std::string &filename)
{
    if (setenv(TARGET, filename.c_str(), 1) != 0) {
        fmt::print(stderr, "{}\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void unset_target()
{
    if (unsetenv(TARGET) != 0) {
        fmt::print(stderr, "{}\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void print_info(const std::string &message)
{
    fmt::print(fg(fmt::terminal_color::green), "[{}] ", get_target().value_or("Unset"));
    fmt::print("{}\n", message);
}

void print_warning(const std::string &message)
{
    fmt::print(stderr, fg(fmt::terminal_color::yellow), "[{}] ", get_target().value_or("Unset"));
    fmt::print(stderr, "{}\n", message);
}

void print_error(const std::string &message)
{
    fmt::print(stderr, fg(fmt::terminal_color::red), "[{}] ", get_target().value_or("Unset"));
    fmt::print(stderr, "{}\n", message);
}

} // namespace reporting
