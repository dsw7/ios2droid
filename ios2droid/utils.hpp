#pragma once

#include <filesystem>
#include <vector>

void print_separator();
std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath);
