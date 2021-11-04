#include "libdocscript/tokenstream.h"
#include "libdocscript/exception.h"
#include "libdocscript/misc.h"
#include "libdocscript/token.h"
#include <algorithm>
#include <string>

namespace docscript::libdocscript {

// FRIEND

// PUBLIC

TokenStream::operator bool() const { return _status == Good; }

Token TokenStream::get() {
    if (_unget_token_status) {
        _unget_token_status = false;
        return _last_token;
    }

    std::string tmpstr;
    Position tmppos;
    EscapeStatus esc_status;

    bool after_point_num = false;

    while (_status == Good) {
        char_type ch = peek_char();
        switch (_pstatus) {
        case Begin: {
            // Skip the space and empty character
            if (std::isspace(ch)) {
                next_char();
            } else if (ch == 0) {
                _status = EoF;
            }
            // --> InComment
            else if (ch == ';') {
                _pstatus = InComment;
                tmppos = _pos;
                next_char();
            }
            // --> InOperator
            else if (ch == '+' || ch == '-') {
                _pstatus = InOperator;
                tmppos = _pos;
                tmpstr.push_back(ch);
                next_char();
            }
            // --> InInteger
            else if (ch >= '0' && ch <= '9') {
                _pstatus = InInteger;
                tmppos = _pos;
                tmpstr.push_back(ch);
                next_char();
            }
            // --> InHashSymbol
            else if (ch == '#') {
                _pstatus = InHashSymbol;
                tmppos = _pos;
                next_char();
            }
            // --> InString
            else if (ch == '"') {
                _pstatus = InString;
                tmppos = _pos;
                next_char();
            }
            // --> InSymbol
            else if (ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
                     ch == '(' || ch == ')' || ch == '\'' || ch == '`' ||
                     ch == ',') {
                _pstatus = InSymbol;
                tmppos = _pos;
            }
            // --> InSimpleName
            else if (!contain("[](){}+-#|`,\'", ch) && !std::isspace(ch) &&
                     !(ch >= '0' && ch <= '9') && !is_last()) {
                _pstatus = InSimpleName;
                tmppos = _pos;
                tmpstr.push_back(ch);
                next_char();
            }
            // --> InComplexName
            else if (ch == '|') {
                _pstatus = InComplexName;
                tmppos = _pos;
                next_char();
            }
            // --> Undefined Character
            else {
                std::string tmp;
                tmpstr.push_back(ch);
                _last_token = Token(TokenType::Undefined, _pos, tmpstr);
                next_char();
                return _last_token;
            }
        } break;
        case InComment: {
            // -> Begin => RETURN
            if (ch == '\n' || is_last()) {
                _pstatus = Begin;
                _last_token = Token(TokenType::Comment, tmppos, tmpstr);
                return _last_token;
            }
            // ->O<-
            else {
                tmpstr.push_back(ch);
                next_char();
            }
        } break;
        case InOperator: {
            // -> InInteger
            if (ch >= '0' && ch <= '9') {
                _pstatus = InInteger;
            }
            // -> InSimpleName
            else {
                _pstatus = InSimpleName;
            }
        } break;
        case InInteger: {
            // ->O<-
            if (ch >= '0' && ch <= '9') {
                tmpstr.push_back(ch);
                next_char();
            }
            // -> InFloat
            else if (ch == '.') {
                _pstatus = InFloat;
                tmpstr.push_back(ch);
                next_char();
            }
            // -> Begin => RETURN
            else if (std::isspace(ch) || is_last() || contain("()[]{}", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::Integer, tmppos, tmpstr);
                return _last_token;
            }
            // -> InSimpleName
            else {
                _pstatus = InSimpleName;
            }
        } break;
        case InFloat: {
            // ->O<-
            if (ch >= '0' && ch <= '9') {
                tmpstr.push_back(ch);
                after_point_num = true;
                next_char();
            }
            // -> Begin => RETURN
            else if (std::isspace(ch) || is_last() || contain("()[]{}", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::Decimal, tmppos, tmpstr);
                return _last_token;
            }
            // -> InSimpleName
            else {
                _pstatus = InSimpleName;
            }
        } break;
        case InHashSymbol: {
            // -> InBooleanTrue
            if (ch == 't') {
                _pstatus = InBooleanTrue;
                next_char();
            }
            // -> InBooleanFalse
            else if (ch == 'f') {
                _pstatus = InBooleanFalse;
                next_char();
            }
            // THROW Illegal Token
            else {
                std::string tmp;
                tmp.push_back(ch);
                throw TokenStreamUnknownTokenException(tmp, _pos);
            }
        } break;
        case InBooleanTrue: {
            // -> Begin => RETURN
            if (std::isspace(ch) || is_last() || contain("[]{}()", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::BooleanTrue, tmppos);
                return _last_token;
            }
            // THROW Illegal Token
            else {
                std::string tmp;
                tmp.push_back(ch);
                throw TokenStreamUnknownTokenException(tmp, _pos);
            }
        } break;
        case InBooleanFalse: {
            // -> Begin => RETURN
            if (std::isspace(ch) || is_last() || contain("[]{}()", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::BooleanFalse, tmppos);
                return _last_token;
            }
            // THROW Illegal Token
            else {
                std::string tmp;
                tmp.push_back(ch);
                throw TokenStreamUnknownTokenException(tmp, _pos);
            }
        } break;
        case InString: {
            // -> InStringEnd
            if (ch == '"') {
                _pstatus = InStringEnd;
                next_char();
            }
            // -> InEscapeSequence
            else if (ch == '\\') {
                _pstatus = InEscapeSequence;
                esc_status = EscapeInString;
                next_char();
            }
            // ->O<-
            else {
                tmpstr.push_back(ch);
                next_char();
            }
        } break;
        case InEscapeSequence: {
            char escape;
            if (contain("\"\'\\\?", ch)) {
                escape = ch;
            } else if (ch == 'a') {
                escape = '\a';
            } else if (ch == 'b') {
                escape = '\b';
            } else if (ch == 'f') {
                escape = '\f';
            } else if (ch == 'n') {
                escape = '\n';
            } else if (ch == 'r') {
                escape = '\r';
            } else if (ch == 't') {
                escape = '\t';
            } else if (ch == 'v') {
                escape = '\v';
            } else {
                escape = ch;
            }
            // -> InString
            tmpstr.push_back(escape);
            if (esc_status == EscapeInString) {
                _pstatus = InString;
            } else {
                _pstatus = InComplexName;
            }
            next_char();
        } break;
        case InStringEnd: {
            // -> Begin => RETURN
            if (std::isspace(ch) || is_last() || contain("[]{}()", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::String, tmppos, tmpstr);
                return _last_token;
            }
            // THROW Illegal Token
            else {
                throw TokenStreamUnknownTokenException("\"" + ch, _pos);
            }
        } break;
        case InSymbol: {
            TokenType ttype;
            ProcessStatus pstat = Begin;
            switch (ch) {
            case '[':
                ttype = TokenType::BracketSquareL;
                if (mode()) {
                    set_embeddingmode();
                }
                pstat = Begin;
                break;
            case ']':
                ttype = TokenType::BracketSquareR;
                if (mode()) {
                    unset_mode();
                    pstat = ((mode() && current_mode() == TextMode) ? InText
                                                                    : Begin);
                } else {
                    pstat = Begin;
                }
                break;
            case '{':
                ttype = TokenType::BracketCurlyL;
                set_textmode();
                pstat = InText;
                break;
            case '}':
                ttype = TokenType::BracketCurlyR;
                if (mode()) {
                    unset_mode();
                    pstat = ((mode() && current_mode() == TextMode) ? InText
                                                                    : Begin);
                } else {
                    pstat = Begin;
                }
                break;
            case '(':
                ttype = TokenType::BracketRoundL;
                break;
            case ')':
                ttype = TokenType::BracketRoundR;
                break;
            case '\'':
                ttype = TokenType::SymbolQuote;
                break;
            case '`':
                ttype = TokenType::SymbolBackquote;
                break;
            case ',':
                ttype = TokenType::SymbolComma;
                break;
            }
            // (-> Begin | -> InText) => RETURN
            _pstatus = pstat;
            _last_token = Token(ttype, tmppos);
            next_char();
            return _last_token;
        } break;
        case InSimpleName: {
            // ->O<-
            if (!std::isspace(ch) && !is_last() &&
                !contain("[](){}#`,\'", ch)) {
                tmpstr.push_back(ch);
                next_char();
            }
            // -> Begin => RETURN
            else if (std::isspace(ch) || is_last() || contain("[](){}", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::SimpleName, tmppos, tmpstr);
                return _last_token;
            }
            // THROW Illegal Token
            else {
                tmpstr.push_back(ch);
                throw TokenStreamUnknownTokenException(tmpstr, tmppos);
            }
        } break;
        case InComplexName: {
            // -> InComplexNameEnd
            if (ch == '|') {
                _pstatus = InComplexNameEnd;
                next_char();
            }
            // -> InEscapeSequence
            else if (ch == '\\') {
                _pstatus = InEscapeSequence;
                esc_status = EscapeInName;
                next_char();
            }
            // THROW Illegal Token
            else if ((std::isspace(ch) && ch != ' ') || contain("[]{}()", ch)) {
                throw TokenStreamUnknownTokenException(tmpstr, tmppos);
            }
            // ->O<-
            else {
                tmpstr.push_back(ch);
                next_char();
            }
        } break;
        case InComplexNameEnd: {
            // -> Begin => RETURN
            if (std::isspace(ch) || is_last() || contain("[]{}()", ch)) {
                _pstatus = Begin;
                _last_token = Token(TokenType::ComplexName, tmppos, tmpstr);
                return _last_token;
            }
            // THROW Illegal Token
            else {
                throw TokenStreamUnknownTokenException("|" + ch, _pos);
            }
        } break;
        case InText: {
            // -> Begin => RETURN
            if (contain("[]{}", ch)) {
                _pstatus = Begin;
                if (tmpstr.size() == 0 || isspace(tmpstr)) {
                    _last_token = Token(TokenType::EmptyLine,
                                        locate_position(_pos, tmpstr));
                } else {
                    _last_token = Token(TokenType::Text,
                                        locate_position(_pos, tmpstr), tmpstr);
                }
                return _last_token;
            }
            // -> InText => RETURN
            else if (ch == '\n') {
                _pstatus = InText;
                if (tmpstr.size() == 0 || isspace(tmpstr)) {
                    _last_token = Token(TokenType::EmptyLine,
                                        locate_position(_pos, tmpstr));
                } else {
                    _last_token = Token(TokenType::Text,
                                        locate_position(_pos, tmpstr), tmpstr);
                }
                next_char();
                return _last_token;
            }
            // -> InTextEscape
            else if (ch == '\\') {
                _pstatus = InTextEscape;
                next_char();
            }
            // ->O<-
            else {
                tmpstr.push_back(ch);
                next_char();
            }
        } break;
        case InTextEscape: {
            tmpstr.push_back(ch);
            // -> InText
            _pstatus = InText;
            next_char();
        } break;
        default:
            break;
        }
    }
    return Token(TokenType::EoF, _pos);
}

TokenStream &TokenStream::get(Token &token) {
    token = get();
    return *this;
}

TokenStream &TokenStream::unget() {
    if (_last_token.position().column != 0 &&
        _last_token.position().line != 0 && !_unget_token_status) {
        _unget_token_status = true;
    }
    return *this;
}

Token TokenStream::peek() {
    auto token = get();
    unget();
    return token;
}

void TokenStream::reset() {
    _pos.line = _pos.column = 1;
    _status = TokenStream::Good;
    _pstatus = TokenStream::Begin;
    _streamBeg = _stream.cbegin();
}

// PRIVATE

TokenStream::char_type TokenStream::get_char() {
    if (_streamBeg != _streamEnd) {
        if (*_streamBeg == '\n') {
            ++_pos.line;
            _prev_line_column = _pos.column;
            _pos.column = 0;
        }
        ++_pos.column;
        return *_streamBeg++;
    } else {
        _status = TokenStream::EoF;
        return 0;
    }
}

void TokenStream::unget_char() {
    if (_streamBeg != _stream.begin()) {
        --_streamBeg;
        if (*_streamBeg == '\n') {
            --_pos.line;
            _pos.column = _prev_line_column;
        } else {
            --_pos.column;
        }
    }
}

void TokenStream::next_char() { get_char(); }

bool TokenStream::is_last() { return _streamBeg == _streamEnd; }

TokenStream::char_type TokenStream::peek_char() {
    return (_streamBeg != _streamEnd) ? *_streamBeg : 0;
}

void TokenStream::set_textmode() { _mode.push(TextMode); }
void TokenStream::set_embeddingmode() { _mode.push(EmbeddingMode); }
void TokenStream::unset_mode() { _mode.pop(); }
TokenStream::ProcessMode TokenStream::current_mode() { return _mode.top(); }
bool TokenStream::mode() { return _mode.size(); }
} // namespace docscript::libdocscript