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

using namespace docscript::libdocscript::ast;

namespace docscript::docsir {

// PUBLIC

const Value &Interpreter::last_result() const { return _env.get_result(); }

std::optional<Value> Interpreter::eval(const SExpression &expr,
                                       bool allow_def) {
    switch (expr.type_sexpr) {
    case SExpressionType::Comment: {
        return std::optional<Value>();
    } break;

    case SExpressionType::Atom: {
        auto result = eval(expr.value.cast<Atom>());
        _env.set_result(result);
        return result;
    } break;

    case SExpressionType::Text: {
        auto result = eval(expr.value.cast<Text>());
        _env.set_result(result);
        return result;
    } break;

    case SExpressionType::RoundList: {
        auto result = eval(expr.value.cast<RoundList>());
        _env.set_result(result);
        return result;
    } break;

    case SExpressionType::SquareList: {
        auto result = eval(expr.value.cast<SquareList>(), allow_def);
        if (result.has_value()) {
            _env.set_result(result.value());
            return result.value();
        } else {
            return std::optional<Value>();
        }
    } break;

    case SExpressionType::Quote: {
        auto result = eval(expr.value.cast<Quote>());
        _env.set_result(result);
        return result;
    } break;

    case SExpressionType::Quasiquote: {
        auto result = eval(expr.value.cast<Quasiquote>());
        _env.set_result(result);
        return result;
    } break;

    case SExpressionType::Unquote: {
        auto result = eval(expr.value.cast<Unquote>());
        if (result.has_value()) {
            _env.set_result(result.value());
            return result.value();
        } else {
            return std::optional<Value>();
        }
    } break;

    default: {
        return std::optional<Value>();
    } break;
    }
}

Value Interpreter::eval(const Atom &atom) {
    switch (atom.type_atom) {
    case AtomType::Name: {
        auto str = atom.value<std::string>();
        if (is_keyword(str)) {
            throw BadSyntaxException("keyword as identifier");
        }
        return _env.get_definition(str);
    } break;

    case AtomType::Integer: {
        return Value::Number(atom.value<ds_integer>());
    } break;

    case AtomType::Decimal: {
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

Value Interpreter::eval(const Text &text) {
    Value result = Value::List();
    auto &reslist = result.cast<List>().value();
    for (const auto &i : text.value) {
        switch (i.type()) {
        case NodeType::TextString: {
            auto val = Value::String(i.cast<TextString>().value);
            reslist.push_back(val);
        } break;
        case NodeType::TextLineEnd: {
            auto val = Value::Symbol("LineEnd");
            reslist.push_back(val);
        } break;
        case NodeType::SExpression: {
            auto val = eval(i.cast<SExpression>(), false).value();
            reslist.push_back(val);
        } break;
        }
    }
    return result;
}

Value Interpreter::eval(const RoundList &list) {
    Value result = Value::List();
    auto &reslist = result.cast<List>().value();
    for (const auto &i : list.value) {
        auto val = eval(i.cast<SExpression>(), false).value();
        reslist.push_back(val);
    }
    return result;
}

std::optional<Value> Interpreter::eval(const SquareList &list, bool allow_def) {
    auto &exprlist = list.value;
    if (exprlist.size() == 0) {
        throw BadSyntaxException("no elements in s-expression");
    }

    auto &first = exprlist[0].cast<SExpression>();
    if (first.type_sexpr == SExpressionType::Atom &&
        first.value.cast<Atom>().type_atom == AtomType::Name &&
        is_keyword(first.value.cast<Atom>().value<Atom::string_type>())) {
        return eval_specialform(exprlist, allow_def);
    } else {
        return eval_procedure(exprlist);
    }
}

Value Interpreter::eval(const Quote &quote) {
    auto &val = quote.value.cast<SExpression>();
    return quote_value(val, false);
}

Value Interpreter::eval(const Quasiquote &quasiquote) {
    auto &val = quasiquote.value.cast<SExpression>();
    return quote_value(val, true);
}

std::optional<Value> Interpreter::eval(const Unquote &unquote) {
    auto &val = unquote.value.cast<SExpression>();
    return eval(val, false);
}

Value Interpreter::eval_procedure(const list_type &list) {
    Value proc = Value::List();
    List args;

    // handle operator

    auto &first = list[0].cast<SExpression>();
    if (first.type_sexpr != SExpressionType::Atom &&
        first.type_sexpr != SExpressionType::SquareList) {
        throw BadSyntaxException("operator must be procedure");
    }
    if (first.type_sexpr == SExpressionType::Atom) {
        auto &atom = first.value.cast<Atom>();
        auto result = eval(atom);
        if (result.type() != ValueType::Procedure) {
            throw BadSyntaxException("operator must be procedure");
        }
        proc = result;
    } else if (first.type_sexpr == SExpressionType::SquareList) {
        auto &expr = first.value.cast<SquareList>();
        auto result = eval(expr);
        if (!result.has_value() ||
            result.value().type() != ValueType::Procedure) {
            throw BadSyntaxException("operator must be procedure");
        }
        proc = result.value();
    }

    // handle operands

    for (decltype(list.size()) i = 1; i != list.size(); ++i) {
        auto &elem = list[i].cast<SExpression>();
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

std::optional<Value> Interpreter::eval_specialform(const list_type &list,
                                                   bool allow_def) {
    auto &first = list[0].cast<SExpression>();
    if (first.type_sexpr != SExpressionType::Atom ||
        first.value.cast<Atom>().type_atom != AtomType::Name ||
        !is_keyword(first.value.cast<Atom>().value<std::string>())) {
        throw InterpreterException("Internal Error, Bad Status");
    }

    auto id = first.value.cast<Atom>().value<std::string>();
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
    // quote
    else if (id == "quote") {
        return eval_quote(list);
    }
    // quasiquote
    else if (id == "quasiquote") {
        return eval_quasiquote(list);
    }
    // unquote
    else if (id == "unquote") {
        return eval_unquote(list);
    }
    // ELSE
    else {
        throw BadSyntaxException("unknown special form");
    }
}

void Interpreter::eval_definition(const list_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("define");
    }
    const auto &second = list[1].cast<SExpression>();
    const auto &third = list[2].cast<SExpression>();

    if (second.type_sexpr != SExpressionType::Atom ||
        second.value.cast<Atom>().type_atom != AtomType::Name) {
        throw BadSyntaxException("define identifier");
    }

    auto &id = second.value.cast<Atom>().value<std::string>();
    if (_env.exist_definition(id)) {
        throw DefineExistedNameException(id);
    }
    if (is_keyword(id)) {
        throw BadSyntaxException("define keyword");
    }
    _env.set_definition(id, eval(third, false).value());
}

Value Interpreter::eval_assignment(const list_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("set!");
    }
    const auto &second = list[1].cast<SExpression>();
    const auto &third = list[2].cast<SExpression>();

    if (second.type_sexpr != SExpressionType::Atom ||
        second.value.cast<Atom>().type_atom != AtomType::Name) {
        throw BadSyntaxException("set! identifier");
    }

    auto &id = second.value.cast<Atom>().value<std::string>();
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

Value Interpreter::eval_lambda(const list_type &list) {
    if (list.size() != 3) {
        throw BadSyntaxException("lambda");
    }
    const auto &second = list[1].cast<SExpression>();
    const auto &third = list[2].cast<SExpression>();

    LambdaProcedure::parameter_type parameters;

    // handle parameters

    if ((second.type_sexpr != SExpressionType::Atom ||
         second.value.cast<Atom>().type_atom != AtomType::Name) &&
        second.type_sexpr != SExpressionType::SquareList) {
        throw BadSyntaxException("lambda parameter");
    }
    if (second.type_sexpr == SExpressionType::Atom &&
        second.value.cast<Atom>().type_atom == AtomType::Name) {
        if (is_keyword(second.value.cast<Atom>().value<std::string>())) {
            throw BadSyntaxException("lambda parameter single");
        }
        parameters.push_back(second.value.cast<Atom>().value<std::string>());
    } else {
        const auto &exprlist = second.value.cast<SquareList>();
        for (const auto &i : exprlist.value) {
            const auto &elem = i.cast<SExpression>();
            if (elem.type_sexpr != SExpressionType::Atom ||
                elem.value.cast<Atom>().type_atom != AtomType::Name ||
                is_keyword(elem.value.cast<Atom>().value<std::string>())) {
                throw BadSyntaxException("lambda parameter list");
            }
            parameters.push_back(elem.value.cast<Atom>().value<std::string>());
        }
    }

    // create procedure
    if (parameters.size() == 0) {
        return Value::LambdaProcedure(third);
    } else {
        return Value::LambdaProcedure(parameters, third);
    }
}

Value Interpreter::eval_logical_and(const list_type &list) {
    Value result = Value::Boolean(true);
    for (list_type::size_type i = 1; i != list.size(); ++i) {
        auto val = eval(list[i].cast<SExpression>(), false).value();
        if (!val) {
            return val;
        } else {
            result = val;
        }
    }
    return result;
}

Value Interpreter::eval_logical_or(const list_type &list) {
    Value result = Value::Boolean(false);
    for (list_type::size_type i = 1; i != list.size(); ++i) {
        auto val = eval(list[i].cast<SExpression>(), false).value();
        if (val) {
            return val;
        } else {
            result = val;
        }
    }
    return result;
}

Value Interpreter::eval_ifelse(const list_type &list) {
    if (list.size() != 4) {
        throw BadSyntaxException("if");
    }
    const auto &second = list[1].cast<SExpression>();
    const auto &third = list[2].cast<SExpression>();
    const auto &fourth = list[3].cast<SExpression>();

    if (eval(second, false).value()) {
        return eval(third, false).value();
    } else {
        return eval(fourth, false).value();
    }
}

Value Interpreter::eval_for(const list_type &list) {
    if (list.size() != 4) {
        throw BadSyntaxException("for");
    }
    const auto &second = list[1].cast<SExpression>();
    const auto &third = list[2].cast<SExpression>();
    const auto &fourth = list[3].cast<SExpression>();

    // handle temp variable
    if (second.type_sexpr != SExpressionType::Atom ||
        second.value.cast<Atom>().type_atom != AtomType::Name ||
        is_keyword(second.value.cast<Atom>().value<std::string>())) {
        throw BadSyntaxException("for illegal name");
    }

    auto id = second.value.cast<Atom>().value<std::string>();

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

Value Interpreter::eval_quote(const list_type &list) {
    if (list.size() != 2) {
        throw BadSyntaxException("quote");
    }

    const auto &second = list[1].cast<SExpression>();

    return quote_value(second, false);
}

Value Interpreter::eval_quasiquote(const list_type &list) {
    if (list.size() != 2) {
        throw BadSyntaxException("quote");
    }

    const auto &second = list[1].cast<SExpression>();

    return quote_value(second, true);
}

Value Interpreter::eval_unquote(const list_type &list) {
    if (list.size() != 2) {
        throw BadSyntaxException("quote");
    }

    const auto &second = list[1].cast<SExpression>();

    Unquote q(new SExpression(second), second.position);
    return quote_value(q, true);
}

Value Interpreter::quote_value(const SExpression &sexpr, bool quasiquote) {
    switch (sexpr.type_sexpr) {
    case SExpressionType::Atom: {
        auto &atom = sexpr.value.cast<Atom>();
        switch (atom.type_atom) {
        case AtomType::Name:
            return Value::Symbol(atom.value<Atom::string_type>());
        case AtomType::Integer:
            return Value::Number(atom.value<Atom::int_type>());
        case AtomType::Decimal:
            return Value::Number(atom.value<Atom::decimal_type>());
        case AtomType::String:
            return Value::String(atom.value<Atom::string_type>());
        case AtomType::Boolean:
            return Value::Boolean(atom.value<Atom::bool_type>());
        }
    } break;

    case SExpressionType::RoundList: {
        Value result = Value::List();
        auto &list = sexpr.value.cast<RoundList>().value;
        for (const auto &i : list) {
            result.cast<List>().value().push_back(
                quote_value(i.cast<SExpression>(), quasiquote));
        }
        return result;
    } break;

    case SExpressionType::SquareList: {
        Value result = Value::List();
        auto &list = sexpr.value.cast<SquareList>().value;
        if (quasiquote && list.size() == 2 &&
            list[0].cast<SExpression>().type_sexpr == SExpressionType::Atom &&
            list[0].cast<SExpression>().value.cast<Atom>().type_atom ==
                AtomType::Name &&
            list[0].cast<SExpression>()
                    .value.cast<Atom>()
                    .value<std::string>() == "unquote") {
            Unquote unquote(new SExpression(list[1].cast<SExpression>()),
                            list[0].cast<SExpression>().position);
            return quote_value(unquote, true);
        } else {
            for (const auto &i : list) {
                result.cast<List>().value().push_back(
                    quote_value(i.cast<SExpression>(), quasiquote));
            }
            return result;
        }
    } break;

    case SExpressionType::Text: {
        Value result = Value::List();
        auto &reslist = result.cast<List>().value();
        auto &list = sexpr.value.cast<Text>().value;
        for (const auto &i : list) {
            switch (i.type()) {
            case NodeType::TextString: {
                auto val = Value::String(i.cast<TextString>().value);
                reslist.push_back(val);
            } break;
            case NodeType::TextLineEnd: {
                auto val = Value::Symbol("LineEnd");
                reslist.push_back(val);
            } break;
            case NodeType::SExpression: {
                auto val = quote_value(i.cast<SExpression>(), quasiquote);
                reslist.push_back(val);
            } break;
            }
        }
        return result;
    } break;

    case SExpressionType::Quote: {
        Value result = Value::List();
        auto &reslist = result.cast<List>().value();
        auto &quote = sexpr.value.cast<Quote>();
        reslist.push_back(Value::Symbol("quote"));
        auto val = quote_value(quote.value.cast<SExpression>(), quasiquote);
        reslist.push_back(val);
        return result;
    } break;

    case SExpressionType::Quasiquote: {
        Value result = Value::List();
        auto &reslist = result.cast<List>().value();
        auto &quote = sexpr.value.cast<Quasiquote>();
        reslist.push_back(Value::Symbol("quasiquote"));
        auto val = quote_value(quote.value.cast<SExpression>(), quasiquote);
        reslist.push_back(val);
        return result;
    } break;

    case SExpressionType::Unquote: {
        if (quasiquote) {
            auto val = eval(
                sexpr.value.cast<Unquote>().value.cast<SExpression>(), false);
            return val.has_value() ? val.value() : Value::List();
        } else {
            Value result = Value::List();
            auto &reslist = result.cast<List>().value();
            auto &quote = sexpr.value.cast<Unquote>();
            reslist.push_back(Value::Symbol("unquote"));
            auto val = quote_value(quote.value.cast<SExpression>(), quasiquote);
            reslist.push_back(val);
            return result;
        }
    } break;
    default:
        return Value::List();
    }
}

// PRIVATE

bool Interpreter::is_keyword(const std::string &id) const {
    return std::find_if(_keywords.begin(), _keywords.end(),
                        [&id](const auto &s) { return id == s; }) !=
           _keywords.end();
}

} // namespace docscript::docsir