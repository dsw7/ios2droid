#include "rename_files.hpp"
#include "utils.hpp"

#include "exif.h"

#include <filesystem>
#include <fmt/core.h>
#include <iostream>
#include <regex>
#include <vector>

namespace
{

bool parse_date_taken_from_exif(const std::filesystem::path &filepath, std::string &date_taken)
{
    std::vector<unsigned char> buffer = load_file_into_buffer(filepath);

    if (buffer.empty())
    {
        std::cerr << fmt::format("File '{}' is empty. Doing nothing\n", filepath.filename().string());
        return false;
    }

    easyexif::EXIFInfo exif_info;

    bool parse_failed = true;

    switch (exif_info.parseFrom(buffer.data(), buffer.size()))
    {
    case PARSE_EXIF_ERROR_NO_JPEG:
        std::cerr << "No JPEG markers found in buffer. Is this an image file?\n";
        break;
    case PARSE_EXIF_ERROR_NO_EXIF:
        std::cerr << "Could not find EXIF header in file\n";
        break;
    case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
        std::cerr << "Byte alignment specified in EXIF file is unknown\n";
        break;
    case PARSE_EXIF_ERROR_CORRUPT:
        std::cerr << "EXIF header found but data is corrupted\n";
        break;
    default:
        parse_failed = false;
        break;
    }

    if (parse_failed)
    {
        return false;
    }

    date_taken = exif_info.DateTimeOriginal;
    return true;
}

void rename_file(const std::filesystem::path &filepath)
{
    static std::regex pattern("^IMG_\\d+$");

    if (!std::regex_match(filepath.stem().string(), pattern))
    {
        std::cout << fmt::format("File '{}' does not match regex ^IMG_\\d+$. Doing nothing\n",
                                 filepath.filename().string());
        return;
    }

    std::string date_taken;

    if (not parse_date_taken_from_exif(filepath, date_taken))
    {
        return;
    }

    std::cout << "Date Taken: " << date_taken << '\n';
}

} // namespace

void rename_files()
{
    const std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Will rename files in directory: " << cwd << '\n';

    if (std::filesystem::is_empty(cwd))
    {
        std::cout << "Directory is empty!\n";
        return;
    }

    for (auto const &target : std::filesystem::directory_iterator{cwd})
    {
        rename_file(target.path());
    }
}
