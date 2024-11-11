#pragma once

#include <string>

namespace reporting
{

void set_target(const std::string &filename);
void unset_target();

void print_to_stdout(const std::string &message);

} // namespace reporting
