#include "libdocscript/runtime/datatype.h"
#include "libdocscript/exception.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include <string>

namespace libdocscript::runtime {

// +--------------------+
//       Contrcutor
// +--------------------+

DataType::DataType(DataType::Kind k)
  : _kind(k)
{}

// +--------------------+
//    Private Functions
// +--------------------+

DataType*
DataType::rawptr_clone() const
{
    return new DataType(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

DataType::operator bool() const
{
    throw InternalUnimplementException("DataType::operator bool()");
}

DataType::operator std::string() const
{
    throw InternalUnimplementException("DataType::operator string()");
}

// +--------------------+
//    Static Functions
// +--------------------+
std::string
DataType::to_string(DataType::Kind k)
{
    switch (k) {
        case Kind::Unspecific:
            return "unspecific";
        case Kind::Nil:
            return "nil";
        case Kind::Error:
            return "error";
        case Kind::Symbol:
            return "symbol";
        case Kind::Number:
            return "number";
        case Kind::String:
            return "string";
        case Kind::List:
            return "list";
        case Kind::Procedure:
            return "procedure";
        default:
            throw InternalUnimplementException(
              "DataType::to_string(DataType::Kind)");
    }
}
}