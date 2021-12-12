#include "libdocscript/runtime/procedure.h"
#include "libdocscript/exception.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/value.h"
#include <string>

namespace libdocscript::runtime {
// +--------------------+
//      Constructor
// +--------------------+

Procedure::Procedure(Type t)
  : DataType(DataType::Kind::Procedure)
  , _type(t)
{}

// +--------------------+
//    Public Functions
// +--------------------+

Value
Procedure::invoke(const args_list& args, Environment& env) const
{
    throw InternalUnimplementException("Procedure::invoke()");
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType*
Procedure::rawptr_clone() const
{
    return new Procedure(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

Procedure::operator bool() const
{
    return true;
}

Procedure::operator std::string() const
{
    return "#procedure";
}

}