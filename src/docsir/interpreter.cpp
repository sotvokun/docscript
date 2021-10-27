#include "docsir/interpreter.h"
#include "docscript.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/environment.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include "libdocscript/ast.h"
#include <algorithm>
#include <optional>
#include <string>
#include <vector>

using docscript::libdocscript::SExpressionAST,
    docscript::libdocscript::SExpressionListAST,
    docscript::libdocscript::AtomAST, docscript::libdocscript::AtomType,
    docscript::libdocscript::ListAST, docscript::libdocscript::TextAST,
    docscript::libdocscript::TextStringAST,
    docscript::libdocscript::TextLineEndAST,
    docscript::libdocscript::ASTNodeType;

namespace docscript::docsir {

// PUBLIC

const Value &Interpreter::last_result() const { return _env.get_result(); }

std::optional<Value> Interpreter::eval(const SExpressionAST &expr,
                                       bool allow_def) {
    switch (expr.sexpr_type) {
    case ASTNodeType::Atom: {
        auto result = eval(cast<AtomAST>(expr.value));
        _env.set_result(result);
        return result;
    } break;

    case ASTNodeType::Text: {
        auto result = eval(cast<TextAST>(expr.value));
        _env.set_result(result);
        return result;
    } break;

    case ASTNodeType::List: {
        auto result = eval(cast<ListAST>(expr.value));
        _env.set_result(result);
        return result;
    } break;

    case ASTNodeType::SExpressionList: {
        auto result = eval(cast<SExpressionListAST>(expr.value), allow_def);
        if (result.has_value()) {
            _env.set_result(result.value());
            return result.value();
        } else {
            return std::optional<Value>();
        }
    } break;

    case ASTNodeType::Comment:
    default: {
        return std::optional<Value>();
    } break;
    }
}

Value Interpreter::eval(const AtomAST &atom) {
    switch (atom.atom_type) {
    case AtomType::Identifier: {
        auto str = atom.value<std::string>();
        if (is_keyword(str)) {
            throw BadSyntaxException("keyword as identifier");
        }
        return _env.get_definition(str);
    } break;

    case AtomType::Symbol: {
        return Value::Symbol(atom.value<std::string>());
    } break;

    case AtomType::IntNumber: {
        return Value::Number(atom.value<ds_integer>());
    } break;

    case AtomType::FloatNumber: {
        return Value::Number(atom.value<ds_float>());
    } break;

    case AtomType::Boolean: {
        return Value::Boolean(atom.value<bool>());
    } break;

    case AtomType::String: {
        return Value::String(atom.value<std::string>());
    } break;
    }
    throw InterpreterException("Internal Error, Bad Status");
}

Value Interpreter::eval(const TextAST &text) {
    Value result = Value::List();
    auto &reslist = result.cast<List>().value();
    for (const auto &i : text.value) {
        switch (i->type) {
        case ASTNodeType::TextString: {
            auto val = Value::String(cast<TextStringAST>(i).value);
            reslist.push_back(val);
        } break;
        case ASTNodeType::TextLineEnd: {
            auto val = Value::Symbol("LineEnd");
            reslist.push_back(val);
        } break;
        case ASTNodeType::SExpression: {
            auto val = eval(cast<SExpressionAST>(i), false).value();
            reslist.push_back(val);
        } break;
        }
    }
    return result;
}

Value Interpreter::eval(const ListAST &list) {
    Value result = Value::List();
    auto &reslist = result.cast<List>().value();
    for (const auto &i : list.value) {
        auto val = eval(cast<SExpressionAST>(i), false).value();
        reslist.push_back(val);
    }
    return result;
}

std::optional<Value> Interpreter::eval(const SExpressionListAST &list,
                                       bool allow_def) {
    auto &exprlist = list.value;
    if (exprlist.size() == 0) {
        throw BadSyntaxException("no elements in s-expression");
    }

    auto &first = cast<SExpressionAST>(exprlist[0]);
    if (first.sexpr_type == ASTNodeType::Atom &&
        cast<AtomAST>(first.value).atom_type == AtomType::Identifier &&
        is_keyword(cast<AtomAST>(first.value).value<std::string>())) {
        return eval_specialform(exprlist, allow_def);
    } else {
        return eval_procedure(exprlist);
    }
}

Value Interpreter::eval_procedure(const sexprlist_type &list) {
    Value proc = Value::List();
    List args;

    // handle operator

    auto &first = cast<SExpressionAST>(list[0]);
    if (first.sexpr_type != ASTNodeType::Atom &&
        first.sexpr_type != ASTNodeType::SExpressionList) {
        throw BadSyntaxException("operator must be procedure");
    }
    if (first.sexpr_type == ASTNodeType::Atom) {
        auto &atom = cast<AtomAST>(first.value);
        auto result = eval(atom);
        if (result.type() != ValueType::Procedure) {
            throw BadSyntaxException("operator must be procedure");
        }
        proc = result;
    } else if (first.sexpr_type == ASTNodeType::SExpressionList) {
        auto &expr = cast<SExpressionListAST>(first.value);
        auto result = eval(expr);
        if (!result.has_value() ||
            result.value().type() != ValueType::Procedure) {
            throw BadSyntaxException("operator must be procedure");
        }
        proc = result.value();
    }

    // handle operands

    for (decltype(list.size()) i = 1; i != list.size(); ++i) {
        auto &elem = cast<SExpressionAST>(list[i]);
        auto val = eval(elem, false).value();
        args.value().push_back(val);
    }

    // invoke operator

    // BuiltIn Procedure
    if (proc.cast<Procedure>().builtin()) {
        auto &func = proc.cast<BuiltInProcedure>();
        return func.value()(args.value());
    }
    // Custom Procedure
    else {
        auto &func = proc.cast<LambdaProcedure>();
        if (func.parameter().size() != args.value().size()) {
            throw ArgumentException::unexpected(func.parameter().size(),
                                                args.value().size());
        }
        Environment env(_env);
        for (decltype(func.parameter().size()) i = 0;
             i != func.parameter().size(); ++i) {
            env.set_definition(func.parameter()[i], args.value()[i]);
        }
        Interpreter interpreter(env);
        return interpreter.eval(func.body(), false).value();
    }
}

std::optional<Value> Interpreter::eval_specialform(const sexprlist_type &list,
                                                   bool allow_def) {
    auto &first = cast<SExpressionAST>(list[0]);
    if (first.sexpr_type != ASTNodeType::Atom ||
        cast<AtomAST>(first.value).atom_type != AtomType::Identifier ||
        !is_keyword(cast<AtomAST>(first.value).value<std::string>())) {
        throw InterpreterException("Internal Error, Bad Status");
    }

    auto id = cast<AtomAST>(first.value).value<std::string>();
    // define
    if (id == "define") {
        if (!allow_def) {
            throw BadSyntaxException(
                "refuse change environment in an expression");
        }
        eval_definition(list);
        return std::optional<Value>();
    }
    // set!
    else if (id == "set!") {
        return eval_assignment(list);
    }
    // lambda | λ
    else if (id == "lambda" || id == "λ") {
        return eval_lambda(list);
    }
    // and
    else if (id == "and") {
        return eval_logical_and(list);
    }
    // or
    else if (id == "or") {
        return eval_logical_or(list);
    }
    // if
    else if (id == "if") {
        return eval_ifelse(list);
    }
    // for
    else if (id == "for") {
        return eval_for(list);
    }
    // ELSE
    else {
        throw BadSyntaxException("unknown special form");
    }
}

void Interpreter::eval_definition(const sexprlist_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("define");
    }
    const auto &second = cast<SExpressionAST>(list[1]);
    const auto &third = cast<SExpressionAST>(list[2]);

