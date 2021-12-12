#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::runtime {

// +--------------------+
//       Constructor
// +--------------------+

BuiltInProcedure::BuiltInProcedure(inner_type func)
  : Procedure(Procedure::BuiltIn)
  , _func(func)
{}

// +--------------------+
//    Public Functions
// +--------------------+

Value
BuiltInProcedure::invoke(const args_list& args, Environment& env) const
{
    Environment subenv = env.derive();
    return _func(args, subenv);
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType*
BuiltInProcedure::rawptr_clone() const
{
    return new BuiltInProcedure(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

BuiltInProcedure::operator std::string() const
{
    return "#builtin-procedure";
}


}