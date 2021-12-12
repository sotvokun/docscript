#include "libdocscript/runtime/datatype.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

String::String(const std::string& str)
    : DataType(DataType::Kind::String), _value(str)
{}

// +--------------------+
//    Public Functions
// +--------------------+

std::string& String::value()
{
    return _value;
}

const std::string& String::const_value() const
{
    return _value;
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType* String::rawptr_clone() const
{
    return new String(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

String::operator bool() const
{
    return true;
}

String::operator std::string() const
{
    return _value;
}

} // namespace libdocscript::runtime