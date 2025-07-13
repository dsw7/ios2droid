cmake_minimum_required(VERSION 3.18)
project(ios2droid VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

string(TIMESTAMP BUILD_DATE)
message(STATUS "Will add the following timestamp to binary: ${BUILD_DATE}")

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}")
add_compile_definitions(BUILD_DATE="${BUILD_DATE}")

add_compile_options(-Wall -Wextra -pedantic -Werror)

message(STATUS "Locating {fmt} library")
find_package(fmt)

if(NOT fmt_FOUND)
  message(FATAL_ERROR "Is the {fmt} package installed?")
endif()

message(STATUS "Checking for EasyEXIF source files")

set(OUTPUT_CPP "${CMAKE_SOURCE_DIR}/external/exif.cpp")
set(OUTPUT_H "${CMAKE_SOURCE_DIR}/external/exif.h")

if(NOT EXISTS ${OUTPUT_CPP})
  message(STATUS "Downloading exif.cpp from EasyEXIF source")
  set(URL_CPP "https://raw.githubusercontent.com/mayanklahiri/easyexif/refs/heads/master/exif.cpp")
  file(DOWNLOAD ${URL_CPP} ${OUTPUT_CPP} SHOW_PROGRESS)
else()
  message(STATUS "exif.cpp already exists, skipping download")
endif()

if(NOT EXISTS ${OUTPUT_H})
  message(STATUS "Downloading exif.h from EasyEXIF source")
  set(URL_H "https://raw.githubusercontent.com/mayanklahiri/easyexif/refs/heads/master/exif.h")
  file(DOWNLOAD ${URL_H} ${OUTPUT_H} SHOW_PROGRESS)
else()
  message(STATUS "exif.h already exists, skipping download")
endif()

include_directories(external)
add_executable(
  ios2droid
  main.cpp
  inspect.cpp
  external/exif.cpp
  rename_files.cpp
  reporting.cpp
  utils.cpp
)
target_link_libraries(ios2droid fmt::fmt)

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local")
install(TARGETS ios2droid DESTINATION bin)
