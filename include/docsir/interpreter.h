#ifndef DOCSIR_INTERPRETER_H
#define DOCSIR_INTERPRETER_H
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/environment.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include "libdocscript/ast.h"
#include <optional>
#include <string>
#include <vector>

namespace docscript::docsir {
class Interpreter final {
  public:
    using list_type = libdocscript::ast::SquareList::value_type;

    Interpreter(Environment &env) : _env(env) {}
    ~Interpreter() = default;

    const Value &last_result() const;

    std::optional<Value> eval(const libdocscript::ast::SExpression &expr,
                              bool allow_def = true);
    Value eval(const libdocscript::ast::Atom &atom);
    Value eval(const libdocscript::ast::Text &text);
    Value eval(const libdocscript::ast::RoundList &list);
    std::optional<Value> eval(const libdocscript::ast::SquareList &list,
                              bool allow_def);
    Value eval(const libdocscript::ast::Quote &quote);
    Value eval(const libdocscript::ast::Quasiquote &quasiquote);
    std::optional<Value> eval(const libdocscript::ast::Unquote &unquote);

    Value eval_procedure(const list_type &list);
    std::optional<Value> eval_specialform(const list_type &list,
                                          bool allow_def);

    void eval_definition(const list_type &list);
    Value eval_assignment(const list_type &list);
    Value eval_lambda(const list_type &list);
    Value eval_logical_and(const list_type &list);
    Value eval_logical_or(const list_type &list);
    Value eval_ifelse(const list_type &list);
    Value eval_for(const list_type &list);

    // Added at 0.2.0
    Value eval_quote(const list_type &list);
    Value eval_quasiquote(const list_type &list);
    Value eval_unquote(const list_type &list);

    Value quote_value(const libdocscript::ast::SExpression &sexpr,
                      bool quasiquote);

  private:
    bool is_keyword(const std::string &id) const;

    const std::vector<std::string> _keywords{
        "define", "set!", "lambda", "λ",          "and",    "or",
        "if",     "for",  "quote",  "quasiquote", "unquote"};
    Environment &_env;
};
} // namespace docscript::docsir

#endif