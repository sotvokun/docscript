#include "libdocscript/ast/ast.h"
#include "libdocscript/ast/expression.h"
#include <memory>

namespace libdocscript::ast {

// +----------------------+
//       Constructor
// +----------------------+

ASTNode::ASTNode(ASTNodeType t) : _type(t) {}

// +----------------------+
//       Copy Control
// +----------------------+

ASTNode::ASTNode(const ASTNode& node) : _type(node._type) {}

ASTNode::ASTNode(ASTNode&& node) : _type(node._type) {}

// +----------------------+
//     Public Functions
// +----------------------+

Expression ASTNode::ptr_clone() const
{
    return Expression(*this);
}

ASTNodeType ASTNode::type() const
{
    return _type;
}

// +----------------------+
//     Private Functions
// +----------------------+

ASTNode* ASTNode::rawptr_clone() const
{
    return new ASTNode(*this);
}

} // namespace libdocscript::ast