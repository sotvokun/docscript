#include "libdocscript/ast/node.h"
#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/comment.h"
#include "libdocscript/ast/list.h"
#include "libdocscript/ast/quote.h"
#include "libdocscript/ast/sexpr.h"
#include "libdocscript/ast/text.h"
#include "libdocscript/exception.h"

namespace docscript::libdocscript::ast {

// Constructors

NodePointer::NodePointer(Node *ptr) : _raw(ptr) {}

NodePointer::NodePointer(const NodePointer &val)
    : _raw(val._raw ? NodePointer::clone(val._raw) : nullptr) {}

NodePointer::NodePointer(NodePointer &&val)
    : _raw(val._raw ? val._raw : nullptr) {
    val._raw = nullptr;
}

// Destructor

NodePointer::~NodePointer() {
    if (_raw) {
        delete _raw;
    }
}

// Operators

NodePointer &NodePointer::operator=(const NodePointer &rhs) {
    if (_raw) {
        delete _raw;
    }
    if (rhs._raw) {
        _raw = NodePointer::clone(rhs._raw);
    }
    return *this;
}

NodePointer &NodePointer::operator=(NodePointer &&rhs) {
    if (_raw && rhs._raw && _raw != rhs._raw) {
        delete _raw;
        _raw = rhs._raw;
    }
    rhs._raw = nullptr;
    return *this;
}

NodePointer::operator bool() const { return _raw; }

// `clone` Static Function Definition

Node *NodePointer::clone(Node *ptr) {
    switch (ptr->type) {
    case NodeType::Comment:
        return new Comment(*dynamic_cast<Comment *>(ptr));

    case NodeType::Atom:
        return new Atom(*dynamic_cast<Atom *>(ptr));

    case NodeType::TextString:
        return new TextString(*dynamic_cast<TextString *>(ptr));

    case NodeType::TextLineEnd:
        return new TextLineEnd(*dynamic_cast<TextLineEnd *>(ptr));

    case NodeType::Text:
        return new Text(*dynamic_cast<Text *>(ptr));

    case NodeType::RoundList:
        return new RoundList(*dynamic_cast<RoundList *>(ptr));

    case NodeType::SquareList:
        return new SquareList(*dynamic_cast<SquareList *>(ptr));

    case NodeType::Quote:
        return new Quote(*dynamic_cast<Quote *>(ptr));

    case NodeType::Quasiquote:
        return new Quasiquote(*dynamic_cast<Quasiquote *>(ptr));
        
    case NodeType::Unquote:
        return new Unquote(*dynamic_cast<Unquote *>(ptr));

    case NodeType::SExpression:
        return new SExpression(*dynamic_cast<SExpression *>(ptr));

    default:
        // TODO: THROW InternalError Bad Status
        throw ParserInternalException("", ptr->position);
    }
}

// `cast' Function Template Specializations

template <> const Comment &NodePointer::cast() const {
    return *dynamic_cast<Comment *>(_raw);
}

template <> const Atom &NodePointer::cast() const {
    return *dynamic_cast<Atom *>(_raw);
}

template <> const TextString &NodePointer::cast() const {
    return *dynamic_cast<TextString *>(_raw);
}

template <> const TextLineEnd &NodePointer::cast() const {
    return *dynamic_cast<TextLineEnd *>(_raw);
}

template <> const Text &NodePointer::cast() const {
    return *dynamic_cast<Text *>(_raw);
}

template <> const RoundList &NodePointer::cast() const {
    return *dynamic_cast<RoundList *>(_raw);
}

template <> const SquareList &NodePointer::cast() const {
    return *dynamic_cast<SquareList *>(_raw);
}

template <> const Quote &NodePointer::cast() const {
    return *dynamic_cast<Quote *>(_raw);
}

template <> const Quasiquote &NodePointer::cast() const {
    return *dynamic_cast<Quasiquote *>(_raw);
}

template <> const Unquote &NodePointer::cast() const {
    return *dynamic_cast<Unquote *>(_raw);
}

template <> const SExpression &NodePointer::cast() const {
    return *dynamic_cast<SExpression *>(_raw);
}

} // namespace docscript::libdocscript::ast