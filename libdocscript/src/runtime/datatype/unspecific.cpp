#include "libdocscript/runtime/datatype.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//   Private Functions
// +--------------------+

DataType* Unspecific::rawptr_clone() const
{
    return new Unspecific(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

Unspecific::operator bool() const
{
    return false;
}

Unspecific::operator std::string() const
{
    return "";
}
} // namespace libdocscript::runtime