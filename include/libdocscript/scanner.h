#ifndef LIBDOCSCRIPT_SCANNER_H
#define LIBDOCSCRIPT_SCANNER_H
#include "libdocscript/token.h"
#include "libdocscript/utility/stringstream.h"
#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

namespace libdocscript {

class Scanner final
{
    friend class ModeAdapter;

public:
    using token_list = std::vector<Token>;

    class ModeAdapter;
    enum class Mode
    {
        Normal,
        Text
    };

    Scanner(StringStream& stream);

    Token get();
    void reset();
    bool done() const;

    operator bool() const;

    static token_list tokenize(StringStream& str);
    static std::string dump(const token_list& list);

private:
    using stack_type = std::stack<Mode>;
    using adaptor_dict = std::unordered_map<Mode, std::unique_ptr<ModeAdapter>>;

    StringStream& _stream;
    stack_type _mode_stack;
    adaptor_dict _adaptor_dict;

    // [Scanner Mode Control]

    Mode current_mode();
    void enter_mode(Mode mode);
    void exit_current_mode();

    // [Adapter Control]

    void register_adapter(Mode mode, ModeAdapter* adapter);
    ModeAdapter& get_adapter();
};

class Scanner::ModeAdapter
{
public:
    ModeAdapter(Scanner& s) : _scanner(s), stream(_scanner._stream) {}
    virtual Token scan() = 0;

    static bool is_digit(char ch, bool include_zero = true);
    static bool is_whitespace(char ch);
    static bool is_delimiter(char ch);
    static bool is_symbol(char ch);
    static bool is_valid_letter(char ch);
    static bool is_subsequent_letter(char ch);

private:
    Scanner& _scanner;

protected:
    StringStream& stream;

    Mode current_mode();
    void enter_mode(Mode mode);
    void exit_current_mode();
};

class ScannerNormalMode : public Scanner::ModeAdapter
{
public:
    ScannerNormalMode(Scanner& s) : ModeAdapter(s) {}

    Token scan() override;

private:
    Token scan_whitespace();
    Token scan_symbol();
    Token scan_comment();
    Token scan_string();
    char escape_seq();
    Token scan_stringlike_identifier();
    Token scan_sign(std::string& content);
    Token scan_integer(std::string& content);
    Token scan_decimal(std::string& content);
    Token scan_id_valid_letter(std::string& content);
    Token scan_id_subsequent(std::string& content);
};

class ScannerTextMode : public Scanner::ModeAdapter
{
public:
    ScannerTextMode(Scanner& s) : ModeAdapter(s) {}

    Token scan() override;

private:
    Token scan_text_content(std::string& content);
    Token scan_emptyline(std::string& content);
};
} // namespace libdocscript

#endif