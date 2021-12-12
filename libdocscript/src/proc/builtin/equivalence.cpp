#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {

Value equal_type(args_list args, Environment& env)
{
    if (args.size() != 2) throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    return Boolean(first.type() == second.type());
}

Value equal(args_list args, Environment& env)
{
    if (args.size() != 2) throw UnexceptNumberOfArgument(2, args.size());

    if (!equal_type(args, env).c_cast<Boolean>().value()) return Boolean(false);

    auto& first = args.front();
    auto& second = args.back();

    return Boolean(first == second);
}

} // namespace libdocscript::proc