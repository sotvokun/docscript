#ifndef LIBDOCSCRIPT_TOKENSTREAM_H
#define LIBDOCSCRIPT_TOKENSTREAM_H
#include "docscript.h"
#include "libdocscript/misc.h"
#include "libdocscript/token.h"
#include <string>
#include <stack>

namespace docscript::libdocscript {
class TokenStream final {
    enum StreamStatus { EoF = -1, Good = 0 };

    enum ProcessStatus {
        Begin,
        InComment,
        InOperator,
        InInteger,
        InFloat,
        InHashSymbol,
        InBooleanTrue,
        InBooleanFalse,
        InSimpleName,
        InComplexName,
        InComplexNameEnd,
        InString,
        InEscapeSequence,
        InStringEnd,
        InSymbol,
        InText,
        InTextEscape,
    };

    enum ProcessMode {
        TextMode,
        EmbeddingMode
    };

    enum EscapeStatus {
        EscapeInString,
        EscapeInName
    };

  public:
    using char_type = unsigned char;

    operator bool() const;

    TokenStream() : _status(EoF), _pstatus(Begin), _pos{0, 0} {}
    TokenStream(const std::string &str)
        : _stream(str), _status(Good), _pstatus(Begin), _pos{1, 1} {
        _streamBeg = _stream.cbegin();
        _streamEnd = _stream.cend();
    }

    inline TokenStream &operator=(const TokenStream &ts) {
        _stream = ts._stream;
        _status = Good;
        _pstatus = Begin;
        _pos = {1, 1};
        _streamBeg = _stream.cbegin();
        _streamEnd = _stream.cend();
        return *this;
    }
    ~TokenStream() = default;

    ///
    /// \brief Extracts the token from the stream
    ///
    TokenStream &get(Token &token);

    ///
    /// \brief Extracts the token from the stream
    ///
    Token get();

    ///
    /// \brief Unextracts the token
    ///
    TokenStream &unget();

    ///
    /// \brief Get the token without extracting
    ///
    Token peek();

    ///
    /// \brief Reset the TokenStream status to default
    ///
    void reset();

  private:
    ///
    /// \brief Extracts the character from the string
    ///
    char_type get_char();

    ///
    /// \brief Get the character without extracting
    ///
    char_type peek_char();

    ///
    /// \brief Unextracts the character
    ///
    void unget_char();

    ///
    /// \brief Extracts the character from the string (ALIAS get_char)
    ///
    void next_char();

    ///
    /// \brief Check whether current character is last
    ///
    bool is_last();

    ///
    /// \brief Set the mode to TextMode
    ///
    void set_textmode();

    ///
    /// \brief Set the mode to EmbeddingMode
    ///
    void set_embeddingmode();

    ///
    /// \brief Unset current mode
    ///
    void unset_mode();

    ///
    /// \brief Get current mode
    ///
    ProcessMode current_mode();

    ///
    /// \brief Check whether the special mode is enable
    ///
    bool mode();

    std::string _stream;
    StreamStatus _status;
    ProcessStatus _pstatus;
    std::stack<ProcessMode> _mode;

    Token _last_token;
    bool _unget_token_status = false;

    Position _pos;
    size_type _prev_line_column;

    std::string::const_iterator _streamBeg;
    std::string::const_iterator _streamEnd;
};
} // namespace libdocscript

#endif