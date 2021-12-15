#ifndef DOCSIR_COMMANDS_EVAL_H
#define DOCSIR_COMMANDS_EVAL_H
#include "docsir/utility.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/parser.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/utility/stringstream.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace docsir {
class EVAL final
{
public:
    EVAL(const std::string& content, const std::string& file_path);

    void evaluate();

private:
    libdocscript::runtime::Environment _env;
    libdocscript::StringStream _stream;
};

EVAL::EVAL(const std::string& content, const std::string& file_path)
    : _stream(content)
{
    libdocscript::runtime::initialize_environment(_env);
    auto exe_abs_path = std::filesystem::current_path().string();
    _env.set<libdocscript::runtime::Value>(
        "__CURRENT_PATH", libdocscript::runtime::String(exe_abs_path));
    if (file_path.size() != 0) {
        auto file_abs_path = std::filesystem::absolute(file_path).string();
        _env.set<libdocscript::runtime::Value>(
            "__FILE_PATH", libdocscript::runtime::String(file_abs_path));
    }
}

void EVAL::evaluate()
{
    auto expr_list = libdocscript::Parser::parse(_stream, _env);
    libdocscript::runtime::Value result = libdocscript::runtime::Nil();
    try {
        for (const auto& expr : expr_list) {
            result = libdocscript::Interpreter(_env).eval(expr);
        }
        print(std::cout, result) << std::endl;
    }
    catch (const libdocscript::Exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << std::endl;
    }
}
} // namespace docsir

#endif