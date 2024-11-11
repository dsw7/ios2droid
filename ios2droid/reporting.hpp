#pragma once

#include <string>

namespace reporting
{

void set_target(const std::string &filename);
void unset_target();

void print_info(const std::string &message);
void print_warning(const std::string &message);
void print_error(const std::string &message);

} // namespace reporting
