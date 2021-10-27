#ifndef LIBDOCSCRIPT_TOKEN_H
#define LIBDOCSCRIPT_TOKEN_H
#include "libdocscript/misc.h"
#include <string>

namespace docscript::libdocscript {
enum class TokenType {
    Undefined = -1,
    EoF,
    Comment,
    IntNumber,
    FloatNumber,
    Identifier,
    BooleanTrue,
    BooleanFalse,
    String,
    Text,
    EmptyLine,

    SymbolBracketL = 10,
    SymbolBracketR,
    SymbolCurlyL,
    SymbolCurlyR,
    SymbolRoundL,
    SymbolRoundR,
    SymbolQuote,
};

class Token final {
  public:
    Token() : Token(TokenType::Undefined, {0, 0}, "") {}
    Token(TokenType t, Position p) : Token(t, p, "") {}
    Token(TokenType t, Position p, const std::string &content)
        : _type(t), _pos(p), _content(content) {}
    ~Token() = default;

    inline const Position &position() const { return _pos; }
    inline const std::string &content() const { return _content; }
    inline const TokenType &type() const { return _type; }

  private:
    Position _pos;
    std::string _content;
    TokenType _type;
};
} // namespace libdocscript

#endif