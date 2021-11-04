#ifndef LIBDOCSCRIPT_AST_COMMENT_H
#define LIBDOCSCRIPT_AST_COMMENT_H
#include "libdocscript/ast/node.h"
#include "libdocscript/misc.h"
#include <string>

namespace docscript::libdocscript::ast {
class Comment final : public Node {
  public:
    using value_type = std::string;

    Comment(const value_type &s, Position p)
        : Node(NodeType::Comment, p), value(s) {}
    ~Comment() override = default;

    value_type value;
};
} // namespace docscript::libdocscript::ast

#endif