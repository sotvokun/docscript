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
    using ptr_type = libdocscript::SExpressionAST::ptr_type;
    using sexprlist_type = libdocscript::SExpressionListAST::value_type;

    Interpreter(Environment &env) : _env(env) {}
    ~Interpreter() = default;

    const Value &last_result() const;

    std::optional<Value> eval(const libdocscript::SExpressionAST &expr,
                              bool allow_def = true);
    Value eval(const libdocscript::AtomAST &atom);
    Value eval(const libdocscript::TextAST &text);
    Value eval(const libdocscript::ListAST &list);
    std::optional<Value> eval(const libdocscript::SExpressionListAST &list,
                              bool allow_def);

    Value eval_procedure(const sexprlist_type &list);
    std::optional<Value> eval_specialform(const sexprlist_type &list,
                                          bool allow_def);

    void eval_definition(const sexprlist_type &list);
    Value eval_assignment(const sexprlist_type &list);
    Value eval_lambda(const sexprlist_type &list);
    Value eval_logical_and(const sexprlist_type &list);
    Value eval_logical_or(const sexprlist_type &list);
    Value eval_ifelse(const sexprlist_type &list);
    Value eval_for(const sexprlist_type &list);

  private:
    template <typename T> inline const T &cast(const ptr_type &ptr) {
        return dynamic_cast<T &>(*ptr);
    }

    bool is_keyword(const std::string &id) const;

    const std::vector<std::string> _keywords{"define", "set!", "lambda", "λ",
                                             "and",    "or",   "if",     "for"};
    Environment &_env;
};
} // namespace docscript::docsir

#endif