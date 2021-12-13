#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {
Value boolean_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();

    return Boolean(first.type() == DataType::Kind::Boolean);
}

Value boolean_eq(args_list args, Environment& env)
{
    if (args.size() < 1) throw UnexceptNumberOfArgument(1, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Boolean)
            throw UnexceptType(DataType::to_string(DataType::Kind::Boolean),
                               DataType::to_string(val.type()));
    }

    const Boolean& result = args.front().c_cast<Boolean>();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (*beg != result) {
            return Boolean(false);
        }
    }

    return result;
}

Value boolean_to_string(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!boolean_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Boolean),
                           DataType::to_string(args.front().type()));

    return String(static_cast<std::string>(args.front()));
}

Value boolean_not(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!boolean_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Boolean),
                           DataType::to_string(args.front().type()));

    return Boolean(!static_cast<bool>(args.front()));
}
} // namespace libdocscript::proc