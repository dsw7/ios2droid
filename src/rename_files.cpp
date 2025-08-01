#include "rename_files.hpp"
#include "reporting.hpp"
#include "utils.hpp"

#include "exif.h"

#include <algorithm>
#include <filesystem>
#include <fmt/core.h>
#include <vector>

namespace {

struct Payload {
    std::string date_taken;
    std::string errmsg;
    std::string make;
};

std::string convert_ios_to_android_datefmt(const std::string &date_ios)
{
    std::string date_android = date_ios;

    date_android.erase(std::remove(date_android.begin(), date_android.end(), ':'), date_android.end());
    std::replace(date_android.begin(), date_android.end(), ' ', '_');

    return date_android;
}

bool parse_date_taken_from_exif(const std::filesystem::path &filepath, Payload &payload)
{
    const std::vector<unsigned char> buffer = load_file_into_buffer(filepath);

    if (buffer.empty()) {
        payload.errmsg = "Skipping file. File is empty";
        return false;
    }

    easyexif::EXIFInfo exif_info;

    bool parse_failed = true;

    switch (exif_info.parseFrom(buffer.data(), buffer.size())) {
        case PARSE_EXIF_ERROR_NO_JPEG:
            payload.errmsg = "No JPEG markers found in buffer. Is this an image file?";
            break;
        case PARSE_EXIF_ERROR_NO_EXIF:
            payload.errmsg = "Could not find EXIF header in file";
            break;
        case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
            payload.errmsg = "Byte alignment specified in EXIF file is unknown";
            break;
        case PARSE_EXIF_ERROR_CORRUPT:
            payload.errmsg = "EXIF header found but data is corrupted";
            break;
        default:
            parse_failed = false;
            break;
    }

    if (parse_failed) {
        return false;
    }

    payload.date_taken = exif_info.DateTimeOriginal;
    payload.make = exif_info.Make;

    return true;
}

void rename_file(const std::filesystem::path &old_file, const std::filesystem::path &new_file)
{
    if (std::filesystem::equivalent(old_file, new_file)) {
        reporting::print_warning("Filename is already in YYYYMMDD_HHMMSS format");
        return;
    }

    try {
        if (std::filesystem::exists(new_file)) {
            reporting::print_warning(fmt::format("'{1}' already exists. Therefore cannot rename '{0}' to '{1}'",
                old_file.filename().string(), new_file.string()));
        } else {
            std::filesystem::rename(old_file, new_file);
            reporting::print_info(fmt::format("-> {}", new_file.string()));
        }
    } catch (const std::filesystem::filesystem_error &e) {
        reporting::print_error(e.what());
    }
}

void process_file(const std::filesystem::path &old_file, bool is_dry_run)
{
    if (not std::filesystem::is_regular_file(old_file)) {
        if (std::filesystem::is_directory(old_file)) {
            reporting::print_error("Is a directory");
        } else {
            reporting::print_error("Not a regular file");
        }
        return;
    }

    Payload payload;

    if (not parse_date_taken_from_exif(old_file, payload)) {
        reporting::print_warning(payload.errmsg);
        return;
    }

    if (payload.make != "Apple") {
        reporting::print_warning("File did not originate from iOS device. Skipping file");
        return;
    }

    const std::string android_fmt = convert_ios_to_android_datefmt(payload.date_taken);
    const std::string new_file = fmt::format("{}{}", android_fmt, old_file.extension().string());

    if (is_dry_run) {
        reporting::print_info(fmt::format("-> {}", new_file));
        return;
    }

    rename_file(old_file, new_file);
}

} // namespace

void rename_files(bool is_dry_run)
{
    print_separator();
    const std::filesystem::path cwd = std::filesystem::current_path();

    if (is_dry_run) {
        fmt::print("> This is a dry run!\n");
        fmt::print("> Will NOT rename files in directory: {}\n", cwd.string());
        fmt::print("> Run program with -r or --rename to rename files\n");
    } else {
        fmt::print("> Renaming files in directory: {}\n", cwd.string());
    }

    print_separator();

    if (std::filesystem::is_empty(cwd)) {
        fmt::print(stderr, "Directory is empty!\n");
        return;
    }

    for (auto const &target: std::filesystem::directory_iterator { cwd }) {
        reporting::set_target(target.path().filename().string());
        process_file(target.path(), is_dry_run);
        reporting::unset_target();
    }
}