    if (second.sexpr_type != ASTNodeType::Atom ||
        cast<AtomAST>(second.value).atom_type != AtomType::Identifier) {
        throw BadSyntaxException("define identifier");
    }

    auto &id = cast<AtomAST>(second.value).value<std::string>();
    if (_env.exist_definition(id)) {
        throw DefineExistedNameException(id);
    }
    if (is_keyword(id)) {
        throw BadSyntaxException("define keyword");
    }
    _env.set_definition(id, eval(third, false).value());
}

Value Interpreter::eval_assignment(const sexprlist_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("set!");
    }
    const auto &second = cast<SExpressionAST>(list[1]);
    const auto &third = cast<SExpressionAST>(list[2]);

    if (second.sexpr_type != ASTNodeType::Atom ||
        cast<AtomAST>(second.value).atom_type != AtomType::Identifier) {
        throw BadSyntaxException("set! identifier");
    }

    auto &id = cast<AtomAST>(second.value).value<std::string>();
    if (!_env.exist_definition(id)) {
        throw AccessUndefinedNameException(id);
    }
    if (is_keyword(id)) {
        throw BadSyntaxException("define keyword");
    }
    auto result = eval(third, false).value();
    _env.set_definition(id, result);
    return result;
}

Value Interpreter::eval_lambda(const sexprlist_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("lambda");
    }
    const auto &second = cast<SExpressionAST>(list[1]);
    const auto &third = cast<SExpressionAST>(list[2]);

    LambdaProcedure::parameter_type parameters;

    // handle parameters

    if ((second.sexpr_type != ASTNodeType::Atom ||
         cast<AtomAST>(second.value).atom_type != AtomType::Identifier) &&
        second.sexpr_type != ASTNodeType::SExpressionList) {
        throw BadSyntaxException("lambda parameter");
    }
    if (second.sexpr_type == ASTNodeType::Atom &&
        cast<AtomAST>(second.value).atom_type == AtomType::Identifier) {
        if (is_keyword(cast<AtomAST>(second.value).value<std::string>())) {
            throw BadSyntaxException("lambda parameter single");
        }
        parameters.push_back(cast<AtomAST>(second.value).value<std::string>());
    } else {
        const auto &exprlist = cast<SExpressionListAST>(second.value);
        for (const auto &i : exprlist.value) {
            const auto &elem = cast<SExpressionAST>(i);
            if (elem.sexpr_type != ASTNodeType::Atom ||
                cast<AtomAST>(elem.value).atom_type != AtomType::Identifier ||
                is_keyword(cast<AtomAST>(elem.value).value<std::string>())) {
                throw BadSyntaxException("lambda parameter list");
            }
            parameters.push_back(
                cast<AtomAST>(elem.value).value<std::string>());
        }
    }

    // create procedure
    if (parameters.size() == 0) {
        return Value::LambdaProcedure(third);
    } else {
        return Value::LambdaProcedure(parameters, third);
    }
}

