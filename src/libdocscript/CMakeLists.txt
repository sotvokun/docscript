cmake_minimum_required(VERSION 3.8)

set(LIBDOCSCRIPT_SRC_FILES
    # Utility
    "utility/stringstream.cpp"

    # Scanner
    "scanner.cpp"
    "scanner/mode_adapter.cpp"
    "scanner/normal_mode.cpp"
    "scanner/text_mode.cpp"

    # Parser
    "parser.cpp"
    "ast/ast_node.cpp"
    "ast/expression.cpp"
    "ast/atom.cpp"
    "ast/atom_helper.cpp"
    "ast/list.cpp"

    # Interpreter
    "interpreter.cpp"
    "runtime/environment.cpp"
    "runtime/environment/initialize_environment.cpp"
    "runtime/value.cpp"

    # Interpreter:Macro
    "runtime/macro.cpp"
    "runtime/macro_expander.cpp"

    # Interpreter:DataType
    "runtime/datatype.cpp"
    "runtime/datatype/unspecific.cpp"
    "runtime/datatype/nil.cpp"
    "runtime/datatype/error.cpp"
    "runtime/datatype/symbol.cpp"
    "runtime/datatype/boolean.cpp"
    "runtime/datatype/number.cpp"
    "runtime/datatype/string.cpp"
    "runtime/list.cpp"
    "runtime/procedure.cpp"
    "runtime/procedure/lambda_procedure.cpp"
    "runtime/procedure/builtin_procedure.cpp"

    # Interpreter:SpecialForm
    "runtime/specialform.cpp"
    "runtime/specialform/define.cpp"
    "runtime/specialform/set.cpp"
    "runtime/specialform/lambda.cpp"
    "runtime/specialform/logic_and.cpp"
    "runtime/specialform/logic_or.cpp"
    "runtime/specialform/if_else.cpp"
    "runtime/specialform/quote.cpp"
    "runtime/specialform/quasiquote.cpp"
    "runtime/specialform/unquote.cpp"
    "runtime/specialform/unquote_splicing.cpp"
    "runtime/specialform/define_macro.cpp"

    # StandardProcedure:BuiltIn
    "proc/builtin/equivalence.cpp"
    "proc/builtin/boolean.cpp"
    "proc/builtin/list.cpp"
    "proc/builtin/symbol.cpp"
    "proc/builtin/string.cpp"
    "proc/builtin/number.cpp"
    "proc/builtin/procedure.cpp"
    "proc/builtin/error.cpp"
    "proc/builtin/nil.cpp"
    "proc/builtin/stdio.cpp"
    "proc/builtin/load.cpp"

    # StandardProcedure:Utility
    "proc/utility.cpp")

add_library(libdocscript ${LIBDOCSCRIPT_SRC_FILES})