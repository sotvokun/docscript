#ifndef DOCSIR_COMMANDS_EVAL_H
#define DOCSIR_COMMANDS_EVAL_H
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/parser.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/utility/stringstream.h"
#include "docsir/utility.h"
#include <string>
#include <iostream>

namespace docsir {
class EVAL final {
  public:
    EVAL(const std::string &content);

    void evaluate(); 

  private:
    libdocscript::runtime::Environment _env;
    libdocscript::StringStream _stream;
};

EVAL::EVAL(const std::string &content)
  : _stream(content)
{
    libdocscript::runtime::initialize_environment(_env);
}

void
EVAL::evaluate()
{
    auto expr_list = libdocscript::Parser::parse(_stream, _env);
    libdocscript::runtime::Value result = libdocscript::runtime::Nil();
    try {
    for(const auto &expr : expr_list) {
        result = libdocscript::Interpreter(_env).eval(expr);
    }
    print(std::cout, result) << std::endl;
    } catch (const libdocscript::Exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "fatal error: " << e.what() << std::endl;
    }
}
}

#endif