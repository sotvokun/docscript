#include "docsir/stdproc_ext.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/stdproc.h"
#include "docsir/value.h"
#include "docsir/environment.h"
#include "docsir/interpreter.h"
#include "libdocscript/parser.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace docscript::docsir::stdproc {

// IO

Value write_file(const argument_type &args) { 
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);

    const auto &path = args[0].cast<String>().value();
    const auto &content = args[1].cast<String>().value();

    try {
        std::ofstream ofs(path, std::ios::trunc);
        ofs << content;
        ofs.close();
        return Value::Boolean(true);
    } catch(...) {
        return Value::Boolean(false);
    }
}

Value read_file(const argument_type &args) { 
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);

    const auto &path = args[0].cast<String>().value();

    try{
        if (!std::filesystem::exists(path)) {
            return Value::List();
        }
        std::ifstream ifs(path);
        std::ostringstream oss;
        oss << ifs.rdbuf();
        ifs.close();
        return Value::String(oss.str());
    } catch(...) {
        return Value::List();
    }
}

Value message(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &arg = args[0];
    std::string result;
    switch (arg.type()) {
    case ValueType::Boolean:
        result = stdproc::boolean_to_string(args).cast<String>().value();
        break;
    case ValueType::List:
        result = stdproc::list_to_string(args).cast<String>().value();
        break;
    case ValueType::Number:
        result = stdproc::number_to_string(args).cast<String>().value();
        break;
    case ValueType::Procedure:
        result = "#procedure";
        break;
    case ValueType::String:
        result = arg.cast<String>().value();
        break;
    case ValueType::Symbol:
        result = stdproc::symbol_to_string(args).cast<String>().value();
        break;
    default:
        throw InterpreterException("Internal Error, Bad Status");
    }
    std::printf("%s\n", result.c_str());
    return Value::String(result);
}

// Environment

BuiltInProcedure::function_type privilege_load(Environment &env) {
    return [&](const argument_type &args)->Value {
        handle_arguments_size(args, 1);
        handle_argument_type(args[0], ValueType::String);

        auto readresult = read_file(args);
        if(readresult.type() == ValueType::List) {
            return Value::List();
        }
        const auto &content = readresult.cast<String>().value();
        libdocscript::Parser parser;
        Interpreter interpreter(env);
        auto tree = parser.parse(content);
        for(const auto &i : tree) {
            interpreter.eval(i);
        }
        return env.to_value();
    };
}

// Interpretation

Value error(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);
    throw CustomErrorException(args[0].cast<String>().value());
}

Value exit(const argument_type &args) {
    if (args.size() == 0) {
        std::exit(0);
    } else {
        handle_argument_type(args[0], ValueType::Number);
        std::exit(args[0].cast<Number>().value_int());
    }
}
} // namespace docscript::docsir::stdproc