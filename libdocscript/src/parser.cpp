#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/interpreter.h"
#include "libdocscript/parser.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/macro.h"
#include "libdocscript/scanner.h"
#include "libdocscript/utility/stringstream.h"
#include <vector>

namespace libdocscript {

using namespace ast;

// +-----------------------------------+
//      Static Data Member Definition
// +-----------------------------------+

ast::AtomHelper Parser::_atom_helper;

// +-------------------+
//      Constructor
// +-------------------+

Parser::Parser(Scanner& s, runtime::Environment& e) : _scanner(s), _env(e) {}

// +-------------------+
//   Static Functions
// +-------------------+

Parser::expression_list Parser::parse(StringStream& stream,
                                      runtime::Environment& env)
{
    Scanner scanner = Scanner(stream);
    Parser parser = Parser(scanner, env);
    expression_list result;
    while (parser._scanner) {
        auto t = parser._scanner.get();
        if (t == TokenType::Whitespace || t == TokenType::Comment) {
            continue;
        }
        else {
            result.push_back(parser.parse_expression(t));
        }
    }
    return result;
}

// +-------------------+
//   Private Functions
// +-------------------+

Expression Parser::parse_expression(const Token& token)
{
    switch (token.type) {
    case TokenType::Identifier:
    case TokenType::String:
    case TokenType::Number:
    case TokenType::Boolean:
        return parse_atom(token);

    case TokenType::TextContent:
    case TokenType::TextEmptyLine:
        return parse_text_element(token);

    case TokenType::SymbolBracketCurlyLeft:
    case TokenType::SymbolBracketRoundLeft:
    case TokenType::SymbolBracketSquareLeft: {
        auto list = parse_list(token);
        if (!list.is_empty() && list.begin()->type() == ASTNodeType::Atom) {
            const auto& first = list.begin()->cast<Atom>();
            // define-macro
            if (first.atom_type() == AtomType::DefineMacro) {
                Interpreter(_env).eval_special_form(list);
            }
            // macro expanding
            if (first.atom_type() == AtomType::Symbol &&
                _env.find<runtime::Macro>(first.content()))
            {
                return Interpreter(_env).expand_macro(list);
            }
        }
        return list;
    }

    case TokenType::SymbolQuote:
    case TokenType::SymbolBackquote:
    case TokenType::SymbolComma:
    case TokenType::SymbolCommaAt:
        return parse_quote_expand(token);

    case TokenType::SymbolHash:
        return parse_keyword(token);

    case TokenType::SymbolBracketCurlyRight:
    case TokenType::SymbolBracketRoundRight:
    case TokenType::SymbolBracketSquareRight:
    case TokenType::Undefined:
    default:
        throw InternalParsingException(
            "parse_expression, " + stringify(token.type), token.position);
    }
}

ast::Atom Parser::parse_atom(const Token& token)
{
    return Atom(token);
}

ast::Atom Parser::parse_keyword(const Token& token)
{
    if (!_scanner) {
        throw UnfinishedInput("unfinished keyword", token.position);
    }

    auto next_token = _scanner.get();
    auto pos_diff = next_token.position - token.position;
    if (next_token.type != TokenType::Identifier ||
        !(pos_diff.line == 0 && pos_diff.column == 1))
    {
        throw IllegalKeyword();
    }

    const auto& content = next_token.content;
    // BOOLEAN:TRUE
    if (content == "t" || content == "true") {
        return Atom(AtomType::Boolean, "t");
    }
    // BOOLEAN:FALSE
    else if (content == "f" || content == "false") {
        return Atom(AtomType::Boolean, "f");
    }
    // NUMBER
    else if (content == "inf" || content == "+inf" || content == "-inf" ||
             content == "nan" || content == "+nan" || content == "-nan")
    {
        return Atom(AtomType::Number, content);
    }
    // NIL
    else if (content == "nil") {
        return Atom(AtomType::Nil, content);
    }
    // Undefined Keyword
    else {
        throw IllegalKeyword(content);
    }
}

ast::List Parser::parse_quote_expand(const Token& token)
{
    List result;

    // QUOTE
    if (token.type == TokenType::SymbolQuote) {
        result.push_back(_atom_helper.create_by_type(AtomType::Quote));
    }
    // QUASIQUOTE
    else if (token.type == TokenType::SymbolBackquote) {
        result.push_back(_atom_helper.create_by_type(AtomType::Quasiquote));
    }
    // UNQUOTE
    else if (token.type == TokenType::SymbolComma) {
        result.push_back(_atom_helper.create_by_type(AtomType::Unquote));
    }
    else if (token.type == TokenType::SymbolCommaAt) {
        result.push_back(
            _atom_helper.create_by_type(AtomType::UnquoteSplicing));
    }

    if (!_scanner) {
        throw UnfinishedInput(
            "unfinished quote / quasiquote / unquote delcaration",
            token.position);
    }

    while (_scanner) {
        auto t = _scanner.get();
        if (t.type == TokenType::Whitespace || t.type == TokenType::Comment) {
            continue;
        }
        else {
            result.push_back(parse_expression(t));
            break;
        }
    }

    return result;
}

ast::List Parser::parse_list(const Token& token)
{
    List result;

    TokenType close_bracket = TokenType::Undefined;

    // Round
    if (token.type == TokenType::SymbolBracketRoundLeft) {
        close_bracket = TokenType::SymbolBracketRoundRight;
    }
    // Square
    else if (token.type == TokenType::SymbolBracketSquareLeft) {
        close_bracket = TokenType::SymbolBracketSquareRight;
    }
    // Curly
    else if (token.type == TokenType::SymbolBracketCurlyLeft) {
        close_bracket = TokenType::SymbolBracketCurlyRight;
        result.is_textlist = true;
    }

    while (_scanner) {
        auto t = _scanner.get();
        if (t.type == TokenType::Whitespace || t.type == TokenType::Comment) {
            continue;
        }
        else if (t.type == close_bracket) {
            return result;
        }
        else {
            result.push_back(parse_expression(t));
        }
    }
    throw UnclosedList(stringify(close_bracket));
}

ast::Expression Parser::parse_text_element(const Token& token)
{
    if (token.type == TokenType::TextContent) {
        return Atom(AtomType::String, token.content);
    }

    if (token.type == TokenType::TextEmptyLine) {
        List result;
        result.push_back(_atom_helper.create_by_type(AtomType::Quote));
        result.push_back(_atom_helper.create_emptyline_symbol());
        return result;
    }

    throw InternalParsingException(
        "parse_text_element, " + stringify(token.type), token.position);
}

} // namespace libdocscript