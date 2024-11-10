#include "parse_exif.hpp"
#include "utils.hpp"

#include "exif.h"

#include <iostream>
#include <vector>

bool parse_date_taken_from_exif(const std::filesystem::path &filepath)
{
    std::vector<unsigned char> buffer = load_file_into_buffer(filepath);

    if (buffer.empty())
    {
        return false;
    }

    easyexif::EXIFInfo exif_info;

    int rv = exif_info.parseFrom(buffer.data(), buffer.size());

    if (rv != 0)
    {
        std::cerr << "Error parsing EXIF: code " << rv << '\n';
        return false;
    }

    std::cout << "Date Taken: " << exif_info.DateTimeOriginal << '\n';
    return true;
}
