#include "libdocscript/exception.h"
#include "libdocscript/runtime/datatype.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//      Constructor
// +--------------------+

Error::Error(const std::string& msg)
    : DataType(DataType::Kind::Error), _msg(msg)
{}

// +--------------------+
//    Public Functions
// +--------------------+

const std::string& Error::message() const
{
    return _msg;
}

RuntimeError Error::raise() const
{
    return RuntimeError(_msg);
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType* Error::rawptr_clone() const
{
    return new Error(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

Error::operator bool() const
{
    return true;
}

Error::operator std::string() const
{
    return "error: " + _msg;
}

} // namespace libdocscript::runtime