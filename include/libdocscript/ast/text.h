#ifndef LIBDOCSCRIPT_AST_TEXT_H
#define LIBDOCSCRIPT_AST_TEXT_H
#include "libdocscript/ast/node.h"
#include "libdocscript/misc.h"
#include <string>
#include <vector>
#include <memory>

namespace docscript::libdocscript::ast {
class TextString final : public Node {
  public:
    using value_type = std::string;

    TextString(const value_type &s, Position p)
        : Node(NodeType::TextString, p), value(s) {}
    ~TextString() override = default;

    value_type value;
};

class TextLineEnd final : public Node {
  public:
    TextLineEnd(Position p) : Node(NodeType::TextLineEnd, p) {}
    ~TextLineEnd() override = default;
};

class Text final : public Node {
  public:
    using value_type = std::vector<NodePointer>;

    Text(Position p) : Node(NodeType::Text, p) {}
    ~Text() override = default;

    value_type value;
};
}

#endif