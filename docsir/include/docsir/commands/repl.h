#ifndef DOCSIR_COMMANDS_REPL_H
#define DOCSIR_COMMANDS_REPL_H
#include "docsir/utility.h"
#include "docsir/version.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/parser.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/value.h"
#include "libdocscript/utility/stringstream.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace docsir {
class REPL final
{
  public:
    REPL();

    inline void run();

    inline void process_repl_command(const std::string& cmd);
    inline void print_repl_help();
    inline void print_welcome_message();

  private:
    libdocscript::runtime::Environment _env;
    libdocscript::StringStream _stream;

    bool prev_input_unfinished = false;
};

// +--------------------+
//      Constructor
// +--------------------+

REPL::REPL()
{
    libdocscript::runtime::initialize_environment(_env);
}

// +--------------------+
//    Public Functions
// +--------------------+

inline void
REPL::run()
{
    print_welcome_message();
    while (true) {
        if (!prev_input_unfinished)
            std::cout << "docsir> ";
        else
            std::cout << "> ";

        std::string user_input;
        std::getline(std::cin, user_input);

        if (user_input.front() == ';') {
            process_repl_command(user_input);
            continue;
        }

        try {
            if (prev_input_unfinished) {
                _stream.push_back(user_input);
                _stream.reset();
                prev_input_unfinished = false;
            } else {
                _stream.reset(user_input);
            }

            auto exprs = libdocscript::Parser::parse(_stream, _env);
            for (const auto& e : exprs) {
                auto value = libdocscript::Interpreter(_env).eval(e);
                if (value.type() !=
                    libdocscript::runtime::DataType::Kind::Unspecific)
                    print(std::cout, value) << std::endl;
            }
        }
        // Unfinished input
        catch (const libdocscript::UnfinishedInput& e) {
            prev_input_unfinished = true;
            _stream.push_back("\n");
        }
        // Unfinished bracket
        catch (const libdocscript::UnclosedList& e) {
            prev_input_unfinished = true;
            _stream.push_back("\n");
        }
        // Another exception
        catch (const libdocscript::Exception& e) {
            std::cerr << e.what() << std::endl;
        }
        // Fatal exception
        catch (const std::exception& e) {
            std::cerr << "fatal error: " << e.what() << std::endl;
            std::exit(1);
        }
    }
}

void
REPL::process_repl_command(const std::string& cmd)
{
    if (cmd == ";h" || cmd == ";help") {
        print_repl_help();
        return;
    }

    if (cmd == ";q" || cmd == ";quit") {
        std::exit(0);
    }
}

void
REPL::print_repl_help()
{
    std::cout << std::left << std::setw(5) << ";h" 
              << "print the help text" << std::endl;
    std::cout << std::left << std::setw(5) << ";q" 
              << "quit the repl" << std::endl;
}

void
REPL::print_welcome_message()
{
    std::cout << "Welcome to DocSir "
              << "v" << DOCSIR_VERSION_MAJOR << "." << DOCSIR_VERSION_MINOR
              << std::endl;

    std::cout << "Type ;h to get the repl help" << std::endl;
}
}

#endif