#ifndef LIBDOCSCRIPT_AST_AST_H
#define LIBDOCSCRIPT_AST_AST_H
#include "libdocscript/exception.h"

namespace libdocscript::ast {

class Atom;
class List;
class Expression;

enum class ASTNodeType
{
    Atom,
    List
};

class ASTNode
{
    friend class Atom;
    friend class List;
    friend class Expression;

  public:
    ASTNode(ASTNodeType t);
    ASTNode(const ASTNode&);
    ASTNode(ASTNode&&);
    virtual ~ASTNode() = default;

    ASTNode& operator=(const ASTNode&) = delete;
    ASTNode& operator=(ASTNode&&) = delete;

    virtual Expression ptr_clone() const;

    ASTNodeType type() const;

  private:
    ASTNodeType _type;

    virtual ASTNode* rawptr_clone() const;
};
} // namespace libdocscript::ast

#endif