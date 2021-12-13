#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/parser.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/scanner.h"
#include "libdocscript/utility/stringstream.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace libdocscript::proc {

Value load_(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    // TODO
    throw InternalUnimplementException(
        "proc::load_(), environment does not know which mode are running");
}

Value load_script(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    // TODO
    throw InternalUnimplementException(
        "proc::load_script(), environment does not know which mode are running");
}

} // namespace libdocscript::proc