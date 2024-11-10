#include "help.hpp"
#include "inspect.hpp"
#include "rename_files.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        rename_files();
        return 0;
    }

    std::string arg_1 = argv[1];

    if (arg_1.compare("--help") == 0 or arg_1.compare("-h") == 0)
    {
        print_summary();
        return 0;
    }

    try
    {
        inspect_file(arg_1);
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
