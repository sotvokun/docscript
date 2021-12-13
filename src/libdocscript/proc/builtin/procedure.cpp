#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {
Value procedure_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    auto first = args.front();

    return Boolean(first.type() == DataType::Kind::Procedure);
}

Value procedure_to_string(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!procedure_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Procedure),
                           DataType::to_string(args.front().type()));

    return String(args.front());
}

Value procedure_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!procedure_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Procedure),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}

Value procedure_apply(args_list args, Environment& env)
{
    if (args.size() != 2) throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::Procedure)
        throw UnexceptType(DataType::to_string(DataType::Kind::Procedure),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::List)
        throw UnexceptType(DataType::to_string(DataType::Kind::Procedure),
                           DataType::to_string(second.type()));

    return first.c_cast<Procedure>().invoke(second.c_cast<List>().craw(), env);
}
} // namespace libdocscript::proc