#ifndef LIBDOCSCRIPT_AST_LIST_H
#define LIBDOCSCRIPT_AST_LIST_H
#include "libdocscript/ast/node.h"
#include "libdocscript/misc.h"
#include <vector>

namespace docscript::libdocscript::ast {
class RoundList final : public Node {
  public:
    using value_type = std::vector<NodePointer>;

    RoundList(Position p) : Node(NodeType::RoundList, p) {}
    ~RoundList() override = default;

    value_type value;
};

class SquareList final : public Node {
  public:
    using value_type = std::vector<NodePointer>;

    SquareList(Position p) : Node(NodeType::SquareList, p) {}
    ~SquareList() override = default;

    value_type value;
};

} // namespace docscript::libdocscript::ast

#endif