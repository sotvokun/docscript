#ifndef LIBDOCSCRIPT_TOKEN_H
#define LIBDOCSCRIPT_TOKEN_H
#include "libdocscript/utility/position.h"
#include "libdocscript/utility/stringstream.h"
#include <string>

namespace libdocscript {
enum class TokenType
{
    Undefined,

    Whitespace,

    Comment,
    Identifier,
    Boolean,
    String,
    Number,

    TextContent,
    TextEmptyLine,

    SymbolBracketRoundLeft,
    SymbolBracketRoundRight,
    SymbolBracketCurlyLeft,
    SymbolBracketCurlyRight,
    SymbolBracketSquareLeft,
    SymbolBracketSquareRight,

    SymbolQuote,
    SymbolBackquote,
    SymbolComma,
    SymbolCommaAt,

    SymbolHash,
};

class Token final
{
public:
    Token() : type(TokenType::Undefined) {}
    Token(TokenType t, Position pos) : type(t), position(pos) {}
    Token(TokenType t, const std::string& str, Position pos)
        : type(t), content(str), position(pos)
    {}
    ~Token() = default;

    inline bool operator==(TokenType t)
    {
        return type == t;
    }
    inline bool operator!=(TokenType t)
    {
        return !operator==(t);
    }

    TokenType type;
    std::string content;
    Position position;

    inline const TokenType& type_ref() const
    {
        return type;
    }
    inline const std::string& content_ref() const
    {
        return content;
    }
    inline const Position& position_ref() const
    {
        return position;
    }

    inline bool is_delimiter() const
    {
        switch (type) {
        case TokenType::Whitespace:
        case TokenType::String:
        case TokenType::SymbolBracketRoundLeft:
        case TokenType::SymbolBracketRoundRight:
        case TokenType::SymbolBracketCurlyLeft:
        case TokenType::SymbolBracketCurlyRight:
        case TokenType::SymbolBracketSquareLeft:
        case TokenType::SymbolBracketSquareRight:
        case TokenType::Comment:
            return true;

        default:
            return false;
        }
    }

    inline static bool is_delimiter(const Token& token)
    {
        return token.is_delimiter();
    }
};

inline std::string stringify(TokenType t)
{
    switch (t) {
    case TokenType::Undefined:
        return "???";
    case TokenType::Whitespace:
        return "SPC";
    case TokenType::Comment:
        return "CMT";
    case TokenType::Identifier:
        return "IDT";
    case TokenType::Boolean:
        return "BOL";
    case TokenType::String:
        return "STR";
    case TokenType::Number:
        return "NUM";
    case TokenType::TextContent:
        return "TXT";
    case TokenType::TextEmptyLine:
        return "EMT";
    case TokenType::SymbolBracketRoundLeft:
        return "(";
    case TokenType::SymbolBracketRoundRight:
        return ")";
    case TokenType::SymbolBracketCurlyLeft:
        return "{";
    case TokenType::SymbolBracketCurlyRight:
        return "}";
    case TokenType::SymbolBracketSquareLeft:
        return "[";
    case TokenType::SymbolBracketSquareRight:
        return "]";
    case TokenType::SymbolQuote:
        return "'";
    case TokenType::SymbolBackquote:
        return "`";
    case TokenType::SymbolComma:
        return ",";
    case TokenType::SymbolHash:
        return "#";
    case TokenType::SymbolCommaAt:
        return ",@";
    default:
        return "___";
    }
}
} // namespace libdocscript

#endif