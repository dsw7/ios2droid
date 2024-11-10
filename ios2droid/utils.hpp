#pragma once

#include <filesystem>
#include <vector>

std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath);
