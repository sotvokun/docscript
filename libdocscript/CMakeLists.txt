cmake_minimum_required(VERSION 3.20)

# DO NOT FORGET TO CHANGE THE VERSION NUMBER AT `include/libdocscript/version.h'
# IF YOU CHANGED THEM HERE !!!
project(libdocscript VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories("include")

set(SRC_FILES 
    # Utility
    "src/utility/stringstream.cpp"

    # Scanner
    "src/scanner.cpp"
    "src/scanner/mode_adapter.cpp"
    "src/scanner/normal_mode.cpp"
    "src/scanner/text_mode.cpp"

    # Parser
    "src/parser.cpp"
    "src/ast/ast_node.cpp"
    "src/ast/expression.cpp"
    "src/ast/atom.cpp"
    "src/ast/atom_helper.cpp"
    "src/ast/list.cpp"

    # Interpreter
    "src/interpreter.cpp"
    "src/runtime/environment.cpp"
    "src/runtime/environment/initialize_environment.cpp"
    "src/runtime/value.cpp"

    # Interpreter:Macro
    "src/runtime/macro.cpp"
    "src/runtime/macro_expander.cpp"

    # Interpreter:DataType
    "src/runtime/datatype.cpp"
    "src/runtime/datatype/unspecific.cpp"
    "src/runtime/datatype/nil.cpp"
    "src/runtime/datatype/error.cpp"
    "src/runtime/datatype/symbol.cpp"
    "src/runtime/datatype/boolean.cpp"
    "src/runtime/datatype/number.cpp"
    "src/runtime/datatype/string.cpp"
    "src/runtime/list.cpp"
    "src/runtime/procedure.cpp"
    "src/runtime/procedure/lambda_procedure.cpp"
    "src/runtime/procedure/builtin_procedure.cpp"

    # Interpreter:SpecialForm
    "src/runtime/specialform.cpp"
    "src/runtime/specialform/define.cpp"
    "src/runtime/specialform/set.cpp"
    "src/runtime/specialform/lambda.cpp"
    "src/runtime/specialform/logic_and.cpp"
    "src/runtime/specialform/logic_or.cpp"
    "src/runtime/specialform/if_else.cpp"
    "src/runtime/specialform/quote.cpp"
    "src/runtime/specialform/quasiquote.cpp"
    "src/runtime/specialform/unquote.cpp"
    "src/runtime/specialform/unquote_splicing.cpp"
    "src/runtime/specialform/define_macro.cpp"

    # StandardProcedure:BuiltIn
    "src/proc/builtin/equivalence.cpp"
    "src/proc/builtin/boolean.cpp"
    "src/proc/builtin/list.cpp"
    "src/proc/builtin/symbol.cpp"
    "src/proc/builtin/string.cpp"
    "src/proc/builtin/number.cpp"
    "src/proc/builtin/procedure.cpp"
    "src/proc/builtin/error.cpp"
    "src/proc/builtin/nil.cpp"

    # StandardProcedure:Utility
    "src/proc/utility.cpp"
)

add_library(libdocscript STATIC ${SRC_FILES})