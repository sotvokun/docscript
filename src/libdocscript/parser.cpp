#include "libdocscript/parser.h"
#include "libdocscript/ast.h"
#include "libdocscript/exception.h"
#include "libdocscript/misc.h"
#include "libdocscript/token.h"
#include "libdocscript/tokenstream.h"
#include <string>
#include <vector>

namespace docscript::libdocscript {

// PUBLIC

std::vector<ast::SExpression> Parser::parse(const std::string &str) {
    _stream = TokenStream(str);
    _init_status = true;
    std::vector<ast::SExpression> result;
    while (_stream) {
        result.push_back(parse_sexpr());
    }
    return result;
}

// PRIVATE

ast::SExpression Parser::parse_sexpr(bool inner) {
    Token token = current();
    switch (token.type()) {
    // -> Comment
    case TokenType::Comment: {
        return ast::SExpression(parse_comment());
    } break;

    // -> Atom
    case TokenType::SimpleName:
    case TokenType::ComplexName:
    case TokenType::Integer:
    case TokenType::Decimal:
    case TokenType::BooleanTrue:
    case TokenType::BooleanFalse:
    case TokenType::String: {
        return ast::SExpression(parse_atom());
    } break;

    // -> Text
    case TokenType::BracketCurlyL: {
        return ast::SExpression(parse_text());
    } break;

    // -> RoundList
    case TokenType::BracketRoundL: {
        return ast::SExpression(parse_roundlist());
    } break;

    // -> SquareList
    case TokenType::BracketSquareL: {
        return ast::SExpression(parse_squarelist());
    } break;

    // -> Quote
    case TokenType::SymbolQuote: {
        return ast::SExpression(parse_quote());
    } break;

    // -> Quasiquote
    case TokenType::SymbolBackquote: {
        return ast::SExpression(parse_quasiquote());
    } break;

    // -> Unquote
    case TokenType::SymbolComma: {
        return ast::SExpression(parse_unquote());
    } break;

    // THROW: Undefined syntax rule
    case TokenType::Undefined: {
        throw ParserUndefinedSyntaxException(token.content(), token.position());
    } break;

    // THROW: EOF
    case TokenType::EoF: {
        if (inner) {
            throw ParserIncompleteException(token.position());
        }
        throw ParserEOF();
    } break;

    // THROW: Illegal syntax
    default: {
        throw ParserInternalException(
            "Unknown token type detcted while parsing s-expression.",
            token.position());
    } break;
    }
}

ast::Comment Parser::parse_comment() {
    Token token = current();
    next();
    return ast::Comment(token.content(), token.position());
}

ast::Atom Parser::parse_atom() {
    Token token = current();
    switch (token.type()) {
    case TokenType::SimpleName:
    case TokenType::ComplexName:
        next();
        return ast::Atom(ast::AtomType::Name, token.content(),
                         token.position());

    case TokenType::Integer:
        next();
        return ast::Atom(std::stol(token.content()), token.position());

    case TokenType::Decimal:
        next();
        return ast::Atom(std::stod(token.content()), token.position());

    case TokenType::BooleanTrue:
        next();
        return ast::Atom(true, token.position());

    case TokenType::BooleanFalse:
        next();
        return ast::Atom(false, token.position());

    case TokenType::String:
        next();
        return ast::Atom(ast::AtomType::String, token.content(),
                         token.position());

    default:
        // TODO: THROW Internal Error Bad Status
        throw ParserInternalException(
            "Unknown token type detcted while parsing atom.", token.position());
    }
}

ast::Text Parser::parse_text() {
    Token curly_l = current();
    Token token = get();
    ast::Text result(curly_l.position());
    bool content_line = false;
    while (token.type() != TokenType::BracketCurlyR) {
        // -> TextString
        if (token.type() == TokenType::Text) {
            if (!content_line)
                content_line = true;
            result.value.push_back(new ast::TextString(parse_textstring()));
        }
        // -> TextLineEnd
        else if (token.type() == TokenType::EmptyLine) {
            if (content_line) {
                ast::TextLineEnd lineend = parse_textlineend();
                if (current().type() != TokenType::BracketCurlyR) {
                    result.value.push_back(new ast::TextLineEnd(lineend));
                }
            }
        }
        // -> SExpression
        else if (token.type() == TokenType::BracketSquareL) {
            result.value.push_back(new ast::SExpression(parse_sexpr(true)));
        }
        // -> THROW: Incomplete exception
        else if (token.type() == TokenType::EoF) {
            throw ParserIncompleteException(token.position());
        }
        // -> THROW: Illegal syntax
        else {
            throw ParserInternalException(
                "Unknown token type detcted while parsing text",
                token.position());
        }
        token = get();
    }
    next();
    return result;
}

ast::TextString Parser::parse_textstring() {
    std::string textstr;
    Token token = current();
    Position first = token.position();
    while (token.type() == TokenType::Text) {
        textstr.append(string_trim(token.content()));
        token = get();
    }
    unget();
    return ast::TextString(textstr, first);
}

ast::TextLineEnd Parser::parse_textlineend() {
    Token token = current();
    Position first = token.position();
    while (token.type() == TokenType::EmptyLine) {
        token = get();
    }
    unget();
    return ast::TextLineEnd(first);
}

ast::RoundList Parser::parse_roundlist() {
    Token round_l = current();
    Token token = get();
    ast::RoundList result(round_l.position());
    while (token.type() != TokenType::BracketRoundR) {
        if (auto res = parse_sexpr(true);
            res.type_sexpr != ast::SExpressionType::Comment) {
            result.value.push_back(new ast::SExpression(res));
        }
        token = current();
    }
    next();
    return result;
}

ast::SquareList Parser::parse_squarelist() {
    Token bracket_l = current();
    Token token = get();
    ast::SquareList result(bracket_l.position());
    while (token.type() != TokenType::BracketSquareR) {
        if (auto res = parse_sexpr(true);
            res.type_sexpr != ast::SExpressionType::Comment) {
            result.value.push_back(new ast::SExpression(res));
        }
        token = current();
    }
    next();
    return result;
}

ast::Quote Parser::parse_quote() {
    Token symbol = current();
    next();
    auto sexpr = parse_sexpr(true);
    while (sexpr.type_sexpr == ast::SExpressionType::Comment) {
        sexpr = parse_sexpr(true);
    }
    return ast::Quote(new ast::SExpression(sexpr), symbol.position());
}

ast::Quasiquote Parser::parse_quasiquote() {
    Token symbol = current();
    next();
    auto sexpr = parse_sexpr(true);
    while (sexpr.type_sexpr == ast::SExpressionType::Comment) {
        sexpr = parse_sexpr(true);
    }
    return ast::Quasiquote(new ast::SExpression(sexpr), symbol.position());
}

ast::Unquote Parser::parse_unquote() {
    Token symbol = current();
    next();
    auto sexpr = parse_sexpr(true);
    while (sexpr.type_sexpr == ast::SExpressionType::Comment) {
        sexpr = parse_sexpr(true);
    }
    return ast::Unquote(new ast::SExpression(sexpr), symbol.position());
}

// STREAM CONTROL

Token Parser::get() {
    _stream.get(_last_token);
    return _last_token;
}

Token Parser::current() {
    if (_init_status) {
        _stream.get(_last_token);
        _init_status = false;
    }
    return _last_token;
}

void Parser::unget() { _stream.unget(); }

void Parser::next() { get(); }
} // namespace docscript::libdocscript