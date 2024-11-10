#pragma once

#include <filesystem>

bool parse_date_taken_from_exif(const std::filesystem::path &filepath);
bool parse_all_data_from_exif(const std::filesystem::path &filepath);
