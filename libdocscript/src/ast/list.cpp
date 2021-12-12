#include "libdocscript/ast/atom.h"
#include "libdocscript/ast/expression.h"
#include "libdocscript/ast/list.h"
#include <utility>
#include <vector>


namespace libdocscript::ast {

// +----------------------+
//       Constructor
// +----------------------+

List::List() : ASTNode(ASTNodeType::List) {}

// +----------------------+
//       Copy Control
// +----------------------+

List::List(const List& list)
    : ASTNode(ASTNodeType::List), is_textlist(list.is_textlist)
{
    for (auto p : list.craw()) {
        _value.push_back(p);
    }
}

List::List(List&& list)
    : ASTNode(ASTNodeType::List), is_textlist(list.is_textlist)
{
    for (auto p : list.craw()) {
        _value.push_back(std::move(p));
    }
}

// +----------------------+
//        Desturctor
// +----------------------+

// +----------------------+
//     Public Functions
// +----------------------+

List::raw_type& List::raw()
{
    return _value;
}

const List::raw_type& List::craw() const
{
    return _value;
}

void List::push_back(Atom atom)
{
    _value.push_back(atom.rawptr_clone());
}

void List::push_back(Expression expr)
{
    _value.push_back(expr);
}

Expression List::ptr_clone() const
{
    return Expression(*this);
}

List::raw_type::size_type List::size() const
{
    return _value.size();
}

bool List::is_empty() const
{
    return _value.empty();
}

List::raw_type::iterator List::begin()
{
    return _value.begin();
}

List::raw_type::iterator List::end()
{
    return _value.end();
}

List::raw_type::const_iterator List::cbegin() const
{
    return _value.cbegin();
}

List::raw_type::const_iterator List::cend() const
{
    return _value.cend();
}

// +----------------------+
//     Private Functions
// +----------------------+

ASTNode* List::rawptr_clone() const
{
    return new List(*this);
}

} // namespace libdocscript::ast