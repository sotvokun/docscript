#ifndef DOCSIR_COMMANDS_VERSION_H
#define DOCSIR_COMMANDS_VERSION_H
#include "cli11/CLI11.hpp"
#include "docsir/version.h"
#include "libdocscript/version.h"
#include <iostream>
#include <sstream>
#include <string>

namespace docsir {

inline std::string
version_formatter(const std::string& name, int major, int minor, int patch)
{
    std::ostringstream oss;
    oss << std::right << std::setw(10);
    oss << name << std::setw(10);
    oss << major << "." << minor << "." << patch;
    return oss.str();
}

inline void
print_version(bool verbose = false)
{
    std::cout << version_formatter("docscript",
                                   LIBDOCSCRIPT_VERSION_MAJOR,
                                   LIBDOCSCRIPT_VERSION_MINOR,
                                   LIBDOCSCRIPT_VERSION_PATCH)
              << std::endl;

    if (verbose) {
        std::cout << std::endl;
        std::cout << version_formatter("docsir",
                                       DOCSIR_VERSION_MAJOR,
                                       DOCSIR_VERSION_MINOR,
                                       DOCSIR_VERSION_PATCH)
                  << std::endl;

        std::cout << version_formatter("*CLI11",
                                       CLI11_VERSION_MAJOR,
                                       CLI11_VERSION_MINOR,
                                       CLI11_VERSION_PATCH)
                  << std::endl;

        std::cout << std::endl;
        std::cout << "* The component of docsir or libdocscript" << std::endl;
    }
}
}

#endif