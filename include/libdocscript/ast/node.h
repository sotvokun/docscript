#ifndef LIBDOCSCRIPT_AST_NODE_H
#define LIBDOCSCRIPT_AST_NODE_H
#include "libdocscript/exception.h"
#include "libdocscript/misc.h"

namespace docscript::libdocscript::ast {
enum class NodeType {
    Comment,
    Atom,
    TextString,
    TextLineEnd,
    Text,
    RoundList,
    SquareList,
    Quote,
    Quasiquote,
    Unquote,
    SExpression,
};

class Comment;
class Atom;
class TextString;
class TextLineEnd;
class Text;
class RoundList;
class SquareList;
class Quote;
class Quasiquote;
class Unquote;
class SExpression;

class Node {
  public:
    Node(NodeType t, Position p) : type(t), position(p) {}
    virtual ~Node() = default;

    NodeType type;
    Position position;
};

class NodePointer final {
  public:
    NodePointer(Node *ptr);
    NodePointer(const NodePointer &);
    NodePointer(NodePointer &&);

    NodePointer &operator=(const NodePointer &);
    NodePointer &operator=(NodePointer &&);

    operator bool() const;

    ~NodePointer();

    inline static Node *clone(Node *ptr);

    template <typename T> inline const T &cast() const {
        throw UnimplementException();
    }
    inline NodeType type() const { return _raw->type; }

  private:
    Node *_raw = nullptr;
};

template <> const Comment &NodePointer::cast() const;
template <> const Atom &NodePointer::cast() const;
template <> const TextString &NodePointer::cast() const;
template <> const TextLineEnd &NodePointer::cast() const;
template <> const Text &NodePointer::cast() const;
template <> const RoundList &NodePointer::cast() const;
template <> const SquareList &NodePointer::cast() const;
template <> const Quote &NodePointer::cast() const;
template <> const Quasiquote &NodePointer::cast() const;
template <> const Unquote &NodePointer::cast() const;
template <> const SExpression &NodePointer::cast() const;
} // namespace docscript::libdocscript::ast

#endif