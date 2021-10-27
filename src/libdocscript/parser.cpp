#include "libdocscript/parser.h"
#include "libdocscript/exception.h"
#include "libdocscript/misc.h"
#include "libdocscript/token.h"
#include "libdocscript/tokenstream.h"
#include <string>
#include <vector>

namespace docscript::libdocscript {

// PUBLIC

std::vector<SExpressionAST> Parser::parse(const std::string &str) {
    _stream = TokenStream(str);
    _init_status = true;
    std::vector<SExpressionAST> result;
    while (_stream) {
        result.push_back(parse_sexpr());
    }
    return result;
}

// PRIVATE

SExpressionAST Parser::parse_sexpr(bool inner) {
    Token token = current();
    switch (token.type()) {
    // -> Atom
    case TokenType::Identifier:
    case TokenType::IntNumber:
    case TokenType::FloatNumber:
    case TokenType::BooleanTrue:
    case TokenType::BooleanFalse:
    case TokenType::String: {
        return SExpressionAST(parse_atom());
    } break;

    // -> Symbol -> Atom
    case TokenType::SymbolQuote: {
        return SExpressionAST(parse_symbol());
    } break;

    // -> Text
    case TokenType::SymbolCurlyL: {
        return SExpressionAST(parse_text());
    } break;

    // -> List
    case TokenType::SymbolRoundL: {
        return SExpressionAST(parse_list());
    } break;

    // -> SExpressionList
    case TokenType::SymbolBracketL: {
        return SExpressionAST(parse_sexprlist());
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

AtomAST Parser::parse_atom() {
    Token token = current();
    switch (token.type()) {
    case TokenType::Identifier:
        next();
        return AtomAST::Identifier(token.content(), token.position());

    case TokenType::IntNumber:
        next();
        return AtomAST::Integer(std::stol(token.content()), token.position());

    case TokenType::FloatNumber:
        next();
        return AtomAST::Float(std::stod(token.content()), token.position());

    case TokenType::BooleanTrue:
        next();
        return AtomAST::Boolean(true, token.position());

    case TokenType::BooleanFalse:
        next();
        return AtomAST::Boolean(false, token.position());

    case TokenType::String:
        next();
        return AtomAST::String(token.content(), token.position());

    default:
        throw ParserInternalException(
            "Unknown token type detcted while parsing atom.", token.position());
    }
}

AtomAST Parser::parse_symbol() {
    Token quote = current();
    Token identifier = get();

    if (identifier.type() != TokenType::Identifier) {
        throw ParserIllegalSyntaxException("Symbol only use for identifier",
                                           quote.position());
    }
    if (identifier.position().line != quote.position().line &&
        identifier.position().column != quote.position().column + 1) {
        throw ParserIllegalSyntaxException("Quote mark must near by identifier",
                                           quote.position());
    }
    next();
    return AtomAST::Symbol(identifier.content(), quote.position());
}

TextAST Parser::parse_text() {
    Token curly_l = current();
    Token token = get();
    TextAST result(curly_l.position());
    bool content_line = false;
    while (token.type() != TokenType::SymbolCurlyR) {
        // -> TextString
        if (token.type() == TokenType::Text) {
            if (!content_line)
                content_line = true;
            result.value.push_back(parse_textstring().share());
        }
        // -> TextLineEnd
        else if (token.type() == TokenType::EmptyLine) {
            if (content_line) {
                TextLineEndAST lineend = parse_textlineend();
                if (current().type() != TokenType::SymbolCurlyR) {
                    result.value.push_back(lineend.share());
                }
            }
        }
        // -> SExpression
        else if (token.type() == TokenType::SymbolBracketL) {
            result.value.push_back(parse_sexpr(true).share());
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

TextStringAST Parser::parse_textstring() {
    std::string textstr;
    Token token = current();
    Position first = token.position();
    while (token.type() == TokenType::Text) {
        // *TODO* add an option to select there be trimed or not
        textstr.append(string_trim(token.content()));
        token = get();
    }
    unget();
    return TextStringAST(textstr, first);
}

TextLineEndAST Parser::parse_textlineend() {
    Token token = current();
    Position first = token.position();
    while (token.type() == TokenType::EmptyLine) {
        token = get();
    }
    unget();
    return TextLineEndAST(first);
}

ListAST Parser::parse_list() {
    Token round_l = current();
    Token token = get();
    ListAST result(round_l.position());
    while (token.type() != TokenType::SymbolRoundR) {
        result.value.push_back(parse_sexpr(true).share());
        token = current();
    }
    next();
    return result;
}

SExpressionListAST Parser::parse_sexprlist() {
    Token bracket_l = current();
    Token token = get();
    SExpressionListAST result(bracket_l.position());
    while (token.type() != TokenType::SymbolBracketR) {
        result.value.push_back(parse_sexpr(true).share());
        token = current();
    }
    next();
    return result;
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