Value Interpreter::eval_logical_and(const sexprlist_type &list) {
    Value result = Value::Boolean(true);
    for (sexprlist_type::size_type i = 1; i != list.size(); ++i) {
        auto val = eval(cast<SExpressionAST>(list[i]), false).value();
        if (!val) {
            return val;
        } else {
            result = val;
        }
    }
    return result;
}

Value Interpreter::eval_logical_or(const sexprlist_type &list) {
    Value result = Value::Boolean(false);
    for (sexprlist_type::size_type i = 1; i != list.size(); ++i) {
        auto val = eval(cast<SExpressionAST>(list[i]), false).value();
        if (val) {
            return val;
        } else {
            result = val;
        }
    }
    return result;
}

Value Interpreter::eval_ifelse(const sexprlist_type &list) {
    if (list.size() != 4) {
        throw BadSyntaxException("if");
    }
    const auto &second = cast<SExpressionAST>(list[1]);
    const auto &third = cast<SExpressionAST>(list[2]);
    const auto &fourth = cast<SExpressionAST>(list[3]);

    if (eval(second, false).value()) {
        return eval(third, false).value();
    } else {
        return eval(fourth, false).value();
    }
}

Value Interpreter::eval_for(const sexprlist_type &list) {
    if (list.size() != 4) {
        throw BadSyntaxException("for");
    }
    const auto &second = cast<SExpressionAST>(list[1]);
    const auto &third = cast<SExpressionAST>(list[2]);
    const auto &fourth = cast<SExpressionAST>(list[3]);

    // handle temp variable
    if (second.sexpr_type != ASTNodeType::Atom ||
        cast<AtomAST>(second.value).atom_type != AtomType::Identifier ||
        is_keyword(cast<AtomAST>(second.value).value<std::string>())) {
        throw BadSyntaxException("for illegal name");
    }

    auto id = cast<AtomAST>(second.value).value<std::string>();

    // handle list
    auto val = eval(third, false).value();
    if (val.type() != ValueType::List) {
        throw BadSyntaxException("for list");
    }

    // do body
    Environment env(_env);
    Value result = Value::List();
    for (const auto &i : val.cast<List>().value()) {
        env.set_definition(id, i);
        Interpreter interpreter(env);
        result = interpreter.eval(fourth, false).value();
    }
    return result;
}

// PRIVATE

bool Interpreter::is_keyword(const std::string &id) const {
    return std::find_if(_keywords.begin(), _keywords.end(),
                        [&id](const auto &s) { return id == s; }) !=
           _keywords.end();
}

} // namespace docscript::docsir