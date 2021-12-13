#ifndef DOCSIR_COMMANDS_VERSION_H
#define DOCSIR_COMMANDS_VERSION_H
#include "CLI/CLI.hpp"
#include "libdocscript/version.h"
#include <iostream>
#include <sstream>
#include <string>

namespace docsir {

inline std::string version_formatter(const std::string& name, int major,
                                     int minor, int patch)
{
    std::ostringstream oss;
    oss << std::right << std::setw(10);
    oss << name << std::setw(10);
    oss << major << "." << minor << "." << patch;
    return oss.str();
}

inline void print_version(bool verbose = false)
{
    std::cout << version_formatter("docscript", LIBDOCSCRIPT_VERSION_MAJOR,
                                   LIBDOCSCRIPT_VERSION_MINOR,
                                   LIBDOCSCRIPT_VERSION_PATCH)
              << std::endl;
}
} // namespace docsir

#endif