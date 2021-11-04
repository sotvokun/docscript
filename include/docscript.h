#ifndef DOCSCRIPT_H
#define DOCSCRIPT_H
#include <cstddef>

namespace docscript {

// Version

static unsigned MajorVersion = 0;
static unsigned MinorVersion = 2;
static unsigned PatchVersion = 0;

// Common type alias

using size_t = std::size_t;
using size_type = std::size_t;

using ds_integer = long;
using ds_float = double;

} // namespace docscript

#endif