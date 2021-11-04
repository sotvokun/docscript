#include "libdocscript/ast/sexpr.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/comment.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/ast/node.h"
#include "libdocscript/ast/quote.h"
#include "libdocscript/ast/text.h"
#include <memory>

namespace docscript::libdocscript::ast {

// Constructor

SExpression::SExpression(const Comment &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Comment), value(new Comment(val)) {}

SExpression::SExpression(const Atom &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Atom), value(new Atom(val)) {}

SExpression::SExpression(const Text &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Text), value(new Text(val)) {}

SExpression::SExpression(const RoundList &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::RoundList), value(new RoundList(val)) {}

SExpression::SExpression(const SquareList &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::SquareList), value(new SquareList(val)) {}

SExpression::SExpression(const Quote &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Quote), value(new Quote(val)) {}

SExpression::SExpression(const Quasiquote &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Quasiquote), value(new Quasiquote(val)) {}

SExpression::SExpression(const Unquote &val)
    : Node(NodeType::SExpression, val.position),
      type_sexpr(SExpressionType::Unquote), value(new Unquote(val)) {}

} // namespace docscript::libdocscript::ast