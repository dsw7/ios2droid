#include "parse_exif.hpp"

#include "exif.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace
{

std::vector<unsigned char> load_file_into_buffer(const std::filesystem::path &filepath)
{
    std::vector<unsigned char> buffer;

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filepath << '\n';
        return buffer;
    }

    std::streamsize size_file = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size_file);

    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_file))
    {
        std::cerr << "Failed to read file: " << filepath << '\n';
    }

    return buffer;
}

} // namespace

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

bool parse_all_data_from_exif(const std::filesystem::path &filepath)
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

    std::cout << std::string(50, '-') << '\n';
    std::cout << "Filename             : " << filepath.filename() << '\n';
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
    std::cout << std::string(50, '-') << '\n';

    return true;
}
