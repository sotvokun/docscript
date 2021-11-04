#ifndef LIBDOCSCRIPT_PARSER_H
#define LIBDOCSCRIPT_PARSER_H
#include "libdocscript/ast.h"
#include "libdocscript/token.h"
#include "libdocscript/tokenstream.h"
#include <string>
#include <vector>

namespace docscript::libdocscript {
class ParserOption final {};

class Parser final {
  public:
    Parser() {}
    Parser(ParserOption option) : _option(option) {}

    std::vector<ast::SExpression> parse(const std::string &str);

  private:
    ast::Comment parse_comment();
    ast::Atom parse_atom();
    ast::TextString parse_textstring();
    ast::TextLineEnd parse_textlineend();
    ast::Text parse_text();
    ast::RoundList parse_roundlist();
    ast::SquareList parse_squarelist();
    ast::SExpression parse_sexpr(bool inner = false);

    ast::Quote parse_quote();
    ast::Quasiquote parse_quasiquote();
    ast::Unquote parse_unquote();

    Token get();
    Token current();
    void unget();
    void next();

    Token _last_token;
    bool _init_status = true;

    TokenStream _stream;
    ParserOption _option;
};
} // namespace libdocscript

#endif