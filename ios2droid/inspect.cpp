#include "inspect.hpp"
#include "utils.hpp"

#include "exif.h"

#include <filesystem>
#include <fmt/core.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{

void parse_all_exif_metadata(const std::vector<unsigned char> &buffer, const std::string &filename)
{
    easyexif::EXIFInfo exif_info;

    switch (exif_info.parseFrom(buffer.data(), buffer.size()))
    {
    case PARSE_EXIF_ERROR_NO_JPEG:
        throw std::runtime_error("No JPEG markers found in buffer. Is this an image file?");
    case PARSE_EXIF_ERROR_NO_EXIF:
        throw std::runtime_error("Could not find EXIF header in file");
    case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
        throw std::runtime_error("Byte alignment specified in EXIF file is unknown");
    case PARSE_EXIF_ERROR_CORRUPT:
        throw std::runtime_error("EXIF header found but data is corrupted");
    }

    print_separator();
    std::cout << "Filename             : " << filename << '\n';
    std::cout << "Camera make          : " << exif_info.Make << '\n';
    std::cout << "Camera model         : " << exif_info.Model << '\n';
    std::cout << "Software             : " << exif_info.Software << '\n';
    std::cout << "Bits per sample      : " << exif_info.BitsPerSample << '\n';
    std::cout << "Image width          : " << exif_info.ImageWidth << '\n';
    std::cout << "Image height         : " << exif_info.ImageHeight << '\n';
    std::cout << "Image description    : " << exif_info.ImageDescription << '\n';
    std::cout << "Image orientation    : " << exif_info.Orientation << '\n';
    std::cout << "Image copyright      : " << exif_info.Copyright << '\n';
    std::cout << "Image date/time      : " << exif_info.DateTime << '\n';
    std::cout << "Original date/time   : " << exif_info.DateTimeOriginal << '\n';
    std::cout << "Digitize date/time   : " << exif_info.DateTimeDigitized << '\n';
    std::cout << "Subsecond time       : " << exif_info.SubSecTimeOriginal << '\n';
    std::cout << "Exposure time        : 1/" << (unsigned)(1.0 / exif_info.ExposureTime) << " s\n";
    std::cout << "F-stop               : f/" << exif_info.FNumber << '\n';
    std::cout << "Exposure program     : " << exif_info.ExposureProgram << '\n';
    std::cout << "ISO speed            : " << exif_info.ISOSpeedRatings << '\n';
    std::cout << "Subject distance     : " << exif_info.SubjectDistance << " m\n";
    std::cout << "Exposure bias        : " << exif_info.ExposureBiasValue << " EV\n";
    std::cout << "Flash used?          : " << exif_info.Flash << '\n';
    std::cout << "Flash returned light : " << exif_info.FlashReturnedLight << '\n';
    std::cout << "Flash mode           : " << exif_info.FlashMode << '\n';
    std::cout << "Metering mode        : " << exif_info.MeteringMode << '\n';
    std::cout << "Lens focal length    : " << exif_info.FocalLength << " mm\n";
    std::cout << "35mm focal length    : " << exif_info.FocalLengthIn35mm << " mm\n";
    std::cout << "Lens min focal length: " << exif_info.LensInfo.FocalLengthMin << " mm\n";
    std::cout << "Lens max focal length: " << exif_info.LensInfo.FocalLengthMax << " mm\n";
    std::cout << "Lens f-stop min      : f/" << exif_info.LensInfo.FStopMin << '\n';
    std::cout << "Lens f-stop max      : f/" << exif_info.LensInfo.FStopMax << '\n';
    std::cout << "Lens make            : " << exif_info.LensInfo.Make << '\n';
    std::cout << "Lens model           : " << exif_info.LensInfo.Model << '\n';
    std::cout << "Focal plane XRes     : " << exif_info.LensInfo.FocalPlaneXResolution << '\n';
    std::cout << "Focal plane YRes     : " << exif_info.LensInfo.FocalPlaneYResolution << '\n';
    print_separator();
}

} // namespace

void inspect_file(const std::string &file_s)
{
    std::filesystem::path file_p = file_s;

    if (not std::filesystem::exists(file_p))
    {
        throw std::runtime_error(fmt::format("File '{}' does not exist", file_s));
    }

    if (not std::filesystem::is_regular_file(file_p))
    {
        throw std::runtime_error(fmt::format("File '{}' is not a regular file", file_s));
    }

    std::vector<unsigned char> buffer = load_file_into_buffer(file_p);

    if (buffer.empty())
    {
        throw std::runtime_error("File is empty");
    }

    parse_all_exif_metadata(buffer, file_s);
}
