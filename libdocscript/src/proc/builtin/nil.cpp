#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {
Value nil_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    auto first = args.front();

    return Boolean(first.type() == DataType::Kind::Nil);
}

Value nil_to_string(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!nil_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Nil),
                           DataType::to_string(args.front().type()));

    return String(args.front());
}

Value nil_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!nil_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Nil),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}
} // namespace libdocscript::proc