#ifndef LIBDOCSCRIPT_AST_QUOTE_H
#define LIBDOCSCRIPT_AST_QUOTE_H
#include "libdocscript/ast/node.h"
#include "libdocscript/misc.h"

namespace docscript::libdocscript::ast {
class Quote final : public Node {
  public:
    using value_type = NodePointer;

    Quote(value_type val, Position p) : Node(NodeType::Quote, p), value(val) {}
    ~Quote() override = default;

    value_type value;
};

class Quasiquote final : public Node {
  public:
    using value_type = NodePointer;

    Quasiquote(value_type val, Position p)
        : Node(NodeType::Quasiquote, p), value(val) {}
    ~Quasiquote() override = default;

    value_type value;
};

class Unquote final : public Node {
  public:
    using value_type = NodePointer;

    Unquote(value_type val, Position p)
        : Node(NodeType::Unquote, p), value(val) {}
    ~Unquote() override = default;

    value_type value;
};
} // namespace docscript::libdocscript::ast

#endif