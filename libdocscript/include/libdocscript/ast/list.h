#ifndef LIBDOCSCRIPT_AST_LIST_H
#define LIBDOCSCRIPT_AST_LIST_H
#include "libdocscript/ast/ast.h"
#include "libdocscript/utility/position.h"
#include "libdocscript/token.h"
#include <vector>

namespace libdocscript::ast {
class List : public ASTNode
{
  public:
    using raw_type = std::vector<Expression>;

    bool is_textlist = false;

    List();
    List(const List&);
    List(List&&);
    ~List() override = default;

    raw_type& raw();
    const raw_type& craw() const;

    void push_back(Atom atom);
    void push_back(Expression node);

    raw_type::size_type size() const;
    bool is_empty() const;

    raw_type::iterator begin();
    raw_type::iterator end();
    raw_type::const_iterator cbegin() const;
    raw_type::const_iterator cend() const;

    Expression ptr_clone() const override;

  private:
    raw_type _value;

    ASTNode* rawptr_clone() const override;
};
}

#endif