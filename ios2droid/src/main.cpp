#include <filesystem>
#include <iostream>
#include <string>

void print_build_information()
{
    std::cout << "Project version: " << PROJECT_VERSION << '\n';
    std::cout << "Binary compiled on: " << BUILD_DATE << '\n';
}

void process_directory()
{
    const std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Will rename files in directory: " << cwd << '\n';

    if (std::filesystem::is_empty(cwd))
    {
        std::cout << "Directory is empty!\n";
        return;
    }
}

int main()
{
    print_build_information();
    process_directory();
    return 0;
}
