#include "libdocscript/runtime/datatype.h"

namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

Boolean::Boolean(bool b) : DataType(DataType::Kind::Boolean), _value(b) {}

// +--------------------+
//   Type Conversion
// +--------------------+

Boolean::operator bool() const
{
    return _value;
}

Boolean::operator std::string() const
{
    return _value ? "#t" : "#f";
}

// +--------------------+
//    Public Functions
// +--------------------+

bool Boolean::value() const
{
    return _value;
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType* Boolean::rawptr_clone() const
{
    return new Boolean(*this);
}

} // namespace libdocscript::runtime