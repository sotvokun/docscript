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

    std::string pwd = env.get<Value>("__CURRENT_PATH").c_cast<String>().const_value();
    std::string script_path = args.front().c_cast<String>().const_value();
    std::string full_path = pwd + "/" + script_path;
    if(std::filesystem::exists(full_path)) {
        std::ifstream file(full_path); 
        std::ostringstream oss;
        oss << file.rdbuf();
        StringStream ss(oss.str());
        auto expressions = Parser::parse(ss, env);
        for(const auto &e : expressions) {
            Interpreter(env).eval(e);
        }
    }
    else {
        throw RuntimeError("Cannot found the file at " + full_path);
    }
    return Unspecific();
}

Value load_script(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    std::string pwd = env.get<Value>("__CURRENT_PATH").c_cast<String>().const_value();
    if(env.find<Value>("__FILE_PATH")) {
        pwd = env.get<Value>("__FILE_PATH").c_cast<String>().const_value();
        pwd = std::filesystem::path(pwd).remove_filename().string();
    }
    std::string script_path = args.front().c_cast<String>().const_value();
    std::string full_path = pwd + script_path;
    if(std::filesystem::exists(full_path)) {
        std::ifstream file(full_path); 
        std::ostringstream oss;
        oss << file.rdbuf();
        StringStream ss(oss.str());
        auto expressions = Parser::parse(ss, env);
        for(const auto &e : expressions) {
            Interpreter(env).eval(e);
        }
    }
    else {
        throw RuntimeError("Cannot found the file at " + full_path);
    }
    return Unspecific();
}

} // namespace libdocscript::proc