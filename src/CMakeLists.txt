set(DOCSIR_SRC_FILES
    "src/docsir/docsir.cpp")

set(LIBDOCSCRIPT_SRC_FILES
    # Utility
    "src/libdocscript/utility/stringstream.cpp"

    # Scanner
    "src/libdocscript/scanner.cpp"
    "src/libdocscript/scanner/mode_adapter.cpp"
    "src/libdocscript/scanner/normal_mode.cpp"
    "src/libdocscript/scanner/text_mode.cpp"

    # Parser
    "src/libdocscript/parser.cpp"
    "src/libdocscript/ast/ast_node.cpp"
    "src/libdocscript/ast/expression.cpp"
    "src/libdocscript/ast/atom.cpp"
    "src/libdocscript/ast/atom_helper.cpp"
    "src/libdocscript/ast/list.cpp"

    # Interpreter
    "src/libdocscript/interpreter.cpp"
    "src/libdocscript/runtime/environment.cpp"
    "src/libdocscript/runtime/environment/initialize_environment.cpp"
    "src/libdocscript/runtime/value.cpp"

    # Interpreter:Macro
    "src/libdocscript/runtime/macro.cpp"
    "src/libdocscript/runtime/macro_expander.cpp"

    # Interpreter:DataType
    "src/libdocscript/runtime/datatype.cpp"
    "src/libdocscript/runtime/datatype/unspecific.cpp"
    "src/libdocscript/runtime/datatype/nil.cpp"
    "src/libdocscript/runtime/datatype/error.cpp"
    "src/libdocscript/runtime/datatype/symbol.cpp"
    "src/libdocscript/runtime/datatype/boolean.cpp"
    "src/libdocscript/runtime/datatype/number.cpp"
    "src/libdocscript/runtime/datatype/string.cpp"
    "src/libdocscript/runtime/list.cpp"
    "src/libdocscript/runtime/procedure.cpp"
    "src/libdocscript/runtime/procedure/lambda_procedure.cpp"
    "src/libdocscript/runtime/procedure/builtin_procedure.cpp"

    # Interpreter:SpecialForm
    "src/libdocscript/runtime/specialform.cpp"
    "src/libdocscript/runtime/specialform/define.cpp"
    "src/libdocscript/runtime/specialform/set.cpp"
    "src/libdocscript/runtime/specialform/lambda.cpp"
    "src/libdocscript/runtime/specialform/logic_and.cpp"
    "src/libdocscript/runtime/specialform/logic_or.cpp"
    "src/libdocscript/runtime/specialform/if_else.cpp"
    "src/libdocscript/runtime/specialform/quote.cpp"
    "src/libdocscript/runtime/specialform/quasiquote.cpp"
    "src/libdocscript/runtime/specialform/unquote.cpp"
    "src/libdocscript/runtime/specialform/unquote_splicing.cpp"
    "src/libdocscript/runtime/specialform/define_macro.cpp"

    # StandardProcedure:BuiltIn
    "src/libdocscript/proc/builtin/equivalence.cpp"
    "src/libdocscript/proc/builtin/boolean.cpp"
    "src/libdocscript/proc/builtin/list.cpp"
    "src/libdocscript/proc/builtin/symbol.cpp"
    "src/libdocscript/proc/builtin/string.cpp"
    "src/libdocscript/proc/builtin/number.cpp"
    "src/libdocscript/proc/builtin/procedure.cpp"
    "src/libdocscript/proc/builtin/error.cpp"
    "src/libdocscript/proc/builtin/nil.cpp"
    "src/libdocscript/proc/builtin/stdio.cpp"
    "src/libdocscript/proc/builtin/load.cpp"

    # StandardProcedure:Utility
    "src/libdocscript/proc/utility.cpp")