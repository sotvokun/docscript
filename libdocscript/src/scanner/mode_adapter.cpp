#include "libdocscript/scanner.h"

namespace libdocscript {

// +---------------------+
//   Protected Functions
// +---------------------+

Scanner::Mode
Scanner::ModeAdapter::current_mode()
{
    return _scanner.current_mode();
}

void
Scanner::ModeAdapter::enter_mode(Mode mode)
{
    _scanner.enter_mode(mode);
}

void
Scanner::ModeAdapter::exit_current_mode()
{
    _scanner.exit_current_mode();
}

// +---------------------+
//    Static Functions
// +---------------------+

bool
Scanner::ModeAdapter::is_digit(char ch, bool include_zero)
{
    if (include_zero)
        return ch >= '0' && ch <= '9';
    else
        return ch >= '1' && ch <= '9';
}

bool
Scanner::ModeAdapter::is_whitespace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool
Scanner::ModeAdapter::is_delimiter(char ch)
{
    return ch == '\"' || ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
           ch == '[' || ch == ']' || is_whitespace(ch) || ch == ';';
}

bool
Scanner::ModeAdapter::is_symbol(char ch)
{
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' ||
           ch == ']' || ch == '\'' || ch == '`' || ch == ',' || ch == '#';
}

bool
Scanner::ModeAdapter::is_valid_letter(char ch)
{
    return !is_whitespace(ch) && !is_symbol(ch) && ch != '\"' && ch != '|' &&
           ch != ';' && !is_digit(ch);
}

bool
Scanner::ModeAdapter::is_subsequent_letter(char ch)
{
    return is_valid_letter(ch) || is_digit(ch) || ch == '+' || ch == '-';
}
} // namespace libdocscript