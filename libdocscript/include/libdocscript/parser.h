#ifndef LIBDOCSCRIPT_PARSER_H
#define LIBDOCSCRIPT_PARSER_H
#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/scanner.h"
#include "libdocscript/utility/position.h"
#include <vector>

namespace libdocscript {
class Parser final
{
  public:
    using expression_list = std::vector<ast::Expression>;

    static expression_list parse(StringStream &stream, runtime::Environment &env);

  private:
    Scanner& _scanner;
    runtime::Environment& _env;
    static ast::AtomHelper _atom_helper;

    Parser(Scanner &scanner, runtime::Environment &env);

    ast::Expression parse_expression(const Token& token);
    ast::Atom parse_atom(const Token& token);
    ast::Atom parse_keyword(const Token& token);
    ast::List parse_quote_expand(const Token& token);
    ast::List parse_list(const Token& token);
    ast::Expression parse_text_element(const Token& token);
};
}

#endif