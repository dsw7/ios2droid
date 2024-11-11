#include "reporting.hpp"

#include <cstdlib>
#include <errno.h>
#include <string.h>

namespace
{
const char *CURRENT_FILE = "CURR_FILE";
}

namespace reporting
{

void set_target(const std::string &filename)
{
    if (setenv(CURRENT_FILE, filename.c_str(), 1) != 0)
    {
        exit(strerror(errno));
    }
}

} // namespace reporting
