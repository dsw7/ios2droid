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
    printf("Camera make          : %s\n", exif_info.Make.c_str());
    printf("Camera model         : %s\n", exif_info.Model.c_str());
    printf("Software             : %s\n", exif_info.Software.c_str());
    printf("Bits per sample      : %d\n", exif_info.BitsPerSample);
    printf("Image width          : %d\n", exif_info.ImageWidth);
    printf("Image height         : %d\n", exif_info.ImageHeight);
    printf("Image description    : %s\n", exif_info.ImageDescription.c_str());
    printf("Image orientation    : %d\n", exif_info.Orientation);
    printf("Image copyright      : %s\n", exif_info.Copyright.c_str());
    printf("Image date/time      : %s\n", exif_info.DateTime.c_str());
    printf("Original date/time   : %s\n", exif_info.DateTimeOriginal.c_str());
    printf("Digitize date/time   : %s\n", exif_info.DateTimeDigitized.c_str());
    printf("Subsecond time       : %s\n", exif_info.SubSecTimeOriginal.c_str());
    printf("Exposure time        : 1/%d s\n", (unsigned)(1.0 / exif_info.ExposureTime));
    printf("F-stop               : f/%.1f\n", exif_info.FNumber);
    printf("Exposure program     : %d\n", exif_info.ExposureProgram);
    printf("ISO speed            : %d\n", exif_info.ISOSpeedRatings);
    printf("Subject distance     : %f m\n", exif_info.SubjectDistance);
    printf("Exposure bias        : %f EV\n", exif_info.ExposureBiasValue);
    printf("Flash used?          : %d\n", exif_info.Flash);
    printf("Flash returned light : %d\n", exif_info.FlashReturnedLight);
    printf("Flash mode           : %d\n", exif_info.FlashMode);
    printf("Metering mode        : %d\n", exif_info.MeteringMode);
    printf("Lens focal length    : %f mm\n", exif_info.FocalLength);
    printf("35mm focal length    : %u mm\n", exif_info.FocalLengthIn35mm);
    printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n", exif_info.GeoLocation.Latitude,
           exif_info.GeoLocation.LatComponents.degrees, exif_info.GeoLocation.LatComponents.minutes,
           exif_info.GeoLocation.LatComponents.seconds, exif_info.GeoLocation.LatComponents.direction);
    printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n", exif_info.GeoLocation.Longitude,
           exif_info.GeoLocation.LonComponents.degrees, exif_info.GeoLocation.LonComponents.minutes,
           exif_info.GeoLocation.LonComponents.seconds, exif_info.GeoLocation.LonComponents.direction);
    printf("GPS Altitude         : %f m\n", exif_info.GeoLocation.Altitude);
    printf("GPS Precision (DOP)  : %f\n", exif_info.GeoLocation.DOP);
    printf("Lens min focal length: %f mm\n", exif_info.LensInfo.FocalLengthMin);
    printf("Lens max focal length: %f mm\n", exif_info.LensInfo.FocalLengthMax);
    printf("Lens f-stop min      : f/%.1f\n", exif_info.LensInfo.FStopMin);
    printf("Lens f-stop max      : f/%.1f\n", exif_info.LensInfo.FStopMax);
    printf("Lens make            : %s\n", exif_info.LensInfo.Make.c_str());
    printf("Lens model           : %s\n", exif_info.LensInfo.Model.c_str());
    printf("Focal plane XRes     : %f\n", exif_info.LensInfo.FocalPlaneXResolution);
    printf("Focal plane YRes     : %f\n", exif_info.LensInfo.FocalPlaneYResolution);
    std::cout << std::string(50, '-') << '\n';

    return true;
}
