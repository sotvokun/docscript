#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {
Value
error_is(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();

    return Boolean(first.type() == DataType::Kind::Error);
}

Value
error_to_string(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!error_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Error),
                           DataType::to_string(args.front().type()));

    return String(args.front());
}

Value
error_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!error_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Error),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}

Value
error_(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Error(args.front().c_cast<String>());
}
}