#ifndef LIBDOCSCRIPT_AST_SEXPR_H
#define LIBDOCSCRIPT_AST_SEXPR_H
#include "libdocscript/ast/node.h"
#include "libdocscript/misc.h"

namespace docscript::libdocscript::ast {
enum class SExpressionType {
    Comment,
    Atom,
    Text,
    RoundList,
    SquareList,
    Quote,
    Quasiquote,
    Unquote
};

class SExpression final : public Node {
  public:
    using value_type = NodePointer;

    SExpression(const Comment &);
    SExpression(const Atom &);
    SExpression(const Text &);
    SExpression(const RoundList &);
    SExpression(const SquareList &);
    SExpression(const Quote &);
    SExpression(const Quasiquote &);
    SExpression(const Unquote &);

    ~SExpression() override = default;

    value_type value;
    SExpressionType type_sexpr;
};
}

#endif