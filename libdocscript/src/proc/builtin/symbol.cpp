#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {

Value symbol_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();

    return Boolean(first.type() == DataType::Kind::Symbol);
}

Value symbol_eq(args_list args, Environment& env)
{
    if (args.size() < 1) throw UnexceptNumberOfArgument(1, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Symbol)
            throw UnexceptType(DataType::to_string(DataType::Kind::Symbol),
                               DataType::to_string(val.type()));
    }

    const auto& symbol = args.front().c_cast<Symbol>();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (*beg != symbol) {
            return Boolean(false);
        }
    }
    return Boolean(true);
}

Value symbol_to_string(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!symbol_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Symbol),
                           DataType::to_string(args.front().type()));

    return String(args.front());
}

Value symbol_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (!symbol_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::Symbol),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}

} // namespace libdocscript::proc