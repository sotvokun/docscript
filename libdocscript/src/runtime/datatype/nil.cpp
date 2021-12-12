#include "libdocscript/runtime/datatype.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//   Private Functions
// +--------------------+

DataType* Nil::rawptr_clone() const
{
    return new Nil(*this);
}

// +--------------------+
//     Type Conversion
// +--------------------+

Nil::operator bool() const
{
    return false;
}

Nil::operator std::string() const
{
    return "#nil";
}
} // namespace libdocscript::runtime