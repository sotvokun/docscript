#include "libdocscript/interpreter.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"


namespace libdocscript::runtime {

// +--------------------+
//       Constructor
// +--------------------+

LambdaProcedure::LambdaProcedure(const parm_list& parameters, func_body body)
    : Procedure(Procedure::Lambda), _parameters(parameters), _expression(body)
{}

// +--------------------+
//    Public Functions
// +--------------------+

Value LambdaProcedure::invoke(const args_list& args, Environment& env) const
{
    if (_parameters.size() != args.size()) {
        throw UnexceptNumberOfArgument(_parameters.size(), args.size());
    }
    Environment subenv = env.derive();
    for (decltype(_parameters.size()) i = 0; i != _parameters.size(); ++i) {
        subenv.set<Value>(_parameters[i], args[i]);
    }
    return Interpreter(subenv).eval(_expression);
}

// +--------------------+
//    Private Functions
// +--------------------+

DataType* LambdaProcedure::rawptr_clone() const
{
    return new LambdaProcedure(*this);
}

// +--------------------+
//    Type Conversion
// +--------------------+

LambdaProcedure::operator std::string() const
{
    return "#lambda-procedure";
}

} // namespace libdocscript::runtime