#include "libdocscript/exception.h"
#include "libdocscript/scanner.h"
#include "libdocscript/utility/stringstream.h"

namespace libdocscript {

// +---------------------+
//    Private Functions
// +---------------------+

Token
ScannerNormalMode::scan()
{
    auto ch = stream.peek();

    // --> whitespace
    if (is_whitespace(ch)) {
        return scan_whitespace();
    }
    // --> comment
    else if (ch == ';') {
        return scan_comment();
    }
    // --> symbol
    else if (is_symbol(ch)) {
        return scan_symbol();
    }
    // --> string
    else if (ch == '"') {
        return scan_string();
    }
    // --> stringlike-identifier
    else if (ch == '|') {
        return scan_stringlike_identifier();
    }
    // --> sign
    else if (ch == '+' || ch == '-') {
        std::string content;
        return scan_sign(content);
    }
    // --> number
    else if (is_digit(ch)) {
        std::string content;
        return scan_integer(content);
    }
    // --> identifier
    else {
        std::string content;
        return scan_id_valid_letter(content);
    }
}

Token
ScannerNormalMode::scan_whitespace()
{
    Position begin_pos = stream.position();
    while (stream.next()) {
        // -->X
        if (!is_whitespace(stream.peek())) {
            break;
        }
    }
    return Token(TokenType::Whitespace, begin_pos);
}

Token
ScannerNormalMode::scan_symbol()
{
    Position begin_pos = stream.position();
    TokenType symbol;
    switch (stream.get()) {
        case '(':
            symbol = TokenType::SymbolBracketRoundLeft;
            break;
        case ')':
            symbol = TokenType::SymbolBracketRoundRight;
            break;
        // => SymbolBracketCurlyLeft --> +TextMode
        case '{':
            symbol = TokenType::SymbolBracketCurlyLeft;
            enter_mode(Scanner::Mode::Text);
            break;
        case '}':
            symbol = TokenType::SymbolBracketCurlyRight;
            break;
        // => SymbolBracketSquareLeft --> +NormalMode
        case '[':
            symbol = TokenType::SymbolBracketSquareLeft;
            enter_mode(Scanner::Mode::Normal);
            break;
        // => SymbolBracketSquareRight --> -NormalMode
        case ']':
            symbol = TokenType::SymbolBracketSquareRight;
            exit_current_mode();
            break;
        case '\'':
            symbol = TokenType::SymbolQuote;
            break;
        case '`':
            symbol = TokenType::SymbolBackquote;
            break;
        case ',':
            if (stream && stream.peek() == '@') {
                stream.ignore();
                symbol = TokenType::SymbolCommaAt;
            } else {
                symbol = TokenType::SymbolComma;
            }
            break;
        case '#':
            symbol = TokenType::SymbolHash;
            break;
    }
    return Token(symbol, begin_pos);
}

Token
ScannerNormalMode::scan_comment()
{
    Position begin_pos = stream.position();
    std::string content;
    content.push_back(stream.get());
    while (stream.next()) {
        auto ch = stream.peek();
        // -->X
        if (ch == '\n') {
            break;
        }
        // <->
        else {
            content.push_back(ch);
        }
    }
    return Token(TokenType::Comment, content, begin_pos);
}

Token
ScannerNormalMode::scan_string()
{
    Position begin_pos = stream.position();
    std::string content;
    while (stream.next()) {
        char ch = stream.peek();
        // -->X
        if (ch == '"') {
            break;
        }
        // --> escape sequence
        else if (ch == '\\') {
            content.push_back(escape_seq());
        }
        // <->
        else {
            content.push_back(ch);
        }
    }
    if (stream.peek() != '"') {
        throw UnfinishedInput("unclosed string", stream.position());
    } else {
        stream.ignore(); // Skip the close double quote
        return Token(TokenType::String, content, begin_pos);
    }
}

char
ScannerNormalMode::escape_seq()
{
    // Skip `\\` character
    stream.ignore();
    if (stream) {
        char ch = stream.peek();
        switch (ch) {
            case 'a':
                return '\a';
            case 'b':
                return '\b';
            case 'f':
                return '\f';
            case 'n':
                return '\n';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            case 'v':
                return '\v';

            case '\'':
            case '\\':
            case '\"':
            case '\?':
            case '|':
                return ch;

            default:
                throw InvalidEscapeSequence(ch, stream.position());
        }
    } else {
        throw UnfinishedInput("incomplete escape sequence", stream.position());
    }
}

Token
ScannerNormalMode::scan_stringlike_identifier()
{
    Position begin_pos = stream.position();
    std::string content;
    while (stream.next()) {
        char ch = stream.peek();
        // -->X
        if (ch == '|') {
            stream.unget();
            break;
        }
        // --> escape sequence
        else if (ch == '\\') {
            content.push_back(escape_seq());
        }
        // <->
        else {
            content.push_back(ch);
        }
        stream.next();
    }
    if (stream.peek() != '|') {
        throw UnfinishedInput("unclosed identifier", begin_pos);
    } else {
        stream.ignore(); // Skip the close vertical bar
        return Token(TokenType::Identifier, content, begin_pos);
    }
}

Token
ScannerNormalMode::scan_sign(std::string& content)
{
    Position begin_pos = stream.position();
    content.push_back(stream.peek());
    while (stream.next()) {
        char ch = stream.peek();
        // -->X | ->> <Identifier>
        if (is_delimiter(ch)) {
            break;
        }
        // --> Integer
        else if (is_digit(ch)) {
            Token resultToken = scan_integer(content);
            return Token(resultToken.type, content, begin_pos);
        }
        // --> Identifier
        else {
            Token resultToken = scan_id_valid_letter(content);
            return Token(resultToken.type, content, begin_pos);
        }
    }
    return Token(TokenType::Identifier, content, begin_pos);
}

Token
ScannerNormalMode::scan_integer(std::string& content)
{
    Position begin_pos = stream.position();
    content.push_back(stream.peek());
    while (stream.next()) {
        char ch = stream.peek();
        // -->X
        if (is_delimiter(ch)) {
            break;
        }
        // --> Decimal
        else if (ch == '.') {
            Token resultToken;
            if (!stream.last() && is_digit(stream.peek())) {
                resultToken = scan_decimal(content);
            } else {
                resultToken = scan_id_valid_letter(content);
            }
            return Token(resultToken.type, content, begin_pos);
        }
        // <->
        else if (is_digit(ch)) {
            content.push_back(ch);
        }
        // --> Identifier
        else {
            Token resultToken = scan_id_valid_letter(content);
            return Token(resultToken.type, content, begin_pos);
        }
    }
    return Token(TokenType::Number, content, begin_pos);
}

Token
ScannerNormalMode::scan_decimal(std::string& content)
{
    Position begin_pos = stream.position();
    content.push_back(stream.peek());
    while (stream.next()) {
        char ch = stream.peek();
        // --> X
        if (is_delimiter(ch)) {
            break;
        }
        // <->
        else if (is_digit(ch)) {
            content.push_back(ch);
        }
        // --> Identifier
        else {
            Token resultToken = scan_id_valid_letter(content);
            return Token(resultToken.type, content, begin_pos);
        }
    }
    return Token(TokenType::Number, content, begin_pos);
}

Token
ScannerNormalMode::scan_id_valid_letter(std::string& content)
{
    Position begin_pos = stream.position();
    if (!is_valid_letter(stream.peek())) {
        throw InvalidIdentifier(stream.peek(), stream.position());
    }
    content.push_back(stream.peek());
    // -->X | ?
    if (stream.next() && !is_delimiter(stream.peek())) {
        char ch = stream.peek();
        // --> Subsequent Letter
        if (is_subsequent_letter(ch)) {
            Token resultToken = scan_id_subsequent(content);
            return Token(resultToken.type, content, begin_pos);
        }
        // THROW: Invalid Identifier
        else {
            throw InvalidIdentifier(stream.peek(), stream.position());
        }
    }
    return Token(TokenType::Identifier, content, begin_pos);
}

Token
ScannerNormalMode::scan_id_subsequent(std::string& content)
{
    content.push_back(stream.peek());
    while (stream.next()) {
        char ch = stream.peek();
        // -->X
        if (is_delimiter(ch)) {
            break;
        }
        // <->
        else if (is_subsequent_letter(ch)) {
            content.push_back(ch);
        }
        // THROW: Invalid Identifier
        else {
            throw InvalidIdentifier(stream.peek(), stream.position());
        }
    }
    return Token(TokenType::Identifier, content, Position());
}
} // namespace libdocscript