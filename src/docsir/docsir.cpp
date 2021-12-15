#include "CLI/CLI.hpp"
#include "docsir/commands/eval.h"
#include "docsir/commands/exec.h"
#include "docsir/commands/repl.h"
#include "docsir/commands/version.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

int main(int argc, const char* argv[])
{
    // +--------------------+
    //    Opts & Cmds Setup
    // +--------------------+
    CLI::App app("the docscript interpreter", "docsir");

    // +--------------------+
    //   Subcommand eval
    // +--------------------+
    auto eval_mode =
        app.add_subcommand("eval", "Evaluate expression and print the result");
    eval_mode->add_option_function<std::string>(
        "expression",
        [&](const std::string& content) {
            docsir::EVAL(content, "").evaluate();
            std::exit(0);
        },
        "the expression");
    eval_mode->callback([&]() {
        std::cout << eval_mode->help() << std::endl;
        std::exit(0);
    });

    // +--------------------+
    //   Subcommand exec;
    // +--------------------+
    auto exec_mode = app.add_subcommand("exec", "Read and evaluate file");
    exec_mode->add_option_function<std::string>(
        "file_path",
        [&](const std::string& filepath) {
            if (docsir::EXEC::is_exist(filepath)) {
                docsir::EVAL(docsir::EXEC::read_content(filepath), filepath)
                    .evaluate();
            }
            else {
                std::cout << "the file path does not exist" << std::endl;
                return std::exit(0);
            }
            std::exit(0);
        },
        "the file path");
    exec_mode->callback([&]() {
        std::cout << exec_mode->help() << std::endl;
        std::exit(0);
    });

    // +--------------------+
    //   Subcommand version
    // +--------------------+
    auto version = app.add_subcommand("version", "Show versions");
    version->callback([&]() {
        docsir::print_version();
        std::exit(0);
    });

    // +--------------------+
    //      REPL MODE
    // +--------------------+
    app.callback([&]() {
        docsir::REPL repl;
        repl.run();
    });

    CLI11_PARSE(app, argc, argv);
}