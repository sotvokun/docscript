#ifndef LIBDOCSCRIPT_AST_EXPRESSION_H
#define LIBDOCSCRIPT_AST_EXPRESSION_H
#include "libdocscript/ast/ast.h"
#include "libdocscript/exception.h"

namespace libdocscript::ast {
class Expression final
{
public:
    Expression() : _ptr(nullptr) {}
    Expression(ASTNode* ptr);
    Expression(const ASTNode& node);
    Expression(const Expression&);
    Expression(Expression&&);
    ~Expression();

    Expression& operator=(const Expression&);
    Expression& operator=(Expression&&);

    ASTNode& operator*();
    const ASTNode& operator*() const;

    ASTNodeType type() const;

    template <typename T>
    inline T& cast()
    {
        throw InternalUnimplementException("Expression::cast<T>()");
    }

    template <typename T>
    inline const T& c_cast() const
    {
        throw InternalUnimplementException("Expression::c_cast<T>()");
    }

private:
    ASTNode* _ptr;
};

template <>
Atom& Expression::cast<Atom>();
template <>
List& Expression::cast<List>();

template <>
const Atom& Expression::c_cast<Atom>() const;
template <>
const List& Expression::c_cast<List>() const;

} // namespace libdocscript::ast

#endif