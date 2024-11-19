# ios2droid
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/dsw7/GPTifier/master/LICENSE)

Image photos originating from iOS devices such as iPhones often have names like:
```
IMG_1728.JPG
```
This naming scheme provides little meaning. Android names image files using a more descriptive scheme:
```
20241113_024948.jpg
```
Which follows `YYYYMMDD_HHMMSS` format. This program renames files from iOS format to Android format by
interpolating Android style timestamps from file EXIF metadata.

## Usage
### Renaming files
First, change directories to wherever the target files are located. Then run:
```console
ios2droid
```
This will perform a dry run. If satisfied with the expected renaming, run:
```console
ios2droid --rename
```
Which will actually rename the files in the current working directory.

### Inspecting a file
Not all files can be renamed. To inspect the raw EXIF metadata associated with a file, simply pass:
```console
ios2droid <foo.jpg>
```
This may provide some additional insight into why a file cannot be renamed.

## Setup
Simply clone this repo, then run:
```console
make compile
```
This program uses [{fmt}](https://github.com/fmtlib/fmt) and this library will need to be installed.
