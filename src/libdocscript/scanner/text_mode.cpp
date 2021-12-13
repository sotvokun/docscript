#include "libdocscript/exception.h"
#include "libdocscript/scanner.h"
#include "libdocscript/utility/stringstream.h"

namespace libdocscript {
Token ScannerTextMode::scan()
{
    char ch = stream.peek();
    Position begin_pos = stream.position();

    // => SymbolBracketCurlyLeft --> +TextMode
    if (ch == '{') {
        enter_mode(Scanner::Mode::Text);
        stream.next();
        return Token(TokenType::SymbolBracketCurlyLeft, begin_pos);
    }
    // => SymbolBracketCurlyRight --> -TextMode
    else if (ch == '}') {
        exit_current_mode();
        stream.next();
        return Token(TokenType::SymbolBracketCurlyRight, begin_pos);
    }
    // => SymbolBracketSqureLeft --> +NormalMode
    else if (ch == '[') {
        enter_mode(Scanner::Mode::Normal);
        stream.next();
        return Token(TokenType::SymbolBracketSquareLeft, begin_pos);
    }
    // Skip the single newline mark that not be first letter of the line.
    else if (ch == '\n' && stream.position().column != 1) {
        stream.next();
        return scan();
    }
    // --> emptyline
    else if (is_whitespace(ch)) {
        std::string content;
        return scan_emptyline(content);
    }
    // --> text
    else {
        std::string content;
        return scan_text_content(content);
    }
}

Token ScannerTextMode::scan_text_content(std::string& content)
{
    Position begin_pos = stream.position();
    while (stream) {
        char ch = stream.peek();

        // -->X
        if (ch == '\n') {
            break;
        }
        // -->X
        else if (ch == '{' || ch == '}' || ch == '[') {
            break;
        }
        // Escape character
        else if (ch == '\\') {
            if (!stream.last()) {
                char peekch = stream.peek_next();
                if (peekch == '{' || peekch == '}' || peekch == '[' ||
                    peekch == ']') {
                    stream.ignore();
                    content.push_back(peekch);
                }
                else {
                    content.push_back('\\');
                }
            }
            else {
                content.push_back(ch);
            }
        }
        // <->
        else {
            content.push_back(ch);
        }
        stream.next();
    }
    auto ch = stream.peek();
    if (ch != '\n' && ch != '{' && ch != '}' && ch != '[') {
        throw UnfinishedInput("unclosed text", stream.position());
    }
    else {
        // Skip the new line mark
        if (ch == '\n') {
            stream.next();
        }
        return Token(TokenType::TextContent, content, begin_pos);
    }
}

Token ScannerTextMode::scan_emptyline(std::string& content)
{
    Position begin_pos = stream.position();
    while (stream) {
        char ch = stream.peek();

        // -->X
        if (ch == '\n') {
            break;
        }
        // -->X | Skip the Empty Text content
        else if (ch == '{' || ch == '}' || ch == '[') {
            return scan();
        }
        // <->
        else if (ch == ' ' || ch == '\t' || ch == '\r') {
            content.push_back(ch);
        }
        // --> textContent
        else {
            Token resultToken = scan_text_content(content);
            return Token(resultToken.type, content, begin_pos);
        }
        stream.next();
    }
    auto ch = stream.peek();
    if (ch != '\n') {
        throw UnfinishedInput("unclosed text", stream.position());
    }
    else {
        // Skip the new line mark
        if (ch == '\n') {
            stream.next();
        }
        return Token(TokenType::TextEmptyLine, begin_pos);
    }
}
} // namespace libdocscript