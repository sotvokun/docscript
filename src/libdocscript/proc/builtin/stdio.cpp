#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/procedure.h"
#include <iostream>
#include <string>

namespace libdocscript::proc {

Value stdio_stdout_write(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    std::cout << static_cast<std::string>(args.front());
    return String(static_cast<std::string>(args.front()));
}

Value stdio_stdin_read_line(args_list args, Environment& env)
{
    if (args.size() != 0) throw UnexceptNumberOfArgument(0, args.size());

    std::string content;
    if (std::cin) {
        std::getline(std::cin, content);
        return String(content);
    }
    else {
        return Boolean(false);
    }
}

Value stdio_stdin_status(args_list args, Environment& env)
{
    if (args.size() != 0) throw UnexceptNumberOfArgument(0, args.size());

    if (std::cin.good()) {
        return Symbol("good");
    }
    else if (std::cin.eof()) {
        return Symbol("eof");
    }
    else {
        return Symbol("bad");
    }
}

Value stdio_stdin_status_to_boolean(args_list args, Environment& env)
{
    if (args.size() == 1) {
        if (args.front().type() != DataType::Kind::Symbol)
            throw UnexceptType(DataType::to_string(DataType::Kind::Symbol),
                               DataType::to_string(args.front().type()));

        const auto& symbol = args.front().c_cast<Symbol>();
        if (symbol.const_content() == "good") {
            return Boolean(true);
        }
        else {
            return Boolean(false);
        }
    }
    else if (args.size() == 0) {
        const auto& symbol = stdio_stdin_status(args, env).c_cast<Symbol>();
        if (symbol.const_content() == "good") {
            return Boolean(true);
        }
        else {
            return Boolean(false);
        }
    }
    else {
        throw UnexceptNumberOfArgument(1, args.size());
    }
}

} // namespace libdocscript::proc