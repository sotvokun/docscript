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

    std::vector<SExpressionAST> parse(const std::string &str);

  private:
    CommentAST parse_comment();
    AtomAST parse_symbol();
    AtomAST parse_atom();
    TextStringAST parse_textstring();
    TextLineEndAST parse_textlineend();
    TextAST parse_text();
    ListAST parse_list();
    SExpressionListAST parse_sexprlist();
    SExpressionAST parse_sexpr(bool inner = false);

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